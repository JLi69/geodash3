#include "SoundBuffer.h"
#include <sndfile.hh>
#include <iostream>
#include <AL/alext.h>
#include <sstream>
#include <cstdlib>

Geodash3::SoundBuffer::SoundBuffer()
{
	//Clear the sound buffer vector	
	m_sounds.clear();
}

Geodash3::SoundBuffer::~SoundBuffer()
{
	//Delete all the buffers	
	alDeleteBuffers(m_sounds.size(), m_sounds.data());	
	//Clear the sound buffer vector	
	m_sounds.clear();
}

Geodash3::SoundBuffer* Geodash3::SoundBuffer::get()
{
	static SoundBuffer* sndbuffer = new SoundBuffer;
	return sndbuffer;
}

ALuint Geodash3::SoundBuffer::AddSoundEffect(const char *filename)
{
	ALenum err, //Store any error that happens
		   format; //Store the format of the sound file
	ALuint buffer; //buffer ID
	
	SNDFILE* soundFile = nullptr;
	SF_INFO sfinfo;
	short* membuf = nullptr;

	//Store the number of frames
	sf_count_t frameCount;
	ALsizei byteCount;

	//Open sound file
	soundFile = sf_open(filename, SFM_READ, &sfinfo);	
#ifndef WINDOWS	
	//Failed to open sound file, attempt to search in .geodash3 in the home directory	
	if(!soundFile)
	{
		std::stringstream newPath;
		//get user's home directory	
		const char* home = getenv("HOME");	
		newPath << home << "/.geodash3/" << filename; 
		soundFile = sf_open(newPath.str().c_str(), SFM_READ, &sfinfo);	
	}
	//Failed to open sound file again, attempt to search in /usr/share/games
	if(!soundFile)
	{	
		std::stringstream newPath;	
		newPath << "/usr/share/games/geodash3/" << filename; 
		soundFile = sf_open(newPath.str().c_str(), SFM_READ, &sfinfo);
	}
#endif
	//Failed to open sound file	
	if(!soundFile)
	{
		std::cout << "Could not open audio file: " << filename << '\n';
		return 0;
	}
	//Invalid frame count
	if(sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT32_MAX / sizeof(short) / sfinfo.channels))
	{
		std::cout << "Bad sample count in file: " << filename << " (" << sfinfo.frames << ")\n";
		sf_close(soundFile);	
		return 0;	
	}

	//Get format of file
	format = AL_NONE;	
	if(sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if(sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	
	//Unsupported format	
	if(!format)
	{
		std::cout << "Unsupported channel count: " << sfinfo.channels << '\n';
		sf_close(soundFile);
		return 0;
	}

	//Allocate the memory to store the sound data
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
	//Calculate the number of frames	
	frameCount = sf_readf_short(soundFile, membuf, sfinfo.frames);
	//Invalid frame count
	if(frameCount < 1)
	{
		free(membuf);
		sf_close(soundFile);	
		std::cout << "Failed to read samples in " << filename << '\n';
		return 0;	
	}
	byteCount = (ALsizei)(frameCount * sfinfo.channels) * (ALsizei)sizeof(short);

	//Send the buffer to the audio device
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, byteCount, sfinfo.samplerate);

	//Deallocate the memory buffer and close the file
	free(membuf);
	sf_close(soundFile);

	//Check if an error occured
	err = alGetError();
	if(err != AL_NO_ERROR)
	{
		std::cout << "OpenAL Error: " << alGetString(err) << '\n';
		if(buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;	
	}

	m_sounds.push_back(buffer); //Add buffer to list
	return buffer;
}

bool Geodash3::SoundBuffer::RemoveSoundEffect(const ALuint &buffer)
{	
	for(auto it = m_sounds.begin(); it != m_sounds.end(); it++)	
		//Delete the sound buffer if it is matches the buffer to delete	
		if(*it == buffer)
		{
			alDeleteBuffers(1, &*it);
			it = m_sounds.erase(it);
			return true;
		}
	return false;
}

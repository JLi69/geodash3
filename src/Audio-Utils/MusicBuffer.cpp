#include "MusicBuffer.h"
#include <malloc.h>
#include <iostream>

void Geodash3::MusicBuffer::Play()
{
	//Clear errors
	alGetError();

	alSourceRewind(m_source);
	alSourcei(m_source, AL_BUFFER, 0);

	ALsizei i;
	for(i = 0; i < NUM_BUFFERS; i++)
	{
		sf_count_t slen = sf_readf_short(m_soundFile, m_membuf, BUFFER_SAMPLES);
		if(slen < 1) break;
		slen *= m_sfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(m_buffers[i], m_format, m_membuf, (ALsizei)slen, m_sfinfo.samplerate);
	}

	//Check for errors
	if(alGetError() != AL_NO_ERROR)
	{
		std::cout << "Error buffering for playback\n";
		return;
	}

	alSourceQueueBuffers(m_source, i, m_buffers);
	alSourcePlay(m_source);
	if(alGetError() != AL_NO_ERROR)
	{
		std::cout << "Error stating playback\n";
		return;
	}
}

void Geodash3::MusicBuffer::UpdateBufferStream()
{
	ALint processed, state;	

	//Clear any errors
	alGetError();

	alGetSourcei(m_source, AL_SOURCE_STATE, &state);
	alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);
	
	m_playing = state == AL_PLAYING;	

	//An error occured	
	if(alGetError() != AL_NO_ERROR)
	{
		std::cout << "Error checking music source state\n";
		return;
	}	

	//Unqueue and handle each processed buffer
	while(processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;
		
		alSourceUnqueueBuffers(m_source, 1, &bufid);
		processed--;

		slen = sf_readf_short(m_soundFile, m_membuf, BUFFER_SAMPLES);
		if(slen > 0)
		{
			slen *= m_sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, m_format, m_membuf, (ALsizei)slen, m_sfinfo.samplerate);
			alSourceQueueBuffers(m_source, 1, &bufid);
		}
	
		if(alGetError() != AL_NO_ERROR)
		{
			std::cout << "Error buffering music data\n";	
			return;	
		}
	}

	if(state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;
		alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);
		if(queued == 0)
			return;
		alSourcePlay(m_source);
		if(alGetError() != AL_NO_ERROR)
		{
			std::cout << "error restarting music playback\n";	
			return;	
		}
	}
}

ALint Geodash3::MusicBuffer::getSource()
{
	return m_source;	
}

Geodash3::MusicBuffer::MusicBuffer(const char *filename)
{
	alGenSources(1, &m_source);
	alGenBuffers(NUM_BUFFERS, m_buffers);

	size_t frameSize;

	m_soundFile = sf_open(filename, SFM_READ, &m_sfinfo);
	if(!m_soundFile)	
	{	
		std::cout << "Failed to open file: " << filename << '\n';	
		throw("Could not open file, path not found");		
	}
		
	//Get format
	m_format = AL_NONE;
	if(m_sfinfo.channels == 1)
		m_format = AL_FORMAT_MONO16;
	else if(m_sfinfo.channels == 2)
		m_format = AL_FORMAT_STEREO16;

	if(!m_format)
	{
		sf_close(m_soundFile);
		m_soundFile = nullptr;
		std::cout << "Unsupported sound format\n";
		return;
	}
	
	frameSize = ((size_t)BUFFER_SAMPLES * (size_t)m_sfinfo.channels) * sizeof(short);
	m_membuf = static_cast<short*>(malloc(frameSize));	
}

Geodash3::MusicBuffer::~MusicBuffer()
{
	alDeleteSources(1, &m_source);	

	//Close the sound file
	if(m_soundFile)
		sf_close(m_soundFile);
	m_soundFile = nullptr;

	free(m_membuf);
	alDeleteBuffers(NUM_BUFFERS, m_buffers);
}

bool Geodash3::MusicBuffer::isPlaying()
{
	return m_playing;
}

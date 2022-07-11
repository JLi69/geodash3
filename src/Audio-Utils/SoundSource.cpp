#include "SoundSource.h"
#include <iostream>

Geodash3::SoundSource::SoundSource()
{
	alGenSources(1, &m_source);	
	alSourcef(m_source, AL_PITCH, 1.0f);
	alSourcef(m_source, AL_GAIN, 1.0f);
	alSource3f(m_source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSource3f(m_source, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSourcei(m_source, AL_LOOPING, false);
	alSourcei(m_source, AL_BUFFER, m_buffer);
}

Geodash3::SoundSource::~SoundSource()
{
	alDeleteSources(1, &m_source);
}

void Geodash3::SoundSource::Play(const ALuint soundToPlay)
{
	ALint state;
	alGetSourcei(m_source, AL_SOURCE_STATE, &state);

	if(state == AL_PLAYING)
		m_playing = true;
	else if(state != AL_PLAYING)
		m_playing = false;

	if(!m_playing)
	{
		if(soundToPlay != m_buffer)
		{
			m_buffer = soundToPlay;
			alSourcei(m_source, AL_BUFFER, (ALint)m_buffer);
		}	
	
		alSourcePlay(m_source);
	}
}

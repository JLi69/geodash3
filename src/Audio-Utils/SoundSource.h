#define AL_LIBTYPE_STATIC
#include <AL/al.h>

namespace Geodash3
{
	class SoundSource
	{
		ALuint m_source,
			   m_buffer;
		bool m_playing = false;		
	public:
		//Constructor
		SoundSource();	
		//Destructor
		~SoundSource();	
	
		//Play the sound
		void Play(const ALuint soundToPlay);
	};
}

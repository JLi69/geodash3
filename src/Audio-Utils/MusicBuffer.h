//#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <sndfile.h>

namespace Geodash3
{
	class MusicBuffer
	{
		ALuint m_source;
		static const int BUFFER_SAMPLES = 8192,
						 NUM_BUFFERS = 4;
		ALuint m_buffers[NUM_BUFFERS];
		SNDFILE* m_soundFile;
		SF_INFO m_sfinfo;
		short* m_membuf;
		ALenum m_format;
	
		bool m_playing = false;
	public:
		void Play();
	
		bool isPlaying();

		//To save memory, only keep a small part of the sound file in memory
		//and play that part and once that part is done, read more of the sound file
		void UpdateBufferStream();

		ALint getSource();
			
		//Constructor - pass in name of music file
		MusicBuffer(const char *filename);
		//Destructor	
		~MusicBuffer();
	};
}

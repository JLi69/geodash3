#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <vector>

namespace Geodash3
{
	class SoundBuffer
	{
		SoundBuffer(); //Constructor
		~SoundBuffer(); //Destructor
		
		std::vector<ALuint> m_sounds;
	public:
		static SoundBuffer* get();

		//Adds a sound to the sound buffer vector
		//and returns the ID of the buffer
		ALuint AddSoundEffect(const char *filename); 
		//Removes a sound from the buffer based on its ID
		//returns true if the ID is found, false if the ID
		//is not found
		bool RemoveSoundEffect(const ALuint &buffer);	
	};
}

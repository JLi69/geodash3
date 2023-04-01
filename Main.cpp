#include "src/Engine.h"
#include "src/Audio-Utils/SoundDevice.h"

int main()
{
	Geodash3::SoundDev* dev = Geodash3::SoundDev::get();
	Geodash3::Engine gameEngine = Geodash3::Engine();	
	gameEngine.Run();
}

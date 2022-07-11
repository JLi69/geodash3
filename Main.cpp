#include "src/Engine.h"
#include "src/Audio-Utils/SoundDevice.h"

int main()
{	
	Geodash3::ToggleSoundDevice(Geodash3::OPEN);	
	Geodash3::Engine gameEngine = Geodash3::Engine();
	gameEngine.Run();
	Geodash3::ToggleSoundDevice(Geodash3::CLOSE);
}

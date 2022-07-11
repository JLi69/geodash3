namespace Geodash3
{
	enum SoundDevMode
	{
		OPEN = 0,
		CLOSE = 1
	};

	//This function will connect to the sound device on the machine
	//and will also disconnect from the sound device on the machine
	void ToggleSoundDevice(SoundDevMode mode);
}

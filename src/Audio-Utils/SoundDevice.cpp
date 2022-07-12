#include "SoundDevice.h"
//#define AL_LIBTYPE_STATIC
#include <AL/alc.h>
//#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <iostream>

void Geodash3::ToggleSoundDevice(SoundDevMode mode)
{
	static ALCdevice* dev = nullptr;
	static ALCcontext* context = nullptr;	

	if(mode == OPEN)
	{
		dev = alcOpenDevice(nullptr);
		//Failed to open sound device	
		if(!dev)
		{
			std::cout << "Failed to open sound device!\n";
			return;	
		}
		context = alcCreateContext(dev, nullptr);	
		//Context failed to open	
		if(!context)
		{
			std::cout << "Failed to set sound context!\n";				
			return;	
		}
		//Make context current
		if(!alcMakeContextCurrent(context))
		{
			std::cout << "Failed to make context current!\n";
			return;
		}

		//Output name of device for debug purposes
		const ALCchar* name = nullptr;
		if(alcIsExtensionPresent(dev, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(dev, ALC_ALL_DEVICES_SPECIFIER);
		if(!name || alcGetError(dev) != AL_NO_ERROR)
			name = alcGetString(dev, ALC_DEVICE_SPECIFIER);
		std::cout << "Opened: " << name << '\n';	
	}	
	else if(mode == CLOSE)
	{		
		//Set the context to be nullptr	
		if(!alcMakeContextCurrent(nullptr))
		{
			std::cout << "Failed to set context to nullptr\n";
			return;
		}
		//Deallocate the context	
		alcDestroyContext(context);
		if(context)
		{
			std::cout << "Failed to unset context\n";
			return;
		}
		//Close the device	
		if(!alcCloseDevice(dev))
		{
			std::cout << "Failed to close device\n";	
			return;
		}
		std::cout << "Successfully closed sound device\n";	
	}
}

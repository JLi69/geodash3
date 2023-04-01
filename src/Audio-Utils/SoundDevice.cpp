#include "SoundDevice.h"
#include <AL/alc.h>
#include <AL/al.h>
#include <iostream>

Geodash3::SoundDev* Geodash3::SoundDev::get()
{
	static SoundDev* dev = new SoundDev;
	return dev;
}

Geodash3::SoundDev::SoundDev()
{
	mDevice = alcOpenDevice(nullptr);
	//Failed to open sound device	
	if(!mDevice)
	{
		std::cout << "Failed to open sound device!\n";
		return;	
	}
	mContext = alcCreateContext(mDevice, nullptr);	
	//Context failed to open	
	if(!mContext)
	{
		std::cout << "Failed to set sound context!\n";				
		return;	
	}
	//Make context current
	if(!alcMakeContextCurrent(mContext))
	{
		std::cout << "Failed to make context current!\n";
		return;
	}

	//Output name of device for debug purposes
	const ALCchar* name = nullptr;
	if(alcIsExtensionPresent(mDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(mDevice, ALC_ALL_DEVICES_SPECIFIER);
	if(!name || alcGetError(mDevice) != AL_NO_ERROR)
		name = alcGetString(mDevice, ALC_DEVICE_SPECIFIER);
	std::cout << "Opened: " << name << '\n';
}

Geodash3::SoundDev::~SoundDev()
{
	//Set the context to be nullptr	
	if(!alcMakeContextCurrent(nullptr))
	{
		std::cout << "Failed to set context to nullptr\n";
		return;
	}
	//Deallocate the context	
	alcDestroyContext(mContext);
	if(mContext)
	{
		std::cout << "Failed to unset context\n";
		return;
	}
	//Close the device	
	if(!alcCloseDevice(mDevice))
	{
		std::cout << "Failed to close device\n";	
		return;
	}
	std::cout << "Successfully closed sound device\n";
}

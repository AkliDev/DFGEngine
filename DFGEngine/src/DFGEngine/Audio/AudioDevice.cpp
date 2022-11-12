#include "dfgpch.h"
#include "AudioDevice.h"

#include <AL\al.h>
#include <AL\alc.h>
//#include <stdio.h>
//#include <vector>

namespace DFGEngine
{
	AudioDevice::AudioDevice()
	{
		m_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
		DFG_ASSERT(m_ALCDevice, "failed to get sound device");
		
		m_ALCContext = alcCreateContext(m_ALCDevice, nullptr);  // create context
		DFG_ASSERT(m_ALCDevice, "Failed to set sound context");
		
		bool result = alcMakeContextCurrent(m_ALCContext); // make context current
		DFG_ASSERT(result, "failed to make context current");
		
		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(m_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		{
			name = alcGetString(m_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		}
		if (!name || alcGetError(m_ALCDevice) != AL_NO_ERROR)
		{
			name = alcGetString(m_ALCDevice, ALC_DEVICE_SPECIFIER);
		}
		DFG_CORE_INFO("Opended Audio Device: {0}", name);
	}

	AudioDevice::~AudioDevice()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_ALCContext);
		alcCloseDevice(m_ALCDevice);
	}
}
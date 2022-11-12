#include "dfgpch.h"
#include "SoundEngine.h"

namespace DFGEngine
{
	AudioDevice* SoundEngine::s_SoundDevice = nullptr;
	std::vector<AudioSource*> SoundEngine::s_SFXSources;
	AudioSource* SoundEngine::s_MusicSource = nullptr;
	AudioLibrary* SoundEngine::s_AudioLibrary = nullptr;


	void SoundEngine::Init()
	{
		s_SoundDevice = new AudioDevice();

		for (uint32_t i = 0; i < SFX_SOURCE_COUNT; i++)
		{
			s_SFXSources.push_back(new AudioSource());
		}
		
		s_MusicSource = new AudioSource();
		s_AudioLibrary = new AudioLibrary();

		s_MusicSource->SetGain(0.2f);
	}

	void SoundEngine::Shutdown()
	{
		delete s_AudioLibrary;

		for (AudioSource* source : s_SFXSources) { delete source; }
		s_SFXSources.clear();

		delete s_MusicSource;
		delete s_SoundDevice;
	}

	void SoundEngine::LoadSFX(const std::string& name, const std::string& filePath)
	{
		s_AudioLibrary->LoadSFX(name, filePath);
	}

	void SoundEngine::LoadMusic(const std::string& name, const std::string& filePath)
	{
		s_AudioLibrary->LoadMusic(name, filePath);
	}

	void SoundEngine::PlaySFX(const std::string& name)
	{
		for (AudioSource* source : s_SFXSources)
		{
			if (source->IsPlaying() == false)
			{
				source->PlaySFX(s_AudioLibrary->GetSFX(name)->GetAudioID());
				return;
			}
		}

		DFG_CORE_ERROR("SoundEngine: There are no AudioSources available for playback");
		//We could double the amount of sound sources here. 
	}

	void SoundEngine::PlayMusic(const std::string& name)
	{
		s_MusicSource->PlayMusic(s_AudioLibrary->GetMusic(name));
	}

	void SoundEngine::Update()
	{
		s_MusicSource->UpdateStream();
	}
}
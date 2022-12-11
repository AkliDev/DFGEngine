#include "AudioDevice.h"
#include "AudioBuffer.h"
#include "AudioSource.h"
				
#include <vector>

namespace DFGEngine
{
	class SoundEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadSFX(const std::string& name, const std::string& filePath);
		static void LoadMusic(const std::string& name, const std::string& filePath);
		static void PlaySFX(const std::string& name);
		static void PlayMusic(const std::string& name);

		static void Update();

	private:
		const static uint32_t SFX_SOURCE_COUNT = 20;
		static AudioDevice* s_SoundDevice;
		static AudioSource* s_MusicSource;
		static std::vector<AudioSource*> s_SFXSources;
		static AudioLibrary* s_AudioLibrary;
	};
}
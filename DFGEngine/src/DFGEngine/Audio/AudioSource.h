// TODO: REMOVE!
typedef unsigned int ALuint;

namespace DFGEngine
{
	class AudioStream;
	class AudioSource
	{
	public:
		AudioSource();
		~AudioSource();

		void PlaySFX(const ALuint& buffer_to_play);
		void PlayMusic(const Ref<AudioStream>& audioStream);
		void UpdateStream();

		bool IsPlaying();

		void SetGain(const float& val);
	private:
		void LoopStream();

	private:
		ALuint m_AudioID;
		ALuint m_Buffer = 0;
		Ref<AudioStream> m_AudioStream;
		bool m_Loop = true;
	};
}
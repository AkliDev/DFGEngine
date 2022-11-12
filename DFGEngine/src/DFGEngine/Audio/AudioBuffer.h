#include <vector>
// TODO: REMOVE!
typedef unsigned int ALuint;
typedef int ALenum;

typedef	struct sf_private_tag SNDFILE;
struct SF_INFO;

namespace DFGEngine
{
	class SFXBuffer
	{
	public:
		SFXBuffer(const std::string& path);
		~SFXBuffer();

		ALuint Load(const char* filename);

		ALuint GetAudioID() const{ return m_AudioID; }

	private:
		ALuint m_AudioID;
	};

	class AudioStream
	{
	public:
		AudioStream(const std::string& path);
		~AudioStream();
	public:
		void Load(const char* filename);

		uint32_t GetSamples() const { return BUFFER_SAMPLES; }
		uint32_t GetAmountOfBuffers() const { return NUM_BUFFERS; }
		const ALuint* GetAudioIDs() const { return m_AudioIDs;}
		SNDFILE* GetSNDFILE() const { return m_SndFile; }
		const SF_INFO* GetSFinfo() const { return m_SFinfo; }
		short* GetMembuf() { return m_Membuf; }
		ALenum GetFormat() const { return m_Format; }
	
	private:
		static const uint32_t BUFFER_SAMPLES = 8192;
		static const uint32_t NUM_BUFFERS = 4;
		ALuint m_AudioIDs[NUM_BUFFERS];
		SNDFILE* m_SndFile = nullptr;
		SF_INFO* m_SFinfo = nullptr;
		short* m_Membuf = nullptr;
		ALenum m_Format;
	};

	class AudioLibrary
	{
	public:
		~AudioLibrary();
		Ref<SFXBuffer> LoadSFX(const std::string& name, const std::string& filePath);
		Ref<AudioStream> LoadMusic(const std::string& name, const std::string& filePath);
		
		Ref<SFXBuffer> GetSFX(const std::string& name);
		Ref<AudioStream> GetMusic(const std::string& name);
		
		bool SFXExists(const std::string& name) const;
		bool MusicExists(const std::string& name) const;
		
	private:
		void AddSFX(const std::string& name, const Ref<SFXBuffer>& audioBuffer);
		void AddMusic(const std::string& name, const Ref<AudioStream>& audioBuffer);

	private:
		std::unordered_map<std::string, Ref<SFXBuffer>> m_SFXBuffers;
		std::unordered_map<std::string, Ref<AudioStream>> m_MusicBuffers;
	};
}
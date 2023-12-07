#include "dfgpch.h"
#include "AudioBuffer.h"

#include <AL/al.h>
#include <AL/alext.h>
#include <sndfile.h>
#include <inttypes.h>

namespace DFGEngine
{
	SFXBuffer::SFXBuffer(const std::string& path)
	{
		Load(path.c_str());
	}

	SFXBuffer::~SFXBuffer()
	{
		alDeleteBuffers(1, &m_AudioID);
		m_AudioID = 0;
	}

	ALuint SFXBuffer::Load(const char* filename)
	{
		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		/* Open the audio file and check that it's usable. */
		sndfile = sf_open(filename, SFM_READ, &sfinfo);
		if (!sndfile)
		{
			DFG_CORE_ERROR("Could not open audio in {0}: {1}", filename, sf_strerror(sndfile));
			return 0;
		}
		if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
		{
			fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
			sf_close(sndfile);
			return 0;
		}

		/* Get the sound format, and figure out the OpenAL format */
		format = AL_NONE;
		if (sfinfo.channels == 1)
			format = AL_FORMAT_MONO16;
		else if (sfinfo.channels == 2)
			format = AL_FORMAT_STEREO16;
		else if (sfinfo.channels == 3)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sfinfo.channels == 4)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!format)
		{
			fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
			sf_close(sndfile);
			return 0;
		}

		/* Decode the whole audio file to a buffer. */
		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1)
		{
			free(membuf);
			sf_close(sndfile);
			fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
			return 0;
		}
		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

		/* Buffer the audio data into a new buffer object, then free the data and
		 * close the file.
		 */
		buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

		free(membuf);
		sf_close(sndfile);

		/* Check if an error occured, and clean up if so. */
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);
			return 0;
		}

		m_AudioID = buffer;
		return buffer;
	}

	AudioLibrary::~AudioLibrary()
	{
		//will delete all buffers because they are reference counted
		m_SFXBuffers.clear();
		m_MusicBuffers.clear();
	}

	//AudioStream
	AudioStream::AudioStream(const std::string& path)
	{
		Load(path.c_str());
	}

	AudioStream::~AudioStream()
	{
		if (m_SndFile)
		{
			sf_close(m_SndFile);
		}
		m_SndFile = nullptr;
		free(m_Membuf);
		alDeleteBuffers(NUM_BUFFERS, m_AudioIDs);
		delete m_SFinfo;
	}

	void AudioStream::Load(const char* filename)
	{
		alGenBuffers(NUM_BUFFERS, m_AudioIDs);

		m_SFinfo = new SF_INFO();
		std::size_t frame_size;

		m_SndFile = sf_open(filename, SFM_READ, m_SFinfo);
		if (!m_SndFile)
		{
			throw("could not open provided music file -- check path");
		}

		/* Get the sound format, and figure out the OpenAL format */
		if (m_SFinfo->channels == 1)
			m_Format = AL_FORMAT_MONO16;
		else if (m_SFinfo->channels == 2)
			m_Format = AL_FORMAT_STEREO16;
		else if (m_SFinfo->channels == 3)
		{
			if (sf_command(m_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				m_Format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (m_SFinfo->channels == 4)
		{
			if (sf_command(m_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				m_Format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!m_Format)
		{
			sf_close(m_SndFile);
			m_SndFile = NULL;
			throw("Unsupported channel count from file");
		}

		frame_size = ((size_t)BUFFER_SAMPLES * (size_t)m_SFinfo->channels) * sizeof(short);
		m_Membuf = static_cast<short*>(malloc(frame_size));
	}

	//AudioLibrary
	void AudioLibrary::AddSFX(const std::string& name, const Ref<SFXBuffer>& audioBuffer)
	{
		DFG_ASSERT(!SFXExists(name), "SFX already exists!");
		m_SFXBuffers[name] = audioBuffer;
	}

	void AudioLibrary::AddMusic(const std::string& name, const Ref<AudioStream>& audioBuffer)
	{
		DFG_ASSERT(!MusicExists(name), "Music already exists!");
		m_MusicBuffers[name] = audioBuffer;
	}

	Ref<SFXBuffer> AudioLibrary::LoadSFX(const std::string& name, const std::string& filePath)
	{
		Ref<SFXBuffer> buffer = CreateRef<SFXBuffer>(filePath);
		AddSFX(name, buffer);
		return buffer;
	}

	Ref<AudioStream> AudioLibrary::LoadMusic(const std::string& name, const std::string& filePath)
	{
		Ref<AudioStream> buffer = CreateRef<AudioStream>(filePath);
		AddMusic(name, buffer);
		return buffer;
	}

	Ref<SFXBuffer> AudioLibrary::GetSFX(const std::string& name)
	{
		DFG_ASSERT(SFXExists(name), "Audio Buffer not found!");
		return m_SFXBuffers[name];
	}

	Ref<AudioStream> AudioLibrary::GetMusic(const std::string& name)
	{
		DFG_ASSERT(MusicExists(name), "Music Buffer not found!");
		return m_MusicBuffers[name];
	}

	bool AudioLibrary::SFXExists(const std::string& name) const
	{
		return m_SFXBuffers.find(name) != m_SFXBuffers.end();
	}

	bool AudioLibrary::MusicExists(const std::string& name) const
	{
		return m_MusicBuffers.find(name) != m_MusicBuffers.end();
	}
}
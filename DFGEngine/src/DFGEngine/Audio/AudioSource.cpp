#include "dfgpch.h"
#include "AudioSource.h"
#include "AudioBuffer.h"

#include <AL\al.h>
#include <sndfile.h>

namespace DFGEngine
{
	AudioSource::AudioSource()
	{
		alGenSources(1, &m_AudioID);
		alSourcei(m_AudioID, AL_BUFFER, m_Buffer);
		//AL_CheckAndThrow();
	}

	AudioSource::~AudioSource()
	{
		alDeleteSources(1, &m_AudioID);
		m_AudioID = 0;
	}

	void AudioSource::PlaySFX(const ALuint& buffer_to_play)
	{
		if (buffer_to_play != m_Buffer)
		{
			m_Buffer = buffer_to_play;
			alSourcei(m_AudioID, AL_BUFFER, (ALuint)m_Buffer);
			//AL_CheckAndThrow();
		}
		alSourcePlay(m_AudioID);
		//AL_CheckAndThrow();
	}

	void AudioSource::PlayMusic(const Ref<AudioStream>& audioStream)
	{
		if (m_AudioStream != audioStream)
		{
			m_AudioStream = audioStream;
			//set file pointer to the beginning
			sf_seek(m_AudioStream->GetSNDFILE(), 0, SEEK_SET);
		}
		alSourcei(m_AudioID, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcef(m_AudioID, AL_ROLLOFF_FACTOR, 0.0);

		// clear any al errors
		alGetError();

		/* Rewind the source position and clear the buffer queue */
		alSourceRewind(m_AudioID);
		alSourcei(m_AudioID, AL_BUFFER, AL_NONE);

		/* Fill the buffer queue */
		ALsizei i;
		for (i = 0; i < m_AudioStream->GetAmountOfBuffers(); i++)
		{
			/* Get some data to give it to the buffer */
			sf_count_t slen = sf_readf_short(m_AudioStream->GetSNDFILE(), m_AudioStream->GetMembuf(), m_AudioStream->GetSamples());

			if (slen < 1) break;
			slen *= m_AudioStream->GetSFinfo()->channels * (sf_count_t)sizeof(short);
			alBufferData(m_AudioStream->GetAudioIDs()[i], m_AudioStream->GetFormat(), m_AudioStream->GetMembuf(), (ALsizei)slen, m_AudioStream->GetSFinfo()->samplerate);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error buffering for playback");
		}

		/* Now queue and start playback! */
		alSourceQueueBuffers(m_AudioID, i, m_AudioStream->GetAudioIDs());
		alSourcePlay(m_AudioID);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error starting playback");
		}
	}

	void AudioSource::UpdateStream()
	{
		if (!m_AudioStream) return;

		ALint processed, state;

		// clear error 
		//alGetError();
		/* Get relevant source info */
		alGetSourcei(m_AudioID, AL_SOURCE_STATE, &state);
		alGetSourcei(m_AudioID, AL_BUFFERS_PROCESSED, &processed);
		//AL_CheckAndThrow();

		/* Unqueue and handle each processed buffer */
		while (processed > 0)
		{
			ALuint bufid;
			sf_count_t slen;

			alSourceUnqueueBuffers(m_AudioID, 1, &bufid);
			processed--;

			/* Read the next chunk of data, refill the buffer, and queue it
			 * back on the source */
			slen = sf_readf_short(m_AudioStream->GetSNDFILE(), m_AudioStream->GetMembuf(), m_AudioStream->GetSamples());
			if (slen > 0)
			{
				slen *= m_AudioStream->GetSFinfo()->channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, m_AudioStream->GetFormat(), m_AudioStream->GetMembuf(), (ALsizei)slen, m_AudioStream->GetSFinfo()->samplerate);
				alSourceQueueBuffers(m_AudioID, 1, &bufid);
			}
			if (alGetError() != AL_NO_ERROR)
			{
				throw("error buffering music data");
			}
		}

		//Make sure the source hasn't underrun
		if (state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;

			/* If no buffers are queued, playback is finished */
			alGetSourcei(m_AudioID, AL_BUFFERS_QUEUED, &queued);
			//AL_CheckAndThrow();
			if (queued == 0)
			{
				if (m_Loop)
				{
					LoopStream();
				}
				else
				{
					return;
				}
			}

			alSourcePlay(m_AudioID);
			//AL_CheckAndThrow();
		}
	}

	bool AudioSource::IsPlaying()
	{
		ALint playState;
		alGetSourcei(m_AudioID, AL_SOURCE_STATE, &playState);
		return (playState == AL_PLAYING);
	}

	void AudioSource::SetGain(const float& val)
	{
		float newval = val;
		if (newval < 0) { newval = 0; }
		alSourcef(m_AudioID, AL_GAIN, val);
	}

	void AudioSource::LoopStream()
	{
		if (!m_AudioStream) return;

		/* Rewind the source position and clear the buffer queue */
		alSourceRewind(m_AudioID);
		alSourcei(m_AudioID, AL_BUFFER, AL_NONE);

		//set file pointer to the beginning
		sf_seek(m_AudioStream->GetSNDFILE(), 0, SEEK_SET);

		/* Fill the buffer queue */
		ALsizei i;
		for (i = 0; i < m_AudioStream->GetAmountOfBuffers(); i++)
		{
			/* Get some data to give it to the buffer */
			sf_count_t slen = sf_readf_short(m_AudioStream->GetSNDFILE(), m_AudioStream->GetMembuf(), m_AudioStream->GetSamples());
			if (slen < 1) break;

			slen *= m_AudioStream->GetSFinfo()->channels * (sf_count_t)sizeof(short);
			alBufferData(m_AudioStream->GetAudioIDs()[i], m_AudioStream->GetFormat(), m_AudioStream->GetMembuf(), (ALsizei)slen, m_AudioStream->GetSFinfo()->samplerate);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error buffering for playback");
		}

		/* Now queue and start playback! */
		alSourceQueueBuffers(m_AudioID, i, m_AudioStream->GetAudioIDs());
	}
}
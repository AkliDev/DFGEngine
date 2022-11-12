struct ALCdevice;
struct ALCcontext;

namespace DFGEngine
{
	class AudioDevice
	{
	public:
		AudioDevice();
		~AudioDevice();

		//void GetLocation(float x, float y, float z);
		//void GetOrientation(float orientation);
		//float GetGain();
		//
		//void SetAttunation(int key);
		//void SetLocation(float x, float y, float z);
		//void SetOrientation( float atx, const float aty, const float atz, const float upx, const float upy, const float upz);
		//void SetGain(float value);

	private:
		ALCdevice* m_ALCDevice;
		ALCcontext* m_ALCContext;
	};
}
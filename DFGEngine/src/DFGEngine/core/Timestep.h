#pragma once

namespace DFGEngine
{
	class Timestep
	{
	public:
		Timestep(uint32_t time = 0)
			:m_Time(time)
		{
		
		}

		operator float() const { return GetSeconds(); }
		
		float GetSeconds() const { return (float)m_Time * .001f; }
		uint32_t  GetMilliseconds() const { return m_Time; }
	private:
		uint32_t m_Time;
	};
}
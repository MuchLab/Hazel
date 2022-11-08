#pragma once

namespace Hazel {
	class Timestep {
	public:
		Timestep(float time)
			: m_Time(time)
		{
		}

		operator float() { return m_Time; }

		inline float GetSecond() const { return m_Time; }
		inline float GetMilliSecond() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}
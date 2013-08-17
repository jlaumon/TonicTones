#ifndef TONICTONES_PRECISONTIMER_H
#define TONICTONES_PRECISONTIMER_H

#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>

class PrecisonTimer
{
	int64_t m_startTime;
	int64_t m_totalTime;
public:
	PrecisonTimer()
	{
		restart();
	}

	void restart() 
	{
		m_startTime = getCurrentTimeUS();
		m_totalTime = 0LL;
	}

	void stop() 
	{
		int64_t stopTime = getCurrentTimeUS();
		m_totalTime += stopTime - m_startTime;
		m_startTime = 0LL;
	}

	void resume() 
	{
		m_startTime = getCurrentTimeUS();
	}

	int64_t getTimeUS() 
	{
		int64_t elapsedTime = 0LL;
		if (m_startTime)
		{
			elapsedTime = getCurrentTimeUS() - m_startTime;
		}
		return m_totalTime + elapsedTime; 
	}

private:
	static int64_t getCurrentTimeUS() 
	{ 
		// Get the frequency once
		static LARGE_INTEGER frequency = {0};
		if (frequency.QuadPart == 0)
		{
			QueryPerformanceFrequency(&frequency);
		}

		// Query the timer
		LARGE_INTEGER performanceCount;
		QueryPerformanceCounter(&performanceCount);

		return 1000000LL * performanceCount.QuadPart / frequency.QuadPart;
	}
};
#else
// todo
class PrecisonTimer
{
public:
	void restart() {}
	void stop() {}
	void resume() {}
	int64_t getTimeUS() { return 0LL; }
};
#endif

#endif
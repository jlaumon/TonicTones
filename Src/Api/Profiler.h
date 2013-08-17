#ifndef TONICTONES_PROFILER_H
#define TONICTONES_PROFILER_H

#include <Singleton.h>
#include <Exports.h>
#include <PrecisionTimer.h>
#include <vector>

#define PROFILING_ENABLED

#ifndef PROFILING_ENABLED
#define PROFILE(x)
#define PROFILE_FUNC()

#else
#define PROFILE(x) ProfilerSession ___profiler_session(x)
#define PROFILE_FUNC() PROFILE(__FUNCTION__) // caveat: __FUNCTION__ is not standard

class TT_API Profiler : public Singleton<Profiler>
{
	friend class ProfilerSession;

	static const int SESSION_NAME_MAX_SIZE = 32;

	struct SessionData
	{
		SessionData() 
			: minTime  (-1)
			, maxTime  (0)
			, totalTime(0)
			, runCount (0)
		{
			name[0] = 0;
		}

		char name[SESSION_NAME_MAX_SIZE];
		uint32_t minTime;
		uint32_t maxTime;
		uint32_t totalTime;
		uint32_t runCount;
	};

	std::vector<SessionData> m_sessions;

	void registerSession(const char* name, uint32_t time);
public:
	void reset();
	void display() const;
	void writeCSV(const char* fileName) const;
};

class ProfilerSession
{
	const char*   m_name;
	PrecisonTimer m_timer;

public:
	ProfilerSession(const char* name)
		: m_name(name)
	{}

	~ProfilerSession()
	{
		Profiler::instance()->registerSession(m_name, (uint32_t)m_timer.getTimeUS());
	}
};

#endif // PROFILING_ENABLED
#endif
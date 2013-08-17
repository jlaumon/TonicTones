#include "Profiler.h"
#include <QtDebug>
#include <string.h>
#include <stdio.h>

#ifdef PROFILING_ENABLED

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

void Profiler::registerSession(const char* name, uint32_t time)
{
	// find the session with a matching name
	SessionData* session = NULL;
	for (int i=0, n=m_sessions.size(); i<n; ++i)
	{
		if (strncmp(m_sessions[i].name, name, SESSION_NAME_MAX_SIZE-1) == 0)
		{
			// found the session
			session = &m_sessions[i];
			break;
		}
	}
	if (session == NULL)
	{
		// register a new session
		m_sessions.push_back(SessionData());
		session = &m_sessions.back();

		strncpy(session->name, name, SESSION_NAME_MAX_SIZE);
		session->name[SESSION_NAME_MAX_SIZE - 1] = 0;
	}

	session->minTime    = time < session->minTime ? time : session->minTime;
	session->maxTime    = time > session->maxTime ? time : session->maxTime;
	session->totalTime += time;
	session->runCount  += 1;
}

void Profiler::reset() 
{ 
	m_sessions.clear(); 
}

void Profiler::display() const
{
#ifdef _WIN32
	OutputDebugStringA("PROFILER DISPLAY\n");
	for (int i=0, n=m_sessions.size(); i<n && m_sessions[i].name[0]; ++i)
	{
		char buff[256];
		sprintf(buff, "%s:\n    AV: %7.2f ms, MIN: %7.2f ms, MAX: %7.2f ms, (NB RUNS: %u)\n", 
			m_sessions[i].name,
			(float)m_sessions[i].totalTime * 0.001f / (float)m_sessions[i].runCount,
			(float)m_sessions[i].minTime * 0.001f,
			(float)m_sessions[i].maxTime * 0.001f,
			m_sessions[i].runCount);
		OutputDebugStringA(buff);
	}
	OutputDebugStringA("\n");
#endif
}

void Profiler::writeCSV(const char* fileName) const
{
	FILE * file;

	file = fopen(fileName , "w");
	if (file)
	{
		const char* firstLine = "\"Session Name\";\"Average Time (ms)\";\"Min Time (ms)\";\"Max Time (ms)\";\"Number of Runs\"\n";
		fwrite(firstLine, strlen(firstLine), 1, file);
		for (int i=0, n=m_sessions.size(); i<n && m_sessions[i].name[0]; ++i)
		{
			char buff[256];
			sprintf(buff, "\"%s\";%.2f;%.2f;%.2f;%u\n",
				m_sessions[i].name,
				(float)m_sessions[i].totalTime * 0.001f / (float)m_sessions[i].runCount,
				(float)m_sessions[i].minTime * 0.001f,
				(float)m_sessions[i].maxTime * 0.001f,
				m_sessions[i].runCount);
			fwrite(buff, strlen(buff), 1, file);
		}
		fclose(file);
	}
	else
	{
		qWarning() << "Can't write profiling infos to " << fileName;
	}
}


#endif
//============ Copyright (c) Cygnus Software, All rights reserved. ============
//
// ETW (Event Tracing for Windows) profiling helpers.
// This allows easy insertion of Generic Event markers into ETW/xperf tracing
// which then aids in analyzing the traces and finding performance problems.
//
//===============================================================================

#include "stdafx.h"
#include "etwprof.h"

#ifdef	ETW_MARKS_ENABLED

// After building the DLL if it has never been registered on this machine or
// if the providers have changed you need to go:
//    xcopy /y FractalX.exe %temp%
//    wevtutil um ETWProvider.man
//    wevtutil im ETWProvider.man

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// These are defined in evntrace.h but you need a Vista+ Windows
// SDK to have them available, so I define them here.
#define EVENT_CONTROL_CODE_DISABLE_PROVIDER 0
#define EVENT_CONTROL_CODE_ENABLE_PROVIDER  1
#define EVENT_CONTROL_CODE_CAPTURE_STATE    2

#ifdef EVNTAPI
// Make sure we don't include evntprov.h before #defining EVNTAPI
// If we do then we may end up calling the imported NTDLL functions and
// then our code will fail to run on Windows XP.
#error "We are calling the imported functions. This will not work on Windows XP"
#endif

// EVNTAPI is used in evntprov.h which is included by ETWProviderGenerated.h
// We define EVNTAPI without the DECLSPEC_IMPORT specifier so that
// we can implement these functions locally instead of using the import library,
// and can therefore still run on Windows XP.
#define EVNTAPI __stdcall
// Include the event register/write/unregister macros compiled from the manifest file.
// Note that this includes evntprov.h which requires a Vista+ Windows SDK.
#include "ETWProviderGenerated.h"

// Typedefs for use with GetProcAddress
typedef ULONG (__stdcall *tEventRegister)( LPCGUID ProviderId, PENABLECALLBACK EnableCallback, PVOID CallbackContext, PREGHANDLE RegHandle);
typedef ULONG (__stdcall *tEventWrite)( REGHANDLE RegHandle, PCEVENT_DESCRIPTOR EventDescriptor, ULONG UserDataCount, PEVENT_DATA_DESCRIPTOR UserData);
typedef ULONG (__stdcall *tEventUnregister)( REGHANDLE RegHandle );

// Helper class to dynamically load Advapi32.dll, find the ETW functions, 
// register the providers if possible, and get the performance counter frequency.
class CETWRegister
{
public:
	CETWRegister()
	{
		QueryPerformanceFrequency( &m_frequency );

		// Find Advapi32.dll. This should always succeed.
		HMODULE pAdvapiDLL = LoadLibraryW( L"Advapi32.dll" );
		if ( pAdvapiDLL )
		{
			// Try to find the ETW functions. This will fail on XP.
			m_pEventRegister = ( tEventRegister )GetProcAddress( pAdvapiDLL, "EventRegister" );
			m_pEventWrite = ( tEventWrite )GetProcAddress( pAdvapiDLL, "EventWrite" );
			m_pEventUnregister = ( tEventUnregister )GetProcAddress( pAdvapiDLL, "EventUnregister" );

			// Register two ETW providers. If registration fails then the event logging calls will fail.
			// On XP these calls will do nothing.
			// On Vista and above, if these providers have been enabled by xperf or logman then
			// the *Context globals will be modified
			// like this:
			//     MatchAnyKeyword: 0xffffffffffffffff
			//     IsEnabled: 1
			//     Level: 255
			// In other words, fully enabled.

			EventRegisterMulti_FrameRate();
			EventRegisterMulti_Main();
			EventRegisterMulti_Worker();
			EventRegisterMulti_Input();

			// Emit the thread ID for the main thread. This also indicates that
			// the main provider is initialized.
			EventWriteThread_ID( GetCurrentThreadId(), "Main thread" );
			// Emit an input system event so we know that it is active.
			EventWriteKey_down( 0, 0, 0 );
		}
	}
	~CETWRegister()
	{
		// Unregister our providers.
		EventUnregisterMulti_Input();
		EventUnregisterMulti_Worker();
		EventUnregisterMulti_Main();
		EventUnregisterMulti_FrameRate();
	}

	tEventRegister m_pEventRegister;
	tEventWrite m_pEventWrite;
	tEventUnregister m_pEventUnregister;

	// QPC frequency
	LARGE_INTEGER m_frequency;

} g_ETWRegister;

// Redirector function for EventRegister. Called by macros in ETWProviderGenerated.h
ULONG EVNTAPI EventRegister( LPCGUID ProviderId, PENABLECALLBACK EnableCallback, PVOID CallbackContext, PREGHANDLE RegHandle )
{
	if ( g_ETWRegister.m_pEventRegister )
		return g_ETWRegister.m_pEventRegister( ProviderId, EnableCallback, CallbackContext, RegHandle );

	return 0;
}

// Redirector function for EventWrite. Called by macros in ETWProviderGenerated.h
ULONG EVNTAPI EventWrite( REGHANDLE RegHandle, PCEVENT_DESCRIPTOR EventDescriptor, ULONG UserDataCount, PEVENT_DATA_DESCRIPTOR UserData )
{
	if ( g_ETWRegister.m_pEventWrite )
		return g_ETWRegister.m_pEventWrite( RegHandle, EventDescriptor, UserDataCount, UserData );
	return 0;
}

// Redirector function for EventUnregister. Called by macros in ETWProviderGenerated.h
ULONG EVNTAPI EventUnregister( REGHANDLE RegHandle )
{
	if ( g_ETWRegister.m_pEventUnregister )
		return g_ETWRegister.m_pEventUnregister( RegHandle );
	return 0;
}

// Call QueryPerformanceCounter
static int64 GetQPCTime()
{
	LARGE_INTEGER time;

	QueryPerformanceCounter( &time );
	return time.QuadPart;
}

// Convert a QueryPerformanceCounter delta into milliseconds
static float QPCToMS( int64 nDelta )
{
	// Convert from a QPC delta to seconds.
	float flSeconds = ( float )( nDelta / double( g_ETWRegister.m_frequency.QuadPart ) );

	// Convert from seconds to milliseconds
	return flSeconds * 1000;
}

// Public functions for emitting ETW events.

int64 ETWMark( const char *pMessage )
{
	// If we are running on Windows XP or if our providers have not been enabled
	// (by xperf or other) then this will be false and we can early out.
	// Be sure to check the appropriate context for the event. This is only
	// worth checking if there is some cost beyond the EventWrite that we can
	// avoid -- the redirectors in this file guarantee that EventWrite is always
	// safe to call.
	if ( !MULTI_MAIN_Context.IsEnabled )
	{
		return 0;
	}

	int64 nTime = GetQPCTime();
	EventWriteMark( pMessage );
	return nTime;
}

int64 ETWMarkPrintf( const char *pMessage, ... )
{
	// If we are running on Windows XP or if our providers have not been enabled
	// (by xperf or other) then this will be false and we can early out.
	// Be sure to check the appropriate context for the event. This is only
	// worth checking if there is some cost beyond the EventWrite that we can
	// avoid -- the redirectors in this file guarantee that EventWrite is always
	// safe to call.
	if ( !MULTI_MAIN_Context.IsEnabled )
	{
		return 0;
	}

	char buffer[1000];
	va_list args;
	va_start( args, pMessage );
	vsprintf_s( buffer, pMessage, args );
	va_end( args );

	int64 nTime = GetQPCTime();
	EventWriteMark( buffer );
	return nTime;
}

// Track the depth of ETW Begin/End pairs. This needs to be per-thread
// if we start emitting marks on multiple threads. Using __declspec(thread)
// has some problems on Windows XP, but since these ETW functions only work
// on Vista+ that doesn't matter.
static __declspec( thread ) int s_nDepth;

int64 ETWBegin( const char *pMessage )
{
	// If we are running on Windows XP or if our providers have not been enabled
	// (by xperf or other) then this will be false and we can early out.
	// Be sure to check the appropriate context for the event. This is only
	// worth checking if there is some cost beyond the EventWrite that we can
	// avoid -- the redirectors in this file guarantee that EventWrite is always
	// safe to call.
	// In this case we also avoid the potentially unreliable TLS implementation
	// (for dynamically loaded DLLs) on Windows XP.
	if ( !MULTI_MAIN_Context.IsEnabled )
	{
		return 0;
	}

	int64 nTime = GetQPCTime();
	EventWriteStart( pMessage, s_nDepth++ );
	return nTime;
}

int64 ETWEnd( const char *pMessage, int64 nStartTime )
{
	// If we are running on Windows XP or if our providers have not been enabled
	// (by xperf or other) then this will be false and we can early out.
	// Be sure to check the appropriate context for the event. This is only
	// worth checking if there is some cost beyond the EventWrite that we can
	// avoid -- the redirectors in this file guarantee that EventWrite is always
	// safe to call.
	// In this case we also avoid the potentially unreliable TLS implementation
	// (for dynamically loaded DLLs) on Windows XP.
	if ( !MULTI_MAIN_Context.IsEnabled )
	{
		return 0;
	}

	int64 nTime = GetQPCTime();
	EventWriteStop( pMessage, --s_nDepth, QPCToMS( nTime - nStartTime ) );
	return nTime;
}



int64 ETWWorkerMark( const char *pMessage )
{
	// If we are running on Windows XP or if our providers have not been enabled
	// (by xperf or other) then this will be false and we can early out.
	// Be sure to check the appropriate context for the event. This is only
	// worth checking if there is some cost beyond the EventWrite that we can
	// avoid -- the redirectors in this file guarantee that EventWrite is always
	// safe to call.
	if ( !MULTI_WORKER_Context.IsEnabled )
	{
		return 0;
	}

	int64 nTime = GetQPCTime();
	EventWriteMarkWorker( pMessage );
	return nTime;
}

int64 ETWWorkerMarkPrintf( const char *pMessage, ... )
{
	// If we are running on Windows XP or if our providers have not been enabled
	// (by xperf or other) then this will be false and we can early out.
	// Be sure to check the appropriate context for the event. This is only
	// worth checking if there is some cost beyond the EventWrite that we can
	// avoid -- the redirectors in this file guarantee that EventWrite is always
	// safe to call.
	if ( !MULTI_WORKER_Context.IsEnabled )
	{
		return 0;
	}

	char buffer[1000];
	va_list args;
	va_start( args, pMessage );
	vsprintf_s( buffer, pMessage, args );
	va_end( args );

	int64 nTime = GetQPCTime();
	EventWriteMarkWorker( buffer );
	return nTime;
}

// Track the depth of ETW Begin/End pairs. This needs to be per-thread
// if we start emitting marks on multiple threads. Using __declspec(thread)
// has some problems on Windows XP, but since these ETW functions only work
// on Vista+ that doesn't matter.
static __declspec( thread ) int s_nWorkerDepth;

int64 ETWWorkerBegin( const char *pMessage )
{
	// If we are running on Windows XP or if our providers have not been enabled
	// (by xperf or other) then this will be false and we can early out.
	// Be sure to check the appropriate context for the event. This is only
	// worth checking if there is some cost beyond the EventWrite that we can
	// avoid -- the redirectors in this file guarantee that EventWrite is always
	// safe to call.
	// In this case we also avoid the potentially unreliable TLS implementation
	// (for dynamically loaded DLLs) on Windows XP.
	if ( !MULTI_WORKER_Context.IsEnabled )
	{
		return 0;
	}

	int64 nTime = GetQPCTime();
	EventWriteStartWorker( pMessage, s_nWorkerDepth++ );
	return nTime;
}

int64 ETWWorkerEnd( const char *pMessage, int64 nStartTime )
{
	// If we are running on Windows XP or if our providers have not been enabled
	// (by xperf or other) then this will be false and we can early out.
	// Be sure to check the appropriate context for the event. This is only
	// worth checking if there is some cost beyond the EventWrite that we can
	// avoid -- the redirectors in this file guarantee that EventWrite is always
	// safe to call.
	// In this case we also avoid the potentially unreliable TLS implementation
	// (for dynamically loaded DLLs) on Windows XP.
	if ( !MULTI_WORKER_Context.IsEnabled )
	{
		return 0;
	}

	int64 nTime = GetQPCTime();
	EventWriteStopWorker( pMessage, --s_nWorkerDepth, QPCToMS( nTime - nStartTime ) );
	return nTime;
}



static int s_nRenderFrameCount;

int ETWGetRenderFrameNumber()
{
	return s_nRenderFrameCount;
}

// Insert a render frame marker using the FrameRate provider. Automatically
// count the frame number and frame time.
void ETWRenderFrameMark()
{
	static int64 s_lastFrameTime;

	int64 nCurrentFrameTime = GetQPCTime();
	float flElapsedFrameTime = 0.0f;
	if ( s_nRenderFrameCount )
	{
		flElapsedFrameTime = QPCToMS( nCurrentFrameTime - s_lastFrameTime );
	}

	EventWriteRenderFrameMark( s_nRenderFrameCount, flElapsedFrameTime );

	++s_nRenderFrameCount;
	s_lastFrameTime = nCurrentFrameTime;
}

void ETWMouseDown( int whichButton, unsigned int flags, int x, int y )
{
	EventWriteMouse_down( whichButton, flags, x, y );
}

void ETWMouseUp( int whichButton, unsigned int flags, int x, int y )
{
	EventWriteMouse_up( whichButton, flags, x, y );
}

void ETWMouseMove( unsigned int flags, int nX, int nY )
{
	EventWriteMouse_move( flags, nX, nY );
}

void ETWMouseWheel( unsigned int flags, int zDelta, int nX, int nY )
{
	EventWriteMouse_wheel( flags, zDelta, nX, nY );
}

void ETWKeyDown( unsigned nChar, unsigned nRepCnt, unsigned flags )
{
	EventWriteKey_down( nChar, nRepCnt, flags );
}

#endif // ETW_MARKS_ENABLED

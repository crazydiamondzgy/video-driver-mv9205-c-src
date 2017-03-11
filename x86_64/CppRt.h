#ifndef CPPRT_HPP_
#define CPPRT_HPP_

#include "WDMHeaders.hpp"

void * _cdecl operator new( size_t sz );
void _cdecl operator delete( void *p );

#ifndef VC_4X_BUILD

// In VC versions below 5.0, the following two cases are covered by the 
// new and delete defined above. The following syntax is invalid in pre-
// 5.0 versions.
void * _cdecl operator new[]( size_t sz );
void _cdecl operator delete []( void *p );

#endif


VOID InitCppRT(void);
VOID TermCppRT(void);

// Timing functions

// Delay function in us. It spends at least the time request, but can be much longer
void udelay( DWORD time ); // delay in us.

// Delay function in ms. It spends at least the time request, but can be much longer
void mdelay( DWORD time );

// GetTickCount in milliseconds
ULONG GetTickCountInMs( void );

// BusyWait in us
void busywaitus( DWORD time );

// Create a new thread
bool CreateThread(PKSTART_ROUTINE ThreadStart, PVOID ptr);

// End the current Thread
void ExitThread(void);

#endif

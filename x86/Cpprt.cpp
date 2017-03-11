
#include "CppRt.h"

/**************************************************************************\
* Module Name: ourcrt.cpp
*
* This file contains magic that prevents the c runtimes from being 
* linked in.  If they are linked in, the driver will not load.
* This primarily comes into play when your driver is C++.
*
\**************************************************************************/

extern "C" const int _fltused = 0;

/**************************************************************************\
* int __cdecl _purecall
*
* This function serves to avoid linking the CRT library for C++ code.
*
* History:
*  24-Aug-1998 -by- Tom Zakrajsek [tomz]
*   Documented it.
*
\**************************************************************************/

int __cdecl _purecall( void )
{
	return( FALSE );
}

VOID InitCppRT(void) {}
VOID TermCppRT(void) {}

//////////////////////////////////////////////////////////////////
// memory allocation operators

void * _cdecl operator new( size_t sz )
{
   PVOID p = ExAllocatePoolWithTag( NonPagedPool, sz , '848b' );
   return p;
}

void _cdecl operator delete( void *p )
{
   if ( p ) {
      ExFreePool( p );
   }
}

#ifndef VC_4X_BUILD
// In VC versions below 5.0, the following two cases are covered by the 
// new and delete defined above. The following syntax is invalid in pre-
// 5.0 versions.
void * _cdecl operator new[]( size_t sz )  
{
   PVOID p = ExAllocatePoolWithTag( NonPagedPool, sz , '848b' );
   return p;
}

void _cdecl operator delete []( void *p )
{
   if ( p ) {
      ExFreePool( p );
   }
}
#endif

////////////////////////////////////////////////////////////////////
// Delay routines

// Delay function in us
void udelay( DWORD time ) // delay in us.
{
	LARGE_INTEGER tm;
	tm.QuadPart = - int( time * 10); // Relative mode
	KeDelayExecutionThread( KernelMode , FALSE , &tm );
}

// Delay function in ms
void mdelay( DWORD time )
{
	LARGE_INTEGER tm;
	tm.QuadPart = - int( time * 10000 ); // Relative mode
	KeDelayExecutionThread( KernelMode , FALSE , &tm );
}

// GetTickCount in milliseconds
ULONG GetTickCountInMs( void )
{
	LARGE_INTEGER t;
	KeQuerySystemTime(&t);
	return (ULONG)( t.QuadPart / 10000 );
}

void busywaitus( DWORD time ) 
{
	// First delay in multiples of 32 us (a?does support up to 32 us)
	DWORD times = time / 32; // Number of times to repeat this
	while (times--) KeStallExecutionProcessor( 32 );
	// Now delay the remaining time.
	KeStallExecutionProcessor(time % 32);
}

bool CreateThread(PKSTART_ROUTINE ThreadStart,	PVOID ptr)
{
	HANDLE Handle;
	NTSTATUS status = PsCreateSystemThread(&Handle,
						(ACCESS_MASK) 0L,
						NULL,
						NULL,
						NULL,
						ThreadStart,
						ptr);

	if (status != STATUS_SUCCESS) {
		return false;
	}

	// Don't need this for anything, so might as well close it now.
	// The thread will call PsTerminateThread on itself when it
	// is done.
	ZwClose(Handle);

	return true;
}

// This is called from a Thread context to end the Thread
void ExitThread(void)
{
	// Just end the System Thread
	PsTerminateSystemThread(STATUS_SUCCESS);
}
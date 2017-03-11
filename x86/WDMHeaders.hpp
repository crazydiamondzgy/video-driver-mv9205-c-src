#ifndef _WDMHEADERS_HPP_
#define _WDMHEADERS_HPP_

extern "C" {
	//#include <ntddk.h>
	#include <wdm.h>
	#include <wdmguid.h>
	#include <strmini.h>
	#include <ks.h>
	#include <ksmedia.h>
	#include <ddkmapi.h>
	
	NTKERNELAPI PHYSICAL_ADDRESS
	MmGetPhysicalAddress (
		IN PVOID BaseAddress
	);
		
#if defined (_AMD64_)
#else
	NTKERNELAPI PVOID 
	MmAllocateContiguousMemory(
		IN ULONG  NumberOfBytes,
		IN PHYSICAL_ADDRESS  HighestAcceptableAddress
		);

	NTKERNELAPI VOID
	MmFreeContiguousMemory (
		IN PVOID BaseAddress
		);

	NTKERNELAPI VOID
		KeSetTargetProcessorDpc (
		IN PRKDPC Dpc,
		IN CCHAR Number
		);
#endif
}


#define _NO_COM
#include "ddkernel.h"

#endif
#include "WDMHeaders.hpp"
#include <initguid.h>
#include "ntddk.h"
#include "wdm.h"
DEFINE_GUID(GUID_MV9205, 0x5665dec0, 0xa40a, 0x11d1, 0xb9, 0x84, 0x0, 0x20, 0xaf, 0xd7, 0x97, 0x84);
#define WRITE_REGISTER_DPGPRINT( addr, data )	{WRITE_REGISTER_ULONG(addr, data); }

#include "main.h"
#include "CppRt.h"
#include "Command.h"

char*	FrameRate_NTSC1[31]={
	"000000000000000000000000000000",	//0
	"000000000000000000000000000001",	//1
	"000000000000001000000000000001",	//2
	"000000000100000000010000000001",	//3
	"000000010000001000000010000001",	//4
	"000001000001000001000001000001",	//5
	"000010000100001000010000100001",	//6
	"000100001000100010001000010001",	//7
	"000100010010001000100010010001",	//8
	"001000100100100010010010001001",	//9
	"001001001001001001001001001001",	//10
	"001010010010010100100100101001",	//11
	"001010010100101001010010100101",	//12
	"010010101001010101001010100101",	//13
	"010101001010101010101001010101",	//14
	"010101010101010101010101010101",	//15
	"010101011010101010101011010101",	//16
	"010110101011010101101010110101",	//17
	"011010110101101011010110101101",	//18
	"011011011010110110101101101101",	//19
	"011011011011011011011011011011",	//20
	"101101101110110110111011011011",	//21
	"101110110111011101110110111011",	//22
	"101110111101110111011110111011",	//23
	"101111011110111101111011110111",	//24
	"110111110111110111110111110111",	//25
	"111011111101111111011111101111",	//26
	"111101111111110111111111101111",	//27
	"111111011111111111111011111111",	//28
	"111111111111110111111111111111",	//29
	"111111111111111111111111111111"	//30
};

char*	FrameRate_PAL1[26]={
	"0000000000000000000000000",	//0
	"0000000000000000000000001",	//1
	"0000000000001000000000001",	//2
	"0000000100000000100000001",	//3
	"0000010000001000001000001",	//4
	"0000100001000010000100001",	//5
	"0001000100001000100010001",	//6
	"0001001000100100010010001",	//7
	"0010010010001001001001001",	//8
	"0010010100100100101001001",	//9
	"0010100101001010010100101",	//10
	"0100101010100101010100101",	//11
	"0101010101001010101010101",	//12
	"0101010101011010101010101",	//13
	"0101101010101101010110101",	//14
	"0110101101011010110101101",	//15
	"0110110110101101101101101",	//16
	"1011011011011011011011011",	//17
	"1011011101101110111011011",	//18
	"1011101110111101110111011",	//19
	"1011110111101111011110111",	//20
	"1101111101111110111110111",	//21
	"1110111111101111111101111",	//22
	"1111101111111111110111111",	//23
	"1111111111101111111111111",	//24
	"1111111111111111111111111",	//25
};

char*	FrameRate_NTSC2[31]={
	"000000000000000000000000000000",	//0
	"000000000000000000000000000010",	//1
	"000000000000010000000000000010",	//2
	"000000001000000000100000000010",	//3
	"000000100000010000000100000010",	//4
	"000010000010000010000010000010",	//5
	"000100001000010000100001000010",	//6
	"001000010001000100010000100010",	//7
	"001000100100010001000100100010",	//8
	"010001001001000100100100010010",	//9
	"010010010010010010010010010010",	//10
	"010100100100101001001001010010",	//11
	"010100101001010010100101001010",	//12
	"100101010010101010010101001010",	//13
	"101010010101010101010010101010",	//14
	"101010101010101010101010101010",	//15
	"101010110101010101010110101010",	//16
	"101101010110101011010101101010",	//17
	"110101101011010110101101011010",	//18
	"110110110101101101011011011010",	//19
	"110110110110110110110110110110",	//20
	"011011011101101101110110110111",	//21
	"011101101110111011101101110111",	//22
	"011101111011101110111101110111",	//23
	"011110111101111011110111101111",	//24
	"101111101111101111101111101111",	//25
	"110111111011111110111111011111",	//26
	"111011111111101111111111011111",	//27
	"111110111111111111110111111111",	//28
	"111111111111101111111111111111",	//29
	"111111111111111111111111111111"	//30
};

char*	FrameRate_PAL2[26]={
	"0000000000000000000000000",	//0
	"0000000000000000000000010",	//1
	"0000000000010000000000010",	//2
	"0000001000000001000000010",	//3
	"0000100000010000010000010",	//4
	"0001000010000100001000010",	//5
	"0010001000010001000100010",	//6
	"0010010001001000100100010",	//7
	"0100100100010010010010010",	//8
	"0100101001001001010010010",	//9
	"0101001010010100101001010",	//10
	"1001010101001010101001010",	//11
	"1010101010010101010101010",	//12
	"1010101010110101010101010",	//13
	"1011010101011010101101010",	//14
	"1101011010110101101011010",	//15
	"1101101101011011011011010",	//16
	"0110110110110110110110111",	//17
	"0110111011011101110110111",	//18
	"0111011101111011101110111",	//19
	"0111101111011110111101111",	//20
	"1011111011111101111101111",	//21
	"1101111111011111111011111",	//22
	"1111011111111111101111111",	//23
	"1111111111011111111111111",	//24
	"1111111111111111111111111",	//25
};

extern "C" 
	ULONG STREAMAPI DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
	)
{
	DriverObject->DriverUnload = CAdapter::DriverUnload;
	DriverObject->DriverExtension->AddDevice = CAdapter::AddDevice;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = CAdapter::DispatchCreate;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = CAdapter::DispatchClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = CAdapter::DispatchControl;
	DriverObject->MajorFunction[IRP_MJ_CLEANUP] = CAdapter::DispatchCleanup;
	DriverObject->MajorFunction[IRP_MJ_POWER] = CAdapter::DispatchPower;
	DriverObject->MajorFunction[IRP_MJ_PNP] = CAdapter::DispatchPnp;

	return STATUS_SUCCESS;
}

CAdapter::CAdapter()
{
	m_dwINTMaskBar = 0xF0FBFFFF;
	m_IRQExpected = true;
	m_InterruptObject = NULL;
	m_pBaseAddr = NULL;

	m_pSystemMemory = NULL;

	m_pRISCMemory = NULL;
	m_dwRiscPhysical = 0;

	m_dwErrorCode = 0;
	for(int i=0; i<4; i++)
	{
		m_bField[i] = FALSE;

		m_pAudioData[i] = NULL;
		m_dwAudioField[i] = 0;
	}
	for(int i=0; i<8; i++)
	{
		m_bCaptureStart[i] = FALSE;
		m_nFrameSkip[i] = 0;

		m_pCaptureMemory[i] = NULL;
		m_pCaptureUser[i] = NULL;
		m_pCaptureMdl[i] = NULL;

		m_dwCapturePhysical[i] = 0;

		KeInitializeDpc(&m_IsrDpc[i], reinterpret_cast <PKDEFERRED_ROUTINE> (CAdapter::cbDpcFunc), this);
		KeSetTargetProcessorDpc(&m_IsrDpc[i], 0);
	}
	for(int i=0; i<8; i++)
	{
		m_pEvent[i] = NULL;
	}

	KeInitializeDpc (
		&m_IsrDpcErr[0], 
		reinterpret_cast <PKDEFERRED_ROUTINE> 
			(CAdapter::cbDpcFuncErr0),
		this
		);
	KeSetTargetProcessorDpc (
		&m_IsrDpcErr[0],	//
		0
		);

	KeInitializeDpc (
		&m_IsrDpcErr[1], 
		reinterpret_cast <PKDEFERRED_ROUTINE> 
			(CAdapter::cbDpcFuncErr1),
		this
		);
	KeSetTargetProcessorDpc (
		&m_IsrDpcErr[1],	//
		0
		);

	KeInitializeDpc (
		&m_IsrDpcErr[2], 
		reinterpret_cast <PKDEFERRED_ROUTINE> 
			(CAdapter::cbDpcFuncErr2),
		this
		);
	KeSetTargetProcessorDpc (
		&m_IsrDpcErr[2],	//
		0
		);

	KeInitializeDpc (
		&m_IsrDpcErr[3], 
		reinterpret_cast <PKDEFERRED_ROUTINE> 
			(CAdapter::cbDpcFuncErr3),
		this
		);
	KeSetTargetProcessorDpc (
		&m_IsrDpcErr[3],	//
		0
		);

	KeInitializeTimer(&DelayTimer);
	KeInitializeTimer(&DMATimer);
}

CAdapter::~CAdapter()
{
	KeCancelTimer(&DMATimer);

	for(int i=0; i<8; i++)
		KeRemoveQueueDpc(&m_IsrDpc[i]);

	for(int i=0; i<4; i++)
		KeRemoveQueueDpc(&m_IsrDpcErr[i]);
}


VOID CAdapter::DriverUnload(
	IN PDRIVER_OBJECT DriverObject
	)
{
}


NTSTATUS CAdapter::AddDevice(
	PDRIVER_OBJECT DriverObject,
	PDEVICE_OBJECT PDO 
	)
{
	PDEVICE_OBJECT deviceObject = NULL;
	PDATA_EXTENSION pDataExtension;
	CAdapter *adapter;
	NTSTATUS status;

	status = IoCreateDevice(
				 DriverObject,
				 sizeof(DATA_EXTENSION),                    
				 0,
				 FILE_DEVICE_UNKNOWN,
				 FILE_DEVICE_SECURE_OPEN,                    
				 FALSE,                
				 &deviceObject
				 );

	pDataExtension = (PDATA_EXTENSION)deviceObject->DeviceExtension;
	adapter = new CAdapter();
	pDataExtension->adapter = adapter;
	
	adapter->TopOfStackDeviceObject = IoAttachDeviceToDeviceStack( deviceObject, PDO );

	deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	
	deviceObject->Flags |= DO_BUFFERED_IO;

	IoRegisterDeviceInterface( PDO, &GUID_MV9205, NULL, &adapter->UnicodeString );
	
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::DispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//DbgPrint(("DispatchCreate\n"));
	PDATA_EXTENSION pDataExtension = (PDATA_EXTENSION)DeviceObject->DeviceExtension;
	CAdapter *adapter = pDataExtension->adapter;

	DWORD dwRead = READ_REGISTER_ULONG( PULONG(adapter->m_pBaseAddr + 0x00C) );
	dwRead &= 0xFFFF0000;
	dwRead |= 0x4008;
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x00C), dwRead );


	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x054), 0x0D200087 );//BaseClk 400KHz

	
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x200), 0x45100708 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x280), 0x45100708 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x300), 0x45100708 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x380), 0x45100708 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x208), 0x20203030 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x288), 0x20203030 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x308), 0x20203030 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x388), 0x20203030 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x218), 0x42000204 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x298), 0x42000204 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x318), 0x42000204 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x398), 0x42000204 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x248), 0x0C0702D0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x2C8), 0x0C0702D0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x348), 0x0C0702D0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x3C8), 0x0C0702D0 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x24C), 0x0180A8F0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x2CC), 0x0180A8F0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x34C), 0x0180A8F0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x3CC), 0x0180A8F0 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x250), 0x4348E0F0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x2D0), 0x4348E0F0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x350), 0x4348E0F0 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x3D0), 0x4348E0F0 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x254), 0x50467271 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x2D4), 0x50467271 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x354), 0x50467271 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x3D4), 0x50467271 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x258), 0x10096375 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x2D8), 0x10096375 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x358), 0x10096375 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x3D8), 0x10096375 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x25C), 0x00C0B920 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x2DC), 0x00C0B920 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x35C), 0x00C0B920 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x3DC), 0x00C0B920 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x260), 0x4ECA8120 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x2E0), 0x4ECA8120 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x360), 0x4ECA8120 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x3E0), 0x4ECA8120 );

	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x268), 0x3C000010 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x2E8), 0x3C000010 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x368), 0x3C000010 );
	WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + 0x3E8), 0x3C000010 );
	
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );

	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::DispatchClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//DbgPrint(("DispatchClose\n"));
	PDATA_EXTENSION pDataExtension = (PDATA_EXTENSION)DeviceObject->DeviceExtension;
	CAdapter *adapter = pDataExtension->adapter;

	DWORD	dwBase[8] = {0x200, 0x280, 0x300, 0x380, 0x270, 0x2F0, 0x370, 0x3F0};
	DWORD	dwRisc[8] = {0x60, 0x74, 0x88, 0x9C, 0xB0, 0xC4, 0xD8, 0xEC};
	DWORD	dwReg;

	adapter->m_dwINTMaskBar = 0xF0FBFFFF;
	//1. Dec off
	for(int i=0; i<8; i++)
	{
		if(adapter->m_bCaptureStart[i])
		{
			adapter->m_bCaptureStart[i] = FALSE;

			dwReg = READ_REGISTER_ULONG( PULONG(adapter->m_pBaseAddr + dwBase[i]) );
			dwReg &= 0x7FFFFFFF;
			WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + dwBase[i]), dwReg );
		}
	}

	//2. 10ms Delay
	adapter->Wait10ms();

	//3. DMA off
	for(int i=0; i<8; i++)
	{
		WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + dwRisc[i]), 0x000E8000 );
		WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + dwRisc[i] + 0x04), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + dwRisc[i] + 0x08), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + dwRisc[i] + 0x0C), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(adapter->m_pBaseAddr + dwRisc[i] + 0x10), 0x00000000 );
	}


	for(int i=0; i<8; i++)
	{
		if(adapter->m_pCaptureUser[i] != NULL)
		{
			// 우선 사용자가 사용하던 UserLevel가상메모리를 해제한다
			MmUnmapLockedPages( 
				adapter->m_pCaptureUser[i], 
				adapter->m_pCaptureMdl[i]
				);

			adapter->m_pCaptureUser[i] = NULL;

			// 그다음 Mdl을 해제한다
			IoFreeMdl( 
				adapter->m_pCaptureMdl[i] 
				);

			adapter->m_pCaptureMdl[i] = 0;
		}

		if(adapter->m_pEvent[i] != NULL)
		{
			//KeSetEvent(adapter->m_pEvent[i], 0, FALSE);
			ObDereferenceObject(adapter->m_pEvent[i]);
			adapter->m_pEvent[i] = NULL;
		}
	}

	for(int i=0; i<4; i++)
	{
		if(adapter->m_pAudioData[i] != NULL)
		{
			ExFreePool(adapter->m_pAudioData[i]);
			adapter->m_pAudioData[i] = NULL;
		}
	}

	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::DispatchControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//DbgPrint(("DispatchControl\n"));

	CAdapter *adapter;
	PDATA_EXTENSION pDataExtension;
	int* pBuf;
	DWORD **pLock;
	DWORD **pGPIO;

	pDataExtension = (PDATA_EXTENSION)DeviceObject->DeviceExtension;
	adapter = pDataExtension->adapter;

	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation( Irp );

	switch(pStack->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_MV9205_REG_WRITE:
		//DbgPrint(("IOCTL_MV9205_REG_WRITE\n"));
		Irp->IoStatus.Status = adapter->RegisterWrite( PMV9205_RW(Irp->AssociatedIrp.SystemBuffer) );
		Irp->IoStatus.Information = 0;
		break;
	case IOCTL_MV9205_REG_READ:
		//DbgPrint(("IOCTL_MV9205_REG_READ\n"));
		Irp->IoStatus.Status = adapter->RegisterRead( PMV9205_RW(Irp->AssociatedIrp.SystemBuffer) );
		Irp->IoStatus.Information = sizeof(MV9205_RW);
		break;
	case IOCTL_MV9205_VIDEO_FORMAT:
		//DbgPrint(("IOCTL_MV9205_VIDEO_FORMAT\n"));
		Irp->IoStatus.Status = adapter->SetVideoFormat( PMV9205_FORMAT(Irp->AssociatedIrp.SystemBuffer) );
		Irp->IoStatus.Information = 0;
		break;
	case IOCTL_MV9205_VIDEO_DISPLAY:
		//DbgPrint(("IOCTL_MV9205_VIDEO_DISPLAY\n"));
		Irp->IoStatus.Status = adapter->SetVideoDisplay( PMV9205_DISPLAY(Irp->AssociatedIrp.SystemBuffer) );
		Irp->IoStatus.Information = 0;
		break;
	case IOCTL_MV9205_CAP_EVENT:
		//DbgPrint(("IOCTL_MV9205_CAP_EVENT\n"));
		Irp->IoStatus.Status = adapter->SetEvent( PMV9205_EVENT(Irp->AssociatedIrp.SystemBuffer), Irp->RequestorMode );
		Irp->IoStatus.Information = 0;
		break;
	case IOCTL_MV9205_CAP_MEM:
		//DbgPrint(("IOCTL_MV9205_CAP_MEM\n"));
		Irp->IoStatus.Status = adapter->GetVideoMemory( PMV9205_VIDEOMEM(Irp->AssociatedIrp.SystemBuffer) );
		Irp->IoStatus.Information = sizeof(MV9205_VIDEOMEM);
		break;
	case IOCTL_MV9205_AUDIO_FORMAT:
		//DbgPrint(("IOCTL_MV9205_AUDIO_FORMAT\n"));
		Irp->IoStatus.Status = adapter->SetAudioFormat( PMV9205_AUDIOFM(Irp->AssociatedIrp.SystemBuffer) );
		Irp->IoStatus.Information = 0;
		break;
	case IOCTL_MV9205_GET_LOCK:
		//DbgPrint(("IOCTL_MV9205_GET_LOCK\n"));
		pLock = (DWORD**)Irp->AssociatedIrp.SystemBuffer;
		Irp->IoStatus.Status = adapter->GetVideoLock( *pLock );
		Irp->IoStatus.Information = 0;
		break;
	case IOCTL_MV9205_SET_GPIO:
		//DbgPrint(("IOCTL_MV9205_SET_GPIO\n"));
		pGPIO = (DWORD**)Irp->AssociatedIrp.SystemBuffer;
		Irp->IoStatus.Status = adapter->SetGPIO( *pGPIO );
		Irp->IoStatus.Information = 0;
		break;
	case IOCTL_MV9205_GET_GPIO:
		//DbgPrint(("IOCTL_MV9205_GET_GPIO\n"));
		pGPIO = (DWORD**)Irp->AssociatedIrp.SystemBuffer;
		Irp->IoStatus.Status = adapter->GetGPIO( *pGPIO );
		Irp->IoStatus.Information = 0;
		break;
	case IOCTL_MV9205_SET_GPOE:
		//DbgPrint(("IOCTL_MV9205_SET_GPOE\n"));
		pGPIO = (DWORD**)Irp->AssociatedIrp.SystemBuffer;
		Irp->IoStatus.Status = adapter->SetGPOE( *pGPIO );
		Irp->IoStatus.Information = 0;
		break;
	default:
		//DbgPrint(("UnKnown\n"));
		Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
		break;
	}
	IoCompleteRequest( Irp, IO_NO_INCREMENT );

	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::DispatchCleanup(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//DbgPrint(("DispatchCleanup\n"));
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::DispatchPower(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//DbgPrint(("DispatchPower\n"));
	PDATA_EXTENSION pDataExtension = (PDATA_EXTENSION)DeviceObject->DeviceExtension;
	CAdapter *adapter = pDataExtension->adapter;

    PoStartNextPowerIrp(Irp);
    IoSkipCurrentIrpStackLocation(Irp);
    return PoCallDriver(adapter->TopOfStackDeviceObject, Irp);

	//IoSkipCurrentIrpStackLocation( Irp );
	//return IoCallDriver( adapter->TopOfStackDeviceObject, Irp );

//	Irp->IoStatus.Status = STATUS_SUCCESS;
//	IoCompleteRequest( Irp, IO_NO_INCREMENT );
//	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::DispatchPnp(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//DbgPrint(("DispatchPnp\n"));
	PDATA_EXTENSION pDataExtension = (PDATA_EXTENSION)DeviceObject->DeviceExtension;
	CAdapter *adapter = pDataExtension->adapter;

	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(Irp);

	switch( pStack->MinorFunction )
		{
		case IRP_MN_START_DEVICE:					DbgPrint("--IRP_MN_START_DEVICE\n");	break;
		case IRP_MN_QUERY_STOP_DEVICE:				DbgPrint("--IRP_MN_QUERY_STOP_DEVICE\n");	break;
		case IRP_MN_STOP_DEVICE:					DbgPrint("--IRP_MN_STOP_DEVICE\n");	break;
		case IRP_MN_CANCEL_STOP_DEVICE:				DbgPrint("--IRP_MN_CANCEL_STOP_DEVICE\n");	break;
		case IRP_MN_QUERY_REMOVE_DEVICE:			DbgPrint("--IRP_MN_QUERY_REMOVE_DEVICE\n");	break;
		case IRP_MN_REMOVE_DEVICE:					DbgPrint("--IRP_MN_REMOVE_DEVICE\n");	break;
		case IRP_MN_CANCEL_REMOVE_DEVICE:			DbgPrint("--IRP_MN_CANCEL_REMOVE_DEVICE\n");	break;
		case IRP_MN_SURPRISE_REMOVAL:				DbgPrint("--IRP_MN_SURPRISE_REMOVAL\n");	break;
		case IRP_MN_QUERY_CAPABILITIES:				DbgPrint("--IRP_MN_QUERY_CAPABILITIES\n");	break;
		case IRP_MN_QUERY_PNP_DEVICE_STATE:			DbgPrint("--IRP_MN_QUERY_PNP_DEVICE_STATE\n");	break;
		case IRP_MN_FILTER_RESOURCE_REQUIREMENTS:	DbgPrint("--IRP_MN_FILTER_RESOURCE_REQUIREMENTS\n");	break;
		case IRP_MN_DEVICE_USAGE_NOTIFICATION:		DbgPrint("--IRP_MN_DEVICE_USAGE_NOTIFICATION\n");	break;
		case IRP_MN_QUERY_DEVICE_RELATIONS:			DbgPrint("--IRP_MN_QUERY_DEVICE_RELATIONS\n");	break;
		case IRP_MN_QUERY_RESOURCES:				DbgPrint("--IRP_MN_QUERY_RESOURCES\n");	break;
		case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:	DbgPrint("--IRP_MN_QUERY_RESOURCE_REQUIREMENTS\n");	break;
		case IRP_MN_QUERY_ID:						DbgPrint("--IRP_MN_QUERY_ID\n");	break;
		case IRP_MN_QUERY_DEVICE_TEXT:				DbgPrint("--IRP_MN_QUERY_DEVICE_TEXT\n");	break;
		case IRP_MN_QUERY_BUS_INFORMATION:			DbgPrint("--IRP_MN_QUERY_BUS_INFORMATION\n");	break;
		case IRP_MN_QUERY_INTERFACE:				DbgPrint("--IRP_MN_QUERY_INTERFACE\n");	break;
		case IRP_MN_READ_CONFIG:					DbgPrint("--IRP_MN_READ_CONFIG\n");	break;
		case IRP_MN_WRITE_CONFIG:					DbgPrint("--IRP_MN_WRITE_CONFIG\n");	break;
		case IRP_MN_EJECT:							DbgPrint("--IRP_MN_EJECT\n");	break;
		case IRP_MN_SET_LOCK:						DbgPrint("--IRP_MN_SET_LOCK\n");	break;
		};

	switch( pStack->MinorFunction )
			{
			case IRP_MN_START_DEVICE:
				{
					//DbgPrint((" IRP_MN_START_DEVICE\n"));
					PCM_RESOURCE_LIST raw = pStack->Parameters.StartDevice.AllocatedResources;
					PCM_RESOURCE_LIST translated = pStack->Parameters.StartDevice.AllocatedResourcesTranslated;

					adapter->PnpStartDevice(DeviceObject, Irp, raw, translated);
					
					IoSkipCurrentIrpStackLocation( Irp );
					return IoCallDriver( adapter->TopOfStackDeviceObject, Irp );
				}
			case IRP_MN_STOP_DEVICE:
				{
					//DbgPrint((" IRP_MN_STOP_DEVICE\n"));
					adapter->PnpStopDevice(DeviceObject);
					
					IoSkipCurrentIrpStackLocation( Irp );
					return IoCallDriver( adapter->TopOfStackDeviceObject, Irp );
				}

			case IRP_MN_REMOVE_DEVICE: // 장치를 제거하는 경우,
				{
					//DbgPrint((" IRP_MN_REMOVE_DEVICE\n"));
					PDEVICE_OBJECT LowerDevice;
					LowerDevice = adapter->TopOfStackDeviceObject;
					IoDetachDevice( adapter->TopOfStackDeviceObject );
					// SAMPLE DeviceObject를 DeviceStack으로 부터 제거.

					IoSetDeviceInterfaceState( &adapter->UnicodeString, FALSE );
					// 사용자가 우리의 DeviceStack에 접근하는 것을 금지
					
					RtlFreeUnicodeString( &adapter->UnicodeString );
					// 사용자가 접근하는 이름을 메모리에서 해제한다
					
					IoDeleteDevice( DeviceObject );
					// SAMPLE DeviceObject를 제거
					
					IoSkipCurrentIrpStackLocation( Irp );

					//DbgPrint("------------\n");
					//DbgPrint("DeleteMemory\n");

					if(adapter->m_pRISCMemory != NULL)
					{
						MmFreeContiguousMemory( adapter->m_pRISCMemory );
						adapter->m_pRISCMemory = NULL;
						adapter->m_dwRiscPhysical = 0;
					}
					if(adapter->m_pSystemMemory != NULL)
					{
						MmFreeContiguousMemory( adapter->m_pSystemMemory );
						adapter->m_pSystemMemory = NULL;
						for(int i=0; i<8; i++)
						{
							adapter->m_pCaptureMemory[i] = NULL;
							adapter->m_dwCapturePhysical[i] = 0;
						}

					}

					if(adapter->m_pBaseAddr != NULL)
					{
						MmUnmapIoSpace(adapter->m_pBaseAddr, 4096);
						adapter->m_pBaseAddr = NULL;
					}
					if(adapter->m_InterruptObject != NULL)
					{
						IoDisconnectInterrupt(adapter->m_InterruptObject);
						adapter->m_InterruptObject = NULL;
					}
					delete adapter;

					pDataExtension->adapter = NULL;
					return IoCallDriver( LowerDevice, Irp );
				}
			default:
				//DbgPrint((" Unknown\n"));
				IoSkipCurrentIrpStackLocation( Irp );
				return IoCallDriver( adapter->TopOfStackDeviceObject, Irp );
			}
			
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::PnpStartDevice(PDEVICE_OBJECT DeviceObject, PIRP Irp, PCM_RESOURCE_LIST raw, PCM_RESOURCE_LIST translated)
{
	//DbgPrint(("PnpStartDevice\n"));
	ULONG		int_Level;
	ULONG		int_Vector;
	KAFFINITY	int_Affinity;
	KINTERRUPT_MODE int_Mode;
	BOOLEAN int_Share;


	PCM_PARTIAL_RESOURCE_DESCRIPTOR resource = translated->List[0].PartialResourceList.PartialDescriptors;
	ULONG nres = translated->List[0].PartialResourceList.Count;

	for (ULONG i = 0; i < nres; ++i, ++resource)
	{						// for each resource
		switch (resource->Type)
		{					// switch on resource type
			case CmResourceTypeMemory:
				if(m_pBaseAddr != NULL)
				{
					MmUnmapIoSpace(m_pBaseAddr, 4096);
					m_pBaseAddr = NULL;
				}
				m_pBaseAddr = (BYTE*)MmMapIoSpace(
					resource->u.Memory.Start,
					4096,
					MmNonCached );
				break;
		
			case CmResourceTypeInterrupt:
				int_Level = (KIRQL) resource->u.Interrupt.Level;
				int_Vector = resource->u.Interrupt.Vector;
				int_Affinity = resource->u.Interrupt.Affinity;
				int_Mode = (resource->Flags == CM_RESOURCE_INTERRUPT_LATCHED)
					? Latched : LevelSensitive;
				int_Share = resource->ShareDisposition == CmResourceShareShared;

				if(m_InterruptObject != NULL)
				{
					IoDisconnectInterrupt(m_InterruptObject);
					m_InterruptObject = NULL;
				}
				IoConnectInterrupt(
					&m_InterruptObject,
					CAdapter::cbInterrupt,//PKSERVICE_ROUTINE (CCaptureDevice::Interrupt),
					this,
					NULL,
					int_Vector,
					(KIRQL)int_Level,
					(KIRQL)int_Level,
					int_Mode,
					int_Share ,
					int_Affinity,
					FALSE
				);
				break;

			default:
				KdPrint(( " - Unexpected I/O resource type %d\n", resource->Type));
				break;
		}					// switch on resource type
	}						// for each resource

	//DbgPrint("------------\n");
	PHYSICAL_ADDRESS padr = {0x0FFFFFFF, 0};
	if(m_pRISCMemory == NULL)
	{
		m_pRISCMemory = (BYTE*)MmAllocateContiguousMemory(RISC_TOTAL, padr);
		//DbgPrint("CreateMemory RISC : %X\n", m_pRISCMemory);
	}
	if(m_pSystemMemory == NULL)
	{
		m_pSystemMemory = (BYTE*)MmAllocateContiguousMemory(CAPTURE_TOTAL, padr);
		//DbgPrint("CreateMemory Capt : %X\n", m_pSystemMemory);
	}
	if(m_pRISCMemory == NULL || m_pSystemMemory == NULL)
	{
		return STATUS_ALLOTTED_SPACE_EXCEEDED;
	}


	// Create RISC Memory;
	m_dwRiscPhysical = MmGetPhysicalAddress(m_pRISCMemory).LowPart;

	m_pCaptureMemory[0] = m_pSystemMemory + CAPTURE_VIDEO_SIZE*0;
	m_pCaptureMemory[1] = m_pSystemMemory + CAPTURE_VIDEO_SIZE*1;
	m_pCaptureMemory[2] = m_pSystemMemory + CAPTURE_VIDEO_SIZE*2;
	m_pCaptureMemory[3] = m_pSystemMemory + CAPTURE_VIDEO_SIZE*3;
	m_pCaptureMemory[4] = m_pSystemMemory + CAPTURE_VIDEO_TOTAL + CAPTURE_AUDIO_SIZE*0;
	m_pCaptureMemory[5] = m_pSystemMemory + CAPTURE_VIDEO_TOTAL + CAPTURE_AUDIO_SIZE*1;
	m_pCaptureMemory[6] = m_pSystemMemory + CAPTURE_VIDEO_TOTAL + CAPTURE_AUDIO_SIZE*2;
	m_pCaptureMemory[7] = m_pSystemMemory + CAPTURE_VIDEO_TOTAL + CAPTURE_AUDIO_SIZE*3;

	for(int i=0; i<8; i++)
	{
		m_dwCapturePhysical[i] = MmGetPhysicalAddress(m_pCaptureMemory[i]).LowPart;
	}
	for(int i=4; i<8; i++)
	{
		for(int j=0; j<CAPTURE_AUDIO_SIZE; j++)
			m_pCaptureMemory[i][j] = 0x80;
	}
	IoSetDeviceInterfaceState( &UnicodeString, TRUE ); 
	// 사용자가 우리의 DeviceStack에 접근하는 것을 허용

	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::PnpStopDevice(PDEVICE_OBJECT DeviceObject)
{
	DbgPrint(("PnpStopDevice\n"));
	IoSetDeviceInterfaceState( &UnicodeString, FALSE );
	// 사용자가 우리의 DeviceStack에 접근하는 것을 금지

	return STATUS_SUCCESS;
}

typedef union {
	struct {
		DWORD	Int_Ch0			:1;
		DWORD	Int_Ch1			:1;
		DWORD	Int_Ch2			:1;
		DWORD	Int_Ch3			:1;
		DWORD	Int_Ch4			:1;
		DWORD	Int_Ch5			:1;
		DWORD	Int_Ch6			:1;
		DWORD	Int_Ch7			:1;
		DWORD	R0				:8;
		DWORD	Int_GPIO		:1;
		DWORD	R1				:1;
		DWORD	DMA_Err			:1;
		DWORD	R2				:5;
		DWORD	AUDField0		:1;
		DWORD	AUDField1		:1;
		DWORD	AUDField2		:1;
		DWORD	AUDField3		:1;
		DWORD	R3				:4;
	} ds;
	DWORD dw;
} INT_STATE;

typedef struct {
		DWORD	DMA		:1;
		DWORD	Op			:1;
}	ERR_Bit;

typedef union {
	struct {
		ERR_Bit	Ch[16]		;
	} ds;
	DWORD dw;
} ERR_STATE;


typedef union {
	struct {
		DWORD	MultiSel0		:2;
		DWORD	MultiSel1		:2;
		DWORD	MultiSel2		:2;
		DWORD	MultiSel3		:2;
		DWORD	MultiSel4		:2;
		DWORD	MultiSel5		:2;
		DWORD	MultiSel6		:2;
		DWORD	MultiSel7		:2;
		DWORD	Field0			:1;
		DWORD	Field1			:1;
		DWORD	Field2			:1;
		DWORD	Field3			:1;
		DWORD	Field4			:1;
		DWORD	Field5			:1;
		DWORD	Field6			:1;
		DWORD	Field7			:1;
		DWORD	Lock0			:1;
		DWORD	Lock1			:1;
		DWORD	Lock2			:1;
		DWORD	Lock3			:1;
		DWORD	Lock4			:1;
		DWORD	Lock5			:1;
		DWORD	Lock6			:1;
		DWORD	Lock7			:1;
	} ds;
	DWORD dw;
} CH_STATE;

BOOLEAN CAdapter::HwInterrupt()
{
   BOOLEAN fMyIRQ = FALSE; 
	INT_STATE IntState;
	CH_STATE  ChState;
	if (m_IRQExpected)
    {
		m_IRQExpected = false;
		IntState.dw = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + 0x1b0) );
		if ((IntState.dw&m_dwINTMaskBar) == 0)
		{
			if( (IntState.dw&0xFF) != 0 )
			{
				ChState.dw = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + 0x1b8) );
			}
			if(IntState.ds.Int_Ch0)
			{
				if(m_nFrameSkip[0] != 0)
				{
					m_nFrameSkip[0]--;
				}
				else
				{
					m_bField[0] = ChState.ds.Field0;
					KeInsertQueueDpc( &m_IsrDpc[0], (PVOID) (0), NULL );
				}
				fMyIRQ = TRUE; 
			}
			if(IntState.ds.Int_Ch1)
			{
				if(m_nFrameSkip[1] != 0)
				{
					m_nFrameSkip[1]--;
				}
				else
				{
					m_bField[1] = ChState.ds.Field1;
					KeInsertQueueDpc( &m_IsrDpc[1], (PVOID) (1), NULL );
				}
				fMyIRQ = TRUE; 
			}
			if(IntState.ds.Int_Ch2)
			{
				if(m_nFrameSkip[2] != 0)
				{
					m_nFrameSkip[2]--;
				}
				else
				{
					m_bField[2] = ChState.ds.Field2;
					KeInsertQueueDpc( &m_IsrDpc[2], (PVOID) (2), NULL );
				}
				fMyIRQ = TRUE; 
			}
			if(IntState.ds.Int_Ch3)
			{
				if(m_nFrameSkip[3] != 0)
				{
					m_nFrameSkip[3]--;
				}
				else
				{
					m_bField[3] = ChState.ds.Field3;
					KeInsertQueueDpc( &m_IsrDpc[3], (PVOID) (3), NULL );
				}
				fMyIRQ = TRUE; 
			}
			if(IntState.ds.Int_Ch4)
			{
				if(m_nFrameSkip[4] != 0)
				{
					m_nFrameSkip[4]--;
				}
				else
				{
					m_dwAudioField[0] = IntState.ds.AUDField0;
					KeInsertQueueDpc( &m_IsrDpc[4], (PVOID) (4), NULL );
				}
				fMyIRQ = TRUE; 
			}
			if(IntState.ds.Int_Ch5)
			{
				if(m_nFrameSkip[5] != 0)
				{
					m_nFrameSkip[5]--;
				}
				else
				{
					m_dwAudioField[1] = IntState.ds.AUDField1;
					KeInsertQueueDpc( &m_IsrDpc[5], (PVOID) (5), NULL );
				}
				fMyIRQ = TRUE; 
			}
			if(IntState.ds.Int_Ch6)
			{
				if(m_nFrameSkip[6] != 0)
				{
					m_nFrameSkip[6]--;
				}
				else
				{
					m_dwAudioField[2] = IntState.ds.AUDField2;
					KeInsertQueueDpc( &m_IsrDpc[6], (PVOID) (6), NULL );
				}
				fMyIRQ = TRUE; 
			}
			if(IntState.ds.Int_Ch7)
			{
				if(m_nFrameSkip[7] != 0)
				{
					m_nFrameSkip[7]--;
				}
				else
				{
					m_dwAudioField[3] = IntState.ds.AUDField3;
					KeInsertQueueDpc( &m_IsrDpc[7], (PVOID) (7), NULL );
				}
				fMyIRQ = TRUE; 
			}

			if(IntState.ds.DMA_Err)
			{
				m_dwErrorCode = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + 0x1b4) );
				KeInsertQueueDpc( &m_IsrDpcErr[0], NULL, NULL );
				DbgPrint("DMA Error : %X\n", m_dwErrorCode);
				fMyIRQ = TRUE; 
			}
		}
		else if(IntState.dw != 0xFFFFFFFF)
		{
			DbgPrint("  [%08X]  INT Error %08X-%8X\n", m_pBaseAddr, m_dwINTMaskBar, IntState.dw);
			m_dwErrorCode = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + 0x1b4) );
			KeInsertQueueDpc( &m_IsrDpcErr[0], NULL, NULL );
			fMyIRQ = TRUE; 
		}
		m_IRQExpected = true;
	}
	return fMyIRQ;
}

void CAdapter::cbDpcFunc(IN PKDPC Dpc,IN CAdapter *pAdapter, IN PVOID SystemArg1, IN PVOID SystemArg2)
{
	DWORD StreamNumber = (DWORD)SystemArg1;
	//DbgPrint("cbDpcFunc %x\n", StreamNumber);
	if(StreamNumber < 4)
	{
		if(pAdapter->m_bCaptureStart[StreamNumber])
			pAdapter -> VideoCaptureRoutine (StreamNumber);
	}
	else if(StreamNumber < 8)
	{
		if(pAdapter->m_bCaptureStart[StreamNumber])
			pAdapter -> AudioCaptureRoutine (StreamNumber);
	}
}

void CAdapter::cbDpcFuncErr0(IN PKDPC Dpc,IN CAdapter *pAdapter, IN PVOID SystemArg1, IN PVOID SystemArg2)
{
	DWORD		dwData;
	DWORD		ProgLocBase;
	ERR_STATE	ErrState;
	ErrState.dw = pAdapter -> m_dwErrorCode;
	
	DbgPrint("cbDpcFuncErr0 - Dec Off [%X] : %X\n", pAdapter, ErrState.dw);

	DWORD		dwBase[8] = {0x200, 0x280, 0x300, 0x380, 0x270, 0x2F0, 0x370, 0x3F0};

	int i;
	
	for(i=0; i<8; i++)
		KeRemoveQueueDpc(&(pAdapter->m_IsrDpc[i]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[0]));

	for(i=0; i<8; i++)
	{
		if( pAdapter->m_bCaptureStart[i] )
		{
			// Decoder Stop
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwBase[i]), pAdapter->m_dwDecDisable[i] );
		}
	}

	LARGE_INTEGER DueTime;
	DueTime.QuadPart = -100000L;
	KeSetTimer(
		&(pAdapter->DMATimer),
		DueTime,
		&(pAdapter->m_IsrDpcErr[1])
		);
}

void CAdapter::cbDpcFuncErr1(IN PKDPC Dpc,IN CAdapter *pAdapter, IN PVOID SystemArg1, IN PVOID SystemArg2)
{
	DWORD		dwData;
	DWORD		ProgLocBase;
	ERR_STATE	ErrState;
	ErrState.dw = pAdapter -> m_dwErrorCode;
	
	DbgPrint("cbDpcFuncErr0 - DMA Off [%X] : %X\n", pAdapter, ErrState.dw);

	DWORD		dwRisc[8] = { 0x60,  0x74,  0x88,  0x9C,  0xB0,  0xC4,  0xD8,  0xEC};

	int i;
	
	for(i=0; i<8; i++)
		KeRemoveQueueDpc(&(pAdapter->m_IsrDpc[i]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[0]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[1]));

	for(i=0; i<8; i++)
	{
		if( pAdapter->m_bCaptureStart[i] )
		{
			// Risc Stop
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i]), 0x000E8000 );
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i] + 0x04), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i] + 0x08), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i] + 0x0C), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i] + 0x10), 0x00000000 );
		}
	}

	LARGE_INTEGER DueTime;
	DueTime.QuadPart = -100000L;
	KeSetTimer(
		&(pAdapter->DMATimer),
		DueTime,
		&(pAdapter->m_IsrDpcErr[2])
		);
}

void CAdapter::cbDpcFuncErr2(IN PKDPC Dpc,IN CAdapter *pAdapter, IN PVOID SystemArg1, IN PVOID SystemArg2)
{
	DWORD		dwData;
	DWORD		ProgLocBase;
	ERR_STATE	ErrState;
	ErrState.dw = pAdapter -> m_dwErrorCode;
	DbgPrint("cbDpcFuncErr0 - DMA On [%X] : %X\n", pAdapter, ErrState.dw);
	
	DWORD		dwRisc[8] = { 0x60,  0x74,  0x88,  0x9C,  0xB0,  0xC4,  0xD8,  0xEC};

	int i;

	for(i=0; i<8; i++)
		KeRemoveQueueDpc(&(pAdapter->m_IsrDpc[i]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[0]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[1]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[2]));

	for(i=0; i<8; i++)
	{
		if(pAdapter->m_bCaptureStart[i])
		{
			DWORD		ProgLocBase = pAdapter->m_dwRiscPhysical + RISC_VIDEO_SIZE*i;
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i] + 0x04), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i] + 0x08), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i] + 0x0C), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i] + 0x10), ProgLocBase );
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwRisc[i]), 0x000E8001 );
		}
	}

	LARGE_INTEGER DueTime;
	DueTime.QuadPart = -100000L;
	KeSetTimer(
		&(pAdapter->DMATimer),
		DueTime,
		&(pAdapter->m_IsrDpcErr[3])
		);
}



void CAdapter::cbDpcFuncErr3(IN PKDPC Dpc,IN CAdapter *pAdapter, IN PVOID SystemArg1, IN PVOID SystemArg2)
{
	DWORD		dwData;
	DWORD		ProgLocBase;
	ERR_STATE	ErrState;
	ErrState.dw = pAdapter -> m_dwErrorCode;
	DbgPrint("cbDpcFuncErr0 - Dec On [%X] : %X\n", pAdapter, ErrState.dw);
	
	DWORD		dwBase[8] = {0x200, 0x280, 0x300, 0x380, 0x270, 0x2F0, 0x370, 0x3F0};

	int i;

	for(i=0; i<8; i++)
		KeRemoveQueueDpc(&(pAdapter->m_IsrDpc[i]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[0]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[1]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[2]));
	KeRemoveQueueDpc(&(pAdapter->m_IsrDpcErr[3]));

	for(i=0; i<8; i++)
	{
		if( pAdapter->m_bCaptureStart[i] )
		{
			// Decoder Start
			pAdapter->m_nFrameSkip[i] = 3;
			WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + dwBase[i]), pAdapter->m_dwDecEnable[i] );
		}
	}
	//WRITE_REGISTER_DPGPRINT( PULONG(pAdapter->m_pBaseAddr + 0x1FC), ErrState.dw );
}

void	CAdapter::VideoCaptureRoutine(DWORD dwStreamNumber)
{
	//if(dwStreamNumber ==0)
	DWORD dwBase[4] = {0x200, 0x280, 0x300, 0x380};
	DWORD dwMulti[4] = {0x98000020, 0xB8000020, 0xD8000020, 0xF8000020};

	DWORD dwRead;
	DWORD dwRange;
	int nMux;

	dwRead = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + 0x1B8) );
	// Decoder Mux read
	nMux = ((dwRead >> (dwStreamNumber*2))&0x3);
	m_byLock[dwStreamNumber][nMux] <<= 1;
	m_byLock[dwStreamNumber][nMux] |= ((dwRead>>(24+dwStreamNumber))&1);
	//DbgPrint("Lock %d-%d-%X\n", dwStreamNumber, nMux, m_byLock[dwStreamNumber][nMux]);


	if(m_nSplit[dwStreamNumber] != 0)
	{
		int nMuxNext;
		nMuxNext = nMux+1;
		if(nMuxNext > m_nSplit[dwStreamNumber])
			nMuxNext = 0;


		// Range Read
		dwRead = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + dwBase[dwStreamNumber] + 0x040) );
		dwRange = (dwRead >> 8) & 0xFF;

		if(nMux == m_nVROMaster[dwStreamNumber] && m_byLock[dwStreamNumber][nMux] != 0xFF)	//VRO Master Change
		{
			PULONG		pProgLoc;
			pProgLoc = m_pRiscVROPos[dwStreamNumber][m_nVROMaster[dwStreamNumber]];
			RISC_WRITE(pProgLoc, m_dwDumyMemory[dwStreamNumber], 32, 1,1,0,0, 0);

			m_nVROMaster[dwStreamNumber] = nMuxNext;

			pProgLoc = m_pRiscVROPos[dwStreamNumber][m_nVROMaster[dwStreamNumber]];
			RISC_SYNC(pProgLoc, SC_VRO, 0, 0);

			//DbgPrint("MasterChange %d-%d\n\n\n", dwStreamNumber, m_nVROMaster[dwStreamNumber]);
		}


		// 1. Dec Off;
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[dwStreamNumber] + 0x00), m_dwDecDisable[dwStreamNumber]);

		// 2. Mux Sel;
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[dwStreamNumber] + 0x1C), dwMulti[nMuxNext]);

		// 3. AGC Set;
		switch(dwStreamNumber)
		{
		case 0:
			m_dwADCAGC &= 0xE1FFFFFF;
			m_dwADCAGC |= DWORD(m_nAGCData[0][nMuxNext])<<25;
			break;
		case 1:
			m_dwADCAGC &= 0xFFE1FFFF;
			m_dwADCAGC |= DWORD(m_nAGCData[1][nMuxNext])<<17;
			break;
		case 2:
			m_dwADCAGC &= 0xFFFFE1FF;
			m_dwADCAGC |= DWORD(m_nAGCData[2][nMuxNext])<<9;
			break;
		case 3:
			m_dwADCAGC &= 0xFFFFFFE1;
			m_dwADCAGC |= DWORD(m_nAGCData[3][nMuxNext])<<1;
			break;
		}
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + 0x1A0), m_dwADCAGC );

		// 4. Dec On;
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[dwStreamNumber] + 0x00), m_dwDecEnable[dwStreamNumber]);


		// AGC Compare

		//DbgPrint("Ranage %X\n", dwRange);

		m_dwAGCChange[dwStreamNumber][nMux] <<= 2;
		if(m_byLock[dwStreamNumber][nMux] == 0xFF)
		{
			if(dwRange > 0xF8 && m_nAGCData[dwStreamNumber][nMux] != 0)
			{
				m_dwAGCChange[dwStreamNumber][nMux] |= 3;

				if(m_dwAGCChange[dwStreamNumber][nMux] == 0xFFFFFFFF)
				{
					m_nAGCData[dwStreamNumber][nMux]--;
					m_dwAGCChange[dwStreamNumber][nMux] = 0;
				}
			}
			else if(dwRange < 0xC8 && m_nAGCData[dwStreamNumber][nMux] != 0xF)
			{
				m_dwAGCChange[dwStreamNumber][nMux] |= 1;
				if(m_dwAGCChange[dwStreamNumber][nMux] == 0x55555555)
				{
					m_nAGCData[dwStreamNumber][nMux]++;
					m_dwAGCChange[dwStreamNumber][nMux] = 0;
				}
			}
		}
		

		if(nMux != 0)
		{
			BYTE*	pSrc = m_pDstAddr[dwStreamNumber][0];
			BYTE*	pDst = m_pDstAddr[dwStreamNumber][nMux];
			for(int i=0; i<m_nHeight[dwStreamNumber]; i++)
			{
				RtlCopyMemory(pDst, pSrc, m_dwLinePitch[dwStreamNumber]);
				pSrc += m_dwLinePitch[dwStreamNumber]*2;
				pDst += m_dwLinePitch[dwStreamNumber]*2;
			}
		}
		//DbgPrint("VideoAGC %d - Mux %d - Rnage %2X - Change %2X - Data %d\n", dwStreamNumber, nMux, dwRange, m_dwAGCChange[dwStreamNumber][nMux], m_nAGCData[dwStreamNumber][nMux]);
	}

	if( m_nSplit[dwStreamNumber] == 0 || nMux == 0)
	{
		//DbgPrint("VideoCapture %d - %d\n\n\n", dwStreamNumber, m_bField[dwStreamNumber]);
		KeSetEvent(m_pEvent[dwStreamNumber], 0, FALSE);
	}
}

void	CAdapter::AudioCaptureRoutine(DWORD dwStreamNumber)
{
	DWORD	dwAudioCh = dwStreamNumber-4;
	if(!m_dwAudioField[dwAudioCh])
	{
		//DbgPrint("Audio%02d Interrupt 1\n",dwStreamNumber);
		RtlCopyMemory(m_pAudioData[dwAudioCh], m_pCaptureMemory[dwStreamNumber]+AUDIO_BUFFER_SIZE, AUDIO_BUFFER_SIZE);
	}
	else
	{
		//DbgPrint("Audio%02d Interrupt 0\n",dwStreamNumber);
		RtlCopyMemory(m_pAudioData[dwAudioCh], m_pCaptureMemory[dwStreamNumber], AUDIO_BUFFER_SIZE);
	}
		
	//DbgPrint("Audio Data : %02x\n", *m_pCaptureMemory[dwStreamNumber]);
	//DbgPrint("Ch%02d Audio Data Out: %02x\n", dwStreamNumber, *m_pCaptureMemory[dwStreamNumber]);
	KeSetEvent(m_pEvent[dwStreamNumber], 0, FALSE);
}

NTSTATUS CAdapter::RegisterWrite(PMV9205_RW pData)
{
	if(pData->dwAddr+pData->nCounter*4 >= 4096)
		return STATUS_INVALID_PARAMETER;

	for(int i=0; i<pData->nCounter; i++)
	{
		if((pData->dwAddr + (DWORD)i*4) == 0x1c4)
		{
			SetGPIO(&(pData->pData[i]));
		}
		else if((pData->dwAddr + (DWORD)i*4) == 0x1c8)
		{
			SetGPOE(&(pData->pData[i]));
		}
		else
			WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + pData->dwAddr + (DWORD)i*4), pData->pData[i]);
	}
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::RegisterRead(PMV9205_RW pData)
{
	if(pData->dwAddr+pData->nCounter*4 >= 4096)
		return STATUS_INVALID_PARAMETER;

	for(int i=0; i<pData->nCounter; i++)
		pData->pData[i] = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + pData->dwAddr + (DWORD)i*4) );
	return STATUS_SUCCESS;
}

void CAdapter::CreateVideoRiscCmdNormal(BOOL bInterlace, int cx, int cy, BOOL bSkip, PULONG *pProgLocation, DWORD dwTAddr)
{
	PULONG pProgLoc = *pProgLocation;

	if(bSkip)
	{
		{RISC_SYNC(pProgLoc, SC_VRO, 0, 0);}
	}
	else
	{
		if(bInterlace)
		{
			// SYNC VRO
			RISC_SYNC(pProgLoc, SC_VRO, 0, 0);

			// SYNC FM1
			RISC_SYNC(pProgLoc, SC_FM1, 0, 0);

			// Image Data
			for(int i=0; i<cy/2; i++)
			{
				RISC_WRITE(pProgLoc, dwTAddr + cx*2*i, cx/4, 1,1,0,0, 0);
			}

			// SYNC FM1
			RISC_SYNC(pProgLoc, SC_FM1, 0, 0);

			// Image Data
			for(int i=0; i<cy/2-1; i++)
			{
				RISC_WRITE(pProgLoc, dwTAddr +cx +cx*2*i, cx/4, 1,1,0,0, 0);
			}

			// Interrupt out
			RISC_WRITE(pProgLoc, dwTAddr + (cy-1)*cx, cx/4, 1,1,1,0, 0);
		}
		else
		{	// Non Interlace
			RISC_SYNC(pProgLoc, SC_VRO, 0, 0);

			RISC_SYNC(pProgLoc, SC_FM1, 0, 0);

			if(cy>288)
			{
				for(int i=0; i<cy/2; i++)
				{
					RISC_WRITE(pProgLoc, dwTAddr+cx*i, cx/4, 1,1,0,0, 0);
				}
				RISC_SYNC(pProgLoc, SC_FM1, 0, 0);
				for(int i=0; i<cy/2-1; i++)
				{
					RISC_WRITE(pProgLoc, dwTAddr+cx*(i+cy/2), cx/4, 1,1,0,0, 0);
				}
			}
			else
			{
				for(int i=0; i<cy-1; i++)
				{
					RISC_WRITE(pProgLoc, dwTAddr+cx*i, cx/4, 1,1,0,0, 0);
				}
			}
			RISC_WRITE(pProgLoc, dwTAddr + (cy-1)*cx, cx/4, 1,1,1,0, 0);
		}
	}
	*pProgLocation = pProgLoc;
}

//void CAdapter::CreateVideoRiscCmd(BOOL bInterlace, int cx, int cy, BOOL bSkip, PULONG *pProgLocation, DWORD dwTAddr, int nSplit)
void CAdapter::CreateVideoRiscCmdSplit(int cx, int cy, PULONG *pProgLocation, DWORD dwTAddr, int nSplit, PULONG pRiscVROPos[][4], DWORD *dwDumyMem)
{
	PULONG pProgLoc = *pProgLocation;

	*dwDumyMem = dwTAddr + cx*2*cy*2;
	if(nSplit >= 1)
	{
		// Dumy write
		for(int i=0; i<120; i++)
		{
			RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,0,0, 0);
		}

		// Channel 0 write
		pRiscVROPos[0][0] = pProgLoc;
		RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,0,0, 0);

		RISC_SYNC(pProgLoc, SC_FM1, 0, 0);
		for(int i=0; i<cy; i++)
		{
			RISC_WRITE(pProgLoc, dwTAddr + cx*2*i, cx/4, 1,1,0,0, 0);
		}
		RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,1,0, 0);

		// Dumy write
		for(int i=0; i<120; i++)
		{
			RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,0,0, 0);
		}
		// Channel 1 write
		pRiscVROPos[0][1] = pProgLoc;
		RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,0,0, 0);

		RISC_SYNC(pProgLoc, SC_FM1, 0, 0);
		for(int i=0; i<cy; i++)
		{
			RISC_WRITE(pProgLoc, dwTAddr + cx*2*i, cx/4, 1,1,0,0, 0);
		}
		RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,1,0, 0);
	}

	if(nSplit >= 2)
	{
		// Dumy write
		for(int i=0; i<120; i++)
		{
			RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,0,0, 0);
		}
		
		// Channel 2 write
		pRiscVROPos[0][2] = pProgLoc;
		RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,0,0, 0);

		RISC_SYNC(pProgLoc, SC_FM1, 0, 0);
		for(int i=0; i<cy; i++)
		{
			RISC_WRITE(pProgLoc, dwTAddr + cx*2*i, cx/4, 1,1,0,0, 0);
		}
		RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,1,0, 0);
	}

	if(nSplit == 3)
	{
		// Dumy write
		for(int i=0; i<120; i++)
		{
			RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,0,0, 0);
		}
		
		// Channel 3 write
		pRiscVROPos[0][3] = pProgLoc;
		RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,0,0, 0);

		RISC_SYNC(pProgLoc, SC_FM1, 0, 0);
		for(int i=0; i<cy; i++)
		{
			RISC_WRITE(pProgLoc, dwTAddr + cx*2*i, cx/4, 1,1,0,0, 0);
		}
		RISC_WRITE(pProgLoc, *dwDumyMem, 32, 1,1,1,0, 0);
	}
	*pProgLocation = pProgLoc;
}

NTSTATUS CAdapter::SetVideoFormat(PMV9205_FORMAT pFormat)
{
	int i = pFormat->nChannel;
	int j;

	//1. DEC off
	DWORD dwBase[4] = {0x200, 0x280, 0x300, 0x380};

	m_dwADCAGC = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + 0x1A0) );

	m_bCaptureStart[i] = FALSE;

	m_dwINTMaskBar |= (1<<i);

	m_dwDecEnable[i] = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + dwBase[i]) );
	m_dwDecEnable[i] &= 0x1CFF8FFF;	//Decoder Enable is Bit [31] , AGC Off is Bit[24], VideoFormat is Bit [13:11] in Register 200
	m_dwDecEnable[i] |= 0x80000000;	//Decoder Enable
	if(pFormat->bStandard)
		m_dwDecEnable[i] |= 0x40000000;// PAL
	if(!pFormat->nSplit)
	{
		m_dwDecEnable[i] |= 0x01000000;// And AGC On
	}
	m_dwDecEnable[i] |= ( (pFormat->nFormat & 0x07)<<12 ); //0:YUV2, 1:BTYUV, 2,Y8, 3:YUV420
	m_dwDecDisable[i] = m_dwDecEnable[i] & 0x7FFFFFFF;

	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i]), m_dwDecDisable[i] );


	//2. 10ms Delay
	Wait10ms();

	//3. DMA off
	DWORD	dwRisc[4] = {0x60, 0x74, 0x88, 0x9C};

	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i]), 0x000E8000 );
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x04), 0x00000000 );
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x08), 0x00000000 );
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x0C), 0x00000000 );
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x10), 0x00000000 );


	//4. Format Change

	m_nSplit[i] = pFormat->nSplit;
	if(!pFormat->bEnable)
		goto END;
	for(j=0; j<4; j++)
	{
		if(pFormat->nSplit)
			m_nAGCData[i][j] = 4;
		else
			m_nAGCData[i][j] = 2;

		m_dwAGCChange[i][j] = 0;
	}
	switch(i)
	{
	case 0:
		m_dwADCAGC &= 0xE1FFFFFF;
		m_dwADCAGC |= DWORD(m_nAGCData[0][0])<<25;
		break;
	case 1:
		m_dwADCAGC &= 0xFFE1FFFF;
		m_dwADCAGC |= DWORD(m_nAGCData[1][0])<<17;
		break;
	case 2:
		m_dwADCAGC &= 0xFFFFE1FF;
		m_dwADCAGC |= DWORD(m_nAGCData[2][0])<<9;
		break;
	case 3:
		m_dwADCAGC &= 0xFFFFFFE1;
		m_dwADCAGC |= DWORD(m_nAGCData[3][0])<<1;
		break;
	}
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + 0x1A0), m_dwADCAGC );

	//Memory allocation check
	if(m_pCaptureUser[i] == NULL)
	{
		m_pCaptureMdl[i] = IoAllocateMdl(m_pCaptureMemory[i], CAPTURE_VIDEO_SIZE, 0, 0, 0 );
		MmBuildMdlForNonPagedPool( m_pCaptureMdl[i] );

		__try
		{
			m_pCaptureUser[i] = (BYTE*)MmMapLockedPagesSpecifyCache(m_pCaptureMdl[i],	UserMode, MmWriteCombined, NULL, FALSE, HighPagePriority);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			m_pCaptureUser[i] = NULL;
			return STATUS_NO_MEMORY;
		}
	}

	//Line Pitch
	switch(pFormat->nFormat)
	{
	case VIDEOFORMAT_YUY2:
		m_dwLinePitch[i] = pFormat->nWidth * 2;
		break;
	case VIDEOFORMAT_YUV420:
	case VIDEOFORMAT_YUV411:
		m_dwLinePitch[i] = pFormat->nWidth * 12 /8;
		break;
	case VIDEOFORMAT_Y8:
		m_dwLinePitch[i] = pFormat->nWidth;
		break;
	}

	//FramePerSec check
	if(pFormat->nFrame < 100)
	{
		if(pFormat->nFrame < 1)
			pFormat->nFrame = 1;
		if(pFormat->nFrame > 30)
			pFormat->nFrame = 30;
	}
	else
	{
		if(pFormat->nFrame < 101)
			pFormat->nFrame = 101;
		if(pFormat->nFrame > 125)
			pFormat->nFrame = 125;
	}

	// Horizontal Scaling
	DWORD				dwReg10;
	dwReg10 = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + dwBase[i] + 0x010) );
	dwReg10 &= 0xFFC00000;
	dwReg10 |= pFormat->nWidth;			//Even Field Image width Size
	dwReg10 |= (pFormat->nWidth << 11);	//Odd  Field Image width Size
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i] + 0x010), dwReg10 );

	// Vertical Scaling
	DWORD				dwReg14;
	dwReg14 = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + dwBase[i] + 0x014) );
	dwReg14 &= 0xFFC00000;
	if(pFormat->bInterlace)
	{
		dwReg14 |= pFormat->nHeight/2;
		dwReg14 |= ((pFormat->nHeight/2) << 11);
	}
	else
	{
		dwReg14 |= pFormat->nHeight;
		dwReg14 |= (pFormat->nHeight << 11);
	}
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i] + 0x014), dwReg14 );

	//Split Setting
	DWORD				dwReg1C;
	if(pFormat->nSplit)
	{
		dwReg1C = 0x90000300;
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i] + 0x004), 0x600A3000 );

		m_pDstAddr[i][0] = m_pCaptureMemory[i] + 0;
		m_pDstAddr[i][1] = m_pCaptureMemory[i] + m_dwLinePitch[i];
		m_pDstAddr[i][2] = m_pCaptureMemory[i] + pFormat->nHeight*m_dwLinePitch[i]*2;
		m_pDstAddr[i][3] = m_pCaptureMemory[i] + pFormat->nHeight*m_dwLinePitch[i]*2 + m_dwLinePitch[i];

		m_nHeight[i] = pFormat->nHeight;
	}
	else
	{
		dwReg1C = 0x00000300;
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i] + 0x004), 0xA00A3000 );
	}
	dwReg1C |= (DWORD(pFormat->nMux)<<29);
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i] + 0x01C), dwReg1C );

	////////////////////////////////////////////////////////////////////////
	// RISC program setting
	DWORD		ProgLocBase = m_dwRiscPhysical + RISC_VIDEO_SIZE*i;
	PULONG		pProgLoc    = PULONG(m_pRISCMemory + RISC_VIDEO_SIZE*i);
	PULONG		pProgLocBase = PULONG(m_pRISCMemory + RISC_VIDEO_SIZE*i);


	if(pFormat->nSplit)
	{
		// Initial Data mem
		DWORD* pTmp = (DWORD*)m_pCaptureMemory[i];
		DWORD	dwDataSize;
		dwDataSize = pFormat->nHeight * m_dwLinePitch[i];
		for(DWORD dwI=0; dwI<dwDataSize; dwI++)
			*pTmp++ = 0x80808080;

		// Split Risc control setting
		m_nVROMaster[i] = 0;
		m_byLock[i][0] = 0xFF;
		m_byLock[i][1] = 0xFF;
		m_byLock[i][2] = 0xFF;
		m_byLock[i][3] = 0xFF;
		m_pRiscVROPos[i][0] = 0;
		m_pRiscVROPos[i][1] = 0;
		m_pRiscVROPos[i][2] = 0;
		m_pRiscVROPos[i][3] = 0;
	}

	// Initial RISC mem
	for(DWORD dwI=0; dwI<RISC_VIDEO_SIZE/8; dwI++)
	{
		RISC_JUMP(pProgLoc, ProgLocBase, 0);
	}
	pProgLoc    = PULONG(m_pRISCMemory + RISC_VIDEO_SIZE*i);

	if(pFormat->nSplit == 0)
	{
		if(pFormat->bStandard)
		{
			if(pFormat->nFrame < 1)
				pFormat->nFrame = 1;
			else if(pFormat->nFrame > 25)
				pFormat->nFrame = 25;

			char *pCh;
			pCh = FrameRate_PAL1[pFormat->nFrame];
			for(j=0; j<25; j++)
			{
				int nFramecheck = j+i;
				if(nFramecheck>25)
					nFramecheck = 0;
				if(pCh[nFramecheck] == '0')
				{	// Skip
					CreateVideoRiscCmdNormal(pFormat->bInterlace, m_dwLinePitch[i], pFormat->nHeight, TRUE, &pProgLoc, m_dwCapturePhysical[i]);
				}
				else
				{
					CreateVideoRiscCmdNormal(pFormat->bInterlace, m_dwLinePitch[i], pFormat->nHeight, FALSE, &pProgLoc, m_dwCapturePhysical[i]);
				}
			}
		}
		else
		{	// NTSC
			if(pFormat->nFrame < 1)
				pFormat->nFrame = 1;
			else if(pFormat->nFrame > 30)
				pFormat->nFrame = 30;

			char *pCh;
			pCh = FrameRate_NTSC1[pFormat->nFrame];
			for(j=0; j<30; j++)
			{
				int nFramecheck = j+i;
				if(nFramecheck>30)
					nFramecheck = 0;
				if(pCh[nFramecheck] == '0')
				{	// Skip
					CreateVideoRiscCmdNormal(pFormat->bInterlace, m_dwLinePitch[i], pFormat->nHeight, TRUE, &pProgLoc, m_dwCapturePhysical[i]);
				}
				else
				{
					CreateVideoRiscCmdNormal(pFormat->bInterlace, m_dwLinePitch[i], pFormat->nHeight, FALSE, &pProgLoc, m_dwCapturePhysical[i]);
				}
			}
		}
		RISC_JUMP(pProgLoc, ProgLocBase, 0);
	}
	else // Split
	{
		CreateVideoRiscCmdSplit(m_dwLinePitch[i], pFormat->nHeight, &pProgLoc, m_dwCapturePhysical[i], pFormat->nSplit, &m_pRiscVROPos[i], &m_dwDumyMemory[i]);
		RISC_JUMP(pProgLoc, ProgLocBase, 0);

		//DbgPrint(" DWORD Pos0 = %X\n DWORD Pos1 = %X\n DWORD Pos2 = %X\n DWORD Pos3 = %X\n", m_pRiscVROPos[i][0], m_pRiscVROPos[i][1], m_pRiscVROPos[i][2], m_pRiscVROPos[i][3]);
		// Default Channel make;
		pProgLoc = m_pRiscVROPos[i][0];
		RISC_SYNC(pProgLoc, SC_VRO, 0, 0);
		//RISC_WRITE(pProgLoc, m_dwDumyMemory[i], 32, 1,1,0,0, 0);
	}
END:

	// 5. DMA On

	if(pFormat->bEnable)
	{
		DWORD		ProgLocBase = m_dwRiscPhysical + RISC_VIDEO_SIZE*i;
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x04), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x08), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x0C), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x10), ProgLocBase );
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i]), 0x000E8001 );
	}

	// 6. Decoder Start

	if(pFormat->bEnable)
	{
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i] + 0x000), m_dwDecEnable[i] );
		m_bCaptureStart[i] = TRUE;
		m_dwINTMaskBar &= ~(1<<i);
	}
		
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::SetVideoDisplay(PMV9205_DISPLAY pDisp)
{
	DWORD dwChBaseAddr = (DWORD)m_pBaseAddr + 0x200 + pDisp->nChannel*0x80;
	DWORD dwData = ((DWORD)pDisp->byBright<<24) + ((DWORD)pDisp->byContrast<<16) + ((DWORD)pDisp->bySaturation<<8) + ((DWORD)pDisp->byHue);
	WRITE_REGISTER_DPGPRINT( PULONG(dwChBaseAddr + 0xC), dwData );
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::SetEvent(PMV9205_EVENT pEv, KPROCESSOR_MODE RequestMode)
{
	NTSTATUS	status;

	if( m_pEvent[pEv->nChannel] != NULL)
	{
		ObDereferenceObject(m_pEvent[pEv->nChannel]);
		m_pEvent[pEv->nChannel] = NULL;
	}

	status = ObReferenceObjectByHandle(pEv->hEvent, EVENT_MODIFY_STATE,
				*ExEventObjectType, RequestMode, (PVOID*) &m_pEvent[pEv->nChannel], NULL);
	if(status != STATUS_SUCCESS)
	{
		if( m_pEvent[pEv->nChannel] != NULL)
		{
			ObDereferenceObject(m_pEvent[pEv->nChannel]);
			m_pEvent[pEv->nChannel] = NULL;
		}
	}
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::GetVideoMemory(PMV9205_VIDEOMEM pVideoMem)
{
	//Memory allocation check
	if(m_pCaptureUser[pVideoMem->nChannel] == NULL)
	{
		// Mdl을 준비한다
		if(pVideoMem->nChannel < 4)
		{	//Video
			m_pCaptureMdl[pVideoMem->nChannel] = IoAllocateMdl( 
				m_pCaptureMemory[pVideoMem->nChannel], 
				CAPTURE_VIDEO_SIZE, 
				0, 0, 0 );
		}
		else
		{	//Audio
			if(m_pAudioData[pVideoMem->nChannel-4] == NULL)
				m_pAudioData[pVideoMem->nChannel-4] = (BYTE*)ExAllocatePoolWithTag(NonPagedPool, AUDIO_BUFFER_SIZE,'ISmD'); //  DmSI

			m_pCaptureMdl[pVideoMem->nChannel] = IoAllocateMdl( 
				m_pAudioData[pVideoMem->nChannel-4], 
				AUDIO_BUFFER_SIZE, 
				0, 0, 0 );
		}
		// Mdl을 완성시킨다
		MmBuildMdlForNonPagedPool( m_pCaptureMdl[pVideoMem->nChannel] );

		// 사용자가 볼 수 있는 가상메모리를 매핑시킨다
		__try
		{
			m_pCaptureUser[pVideoMem->nChannel] = (BYTE*)MmMapLockedPagesSpecifyCache(m_pCaptureMdl[pVideoMem->nChannel],	UserMode, MmWriteCombined, NULL, FALSE, HighPagePriority);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			m_pCaptureUser[pVideoMem->nChannel] = NULL;
			return STATUS_NO_MEMORY;
		}
	}

	pVideoMem->pMem = m_pCaptureUser[pVideoMem->nChannel];
	pVideoMem->dwSize = CAPTURE_VIDEO_SIZE;
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::SetAudioFormat(PMV9205_AUDIOFM pAud)
{
	if(CAPTURE_AUDIO_SIZE/2 < 4096)
		return STATUS_INVALID_PARAMETER;

	int i, j;
	DWORD dwBase[4] = {0x270, 0x2F0, 0x370, 0x3F0};
	DWORD dwRisc[4] = {0xB0, 0xC4, 0xD8, 0xEC};

	//1. Dec Off
	for(i=0; i<4; i++)
	{
		m_bCaptureStart[i+4] = FALSE;
		m_dwINTMaskBar |= (1<<(i+4));
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i]), m_dwDecDisable[i] );
	}
	//DbgPrint(("SetAudioFormat - Dec Off\n"));

	//2. 10ms Delay
	Wait10ms();

	//3. DMA off
	for(i=0; i<4; i++)
	{
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i]), 0x000E8000 );
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x04), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x08), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x0C), 0x00000000 );
		WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x10), 0x00000000 );
	}
	//DbgPrint(("SetAudioFormat - DMA Off\n"));

	//4. Audio Data Memory Initial
	for(i=0; i<4; i++)
	{
		//DbgPrint(("SetAudioFormat - Memory Check - start\n"));
		//DbgPrint("SetAudioFormat - Memory Check - %p\n",m_pAudioData[i]);
		// Audio Memory Check
		if(m_pAudioData[i] == NULL)
			return STATUS_NO_MEMORY;
		//DbgPrint(("SetAudioFormat - Memory Check - end\n"));


		// Data Initial
		for(j=0; j<AUDIO_BUFFER_SIZE; j++)
		{
			m_pAudioData[i][j] = 0x80;
		}
		//DbgPrint(("SetAudioFormat - Data Initial\n"));

		if(m_pCaptureUser[i+4] == NULL)
		{
			// Mdl을 준비한다
			m_pCaptureMdl[i+4] = IoAllocateMdl( 
				m_pAudioData[i], 
				AUDIO_BUFFER_SIZE, 
				0, 0, 0 );

			// Mdl을 완성시킨다
			MmBuildMdlForNonPagedPool( m_pCaptureMdl[i+4] );

			__try
			{
				m_pCaptureUser[i+4] = (BYTE*)MmMapLockedPagesSpecifyCache(m_pCaptureMdl[i+4],	UserMode, MmWriteCombined , NULL, FALSE, HighPagePriority);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				m_pCaptureUser[i+4] = NULL;
				return STATUS_NO_MEMORY;
			}
		}
		//DbgPrint(("SetAudioFormat - MDL\n"));

		DWORD				ProgLocBase;
		PULONG				pProgLoc;

		ProgLocBase = m_dwRiscPhysical + RISC_VIDEO_TOTAL + RISC_AUDIO_SIZE*(i);
		pProgLoc = PULONG(m_pRISCMemory + RISC_VIDEO_TOTAL + RISC_AUDIO_SIZE*(i));

		// Create RISC Instruction
		//0~3
		RISC_SYNC(pProgLoc, SC_VRO, 0, 0);
		RISC_WRITE(pProgLoc, m_dwCapturePhysical[i+4]                     ,AUDIO_BUFFER_SIZE/2/4 ,1,1,1,0, 0);
		RISC_WRITE(pProgLoc, m_dwCapturePhysical[i+4]+AUDIO_BUFFER_SIZE/2 ,AUDIO_BUFFER_SIZE/2/4 ,1,1,0,0, 0);
		RISC_JUMP(pProgLoc, ProgLocBase+8*4, 0);

		//8~11
		RISC_SYNC(pProgLoc, SC_VRE, 0, 0);
		RISC_WRITE(pProgLoc, m_dwCapturePhysical[i+4]+AUDIO_BUFFER_SIZE                    ,AUDIO_BUFFER_SIZE/2/4 ,1,1,1,0, 0);
		RISC_WRITE(pProgLoc, m_dwCapturePhysical[i+4]+AUDIO_BUFFER_SIZE+AUDIO_BUFFER_SIZE/2,AUDIO_BUFFER_SIZE/2/4 ,1,1,0,0, 0);

		RISC_JUMP(pProgLoc, ProgLocBase, 0);
		//DbgPrint(("SetAudioFormat - RISC Create\n"));
	}



	// 5. DMA On
	for(i=0; i<4; i++)
	{
		if(pAud->bEnable[i])
		{
			DWORD	ProgLocBase = m_dwRiscPhysical + RISC_VIDEO_TOTAL + RISC_AUDIO_SIZE*(i);
			WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x04), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x08), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x0C), 0x00000000 );
			WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i] + 0x10), ProgLocBase );
			WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwRisc[i]), 0x000E8001 );
		}
	}
	//DbgPrint(("SetAudioFormat - DMA on\n"));

	// 6. Wait 10ms
	//Wait10ms();

	// 7. Decoder Start
	for(i=0; i<4; i++)
	{
		if(pAud->bEnable[i])
		{
			m_dwDecEnable[i+4] = 0x80000000;

			//addr 0x54: write 0x0D00021C
			//ClkScale = Freq / MainClk(100Khz) *AUDIO_BUFFER_SIZE
			//DWORD	dwDst = ((pAud->dwFreq) * AUDIO_BUFFER_SIZE + 1000) / 100000;
			//ClkScale = Freq / MainClk(400Khz) *AUDIO_BUFFER_SIZE
			
			DWORD	dwDst = ((pAud->dwFreq) * 4096 + 500) / 400000;
			m_dwDecEnable[i+4] |= (dwDst&0xFFF);

			// Line is 2
			m_dwDecEnable[i+4] |= 0x04000000;

			// Audio Mode and Buffer size
			m_dwDecEnable[i+4] |= (pAud->nFormat&0x3) << 29;

			switch(pAud->nFormat)
			{
			case AUDIOFORMAT_8MONO:
				m_dwDecEnable[i+4] |= (AUDIO_BUFFER_SIZE/2) << 12;	//data size : 8bit
				break;
			case AUDIOFORMAT_8STEREO:
				m_dwDecEnable[i+4] |= (AUDIO_BUFFER_SIZE/2/2) << 12;	//data size : 16bit
				break;
			case AUDIOFORMAT_16MONO:
				m_dwDecEnable[i+4] |= (AUDIO_BUFFER_SIZE/2/2) << 12;	//data size : 16bit
				break;
			case AUDIOFORMAT_16STEREO:
				m_dwDecEnable[i+4] |= (AUDIO_BUFFER_SIZE/4/2) << 12;	//data size : 32bit
				break;
			}
			m_dwDecDisable[i] = m_dwDecEnable[i+4] & 0x7FFFFFFF;
			WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + dwBase[i]), m_dwDecEnable[i+4] );

			m_dwINTMaskBar &= ~(1<<(i+4));

			m_bCaptureStart[i+4] = TRUE;
		}
	}
	//DbgPrint(("SetAudioFormat - Dec on\n"));

	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::GetVideoLock(DWORD *pLock)
{
	//*pLock = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + 0x1AC) );

	*pLock = 0;
	for(int i=3; i>=0; i--)
	{
		for(int j=3; j>=0; j--)
		{
			*pLock <<= 1;
			*pLock |= (m_byLock[i][j] == 0xFF);
		}
	}
	
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::SetGPIO(DWORD *pGPIO)
{
	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + 0x1C4), *pGPIO );
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::GetGPIO(DWORD *pGPIO)
{
	*pGPIO = READ_REGISTER_ULONG( PULONG(m_pBaseAddr + 0x1C4) );
	return STATUS_SUCCESS;
}

NTSTATUS CAdapter::SetGPOE(DWORD *pGPIO)
{
	*pGPIO &= 0xFFFFFF;

	WRITE_REGISTER_DPGPRINT( PULONG(m_pBaseAddr + 0x1C8), *pGPIO );	

	return STATUS_SUCCESS;
}

BOOL CAdapter::Wait10ms()
{
	BOOL bResult;
	LARGE_INTEGER time;

	//DbgPrint(("Wait Start\n"));

	time.QuadPart = (LONGLONG) -100000;	//100ns base

	KeSetTimer(&DelayTimer, time, NULL);

	bResult =  KeWaitForSingleObject(&DelayTimer,
		Executive,
		KernelMode,
		FALSE,             // Not allertable
		NULL);             // No timeout structure

	//DbgPrint(("Wait Stop\n"));
	return bResult;
}

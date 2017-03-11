#ifndef __MV9205__MAIN_h__
#define __MV9205__MAIN_h__

#include "MV9205_Drv_Lib.h"


// (  REGW(1) + W(288+144) + Sync(2) + W(288) + jump(1)) * 4 *8 = 23168
// Frame Set : 23168*30 = 695040
// KByte : (695040 + 1023 ) /1024*1024 = 695296 = 679KByte

// Sync(4) + VideoLie(576) + Jump(1) = 581 * 8 = 4648
// Frame Set : 4648*30 = 139440
// KByte : (139440 + 1023 ) /1024*1024 = 140288 = 137KByte
#define RISC_VIDEO_SIZE	695296

// Sync(4) + AudioLine(4) + Jump(1) = 9 * 8 = 72
// KByte : (72 + 1023) / 1024 * 1024 = 1024 = 1KByte
#define RISC_AUDIO_SIZE	1024

//Yx4 Ux4 Vx4 = 12
#define RISC_VIDEO_TOTAL RISC_VIDEO_SIZE*4
//Audio x 4
#define RISC_AUDIO_TOTAL RISC_AUDIO_SIZE*4
#define RISC_TOTAL	RISC_VIDEO_TOTAL+RISC_AUDIO_TOTAL

// Image Size(768*576*2) * Dumy Size(768*4*2)  = 890922
// KByte : ( 890922 + 1023 ) /1024 * 1024 = 891904 = 871KByte
#define CAPTURE_VIDEO_SIZE	891904
#define CAPTURE_AUDIO_SIZE	44*1024
#define	CAPTURE_VIDEO_TOTAL	CAPTURE_VIDEO_SIZE*4
#define CAPTURE_AUDIO_TOTAL	CAPTURE_AUDIO_SIZE*4
#define CAPTURE_TOTAL	CAPTURE_VIDEO_TOTAL + CAPTURE_AUDIO_TOTAL

#define AUDIO_BUFFER_SIZE 4096

class CAdapter{
public:
	typedef struct tagDATA_EXTENSION {
		class CAdapter * adapter;			// Pointer to our instance	
	} DATA_EXTENSION, *PDATA_EXTENSION;
	
	CAdapter();
	~CAdapter();

	/////////////////////////////////////////////////////////////////////////
	// Using DriverEntry
	static VOID DriverUnload(IN PDRIVER_OBJECT DriverObject);
	static NTSTATUS AddDevice(PDRIVER_OBJECT DriverObject,PDEVICE_OBJECT PDO );

	/////////////////////////////////////////////////////////////////////////
	// Dispatch function
	static NTSTATUS DispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	static NTSTATUS DispatchClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	static NTSTATUS DispatchControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	static NTSTATUS DispatchCleanup(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	static NTSTATUS DispatchPower(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	static NTSTATUS DispatchPnp(PDEVICE_OBJECT DeviceObject, PIRP Irp);






	/////////////////////////////////////////////////////////////////////////
	// PnP Function
	NTSTATUS	PnpStartDevice(PDEVICE_OBJECT DeviceObject, PIRP Irp, PCM_RESOURCE_LIST raw, PCM_RESOURCE_LIST translated);
	NTSTATUS	PnpStopDevice(PDEVICE_OBJECT DeviceObject);

	/////////////////////////////////////////////////////////////////////////
	// Calling PNP PDO.
	PDEVICE_OBJECT TopOfStackDeviceObject;
	// DeviceStack Name
	UNICODE_STRING UnicodeString; 


	/////////////////////////////////////////////////////////////////////////
	// Interrupt control
	static BOOLEAN cbInterrupt(PKINTERRUPT Interrupt,PVOID  ServiceContext)
	{
		return ((CAdapter*)ServiceContext)->HwInterrupt();
	}
	static void cbDpcFunc(IN PKDPC Dpc,IN CAdapter *pAdapter,IN PVOID SystemArg1,IN PVOID SystemArg2);
	static void cbDpcFuncErr0(IN PKDPC Dpc,IN CAdapter *pAdapter,IN PVOID SystemArg1,IN PVOID SystemArg2);
	static void cbDpcFuncErr1(IN PKDPC Dpc,IN CAdapter *pAdapter,IN PVOID SystemArg1,IN PVOID SystemArg2);
	static void cbDpcFuncErr2(IN PKDPC Dpc,IN CAdapter *pAdapter,IN PVOID SystemArg1,IN PVOID SystemArg2);
	static void cbDpcFuncErr3(IN PKDPC Dpc,IN CAdapter *pAdapter,IN PVOID SystemArg1,IN PVOID SystemArg2);

	PKINTERRUPT		m_InterruptObject;
	BOOL			m_IRQExpected;
	KDPC			m_IsrDpc[8];
	KDPC			m_IsrDpcErr[4];
	KTIMER			DMATimer;
	KTIMER			DelayTimer;

	/////////////////////////////////////////////////////////////////////////
	// MV9205 Control
	/////////////////////////////////////////////////////////////////////////

	// Interrupt
	BOOLEAN		HwInterrupt();
	void		VideoCaptureRoutine(DWORD dwStreamNumber);
	void		AudioCaptureRoutine(DWORD dwStreamNumber);

	// User Control Function
	NTSTATUS	RegisterWrite(PMV9205_RW pData);
	NTSTATUS	RegisterRead(PMV9205_RW pData);
	NTSTATUS	SetVideoFormat(PMV9205_FORMAT pFormat);
	NTSTATUS	SetVideoDisplay(PMV9205_DISPLAY pDisp);
	NTSTATUS	SetEvent(PMV9205_EVENT pEv, KPROCESSOR_MODE RequestMode);
	NTSTATUS	GetVideoMemory(PMV9205_VIDEOMEM pVideoMem);

	NTSTATUS	SetAudioFormat(PMV9205_AUDIOFM pAud);
	NTSTATUS	GetVideoLock(DWORD *pLock);
	NTSTATUS	SetGPIO(DWORD *pGPIO);
	NTSTATUS	GetGPIO(DWORD *pGPIO);
	NTSTATUS	SetGPOE(DWORD *pGPIO);

//	void		CreateVideoRiscCmd(BOOL bInterlace, int cx, int cy, BOOL bSkip, PULONG *pProgLocation, DWORD dwTAddr, BOOL bSplit);

	void		CreateVideoRiscCmdNormal(BOOL bInterlace, int cx, int cy, BOOL bSkip, PULONG *pProgLocation, DWORD dwTAddr);
	void		CreateVideoRiscCmdSplit(int cx, int cy, PULONG *pProgLocation, DWORD dwTAddr, int bSplit, PULONG pRiscVROPos[][4], DWORD *dwDumyMem);
	BOOL		Wait10ms();

	BYTE*	m_pBaseAddr;

	BYTE*	m_pSystemMemory;
	BYTE*	m_pRISCMemory;
	DWORD	m_dwRiscPhysical;

	BYTE*	m_pCaptureMemory[8];
	DWORD	m_dwCapturePhysical[8];

	PMDL	m_pCaptureMdl[8];
	BYTE*	m_pCaptureUser[8];
	PKEVENT	m_pEvent[8];

	BOOL	m_bCaptureStart[16];

	// Video Format
	BOOL	m_bField[4];

	// Video Split;
	int		m_nSplit[4];
	int		m_nAGCData[4][4];
	DWORD	m_dwAGCChange[4][4];
	DWORD	m_dwADCAGC;
	DWORD	m_dwDecEnable[8];
	DWORD	m_dwDecDisable[8];
	BYTE*	m_pDstAddr[4][4];
	DWORD	m_dwLinePitch[4];
	int		m_nHeight[4];

	int		m_nVROMaster[4];
	DWORD	m_dwDumyMemory[4];
	BYTE	m_byLock[4][4];
	PULONG	m_pRiscVROPos[4][4];



	// Audio Format
	BYTE*	m_pAudioData[4];
	DWORD	m_dwAudioField[4];
	DWORD	m_dwErrorCode;

	DWORD	m_dwGPIO;
	DWORD	m_dwINTMaskBar;
	int		m_nFrameSkip[8];
};

#endif

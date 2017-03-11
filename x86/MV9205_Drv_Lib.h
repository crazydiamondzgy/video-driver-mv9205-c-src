#ifndef __MV9205_DRIVER_LIB_H__
#define __MV9205_DRIVER_LIB_H__

#define FILE_DEVICE_UNKNOWN             0x00000022
#define METHOD_BUFFERED                 0
#define FILE_ANY_ACCESS                 0
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
#define IOCTL_MV9205_REG_WRITE		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_REG_READ		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_VIDEO_FORMAT	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_VIDEO_DISPLAY	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_AUDIO_FORMAT	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_CAP_EVENT		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_CAP_MEM		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_GET_LOCK		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_SET_GPIO		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_GET_GPIO		CTL_CODE(FILE_DEVICE_UNKNOWN, 0xA, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MV9205_SET_GPOE		CTL_CODE(FILE_DEVICE_UNKNOWN, 0xB, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define VIDEOFORMAT_YUY2	0
#define VIDEOFORMAT_YUV411	1
#define VIDEOFORMAT_Y8		2
#define VIDEOFORMAT_YUV420	3

#define AUDIOFORMAT_8MONO		0
#define AUDIOFORMAT_8STEREO		1
#define AUDIOFORMAT_16MONO		2
#define AUDIOFORMAT_16STEREO	3

typedef struct {
	DWORD dwAddr;			//Resigter Address
	int	nCounter;			//Data Counter
	DWORD *pData;			//Data
} MV9205_RW, *PMV9205_RW;

typedef struct {
	int nChannel;			//0~3
	BOOL	bEnable;
	int		nWidth;		// Image Width
	int		nHeight;	// Image Height
	BOOL	bInterlace;
	BOOL	bStandard;	//0: NTSC, 1:PAL
	int		nFormat;		// VIDEOFORMAT_XXXX
	int		nFrame;		// NTSC : 1~30, PAL : 1~25
	int		nSplit;		// Split (0:nonsplit,1~3:split);
	int		nMux;		// Mux sel
} MV9205_FORMAT, *PMV9205_FORMAT;

typedef struct {
	int nChannel;			//0~3
	BYTE byBright;			//0~255	: Brightness
	BYTE byContrast;		//0~255	: Contrast
	BYTE bySaturation;		//0~255	: Saturation
	BYTE byHue;				//0~255	: Hue
} MV9205_DISPLAY, *PMV9205_DISPLAY;

typedef struct {
	int nChannel;			//0~7	0~3:Video, 4~7:Audio
	HANDLE	hEvent;			// Capture Event
} MV9205_EVENT, *PMV9205_EVENT;

typedef struct {
	int nChannel;			//0~7	0~3:Video, 4~7:Audio
	BYTE *pMem;				//Memory Address
	DWORD	dwSize;			//Memory Size
} MV9205_VIDEOMEM, *PMV9205_VIDEOMEM;

typedef struct {
	BOOL	bEnable[4];
	DWORD	dwFreq;				//Sampling Frequency
	int		nFormat;			//AUDIOFORMAT_XXXX
} MV9205_AUDIOFM, *PMV9205_AUDIOFM;

#endif
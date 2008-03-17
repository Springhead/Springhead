/* ========================================================================
*  PROJECT: DirectShow Video Processing Library (DSVL)
*  Version: 0.0.8 (05/13/2005)
*  ========================================================================
*  Author:  Thomas Pintaric, Vienna University of Technology
*  Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
*  =======================================================================
* 
*  Copyright (C) 2005  Vienna University of Technology
* 
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*  For further information please contact Thomas Pintaric under
*  <pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
*  Vienna University of Technology, Favoritenstr. 9-11/E188/2, A-1040
*  Vienna, Austria.
* ========================================================================*/


#include <streams.h>
#include <atlcomcli.h>	// CComPtr
#include <comutil.h>	// _bstr_t
#include "Qedit.h"
#include <map>
using namespace std;
#include "DSVL_PixelFormat.h"

const PIXELFORMAT default_PIXELFORMAT = PIXELFORMAT_RGB32;
#include "tinyxml.h"
// -----------------------------------------------------------------------------------------------------------------

#define WM_GRAPHNOTIFY  WM_APP + 1
#define CLASSNAME "EventNotify"

// -----------------------------------------------------------------------------------------------------------------
typedef enum _VIDEO_INPUT_DEVICE
{
	WDM_VIDEO_CAPTURE_FILTER = 0, // select video source from among WDM Streaming Capture Device filters in 
							      // class CLSID_VideoInputDeviceCategory.
	ASYNC_FILE_INPUT_FILTER  = 1, // read video from an AVI file (through an asynchronous file input filter)
	INVALID_INPUT_FILTER  = 2     // do not use

} VIDEO_INPUT_DEVICE;


static const char *VIDEO_INPUT_DEVICE_names[]=
{
	"WDM_CAP",
	"AVI_FILE",
	"invalid" // do not use
};

const VIDEO_INPUT_DEVICE default_VIDEO_INPUT_DEVICE = WDM_VIDEO_CAPTURE_FILTER;
// -----------------------------------------------------------------------------------------------------------------
typedef enum _VIDEO_INPUT_FLAGS // can be combined, i.e. (WDM_SHOW_DIALOG|WDM_MATCH_FILTER_NAME)
{
	WDM_MATCH_FORMAT   		= 0x0001, // first media type matching the a given set of format requirements
	WDM_SHOW_FORMAT_DIALOG	= 0x0002, // displays either the capture pin's property pages (non-DV cameras) or
										  // the DV-decoder filter's format dialog before connecting anything
	WDM_SHOW_CONTROL_DIALOG	= 0x0004, // displays the source filter's property pages before connecting anything
	WDM_MATCH_FILTER_NAME	= 0x0008, // tries find a match based on a filter name substring, i.e. "QuickCam" or "Sony"
	WDM_MATCH_IEEE1394_ID	= 0x0010, // match filter based on a unique 64-bit IEEE 1394 device ID (MSDV driver only)
								      // this technique allows to explicitly choose between different cameras connected
								      // to the same IEEE 1394 bus. Setting this flag usually requires use of the WDM
								      // MSDV driver(qdv.dll)
								      // >> use "/bin/IEEE1394_id.exe" to determine your camera's ID.
	WDM_USE_MAX_FRAMERATE	= 0x0020  // uses the max. available frame rate for the format requested

} VIDEO_INPUT_FLAGS;

// Media type matching is based on the AM_MEDIA_TYPE structures obtained from
// each WDM Video Capture Filter.
//
//   AM_MEDIA_TYPE.majortype (MUST BE MEDIATYPE_Video)
//   AM_MEDIA_TYPE.subtype
//   AM_MEDIA_TYPE.formattype (MUST BE FORMAT_VideoInfo)
//	 VIDEOINFOHEADER.AvgTimePerFrame
//	 VIDEOINFOHEADER.BITMAPINFOHEADER.biWidth 
//	 VIDEOINFOHEADER.BITMAPINFOHEADER.biHeight 

const VIDEO_INPUT_FLAGS default_VIDEO_INPUT_FLAGS = WDM_SHOW_FORMAT_DIALOG;
// -----------------------------------------------------------------------------------------------------------------
typedef enum _ASYNC_INPUT_FLAGS
{
	ASYNC_INPUT_DO_NOT_USE_CLOCK   = 0x0100, // call IMediaFilter::SetSyncSource(NULL) on async file source filter
										     // this will effectively prevent samples from being dropped, but
										     // may result in 100% CPU usage on single-processor systems
	ASYNC_LOOP_VIDEO               = 0x0200, // continuously loops through an input file
	ASYNC_RENDER_SECONDARY_STREAMS = 0x0400  // The Async File Source Filter is always used in combination with an
	                                         // AVI Splitter Filter (CLSID_AviSplitter), where Stream 0 is assumed
										     // to contain video data (MEDIATYPE_Video). This flag instructs DSVL_GraphManager
										     // to call IGraphBuilder->Render(IPin*) on Streams 1..n
										     // Set this flag if your AVI file contains an audio stream AND you want it
										     // rendered by DirectSound (usually through CLSID_DSoundRender).
										     // DO NOT SET this flag if your AVI file contains more than one video stream
// not yet implemented:
//	ASYNC_SHOW_DECODER_DIALOG      = 0x0800, // displays either the video decoder's property pages


} ASYNC_INPUT_FLAGS;

const DWORD default_ASYNC_INPUT_FLAGS = 0;
// -----------------------------------------------------------------------------------------------------------------
typedef enum _CP_INTERFACE
{
	CP_CameraControl	= 0, // IAMCameraControl
	CP_VideoProcAmp		= 1  // IAMVideoProcAmp
} CP_INTERFACE;


typedef void (*PTR_UserCallback)(IMediaSample*);
typedef void (*PTR_SimpleUserCallback)(BYTE*,int,int,PIXELFORMAT,REFERENCE_TIME); 

// -----------------------------------------------------------------------------------------------------------------
typedef struct _DS_MEDIA_FORMAT
{
	VIDEO_INPUT_DEVICE inputDevice;
	LONG biWidth;
	LONG biHeight;
	double frameRate;
	GUID subtype;
	bool isInterlaced;
	LPWSTR sourceFilterName;   // (inputDevice == WDM_VIDEO_CAPTURE_FILTER): WDM capture filter's friendly/device name
	                           // (inputDevice == ASYNC_FILE_INPUT_FILTER):  input file name
	bool isDeviceName;         // false: friendly name, true: device name

	DWORD inputFlags;          // (inputDevice == WDM_VIDEO_CAPTURE_FILTER): combination of ASYNC_INPUT_FLAGS
	                           // (inputDevice == ASYNC_FILE_INPUT_FILTER):  combination of VIDEO_INPUT_FLAGS
							   // NOTE: don't forget to set (defaultInputFlags = false) if you use custom flags!
	bool defaultInputFlags;
	PIXELFORMAT pixel_format;  // default: PIXELFORMAT_RGB32
	char* ieee1394_id;         // unique 64-bit camera identifier (IEEE 1394 devices only)
							   // will be ignored/set to zero for non-IEEE 1394 cameras

	bool flipH; // applies to PIXELFORMAT_RGB32 only
	bool flipV; // applies to PIXELFORMAT_RGB32 only

} DS_MEDIA_FORMAT;

DS_MEDIA_FORMAT default_DS_MEDIA_FORMAT();
// -----------------------------------------------------------------------------------------------------------------
#ifndef __MEMORY_BUFFER_HANDLE__
#define __MEMORY_BUFFER_HANDLE__

#define MIN_ALLOCATOR_BUFFERS_PER_CLIENT  3
#define DEF_CONCURRENT_CLIENTS            3
struct MemoryBufferHandle
{
	unsigned long  n; // sample number
	LONGLONG t; // timestamp
};

#endif

struct MemoryBufferEntry
{
	unsigned int use_count;
	REFERENCE_TIME timestamp;
	IMediaSample* media_sample;	
};

//###############################################################################################
class DSVL_GraphManager;

//------------------------------------------------------------------------------------------------
double avg2fps(REFERENCE_TIME AvgTimePerFrame, int precision = 3);
REFERENCE_TIME fps2avg(double fps);

//------------------------------------------------------------------------------------------------
class DSVL_GraphManager : public ISampleGrabberCB
{
public:
	//IUnknown interface
	HRESULT WINAPI QueryInterface( REFIID iid, void** ppvObject );
	ULONG WINAPI AddRef();
	ULONG WINAPI Release();

	// ISampleGrabberCB interfaces
	HRESULT WINAPI SampleCB(double SampleTime, IMediaSample *pSample);
	HRESULT WINAPI BufferCB(double sampleTimeSec, BYTE* bufferPtr, long bufferLength);
private:
	unsigned long	fRefCount;


public:
	DSVL_GraphManager();
	~DSVL_GraphManager();

/*****************************************************************************/
	HRESULT BuildGraphFromXMLString(char* xml_string = NULL);
	HRESULT BuildGraphFromXMLFile(char* xml_filename = NULL);
/*****************************************************************************/
	HRESULT ReleaseGraph();
	bool IsGraphInitialized();

protected:
	REFERENCE_TIME current_timestamp;
	std::map<unsigned long, MemoryBufferEntry> mb;
	unsigned long sample_counter;
	unsigned int m_currentAllocatorBuffers;

public:
	HRESULT GetCurrentMediaFormatEx(DS_MEDIA_FORMAT *mf);
	HRESULT GetCurrentMediaFormat(LONG* frame_width, LONG *frame_height, double* frames_per_second, PIXELFORMAT* pixel_format);

	HRESULT GetCurrentTimestamp(REFERENCE_TIME *Timestamp);
			// I'd recommend you Lock() before querying for timestamps
	REFERENCE_TIME GetCurrentTimestamp();

	DWORD WaitForNextSample(long dwMilliseconds = INFINITE); 
		// behaves just like WaitForSingleObject

	HRESULT EnableMemoryBuffer(unsigned int _maxConcurrentClients = DEF_CONCURRENT_CLIENTS,
                               unsigned int _allocatorBuffersPerClient = MIN_ALLOCATOR_BUFFERS_PER_CLIENT);

	HRESULT DisableMemoryBuffer();

	
	HRESULT CheckoutMemoryBuffer(MemoryBufferHandle* pHandle, 
								 BYTE** Buffer,
								 unsigned int *Width = NULL,
								 unsigned int *Height = NULL,
								 PIXELFORMAT* PixelFormat = NULL,
								 REFERENCE_TIME* Timestamp = NULL);
	// every check-out operation will increment an internal instance counter
	// samples get released only if there are no active instances

	HRESULT CheckinMemoryBuffer(MemoryBufferHandle Handle, bool ForceRelease = false); 
	// decrements the instance counter

protected:
	// display property pages
	HRESULT ShowFilterProperties(HWND hWnd = NULL);
	HRESULT ShowPinProperties(HWND hWnd = NULL);

private:
	HRESULT BuildGraphFromXMLHandle(TiXmlHandle xml_h);

// ------------------------------------------------------------------------
public:
	// valid types for (long property): CameraControlProperty, VideoProcAmpProperty
	HRESULT GetCameraParameterRange(CP_INTERFACE interface_type,  
									long property,
									long *pMin,
									long *pMax,
									long *pSteppingDelta,
									long *pDefault,
									long *pCapsFlags);

	HRESULT GetCameraParameter(CP_INTERFACE interface_type, long Property, long *lValue, bool *bAuto);
	HRESULT SetCameraParameter(CP_INTERFACE interface_type, long Property, long lValue, bool bAuto);

protected:
	long GetCameraPropertyAUTOFlag(CP_INTERFACE interface_type, bool bAUTO = true);
	HRESULT GetCameraParameter(CP_INTERFACE interface_type, long Property, long *lValue, long *Flags);
	HRESULT SetCameraParameter(CP_INTERFACE interface_type, long Property, long lValue, long Flags);

public:
	// applies settings to a normalized the parameter range [0..1]
	HRESULT GetCameraParameterN(CP_INTERFACE interface_type, long Property, double *dValue);
	HRESULT SetCameraParameterN(CP_INTERFACE interface_type, long Property, double dValue); // dValue will be clamped to [0..1]

	HRESULT SetCameraParameterToDefault(CP_INTERFACE interface_type, long Property, bool bAuto = TRUE); 
	// bAuto: indicates if the property should be controlled automatically

	HRESULT ResetCameraParameters(bool bAuto = TRUE); // reset all parameters to their defaults

	// Applies to all methods above:
	// -----------------------------
	// "Property" can be one of the following: 
	// (see the Microsoft DirectX 8.1 (C++) SDK documentation for details)
	//
	//		interface_type == CP_CameraControl
	//			CameraControl_Pan
    //			CameraControl_Tilt
    //			CameraControl_Roll
    //			CameraControl_Zoom
    //			CameraControl_Exposure
    //			CameraControl_Iris
	//			CameraControl_Focus
	//		interface_type == CP_VideoProcAmp
	//			VideoProcAmp_Brightness,
	//			VideoProcAmp_Contrast,
	//		    VideoProcAmp_Hue,
	//			VideoProcAmp_Saturation,
	//			VideoProcAmp_Sharpness,
	//			VideoProcAmp_Gamma,
	//			VideoProcAmp_ColorEnable,
	//			VideoProcAmp_WhiteBalance,
	//			VideoProcAmp_BacklightCompensation,
	//			VideoProcAmp_Gain
	//
	//
	// Return values:  S_OK			 call succeeded
	// --------------  S_FALSE		 call succeeded, but value remained unchanged 
	//								 (GetCameraParameterN/SetCameraParameterN only)
	//				   E_INVALIDARG  interface not supported by WDM driver / unknown interface
	//				   E_FAIL		 call failed
// ------------------------------------------------------------------------

	// media flow control
	HRESULT Run();
	HRESULT Pause();
	HRESULT Stop(bool forcedStop = false);

	inline void Lock() { m_CSec.Lock(); }
	inline void Unlock() { m_CSec.Unlock(); }
	//inline void Lock() {}
	//inline void Unlock() {}

private:

	bool m_bGraphIsInitialized;
	CCritSec m_CSec;

	#ifdef _DEBUG
	DWORD dwRegisterROT;
	#endif

	DS_MEDIA_FORMAT media_format;

	HANDLE m_ESync;
	const LPTSTR m_ESyncName;
	//void* sync_uuid_p; // (GUID*)

//protected:

#pragma warning(disable : 4251)
// >> warning C4251: 'DSVL_GraphManager::captureGraphBuilder' : class 'ATL::CComPtr<T>'
// >> needs to have dll-interface to be used by clients of class 'DSVL_GraphManager'

	CComPtr <IGraphBuilder>	graphBuilder;
	CComPtr <IMediaControl>	mediaControl;
	CComPtr <ICaptureGraphBuilder2>	captureGraphBuilder;
	CComPtr <IMediaEventEx>	mediaEvent;
	CComPtr <IMediaSeeking>	mediaSeeking;
	CComPtr <IAMCameraControl>	cameraControl;
	CComPtr <IAMDroppedFrames>	droppedFrames;
	CComPtr <IAMVideoControl>	videoControl;
	CComPtr <IAMVideoProcAmp>	videoProcAmp;
	CComPtr<IBaseFilter>	sourceFilter;
	CComPtr<IPin>			capturePin;
	CComPtr<IBaseFilter>	decoderFilter;
	CComPtr<IBaseFilter>	rendererFilter;
	CComPtr<IBaseFilter>	grabberFilter;
	CComPtr<ISampleGrabber> sampleGrabber;


};

DWORD WINAPI TShowFilterProperties(LPVOID lpParameter);
DWORD WINAPI TShowPinProperties(LPVOID lpParameter);

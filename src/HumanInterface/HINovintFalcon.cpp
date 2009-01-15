#include "HumanInterface.h"
#ifdef _WIN32
 #pragma hdrstop
#endif
#include "HINovintFalcon.h"
#include <WinBasis/WinBasis.h>
#include <WinBasis/WBDllLoader.h>

namespace Spr {;

#define HDLAPIENTRY  __stdcall

typedef int HDLDeviceHandle;
#define HDL_INVALID_HANDLE -1

typedef int HDLError;
#define HDL_NO_ERROR 0x0                  /**< No errors on error stack */
#define HDL_ERROR_STACK_OVERFLOW 0x01     /**< Overflow of error stack */
#define HDL_ERROR_INIT_FAILED    0x10     /**< Device initialization error */

#define HDAL_NOT_CALIBRATED    0x04

extern "C"{
//	DLLスタブの実装
static WBDllLoader dllLoader("hdl.dll");	//	グローバル変数でローダーを作る．

//	__declspec(dllexport) HDLDeviceHandle HDLAPIENTRY hdlInitNamedDevice(const char* deviceName, const char* configPath = 0);
#define DLLFUNC_RTYPE	HDLDeviceHandle						//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlInitNamedDevice					//	関数名
#define DLLFUNC_STR		"_hdlInitNamedDevice@8"				//	関数名
#define DLLFUNC_ARGDEF	(const char* deviceName, const char* configPath)	//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(deviceName, configPath)			//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) int HDLAPIENTRY hdlGetState();
#define DLLFUNC_RTYPE	int									//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlGetState							//	関数名
#define DLLFUNC_STR		"_hdlGetState@0"					//	関数名
#define DLLFUNC_ARGDEF	()									//	関数宣言時の引数
#define DLLFUNC_ARGCALL	()									//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void hdlUninitDevice(HDLDeviceHandle hHandle);
#define DLLFUNC_RTYPE	void								//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlUninitDevice						//	関数名
#define DLLFUNC_STR		"_hdlUninitDevice@4"				//	関数名
#define DLLFUNC_ARGDEF	(HDLDeviceHandle hHandle)			//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(hHandle)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void hdlSetToolForce(double force[3]);
#define DLLFUNC_RTYPE	void								//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlSetToolForce						//	関数名
#define DLLFUNC_STR		"_hdlSetToolForce@4"				//	関数名
#define DLLFUNC_ARGDEF	(double force[3])					//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(force)								//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void hdlToolPosition(double force[3]);
#define DLLFUNC_RTYPE	void								//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlToolPosition						//	関数名
#define DLLFUNC_STR		"_hdlToolPosition@4"				//	関数名
#define DLLFUNC_ARGDEF	(double pos[3])						//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(pos)								//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void hdlToolButtons(int* pButtons);
#define DLLFUNC_RTYPE	void								//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlToolButtons						//	関数名
#define DLLFUNC_STR		"_hdlToolButtons@4"					//	関数名
#define DLLFUNC_ARGDEF	(int * pButtons)					//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(pButtons)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>
}

//	__declspec(dllexport) HDLError HDLAPIENTRY hdlGetError();
#define DLLFUNC_RTYPE	HDLError							//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlGetError							//	関数名
#define DLLFUNC_STR		"_hdlGetError@0"					//	関数名
#define DLLFUNC_ARGDEF	()									//	関数宣言時の引数
#define DLLFUNC_ARGCALL	()									//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void HDLAPIENTRY hdlStart();
#define DLLFUNC_RTYPE	void								//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlStart							//	関数名
#define DLLFUNC_STR		"_hdlStart@0"						//	関数名
#define DLLFUNC_ARGDEF	()									//	関数宣言時の引数
#define DLLFUNC_ARGCALL	()									//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void HDLAPIENTRY hdlStop();
#define DLLFUNC_RTYPE	void								//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlStop								//	関数名
#define DLLFUNC_STR		"hdlStop"							//	関数名
#define DLLFUNC_ARGDEF	()									//	関数宣言時の引数
#define DLLFUNC_ARGCALL	()									//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void hdlMakeCurrent(HDLDeviceHandle hHandle);
#define DLLFUNC_RTYPE	void								//	返り型
#define DLLFUNC_CTYPE	HDLAPIENTRY							//	呼び出し型
#define DLLFUNC_NAME	hdlMakeCurrent						//	関数名
#define DLLFUNC_STR		"_hdlMakeCurrent@4"					//	関数名
#define DLLFUNC_ARGDEF	(HDLDeviceHandle hHandle)			//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(hHandle)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>



HINovintFalcon::HINovintFalcon():deviceHandle(HDL_INVALID_HANDLE),button(0), good(false){
}
HINovintFalcon::~HINovintFalcon(){
	hdlUninitDevice(deviceHandle);
	deviceHandle = HDL_INVALID_HANDLE;
}
bool testHDLError(char* str){
    HDLError err = hdlGetError();
	// Check for error.
    if (err != HDL_NO_ERROR)
    {
		// Display message.
		DSTR << "Error: Novient Falcon - " << str << std::endl;
		return false;
    }
	return true;
}
bool isFalconCalibrated()
{
	// Call function to check the state of hdl.
	// State is return as bitmask.
    unsigned int state = hdlGetState();
	// Return true of the NOT_CALIBRATED bit is not set.
	return (state & HDAL_NOT_CALIBRATED) == 0;
}
void calibrateFalcon(){
	DSTR << "Please calibrate the Falcon by extending then pushing the arms all the way in.\nThe Falcon is calibrated when the light changes from RED to BLUE." << std::endl;
	int i;
	int n = 30;
	for (i=0; i< n && !isFalconCalibrated(); ++i){
		Sleep(100);
		DSTR << ".";
	}
	if (i<n)	DSTR << "Calibration finished." << std::endl;
	else DSTR << "Calibration failed!! make sure your Falcon is in order." << std::endl;
}
bool HINovintFalcon::Init(const void* desc){
	dllLoader.Load();
	deviceHandle = hdlInitNamedDevice(NULL, NULL);
	// Check return value of the handle.
    if (deviceHandle == HDL_INVALID_HANDLE){
		// There was an error trying to initialize the device.
		DSTR << "Error: Novient Falcon - Could not open device." << std::endl;
		return false;
    }

	// Start the servo thread
	hdlStart();
	testHDLError("Could not start servo thread");

	// Sleep for 1sec to allow everything to start running.
	Sleep(1000);

	// Make the device associated with the returned handle
	// the current device.  All subsequent calls will
    // be directed towards the current device.
    hdlMakeCurrent(deviceHandle);
    testHDLError("Could not make Falcon device current");

	calibrateFalcon();
	good = true;
	return true;
}
bool HINovintFalcon::BeforeCalibration(){
	return true;
}
bool HINovintFalcon::Calibration(){
	calibrateFalcon();
	return true;
}
int HINovintFalcon::GetButton(int ch){
	return (button & (1<<ch)) != 0 ;
}
int HINovintFalcon::GetButtonBit(){
	return button;
}
void HINovintFalcon::Update(float dt){
	if (good){
		Vec3d p;
		hdlToolPosition(p);
		pos = p;
		hdlToolButtons(&button);
		Vec3d f = force;
		hdlSetToolForce(f);
	}
	HIForceInterface6D::Update(dt);
}

}//	namespace Spr

#include "Device.h"
#pragma hdrstop
//#include "DRAdapio.h"

using namespace std;

namespace Spr {
static WBDllLoader dllLoader("TUADAPIO.DLL");	//	グローバル変数でローダーを作る．
	
DRAdapio::DVDA::DVDA(DRAdapio* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s D/A ch %d", realDevice->Name(), ch);
}
DRAdapio::DVAD::DVAD(DRAdapio* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s A/D ch %d", realDevice->Name(), ch);
}
DRAdapio::DRAdapio(int i){
	id = i;
	sprintf(name, "ADAPIO %d", id);
}
DRAdapio::~DRAdapio(){
	CloseDevice();
}
/// 初期化
bool DRAdapio::Init(){
	return dllLoader.Load() && Adapio_Device_Open(id)==0;
}
void DRAdapio::Register(DVVirtualDevicePool& vpool){
	for(int i=0; i<8; i++){
		vpool.Register(new DVAD(this, i));
	}
	for(int i=0; i<8; i++){
		vpool.Register(new DVDA(this, i));
	}
}
///	電圧出力
void DRAdapio::DAVoltage(int ch, float v){
	int dat = (int) (v / 2.5f) * 0x100;
	if (dat > 0xFF) dat = 0xFF;
	if (dat < 0) dat = 0;
	Adapio_Dac_Out(id, ch, dat);
}
void DRAdapio::DADigit(int ch, int dat){
	if (dat > 0xFF) dat = 0xFF;
	if (dat < 0) dat = 0;
	Adapio_Dac_Out(id, ch, dat);
}
///	電圧入力
float DRAdapio::ADVoltage(int ch){
	short dat=0;
	Adapio_Adc_SingleSample(id, ch, &dat);
	return dat * (2.5f / 1024);
}
int DRAdapio::ADDigit(int ch){
	short dat=0;
	Adapio_Adc_SingleSample(id, ch, &dat);
	return dat;
}
/// 終了処理
void DRAdapio::CloseDevice(){
	Adapio_Device_Close(id);
}


extern "C" {
//	short __stdcall Adapio_Device_Open(short id)
#define DLLFUNC_RTYPE	short					//	返り型
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Device_Open		//	関数名
#define DLLFUNC_STR		"Adapio_Device_Open"	//	関数名
#define DLLFUNC_ARGDEF	(short id)				//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id)					//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void __stdcall Adapio_Device_Close(short id);
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Device_Close		//	関数名
#define DLLFUNC_STR		"Adapio_Device_Close"	//	関数名
#define DLLFUNC_ARGDEF	(short id)				//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id)					//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_LowDirection(short id, unsigned char dir);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_LowDirection				//	関数名
#define DLLFUNC_STR		"Adapio_Pio_LowDirection"			//	関数名
#define DLLFUNC_ARGDEF	(short id, unsigned char dir)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, dir)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_HighDirection(short id,unsigned char dir);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_HighDirection			//	関数名
#define DLLFUNC_STR		"Adapio_Pio_HighDirection"			//	関数名
#define DLLFUNC_ARGDEF	(short id, unsigned char dir)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, dir)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_LowByteWrite(short id,unsigned char dat);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_LowByteWrite				//	関数名
#define DLLFUNC_STR		"Adapio_Pio_LowByteWrite"			//	関数名
#define DLLFUNC_ARGDEF	(short id,unsigned char dat)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, dat)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_HighByteWrite(short id,unsigned char dat);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_HighByteWrite			//	関数名
#define DLLFUNC_STR		"Adapio_Pio_HighByteWrite"			//	関数名
#define DLLFUNC_ARGDEF	(short id,unsigned char dat)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, dat)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_LowByteRead(short id,unsigned char *dat);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_LowByteRead				//	関数名
#define DLLFUNC_STR		"Adapio_Pio_LowByteRead"			//	関数名
#define DLLFUNC_ARGDEF	(short id, unsigned char* dat)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, dat)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_HighByteRead(short id,unsigned char *dat);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_HighByteRead				//	関数名
#define DLLFUNC_STR		"Adapio_Pio_HighByteRead"			//	関数名
#define DLLFUNC_ARGDEF	(short id, unsigned char* dat)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, dat)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_WordWrite(short id,int dat);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_WordWrite				//	関数名
#define DLLFUNC_STR		"Adapio_Pio_WordWrite"				//	関数名
#define DLLFUNC_ARGDEF	(short id, int dat)					//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, dat)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_WordRead(short id,int *dat);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_WordRead					//	関数名
#define DLLFUNC_STR		"Adapio_Pio_WordRead"				//	関数名
#define DLLFUNC_ARGDEF	(short id, int* dat)				//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, dat)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_Clock_Out(short id,int count );
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_Clock_Out				//	関数名
#define DLLFUNC_STR		"Adapio_Pio_Clock_Out"				//	関数名
#define DLLFUNC_ARGDEF	(short id, int count)				//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, count)							//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_Clock_Stop(short id);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Pio_Clock_Stop				//	関数名
#define DLLFUNC_STR		"Adapio_Pio_Clock_Stop"				//	関数名
#define DLLFUNC_ARGDEF	(short id)							//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id)								//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Dac_Out(short id,short ch,unsigned char dat);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Dac_Out						//	関数名
#define DLLFUNC_STR		"Adapio_Dac_Out"					//	関数名
#define DLLFUNC_ARGDEF	(short id, short ch, unsigned char dat)	//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, ch, dat)						//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_SingleSample(short id,short ch,short *dat);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Adc_SingleSample				//	関数名
#define DLLFUNC_STR		"Adapio_Adc_SingleSample"			//	関数名
#define DLLFUNC_ARGDEF	(short id, short ch, short *dat)	//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, ch, dat)						//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_DigitalTrg(short id,short EndCh,short BufSize);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Adc_DigitalTrg				//	関数名
#define DLLFUNC_STR		"Adapio_Adc_DigitalTrg"				//	関数名
#define DLLFUNC_ARGDEF	(short id,short EndCh,short BufSize)//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(id, EndCh, BufSize)				//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_AnalogTrg(short id,short EndCh,short BufSize, short Threshold,short TrgCh ,short UpOrDown);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Adc_AnalogTrg				//	関数名
#define DLLFUNC_STR		"Adapio_Adc_AnalogTrg"				//	関数名
#define DLLFUNC_ARGDEF	(short id,short EndCh,short BufSize, short Threshold,short TrgCh ,short UpOrDown)
#define DLLFUNC_ARGCALL	(id, EndCh, BufSize, Threshold, TrgCh, UpOrDown)
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_GetStatus(short id,short *Running, short *SampledNum);
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall							//	呼び出し型
#define DLLFUNC_NAME	Adapio_Adc_GetStatus
#define DLLFUNC_STR		"Adapio_Adc_GetStatus"
#define DLLFUNC_ARGDEF	(short id,short *Running, short *SampledNum)
#define DLLFUNC_ARGCALL	(id, Running, SampledNum)
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_GetDatas(short id,short *databuf, short Len );
#define DLLFUNC_RTYPE	short								//	返り型
#define DLLFUNC_CTYPE	__stdcall				//	呼び出し型
#define DLLFUNC_NAME	Adapio_Adc_GetDatas
#define DLLFUNC_STR		"Adapio_Adc_GetDatas"
#define DLLFUNC_ARGDEF	(short id,short *databuf, short Len )
#define DLLFUNC_ARGCALL	(id,databuf, Len)
#include <WinBasis/WBDllLoaderImpl.h>
}	//	extern "C"
}	//	namespace Spr

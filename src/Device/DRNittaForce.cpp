#include "Device.h"
#pragma hdrstop
namespace Spr {


DRNittaForce::DRNittaForce(int n){
	if (n>10){
		baseAdr = n;
		boardNumber = -1;
		sprintf(name, "Nitta 6DOF force sensor at 0x%03X", baseAdr);
	}else{
		boardNumber = n;
		baseAdr = -1;
		sprintf(name, "Nitta 6DOF force sensor #%d", boardNumber);
	}
	for(int i=0; i < DOF; ++i){
		force[i] = 0;
		full_range[i] = 1;
	}
	model_no = 0;
	software_ver_no = 0;
	serial_no = 0;
	units = 0;
}
DRNittaForce::~DRNittaForce(){
}

//	力センサのドライバDLL jr3.dll をロードする．

static WBDllLoader dllLoader("jr3.dll");	//	グローバル変数でローダーを作る．

//	int DllFunc(int arg)  の場合
//	DWORD DllExport JR3read(short boardNumber, short address, short* data) ;
#define DLLFUNC_RTYPE	DWORD		//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	JR3read		//	関数名
#define DLLFUNC_STR		"JR3read"
#define DLLFUNC_ARGDEF		(short boardNumber, short address, short* data)	
									//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(boardNumber, address, data)
									//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>
//	DWORD DllExport JR3write(short boardNumber, short address, short data) ;
#define DLLFUNC_RTYPE	DWORD		//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	JR3write	//	関数名
#define DLLFUNC_STR		"JR3write"
#define DLLFUNC_ARGDEF		(short boardNumber, short address, short data)	
									//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(boardNumber, address, data)
									//	関数呼び出しの引数
#include <WinBasis/WBDllLoaderImpl.h>

void DRNittaForce::ReadReg(unsigned short a, unsigned short* d){
	if (baseAdr != -1){
		_outpw(baseAdr, a);
		*d = _inpw(baseAdr + 2);
	}else{
		JR3read(boardNumber, a, (short*)d);
	}
}
void DRNittaForce::WriteReg(unsigned short a, unsigned short d){
	if (baseAdr != -1){
		_outpw(baseAdr, a);
		_outpw(baseAdr + 2, d);
	}else{
		JR3write(boardNumber, a, d);
	}
}	
bool DRNittaForce::Init(){
	if (baseAdr != -1){	//	直接アクセスの場合はGiveIO.sysを使ってポートへのアクセス許可をもらう
		WBGetPortIO();
	}
	ReadReg(software_ver_no_addr, &software_ver_no);
	ReadReg(serial_no_addr, &serial_no);
 	ReadReg(model_no_addr, &model_no);
	ReadReg(force_units_addr, &units);
	ReadReg(full_fx_addr, &full_range[0]);
	ReadReg(full_fy_addr, &full_range[1]);
	ReadReg(full_fz_addr, &full_range[2]);
	ReadReg(full_mx_addr, &full_range[3]);
	ReadReg(full_my_addr, &full_range[4]);
	ReadReg(full_mz_addr, &full_range[5]);
	if (software_ver_no == 0xFFFF && serial_no == 0xFFFF && model_no == 0xFFFF){
		return false;
	}
	return true;
}
void DRNittaForce::Register(DVVirtualDevicePool& vpool){
	vpool.Register(new VirtualDevice(this));
}
void DRNittaForce::ReadForce(int ch){
	unsigned short raw;
	ReadReg(force_addr+ch, &raw);
	force[ch] = raw;
}
unsigned short DRNittaForce::GetCounter2K(){
	unsigned short raw;
	ReadReg(count2_addr, &raw);
	return raw;
}

unsigned short DRNittaForce::GetCounter8K(){
	unsigned short raw;
	ReadReg(count1_addr, &raw);
	return raw;
}

void DRNittaForce::Reset()
{	
	unsigned short i;
	unsigned short raw;
	ReadReg(fx0_addr, &raw);   ReadReg(0x88, &i);   WriteReg(0x88, i + raw);	 
	ReadReg(fy0_addr, &raw);   ReadReg(0x89, &i);   WriteReg(0x89, i + raw);	 
	ReadReg(fz0_addr, &raw);   ReadReg(0x8a, &i);   WriteReg(0x8a, i + raw); 
	ReadReg(mx0_addr, &raw);   ReadReg(0x8b, &i);   WriteReg(0x8b, i + raw);	 
	ReadReg(my0_addr, &raw);   ReadReg(0x8c, &i);   WriteReg(0x8c, i + raw);	 
	ReadReg(mz0_addr, &raw);   ReadReg(0x8d, &i);   WriteReg(0x8d, i + raw);
}

}	//	namespace Spr

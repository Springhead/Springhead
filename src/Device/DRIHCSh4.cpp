// DRIHCSh4.cpp: SH4上で動く，デバイスアクセスクラス
//
//////////////////////////////////////////////////////////////////////
//#include "DRIHCSh4.h"
#include "Device.h"
//#include <IHCBase/IHRegDefs.h>

namespace Spr {;
DRIHCSh4::VirtualDeviceDa::VirtualDeviceDa(DRIHCSh4* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s D/A Ch %d", realDevice->Name(), ch);
}
DRIHCSh4::VirtualDeviceCounter::VirtualDeviceCounter(DRIHCSh4* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s counter Ch %d", realDevice->Name(), ch);
}
DRIHCSh4::VirtualDevicePio::VirtualDevicePio(DRIHCSh4* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s PIO Ch %d", realDevice->Name(), ch);
}
DRIHCSh4::VirtualDeviceAd::VirtualDeviceAd(DRIHCSh4* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s A/D Ch %d", realDevice->Name(), ch);
}

DRIHCSh4::DRIHCSh4(){
	sprintf(name, "IHC SH4");
	adVoltPerDigit = 24.0f/4096.0f;
	for(int i=0; i<8; ++i){
		count[i] = 0;
		countOffset[i] = 0;
		daOut[i] = 0;
		adIn[i] = 0;
	}
	for(int i=0; i<16; ++i){
		pioLevel[i] = 0;
	}
}
DRIHCSh4::~DRIHCSh4(){
}
bool DRIHCSh4::Init(){
	for(int i=0; i<8; ++i){
		count[i] = 0;
		countOffset[i] = 0;
		daOut[i] = 0;
		adIn[i] = 0;
	}
	for(int i=0; i<16; ++i){
		pioLevel[i] = 0;
	}
	R_CTL = 0x08;		//	エンコーダクリア
	R_CNT_PHS = 0x0;	//	エンコーダ相順設定
	Write();
	return true;
}

void DRIHCSh4::Register(DVVirtualDevicePool& vpool){
	for(int i=0; i<8; i++){
		vpool.Register(new VirtualDevicePio(this, i));
		vpool.Register(new VirtualDeviceCounter(this, i));
		vpool.Register(new VirtualDeviceDa(this, i));
		vpool.Register(new VirtualDeviceAd(this, i));
	}
}
void DRIHCSh4::Update(){
	Read();
	Write();
}
void DRIHCSh4::Write(){
	for(int i=0; i<8; ++i){
		R_DAC_V[i] = daOut[i];
	}
	R_CTL = 0x01; //DAセット
}
void DRIHCSh4::Read(){
	R_CNT_HLD = 0xFF;// エンコーダホールド
	for(int i=0; i<8; ++i){
		unsigned ctIn = R_CNT_V[i];
		//	カウンタは16bitしかないので，前回の値から32bitに拡張する．
		unsigned ct = -count[i];
		unsigned last = ct&0xFFFF;
		int diff = (int)ctIn - (int)last;
		if (diff > 0){
			if (diff > 0x8000){
				diff = int(ctIn-0x10000) - (int)last;
			}
		}else{
			if (diff < -0x8000){
				diff = (int)(ctIn+0x10000) - (int)last;
			}
		}
		ct += diff;
		count[i] = -ct;
	}
	R_CNT_HLD = 0x00;// エンコーダホールド解除
}
	
} //namespace Spr


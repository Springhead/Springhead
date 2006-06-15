#ifndef SPR_DRIHCSh4_H
#define SPR_DRIHCSh4_H

#ifdef _WIN32
// #error This program runs on SH4
#endif

#include "DRRealDevice.h"
#include "DVDaBase.h"
#include "DVAdBase.h"
#include "DVPioBase.h"
#include "DVCounterBase.h"

#include <Base/Env.h>

namespace Spr {

/// DRIHCSh4.cpp: SH4上で動く，デバイスアクセスクラス
class SPR_DLL DRIHCSh4 : public DRRealDevice{
public:
	///	仮想デバイス(DA)
	class VirtualDeviceDa:public DVDaBase{
	protected:
		DRIHCSh4* realDevice;
		int ch;
		char name[100];
	public:
		VirtualDeviceDa(DRIHCSh4* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->DaVoltage(ch, v); }
		virtual void Digit(int v){ realDevice->DaDigit(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	///	仮想デバイス(Counter)
	class VirtualDeviceCounter:public DVCounterBase{
	protected:
		DRIHCSh4* realDevice;
		int ch;
		char name[100];
	public:
		VirtualDeviceCounter(DRIHCSh4* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		///	カウンタ値の設定
		virtual void Count(long c){ realDevice->Count(ch, c); }
		///	カウンタ値の読み出し
		virtual long Count(){ return realDevice->Count(ch); }
		///	名前
		virtual const char* Name() const{ return name; }
	};
	///	仮想デバイス(Pio)
	class VirtualDevicePio:public DVPioBase{
	protected:
		DRIHCSh4* realDevice;
		int ch;
		char name[100];
	public:
		VirtualDevicePio(DRIHCSh4* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Set(int l){ realDevice->Pio(ch, l!=0); }
		virtual int Get(){ return realDevice->Pio(ch) ? 1 : 0; }
		virtual const char* Name() const{ return name; }
	};
	///	仮想デバイス(AD)
	class VirtualDeviceAd:public DVAdBase{
	protected:
		DRIHCSh4* realDevice;
		int ch;
		char name[100];
	public:
		VirtualDeviceAd(DRIHCSh4* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual float Voltage(){ return realDevice->AdVoltage(ch); }
		virtual int Digit(){ return realDevice->AdDigit(ch); }
		virtual const char* Name() const{ return name; }
	};

protected:
	char name[100];
	float adVoltPerDigit;

	long count[8];
	long countOffset[8];
	int daOut[8];
	int adIn[8];
	int pioLevel[16];

public:
	DRIHCSh4();
	virtual ~DRIHCSh4();
	virtual const char* Name() const { return "IHCSh4"; }
	virtual bool Init();
	///	virtual device の登録
	virtual void Register(DVVirtualDevicePool& vpool);

	///	電圧出力
	void DaVoltage(int ch, float v){
		assert(0 <= ch && ch < 8);
		const float DigitPerVolt[]={	//	DA指令値/出力電圧 アンプごとに異なるので計測値をアンプに書いておき，読み出す必要がある．
			415.0f,
			415.0f,
			415.0f,
			415.0f,
			415.0f,
			415.0f,
			415.0f,
			415.0f,
		};
		daOut[ch] = (int)(v * DigitPerVolt[ch]);
	}
	///	電圧出力(数値指定)
	void DaDigit(int ch, int v){
		daOut[ch] = v;
	}
	///	カウンタ値の設定
	void Count(int ch, long c){
		countOffset[ch] = - (count[ch] - c); 
	}
	///	カウンタ値の読み出し
	long Count(int ch){
		return count[ch] + countOffset[ch];
	}
	///	PIOポートの設定
	void Pio(int ch, bool level){
		pioLevel[ch] = level ? 1 : 0;
	}
	///	PIOポートの読み出し
	bool Pio(int ch){
		return pioLevel[ch] == 1;
	}
	///	スイッチ（ポートの上位5-8ビット）の読み出し
	int RotarySwitch(){
		int sw=0;
		for(int i=0; i<4; ++i){
			sw |= pioLevel[i+4] << i;
		}
		return sw;
	}
	///	ADの電圧
	float AdVoltage(int ch){ return adIn[ch] * adVoltPerDigit; }
	///	ADの数値
	int AdDigit(int ch){ return adIn[ch]; }

	///	状態の更新
	virtual void Update();
	void Read();
	void Write();
};

} //namespace Spr

#endif // SPR_DRIHCSh4_H

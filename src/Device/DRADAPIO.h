#ifndef DRADAPIO_H
#define DRADAPIO_H

#include "DRRealDevice.h"
#include "DVDaBase.h"
#include "DVAdBase.h"
#include "tuadapio.h"

namespace Spr {

///	タートル工業のA/D・D/A・PIO用のドライバ.
class SPR_DLL DRAdapio:public DRRealDevice{
public:
	///	仮想デバイス
	class DVDA:public DVDaBase{
	protected:
		int ch;
		DRAdapio* realDevice;
		char name[100];
	public:
		DVDA(DRAdapio* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->DAVoltage(ch, v); }
		virtual void Digit(int d){ realDevice->DADigit(ch, d); }
		virtual const char* Name() const{ return name; }
	};
	class DVAD:public DVAdBase{
	protected:
		int ch;
		DRAdapio* realDevice;
		char name[100];
	public:
		DVAD(DRAdapio* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual int Digit(){ return realDevice->ADDigit(ch); }
		virtual float Voltage(){ return realDevice->ADVoltage(ch); }
		virtual const char* Name() const{ return name; }
	};
protected:
	char name[100];
	int id;
public:
	/**	コンストラクタ
		@param id		ADAPIOが複数ある場合，何番目のデバイスかを指定．*/
	DRAdapio(int id=0);
	virtual ~DRAdapio();
	///	デバイスの名前
	virtual const char* Name() const { return name; }
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual void Register(DVVirtualDevicePool& vpool);
	///	電圧出力
	void DAVoltage(int ch, float v);
	void DADigit(int ch, int d);
	///	電圧入力
	float ADVoltage(int ch);
	int ADDigit(int ch);
	/// 終了処理
	virtual void CloseDevice();
};


}	//	namespace Spr

#endif

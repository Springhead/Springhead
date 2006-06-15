#if !defined(AFX_DRUSBH8_H__618658E7_7169_4BF7_A25E_562BC6BDEE17__INCLUDED_)
#define AFX_DRUSBH8_H__618658E7_7169_4BF7_A25E_562BC6BDEE17__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DVCounterBase.h"
#include "DVDaBase.h"
#include "DRRealDevice.h"
#include <WinBasis/WBWin.h>

namespace Spr {

///	CyverseのUSB８軸モータドライバH8版のドライバ.
class SPR_DLL DRUsbH8Simple:public DRRealDevice
{
public:
	///	仮想デバイス
	class VirtualDeviceDa:public DVDaBase{
	protected:
		int ch;
		DRUsbH8Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceDa(DRUsbH8Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual void Digit(int v){ realDevice->Digit(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	class VirtualDeviceCounter:public DVCounterBase{
	protected:
		int ch;
		DRUsbH8Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceCounter(DRUsbH8Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		///	カウンタ値の設定
		virtual void Count(long c){ realDevice->Count(ch, c); }
		///	カウンタ値の読み出し
		virtual long Count(){ return realDevice->Count(ch); }
		///	名前
		virtual const char* Name() const{ return name; }
	};
//----------------------------------------------------------------------------
protected:
	char name[100];
	int channel;
	HANDLE hSpidar;
	
	int sign[8];
	long count[8];
	long countOffset[8];
	int daOut[8];
public:
	DRUsbH8Simple(int ch);
	///	デバイスの名前
	virtual const char* Name() const { return name; }
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual void Register(DVVirtualDevicePool& vpool);

	///	電圧出力
	void Voltage(int ch, float v);
	///	電圧出力(数値)
	void Digit(int ch, int v);
	///	カウンタ値の設定
	void Count(int ch, long c);
	///	カウンタ値の読み出し
	long Count(int ch);
	///	状態の更新
	virtual void Update();
};

}	//	namespace Spr;

#endif // !defined(AFX_DRUSBH8_H__618658E7_7169_4BF7_A25E_562BC6BDEE17__INCLUDED_)

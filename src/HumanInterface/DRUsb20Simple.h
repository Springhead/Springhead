/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRUsb20Simple.h: DRUsb20Simple クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef SPR_DRUSB20SIMPLE_H
#define SPR_DRUSB20SIMPLE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <HumanInterface/HIRealDevice.h>
#include "DVCounterBase.h"
#include "DVDaBase.h"
#include "DVPioBase.h"
#include <HumanInterface/HIRealDevice.h>
#include <WinBasis/WBWin.h>

namespace Spr {

///	CyverseのUSB2.0(LDR-SPIDAR-AMP)のドライバ
class SPR_DLL DRUsb20Simple : public HIRealDevice{
public:
	SPR_OBJECTDEF(DRUsb20Simple);
	///	仮想デバイス(DA)
	class VirtualDeviceDa:public DVDaBase{
	protected:
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceDa(DRUsb20Simple* r, int c);
		virtual HIRealDeviceIf* RealDevice() { return realDevice->Cast(); }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual void Digit(int v){ realDevice->Digit(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	///	仮想デバイス(Counter)
	class VirtualDeviceCounter:public DVCounterBase{
	protected:
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceCounter(DRUsb20Simple* r, int c);
		virtual HIRealDeviceIf* RealDevice() { return realDevice->Cast(); }
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
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDevicePio(DRUsb20Simple* r, int c);
		virtual HIRealDeviceIf* RealDevice() { return realDevice->Cast(); }
		virtual void Set(int l){ realDevice->Pio(ch, l!=0); }
		virtual int Get(){ return realDevice->Pio(ch) ? 1 : 0; }
		virtual const char* Name() const{ return name; }
	};

//----------------------------------------------------------------------------
protected:
	char name[100];
	int channel;
	HANDLE hSpidar;

	int sign[8]; //DA出力用の符号
	long count[8];
	long countOffset[8];
	int daOut[8];
	int adIn[8];
	int pioLevel[16];

public:
	///	コンストラクタ	chは背面のスイッチになる予定
	DRUsb20Simple(const DRUsb20SimpleDesc& d=DRUsb20SimpleDesc());
	virtual ~DRUsb20Simple();

	///	デバイスの名前
	virtual const char* Name() const { return name; }
	///	初期化
	virtual bool Init();
	///	初期化(チャンネル, PIDVIDを気にせずに初期化)
	virtual bool InitAny();
	///	仮想デバイスの登録
	void Register(HISdkIf* intf);
	
	///	電圧出力
	void Voltage(int ch, float v);
	///	電圧出力(数値指定)
	void Digit(int ch, int v);
	///	カウンタ値の設定
	void Count(int ch, long c);
	///	カウンタ値の読み出し
	long Count(int ch);
	///	状態の更新
	virtual void Update();
	
	///	PIOポートの設定
	void Pio(int ch, bool level);
	///	PIOポートの読み出し
	bool Pio(int ch);
	///	スイッチ（ポートの上位5-8ビット）の読み出し
	int RotarySwitch();

	/// リセット
	virtual void Reset();

	///	USBのファイルハンドル
	HANDLE GetHandle(){ return hSpidar; };
protected:
	virtual void UsbReset();
	virtual void UsbCounterGet();
	virtual void UsbCounterClear();
	virtual void UsbDaSet();
	virtual DWORD UsbVidPid(HANDLE h);
	virtual bool FindDevice(int ch);
	virtual HANDLE UsbOpen(int id);
	virtual bool UsbClose(HANDLE& h);
};

} //namespace Spr

#endif // SPR_DRUSB20SIMPLE_H

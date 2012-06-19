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

#ifndef DR_USB20SIMPLE_H
#define DR_USB20SIMPLE_H

#include <HumanInterface/HIDevice.h>
#include <HumanInterface/SprHIDRUsb.h>
#include <HumanInterface/HumanInterfaceDecl.hpp>

namespace Spr {

/**
	CyverseのUSB2.0(LDR-SPIDAR-AMP)のドライバ
 **/
class SPR_DLL DRUsb20Simple : public HIRealDevice{
public:
	SPR_OBJECTDEF(DRUsb20Simple);
	SPR_DECLMEMBEROF_DRUsb20SimpleDesc;

	///	仮想デバイス(DA)
	class Da: public DVDa{
	public:
		Da(DRUsb20Simple* r, int c):DVDa(r, c){}
		DRUsb20Simple* GetRealDevice() { return realDevice->Cast(); }
		
		virtual void Voltage(float v){ GetRealDevice()->WriteVoltage(portNo, v); }
		virtual void Digit(int v){ GetRealDevice()->WriteDigit(portNo, v); }
		virtual void Update(){ GetRealDevice()->Update(); }
	};
	///	仮想デバイス(Counter)
	class Counter: public DVCounter{
	public:
		Counter(DRUsb20Simple* r, int c):DVCounter(r, c){}
		DRUsb20Simple* GetRealDevice() { return realDevice->Cast(); }
		
		virtual void Count(long c){ GetRealDevice()->WriteCount(portNo, c); }
		virtual long Count(){ return GetRealDevice()->ReadCount(portNo); }
		virtual void Update(){ GetRealDevice()->Update(); }
	};
	///	仮想デバイス(Pio)
	class Pio: public DVPio{
	public:
		Pio(DRUsb20Simple* r, int c):DVPio(r, c){}
		DRUsb20Simple* GetRealDevice() { return realDevice->Cast(); }

		virtual void Set(int l){ GetRealDevice()->WritePio(portNo, l!=0); }
		virtual int Get(){ return GetRealDevice()->ReadPio(portNo) ? 1 : 0; }
		virtual void Update(){ GetRealDevice()->Update(); }
	};

//----------------------------------------------------------------------------
protected:
	void*	hSpidar;

	int		sign[8]; //DA出力用の符号
	long	count[8];
	long	countOffset[8];
	int		daOut[8];
	int		adIn[8];
	int		pioLevel[16];


public:
	///	コンストラクタ	chは背面のスイッチになる予定
	DRUsb20Simple(const DRUsb20SimpleDesc& d=DRUsb20SimpleDesc());
	virtual ~DRUsb20Simple();

	///	初期化
	virtual bool Init();
	///	初期化(チャンネル, PIDVIDを気にせずに初期化)
	//virtual bool InitAny();
	///	仮想デバイスの登録
	//void Register(HISdkIf* intf);
	
	///	電圧出力
	void WriteVoltage(int ch, float v);
	///	電圧出力(数値指定)
	void WriteDigit(int ch, int v);
	///	カウンタ値の設定
	void WriteCount(int ch, long c);
	///	カウンタ値の読み出し
	long ReadCount(int ch);
	///	状態の更新
	virtual void Update();
	
	///	PIOポートの設定
	void WritePio(int ch, bool level);
	///	PIOポートの読み出し
	bool ReadPio(int ch);
	///	スイッチ（ポートの上位5-8ビット）の読み出し
	int ReadRotarySwitch();

	/// リセット
	virtual void Reset();

	///	USBのファイルハンドル
	void* GetHandle(){ return hSpidar; };
protected:
	//	デバイスを見つけ、チャンネルを返す。チャンネルが指定された場合、指定したチャンネルのデバイスしか返さない。
	//	失敗すると-1を返す。
	int FindDevice(int ch=-1);
	//	名前のベース部分
	virtual const char* BaseName() const {return "Cyverse USB2.0 Simple";}

	virtual unsigned	GetVidPid(){ return 0x0CEC0203; }
	virtual void		UsbReset();
	virtual void		UsbCounterGet();
	virtual void		UsbCounterClear();
	virtual void		UsbDaSet();
	virtual unsigned	UsbVidPid(void* h);
	virtual void*		UsbOpen(int id);
	virtual bool		UsbClose(void*& h);
};

} //namespace Spr

#endif // SPR_DRUSB20SIMPLE_H

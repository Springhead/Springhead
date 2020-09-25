/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRUARTMotorDriver.h: DRUARTMotorDriver クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef DR_UARTMotorDriver_H
#define DR_UARTMotorDriver_H

#include <HumanInterface/HIDevice.h>
#include <HumanInterface/HumanInterfaceDecl.hpp>

namespace Spr {

class DRUARTMotorDriverImpl;

/**
	CyverseのUSB2.0(LDR-SPIDAR-AMP)のドライバ
 **/
class SPR_DLL DRUARTMotorDriver : public HIRealDevice {
public:
	SPR_OBJECTDEF(DRUARTMotorDriver);
	SPR_DECLMEMBEROF_DRUARTMotorDriverDesc;
	friend DRUARTMotorDriverImpl;
	std::vector<int> counts, offsets;
	std::vector<int> currents;
	int retry, retryMax;

	///	仮想デバイス(DA)
	class Da : public DVDa {
	public:
		Da(DRUARTMotorDriver* r, int c) :DVDa(r, c) {}
		DRUARTMotorDriver* GetRealDevice() { return realDevice->Cast(); }

		virtual void Voltage(float v) { GetRealDevice()->WriteVoltage(portNo, v); }
		virtual void Digit(int v) { GetRealDevice()->WriteDigit(portNo, v); }
		virtual void Update() { GetRealDevice()->Update(); }
	};
	///	仮想デバイス(Counter)
	class Counter : public DVCounter {
	public:
		Counter(DRUARTMotorDriver* r, int c) :DVCounter(r, c) {}
		DRUARTMotorDriver* GetRealDevice() { return realDevice->Cast(); }

		virtual void Count(long c) { GetRealDevice()->WriteCount(portNo, c); }
		virtual long Count() { return GetRealDevice()->ReadCount(portNo); }
		virtual void Update() { GetRealDevice()->Update(); }
	};
public:
	///	コンストラクタ	chは背面のスイッチになる予定
	DRUARTMotorDriver(const DRUARTMotorDriverDesc& d = DRUARTMotorDriverDesc());
	virtual ~DRUARTMotorDriver();
	int NMotor() { return (int) counts.size(); }

	///	初期化
	virtual bool Init();

	///	電圧出力
	void WriteVoltage(int ch, float v);
	///	電圧出力(数値指定)
	void WriteDigit(int ch, int v);
	///	カウンタ値の設定
	void WriteCount(int ch, long c);
	///	カウンタ値の読み出し
	long ReadCount(int ch);
	void UpdateCounter(int ch, short ct);

	///	状態の更新
	virtual void Update();

	/// リセット
	virtual void Reset();

	///	UARTのファイルハンドル
	void* GetHandle() { return hUART; };
protected:
	void* hUART;
	class DRUARTMotorDriverImpl* impl;

	//	名前のベース部分
	virtual const char* BaseName() const { return "UART Motor Driver by SoftCreature"; }
	//	COMポートの初期化
	bool InitCom();
	bool ClearCom();
	bool ClearComRead();
};

} //namespace Spr

#endif // SPR_DRUARTMotorDriver_H

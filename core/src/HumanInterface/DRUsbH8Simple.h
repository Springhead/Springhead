/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DR_USBH8SIMPLE_H
#define DR_USBH8SIMPLE_H

#include <HumanInterface/HIDevice.h>

namespace Spr {

/**
	CyverseのUSB８軸モータドライバH8版のドライバ.
 **/
class SPR_DLL DRUsbH8Simple: public HIRealDevice{
public:
	SPR_OBJECTDEF_NOIF(DRUsbH8Simple);

	///	仮想デバイス
	class Da: public DVDa{
	public:
		Da(DRUsbH8Simple* r, int c):DVDa(r, c){}
		DRUsbH8Simple* GetRealDevice() { return realDevice->Cast(); }

		virtual void Voltage(float v){ GetRealDevice()->Voltage(portNo, v); }
		virtual void Digit(int v){ GetRealDevice()->Digit(portNo, v); }
	};
	class Counter: public DVCounter{
	public:
		Counter(DRUsbH8Simple* r, int c):DVCounter(r, c){}
		DRUsbH8Simple* GetRealDevice() { return realDevice->Cast(); }

		///	カウンタ値の設定
		virtual void Count(long c){ GetRealDevice()->Count(portNo, c); }
		///	カウンタ値の読み出し
		virtual long Count(){ return GetRealDevice()->Count(portNo); }
	};

protected:
	int		channel;
	void*	hSpidar;
	
	int		sign[8];
	long	count[8];
	long	countOffset[8];
	int		daOut[8];
public:
	DRUsbH8Simple(int ch=0);

	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	//virtual void Register(HIVirtualDevicePool& vpool);

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

#endif

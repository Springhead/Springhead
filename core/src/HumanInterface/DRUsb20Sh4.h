/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRUsb20Sh4.h: DRUsb20Sh4 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef DR_USB20SH4_H
#define DR_USB20SH4_H

#include <HumanInterface/DRUsb20Simple.h>

namespace Spr {;

/**
	USB2.0 SH アンプ のドライバ
 **/
class SPR_DLL DRUsb20Sh4 : public DRUsb20Simple{
protected:
	float adVoltPerDigit;
public:
	SPR_OBJECTDEF(DRUsb20Sh4);
	SPR_DECLMEMBEROF_DRUsb20Sh4Desc;

	///	仮想デバイス(A/Dコンバータ)
	class Ad: public DVAd{
	public:
		Ad(DRUsb20Sh4* r, int c):DVAd(r, c){}
		DRUsb20Sh4* GetRealDevice() { return realDevice->Cast(); }
		
		virtual float Voltage(){ return GetRealDevice()->AdVoltage(portNo); }
		virtual int Digit(){ return GetRealDevice()->AdDigit(portNo); }
	};

	///	コンストラクタ	chは背面のスイッチになる予定
	DRUsb20Sh4(const DRUsb20Sh4Desc& d = DRUsb20Sh4Desc());
	
	virtual bool Init();

	float	AdVoltage(int ch){ return adIn[ch] * adVoltPerDigit; }
	int		AdDigit(int ch){ return adIn[ch]; }
	
	///	状態の更新
	virtual void Update();
	/// リセット
	virtual void Reset();


protected:
	//	名前のベース部分
	virtual const char* BaseName() const {return "Cyverse USB2.0 Sh4";}

	virtual unsigned	GetVidPid(){ return 0x0CEC0205; }
	virtual void UsbUpdate();
	///	512byteの送信
	virtual void UsbSend(unsigned char* outBuffer);
	///	512byteの受信
	virtual void UsbRecv(unsigned char* inBuffer);
};

} //namespace Spr

#endif

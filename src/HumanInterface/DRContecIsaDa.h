/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VR_DRCONTECISADA_H
#define VR_DRCONTECISADA_H

#include <HumanInterface/HIRealDevice.h>
#include "DVDaBase.h"

namespace Spr {

///	ContecのISA D/Aカード用のドライバ.
class SPR_DLL DRContecIsaDa:public HIRealDevice{
public:
	///	仮想デバイス
	class VirtualDevice:public DVDaBase{
	protected:
		int ch;
		DRContecIsaDa* realDevice;
		char name[100];
	public:
		VirtualDevice(DRContecIsaDa* r, int c);
		virtual HIRealDeviceIf* RealDevice() { return realDevice->Cast(); }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual void Digit(int d){ realDevice->Digit(ch, d); }
		virtual const char* Name() const{ return name; }
	};
protected:
	char name[100];
	///	D/AのIOアドレス
	int address;
public:
	/**	コンストラクタ
		@param address		D/AのI/Oアドレス	*/
	DRContecIsaDa(int address);
	///	デバイスの名前
	virtual const char* Name() const { return name; }
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual void Register(HIVirtualDevicePool& vpool);

	///	電圧出力
	void Voltage(int ch, float v);
	///	電圧出力(数値指定)
	void Digit(int ch, int v);
};


}	//	namespace Spr

#endif
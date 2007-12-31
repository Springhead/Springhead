/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VR_DRCONTECISACOUNTER_H
#define VR_DRCONTECISACOUNTER_H

#include <HumanInterface/HIRealDevice.h>
#include "DVCounterBase.h"

namespace Spr {

///	ContecのISA D/Aカード用のドライバ.
class SPR_DLL DRContecIsaCounter:public HIRealDevice{
public:
	///	仮想デバイス
	class VirtualDevice:public DVCounterBase{
	protected:
		int ch;
		DRContecIsaCounter* realDevice;
		char name[100];
	public:
		VirtualDevice(DRContecIsaCounter* r, int c);
		virtual HIRealDeviceIf* RealDevice() { return realDevice->Cast(); }
		///	カウンタ値の設定
		virtual void Count(long c){ realDevice->Count(ch, c); }
		///	カウンタ値の読み出し
		virtual long Count(){ return realDevice->Count(ch); }
		///	名前
		virtual const char* Name() const{ return name; }
	};
protected:
	char name[100];
	///	カウンタボードのIOアドレス
	int address;

public:
	/**	コンストラクタ
		@param address		カウンタボードのI/Oアドレス
	*/
	DRContecIsaCounter(int address);
	///	デバイスの名前
	virtual const char* Name() const { return name; }
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual void Register(HIVirtualDevicePool& vpool);
	///	カウンタ値の設定
	void Count(int ch, long c);
	///	カウンタ値の読み出し
	long Count(int ch);
};
}	//	namespace Spr

#endif
/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VR_DRCONTECISACOUNTER_H
#define VR_DRCONTECISACOUNTER_H

#if ( _MSC_VER > 1500 )

#include <HumanInterface/HIDevice.h>

namespace Spr {

///	ContecのISA D/Aカード用のドライバ.
class SPR_DLL DRContecIsaCounter: public HIRealDevice{
public:
	SPR_OBJECTDEF_NOIF(DRContecIsaCounter);

	///	仮想デバイス
	class DV: public DVCounter{
	public:
		DV(DRContecIsaCounter* r, int c):DVCounter(r, c){}
		DRContecIsaCounter* GetRealDevice() { return realDevice->Cast(); }

		///	カウンタ値の設定
		virtual void Count(long c){ GetRealDevice()->Count(portNo, c); }
		///	カウンタ値の読み出し
		virtual long Count(){ return GetRealDevice()->Count(portNo); }
	};
protected:
	///	カウンタボードのIOアドレス
	int address;

public:
	/**	コンストラクタ
		@param address		カウンタボードのI/Oアドレス
	*/
	DRContecIsaCounter(int address = 0);

	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	//virtual void Register(HIVirtualDevicePool& vpool);
	///	カウンタ値の設定
	void Count(int ch, long c);
	///	カウンタ値の読み出し
	long Count(int ch);
};
}	//	namespace Spr

#endif	// ( _MSC_VER > 1500 )

#endif

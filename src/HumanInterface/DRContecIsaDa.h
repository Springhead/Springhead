/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VR_DRCONTECISADA_H
#define VR_DRCONTECISADA_H

#if ( _MSC_VER > 1500 )

#include <HumanInterface/HIDevice.h>

namespace Spr {

///	ContecのISA D/Aカード用のドライバ.
class SPR_DLL DRContecIsaDa: public HIRealDevice{
public:
	SPR_OBJECTDEF_NOIF(DRContecIsaDa);

	///	仮想デバイス
	class Da: public DVDa{
	public:
		Da(DRContecIsaDa* r, int c):DVDa(r, c){}
		DRContecIsaDa* GetRealDevice() { return realDevice->Cast(); }
		
		virtual void Voltage(float v){ GetRealDevice()->Voltage(portNo, v); }
		virtual void Digit(int d){ GetRealDevice()->Digit(portNo, d); }
	};
protected:
	///	D/AのIOアドレス
	int address;
public:
	/**	コンストラクタ
		@param address		D/AのI/Oアドレス	*/
	DRContecIsaDa(int address = 0);
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	//virtual void Register(HIVirtualDevicePool& vpool);

	///	電圧出力
	void Voltage(int ch, float v);
	///	電圧出力(数値指定)
	void Digit(int ch, int v);
};


}	//	namespace Spr

#endif	// ( _MSC_VER > 1500 )

#endif

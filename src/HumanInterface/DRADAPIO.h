/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DRADAPIO_H
#define DRADAPIO_H

#include <HumanInterface/HIDevice.h>

namespace Spr {

///	タートル工業のA/D・D/A・PIO用のドライバ.
class SPR_DLL DRAdapio: public HIRealDevice{
public:
	SPR_OBJECTDEF_NOIF(DRAdapio);

	///	仮想デバイス
	class Da: public DVDa{
	public:
		Da(DRAdapio* r, int c):DVDa(r, c){}
		DRAdapio* GetRealDevice() { return realDevice->Cast(); }

		virtual void Voltage(float v){ GetRealDevice()->DAVoltage(portNo, v); }
		virtual void Digit(int d){ GetRealDevice()->DADigit(portNo, d); }
	};
	class Ad: public DVAd{
	public:
		Ad(DRAdapio* r, int c):DVAd(r, c){}
		DRAdapio* GetRealDevice() { return realDevice->Cast(); }

		virtual int Digit(){ return GetRealDevice()->ADDigit(portNo); }
		virtual float Voltage(){ return GetRealDevice()->ADVoltage(portNo); }
	};
protected:
	int id;
public:
	/**	コンストラクタ
		@param id		ADAPIOが複数ある場合，何番目のデバイスかを指定．*/
	DRAdapio(int id=0);
	virtual ~DRAdapio();

	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	//virtual void Register(HIVirtualDevicePool& vpool);

	///	電圧出力
	void DAVoltage(int ch, float v);
	void DADigit(int ch, int d);
	
	///	電圧入力
	float ADVoltage(int ch);
	int ADDigit(int ch);
	
	/// 終了処理
	void CloseDevice();
};

}	//	namespace Spr

#endif

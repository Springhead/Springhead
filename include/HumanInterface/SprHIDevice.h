/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprDRDevice.h
 *	@brief ヒューマンインタフェースを作るための材料になるデバイスのクラス．たとえば，D/A,A/D,PIOなど．
*/

#ifndef SPR_HIDevice_H
#define SPR_HIDevice_H
#include <SprFoundation.h>

namespace Spr{;
struct HISdkIf;

/**	\addtogroup	gpHumanInterface	*/
//@{


///	実デバイス．インタフェースカードとか，USBデバイスとか，
struct HIRealDeviceIf:  public ObjectIf{
	IF_DEF(HIRealDevice);
	///	デバイスの名前．クラスと対応するボードのIDなどから自動で決まる名前．設定不可．
	virtual const char* Name()=0;
	///	初期化
	virtual bool Init()=0;
	///	仮想デバイスの登録
	virtual void Register(HISdkIf* sdk)=0;
	///	状態の更新
	virtual void Update()=0;
};

///	バーチャルデバイス．A/D, D/Aの1チャンネル分とか，機能としてのデバイス．
struct HIVirtualDeviceIf:  public ObjectIf{
	IF_DEF(HIVirtualDevice);
public:
	///	デバイスの名前
	virtual const char* Name() const=0;
	///	デバイスの種類
	virtual const char* Type() const=0;
	///	実デバイスへのポインタ
	virtual HIRealDeviceIf* RealDevice()=0;
	///	ダンプ出力
	virtual void Print(std::ostream& o) const=0;
	///
	virtual void Update()=0;
};

//@}
}

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	SPR_IFDEF(HIRealDevice);
	///	デバイスの名前．クラスと対応するボードのIDなどから自動で決まる名前．設定不可．
	const char* Name();
	///	初期化
	bool Init();
	///	仮想デバイスの登録
	void Register(HISdkIf* sdk);
	///	状態の更新
	void Update();
};

///	バーチャルデバイス．A/D, D/Aの1チャンネル分とか，機能としてのデバイス．
struct HIVirtualDeviceIf:  public ObjectIf{
	SPR_IFDEF(HIVirtualDevice);
public:
	///	デバイスの名前
	const char* Name() const;
	///	デバイスの種類
	const char* Type() const;
	///	実デバイスへのポインタ
	HIRealDeviceIf* RealDevice();
	///	ダンプ出力
	void Print(std::ostream& o) const;
	///
	void Update();
};

//@}
}

#endif

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

struct DVAdBaseIf:public HIVirtualDeviceIf{
	SPR_IFDEF(DVAdBase);
public:
	///	入力デジタル値の取得
	int Digit();
	///	入力電圧の取得
	float Voltage();
	///	デバイスの名前
	const char* Name() const;
	///	デバイスの種類
	const char* Type() const;
};
struct DVDaBaseIf:public HIVirtualDeviceIf{
	SPR_IFDEF(DVDaBase);
public:
	///	出力するデジタル値の設定
	void Digit(int d);
	///	出力電圧の設定
	void Voltage(float volt);
	///	デバイスの名前
	const char* Name() const;
	///	デバイスの種類
	const char* Type();
};
struct DVCounterBaseIf:public HIVirtualDeviceIf{
	SPR_IFDEF(DVCounterBase);
public:
	///	カウンタ値の設定
	void Count(long count);
	///	カウンタ値の読み出し
	long Count();
	///	デバイスの名前
	const char* Name() const;
	///	デバイスの種類
	const char* Type() const;
};

///	入出力ポートのための定数の定義などだけを行う．
struct DVPortEnum{
	enum TLevel {LEVEL_LO, LEVEL_HI};
	enum TDir {DIR_IN, DIR_OUT};
};
///
struct DVPioBaseIf:public HIVirtualDeviceIf, public DVPortEnum{
	SPR_IFDEF(DVPioBase);
public:
	///	ポートのロジックレベルの入力。Hi:true Lo:false
	int Get();
	///	ポートのロジックレベルの出力。Hi:true Lo:false
	void Set(int l);
	///	デバイスの名前
	const char* Name() const;
	///	デバイスの種類
	const char* Type() const;
};


//@}
}

#endif

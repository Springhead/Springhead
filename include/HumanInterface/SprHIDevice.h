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

#include <Foundation/SprObject.h>

namespace Spr{;
struct HISdkIf;

/**	\addtogroup	gpHumanInterface	*/
//@{

/**
	デバイスの基本クラス
 **/
struct HIDeviceIf : NamedObjectIf{
	SPR_IFDEF(HIDevice);
private:
	///	デバイスの名前は、クラスと対応するボードのIDなどから自動で決まるので、設定不可。
	void SetName(const char* n);	
};

struct HIVirtualDeviceIf;

/**
	実デバイス．インタフェースカードとか，USBデバイスとか，
 **/
struct HIRealDeviceIf : HIDeviceIf{
	SPR_IFDEF(HIRealDevice);

	/// デバイス名を取得
	//UTString	GetDeviceName();

	///	初期化と仮想デバイスの登録
	bool Init();
	///	仮想デバイスの登録
	//void Register(HISdkIf* sdk);

	/// 仮想デバイスの取得
	HIVirtualDeviceIf*	Rent(const IfInfo* ii, const char* name, int portNo);
	/// 仮想デバイスの返却
	bool				Return(HIVirtualDeviceIf* dv);

	///	状態の更新
	void Update();
};

/**
	バーチャルデバイス．A/D, D/Aの1チャンネル分とか，機能としてのデバイス．
 **/
struct HIVirtualDeviceIf : HIDeviceIf{
	SPR_IFDEF(HIVirtualDevice);

	/// ポート番号を取得（ポート番号を持つデバイスのみ）
	int	GetPortNo() const;
	
	/// 使用状態の取得
	bool IsUsed();

	///	実デバイスへのポインタ
	HIRealDeviceIf* GetRealDevice();
	
	///
	void Update();
};

/**
	A/D変換
 **/
struct DVAdIf : HIVirtualDeviceIf{
	SPR_IFDEF(DVAd);

	///	入力デジタル値の取得
	int Digit();
	///	入力電圧の取得
	float Voltage();
};

/**
	D/A変換
 **/
struct DVDaIf : HIVirtualDeviceIf{
	SPR_IFDEF(DVDa);

	///	出力するデジタル値の設定
	void Digit(int d);
	///	出力電圧の設定
	void Voltage(float volt);
};

/**
	カウンタ
 **/
struct DVCounterIf : HIVirtualDeviceIf{
	SPR_IFDEF(DVCounter);

	///	カウンタ値の設定
	void Count(long count);
	///	カウンタ値の読み出し
	long Count();
};

///	入出力ポートのための定数の定義などだけを行う．
struct DVPortEnum{
	enum TLevel {LEVEL_LO, LEVEL_HI};
	enum TDir {DIR_IN, DIR_OUT};
};

/**
	パラレルI/O
 **/
struct DVPioIf : public HIVirtualDeviceIf, public DVPortEnum{
	SPR_IFDEF(DVPio);

	///	ポートのロジックレベルの入力。Hi:true Lo:false
	int Get();
	///	ポートのロジックレベルの出力。Hi:true Lo:false
	void Set(int l);
};

/**
	力の入力
 */
struct DVForceIf : public HIVirtualDeviceIf{
	SPR_IFDEF(DVForce);

	///	自由度の取得
	int GetDOF();
	///	力の取得
	float GetForce(int ch);
	///	力の取得
	void GetForce3(Vec3f& f);
	///	力の取得
	void GetForce6(Vec3f& f, Vec3f& t);
};

//@}
}

#endif

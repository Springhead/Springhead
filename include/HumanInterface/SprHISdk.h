/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprHISdk.h
 *	@brief ヒューマンインタフェースSDK
*/
#ifndef SPR_HISdkIF_H
#define SPR_HISdkIF_H
#include <HumanInterface/SprHIBase.h>

namespace Spr{;

/** \addtogroup gpPhysics */
//@{

struct PHSceneIf;
struct PHSceneDesc;

//	自動ロードのための中身なしのデスクリプタ
struct HISdkDesc{
};

///	物理シミュレーションSDK
struct HISdkIf : public NameManagerIf{
	IF_DEF(HISdk);
	//	API
	///	ヒューマンインタフェースの作成
	virtual HIBaseIf* CreateHumanInterface(const char* name)=0;
	///	実デバイス(ボードやUSBインタフェースなどのRawデバイス)の作成&登録
	virtual void RegisterDevice(const char* name)=0;

	//	SDK生成、登録関数
	///	HISdkのインスタンスを作成
	static HISdkIf* SPR_CDECL CreateSdk();
	///	HISdkをファイルローダーなどに登録
	static void SPR_CDECL RegisterSdk();
};

//@}

}	//	namespace Spr
#endif

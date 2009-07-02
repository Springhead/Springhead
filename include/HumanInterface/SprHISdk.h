/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

/** \addtogroup gpHumanInterface ヒューマンインタフェースSDK */
//@{

struct PHSceneIf;
struct PHSceneDesc;

//	自動ロードのための中身なしのデスクリプタ
struct HISdkDesc{
};

///	物理シミュレーションSDK
struct HISdkIf : public NameManagerIf{
	SPR_IFDEF(HISdk);

	///	SDKの初期化
	void Init();
	///	作成・登録したRealDevice/VirtualDevice/HumanInterfaceをすべて削除
	void Clear();
	///	Rent device.
	HIVirtualDeviceIf* RentVirtualDevice(const char* type, const char* name=NULL);
	HIVirtualDeviceIf* RentVirtualDeviceNo(const char* type, int No, const char* name=NULL );
	///	Return virutal device.
	bool ReturnVirtualDevice(HIVirtualDeviceIf* dev);
	
	///	Create and register real device.
	bool AddRealDevice(const IfInfo* keyInfo, const void* desc=NULL);

	///	Get real device.
	HIRealDeviceIf* FindRealDevice(const char* name=NULL);
	///	ヒューマンインタフェースの作成
	UTRef<HIBaseIf> CreateHumanInterface(const IfInfo* info);
	///	ヒューマンインタフェースの作成
	UTRef<HIBaseIf> CreateHumanInterface(const char* name);

	//	SDK生成、登録関数
	///	HISdkのインスタンスを作成
	static HISdkIf* SPR_CDECL CreateSdk();
	///	HISdkをファイルローダーなどに登録
	static void SPR_CDECL RegisterSdk();
};

//@}

}	//	namespace Spr
#endif

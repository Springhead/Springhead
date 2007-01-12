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

	///	SDKの初期化
	virtual void Init()=0;
	///	作成・登録したRealDevice/VirtualDevice/HumanInterfaceをすべて削除
	virtual void Clear()=0;
	///	Rent virtual device.
	virtual HIVirtualDeviceIf* RentVirtualDevice(const char* type, const char* name=NULL)=0;
	///	Return virutal device.
	virtual bool ReturnVirtualDevice(HIVirtualDeviceIf* dev)=0;
	
	///	Create and register real device.
	virtual bool AddRealDevice(const IfInfo* keyInfo, const void* desc=NULL)=0;

	///	Get real device.
	virtual HIRealDeviceIf* FindRealDevice(const char* name=NULL)=0;
	///	ヒューマンインタフェースの作成
	virtual UTRef<HIBaseIf> CreateHumanInterface(const IfInfo* info, const void* desc)=0;
	///	ヒューマンインタフェースの作成
	virtual UTRef<HIBaseIf> CreateHumanInterface(const char* name, const char* desc)=0;

	//	SDK生成、登録関数
	///	HISdkのインスタンスを作成
	static HISdkIf* SPR_CDECL CreateSdk();
	///	HISdkをファイルローダーなどに登録
	static void SPR_CDECL RegisterSdk();
};

//@}

}	//	namespace Spr
#endif

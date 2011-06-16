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
#include <Foundation/SprScene.h>

namespace Spr{;

/** \addtogroup gpHumanInterface ヒューマンインタフェースSDK */
//@{

/** デバイス名一覧
	[DR] ADAPIO n
	[DV] ADAPIO n D/A ch m
	[DV] ADAPIO n A/D ch m
	
	[DR] Contec ISA Counter at 0xXXX
	[DV] Contec ISA Counter at 0xXXX Channel m

	[DR] Contec ISA D/A at 0xXXX
	[DV] Contec ISA D/A at 0xXXX Channel m

	[DR] KeyMouseWin32
	[DV] KeyMouseWin32

	[DR] KeyMouseGLUT
	[DV] KeyMouseGLUT

	[DR] Nitta 6DOF force sensor at 0xXXX
	[DV] Nitta 6DOF force sensor at 0xXXX

	[DR] Tokin 3D motion sensor
	[DV] Tokin 3D motion sensor input port ch m

	[DR] Cyberse USB2.0 SH4 #%d
	[DV] Cyberse USB2.0 SH4 #%d A/D Ch %d

	[DR] Cyberse USB2.0 Simple #%d
	[DV] Cyberse USB2.0 Simple #%d D/A Ch %d
	[DV] Cyberse USB2.0 Simple #%d counter Ch %d
	[DV] Cyberse USB2.0 Simple #%d PIO Ch %d

	[DR] Cyberse USB H8 Simple #%d
	[DV] Cyberse USB H8 Simple #%d D/A Ch %d
	[DV] Cyberse USB H8 Simple #%d counter Ch %d


 */

/** 仮想デバイス名一覧
	

 */

//	自動ロードのための中身なしのデスクリプタ
struct HISdkDesc{
};

///	物理シミュレーションSDK
struct HISdkIf: public NameManagerIf{
	SPR_IFDEF(HISdk);

	///	SDKの初期化
	//void Init();
	
	/** @brief	仮想デバイスを借りてくる	Rent virtual device from device pool
		@param	type	デバイスの種類
		@param	name	デバイスの名前
		@param	portNo	ポート番号

		種類，名前およびポート番号がの合致する未使用のデバイスを探し，
		最初に見つかったデバイスを使用状態にしてから返す．
		nameがNULLの場合は名前は任意．
		portNoが-1の場合はポート番号は任意．
		該当するデバイスが見つからない場合はNULLが返される．
	 */
	HIVirtualDeviceIf* RentVirtualDevice(const IfInfo* ii, const char* name=NULL, int portNo = -1);
	//HIVirtualDeviceIf* RentVirtualDeviceNo(const char* type, int No, const char* name=NULL );
	
	/** @brief	仮想デバイスを返却する	Return virtual device to device pool
		デバイスを未使用状態にする
	 */
	bool ReturnVirtualDevice(HIVirtualDeviceIf* dev);
	
	/**	@brief 実デバイスを登録	Create and register real device.
		@param IfInfo	作成するデバイスのIfInfo
		@param desc		作成するデバイスのディスクリプタ
	 */
	HIRealDeviceIf* AddRealDevice(const IfInfo* keyInfo, const void* desc=NULL);
	/** @brief 実キーボード・マウスを登録
		@param IfInfo	
		@param win		関連づけるウィンドウ
		@param desc
	 */
	//bool AddRealKeyMouse(const IfInfo* keyInfo, void* win, const void* desc=NULL);

	/**	@brief 実デバイスを取得	Get real device.
	 */
	HIRealDeviceIf* FindRealDevice(const char* name);
	HIRealDeviceIf*	FindRealDevice(const IfInfo* ii);

	/**	@brief	ヒューマンインタフェースの作成
		@param	info	作成するHIの型情報
	 **/
	HIBaseIf* CreateHumanInterface(const IfInfo* info);
	/**	@brief	ヒューマンインタフェースの作成
		@param	name	作成するHIの型名
	 **/
	HIBaseIf* CreateHumanInterface(const char* name);

	//	SDK生成、登録関数
	///	HISdkのインスタンスを作成
	static HISdkIf* SPR_CDECL CreateSdk();
	///	HISdkをファイルローダーなどに登録
	static void SPR_CDECL RegisterSdk();
};

//@}

}	//	namespace Spr
#endif

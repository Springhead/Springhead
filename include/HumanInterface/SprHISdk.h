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

//	自動ロードのための中身なしのデスクリプタ
struct HISdkDesc{
};

///	物理シミュレーションSDK
struct HISdkIf: public NameManagerIf{
	SPR_IFDEF(HISdk);

	/** @brief	仮想デバイスを借りてくる	Rent virtual device from device pool
		@param	type	仮想デバイスの種類
		@param	name	実デバイスの名前
		@param	portNum	実デバイス内の何個目の仮想デバイスを使用するか指定

		仮想デバイスの種類，実デバイス名が合致するデバイスを探す。
		デバイスが見つかり、未使用の場合は、使用状態にして返す。
		デバイスが見つからないか、使用中の場合はNULLを返す。
		nameがNULLの場合実デバイス名は任意。
		portNumが-1の時は最初の空いているデバイス。	*/
	HIVirtualDeviceIf* RentVirtualDevice(const IfInfo* ii, const char* rname=NULL, int portNum = -1);
	HIVirtualDeviceIf* RentVirtualDevice(const char* itype, const char* rname=NULL, int portNum = -1);
	
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

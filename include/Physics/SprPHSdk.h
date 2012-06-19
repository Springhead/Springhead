/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHSdk.h
 *	@brief 物理シミュレーションSDK
*/
#ifndef SPR_PHSDKIF_H
#define SPR_PHSDKIF_H

#include <Collision/SprCDShape.h>
#include <Physics/SprPHScene.h>

namespace Spr{;

/** \addtogroup gpPhysics 物理エンジンSDK */
//@{

//	自動ロードのための中身なしのデスクリプタ
struct PHSdkDesc{
};

///	物理シミュレーションSDK
struct PHSdkIf : public SdkIf{
	SPR_IFDEF(PHSdk);
	//	API
	///	Sceneの作成
	PHSceneIf* CreateScene(const PHSceneDesc& desc = PHSceneDesc());
	///	Sceneの数を取得
	int NScene();
	///	Sceneの取得
	PHSceneIf* GetScene(int i);
	/// Sceneのマージ
	void MergeScene(PHSceneIf* scene0, PHSceneIf* scene1);
	/** @brief Shapeを作成する
		@param desc 作成するShapeのディスクリプタ
		@return Shapeのインタフェース
	 */
	CDShapeIf* CreateShape(const IfInfo* ii, const CDShapeDesc& desc);
	template <class T> CDShapeIf* CreateShape(const T& desc){
		return CreateShape(T::GetIfInfo(), desc);
	}
	
	
	///	Shapeの数
	int NShape();

	///	Shapeの取得
	CDShapeIf* GetShape(int i);

	//	SDK生成、登録関数
	///	PHSdkのインスタンスを作成
	static PHSdkIf* SPR_CDECL CreateSdk();
	///	PHSdkをファイルローダーなどに登録
	static void SPR_CDECL RegisterSdk();
};

//@}

}	//	namespace Spr
#endif

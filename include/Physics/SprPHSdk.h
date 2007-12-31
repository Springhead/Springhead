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

namespace Spr{;

/** \addtogroup gpPhysics */
//@{

struct PHSceneIf;
struct PHSceneDesc;

//	自動ロードのための中身なしのデスクリプタ
struct PHSdkDesc{
};

///	物理シミュレーションSDK
struct PHSdkIf : public SdkIf{
	IF_DEF(PHSdk);
	//	API
	///	Sceneの作成
	virtual PHSceneIf* CreateScene()=0;
	///	Sceneの作成
	virtual PHSceneIf* CreateScene(const PHSceneDesc& desc)=0;
	///	Sceneの数を取得
	virtual int NScene()=0;
	///	Sceneの取得
	virtual PHSceneIf* GetScene(size_t i)=0;
	/// Sceneのマージ
	virtual void MergeScene(PHSceneIf* scene0, PHSceneIf* scene1)=0;
	/** @brief Shapeを作成する
		@param desc 作成するShapeのディスクリプタ
		@return Shapeのインタフェース
	 */
	virtual CDShapeIf* CreateShape(const IfInfo* ii, const CDShapeDesc& desc)=0;
	template <class T> CDShapeIf* CreateShape(const T& desc){
		return CreateShape(T::GetIfInfo(), desc);
	}
	
	
	///	Shapeの数
	virtual int NShape()=0;

	///	Shapeの取得
	virtual CDShapeIf* GetShape(int i)=0;

	//	SDK生成、登録関数
	///	PHSdkのインスタンスを作成
	static PHSdkIf* SPR_CDECL CreateSdk();
	///	PHSdkをファイルローダーなどに登録
	static void SPR_CDECL RegisterSdk();
};

//@}

}	//	namespace Spr
#endif

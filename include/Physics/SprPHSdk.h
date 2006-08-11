/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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
struct PHSdkIf : public NameManagerIf{
	IF_DEF(PHSdk);
	///	Sceneの作成
	virtual PHSceneIf* CreateScene()=0;
	///	Sceneの作成
	virtual PHSceneIf* CreateScene(const PHSceneDesc& desc)=0;
	///	Sceneの数を取得
	virtual int NScene()=0;
	///	Sceneの取得
	virtual PHSceneIf* GetScene(size_t i)=0;
	
	///	Shape作成
	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc)=0;
	///	Shapeの数
	virtual int NShape()=0;
	///	Shapeの取得
	virtual CDShapeIf* GetShape(int i)=0;
	//virtual CDShapeIf** GetShapes()=0;
};
PHSdkIf* SPR_CDECL CreatePHSdk();

void PHRegisterSdk();

//@}

}	//	namespace Spr
#endif

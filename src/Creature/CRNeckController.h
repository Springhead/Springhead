/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRNECKCONTROLLER_H
#define CRNECKCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 首（頭部）運動コントローラ
*/
class CRNeckController : public SceneObject, public CRNeckControllerIfInit, public CRNeckControllerDesc {
private:
	/** @brief 注視点
	*/
	Vec3d pos, vel;

public:
	OBJECTDEF(CRNeckController, SceneObject);
	ACCESS_DESC_STATE(CRNeckController);

	CRNeckController(){}
	CRNeckController(const CRNeckControllerDesc& desc, SceneIf* s=NULL) : CRNeckControllerDesc(desc) {
		if(s){SetScene(s);}
	}

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel);

	/** @brief 制御処理を実行する
	*/
	virtual void Step();
};
}
//@}

#endif//CREYECONTROLLER_H
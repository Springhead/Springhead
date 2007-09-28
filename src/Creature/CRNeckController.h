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

#include "CRController.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 首（頭部）運動コントローラ
*/
class CRNeckController : public CRController, public CRNeckControllerIfInit, public CRNeckControllerDesc {
private:
	/// 注視点
	Vec3d pos, vel;

	/// 注意の度合
	float attractiveness;

	/// 制御につかう剛体
	PHSolidIf *soHead, *soNeck, *soChest, *soAbdomen;

	/// 制御につかう関節
	PHHingeJointIf *joNeckHeadX, *joChestNeckY, *joAbdomenChest;

	/// 基準方向（ここを中心に割合移動する，attractivenessが一定値を超えるとその方向へ変更される）
	double origX, origZ;

public:
	OBJECTDEF(CRNeckController, CRController);
	ACCESS_DESC_STATE(CRNeckController);

	CRNeckController(){}
	CRNeckController(const CRNeckControllerDesc& desc, CRCreatureIf* c=NULL)
		: CRNeckControllerDesc(desc)
		, CRController((const CRControllerDesc&)desc, c)
	{
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();

	/** @brief 制御処理を実行する
	*/
	virtual void Step();

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel, float attractiveness);
};
}
//@}

#endif//CRNECKCONTROLLER_H

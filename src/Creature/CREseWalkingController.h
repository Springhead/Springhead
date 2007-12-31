/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRESEWALKINGCONTROLLER_H
#define CRESEWALKINGCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include "CRController.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 偽歩行コントローラ
*/
class CREseWalkingController : public CRController, public CREseWalkingControllerIfInit, public CREseWalkingControllerDesc {
private:
	/// 制御対象となるボディの腰の剛体
	PHSolidIf* soWaist;

	/// ボディを固定するための不可視剛体と関節
	PHSolidIf* soFixpoint;
	PHJointIf* joFixpoint;

	/// 歩行スピード
	float speed;

	/// 転回角度
	float rotation;

public:
	OBJECTDEF(CREseWalkingController, CRController);
	ACCESS_DESC(CREseWalkingController);

	CREseWalkingController(){
		speed    = 0.0f;
		rotation = 0.0f;
	}
	CREseWalkingController(const CREseWalkingControllerDesc& desc, CRCreatureIf* c=NULL) 
		: CREseWalkingControllerDesc(desc) 
		, CRController((const CRControllerDesc&)desc, c)
	{
		speed    = 0.0f;
		rotation = 0.0f;
	}

	/** @brief 初期化を行う
	*/
	virtual void Init();

	/** @brief 制御を実行する
	*/
	virtual void Step();

	/** @brief 歩行の速度を設定する
	*/
	virtual void SetSpeed(float speed);

	/** @brief 転回角度を設定する
	*/
	virtual void SetRotationAngle(float rot);

	/** @brief 位置を設定する（初期状態決定のため）
	*/
	virtual void SetPos(Vec3f pos);
};
}
//@}

#endif//CRESEWALKINGCONTROLLER_H

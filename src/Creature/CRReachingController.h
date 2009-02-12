/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRREACHINGCONTROLLER_H
#define CRREACHINGCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>

#include "CRController.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 到達運動コントローラ
*/
class CRReachingController : public CRController, public CRReachingControllerDesc {
private:
	/// 経過時間
	float time;

	/// 到達目標時間
	float period;

	/// 最終目標位置・速度・姿勢・角速度
	Vec3f fP, fV;

	/// 有効かどうか
	bool bActive;

	/// 制御対象のikcp
	PHIKPosCtlIf* ikcp;

public:
	SPR_OBJECTDEF(CRReachingController);
	ACCESS_DESC(CRReachingController);

	CRReachingController(){}
	CRReachingController(const CRReachingControllerDesc& desc, CRCreatureIf* c=NULL) 
		: CRController((const CRControllerDesc&)desc, c)
		, CRReachingControllerDesc(desc)
	{
		bActive = false;
	}

	/** @ brief 初期化を実行する
	*/
	virtual void Init();

	/** @ brief 制御のステップを実行する
	*/
	virtual void Step();

	/** @brief 位置を到達させる
	*/
	virtual void Reach(PHIKPosCtlIf* ikcp, Vec3d pos, Vec3d v, float t);

	/** @brief 目標位置の再設定
	*/
	void SetPos(Vec3f pos){ fP = pos; }
};
}
//@}

#endif//CRREACHINGCONTROLLER_H

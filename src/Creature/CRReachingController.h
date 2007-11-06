/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRREACHINGCONTROLLER_H
#define CRREACHINGCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include "CRController.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 到達運動コントローラ
*/
class CRReachingController : public CRController, public CRReachingControllerIfInit , public CRReachingControllerDesc {
private:
	/// 制御用のバネダンパ
	PHSpringIf *springDirect, *springHinged;

	/// 制御用の仮目標剛体
	PHSolidIf  *soTargetDirect, *soTargetHinged;

	/// 経過時間
	float time;

	/// 到達目標時間
	float period;

	/// 達成後の待機時間
	float offset;

	/// 最終目標位置・速度・姿勢・角速度
	Vec3f finalPos, finalVel, finalAngV;
	Quaterniond finalQuat;

	/// 目標剛体のローカル座標
	Vec3f localPos;

	/// 制御モード
	CRReachingControllerIf::ConstraintMode constraintMode;

	/// 有効かどうか
	bool bActive;

	/// 制御対象のボディ
	CRHingeHumanBodyIf* body;

public:
	OBJECTDEF(CRReachingController, CRController);
	ACCESS_DESC(CRReachingController);

	CRReachingController(){}
	CRReachingController(const CRReachingControllerDesc& desc, CRCreatureIf* c=NULL) 
		: CRController((const CRControllerDesc&)desc, c)
		, CRReachingControllerDesc(desc)
	{
	}

	/** @ brief 初期化を実行する
	*/
	virtual void Init();

	/** @ brief 制御のステップを実行する
	*/
	virtual void Step();

	/** @brief 制御対象の剛体を返す
	*/
	virtual PHSolidIf* GetSolid();

	/** @brief 目標位置を設定する
		@param p 目標位置
		@param v 目標の速度
	*/
	virtual void SetTargetPos(Vec3f p, Vec3f v);

	/** @brief 目標姿勢を設定する
		@param q 目標姿勢
		@param av 目標角速度
	*/
	virtual void SetTargetOri(Quaterniond q, Vec3f av);

	/** @brief 到達目標時間を設定する
		@param t 目標到達時間
	*/
	virtual void SetTargetTime(float t);

	/** @brief 到達運動を開始する
		@param mode 到達の拘束モード
		@param keeptime 到達運動終了後に保持を続ける時間（負なら保持を続ける）
	*/
	virtual void Start(CRReachingControllerIf::ConstraintMode mode, float keeptime);

	/** @brief 制御の残り時間を返す
	*/
	virtual float GetRemainingTime();

	/** @brief 到達状況を返す
	*/
	virtual CRReachingControllerIf::ReachState GetReachState();

	/** @brief 到達状態をやめる
	*/
	virtual void Reset();
};
}
//@}

#endif//CRREACHINGCONTROLLER_H

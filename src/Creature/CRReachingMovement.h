/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRREACHINGMOVEMENT_H
#define CRREACHINGMOVEMENT_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 到達運動コントローラ
*/
class CRReachingMovement : public SceneObject, public CRReachingMovementIfInit, public CRReachingMovementDesc {
private:
	/// 制御用のバネダンパ
	PHSpringIf* spring;

	/// 制御用の仮目標剛体
	PHSolidIf*  soTarget;

	/// 目標時間
	float time, period;

	/// 達成後の待機時間
	float offset;

	/// 姿勢制御のバネダンパ係数
	float springOri, damperOri;

	/// 最終目標位置・速度・姿勢・角速度
	Vec3f finalPos, finalVel, finalAngV;
	Quaterniond finalQuat;

	/// 目標剛体のローカル座標
	Vec3f localPos;

	/// 有効かどうか
	bool bActive, bOri;

	/// 初期化を行う
	void Init();

	/// リセットする
	void Reset();

public:
	OBJECTDEF(CRReachingMovement, SceneObject);
	ACCESS_DESC(CRReachingMovement);

	CRReachingMovement(){}
	CRReachingMovement(const CRReachingMovementDesc& desc, SceneIf* s=NULL) : CRReachingMovementDesc(desc) {
		if(s){SetScene(s);}
		Init();
		Reset();
	}

	/** @brief 目標位置を設定する
		@param p 目標位置
		@param v 目標の速度
		@param t 目標到達時間
		@param o 到達後の待機時間
	*/
	virtual void SetTarget(Vec3f p, Vec3f v, float t, float o);

	/** @brief 目標位置・姿勢を設定する
		@param p 目標位置
		@param v 目標の速度
		@param q 目標姿勢
		@param av 目標角速度
		@param t 目標到達時間
		@param o 到達後の待機時間
	*/
	virtual void SetTarget(Vec3f p, Vec3f v, Quaterniond q, Vec3f av, float t, float o);

	/** @ brief 制御のステップを実行する
	*/
	virtual void Step();
};
}
//@}

#endif//CRREACHINGMOVEMENT_H

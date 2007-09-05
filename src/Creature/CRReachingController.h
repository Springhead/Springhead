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
class CRReachingController : public CRController, public CRReachingControllerIfInit, public CRReachingControllerDesc {
private:
	/// 関節固定モード
	enum CRRFixMode {
		CRR_FIXED = 0, CRR_NORMAL, CRR_UNFIXED,
	} fixmode;

	/// 制御用のバネダンパ
	PHSpringIf* spring;

	/// 制御用の仮目標剛体
	PHSolidIf*  soTarget;

	/// 目標時間
	float time, period;

	/// 達成後の待機時間
	float offset;

	/// 最終目標位置・速度・姿勢・角速度
	Vec3f firstPos, finalPos, finalVel, finalAngV;
	Quaterniond finalQuat;

	/// 目標剛体のローカル座標
	Vec3f localPos;

	/// 有効かどうか
	bool bActive, bOri;

	/// 制御対象のボディ
	CRHingeHumanBodyIf* body;

	/** @brief リセットする
	*/
	void Reset();

	/** @brief 制御対象の関節を柔らかくする
	*/
	void UnfixHinge();

	/** @brief 制御対象の関節を固くする
	*/
	void FixHinge();

public:
	OBJECTDEF(CRReachingController, CRController);
	ACCESS_DESC(CRReachingController);

	CRReachingController(){}
	CRReachingController(const CRReachingControllerDesc& desc, CRCreatureIf* c=NULL) 
		: CRReachingControllerDesc(desc) 
		, CRController((const CRControllerDesc&)desc, c)
	{
	}

	/** @ brief 初期化を実行する
	*/
	virtual void Init();

	/** @ brief 制御のステップを実行する
	*/
	virtual void Step();

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

	/** @brief 作動中かどうかを返す
	*/
	virtual bool IsActive();
};
}
//@}

#endif//CRREACHINGCONTROLLER_H

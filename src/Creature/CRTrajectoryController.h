/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRTRAJECTORYCONTROLLER_H
#define CRTRAJECTORYCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>

#include "CREngine.h"

//@{
namespace Spr{;
/** @brief 軌道運動コントローラ
*/
class CRTrajectoryController : public CREngine, public CRTrajectoryControllerDesc {
public:
	/// 予測シミュレーションのために格納しておく状態
	UTRef<ObjectStatesIf> states;

	/// 計算された関節角
	struct JointTargetValue {
		PHJointIf*	joint;
		double		hingePosition;
		double		hingeVelocity;
		double		hingeOffset;
		Quaterniond ballPosition;
		Vec3d		ballVelocity;
		Vec3d		ballOffset;
		double		spring;
		double		damper;
	};
	std::vector<JointTargetValue> jointTargetValues;

	/// クリーチャ身体
	CRBodyIf* crBody;

	// --- --- --- --- --- --- --- --- --- ---

	SPR_OBJECTDEF(CRTrajectoryController);
	ACCESS_DESC(CRTrajectoryController);

	CRTrajectoryController()
	{
		Init();
	}
	CRTrajectoryController(const CRTrajectoryControllerDesc& desc) 
		: CRTrajectoryControllerDesc(desc)
	{
		Init();
	}

	// --- --- --- --- --- --- --- --- --- ---
	// API Methods

	/** @brief プライオリティ
	*/
	virtual int GetPriority() {
		return CREngineDesc::CREP_CONTROLLER_ACT;
	}

	/** @brief １ステップ処理
	*/
	virtual void Step();

	// --- --- --- --- --- --- --- --- --- ---
	// Non-API Methods
	/** @brief 初期化
	*/
	void Init();

	/** @brief 関節角計算
	*/
	void CompJointAngle();

	/** @brief 軌道追従用オフセットの計算
	*/
	void CompOffsetForce();
};
}
//@}

#endif//CRTRAJECTORYCONTROLLER_H

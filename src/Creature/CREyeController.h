/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CREYECONTROLLER_H
#define CREYECONTROLLER_H

#include <SprCreature.h>
#include <SprFoundation.h>
#include <SprPhysics.h>

#include <Foundation/Object.h>
#include "IfStubDumpCreature.h"

namespace Spr{;

/// 眼球運動コントローラの実装
class CREyeController : public SceneObject,	public CREyeControllerIfInit, 	public CREyeControllerDesc {
private:
	CREyeControllerState::ControlState GetNextState(ControlState currentCS);
	void SaccadeControl();
	void PursuitControl();
	void ControlEyeToTargetDir(PHSolidIf* soEye, Vec3f aim);

	// 最終的にはStateクラスに移動することが望まれる変数群
	Vec3f currLookatPos; ///< 現在視線移動中の注視点
	Vec3f currLookatVel; ///< 現在視線移動中の注視点の移動速度ベクトル
	Vec3f nextLookatPos; ///< 次の注視点
	Vec3f nextLookatVel; ///< 次の注視点の移動速度ベクトル

	float saccadeTimer; ///< サッケード制御の時間経過を示すタイマ
	Vec3f saccadeFromL; ///< サッケード開始時の左目の視線方向
	Vec3f saccadeFromR; ///< サッケード開始時の左目の視線方向


public:
	OBJECTDEF(CREyeController, SceneObject);
	ACCESS_DESC_STATE(CREyeController);

	CREyeController(){
		currLookatPos = currLookatVel = Vec3f(0,0,0);
		nextLookatPos = nextLookatVel = Vec3f(0,0,0);
		saccadeTimer = 0.0;
	}
	CREyeController(const CREyeControllerDesc& desc, SceneIf* s=NULL)
		:CREyeControllerDesc(desc){ if(s){SetScene(s);}	}

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

#endif//CREYECONTROLLER_H
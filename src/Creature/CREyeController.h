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
	/** @brief 視覚制御に用いる眼球の状態量を計算する
	*/
	void CalcEyeStatusValue();

	/** @brief 次の制御状態を求める
	*/
	CREyeControllerState::ControlState GetNextState(ControlState currentCS);

	/** @briefサッケード制御を実行する
	*/
	void SaccadeControl();

	/** @brief スムースパーシュート制御を実行する
	*/
	void PursuitControl();

	/** @brief 眼球の向きを目標の方向へPD制御する
		@param soEye 制御対象の眼球
		@param aim   眼球方向の目標値
	*/
	void ControlEyeToTargetDir(PHSolidIf* soEye, Vec3d aim);

	/** @brief ベクトルのXZ平面への射影がZ軸となす角（Z軸→X軸向きの回転を正とする）
		@param v 入力ベクトル
	*/
	double Vec3ToAngH(Vec3d v);

	/** @brief ベクトルのYZ平面への射影がZ軸となす角（Z軸→Y軸向きの回転を正とする）
		@param v 入力ベクトル
	*/
	double Vec3ToAngV(Vec3d v);

	// 最終的にはStateクラスに移動することが望まれる変数群
	// -- 眼球および頭部の状態量
	double locLEyeAxisH, locLEyeAxisV, locREyeAxisH, locREyeAxisV;
	double locErrLH, locErrLV, locErrRH, locErrRV;
	double locDErrLH, locDErrLV, locDErrRH, locDErrRV;
	double locHeadAngvelH, locHeadAngvelV;
	Quaterniond qToLoc, qToGlo;
	double locLastErrLH, locLastErrLV, locLastErrRH, locLastErrRV;
	bool bContLocErr;
	// -- 目標地点
	Vec3f lookatPos; ///< 注視点
	Vec3f lookatVel; ///< 注視点の移動速度ベクトル
	// -- サッケード制御関連
	double saccadeTimer; ///< サッケード制御の時間経過を示すタイマ
	Vec3d  saccadeFromL; ///< サッケード開始時の左目の視線方向
	Vec3d  saccadeFromR; ///< サッケード開始時の左目の視線方向
	// -- スムースパーシュート制御関連
	double integrator_L, integrator_R, integrator_Lv, integrator_Rv; ///< 積分器

public:
	OBJECTDEF(CREyeController, SceneObject);
	ACCESS_DESC_STATE(CREyeController);

	CREyeController(){
		saccadeTimer = 0.0f;
		integrator_L = integrator_R = integrator_Lv = integrator_Rv = 0.0f;
		locLastErrLH=0; locLastErrLV=0; locLastErrRH=0; locLastErrRV=0;
		bContLocErr = false;
	}
	CREyeController(const CREyeControllerDesc& desc, SceneIf* s=NULL)
		:CREyeControllerDesc(desc){ 
		if(s){SetScene(s);}
		saccadeTimer = 0.0f;
		integrator_L = integrator_R = integrator_Lv = integrator_Rv = 0.0f;
		locLastErrLH=0; locLastErrLV=0; locLastErrRH=0; locLastErrRV=0;
		bContLocErr = false;
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

#endif//CREYECONTROLLER_H
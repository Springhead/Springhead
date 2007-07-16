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
	void ControlEyeToTargetDir(PHSolidIf* soEye, Vec3f aim);

	/** @brief ベクトルのXZ平面への射影がZ軸となす角（Z軸→X軸向きの回転を正とする）
		@param v 入力ベクトル
	*/
	double Vec3ToAngH(Vec3d v);

	/** @brief ベクトルのYZ平面への射影がZ軸となす角（Z軸→Y軸向きの回転を正とする）
		@param v 入力ベクトル
	*/
	double Vec3ToAngV(Vec3d v);

	// 最終的にはStateクラスに移動することが望まれる変数群
	double locLEyeAxisH, locLEyeAxisV, locREyeAxisH, locREyeAxisV;
	double locErrLH, locErrLV, locErrRH, locErrRV;
	double locDErrLH, locDErrLV, locDErrRH, locDErrRV;
	double locLastErrLH, locLastErrLV, locLastErrRH, locLastErrRV;
	double locHeadAngvelH, locHeadAngvelV;
	Quaterniond qToLoc, qToGlo;

	// -- 眼球の状態量
	float t1, t2, t3, t4; ///< 視線方向の角度を示す数値
	float eL, eR, eLV, eRV; ///< 視線方向と目標方向の誤差量
	float vL, vR, vLV, vRV; ///< 視線方向と目標方向の誤差量の変化量
	float last_t1_a, last_t2_a, last_t3_a, last_t4_a; ///< 前回の目標位置（視角）
	// -- 目標地点関連
	Vec3f currLookatPos; ///< 現在視線移動中の注視点
	Vec3f currLookatVel; ///< 現在視線移動中の注視点の移動速度ベクトル
	Vec3f nextLookatPos; ///< 次の注視点
	Vec3f nextLookatVel; ///< 次の注視点の移動速度ベクトル
	// -- サッケード制御関連
	float saccadeTimer; ///< サッケード制御の時間経過を示すタイマ
	Vec3f saccadeFromL; ///< サッケード開始時の左目の視線方向
	Vec3f saccadeFromR; ///< サッケード開始時の左目の視線方向
	// -- スムースパーシュート制御関連
	float integrator_L, integrator_R, integrator_Lv, integrator_Rv; ///< 積分器

public:
	OBJECTDEF(CREyeController, SceneObject);
	ACCESS_DESC_STATE(CREyeController);

	CREyeController(){
		currLookatPos = currLookatVel = Vec3f(0,0,0);
		nextLookatPos = nextLookatVel = Vec3f(0,0,0);
		saccadeTimer = 0.0f;
		t1 = t2 = t3 = t4 = 0.0f;
		eL = eR = eLV = eRV = 0.0f;
		vL = vR = vLV = vRV = 0.0f;
		last_t1_a = last_t2_a = last_t3_a = last_t4_a = 0.0f;
		integrator_L = integrator_R = integrator_Lv = integrator_Rv = 0.0f;
		locLastErrLH=0; locLastErrLV=0; locLastErrRH=0; locLastErrRV=0;
	}
	CREyeController(const CREyeControllerDesc& desc, SceneIf* s=NULL)
		:CREyeControllerDesc(desc){ 
		if(s){SetScene(s);}
		currLookatPos = currLookatVel = Vec3f(0,0,0);
		nextLookatPos = nextLookatVel = Vec3f(0,0,0);
		saccadeTimer = 0.0f;
		t1 = t2 = t3 = t4 = 0.0f;
		eL = eR = eLV = eRV = 0.0f;
		vL = vR = vLV = vRV = 0.0f;
		last_t1_a = last_t2_a = last_t3_a = last_t4_a = 0.0f;
		integrator_L = integrator_R = integrator_Lv = integrator_Rv = 0.0f;
		locLastErrLH=0; locLastErrLV=0; locLastErrRH=0; locLastErrRV=0;
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
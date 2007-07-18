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

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRPursuitController
// スムースパーシュート用コントローラの状態量定義クラス
class CRPursuitControllerState {
public:
	double angleLH, angleRH, angleV;
	double intL, intR;
	double lastIL, lastIR;

	CRPursuitControllerState() {
		angleLH = angleRH = angleV = 0.0;
		lastIL = lastIR = 0.0;
	}

	void Reset(){
		intL = intR = 0.0;
		lastIL = lastIR = 0.0;
	}
};

// スムースパーシュート用コントローラのパラメータ定義クラス
class CRPursuitControllerDesc : public CRPursuitControllerState {
public:
	double R1, R2;
	double N, H, S, K;
	double A1;
	CRPursuitControllerDesc() : CRPursuitControllerState() {
		R1 = 1.5;
		R2 = 0.5;
		N  = 0.5;
		H  = 0.2;
		S  = 1.0;
		K  = 0.5;
		A1 = 0.5;
	}
};

// スムースパーシュート用コントローラ(CREyeControllerが内部で使う、非APIクラス)
class CRPursuitController : public CRPursuitControllerDesc {
private:
public:
	CRPursuitController() : CRPursuitControllerDesc() {}
	CRPursuitController(CRPursuitControllerDesc& desc) : CRPursuitControllerDesc(desc){}
	void StepHoriz(double destLH, double destRH, double angleHeadH, double dt);
	void StepVert(double destV, double angleHeadV, double dt);
};


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CREyeController
/// 眼球運動コントローラの実装
class CREyeController : public SceneObject,	public CREyeControllerIfInit, public CREyeControllerDesc {
private:
	/** @brief 視覚制御に用いる眼球の状態量を計算する
	*/
	void CalcEyeStatusValue();

	/** @brief 次の制御状態を求める
	*/
	CREyeControllerState::ControlState GetNextState(ControlState currentCS);

	/** @brief 眼球の向きを目標の方向へPD制御する
		@param soEye 制御対象の眼球
		@param aim   眼球方向の目標値
	*/
	void ControlEyeToTargetDir(PHSolidIf* soEye, Vec3d aim);

	/** @brief 眼球の向きを目標の方向へPD制御する
		@param soEye 制御対象の眼球
		@param horiz 目標角度（水平方向）
		@param vert  目標角度（垂直方向）
	*/
	void ControlEyeToTargetDir(PHSolidIf* soEye, double horiz, double vert);

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
	Quaterniond qToLoc, qToGlo;
	double locLEyeAxisH, locLEyeAxisV, locREyeAxisH, locREyeAxisV;
	double locLLookatH,  locLLookatV,  locRLookatH,  locRLookatV;
	// -- 目標地点
	Vec3f lookatPos; ///< 注視点
	Vec3f lookatVel; ///< 注視点の移動速度ベクトル
	
	// スムースパーシュートコントローラ
	CRPursuitController pursuitCtrl;

public:
	OBJECTDEF(CREyeController, SceneObject);
	ACCESS_DESC_STATE(CREyeController);

	CREyeController(){}
	CREyeController(const CREyeControllerDesc& desc, SceneIf* s=NULL) : CREyeControllerDesc(desc) {if(s){SetScene(s);}}

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel);

	/** @brief 制御処理を実行する
	*/
	virtual void Step();
};

}//@}

#endif//CREYECONTROLLER_H
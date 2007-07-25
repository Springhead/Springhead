/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CREYECONTROLLER_H
#define CREYECONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief サッケード用コントローラ(CREyeControllerが内部で使う、非APIクラス)
*/
class CRSaccadeController {
private:
	/// 角度指令値
	double angleLH, angleRH, angleV;

public:
	CRSaccadeController() {
	}

	/** @brief 状態をゼロリセット
	*/
	void Reset();

	/** @brief 現在の状態を指定してリセット
	*/
	void Reset(double angleLH, double angleRH, double angleV);

	/** @brief 水平方向の目標値をセットして積分
	*/
	void StepHoriz(double destLH, double destRH, double angleHeadH, double dt);

	/** @brief 垂直方向の目標値をセットして積分
	*/
	void StepVert(double destV, double angleHeadV, double dt);

	/** @brief  左目の角度指令値を取得
		@return Vec2d(垂直角度, 水平角度) 正面が0度, 回転方向は右手座標系に準ずる
	*/
	Vec2d GetLEyeAngle();

	/** @brief 右目の角度指令値を取得
		@return Vec2d(垂直角度, 水平角度) 正面が0度, 回転方向は右手座標系に準ずる
	*/
	Vec2d GetREyeAngle();
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief スムースパーシュート用コントローラ(CREyeControllerが内部で使う、非APIクラス)
*/
class CRPursuitController {
private:
	// 状態変数
	/// 積分器
	double intL, intR;
	/// 前回の誤差値
	double lastIL, lastIR;
	/// 角度指令値
	double angleLH, angleRH, angleV;

public:
	// コントローラの係数
	/// 出力にかかる係数
	double R1, R2;
	/// 視標誤差の入力にかかる係数
	double N, H, S, K;
	/// 頭部運動の入力にかかる係数
	double A1;

	CRPursuitController() {
		// 張先生の論文によるデフォルト値
		R1 = 1.5;  R2 = 0.5;
		N  = 0.5;  H  = 0.2;  S  = 1.0;  K  = 0.5;
		A1 = 0.5;
		Reset();
	}

	/** @brief 状態をゼロリセット
	*/
	void Reset();

	/** @brief 現在の状態を指定してリセット
	*/
	void Reset(double angleLH, double angleRH, double angleV);

	/** @brief 水平方向の目標値をセットして積分
	*/
	void StepHoriz(double destLH, double destRH, double angleHeadH, double dt);

	/** @brief 垂直方向の目標値をセットして積分
	*/
	void StepVert(double destV, double angleHeadV, double dt);

	/** @brief  左目の角度指令値を取得
		@return Vec2d(垂直角度, 水平角度) 正面が0度, 回転方向は右手座標系に準ずる
	*/
	Vec2d GetLEyeAngle();

	/** @brief 右目の角度指令値を取得
		@return Vec2d(垂直角度, 水平角度) 正面が0度, 回転方向は右手座標系に準ずる
	*/
	Vec2d GetREyeAngle();
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 眼球運動コントローラの実装
*/
class CRPhysicalEye {
private:
	/// 制御に使用する剛体
	PHSolidIf *soLEye, *soREye, *soHead;
	/// 視標位置
	Vec3d targetPos, targetVel;
	/// 制御の係数
	double Kp, Kd;

	/** @brief 片目に関して制御
	*/
	void Control(PHSolidIf* soEye, Vec2d angle);

	/** @brief ベクトルのXZ平面への射影がZ軸となす角（Z軸→X軸向きの回転を正とする）
		@param v 入力ベクトル
	*/
	double Vec3ToAngH(Vec3d v);

	/** @brief ベクトルのYZ平面への射影がZ軸となす角（Z軸→Y軸向きの回転を正とする）
		@param v 入力ベクトル
	*/
	double Vec3ToAngV(Vec3d v);

public:
	CRPhysicalEye() : soLEye(NULL), soREye(NULL), soHead(NULL) {
		Kp = Kd = 0.0;
	}

	CRPhysicalEye(PHSolidIf* lEye, PHSolidIf* rEye, PHSolidIf* head, double kp, double kd) : soLEye(lEye), soREye(rEye), soHead(head), Kp(kp), Kd(kd) {
	}

	/** @brief ３次元空間内の視標位置を設定する（絶対位置）
	*/
	void SetTarget(Vec3d pos, Vec3d vel);

	/** @brief 眼球運動制御を実行する
		@param angleLEye 左目の角度指令値
		@param angleREye 右目の角度指令値
	*/
	void Control(Vec2d angleLEye, Vec2d angleREye);

	/** @brief 左目の現在の視軸角度を取得する
	*/
	Vec2d GetAxisL();

	/** @brief 右目の現在の視軸角度を取得する
	*/
	Vec2d GetAxisR();

	/** @brief 左目から見た視標方向の角度を取得する
	*/
	Vec2d GetTargetFromL();

	/** @brief 右目から見た視標方向の角度を取得する
	*/
	Vec2d GetTargetFromR();
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 眼球運動コントローラの実装
*/
class CREyeController : public SceneObject,	public CREyeControllerIfInit, public CREyeControllerDesc {
private:
	/** @brief 次の制御状態を求める
	*/
	CREyeControllerState::ControlState GetNextState(ControlState currentCS);

	/** @brief 制御を実行する
	*/
	void Control();

	/// 物理世界内での眼球を取り扱うレイヤー
	CRPhysicalEye physicalEye;
	/// スムースパーシュートコントローラ
	CRPursuitController pursuitCtrl;
	/// サッケードコントローラ
	CRSaccadeController saccadeCtrl;

public:
	OBJECTDEF(CREyeController, SceneObject);
	ACCESS_DESC_STATE(CREyeController);

	CREyeController(){}
	CREyeController(const CREyeControllerDesc& desc, SceneIf* s=NULL) : CREyeControllerDesc(desc) {
		if(s){SetScene(s);}
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

}//@}

#endif//CREYECONTROLLER_H
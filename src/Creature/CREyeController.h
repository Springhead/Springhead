/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CREYECONTROLLER_H
#define CREYECONTROLLER_H

#include <Creature/CREngine.h>
#include <Creature/SprCRGazeController.h>

//@{
namespace Spr{;

struct PHSolidIf;
struct PHHingeJointIf;
struct PHBallJointIf;

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
	void StepHoriz(double destLH, double destRH, double angvelHeadH, double dt);

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
/** @brief 眼球コントローラの物理レイヤ(CREyeControllerが内部で使う、非APIクラス)
*/
class CRPhysicalEye {
private:
	/// 制御対象のクリーチャ
	CRCreatureIf* creature;

	/// 制御に使用する剛体
	PHSolidIf *soLEye, *soREye, *soHead;

	/// 制御に使用する関節
	PHHingeJointIf *joLEyeX, *joLEyeY, *joREyeX, *joREyeY;
	PHBallJointIf  *joLEye,  *joREye;

	/// 視標位置
	Vec3d targetPos, targetVel;

	/** @brief 片目に関して制御
	*/
	void Control(PHHingeJointIf* joX, PHHingeJointIf* joY, Vec2d angle);
	void Control(PHBallJointIf*  jo, Vec2d angle);

	/** @brief (0,0,-1)と成す角を、右手系でX軸・Y軸周りの回転として求める
		@param v 入力ベクトル
		@return (ZY射影が(0,0,-1)と成す角(X軸回転), ZX射影が(0,0,-1)と成す角(Y軸回転))
	*/
	Vec2d Vec3ToAngle(Vec3d v);

public:
	CRPhysicalEye() {
		soLEye = soREye = soHead = NULL;
		joLEyeX = joLEyeY = joREyeX = joREyeY = NULL;
		joLEye = joREye = NULL;
	}

	void SetCreature(CRCreatureIf* c) { creature = c; }

	/** @brief 初期化を実行する
	*/
	void Init();

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

	/** @brief 頭部の角度（絶対座標系）を取得する
	*/
	Vec2d GetHeadAngle();

	/** @brief 頭部の各速度（頭部座標系）を取得する
	*/
	Vec2d GetHeadAngvel();
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 眼球運動コントローラの実装
*/
class CREyeController : public CREngine, public CREyeControllerDesc {
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
	SPR_OBJECTDEF(CREyeController);
	ACCESS_DESC_STATE(CREyeController);

	CREyeController(){}
	CREyeController(const CREyeControllerDesc& desc) 
		: CREyeControllerDesc(desc) 
	{
		// 従来ここで↓こんな感じのことをやっていたが今はたぶんこれでは動かない
		// ので別の場所に移す必要がある（09/06/20 mitake）
		// physicalEye->SetCreature(GetScene()->Cast());
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();

	/** @brief 制御処理を実行する
	*/
	virtual void Step();

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel);

	/** @brief サッケード中かどうかを返す
	*/
	virtual bool IsSaccading();

	// 非API関数
	/** @brief 現在のコントロール状態を取得する
	*/
	CREyeControllerState::ControlState GetControlState();
};

}//@}

#endif//CREYECONTROLLER_H
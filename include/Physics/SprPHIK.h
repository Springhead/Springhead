/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/** \addtogroup gpPhysics */
//@{

/**
 *	@file SprPHIK.h
 *	@brief 逆運動学(IK)計算
*/
#ifndef SPR_PHIKIf_H
#define SPR_PHIKIf_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct PHSolidIf;
struct PHHingeJointIf;
struct PHBallJointIf;

/** \defgroup gpIK 逆運動学(IK)計算*/
//@{

// ------------------------------------------------------------------------------
/// IKのエンドエフェクタ（到達目標に向けて動かされるもの）

/// IKエンドエフェクタのステート
struct PHIKEndEffectorState{	
	Posed solidTempPose;  ///< IK-FK計算用の一時変数：剛体姿勢
	PHIKEndEffectorState(){
		solidTempPose = Posed();
	}
};

/// IKエンドエフェクタのデスクリプタ
struct PHIKEndEffectorDesc : public PHIKEndEffectorState {
	// 姿勢制御のモード
	enum OriCtlMode {
		MODE_QUATERNION, // 姿勢制御：targetOrientationに示される姿勢をとろうとする
		MODE_DIRECTION,  // 方向制御：targetLocalDirectionがtargetDirectionに示される向きをとろうとする
		MODE_LOOKAT,     // 視線方向制御：targetLocalDirectionがtargetLookatに示される位置のほうを向く
	};

	bool   bEnabled;				///< エンドエフェクタを作動させるかどうか

	bool   bPosition;				///< 位置制御を有効にするかどうか
	bool   bOrientation;			///< 姿勢制御を有効にするかどうか
	OriCtlMode oriCtlMode;			///< 姿勢制御のモード（Quaternion, Direction, Lookat）
	bool   bForce;					///< 力制御を有効にするかどうか
	bool   bTorque;					///< トルク制御を有効にするかどうか

	double positionPriority;		///< 位置制御の達成優先度（1～0、大きいほど優先度が高い）
	double orientationPriority;		///< 姿勢制御の達成優先度（1～0、大きいほど優先度が高い）
	double forcePriority;			///< 力制御の達成優先度（1～0、大きいほど優先度が高い）
	double torquePriority;			///< トルク制御の達成優先度（1～0、大きいほど優先度が高い）

	Vec3d		targetPosition;				///< 到達目標位置
	Vec3d		targetVelocity;				///< 目標速度
	Vec3d		targetLocalPosition;		///< エンドエフェクタにおける到達させたい部位の位置
	Vec3d		targetLocalDirection;		///< 方向制御をする場合の基準ベクトル（剛体ローカル座標系）
	Quaterniond	targetOrientation;			///< 到達目標姿勢
	Vec3d		targetDirection;			///< 方向制御（Direction）の目標方向
	Vec3d		targetLookat;				///< 方向制御（Lookat）の目標位置
	Vec3d		targetAngVel;               ///< 目標角速度
	Vec3d		targetForce;				///< 力の目標値
	Vec3d		targetForceWorkingPoint;	///< 出したい力の作用点
	Vec3d		targetTorque;				///< トルクの目標値

	PHIKEndEffectorDesc();
};

/// IKエンドエフェクタのインタフェース
struct PHIKEndEffectorIf : SceneObjectIf{
	SPR_IFDEF(PHIKEndEffector);

	/** @brief 動作対象の剛体を設定する（１エンドエフェクタにつき１剛体が必ず対応する）
	*/
	void SetSolid(PHSolidIf* solid);

	/** @brief 動作対象として設定された剛体を取得する
	*/
	PHSolidIf* GetSolid();

	// --- --- --- --- ---

	/** @brief 有効・無効を設定する
	*/
	void Enable(bool enable);

	/** @brief 有効・無効を取得する
	*/
	bool IsEnabled();

	// --- --- --- --- ---

	/** @brief 剛体において到達させたい位置の剛体ローカル座標を設定する
	*/
	void SetTargetLocalPosition(Vec3d localPosition);

	/** @brief エンドエフェクタにおける到達させたい位置の設定された目標値を取得する
	*/
	Vec3d GetTargetLocalPosition();

	/** @brief 方向制御をする場合の方向基準ベクトル（剛体ローカル座標系において）を設定する
	*/
	void SetTargetLocalDirection(Vec3d localDirection);

	/** @brief 方向制御をする場合の方向基準ベクトル（剛体ローカル座標系において）を取得する
	*/
	Vec3d GetTargetLocalDirection();

	// --- --- --- --- ---

	/** @brief 位置の制御の有効・無効を切り替える
	*/
	void EnablePositionControl(bool enable);

	/** @brief 位置の制御の有効・無効を取得する
	*/
	bool IsPositionControlEnabled();

	/** @brief 位置制御のプライオリティを設定する
	*/
	void SetPositionPriority(double priority);
	double GetPositionPriority();

	/** @brief 位置の目標値を設定する
	*/
	void SetTargetPosition(Vec3d position);

	/** @brief 設定された位置の目標値を取得する
	*/
	Vec3d GetTargetPosition();

	// --- --- --- --- ---

	/** @brief 姿勢の制御の有効・無効を切り替える
	*/
	void EnableOrientationControl(bool enable);

	/** @brief 姿勢の制御の有効・無効を取得する
	*/
	bool IsOrientationControlEnabled();

	/** @brief 姿勢制御のプライオリティを設定する
	*/
	void SetOrientationPriority(double priority);
	double GetOrientationPriority();

	/** @brief 姿勢制御のモードを設定する
	*/
	void SetOriCtlMode(PHIKEndEffectorDesc::OriCtlMode mode);
	PHIKEndEffectorDesc::OriCtlMode GetOriCtlMode();

	/** @brief 姿勢の目標値を設定する
	*/
	void SetTargetOrientation(Quaterniond orientation);

	/** @brief 設定された姿勢の目標値を取得する
	*/
	Quaterniond GetTargetOrientation();

	/** @brief 方向の目標値を設定する
	*/
	void SetTargetDirection(Vec3d direction);

	/** @brief 方向の目標値を取得する
	*/
	Vec3d GetTargetDirection();

	/** @brief 視線方向の目標値を設定する
	*/
	void SetTargetLookat(Vec3d lookat);

	/** @brief 視線方向の目標値を取得する
	*/
	Vec3d GetTargetLookat();

	// --- --- --- --- ---

	/** @brief 速度の目標値を設定する
	*/
	void SetTargetVelocity(Vec3d velocity);

	/** @brief 速度の目標値を取得する
	*/
	Vec3d GetTargetVelocity();

	/** @brief 角速度の目標値を設定する
	*/
	void SetTargetAngularVelocity(Vec3d angVel);

	/** @brief 角速度の目標値を設定する
	*/
	Vec3d GetTargetAngularVelocity();

	// --- --- --- --- ---

	/** @brief 力の制御の有効・無効を切り替える
	*/
	void EnableForceControl(bool enable);

	/** @brief 力の目標値を設定する
	*/
	void SetTargetForce(Vec3d force, Vec3d workingPoint = Vec3d());

	/** @brief 設定された力の目標値を取得する
	*/
	Vec3d GetTargetForce();

	/** @brief 設定された力の作用点を取得する
	*/
	Vec3d GetTargetForceWorkingPoint();

	// --- --- --- --- ---

	/** @brief トルクの制御の有効・無効を切り替える
	*/
	void EnableTorqueControl(bool enable);

	/** @brief トルクの目標値を設定する
	*/
	void SetTargetTorque(Vec3d torque);

	/** @brief 設定されたトルクの目標値を取得する
	*/
	Vec3d GetTargetTorque();

	/** @brief 一時変数の剛体姿勢を取得する
	*/

	Posed GetSolidTempPose();

	// --- --- --- --- ---

	/** @brief 一時変数の剛体姿勢を現実の剛体姿勢に合わせる
	*/
	void ApplyExactState();
};

// ------------------------------------------------------------------------------
/// IK用のアクチュエータ（目標の達成のために用いることのできる作動部品（おもに関節に付随））
struct PHIKActuatorIf : SceneObjectIf{
	SPR_IFDEF(PHIKActuator);

	/** @brief IKの計算準備をする
	*/
	void PrepareSolve();

	/** @brief IKの計算繰返しの１ステップを実行する
	*/
	void ProceedSolve();

	/** @brief 擬似逆解を解いたままの"生の"計算結果を取得する
	*/
	PTM::VVector<double> GetRawSolution();

	// --- --- --- --- ---

	/** @brief 計算結果に従って制御対象を動かす
	*/
	void Move();

	/** @brief 一時変数の関節角度を現実の関節角度に合わせる
	*/
	void ApplyExactState(bool reverse=false);

	// --- --- --- --- ---

	/** @brief 動かしにくさを設定する
	*/
	void SetBias(float bias);

	/** @brief 動かしにくさを取得する
	*/
	float GetBias();

	/** @brief 標準姿勢への復帰率を設定（0.0～1.0）
	*/
	void SetPullbackRate(double pullbackRate);

	/** @brief 標準姿勢への復帰率を取得
	*/
	double GetPullbackRate();

	/** @brief 有効・無効を設定する
	*/
	void Enable(bool enable);

	/** @brief 有効・無効を取得する
	*/
	bool IsEnabled();

	/** @brief 祖先を取得する
	*/
	int NAncestors();
	PHIKActuatorIf* GetAncestor(int i);

	/** @brief 直接の親を取得する
	*/
	PHIKActuatorIf* GetParent();

	/** @brief 直接の子アクチュエータを取得する
	*/
	int NChildActuators();
	PHIKActuatorIf* GetChildActuator(int i);
};

/// IKアクチュエータのステート
struct PHIKActuatorState{
	Posed       solidTempPose;    ///< IK-FK計算用の一時変数：プラグ剛体姿勢	
	Quaterniond jointTempOri;     ///< IK-FK計算用の一時変数：関節角度

	// <!!> ヒンジでしか使わないが，Stateの多重継承を防ぐためここに入れてある．(13/07/08 mitake)
	double      jointTempAngle; ///< IK-FK計算用の一時変数：関節角度（ヒンジ用）

	PHIKActuatorState() {
		solidTempPose    = Posed();
		jointTempOri     = Quaterniond();
		jointTempAngle = 0.0;
	}
};

/// IKアクチュエータのディスクリプタ
struct PHIKActuatorDesc : public PHIKActuatorState{
	SPR_DESCDEF(PHIKActuator);

	bool    bEnabled;     ///< 有効かどうか
	float	bias;	      ///< 動かしにくさの係数
	double  pullbackRate; ///< 標準姿勢復帰の割合
	
	PHIKActuatorDesc() {
		bEnabled     = true;
		bias         = 1.0;
		pullbackRate = 0.1;
	}
};

/// ３軸アクチュエータ（PHBallJointを駆動する）
struct PHIKBallActuatorIf : PHIKActuatorIf{
	SPR_IFDEF(PHIKBallActuator);

	/** @brief 動作対象の関節を設定する（１アクチュエータにつき１関節が必ず対応する）
	*/
	void SetJoint(PHBallJointIf* joint);

	/** @brief 動作対象として設定された関節を取得する
	*/
	PHBallJointIf* GetJoint();
};

/// ３軸アクチュエータのディスクリプタ
struct PHIKBallActuatorDesc : PHIKActuatorDesc{
	SPR_DESCDEF(PHIKBallActuator);
};

/// １軸アクチュエータ（PHHingeJointを駆動する）
struct PHIKHingeActuatorIf : PHIKActuatorIf{
	SPR_IFDEF(PHIKHingeActuator);

	/** @brief 動作対象の関節を設定する（１アクチュエータにつき１関節が必ず対応する）
	*/
	void SetJoint(PHHingeJointIf* joint);

	/** @brief 動作対象として設定された関節を取得する
	*/
	PHHingeJointIf* GetJoint();
};

/// １軸アクチュエータのディスクリプタ
struct PHIKHingeActuatorDesc : PHIKActuatorDesc{
	SPR_DESCDEF(PHIKHingeActuator);
};

//@}
//@}

}

#endif

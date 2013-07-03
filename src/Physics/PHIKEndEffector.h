/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_IKENDEFFECTOR_H
#define PH_IKENDEFFECTOR_H

#include "../Foundation/Object.h"
#include <Physics/SprPHIK.h>
#include <set>

namespace Spr{;

class PHIKActuator;
class PHIKEndEffector;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IKEndEffector：IKのエンドエフェクタ（到達させる対象）
// 

class PHIKEndEffector : public SceneObject, public PHIKEndEffectorDesc {
public:
	SPR_OBJECTDEF(PHIKEndEffector);
	ACCESS_DESC(PHIKEndEffector)

	/// 自由度
	int ndof;

	/// 自由度変化フラグ
	bool bNDOFChanged;

	/// 番号
	int number;

	/// 制御点のある剛体
	PHSolidIf* solid;

	/// IK-FK計算用の一時変数：剛体姿勢
	Posed solidTempPose;

	/** @brief 初期化する
	*/
	void Init() {
		solid = NULL;
		ndof = 3;
		bNDOFChanged = true;
		solidTempPose = Posed();
		number = -1;
	}

	/** @brief デフォルトコンストラクタ
	*/
	PHIKEndEffector() {
		Init();
	}

	/** @brief コンストラクタ
	*/
	PHIKEndEffector(const PHIKEndEffectorDesc& desc) {
		Init();
		SetDesc(&desc);
	}

	// --- --- --- --- ---

	/** @brief 動作対象の剛体を設定する（１エンドエフェクタにつき１剛体が必ず対応する）
	*/
	void SetSolid(PHSolidIf* solid) { this->solid = solid; }

	/** @brief 動作対象として設定された剛体を取得する
	*/
	PHSolidIf* GetSolid() { return this->solid; }

	// --- --- --- --- ---

	/** @brief 有効・無効を設定する
	*/
	void Enable(bool enable);

	/** @brief 有効・無効を取得する
	*/
	bool IsEnabled() { return bEnabled; }

	// --- --- --- --- ---

	void ChangeNDOF() {
		if (bPosition && bOrientation) {
			if (ndof != 6) {
				ndof = 6;
				bNDOFChanged = true;
			}
		} else if (bPosition || bOrientation) {
			if (ndof != 3) {
				ndof = 3;
				bNDOFChanged = true;
			}
		}
	}

	virtual void AfterSetDesc() {
		ChangeNDOF();
	}

	// --- --- --- --- ---

	/** @brief 位置の制御の有効・無効を切り替える
	*/
	void EnablePositionControl(bool enable) {
		bPosition = enable;
		ChangeNDOF();
	}

	/** @brief 位置の制御の有効・無効を取得する
	*/
	bool IsPositionControlEnabled() { return bPosition; }

	/** @brief 位置制御のプライオリティを設定する
	*/
	void SetPositionPriority(double priority){ positionPriority = priority; }

	/** @brief 位置の目標値を設定する
	*/
	void SetTargetPosition(Vec3d position) { targetPosition = position; }

	/** @brief 剛体において到達させたい位置の剛体ローカル座標を設定する
	*/
	void SetTargetLocalPosition(Vec3d localPosition) { targetLocalPosition = localPosition; }

	/** @brief 設定された位置の目標値を取得する
	*/
	Vec3d GetTargetPosition() { return targetPosition; }

	/** @brief エンドエフェクタにおける到達させたい位置の設定された目標値を取得する
	*/
	Vec3d GetTargetLocalPosition() { return targetLocalPosition; }

	// --- --- --- --- ---

	/** @brief 姿勢の制御の有効・無効を切り替える
	*/
	void EnableOrientationControl(bool enable) {
		bOrientation = enable;
		ChangeNDOF();
	}

	/** @brief 姿勢の制御の有効・無効を取得する
	*/
	bool IsOrientationControlEnabled() { return bOrientation; }

	/** @brief 姿勢制御のプライオリティを設定する
	*/
	void SetOrientationPriority(double priority){ orientationPriority = priority; }

	/** @brief 姿勢の目標値を設定する
	*/
	void SetTargetOrientation(Quaterniond orientation) { targetOrientation = orientation; }

	/** @brief 設定された姿勢の目標値を取得する
	*/
	Quaterniond GetTargetOrientation() { return targetOrientation; }

	// --- --- --- --- ---

	/** @brief 力の制御の有効・無効を切り替える
	*/
	void EnableForceControl(bool enable) { bForce = enable; }

	/** @brief 力の目標値を設定する
	*/
	void SetTargetForce(Vec3d force, Vec3d workingPoint) {
		targetForce				= force;
		targetForceWorkingPoint	= workingPoint;
	}

	/** @brief 設定された力の目標値を取得する
	*/
	Vec3d GetTargetForce() { return targetForce; }

	/** @brief 設定された力の作用点を取得する
	*/
	Vec3d GetTargetForceWorkingPoint() { return targetForceWorkingPoint; }

	// --- --- --- --- ---

	/** @brief トルクの制御の有効・無効を切り替える
	*/
	void EnableTorqueControl(bool enable) { bTorque = enable; }

	/** @brief トルクの目標値を設定する
	*/
	void SetTargetTorque(Vec3d torque) { targetTorque = torque; }

	/** @brief 設定されたトルクの目標値を取得する
	*/
	Vec3d GetTargetTorque() { return targetTorque; }

	// --- --- --- --- ---

	virtual bool		AddChildObject(ObjectIf* o);
	virtual ObjectIf*	GetChildObject(size_t pos);
	virtual	size_t		NChildObject() const;
	
	// --- --- --- --- --- --- --- --- --- ---
	// Non API Methods

	/** @brief 暫定目標地点を取得する
	*/
	void GetTempTarget(PTM::VVector<double> &v);

	/** @brief 目標地点までの距離を取得する
	*/
	double PosDeviation() {
		return ((solidTempPose * targetLocalPosition) - targetPosition).norm();
	}
};

}

#endif

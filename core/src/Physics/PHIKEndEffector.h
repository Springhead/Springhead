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
#include <Physics/SprPHSolid.h>
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
	ACCESS_DESC_STATE(PHIKEndEffector);

	/// 自由度
	int ndof;

	/// 自由度変化フラグ
	bool bNDOFChanged;

	/// 番号
	int number;

	/// 制御点のある剛体
	PHSolidIf* solid;

	//直近Actuator
	//SprPHIK.h、PHIKActuator.cppの23行目、およびこの下にも変更がありますので不要なら関係部削除を
	PHIKActuatorIf* ika;

	/// IK Iterationの1回前でのsolidTempPose
	Posed lastSolidTempPose;

	// <!!>
	Vec3d lagMulP;
	Vec3d lagMulR;

	/** @brief 初期化する
	*/
	void Init() {
		solid = NULL;
		ika = NULL;
		ndof = 3;
		bNDOFChanged = true;
		solidTempPose = Posed();
		lastSolidTempPose = Posed();
		number = -1;
		lagMulP = Vec3d();
		lagMulR = Vec3d();
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

	//取り合えず直近のActuatorを親認定しときます
	void SetParentActuator(PHIKActuatorIf* ika){ this->ika = ika; }

	PHIKActuatorIf* GetParentActuator(){ return this->ika; }

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
		} else if (!bPosition && !bOrientation) {
			if (ndof != 0) {
				ndof = 0;
				bNDOFChanged = true;
			}
		} else {
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

	/** @brief 剛体において到達させたい位置の剛体ローカル座標を設定する
	*/
	void SetTargetLocalPosition(Vec3d localPosition) { targetLocalPosition = localPosition; }

	/** @brief エンドエフェクタにおける到達させたい位置の設定された目標値を取得する
	*/
	Vec3d GetTargetLocalPosition() { return targetLocalPosition; }

	/** @brief 方向制御をする場合の方向基準ベクトル（剛体ローカル座標系において）を設定する
	*/
	void SetTargetLocalDirection(Vec3d localDirection) { targetLocalDirection = localDirection; }

	/** @brief 方向制御をする場合の方向基準ベクトル（剛体ローカル座標系において）を取得する
	*/
	Vec3d GetTargetLocalDirection() { return targetLocalDirection; }

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
	double GetPositionPriority(){ return positionPriority; }

	/** @brief 位置の目標値を設定する
	*/
	void SetTargetPosition(Vec3d position) { targetPosition = position; }

	/** @brief 設定された位置の目標値を取得する
	*/
	Vec3d GetTargetPosition() { return targetPosition; }

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
	double GetOrientationPriority(){ return orientationPriority; }

	/** @brief 姿勢制御のモードを設定する
	*/
	void SetOriCtlMode(PHIKEndEffectorDesc::OriCtlMode mode) { oriCtlMode = (int)mode; }
	PHIKEndEffectorDesc::OriCtlMode GetOriCtlMode() { return (PHIKEndEffectorDesc::OriCtlMode)oriCtlMode; }

	/** @brief 姿勢の目標値を設定する
	*/
	void SetTargetOrientation(Quaterniond orientation) { targetOrientation = orientation; }

	/** @brief 設定された姿勢の目標値を取得する
	*/
	Quaterniond GetTargetOrientation() { return targetOrientation; }

	/** @brief 方向の目標値を設定する
	*/
	void SetTargetDirection(Vec3d direction) { targetDirection = direction; }

	/** @brief 方向の目標値を取得する
	*/
	Vec3d GetTargetDirection() { return targetDirection; }

	/** @brief 視線方向の目標値を設定する
	*/
	void SetTargetLookat(Vec3d lookat) { targetLookat = lookat; }

	/** @brief 視線方向の目標値を取得する
	*/
	Vec3d GetTargetLookat() { return targetLookat; }

	// --- --- --- --- ---

	/** @brief 速度の目標値を設定する
	*/
	void SetTargetVelocity(Vec3d velocity) { targetVelocity = velocity; }

	/** @brief 速度の目標値を取得する
	*/
	Vec3d GetTargetVelocity() { return targetVelocity; }

	/** @brief 角速度の目標値を設定する
	*/
	void SetTargetAngularVelocity(Vec3d angVel) { targetAngVel = angVel; }

	/** @brief 角速度の目標値を設定する
	*/
	Vec3d GetTargetAngularVelocity() { return targetAngVel; }

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

	/** @brief 一時変数の剛体の姿勢を取得する
	*/
	Posed GetSolidTempPose() {return solidTempPose; }


	// --- --- --- --- ---

	virtual bool		AddChildObject(ObjectIf* o);
	virtual ObjectIf*	GetChildObject(size_t pos);
	virtual	size_t		NChildObject() const;
	
	// --- --- --- --- --- --- --- --- --- ---
	// Non API Methods

	/** @brief 一時変数の剛体姿勢を現実の剛体姿勢に合わせる
	*/
	virtual void ApplyExactState() {
		solidTempPose  = solid->GetPose();
	}

	/** @brief 暫定目標地点を取得する
	*/
	void GetTempTarget(PTM::VVector<double> &v);
	void GetTempTarget(PTM::VVector<double> &v, PTM::VVector<double> &w);
	void UpdateLagrangeMultiplier(PTM::VVector<double> &l);

	/** @brief 暫定目標速度を取得する
	*/
	void GetTempVelocity(PTM::VVector<double> &v);

	/** @brief 目標地点までの距離を取得する
	*/
	double PosDeviation() {
		return ((solidTempPose * targetLocalPosition) - targetPosition).norm();
	}
};

}

#endif

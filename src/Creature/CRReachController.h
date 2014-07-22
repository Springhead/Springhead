/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRREACHCONTROLLER_H
#define CRREACHCONTROLLER_H


#include <Creature/CREngine.h>
#include <Creature/SprCRController.h>
#include <Creature/SprCRReachController.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHIK.h>
#include <Creature/SprCRCreature.h>


//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/// 軌道運動コントローラ
class CRReachController : public CRController, public CRReachControllerDesc {
private:
	// 到達運動対象のエンドエフェクタ
	PHIKEndEffectorIf* ikEff;

	// <!!>
	Vec3d lastFinalPos, vMarginalPosLPF;
	bool bWaitingTargetSpeedDown;
	bool bFinished;
	int tempCounter; // <!!>

public:
	SPR_OBJECTDEF(CRReachController);
	ACCESS_DESC_STATE(CRReachController);
	UTRef<ObjectStatesIf>	states;
	Quaterniond	tempori;

	// コンストラクタ
	CRReachController(){ InitVars(); }
	CRReachController(const CRReachControllerDesc& desc) : CRReachControllerDesc(desc) { InitVars(); }
	void InitVars() {
		ikEff = NULL;
		lastFinalPos = Vec3d();
		vMarginalPosLPF = Vec3d();
		bWaitingTargetSpeedDown = false;
		bFinished = true;
		tempCounter = 0; // <!!>
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  親クラス（CRController）のAPI

	/// 初期化を実行する
	virtual void Init() {
		currPos  = ikEff->GetSolid()->GetPose() * ikEff->GetTargetLocalPosition();
		currVel  = Vec3d();
		initPos  = currPos;
		initVel  = currVel;
	}

	/// 制御処理を実行する
	virtual void Step();

	///  状態をリセットする
	virtual void Reset() {
		time      =  0;
		reachTime = -1;
		viaTime   = -1;
	}

	///  現状を返す
	virtual int  GetStatus() {
		if (this->reachTime <= 0) {
			return CRControllerDesc::CS_WAITING;
		} else {
			return CRControllerDesc::CS_WORKING;
		}
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  このクラスのAPI

	/** @brief 到達に使うエンドエフェクタを設定・取得する
	*/
	void SetIKEndEffector(PHIKEndEffectorIf* ikEff) { this->ikEff = ikEff; }
	PHIKEndEffectorIf* GetIKEndEffector() { return this->ikEff; }

	/** @brief 最終到達目標位置をセットする
	*/
	void SetFinalPos(Vec3d pos) { this->finalPos = pos; }

	/** @brief 最終到達目標速度をセットする（デフォルトは (0,0,0)）
	*/
	void SetFinalVel(Vec3d vel) { this->finalVel = vel; }
	
	/** @brief 経由地点通過時刻をセットする（負の場合、経由地点を用いない）
	*/
	void SetViaTime(float time) { viaTime = time; }

	/** @brief 経由地点をセットする
	*/
	void SetViaPos(Vec3d pos) { viaPos = pos; }

	// ----- ----- -----

	/** @brief 平均到達速度をセットする（ここから目標到達時間が計算される）
	*/
	void SetAverageSpeed(double speed) { this->averageSpeed = speed; }
	double GetAverageSpeed() { return this->averageSpeed; }

	/** @brief マージン（FinalPosからこの半径内に到達すればよい）をセットする
	*/
	void SetMargin(double margin) { this->margin = margin; }

	/** @brief 目標がこの速度以上になったら到達目標の更新を一旦停止
	*/
	void SetWaitVel(double vel) { this->waitVel = vel; }

	/** @brief Wait後に目標がこの速度以下になったら到達運動をリスタート
	*/
	void SetRestartVel(double vel) { this->restartVel = vel; }

	// ----- ----- -----

	/** @brief 軌道の通過点を返す s=0.0～1.0
	*/
	Vec6d CRReachController::GetTrajectory(float s);

	/** @brief 目標到達時間を返す
	*/
	float GetReachTime() { return this->reachTime; }

	/** @brief 現在時刻を返す
	*/
	float GetCurrentTime() { return this->time; }

	/** @brief デバッグ情報を描画する
	*/
	void Draw();

	// ----- ----- -----

	/** @brief 姿勢制御完了時の時間の割合をセットする
	*/
	void SetOriControlCompleteTimeRatio(float oriTime) {
		this->oricontTimeRatio = oriTime;
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// 子要素の扱い

	virtual size_t NChildObject() const {
		return( ((ikEff==NULL)?0:1) );
	}

	virtual ObjectIf* GetChildObject(size_t i) {
		if (i==0) {
			return ikEff;
		}
		return NULL;
	}

	virtual bool AddChildObject(ObjectIf* o) {
		if (!ikEff) { ikEff = o->Cast(); if (ikEff) { return true; } }
		return false;
	}

	virtual bool DelChildObject(ObjectIf* o) {
		if (o==ikEff) { ikEff = NULL; return true; }
		return false;
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  非API関数

	double GetLength();

	void FrontKeep();

};
}
//@}

#endif//CRREACHCONTROLLER_H

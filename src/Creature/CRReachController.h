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
	Vec3d lastFinalPos, vFinalPosLPF;
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
		vFinalPosLPF = Vec3d();
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

	// ----- ----- -----
	// 高レベルAPI

	/** @brief 最終到達目標位置をセットする
	*/
	void SetFinalPos(Vec3d pos) {
		this->finalPos = pos;
	}

	/** @brief 最終到達目標速度をセットする（デフォルトは (0,0,0)）
	*/
	void SetFinalVel(Vec3d vel) {
		this->finalVel = vel;
	}
	
	/** @brief 平均到達速度をセットする（ここから目標到達時間が計算される）
	*/
	void SetAverageSpeed(double speed) {
		this->averageSpeed = speed;
	}
	double GetAverageSpeed() {
		return this->averageSpeed;
	}

	/** @brief 許容位置誤差をセットする（これ以上の誤差がある限り再度挑戦する）
	*/
	void SetAcceptablePosError(double err) {
		this->acceptablePosError = err;
	}

	/** @brief 最終到達目標位置が現在の目標到達位置からこれ以上遠ざかると到達運動を強制的に再始動する
	*/
	void SetRestartDistance(double dist) {
		this->restartDistance = dist;
	}

	/** @brief 到達予定時刻経過後の待ち時間をセットする
	*/
	void SetReachTimeMargin(double margin) {
		this->reachTimeMargin = margin;
	}

	/** @brief 姿勢制御完了時の時間の割合をセットする
	*/
	void SetOriControlCompleteTimeRatio(float oriTime) {
		this->oricontTimeRatio = oriTime;
	}

	// ----- ----- -----
	// 低レベルAPI

	/** @brief 到達運動を開始する
	*/
	void Start(float reachTime) {
		if (this->reachTime <= 0) {
			// 待機中：手先の現在の位置を初期状態とする
			Vec3d eefPos  = ikEff->GetTargetLocalPosition();
			PHSolidIf* so = ikEff->GetSolid();
			currPos  = so->GetPose() * eefPos;
			currVel  = Vec3d(); // so->GetVelocity() + (so->GetAngularVelocity() % eefPos);
			currOri  = so->GetPose().Ori();
			currAVel = Vec3d(); // so->GetAngularVelocity();
		}
		// 現在位置から滑らかに接続する
		initPos = currPos; initVel  = currVel;
		initOri = currOri; initAVel = currAVel;

		this->reachTime = reachTime;
		this->time      = 0;
	}

	/** @brief 経由地点通過時刻をセットする（負の場合、経由地点を用いない）
	*/
	void SetViaTime(float time) { viaTime = time; }


	/** @brief 到達目標位置をセットする
	*/
	void SetTargetPos(Vec3d pos) { targPos = pos; }

	/** @brief 到達目標速度をセットする（デフォルトは (0,0,0)）
	*/
	void SetTargetVel(Vec3d vel) { targVel = vel; }

	/** @brief 経由地点をセットする
	*/
	void SetViaPos(Vec3d pos) { viaPos = pos; }


	/** @brief 到達目標姿勢をセットする
	*/
	void SetTargetOri(Quaterniond ori) { targOri = ori; }

	/** @brief 到達目標角速度をセットする（デフォルトは (0,0,0)）
	*/
	void SetTargetAVel(Vec3d avel) { targAVel = avel; }

	/** @brief 経由姿勢をセットする
	*/
	void SetViaOri(Quaterniond ori) { viaOri = ori; }


	/** @brief 軌道の通過点を返す s=0.0～1.0
	*/
	Vec6d CRReachController::GetTrajectory(float s);

	/** @brief 目標到達時間を返す
	*/
	float GetReachTime() { return this->reachTime; }

	/** @brief 現在時刻を返す
	*/
	float GetCurrentTime() { return this->time; }

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

	/** @brief 到達に使うエンドエフェクタを設定・取得する
	*/
	void SetIKEndEffector(PHIKEndEffectorIf* ikEff) { this->ikEff = ikEff; }
	PHIKEndEffectorIf* GetIKEndEffector() { return this->ikEff; }

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  非API関数

	double GetLength();

	void FrontKeep();

	#if 0
	/** @brief 軌道の通過点を追加する
	*/
	void AddNode(CRReachNode node);

	/** @brief i番目（時刻ベース）の通過点を取得する
	*/
	CRReachNode GetReachNode(int i);

	/** @brief 通過点を数を取得する
	*/
	int NReachNodes() { return (int)(nodes.size()); }

	/** @brief 時刻tにおけるの通過点を取得する
	*/
	CRReachNode GetReachNodeAt(float time);

	/** @brief i番目（時刻ベース）の通過点を設定する
		（追加した軌道通過点を後から編集したい場合に使う．普通は使わない）
	*/
	void SetReachNode(int i, CRReachNode node);

	/** @brief 現在通過中の点を取得する
	*/
	CRReachNode GetCurrentNode();

	/** @brief 軌道の通過点を全削除する
	*/
	void ClearReach(bool apply=0);

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  非API関数

	/** @brief 軌道を計画する
	*/
	void Plan();

	/** @brief 軌道の一部分を計画する
	*/
	void PlanSegment(CRReachNode &from, CRReachNode &to);
	#endif
};
}
//@}

#endif//CRREACHCONTROLLER_H

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

#include <vector>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/// 軌道運動コントローラ
class CRReachController : public CRController, public CRReachControllerDesc {
private:
	// 到達運動対象のエンドエフェクタ
	std::vector<PHIKEndEffectorIf*> ikEffs;
	std::vector<bool>               ikEffUseFlags;
	std::vector<PHJointIf*>         baseJoints;

	// <!!>
	Vec3d lastMarginalPos, vMarginalPosLPF;
	bool bWaitingTargetSpeedDown;
	bool bFinished;
	int tempCounter; // <!!>

	bool bLookatMode;
	bool bForceRestart;

	int numUseHands; // 使用する手の本数 (負の場合すべて使う)

public:
	SPR_OBJECTDEF(CRReachController);
	ACCESS_DESC_STATE(CRReachController);

	UTRef<ObjectStatesIf>	states;
	Quaterniond	tempori;

	// コンストラクタ
	CRReachController(){
		InitVars(); states = ObjectStatesIf::Create();
	}
	CRReachController(const CRReachControllerDesc& desc) : CRReachControllerDesc(desc) { InitVars(); }
	void InitVars() {
		lastMarginalPos = Vec3d();
		vMarginalPosLPF = Vec3d();
		bWaitingTargetSpeedDown = false;
		bFinished = true;
		tempCounter = 0; // <!!>
		bLookatMode = false;
		bForceRestart = false;
		numUseHands = -1;
	}

	Vec3d GetTipPos() {
		Vec3d tipPosSum = Vec3d();
		double num = 0;
		for (size_t i=0; i<ikEffs.size(); ++i) {
			if (ikEffUseFlags[i]) {
				tipPosSum += (ikEffs[i]->GetSolid()->GetPose() * ikEffs[i]->GetTargetLocalPosition());
				num += 1;
			}
		}
		return tipPosSum * (1/num);
	}

	Vec3d GetTipDir() {
		Vec3d tipDirSum = Vec3d();
		double num = 0;
		for (size_t i=0; i<ikEffs.size(); ++i) {
			if (ikEffUseFlags[i]) {
				tipDirSum += (ikEffs[i]->GetSolid()->GetPose().Ori() * ikEffs[i]->GetTargetLocalDirection());
				num += 1;
			}
		}
		return tipDirSum * (1/num);
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  親クラス（CRController）のAPI

	/// 初期化を実行する
	virtual void Init() {
		currPos  = GetTipPos();
		currVel  = Vec3d();

		initPos  = currPos;
		initVel  = currVel;
		lastMarginalPos = finalPos = currPos;

		for (size_t i=0; i<ikEffs.size(); ++i) {
			ikEffs[i]->SetTargetPosition(currPos); // <!!> Offset付けられるようにする
		}
	}

	/// 制御処理を実行する
	virtual void Step();

	///  状態をリセットする
	virtual void Reset();

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
	void SetIKEndEffector(PHIKEndEffectorIf* ikEff, int n=0) {
		while ((int) ikEffs.size() <= n) {
			ikEffs.push_back(NULL);
			ikEffUseFlags.push_back(true);
			baseJoints.push_back(NULL);
		}
		this->ikEffs[n] = ikEff;
	}
	PHIKEndEffectorIf* GetIKEndEffector(int n=0) {
		if ((int) ikEffs.size() <= n) { return NULL; }
		return this->ikEffs[n];
	}

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

	/** @brief 視線モードを有効にする
	*/
	void EnableLookatMode(bool bEnable) { bLookatMode = bEnable; }
	bool IsLookatMode() { return bLookatMode; }

	/** @brief 手の使用数を設定・取得する
	*/
	void SetNumUseHands(int n) { numUseHands = n; }
	int GetNumUseHands() { return numUseHands; }

	/** @brief i番目の腕の付け根関節をセットする（距離に基づく使用判定に使う）
	*/
	void SetBaseJoint(int n, PHJointIf* jo) {
		if ((int) baseJoints.size() <= n) { baseJoints.resize(n+1); }
		baseJoints[n] = jo;
	}

	// ----- ----- -----

	/** @brief 平均到達速度をセットする（ここから目標到達時間が計算される）
	*/
	void SetAverageSpeed(double speed) { this->averageSpeed = speed; }
	double GetAverageSpeed() { return this->averageSpeed; }

	/** @brief マージン（FinalPosからこの半径内に到達すればよい）をセットする
	*/
	void SetMargin(double margin) { this->margin = margin; }

	/** @brief 内側マージン（FinalPosからこの半径の中には入らないようにする）をセットする
	*/
	void SetInnerMargin(double margin) { this->innerMargin = margin; }

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
		return ikEffs.size();
	}

	virtual ObjectIf* GetChildObject(size_t i) {
		if (0 <= i && i < ikEffs.size()) {
			return ikEffs[i];
		}
		return NULL;
	}

	virtual bool AddChildObject(ObjectIf* o) {
		PHIKEndEffectorIf* ie = o->Cast();
		if (ie) {
			ikEffs.push_back(ie);
			ikEffUseFlags.push_back(true);
			baseJoints.push_back(NULL);
			return true;
		}
		return false;
	}

	virtual bool DelChildObject(ObjectIf* o) {
		PHIKEndEffectorIf* ie = o->Cast();
		if (ie) {
			std::vector<PHIKEndEffectorIf*>::iterator it = std::find(ikEffs.begin(), ikEffs.end(), ie);
			if (it != ikEffs.end()) {
				ikEffs.erase(it);
				return true;
			}
		}
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

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRREACHCONTROLLERIF_H
#define SPR_CRREACHCONTROLLERIF_H

#include <Foundation/SprObject.h>
#include <Creature/SprCRController.h>

namespace Spr{;

///	軌道運動コントローラ
struct CRReachControllerIf : public CRControllerIf{
	SPR_IFDEF(CRReachController);

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// 高レベルAPI

	/** @brief 最終到達目標位置をセットする
	*/
	void SetTargetPos(Vec3d pos);

	/** @brief 平均到達速度をセットする（ここから目標到達時間が計算される）
	*/
	void SetAverageSpeed(double speed);
	double GetAverageSpeed();

	/** @brief WaitステートからTrajステートへ移行する際の最低待ち時間をセットする
	*/
	void SetMinWait(double t);

	/** @brief マージンをセットする
	*/
	void SetPosInnerMargin(double margin);
	void SetPosOuterMargin(double margin);
	void SetDirInnerMargin(double margin);
	void SetDirOuterMargin(double margin);

	/** @brief 軌道の通過点を返す s=0.0～1.0
	*/
	Vec3d GetTrajectory(float s);

	/** @brief 目標到達時間を返す
	*/
	float GetReachTime();

	/** @brief 現在時刻を返す
	*/
	float GetCurrentTime();

	/** @brief 到達に使うエンドエフェクタを設定・取得する
	*/
	void SetIKEndEffector(PHIKEndEffectorIf* ikEff);
	PHIKEndEffectorIf* GetIKEndEffector();
};

//@{
///	軌道運動コントローラのState
struct CRReachControllerState{
	/// 軌道運動開始からの経過時間
	float time;
	
	/// 到達予定時刻（運動開始時を0とする）
	float reachTime;

	/// 経由点通過予定時刻（経由点を使わない場合は負の値とする）
	float viaTime;

	/// 軌道始点・終点の位置・速度・姿勢
	Vec3d pos0, vel0, pos1, vel1, pos1AtStartTime;
	Quaterniond ori0, ori1;

	/// 経由点の目標位置
	Vec3d viaPos;

	/// 現在の位置・速度
	Vec3d currPos, currVel;

	/// 目標位置
	Vec3d targetPos, lastTargetPos, tempTargetPos, lastTempTargetPos, tempTargetPosAtStartTime;

	// マージン
	double posInnerMargin, posOuterMargin, dirInnerMargin, dirOuterMargin;

	// マージン内外状態
	enum CRReachInOut { CRREACH_IN, CRREACH_OUT };
	CRReachInOut posInOut, dirInOut;

	// 制御状態
	enum CRReachStat { CRREACH_TRAJ, CRREACH_FOLLOW, CRREACH_WAIT };
	CRReachStat posStat, dirStat;

	// WAIT状態に入った時刻
	double posWaitStarted, dirWaitStarted;

	CRReachControllerState(){
		time = 0; reachTime = -1; viaTime = -1;

		pos0  = Vec3d(); vel0  = Vec3d(); ori0  = Quaterniond();
		pos1  = Vec3d(); vel1  = Vec3d(); ori1  = Quaterniond(); pos1AtStartTime = Vec3d();

		currPos = Vec3d(); currVel = Vec3d();
		targetPos = Vec3d(); lastTargetPos = Vec3d(); tempTargetPos = Vec3d(); lastTempTargetPos = Vec3d(); tempTargetPosAtStartTime = Vec3d();
		viaPos = Vec3d();

		posInnerMargin = 0.0;    dirInnerMargin = Rad(0);
		posOuterMargin = 0.1;    dirOuterMargin = Rad(10);
		posInOut = CRREACH_OUT;  dirInOut = CRREACH_OUT;
		posStat  = CRREACH_WAIT; dirStat  = CRREACH_WAIT;
		posWaitStarted = 0;      dirWaitStarted = 0;
	}
};

/// 軌道運動コントローラのデスクリプタ
struct CRReachControllerDesc : public CRControllerDesc, public CRReachControllerState {
	SPR_DESCDEF(CRReachController);

	// 平均到達速度
	double averageSpeed;

	// Wait->Traj最低待ち時間
	double minWait;

	CRReachControllerDesc() {
		averageSpeed       = 0.2;
		minWait            = 0.0;
	}
};

//@}

}

#endif//SPR_CRREACHCONTROLLERIF_H

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
	void SetFinalPos(Vec3d pos);

	/** @brief 最終到達目標速度をセットする（デフォルトは (0,0,0)）
	*/
	void SetFinalVel(Vec3d vel);
	
	/** @brief 平均到達速度をセットする（ここから目標到達時間が計算される）
	*/
	void SetAverageSpeed(double speed);
	double GetAverageSpeed();

	/** @brief 許容位置誤差をセットする（これ以上の誤差がある限り再度挑戦する）
	*/
	void SetAcceptablePosError(double err);

	/** @brief 最終到達目標位置が現在の目標到達位置からこれ以上遠ざかると到達運動を強制的に再始動する
	*/
	void SetRestartDistance(double dist);

	/** @brief 到達予定時刻経過後の待ち時間をセットする
	*/
	void SetReachTimeMargin(double margin);


	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// 低レベルAPI

	/** @brief 到達運動を開始する
	*/
	void Start(float reachTime);

	/** @brief 経由地点通過時刻をセットする（負の場合、経由地点を用いない）
	*/
	void SetViaTime(float time);


	// （注）位置制御・姿勢制御それぞれの有効・無効はIKEndEffectorでセットすること。

	/** @brief 到達目標位置をセットする
	*/
	void SetTargetPos(Vec3d pos);

	/** @brief 到達目標速度をセットする（デフォルトは (0,0,0)）
	*/
	void SetTargetVel(Vec3d vel);

	/** @brief 経由地点をセットする
	*/
	void SetViaPos(Vec3d pos);


	/** @brief 到達目標姿勢をセットする
	*/
	void SetTargetOri(Quaterniond ori);

	/** @brief 到達目標角速度をセットする（デフォルトは (0,0,0)）
	*/
	void SetTargetAVel(Vec3d avel);

	/** @brief 経由姿勢をセットする
	*/
	void SetViaOri(Quaterniond ori);


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
	
	/// 目標到達時刻（運動開始時を0とする）
	float reachTime;

	/// 目標経由点通過時刻（経由点を使わない場合は負の値とする）
	float viaTime;

	/// 運動開始時の位置・速度・姿勢・角速度
	Vec3d initPos, initVel, initAVel;
	Quaterniond initOri;

	/// 経由点の目標位置・姿勢
	Vec3d viaPos;
	Quaterniond viaOri;

	/// 現在到達目標としている位置・速度・姿勢・角速度
	Vec3d targPos, targVel, targAVel;
	Quaterniond targOri;

	/// 現在の位置・速度・姿勢・角速度
	Vec3d currPos, currVel, currAVel;
	Quaterniond currOri;

	/// 最終的なの目標位置・速度・姿勢・角速度
	Vec3d finalPos, finalVel, finalAVel;
	Quaterniond finalOri;

	CRReachControllerState(){
		time = 0; reachTime = -1; viaTime  = -1;
		initPos  = Vec3d(); initVel  = Vec3d(); initOri  = Quaterniond(); initAVel  = Vec3d();
		targPos  = Vec3d(); targVel  = Vec3d(); targOri  = Quaterniond(); targAVel  = Vec3d();
		currPos  = Vec3d(); currVel  = Vec3d(); currOri  = Quaterniond(); currAVel  = Vec3d();
		finalPos = Vec3d(); finalVel = Vec3d(); finalOri = Quaterniond(); finalAVel = Vec3d();
		viaPos   = Vec3d(); viaOri   = Quaterniond();
	}
};

/// 軌道運動コントローラのデスクリプタ
struct CRReachControllerDesc : public CRControllerDesc, public CRReachControllerState {
	SPR_DESCDEF(CRReachController);

	// 平均到達速度
	double averageSpeed;

	// 許容位置誤差
	double acceptablePosError;

	// 到達運動をやりなおす限度
	double restartDistance;

	// 到達目標時刻経過後の待ち時間
	double reachTimeMargin;

	CRReachControllerDesc() {
		averageSpeed       = 0.2;
		acceptablePosError = 0.05;
		restartDistance    = 0.5;
		reachTimeMargin    = 2.0;
	}
};

//@}

}

#endif//SPR_CRREACHCONTROLLERIF_H

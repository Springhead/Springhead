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

	/** @brief 到達に使うエンドエフェクタを設定・取得する
	*/
	void SetIKEndEffector(PHIKEndEffectorIf* ikEff);
	PHIKEndEffectorIf* GetIKEndEffector();

	/** @brief 最終到達目標位置をセットする
	*/
	void SetFinalPos(Vec3d pos);

	/** @brief 最終到達目標速度をセットする（デフォルトは (0,0,0)）
	*/
	void SetFinalVel(Vec3d vel);
	
	/** @brief 経由地点通過時刻をセットする（負の場合、経由地点を用いない）
	*/
	void SetViaTime(float time);

	/** @brief 経由地点をセットする
	*/
	void SetViaPos(Vec3d pos);

	// ----- ----- -----

	/** @brief 平均到達速度をセットする（ここから目標到達時間が計算される）
	*/
	void SetAverageSpeed(double speed);
	double GetAverageSpeed();

	/** @brief マージン（FinalPosからこの半径内に到達すればよい）をセットする
	*/
	void SetMargin(double margin);

	/** @brief 目標がこの速度以上になったら到達目標の更新を一旦停止
	*/
	void SetWaitVel(double vel);

	/** @brief Wait後に目標がこの速度以下になったら到達運動をリスタート
	*/
	void SetRestartVel(double vel);

	// ----- ----- -----

	/** @brief 軌道通過点の位置・速度を返す s=0.0～1.0
	*/
	Vec6d GetTrajectory(float s);

	/** @brief 目標到達時間を返す
	*/
	float GetReachTime();

	/** @brief 現在時刻を返す
	*/
	float GetCurrentTime();

	/** @brief デバッグ情報を描画する
	*/
	void Draw();

	// ----- ----- -----

	/** @brief 姿勢制御完了時の時間の割合をセットする
	*/
	void SetOriControlCompleteTimeRatio(float oriTime);
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

	/// 姿勢制御完了時の時間の割合
	float oricontTimeRatio;

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
		time = 0; reachTime = -1; viaTime  = -1; oricontTimeRatio = 0;
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

	// マージン
	double margin;

	// 平均到達速度
	double averageSpeed;

	// 目標更新待ち速度
	double waitVel;

	// 更新待ち後到達運動再開速度
	double restartVel;

	CRReachControllerDesc() {
		margin             = 0.0;
		averageSpeed       = 5.0;
		waitVel            = 5.0;
		restartVel         = 2.5;
	}
};

//@}

}

#endif//SPR_CRREACHCONTROLLERIF_H

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CR_TRAJECTORY_CONTROLLER_IF_H
#define SPR_CR_TRAJECTORY_CONTROLLER_IF_H

namespace Spr{;

// ------------------------------------------------------------------------------

struct CRSolidIf;

/// 個々の手先の軌道
struct CRTrajectoryIf : CREngineIf {
	SPR_IFDEF(CRTrajectory);

	/** @brief 現在の位置の取得
	*/
	Vec3f GetPosition();

	/** @brief 現在の速度の取得
	*/
	Vec3f GetVelocity();

	/** @brief 現在の姿勢の取得
	*/
	Quaternionf GetOrientation();

	/** @brief 現在の角速度の取得
	*/
	Vec3f GetAngularVelocity();

	/** @brief 軌道運動開始
	*/
	void Start();

	/** @brief 時刻を1ステップ進める
	*/
	void Step();

	/** @brief 到達目標時間をセット
	*/
	void SetTimeLimit(double tL);

	/** @brief 到達目標位置をセット
	*/
	void SetTargetPosition(Vec3f pos);
	void EnablePositionControl(bool enable);

	/** @brief 被到達目標位置をセット
	*/
	void SetTargetPositionInSolid(Vec3f pos);

	/** @brief 到達目標速度をセット
	*/
	void SetTargetVelocity(Vec3f vel);
	void EnableVelocityControl(bool enable);

	/** @brief 到達目標姿勢をセット
	*/
	void SetTargetOrientation(Vec3f ori);
	void EnableOrientationControl(bool enable);

	/** @brief 到達目標角速度をセット
	*/
	void SetTargetAngularVelocity(Vec3f angvel);
	void EnableAngularVelocityControl(bool enable);

	/** @brief 有効・無効セット
	*/
	void Enable(bool enable);
};
/// 個々の手先の軌道のステート
struct CRTrajectoryState {
	bool		posEnabled;
	bool		velEnabled;
	bool		oriEnabled;
	bool		angvelEnabled;
	double		time;
	Vec3f		posStart;
	Vec3f		posEnd;
	Vec3f		velStart;
	Vec3f		velEnd;
	Quaternionf	oriStart;
	Quaternionf	oriEnd;
	Vec3f		angvelStart;
	Vec3f		angvelEnd;
	Vec3f		currentPosition;
	Quaternionf	currentOrientation;

	CRTrajectoryState() {
		time = 0;
	}
};
/// 個々の手先の軌道のデスクリプタ
struct CRTrajectoryDesc : public CRTrajectoryState, public CREngineDesc {
	SPR_DESCDEF(CRTrajectory);

	Vec3f		posInSolid;
	Vec3f		posInWorld;
	double		timeLimit;
	bool		bEnabled;

	CRTrajectoryDesc() {
		timeLimit = 0;
		bEnabled = false;
	}
};


/// 軌道運動運動コントローラのインターフェース
struct CRTrajectoryControllerIf : CREngineIf{
	SPR_IFDEF(CRTrajectoryController);
};

/// 軌道運動コントローラのデスクリプタ
struct CRTrajectoryControllerDesc : public CREngineDesc{
	SPR_DESCDEF(CRTrajectoryController);
};


}

#endif // SPR_CR_TRAJECTORY_CONTROLLER_IF_H

/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CREyeControllerIf_H
#define SPR_CREyeControllerIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

// ------------------------------------------------------------------------------

/// 眼球運動コントローラのステート
struct CREyeControllerState{
	Vec3f currLookatPos; ///< 現在視線移動中の注視点
	Vec3f currLookatVel; ///< 現在視線移動中の注視点の移動速度ベクトル
	Vec3f nextLookatPos; ///< 次の注視点
	Vec3f nextLookatVel; ///< 次の注視点の移動速度ベクトル

	enum ControlState{
		CS_SACCADE, CS_PURSUIT,
	} controlState; ///< 制御の状態

	float saccadeTimer; ///< サッケード制御の時間経過を示すタイマ
};

/// 眼球運動コントローラのデスクリプタ
struct CREyeControllerDesc: public CREyeControllerState{
	PHSolidIf* soLEye; ///< 左目のソリッド
	PHSolidIf* soREye; ///< 右目のソリッド
	
	PHBallJointIf* joLEye; ///< 左目を固定するボールジョイント
	PHBallJointIf* joREye; ///< 右目を固定するボールジョイント

	PHSolidIf* soHead; ///< 頭のソリッド
	PHBallJointIf* joNeck; ///< 頭を固定するボールジョイント

	float Kp; ///< 眼球PD制御のP係数
	float Kd; ///< 眼球PD制御のD係数

	CREyeControllerDesc(){
		currLookatPos = currLookatVel = Vec3f(0,0,0);
		nextLookatPos = nextLookatVel = Vec3f(0,0,0);
		soLEye = NULL; soREye = NULL; joLEye = NULL; joREye = NULL;
		soHead = NULL; joNeck = NULL;

		Kp = 0.3f; Kd = 0.025f;

		controlState = CS_PURSUIT; saccadeTimer = 0.0;
	}
};

/// 眼球運動コントローラのインターフェイス
struct CREyeControllerIf : SceneObjectIf{
	IF_DEF(CREyeController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel)= 0;

	/** @brief 制御処理を実行する
	*/
	virtual void Step()= 0;
};

// ------------------------------------------------------------------------------

/// 首運動コントローラのステート
struct CRNeckControllerState{
};

/// 首運動コントローラのデスクリプタ
struct CRNeckControllerDesc: public CRNeckControllerState{
	PHSolidIf* soHead; ///< 頭のソリッド
	PHBallJointIf* joNeck; ///< 頭を固定しているジョイント

	CRNeckControllerDesc(){
		soHead = NULL;
		joNeck = NULL;
	}
};

/// 首運動コントローラのインターフェイス
struct CRNeckControllerIf : SceneObjectIf{
	IF_DEF(CRNeckController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel)= 0;

	/** @brief 制御処理を実行する
	*/
	virtual void Step()= 0;
};

// ------------------------------------------------------------------------------

/// 視線コントローラのステート
struct CRGazeControllerState{
};

/// 視線コントローラのデスクリプタ
struct CRGazeControllerDesc: public CRGazeControllerState{
	CREyeControllerIf*  crEyeCtrl;
	CRNeckControllerIf* crNeckCtrl;
	CRGazeControllerDesc(){
		crEyeCtrl  = NULL;
		crNeckCtrl = NULL;
	}
};

/// 視線コントローラ（眼球運動・首運動のコントローラの上位機構）のインターフェイス
struct CRGazeControllerIf : SceneObjectIf{
	IF_DEF(CRGazeController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
		@param attractiveness 注意を引く度合い
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel, float attractiveness)= 0;

	/** @brief 制御処理を実行する
	*/
	virtual void Step()= 0;
};

// ------------------------------------------------------------------------------

//@}

}

#endif//SPR_CREyeControllerIf_H

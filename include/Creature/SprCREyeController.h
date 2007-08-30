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

struct PHSolidIf;
struct PHHingeJointIf;
struct CREyeControllerIf;
struct CRNeckControllerIf;

// ------------------------------------------------------------------------------

/// 眼球運動コントローラのステート
struct CREyeControllerState{
	/// 制御の状態
	enum ControlState{
		CS_SACCADE, CS_PURSUIT,
	} controlState; 
};

/// 眼球運動コントローラのデスクリプタ
struct CREyeControllerDesc: public CREyeControllerState{
	PHSolidIf* soLEye; ///< 左目のソリッド
	PHSolidIf* soREye; ///< 右目のソリッド
	PHSolidIf* soHead; ///< 頭のソリッド
	
	PHHingeJointIf* joLEyeX; ///< 左目を固定するボールジョイント
	PHHingeJointIf* joLEyeY; ///< 左目を固定するボールジョイント
	PHHingeJointIf* joREyeX; ///< 右目を固定するボールジョイント
	PHHingeJointIf* joREyeY; ///< 右目を固定するボールジョイント

	CREyeControllerDesc(){
		soHead = soLEye = soREye = NULL; 
		joLEyeX = joLEyeY = joREyeX = joREyeY = NULL;

		controlState = CS_PURSUIT;
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
	PHSolidIf* soChest; ///< 胸部のソリッド
	PHSolidIf* soNeck;  ///< 首のソリッド
	PHSolidIf* soHead;  ///< 頭のソリッド
	PHHingeJointIf* joChestNeckX; ///< 胸と首を固定しているジョイント
	PHHingeJointIf* joChestNeckY; ///< 胸と首を固定しているジョイント
	PHHingeJointIf* joChestNeckZ; ///< 胸と首を固定しているジョイント
	PHHingeJointIf* joNeckHeadX;  ///< 首と頭を固定しているジョイント
	PHHingeJointIf* joNeckHeadZ;  ///< 首と頭を固定しているジョイント

	CRNeckControllerDesc(){
		soChest = soNeck = soHead = NULL;
		joChestNeckX = joChestNeckY = joChestNeckZ = NULL;
		joNeckHeadX  = joNeckHeadZ  = NULL;
	}
};

/// 首運動コントローラのインターフェイス
struct CRNeckControllerIf : SceneObjectIf{
	IF_DEF(CRNeckController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel, float attractiveness)= 0;

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

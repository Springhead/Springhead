/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CR_GAZECONTROLLER_IF_H
#define SPR_CR_GAZECONTROLLER_IF_H

namespace Spr{;

// ------------------------------------------------------------------------------

/// 眼球運動コントローラのインターフェイス
struct CREyeControllerIf : CREngineIf{
	SPR_IFDEF(CREyeController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	void LookAt(Vec3f pos, Vec3f vel);

	/** @brief サッケード中かどうかを返す
	*/
	bool IsSaccading();
};

/// 眼球運動コントローラのステート
struct CREyeControllerState{
	/// 制御の状態
	enum ControlState{
		CS_SACCADE=0, CS_PURSUIT,
	} controlState; 
};

/// 眼球運動コントローラのデスクリプタ
struct CREyeControllerDesc: public CREyeControllerState, public CREngineDesc{
	SPR_DESCDEF(CREyeController);

	/// 頭部剛体を指定するラベル
	std::string labelHeadSolid;
	/// 眼球剛体を指定するラベル
	std::string labelLEyeSolid;
	std::string labelREyeSolid;
	/// 眼球関節を指定するラベル
	std::string labelLEyeJoint;
	std::string labelREyeJoint;

	CREyeControllerDesc(){
		labelHeadSolid = "head";
		labelLEyeSolid = "left_eye";
		labelREyeSolid = "right_eye";
		labelLEyeJoint = "left_eye_joint";
		labelREyeJoint = "right_eye_joint";
	}
};

// ------------------------------------------------------------------------------

/// 首運動コントローラのインターフェイス
struct CRNeckControllerIf : CREngineIf{
	SPR_IFDEF(CRNeckController);

	/** @brief 注視点を設定する
		@param pos   注視点の３次元座標
		@param vel   注視点の移動速度ベクトル
		@param ratio 運動割合（1:完全に対象の方を向く 0:全く動かない）
	*/
	void LookAt(Vec3f pos, Vec3f vel, float ratio);
};

/// 首運動コントローラのステート
struct CRNeckControllerState{
};

/// 首運動コントローラのデスクリプタ
struct CRNeckControllerDesc: public CRNeckControllerState, public CREngineDesc{
	SPR_DESCDEF(CRNeckController);

	/// 頭部剛体を指定するラベル
	std::string labelHeadSolid;

	CRNeckControllerDesc(){
		labelHeadSolid = "head";
	}
};

// ------------------------------------------------------------------------------

/// 視線コントローラ（眼球運動・首運動のコントローラの上位機構）のインターフェイス
struct CRGazeControllerIf : CREngineIf{
	SPR_IFDEF(CRGazeController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
		@param attractiveness 注意を引く度合い
	*/
	void LookAt(Vec3f pos, Vec3f vel, float attractiveness);
};

/// 視線コントローラのステート
struct CRGazeControllerState{
};

/// 視線コントローラのデスクリプタ
struct CRGazeControllerDesc: public CRGazeControllerState, public CREngineDesc{
	SPR_DESCDEF(CRGazeController);

	CRGazeControllerDesc(){
	}
};

}

#endif // SPR_CR_GAZECONTROLLER_IF_H

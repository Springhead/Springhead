/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CR_EYECONTROLLER_IF_H
#define SPR_CR_EYECONTROLLER_IF_H

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

	CREyeControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 首運動コントローラのインターフェイス
struct CRNeckControllerIf : CREngineIf{
	SPR_IFDEF(CRNeckController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	void LookAt(Vec3f pos, Vec3f vel, float attractiveness);
};

/// 首運動コントローラのステート
struct CRNeckControllerState{
};

/// 首運動コントローラのデスクリプタ
struct CRNeckControllerDesc: public CRNeckControllerState, public CREngineDesc{
	SPR_DESCDEF(CRNeckController);

	/// 首を動かしはじめるAttractivenessの閾値
	float lowerAttractiveness;
	/// 首を完全に対象に向けるAttractivenessの閾値
	float upperAttractiveness;

	CRNeckControllerDesc(){
		lowerAttractiveness = -1.0;
		upperAttractiveness =  0.0;
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

// ------------------------------------------------------------------------------

/// 注意コントローラのインタフェース
struct CRAttentionControllerIf : CREngineIf{
	SPR_IFDEF(CRAttentionController);

};

/// 注意コントローラのデスクリプタ
struct CRAttentionControllerDesc : public CREngineDesc{
	SPR_DESCDEF(CRAttentionController);

	CRAttentionControllerDesc(){
	}
};

}

#endif // SPR_CR_EYECONTROLLER_IF_H

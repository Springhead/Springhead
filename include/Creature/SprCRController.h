/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRControllerIf_H
#define SPR_CRControllerIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{a

struct PHSolidIf;
struct PHHingeJointIf;

// ------------------------------------------------------------------------------

/// コントローラのインターフェイス
struct CRControllerIf : SceneObjectIf{
	IF_DEF(CRController);

	/** @brief 初期化を行う
	*/
	virtual void Init()= 0;

	/** @brief 制御処理を実行する
	*/
	virtual void Step()= 0;

	/** @breif 有効・無効を切り替える
	*/
	virtual void SetEnable(bool enable)= 0;

	/** @brief 有効・無効かを返す
	*/
	virtual bool IsEnabled()= 0;
};

/// コントローラのデスクリプタ
struct CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRController);

	CRControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 眼球運動コントローラのインターフェイス
struct CREyeControllerIf : CRControllerIf{
	IF_DEF(CREyeController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel)= 0;
};

/// 眼球運動コントローラのステート
struct CREyeControllerState{
	/// 制御の状態
	enum ControlState{
		CS_SACCADE, CS_PURSUIT,
	} controlState; 
};

/// 眼球運動コントローラのデスクリプタ
struct CREyeControllerDesc: public CREyeControllerState, public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CREyeController);

	CREyeControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 首運動コントローラのインターフェイス
struct CRNeckControllerIf : CRControllerIf{
	IF_DEF(CRNeckController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel, float attractiveness)= 0;
};

/// 首運動コントローラのステート
struct CRNeckControllerState{
};

/// 首運動コントローラのデスクリプタ
struct CRNeckControllerDesc: public CRNeckControllerState, public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRNeckController);

	CRNeckControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 視線コントローラ（眼球運動・首運動のコントローラの上位機構）のインターフェイス
struct CRGazeControllerIf : CRControllerIf{
	IF_DEF(CRGazeController);

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
		@param attractiveness 注意を引く度合い
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel, float attractiveness)= 0;
};

/// 視線コントローラのステート
struct CRGazeControllerState{
};

/// 視線コントローラのデスクリプタ
struct CRGazeControllerDesc: public CRGazeControllerState, public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRGazeController);

	CRGazeControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 注意コントローラのインタフェース
struct CRAttentionControllerIf : CRControllerIf{
	IF_DEF(CRAttentionController);

};

/// 注意コントローラのデスクリプタ
struct CRAttentionControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRAttentionController);

	CRAttentionControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 到達運動コントローラのインターフェース
struct CRReachingControllerIf : CRControllerIf{
	IF_DEF(CRReachingController);

	/** @brief 目標位置を設定する
		@param p 目標位置
		@param v 目標の速度
		@param t 目標到達時間
		@param o 到達後の待機時間
	*/
	virtual void SetTarget(Vec3f p, Vec3f v, float t, float o)= 0;

	/** @brief 目標位置・姿勢を設定する
		@param p 目標位置
		@param v 目標の速度
		@param q 目標姿勢
		@param av 目標角速度
		@param t 目標到達時間
		@param o 到達後の待機時間
	*/
	virtual void SetTarget(Vec3f p, Vec3f v, Quaterniond q, Vec3f av, float t, float o)= 0;

	/** @brief 作動中かどうかを返す
	*/
	virtual bool IsActive()= 0;

	/** @brief リセットする
	*/
	virtual void Reset()= 0;
};

/// 到達運動コントローラのデスクリプタ
struct CRReachingControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRReachingController);

	PHSolidIf* solid; ///< 到達させたい剛体
	CRHingeHumanBodyDesc::CRHumanSolids solidNo; ///< 到達させたい剛体

	float limitForce; ///< 力の最大値
	float springPos, damperPos; ///< 位置制御のバネダンパ係数
	float springOri, damperOri; ///< 姿勢制御のバネダンパ係数
	float softenRate; ///< 関節を柔らかい状態にするためのバネダンパ倍率
	float hardenRate; ///< 関節を固い状態にするためのバネダンパ倍率

	CRReachingControllerDesc(){
		limitForce = 1000.0f;
		springPos = 500.0f;  damperPos =  20.0f;
		springOri =  10.0f;  damperOri =   0.5f;
		softenRate = 0.2f;
		hardenRate = 5.0f;
	}
};

// ------------------------------------------------------------------------------
/// 歩行コントローラのインタフェース
struct CRWalkingControllerIf : CRControllerIf{
	IF_DEF(CRWalkingController);

	/** @brief 歩行の速度を設定する
	*/
	virtual void SetSpeed(float speed)= 0;

	/** @brief 転回角度を設定する
	*/
	virtual void SetRotationAngle(float rot)= 0;
};

/// 歩行コントローラのデスクリプタ
struct CRWalkingControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRWalkingController);

	/// 歩行コントローラの特性を決めるパラメータをここに記述．
	/// 実行中に変化させるものや，ほかの要素（Bodyなど）から計算で求めるものは除く．

	CRWalkingControllerDesc(){
		/// 各変数にデフォルトの値を入れる．ゼロとかNULLでもいい．
	}
};

// ------------------------------------------------------------------------------
/// 偽歩行コントローラ：本当の歩行コントローラができるまでの中継ぎ
struct CREseWalkingControllerIf : CRControllerIf{
	IF_DEF(CREseWalkingController);

	/** @brief 歩行の速度を設定する
	*/
	virtual void SetSpeed(float speed)= 0;

	/** @brief 転回角度を設定する
	*/
	virtual void SetRotationAngle(float rot)= 0;

	/** @brief 位置を設定する（初期状態決定のため）
	*/
	virtual void SetPos(Vec3f pos)= 0;
};

/// 偽歩行コントローラのデスクリプタ
struct CREseWalkingControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CREseWalkingController);

	CREseWalkingControllerDesc(){
	}
};

// ------------------------------------------------------------------------------
/// 移動コントローラ：目標地点への移動と障害物回避
struct CRTravelControllerIf : CRControllerIf{
	IF_DEF(CRTravelController);

	/** @brief 目標地点を設定する
	*/
	virtual void SetGoal(Vec2f goal)= 0;
};

/// 移動コントローラのデスクリプタ
struct CRTravelControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRTravelController);

	CRTravelControllerDesc(){
	}
};

// ------------------------------------------------------------------------------
/// 把持コントローラ
struct CRGrabControllerIf : CRControllerIf{
	IF_DEF(CRGrabController);

	/** @brief 掴む
	*/
	virtual void Grab(PHSolidIf* solid, float radius)= 0;

	/** @brief 現在何か掴んでいれば，放す
	*/
	virtual void Ungrab()= 0;

	/** @brief 現在何か掴んでいれば，それを持って移動する
	*/
	virtual void MoveTo(Vec3f pos)= 0;

	/** @brief 現在の把持の状態を返す
	*/
	enum GrabState{
		GS_STANDBY = 0, /// 待機状態
		GS_GRAB_START,  /// 把持動作開始
		GS_GRAB,        /// 把持中
		GS_MOVE,        /// 把持物体を移動中
	};
	virtual CRGrabControllerIf::GrabState GetGrabState()= 0;
};

/// 把持コントローラのデスクリプタ
struct CRGrabControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRGrabController);

	CRGrabControllerDesc(){
	}
};

//@}

}

#endif//SPR_CRControllerIf_H

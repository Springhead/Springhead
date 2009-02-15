/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	SPR_IFDEF(CRController);

	/** @brief 初期化を行う
	*/
	void Init();

	/** @brief 制御処理を実行する
	*/
	void Step();

	/** @breif 有効・無効を切り替える
	*/
	void SetEnable(bool enable);

	/** @brief 有効・無効かを返す
	*/
	bool IsEnabled();
};

/// コントローラのデスクリプタ
struct CRControllerDesc{
	SPR_DESCDEF(CRController);

	CRControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 眼球運動コントローラのインターフェイス
struct CREyeControllerIf : CRControllerIf{
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
		CS_SACCADE, CS_PURSUIT,
	} controlState; 
};

/// 眼球運動コントローラのデスクリプタ
struct CREyeControllerDesc: public CREyeControllerState, public CRControllerDesc{
	SPR_DESCDEF(CREyeController);

	CREyeControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 首運動コントローラのインターフェイス
struct CRNeckControllerIf : CRControllerIf{
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
struct CRNeckControllerDesc: public CRNeckControllerState, public CRControllerDesc{
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

/// 到達運動コントローラのインターフェース
struct CRReachingControllerIf : CRControllerIf{
	SPR_IFDEF(CRReachingController);

	/** @brief 目標位置の再設定
		@param pos 目標位置
	*/
	void SetPos(Vec3f pos);

	/** @brief 位置を到達させる
		@param pos 目標位置
		@param v 目標速度
		@param t 目標到達時間
	*/
	void Start(Vec3d pose, Vec3d v, float t);

	/** @brief 到達状態を解除する
	*/
	void Stop();

	/** @brief IK制御点の設定
	*/
	void SetIKCP(PHIKPosCtlIf* ikcp);

	/** @brief IK制御点の取得
	*/
	PHIKPosCtlIf* GetIKCP();
};

/// 到達運動コントローラのデスクリプタ
struct CRReachingControllerDesc : public CRControllerDesc{
	SPR_DESCDEF(CRReachingController);
};

/// 到達運動コントローラの集合体のインターフェース
struct CRReachingControllersIf : CRControllerIf{
	SPR_IFDEF(CRReachingControllers);

	/** @brief 到達運動コントローラを取得する
		@param solid 到達対象の剛体
	*/
	CRReachingControllerIf* GetReachingController(PHSolidIf* solid);
};

/// 到達運動コントローラのデスクリプタ
struct CRReachingControllersDesc : public CRControllerDesc{
	SPR_DESCDEF(CRReachingControllers);
};

// ------------------------------------------------------------------------------

/// 視線コントローラ（眼球運動・首運動のコントローラの上位機構）のインターフェイス
struct CRGazeControllerIf : CRControllerIf{
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
struct CRGazeControllerDesc: public CRGazeControllerState, public CRControllerDesc{
	SPR_DESCDEF(CRGazeController);

	CRGazeControllerDesc(){
	}
};

// ------------------------------------------------------------------------------

/// 注意コントローラのインタフェース
struct CRAttentionControllerIf : CRControllerIf{
	SPR_IFDEF(CRAttentionController);

};

/// 注意コントローラのデスクリプタ
struct CRAttentionControllerDesc : public CRControllerDesc{
	SPR_DESCDEF(CRAttentionController);

	CRAttentionControllerDesc(){
	}
};

// ------------------------------------------------------------------------------
/// 倒れたら立とうと努力するコントローラのインタフェース
struct CRTryStandingUpControllerIf : CRControllerIf{
	SPR_IFDEF(CRTryStandingUpController);

	/** @brief 初期化を行う
	*/
	void Init();

	/** @brief 制御のシミュレーションをする
	*/
	void Step();

	/** @brief 遺伝子情報のシンクロだけする
	*/
	void Sync() ;

};
/// 倒れたら立とうと努力するコントローラのデスクリプタ
struct CRTryStandingUpControllerDesc : public CRControllerDesc{
	SPR_DESCDEF(CRTryStandingUpController);
	
	CRTryStandingUpControllerDesc(){
	}
};

// ------------------------------------------------------------------------------
/// 歩行コントローラのインタフェース
struct CRWalkingControllerIf : CRControllerIf{
	SPR_IFDEF(CRWalkingController);

	/** @brief 歩行の速度を設定する
	*/
	void SetSpeed(float speed);

	/** @brief 転回角度を設定する
	*/
	void SetRotationAngle(float rot);

	/** @brief ワールド座標系の進行角度を設定する
	*/
	void SetRotationWorldCoordinate(double r);

	/** @brief 静止する
	*/
	void Stop();

	/** @brief 後ろ向きに歩く
	*/
	void Reverse();

	/** @brief 位置を設定する（初期状態決定のため）
	*/
	void SetPos(Vec3f pos);

	/** @brief 基本歩行周期を取得
	*/
	double GetBasicCycle();

	/** @brief 歩行の継続が不可能か？
	*/
	bool IsCompleteFall();
};

/// 歩行コントローラのデスクリプタ
struct CRWalkingControllerDesc : public CRControllerDesc{
	SPR_DESCDEF(CRWalkingController);

	/// 歩行コントローラの特性を決めるパラメータをここに記述．
	/// 実行中に変化させるものや，ほかの要素（Bodyなど）から計算で求めるものは除く．
	//double paramVelocityX, paramVelocityZ;              //X, Z方向の目標歩行速度 
    //double paramHalfStrideX, paramHalfStrideZ;          //X, Z方向の目標歩幅          
    double paramLdx, paramLdz;                          //両足支持期間に進むX, Z方向の距離 
    double maxHalfStride;                               //歩幅の限界値 
    double height;                                      //目標重心高 
    double minCycleRate;                                //最短着地時間の基本周期に対する割合 
    double maxDSRate;                                   //両足支持期間の最大延長割合 
	double FootLength;                                  //標準状態の脚の長さ
    double MaxFootLength;                               //歩行中の脚の最長値
    double MinFootLength;                               //歩行中の脚の最短値
    double MaxRoGround;                                 //歩行を継続する上体角度の閾値
	double MaxRoLandingSiteFront;                       //着地を維持可能な上体と脚の角度の閾値
	double MaxRoLandingSiteSide;                        //着地を維持可能な上体と脚の角度の閾値
	double MaxRoLandingSiteBack;                        //着地を維持可能な上体と脚の角度の閾値
    double MaxRoConstraint;                             //上体と脚の角度の限界値
	double MaxFootSpeedFront;                           //足の移動速度の限界値
	double MaxFootSpeedSide;                            //足の移動速度の限界値
	double MaxFootSpeedBack;                            //足の移動速度の限界値
	double MaxFootAccelerationFront;                    //足の加速度の最大値
	double MaxFootAccelerationSide;                     //足の加速度の最大値
	double MaxFootAccelerationBack;                     //足の加速度の最大値
    double DoubleSupportLimitRate;                      //バランスが崩れた時に両脚支持期間の長さ決定するパラメータ
    double LimitChange;                                 //どれほどバランスを崩したときに両脚支持期間を縮めるか決定する閾値
	double pi;                                          //π
	double footsize;
	double miu;

	CRWalkingControllerDesc(){
		/// 各変数にデフォルトの値を入れる．ゼロとかNULLでもいい．
		//paramVelocityX = 0.6;           
		//paramHalfStrideX = 0.115;            
		//paramHalfStrideZ = 0.015;          
		paramLdx = 0.00001;             
		paramLdz = 0.0000001;                 
		maxHalfStride = 0.83;                   
		height = 1.1;                   
		minCycleRate = 0.55;                
		maxDSRate = 1.5;     
		FootLength = 0.85;
		MaxFootLength = 1.05;           
		MinFootLength = 0.35;         
		MaxRoGround = 3.14 / 2.0;    
		MaxRoLandingSiteFront = 3.14 / 2.2; 
	    MaxRoLandingSiteSide = 3.14 / 4.0; 
	    MaxRoLandingSiteBack = 3.14 / 3.5; 
		MaxRoConstraint = 3.14 / 2.2; 
		MaxFootSpeedFront = 50.0;    
	    MaxFootSpeedSide = 35.0;
	    MaxFootSpeedBack = 40.0;  
	    MaxFootAccelerationFront = 50.0;  
	    MaxFootAccelerationSide = 35.0;
	    MaxFootAccelerationBack = 40.0; 
		DoubleSupportLimitRate = 0.05;    
		LimitChange = 11.0;  
		pi = 3.1415926535;
		footsize = 0.0619;
		miu = 0.8;
	}
};

// ------------------------------------------------------------------------------
/// 偽歩行コントローラ：本当の歩行コントローラができるまでの中継ぎ
struct CREseWalkingControllerIf : CRControllerIf{
	SPR_IFDEF(CREseWalkingController);

	/** @brief 歩行の速度を設定する
	*/
	void SetSpeed(float speed);

	/** @brief 転回角度を設定する
	*/
	void SetRotationAngle(float rot);

	/** @brief 位置を設定する（初期状態決定のため）
	*/
	void SetPos(Vec3f pos);
};

/// 偽歩行コントローラのデスクリプタ
struct CREseWalkingControllerDesc : public CRControllerDesc{
	SPR_DESCDEF(CREseWalkingController);

	CREseWalkingControllerDesc(){
	}
};

// ------------------------------------------------------------------------------
/// 移動コントローラ：目標地点への移動と障害物回避
struct CRTravelControllerIf : CRControllerIf{
	SPR_IFDEF(CRTravelController);

	/** @brief 目標地点を設定する
	*/
	void SetGoal(Vec3f goal);
};

/// 移動コントローラのデスクリプタ
struct CRTravelControllerDesc : public CRControllerDesc{
	SPR_DESCDEF(CRTravelController);

	CRTravelControllerDesc(){
	}
};

// ------------------------------------------------------------------------------
/// 把持コントローラ
struct CRGrabControllerIf : CRControllerIf{
	SPR_IFDEF(CRGrabController);

	/** @brief 物体の所へ手を伸ばしてつかむ．
	*** @return true: Reach開始した． false: その物体へは手が届かない．
	*/
	bool Reach(PHSolidIf* solid, float radius);

	/** @brief 対象SolidへReachが可能かどうかを返す（距離とか）
	*/
	bool IsReachable(PHSolidIf* solid);

	/** @brief 対象SolidへReachが可能かどうかを返す（距離とか）
	*** @param safety 安全係数：1以下の係数，距離をsafety倍して計算．1.0のときぎりぎり到達可能
	*/
	bool IsReachable(PHSolidIf* solid, float safety);

	/** @brief Reachが完了したかどうかを返す
	*/
	bool IsReachComplete();

	/** @brief 現在物体をつかんでいれば，その物体を手元に引き寄せ保持する．
	*** @return true: Uphold開始した． false: 物体をつかんでいない(Reach未完了含む．)
	*/
	bool Uphold();

	/** @brief Upholdが可能かどうかを返す
	*/
	bool IsUpholdable();

	/** @brief Upholdが完了したかどうかを返す
	*/
	bool IsUpholdComplete();

	/** @brief 現在物体をつかんでいれば，その物体を特定の場所に置く．
	*** @return true: Place開始した． false: その場所へは手が届かない，または物体を持ってない．
	*/
	bool Place(Vec3d pos);

	/** @brief Placeが可能かどうかを返す
	*/
	bool IsPlaceable(Vec3d pos);

	/** @brief Placeが可能かどうかを返す
	*** @param safety 安全係数：1以下の係数，距離をsafety倍して計算．1.0のときぎりぎり到達可能
	*/
	bool IsPlaceable(Vec3d pos, float safety);

	/** @brief Placeが完了したかどうかを返す
	*/
	bool IsPlaceComplete();

	/** @brief 現在の動作を中断する
	*/
	void Abort();

	/** @brief すべての把持動作を中断する
	*/
	void AbortAll();

	/** @brief 現在の動作状態を返す
	*/
	enum CRGCControlState {
		CRGC_STANDBY=0,
		CRGC_REACH,  CRGC_REACH_COMPLETE,
		CRGC_UPHOLD, CRGC_UPHOLD_COMPLETE,
		CRGC_PLACE,  CRGC_PLACE_COMPLETE,
	};
	CRGrabControllerIf::CRGCControlState GetControlState();
};

/// 把持コントローラのデスクリプタ
struct CRGrabControllerDesc : public CRControllerDesc {
	SPR_DESCDEF(CRGrabController);

	/// 体を柔らかくするためのバネダンパへの係数
	double rateSpringSoften, rateDamperSoften;

	/// 体をかたくするためのバネダンパへの係数
	double rateSpringHarden, rateDamperHarden;

	CRGrabControllerDesc(){
		rateSpringSoften = 0.0;  rateDamperSoften = 0.5;
		rateSpringHarden = 1.5;  rateDamperHarden = 1.5;
	}
};

//@}

}

#endif//SPR_CRControllerIf_H

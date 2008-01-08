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

	/** @brief サッケード中かどうかを返す
	*/
	virtual bool IsSaccading()= 0;
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
	IF_DEF(CRReachingController);

	/** @brief 制御対象の剛体を返す
	*/
	virtual PHSolidIf* GetSolid()= 0;

	/** @brief 目標位置を設定する
		（現在の実装では、到達運動の途中でも目標位置の変更は反映される。）
		@param p 目標位置
		@param v 目標の速度
	*/
	virtual void SetTargetPos(Vec3f p, Vec3f v)= 0;

	/** @brief 目標姿勢を設定する
		（現在の実装では、到達運動の途中でも目標姿勢の変更は反映される。）
		@param q 目標姿勢
		@param av 目標角速度
	*/
	virtual void SetTargetOri(Quaterniond q, Vec3f av)= 0;

	/** @brief 到達目標時間を設定する
		@param t 目標到達時間
	*/
	virtual void SetTargetTime(float t)= 0;

	/** @brief 到達運動を開始する
		（現在の実装では、到達運動の途中でもただちに新しい条件で運動を開始する。）
		@param mode 到達の拘束モード
		@param keeptime 到達運動終了後に保持を続ける時間（負なら保持を続ける）
	*/
	enum ConstraintMode {
		CM_P3R0=0,  // ３自由度拘束（位置のみを合わせる）
		CM_P3R2,    // ３＋２自由度拘束（位置と向きを合わせるが、向きに１軸の自由度がある）
		CM_P3R3,    // ３＋３自由度拘束（位置と向きを合わせる）
	};
	virtual void Start(CRReachingControllerIf::ConstraintMode mode, float keeptime)=0;

	/** @brief 制御の残り時間を返す
	*/
	virtual float GetRemainingTime()= 0;

	/** @brief 到達状況を返す
	*/
	enum ReachState {
		RS_NOTHING_REACHED=0,  // 目標剛体も対象剛体も到達していない
		RS_TARGET_REACHED,     // 目標剛体は到達した（対象剛体はまだ追いついていない）
		RS_SOLID_REACHED,      // 対象剛体も到達した
		RS_STOP,               // 動作していない
	};
	virtual CRReachingControllerIf::ReachState GetReachState()= 0;

	/** @brief 到達状態をやめる
	*/
	virtual void Reset()= 0;
};

/// 到達運動コントローラのデスクリプタ
struct CRReachingControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRReachingController);

	PHSolidIf*  solid;    ///< 到達させたい剛体
	Vec3f       reachPos; ///< 剛体内の到達させたいポイント
	Quaterniond fixOri;   ///< HingeJointを入れる向き

	float limitForce; ///< 力の最大値
	float springPos;  ///< 位置制御のバネ係数
	float damperPos;  ///< 位置制御のダンパ係数
	float springOri;  ///< 姿勢制御のバネ係数
	float damperOri;  ///< 姿勢制御のダンパ係数

	CRReachingControllerDesc(){
		solid = NULL;
		limitForce = 1000.0f;
		springPos = 500.0f;  damperPos =  20.0f;
		springOri = 500.0f;  damperOri =  20.0f;
		reachPos = Vec3f(0,0,0);
		fixOri = Quaterniond();
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
/// 倒れたら立とうと努力するコントローラのインタフェース
struct CRTryStandingUpControllerIf : CRControllerIf{
	IF_DEF(CRTryStandingUpController);

	/** @brief 初期化を行う
	*/
	virtual void Init() = 0;

	/** @brief 制御のシミュレーションをする
	*/
	virtual void Step() = 0;

	/** @brief 遺伝子情報のシンクロだけする
	*/
	virtual void Sync() =0;

};
/// 倒れたら立とうと努力するコントローラのデスクリプタ
struct CRTryStandingUpControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRTryStandingUpController);
	
	CRTryStandingUpControllerDesc(){
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

	/** @brief ワールド座標系の進行角度を設定する
	*/
	virtual void SetRotationWorldCoordinate(double r)= 0;

	/** @brief 静止する
	*/
	virtual void Stop()= 0;

	/** @brief 後ろ向きに歩く
	*/
	virtual void Reverse()= 0;

	/** @brief 位置を設定する（初期状態決定のため）
	*/
	virtual void SetPos(Vec3f pos)= 0;

	/** @brief 基本歩行周期を取得
	*/
	virtual double GetBasicCycle()= 0;

	/** @brief 歩行の継続が不可能か？
	*/
	virtual bool IsCompleteFall()= 0;
};

/// 歩行コントローラのデスクリプタ
struct CRWalkingControllerDesc : public CRControllerDesc{
	DESC_DEF_FOR_OBJECT(CRWalkingController);

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
	virtual void SetGoal(Vec3f goal)= 0;
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

	/** @brief 物体の所へ手を伸ばしてつかむ．
	*** @return true: Reach開始した． false: その物体へは手が届かない．
	*/
	virtual bool Reach(PHSolidIf* solid, float radius)= 0;

	/** @brief 対象SolidへReachが可能かどうかを返す（距離とか）
	*/
	virtual bool IsReachable(PHSolidIf* solid)= 0;

	/** @brief 対象SolidへReachが可能かどうかを返す（距離とか）
	*** @param safety 安全係数：1以下の係数，距離をsafety倍して計算．1.0のときぎりぎり到達可能
	*/
	virtual bool IsReachable(PHSolidIf* solid, float safety)= 0;

	/** @brief Reachが完了したかどうかを返す
	*/
	virtual bool IsReachComplete()= 0;

	/** @brief 現在物体をつかんでいれば，その物体を手元に引き寄せ保持する．
	*** @return true: Uphold開始した． false: 物体をつかんでいない(Reach未完了含む．)
	*/
	virtual bool Uphold()= 0;

	/** @brief Upholdが可能かどうかを返す
	*/
	virtual bool IsUpholdable()= 0;

	/** @brief Upholdが完了したかどうかを返す
	*/
	virtual bool IsUpholdComplete()= 0;

	/** @brief 現在物体をつかんでいれば，その物体を特定の場所に置く．
	*** @return true: Place開始した． false: その場所へは手が届かない，または物体を持ってない．
	*/
	virtual bool Place(Vec3d pos)= 0;

	/** @brief Placeが可能かどうかを返す
	*/
	virtual bool IsPlaceable(Vec3d pos)= 0;

	/** @brief Placeが可能かどうかを返す
	*** @param safety 安全係数：1以下の係数，距離をsafety倍して計算．1.0のときぎりぎり到達可能
	*/
	virtual bool IsPlaceable(Vec3d pos, float safety)= 0;

	/** @brief Placeが完了したかどうかを返す
	*/
	virtual bool IsPlaceComplete()= 0;

	/** @brief 現在の動作を中断する
	*/
	virtual void Abort()= 0;

	/** @brief すべての把持動作を中断する
	*/
	virtual void AbortAll()= 0;

	/** @brief 現在の動作状態を返す
	*/
	enum CRGCControlState {
		CRGC_STANDBY=0,
		CRGC_REACH,  CRGC_REACH_COMPLETE,
		CRGC_UPHOLD, CRGC_UPHOLD_COMPLETE,
		CRGC_PLACE,  CRGC_PLACE_COMPLETE,
	};
	virtual CRGrabControllerIf::CRGCControlState GetControlState()= 0;
};

/// 把持コントローラのデスクリプタ
struct CRGrabControllerDesc : public CRControllerDesc {
	DESC_DEF_FOR_OBJECT(CRGrabController);

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

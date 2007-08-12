/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRReachingMovementIf_H
#define SPR_CRReachingMovementIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

// ------------------------------------------------------------------------------
/// 到達運動コントローラのデスクリプタ
struct CRReachingMovementDesc{
	PHSolidIf* solid; ///< 制御対象のソリッド
	float limitForce; ///< 力の最大値
	float springPos, damperPos; ///< 位置制御のバネダンパ係数
	float springOri, damperOri; ///< 姿勢制御のバネダンパ係数

	CRReachingMovementDesc(){
		solid = NULL;
		limitForce = 1000.0f;
		springPos = 500.0;  damperPos =  20.0;
		springOri =  10.0;  damperOri =   0.5;
	}
};

/// 到達運動コントローラのインターフェース
struct CRReachingMovementIf : SceneObjectIf{
	IF_DEF(CRReachingMovement);

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

	/** @ brief 制御のステップを実行する
	*/
	virtual void Step()= 0;
};

//@}

}

#endif//SPR_CRReachingMovementIf_H

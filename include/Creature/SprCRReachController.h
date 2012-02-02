/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CR_REACHCONTROLLER_IF_H
#define SPR_CR_REACHCONTROLLER_IF_H

#include <Creature/SprCREngine.h>

namespace Spr{;

// ------------------------------------------------------------------------------

struct PHSolidIf;
struct CRIKSolidIf;

/// 到達運動コントローラのインターフェース
struct CRReachingControllerIf : CREngineIf{
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

	/** @brief 制御対象剛体の設定
	*/
	void SetCRSolid(CRIKSolidIf* cso);

	/** @brief IK制御点の取得
	*/
	CRIKSolidIf* GetCRSolid();
};

/// 到達運動コントローラのデスクリプタ
struct CRReachingControllerDesc : public CREngineDesc{
	SPR_DESCDEF(CRReachingController);
};

/// 到達運動コントローラの集合体のインターフェース
struct CRReachingControllersIf : CREngineIf{
	SPR_IFDEF(CRReachingControllers);

	/** @brief 到達運動コントローラを取得する
		@param solid 到達対象の剛体
	*/
	CRReachingControllerIf* GetReachingController(PHSolidIf* solid);
};

/// 到達運動コントローラの集合体のデスクリプタ
struct CRReachingControllersDesc : public CREngineDesc{
	SPR_DESCDEF(CRReachingControllers);
};

// ------------------------------------------------------------------------------
/// 把持コントローラ（つかんで，離すだけ）
struct CRGrabControllerIf : CREngineIf{
	SPR_IFDEF(CRGrabController);

	/** @brief 指定した物体をつかむ．
	*/
	void Grab(PHSolidIf* targetSolid);

	/** @brief つかんでいる物体を返す．つかんでいなければNULL
	*/
	PHSolidIf* GetGrabbingSolid();

	/** @brief つかんでいる物体を放す．
	*/
	void Release();
};

/// 把持コントローラのデスクリプタ
struct CRGrabControllerDesc : public CREngineDesc {
	SPR_DESCDEF(CRGrabController);

	CRGrabControllerDesc(){
	}
};

}

#endif // SPR_CR_REACHCONTROLLER_IF_H

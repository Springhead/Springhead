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
/// 把持コントローラ
struct CRGrabControllerIf : CREngineIf{
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
struct CRGrabControllerDesc : public CREngineDesc {
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

}

#endif // SPR_CR_REACHCONTROLLER_IF_H

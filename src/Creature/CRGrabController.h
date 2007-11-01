/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRGRABCONTROLLER_H
#define CRGRABCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include "CRController.h"

#include <map>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 把持コントローラ
*/
class CRGrabController : public CRController, public CRGrabControllerIfInit, public CRGrabControllerDesc {
private:
	/// 下位のコントローラ：ReachingController
	CRReachingControllerIf *reachLeft, *reachRight, *reachChest;

	/// ボディ
	CRBodyIf* body;

	/// 腰の剛体（いろいろ基準になる）
	PHSolidIf* soWaist;

	/// 胸の剛体（やはりいろいろ基準になる）
	PHSolidIf* soChest;

	/// 腰を中心に到達可能な距離
	float reachableDistance;

	/// 腰からみた肩の高さ
	float shoulderHeightFromWaist;

	/// 把持対象のSolid
	PHSolidIf* targetSolid;

	/// 把持対象の大きさ（半径）
	float targetRadius;

	/// 把持した物体を置く(Place)時の目標位置
	Vec3f placePos;

	/// 現在の制御状態
	CRGrabControllerIf::CRGCControlState controlState;

	/// 現在の物体を把持するためのSpringのペア
	typedef std::pair<PHSpringIf*, PHSpringIf*> SpringPair;
	SpringPair grabSpring;

	/// 把持を行うための作成済Springのマップ
	//// 把持対象の剛体→（左手連結ばね・右手連結ばね）
	typedef std::map< PHSolidIf*, std::pair<PHSpringIf*, PHSpringIf*> > GrabSpringMap;
	GrabSpringMap grabSpringMap;

	///
	int waittime;

public:
	OBJECTDEF(CRGrabController, CRController);
	ACCESS_DESC(CRGrabController);

	CRGrabController(){}
	CRGrabController(const CRGrabControllerDesc& desc, CRCreatureIf* c=NULL) 
		: CRGrabControllerDesc(desc) 
		, CRController((const CRControllerDesc&)desc, c)
	{
	}

	/** @brief 初期化
	*/
	virtual void Init();

	/** @brief １ステップ
	*/
	virtual void Step();

	/** @brief 物体の所へ手を伸ばしてつかむ．
	*** @return true: Reach開始した． false: その物体へは手が届かない．
	*/
	virtual bool Reach(PHSolidIf* solid, float radius);

	/** @brief 対象SolidへReachが可能かどうかを返す（距離とか）
	*/
	virtual bool IsReachable(PHSolidIf* solid);

	/** @brief 対象SolidへReachが可能かどうかを返す（距離とか）
	*** @param safety 安全係数：1以下の係数，距離をsafety倍して計算．1.0のときぎりぎり到達可能
	*/
	virtual bool IsReachable(PHSolidIf* solid, float safety);

	/** @brief Reachが完了したかどうかを返す
	*/
	virtual bool IsReachComplete();

	/** @brief 現在物体をつかんでいれば，その物体を手元に引き寄せ保持する．
	*** @return true: Uphold開始した． false: 物体をつかんでいない(Reach未完了含む．)
	*/
	virtual bool Uphold();

	/** @brief Upholdが可能かどうかを返す
	*/
	virtual bool IsUpholdable();

	/** @brief Upholdが完了したかどうかを返す
	*/
	virtual bool IsUpholdComplete();

	/** @brief 現在物体をつかんでいれば，その物体を特定の場所に置く．
	*** @return true: Place開始した． false: その場所へは手が届かない，または物体を持ってない．
	*/
	virtual bool Place(Vec3d pos);

	/** @brief Placeが可能かどうかを返す
	*/
	virtual bool IsPlaceable(Vec3d pos);

	/** @brief Placeが可能かどうかを返す
	*** @param safety 安全係数：1以下の係数，距離をsafety倍して計算．1.0のときぎりぎり到達可能
	*/
	virtual bool IsPlaceable(Vec3d pos, float safety);

	/** @brief Placeが完了したかどうかを返す．
	*/
	virtual bool IsPlaceComplete();

	/** @brief 動作を中断する
	*/
	virtual void Abort();

	/** @brief すべての把持動作を中断する
	*/
	virtual void AbortAll();

	/** @brief 現在の動作状態を返す
	*/
	virtual CRGrabControllerIf::CRGCControlState GetControlState();
};
}
//@}

#endif//CRGRABCONTROLLER_H

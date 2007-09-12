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

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 把持コントローラ
*/
class CRGrabController : public CRController, public CRGrabControllerIfInit, public CRGrabControllerDesc {
private:
	/// 下位のコントローラ：ReachingController
	CRReachingControllerIf *reachLeft, *reachRight;

	/// 把持対象のSolid
	PHSolidIf* grabSolid;

	/// 把持対象の大きさ（半径）
	float radius;

	/// 把持した物体を移動する時の目標位置
	Vec3f movetoPos;

	/// 現在の把持状態
	CRGrabControllerIf::GrabState grabState;

	/// 把持完了後に移動を行うかどうか
	bool bMoveto;

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

	/** @brief 掴む
	*/
	virtual void Grab(PHSolidIf* solid, float radius);

	/** @brief 現在何か掴んでいれば，放す
	*/
	virtual void Ungrab();

	/** @brief 現在何か掴んでいれば，それを持って移動する
	*/
	virtual void MoveTo(Vec3f pos);

	/** @brief 現在の把持の状態を返す
	*/
	virtual CRGrabControllerIf::GrabState GetGrabState();
};
}
//@}

#endif//CRGRABCONTROLLER_H

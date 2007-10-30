/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRTRUNKFOOTMAMMALBODY_H
#define CRTRUNKFOOTMAMMALBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRTrunkFootMammalBody
// ヒンジジョイントを用いた哺乳類モデル・クラスの実装
class CRTrunkFootMammalBody : public CRBody, public CRTrunkFootMammalBodyIfInit, public CRTrunkFootMammalBodyDesc {
private:
	void InitBody();
	void CreateWaist();
	void CreateChest();
	void CreateHead();

	void InitLegs();
	void CreateFoot(LREnum lr);

	void InitContact();

public:
	OBJECTDEF(CRTrunkFootMammalBody, CRBody);
	ACCESS_DESC(CRTrunkFootMammalBody);

	CRTrunkFootMammalBody(){}
	CRTrunkFootMammalBody(const CRTrunkFootMammalBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRTrunkFootMammalBodyDesc(desc) 
		, CRBody((const CRBodyDesc&)desc, c)
	{
		solids.resize(CRTrunkFootMammalBodyDesc::SO_NSOLIDS);
		joints.resize(CRTrunkFootMammalBodyDesc::JO_NJOINTS);

		InitBody();
		InitLegs();

		InitContact();
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();
};

}
//@}

#endif//CRTRUNKFOOTMammalBODY_H

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRTRUNKFOOTHUMANBODY_H
#define CRTRUNKFOOTHUMANBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRTrunkFootHumanBody
// ヒンジジョイントを用いた人体モデル・クラスの実装
class CRTrunkFootHumanBody : public CRBody, public CRTrunkFootHumanBodyDesc {
private:
	void InitBody();
	void CreateWaist();
	void CreateChest();
	void CreateHead();

	void InitLegs();
	void CreateFoot(LREnum lr);

	void InitContact();

public:
	SPR_OBJECTDEF(CRTrunkFootHumanBody);
	ACCESS_DESC(CRTrunkFootHumanBody);

	CRTrunkFootHumanBody(){}
	CRTrunkFootHumanBody(const CRTrunkFootHumanBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRTrunkFootHumanBodyDesc(desc) 
		, CRBody((const CRBodyDesc&)desc, c)
	{
		solids.resize(CRTrunkFootHumanBodyDesc::SO_NSOLIDS);
		joints.resize(CRTrunkFootHumanBodyDesc::JO_NJOINTS);

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

#endif//CRTRUNKFOOTHUMANBODY_H

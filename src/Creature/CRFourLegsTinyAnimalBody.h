/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRFOURLEGSTINYANIMALBODY_H
#define CRFOURLEGSTINYANIMALBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsTinyAnimalBody
// ヒンジジョイントを用いた哺乳類モデル・クラスの実装（未実装：中身はTrunkFootHumanBody）
class CRFourLegsTinyAnimalBody : public CRBody, public CRFourLegsTinyAnimalBodyDesc {
private:
	void CreateBody();
	void CreateHead();
	void CreateFrontLegs(LREnum lr);
	void CreateRearLegs(LREnum lr);
	void InitLegs();

	void InitContact();

public:
	SPR_OBJECTDEF(CRFourLegsTinyAnimalBody);
	ACCESS_DESC(CRFourLegsTinyAnimalBody);

	CRFourLegsTinyAnimalBody(){}
	CRFourLegsTinyAnimalBody(const CRFourLegsTinyAnimalBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRFourLegsTinyAnimalBodyDesc(desc) 
		, CRBody((const CRBodyDesc&)desc, c)
	{
		solids.resize(CRFourLegsTinyAnimalBodyDesc::SO_NSOLIDS);
		joints.resize(CRFourLegsTinyAnimalBodyDesc::JO_NJOINTS);
	
		CreateBody();
		CreateHead();
		InitLegs();
		InitContact();

		Init();
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();

};

}
//@}

#endif//CRFourLegsTinyAnimalBODY_H

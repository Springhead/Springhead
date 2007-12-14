/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRFOURLEGSANIMALBODY_H
#define CRFOURLEGSANIMALBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsAnimalBody
// ヒンジジョイントを用いた哺乳類モデル・クラスの実装
class CRFourLegsAnimalBody : public CRBody, public CRFourLegsAnimalBodyIfInit, public CRFourLegsAnimalBodyDesc {
private:
	void InitBody();
	void CreateWaist();
	void CreateChest();
	void CreateTail();

	void InitHead();
	void CreateNeck();
	void CreateHead();

	void InitFrontLegs();
	void CreateBreastBone(LREnum lr);
	void CreateRadius(LREnum lr);
	void CreateFrontCannonBone(LREnum lr);
	void CreateFrontToeBones(LREnum lr);

	void InitRearLegs();
	void CreateFemur(LREnum lr);
	void CreateTibia(LREnum lr);
	void CreateRearCannonBone(LREnum lr);
	void CreateRearToeBones(LREnum lr);
	
	void InitEyes();
	void CreateEye(LREnum lr);

	void InitContact();

public:
	OBJECTDEF(CRFourLegsAnimalBody, CRBody);
	ACCESS_DESC(CRFourLegsAnimalBody);

	CRFourLegsAnimalBody(){}
	CRFourLegsAnimalBody(const CRFourLegsAnimalBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRFourLegsAnimalBodyDesc(desc) 
		, CRBody((const CRBodyDesc&)desc, c)
	{
		solids.resize(CRFourLegsAnimalBodyDesc::SO_NSOLIDS);
		joints.resize(CRFourLegsAnimalBodyDesc::JO_NJOINTS);

		InitBody();
		InitHead();
		InitFrontLegs();
		InitRearLegs();
		InitEyes();
		

		InitContact();
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();
};

}
//@}

#endif//CRFOURLEGSANIMALBODY_H

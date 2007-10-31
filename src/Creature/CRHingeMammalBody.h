/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRHINGEMAMMALBODY_H
#define CRHINGEMAMMALBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRHingeMammalBody
// ヒンジジョイントを用いた哺乳類モデル・クラスの実装
class CRHingeMammalBody : public CRBody, public CRHingeMammalBodyIfInit, public CRHingeMammalBodyDesc {
private:
	void InitBody();
	void CreateWaist();
	void CreateAbdomen();
	void CreateChest();
	void CreateTail();

	void InitHead();
	void CreateNeck();
	void CreateHead();

	void InitFrontLegs();
	void CreateFrontUpperLeg(LREnum lr);
	void CreateFrontLowerLeg(LREnum lr);
	void CreateFrontFoot(LREnum lr);

	void InitRearLegs();
	void CreateRearUpperLeg(LREnum lr);
	void CreateRearLowerLeg(LREnum lr);
	void CreateRearFoot(LREnum lr);
	
	void InitEyes();
	void CreateEye(LREnum lr);

	void InitContact();

public:
	OBJECTDEF(CRHingeMammalBody, CRBody);
	ACCESS_DESC(CRHingeMammalBody);

	CRHingeMammalBody(){}
	CRHingeMammalBody(const CRHingeMammalBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRHingeMammalBodyDesc(desc) 
		, CRBody((const CRBodyDesc&)desc, c)
	{
		solids.resize(CRHingeMammalBodyDesc::SO_NSOLIDS);
		joints.resize(CRHingeMammalBodyDesc::JO_NJOINTS);

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

#endif//CRHINGEMAMMALBODY_H

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
#include <Physics/PHConstraint.h>
#include <Physics/PhysicsDecl.hpp>

#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsTinyAnimalBody
// ヒンジジョイントを用いた哺乳類モデル・クラスの実装（未実装：中身はTrunkFootHumanBody）
class CRFourLegsTinyAnimalBody : public CRBody, public CRFourLegsTinyAnimalBodyDesc {
private:
	PHSolidIf* CreateBody();
	void CreateFrontLegs(LREnum lr, PHSolidIf* sBody);
	void CreateRearLegs( LREnum lr, PHSolidIf* sBody);
	PHSolidIf* InitBody();
	PHSolidIf* InitFrontLeg0(LREnum lr, PHSolidIf* sBody);
	void InitFrontLeg1(LREnum lr, PHSolidIf* sLeg0);
	PHSolidIf* InitRearLeg0(LREnum lr, PHSolidIf* sBody);
	void InitRearLeg1(LREnum lr, PHSolidIf* sLeg0);
	void InitLegs(PHSolidIf* sBody);

	void InitContact();
	void InitControlMode(PHJointDesc::PHControlMode m = PHJointDesc::MODE_POSITION);	//< ボディの制御モードを設定する．


public:
	SPR_OBJECTDEF(CRFourLegsTinyAnimalBody);
	ACCESS_DESC(CRFourLegsTinyAnimalBody);

	enum ShapeMode{
		MODE_ROUNDCONE = 0,
		MODE_BOX, MODE_CAPSULE,
	} shapeMode;

	CRFourLegsTinyAnimalBody(){}
	CRFourLegsTinyAnimalBody(const CRFourLegsTinyAnimalBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRFourLegsTinyAnimalBodyDesc(desc) 
		, CRBody((const CRBodyDesc&)desc, c)
	{
		shapeMode		= MODE_CAPSULE;//MODE_BOX;
		PHSolidIf* b	= InitBody();
		InitLegs(b);
		InitContact();
		InitControlMode();
		Init();
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();
	virtual Vec2d GetSwingLimit(){return rangeFrontSwing;}
	virtual Vec2d GetTwistLimit(){return rangeFrontTwist;}

};

}
//@}

#endif//CRFourLegsTinyAnimalBODY_H

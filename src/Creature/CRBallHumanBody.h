/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRBallHUMANBODY_H
#define CRBallHUMANBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>

#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRBallHumanBody
// ボールジョイントを用いた人体モデル・クラスの実装
class CRBallHumanBody : public CRBody, public CRBallHumanBodyDesc {
private:
	void InitBody();
	void CreateWaist();
	void CreateAbdomen();
	void CreateChest();

	void InitHead();
	void CreateNeck();
	void CreateHead();

	void InitArms();
	void CreateUpperArm(LREnum lr);
	void CreateLowerArm(LREnum lr);
	void CreateHand(LREnum lr);

	void InitEyes();
	void CreateEye(LREnum lr);

	void InitLegs();
	void CreateUpperLeg(LREnum lr);
	void CreateLowerLeg(LREnum lr);
	void CreateFoot(LREnum lr);

	void InitContact();
	void InitSolidPose();

	void SetJointSpringDamper(PHBallJointDesc  &ballDesc,  double springOrig, double damperOrig, double actuatorMass);
	void SetJointSpringDamper(PHHingeJointDesc &hingeDesc, double springOrig, double damperOrig, double actuatorMass);

	void CreateIKNode(int n);
	void CreateIKControlPoint(int n);

public:
	SPR_OBJECTDEF(CRBallHumanBody);
	ACCESS_DESC(CRBallHumanBody);

	CRBallHumanBody(){}
	CRBallHumanBody(const CRBallHumanBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRBallHumanBodyDesc(desc) 
		, CRBody((const CRBodyDesc&)desc, c)
	{
		solids.resize(CRBallHumanBodyDesc::SO_NSOLIDS);
		joints.resize(CRBallHumanBodyDesc::JO_NJOINTS);
		ikNodes.resize(CRBallHumanBodyDesc::JO_NJOINTS);
		ikControlPoints.resize(3*CRBallHumanBodyDesc::SO_NSOLIDS);

		InitBody();
		InitHead();
		InitArms();
		InitEyes();
		InitLegs();

		InitSolidPose();
		InitContact();
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();

	// インタフェースの実装
	virtual int		NBallJoints();				//< ボディに含まれているボールジョイントの数を返す
	virtual int		NHingeJoints();				//< ボディに含まれているヒンジジョイントの数を返す
};

}
//@}

#endif//CRBallHUMANBODY_H

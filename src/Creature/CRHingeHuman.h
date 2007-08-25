/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRHINGEHUMAN_H
#define CRHINGEHUMAN_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <vector>

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRHingeHuman
// ヒンジジョイントを用いた人体モデル・クラスの実装
class CRHingeHuman : public SceneObject, public CRHingeHumanIfInit, public CRHingeHumanDesc {
private:
	enum LREnum{LEFTPART=-1, RIGHTPART=+1};

	std::vector<PHSolidIf*> solids;
	std::vector<PHJointIf*> joints;

	CREyeControllerIf *eyeCtrl;

	PHSceneIf *phScene;
	PHSdkIf   *phSdk;

	PHJointIf* CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHHingeJointDesc desc);

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

public:
	OBJECTDEF(CRHingeHuman, SceneObject);
	ACCESS_DESC(CRHingeHuman);

	CRHingeHuman(){eyeCtrl=NULL;}
	CRHingeHuman(const CRHingeHumanDesc& desc, SceneIf* s=NULL) : CRHingeHumanDesc(desc), solids(SO_NSOLIDS), joints(JO_NJOINTS) {
		eyeCtrl=NULL;
		if(s){
			SetScene(s);Init();
		}
	}

	/** @brief 剛体の数を得る
	*/
	virtual int NSolids();

	/** @brief i番目の剛体を得る
	*/
	virtual PHSolidIf* GetSolid(int i);

	/** @brief 関節の数を得る
	*/
	virtual int NJoints();

	/** @brief i番目の関節を得る
	*/
	virtual PHJointIf* GetJoint(int i);

	/** @brief 人体モデルの初期化（構築含む）を行う
	*/
	void Init();
};

}
//@}

#endif//CRHINGEHUMAN_H

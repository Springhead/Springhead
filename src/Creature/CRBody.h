/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRBODY_H
#define CRBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ
*/
class CRBody : public SceneObject, public CRBodyIfInit, public CRBodyDesc {
protected:
	/// ボディを構成する剛体
	std::vector<PHSolidIf*> solids;

	/// ボディを構成する関節
	std::vector<PHJointIf*> joints;

	/// このセンサの持ち主たるCreature
	CRCreatureIf* creature;

	/// 所属するシーン
	PHSceneIf* phScene;

	/// 所属するSdk
	PHSdkIf* phSdk;

	/** @brief 関節を作る
	*/
	PHJointIf* CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHHingeJointDesc desc);

public:
	OBJECTDEF(CRBody, SceneObject);
	ACCESS_DESC(CRBody);

	CRBody(){}
	CRBody(const CRBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRBodyDesc(desc)
		, solids(CRHingeHumanBodyDesc::SO_NSOLIDS)
		, joints(CRHingeHumanBodyDesc::JO_NJOINTS)  
	{
		if(c!=NULL && c->GetScene()){SetScene(c->GetScene());}
		creature = c;
	}

	/** @brief 初期化を行う
	*/
	virtual void Init();

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
};
}
//@}

#endif//CRBODY_H

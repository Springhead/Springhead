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
	enum LREnum{LEFTPART=-1, RIGHTPART=+1};

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

	/// 位置・姿勢の制御点の集合
	std::vector<CRIKControlIf*> postureControls;

	/// 位置・姿勢の制御対象の集合
	std::vector<CRIKMovableIf*> postureMovables;

	/** @brief 関節を作る
	*/
	PHJointIf* CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHHingeJointDesc desc);
	PHJointIf* CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHBallJointDesc desc);

public:
	OBJECTDEF(CRBody, SceneObject);
	ACCESS_DESC(CRBody);

	CRBody(){}
	CRBody(const CRBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRBodyDesc(desc)
	{
		if(c!=NULL && c->GetScene()){SetScene(c->GetScene());}
		creature = c;
		phScene = DCAST(PHSceneIf, c->GetScene());
		phSdk   = phScene->GetSdk();
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

	/** @brief IK用の制御点を追加する
	*/
	virtual CRIKControlIf* CreateIKControl(const IfInfo* ii, const CRIKControlDesc& desc);
	
	/** @brief IK用の可動物を追加する
	*/
	virtual CRIKMovableIf* CreateIKMovable(const IfInfo* ii, const CRIKMovableDesc& desc);

	/** @brief IKを計算する
	*/
	virtual void CalcIK();
	
};
}
//@}

#endif//CRBODY_H

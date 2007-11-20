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
/** @brief IKの制御点
*/
class CRControlPoint{
public:
	/// 制御点の種別
	CRBodyIf::CRIKCPType cpType;

	/// 制御点のあるSolid
	PHSolidIf* cpSolid;

	/// 制御点の位置(SolidLocal座標系)
	Vec3d cpPos;

	/// 制御点の目標値
	Vec3d cpGoal;

	CRControlPoint(){
		cpType = CRBodyIf::IKCP_ORI;
		cpSolid = NULL;
		cpPos = Vec3d(0,0,0);
		cpGoal = Vec3d(0,0,0);
	}
};
/** @brief IKのための可動物
*/
class CRMovableObject{
public:
	/// 可動なボールジョイント
	PHBallJointIf* movableBallJoint;

	/// 可動な３連ヒンジジョイント
	PHHingeJointIf *movableHinge1, *movableHinge2, *movableHinge3;

	/// 可動な剛体
	PHSolidIf* movableSolid;

	CRMovableObject(){
		movableBallJoint = NULL;
		movableHinge1 = NULL;
		movableHinge2 = NULL;
		movableHinge3 = NULL;
		movableSolid = NULL;
	}
};
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

	/// ボディ全体のヤコビアンを管理する行列
	PTM::VMatrixCol<Matrix3d> jacobian;

	/// 可動性を管理する行列
	PTM::VMatrixCol<bool>     movability;

	/// 行番号と制御点の対応
	std::vector<CRControlPoint>  cpNums;

	/// 列番号と可動物の対応
	std::vector<CRMovableObject> moNums;

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
	virtual int AddIKControlPoint(CRBodyIf::CRIKCPType type, PHSolidIf* solid, Vec3d pos);
	
	/** @brief IK用の可動物を追加する
	*/
	virtual int AddIKMovableJoint(int cpnum, PHBallJointIf* ballJoint);
	virtual int AddIKMovableJoint(int cpnum, PHHingeJointIf* jo1, PHHingeJointIf* jo2, PHHingeJointIf* jo3);
	virtual int AddIKMovableSolid(int cpnum, PHSolidIf* solid);
};
}
//@}

#endif//CRBODY_H

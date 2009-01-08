/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHPATHJOINT_H
#define PHPATHJOINT_H

#include <SprPhysics.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

/*	2つの剛体間の相対位置・姿勢を返す関数
		[p; q] = f(s)
	があるとする．ここでpは相対位置, qは相対quaternion，sは一般化座標を表わすスカラー．
	微分して
		[p'; q'] = (df/ds)(s)s'．
	quaternionの時間微分から角速度を得る行列をE(q)とすると
		[v; w] = [I O; O E(q(s))](df/ds)(s)s' =: J(s)s'
	上の式よりsとs'が決まれば相対速度[v; w]が決まる．
	これを拘束条件の形に変える．6次元ベクトルJ(s)は相対速度の向きを表わしているので，
	これに直交するベクトルを行ベクトルに持つ行列をJ~(s)とすると，拘束条件は
		J~(s)[v; w] = 0

	Correctionは，
	velocity update後の相対位置に最も近い軌道im(f)上の点を取得，その点に合わせる．
	本当の拘束条件は[p; q] in im(f)だが非線形なので扱えない．
 */

/// 関節の軌道のキーフレームを持ち，補完して返したりヤコビアンを計算したりするクラス
struct PHPathPointWithJacobian : public PHPathPoint{
	Matrix6d	J;
};

class PHPath : public SceneObject, public std::vector<PHPathPointWithJacobian>{
	bool bReady;
	bool bLoop;	//[-pi, pi]の無限回転関節
	iterator Find(double &s);
public:
	SPR_OBJECTDEF_ABST(PHPath);

	virtual bool GetDesc(void* desc)const;
	virtual void SetDesc(const void* desc);

	virtual void SetLoop(bool OnOff = true){bLoop = OnOff;}
	virtual bool IsLoop(){return bLoop;}
	void AddPoint(double s, const Posed& pose);
	void CompJacobian();
	void GetPose(double s, Posed& pose);
	void GetJacobian(double s, Matrix6d& J);
	void Rollover(double& s);
	PHPath(const PHPathDesc& desc=PHPathDesc());
};

class PHPathJoint;
class PHPathJointNode : public PHTreeNode1D{
public:
	SPR_OBJECTDEF(PHPathJointNode);
	SPR_DECLMEMBEROF_PHPathJointNodeDesc;
	PHPathJoint* GetJoint(){return PHTreeNode1D::GetJoint()->Cast();}
	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void UpdateJointPosition(double dt);
	PHPathJointNode(const PHPathJointNodeDesc& desc = PHPathJointNodeDesc()){}
};

class PHPathJoint : public PHJoint1D{
public:
	SPR_OBJECTDEF(PHPathJoint);
	SPR_DECLMEMBEROF_PHPathJointDesc;

	UTRef<PHPath> path;
	//virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::PATHJOINT;}
	virtual PHTreeNode* CreateTreeNode(){
		return DBG_NEW PHPathJointNode();
	}
	virtual void SetPosition(double pos){position[0] = pos;}
	virtual bool AddChildObject(ObjectIf* o);
	virtual void ModifyJacobian();
	virtual void CompBias();
	virtual void UpdateJointState();
	//virtual void CompError(double dt);
	//virtual void ProjectionCorrection(double& F, int k);
	PHPathJoint(const PHPathJointDesc& desc = PHPathJointDesc());
};

}

#endif

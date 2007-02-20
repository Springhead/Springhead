/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHTREENODE_H
#define PHTREENODE_H

#include <SprPhysics.h>
#include <Physics/PHSpatial.h>

namespace Spr{;

///	ツリーノードの基本クラス
class PHRootNode;
class PHTreeNode : public SceneObject, public PHTreeNodeIfInit, public UTTreeNode<PHTreeNode>{
public:
	OBJECTDEF_ABST(PHTreeNode, SceneObject);
	
	bool		Includes(PHTreeNode* node);		///< 自分以下にノードnodeがあるか
	PHTreeNode*	FindBySolid(PHSolid* solid);	///< 自分以下のツリーでsolidを参照しているノードを探す
	PHTreeNode* FindByJoint(PHJoint* joint);	///< 自分以下のツリーでjointを参照しているノードを探す
	void		RemoveGearNode();
	//int GetNumOfNodes();
	//int GetTotalDof();							///< 子ノードを含めた自由度の合計
	//int AssignID(int id, std::vector<PHTreeNode*>& table);		///< 再帰的にIDを割り振る

	PHTreeNode();

	virtual PHSolid*	GetSolid(){return joint->solid[1];}
	virtual PHRootNode*	GetRootNode(){return GetParent()->GetRootNode();}
	
	virtual void CompAccel(){}
	virtual void CompAccelDiff(bool bUpdate){}
	/// このノードに加わる拘束力の変化量から他のノードの速度変化量への影響を計算．LCPで使用
	virtual void CompResponse(const SpatialVector& dF, bool bUpdate = true);
	virtual void CompBiasForceDiff(bool bUpdate){}
	virtual void SetupLCP();
	virtual void IterateLCP();
	virtual void InitArticulatedInertia();
			void InitArticulatedBiasForce();
	virtual void CompArticulatedInertia();			///< Articulated Inertiaの計算
	virtual void CompArticulatedBiasForce();		///< Articulated Bias Forceの計算
	virtual	void CompSpatialTransform();			///< ノード間の座標変換の計算
	virtual void CompCoriolisAccel();
	virtual void CompJointJacobian(){}				///< 関節ヤコビアンを計算
	virtual void CompJointCoriolisAccel(){}			///< コリオリの加速度を計算	
	virtual void AccumulateInertia(){}				///< Iaを親ノードのIaに積み上げる
	virtual void AccumulateBiasForce(){}			///< Zaを親ノードのZaに積み上げる
	virtual void UpdateVelocity(double dt);			///< 剛体の速度の更新
	virtual void UpdatePosition(double dt);			///< 剛体の位置の更新
	virtual void UpdateJointVelocity(double dt){}	///< 関節速度の更新
	virtual void UpdateJointPosition(double dt){}	///< 関節位置の更新
	virtual void CompRelativeVelocity(){}			///< 関節速度から剛体間相対速度を計算
	virtual void CompRelativePosition(){}			///< 関節位置から剛体間相対位置を計算
	
	//int			id;						///< ID．ルートノードが0でdepth-firstで割り当てられる．
	PHJoint*	joint;						///< 親ノードとこのノードとをつなぐ拘束
	//bool		opposite;					///< ツリーノードの親子関係と拘束の親子関係（ソケットがついてるのが親）が逆ならtrue

	SpatialMatrix			I;				///< Articulated Inertia
	SpatialVector			Z;				///< Articulated Bias Force
	SpatialVector			dZ;				///< 拘束力の変化によるZの変化量
	SpatialVector			c;				///< コリオリ加速度
	SpatialVector			cj;				///< 関節速度によるコリオリ加速度

	SpatialTransform		Xcp, Xcj;
	SpatialMatrix			XIX, XtrIJ_JIJinv_Jtr, XtrIJ_JIJinv_JtrIX;
	SpatialVector			a;				///< 加速度
	SpatialVector			da;				///< 拘束力の変化によるaの変化量
	SpatialVector			ap, Ic, ZplusIc, XtrZplusIc;

	PHScene*	scene;
	PHConstraintEngine* engine;
};

class PHRootNode : public PHTreeNode, public PHRootNodeIfInit{
public:
	OBJECTDEF(PHRootNode, PHTreeNode);

	void SetupABA();
	
	virtual PHSolid* GetSolid(){return solid;}
	virtual PHRootNode* GetRootNode(){return this;}
	virtual int	 GetDof(){ return 6; }
	virtual void CompArticulatedInertia();
	virtual void CompArticulatedBiasForce();
	virtual void CompAccel();
	virtual void CompBiasForceDiff(bool Update);
	virtual void UpdateVelocity(double dt);
	virtual void UpdatePosition(double dt);
	
	PHSolid*	solid;		///< ルートノードの剛体

	PHRootNode(const PHRootNodeDesc& desc = PHRootNodeDesc()){
		solid = NULL;
	}
protected:
	SpatialMatrix		Iinv;
};

///	N自由度の関節の基本クラス
template<int NDOF>
class PHTreeNodeND : public PHTreeNode{
public:
	PHJointND<NDOF>* GetJoint(){return (PHJointND<NDOF>*)DCAST(PHJoint , joint);}

	/// ABA関係の関数
	virtual void AccumulateInertia();
	virtual void AccumulateBiasForce();
	virtual void CompJointJacobian();
	virtual void CompAccel();
	virtual void CompAccelDiff(bool bUpdate);
	virtual void CompBiasForceDiff(bool bUpdate);
	virtual void UpdateJointVelocity(double dt);
	virtual void UpdateJointPosition(double dt);

	/// LCP関係の関数
			void CompResponse(const PTM::TVector<NDOF, double>& tau, bool bUpdate = true);
			void CompResponseMatrix();
	virtual void ModifyJacobian();
	virtual void SetupLCP();
	virtual void IterateLCP();

	virtual void CompBias()=0;
	virtual void Projection(double& f, int k)=0;

	PHTreeNodeND();
protected:
	bool			constr[NDOF];			///< ABAとして各自由度を拘束するか
	SpatialVector	J[NDOF];				///< 関節座標から相対速度へのヤコビアン
	SpatialVector	IJ[NDOF], XtrIJ[NDOF], J_JIJinv[NDOF], IJ_JIJinv[NDOF], XtrIJ_JIJinv[NDOF];
	PTM::TMatrixCol<NDOF, NDOF, double> JIJ, JIJinv;
	PTM::TVector<NDOF, double>	JtrZplusIc;
	PTM::TVector<NDOF, double>	accel, daccel, dtau;

	PTM::TVector<NDOF, double> A, Ainv, dA, b, db, f;	///< 関節座標LCPのための変数
	PTM::TMatrixCol<NDOF, NDOF, double> Jq;	///< 関節座標LCPにおける関節速度から拘束変数へのヤコビアン
};

///	1自由度の関節
class PHTreeNode1D;
class PHTreeNode1D : public PHTreeNodeND<1>{
public:
	OBJECTDEF_ABST_NOIF(PHTreeNode1D, PHTreeNode);
	PHGear*			gear;				///< ギアへの参照．このギアを介してgearedParentノードと連動する
	/// ギアノードへの参照．NULL->連動なし, this->ギアトレイン最上段
	PHTreeNode1D*	gearNode;
	PHTreeNode1D*	parent1D;
	SpatialTransform	Xcg;			///< ギアトレインの親ノードとの間の座標変換
	SpatialMatrix	sumXIX;
	SpatialVector	sumXtrIJ, sumXtrIJ_sumJIJinv;
	SpatialVector	sumXtrZplusIc, sumXtrdZ;
	double			sumJIJ, sumJIJinv;
	double			sumJtrZplusIc, sumJtrdZ;
	double			sumtorque;
	
	void			AddGear(PHGear* gear, PHTreeNode1D* child);
	PHJoint1D*		GetJoint(){return DCAST(PHJoint1D, joint);}
	virtual void	CompSpatialTransform();
	virtual void	CompJointJacobian();
	virtual void	CompCoriolisAccel();
	virtual void	InitArticulatedInertia();
	virtual void	AccumulateInertia();
	virtual void	AccumulateBiasForce();
	virtual void	CompBiasForceDiff(bool bUpdate);
	virtual void	CompAccel();
	virtual void	CompAccelDiff(bool bUpdate);
	virtual void	Projection(double& f, int k);
	virtual void	UpdateJointVelocity(double dt);
	virtual void	UpdateJointPosition(double dt);
	
	PHTreeNode1D();
};

}

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHTREENODE_H
#define PHTREENODE_H

#include <SprPhysics.h>
#include <Physics/PHJoint.h>
#include <Physics/PhysicsDecl.hpp>

namespace Spr{;

///	ツリーノードの基本クラス
class PHRootNode;
class PHTreeNode : public SceneObject, public UTTreeNode<PHTreeNode>, public PHTreeNodeDesc{
public:
	SPR_OBJECTDEF_ABST(PHTreeNode);
	
	virtual bool AddChildObject(ObjectIf* o);
	virtual size_t NChildObject();
	virtual ObjectIf* GetChildObject(size_t i);

	void Enable(bool bEnable = true);
	bool IsEnabled(){ return bEnabled; }
	int  NChildren(){ return (int)Children().size(); }
	PHTreeNodeIf* GetChild(int i){ return Children()[i]->Cast(); }
	
	bool		Includes(PHTreeNode* node);		///< 自分以下にノードnodeがあるか
	PHTreeNode*	FindBySolid(PHSolid* solid);	///< 自分以下のツリーでsolidを参照しているノードを探す
	PHTreeNode* FindByJoint(PHJoint* joint);	///< 自分以下のツリーでjointを参照しているノードを探す
	void		RemoveGearNode();
	void		Prepare(PHScene* scene, PHConstraintEngine* engine);	///< 再帰的にsceneとengineをセットする

	PHTreeNode();

	virtual PHSolid*	GetSolid(){return joint->solid[1];}
	virtual PHRootNode*	GetRootNode(){return GetParent()->GetRootNode();}
	virtual void ResetGearNode();
	virtual void CompAccel()=0;
	virtual void CompAccelDiff(bool bUpdate, bool bImpulse)=0;
	/// このノードに加わる拘束力の変化量から他のノードの速度変化量への影響を計算．LCPで使用
	virtual void CompResponse(const SpatialVector& df, bool bUpdate, bool bImpulse);
	virtual void CompBiasForceDiff(bool bUpdate, bool bImpulse)=0;
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
			void ResetCorrectionVarsRecursive();
	virtual void ResetCorrectionVars();				///< Correctionの変数の初期化
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
	SpatialVector			a, da;			///< 加速度とその変化量
	//SpatialVector			v, dv;			///< 速度とその変化量
	SpatialVector			ap, Ic, ZplusIc, XtrZplusIc;

	PHScene*	scene;
	PHConstraintEngine* engine;
};

class PHRootNode : public PHTreeNode{
public:
	SPR_OBJECTDEF(PHRootNode);
	SPR_DECLMEMBEROF_PHRootNodeDesc;

	virtual bool AddChildObject(ObjectIf* o);
	virtual size_t NChildObject();
	virtual ObjectIf* GetChildObject(size_t i);
	
	void SetupABA();
	void SetupCorrectionABA();
	
	virtual PHSolid* GetSolid(){return solid;}
	virtual PHRootNode* GetRootNode(){return this;}
	virtual int	 GetDof(){ return 6; }
	virtual void CompArticulatedInertia();
	virtual void CompArticulatedBiasForce();
	virtual void CompAccel();
	virtual void CompAccelDiff(bool bUpdate, bool bImpulse){}
	virtual void CompBiasForceDiff(bool bUpdate, bool bImpulse);
	virtual void UpdateVelocity(double dt);
	virtual void UpdatePosition(double dt);
	
	PHSolid*	solid;		///< ルートノードの剛体

	PHRootNode(const PHRootNodeDesc& desc = PHRootNodeDesc()){
		solid = NULL;
	}
protected:
	SpatialMatrix		Iinv;
};

class PHGear;
///	N自由度の関節の基本クラス
template<int NDOF>
class PHTreeNodeND : public PHTreeNode{
public:
	typedef	PTM::TVector<NDOF, double>	VecNd;
	typedef PTM::TMatrixCol<6, NDOF, double> Matrix6Nd;
	typedef PTM::TMatrixCol<NDOF, NDOF, double> MatrixNd;
	
	PHJointND<NDOF>* GetJoint(){return (PHJointND<NDOF>*)DCAST(PHJoint , joint);}

	/// ABA関係の関数
	virtual void ResetGearNode();
	virtual void CompCoriolisAccel();
	virtual void CompSpatialTransform();
	virtual void InitArticulatedInertia();
	virtual void AccumulateInertia();
	virtual void AccumulateBiasForce();
	virtual void CompJointJacobian();
	virtual void CompAccel();
	virtual void ResetCorrectionVars();
	virtual void CompAccelDiff(bool bUpdate, bool bImpulse);
	virtual void CompBiasForceDiff(bool bUpdate, bool bImpulse);
	virtual void UpdateJointVelocity(double dt);
	virtual void UpdateJointPosition(double dt);

	/// LCP関係の関数
	virtual void CompResponse(const VecNd& tau, bool bUpdate);
			void CompResponseMatrix();
	virtual void ModifyJacobian();
	virtual void SetupLCP();
	virtual void IterateLCP();
	virtual void CompBias()=0;
	virtual void Projection(double& f, int k)=0;

	void AddGear(PHGear* gear, PHTreeNodeND<NDOF>* child);
	PHGear*			gear;				///< ギアへの参照．このギアを介して親ノードと連動する
	PHTreeNodeND<NDOF>*	gearNode;			///< ギアノードへの参照．NULL:連動なし, this:ギアトレイン最上段
	std::vector<PHTreeNodeND<NDOF>*> gearChildren;	///< 自身がgearNodeの場合，連動しているノードの配列
	PHTreeNodeND<NDOF>*	parentND;
	
	PHTreeNodeND();
//protected:
	bool			constr[NDOF];			///< ABAとして各自由度を拘束するか
	Matrix6Nd		J;						///< 関節座標から相対速度へのヤコビアン
	Matrix6Nd		IJ, XtrIJ, J_JIJinv, IJ_JIJinv, XtrIJ_JIJinv;
	MatrixNd		JIJ, JIJinv;
	VecNd	JtrZplusIc;
	VecNd	accel, daccel, dtau, vel;	///< 関節加速度（と変化量），関節トルクの変化量，関節速度

	SpatialTransform	Xcg;			///< ギアトレインの親ノードとの間の座標変換
	SpatialMatrix	sumXIX;
	SpatialVector	sumXtrZplusIc, sumXtrdZ;
	Matrix6Nd		sumXtrIJ, sumXtrIJ_sumJIJinv;
	MatrixNd		sumJIJ, sumJIJinv;
	VecNd			sumJtrZplusIc, sumJtrdZ, sumtorque, JtrdZ;
	

	VecNd A, Ainv, dA, b, db, f;	///< 関節座標LCPのための変数
	MatrixNd Jq;					///< 関節座標LCPにおける関節速度から拘束変数へのヤコビアン
};

///	1自由度の関節
class PHTreeNode1D : public PHTreeNodeND<1>{
public:
	SPR_OBJECTDEF_ABST1(PHTreeNode1D, PHTreeNode);
	SPR_DECLMEMBEROF_PHTreeNode1DDesc;
	
	PHJoint1D*		GetJoint(){return DCAST(PHJoint1D, joint);}
	virtual void	CompBias();
	virtual void	Projection(double& f, int k);
	
	PHTreeNode1D();
};

}

#endif

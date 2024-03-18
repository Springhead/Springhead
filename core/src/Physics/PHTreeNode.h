/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHTREENODE_H
#define PHTREENODE_H

#include <Physics/SprPHJoint.h>
#include <Physics/PHJoint.h>
#include <Physics/PHGear.h>
#include <Physics/PhysicsDecl.hpp>

namespace Spr{;

class PHGear;

///	ツリーノードの基本クラス
class PHRootNode;
class PHTreeNode : public SceneObject, public UTTreeNode<PHTreeNode>, public PHTreeNodeDesc{
public:
	SPR_OBJECTDEF_ABST(PHTreeNode);

	SpatialMatrix                I;				///< Articulated Inertia
	SpatialVector                Z;				///< Articulated Bias Force
	SpatialVector                c;				///< コリオリ加速度
	SpatialVector                cj;			///< 関節速度によるコリオリ加速度

	SpatialTransform             Xcp, Xcj;
	SpatialMatrix                Xcp_mat, Xcj_mat;
	SpatialMatrix                XIX, XtrIJ_JIJinv_Jtr, XtrIJ_JIJinv_JtrIX, J_JIJinv_Jtr;
	SpatialVector                Ic, ZplusIc, XtrZplusIc;

	SpatialMatrix                Tcp;			///< 親ノードのdvからこのノードのdvを与える行列．転置は子ノードのdZから親ノードのdZを与える
	std::vector<SpatialMatrix>   dZdv_map;		///< 他のノードのdZからノードのdvを与える行列
	
	int                          id;			///< ノードID
	int                          depth;         ///< ギアトレインを考慮したルートからの深さ
	PHJoint*                     joint;			///< 親ノードとこのノードとをつなぐ関節
	PHSolid*                     solid;			///< このノードに関連づけられている剛体
	PHRootNode*                  root;


public:
	PHTreeNode();
	
	/// Objectの仮想関数
	virtual bool      AddChildObject(ObjectIf* o);
	virtual size_t    NChildObject  () const;
	virtual ObjectIf* GetChildObject(size_t i);

	/// インタフェースの実装
	void            Enable       (bool bEnable = true);
	bool            IsEnabled    ()     { return bEnabled; }
	int             NChildren    ()     { return (int)Children().size(); }
	PHTreeNodeIf*   GetParentNode()     { return parent->Cast(); }
	PHTreeNodeIf*   GetChildNode (int i){ return Children()[i]->Cast(); }
	SpatialMatrix   GetI         ()     { return I; }
	SpatialVector   GetIc        ()     { return Ic; }
	SpatialVector   GetC         ()     { return c; }
	SpatialVector   GetZ         ()     { return Z; }
	SpatialMatrix   GetXcp_mat	 ()     { return Xcp_mat; }
	PHSolidIf*      GetSolid     ()     { return solid->Cast(); }
	PHRootNodeIf*	GetRootNode  ();
		
	bool		    Includes         (PHTreeNode* node);		///< 自分以下にノードnodeがあるか
	PHTreeNode*	    FindBySolid      (PHSolid* solid);			///< 自分以下のツリーでsolidを参照しているノードを探す
	PHTreeNode*     FindByJoint      (PHJoint* joint);			///< 自分以下のツリーでjointを参照しているノードを探す
	void		    AssignID         (std::vector<PHTreeNode*>& n);
	PHScene*	    GetPHScene       ();
	
	virtual void        ResetGear           (){};
	virtual void        AddToGearTrain      (PHGear* gear, PHTreeNode* upper, bool isParent){}
	virtual PHTreeNode* GetParentOfGearTrain(){ return parent; }	///< ギアトレインを考慮した親ノード
	virtual PHTreeNode* GetGearRoot         (){ return 0; }			///< ギアトレインの最上段ノード

	virtual void   Prepare                 ();
	        void   PrepareGear             (PHGears& gears);
			void   PrepareDepth            ();
	virtual void   CompAccel               (){}
	virtual void   InitArticulatedInertia  ();
			void   InitArticulatedBiasForce();
	virtual void   CompArticulatedInertia  ();			///< Articulated Inertiaの計算
	virtual void   CompArticulatedBiasForce();			///< Articulated Bias Forceの計算
	virtual	void   CompSpatialTransform    ();			///< ノード間の座標変換の計算
	virtual void   CompCoriolisAccel       ();			///< コリオリ加速度の計算
	virtual void   AccumulateInertia       (){}			///< Iを親ノードのIに積み上げる
	virtual void   AccumulateBiasForce     (){}			///< Zを親ノードのZに積み上げる
	        void   CompResponse            (const SpatialVector& df);
	virtual void   ClearCorrection()	   {}			///< 位置のCorrectionのためのvelを誰もクリアしていないので追加
			void   CompResponseCorrection  (const SpatialVector& dF);
	        double GetResponse             (PHTreeNode* src, const SpatialVector& J, const SpatialVector& df);
	virtual void   CompResponse            (PHTreeNode* src, const SpatialVector& df);
	virtual void   CompResponseCorrection  (PHTreeNode* src, const SpatialVector& dF);
	virtual void   CompResponseMatrix      ();
	virtual void   CompResponseMap         (){}
	virtual void   UpdateVelocity          (double* dt);	///< 剛体の速度の更新
	virtual void   UpdateVelocitySolid          (double* dt);	///< 剛体の速度の更新
	virtual void   UpdatePosition          (double dt);		///< 剛体の位置の更新
	virtual void   UpdateJointVelocity     (){}	            ///< 関節速度の更新
	virtual void   UpdateJointPosition     (double dt){}	///< 関節位置の更新

	/// 個々の関節種類に対応する派生クラスが実装する仮想関数
	virtual void CompJointJacobian       (){}				///< 関節ヤコビアンを計算
	virtual void CompJointCoriolisAccel  (){}			///< コリオリの加速度を計算	
	virtual void CompRelativeVelocity    (){}			///< 関節速度から剛体間相対速度を計算
	virtual void CompRelativePosition    (){}			///< 関節位置から剛体間相対位置を計算

};

class PHRootNode : public PHTreeNode{
public:
	SPR_OBJECTDEF(PHRootNode);
	SPR_DECLMEMBEROF_PHRootNodeDesc;
	SPR_DECLMEMBEROF_PHRootNodeState;

	int									treeId;
	bool								bReady;
	PHConstraintEngine*					engine;
	std::vector<PHTreeNode*>			nodes;				///< IDによるアクセス用ノード配列
	SpatialMatrix						Iinv;				///< Iの逆行列

public:
	void Setup();
	void SetupCorrection();
	bool IsUseNextPose() { return useNextPose; }
	void SetUseNextPose(bool bOn) { useNextPose = bOn; }
	void SetNextPose(const Posed& p) { nextPose = p; } ///< 剛体の姿勢を上書き

	/// Objectの仮想関数
	virtual bool      AddChildObject(ObjectIf* o);
	virtual size_t    NChildObject  () const;
	virtual ObjectIf* GetChildObject(size_t i);
	
	/// TreeNodeの仮想関数
	virtual void CompArticulatedInertia  ();
	virtual void CompArticulatedBiasForce();
	virtual void CompAccel               ();
	virtual void CompResponseMap         ();
	virtual void UpdateVelocity          (double* dt);
	virtual void UpdateVelocitySolid     (double* dt);	///< 剛体の速度の更新
	virtual void UpdatePosition          (double dt);
	
	PHRootNode(const PHRootNodeDesc& desc = PHRootNodeDesc());

};

///	N自由度の関節の基本クラス
template<int NDOF>
class PHTreeNodeND : public PHTreeNode{
public:
	typedef	PTM::TVector<NDOF, double>	        VecNd;
	typedef PTM::TMatrixCol<6, NDOF, double>    Matrix6Nd;
	typedef PTM::TMatrixCol<NDOF, 6, double>    MatrixN6d;
	typedef PTM::TMatrixCol<NDOF, NDOF, double> MatrixNd;

	Matrix6Nd		J;						///< 関節座標から相対速度へのヤコビアン
	Matrix6Nd		IJ, XtrIJ, J_JIJinv, IJ_JIJinv, XtrIJ_JIJinv;
	MatrixNd		JIJ, JIJinv;
	VecNd	        JtrZplusIc;
	VecNd	        dvel, vel;		///< 関節加速度と変化量，関節速度

	PHGear*             gear;
	PHTreeNodeND<NDOF>* gearRoot;		///< ギアトレインの最上段
	PHTreeNodeND<NDOF>* gearParent;		///< 親ノードと連動している場合，parentに一致
	PHTreeNodeND<NDOF>* gearUpper;		///< ギアトレインの一つ上段にあるノード．兄弟ノードか親ノード
	double              gearRatio;		///< ギアの向きを考慮したギア比
	double				gearOffset;		///< ギアの向きを考慮したギア位相差
	double              gearRatioAbs;	///< ギアトレインの累積ギア比

	SpatialTransform	Xcg;
	SpatialMatrix		Xcg_mat;

	SpatialVector	sumXtrZplusIc;
	VecNd			sumJtrZplusIc;
	SpatialMatrix	sumXIX;
	MatrixNd		sumJIJ, sumJIJinv;
	Matrix6Nd		sumXtrIJ, sumXtrIJ_sumJIJinv;

	std::vector<MatrixN6d>	dZdvel_map;		///< ツリー上の任意の他ノードのdZからこのノードのdvelを与える行列
	PHNDJoint<NDOF>* GetJoint(){ return (PHNDJoint<NDOF>*)joint; }

	virtual void        ResetGear           ();
	virtual void        AddToGearTrain      (PHGear* gear, PHTreeNode* upper, bool isParent);
	virtual PHTreeNode* GetParentOfGearTrain(){ return gearRoot ? gearRoot->parent : parent; }
	virtual PHTreeNode* GetGearRoot         (){ return gearRoot; }

	/// ABA関係の関数
	virtual void Prepare               ();
	virtual void InitArticulatedInertia();
	virtual void CompCoriolisAccel     ();
	virtual void CompSpatialTransform  ();
	virtual void AccumulateInertia     ();
	virtual void AccumulateBiasForce   ();
	virtual void CompJointJacobian     ();
	virtual void CompAccel             ();
	virtual void CompResponse          (PHTreeNode* src, const SpatialVector& df);
	virtual void CompResponseCorrection(PHTreeNode* src, const SpatialVector& dF);
	virtual void CompResponseMatrix    ();
	virtual void CompResponseMap       ();
	virtual void UpdateJointVelocity   ();
	virtual void UpdateJointPosition   (double dt);
	virtual void ClearCorrection() { vel.clear(); }

	PHTreeNodeND();
};

///	1自由度の関節
class PHTreeNode1D : public PHTreeNodeND<1>{
public:
	SPR_OBJECTDEF_ABST1(PHTreeNode1D, PHTreeNode);
	SPR_DECLMEMBEROF_PHTreeNode1DDesc;
	
public:
	PH1DJoint*		GetJoint(){return DCAST(PH1DJoint, joint);}

	virtual void   UpdateJointPosition(double dt);

	PHTreeNode1D();
};

}

#endif

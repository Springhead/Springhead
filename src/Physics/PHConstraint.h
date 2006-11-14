/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONSTRAINT_H
#define PHCONSTRAINT_H

#include <SprPhysics.h>
#include <Physics/PHContactDetector.h>
#include <Physics/PHSpatial.h>

namespace Spr{;

class PHTreeNode;

/// PHConstraint派生クラスのためのルータ
template<class inf, class base>
class InheritConstraint : public InheritSceneObject<inf, base>{
public:
	void Enable(bool bEnable = true){base::Enable(bEnable);}
	bool IsEnabled(){return base::IsEnabled();}
};

///
class PHConstraint : public InheritSceneObject<PHConstraintIf, SceneObject>{
public:
	OBJECT_DEF_ABST(PHConstraint);
	enum PHControlMode{
		MODE_TORQUE,
		MODE_POSITION,
		MODE_VELOCITY
	} mode;
	
	bool		bEnabled;			///< 有効化されている場合にtrue
	bool		bFeasible;			///< 両方の剛体がundynamicalな場合true
	bool		bInactive[2];		///< 剛体が解析法に従う場合true
	bool		bArticulated;		///< 関節系を構成している場合true
	
	PHSolid*			solid[2];	///< 拘束する剛体
	SpatialTransform	Xj[2];		///< 剛体に対するソケット，プラグの位置と向き
	SpatialTransform	Xjrel;		///< ソケットに対するプラグの位置と向き
	Quaterniond	qjrel; 
	SpatialVector		vjrel;		///< ソケットに対するプラグの相対速度
	
	SpatialTransform		Js[2];		///< 拘束ヤコビアン SpatialTranform形式
	SpatialMatrix			J[2];		///< 拘束ヤコビアン 行列形式
	SpatialMatrix			AinvJ[2];
	SpatialMatrix			T[2];
	
	bool		constr[6];			///< 拘束する自由度．trueならば拘束する．

	SpatialVector f;				///< 拘束力の力積
	//Vec3d		Fv, Fq;				/// correctionにおける関節力

	SpatialVector b, db;			///< LCPのbベクトルとその補正量
	SpatialVector A, dA, Ainv;		///< LCPのA行列の対角成分とその補正量，逆数
	
	void	Init();
	void	SetupDynamics();
	void	IterateDynamics();
	void	SetScene(SceneIf* s){scene = DCAST(PHScene, s);}
	void	SetEngine(PHConstraintEngine* e){engine = e;}
	void	UpdateState();
	void	CompJacobian();
	void	CompResponseMatrix();
	//void SetupCorrection(double dt, double max_error);
	//void IterateCorrection();
	
	/// インタフェースの実装
	virtual bool AddChildObject(ObjectIf* o);
	virtual void Enable(bool bEnable = true){bEnabled = bEnable;}
	virtual bool IsEnabled(){return bEnabled;}
	virtual void SetInactive(int index = 0, bool Inaction = true){bInactive[index] = Inaction;}
	virtual bool IsInactive(int index = 0){return bInactive[index];}

	/// 派生クラスの機能
	virtual void SetDesc(const PHConstraintDesc& desc);
	virtual void AddMotorTorque(){}
	virtual void CompBias(){}
	virtual void Projection(double& f, int k){}
	virtual void UpdateJointState(){}			///< 関節座標の位置・速度を更新する
	//virtual void CompError(double dt)=0;			/// Bv, Bqを設定する
	//virtual void ProjectionCorrection(double& F, int k){}
	PHConstraint();
protected:
	PHScene* scene;
	PHConstraintEngine* engine;
};
class PHConstraints : public std::vector< UTRef<PHConstraint> >{
public:
	/// 指定された剛体の組に作用している拘束を返す
	PHConstraint* FindBySolidPair(PHSolid* lhs, PHSolid* rhs){
		for(iterator it = begin(); it != end(); it++)
			if((*it)->solid[0] == lhs && (*it)->solid[1] == rhs)
				return *it;
		return NULL;
	}
	void SetupDynamics(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->SetupDynamics();
	}
	void IterateDynamics(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->IterateDynamics();
	}
	/*void SetupCorrection(double dt, double max_error){
		for(iterator it = begin(); it != end(); it++)
			(*it)->SetupCorrection(dt, max_error);
	}*/
	/*void IterateCorrection(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->IterateCorrection();
	}*/
	void UpdateState(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->UpdateState();
	}
};

}

#endif

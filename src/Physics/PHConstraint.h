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

class PHConstraint : public SceneObject, public PHConstraintIfInit{
public:
	OBJECTDEF_ABST(PHConstraint, SceneObject);
	enum PHControlMode{
		MODE_TORQUE,
		MODE_POSITION,
		MODE_VELOCITY
	} mode;

	PHScene*			scene;
	PHConstraintEngine* engine;

	/// 剛体から見た関節の位置と傾き
	Posed poseSocket;
	Posed posePlug;

	bool		bEnabled;			///< 有効化されている場合にtrue
	bool		bFeasible;			///< 両方の剛体がundynamicalな場合true
	bool		bArticulated;		///< 関節系を構成している場合true
	bool		bInactive[2];		///< 剛体が解析法に従う場合true	
	PHSolid*			solid[2];	///< 拘束する剛体
	SpatialTransform	Xj[2];		///< 剛体に対するソケット，プラグの位置と向き
	SpatialTransform	Xjrel;		///< ソケットに対するプラグの位置と向き
	SpatialVector		vjrel;		///< ソケットに対するプラグの相対速度
	
	SpatialTransform	Js[2];		///< 拘束ヤコビアン SpatialTranform形式
	SpatialMatrix		J[2];		///< 拘束ヤコビアン 行列形式
	SpatialMatrix		AinvJ[2];
	SpatialMatrix		T[2];
	
	SpatialVector f;				///< 拘束力の力積
	SpatialVector F;				///< 拘束誤差を位置のLCPで補正する場合の補正量*質量
	
	SpatialVector b, db, B;			///< LCPのbベクトルとその補正量
	SpatialVector A, dA, Ainv;		///< LCPのA行列の対角成分とその補正量，逆数
	SpatialVector scale;

	bool		constr[6];			///< 速度を拘束する自由度. 可動範囲，バネ・ダンパが有効な場合はtrueとなる
	bool		constrCorrection[6];///< 位置を拘束する自由度. 可動範囲が有効な場合はtrueとなる

	///コンストラクタ
	PHConstraint();

	///このクラス内の機能
	void		CompJacobian();
	void		SetupLCP();
	void		IterateLCP();
	void		UpdateState();
	void		CompResponseMatrix();
	void		CompResponseMatrixABA();
	void		SetupCorrectionLCP();
	void		IterateCorrectionLCP();
	
	///派生クラスの機能
	virtual void		 AddMotorTorque(){}							///< 拘束力に関節トルク分を加算
	virtual void		 SetConstrainedIndex(bool* con){}			///< どの自由度を速度拘束するかを設定
	virtual void		 SetConstrainedIndexCorrection(bool* con){	///< どの自由度を位置拘束するかを設定
		SetConstrainedIndex(con);
	}														
	virtual void		 ModifyJacobian(){}							///< 独自座標系を使う場合のヤコビアンの修正
	virtual void		 CompBias(){}								///< 
	virtual void		 Projection(double& f, int k){}				///< 拘束力の射影
	virtual void		 UpdateJointState(){}						///< 関節座標の位置・速度を更新する
	virtual void		 CompError(){}								///< Correction用の拘束誤差を設定する
	virtual void		 ProjectionCorrection(double& F, int k){}	///< 
	
	/// インタフェースの実装
	//virtual PHConstraintDesc::ConstraintType GetConstraintType(){ assert(0); return PHConstraintDesc::INVALID_CONSTRAINT; }
	virtual void		 Enable(bool bEnable = true){bEnabled = bEnable;}
	virtual bool		 IsEnabled(){return bEnabled;}
	virtual void		 SetInactive(int index = 0, bool Inaction = true){bInactive[index] = Inaction;}
	virtual bool		 IsInactive(int index = 0){return bInactive[index];}
	virtual void		 GetRelativePose(Posed& p){p.Pos() = Xjrel.r; p.Ori() = Xjrel.q;}
	virtual Vec3d		 GetRelativePoseR(){return Xjrel.r;}
	virtual Quaterniond	 GetRelativePoseQ(){return Xjrel.q;}
	virtual void		 GetRelativeVelocity(Vec3d& v, Vec3d& w){v = vjrel.v(); w = vjrel.w();}
	virtual void		 GetConstraintForce(Vec3d& _f, Vec3d& _t){_f = f.v(); _t = f.w();}
	virtual bool		 AddChildObject(ObjectIf* o);
	virtual size_t		 NChildObject();
	virtual ObjectIf*	 GetChildObject(size_t i);
	virtual bool		 GetDesc(void* desc);
	virtual void		 SetDesc(const void* desc);

};

class PHConstraints : public std::vector< UTRef<PHConstraint> >, public SceneObject, public PHConstraintsIfInit{
	
public:
	OBJECTDEF(PHConstraints, SceneObject);
	virtual PHConstraintIf* FindBySolidPair(PHSolidIf* lhs, PHSolidIf* rhs){
		for(iterator it = begin(); it != end(); it++)
			if((*it)->solid[0] == DCAST(PHSolid, lhs) && (*it)->solid[1] == DCAST(PHSolid, rhs))
				return XCAST(*it);
			else if((*it)->solid[0] == DCAST(PHSolid, rhs) && (*it)->solid[1] == DCAST(PHSolid, lhs))
				return XCAST(*it);

		return NULL;
	}
	
	/// 指定された剛体の組に作用している拘束を返す
	PHConstraint* FindBySolidPair(PHSolid* lhs, PHSolid* rhs){
		for(iterator it = begin(); it != end(); it++)
			if((*it)->solid[0] == lhs && (*it)->solid[1] == rhs)
				return *it;

		return NULL;
	}
};


}

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
#include <vector>

namespace Spr{;

class PHTreeNode;

struct PHConstraintState{
	SpatialVector f;				///< 拘束力の力積
	SpatialVector F;				///< 拘束誤差を位置のLCPで補正する場合の補正量*質量
	
	//一時的にSaveStateを有効にするためのソースコード
	Vec3d motorf;			//< もとはConstraintにあったfが分離したもの(PHMotor.hのf)
	Vec3d limitf;			//< もとはConstraintにあったfが分離したもの(PHJointLimit.hのf)
	//Vec3d limitF;
	Vec3d fs[50];				//< motorfを過去50ステップ分保存
	SpatialVector xs[2];	///< (3要素モデル専用)ばね部の距離

};

class PHConstraint : public SceneObject, public PHConstraintDesc, public PHConstraintState{
public:
	SPR_OBJECTDEF_ABST(PHConstraint);
	ACCESS_DESC_STATE(PHConstraint);


	PHConstraintEngine* engine;

	bool		bFeasible;				///< 両方の剛体がundynamicalな場合true
	bool		bArticulated;			///< 関節系を構成している場合true
	bool		bInactive[2];			///< 剛体が解析法に従う場合true	
	PHSolid*			solid[2];		///< 拘束する剛体
	SpatialTransform    X[2];			///< ワールド座標系の中心に対する親(子)剛体の位置と向き		#* 剛体から毎回取ってくる値
	SpatialTransform	Xj[2];			///< 剛体の質量中心に対するソケット，プラグの位置と向き		#* 関節を作るときに設定する値
	SpatialTransform	Xjrel;			///< ソケットに対するプラグの位置と向き						#* Xから計算
	SpatialVector		vjrel;			///< ソケットに対するプラグの相対速度,角速度				#* 剛体から計算
	
	SpatialTransform	Js[2];			///< 拘束ヤコビアン SpatialTranform形式，					#  Xj Xjrelから計算
												//[0]：親剛体中心からSocket座標系へ変換するヤコビアン
												//[1]：子剛体中心からSocket座標系へ変換するのヤコビアン
	SpatialMatrix		J[2];			///< 拘束ヤコビアン 行列形式								#[n_c x 6] Jsから計算。行列型が必要
												//[0]：親剛体の質量中心からSocket座標系へのヤコビアン
												//[1]：子剛体の質量中心からPlug座標系経由でSocket座標系へのヤコビアン
	SpatialMatrix		AinvJ[2];		//															# 不使用変数
	SpatialMatrix		T[2];			//	T = M.inv() * J^t										#[6 x n_c] ガウスザイデルで使用 拘束のある列だけで良い
	
	SpatialVector b, db, B;				///< LCPのbベクトルとその補正量								#[n_c] 拘束のある行だけで良い
	SpatialVector A, dA, Ainv;			///< LCPのA行列の対角成分とその補正量，逆数					#[n_c]
	SpatialVector scale;				//	不使用
	
	bool		constr[6];				///< 速度を拘束する自由度. 可動範囲，バネ・ダンパが有効な場合はtrueとなる	# 不要
	int			ConstNum[6];			// constrの変わり。計算の必要な軸番号を入れる
	int			ConstAxis;
	bool		constrCorrection[6];	///< 位置を拘束する自由度. 可動範囲が有効な場合はtrueとなる					# 不要
	
	double fMax;
	double fMaxDt[6],fMinDt[6];

	///コンストラクタ
	PHConstraint();

	///このクラス内の機能.
	virtual void	CompJacobian();
	virtual void	SetupLCP();
	virtual	void	IterateLCP();
	virtual void	SetupCorrectionLCP();
	virtual void	IterateCorrectionLCP();
	void		UpdateState();
	void		CompResponseMatrix();
	void		CompResponse(double df, int j);
	virtual PHSceneIf*	GetScene() const;
	
	///派生クラスの機能.
	//virtual void	AddMotorTorque(){}							///< 拘束力に関節トルク分を加算
	virtual void	SetConstrainedIndex(int* con){}			///< どの自由度を速度拘束するかを設定
	//virtual void	SetConstrainedIndexCorrection(bool* con){	///< どの自由度を位置拘束するかを設定
	//	SetConstrainedIndex(con);
	//}														
	virtual void	ModifyJacobian(){}							///< 独自座標系を使う場合のヤコビアンの修正
	virtual void	CompBias(){}								///< LCPの補正値の計算．誤差修正用
	virtual void	Projection(double& f, int k){}				///< 拘束力の射影
	virtual void	UpdateJointState(){}						///< 関節座標の位置・速度を更新する
	virtual void	CompError(){}								///< Correction用の拘束誤差を設定する
	virtual void	ProjectionCorrection(double& F, int k){}	///< 
	
	/// インタフェースの実装
	//virtual PHConstraintDesc::ConstraintType GetConstraintType(){ assert(0); return PHConstraintDesc::INVALID_CONSTRAINT; }
	virtual PHSolidIf* GetSocketSolid(){ return solid[0]->Cast(); }
	virtual PHSolidIf* GetPlugSolid(){ return solid[1]->Cast(); }
	virtual void GetSocketPose(Posed& pose){pose = poseSocket;}
	virtual void SetSocketPose(const Posed& pose){
		poseSocket = pose;
		Xj[0].q = pose.Ori();
		Xj[0].r = pose.Pos();
	}
	virtual void GetPlugPose(Posed& pose){pose = posePlug;}
	virtual void SetPlugPose(const Posed& pose){
		posePlug = pose;
		Xj[1].q = pose.Ori();
		Xj[1].r = pose.Pos();
	}
	virtual void		 Enable(bool bEnable = true){bEnabled = bEnable;}
	virtual bool		 IsEnabled(){return bEnabled;}
	virtual void		 SetInactive(int index = 0, bool Inaction = true){bInactive[index] = Inaction;}
	virtual bool		 IsInactive(int index = 0){return bInactive[index];}
	virtual void		 GetRelativePose(Posed& p){UpdateState(); p.Pos() = Xjrel.r; p.Ori() = Xjrel.q;}
	virtual Vec3d		 GetRelativePoseR(){UpdateState();return Xjrel.r;}
	virtual Quaterniond	 GetRelativePoseQ(){UpdateState();return Xjrel.q;}
	virtual Quaterniond  GetAbsolutePoseQ(){UpdateState();return Xjrel.q * X[0].q;}	//< Socketをつける位置も気にするべきか？
	virtual void		 GetRelativeVelocity(Vec3d& v, Vec3d& w);//{UpdateCacheLCP(GetScene()->GetTimeStep()); UpdateState();v = vjrel.v(); w = vjrel.w();}
	virtual void		 GetConstraintForce(Vec3d& _f, Vec3d& _t);
	virtual bool		 AddChildObject(ObjectIf* o);
	virtual size_t		 NChildObject() const;
	virtual ObjectIf*	 GetChildObject(size_t i);
	virtual	Vec3d		 GetMotorf(){return motorf;}
	virtual Vec3d		 GetLimitf(){return limitf;}

protected:
	virtual void AfterSetDesc();
};

class PHConstraints : public std::vector< UTRef<PHConstraint> >, public SceneObject{
public:
	SPR_OBJECTDEF(PHConstraints);
	virtual PHConstraintIf* FindBySolidPair(PHSolidIf* lhs, PHSolidIf* rhs){
		for(iterator it = begin(); it != end(); it++) {
			if((*it)->solid[0] == DCAST(PHSolid, lhs) && (*it)->solid[1] == DCAST(PHSolid, rhs)) {
				return XCAST(*it);
			} else if((*it)->solid[0] == DCAST(PHSolid, rhs) && (*it)->solid[1] == DCAST(PHSolid, lhs)) {
				return XCAST(*it);
			}
		}

		return NULL;
	}
	
	/// 指定された剛体の組に作用している拘束を返す
	PHConstraint* FindBySolidPair(PHSolid* lhs, PHSolid* rhs){
		for(iterator it = begin(); it != end(); it++)
			if((*it)->solid[0] == lhs && (*it)->solid[1] == rhs)
				return *it;

		return NULL;
	}

	/// 指定された剛体の組に作用している総合的な力を返す
	Vec3d GetTotalForce(PHSolidIf* lhs, PHSolidIf* rhs){
		Vec3d total = Vec3f();
		for(iterator it = begin(); it != end(); it++){
			if((*it)->solid[0] == DCAST(PHSolid, lhs) && (*it)->solid[1] == DCAST(PHSolid, rhs)){
				Vec3d _f , _t;
				(*it)->GetConstraintForce(_f, _t);
				SpatialVector __f;
				__f.v() = _f; __f.w() = _t;
				// DSTR << "J[0].tarns : " << ((*it)->J[0].trans()) << std::endl;
				PTM::TVector<6,double> Jf = ((*it)->J[0].trans() * __f);
				Vec3d Jfv;
				for(int i=0; i<3; ++i){ Jfv[i] = Jf[i]; }
				total += Jfv;
			}
		}
		return total;
	}
};

}

#endif

/*
 *  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONSTRAINT_H
#define PHCONSTRAINT_H

#include <Physics/PHContactDetector.h>
#include <Physics/PHSpatial.h>
#include <Physics/PHAxisIndex.h>

#include <vector>

namespace Spr{;

class PHTreeNode;

/// 拘束のステート
struct PHConstraintState {
	/// 拘束力の力積
	SpatialVector f;

	/// 拘束誤差を位置のLCPで補正する場合の補正量*質量
	SpatialVector F;

	// ----- PHJointに関連する変数

	/// ばね部の距離（三要素モデル用）
	SpatialVector xs;

	/// 拘束力にローパスをかけたもの
	SpatialVector fAvg;

	/// 降伏したかどうか
	bool   bYielded;

	PHConstraintState() {
		xs       = SpatialVector();
		fAvg     = SpatialVector();
		bYielded = false;
	}
};

/// 拘束
class PHConstraint : public SceneObject, public PHConstraintDesc, public PHConstraintState {
public:

	SPR_OBJECTDEF_ABST(PHConstraint);
	ACCESS_DESC_STATE(PHConstraint);

	/// 拘束を管理するエンジン
	PHConstraintEngine* engine;

	/// 拘束する剛体
	PHSolid* solid[2];

	// ----- フラグ

	/// 両方の剛体がundynamicalな場合true
	bool bFeasible;				

	/// 関節系を構成している場合true
	bool bArticulated;			

	/// 剛体が解析法に従う場合true	
	bool bInactive[2];			

	// ----- 計算用変数

	/// ワールド座標系の中心に対する親(子)剛体の位置と向き   #* 剛体から毎回取ってくる値
	SpatialTransform X[2];

	/// 剛体の質量中心に対するソケット，プラグの位置と向き   #* 関節を作るときに設定する値
	SpatialTransform Xj[2];

	/// ソケットに対するプラグの位置と向き                   #* Xから計算
	SpatialTransform Xjrel;

	/// ソケットに対するプラグの相対速度,角速度              #* 剛体から計算
	SpatialVector vjrel;			
	
	/// 拘束ヤコビアン Pose(Vec+Quaternion)形式              #* Xj Xjrelから計算
	///   [0]：親剛体中心からSocket座標系へ変換するヤコビアン
	///   [1]：子剛体中心からSocket座標系へ変換するのヤコビアン
	SpatialTransform Js[2];
	
	/// 拘束ヤコビアン 行列形式                              #[n_c x 6] Jsから計算。行列型が必要
	///   [0]：親剛体の質量中心からSocket座標系へのヤコビアン
	///   [1]：子剛体の質量中心からPlug座標系経由でSocket座標系へのヤコビアン
	SpatialMatrix J[2];

	/// T = M.inv() * J^t ガウスザイデルで使用               #[6 x n_c] 拘束のある行だけで良い
	SpatialMatrix T[2];
	
	/// LCPのbベクトルとその補正量                           #[n_c]     拘束のある行だけで良い
	SpatialVector b, db, B;
	
	/// LCPのA行列の対角成分とその補正量，逆数               #[n_c]
	SpatialVector A, dA, Ainv;	

	/// Projection用の各軸のMin/Max
	double fMaxDt[6], fMinDt[6];

	// ----- 拘束軸管理

	/// 拘束軸管理クラス
	AxisIndex<6> axes;

	/// 拘束の対象となりうる軸番号リスト
	int targetAxes[6];

	/// targetAxesの要素数
	int nTargetAxes;

	/// 可動（＝本来拘束しない）軸番号リスト
	int movableAxes[6];

	/// movableAxesの要素数
	int nMovableAxes;

	// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

	/// コンストラクタ
	PHConstraint();

	/// 拘束対象軸番号リストの構築．コンストラクタでmovableAxesを決めた後に呼ぶ
	void InitTargetAxes();

	// ----- エンジンから呼び出される関数

	/// 状態の更新
	void UpdateState();

	/// LCPを解く前段階の計算
	virtual void SetupLCP();

	/// LCPの繰り返し計算
	virtual	void IterateLCP();

	/// 位置LCPを解く前段階の計算
	virtual void SetupCorrectionLCP();

	/// 位置LCPの繰り返し計算
	virtual void IterateCorrectionLCP();

	// ----- このクラスで実装する機能

	/// 拘束する2つの剛体の各速度から相対速度へのヤコビアンを計算
	virtual void CompJacobian();

	/// Aの対角成分を計算する．A = J * M^-1 * J^T
	void CompResponseMatrix();

	/// 拘束力変化量(df)に対する加速度変化量(dvの差分)を計算して反映
	void CompResponse(double df, int j);

	// ----- 派生クラスで実装する機能

	/// 関節座標の位置・速度を更新する
	virtual void	UpdateJointState(){}

	/// どの自由度を速度拘束するかを設定
	virtual void	SetupAxisIndex();

	/// 独自座標系を使う場合のヤコビアンの修正
	virtual void	ModifyJacobian(){}							

	/// LCPの補正値の計算．誤差修正用
	virtual void	CompBias(){}

	/// 拘束力の射影
	virtual void	Projection(double& f_, int i);

	/// Correction用の拘束誤差を設定する
	virtual void	CompError(){}

	/// Correction用の射影
	virtual void	ProjectionCorrection(double& F, int k){}

	// ----- インタフェースの実装

	virtual PHSceneIf*	 GetScene() const { return DCAST(PHSceneIf, SceneObject::GetScene()); }
	virtual PHSolidIf*	 GetSocketSolid() { return solid[0]->Cast(); }
	virtual PHSolidIf*	 GetPlugSolid()   { return solid[1]->Cast(); }

	virtual void		 Enable(bool bEnable = true){ bEnabled = bEnable; }
	virtual bool		 IsEnabled(){return bEnabled;}

	virtual void		 SetInactive(int index = 0, bool Inaction = true){ bInactive[index]=Inaction; }
	virtual bool		 IsInactive(int index = 0){ return bInactive[index]; }

	virtual void		 GetSocketPose(Posed& pose){ pose=poseSocket; }
	virtual void		 SetSocketPose(const Posed& pose){ poseSocket=pose; Xj[0].q=pose.Ori(); Xj[0].r=pose.Pos(); }
	virtual void		 GetPlugPose(Posed& pose){ pose=posePlug; }
	virtual void		 SetPlugPose(const Posed& pose){ posePlug=pose; Xj[1].q=pose.Ori(); Xj[1].r=pose.Pos(); }

	virtual void		 GetRelativePose(Posed& p){ UpdateState(); p.Pos()=Xjrel.r; p.Ori()=Xjrel.q; }
	virtual Vec3d		 GetRelativePoseR(){ UpdateState(); return Xjrel.r; }
	virtual Quaterniond	 GetRelativePoseQ(){ UpdateState(); return Xjrel.q; }
	virtual Quaterniond  GetAbsolutePoseQ(){ UpdateState(); return Xjrel.q * X[0].q; } // Socketをつける位置も気にするべきか？

	virtual void		 GetRelativeVelocity(Vec3d& v, Vec3d& w);
	virtual void		 GetConstraintForce(Vec3d& _f, Vec3d& _t);

	virtual bool		 AddChildObject(ObjectIf* o);
	virtual size_t		 NChildObject() const;
	virtual ObjectIf*	 GetChildObject(size_t i);

protected:
	virtual void		 AfterSetDesc();

};

/// 拘束コンテナ
class PHConstraints : public std::vector< UTRef<PHConstraint> >, public SceneObject {
public:

	SPR_OBJECTDEF(PHConstraints);

	/// 指定された剛体の組に作用している拘束を返す（If版）
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
		for(iterator it = begin(); it != end(); it++) {
			if((*it)->solid[0] == lhs && (*it)->solid[1] == rhs) {
				return *it;
			}
		}

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

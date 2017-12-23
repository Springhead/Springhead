/*
 *  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONSTRAINT_H
#define PHCONSTRAINT_H

#include <SprDefs.h>
#include "PHContactDetector.h"
#include "PHAxisIndex.h"

#include <vector>

#ifdef USE_OPENMP_PHYSICS
# include <omp.h>
#endif

namespace Spr{;

class PHTreeNode;

/** 拘束のステート */
struct PHConstraintState {
	/// 拘束力の力積
	Vec6d f;

	/// 拘束誤差を位置のLCPで補正する場合の補正量*質量
	Vec6d F;

	// ----- PHJointに関連する変数

	/// ばね部の距離（三要素モデル用）
	Vec6d xs;

	/// 拘束力にローパスをかけたもの
	Vec6d fAvg;

	/// 降伏したかどうか
	bool   bYielded;

	PHConstraintState() {
		bYielded = false;
	}
};

/// 拘束の基本クラス．PHConstraint, PHJointLimit, PHJointMotor, PHGearが派生
class PHConstraintBase{
public:
	SpatialVector b, db, B;
	SpatialVector A, dA, Ainv;
	SpatialVector f;
	SpatialVector F;
	SpatialVector fnew;
	SpatialVector Fnew;
	SpatialVector df;
	SpatialVector dv;
	SpatialVector dF;
	SpatialVector dV;
	SpatialVector res;  //response
	
	AxisIndex<6> axes;		///< 拘束軸管理クラス
	bool         dv_changed[6];
	bool         dv_changed_next[6];

	virtual void SetupAxisIndex    (){}
	virtual void Setup             (){}	///< 速度LCPの前処理
	virtual void SetupCorrection   (){}	///< 位置LCPの前処理
	virtual	bool Iterate           (){ return true; }	///< 速度LCP(GS)の繰り返し計算
	virtual bool IterateCorrection (){ return true; }	///< 位置LCP(GS)の繰り返し計算
	virtual void CompResponseMatrix(){}
	virtual void CompResponse      (double df, int i){}
};

/// 拘束
class PHConstraint : public SceneObject, public PHConstraintDesc, public PHConstraintBase{
public:
	SPR_OBJECTDEF_ABST(PHConstraint);
	ACCESS_DESC(PHConstraint);

	/// 拘束を管理するエンジン
	PHConstraintEngine* engine;

	/// 拘束する剛体
	PHSolid* solid[2];
	
	/// 関節系を構成している場合のツリーノード
	PHTreeNode*		treeNode;

	/// 剛体が解析法に従う場合true	
	bool bInactive[2];

	int solidState[2];

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

	SpatialVector fAvg;					///< 拘束力にローパスをかけたもの
	SpatialVector xs;					///< ばね部の距離（三要素モデル用）
	bool   bYielded;					///< 降伏したかどうか
		
	double fMaxDt[6], fMinDt[6];	///< Projection用の各軸のMin/Max

	/** ----- 拘束軸管理
		拘束空間は全6自由度（並進x,y,z + 回転x,y,z）．
		- movableAxes : 可動自由度の集合
		- targetAxes  : 拘束自由度の集合．movableAxesの補集合
		
		- movableAxesに属する自由度は，モータ有効時や可動範囲境界にある場合に動的に拘束対象となる．
		  axesは拘束自由度および動的に拘束対象となっている可動自由度の両方を保持し，かつ前ステップからの拘束状態の切り替わりを記憶する．
	 */

	AxisIndex<6>	movableAxes;		///< 可動自由度
	AxisIndex<6>	targetAxes;			///< 関節自身，可動範囲，モータのいずれかによって拘束される自由度

public:
	// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

	/// コンストラクタ
	PHConstraint();
	virtual ~PHConstraint();

	/// 拘束対象軸番号リストの構築．コンストラクタでmovableAxesを決めた後に呼ぶ
	void InitTargetAxes();

	// ----- エンジンから呼び出される関数
	/// ツリーノードを構成しているか
	bool IsArticulated();
	/// 
	bool IsFeasible();
	/// 状態の更新
	void UpdateState();
	
	/// PHConstraintBaseの仮想関数
	virtual void Setup                 ();
	virtual void SetupCorrection       ();
	virtual	bool Iterate               ();
	virtual bool IterateCorrection     ();
	virtual bool Projection            (double& f_, int i);
	virtual bool ProjectionCorrection  (double& F_, int i){ return false; }
	virtual void CompResponse          (double  df, int i); ///< (M^-1 J^T) df
	virtual void CompResponseCorrection(double  dF, int i); ///< (M^-1 J^T) dF
	virtual void CompResponseMatrix    ();
	
	// ----- このクラスで実装する機能

	/// 拘束する2つの剛体の各速度から相対速度へのヤコビアンを計算
	void CompJacobian();

	// ----- 派生クラスで実装する機能

	/// 関節座標の位置・速度を更新する
	virtual void	UpdateJointState(){}

	/// どの自由度を速度拘束するかを設定
	virtual void	SetupAxisIndex();

	/// 独自座標系を使う場合のヤコビアンの修正
	//virtual void	ModifyJacobian(){}							

	/// LCPの補正値の計算．誤差修正用
	virtual void	CompBias(){}

	/// Correction用の拘束誤差を設定する
	virtual void	CompError(){}

	// ----- インタフェースの実装
	PHSceneIf*	 GetScene() const { return DCAST(PHSceneIf, SceneObject::GetScene()); }
	PHSolidIf*	 GetSocketSolid() { return solid[0]->Cast(); }
	PHSolidIf*	 GetPlugSolid()   { return solid[1]->Cast(); }

	void		 Enable(bool bEnable = true){ bEnabled = bEnable; }
	bool		 IsEnabled(){return bEnabled;}

	void		 SetInactive(int index = 0, bool Inaction = true){ bInactive[index]=Inaction; }
	bool		 IsInactive(int index = 0){ return bInactive[index]; }

	void		 GetSocketPose(Posed& pose){ pose=poseSocket; }
	void		 SetSocketPose(const Posed& pose){ poseSocket=pose; Xj[0].q=pose.Ori(); Xj[0].r=pose.Pos(); }
	void		 GetPlugPose(Posed& pose){ pose=posePlug; }
	void		 SetPlugPose(const Posed& pose){ posePlug=pose; Xj[1].q=pose.Ori(); Xj[1].r=pose.Pos(); }

	void		 GetRelativePose(Posed& p){ UpdateState(); p.Pos()=Xjrel.r; p.Ori()=Xjrel.q; }
	Vec3d		 GetRelativePoseR(){ UpdateState(); return Xjrel.r; }
	Quaterniond	 GetRelativePoseQ(){ UpdateState(); return Xjrel.q; }
	Quaterniond  GetAbsolutePoseQ(){ UpdateState(); return Xjrel.q * X[0].q; } // Socketをつける位置も気にするべきか？

	void		 GetRelativeVelocity(Vec3d& v, Vec3d& w);
	void		 GetConstraintForce(Vec3d& _f, Vec3d& _t);

	bool         IsYielded(){ return this->bYielded; }

	/// Objectの仮想関数
	virtual bool		 AddChildObject(ObjectIf* o);
	virtual size_t		 NChildObject() const;
	virtual ObjectIf*	 GetChildObject(size_t i);

	virtual size_t       GetStateSize() const { return sizeof(PHConstraintState); }
	virtual bool         GetState(void* s) const;
	virtual void         SetState(const void* s);
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
			if((*it)->solid[0] == lhs && (*it)->solid[1] == rhs && (*it)->IsEnabled()) {
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
				__f.v() = _f;
				__f.w() = _t;
#ifdef	_WIN32
				SpatialVector Jf = (const SpatialVector&)((*it)->J[0].trans() * __f);
#else
				Vec6d tmp_matrix = (*it)->J[0].trans() * __f;
				SpatialVector Jf = (const SpatialVector&) tmp_matrix;
#endif
				total += Jf.v();
			}
		}
		return total;
	}

};

}

#endif

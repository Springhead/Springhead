/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSOLID_H
#define PHSOLID_H

#include "PHBody.h"
#include "PHEngine.h"
#include "SprPhysicsDecl.hpp"

namespace Spr{;

class PHSolid;
class CDShape;


class PHTreeNode;
class PHScene;
class PHConstraintEngine;

struct PHSolidStatePrivate{
	DUMPLABEL(PHSolidStatePrivate)
	bool		bFrozen;		///<	フリーズ状態か
	bool		bUpdated;		///<	複数のエンジンでSolidの更新を管理するためのフラグ
	//	剛体に加えられた力
	Vec3d		nextForce;		///<	次の積分でこの剛体に加わわる力(World系)
	Vec3d		nextTorque;		///<	次の積分でこの剛体に加わわるトルク(World系)
	Vec3d		force;			///<	前の積分でこの剛体に加わった力(World系)
	Vec3d		torque;			///<	前の積分でこの剛体に加わったトルク(World系)
	Vec3d		accel;
	Vec3d		angAccel;
};

///	剛体
class PHSolid : public PHBody, public PHSolidDesc, public PHSolidStatePrivate{
protected:
	//Matrix3d	inertia_inv;	///<	慣性テンソルの逆数(Local系・キャッシュ)

	///	積分方式
	PHIntegrationMode integrationMode;

	/// オイラーの運動方程式
	/// 慣性行列は対角行列を前提．
	Vec3d	Euler(const Matrix3d& I, const Vec3d& t, const Vec3d& w){
		return Vec3d(
			(t[0] - (I[2][2] - I[1][1]) * w.Y() * w.Z()) / I[0][0],
			(t[1] - (I[0][0] - I[2][2]) * w.Z() * w.X()) / I[1][1],
			(t[2] - (I[1][1] - I[0][0]) * w.X() * w.Y()) / I[2][2]);
	}
public:
	///@name LCP関連補助変数
	//@{
	DUMPLABEL(LCPRelated)
	PHConstraintEngine* engine;
	PHTreeNode*	        treeNode;	  ///< 関節系を構成している場合の対応するノード
	///@name LCP関連補助変数
	//@{
	DUMPLABEL(minv)
	double		        minv;		  ///< 質量の逆数
	Matrix3d	        Iinv;		  ///< 慣性行列の逆行列
	SpatialMatrix		Minv;
	DUMPLABEL(fv)
	SpatialVector       f;			  ///< ローカル座標での外力
	SpatialVector       v;			  ///< ローカル座標での現在の速度
	SpatialVector       dv0;		  ///< 外力のみによる速度変化
	DUMPLABEL(dv)
	SpatialVector		dv;			  ///< 外力と拘束力による速度変化
	DUMPLABEL(DV)
	SpatialVector       dV;			  ///< Correctionによる移動量，回転量
	double				velocityNorm;
	double				angVelocityNorm;
	DUMPLABEL(LCPRelatedEnd)
	//@}

	///	LCP関連補助変数の初期化。毎ステップLCPの前に呼ばれる。
	void UpdateCacheLCP(double dt);
	///	dvを速度に足し込む 
	virtual void UpdateVelocity(double* dt);
	///	速度を位置に足し込む。UpdateVelocity()の後でUpdatePosition()ならば、シンプレクティック数値積分になる。
	virtual void UpdatePosition(double dt);
	///	ABAの支配下にあるかどうか
	bool IsArticulated();
	//@}
		
public:
	SPR_OBJECTDEF(PHSolid);
	ACCESS_DESC(PHSolid);
	ACCESS_STATE_PRIVATE1(PHSolid, PHBody);
	PHSolid(const PHSolidDesc& desc=PHSolidDesc(), SceneIf* s=NULL);

	virtual SceneObjectIf* CloneObject();
	CDShapeIf* CreateAndAddShape(const IfInfo* info, const CDShapeDesc& desc);
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	bool AddChildObject(ObjectIf* obj);
	bool DelChildObject(ObjectIf* obj);
	size_t NChildObject() const{
		return frames.size();
	}
	ObjectIf* GetChildObject(size_t pos) {
		return frames[pos]->Cast();
	}

	virtual bool CalcBBox();						///< 剛体と各形状のローカルBBoxを計算
	virtual void CalcAABB();						///< 剛体と各形状のワールドBBoxを計算
	virtual void GetBBoxSupport(const Vec3f& dir, float& minS, float& maxS);
	virtual void GetBBox(Vec3d& bbmin, Vec3d& bbmax, bool world);

	void		SetUpdated(bool set){bUpdated = set;}	///< 
	bool		IsUpdated(){return bUpdated;}			///< 
	void		Step();									///< 時刻を進める．
	
	void		AddForce(Vec3d f);						///< 力を質量中心に加える
	void		AddTorque(Vec3d t);						///< トルクを加える
	void		AddForce(Vec3d f, Vec3d r);				///< 力を 位置r(World系) に加える
	/// 拘束力以外の剛体に加わった外力。拘束力はPHConstraintから取得しなければならない。
	Vec3d		GetForce() const {return force;}
	/// 拘束力以外の剛体に加わった外力によるトルク。拘束トルクはPHConstraintから取得しなければならない。
	Vec3d		GetTorque() const {return torque;}

	double		GetMass() {return mass;}						///< 質量
	double		GetMassInv(){return 1.0 / mass;}				///< 質量の逆数
	void		SetMass   (double m)   {mass = m;}				///< 質量の設定
	void		SetMassInv(double minv){mass = 1.0 / minv;}		///< 質量の逆数の設定
	Vec3d		GetCenterOfMass() {return center;}				///< ローカルフレームから見た，剛体の質量中心位置の設定
	void		SetCenterOfMass(const Vec3d& c){center = c;}	///< ローカルフレームから見た，剛体の質量中心位置の取得
	Matrix3d	GetInertia   (){return inertia;}				///< 慣性テンソル
	Matrix3d	GetInertiaInv(){return Iinv;}					///< 慣性テンソルの逆数
	Matrix3d	GetGlobalInertia();              				///< グローバル回転の慣性テンソル
	void		SetInertia   (const Matrix3d& I){ inertia = I; Iinv = I.inv(); }				///< 慣性テンソルの設定
	void		SetInertiaInv(const Matrix3d& _Iinv){ Iinv = _Iinv; inertia = Iinv.inv(); }		///< 慣性テンソルを逆数で設定
	void		CompInertia();

	///	積分方式の取得
	PHIntegrationMode GetIntegrationMode(){ return integrationMode; }
	///	積分方式の設定
	void SetIntegrationMode(PHIntegrationMode m){ integrationMode=m; }

	Posed		GetPose             () { return pose; }
	Vec3d		GetFramePosition    () { return pose.Pos(); }
	Posed 		GetPrevPose();
	Vec3d		GetDeltaPosition    ();
	Vec3d		GetDeltaPosition    (const Vec3d& p);
	Vec3d		GetPrevFramePosition() { return pose.Pos() - GetDeltaPosition(); }
	Matrix3d	GetRotation         () { Matrix3d rv; pose.Ori().ToMatrix(rv); return rv; }		///	向きの取得
	Quaterniond GetOrientation      () { return pose.Ori(); }									///	向きの取得
	void		SetPose             (const Posed& p)      { pose       = p;                     aabbReady = false; }
	void		SetFramePosition    (const Vec3d& p)      { pose.Pos() = p;                     aabbReady = false; }
	void		SetCenterPosition   (const Vec3d& p)      { pose.Pos() = p - pose.Ori()*center; aabbReady = false; }	///< 重心位置の設定(World)
	void		SetRotation         (const Matrix3d& r)   { pose.Ori().FromMatrix(r);           aabbReady = false; }	///	向きの設定
	void		SetOrientation      (const Quaterniond& q){ pose.Ori() = q;                     aabbReady = false; }	///	向きの設定
	void        SetDv               (const SpatialVector& a){ dv = a; }   /// 速度の変化量を設定する <!!>aabbReadyが必要なのかが分からない

	///	質量中心の速度の取得
	Vec3d		GetVelocity() {return velocity;}
	///	質量中心の速度の設定
	void		SetVelocity(const Vec3d& v);

	///	角速度の取得
	Vec3d		GetAngularVelocity() {return angVelocity;}
	///	角速度の設定
	void		SetAngularVelocity(const Vec3d& av);

	//GMS用
	///	質量中心の速度と角速度をまとめて取得
	//SpatialVector GetSpatialVelocity() const { return SpatialVector(velocity, angVelocity); }

	///	（最後のStep()での）剛体の質量中心の加速度
	//SpatialVector GetAcceleration() const;

	/// 重力を加えるかどうか設定
	void		SetGravity(bool bOn);
	/// 物理法則に従うかどうか設定
	void		SetDynamical(bool bOn){ dynamical = bOn; }
	/// 物理法則に従っているかどうかを取得
	bool		IsDynamical(){ return dynamical; }
	void        SetStationary(bool bOn){ stationary = bOn; }
	bool        IsStationary(){ return stationary; }
	/// 速度が一定以下の時，積分を行わないように設定
	void		SetFrozen(bool bOn){bFrozen = bOn;}
	/// 速度が一定以下で積分を行わないかどうかを取得
	bool		IsFrozen(){return bFrozen;}
	PHTreeNodeIf* GetTreeNode();

	//For FEM multiple interface collision
	int			femIndex;   //Stores the FEM mesh index

protected:
	virtual void AfterSetDesc();
};

typedef std::vector< PHSolid* >	PHSolids;
typedef std::vector< UTRef<PHSolid> >	PHSolidRefs;

/**	Solidを保持するクラス．Solidの更新も行う．	*/
class PHSolidContainer:public PHEngine{
	SPR_OBJECTDEF_NOIF(PHSolidContainer);
public:
	PHSolidRefs solids;
	///
	int GetPriority() const {return SGBP_SOLIDCONTAINER;}
	/// solidのリセット
	void Reset();
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	
	virtual void Clear(){ solids.clear(); }
	bool AddChildObject(ObjectIf* o);
	bool DelChildObject(ObjectIf* o);
	///	所有しているsolidの数
	virtual size_t NChildObject() const { return solids.size(); }
	///	所有しているsolid
	virtual ObjectIf* GetChildObject(size_t i){ return solids[i]->Cast(); }

	PHSolidContainer();
};

/** SolidContainerのResetを呼ぶためだけのエンジン */
class PHSolidInitializer : public PHEngine{
	SPR_OBJECTDEF_NOIF(PHSolidInitializer);
public:
	PHSolidContainer* container;
	
	///
	int GetPriority() const {return SGBP_SOLIDINITIALIZER;}
	///	updatedフラグをリセットする
	virtual void Step();
};


}	//	namespace Spr
#endif

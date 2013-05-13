/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSOLID_H
#define PHSOLID_H

#include <Physics/SprPHSolid.h>
#include <Collision/SprCDShape.h>
#include "../Physics/PHEngine.h"
#include "../Physics/PHSpatial.h"
#include "../Foundation/Object.h"

namespace Spr{;

class CDShape;

///	バウンディングボックスの実装
class PHBBox{
	Vec3f bboxCenter;	///<	BBoxの中心(ローカル系)
	Vec3f bboxExtent;	///<	BBoxの大きさ(ローカル系)
public:
	///	バウンディングボックスの設定
	void SetBBoxCenterExtent(Vec3f c, Vec3f e){
		bboxCenter = c;
		bboxExtent = e;
	}
	///	バウンディングボックスの設定
	void SetBBoxMinMax(Vec3f bmin, Vec3f bmax){
		bboxCenter = (bmin+bmax)*0.5f;
		bboxExtent = (bmax-bmin)*0.5f;
	}
	///	バウンディングボックスの追加
	void AddBBox(Vec3f bmin, Vec3f bmax){
		Vec3f bboxMin = GetBBoxMin();
		Vec3f bboxMax = GetBBoxMax();
		bboxMin.element_min(bmin);
		bboxMax.element_max(bmax);
		SetBBoxMinMax(bboxMin, bboxMax);
	}
	///	中心
	Vec3f GetBBoxCenter(){ return bboxCenter; }
	///	大きさ
	Vec3f GetBBoxExtent(){ return bboxExtent; }
	///	小さい端点
	Vec3f GetBBoxMin(){ return bboxCenter-bboxExtent; }
	///	大きい端点
	Vec3f GetBBoxMax(){ return bboxCenter+bboxExtent; }

	///	与えられたベクトルとの内積が最大と最小の点
	void GetSupport(const Vec3f& dir, float& minS, float& maxS);
	
	///	SolidのPoseを代入することで，world座標系の最小値,最大値を取得
	/// (注意）バウンディングボックスよりも大きなボックスで判定されてしまう．
	//
	void GetBBoxWorldMinMax(Posed& pos , Vec3d& min, Vec3d& max);
};


enum PHIntegrationMode{
		PHINT_NONE,				///	積分しない
		PHINT_ARISTOTELIAN,		///	f = mv
		PHINT_EULER,			///	オイラー法
		PHINT_SIMPLETIC,		/// シンプレクティック法
		PHINT_ANALYTIC,			
		PHINT_RUNGEKUTTA2,		///	２次ルンゲクッタ法
		PHINT_RUNGEKUTTA4		///	４次ルンゲクッタ法
};

class PHSolid;
class PHFrame: public SceneObject, public PHFrameDesc{
	friend class PHSolid;
protected:
	PHSolid* solid;
	CDShape* shape;

	double		mass;
	Vec3d		center;
	Matrix3d	inertia;
public:
	SPR_OBJECTDEF(PHFrame);
	ACCESS_DESC(PHFrame);

	PHFrame();
	PHFrame(const PHFrameDesc& desc);
	void	CompInertia();
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual size_t NChildObject() const;
	virtual Posed GetPose();
	virtual void SetPose(Posed p);
};

class PHTreeNode;
class PHScene;
class PHConstraintEngine;

struct PHSolidStatePrivate{
	bool		bFrozen;		///<	フリーズ状態か
	bool		bUpdated;		///<	複数のエンジンでSolidの更新を管理するためのフラグ
	//	剛体に加えられた力
	Vec3d		nextForce;		///<	次の積分でこの剛体に加わわる力(World系)
	Vec3d		nextTorque;		///<	次の積分でこの剛体に加わわるトルク(World系)
	Vec3d		force;			///<	前の積分でこの剛体に加わった力(World系)
	Vec3d		torque;			///<	前の積分でこの剛体に加わったトルク(World系)
	///@name LCP関連補助変数
	//@{
	//	LCP関連補助変数だが、加速度の取得に用いるのでStateにしなければならない。
	SpatialVector dv;			///<	拘束エンジンによる速度変化量（外力を含む）
	//@}
};

///	剛体
class PHSolid : public SceneObject, public PHSolidDesc, public PHSolidStatePrivate{
protected:
	Matrix3d	inertia_inv;	///<	慣性テンソルの逆数(Local系・キャッシュ)

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
	PHScene*	scene;
	PHConstraintEngine* engine;
	PHTreeNode*	treeNode;	///< 関節系を構成している場合の対応するノード
	double		minv;		///< 質量の逆数
	Matrix3d	Iinv;		///< 慣性行列の逆行列
	SpatialVector f;		///< ローカル座標での外力
	SpatialVector v;		///< ローカル座標での現在の速度
	SpatialVector dv0;		///< 外力のみによる速度変化
	SpatialVector dV;		///< Correctionによる移動量，回転量
	///	LCP関連補助変数の初期化。毎ステップLCPの前に呼ばれる。
	void UpdateCacheLCP(double dt);
	///	dvを速度に足し込む 
	virtual void UpdateVelocity(double dt);
	///	速度を位置に足し込む。UpdateVelocity()の後でUpdatePosition()ならば、シンプレクティック数値積分になる。
	virtual void UpdatePosition(double dt);
	///	ABAの支配下にあるかどうか
	bool IsArticulated();
	//@}
		
public:
	std::vector< UTRef<PHFrame> > frames;
	PHBBox bbox;

	SPR_OBJECTDEF(PHSolid);
	ACCESS_DESC_STATE_PRIVATE(PHSolid);
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

	void CalcBBox();
	void GetBBoxSupport(const Vec3f& dir, float& minS, float& maxS);
	
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

	double		GetMass(){return mass;}					///< 質量
	double		GetMassInv(){return 1.0 / mass;}		///< 質量の逆数
	void		SetMass(double m){mass = m;}			///< 質量の設定
	void		SetMassInv(double minv){mass = 1.0 / minv;}	///< 質量の逆数の設定
	Vec3d		GetCenterOfMass(){return center;}		///< ローカルフレームから見た，剛体の質量中心位置の設定
	void		SetCenterOfMass(const Vec3d& c){center = c;}	///< ローカルフレームから見た，剛体の質量中心位置の取得
	Matrix3d	GetInertia(){return inertia;}			///< 慣性テンソル
	Matrix3d	GetInertiaInv(){return inertia_inv;}	///< 慣性テンソルの逆数
	void		SetInertia(const Matrix3d& I){			///< 慣性テンソルの設定
		inertia = I;
		inertia_inv = I.inv();
	}
	void		SetInertiaInv(const Matrix3d& Iinv){	///< 慣性テンソルを逆数で設定
		inertia_inv = Iinv;
		inertia = Iinv.inv();
	}
	void		CompInertia();

	///	積分方式の取得
	PHIntegrationMode GetIntegrationMode(){ return integrationMode; }
	///	積分方式の設定
	void SetIntegrationMode(PHIntegrationMode m){ integrationMode=m; }

	Posed		GetPose() const { return pose; }
	void		SetPose(const Posed& p) { pose = p; }
	Vec3d		GetFramePosition() const {return pose.Pos();}
	void		SetFramePosition(const Vec3d& p){pose.Pos() = p;}
	Vec3d		GetDeltaPosition() const ;
	Vec3d		GetDeltaPosition(const Vec3d& pos) const ;
	Vec3d		GetPrevFramePosition() const { return pose.Pos()-GetDeltaPosition(); }
	Vec3d		GetCenterPosition() const {return pose*center;} 	///< 重心位置の取得(World)
	void		SetCenterPosition(const Vec3d& p){					///< 重心位置の設定(World)
		pose.Pos() = p - pose.Ori()*center;
	}

	///	向きの取得
	Matrix3d	GetRotation() const { Matrix3d rv; pose.Ori().ToMatrix(rv); return rv; }
	///	向きの設定
	void		SetRotation(const Matrix3d& r){
		pose.Ori().FromMatrix(r);
	}

	///	向きの取得
	Quaterniond GetOrientation() const {return pose.Ori();}
	///	向きの設定
	void		SetOrientation(const Quaterniond& q){
		pose.Ori() = q;
	}

	///	質量中心の速度の取得
	Vec3d		GetVelocity() const {return velocity;}
	///	質量中心の速度の設定
	void		SetVelocity(const Vec3d& v){
		velocity = v;
		SetFrozen(false);
	}

	///	角速度の取得
	Vec3d		GetAngularVelocity() const {return angVelocity;}
	///	角速度の設定
	void		SetAngularVelocity(const Vec3d& av){
		angVelocity = av;
		SetFrozen(false);
	}

	///	任意の位置での速度の取得
	Vec3d		GetPointVelocity(Vec3d posW) const {
		return velocity + (angVelocity^(posW - pose*center));
	}

	///	質量中心の速度と角速度をまとめて取得
	SpatialVector GetSpatialVelocity() const { return SpatialVector(velocity, angVelocity); }

	///	（最後のStep()での）剛体の質量中心の加速度
	SpatialVector GetAcceleration() const;

	///	shapeの数。
	int			NFrame();
	///	shapeを、位置指定込みで追加する．
	void		AddFrame(PHFrameIf* frame);	
	///	
	void		DelFrame(int i);
	///	frameで取得
	PHFrameIf*	GetFrame(int i);

	///	この剛体が持つ Spr::CDShape の数
	int			NShape();
	///	この剛体が持つ i番目の SPR::CDShape の取得
	CDShapeIf*	GetShape(int i);
	///	shape を この剛体が持つSpr::CDShapeのリスト の最後に追加する．
	void		AddShape(CDShapeIf* shape);
	/// i番目のshapeを削除
	void		RemoveShape(int i);
	///	この剛体が持つshapeを全て削除
	void		RemoveShape(CDShapeIf* shape);
	///	この剛体が持つ i番目の SPR::CDShape のこの剛体から見た姿勢を取得
	Posed		GetShapePose(int i);
	///	この剛体が持つ i番目の SPR::CDShape のこの剛体から見た姿勢を設定
	void		ClearShape();
	/// この剛体が持つSPR::CDShape を削除
	void		SetShapePose(int i, const Posed& pose);
	/// 重力を加えるかどうか設定
	void		SetGravity(bool bOn);
	/// 物理法則に従うかどうか設定
	void		SetDynamical(bool bOn){dynamical = bOn;}
	/// 物理法則に従っているかどうかを取得
	bool		IsDynamical(){return dynamical;}
	/// 速度が一定以下の時，積分を行わないように設定
	void		SetFrozen(bool bOn){bFrozen = bOn;}
	/// 速度が一定以下で積分を行わないかどうかを取得
	bool		IsFrozen(){return bFrozen;}
	PHTreeNodeIf* GetTreeNode();

protected:
	virtual void AfterSetDesc();
};

typedef std::vector< UTRef<PHSolid> >	PHSolidRefs;
typedef std::vector< PHSolid* >			PHSolids;

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

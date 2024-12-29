/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHBODY_H
#define PHBODY_H

#include <Physics/SprPHSolid.h>
#include "../Foundation/Object.h"

namespace Spr{;

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
	Vec3f GetBBoxCenter() const { return bboxCenter; }
	///	大きさ
	Vec3f GetBBoxExtent() const { return bboxExtent; }
	///	小さい端点
	Vec3f GetBBoxMin() const { return bboxCenter-bboxExtent; }
	///	大きい端点
	Vec3f GetBBoxMax() const { return bboxCenter+bboxExtent; }

	///	与えられたベクトルとの内積が最大と最小の点
	void GetSupport(const Vec3f& dir, float& minS, float& maxS);
	
	///	SolidのPoseを代入することで，world座標系の最小値,最大値を取得
	/// (注意）バウンディングボックスよりも大きなボックスで判定されてしまう．
	//
	void GetBBoxWorldMinMax(Posed& pose, Vec3d& _min, Vec3d& _max);

	/// 交差判定
	static bool Intersect(PHBBox& lhs, PHBBox& rhs);
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

class PHBody;
class CDShape;

struct PHFrameState {
	Posed			pose_abs;	///< ワールドに対する形状フレームの位置と向き
	Vec3d			delta;		///< 形状フレームの移動量
	DUMPLABEL(PHFrame_bboxReady)
	bool            bboxReady;
	PHBBox			bbShape;		///< 形状座標のBBox
	PHBBox			bbBody;			///< 物体座標のBBox
	DUMPLABEL(PHFrame_bbWorld)
	PHBBox			bbWorld[2];		///< ワールド座標のBBox(0:非CCD 1:CCD)
};

/// Bodyに取り付けられた形状の座標系
class PHFrame : public SceneObject, public PHFrameDesc, public PHFrameState {
public:
	PHBody*			body;
	CDShape*		shape;

	double			mass;
	Vec3d			center;
	Matrix3d		inertia;
public:
	SPR_OBJECTDEF(PHFrame);
	ACCESS_DESC_STATE(PHFrame);

	PHFrame();
	PHFrame(PHBody* so, CDShape* sh);
	PHFrame(const PHFrameDesc& desc);

	bool      CalcBBox();
	void      CalcAABB();
	void      CompInertia();

	// インタフェースの実装
	Posed     GetPose();
	void      SetPose(Posed p);
	CDShapeIf* GetShape() { return (CDShapeIf*)shape; }

	// Objectの仮想関数
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual bool      AddChildObject(ObjectIf* o);
	virtual bool      DelChildObject(ObjectIf* o);
	virtual size_t    NChildObject() const;
};


typedef std::vector< PHBody* >			PHBodies;


struct PHBodyState {
	DUMPLABEL(PHBodyBegin);
	//	int                 id;		//< なぞ、たぶん不要。誰が足したの？
	bool				bboxReady;			///< bboxの再計算用フラグ
	bool                aabbReady;			///< aabbの再計算用フラグ
	PHBBox              bbLocal;			///< ローカル座標のBBox
	PHBBox              bbWorld;			///< ワールド座標のBBox
	DUMPLABEL(PHBodyEnd);
};
///	物体
class PHBody : public SceneObject, public PHBodyState {
public:
	SPR_OBJECTDEF_ABST(PHBody);
	ACCESS_STATE(PHBody);
	std::vector< UTRef<PHFrame> > frames;	///< Collision形状


	///	位置と向き
	virtual Posed GetPose() = 0;
	///	重心位置(回転中心の位置)(Local)
	virtual Vec3d GetCenterOfMass() = 0;
	///	フレーム位置の取得
	virtual Vec3d GetFramePosition() = 0;
	/// 重心位置(回転中心)の取得(World)
	Vec3d GetCenterPosition() { return GetPose()*GetCenterOfMass(); }
	///	速度
	virtual Vec3d GetVelocity() = 0;
	///	角速度
	virtual Vec3d GetAngularVelocity() = 0;
	///	任意の位置での速度の取得
	Vec3d GetPointVelocity(Vec3d posW) {
		return GetVelocity() + (GetAngularVelocity() ^ (posW - GetPose()*GetCenterOfMass()));
	}

	/// 速度が一定以下の時，積分を行わないように設定
	virtual void SetFrozen(bool bOn) {}
	/// 速度が一定以下で積分を行わないかどうかを取得
	virtual bool IsFrozen() { return false; }
	///	動力学シミュレーションの対象かどうか
	virtual bool IsDynamical() { return true; }
	virtual bool InvalidateBbox() { bool rv = bboxReady; bboxReady = false; return rv; }

	virtual bool CalcBBox()=0;						///< 剛体と各形状のローカルBBoxを計算
	virtual void CalcAABB()=0;						///< 剛体と各形状のワールドBBoxを計算
	virtual void GetBBoxSupport(const Vec3f& dir, float& minS, float& maxS)=0;
	virtual void GetBBox(Vec3d& bbmin, Vec3d& bbmax, bool world)=0;

	///	shapeの数。
	int			NFrame();
	///	shapeを、位置指定込みで追加する．
	void		AddFrame(PHFrameIf* frame);
	///	
	//void		DelFrame(int i);
	///	frameで取得
	PHFrameIf*	GetFrame(int i);

	///	この剛体が持つ Spr::CDShape の数
	int			NShape();
	///	この剛体が持つ i番目の SPR::CDShape の取得
	CDShapeIf*	GetShape(int i);
	///	shape を この剛体が持つSpr::CDShapeのリスト の最後に追加する．
	void		AddShape(CDShapeIf* shape);
	void		AddShapes(CDShapeIf** shBegin, CDShapeIf** shEnd);
	/// i番目のshapeを削除
	void		RemoveShape(int i);
	/// iBegin番目からiEnd-1番目までを削除
	void        RemoveShapes(int iBegin, int iEnd);
	///	この剛体が持つshapeを全て削除
	void		RemoveShape(CDShapeIf* shape);
	///	この剛体が持つ i番目の SPR::CDShape のこの剛体から見た姿勢を取得
	Posed		GetShapePose(int i);
	///	この剛体が持つ i番目の SPR::CDShape のこの剛体から見た姿勢を設定
	void		ClearShape();
	/// この剛体が持つSPR::CDShape を削除
	void		SetShapePose(int i, const Posed& pose);

};

}	//	namespace Spr
#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDCONVEXMESH_H
#define CDCONVEXMESH_H

#include "../Foundation/Object.h"
#include "CDConvex.h"
#include <vector>

namespace Spr{;

class CDVertexIDs:public std::vector<int>{
public:
	int FindPos(int id) const;
};

/**	凸多面体の面を表すクラス．CDConvexMesh が所有．
	CDContactAnalysis で使われる．
*/
class CDFace:public Object{
public:
	SPR_OBJECTDEF(CDFace);
	int      vtxs[3];		///< 面の頂点ID
	Vec3f    normal;		///< 法線

	/// CDFaceの面のインデックス数
	virtual int NIndex(){ return 3; }
	/// CDFaceのインデックス配列を取得
	virtual int* GetIndices(){ return vtxs; }
};

class CDFaces:public std::vector<CDFace>{

};

///	凸多面体
class CDConvexMesh : public CDConvex{
protected:
	/// 全頂点の平均
	Vec3f average;
	/// 体積
	float volume;
	/// 重心
	Vec3f center;
	/// 慣性行列
	Matrix3f inertia;

public:
	SPR_OBJECTDEF(CDConvexMesh);
	//	Descのメンバ、SPR_DECLMEMBEROF_CDConvexMeshDesc は使わない。代わりにGetDesc, SetDesc, GetDescSizeを使う

	///	探索開始頂点番号
	mutable int curPos;
	///	頂点の座標(ローカル座標系)
	std::vector<Vec3f> base;

	///	頂点の隣の頂点の位置が入った配列
	std::vector< std::vector<int> > neighbor;
	///	面(3角形 0..nPlanes-1 が独立な面，それ以降はMargeFaceで削除される同一平面上の面)
	CDFaces faces;
	///	面(3角形のうち，MergeFace()で残った数)
	int nPlanes;

public:
	CDConvexMesh();
	CDConvexMesh(const CDConvexMeshDesc& desc);

	///	頂点から面や接続情報を生成する．
	void CalcFace();
	/// 体積, 重心, 慣性行列の計算
	void CalcMetric();

	/// CDShapeの仮想関数
	virtual float    CalcVolume();
	virtual Vec3f    CalcCenterOfMass();
	virtual Matrix3f CalcMomentOfInertia();
	virtual bool     IsInside(const Vec3f& p);
	virtual int      LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset);
	
	virtual int               Support(Vec3f& w, const Vec3f& v) const;
	virtual bool              FindCutRing(CDCutRing& r, const Posed& toW);
	virtual std::vector<int>& FindNeighbors(int vtx);
	virtual Vec3f*            GetBase() { return &*base.begin(); };

	virtual int		GetVtxCount() const { return base.size(); };

	CDFaceIf* GetFace(int i);
	int       NFace();
	Vec3f*    GetVertices();
	int       NVertex();

	///	デスクリプタCDConvexMeshDescの読み書き	
	virtual bool GetDesc(void *desc) const;
	virtual void SetDesc(const void* desc);
	virtual size_t GetDescSize() const { return sizeof(CDConvexMeshDesc); }

	virtual void Print(std::ostream& os) const;

protected:
	///	同一平面上で接続されている3角形をマージする
	void MergeFace();
	/// 面の法線を計算(Inside用)
	void CalcFaceNormals();

	///	平均座標を計算する。
	void CalcAverage();

};

}	//	namespace Spr
#endif

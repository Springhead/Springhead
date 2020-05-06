/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDBOX_H
#define CDBOX_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include <Collision/CDConvex.h>

namespace Spr{;

/**	立方体の面(四角形)を表すクラス．CDBoxが所有．
	CDContactAnalysis で使われる．
*/
class CDQuadFace: public Object{
public:
	SPR_OBJECTDEF(CDQuadFace);
	int vtxs[4];	///< 面の頂点ID
	Vec3f normal;	///< 面の法線

	/// CDQuadFaceの面のインデックス数
	virtual int NIndex(){ return 4; }
	/// CDQuadFaceインデックス配列を取得
	virtual int* GetIndices(){ return vtxs; }
};

class CDQuadFaces:public std::vector<CDQuadFace>{
};

/// 直方体
class CDBox: public CDConvex{
public:
	mutable int curPos;
	SPR_OBJECTDEF(CDBox);
	SPR_DECLMEMBEROF_CDBoxDesc;

	std::vector<Vec3f> base;				///< 頂点の座標(ローカル座標系)
	static CDQuadFaces qfaces;				///< 面（四角形:quadrangular face）
	static std::vector<int> neighbor[8];	///< 頂点の隣の点
	
	CDBox();
	CDBox(const CDBoxDesc& desc);
	
	virtual bool IsInside(const Vec3f& p);
	virtual float CalcVolume();
	virtual Matrix3f CalcMomentOfInertia();
	
	///	サポートポイントを求める．
	int Support(Vec3f& w, const Vec3f& v) const ;
	///	切り口を求める．接触解析に使う．
	/// ただし、球体に関しては、切り口は求めない。接触解析時には最近傍の１点さえあればいい。
	/// 球体に関してFindCutRing()が呼び出された場合には、assertionが発生する。
	virtual bool FindCutRing(CDCutRing& r, const Posed& toW);
	///	指定の頂点 vtx の隣の頂点番号を返す
	virtual std::vector<int>& FindNeighbors(int vtx);
	///	頂点バッファを返す。
	virtual Vec3f* GetBase(){return &*base.begin();}	
	
	/// 直方体のサイズを取得
	Vec3f GetBoxSize();
	Vec3f* GetVertices();
	CDFaceIf* GetFace(int i);

	/// 直方体のサイズを設定
	Vec3f SetBoxSize(Vec3f boxSize);

	/// 内部情報の再計算
	void Recalc();

	virtual int LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset);
};
	

}	//	namespace Spr

#endif	// CDBOX_H

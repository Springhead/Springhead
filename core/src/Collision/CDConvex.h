/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDCONVEX_H
#define CDCONVEX_H

#include "../Foundation/Object.h"
#include "CDShape.h"
#include "CDCutRing.h"
#include "CollisionDecl.hpp"

namespace Spr{;

class CDConvex : public CDShape{
public:
	SPR_OBJECTDEF_ABST(CDConvex);

	/** ボクセル近似による体積，重心，慣性行列の計算
		使用していないが比較のために残してある
	 */
	struct Boxel{
		float		volume;
		Vec3f		center;
		Matrix3f	inertia;

		Boxel(float v, const Vec3f& c, const Matrix3f& I):volume(v), center(c), inertia(I){}
	};
	std::vector<Boxel> boxels;
	float   maxSurfArea;
	bool	bboxReady;
	
	void	AccumulateBoxels(const Vec3f& bbmin, const Vec3f& bbmax, float eps);
	void	CalcMetricByBoxel(float& volume, Vec3f& center, Matrix3f& inertia);

	/**	サポートポイント(方向ベクトルvとの内積が最大の点)をwに格納する。
		戻り値には、頂点番号があれば返す。無ければ-1。頂点番号はメッシュの頂点の場合
		のほか、RoundConeなどの球の中心の番号のこともある	*/
	virtual int Support(Vec3f&w, const Vec3f& v) const =0;
	///	切り口を求める．接触解析に使う．
	virtual bool FindCutRing(CDCutRing& r, const Posed& toW) =0;
	///	頂点vtx のとなりの頂点を列挙する。
	virtual std::vector<int>& FindNeighbors(int vtx);
	///	頂点バッファを返す。
	virtual Vec3f* GetBase(){return NULL;}

	// 頂点数を返す
	virtual int GetVtxCount() const = 0;
	virtual float GetMaxSurf() { return maxSurfArea; };
	///	バウンディングボックスを求める．
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose);
	/// 表面上の点pにおける曲率半径を求める
	virtual double CurvatureRadius(Vec3d p){ return 1e+10; } /// 平面とした場合の値。現時点ではRoundConeについてのみ実装されている(09/02/08, mitake)
	///< 表面上の点pにおける法線
	virtual Vec3d Normal(Vec3d p){ return Vec3d(); } /// 現時点ではRoundConeについてのみ実装されている(09/02/14, mitake)

	CDConvex();
};

}	//	namespace Spr
#endif

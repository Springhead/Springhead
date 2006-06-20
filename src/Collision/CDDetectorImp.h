/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDDETECTORIMP_H
#define CDDETECTORIMP_H
#include "CDQuickHull3D.h"
#include "CDConvexMesh.h"
#include "CDSphere.h"
#include "CDBox.h"

namespace Spr {;

#define CD_EPSILON	HULL_EPSILON
#define CD_INFINITE	HULL_INFINITE

class CDFace;
class CDConvex;

/// Shapeの組の状態
struct CDShapePairState{
	unsigned lastContactCount;
	Vec3d normal;				///<	衝突の法線(0から1へ) (Global)
	double depth;				///<	衝突の深さ：最近傍点を求めるために，2物体を動かす距離．
	//CDShapePairState(const CDShapePair& s):
	//	lastContactCount(s.lastContactCount),normal(s.normal), depth(s.depth){}
};
class CDShapePair: public CDShapePairState, public UTRefCount{
public:
	CDConvex* shape[2];			// 判定対象の形状
	Posed shapePoseW[2];		// World系での形状の姿勢のキャッシュ

	//	接触判定結果
	Vec3d	closestPoint[2];		///< 最近傍点（ローカル座標系）
	Vec3d	commonPoint;			///< 交差部分の内点（World座標系）
	bool	bActive;				///< 現在接触が生じているか
	unsigned lastContactCount;
	enum State{
		NEW,
		CONTINUE,
	} state;
	Vec3d iNormal;				///<	積分による法線
	Vec3d center;				///<	2つの最侵入点の中間の点，CDContactAnalysis::CalcNormal が更新する．

public:
	//virtual void Clear();
	void SetState(const CDShapePairState& s){
		(CDShapePairState&)*this = s;
	}

	///	形状の姿勢(shapePoseW)の更新．各Shapeの親Solidの姿勢を渡す．
	///void UpdateShapePose(Posed pose0, Posed pose1);
	
	///	接触判定
	bool Detect(unsigned ct, CDConvex* s0, CDConvex* s1, const Posed& pose0, const Posed& pose1);
	
};

///	BBox同士の交差判定．交差していれば true．
bool BBoxIntersection(Posed postureA, Vec3f centerA, Vec3f extentA,
					  Posed postureB, Vec3f centerB, Vec3f extentB);

///	GJKで共有点を見つける
bool FindCommonPoint(const CDConvex* a, const CDConvex* b,
					 const Posed& a2w, const Posed& b2w,
					 Vec3d& v, Vec3d& pa, Vec3d& pb);

/// GJKで最近傍点対を見つける
void FindClosestPoints(const CDConvex* a, const CDConvex* b,
					   const Posed& a2w, const Posed& b2w,
					   Vec3d& pa, Vec3d& pb);

class CDFace;


///	ContactAnalysisが使用する凸多面体の面を表す．
class CDContactAnalysisFace{
public:
	class DualPlanes: public std::vector<CDQHPlane<CDContactAnalysisFace>*>{};

	CDFace* face;	///<	面を双対変換した頂点でQuickHullをするので，CDFaceがVtxs.
	int id;			///<	どちらのSolidの面だか表すID．

	//@group CDContactAnalysis が交差部分の形状を求める際に使う作業領域
	//@{
	Vec3f normal;	///<	面の法線ベクトル
	float dist;		///<	原点からの距離
	/**	QuickHullアルゴリズム用ワークエリア．
		一番遠い頂点から見える面を削除したあと残った形状のエッジ部分
		を一周接続しておくためのポインタ．
		頂点→面の接続．	面→頂点は頂点の並び順から分かる．	*/
	CDQHPlane<CDContactAnalysisFace>* horizon;
	//@}
	
	///	QuickHullにとっての頂点．この面を双対変換してできる頂点
	Vec3f GetPos() const { return normal / dist; }
	/**	双対変換を行う．baseに渡す頂点バッファは，双対変換が可能なように
		双対変換の中心が原点となるような座標系でなければならない．	*/
	bool CalcDualVtx(Vec3f* base);

	/**	この面を双対変換してできる頂点を含む面．
		つまり，交差部分の形状を構成する頂点のうち，
		この面にあるもの	*/
	DualPlanes dualPlanes;
	///	交差部分の形状を構成する頂点のうちこの面にあるものの数.
	size_t NCommonVtx(){ return dualPlanes.size(); }
	///	交差部分の形状を構成する頂点のうちこの面にあるもの.
	Vec3f CommonVtx(int i);
	///	デバッグ用表示
	void Print(std::ostream& os) const;
};
inline std::ostream& operator << (std::ostream& os, const CDContactAnalysisFace& f){
	f.Print(os);
	return os;
}

/**	交差部分の解析をするクラス．(交差部分の形状を求める/初回の法線を積分で求める)	*/
class CDContactAnalysis{
public:
	/// \defgroup quickHull QuickHullのための頂点と平面
	//@{
	
	typedef std::vector<CDContactAnalysisFace*> Vtxs;	
	static Vtxs vtxs;					///<	QuickHullの頂点
	typedef std::vector<CDContactAnalysisFace> VtxBuffer;	
	static VtxBuffer vtxBuffer;

	static CDQHPlanes<CDContactAnalysisFace> planes;	///<	面
	bool isValid;						///<	交差部分のポリゴンは有効？
	//@}
	std::vector<Vec3f> tvtxs[2];		///<	対象の2つの凸多面体のCommonPoint系での頂点の座標
	/**	共通部分の形状を求める．
		結果は，共通部分を構成する面を vtxs.begin() から返り値までに，
		共通部分を構成する頂点を， planes.begin から planes.end のうちの
		deleted==false のものに入れて返す．
		cp の shapePoseW に shape[0], shape[1]の頂点をWorld系に変換する
		変換行列が入っていなければならない．	*/
	CDContactAnalysisFace** FindIntersection(CDShapePair* cp);
	/**	交差部分の形状の法線を積分して，衝突の法線を求める．
		物体AとBの衝突の法線は，交差部分の面のうち，Aの面の法線の積分
		からBの面の法線の積分を引いたものになる．	*/
	void IntegrateNormal(CDShapePair* cp);
	/**	法線の計算．前回の法線の向きに物体を動かし，
		物体を離して最近傍点を求める．	*/
	void CalcNormal(CDShapePair* cp);
};

}
#endif

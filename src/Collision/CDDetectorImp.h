#ifndef CDDETECTORIMP_H
#define CDDETECTORIMP_H
#include "CDQuickHull3D.h"

namespace Spr {;

#define CD_EPSILON	HULL_EPSILON
#define CD_INFINITE	HULL_INFINITE

class CDFace;
class CDConvex;
class CDConvexPair;
class CDConvexPairWithNormal;
class SGScene;

///	BBox同士の交差判定．交差していれば true．
bool BBoxIntersection(Affinef postureA, Vec3f centerA, Vec3f extentA,
					  Affinef postureB, Vec3f centerB, Vec3f extentB);

///	GJKで共有点を見つける
bool FindCommonPoint(const CDConvex& a, const CDConvex& b,
					 const Affinef& a2w, const Affinef& b2w,
					 Vec3d& v, Vec3d& pa, Vec3d& pb);

/// GJKで最近傍点対を見つける
void FindClosestPoints(const CDConvex& a, const CDConvex& b,
					   const Affinef& a2w, const Affinef& b2w,
					   Vec3d& pa, Vec3d& pb);

/**	衝突判定結果(交叉部分の形状を求める機能/法線を求める機能付き)	*/
class CDContactAnalysis{
public:
	/// \addtogroup quickHull QuickHullのための頂点と平面
	//@{
	///	面を双対変換した頂点でQuickHullをするので，CDFaceがVtxs.
	typedef std::vector<CDFace*> Vtxs;	
	static Vtxs vtxs;					///<	QuickHullの頂点
	static CDQHPlanes<CDFace> planes;	///<	面
	bool isValid;						///<	交差部分のポリゴンは有効？
	//@}
	/**	共通部分の形状を求める．
		結果は，共通部分を構成する面を vtxs.begin() から返り値までに，
		共通部分を構成する頂点を， planes.begin から planes.end のうちの
		deleted==false のものに入れて返す．
		convex[0], convex[1]の頂点をWorld系に変換する変換行列を
		af0, af1 に渡さなければならない．	*/
	CDFace** FindIntersection(CDConvexPair& cp, Affinef* af);
	/**	交差部分の形状の法線を積分して，衝突の法線を求める．
		物体AとBの衝突の法線は，交差部分の面のうち，Aの面の法線の積分
		からBの面の法線の積分を引いたものになる．	*/
	void IntegrateNormal(CDConvexPairWithNormal& cp, Affinef* af);
	/**	法線の計算．前回の法線の向きに物体を動かし，
		物体を離して最近傍点を求める．	*/
	void CalcNormal(CDConvexPairWithNormal& cp,  Affinef* af);
	/**	共通点と交差部分の形状の描画	*/
	void Draw(CDConvexPair& cp, Affinef afw, SGScene* s);
};

}
#endif

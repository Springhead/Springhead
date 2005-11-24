#ifndef CDDETECTORIMP_H
#define CDDETECTORIMP_H
#include "CDQuickHull3D.h"

namespace Spr {;

#define CD_EPSILON	HULL_EPSILON
#define CD_INFINITE	HULL_INFINITE

class CDFace;
class CDConvex;

class CDShapePair: public UTRefCount{
public:
	//	判定対象のShape
	CDShape* shape[2];
	//	接触判定結果
	Vec3d closestPoint[2]; // ローカル座標系
	Vec3d commonPoint;     // World座標系
	unsigned int lastContactCount;
	enum State{
		NEW,
		CONTINUE,
	} state;
	//	法線計算結果
	Vec3d normal;				///<	衝突の法線(0から1へ) (Global)
	Vec3d iNormal;				///<	積分による法線
	Vec3d center;				///<	2つの最侵入点の中間の点，CDContactAnalysis::CalcNormal が更新する．
	double depth;				///<	衝突の深さ：最近傍点を求めるために，2物体を動かす距離．

	//	World系での形状の姿勢をキャッシュ
	Posed shapePoseW[2];

	///	
	CDShapePair(CDShape* s0, CDShape* s1){
		shape[0] = s0;
		shape[1] = s1;
	}
	///	形状の姿勢(shapePoseW)の更新．各Shapeの親Solidの姿勢を渡す．
	void UpdateShapePose(Posed pose0, Posed pose1);
	///	接触判定
	bool Detect(unsigned ct);
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
		cp の shapePoseW に shape[0], shape[1]の頂点をWorld系に変換する
		変換行列が入っていなければならない．	*/
	CDFace** FindIntersection(CDShapePair* cp);
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

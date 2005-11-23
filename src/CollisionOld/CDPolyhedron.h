#ifndef CDPOLYHEDRON_H
#define CDPOLYHEDRON_H
#include "CDGeometry.h"
#include "CDConvex.h"
#include "CDQuickHull3D.h"

namespace Spr {;

class CDPolyhedron;
class CDFace{
public:
	class DualPlanes: public std::vector<CDQHPlane<CDFace>*>{
	public:
	};
	int vtxs[3];	///< 面の頂点

	//	交差部分の形状を求める計算のための作業領域
	Vec3f normal;	///<	面の法線ベクトル
	float dist;		///<	原点からの距離
	///	QuickHullにとっての頂点．この面を双対変換してできる頂点
	Vec3f GetPos() const { return normal / dist; }
	///
	/**	QuickHullアルゴリズム用ワークエリア．
		一番遠い頂点から見える面を削除したあと残った形状のエッジ部分
		を一周接続しておくためのポインタ．
		頂点→面の接続．	面→頂点は頂点の並び順から分かる．	*/
	CDQHPlane<CDFace>* horizon;

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
inline std::ostream& operator << (std::ostream& os, const CDFace& f){
	f.Print(os);
	return os;
}
class CDFaces:public std::vector<CDFace>{
};
class CDVertexIDs:public std::vector<int>{
public:
	int FindPos(int id) const;
};

/**	3角形だけから構成される凸多面体	*/
class CDPolyhedron:public CDConvex{
public:
	SGOBJECTDEF(CDPolyhedron);
	///	探索開始頂点番号
	mutable int curPos;
	///	GeomtryID
	virtual int GeometryID(){ return CONVEX; }
	///	頂点の座標(ローカル座標系)
	Vec3f* base;
	///	作業領域：頂点の座標(変換後の座標系)
	Vec3f* tvtxs;

	///	この凸多面体を構成する頂点たち．posはこの配列内の位置を表す．
	CDVertexIDs vtxIDs;
	///	頂点の隣の頂点の位置が入った配列
	std::vector< std::vector<int> > neighbor;
	///	面(3角形 0..nPlanes-1 が独立な面，それ以降はMargeFaceで削除される同一平面上の面)
	CDFaces faces;
	///	面(3角形のうち，MergeFace()で残った数)
	int nPlanes;

	///	頂点の座標を返す．
	Vec3f Vertex(int pos) const { return base[vtxIDs[pos]]; }

	///	頂点から面や接続情報を生成する．
	void CalcFace();
	///	同一平面上で接続されている3角形をマージする
	void MergeFace();
	
	///	面を持っているかどうか調べる
	bool HasFace(CDFace* face){
		return &*faces.begin() <= face && face <= &*faces.end();
	}
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const;
private:
	bool VertexNear(int v1, int v2) const;
};

}
#endif

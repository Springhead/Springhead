#ifndef CDCONVEXMESH_H
#define CDCONVEXMESH_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include "CDConvex.h"

namespace Spr{;

class CDVertexIDs:public std::vector<int>{
public:
	int FindPos(int id) const;
};

/**	凸多面体の面を表すクラス．CDConvexMesh が所有．
	CDContactAnalysis で使われる．
*/
class CDFace: public CDFaceIf{
public:
	int vtxs[3];	///< 面の頂点ID

	///
	virtual int GetNIndices(){ return 3; }
	///
	virtual int* GetIndices(){ return vtxs; }
};

class CDFaces:public std::vector<CDFace>{
};

class CDConvexMesh : public CDConvexMeshIf, public CDConvex{
public:
	OBJECTDEF(CDConvexMesh);
	BASEIMP_CDCONVEX(CDConvex);

	///	探索開始頂点番号
	mutable int curPos;
	///	この凸多面体を構成する頂点たち．posはこの配列内の位置を表す．
	CDVertexIDs vtxIDs;
	///	頂点の座標(ローカル座標系)
	std::vector<Vec3f> base;

	///	頂点の隣の頂点の位置が入った配列
	std::vector< std::vector<int> > neighbor;
	///	面(3角形 0..nPlanes-1 が独立な面，それ以降はMargeFaceで削除される同一平面上の面)
	CDFaces faces;
	///	面(3角形のうち，MergeFace()で残った数)
	int nPlanes;

	CDConvexMesh();
	CDConvexMesh(const CDConvexMeshDesc& desc);

	///	ShapeType
	virtual int ShapeType(){ return CDShapeDesc::CONVEXMESH; }
	///	頂点の座標を返す．
	Vec3f Vertex(int pos) const { return base[pos]; }

	///	頂点から面や接続情報を生成する．
	void CalcFace();
	///	同一平面上で接続されている3角形をマージする
	void MergeFace();
	
	///	面を持っているかどうか調べる
/*	bool HasFace(CDFace* face){
		return &*faces.begin() <= face && face <= &*faces.end();
	}
*/	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const;
private:
	bool VertexNear(int v1, int v2) const;

	virtual void Print(std::ostream& os) const;

	CDFaceIf* GetFace(size_t i);
	size_t GetNFaces();
	Vec3f* GetVertices();
	size_t GetNVertices();
};

}	//	namespace Spr
#endif

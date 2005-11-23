#ifndef CDMESH_H
#define CDMESH_H
#include "CDConvex.h"

namespace Spr {;

/**	メッシュ	*/
class CDMesh:public CDGeometry{
	SGOBJECTDEF(CDMesh);
public:
	///	GeomtryID
	virtual int GeometryID(){ return MESH; }
	///	頂点の座標(ローカル系)
	std::vector<Vec3f> vertices;
	///	頂点の座標(作業領域)
	std::vector<Vec3f> tvtxs;
	///	凸形状	本当は Tree にしたい．
	CDGeometries conveces;
	///	ツリーの作成
	void CreateTree();
	///
	void MergeVertices();
	///
	virtual void CalcBBox(Vec3f& bbMin, Vec3f& bbMax);
	///	
	virtual bool AddChildObject(SGObject* o, SGScene* s);
};



}
#endif

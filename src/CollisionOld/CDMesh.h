/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDMESH_H
#define CDMESH_H
#include "CDConvex.h"

namespace Spr {;

/**	メッシュ	*/
class CDMesh:public CDGeometry{
	SGOBJECT_DEF(CDMesh);
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

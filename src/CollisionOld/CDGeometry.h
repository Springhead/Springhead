/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDGEOMETRY_H
#define CDGEOMETRY_H

namespace Spr {;

class CDConvex;
class CDMesh;
class CDFace;

/**	衝突判定対象のID	*/
enum CDGeometryID{
	GEOMETRY=0,
	MESH,
	CONVEX,
	WATER,
	NGEOMETRYID,
};

class CDPhysicalMaterial;

/**	衝突判定のための形状データ
*/
class SPR_DLL CDGeometry:public SGObject{
public:
	SGOBJECT_DEF_ABST(CDGeometry);
	///
	virtual int GeometryID()=0;
	///
	virtual void CalcBBox(Vec3f& bbMin, Vec3f& bbMax){}
	///	物理マテリアル
	UTRef<CDPhysicalMaterial> pmaterial;
	///	
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///
	virtual size_t NChildObjects(){ return pmaterial ? 1 : 0; }
	///
	virtual SGObject* ChildObject(size_t i);
};
/// Geometryの配列
class SPR_DLL CDGeometries:public std::vector< UTRef<class CDGeometry> >{
public:
};

}
#endif

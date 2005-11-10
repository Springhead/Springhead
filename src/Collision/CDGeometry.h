#ifndef CDGEOMETRY_H
#define CDGEOMETRY_H
#include <SceneGraph/SGObject.h>

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
	SGOBJECTDEFABST(CDGeometry);
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

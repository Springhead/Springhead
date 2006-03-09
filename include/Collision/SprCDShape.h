#ifndef SPR_CDSHAPEIF_H
#define SPR_CDSHAPEIF_H
#include <Foundation/Object.h>

namespace Spr{;
/*	memo
	shapeに階層構造を持たせるかどうか．
	Novodex ODEとも 階層構造は持っていない．
	階層を動かすときには，重心や 慣性モーメントも書き換えなければならないので，
	そんなに使わないという判断だと思う．
	少なくとも，シミュレーション中に階層で動かすのは困ると思う．

	なので，階層は持たないという方針で作る．

	Solid は複数の Shape を持つことができる．
	Shape は階層構造は持たない．

	階層構造は物理ライブラリとは別に，シーングラフライブラリが持つ
*/

struct CDShapeIf : public NamedObjectIf{
	IF_DEF(CDShape);
};

struct CDConvexIf : public CDShapeIf{
	IF_DEF(CDConvex);
};

/**	面	*/
struct CDFaceIf{
	virtual int NIndex()=0;
	virtual int* GetIndices()=0;
protected:
	virtual ~CDFaceIf() {};
};



struct PHMaterial{
	PHMaterial(){
		mu = mu0 = e = 0.2f;
	}
	float mu;		///<	動摩擦摩擦係数
	float mu0;		///<	静止摩擦係数
	float e;		///<	跳ね返り係数
};

struct CDShapeDesc{
	enum ShapeType{
		CONVEX,
		CONVEXMESH,
	} type;
	CDShapeDesc(ShapeType t=CONVEXMESH): type(t){}
};


/**	凸形状のメッシュの生成用
*/
struct CDConvexMeshDesc: public CDShapeDesc{
	CDConvexMeshDesc():CDShapeDesc(CONVEXMESH){}
	std::vector<Vec3f> vertices;	///<	頂点の座標
	PHMaterial material;			///<	材質
};
/**
	凸形状のメッシュ	頂点を追加
*/
struct CDConvexMeshIf: public CDConvexIf{
	IF_DEF(CDConvexMesh);
	virtual CDFaceIf* GetFace(size_t i)=0;
	virtual size_t NFace()=0;
	virtual Vec3f* GetVertices()=0;
	virtual size_t NVertex()=0;
};

}

#endif

/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CDSHAPEIF_H
#define SPR_CDSHAPEIF_H
#include <Foundation/SprObject.h>

namespace Spr{;

/**	\defgroup	gpShape	形状・接触判定クラス	*/	
//@{
/**	@file SprCDShape.h
	剛体の形状
 */	

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

///	形状の基本クラス
struct CDShapeIf : public NamedObjectIf{
	IF_DEF(CDShape);
};

///	凸形状の基本クラス
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


///	物理シミュレーションに関係する材質
struct PHMaterial{
	PHMaterial(){
		mu = mu0 = e = 0.2f;
	}
	float mu;				///< 動摩擦摩擦係数
	float mu0;				///< 静止摩擦係数
	float e;				///< 跳ね返り係数
};

///	形状のディスクリプタ(基本クラス)
struct CDShapeDesc{
	PHMaterial material;	///<	材質
};


/**	凸形状のメッシュ*/
struct CDConvexMeshIf: public CDConvexIf{
	IF_DEF(CDConvexMesh);
	virtual CDFaceIf* GetFace(size_t i)=0;
	virtual size_t NFace()=0;
	virtual Vec3f* GetVertices()=0;
	virtual size_t NVertex()=0;
};
/**	凸形状のメッシュのディスクリプタ	*/	
struct CDConvexMeshDesc: public CDShapeDesc{
	DESC_DEF_FOR_OBJECT(CDConvexMesh);
	CDConvexMeshDesc():CDShapeDesc(){}
	std::vector<Vec3f> vertices;	///<	頂点の座標
};
	
/** 球体　*/
struct CDSphereIf: public CDConvexIf{
	IF_DEF(CDSphere);
	virtual float GetRadius()=0;
};	
/** 球体のディスクリプタ　*/
struct CDSphereDesc: public CDShapeDesc{
	DESC_DEF_FOR_OBJECT(CDSphere);
	CDSphereDesc():CDShapeDesc(){
		radius = 1.0f;
	}
	float radius;					// 球体の半径
};	

/** カプセル　*/
struct CDCapsuleIf: public CDConvexIf{
	IF_DEF(CDCapsule);
	virtual float GetRadius()=0;
	virtual float GetLength()=0;
};	
/** カプセルのディスクリプタ　*/
struct CDCapsuleDesc: public CDShapeDesc{
	DESC_DEF_FOR_OBJECT(CDCapsule);
	CDCapsuleDesc():CDShapeDesc(){
		radius = 1.0f;
		length = 1.0f;
	}
	float radius;					///< カプセルの球の半径
	float length;					///< カプセルの長さ Z軸向きが長い
};	

	
/** 直方体 */
struct CDBoxIf: public CDConvexIf{
	IF_DEF(CDBox);
	virtual Vec3f GetBoxSize()=0;
	virtual Vec3f* GetVertices()=0;
	virtual CDFaceIf* GetFace(size_t i)=0;
};
/** 直方体のディスクリプタ */
struct CDBoxDesc: public CDShapeDesc{
	DESC_DEF_FOR_OBJECT(CDBox);
	CDBoxDesc():CDShapeDesc(){
		boxsize = Vec3f(1.0f, 1.0f, 1.0f);
	}
	Vec3f boxsize;					// 直方体のサイズ（各辺の長さ）
};	


//@}

}

#endif

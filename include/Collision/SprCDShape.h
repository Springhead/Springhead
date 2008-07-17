/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	SPR_IFDEF(CDShape);

	void	SetStaticFriction(float mu0);
	float	GetStaticFriction();
	void	SetDynamicFriction(float mu);
	float	GetDynamicFriction();
	void	SetElasticity(float e);
	float	GetElasticity();
	void	SetDensity(float d);
	float	GetDensity();

	float	CalcVolume();			///< 体積を計算
	Vec3f	CalcCenterOfMass();		///< 質量中心
	Matrix3f CalcMomentOfInertia();	///< 単位密度における質量中心に関する慣性行列
};

///	凸形状の基本クラス
struct CDConvexIf : public CDShapeIf{
	SPR_IFDEF(CDConvex);
};

/**	面	*/
struct CDFaceIf: public ObjectIf{
	SPR_IFDEF(CDFace);
	int NIndex();
	int* GetIndices();
};
struct CDQuadFaceIf: public ObjectIf{
	SPR_IFDEF(CDQuadFace);
	int NIndex();
	int* GetIndices();
};


///	物理シミュレーションに関係する材質
struct PHMaterial{
	PHMaterial();
	float mu;				///< 動摩擦摩擦係数
	float mu0;				///< 静止摩擦係数
	float e;				///< 跳ね返り係数
	float density;			///< 密度

	float vibA;
	float vibB;
	float vibW;
};

///	形状のディスクリプタ(基本クラス)
struct CDShapeDesc{
	PHMaterial material;	///<	材質
};


/**	凸形状のメッシュ*/
struct CDConvexMeshIf: public CDConvexIf{
	SPR_IFDEF(CDConvexMesh);
	CDFaceIf* GetFace(size_t i);
	size_t NFace();
	Vec3f* GetVertices();
	size_t NVertex();
};
/**	凸形状のメッシュのディスクリプタ	*/	
struct CDConvexMeshDesc: public CDShapeDesc{
	SPR_DESCDEF(CDConvexMesh);
	CDConvexMeshDesc():CDShapeDesc(){}
	std::vector<Vec3f> vertices;	///<	頂点の座標
};
	
/** 球体　*/
struct CDSphereIf: public CDConvexIf{
	SPR_IFDEF(CDSphere);
	float GetRadius();
};	
/** 球体のディスクリプタ　*/
struct CDSphereDesc: public CDShapeDesc{
	SPR_DESCDEF(CDSphere);
	CDSphereDesc():CDShapeDesc(){
		radius = 1.0f;
	}
	float radius;					// 球体の半径
};	

/** カプセル　*/
struct CDCapsuleIf: public CDConvexIf{
	SPR_IFDEF(CDCapsule);
	float GetRadius();
	float GetLength();
};	
/** カプセルのディスクリプタ　*/
struct CDCapsuleDesc: public CDShapeDesc{
	SPR_DESCDEF(CDCapsule);
	CDCapsuleDesc():CDShapeDesc(){
		radius = 1.0f;
		length = 1.0f;
	}
	float radius;					///< カプセルの球の半径
	float length;					///< カプセルの長さ Z軸向きが長い
};	

	
/** 直方体 */
struct CDBoxIf: public CDConvexIf{
	SPR_IFDEF(CDBox);
	Vec3f GetBoxSize();
	Vec3f* GetVertices();
	CDFaceIf* GetFace(size_t i);
	Vec3f SetBoxSize(Vec3f boxsize);
};
/** 直方体のディスクリプタ */
struct CDBoxDesc: public CDShapeDesc{
	SPR_DESCDEF(CDBox);
	CDBoxDesc():CDShapeDesc(){
		boxsize = Vec3f(1.0f, 1.0f, 1.0f);
	}
	Vec3f boxsize;					// 直方体のサイズ（各辺の長さ）
};	


//@}

}

#endif

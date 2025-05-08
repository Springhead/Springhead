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

struct PHMaterial;

///	形状の基本クラス
struct CDShapeIf : public NamedObjectIf{
	SPR_IFDEF(CDShape);
	/// 静止摩擦係数の設定
	void	SetStaticFriction(float mu0);
	/// 静止摩擦係数の取得
	float	GetStaticFriction();
	/// 動摩擦係数の設定
	void	SetDynamicFriction(float mu);
	/// 動摩擦係数の取得
	float	GetDynamicFriction();
	/// 反発係数(はねかえり係数)の設定
	void	SetElasticity(float e);
	/// 反発係数(はねかえり係数)の取得
	float	GetElasticity();
	/// 密度の設定
	void	SetDensity(float d);
	/// 密度の取得
	float	GetDensity();
	/// 静接触の弾性の設定
	void    SetContactSpring(float K);
	/// 静接触の弾性の取得
	float   GetContactSpring();
	/// 静接触の粘性の設定
	void    SetContactDamper(float D);
	/// 静接触の粘性の取得
	float   GetContactDamper();

	// ペナルティ法演算用
	/// 跳ね返りバネ係数の設定
	void	SetReflexSpring(float K);
	/// 跳ね返りバネ係数の取得
	float	GetReflexSpring();
	/// 跳ね返りダンパ係数の設定
	void	SetReflexDamper(float D);
	/// 跳ね返りダンパ係数の取得
	float	GetReflexDamper();
	/// 摩擦バネ係数の設定
	void	SetFrictionSpring(float K);
	/// 摩擦バネ係数の取得
	float	GetFrictionSpring();
	/// 摩擦ダンパ係数の設定
	void	SetFrictionDamper(float D);
	/// 摩擦ダンパ係数の取得
	float	GetFrictionDamper();

	void    SetVibration(float vibA, float vibB, float vibW);
	void    SetVibA(float vibA);
	float   GetVibA();
	void    SetVibB(float vibB);
	float   GetVibB();
	void    SetVibW(float vibW);
	float   GetVibW();
	void    SetVibT(float vibT);
	float   GetVibT();
	void    SetVibContact(bool vibContact);
	bool    GetVibContact();

	/// 物性をまとめて取得
	const PHMaterial&	GetMaterial();
	/// 物性をまとめて設定
	void				SetMaterial(const PHMaterial& mat);


	/// 幾何計算

	/** 体積を計算
		@return 形状の体積
	 */
	float	 CalcVolume();

	/** 質量中心を計算
		@return 形状の質量中心の位置
	 */
	Vec3f	 CalcCenterOfMass();

	/** 慣性モーメントを計算
		@return 密度を1としたときの質量中心に関する慣性行列を計算する．
	 */
	Matrix3f CalcMomentOfInertia();

	/** 点との交差判定
		@param	p	判定点
		@return		形状の境界上あるいは内部ならtrue
		点pが形状に含まれるか判定する
	 */
	bool     IsInside(const Vec3f& p);

	/** bounding boxを計算
		@param bbmin	bounding boxの下限
		@param bbmax	bounding boxの上限
		@param pose		bounding boxを定義する座標変換
		形状の位置と向きがposeで与えられたときの，その座標系に関するaxis-aligned bounding box (AABB)を計算する．
	 */
	void     CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose = Posed());
};

///	凸形状の基本クラス
struct CDConvexIf : public CDShapeIf{
	SPR_IFDEF(CDConvex);
	double CurvatureRadius(Vec3d p); ///< 表面上の点pにおける曲率半径
	Vec3d Normal(Vec3d p); ///< 表面上の点pにおける法線
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
	//	質量・慣性テンソルの計算用
	float density;			///< 密度
	//	LCP(PHConstarintEngine)による拘束力計算用
	float mu;				///< 動摩擦摩擦係数
	float mu0;				///< 静止摩擦係数	
	float e;				///< 跳ね返り係数
	float rotationFriction;	///< 点接触の回転摩擦の並進に対する割合
	// 静接触の粘弾性．springとdamperが0の場合は粘弾性なし
	float spring;			///< 静接触のバネ係数
	float damper;			///< 静接触のダンパ係数

	//	ペナルティ法(PHPenaltyEngine)のためのバネ・ダンパ係数
	float reflexSpring;
	float reflexDamper;
	float frictionSpring;
	float frictionDamper;
	
	///	固有振動提示のための係数
	float vibA;				///< 振幅係数
	float vibB;				///< 減衰係数
	float vibW;				///< 周波数
	float vibT;				///< 接触時間
	bool vibContact;		///< 接触時の固有振動を発生させるかどうか
	bool vibFric;			///< 摩擦時の固有振動を発生させるかどうか

	/**	時変摩擦係数のための係数	静止摩擦 = mu + A log(1+Bt),  動摩擦 = mu + A log (1+B C/v)  [Dieterich 1979]
		Cは動摩擦時のスリップ距離を意味する C > v*dt(時間刻み) のときは、v= C/dt を使う 
		A, Bの代わりに、t = 60秒に mu0 になり、 timeVaryFrictionHalfTime で (mu+mu0)/2になるようなA,Bを使う
		mu0 - mu = A log(1+60B), 60B= exp((mu0-mu)/A) - 1
		(mu0 - mu)/2 = A log(1+half B), half B = exp((mu0-mu)/2A) - 1
		(exp((mu0-mu)/A) - 1)*half/60 = exp((mu0-mu)/2A) - 1
		(exp((mu0-mu)/A) - 1)*half/60 - exp((mu0-mu)/2A) =  - 1

	*/
//	float timeVaryFrictionHalfTime;
	float timeVaryFrictionA;
	float timeVaryFrictionB;
	float timeVaryFrictionC;
	///	粘性摩擦のための係数	f_t = frictionViscocity * vel * f_N
	float frictionViscosity;

	float stribeckVelocity;
	float stribeckmu;
};

///	形状のディスクリプタ(基本クラス)
struct CDShapeDesc{
	SPR_DESCDEF(CDShape);
	PHMaterial material;	///<	材質
};


/**	凸形状のメッシュ*/
struct CDConvexMeshIf: public CDConvexIf{
	SPR_IFDEF(CDConvexMesh);
	CDFaceIf* GetFace(int i);
	int NFace();
	Vec3f* GetVertices();
	int NVertex();
};
/**	凸形状のメッシュのディスクリプタ	*/	
struct CDConvexMeshDesc: public CDShapeDesc{
	SPR_DESCDEF(CDConvexMesh);
	CDConvexMeshDesc():CDShapeDesc(){}
	std::vector<Vec3f> vertices;	///<	頂点の座標
};

/**	凸形状のメッシュ SupportPointの補完つき*/
struct CDConvexMeshInterpolateIf: public CDConvexMeshIf{
	SPR_IFDEF(CDConvexMeshInterpolate);
};
/**	凸形状のメッシュのディスクリプタ	*/	
struct CDConvexMeshInterpolateDesc: public CDConvexMeshDesc{
	SPR_DESCDEF(CDConvexMeshInterpolate);
	CDConvexMeshInterpolateDesc():CDConvexMeshDesc(){}
};


/** 球体　*/
struct CDSphereIf: public CDConvexIf{
	SPR_IFDEF(CDSphere);
	float	GetRadius();
	void	SetRadius(float r);
};	
/** 球体のディスクリプタ　*/
struct CDSphereDesc: public CDShapeDesc{
	SPR_DESCDEF(CDSphere);
	CDSphereDesc():CDShapeDesc(){
		radius = 1.0f;
	}
	float radius;					// 球体の半径
};	

/** 楕円体　*/
struct CDEllipsoidIf: public CDConvexIf{
	SPR_IFDEF(CDEllipsoid);
	Vec3d	GetRadius();
	void	SetRadius(Vec3d r);
};	
/** 楕円のディスクリプタ　*/
struct CDEllipsoidDesc: public CDShapeDesc{
	SPR_DESCDEF(CDEllipsoid);
	CDEllipsoidDesc():CDShapeDesc(){
		radius = Vec3d(1, 0.2, 1);
	}
	Vec3d radius;					// 楕円体の半径
};	

/** カプセル　*/
struct CDCapsuleIf: public CDConvexIf{
	SPR_IFDEF(CDCapsule);
	float	GetRadius();
	void	SetRadius(float r);
	float	GetLength();
	void	SetLength(float l);
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

/** カプセルの両端のサイズが違うやつ　*/
struct CDRoundConeIf: public CDConvexIf{
	SPR_IFDEF(CDRoundCone);
	Vec2f GetRadius();
	float GetLength();
	void  SetRadius(Vec2f r);
	void  SetLength(float l);
	void  SetWidth(Vec2f r) ;		///< 長さを変えないで，太さだけ変更する
};	
/** CDRoundConeIfのディスクリプタ　*/
struct CDRoundConeDesc: public CDShapeDesc{
	SPR_DESCDEF(CDRoundCone);
	CDRoundConeDesc():CDShapeDesc(){
		radius = Vec2f(1.2f, 0.8f);
		length = 1.0f;
	}
	Vec2f radius;					///< カプセルの球の半径．Z-が radius[0], Z+が radius[1]．
	float length;					///< 2つの球の中心間距離．
};	
	
/** 直方体 */
struct CDBoxIf: public CDConvexIf{
	SPR_IFDEF(CDBox);
	Vec3f GetBoxSize();
	Vec3f* GetVertices();
	CDFaceIf* GetFace(int i);
	Vec3f SetBoxSize(Vec3f boxsize);
};
/** 直方体のディスクリプタ */
struct CDBoxDesc: public CDShapeDesc{
	SPR_DESCDEF(CDBox);
	CDBoxDesc():CDShapeDesc(){
		boxsize = Vec3f(1.0f, 1.0f, 1.0f);
	}
	CDBoxDesc(Vec3d bs):CDShapeDesc(),boxsize(bs){
	}
	Vec3f boxsize;					// 直方体のサイズ（各辺の長さ）
};	


//@}

}

#endif

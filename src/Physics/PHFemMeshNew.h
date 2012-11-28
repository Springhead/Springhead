/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef PH_FEMMESH_NEW_H
#define PH_FEMMESH_NEW_H

#include <Physics/SprPHFemMeshNew.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>

namespace Spr{;

/// 計算モジュールの共通部分
class PHFem: public SceneObject{
public:
	SPR_OBJECTDEF_ABST(PHFem);
	SPR_DECLMEMBEROF_PHFemDesc;

	PHFem(){}

};

/* 四面体を表すためのクラス、構造体の宣言 */
//	頂点
class FemVertex{
public:
	Vec3d pos;
	std::vector<int> tets;
	std::vector<int> edges;
	std::vector<int> faces;
	bool vtxDeformed;		///< 四面体の変形でこの節点がローカル座標基準で移動したかどうか
	double disFromOrigin;	///< x-z平面でのローカル座標の原点からの距離
};
//	四面体
class Tet{
public:
	int vertices[4];	///< 頂点ID																																	  
	int faces[4];		///< 表面４つ
	int edges[6];		///< 対応する辺のID。0:辺01, 1:辺12, 2:辺20, 3:辺03, 4:辺13, 5:辺23
	double volume;		///< 積分計算で用いるための体積
	PTM::TMatrixRow<4,4,double> matk[3];	///<	
	PTM::TVector<4,double> vecf[4];			///<	{f1}:vecf[0],{f2}:vecf[1],{f3}:vecf[2],...
	int& edge(int i, int j);
};

// 面
class FemFace{
	int sorted[3];		///< 比較するための、ソート済みの頂点id。Update()で更新。
public:
	int vertices[3];	///<頂点ID。順番で面の表裏を表す。
	void Update();
	bool operator < (const FemFace& f2);	///< 頂点IDで比較
	bool operator == (const FemFace& f2);	///< 頂点IDで比較
	double area;						///< 四面体の各面の面積
};
//	辺
struct FemEdge{
	int vertices[2];
	bool operator < (const FemEdge& e2); 	///< 頂点IDで比較
	bool operator == (const FemEdge& e2);	///< 頂点IDで比較
	double k;	///< 全体剛性行列Kの要素
	double c;	///<
	double b;	///< ガウスザイデル計算で用いる定数b
	FemEdge(int v1=-1, int v2=-1);
};


class PHFemVibration;
class PHFemMeshNew : public SceneObject{//, public PHFemMeshNewDesc{
public:
	/* PHFemMeshNewのメンバ変数、関数群 */
	SPR_OBJECTDEF(PHFemMeshNew);
	//SPR_DECLMEMBEROF_PHFemMeshNewDesc;
protected:
	///	基本情報(生成時にデスクリプタから与えられる情報)
	PHSolidIf* solid;					///< 関連づけられている剛体
public:
	std::vector<FemVertex> vertices;	///< 頂点
	std::vector<Tet> tets;				///< 四面体
	
	/// 追加情報	基本情報からSetDesc()が計算して求める。
	std::vector<int> surfaceVertices;	///< 物体表面の頂点のID
	std::vector<FemFace> faces;	///< 面
	unsigned nSurfaceFace;		///< 物体表面に存在する面の数。表面：faces[0],..,faces[nSurfaceFace-1]、内面:faces[nSurfaceFace],..,faces[faces.size()]
	std::vector<FemEdge> edges;	///< 辺
	unsigned nSurfaceEdge;		///< 物体表面に存在する辺。表面:edges[0],..,edges[nSurfaceEdge-1]、内面:edges[nSurfaceEdge],..,edges[edges.size()]

	/// 計算モジュール
	//PHFems** fems;
	UTRef< PHFemVibration > femVibration;
	//PHFemThermo*	thermo;

	///	デスクリプタ
	PHFemMeshNew(const PHFemMeshNewDesc& desc = PHFemMeshNewDesc(), SceneIf* s=NULL);
	///	デスクリプタのサイズ
	virtual size_t GetDescSize() const ;
	///	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* p) const ;
	///	デスクリプタの設定。ここで、頂点座標と四面体の4頂点のIDの情報から、面や辺についての情報を計算しておく。
	virtual void SetDesc(const void* p);
	/// 子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o);

	///	時刻をdt進める。PHFemEngineが呼び出す。
	virtual void Step(double dt);
	/// 剛体を関連づける
	void SetPHSolid(PHSolidIf* s);
	/// 関連付けされている剛体を返す
	PHSolidIf* GetPHSolid();
	///	面の総数を返す
	unsigned GetNFace();
	///	Face辺の両端点の座標を返す?
	std::vector<Vec3d> GetFaceEdgeVtx(unsigned id);
	//	Face辺の両端点の座標を返す?
	Vec3d GetFaceEdgeVtx(unsigned id, unsigned vtx);
	/// 四面体の計算(対象によらずに必要になる形状関数のXXを計算する関数)
	void UpdateJacobian();
};

}

#endif

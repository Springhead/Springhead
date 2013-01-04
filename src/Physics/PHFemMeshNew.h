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
#include "../Foundation/Object.h"
#include "PHScene.h"

namespace Spr{;
using namespace PTM;

/// 計算モジュールの共通部分
class PHFemMeshNew;
class PHFem: public SceneObject{
public:
	SPR_OBJECTDEF_ABST(PHFem);
	SPR_DECLMEMBEROF_PHFemDesc;
protected:
	UTRef< PHFemMeshNew > phFemMesh;
public:
	PHFem(){}
	virtual void Init(){}
	virtual void Step(){}

	void SetPHFemMesh(PHFemMeshNew* m){ phFemMesh = m; }
	PHFemMeshNew* GetPHFemMesh(){ return phFemMesh; }
	int NVertices();
};

/* 四面体を表すためのクラス、構造体の宣言 */
//	頂点
class FemVertex{
public:
	Vec3d pos;
	Vec3d initialPos;
	std::vector<int> tetIDs;
	std::vector<int> edgeIDs;
	std::vector<int> faceIDs;
	bool vtxDeformed;		///< 四面体の変形でこの節点がローカル座標基準で移動したかどうか
	double disFromOrigin;	///< x-z平面でのローカル座標の原点からの距離
};
//	四面体
class FemTet{
public:
	int vertexIDs[4];	///< 頂点ID																																	  
	int faceIDs[4];		///< 表面４つ
	int edgeIDs[6];		///< 対応する辺のID。0:辺01, 1:辺12, 2:辺20, 3:辺03, 4:辺13, 5:辺23
	double volume;		///< 四面体の体積
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
	int vertexIDs[2];
	bool operator < (const FemEdge& e2); 	///< 頂点IDで比較
	bool operator == (const FemEdge& e2);	///< 頂点IDで比較
	FemEdge(int v1=-1, int v2=-1);
};

typedef std::vector< UTRef< PHFem > > PHFems;
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
	//std::vector<FemVertex> initVertices;///< 初期頂点
	std::vector<FemTet> tets;			///< 四面体
	
	/// 追加情報	基本情報からSetDesc()が計算して求める。
	std::vector<int> surfaceVertices;	///< 物体表面の頂点のID
	std::vector<FemFace> faces;	///< 面
	unsigned nSurfaceFace;		///< 物体表面に存在する面の数。表面：faces[0],..,faces[nSurfaceFace-1]、内面:faces[nSurfaceFace],..,faces[faces.size()]
	std::vector<FemEdge> edges;	///< 辺
	unsigned nSurfaceEdge;		///< 物体表面に存在する辺。表面:edges[0],..,edges[nSurfaceEdge-1]、内面:edges[nSurfaceEdge],..,edges[edges.size()]

	/// 計算モジュール
	PHFems fems;
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

	/// 初期化
	virtual void Init();
	///	時刻をdt進める。PHFemEngineが呼び出す。
	virtual void Step(double dt);
	/// 剛体を関連づける
	void SetPHSolid(PHSolidIf* s);
	/// 関連付けされている剛体を返す
	PHSolidIf* GetPHSolid();
	/// PHFemVibrationIfを返す
	PHFemVibrationIf* GetPHFemVibration();
	/// 頂点の総数を返す
	int NVertices();
	///	面の総数を返す
	int NFaces();
	///	Face辺の両端点の座標を返す?
	std::vector<Vec3d> GetFaceEdgeVtx(unsigned id);
	//	Face辺の両端点の座標を返す?
	Vec3d GetFaceEdgeVtx(unsigned id, unsigned vtx);
	/// 四面体の計算(対象によらずに必要になる形状関数のXXを計算する関数)
	void UpdateJacobian();
	/// 四面体の体積を返す
	double GetTetrahedronVolume(int tetID);

	/// 頂点の初期位置を取得する（ローカル座標系）
	Vec3d GetVertexInitPositionL(int vtxId);

	/// 頂点の位置を取得する（ローカル座標系）
	Vec3d GetVertexPositionL(int vtxId);
	/// 頂点の変位を取得する（ローカル座標系）
	Vec3d GetVertexDisplacementL(int vtxId);
	
	/// 頂点に変位を与える（ワールド座標系）
	bool AddVertexDisplacementW(int vtxId, Vec3d disW);
	/// 頂点に変位を与える（ローカル座標系）
	bool AddVertexDisplacementL(int vtxId, Vec3d disL);
	
	/// 頂点の位置を指定する（ワールド座標系）
	bool SetVertexPositionW(int vtxId, Vec3d posW);
	/// 頂点の位置を指定する（ローカル座標系）
	bool SetVertexPositionL(int vtxId, Vec3d posL);
};

}

#endif

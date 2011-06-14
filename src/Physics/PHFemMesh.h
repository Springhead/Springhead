/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHFEMMESH_H
#define PHFEMMESH_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>

namespace Spr{;

///	有限要素法シミュレーションのためのメッシュ(4面体メッシュ)

class PHFemMesh: public SceneObject{
public:
	//	頂点
	struct FemVertex{
		Vec3d pos;
		std::vector<int> tets;
		std::vector<int> edges;
		FemVertex();
	};
	//	四面体
	struct Tet{
		int vertices[4];	//	頂点ID
		int edges[6];		//	対応する辺のID。0:辺01, 1:辺12, 2:辺20, 3:辺03, 4:辺13, 5:辺23
		int& edge(int i, int j);
	};
	//	四面体の面。
	class Face{
		///	比較するための、ソート済みの頂点id。Update()で更新。
		int sorted[3];
	public:
		///	頂点ID。順番で面の表裏を表す。
		int vertices[3];
		void Update();
		///	頂点IDで比較
		bool operator < (const Face& f2);
		///	頂点IDで比較
		bool operator == (const Face& f2);
	};
	//	辺
	struct Edge{
		Edge(int v1=-1, int v2=-1);
		int vertices[2];
		///	頂点IDで比較
		bool operator < (const Edge& e2);
		///	頂点IDで比較
		bool operator == (const Edge& e2);
	};
	//	基本情報(生成時に与えられる情報)
	///	頂点
	std::vector<FemVertex> vertices;
	///	四面体
	std::vector<Tet> tets;
	
	//	以下は基本情報から計算して求める。
	///	物体表面の頂点のID
	std::vector<int> surfaceVertices;
	///	物体表面の面
	std::vector<Face> faces;
	///	面のうち物体表面のものが、faces[0]..faces[nSurfaceFace-1]
	unsigned nSurfaceFace;
	///	物体表面の辺
	std::vector<Edge> edges;
	///	辺のうち物体表面のものが、edges[0]..edges[nSurfaceEdge]
	unsigned nSurfaceEdge;

	
	SPR_OBJECTDEF(PHFemMesh);
	PHFemMesh(const PHFemMeshDesc& desc=PHFemMeshDesc(), SceneIf* s=NULL);
	//	デスクリプタのサイズ
	virtual size_t GetDescSize() const { return sizeof(PHFemMeshDesc); };
	//	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* desc) const ;
	// デスクリプタの設定
	virtual void SetDesc(const void* desc);
	//	四面体単位の係数行列を、頂点単位の係数行列へ変換する。
	void Tet2Vertex();

};




}	//	namespace Spr
#endif

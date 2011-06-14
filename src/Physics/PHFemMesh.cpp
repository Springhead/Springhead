/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#include "PHFemMesh.h"
#pragma hdrstop
#include <float.h>

using namespace PTM;
namespace Spr{;

PHFemMeshDesc::PHFemMeshDesc(){
	Init();
}
void PHFemMeshDesc::Init(){
	
}


void PHFemMesh::Face::Update(){
	for(int i=0; i<3; ++i) sorted[i] = vertices[i];
	std::sort(sorted, sorted+3);
}
bool PHFemMesh::Face::operator < (const Face& f2){
	const Face& f1 = *this;
	for(int i=0; i<3; ++i){
		if (f1.sorted[i] < f2.sorted[i]) return true;
		if (f1.sorted[i] > f2.sorted[i]) return false;
	}
	return false;
} 
bool PHFemMesh::Face::operator == (const Face& f2){
	const Face& f1 = *this;
	for(int i=0; i<3; ++i){
		if (f1.sorted[i] != f2.sorted[i]) return false;
	}
	return true;
}

PHFemMesh::Edge::Edge(int v1, int v2){
	if (v1>v2) std::swap(v1, v2);
	assert(v1==-1 && v2==-1 || v1 < v2);
	vertices[0] = v1;
	vertices[1] = v2;
}
bool PHFemMesh::Edge::operator < (const Edge& e2){
	if (vertices[0] < e2.vertices[0]) return true;
	if (vertices[0] > e2.vertices[0]) return false;
	if (vertices[1] < e2.vertices[1]) return true;
	return false;
}
bool PHFemMesh::Edge::operator == (const Edge& e2){
	return vertices[0] == e2.vertices[0] && vertices[1] == e2.vertices[1];
}



int& PHFemMesh::Tet::edge(int i, int j){
	if (i>j) std::swap(i, j);
	if (j==3) return edges[3+i];
	if (j==2 && i==0) return edges[2];
	return edges[i];
}


///////////////////////////////////////////////////////////////////
//	PHFemMesh
PHFemMesh::FemVertex::FemVertex(){
}



PHFemMesh::PHFemMesh(const PHFemMeshDesc& desc, SceneIf* s){
	SetDesc(&desc);
	if (s){ SetScene(s); }

	//ThermalFEMの実装を追加していく

	//計算の関数については、ガウス・ザイデルで　

}


void PHFemMesh::SetDesc(const void* p) {
	PHFemMeshDesc* d = (PHFemMeshDesc*)p;
	tets.resize(d->tets.size() / 4);
	vertices.resize(d->vertices.size());
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j)
			tets[i].vertices[j] = d->tets[i*4+j];
	}
	for(unsigned i=0; i<vertices.size(); ++i){
		vertices[i].pos = d->vertices[i];
		vertices[i].tets.clear();
	}
	//	接続情報の更新
	//	頂点に属する四面体を追加
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j){
			vertices[tets[i].vertices[j]].tets.push_back(i);
//			for(unsigned k=0; k<4; ++k)
//				if (j!=k) vertices[tets[i].vertices[j]].neighbors.push_back(tets[i].vertices[k]);
		}
	}
/*
	//	neighborsの重複の削除
	for(unsigned i=0; i<vertices.size(); ++i){
		std::sort(vertices[i].neighbors.begin(), vertices[i].neighbors.end());
		std::vector<int>::iterator newEnd = std::unique(vertices[i].neighbors.begin(), vertices[i].neighbors.end());
		vertices[i].neighbors.erase(newEnd, vertices[i].neighbors.end());
	}
*/	
	//	表面を探す
	std::vector<Face> allFaces;
	//	裏表を考える必要がある。
	/*
					0


			1			3
				2
		012, 023, 031, 321
	*/
	int tfs[4][3]={{0,1,2}, {0,2,3}, {0,3,1}, {3,2,1}};
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j){
			Face f;	
			for(unsigned k=0; k<3; ++k) f.vertices[k] = tets[i].vertices[tfs[j][k]];
			f.Update();
			allFaces.push_back(f);
		}
	}
	std::sort(allFaces.begin(), allFaces.end());

	std::vector<Face> ifaces;
	for(unsigned i=0; i<allFaces.size(); ++i){
		if (i+1<allFaces.size() && allFaces[i] == allFaces[i+1]){
			ifaces.push_back(allFaces[i]);	//	中面
			i++;
		}else{
			faces.push_back(allFaces[i]);	//	表面
		}
	}
	nSurfaceFace = faces.size();
	faces.insert(faces.end(), ifaces.begin(), ifaces.end());
	//	表面の頂点の列挙
	for(unsigned i=0; i<nSurfaceFace; ++i){
		for(unsigned j=0; j<3; ++j){
			surfaceVertices.push_back(faces[i].vertices[j]);
		}
	}
	std::sort(surfaceVertices.begin(), surfaceVertices.end());
	std::vector<int>::iterator newEnd = std::unique(surfaceVertices.begin(), surfaceVertices.end());
	surfaceVertices.erase(newEnd, surfaceVertices.end());

	//	辺の列挙
	//	まず表面の辺
	for(unsigned i=0; i<nSurfaceFace; ++i){
		for(unsigned j=0; j<3; ++j){
			edges.push_back(Edge(faces[i].vertices[j], faces[i].vertices[(j+1)%3]));
		}
	}
	std::sort(edges.begin(), edges.end());
	std::vector<Edge>::iterator newEEnd = std::unique(edges.begin(), edges.end());
	edges.erase(newEEnd, edges.end());
	nSurfaceEdge = edges.size();
	//	内部の辺の列挙
	std::vector<Edge> iEdges;
	for(unsigned i=nSurfaceFace; i<faces.size() ;++i){
		for(unsigned j=0; j<3; ++j){
			iEdges.push_back(Edge(faces[i].vertices[j], faces[i].vertices[(j+1)%3]));
		}
	}
	//	重複を削除
	std::sort(iEdges.begin(), iEdges.end());
	newEEnd = std::unique(iEdges.begin(), iEdges.end());
	iEdges.erase(newEEnd, iEdges.end());
	//	表の辺(edgesのnSurfaceEdgeまで)に含まれない物を、edgesの後ろに追加
	edges.resize(nSurfaceEdge + iEdges.size());
	newEEnd = std::set_difference(iEdges.begin(), iEdges.end(), edges.begin(), edges.begin()+nSurfaceEdge, edges.begin()+nSurfaceEdge);
	edges.erase(newEEnd, edges.end());

	//	頂点に辺を追加
	for(unsigned i=0; i<edges.size(); ++i){
		for(int j=0; j<2; ++j){
			vertices[edges[i].vertices[j]].edges.push_back(i);
		}
	}
	//	四面体に辺を追加
	for(unsigned i=0; i<tets.size(); ++i){
		int count = 0;
		for(unsigned j=0; j<4; ++j){
			FemVertex& vtx = vertices[tets[i].vertices[j]];
			//	四面体のある頂点から出ている辺のうち、その頂点が始点(vertices[0])になっているものについて
			for(unsigned k=0; k<vtx.edges.size(); ++k){
				Edge& e = edges[vtx.edges[k]];
				if (e.vertices[0] != tets[i].vertices[j]) continue;
				//	辺が四面体に含まれる場合、辺を設定
				for(int l=0; l<4; ++l){
					if (e.vertices[1] == tets[i].vertices[l]){
						tets[i].edge(j, l) = vtx.edges[k];
						count ++;
						break;
					}
				}
			}
		}
		assert(count == 6);
	}
}

bool PHFemMesh::GetDesc(void* p) const {
	PHFemMeshDesc* d = (PHFemMeshDesc*)p;
	d->tets.resize(tets.size() * 4);
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; i<4; ++j)
			d->tets[i*4+j] = tets[i].vertices[j];
	}
	d->vertices.resize(vertices.size());
	for(unsigned i=0; i<vertices.size(); ++i){
		d->vertices[i] = vertices[i].pos;
	}
	return true;
}

/*
	有限要素法では、要素（＝四面体）の体積や面積の積分から、各ノードの物理量を求める。
	各ノードは複数の四面体で共有されているので、それらの合成を持つことになる。

	頂点と頂点の間に係数がある。
	    d    
		     x

i         x  d  o

j		     o  d

		i j 
	Ax + b = 0,  (D-F)x + b = 0 x = D^{-1} F x -  D^{-1} b, x = D^{-1} F x - b'
	x_i =   (F_i * x)/D_ii - b_i/D_ii
	i->j, j->i で同じ係数(F_ij, F_ji)が必要だが、計算は上から行うので、両方から係数が見えないとならない。
	
	1. 全四面体について係数を求め、i<jに係数を記録していく
	2. ijの係数をjiにコピー
		頂点i -> ijの係数 <- 頂点j とするか？	(2なし)
		頂点i -> ijの係数  頂点j -> jiの係数　とするか？	(2あり)

	頂点に隣のID(i->j)=辺を記録しておきたい。
	四面体からも、頂点iの(i->j)のうちどのjが自分に含まれるかすぐ分かると良い。
	四面体に辺への参照を持たせたい
	
	∴辺構造体が必要
		頂点i -> ijの係数 <- 頂点j とするのが良い	(2なし)
*/
void PHFemMesh::Tet2Vertex(){
}

}

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


///////////////////////////////////////////////////////////////////
//	PHFemMesh
PHFemMesh::FemVertex::FemVertex(){
	surfaceFlag = false;
}



PHFemMesh::PHFemMesh(const PHFemMeshDesc& desc, SceneIf* s){
	SetDesc(&desc);
	if (s){ SetScene(s); }

	//ThermalFEMの実装を追加していく

	//計算の関数については、ガウス・ザイデルで　

}

struct PHFemMeshFace{
	int vertices[3];
};
bool operator < (const PHFemMeshFace& f1, const PHFemMeshFace& f2){
	for(int i=0; i<3; ++i){
		if (f1.vertices[i] < f2.vertices[i]) return true;
		if (f1.vertices[i] > f2.vertices[i]) return false;
	}
	return false;
} 
bool operator == (const PHFemMeshFace& f1, const PHFemMeshFace& f2){
	for(int i=0; i<3; ++i){
		if (f1.vertices[i] != f2.vertices[i]) return false;
	}
	return true;
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
			for(unsigned k=0; k<4; ++k)
				if (j!=k) vertices[tets[i].vertices[j]].neighbors.push_back(tets[i].vertices[k]);
		}
	}
	//	neighborsの重複の削除
	for(unsigned i=0; i<vertices.size(); ++i){
		std::sort(vertices[i].neighbors.begin(), vertices[i].neighbors.end());
		std::vector<int>::iterator newEnd = std::unique(vertices[i].neighbors.begin(), vertices[i].neighbors.end());
		vertices[i].neighbors.erase(newEnd, vertices[i].neighbors.end());
	}
	
	//	表面を探す
	std::vector<PHFemMeshFace> faces;
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j){
			PHFemMeshFace f;
			for(unsigned k=0; k<3; ++k)
				f.vertices[k] = tets[i].vertices[(j+k)%4];
			std::sort(f.vertices, f.vertices+3);
			faces.push_back(f);
		}
	}
	std::sort(faces.begin(), faces.end());
	std::vector<PHFemMeshFace> surfaces;

	for(unsigned i=0; i<faces.size(); ++i){
		if (i+1<faces.size() && faces[i] == faces[i+1]){
			i++;
		}else{
			surfaces.push_back(faces[i]);
		}
	}
	for(unsigned i=0; i<surfaces.size(); ++i){
		for(unsigned j=0; j<3; ++j){
			vertices[surfaces[i].vertices[j]].surfaceFlag = true;
		}
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

}

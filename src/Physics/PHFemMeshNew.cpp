/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Physics/PHFemMeshNew.h>
#include <Physics/PHFemBase.h>
#include <Physics/PHFemVibration.h>
#include <Physics/PHFemThermo.h>

namespace Spr{;

///////////////////////////////////////////////////////////////////
/* 四面体を表すためのクラス、構造体の定義 */
// 四面体
int& FemTet::edge(int i, int j){
	if (i>j) std::swap(i, j);
	if (j==3) return edgeIDs[3+i];
	if (j==2 && i==0) return edgeIDs[2];
	return edgeIDs[i];
}

// 四面体の面
void FemFace::Update(){
	for(int i=0; i<3; ++i) sorted[i] = vertexIDs[i];
	std::sort(sorted, sorted+3);
}

bool FemFace::operator < (const FemFace& f2){
	const FemFace& f1 = *this;
	for(int i=0; i<3; ++i){
		if (f1.sorted[i] < f2.sorted[i]) return true;
		if (f1.sorted[i] > f2.sorted[i]) return false;
	}
	return false;
} 
bool FemFace::operator == (const FemFace& f2){
	const FemFace& f1 = *this;
	for(int i=0; i<3; ++i){
		if (f1.sorted[i] != f2.sorted[i]) return false;
	}
	return true;
}

// 辺
FemEdge::FemEdge(int v1, int v2){
	if (v1>v2) std::swap(v1, v2);
	assert(v1==-1 && v2==-1 || v1 < v2);
	vertexIDs[0] = v1;
	vertexIDs[1] = v2;
}
bool FemEdge::operator < (const FemEdge& e2){
	if (vertexIDs[0] < e2.vertexIDs[0]) return true;
	if (vertexIDs[0] > e2.vertexIDs[0]) return false;
	if (vertexIDs[1] < e2.vertexIDs[1]) return true;
	return false;
}
bool FemEdge::operator == (const FemEdge& e2){
	return vertexIDs[0] == e2.vertexIDs[0] && vertexIDs[1] == e2.vertexIDs[1];
}

///////////////////////////////////////////////////////////////////
/* PHFemMeshNewのメンバ関数の定義 */
PHFemMeshNew::PHFemMeshNew(const PHFemMeshNewDesc& desc, SceneIf* s){
	SetDesc(&desc);
	if (s){ SetScene(s); }

	//For multiple FEM implementation
	this->debugVertexInside = NULL;
	this->debugFacesInside = NULL;
	this->debugFixedPoints = NULL;
	this->debugPairs = NULL;
}

size_t PHFemMeshNew::GetDescSize() const { 
	return sizeof(PHFemMeshNewDesc);
}

bool PHFemMeshNew::GetDesc(void* p) const {
	PHFemMeshNewDesc* d = (PHFemMeshNewDesc*)p;
	d->tets.resize(tets.size() * 4);
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j)
			d->tets[i*4+j] = tets[i].vertexIDs[j];
	}
	d->vertices.resize(vertices.size());
	for(unsigned i=0; i < vertices.size(); ++i){
		d->vertices[i] = vertices[i].pos;
	}
	return true;
}

void PHFemMeshNew::SetDesc(const void* p){
	PHFemMeshNewDesc* d = (PHFemMeshNewDesc*)p;
	tets.clear();
	tets.resize(d->tets.size() / 4);
	vertices.clear();
	vertices.resize(d->vertices.size());
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j)
			tets[i].vertexIDs[j] = d->tets[i*4+j];
	}
	for(unsigned i=0; i<vertices.size(); ++i){
		vertices[i].pos = d->vertices[i];
		vertices[i].initialPos = vertices[i].pos;
		vertices[i].tetIDs.clear();
	}
	//	接続情報の更新
	//	頂点に属する四面体を追加
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j){
			vertices[tets[i].vertexIDs[j]].tetIDs.push_back(i);
		}
	}
	//	表面を探す
	std::vector<FemFace> allFaces;
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
			FemFace f;	
			for(unsigned k=0; k<3; ++k) f.vertexIDs[k] = tets[i].vertexIDs[tfs[j][k]];
			f.Update();
			allFaces.push_back(f);
		}
	}
	std::sort(allFaces.begin(), allFaces.end());

	faces.clear();
	std::vector<FemFace> ifaces;
	for(unsigned i=0; i<allFaces.size(); ++i){
		if (i+1<allFaces.size() && allFaces[i] == allFaces[i+1]){
			ifaces.push_back(allFaces[i]);	//	中面
			i++;
		}else{
			faces.push_back(allFaces[i]);	//	表面
		}
	}
	nSurfaceFace = (unsigned)faces.size();
	faces.insert(faces.end(), ifaces.begin(), ifaces.end());
	surfaceVertices.clear();
	//	表面の頂点の列挙
	for(unsigned i=0; i<nSurfaceFace; ++i){
		for(unsigned j=0; j<3; ++j){
			surfaceVertices.push_back(faces[i].vertexIDs[j]);
		}
	}
	std::sort(surfaceVertices.begin(), surfaceVertices.end());
	std::vector<int>::iterator newEnd = std::unique(surfaceVertices.begin(), surfaceVertices.end());
	surfaceVertices.erase(newEnd, surfaceVertices.end());

	//	辺の列挙
	//	まず表面の辺
	edges.clear();
	for(unsigned i=0; i<nSurfaceFace; ++i){
		for(unsigned j=0; j<3; ++j){
			edges.push_back(FemEdge(faces[i].vertexIDs[j], faces[i].vertexIDs[(j+1)%3]));
		}
	}
	std::sort(edges.begin(), edges.end());
	std::vector<FemEdge>::iterator newEEnd = std::unique(edges.begin(), edges.end());
	edges.erase(newEEnd, edges.end());
	nSurfaceEdge = (unsigned)edges.size();
	//	内部の辺の列挙
	std::vector<FemEdge> iEdges;
	for(unsigned i=nSurfaceFace; i<faces.size() ;++i){
		for(unsigned j=0; j<3; ++j){
			iEdges.push_back(FemEdge(faces[i].vertexIDs[j], faces[i].vertexIDs[(j+1)%3]));
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
			vertices[edges[i].vertexIDs[j]].edgeIDs.push_back(i);
		}
	}
	//	四面体に面を追加
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j){
			FemFace f;
			for(unsigned k=0; k<3; ++k) f.vertexIDs[k] = tets[i].vertexIDs[k<j ? k : k+1];
			f.Update();
			unsigned k;
			for(k=0; k<faces.size(); ++k){
				if (faces[k] == f){
					tets[i].faceIDs[j] = k;
					break;
				}
			}
			assert(k < faces.size());
		}
	}
	//	四面体に辺を追加
	for(unsigned i=0; i<tets.size(); ++i){
		int count = 0;
		for(unsigned j=0; j<4; ++j){
			FemVertex& vtx = vertices[tets[i].vertexIDs[j]];
			//	四面体のある頂点から出ている辺のうち、その頂点が始点(vertices[0])になっているものについて
			for(unsigned k=0; k<vtx.edgeIDs.size(); ++k){
				FemEdge& e = edges[vtx.edgeIDs[k]];
				if (e.vertexIDs[0] != tets[i].vertexIDs[j]) continue;
				//	辺が四面体に含まれる場合、辺を設定
				for(int l=0; l<4; ++l){
					if (e.vertexIDs[1] == tets[i].vertexIDs[l]){
						tets[i].edge(j, l) = vtx.edgeIDs[k];
						count ++;
						break;
					}
				}
			}
		}
		assert(count == 6);
	}
	//	頂点に属する面を追加
	for(unsigned i=0;i<faces.size();i++){
		for(unsigned j=0;j<3;j++){
			vertices[faces[i].vertexIDs[j]].faceIDs.push_back(i);
		}
	}

this->root = NULL; //Clear the mesh KDTree root

	int fs = (int) faces.size();
	for(int i=0;i<fs;i++){
		faces[i].centroid.x = (vertices[faces[i].vertexIDs[0]].pos.x + vertices[faces[i].vertexIDs[1]].pos.x + vertices[faces[i].vertexIDs[2]].pos.x)  /3.0f;
		faces[i].centroid.y = (vertices[faces[i].vertexIDs[0]].pos.y + vertices[faces[i].vertexIDs[1]].pos.y + vertices[faces[i].vertexIDs[2]].pos.y)  /3.0f;
		faces[i].centroid.z = (vertices[faces[i].vertexIDs[0]].pos.z + vertices[faces[i].vertexIDs[1]].pos.z + vertices[faces[i].vertexIDs[2]].pos.z)  /3.0f;
	}

	//Calculating the Area of each tetra face
	for(int i=0;i<fs;i++){
		Vec3d vec[2];
		vec[0] = vertices[faces[i].vertexIDs[1]].pos - vertices[faces[i].vertexIDs[0]].pos;  //vec[0] = pos[1] - pos[0];
		vec[1] = vertices[faces[i].vertexIDs[2]].pos - vertices[faces[i].vertexIDs[0]].pos;  //vec[1] = pos[2] - pos[0];
		
		Vec3d normal = vec[1] % vec[0];
		
		faces[i].area = sqrt (( normal.x * normal.x ) + (normal.y * normal.y) + (normal.z * normal.z)) / 2.0f;
	}

	//Calculating normal
	for(int i=0;i<fs;i++){
		faces[i].normal = this->CompFaceNormal(i, false);
	}

	//This code calculates the distance from the vertex to the 
	//closest nieghbor face centroid
	int nv = NVertices();
	for (int i=0; i< nv ;i++){
		int nf = (int) vertices[i].faceIDs.size();
		double maxDist = DBL_MIN;

		for (int j=0; j< nf ;j++) {
			int face = vertices[i].faceIDs[j];

			if (face > (int) nSurfaceFace) { continue; }

			for (int k=0; k<3 ;k++) {
				int vid = faces[face].vertexIDs[k];

				if (vid == i) { continue; }

				double dd = (vertices[vid].pos - vertices[i].pos).norm();
				if (dd > maxDist ) {
					maxDist = dd;
				}
			}
		}
		vertices[i].centerDist = maxDist;
	}

	//for the faces in the surface saves the correspondant tetId 
	for (int i=0; i < (int) nSurfaceFace; i++) {
		faces[i].tetraId = FindTetFromFace(i);
	}

	//saves if the mesh is spheric or not from the sprfile flag
	this->spheric = d->spheric;
}

bool PHFemMeshNew::AddChildObject(ObjectIf* o){
	bool bCheck = false;
	PHFemVibration* fVib = DCAST(PHFemVibration, o);
	if(fVib){
		femVibration = fVib;
		bCheck = true;
	}

	PHFemThermo* fThermo = DCAST(PHFemThermo, o);
	if(fThermo){
		femThermo = fThermo;	
		bCheck = true;
	}

	PHFemBase* f = DCAST(PHFemBase, o);
	if(f){
		f->SetPHFemMesh(this);
		femBases.push_back(f);
	}
	return bCheck;
}

void PHFemMeshNew::Init(){
	for(PHFemBases::iterator it = femBases.begin(); it != femBases.end(); it++)
		(*it)->Init();
}

void PHFemMeshNew::Step(double dt){
	for(PHFemBases::iterator it = femBases.begin(); it != femBases.end(); it++)
		(*it)->Step();
}

void PHFemMeshNew::SetPHSolid(PHSolidIf* s){
	solid = s;
}

PHSolidIf* PHFemMeshNew::GetPHSolid(){
	return solid;
}

PHFemVibrationIf* PHFemMeshNew::GetPHFemVibration(){
	return femVibration->Cast();
}

PHFemThermoIf* PHFemMeshNew::GetPHFemThermo(){
	return femThermo->Cast();
}

int PHFemMeshNew::NVertices(){
	return (int)vertices.size();
}

int PHFemMeshNew::NFaces(){
	return (int)faces.size();
}

///////////////////////////////////////////////////////////////////////////////////////////
//* 頂点に関する関数 */
Vec3d PHFemMeshNew::GetVertexInitalPositionL(int vtxId){
	if(0 <= vtxId && vtxId <= (int)vertices.size() -1){
		return vertices[vtxId].initialPos;
	}
	return Vec3d();
}

Vec3d PHFemMeshNew::GetVertexPositionL(int vtxId){
	if(0 <= vtxId && vtxId <= (int)vertices.size() -1){
		return vertices[vtxId].pos;
	}
	return Vec3d();
}

Vec3d PHFemMeshNew::GetVertexDisplacementL(int vtxId){
	if(0 <= vtxId && vtxId <= (int)vertices.size() -1){
		return vertices[vtxId].pos - vertices[vtxId].initialPos;
	}
	return Vec3d();
}

Vec3d PHFemMeshNew::GetVertexVelocityL(int vtxId){
	if(0 <= vtxId && vtxId <= (int)vertices.size() -1){
		return vertices[vtxId].vel;
	}
	return Vec3d();
}

bool PHFemMeshNew::AddVertexDisplacementW(int vtxId, Vec3d disW){
	if(0 <= vtxId && vtxId <= (int)vertices.size() -1){
		vertices[vtxId].pos += GetPHSolid()->GetPose().Inv() * disW;
		return true;
	}
	return false;
}

bool PHFemMeshNew::AddVertexDisplacementL(int vtxId, Vec3d disL){
	if(0 <= vtxId && vtxId <= (int)vertices.size() -1){
		vertices[vtxId].pos += disL;
		return true;
	}
	return false;
}

bool PHFemMeshNew::SetVertexPositionW(int vtxId, Vec3d posW){
	if(0 <= vtxId && vtxId <= (int)vertices.size() -1){
		vertices[vtxId].pos = GetPHSolid()->GetPose().Inv() * posW;
		return true;
	}
	return false;
}

bool PHFemMeshNew::SetVertexPositionL(int vtxId, Vec3d posL){
	if(0 <= vtxId && vtxId <= (int)vertices.size() -1){
		vertices[vtxId].pos = posL;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////
//* 四面体に関する関数 */

double PHFemMeshNew::CompTetVolume(const Vec3d pos[4]){
	TMatrixRow< 4, 4, double > mat;
	mat.clear(0.0);
	for(int i = 0; i < 4; i++){
		mat[i][0] = 1.0;
		mat[i][1] =	pos[i].x;
		mat[i][2] = pos[i].y;
		mat[i][3] = pos[i].z;
	}
	double volume = mat.det() / 6.0;
	if(volume < 0.0) volume = 0.0;
	return volume;
}

double PHFemMeshNew::CompTetVolume(const int& tetID, const bool& bDeform){
	Vec3d pos[4];
	for(int i = 0; i < 4; i++){
		if(bDeform)		pos[i] = vertices[tets[tetID].vertexIDs[i]].pos;
		else			pos[i] = vertices[tets[tetID].vertexIDs[i]].initialPos;
	}
	return CompTetVolume(pos);
}

TMatrixRow< 4, 4, double > PHFemMeshNew::CompTetShapeFunctionCoeff(Vec3d pos[4]){
	PTM::TMatrixRow< 4, 4, double > matPos;
	for(int i = 0; i < 4; i++){
		matPos.item(i, 0) = 1.0;
		matPos.item(i, 1) = pos[i].x;
		matPos.item(i, 2) = pos[i].y;
		matPos.item(i, 3) = pos[i].z;
	}
	PTM::TMatrixRow< 4, 4, double > funcCoeff;		// matの余因子行列の転置
	funcCoeff = (matPos.det() * matPos.inv()).trans();
	return funcCoeff;
}

TMatrixRow< 4, 4, double > PHFemMeshNew::CompTetShapeFunctionCoeff(const int& tetId, const bool& bDeform){
	Vec3d pos[4];
	for(int i = 0; i < 4; i++){
		if(bDeform)		pos[i] = vertices[tets[tetId].vertexIDs[i]].pos;
		else			pos[i] = vertices[tets[tetId].vertexIDs[i]].initialPos;
	}
	return CompTetShapeFunctionCoeff(pos);
}

bool PHFemMeshNew::CompTetShapeFunctionValue(const TMatrixRow< 4, 4, double >& sf, const double& vol, const Vec3d& posL, Vec4d& value){
	bool bCorrect = true;
	double volInv = 1.0 / (6.0 * vol);
	for(int i = 0; i < 4; i++){
		value[i] = volInv * (sf[i][0] + sf[i][1] * posL.x + sf[i][2] * posL.y + sf[i][3] * posL.z);
		if(abs(value[i]) < 1e-10) value[i] = 0;		// valueが閾値内の場合の補正(*posが四面体の辺、頂点付近にある場合数値誤差で-になることがある）
		if(value[i] < 0) bCorrect = false;
	}
	return bCorrect;
}

bool PHFemMeshNew::CompTetShapeFunctionValue(const int& tetId, const Vec3d& posL, Vec4d& value, const bool& bDeform){
	TMatrixRow< 4, 4, double > shapeFunc = CompTetShapeFunctionCoeff(tetId, bDeform);
	double vol = CompTetVolume(tetId, bDeform);
	return CompTetShapeFunctionValue(shapeFunc, vol, posL, value);
}

int PHFemMeshNew::FindTetFromFace(int faceId){
	for(int i = 0; i < (int)tets.size(); i++){
		for(int j = 0; j < 4; j++){
			if(faceId == tets[i].faceIDs[j]){
				return i;
			}
		}
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////
//* 面に関する関数 */
std::vector< Vec3d > PHFemMeshNew::GetFaceEdgeVtx(unsigned id){
	std::vector<Vec3d> fev;
	for(unsigned i=0;i<3;i++){
		fev.push_back(vertices[faces[id].vertexIDs[i]].pos);
	}
	return fev;
}

Vec3d PHFemMeshNew::GetFaceEdgeVtx(unsigned id, unsigned vtx){
	return vertices[faces[id].vertexIDs[vtx]].pos;
}

double PHFemMeshNew::CompFaceArea(const Vec3d pos[3]){
	Vec3d vec[2];
	vec[0] = pos[1] - pos[0];
	vec[1] = pos[2] - pos[0];
	double n[2];
	n[0] = vec[0].norm();	n[1] = vec[1].norm();
	double S = 0.5 * sqrt(pow(n[0], 2) * pow(n[1], 2) - pow((vec[0] * vec[1]), 2));
	if(S < 0.0) S = 0.0;
	return S;
}

double PHFemMeshNew::CompFaceArea(const int& faceId, const bool& bDeform){
	Vec3d pos[3];
	for(int i = 0; i < 3; i++){
		if(bDeform)		pos[i] = vertices[faces[faceId].vertexIDs[i]].pos;
		else			pos[i] = vertices[faces[faceId].vertexIDs[i]].initialPos;
	}
	return CompFaceArea(pos);
}

Vec3d PHFemMeshNew::CompFaceNormal(const Vec3d pos[3]){
	Vec3d vec[2];
	vec[0] = pos[1] - pos[0];
	vec[1] = pos[2] - pos[0];
	// FemMeshの頂点は表面から見て時計周りに並ぶ
	// 表面方向の法線は外積は反時計まわりにかける
	return (vec[1] % vec[0]).unit();
}

Vec3d PHFemMeshNew::CompFaceNormal(const int& faceId, const bool& bDeform){
	Vec3d pos[3];
	for(int i = 0; i < 3; i++){
		if(bDeform)		pos[i] = vertices[faces[faceId].vertexIDs[i]].pos;
		else			pos[i] = vertices[faces[faceId].vertexIDs[i]].initialPos;
	}
	return CompFaceNormal(pos);
}



}
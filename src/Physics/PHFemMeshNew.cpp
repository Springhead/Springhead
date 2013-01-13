/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Physics/PHFemMeshNew.h>
#include <Physics/PHFemVibration.h>

//using namespace PTM;
namespace Spr{;

///////////////////////////////////////////////////////////////////
/* 計算モジュールの共通部分 */
int PHFem::NVertices(){ return GetPHFemMesh()->NVertices(); }

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
}

bool PHFemMeshNew::AddChildObject(ObjectIf* o){
	bool bCheck = false;
	PHFemVibration* fVib = DCAST(PHFemVibration, o);
	if(fVib){
		femVibration = fVib;
		bCheck = true;
	}

/*	PHFemThermo* fThermo = DCAST(PHFemThermo, o);
	if(fThermo){
		femThermo = fThermo;	
		bCheck = true;
	}*/ 

	PHFem* f = DCAST(PHFem, o);
	if(f){
		f->SetPHFemMesh(this);
		fems.push_back(f);
	}
	return bCheck;
}

void PHFemMeshNew::Init(){
	for(PHFems::iterator it = fems.begin(); it != fems.end(); it++)
		(*it)->Init();
}

void PHFemMeshNew::Step(double dt){
	for(PHFems::iterator it = fems.begin(); it != fems.end(); it++)
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

int PHFemMeshNew::NVertices(){
	return (int)vertices.size();
}

int PHFemMeshNew::NFaces(){
	return (int)faces.size();
}

///////////////////////////////////////////////////////////////////////////////////////////
//* 頂点に関する関数 */
Vec3d PHFemMeshNew::GetVertexInitPositionL(int vtxId){
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
#define DET2_INV_TMATRIXBASE(a,b,c,d)	(a*d - b*c)
inline Matrix3d invDet(const Matrix3d& a){
	Matrix3d rtv;
	rtv.item(0,0) = DET2_INV_TMATRIXBASE(a.item(1,1), a.item(1,2), a.item(2,1), a.item(2,2));
	rtv.item(1,0) = DET2_INV_TMATRIXBASE(a.item(1,2), a.item(1,0), a.item(2,2), a.item(2,0));
	rtv.item(2,0) = DET2_INV_TMATRIXBASE(a.item(1,0), a.item(1,1), a.item(2,0), a.item(2,1));
		
	rtv.item(0,1) = DET2_INV_TMATRIXBASE(a.item(2,1), a.item(2,2), a.item(0,1), a.item(0,2));
	rtv.item(1,1) = DET2_INV_TMATRIXBASE(a.item(2,2), a.item(2,0), a.item(0,2), a.item(0,0));
	rtv.item(2,1) = DET2_INV_TMATRIXBASE(a.item(2,0), a.item(2,1), a.item(0,0), a.item(0,1));
	
	rtv.item(0,2) = DET2_INV_TMATRIXBASE(a.item(0,1), a.item(0,2), a.item(1,1), a.item(1,2));
	rtv.item(1,2) = DET2_INV_TMATRIXBASE(a.item(0,2), a.item(0,0), a.item(1,2), a.item(1,0));
	rtv.item(2,2) = DET2_INV_TMATRIXBASE(a.item(0,0), a.item(0,1), a.item(1,0), a.item(1,1));
	return rtv;
}

void PHFemMeshNew::UpdateJacobian(){
	for(unsigned t=0; t<tets.size(); ++t){
		Matrix3d J;	///<	各四面体の直交座標系(ξ,η,ζ)から四面体(x,y,z)へのヤコビアン (d(x,y,z) / d(ξ,η,ζ))
		for(int i=1; i<3; ++i){
			for(int j=0; j<3; ++j){
				J[i][j] = vertices[tets[t].vertexIDs[i+1]].pos[j] - vertices[tets[t].vertexIDs[0]].pos[j];
			}
		}
		Matrix3d A = invDet(J);
		Vec4d Nx = Vec4d(- A[0][0] - A[0][1] -A[0][2],  A[0][0],  A[0][1],  A[0][2]);
		Vec4d Ny = Vec4d(- A[1][0] - A[1][1] -A[1][2],  A[1][0],  A[1][1],  A[1][2]);
		Vec4d Nz = Vec4d(- A[2][0] - A[2][1] -A[2][2],  A[2][0],  A[2][1],  A[2][2]);
		Affined Km;
		for(int i=0; i<4; ++i){
			for(int j=0; j<4; ++j){
				Km[i][j] = Nx[i]*Nx[j] + Ny[i]*Ny[j] + Nz[i]*Nz[j];
			}
		}
	}
}

double PHFemMeshNew::CompTetVolume(int tetID){
	TMatrixRow< 4, 4, double > mat;
	mat.clear(0.0);
	for(int i = 0; i < 4; i++){
		mat[i][0] = 1.0;
		mat[i][1] =	vertices[tets[tetID].vertexIDs[i]].pos.x;
		mat[i][2] = vertices[tets[tetID].vertexIDs[i]].pos.y;
		mat[i][3] = vertices[tets[tetID].vertexIDs[i]].pos.z;
	}
	double volume = mat.det() / 6.0;
	if(volume < 0.0) volume = 0.0;
	return volume;
}

TMatrixRow< 4, 4, double > PHFemMeshNew::CompTetShapeFunctionCoeff(int tetId){
	PTM::TMatrixRow< 4, 4, double > matPos;
	for(int i = 0; i < 4; i++){
		Vec3d pos = vertices[tets[tetId].vertexIDs[i]].pos;
		matPos.item(i, 0) = 1.0;
		matPos.item(i, 1) = pos[0];
		matPos.item(i, 2) = pos[1];
		matPos.item(i, 3) = pos[2];
	}
	PTM::TMatrixRow< 4, 4, double > funcCoeff;		// matの余因子行列
	funcCoeff = (matPos.det() * matPos.inv()).trans();
	return funcCoeff;
}

bool PHFemMeshNew::CompTetShapeFunctionValue(int tetId, Vec3d pos, Vec4d& value){
	bool bCorrect = true;
	TMatrixRow< 4, 4, double > matCofact;
	matCofact = CompTetShapeFunctionCoeff(tetId);
	for(int i = 0; i < 4; i++){
		value[i] = matCofact[i][0] + matCofact[i][1] * pos.x + 	matCofact[i][2] * pos.y + 	matCofact[i][3] * pos.z;
		if(value[i] < 0) bCorrect = false;
	}
	return bCorrect;
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

double PHFemMeshNew::CompFaceArea(int faceId){
	return CompFaceNormal(faceId).norm() * 0.5;
}

Vec3d PHFemMeshNew::CompFaceNormal(int faceId){
	FemFace face = faces[faceId];
	Vec3d pos[3];
	for(int i = 0; i < 3; i++){
		pos[i] = vertices[face.vertexIDs[i]].pos;
	}
	Vec3d vec[2];
	vec[0] = pos[1] - pos[0];
	vec[1] = pos[2] - pos[0];
	// 頂点は表面から見て時計周り
	// 外積は反時計まわりにかける
	return (vec[1] % vec[0]).unit();
}




}
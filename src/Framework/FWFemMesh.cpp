#include "Framework.h"
#include <Framework/SprFWFemMesh.h>
#include <Graphics/GRFrame.h>
#include <Physics/PHConstraint.h>
#include <Foundation/UTLoadHandler.h>
#include "FWFemMesh.h"
#include "tetgen.h"
#include "ThermalFEM.h"


#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace std;
using namespace Spr;

namespace Spr{;

FWFemMesh::FWFemMesh(const FWFemMeshDesc& d):grMesh(NULL){
	SetDesc(&d);
}
size_t FWFemMesh::NChildObject() const{
	return FWObject::NChildObject() + (grMesh ? 1 : 0);
}
ObjectIf* FWFemMesh::GetChildObject(size_t pos){
	if (pos < FWObject::NChildObject()){
		return FWObject::GetChildObject(pos);
	}
	if (grMesh && pos < NChildObject()) return grMesh->Cast();
	return NULL;
}
bool FWFemMesh::AddChildObject(ObjectIf* o){
	GRMesh* mesh = o->Cast();
	if (mesh){
		grMesh = mesh;
		return true;
	}else{
		return FWObject::AddChildObject(o);
	}
}
void FWFemMesh::Loaded(UTLoadContext*){
	if (!phMesh) GeneratePHFemMesh();
	if (grFrame){
		grFrame->DelChildObject(grMesh->Cast());
		grFrame->AddChildObject(CreateGRFromPH()->Cast());
	}
}
bool FWFemMesh::GeneratePHFemMesh(){
	//	呼び出された時点で grMesh にグラフィクスのメッシュが入っている
	//	grMeshを変換して、phMeshをつくる。
	std::cout << "メッシュ生成" << std::endl;
	
	//	以下で、grMeshからtetgenを呼び出して変換して、pmdに値を入れていけば良い。


	PHFemMeshDesc pmd;
	
	//TetGenで四面体メッシュ化


	//定義を加えながら変換していく
	//int i;
	//頂点の開始番号
	ThermalFEM FEM;
	tetgenio::facet *f;
	tetgenio::polygon *p;

	FEM.in.firstnumber = 0;
	////頂点座標と数の入力
	FEM.in.numberofpoints = grMesh->NVertex();		//grMeshの頂点サイズの代入
	FEM.in.pointlist = DBG_NEW REAL[FEM.in.numberofpoints * 3];
	for(int j=0; j < grMesh->NVertex(); j++){	//ThermoMeshの頂点番号はj / 3の整数部分
		for(int k=0; k<3; ++k)
			FEM.in.pointlist[j*3+k] = grMesh->GetVertices()[j][k] ;
	}
	//for(int j=0; j < grMesh->NVertex(); j++){	//ThermoMeshの頂点番号はj / 3の整数部分
	//	for(int k=0; k<3; ++k){
	//		DSTR << FEM.in.pointlist[j*3+k];
	//	}
	//	DSTR << std::endl;
	//}
	//-0.50.50.5
	//0.50.50.5
	//0.5-0.50.5
	//-0.5-0.50.5
	//0.50.5-0.5
	//0.5-0.5-0.5
	//-0.50.5-0.5
	//-0.5-0.5-0.5

	////面の数の代入
	FEM.in.numberoffacets = grMesh->NFace();
	FEM.in.facetlist = DBG_NEW tetgenio::facet[FEM.in.numberoffacets];
	FEM.in.facetmarkerlist = DBG_NEW int[FEM.in.numberoffacets];
	////面の情報の入力
	GRMeshFace* faces = grMesh->GetFaces();
	for(int j =0; j < FEM.in.numberoffacets ; j++){
		f = &FEM.in.facetlist[(int)j];
		f->numberofpolygons = 1;
		f->polygonlist = DBG_NEW tetgenio::polygon[f->numberofpolygons];
		f->numberofholes = 0;
		f->holelist = NULL;
		p = &f->polygonlist[0];
		p->numberofvertices = faces[j].nVertices;
		p->vertexlist = DBG_NEW int[p->numberofvertices];
		for(int k =0; k < p->numberofvertices; k++){
			p->vertexlist[k] = faces[j].indices[k];
		}
	}
	for(int j = 0; j < FEM.in.numberoffacets ;j++){
		FEM.in.facetmarkerlist[j] = 0;
	}
	//FEM.in.save_nodes("barpq1.4a1.0in");
	//FEM.in.save_poly("barpq1.4a1.0in");
	//FEM.in.save_elements("barpq1.4a0.5in");
	////四面体メッシュ化
	//FEM.TFEMTetrahedralize("pqa100");
	////メッシュ化したファイルの保存
	//FEM.out.save_nodes("barpq1.4a0.01out");			
	//FEM.out.save_elements("barpqa100out");
	//FEM.out.save_faces("barpqa0.5out");
	//return FEM.outに入っているメッシュファイル⇒これをPHに入れる
	FEM.in.save_nodes("barpqin");
	FEM.in.save_poly("barpqain");
	FEM.in.save_elements("barpqain");

	FEM.TFEMTetrahedralize("pq1.1a0.0001");
	
	FEM.out.save_nodes("spherepq1.1a0.0001out");			
	FEM.out.save_elements("spherepq1.1a0.0001out");
	FEM.out.save_faces("spherepq1.1a0.0001out");

	
	//pmdに値を入れていく
	for(int i=0; i < FEM.out.numberofpoints; i++){
		pmd.vertices.push_back(Vec3d(FEM.out.pointlist+i*3));
	} 
	pmd.tets.assign(FEM.out.tetrahedronlist, FEM.out.tetrahedronlist + FEM.out.numberoftetrahedra*4);
	
	//phMeshに必要な値は何？熱計算に必要な物理量PHのソリッド関係は、SPRのものを使う。
	//熱伝導の処理を書き加えながら、必要なパラメータを、descに加えて行く。
	phMesh = DBG_NEW PHFemMesh(pmd);
	
	return true;
}

GRMesh* FWFemMesh::CreateGRFromPH(){
	//	頂点の対応表を用意
	std::vector<int> vtxMap;
	vtxMap.resize(phMesh->vertices.size(), -1);
	for(unsigned i=0; i<phMesh->surfaceVertices.size(); ++i){
		vtxMap[phMesh->surfaceVertices[i]] = i;
	}
	GRMeshDesc gmd;	//	新しいGRMeshのデスクリプタ
	//	表面の頂点を設定
	for(unsigned i=0; i< phMesh->surfaceVertices.size(); ++i) 
		gmd.vertices.push_back(phMesh->vertices[ phMesh->surfaceVertices[i] ].pos);
	//	表面の三角形を設定
	for(unsigned i=0; i< phMesh->surfaceFaces.size(); ++i) {
		GRMeshFace f;
		f.nVertices = 3;
		for(int j=0; j<3; ++j){
			f.indices[j] = vtxMap[phMesh->surfaceFaces[i].vertices[j]];
		}
		gmd.faces.push_back(f);
	}
	//	phMeshの頂点に対応するGRMeshの三角形と３点の重みづけを求める。
	unsigned* gTris = grMesh->GetTriangles();
	Vec3f* gVtxs = grMesh->GetVertices();
	
	std::vector<Vec3f> gNormals, gWalls, gDirs;
	std::vector<double> gEdgeLens;
	gNormals.resize(grMesh->NTriangle());
	gDirs.resize(gNormals.size() * 3);
	gWalls.resize(gNormals.size() * 3);
	gEdgeLens.resize(gNormals.size() * 3);
	for(int j=0; j<grMesh->NTriangle(); ++j){
		gNormals[j] = ((gVtxs[gTris[j*3+2]] - gVtxs[gTris[j*3]]) % (gVtxs[gTris[j*3+1]] - gVtxs[gTris[j*3]])).unit();
		for(int k=0; k<3; ++k){
			gDirs[j*3+k] = (gVtxs[gTris[j*3+(k+1)%3]] - gVtxs[gTris[j*3+k]]).unit();
			gWalls[j*3+k] = (gNormals[j] % gDirs[j*3+k]).unit();
		}
	}
	std::vector<double> dists;
	std::vector<int> nearestTris;
	dists.resize(phMesh->surfaceVertices.size());
	nearestTris.resize(phMesh->surfaceVertices.size());
	for(unsigned i=0; i<phMesh->surfaceVertices.size(); ++i){
		Vec3f pos = phMesh->vertices[phMesh->surfaceVertices[i]].pos;
		double minDist = DBL_MAX;
		int minId = -1;
		for(int j=0; j<grMesh->NTriangle(); ++j){
			double h = (pos - gVtxs[gTris[j*3]]) * gNormals[j];
			Vec3f ph = pos - h*gNormals[j];
			double dist;
			int k;
			for(k=0; k<3; ++k){
				double d = (ph - gVtxs[gTris[j*3+k]]) * gWalls[j*3+k];
				if (d > 0){
					Vec3d pw = ph - d*gWalls[j*3+k];
					double dl = (pw - gVtxs[gTris[j*3]+k]) * gDirs[gTris[j*3]+k];
					if (dl < 0){
						dist = sqrt(h*h + d*d + dl*dl);
					}else if (dl > gEdgeLens[gTris[j*3]+k]){
						dl -= gEdgeLens[gTris[j*3]+k];
						dist = sqrt(h*h + d*d + dl*dl);
					}else{
						dist = sqrt(h*h + d*d);
					}
					break;
				}
			}
			if (k == 3){
				dist = h;
			}
			if (dist < minDist){
				minDist = dist;
				minId = j;
			}
		}
		dists[i] = minDist;
		nearestTris[i] = minId;
	}
	std::vector<Vec3d> weights;
	weights.resize(phMesh->surfaceVertices.size());
	for(unsigned i=0; i<phMesh->surfaceVertices.size(); ++i){
		Matrix3f vtxs;
		for(unsigned j=0; j<3; ++j){
			vtxs[j] = gVtxs[gTris[nearestTris[i]*3+j]];
		}
		int tmp[3];
		vtxs.trans().gauss(weights[i], phMesh->vertices[phMesh->surfaceVertices[i]].pos, tmp);
	}
	//	求めた重みにしたがって、テクスチャ座標を決める
	for(unsigned i=0; i<gmd.vertices.size(); ++i){
		gmd.texCoords.resize(gmd.vertices.size(), Vec2f());
		for(int j=0; j<3; ++j) gmd.texCoords[i] += weights[i][j] * grMesh->texCoords[gTris[nearestTris[i]*3+j]];
	}
	//	求めた重みにしたがって、法線ベクトルを決める
	if (grMesh->normals.size() == grMesh->vertices.size()){
		gmd.normals.resize(gmd.vertices.size(), Vec2f());
		for(unsigned i=0; i<gmd.vertices.size(); ++i){
			for(int j=0; j<3; ++j) gmd.normals[i] += weights[i][j] * grMesh->normals[gTris[nearestTris[i]*3+j]];
		}
	}
	//	一番近くの面のマテリアルを設定したい。
	//	まず三角形と面の対応表を作る
	std::vector<int> gTri2FaceMap;
	for(unsigned i=0; i<grMesh->faces.size(); ++i){
		gTri2FaceMap.push_back(i);
		if (grMesh->faces[i].nVertices == 4){
			gTri2FaceMap.push_back(i);
		}
	}
	//	一番近い面のマテリアルを適用する。
	if (!grMesh->materialList.empty())
		//	頂点に一番近い面を見つけ、多数決を取る
		for(unsigned i=0; i<gmd.faces.size(); ++i){
			int nearestFace[3];
			for(unsigned j=0; j<3; ++j) nearestFace[j] = gTri2FaceMap[nearestTris[gmd.faces[i].indices[j]]];
			if (nearestFace[1] == nearestFace[2]) gmd.materialList.push_back(grMesh->materialList[nearestFace[1]]);
			else gmd.materialList.push_back(grMesh->materialList[nearestFace[0]]);
		}
	//	GRMeshを作成
	GRMesh* rv = grMesh->GetNameManager()->CreateObject(GRMeshIf::GetIfInfoStatic(), &gmd)->Cast();
	//	マテリアルの追加
	for (unsigned i=0; i<grMesh->NChildObject(); ++i){
		rv->AddChildObject(grMesh->GetChildObject(i));
	}
	//	テクスチャモードをコピー
	rv->tex3d = grMesh->tex3d;
	return rv;
}

}

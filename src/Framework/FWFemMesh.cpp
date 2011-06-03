#include "Framework.h"
#include <Framework/SprFWFemMesh.h>
#include <Graphics/GRFrame.h>
#include <Physics/PHConstraint.h>
#include <Foundation/UTLoadHandler.h>
#include "FWFemMesh.h"
#include "tetgen.h"
#include "ThermalFEM.h"


#include <Collision/CDQuickHull2DImp.h>


#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#ifdef _DEBUG
# define PDEBUG(x)	x
#else
# define PDEBUG(x)
#endif

namespace Spr{;

FWFemMesh::FWFemMesh(const FWFemMeshDesc& d):grMesh(NULL){
	SetDesc(&d);
}
void FWFemMesh::Sync(bool ph2gr){	
	//	テスト用
	static double value, delta;
	if (value <= 0) delta = 0.01;
	if (value >= 1) delta = -0.01;
	value += delta;

	//	同期処理
	FWObject::Sync(ph2gr);
	if (ph2gr && grMesh->IsTex3D()){
		float* gvtx = grMesh->GetVertexBuffer();
		if (gvtx){
			int tex = grMesh->GetTexOffset();
			int stride = grMesh->GetStride();
			for(unsigned gv=0; gv<vertexIdMap.size(); ++gv){
				int pv = vertexIdMap[gv];
				//	PHから何らかの物理量を取ってくる
				phMesh->vertices[pv].integralSelf.volume;
				//	GRのテクスチャ座標として設定する。	s t r q の rを設定
				gvtx[stride*gv + tex + 2] = value + gvtx[stride*gv];
			}
		}	
	}else{
		assert(0);	//	not supported.
	}
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
	if (!phMesh) CreatePHFromGR();
	if (grFrame){
		grFrame->DelChildObject(grMesh->Cast());
		CreateGRFromPH();
		grFrame->AddChildObject(grMesh->Cast());
	}
}
bool FWFemMesh::CreatePHFromGR(){
	//	呼び出された時点で grMesh にグラフィクスのメッシュが入っている
	//	grMeshを変換して、phMeshをつくる。

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

	FEM.TFEMTetrahedralize("pq1.5a0.0001");
	
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

void FWFemMesh::CreateGRFromPH(){
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
	//	phMeshの三角形とgrMeshの三角形の対応表をつくる	重なっている面積が最大のものが対応する面
	//	まず、法線が近いものを探し、面1と面2上の頂点の距離が近いものに限り、重なっている面積を求める。
	std::vector<Vec3f> pnormals(gmd.faces.size());
	for(unsigned pf=0; pf<gmd.faces.size(); ++pf){
		assert(gmd.faces[pf].nVertices == 3);
		pnormals[pf] = ((gmd.vertices[gmd.faces[pf].indices[2]] - gmd.vertices[gmd.faces[pf].indices[0]]) %
		(gmd.vertices[gmd.faces[pf].indices[1]] - gmd.vertices[gmd.faces[pf].indices[0]])).unit();
	}
	std::vector<Vec3f> gnormals(grMesh->faces.size());
	struct FaceWall{
		Vec3f wall[4];
	};
	std::vector<FaceWall> gWalls(gnormals.size());
	for(unsigned gf=0; gf<gnormals.size(); ++gf){
		gnormals[gf] = ((grMesh->vertices[grMesh->faces[gf].indices[2]] - grMesh->vertices[grMesh->faces[gf].indices[0]]) %
			(grMesh->vertices[grMesh->faces[gf].indices[1]] - grMesh->vertices[grMesh->faces[gf].indices[0]])).unit();
		int nv = grMesh->faces[gf].nVertices;
		for(int i=0; i<nv; ++i){
			gWalls[gf].wall[i] = ((grMesh->vertices[grMesh->faces[gf].indices[(i+1)%nv]] - grMesh->vertices[grMesh->faces[gf].indices[i]]) % gnormals[gf]).unit();
		}
	}
	std::vector<int> pFaceMap(pnormals.size());
	for(unsigned pf=0; pf<pnormals.size(); ++pf){
		std::vector<int> cand;
		for(unsigned gf=0; gf<gnormals.size(); ++gf){
			if (pnormals[pf] * gnormals[gf] > 0.8){	//	法線が遠いのはだめ
				int i;
				for(i=0; i<3; ++i){
					double d = gnormals[gf] * (gmd.vertices[gmd.faces[pf].indices[i]] - grMesh->vertices[grMesh->faces[gf].indices[0]]);
					if (d*d > 0.01) break;	//	距離が離れすぎているのはだめ
				}	
				if (i==3){
					//	phの三角形の重心が、grの中の面に入っている物をさがす
					Vec3f center;
					for(int v=0;v<3; ++v) center += gmd.vertices[gmd.faces[pf].indices[v]];
					center /= 3;
					int v;
					for(v=0; v<grMesh->faces[gf].nVertices; ++v){
						double d = gWalls[gf].wall[v] * (center - grMesh->vertices[grMesh->faces[gf].indices[v]]);
						if (d<0) break;
					}
					if (v == grMesh->faces[gf].nVertices){
						pFaceMap[pf] = gf;
						//	DEBUG出力
						//	DSTR << "center:" << center <<std::endl;
						//	for(int v=0; v<grMesh->faces[gf].nVertices; ++v)
						//		DSTR << "vtx" << grMesh->vertices[grMesh->faces[gf].indices[v]] << "wall:" << gWalls[gf].wall[v] << std::endl;
						break;
					}
				}
			}
		}
	}
	PDEBUG(	DSTR << "FaceMap PHtoGR:"; )
	PDEBUG(	for(unsigned i=0; i<pFaceMap.size(); ++i) DSTR << pFaceMap[i] << " "; )
	PDEBUG(	DSTR << std::endl; )
	//	対応表に応じてマテリアルリストを設定。
	gmd.materialList.resize(grMesh->materialList.size() ? pFaceMap.size() : 0);
	for(unsigned pf=0; pf<gmd.materialList.size(); ++pf){
		gmd.materialList[pf] = grMesh->materialList[pFaceMap[pf]];
	}
	//	新しく作るGRMeshの頂点からphMeshの頂点への対応表
	vertexIdMap.resize(gmd.vertices.size(), -1);
	//	対応表に応じて、頂点のテクスチャ座標を作成
	//		phの１点がgrの頂点複数に対応する場合がある。
	//		その場合は頂点のコピーを作る必要がある。
	std::vector<bool> vtxUsed(gmd.vertices.size(), false);
	for(unsigned pf=0; pf<pFaceMap.size(); ++pf){		
		for(unsigned i=0; i<3; ++i){
			int pv = gmd.faces[pf].indices[i];
			//	テクスチャ座標を計算
			Vec2f texCoord;
			Vec3f normal;
			GRMeshFace& gFace = grMesh->faces[pFaceMap[pf]];
			GRMeshFace* gNormal = NULL;
			if (grMesh->normals.size()){
				gNormal = &gFace;
				if (grMesh->faceNormals.size()) gNormal = &grMesh->faceNormals[pFaceMap[pf]];
			}
			if (gFace.nVertices == 3){
				Vec3f weight;
				Matrix3f vtxs;
				for(unsigned j=0; j<3; ++j){
					vtxs.col(j) = grMesh->vertices[gFace.indices[j]];
				}
				int tmp[3];
				vtxs.gauss(weight, gmd.vertices[pv], tmp);
				for(unsigned j=0; j<3; ++j){
					assert(weight[j] > -0.001);
					texCoord += weight[j] * grMesh->texCoords[gFace.indices[j]];
					if(gNormal) normal += weight[j] * grMesh->normals[gNormal->indices[j]];
				}
			}else{	//	4頂点
				//	どの３頂点で近似すると一番良いかを調べ、その３頂点を補間
				Vec3f weight[4];
				Matrix3f vtxs[4];
				double wMin[4];
				double wMinMax = -DBL_MAX;
				int maxId=-1;
				for(int j=0; j<4; ++j){
					for(int k=0; k<3; ++k){
						vtxs[j].col(k) = grMesh->vertices[gFace.indices[k<j ? k : k+1]];
					}
					int tmp[3];
					vtxs[j].gauss(weight[j], gmd.vertices[pv], tmp);
					wMin[j] = DBL_MAX;
					for(int l=0; l<3; ++l) if (wMin[j] > weight[j][l]) wMin[j] = weight[j][l];
					if (wMin[j] > wMinMax){
						wMinMax = wMin[j];
						maxId = j;
					}
				}
				for(int j=0; j<3; ++j){
					texCoord += weight[maxId][j] * grMesh->texCoords[gFace.indices[j<maxId?j:j+1]];
					if(gNormal)
						normal += weight[maxId][j] * grMesh->normals[gFace.indices[j<maxId?j:j+1]];
				}
			}
			gmd.texCoords.resize(gmd.vertices.size());
			if (grMesh->normals.size()) gmd.normals.resize(gmd.vertices.size());
			//	重複頂点の場合はコピーを作りながら代入
			if (vtxUsed[pv]){
				if (gmd.texCoords[pv] != texCoord || 
					(grMesh->normals.size() && gmd.normals[pv] != normal)){	
					//	頂点のコピーの作成
					gmd.vertices.push_back(gmd.vertices[pv]);
					gmd.texCoords.push_back(texCoord);
					if (gmd.normals.size()) gmd.normals.push_back(normal);
					gmd.faces[pf].indices[i] = gmd.vertices.size()-1;
					vertexIdMap.push_back(phMesh->surfaceVertices[pv]);
				}
			}else{	//	そうでなければ、直接代入
				gmd.texCoords[pv] = texCoord;
				if (gmd.normals.size()) gmd.normals[pv] = normal; 
				vertexIdMap[pv] = phMesh->surfaceVertices[pv];
				vtxUsed[pv] = true;
			}
		}
	}
	//	GRMeshを作成
	GRMesh* rv = grMesh->GetNameManager()->CreateObject(GRMeshIf::GetIfInfoStatic(), &gmd)->Cast();
	//	マテリアルの追加
	for (unsigned i=0; i<grMesh->NChildObject(); ++i){
		rv->AddChildObject(grMesh->GetChildObject(i));
	}
	//	テクスチャモードをコピー
	rv->tex3d = grMesh->tex3d;
	grMesh = rv;
}

}

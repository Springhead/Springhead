/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Graphics/GRMesh.h>
#include <Foundation/UTLoadHandler.h>
#include "FWFemMeshNew.h"
#include "FWSprTetgen.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#ifdef _DEBUG
# define PDEBUG(x)	x
#else
# define PDEBUG(x)
#endif

//メッシュエッジの表示、など
//#define VTX_DBG

namespace Spr{;

FWFemMeshNew::FWFemMeshNew(const FWFemMeshNewDesc& d){
	grFemMesh = NULL;
	SetDesc(&d);
	texture_mode = 2;		//	テクスチャ表示の初期値：温度
}

size_t FWFemMeshNew::NChildObject() const{
	return FWObject::NChildObject() + (grFemMesh ? 1 : 0) + (phFemMesh ? 1 : 0);
}

ObjectIf* FWFemMeshNew::GetChildObject(size_t pos){
	if(pos < FWObject::NChildObject()){
		return FWObject::GetChildObject(pos);
	}else{
		pos -= FWObject::NChildObject();
	}	
	if(grFemMesh){
		if (pos == 0) return grFemMesh->Cast();
		else pos --;
	}
	if(phFemMesh){
		if (pos == 0) return phFemMesh->Cast();
		else pos --;
	}
	return NULL;
}

bool FWFemMeshNew::AddChildObject(ObjectIf* o){
	PHFemMeshNew* pm = o->Cast();
	if (pm){
		phFemMesh = pm;
		return true;
	}
	GRMesh* mesh = o->Cast();
	if (mesh){
		grFemMesh = mesh;
		return true;
	}
	return FWObject::AddChildObject(o);
}

void FWFemMeshNew::Loaded(UTLoadContext*){
	//if (!phFemMesh && grFemMesh) CreatePHFromGR();
	//if (grFrame){
	//	grFrame->DelChildObject(grFemMesh->Cast());
	//	CreateGRFromPH();
	//	grFrame->AddChildObject(grFemMesh->Cast());
	//}
}

bool FWFemMeshNew::CreatePHFromGR(){
	//	呼び出された時点で grFemMesh にグラフィクスのメッシュが入っている
	//	grFemMeshを変換して、phFemMeshをつくる。
	
	//TetGenで四面体メッシュ化
	Vec3d* vtxsOut=NULL;
	int* tetsOut=NULL;
	int nVtxsOut=0, nTetsOut=0;
	std::vector<Vec3d> vtxsIn;
	for(unsigned i=0; i<grFemMesh->vertices.size(); ++i) vtxsIn.push_back(grFemMesh->vertices[i]);
	// swithes q+(半径/最短辺) (e.g. = q1.0~2.0) a 最大の体積 
	sprTetgen(nVtxsOut, vtxsOut, nTetsOut, tetsOut, (int)grFemMesh->vertices.size(), &vtxsIn[0], (int)grFemMesh->faces.size(), &grFemMesh->faces[0], "pq2.1a0.002");//a0.3 //a0.003 
	
	//	phFemMesh用のディスクリプタpmdに値を入れていく
	PHFemMeshNewDesc pmd;
	for(int i=0; i < nVtxsOut; i++){
		pmd.vertices.push_back(vtxsOut[i]);
	} 
	pmd.tets.assign(tetsOut, tetsOut + nTetsOut*4);
	//	phFemMeshの生成
	phFemMesh = DBG_NEW PHFemMeshNew(pmd);
		
	if(GetPHSolid() && GetPHSolid()->GetScene()){
		GetPHSolid()->GetScene()->AddChildObject(phFemMesh->Cast());
	}
	return true;
}

struct FaceMap{
	FaceMap(){
		vtxs[0] = vtxs[1] = vtxs[2] = -1;
		face = -1;
	}
	int vtxs[3];
	int& operator [](int i) {return vtxs[i]; }
	int face;
};

void FWFemMeshNew::CreateGRFromPH(){
	//	頂点の対応表を用意
	std::vector<int> vtxMap;
	vtxMap.resize(phFemMesh->vertices.size(), -1);
	for(unsigned i=0; i<phFemMesh->surfaceVertices.size(); ++i){
		vtxMap[phFemMesh->surfaceVertices[i]] = i;
	}
	GRMeshDesc gmd;	//	新しいGRMeshのデスクリプタ
	//	表面の頂点を設定
	for(unsigned i=0; i< phFemMesh->surfaceVertices.size(); ++i) 
		gmd.vertices.push_back(phFemMesh->vertices[ phFemMesh->surfaceVertices[i] ].pos);
	//	表面の三角形を設定
	for(unsigned i=0; i< phFemMesh->nSurfaceFace; ++i) {
		GRMeshFace f;
		f.nVertices = 3;
		for(int j=0; j<3; ++j){
			f.indices[j] = vtxMap[phFemMesh->faces[i].vertices[j]];
		}
		gmd.faces.push_back(f);
	}
	//	phFemMeshの三角形とgrFemMeshの三角形の対応表をつくる	重なっている面積が最大のものが対応する面
	//	まず、法線が近いものを探し、面1と面2上の頂点の距離が近いものに限り、重なっている面積を求める。
	std::vector<Vec3f> pnormals(gmd.faces.size());
	for(unsigned pf=0; pf<gmd.faces.size(); ++pf){
		assert(gmd.faces[pf].nVertices == 3);
		pnormals[pf] = ((gmd.vertices[gmd.faces[pf].indices[2]] - gmd.vertices[gmd.faces[pf].indices[0]]) %
		(gmd.vertices[gmd.faces[pf].indices[1]] - gmd.vertices[gmd.faces[pf].indices[0]])).unit();
	}
	std::vector<Vec3f> gnormals(grFemMesh->faces.size());
	struct FaceWall{
		Vec3f wall[4];
	};
	std::vector<FaceWall> gWalls(gnormals.size());
	for(unsigned gf=0; gf<gnormals.size(); ++gf){
		gnormals[gf] = ((grFemMesh->vertices[grFemMesh->faces[gf].indices[2]] - grFemMesh->vertices[grFemMesh->faces[gf].indices[0]]) %
			(grFemMesh->vertices[grFemMesh->faces[gf].indices[1]] - grFemMesh->vertices[grFemMesh->faces[gf].indices[0]])).unit();
		int nv = grFemMesh->faces[gf].nVertices;
		for(int i=0; i<nv; ++i){
			gWalls[gf].wall[i] = ((grFemMesh->vertices[grFemMesh->faces[gf].indices[(i+1)%nv]] - grFemMesh->vertices[grFemMesh->faces[gf].indices[i]]) % gnormals[gf]).unit();
		}
	}
	std::vector< FaceMap > pFaceMap(pnormals.size());
	for(unsigned pf=0; pf<pnormals.size(); ++pf){
		//	物理の平面(pf)と同一平面に載っているグラフィクスの平面を列挙
		std::vector<int> gfCands;
		for(unsigned gf=0; gf<gnormals.size(); ++gf){
			if (pnormals[pf] * gnormals[gf] > 0.999){	//	法線が遠いのはだめ
				int pv;
				for(pv=0; pv<3; ++pv){
					double d = gnormals[gf] * (gmd.vertices[gmd.faces[pf].indices[pv]] - grFemMesh->vertices[grFemMesh->faces[gf].indices[0]]);
					if (d*d > 1e-8) break;	//	距離が離れすぎているのはだめ
				}
				if (pv==3) gfCands.push_back(gf);
			}
		}
		//	pfの各頂点に対応するgfを見つける
		std::vector<int> gfForPv[3];
		for(unsigned pv=0; pv<3; ++pv){
			for(unsigned i=0; i<gfCands.size(); ++i){
				int gf = gfCands[i];
				int gv;
				for(gv=0; gv<grFemMesh->faces[gf].nVertices; ++gv){
					double d = gWalls[gf].wall[gv] * (gmd.vertices[gmd.faces[pf].indices[pv]] - grFemMesh->vertices[grFemMesh->faces[gf].indices[gv]]);
					if (d < -1e-6) break;
				}
				if (gv == grFemMesh->faces[gf].nVertices){
					gfForPv[pv].push_back(gf);
				}
			}
		}
		//	３物理頂点が、１つのグラフィクス三角形を共有する場合、共有するものを優先
		std::vector< std::pair<int, int> > votes;
		for(int i=0; i<3; ++i){
			for(unsigned j=0; j<gfForPv[i].size(); ++j){
				unsigned k;
				for(k=0; k<votes.size(); ++k){
					if (votes[k].second == gfForPv[i][j]){
						votes[k].first ++;
						break;
					}
				}
				if (k==votes.size()) votes.push_back(std::make_pair(1, gfForPv[i][j]));
			}
		}
		std::sort(votes.begin(), votes.end(), std::greater<std::pair<int, int> >());
		for(int i=0; i<3; ++i){
			for(unsigned j=0; j<votes.size(); ++j){
				for(unsigned k=0; k<gfForPv[i].size(); ++k){
					if (votes[j].second == gfForPv[i][k]){
						pFaceMap[pf][i] = gfForPv[i][k];
						goto nextPv;
					}
				}
			}
			nextPv:;
		}
		if (pFaceMap[pf][0] == pFaceMap[pf][1]) pFaceMap[pf].face = pFaceMap[pf][0];
		else if (pFaceMap[pf][1] == pFaceMap[pf][2]) pFaceMap[pf].face = pFaceMap[pf][1];
		else if (pFaceMap[pf][2] == pFaceMap[pf][0]) pFaceMap[pf].face = pFaceMap[pf][2];
		else pFaceMap[pf].face = pFaceMap[pf][0];
		if (pFaceMap[pf].face == -1){
			return;
		}
	}
#if 0
	DSTR << "FaceMap PHtoGR:\n";
	for(unsigned i=0; i<pFaceMap.size(); ++i)
		DSTR << pFaceMap[i].face << ":" << pFaceMap[i][0] << " " << pFaceMap[i][1] << " " << pFaceMap[i][2] << ", ";
	DSTR << std::endl;
#endif

	//	対応表に応じてマテリアルリストを設定。
	gmd.materialList.resize(grFemMesh->materialList.size() ? pFaceMap.size() : 0);
	for(unsigned pf=0; pf<gmd.materialList.size(); ++pf){
		gmd.materialList[pf] = grFemMesh->materialList[pFaceMap[pf].face];
	}
	//	新しく作るGRMeshの頂点からphFemMeshの頂点への対応表
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
			GRMeshFace& gFace = grFemMesh->faces[pFaceMap[pf][i]];
			GRMeshFace& gFaceNormal = grFemMesh->faceNormals[pFaceMap[pf].face];
			GRMeshFace* gNormal = NULL;
			if (grFemMesh->normals.size()){
				gNormal = &gFace;
				if (grFemMesh->faceNormals.size()) gNormal = &grFemMesh->faceNormals[pFaceMap[pf].face];
			}
			if (gFace.nVertices == 3){
				Vec3f weight;
				Matrix3f vtxs;
				for(unsigned j=0; j<3; ++j){
					vtxs.col(j) = grFemMesh->vertices[gFace.indices[j]];
				}
				int tmp[3];
				vtxs.gauss(weight, gmd.vertices[pv], tmp);
				for(unsigned j=0; j<3; ++j){
					if (weight[j] <= -0.001){
						DSTR << "グラフィクスの3頂点の外側に物理の頂点があります。" << std::endl; 
						for(unsigned k=0; k<3; ++k){
							for(unsigned ax=0; ax<3; ++ax){
								DSTR << grFemMesh->vertices[gFace.indices[k]][ax];
								DSTR << (ax==2 ? "\n" : "\t");
							}
						}
						for(unsigned ax=0; ax<3; ++ax){
							DSTR << gmd.vertices[pv][ax];
							DSTR << (ax==2 ? "\n" : "\t");
						}
					}
					texCoord += weight[j] * grFemMesh->texCoords[gFace.indices[j]];
					if(gNormal) normal += weight[j] * grFemMesh->normals[gNormal->indices[j]];
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
						vtxs[j].col(k) = grFemMesh->vertices[gFace.indices[k<j ? k : k+1]];
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
					if (weight[maxId][j] <= -0.001){
						DSTR << "グラフィクスの3頂点\t"; 
						for(unsigned k=0; k<3; ++k){
							DSTR << grFemMesh->vertices[gFace.indices[k]] << "\t";
						}
						DSTR << "の外側に物理の頂点:\t" << gmd.vertices[pv] << "があります" << std::endl;
					}
					texCoord += weight[maxId][j] * grFemMesh->texCoords[gFace.indices[j<maxId?j:j+1]];
					if(gNormal){
						normal += weight[maxId][j] * grFemMesh->normals[gFaceNormal.indices[j<maxId?j:j+1]];
					}
				}
			}
			gmd.texCoords.resize(gmd.vertices.size());
			if (grFemMesh->normals.size()) gmd.normals.resize(gmd.vertices.size());
			//	重複頂点の場合はコピーを作りながら代入
			if (vtxUsed[pv]){
				if (gmd.texCoords[pv] != texCoord || 
					(grFemMesh->normals.size() && gmd.normals[pv] != normal)){	
					//	頂点のコピーの作成
					gmd.vertices.push_back(gmd.vertices[pv]);
					gmd.texCoords.push_back(texCoord);
					if (gmd.normals.size()) gmd.normals.push_back(normal);
					gmd.faces[pf].indices[i] = (int)gmd.vertices.size()-1;
					vertexIdMap.push_back(phFemMesh->surfaceVertices[pv]);
				}
			}else{	//	そうでなければ、直接代入
				gmd.texCoords[pv] = texCoord;
				if (gmd.normals.size()) gmd.normals[pv] = normal; 
				vertexIdMap[pv] = phFemMesh->surfaceVertices[pv];
				vtxUsed[pv] = true;
			}
		}
	}
	//	GRMeshを作成
	GRMesh* rv = grFemMesh->GetNameManager()->CreateObject(GRMeshIf::GetIfInfoStatic(), &gmd)->Cast();
	//	マテリアルの追加
	for (unsigned i=0; i<grFemMesh->NChildObject(); ++i){
		rv->AddChildObject(grFemMesh->GetChildObject(i));
	}
	//	テクスチャモードをコピー
	rv->tex3d = grFemMesh->tex3d;
	grFemMesh = rv;
}

void FWFemMeshNew::Sync(){	

#ifdef VTX_DBG
	////	デバッグ用
	//// face辺を描画
	DrawFaceEdge();
#endif
	
	//	同期処理
	FWObject::Sync();
	if(syncSource == FWObjectDesc::PHYSICS){
		if(grFemMesh && grFemMesh->IsTex3D()){
			float* gvtx = grFemMesh->GetVertexBuffer();
			if(gvtx){
				int tex = grFemMesh->GetTexOffset();
				int stride = grFemMesh->GetStride();
				for(unsigned gv = 0; gv < vertexIdMap.size(); ++gv){
					int pv = vertexIdMap[gv];
				}
			}	
		}else{
			DSTR << "Error: " << "FWFemMeshNew::Sync()" << " : FWFemMeshNew does not have GRMesh." << std::endl;
		}
	}
}

/// Draw関係はFWSceneに移動させる
#include <GL/glew.h>
#include <GL/glut.h>
void FWFemMeshNew::DrawVtxLine(float length, float x, float y, float z){
	Vec3d pos = Vec3f(x,y,z);
	Vec3d wpos = this->GetGRFrame()->GetWorldTransform() * pos; //* ローカル座標を 世界座標への変換して代入
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(wpos.x,wpos.y,wpos.z);
		wpos.y = wpos.y + length;
		glVertex3f(wpos.x,wpos.y,wpos.z);
	glEnd();
	glFlush();
}

void FWFemMeshNew::DrawEdge(float x0, float y0, float z0, float x1, float y1, float z1){
	Vec3d pos0 = Vec3f(x0,y0,z0);
	Vec3d pos1 = Vec3f(x1,y1,z1);
	Vec3d wpos0 = this->GetGRFrame()->GetWorldTransform() * pos0; //* ローカル座標を 世界座標への変換して代入
	Vec3d wpos1 = this->GetGRFrame()->GetWorldTransform() * pos1; //* ローカル座標を 世界座標への変換して代入
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(wpos0[0],wpos0[1],wpos0[2]);
		glVertex3f(wpos0[0] + wpos1[0], wpos0[1] + wpos1[1], wpos0[2] + wpos1[2]);
	glEnd();
	glFlush();
}

void FWFemMeshNew::DrawEdge(Vec3d vtx0, Vec3d vtx1){
	Vec3d pos0 = vtx0;
	Vec3d pos1 = vtx1;
	Vec3d wpos0 = this->GetGRFrame()->GetWorldTransform() * pos0; //* ローカル座標を 世界座標への変換して代入
	Vec3d wpos1 = this->GetGRFrame()->GetWorldTransform() * pos1; //* ローカル座標を 世界座標への変換して代入
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(wpos0[0],wpos0[1],wpos0[2]);
		glVertex3f(wpos0[0] + wpos1[0], wpos0[1] + wpos1[1], wpos0[2] + wpos1[2]);
	glEnd();
	glFlush();
}

void FWFemMeshNew::DrawFaceEdge(){
	//	使用例
	//	phFemMesh->GetFaceEdgeVtx(0,1);
	for(unsigned i =0; i < phFemMesh->GetNFace();i++){
		for(unsigned j =0;j < 3;j++){
			glBegin(GL_LINES);
			Vec3d wpos = this->GetGRFrame()->GetWorldTransform() * phFemMesh->GetFaceEdgeVtx(i,j);
			double posx = wpos.x;
			double posy = wpos.y;
			double posz = wpos.z;
			glVertex3d(wpos.x,wpos.y,wpos.z);
			wpos = this->GetGRFrame()->GetWorldTransform() * phFemMesh->GetFaceEdgeVtx(i,(j+1)%3);
			posx = wpos.x;
			posy = wpos.y;
			posz = wpos.z;
			glVertex3d(wpos.x,wpos.y,wpos.z);
			glEnd();
			//glFlush();	//ただちに実行
		}
	}
	glutPostRedisplay();
}

}

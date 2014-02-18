/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHFemMesh.h>

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

// std::vector<Spr::PHFemMesh::Edge> PHFemMesh::GetEdge(){
//	return edges;
//}

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

void PHFemMesh::Step(double dt){
	//熱伝導計算の実装を行う？
	//ここから、PHFemMeshThermoのStepを呼んで、実行させる。

	//PHFemMeshThermo::Step()がオーバーライドしている
	

}

PHFemMesh::PHFemMesh(const PHFemMeshDesc& desc, SceneIf* s){
	SetDesc(&desc);
	if (s){ SetScene(s); }

	//ThermalFEMの実装を追加していく

	//計算の関数については、ガウス・ザイデルで　
		
}

void PHFemMesh::SetDesc(const void* p) {
	PHFemMeshDesc* d = (PHFemMeshDesc*)p;
	tets.clear();
	tets.resize(d->tets.size() / 4);
	vertices.clear();
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
		}
	}
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

	faces.clear();
	std::vector<Face> ifaces;
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
			surfaceVertices.push_back(faces[i].vertices[j]);
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
			edges.push_back(Edge(faces[i].vertices[j], faces[i].vertices[(j+1)%3]));
		}
	}
	std::sort(edges.begin(), edges.end());
	std::vector<Edge>::iterator newEEnd = std::unique(edges.begin(), edges.end());
	edges.erase(newEEnd, edges.end());
	nSurfaceEdge = (unsigned)edges.size();
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
	//	四面体に面を追加
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j){
			Face f;
			for(unsigned k=0; k<3; ++k) f.vertices[k] = tets[i].vertices[k<j ? k : k+1];
			f.Update();
			unsigned k;
			for(k=0; k<faces.size(); ++k){
				if (faces[k] == f){
					tets[i].faces[j] = k;
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
	//	頂点に属する面を追加
	for(unsigned i=0;i<faces.size();i++){
		for(unsigned j=0;j<3;j++){
			vertices[faces[i].vertices[j]].faces.push_back(i);
		}
	}



	/*小野原追加ここから--------------------------------------------*/
	//beCondVtxsとtoofarにFalseをいれておく。初期化
	for(unsigned i=0; i<surfaceVertices.size(); i++){
		vertices[surfaceVertices[i]].beCondVtxs = vertices[surfaceVertices[i]].toofar = false;
	}
	/*小野原追加ここまで--------------------------------------------*/

	//	faceの法線を計算
	//.	表面の頂点に、法線ベクトルを追加
	//.	について再帰的に実行
	Vec3d extp;		//	外向き法線
	Vec3d tempV;	//	外向き判定比較頂点(該当face面上にない頂点序数)
	DSTR << "tets.size(): " << tets.size() << std::endl;
	for(unsigned tid=0; tid < tets.size(); tid++){
		//	どの頂点IDでfaceが構成されているのか
		unsigned idsum = 0;
		for(unsigned i=0;i<4;i++){
			idsum += tets[tid].vertices[i];
		}
		for(unsigned fid = 0; fid < 4; fid++){
			//DSTR << "fid :" << fid <<std::endl;
			extp = (vertices[faces[tets[tid].faces[fid]].vertices[1]].pos - vertices[faces[tets[tid].faces[fid]].vertices[0]].pos)
				% (vertices[faces[tets[tid].faces[fid]].vertices[2]].pos - vertices[faces[tets[tid].faces[fid]].vertices[0]].pos);
			extp = extp / extp.norm();
			Vec3d chkN[2] = {vertices[faces[tets[tid].faces[fid]].vertices[1]].pos - vertices[faces[tets[tid].faces[fid]].vertices[2]].pos
				, vertices[faces[tets[tid].faces[fid]].vertices[2]].pos - vertices[faces[tets[tid].faces[fid]].vertices[1]].pos};
			if(extp * chkN[0]/(extp.norm() * chkN[0].norm()) > 1e-15 ){		// 1e-17くらい0より大きく、完全な法線にはなっていないため
				DSTR << "this normal is invalid. make sure to check it out. " << "tid: "<< tid << ", fid: " << fid << " ; "<< this->GetName() << std::endl;
				DSTR << "the invalid value is... " << extp * chkN[0]/(extp.norm() * chkN[0].norm()) <<", " << extp * chkN[1]/(extp.norm() * chkN[1].norm()) << std::endl;
				assert(0);
			}
			if(extp == 0){
				DSTR << "ERROR: extp value == 0" << "tid = " << tid << ", fid = " << fid << std::endl;
			}
	
			//unsigned expVtx =0;		//	face面上にない、0~3番目の四面体頂点
			unsigned idsumt =idsum;
			for(unsigned j=0;j<3;j++){
				idsumt -= faces[tets[tid].faces[fid]].vertices[j];
				//DSTR << "faces[" << fid << "].vertices["<<j <<"]: "<< faces[tets[tid].faces[fid]].vertices[j];
			}
			//if(fid==0){	   expVtx = 3;}	//	0,1,2
			//else if(fid== 1){expVtx = 1;}		//	0,2,3
			//else if(fid== 2){expVtx = 2;} 	//	0,3,1
			//else if(fid== 3){expVtx = 0;} 	//	3,2,1
			
			//. face重心からface外頂点へのベクトルtempV計算
			Vec3d jushin = vertices[faces[tets[tid].faces[fid]].vertices[0]].pos + vertices[faces[tets[tid].faces[fid]].vertices[1]].pos
				+ vertices[faces[tets[tid].faces[fid]].vertices[2]].pos;
			jushin *= 1.0 / 3.0;
			tempV = vertices[idsumt].pos - jushin;
			//DSTR << "tempV:" << tempV <<std::endl;
			if(tempV==Vec3d(0.0,0.0,0.0)){
				DSTR <<"ERROR:	for normal calculating, some vertices judging is invalids"<< std::endl;
			}
			if((tempV * extp / (tempV.norm() * extp.norm()) ) < 0.0){
				//extpとtempVが±９０度以上離れている：extpが外向き法線
				faces[tets[tid].faces[fid]].normal = extp / 10.0;		//	長さを１0cmに
			}else{
				//extpとtempVが９０度以内：extpの向きを180度変えて、faces[fid].normalに代入
				faces[tets[tid].faces[fid]].normal = - extp / 10.0;		// 逆ベクトル
			}
			int debughogeshi=0;
		}
		//Debug
		//全faceに、外向き法線ベクトルを表示させてみて、様子を見れば、確認できるかな？又は、シンプルなメッシュで表示してみるか
	}
	//debug
	//DSTR << "faces.normal checking" << std::endl;
	//DSTR << "tets.size():" << tets.size() << std::endl;
	//for(unsigned tid=0;tid<tets.size();tid++){
	//	for(unsigned fid=0;fid<4;fid++){
	//		DSTR << "faces[tets[" << tid << "].faces[" << fid << "]].normal:" << faces[tets[tid].faces[fid]].normal << std::endl;
	//	}
	//}

	//	頂点の法線を計算
	//	頂点の属するface面より平均？正規化した頂点法線を求める
	std::vector<Vec3d> faceNormal;
	faceNormal.clear();
	for(unsigned vid = 0; vid < vertices.size(); vid++ ){
		//unsigned fsize = vertices[vid].faces.size();
		for(unsigned fid = 0; fid < vertices[vid].faces.size(); fid++ ){
			//.	属するface法線がほぼ同じ方向を向いてるものが見つかった場合は、1つだけ加算して平均をとるように変更する
/*			if(fid == 0) faceNormal.push_back( faces[vertices[vid].faces[fid]].normal);		//	1つ目を入れておく
			//.	verticesが属する全faceIDについてその法線がすでにvectorに入っていないか、チェック
			for(unsigned i=0; i < faceNormal.size(); i++){										//  2つ目以降は、格納済み法線ベクトルと比較して入れる
			//	DSTR << "内積: " << (faceNormal[i] * faces[vertices[vid].faces[fid]].normal ) 
			//		/ ( faceNormal[i].norm() * faces[vertices[vid].faces[fid]].normal.norm() ) << std::endl;  
				if( ( (faceNormal[i] * faces[vertices[vid].faces[fid]].normal )
					/ ( faceNormal[i].norm() * faces[vertices[vid].faces[fid]].normal.norm() ) )  < 0.99  ){		// >0.9 0.99		//:計算誤差でほぼ同じ向きだが別値法線を排除するため

					faceNormal.push_back( faces[vertices[vid].faces[fid]].normal );			// 落ちる原因は、このコードであることが分かった
				}
			}
*/
			//外側の頂点の法線だけ加算			
			if(vertices[vid].faces[fid] < (int)nSurfaceFace){
				vertices[vid].normal += faces[vertices[vid].faces[fid]].normal;		// このコードに代わって、上記vectorコードと以下の加算コードに置き換え
			}
				/*			if(faces[vertices[vid].faces[fid]].normal == 0){
				DSTR << "facenormal value invalid : vertices[" << vid << "].faces[" << fid << "]" << std::endl;
				assert(0);
			}
*/
		}
/*
		for(unsigned j = 0; j < faceNormal.size(); j++){
			vertices[vid].normal += faceNormal[j];									//	置き換えコード
			//DSTR << "faceNormal[" << j << "]: " << faceNormal[j] << std::endl; 
		}
*/
		vertices[vid].normal = vertices[vid].normal / vertices[vid].normal.norm();		//	単位ベクトル化
	}
}

bool PHFemMesh::GetDesc(void* p) const {
	PHFemMeshDesc* d = (PHFemMeshDesc*)p;
	d->tets.resize(tets.size() * 4);
	for(unsigned i=0; i<tets.size(); ++i){
		for(unsigned j=0; j<4; ++j)
			d->tets[i*4+j] = tets[i].vertices[j];
	}
	d->vertices.resize(vertices.size());
	for(unsigned i=0; i<vertices.size(); ++i){
		d->vertices[i] = vertices[i].pos;
	}
	return true;
}

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

//	後で記録すべき変数を考えることにしてとりあえず、計算だけしてみる。
void PHFemMesh::UpdateJacobian(){
	for(unsigned t=0; t<tets.size(); ++t){
		Matrix3d J;	//	各四面体の直交座標系(ξ,η,ζ)から四面体(x,y,z)へのヤコビアン (d(x,y,z) / d(ξ,η,ζ))
		for(int i=1; i<3; ++i){
			for(int j=0; j<3; ++j){
				J[i][j] = vertices[tets[t].vertices[i+1]].pos[j] - vertices[tets[t].vertices[0]].pos[j];
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
		//hozonnga hituyouna monowo sagasu 
		//kakunin shinagara yaru 
	}
}

unsigned PHFemMesh::GetNFace(){
	return (unsigned)faces.size();
}

std::vector<Vec3d> PHFemMesh::GetFaceEdgeVtx(unsigned id){
	std::vector<Vec3d> fev;
	for(unsigned i=0;i<3;i++){
		fev.push_back( vertices[faces[id].vertices[i]].pos);
	}
	return fev;
}

Vec3d PHFemMesh::GetFaceEdgeVtx(unsigned id, unsigned vtx){
	return vertices[faces[id].vertices[vtx]].pos;	
}


//Vec2d PHFemMesh::GetIHBorderX(){
//	return Vec2d(0.0, 0.0);
//}

}

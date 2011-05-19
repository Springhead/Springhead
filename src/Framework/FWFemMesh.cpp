#include "Framework.h"
#include <Framework/SprFWFemMesh.h>
#include <Graphics/GRFrame.h>
#include <Physics/PHConstraint.h>
#include "FWFemMesh.h"
#include "../Samples/Physics/FEMThermo/tetgen.h"
#include "../Samples/Physics/FEMThermo/ThermalFEM.h"





#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWFemMesh::FWFemMesh(const FWFemMeshDesc& d):FWFemMeshDesc(d), grMesh(NULL){
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
	if (DCAST(GRMesh, o)){
		GRMesh* grMesh = (GRMesh*)o;
		//
		GRFrameIf* grf = GetGRFrame();
		//
		//	tetgenとかやってPHを作る			//←ここに記述する処理なのか？
		//grMeshにxファイルを入れる
		//Tetgenに入れる
		//PHMesh = IntoTetGen(grf->Cast());							//TetGen使うなら、GRThermoMesh.cppで記述した処理を行う関数を作る。
		IntoTetGen(grf->Cast());
		//
		//Tetrahedralize()してできたファイルを、PHのvector又は有限個の配列に入れる。
		return true;
	}else{
		return FWObject::AddChildObject(o);
	}
}
bool FWFemMesh::IntoTetGen(GRMesh* grm){
	
	//定義を加えながら変換していく
	tetgenio::facet *f;
	tetgenio::polygon *p;
	int i;
	//頂点の開始番号
	//FEM.in.firstnumber = 1;
	////頂点座標と数の入力
	//FEM.in.numberofpoints = tvtxs.size();//頂点のサイズの代入
	//FEM.in.pointlist = new REAL[FEM.in.numberofpoints * 3];
	//for(unsigned int j=0; j < tvtxs.size(); j++){	//ThermoMeshの頂点番号はj / 3の整数部分
	//	Vec3d rv;			//頂点の位置座標
	//	rv = tvtxs[j].GetPos(1);	
	//	DSTR << "rv ["<<j<<"]"<< rv <<endl;
	//	FEM.in.pointlist[3*j] = rv.x;	//input x coordinate	// thermovertexの各座標を引っ張ってくられるようにメンバ変数に追加する			
	//	FEM.in.pointlist[3*j+1] = rv.y;	//input y coordinate
	//	FEM.in.pointlist[3*j+2] = rv.z;	//input z coordinate
	//	//DSTR<<j<<"\t"<<FEM.in.pointlist[3*j]<<"\t"<<FEM.in.pointlist[3*j+1]<<"\t"<<FEM.in.pointlist[3*j+2]<<endl;		
	//}
	//DSTR<<"tvtxs.size() *3: " << tvtxs.size() *3 <<endl;
	////面の数の代入
	//FEM.in.numberoffacets = faces.size()/3;
	//FEM.in.facetlist = new tetgenio::facet[FEM.in.numberoffacets];
	//FEM.in.facetmarkerlist = new int[FEM.in.numberoffacets];
	////面の情報の入力
	//for(vector<int>::size_type i =0; i < faces.size() / 3; i++){
	//	f = &FEM.in.facetlist[(int)i];
	//	f->numberofpolygons = 1;
	//	f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
	//	f->numberofholes = 0;
	//	f->holelist = NULL;
	//	p = &f->polygonlist[0];
	//	p->numberofvertices = 3;
	//	p->vertexlist = new int[p->numberofvertices];
	//	p->vertexlist[0] = faces[3 * i]+1;
	//	p->vertexlist[1] = faces[3 * i + 1]+1;
	//	p->vertexlist[2] = faces[3 * i + 2]+1;
	//}
	//for(vector<int>::size_type i = 0; i < faces.size() /3 ;i++){
	//	FEM.in.facetmarkerlist[i] = 0;
	//}
	//FEM.in.save_nodes("barin");
	//FEM.in.save_poly("barin");
	////四面体メッシュ化
	//FEM.TFEMTetrahedralize("pq100");
	////メッシュ化したファイルの保存
	//FEM.out.save_nodes("bar100out");			
	//FEM.out.save_elements("bar100out");
	//FEM.out.save_faces("bar100out");
	//return FEM.outに入っているメッシュファイル⇒これをPHに入れる
	std::cout << "メッシュGET成功?" << std::endl;
	return true;
}


}

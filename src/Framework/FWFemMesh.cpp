#include "Framework.h"
#include <Framework/SprFWFemMesh.h>
#include <Graphics/GRFrame.h>
#include <Physics/PHConstraint.h>
#include "FWFemMesh.h"
//#include "../Samples/Physics/FEMThermo/tetgen.h"
//#include "tetgen.h"
//#include "../Samples/Physics/FEMThermo/ThermalFEM.h"





#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

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
	if (DCAST(GRMesh, o)){
		grMesh = (GRMesh*)o;
		if (!phMesh) GeneratePHFemMesh();
		return true;
	}else{
		return FWObject::AddChildObject(o);
	}
}
bool FWFemMesh::GeneratePHFemMesh(){
	//	呼び出された時点で grMesh にグラフィクスのメッシュが入っている
	//	grMeshを変換して、phMeshをつくる。
	std::cout << "メッシュ生成" << std::endl;
	
	PHFemMeshDesc pmd;
	

	
	//	以下で、grMeshからtetgenを呼び出して変換して、pmdに値を入れていけば良い。
	//	PHFemMeshDescには、有限要素法の計算に必要なメッシュの情報を入れれば良い。
	
	//	以下、作ってもらえますか？ PHFemMeshDesc に必要に応じてメンバを加えてください。
	//  作ります。



	//定義を加えながら変換していく
	//int i;
	//頂点の開始番号
#if 0
	in.firstnumber = 1;
	////頂点座標と数の入力
	in.numberofpoints = grMesh->NVertex();		//grMeshの頂点サイズの代入
	in.pointlist = new REAL[in.numberofpoints * 3];
	for(int j=0; j < grMesh->NVertex(); j++){	//ThermoMeshの頂点番号はj / 3の整数部分
		for(int k=0; k<3; ++k)
			in.pointlist[j*3+k] = grMesh->GetVertices()[j][k];
	}

	////面の数の代入
	in.numberoffacets = grMesh->NFace() / 3;		//faces.size()/3; /3にしているのは、なぜなのか？
	in.facetlist = new tetgenio::facet[in.numberoffacets];
/*	in.numberoffacets
	in.facetlist[in.numberoffacets];*/				//	tetgenio::facet 
	in.facetmarkerlist = new int[in.numberoffacets];
	////面の情報の入力
	for(int j =0; j < in.numberoffacets ; j++){
		f = &FEM.in.facetlist[(int)j];
		f->numberofpolygons = 1;
		f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
		f->numberofholes = 0;
		f->holelist = NULL;
		p = &f->polygonlist[0];
		p->numberofvertices = 3;
		p->vertexlist = new int[p->numberofvertices];
		for(int k =0; k < 3 ; k++){
			p->vertexlist[k] = grMesh->GetFaces()[3 * j + k]+1;
		}
	}
	for(int j = 0; j < in.numberoffacets ;j++){
		in.facetmarkerlist[j] = 0;
	}
	in.save_nodes("barin");
	in.save_poly("barin");
	////四面体メッシュ化
	//FEM.TFEMTetrahedralize("pq100");
	////メッシュ化したファイルの保存
	//FEM.out.save_nodes("bar100out");			
	//FEM.out.save_elements("bar100out");
	//FEM.out.save_faces("bar100out");
	//return FEM.outに入っているメッシュファイル⇒これをPHに入れる
	
	phMesh = DBG_NEW PHFemMesh(pmd);
#endif
	return true;
}


}

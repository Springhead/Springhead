#include "tetgen.h"
#include "sprTetgen.h"

bool sprTetgenCpp(int& nVtxsOut, Spr::Vec3d*& vtxsOut, int& nTetsOut, int*& tetsOut, int nVtxIn, Spr::Vec3d* vtxIn, int nFacesIn, Spr::GRMeshFace* facesIn, char* option){
	//TetGenで四面体メッシュ化
	tetgenio in, out;	//	tetgenのディスクリプタ

	//	頂点の基数は0に
	in.firstnumber = 0;
	in.numberofpoints = nVtxIn;						//	grMeshの頂点の数を設定
	in.pointlist = DBG_NEW REAL[in.numberofpoints * 3];
	for(int j=0; j < nVtxIn; j++){	//	頂点の座標を設定
		for(int k=0; k<3; ++k){
			in.pointlist[j*3+k] = vtxIn[j][k];
		}
	}

	in.numberoffacets = nFacesIn;	//面の数
	//	面情報用のメモリの確保
	in.facetlist = DBG_NEW tetgenio::facet[in.numberoffacets];
	in.facetmarkerlist = DBG_NEW int[in.numberoffacets];
	//	面情報の代入
	for(int j =0; j < in.numberoffacets ; j++){
		tetgenio::facet* f = &in.facetlist[(int)j];
		f->numberofpolygons = 1;
		f->polygonlist = DBG_NEW tetgenio::polygon[f->numberofpolygons];
		f->numberofholes = 0;
		f->holelist = NULL;
		tetgenio::polygon* p = &f->polygonlist[0];
		p->numberofvertices = facesIn[j].nVertices;
		p->vertexlist = DBG_NEW int[p->numberofvertices];
		for(int k =0; k < p->numberofvertices; k++){
			p->vertexlist[k] = facesIn[j].indices[k];
		}
		in.facetmarkerlist[j] = 0;
	}
	/*	入力情報の確認用
	in.save_nodes("tetgenIn");
	in.save_poly("tetgenIn");
	in.save_elements("tetgenIn");
	*/
	tetrahedralize(option, &in, &out);	
	//	出力情報の確認用
	out.save_nodes("tetgenOut");			
	out.save_elements("tetgenOut");
	out.save_faces("tetgenOut");
	
	nVtxsOut = out.numberofpoints;
	vtxsOut = DBG_NEW Spr::Vec3d[nVtxsOut];
	for(int i=0; i<nVtxsOut; ++i) vtxsOut[i] = out.pointlist + 3*i;
	nTetsOut = out.numberoftetrahedra;
	tetsOut = DBG_NEW int [4*nTetsOut];
	for(int i=0; i<4*nTetsOut; ++i) tetsOut[i] = out.tetrahedronlist[i];
	if (nVtxsOut && nTetsOut) return true;
	return false;
}
void sprTetgenFreeCpp(Spr::Vec3d* vtxsOut, int* tetsOut){
	delete vtxsOut;
	delete tetsOut;
}
extern "C"{
	bool sprTetgen(int& nVtxsOut, Spr::Vec3d*& vtxsOut, int& nTetsOut, int*& tetsOut, int nVtxIn, Spr::Vec3d* vtxIn, int nFacesIn, Spr::GRMeshFace* facesIn, char* option){
		return sprTetgenCpp(nVtxsOut, vtxsOut, nTetsOut, tetsOut, nVtxIn, vtxIn, nFacesIn, facesIn, option);
	}
	void sprTetgenFree(Spr::Vec3d* vtxsOut, int* tetsOut){
		sprTetgenFreeCpp(vtxsOut, tetsOut);
	}
}

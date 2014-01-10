#include "Framework.h"
#include <Framework/SprFWFemMesh.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRMesh.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHFemMeshThermo.h>
#include <Foundation/UTLoadHandler.h>
#include "FWFemMesh.h"
#include "FWSprTetgen.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <Collision/CDQuickHull2DImp.h>

#if (_MSC_VER >= 1700)
#include <functional>
#endif

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

FWFemMesh::FWFemMesh(const FWFemMeshDesc& d):grMesh(NULL){
	SetDesc(&d);
	texture_mode = 5;		//	テクスチャ表示の初期値：温度
}

void FWFemMesh::DrawIHBorder(double xs, double xe){
	//xs:0.095,xe=0.1
	//	値を直打ち
	Vec3d xS = Vec3d(xs, 0.003, -0.2);
	Vec3d xE = Vec3d(xe, 0.003, -0.2);
	Vec3d wS = this->GetGRFrame()->GetWorldTransform() * xS;
	Vec3d wE = this->GetGRFrame()->GetWorldTransform() * xE;

	glBegin(GL_LINES);
	glVertex3d(wS[0], wS[1], wS[2]);
	glVertex3d(wS[0], wS[1], wS[2]+0.2);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(wE[0], wE[1], wE[2]);
	glVertex3d(wE[0], wE[1], wE[2]+0.2);
	glEnd();
}

//小野原追加ここから---------------------------------------
void FWFemMesh::DrawCompanion(Vec3d vtx){
	Vec3d wVtx = this->GetGRFrame()->GetWorldTransform() * vtx;
	glBegin(GL_LINES);
		glColor3d(1.0,0.0,0.0);
		glVertex3d(wVtx[0], wVtx[1], wVtx[2]);
		glVertex3d(wVtx[0], wVtx[1]-0.05, wVtx[2]);
	glEnd();
}
//小野原追加ここまで---------------------------------------

void FWFemMesh::DrawIHBorderXZPlane(bool sw){
	if(sw){
		Vec3d origin = Vec3d(0.0, 0.0, 0.0);
		origin = origin + Vec3d(0.07,0.0,-0.05); 
		Vec3d wpos = this->GetGRFrame()->GetWorldTransform() * origin;
		double dl = 0.1;
		glBegin(GL_QUADS);
		glVertex3d( wpos.x - dl, wpos.y, wpos.z - dl);
		glVertex3d( wpos.x - dl, wpos.y, wpos.z + dl);
		glVertex3d( wpos.x + dl, wpos.y, wpos.z + dl);
		glVertex3d( wpos.x + dl, wpos.y, wpos.z - dl);
		glEnd();
	}

}
void FWFemMesh::DrawFaceEdge(){
	//	使用例
	//	phMesh->GetFaceEdgeVtx(0,1);
	for(unsigned i =0; i < phMesh->GetNFace();i++){
		for(unsigned j =0;j < 3;j++){
			glBegin(GL_LINES);
			Vec3d wpos = this->GetGRFrame()->GetWorldTransform() * phMesh->GetFaceEdgeVtx(i,j);
			glVertex3d(wpos.x,wpos.y,wpos.z);
			wpos = this->GetGRFrame()->GetWorldTransform() * phMesh->GetFaceEdgeVtx(i,(j+1)%3);
			glVertex3d(wpos.x,wpos.y,wpos.z);
			glEnd();
			//glFlush();	//ただちに実行
		}
	}
	glutPostRedisplay();

	//	gomi
	//PHFemMeshIf* phfem = this->GetPHMesh();
	//std::vector<Vec3f> fev;
	//fev.push_back( phMesh->GetFaceEdgeVtx(0));
	//GRMeshDesc gmd;
	//for(unsigned i ; i < gmd.vertices.size(); i++){
	//}
}


void FWFemMesh::DrawNormal(Vec3d vtx0, Vec3d vtx1){
	Vec3d wpos0 = this->GetGRFrame()->GetWorldTransform() * vtx0; //* ローカル座標を 世界座標への変換して代入
	Vec3d wpos1 = this->GetGRFrame()->GetWorldTransform() *( vtx0 + vtx1); //* ローカル座標を 世界座標への変換して代入
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3d(wpos0[0],wpos0[1],wpos0[2]);
		glVertex3d(wpos1[0],wpos1[1],wpos1[2]);
	glEnd();
	//glFlush();
}

void FWFemMesh::DrawEdgeCW(Vec3d vtx0, Vec3d vtx1,float R,float G,float B){
	// 入力された2頂点座標間を結ぶ
	Vec3d wpos0 = this->GetGRFrame()->GetWorldTransform() * vtx0; //* ローカル座標を 世界座標への変換して代入
	Vec3d wpos1 = this->GetGRFrame()->GetWorldTransform() * vtx1; //* ローカル座標を 世界座標への変換して代入
	glBegin(GL_LINES);
		glColor3d(R,G,B);
		glVertex3d(wpos0[0],wpos0[1],wpos0[2]);
		glVertex3d(wpos1[0],wpos1[1],wpos1[2]);
	glEnd();
	//glFlush();
}


void FWFemMesh::DrawEdge(Vec3d vtx0, Vec3d vtx1){
	Vec3d wpos0 = this->GetGRFrame()->GetWorldTransform() * vtx0; //* ローカル座標を 世界座標への変換して代入
	Vec3d wpos1 = this->GetGRFrame()->GetWorldTransform() * vtx1; //* ローカル座標を 世界座標への変換して代入
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3d(wpos0[0],wpos0[1],wpos0[2]);
		glVertex3d(wpos1[0],wpos1[1],wpos1[2]);
	glEnd();
	//glFlush();
}

void FWFemMesh::DrawEdge(float x0, float y0, float z0, float x1, float y1, float z1){
	Vec3d pos0 = Vec3f(x0,y0,z0);
	Vec3d pos1 = Vec3f(x1,y1,z1);
	Vec3d wpos0 = this->GetGRFrame()->GetWorldTransform() * pos0; //* ローカル座標を 世界座標への変換して代入
	Vec3d wpos1 = this->GetGRFrame()->GetWorldTransform() * pos1; //* ローカル座標を 世界座標への変換して代入
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(wpos0[0],wpos0[1],wpos0[2]);
		glVertex3f(wpos1[0],wpos1[1],wpos1[2]);
	glEnd();
	glFlush();
}

void FWFemMesh::DrawVtxLine(float length, float x, float y, float z){
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


void FWFemMesh::Sync(){	
	//	テスト用
	//static double value, delta;
	//if (value <= 0) delta = 0.01;
	//if (value >= 1) delta = -0.01;
	//value += delta;
#ifdef VTX_DBG
	////	デバッグ用
	//// face辺を描画
	DrawFaceEdge();
	////	XZ平面を描画	true:描画する
	//DrawIHBorderXZPlane(0);
	////	IH加熱領域の境界線を引く
	//DrawIHBorder(0.095,0.1);
#endif

	std::string fwfood;
	fwfood = this->GetName();		//	fwmeshの名前取得

	//テクスチャの設定
	//焦げテクスチャの枚数
	unsigned kogetex	= 5;
	//水分テクスチャの枚数
	unsigned watex		= 2;
	//サーモテクスチャの枚数
	unsigned thtex		= 6;
	unsigned thcamtex   = 9;		//熱カメラっぽい表示用
	//	ロードテクスチャーが焦げ→水→温度の順	（または）水→温度→焦げ	にも変更可能（ファイル名のリネームが必要）

	// num of texture layers
	if(fwfood == "fwNegi"){		///	テクスチャと温度、水分量との対応表は、Samples/Physics/FEMThermo/テクスチャの色と温度の対応.xls	を参照のこと
		kogetex	= 5;
	}
	else if(fwfood == "fwNsteak"){
		kogetex	= 7;		//7にする
	}
	else if(fwfood == "fwPan"){
		//kogetex = 5;
		kogetex = kogePics;
	}

	double dtex =(double) 1.0 / ( kogetex + thtex + watex + thcamtex);		//	テクスチャ奥行座標の層間隔
	double texstart = dtex /2.0;										//	テクスチャ座標の初期値 = 焦げテクスチャのスタート座標
	double wastart = texstart + kogetex * dtex;							//	水分量表示テクスチャのスタート座標
	double thstart = texstart + kogetex * dtex + 1.0 * dtex;			//	サーモのテクスチャのスタート座標 水分テクスチャの2枚目からスタート
	double thcamstart = texstart + (thtex + kogetex + watex) * dtex;	//	
	
	if(fwfood == "fwNegi"){
		 //vertexの法線？表示
		//for(unsigned i =0; i < phMesh->vertices.size();i++){
		//	DrawNormal(phMesh->vertices[i].pos, phMesh->vertices[i].normal);
		//}

		for(unsigned i=0; i < phMesh->faces.size(); i++){
			//.	faceエッジを表示	
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,0),phMesh->GetFaceEdgeVtx(i,1),1.0,0.5,0.1);
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,1),phMesh->GetFaceEdgeVtx(i,2),1.0,0.5,0.1);
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,2),phMesh->GetFaceEdgeVtx(i,0),1.0,0.5,0.1);
		}
		//for(unsigned i = 0; i < phMesh->faces.size(); i++){
		//	//.	三角形の重心からの法線を表示
		//	Vec3d jushin = Vec3d(0.0,0.0,0.0);
		//	for(unsigned j=0; j< 3;j++){
		//		jushin += phMesh->vertices[phMesh->faces[i].vertices[j]].pos;
		//	}
		//	jushin *= 1.0/3.0;
		//	DrawNormal(jushin,phMesh->faces[i].normal);
		//	
		//}
	}

	if(fwfood == "fwPan"){
		//for(unsigned i =0; i < phMesh->vertices.size();i++){
		//	DrawNormal(phMesh->vertices[i].pos, phMesh->vertices[i].normal);
		//}
		for(unsigned i=0; i < phMesh->faces.size(); i++){
			//.	faceエッジを表示	
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,0),phMesh->GetFaceEdgeVtx(i,1),1.0,0.5,0.1);
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,1),phMesh->GetFaceEdgeVtx(i,2),1.0,0.5,0.1);
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,2),phMesh->GetFaceEdgeVtx(i,0),1.0,0.5,0.1);
		}
		//小野原追加ここから(デバッグ用)---------------------------------------
		for(unsigned i=0; i<phMesh->vertices.size(); i++){
			if(phMesh->vertices[i].beCondvtx == true){
				DrawCompanion(phMesh->vertices[i].pos);
			}
		}
		//小野原追加ここまで(デバッグ用)---------------------------------------
	}
	
	if(fwfood == "fwNsteak"){
		//for(unsigned i =0; i < phMesh->vertices.size();i++){
		//	DrawNormal(phMesh->vertices[i].pos, phMesh->vertices[i].normal);
		//}
		for(unsigned i=0; i < phMesh->faces.size(); i++){
			//.	faceエッジを表示	
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,0),phMesh->GetFaceEdgeVtx(i,1),1.0,0.5,0.1);
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,1),phMesh->GetFaceEdgeVtx(i,2),1.0,0.5,0.1);
			DrawEdgeCW(phMesh->GetFaceEdgeVtx(i,2),phMesh->GetFaceEdgeVtx(i,0),1.0,0.5,0.1);
		}
	}


	//	50度刻み:テクスチャの深さ計算(0~50)	( vertices.temp - 50.0 ) * dtex / 50.0
	//	50度刻み:テクスチャの深さ計算(50~100)	( vertices.temp - 100.0 ) * dtex / 50.0
	//	50度刻み:テクスチャの深さ計算(100~150)	( vertices.temp - 150.0 ) * dtex / 50.0
	//	これを満たすように、50,100,150度などを変数にしてもよい。が、他に流用しないし、一目でわかりやすいので、このままでいいかも。
	//	50度刻みごとにdtexを加算せずに、gvtx[stride*gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + thstart;だけでやるのも手

	
	//	同期処理
	if(fwfood == "fwNsteak"){ //小野原追加 デバック用（本来は、syncを呼び出すだけだが、デバック用にステーキの場合はsync2を呼ぶ）
		FWObject::Sync2();
	}
	else{
		FWObject::Sync();
	}
	if (syncSource==FWObjectDesc::PHYSICS){
		if (grMesh && grMesh->IsTex3D()){
			float* gvtx = grMesh->GetVertexBuffer();
			if (gvtx){
				int tex = grMesh->GetTexOffset();
				int stride = grMesh->GetStride();
				for(unsigned gv = 0; gv < vertexIdMap.size(); ++gv){
					int pv = vertexIdMap[gv];
					//	PHから何らかの物理量を取ってくる
							//phから節点の温度を取ってくる
					//PHFemMeshThermoの各節点の温度を取ってくる。
					//温度の値に応じて、↑の係数を用いて、テクスチャ座標を計算する
					//	value = phMeshの派生クラス->thermo[pv];
					//	GRのテクスチャ座標として設定する。	s t r q の rを設定
					//gvtx[stride*gv + tex + 2] = value + gvtx[stride*gv];	//	gvtx[stride*gv]で場所によって違う深度を拾ってくることに
					//gvtx[stride*gv + tex + 2] = 0.1 + value;
					////gvtx[stride*gv + tex + 2] = thstart;
	//				gvtx[stride*gv + tex + 2] = thstart;

					//	どのテクスチャにするかの条件分岐を作る
					//	直前のテクスチャ座標を保存しておく。なければ、初期値を設定
					//	テクスチャの表示モードを切り替えるSWをキーボードから切り替え⇒SampleApp.hのAddHotkey、AddAction周りをいじる
				
					//	CADThermoの該当部分のソース
					//if(tvtxs[j].temp <= tvtxs[j].temp5){		//tvtxs[j].wmass > wmass * ratio1
					//	texz	= texbegin;
					//	double texznew =diff * grad + texz;//実質,テクスチャ座標の初期値
					//	////前のテクスチャｚ座標よりも今回の計算値が深かったら、この計算結果を反映させる
					//	if(tvtxs[j].tex1memo <= texznew){			//初めはこの条件がなくてもいいけれど、一度温度が上がって、冷めてからは必要になる
					//		tvtxs[j].SetTexZ(tratio * dl + texz);	//テクスチャのZ座標を決定する。//表示テクスチャはその線形和を表示させるので、Z座標も線形和で表示するので、線形和の計算を使用
					//		tvtxs[j].tex1memo = tratio * dl + texz;	//tex1memoを更新する
					//	}
					//}
					//if(texturemode == BROWNED){
					if(texture_mode == 1){
						//	焦げテクスチャ切り替え
						//	焼け具合に沿った変化
					//gvtx[stride*gv + tex + 2] = texstart;		// 焦げテクスチャの初期座標
#if 0
						//メッシュの判別
						//DSTR << "this->GetName(): " << this->GetName() << std::endl; ;	//phMesh->GetName():fem4
						//下記三種のどのやり方でもOK
						std::string strg;
						strg = this->GetName();
						if(strg == "fwNegi"){
							DSTR << "Negi STR" << std::endl;
						}
						FWFemMeshIf* fmeshif;
						GetScene()->FindObject(fmeshif,"fwNegi");
						if( fmeshif ){
							DSTR << fmeshif->GetName() << std::endl;
							DSTR << "NEGINEGI GET" << std::endl;

						}
						//
						FWFemMeshIf* fnsteakifif;
						GetScene()->FindObject(fnsteakifif,"fwNsteak");
						if( fnsteakifif ){
							DSTR << fnsteakifif->GetName() << std::endl;
							DSTR << "NIKUNIKU GET" << std::endl;
						}  
						
#endif	

						if(fwfood == "fwPan"){
							gvtx[stride * gv + tex + 2] = texstart;// + dtex;		// ねずみ色の底面
						}
						else if(fwfood == "fwNegi"){
							// 温度変化と同じで　
							double temp = phMesh->vertices[pv].temp;
							// -50.0~0.0:aqua to blue
							if(temp <= -50.0){
								gvtx[stride * gv + tex + 2] = texstart + dtex;
							}
							else if(-50.0 < temp && temp <= 0.0){	
								gvtx[stride * gv + tex + 2] = texstart + dtex;//(texstart ) + ((temp + 50.0) * dtex /50.0);
							}
							//	0~50.0:blue to green
							else if(0.0 < temp && temp <= 50.0 ){
								//double green = temp * dtex / 50.0 + thstart;
								gvtx[stride * gv + tex + 2] = (temp - 50.0)  * dtex / 50.0 + texstart + dtex; //+     dtex;
							}
							//	50.0~100.0:green to yellow
							else if(50.0 < temp && temp <= 100.0){
								gvtx[stride * gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
							}
							//	100.0~150:yellow to orange	
							else if(100.0 < temp && temp <= 150.0){
								gvtx[stride * gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
							}
							//	150~200:orange to red
							else if(150.0 < temp && temp <= 200.0){
								double pinkc = (temp - 50.0 ) * dtex / 50.0 + thstart ;
								gvtx[stride * gv + tex + 2] = dtex * 4.0 + texstart;//(temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
							}
							//	200~250:red to purple
							else if(200.0 < temp && temp <= 250.0){
								gvtx[stride * gv + tex + 2] = dtex * 4.0 + texstart;//(temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
							}
							///	250~:only purple
							else if(250.0 < temp){
								gvtx[stride * gv + tex + 2] = dtex * 4.0 + texstart;
								//gvtx[stride*gv + tex + 2] = wastart;			//whit
							}
							else{
								DSTR << "phMesh->vertices[" << pv << "].temp = " << phMesh->vertices[pv].temp << std::endl;
							}
						}
						else if(fwfood == "fwNsteak"){
							// 温度変化と同じで　
							double temp = phMesh->vertices[pv].temp;
							// -50.0~0.0:aqua to blue
							if(temp <= -50.0){
								gvtx[stride * gv + tex + 2] = texstart + dtex;
							}
							else if(-50.0 < temp && temp <= 0.0){	
								gvtx[stride * gv + tex + 2] = texstart + dtex;//(texstart ) + ((temp + 50.0) * dtex /50.0);
							}
							//	0~50.0:blue to green
							else if(0.0 < temp && temp <= 50.0 ){
								//double green = temp * dtex / 50.0 + thstart;
								gvtx[stride * gv + tex + 2] = (temp - 50.0)  * dtex / 50.0 + texstart + dtex; //+     dtex;
							}
							//	50.0~100.0:green to yellow
							else if(50.0 < temp && temp <= 100.0){
								gvtx[stride * gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
							}
							//	100.0~150:yellow to orange	
							else if(100.0 < temp && temp <= 150.0){
								gvtx[stride * gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
							}
							//	150~200:orange to red
							else if(150.0 < temp && temp <= 200.0){
								double pinkc = (temp - 50.0 ) * dtex / 50.0 + thstart ;
								gvtx[stride * gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
							}
							//	200~250:red to purple
							else if(200.0 < temp && temp <= 250.0){
								gvtx[stride * gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
							}
							///	250~:only purple
							else if(250.0 < temp){
								gvtx[stride * gv + tex + 2] = dtex * 6.0 + texstart;
								//gvtx[stride*gv + tex + 2] = wastart;			//white	 ///	まだらになっちゃう
							}
							else{
								DSTR << "phMesh->vertices[" << pv << "].temp = " << phMesh->vertices[pv].temp << std::endl;
							}
						}
						else if(fwfood == "tPan"){
							DSTR << "tPan are there" << std::endl;
						}

						int phmeshdebug =0;
					//}else if(texturemode == MOISTURE){
					}else if(texture_mode == 3){
						//	水分蒸発表示モード
						//	残水率に沿った変化
						gvtx[stride * gv + tex + 2] = wastart + 2 * dtex;
						for(unsigned j =0; j < phMesh->tets.size(); j++){
							//	割合直打ちでいいや
							if(0.5 < phMesh->tets[j].wratio && phMesh->tets[j].wratio < 1.0){
								gvtx[stride * gv + tex + 2] = wastart + 2 * dtex - ( (phMesh->tets[j].wratio -0.5) * (dtex / 0.5) );
							}
							else if(0.0 < phMesh->tets[j].wratio && phMesh->tets[j].wratio < 0.5){
								gvtx[stride * gv + tex + 2] = wastart + 1 * dtex - ( (phMesh->tets[j].wratio -0.5) * (dtex / 0.5) );
							}
							//grad	= dl/(ratio1-ratio2); //0.125
							//ratio	= tvtxs[j].wmass / wmass;
							//diff	= ratio1 - ratio;
							//tvtxs[j].SetTexZ( diff * grad + texz);
							//tvtxs[j].tex3memo = diff * grad + texz;
						}

					//}else if(texturemode == THERMAL){
					}else if(texture_mode == 2){	//	温度変化表示モード	//サーモが非テクスチャ化された場合、テクスチャのロードは不要になるので、以下のコードを変更
						double temp = phMesh->vertices[pv].temp;
						// -50.0~0.0:aqua to blue
						if(temp <= -50.0){
							gvtx[stride * gv + tex + 2] = thstart;
						}
						else if(-50.0 < temp && temp <= 0.0){	
							gvtx[stride*gv + tex + 2] = (thstart ) + ((temp + 50.0) * dtex /50.0);
						}
						//	0~50.0:blue to green
						else if(0.0 < temp && temp <= 50.0 ){
							//double green = temp * dtex / 50.0 + thstart;
							gvtx[stride*gv + tex + 2] = temp * dtex / 50.0 + thstart + dtex;
						}
						//	50.0~100.0:green to yellow
						else if(50.0 < temp && temp <= 100.0){
							gvtx[stride*gv + tex + 2] = (temp - 50.0 ) * dtex /	 50.0 + thstart + 2 * dtex;
						}
						//	100.0~150:yellow to orange	
						else if(100.0 < temp && temp <= 150.0){
							gvtx[stride*gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + thstart + 2 * dtex;
						}
						//	150~200:orange to red
						else if(150.0 < temp && temp <= 200.0){
							double pinkc = (temp - 50.0 ) * dtex / 50.0 + thstart ;
							gvtx[stride*gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + thstart + 2 * dtex;
						}
						//	200~250:red to purple
						else if(200.0 < temp && temp <= 250.0){
							gvtx[stride*gv + tex + 2] = (temp - 50.0 ) * dtex / 50.0 + thstart + 2 * dtex;
						}
						///	250~:only purple
						else if(250.0 < temp){
							gvtx[stride*gv + tex + 2] = dtex * 6.0 + thstart;
							//gvtx[stride*gv + tex + 2] = wastart;			//white	 ///	まだらになっちゃう
						}
						else{
							DSTR << "phMesh->vertices[" << pv << "].temp = " << phMesh->vertices[pv].temp << std::endl;
						}
					}
					else if(texture_mode == 4){
						double temp = phMesh->vertices[pv].temp;
						// -50.0~0.0:aqua to blue => 20 : purple
						if(temp < 20.0){
							gvtx[stride * gv + tex + 2] = thstart + 6.0 * dtex; 
						}
						else if(temp == 20.0){
							gvtx[stride * gv + tex + 2] = thcamstart;
						}
						else if(20.0 < temp && temp <= 30.0){	
							gvtx[stride*gv + tex + 2] = thcamstart + (temp - 20.0) * dtex / 10.0;
						}
						//	0~50.0:blue to green
						else if(30.0 < temp && temp <= 40.0 ){
							//double green = temp * dtex / 50.0 + thstart;
							gvtx[stride*gv + tex + 2] = thcamstart + dtex + (temp - 30.0) * dtex / 10.0;
						}
						//	50.0~100.0:green to yellow
						else if(40.0 < temp && temp <= 50.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 2 * dtex + (temp - 40.0) * dtex / 10.0;
						}
						//	100.0~150:yellow to orange	
						else if(50.0 < temp && temp <= 60.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 3 * dtex + (temp - 50.0) * dtex / 10.0;
						}
						//	150~200:orange to red
						else if(60.0 < temp && temp <= 70.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 4 * dtex + (temp - 60.0) * dtex / 10.0;
						}
						//	200~250:red to purple
						else if(70.0 < temp && temp <= 80.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 5 * dtex + (temp - 70.0) * dtex / 10.0;
						}
						///	250~:only purple
						else if(80.0 < temp && temp <= 90.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 6 * dtex + (temp - 80.0) * dtex / 10.0;
						}
						else if(90.0 < temp && temp <= 100.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 7 * dtex + (temp - 90.0) * dtex / 10.0;
						}
						else if(100.0 < temp){
							gvtx[stride*gv + tex + 2] = thcamstart + 8 * dtex;
						}
						else{
							DSTR << "phMesh->vertices[" << pv << "].temp = " << phMesh->vertices[pv].temp << std::endl;
						}
					}
					else if(texture_mode == 5){//小野原追加　デバックモード
						if(fwfood == "fwPan"){
							DSTR << phMesh->vertices[pv].temp << std::endl;
						}
						double temp = phMesh->vertices[pv].temp;
						if(temp < 30.0){
							gvtx[stride * gv + tex + 2] = thstart + 6.0 * dtex; 
						}
						else if(30.0 < temp && temp <= 35.0 ){
							gvtx[stride*gv + tex + 2] = thcamstart + dtex + (temp - 30.0) * dtex / 10.0;
						}
						else if(35.0 < temp && temp <= 40.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 2 * dtex + (temp - 35.0) * dtex / 10.0;
						}
						else if(40.0 < temp && temp <= 45.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 3 * dtex + (temp - 40.0) * dtex / 10.0;
						}
						else if(45.0 < temp && temp <= 50.0){
							gvtx[stride*gv + tex + 2] = thcamstart + 4 * dtex + (temp - 45.0) * dtex / 10.0;
						}
						else{
						//	DSTR << "phMesh->vertices[" << pv << "].temp = " << phMesh->vertices[pv].temp << std::endl;
						}
					}
				}
			}	
		}else{
			DSTR << "Error: " << GetName() << ":FWFemMesh does not have 3D Mesh" << std::endl;
		}
	}
}


size_t FWFemMesh::NChildObject() const{
	return FWObject::NChildObject() + (grMesh ? 1 : 0) + (phMesh ? 1 : 0);
}
ObjectIf* FWFemMesh::GetChildObject(size_t pos){
	if (pos < FWObject::NChildObject()){
		return FWObject::GetChildObject(pos);
	}else{
		pos -= FWObject::NChildObject();
	}	
	if (grMesh){
		if (pos == 0) return grMesh->Cast();
		else pos --;
	}
	if (phMesh){
		if (pos == 0) return phMesh->Cast();
		else pos --;
	}
	//if (phMeshThermo){
	//	if (pos == 0) return phMeshThermo->Cast();
	//	else pos --;
	//}
	return NULL;
}
bool FWFemMesh::AddChildObject(ObjectIf* o){
	PHFemMesh* pm = o->Cast();
	if (pm){
		phMesh = pm;
		return true;
	}
	GRMesh* mesh = o->Cast();
	if (mesh){
		grMesh = mesh;
		return true;
	}
	//PHFemMeshThermo* pmth = o->Cast();
	//if (pmth){
	//	phMeshThermo = pmth;
	//	return true;
	//}
	return FWObject::AddChildObject(o);
}
void FWFemMesh::Loaded(UTLoadContext*){
	//if (!phMesh && grMesh) CreatePHFromGR();
	if (!phMesh && grMesh) CreatePHFromGRroughness();

	if (grFrame){
		grFrame->DelChildObject(grMesh->Cast());
		CreateGRFromPH();
		grFrame->AddChildObject(grMesh->Cast());
	}
}
bool FWFemMesh::CreatePHFromGRroughness(){
	//	呼び出された時点で grMesh にグラフィクスのメッシュが入っている
	//	grMeshを変換して、phMeshをつくる。
	//	以下で、grMeshからtetgenを呼び出して変換して、pmdに値を入れていけば良い。
	PHFemMeshThermoDesc pmd;
	
	//TetGenで四面体メッシュ化
	Vec3d* vtxsOut=NULL;
	int* tetsOut=NULL;
	int nVtxsOut=0, nTetsOut=0;
	std::vector<Vec3d> vtxsIn;
	for(unsigned i=0; i<grMesh->vertices.size(); ++i) vtxsIn.push_back(grMesh->vertices[i]);
	// swithes q+(半径/最短辺) (e.g. = q1.0~2.0) a 最大の体積 
	sprTetgen(nVtxsOut, vtxsOut, nTetsOut, tetsOut, (int)grMesh->vertices.size(), &vtxsIn[0], (int)grMesh->faces.size(), &grMesh->faces[0], (char*)meshRoughness.c_str());
	//sprTetgen(nVtxsOut, vtxsOut, nTetsOut, tetsOut, (int)grMesh->vertices.size(), &vtxsIn[0], (int)grMesh->faces.size(), &grMesh->faces[0], "pq2.0a1e-7V");//"pq10.1a1.0e-1V");//a0.3 //a0.003 //pq2.1a0.002:20130322
	
	//	PHMesh用のディスクリプタpmdに値を入れていく
	for(int i=0; i < nVtxsOut; i++){
		pmd.vertices.push_back(vtxsOut[i]);
	} 
	pmd.tets.assign(tetsOut, tetsOut + nTetsOut*4);
	//	PHMeshの生成
	phMesh = DBG_NEW PHFemMeshThermo(pmd);
	//	PHFemMeshThermoの生成を追加
	//phMeshThermo = DBG_NEW PHFemMeshThermo(pmd);
	//phMesh = phMeshThermo;
	if (GetPHSolid() && GetPHSolid()->GetScene())
		GetPHSolid()->GetScene()->AddChildObject(phMesh->Cast());
		//GetPHSolid()->GetScene()->AddChildObject(phMeshThermo->Cast());
	return true;
}

bool FWFemMesh::CreatePHFromGR(){
	//	呼び出された時点で grMesh にグラフィクスのメッシュが入っている
	//	grMeshを変換して、phMeshをつくる。
	//	以下で、grMeshからtetgenを呼び出して変換して、pmdに値を入れていけば良い。
	PHFemMeshThermoDesc pmd;
	
	//TetGenで四面体メッシュ化
	Vec3d* vtxsOut=NULL;
	int* tetsOut=NULL;
	int nVtxsOut=0, nTetsOut=0;
	std::vector<Vec3d> vtxsIn;
	for(unsigned i=0; i<grMesh->vertices.size(); ++i) vtxsIn.push_back(grMesh->vertices[i]);
	// swithes q+(半径/最短辺) (e.g. = q1.0~2.0) a 最大の体積 
	sprTetgen(nVtxsOut, vtxsOut, nTetsOut, tetsOut, (int)grMesh->vertices.size(), &vtxsIn[0], (int)grMesh->faces.size(), &grMesh->faces[0], "pq2.1a1.0e-5");
	//sprTetgen(nVtxsOut, vtxsOut, nTetsOut, tetsOut, (int)grMesh->vertices.size(), &vtxsIn[0], (int)grMesh->faces.size(), &grMesh->faces[0], "pq2.0a1e-7V");//"pq10.1a1.0e-1V");//a0.3 //a0.003 //pq2.1a0.002:20130322
	
	//	PHMesh用のディスクリプタpmdに値を入れていく
	for(int i=0; i < nVtxsOut; i++){
		pmd.vertices.push_back(vtxsOut[i]);
	} 
	pmd.tets.assign(tetsOut, tetsOut + nTetsOut*4);
	//	PHMeshの生成
	phMesh = DBG_NEW PHFemMeshThermo(pmd);
	//	PHFemMeshThermoの生成を追加
	//phMeshThermo = DBG_NEW PHFemMeshThermo(pmd);
	//phMesh = phMeshThermo;
	if (GetPHSolid() && GetPHSolid()->GetScene())
		GetPHSolid()->GetScene()->AddChildObject(phMesh->Cast());
		//GetPHSolid()->GetScene()->AddChildObject(phMeshThermo->Cast());
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
	for(unsigned i=0; i< phMesh->nSurfaceFace; ++i) {
		GRMeshFace f;
		f.nVertices = 3;
		for(int j=0; j<3; ++j){
			f.indices[j] = vtxMap[phMesh->faces[i].vertices[j]];
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
	DSTR << grMesh->faces.size() << std::endl;
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
	std::vector< FaceMap > pFaceMap(pnormals.size());
	for(unsigned pf=0; pf<pnormals.size(); ++pf){
		//	物理の平面(pf)と同一平面に載っているグラフィクスの平面を列挙
		std::vector<int> gfCands;
		for(unsigned gf=0; gf<gnormals.size(); ++gf){
			if (pnormals[pf] * gnormals[gf] > 0.999){	//	法線が遠いのはだめ
				int pv;
				for(pv=0; pv<3; ++pv){
					double d = gnormals[gf] * (gmd.vertices[gmd.faces[pf].indices[pv]] - grMesh->vertices[grMesh->faces[gf].indices[0]]);
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
				for(gv=0; gv<grMesh->faces[gf].nVertices; ++gv){
					double d = gWalls[gf].wall[gv] * (gmd.vertices[gmd.faces[pf].indices[pv]] - grMesh->vertices[grMesh->faces[gf].indices[gv]]);
					if (d < -1e-6) break;
				}
				if (gv == grMesh->faces[gf].nVertices){
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
			DSTR << "ERROR:face of Physics object is different from  face of Graphics object . physics is FEM ? PHSolid? :same faces require foe ph and gr(// FWFemMesh.cpp:l675" << std::endl;
		}
	}
#if 0
	DSTR << "FaceMap PHtoGR:\n";
	for(unsigned i=0; i<pFaceMap.size(); ++i)
		DSTR << pFaceMap[i].face << ":" << pFaceMap[i][0] << " " << pFaceMap[i][1] << " " << pFaceMap[i][2] << ", ";
	DSTR << std::endl;
#endif

	//	対応表に応じてマテリアルリストを設定。
	gmd.materialList.resize(grMesh->materialList.size() ? pFaceMap.size() : 0);
	for(unsigned pf=0; pf<gmd.materialList.size(); ++pf){
		gmd.materialList[pf] = grMesh->materialList[pFaceMap[pf].face];
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
			GRMeshFace& gFace = grMesh->faces[pFaceMap[pf][i]];
			GRMeshFace& gFaceNormal = grMesh->faceNormals[pFaceMap[pf].face];
			GRMeshFace* gNormal = NULL;
			if (grMesh->normals.size()){
				gNormal = &gFace;
				if (grMesh->faceNormals.size()) gNormal = &grMesh->faceNormals[pFaceMap[pf].face];
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
					if (weight[j] <= -0.001){
						DSTR << "グラフィクスの3頂点の外側に物理の頂点があります。" << std::endl; 
						for(unsigned k=0; k<3; ++k){
							for(unsigned ax=0; ax<3; ++ax){
								DSTR << grMesh->vertices[gFace.indices[k]][ax];
								DSTR << (ax==2 ? "\n" : "\t");
							}
						}
						for(unsigned ax=0; ax<3; ++ax){
							DSTR << gmd.vertices[pv][ax];
							DSTR << (ax==2 ? "\n" : "\t");
						}
					}
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
					if (weight[maxId][j] <= -0.001){
						DSTR << "グラフィクスの3頂点\t"; 
						for(unsigned k=0; k<3; ++k){
							DSTR << grMesh->vertices[gFace.indices[k]] << "\t";
						}
						DSTR << "の外側に物理の頂点:\t" << gmd.vertices[pv] << "があります" << std::endl;
					}
					texCoord += weight[maxId][j] * grMesh->texCoords[gFace.indices[j<maxId?j:j+1]];
					if(gNormal){
						normal += weight[maxId][j] * grMesh->normals[gFaceNormal.indices[j<maxId?j:j+1]];
					}
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
					gmd.faces[pf].indices[i] = (int)gmd.vertices.size()-1;
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

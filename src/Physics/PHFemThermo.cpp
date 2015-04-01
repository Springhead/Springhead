/*
 *  Copyright (c) 2003 - 2011, Fumihiro Kato, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <SprDefs.h>
#include <Scilab/SprScilab.h>
#include <Physics/PHFemThermo.h>
#include <Base/Affine.h>
#include <iomanip> 
//#include <Framework/FWObject.h>
//#include <Framework/sprFWObject.h>

#include "windows.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <Foundation/UTClapack.h>

#define FEMLOG(x)

//%%%%%			実験用設定パラメータAPI化して移動予定			%%%%%

// 弱火加熱パラメータ
//#define inr 0.048
//#define outR 0.052
//#define weekPow 231.9 * 1e3 * 0.02 // 231.9 * 1e3:算出した値、0.02：物理ステップ時間 
	//#define weekPow 107		//100:[W]=[J/s]  //50.5801 // 鉄板の1/4だけの面積に加わる加熱熱量  	//行列作成後に[J/(m^2・sec)]なる

//	渦電流加熱のための電気抵抗が温度依存性がある場合のモデル。
//#define TempDependHeat		// 

#if 0
	#define THCOND 83.5 // W/(m K) = [J/ (m・K・s)] //67
#endif
	#define RHO	7874	//	
#if 0
	#define SPECIFICHEAT 459.94// 298.15K:447.130, 328.15K(30℃):459.94, 400K:490.643	
#endif

#define dMatkUp 0.015
#define dMatKinf 0.0001
	//#define inr 0.034	//0.048
	//#define outR 0.079	//0.052
//#define weekPow 231.9 * 0.02

//[K][C]{F}の動作確認実験で使用
//#define weekPow 0.5281 / 0.02		// J/sec -> J /step sec　に変換する必要がある、設定値は1秒当たりの値にし、計算でステップ時間に合わせる

//行列のテスト
//#define weekPow 0.0

//#define mai.cpp の only1deg

//水の値
//7.874 * 10^3	 // 密度	7.874 * 10^3 [kg m^-3]
//24.97　// 比熱	J/(K・kg)
//#define THCOND 0.618//:水 
//#define RHO	1.0e6
//#define SPECIFICHEAT 4.2

#define WaterLatentHeat	2257000	//水の蒸発潜熱 [J/kg]

//	%%%%%%%		動作条件　挙動条件

//境界条件
#define NOTUSE_HEATTRANS_HERE

//一定加熱量×３によるフラットがちなコイル加熱を使うか否か
#define DISABLE_COIL

//#define badMatK
#define UseMatAll
#define UseGaussSeidel
//#define outMatK
//#define UseScilab
//#define DEBUG
//#define DumK		//debug用のdummyK?	使っていない

//	LMS表示などをDSTRするかどうかSW
//#define HYOKA


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

using namespace PTM;

namespace Spr{;

PHFemThermoDesc::PHFemThermoDesc(){
	Init();
}
void PHFemThermoDesc::Init(){
#if 1
	thConduct = 83.5;		//THCOND;
	thConduct_x = 83.5;
	thConduct_y = 83.5;
	thConduct_z = 83.5;
#endif
	rho = 7874;	//RHO;
	heatTrans = 25;//0;//25;
	specificHeat = 459.94;//SPECIFICHEAT;//0.196;		//1960
	radiantHeat =0;
	initial_temp=0.0;
}

///////////////////////////////////////////////////////////////////
//	PHFemThermo

PHFemThermo::PHFemThermo(const PHFemThermoDesc& desc, SceneIf* s){
	deformed = true;			//変数の初期化、形状が変わったかどうか
	doCalc = true;
	//%%%	ガウスザイデル引数
	NofCyc = 100;
	epsilonG = 1.0;
	//%%%	初期条件
	jout = 22.9;	//A5052熱伝達実験時:22.9, SUS430放熱実験時:29.9	//(48.0+30.0)/2.0;		// 150:77.85, 200:94.25, 100:58.7
	ems = 0.0;//3.8e-2;//3.63e-2;//3.58		//	節点での熱輻射係数：温度の差分に比例する値なので、3.58e-2	SUS430での値
	ems_const = 0.0;//-1.14;//-1.1507//-1.063;
	temp_c = 30.0;
	temp_out = 20.0;//23.8;	//A5052試料時室温:23.8, SUS430放熱実験時:29.9

	//%%%%%4
	weekPow_FULL = 120.0;
	weekPow_ = 77.0;
	inr_ = 0.0355;
	outR_ = 0.0825;
	//%%%%%
	weekPow_add = 3.1;
	inr_add = 0.009;
	outR_add = 0.03;
	//%%%%%
	weekPow_decr = 2.2;		//　値はプラスだが、計算関数内でマイナスに成る		//2.1
	inr_decr = 0.039;
	outR_decr = 0.041;
	//%%%
	stopTime = 180.3;

	// ディスクリプタにセット
	SetDesc(&desc);
	if (s){ SetScene(s); }
	StepCount =0;				// ステップ数カウンタ
	StepCount_ =0;				// ステップ数カウンタ

}

void PHFemThermo::Init(){

	weekPow_FULL = weekPow_full;
	matkupSwitch = true;
	
	PHFemMeshNew* mesh = GetPHFemMesh();
	PHFemThermoIf* phm = mesh->GetPHFemThermo();

	int nSurfaceVtxs = NSurfaceVertices();
	for(unsigned i=0; i < (unsigned) nSurfaceVtxs; i++){
		int vtxid = GetSurfaceVertex(i);
		//円柱メッシュの底面上にある頂点
		if(GetVertexPose(vtxid).y < -0.00749){
			bottomVertexNums.push_back(vtxid);
		}
		//円柱メッシュの上面上にある頂点
		else if(GetVertexPose(vtxid).y > 0.00749){
			topVertexNums.push_back(vtxid);
		}
	}

	////各四面体と頂点番号を出力
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	for(unsigned i=0; i < mesh->NTets(); i++){
	//		DSTR << "tetID:" << i << " vtxIDs";
	//		for(unsigned j=0; j < 4; j++){
	//			DSTR << mesh->tets[i].vertexIDs[j] << ",";
	//		}
	//		DSTR << std::endl;
	//	}
	//}

	////各頂点の座標を出力
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	for(unsigned i=0; i < mesh->NVertices(); i++){
	//		FemVertex vtx = mesh->vertices[i];
	//		DSTR << "頂点" << i << ":(" << vtx.pos.x << "," << vtx.pos.y << "," << vtx.pos.z << ")" << std::endl;
	//	}
	//}

	////すべての頂点を回転
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	Quaterniond rot = Quaterniond::Rot(Rad(80.0), 'x');
	//	for(unsigned i=0; i < mesh->vertices.size(); i++){
	//		mesh->vertices[i].pos = rot * mesh->vertices[i].pos;
	//		//mesh->vertices[i].pos[0] = (-1) * mesh->vertices[i].pos[0];
	//		//mesh->vertices[i].pos[2] = (-1) * mesh->vertices[i].pos[2];
	//	}
	//	//回転後の頂点の座標を出力
	//	for(unsigned i=0; i < mesh->NVertices(); i++){
	//		FemVertex vtx = mesh->vertices[i];
	//		DSTR << "頂点" << i << ":(" << vtx.pos.x << "," << vtx.pos.y << "," << vtx.pos.z << ")" << std::endl;
	//	}
	//}

#ifdef UseScilab
	if(strcmp(mesh->GetName(), "femNsteak") == 0){
		if (!ScilabStart()) DSTR << "Error : ScilabStart \n";
		else DSTR << "ScilabStart \n";
	}
#endif

	for(unsigned i = 0; i < mesh->vertices.size(); i++){
		StateVertex vars;
		vertexVars.push_back(vars);
	}
	for(unsigned i = 0; i < mesh->edges.size(); i++){
		StateEdge vars;
		edgeVars.push_back(vars);
	}
	for(unsigned i = 0; i < mesh->faces.size(); i++){
		StateFace vars;
		faceVars.push_back(vars);
	}
	for(unsigned i = 0; i < mesh->tets.size(); i++){
		StateTet vars;
		tetVars.push_back(vars);
	}
	//%%%	初期化類		%%%//

	//各種メンバ変数の初期化⇒コンストラクタでできたほうがいいかもしれない。
	///	Edges
	for(unsigned i =0; i < mesh->edges.size();i++){
		edgeVars[i].c = 0.0;	
		edgeVars[i].k = 0.0;
	}
	if(GetPHFemMesh()->GetName() == "fwNsteak"){
		DSTR << "this is fwNsteak" << std::endl;
	}
	if(GetPHFemMesh()->GetName() == "fwPan"){
		DSTR << "this is fwPan" << std::endl;
	}
	if(GetPHFemMesh()->GetName() == "fwNegi"){
		DSTR << "this is fwNegi" << std::endl;
	}
	if(GetPHFemMesh()->GetName() == "metal5x1x2_fem"){
		DSTR << "this is metal5x1x2_fem" << std::endl;
	}
	DSTR<< "My name is ... " << GetPHFemMesh()->GetName() <<std::endl;
	///	faces
	for(unsigned i=0;i<mesh->faces.size();i++){
		faceVars[i].alphaUpdated = true;
		faceVars[i].area = 0.0;
		faceVars[i].heatTransRatio = 0.0;
		faceVars[i].deformed = true;				//初期状態は、変形後とする
		for(unsigned j=0;j<4;++j){
			faceVars[i].fluxarea[j] =0.0;
		}
		faceVars[i].map = INT_MAX;	//
		//faces[i].thermalEmissivity =0.0;
		//faces[i].thermalEmissivity_const =0.0;
		//faces[i].heatflux.clear();				// 初期化
		//faces[i].heatflux[hum]の領域確保：配列として、か、vectorとしてのpush_backか、どちらかを行う。配列ならここに記述。
		for(unsigned mode =0; mode < HIGH +1 ; mode++){			// 加熱モードの数だけ、ベクトルを生成
			for(unsigned j=0;j<4;++j){
				faceVars[i].heatflux[mode][j] = 0.0;
			}
		}
	}

	//行列の成分数などを初期化
	bVecAll.resize(mesh->vertices.size(),1);
	TVecAll.resize(mesh->vertices.size());
	preTVecAll.resize(mesh->vertices.size());

	//	メッシュ節点温度 初期化
	//SetVerticesTempAll(30.0);

	//	空気への熱伝達による熱輻射境界条件による計算実行の設定
	SetVerticesTempAll(jout);
	SetVerticesTempAll(initial_temp);
	SetVerticesPreTempAll(initial_temp);

	//matkcheck.open("matkcheck.csv",std::fstream::out,std::fstream::app);
	matkcheck.open("matkcheck.csv");
	matkcheck << "jout,ems,ems_const" << std::endl;
	matkcheck << jout <<","<< ems <<","<< ems_const << std::endl;

//	vecFOutPan.open("vecFOutPan.csv");
	vecFOutSteak.open("vecFOutSteak.csv");
	
//	vecFOutSteak << "時間" << ",";
//	for(unsigned i=0; i < mesh->vertices.size();i++){
//		if(i != mesh->vertices.size() -1){
//			vecFOutSteak << "頂点" << i << ", ";	
//		}
//		else{
//			vecFOutSteak << "頂点" << i << std::endl;
//		}
//	}

	
	//評価実験用コード
	//>	LMSSのためのコード
	//	節点温度の初期設定(行列を作る前に行う)

	//	j以遠の温度は全体をこの温度にして設定
	//SetVerticesTempAll((158.5 + (30.0 + 158.5) /2.0)/2.0 );
	//SetVerticesTempAll((158.5+30.0)/2.0);
	


//	jout = 77.85;
//	ems = 4.9258;
	
	// main.cppで動くように設定する
	round.clear();
	tempe.clear();
	for(unsigned i=0;i<11;++i){
		round.push_back(i*0.01);
	}
	//%%%		放熱のパラメータ
	//	200℃から放熱
	//tempe.push_back(211.3);
	//tempe.push_back(211.2);
	//tempe.push_back(210.2);
	//tempe.push_back(207.6);
	//tempe.push_back(203.9);

	//tempe.push_back(198.0);
	//tempe.push_back(189.3);
	//tempe.push_back(178.7);
	//tempe.push_back(169.8);
	//tempe.push_back(158.5);
	
	//	150℃から放熱
	//double tempe2[10] = {158.6, 158.3, 156.8, 153.9, 150.4, 146.0, 140.3, 134.8, 130.8, 125.7};
	//tempe.push_back(158.6);
	//tempe.push_back(158.3);
	//tempe.push_back(156.8);
	//tempe.push_back(153.9);
	//tempe.push_back(150.4);

	//tempe.push_back(146);
	//tempe.push_back(140.3);
	//tempe.push_back(134.8);
	//tempe.push_back(130.8);
	//tempe.push_back(125.7);

	//150℃
	//tempe.push_back(100.0);
	//tempe.push_back(100.0);
	//tempe.push_back(99.4);
	//tempe.push_back(98.2);
	//tempe.push_back(96.8);

	//tempe.push_back(95.0);
	//tempe.push_back(92.6);
	//tempe.push_back(90.6);
	//tempe.push_back(89.3);
	//tempe.push_back(87.4);
	
	//50	10sec
	//tempe.push_back(50.0);
	//tempe.push_back(50.0);
	//tempe.push_back(49.9);
	//tempe.push_back(49.8);
	//tempe.push_back(49.7);

	//tempe.push_back(49.3);
	//tempe.push_back(48.7);
	//tempe.push_back(48.5);
	//tempe.push_back(48.4);
	//tempe.push_back(48.0);

	//	50	更新タイミング 
	//tempe.push_back(50.0);
	//tempe.push_back(49.9);
	//tempe.push_back(49.9);
	//tempe.push_back(49.7);
	//tempe.push_back(49.7);

	//tempe.push_back(49.2);
	//tempe.push_back(48.7);
	//tempe.push_back(48.4);
	//tempe.push_back(48.3);
	//tempe.push_back(48.0);

	//%%%		加熱のパラメータ
	//	はじめから　10sec 10:01:56
	//tempe.push_back(29.7);
	//tempe.push_back(29.7);
	//tempe.push_back(29.6);
	//tempe.push_back(29.8);
	//tempe.push_back(29.7);

	//tempe.push_back(29.7);
	//tempe.push_back(29.7);
	//tempe.push_back(29.7);
	//tempe.push_back(29.8);
	//tempe.push_back(29.8);



	//%%%%%		初期温度として、円環状の温度分布を使いたいときに使う	SetConcentricHeatMap()と一緒に使う
//	SetConcentricHeatMap(round,tempe,Vec2d(0.0, -0.005));		//	-0.001にしても、初期温度は不変だった。 7/4コメントアウト shiba
	//DSTR << "入力 check it out" <<std::endl;
	//for(unsigned i=0; i< tempe.size();++i){
	//	DSTR << round[i] << "; "<< i*0.01 <<"; " <<  tempe[i] << std::endl;
	//}
	//周囲への熱伝達温度の初期化(temp度にする)　≠　熱輻射
	InitTcAll(temp_c);
	InitToutAll(temp_out);
	SetThermalEmissivityToVerticesAll(ems,ems_const);	
	for(int id=0;id<NSurfaceVertices();++id){
		vertexVars[id].beRadiantHeat =true;
	}
	///	熱伝達率を各節点に格納
	SetHeatTransRatioToAllVertex();
	
	DSTR << "temp_c " << temp_c <<std::endl;
	//>	熱放射関係	LMSS
	DSTR << "temp_out "<< temp_out <<std::endl; 
	DSTR << "ems "<< ems <<std::endl; 
	//SetThermalEmissivityToVerticesAll(radiantHeat);				///	暫定値0.0で初期化	：熱放射はしないｗ	

	//vertices.tempをすべて、TVecAllへ代入する
	CreateTempVertex();

	//熱伝導率、密度、比熱、熱伝達率　のパラメーターを設定・代入
		//PHFemThermoのメンバ変数の値を代入 CADThermoより、0.574;//玉ねぎの値//熱伝導率[W/(ｍ・K)]　Cp = 1.96 * (Ndt);//玉ねぎの比熱[kJ/(kg・K) 1.96kJ/(kg K),（玉ねぎの密度）食品加熱の科学p64より970kg/m^3
		//熱伝達率の単位系　W/(m^2 K)⇒これはSI単位系なのか？　25は論文(MEAT COOKING SIMULATION BY FINITE ELEMENTS)のオーブン加熱時の実測値
		//SetInitThermoConductionParam(0.574,970,1.96,25);
	//. 熱伝達する SetInitThermoConductionParam(0.574,970,0.1960,25 * 0.001 );		//> thConduct:熱伝導率 ,roh:密度,	specificHeat:比熱 J/ (K・kg):1960 ,　heatTrans:熱伝達率 W/(m^2・K)
	//. 熱伝達しない
//	SetInitThermoConductionParam(THCOND,RHO,SPECIFICHEAT,0 );		// 熱伝達率=0;にしているw	//	SetInitThermoConductionParam(0.574,970,0.1960,0 );
	DSTR << "thConduct:" << thConduct << std::endl;
	

	//断熱過程
	//SetInitThermoConductionParam(0.574,970,0.1960,0.0);		//> thConduct:熱伝導率 ,roh:密度,	specificHeat:比熱 J/ (K・kg):1960 ,　heatTrans:熱伝達率 W/(m^2・K)
	//これら、変数値は後から計算の途中で変更できるようなSetParam()関数を作っておいたほうがいいかな？

	//> 熱流束の初期化
	//SetVtxHeatFluxAll(0.0);


	//>	行列の作成　行列の作成に必要な変数はこの行以前に設定が必要
		//計算に用いるマトリクス、ベクトルを作成（メッシュごとの要素剛性行列/ベクトル⇒全体剛性行列/ベクトル）
		//{T}縦ベクトルの節点の並び順に並ぶように、係数行列を加算する。係数行列には、面積や体積、熱伝達率などのパラメータの積をしてしまったものを入れる。


	//> IH加熱するfaceをある程度(表面face && 下底面)絞る、関係しそうなface節点の原点からの距離を計算し、face[].mayIHheatedを判定
	//CalcVtxDisFromOrigin();
	CalcVtxDisFromVertex(Vec2d(0.0, -0.005));		// 鉄板の原点座標を引数に入れ、対称性が合うようになっている。
	
	//>	IHからの単位時間当たりの加熱熱量
	//単位時間当たりの総加熱熱量	231.9; //>	J/sec
		
	//..debug 
	//バンド状加熱
	//	CalcIHdqdtband_(-0.02,0.20,231.9 * 0.005 * 1e6);

	
	//%%	IH加熱のモード切替
	//	ライン状に加熱
	//	CalcIHdqdtband_(0.09,0.10,231.9 * 5e3);		//*0.5*1e4	値を変えて実験	//*1e3　//*1e4 //5e3
	//	円環状に加熱
	
	//CalcIHarea(0.04,0.095,231.9 * 0.005 * 1e6);		//	使っている？



	//	この後で、熱流束ベクトルを計算する関数を呼び出す
	InitCreateMatC();					///	CreateMatCの初期化
	InitVecFAlls();					///	VecFAll類の初期化
	InitCreateMatk();					///	CreateMatKの初期化
	//..	CreateLocalMatrixAndSet();			//> 以上の処理を、この関数に集約

	keisuInv.resize(mesh->vertices.size(),mesh->vertices.size());
	keisuInv.clear();
	rightkeisu.resize(mesh->vertices.size(),mesh->vertices.size());
	rightkeisu.clear();

	//std::ofstream matktetout;
	////各四面体のmatKを出力
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	matktetout.open("matktetout.csv");
	//}

	//	四面体ごとに計算
	for(unsigned i=0; i < mesh->tets.size(); i++){

#if 1
		/*小野原追加ここから--------------------------------------------*/
		//表面faceの面積を計算
		for(unsigned j= 0 ; j < 4; j++){
			if(mesh->tets[i].faceIDs[j] < (int)mesh->nSurfaceFace){			///	外殻の面
				///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
				faceVars[mesh->tets[i].faceIDs[j]].area = CalcTriangleArea(mesh->faces[mesh->tets[i].faceIDs[j]].vertexIDs[0], mesh->faces[mesh->tets[i].faceIDs[j]].vertexIDs[1], mesh->faces[mesh->tets[i].faceIDs[j]].vertexIDs[2]);
			}
		}
		/*小野原追加ここまで--------------------------------------------*/
#endif
		tetVars[i].volume = CalcTetrahedraVolume2(i);

		//各行列を作って、ガウスザイデルで計算するための係数の基本を作る。Timestepの入っている項は、このソース(SetDesc())では、実現できないことが分かった(NULLが返ってくる)
		CreateMatkLocal(i);				///	Matk1 Matk2(更新が必要な場合がある)を作る	//ifdefスイッチで全体剛性行列も(表示用だが)生成可能
		//CreateMatKall();		//CreateMatkLocal();に実装したので、後程分ける。
		CreatedMatCAll(i);
		CreateVecFAll(i);

	//	//四面体ごとにmatkを出力
	//	if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//		//matktetout << i << std::endl;
	//		DSTR << i << std::endl;
	//		for(unsigned k=0; k < 4; k++){
	//			for(unsigned l=0; l < 4; l++){
	//				//matktetout << matk[k][l] << ",";
	//				DSTR << std::setw(13) << matk[k][l] << "\t";
	//			}
	//			//matktetout << std::endl;
	//			DSTR << std::endl;
	//		}
	//		//matktetout << std::endl;
	//		DSTR << std::endl;
	//	}
	}

	////fを分割する比率を表すfpatを作成
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	makeFPartition(1);
	//	DSTR << "fpat" << std::endl;
	//	for(unsigned i=0; i < mesh->vertices.size(); i++){
	//		for(unsigned j=0; j < mesh->vertices.size(); j++){
	//			DSTR << std::setw(5) << fpat[i][j] << "\t";
	//		}
	//		DSTR << std::endl;
	//	}
	//	DSTR << std::endl;
	//}

	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	matktetout.close();
	//}

	//for(unsigned j =0;j<mesh->vertices.size();j++){
	//	if(matKAll[j][j] != dMatKAll[0][j]){
	//		DSTR << j <<" 成分の要素はおかしい！調査が必要である。 matKAll：" << matKAll[j][j] << " dMatKAll：" << dMatKAll[0][j] <<std::endl;
	//	}
	//}

	for(unsigned vtxid=0; vtxid < mesh->vertices.size(); vtxid++){
		CalcVertexVolume(vtxid);
		//if(strcmp(mesh->GetName(), "femNsteak") == 0){
		//	DSTR << vtxid << ":" << vertexVars[vtxid].vVolume << std::endl;
		//}
	}

	TVecAllSub.resize(mesh->vertices.size());
	for(unsigned vtxid=0; vtxid < mesh->vertices.size(); vtxid++){
		TVecAllSub[vtxid] = initial_temp;
	}
//	if(strcmp(mesh->GetName(), "femPan") == 0){
//		for(unsigned vtxid=0; vtxid < mesh->vertices.size(); vtxid++){
//			vecFOutPan << vecFAllSum[vtxid] << ",";
//		}
//		vecFOutPan << std::endl;
//	}
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	//for(unsigned vtxid=0; vtxid < mesh->vertices.size(); vtxid++){
	//	//	vecFOutSteak << vecFAllSum[vtxid] << ",";
	//	//}
	//	//vecFOutSteak << std::endl;
	//	std::ofstream dtKout;
	//	std::ofstream Cout;
	//	dtKout.open("dtKSteak.txt");
	//	Cout.open("CSteak.txt");

	//	//dtKout << "[";
	//	//Cout << "[";
	//	for(unsigned i=0; i < mesh->vertices.size(); i++){
	//		for(unsigned j=0; j < mesh->vertices.size(); j++){
	//			dtKout << tdt * matKAll[i][j] << " ";
	//			Cout << matCAll[i][j] << " ";
	//		}
	//		dtKout/* << ";" */<< std::endl;
	//		Cout/* << ";" */<< std::endl;
	//	}
	//	//dtKout << "]";
	//	//Cout <<"]"; 

	//	dtKout.close();
	//	Cout.close();
	//}

#if 1
	//頂点の表面面積を計算（小野原追加）
	calcVerticesArea();
#endif

#if 0

	//	頂点１の担当体積に対し、熱量を加えるために、担当体積換算で熱量を頂点の温度として与える。{F}を使わないので、熱流束を使わない。
	double rcv=0.0;
	for(unsigned i=0;i<vertices[0].tets.size();i++){
		rcv += tets[vertices[0].tets[i]].volume * RHO * SPECIFICHEAT * 5 / 20; 
	}
	double kuwae =1.58;	//	加える熱量
	//vertices[0].temp = kuwae / rcv;
	SetVertexTemp(0,kuwae / rcv);
#endif
	//このtempをTVecAllに設定
	//C,Kだけの計算をさせて様子を見る


	int hogeshidebug =0;
	//	節点温度推移の書き出し
//	templog.open("templog.csv");

	//matCAllout.open("matCAllout.txt"); 
	//matKAllout.open("matKAllout.txt");
	//
	//matCAllout << matCAll << std::endl;
	//matKAllout << matKAll << std::endl;
	//
	//matCAllout.close();

	//matCAllout.open("matCAll-1out.txt");
	//matCAllout << matCAll.inv() << std::endl;
	//matCAllout.close();
	////scilabを呼ばないで、SPRの機能で、C.inv() K を求める
	//matCAllout.open("matCAll.inv()xmatKAll.txt");
	//matCAllout << matCAll.inv() * matKAll << std::endl; 
	//matCAllout.close();

	//tempMat.resize(vertices.size(),vertices.size());
	//tempMat.clear();
	//tempMat = matCAll.inv() * matKAll;

	//std::vector<double> rowval;
	//double tempval=0;
	//for(unsigned i=0;i<vertices.size();i++){
	//	for(unsigned j=0;j<vertices.size();j++){
	//	tempval += tempMat[i][j];
	//	}
	//	rowval.push_back(tempval);
	//}
	//matCAllout.open("matCAll.inv()xmatKAllの各列の和.txt");
	//for(unsigned i=0;i<vertices.size();i++){
	//	matCAllout << rowval[i] <<std::endl;
	//}
	//matCAllout.close();

	//カウントの初期化
	COUNT = 0;

	//温度変化出力
	checkTVecAllout.open("checkTVecAllout.csv");
	checkTVecAllout <<"時間" << COUNT<<", ";
	for(unsigned i=0; i < mesh->vertices.size();i++){
		if(i != mesh->vertices.size() -1){
			checkTVecAllout << "頂点" << i << ", ";	
		}
		else{
			checkTVecAllout << "頂点" << i << std::endl;
		}
	}
	FEMLOG.open("femLog.csv");

	//	CPSの経時変化を書き出す
	//cpslog.open("cpslog.csv");

	// カウントの初期化
	Ndt =0;

	//水分蒸発周りの初期化
	//InitMoist();

	//this->OutputMatKall();

	InitFaceNormalAll();
	CalcFaceNormalAll();
	InitVertexNormalAll();
	CalcVertexNormalAll();

//#ifdef UseMatAll
//	if(strcmp(mesh->GetName(), "femNsteak") == 0){
//		OutEig();
//	}
//#endif

	I.resize(mesh->vertices.size(), mesh->vertices.size());
	I.clear();
	for(unsigned i=0; i < mesh->vertices.size(); i++){
		I[i][i] = 1.0;
	}
	makeAFlag = true;
	if(strcmp(mesh->GetName(), "femNsteak") == 0){
		convTest.resize(mesh->vertices.size());
		ggConvOut.open("ggConvOut.csv");
		for(unsigned i=0; i < mesh->vertices.size(); i++){
			ggConvOut << "頂点" << i << ",";
		}
		ggConvOut << std::endl;
	}

}

void PHFemThermo::SetStopTimespan(double timespan){
	stopTime = timespan;
	//	本来は、PHSceneが時間をmanageすべきなため、暫定的
}
void PHFemThermo::SetIHParamWEEK(double inr2, double outR2, double weekPow2){
	inr_ = inr2;
	outR_ = outR2;
	weekPow_ = weekPow2;
}
void PHFemThermo::SetWeekPow(double weekPow2){
	weekPow_ = weekPow2;
}

void PHFemThermo::SetOuterTemp(double temp){
	jout = temp;
}
void PHFemThermo::SetThermalRadiation(double ems_,double ems_const_){
	ems = ems_;
	ems_const = ems_const_;
}
void PHFemThermo::SetGaussCalcParam(unsigned cyc,double epsilon_){
	NofCyc = cyc;
	epsilonG = epsilon_;
}

double PHFemThermo::CalcTempInnerTets(unsigned id,PTM::TVector<4,double> N){
	PHFemMeshNew* mesh = GetPHFemMesh();

	double temp = 0.0;
	for(unsigned i=0;i<4;i++){
		 temp += N[i] * vertexVars[mesh->tets[id].vertexIDs[i]].temp;
	}
	return temp;
};


double PHFemThermo::GetVtxTempInTets(Vec3d temppos){
	PHFemMeshNew* mesh = GetPHFemMesh();

	PTM::TMatrixCol<4,4,double> Vertex;		//	四面体を成す4点の位置座標
	PTM::TVector<4,double> coeffk;			//	形状関数的な？
	PTM::TVector<4,double> arbitPos;		//	任意点座標
	// [a][x] = [b]を解く
	Vertex.clear();		//a
	coeffk.clear();		//x
	arbitPos.clear();	//b
	// b: ax =b ：単に逆行列から求めるとき
	arbitPos[0] = temppos[0];
	arbitPos[1] = temppos[1];
	arbitPos[2] = temppos[2];
	arbitPos[3] = 1.0;
	for(unsigned id =0;  id < mesh->tets.size(); id++){
		// 四面体ごとに点が含まれるか判定		
		for(unsigned j=0; j < 4;j++){
			Vertex[0][j] = mesh->vertices[mesh->tets[id].vertexIDs[j]].pos.x;
			Vertex[1][j] = mesh->vertices[mesh->tets[id].vertexIDs[j]].pos.y;
			Vertex[2][j] = mesh->vertices[mesh->tets[id].vertexIDs[j]].pos.z;
			Vertex[3][j] = 1.0;
		}
		// 逆行列で解く
		coeffk = Vertex.inv() * arbitPos;
		//	四面体の
		if( 0-1e-8 <= coeffk[0] && coeffk[0] <= 1+1e-8 && 0-1e-8 <= coeffk[1] && coeffk[1] <= 1+1e-8 && 0-1e-8 <= coeffk[2] && coeffk[2] <= 1+1e-8 && 0-1e-8 <= coeffk[3] && coeffk[3] <= 1+1e-8 ){	//	近接四面体に入ってしまう場合がありそう。その区別がつかないので、0や1で区切る方が良いと思う。
			//	形状関数から、四面体内の温度を求める
			return CalcTempInnerTets( id , coeffk);		
		}
		coeffk.clear();
	}
	return DBL_MAX;		//	見つからなかったサイン
}



struct ID_LENGTH{
	unsigned id;
	double coord;	//value of axis
};
class LessLength{
public:
	bool operator()(const ID_LENGTH& a, const ID_LENGTH& b)
	{
		return a.coord < b.coord;
	}
};

Vec3d PHFemThermo::GetDistVecDotTri(Vec3d Dotpos,Vec3d trivtx[3]){
	//				 a
	//				/|
	//			   / |
	//		.Q	  / P|
	//			 / . |
	//	.		/____|
	//	O		b   c
	//	triedge[0] = b->a, triedge[1] = b->c,
	//  QP =  OP - OQ = Ob + param[0] * ba + param[1] * bc - OQ
	//	QP⊥ba,QP⊥bc => param[0~1]を求める 
	double param[2] = {0.0,0.0}; 
	Vec3d triedge[2] = {Vec3d(0.0,0.0,0.0),Vec3d(0.0,0.0,0.0)};
	//
	triedge[0] = trivtx[1] - trivtx[0];
	triedge[1] = trivtx[2] - trivtx[0];
	param[1] = (triedge[0] * trivtx[0]) * (triedge[0] * triedge[1]) - triedge[0].norm() * triedge[0].norm() * (trivtx[0] * triedge[1])
		/ ( (triedge[0].norm() * triedge[0].norm() )  *  (triedge[1].norm() * triedge[1].norm() ) - (triedge[0] * triedge[1]) * (triedge[0] * triedge[1]) );
	param[0] = (-1) / (triedge[0].norm() * triedge[0].norm()) * ((triedge[0] * triedge[1]) * param[1] + (triedge[0] * trivtx[0]));
	Vec3d VecQP = trivtx[0] + param[0] * triedge[0] + param[1] * triedge[1] - Dotpos;
	return VecQP;
}

#if 1		// 以下、実装が途中?
double PHFemThermo::GetArbitraryPointTemp(Vec3d temppos){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//tempposがどの四面体に属するか
	//四面体のface面の向きで判定
	DSTR << "from origin: (0.0,0.0,0.0) " << std::endl;
	for(unsigned i=0;i<mesh->faces.size(); i++){
		Vec3d facevtx[3] = {mesh->vertices[mesh->faces[i].vertexIDs[0]].pos,mesh->vertices[mesh->faces[i].vertexIDs[1]].pos,mesh->vertices[mesh->faces[i].vertexIDs[2]].pos};
		DSTR << "facevtx[0]: " << facevtx[0] << "facevtx[1]: " << facevtx[1] << "facevtx[2]: " << facevtx[2] << std::endl;
		DSTR <<"i: " << i <<", GetDistVecDotTri(temppos,facevtx): " << GetDistVecDotTri(temppos,facevtx) << std::endl;
		DSTR << std::endl;
	}
	//小さい順にsortしてくれるコンテナを使う map? list? 中には、長さと頂点idを入れる。最初から4つめまでの頂点IDEALLYを含む四面体を見つける
	//最初に、faceとマッチングをとって、その後で、そのfaceを含む四面体とのマッチングをとる方法もありそう。具体的なアルゴリズムが浮かばない
	double length = 0.0;
	for(unsigned id=0; id < mesh->vertices.size(); id++){
		//一番近い順に４つの点をソート
		length = sqrt( (temppos.x - mesh->vertices[id].pos.x) * (temppos.x - mesh->vertices[id].pos.x)
			+ (temppos.y - mesh->vertices[id].pos.y) * (temppos.y - mesh->vertices[id].pos.y)
			+ (temppos.z - mesh->vertices[id].pos.z) * (temppos.z - mesh->vertices[id].pos.z) 
			);
	}
	//AABBでやる
	//4点からx,y,zのmin/max
	//小さい順位並べる
	ID_LENGTH idl;
	std::vector<ID_LENGTH> id_length_x;
	//std::vector<double> xarray,yarray,zarray;
	for(unsigned id=0; id < mesh->tets.size(); id++){
		for(unsigned j=0; j < 4; j++){
			idl.id = mesh->tets[id].vertexIDs[j];
			idl.coord = mesh->vertices[mesh->tets[id].vertexIDs[j]].pos.x;
			id_length_x.push_back(idl);
		}
	}
	DSTR << "begfore .sort()" << std::endl;
	for(unsigned i=0; i < id_length_x.size() ; i++){
		DSTR << "id_length_x["<< i <<"].id: " << id_length_x[i].id << " .coord: " << id_length_x[i].coord << std::endl; 
	}
	//tempposに近い順に並べる
	std::sort( id_length_x.begin(), id_length_x.end(), LessLength());
	
	DSTR << "after .sort()" << std::endl;
	for(unsigned i=0; i < id_length_x.size() ; i++){
		DSTR << "id_length_x["<< i <<"].id: " << id_length_x[i].id << " .coord: " << id_length_x[i].coord << std::endl; 
	}

	//最近と最遠をXmin、xmaxに入れる。その際に、頂点idが必要なので、上記構造体では、座標値(param)を調べるソートアルゴリズムを記述する。
	//paramが最小値の時のid、最大値のときのidを使って、頂点の識別をする
		
	//AABBから、tempposeがこの四面体の中か、近くか、判定する。
	//中なら良し！
	//中じゃなければ、近隣の四面体を探す
	

	//属する四面体の形状関数から、温度を取得 T=NTより

	//温度を取得できたら、返す。ダメならnullかDBL_MAX
	//if(temp) return temp:
	//else	return null;
	return 1;
}
#endif

void PHFemThermo::CalcVtxDisFromOrigin(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//>	nSurfaceの内、x,z座標から距離を求めてsqrt(2乗和)、それをFemVertexに格納する
	//> 同心円系の計算に利用する　distance from origin
	
	/// 判定フラグの初期化
	for(unsigned i=0; i<mesh->nSurfaceFace; i++){
		faceVars[i].mayIHheated = false;
	}
	/// 初期化
	for(unsigned i =0;i<mesh->vertices.size();i++){
		vertexVars[i].disFromOrigin =0.0;
	}

	/// debug
	//DSTR << "faces.size(): " << faces.size() << std::endl;

	//> 表面faceの内、原点から各faceの節点のローカル(x,z)座標系での平面上の距離の計算を、faceの全節点のy座標が負のものに対して、IH加熱の可能性を示すフラグを設定
	for(unsigned i=0;i<mesh->nSurfaceFace;i++){
		//> 表面のfaceの全節点のy座標が負ならば、そのfaceをIH加熱のface面と判定し、フラグを与える
		if(mesh->vertices[mesh->faces[i].vertexIDs[0]].pos.y < 0.0 && mesh->vertices[mesh->faces[i].vertexIDs[1]].pos.y < 0.0 && mesh->vertices[mesh->faces[i].vertexIDs[2]].pos.y < 0.0){
			faceVars[i].mayIHheated = true;
			//	(x,z)平面におけるmayIHheatedのface全節点の原点からの距離を計算する
			for(unsigned j=0; j<3; j++){
				vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin = sqrt(mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x * mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x + mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.z * mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.z);
			}
		}
	}

	//	debug		//>	高速化対応時にはコメントアウトする
	//>	座標値を確認する
	for(unsigned i=0; i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){
			/// 3つの頂点の組み＝辺のx,zが同じで、y座標だけが異なる点の組みがないことを確認する
			for(unsigned j=0;j<3;j++){
				if(mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x == mesh->vertices[mesh->faces[i].vertexIDs[(j+1)%3]].pos.x
					&& mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.z == mesh->vertices[mesh->faces[i].vertexIDs[(j+1)%3]].pos.z
					 && mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.y != mesh->vertices[mesh->faces[i].vertexIDs[(j+1)%3]].pos.y){
					DSTR <<i << "th: " << mesh->vertices[mesh->faces[i].vertexIDs[j]].pos << " : " << mesh->vertices[mesh->faces[i].vertexIDs[(j+1)%3]].pos << " : " << mesh->vertices[mesh->faces[i].vertexIDs[(j+2)%3]].pos<<std::endl;
					DSTR << "CalcVtxDisFromOrigin() error" << std::endl;
					assert(0);
				}
			}
		}
	}
}

void PHFemThermo::CalcVtxDisFromVertex(Vec2d originVertexIH){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//>	nSurfaceの内、x,z座標から距離を求めてsqrt(2乗和)、それをFemVertexに格納する
	//> 同心円系の計算に利用する　distance from origin
	
	/// 判定フラグの初期化
	for(unsigned i=0; i<mesh->nSurfaceFace; i++){
		faceVars[i].mayIHheated = false;
	}
	/// 初期化
	for(unsigned i =0;i<mesh->vertices.size();i++){
		vertexVars[i].disFromOrigin =0.0;
	}
	//> 表面faceの内、原点から各faceの節点のローカル(x,z)座標系での平面上の距離の計算を、faceの全節点のy座標が負のものに対して、IH加熱の可能性を示すフラグを設定
	for(unsigned i=0;i<mesh->nSurfaceFace;i++){
		//> 表面のfaceの全節点のy座標が負ならば、そのfaceをIH加熱のface面と判定し、フラグを与える
		if(mesh->vertices[mesh->faces[i].vertexIDs[0]].pos.y < 0.0 && mesh->vertices[mesh->faces[i].vertexIDs[1]].pos.y < 0.0 && mesh->vertices[mesh->faces[i].vertexIDs[2]].pos.y < 0.0){
			faceVars[i].mayIHheated = true;
			//	(x,z)平面におけるmayIHheatedのface全節点の原点からの距離を計算する
			for(unsigned j=0; j<3; j++){
				double dx = mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x - originVertexIH[0];
				double dz = mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.z - originVertexIH[1];	//	表記はyだが、実質z座標が入っている
				vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin = sqrt( dx * dx + dz * dz);
			}
		}
	}
}


void PHFemThermo::ScilabTest(){
	if (!ScilabStart()) std::cout << "Error : ScilabStart \n";

	//	行列の読み書き
	Matrix2f A;
	A.Ex() = Vec2f(1,2);
	A.Ey() = Vec2f(3,4);
	std::cout << "A:  " << A ;
	std::cout << "A00:" << A[0][0] << std::endl;
	std::cout << "A01:" << A[0][1] << std::endl;
	std::cout << "A10:" << A[1][0] << std::endl;
	std::cout << "A11:" << A[1][1] << std::endl;
	
	ScilabSetMatrix("A", A);
	ScilabJob("b=[4;5]");
	std::cout << "A=";
	ScilabJob("disp(A);");
	std::cout << "b=";
	ScilabJob("disp(b);");
	std::cout << "x=A\\b" << std::endl;
	ScilabJob("A,b,x=A\\b;");
	
	ScilabGetMatrix(A, "A");
	SCMatrix b = ScilabMatrix("b");
	SCMatrix x = ScilabMatrix("x");
	std::cout << "x:" << x << std::endl;

	ScilabJob("y = A;");
	SCMatrix y = ScilabMatrix("y");
	std::cout << "y=" << y;
	y = 2*A;
	std::cout << "y = 2*A is done by C++ code" << std::endl;
	std::cout << "y=";
	ScilabJob("disp(y);");
	std::cout << A;
	std::cout << y;
	ScilabJob("clear;");

	//	グラフ描画
	ScilabJob("t = 0:0.01:2*3.141592653;");
	ScilabJob("x = sin(t);");
	ScilabJob("y = cos(t);");
	ScilabJob("plot2d(x, y);");
	for(int i=0; i<100000; ++i){
		ScilabJob("");
	}
//	ScilabEnd();
}

void PHFemThermo::UsingFixedTempBoundaryCondition(unsigned id,double temp){
	//温度固定境界条件
	SetVertexTemp(id,temp);
}

void PHFemThermo::UsingHeatTransferBoundaryCondition(unsigned id,double temp,double heatTransRatio){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//熱伝達境界条件
	//節点の周囲流体温度の設定(K,C,Fなどの行列ベクトルの作成後に実行必要あり)
//	if(vertices[id].Tc != temp){					//更新する節点のTcが変化した時だけ、TcやFベクトルを更新する
	SetLocalFluidTemp(id,temp);
	vertexVars[id].heatTransRatio = heatTransRatio;
	//熱伝達境界条件が使われるように、する。				///	＋＝してベクトルを作っているので、下のコードでは、余計に足してしまっていて、正しい行列を作れない。
	//for(unsigned i =0;i < vertices[id].tets.size();i++){
	//	CreateVecfLocal(tets[vertices[id].tets[i]]);		//	Tcを含むベクトルを更新する
	//}

	///	熱伝達率を含む項(K2,f3)のみ再計算
	InitCreateVecf_();				///	変更する必要のある項のみ、入れ物を初期化
	InitCreateMatk_();
	for(unsigned i =0; i < mesh->edges.size();i++){
		edgeVars[i].k = 0.0;
	}
	for(unsigned i=0; i< mesh->tets.size();i++){
		CreateVecFAll(i);				///	VecFの再作成
		CreateMatkLocal(i);				///	MatK2の再作成 →if(deformed==true){matk1を生成}		matK1はmatk1の変数に入れておいて、matk2だけ、作って、加算
	}
//	}
	///	節点の属する面のalphaUpdatedをtrueにする
	for(unsigned i=0;i<mesh->vertices[id].faceIDs.size();i++){
		faceVars[mesh->vertices[id].faceIDs[i]].alphaUpdated = true;
		alphaUpdated = true;
	}
}

void PHFemThermo::UsingHeatTransferBoundaryCondition(unsigned id,double temp){
	//熱伝達境界条件
	//節点の周囲流体温度の設定(K,C,Fなどの行列ベクトルの作成後に実行必要あり)
//	if(vertices[id].Tc != temp){					//更新する節点のTcが変化した時だけ、TcやFベクトルを更新する
		SetLocalFluidTemp(id,temp);
		//熱伝達境界条件が使われるように、する。
		//for(unsigned i =0;i < vertices[id].tets.size();i++){
		//	CreateVecfLocal(tets[vertices[id].tets[i]]);		//	Tcを含むベクトルを更新する
		//}
		InitCreateVecf_();
		for(unsigned i=0; i < GetPHFemMesh()->tets.size();i++){
			CreateVecFAll(i);				///	VeecFの再作成
													///	MatK2の再作成→matK1はmatk1の変数に入れておいて、matk2だけ、作って、加算
		}
//	}
}

void PHFemThermo::SetRhoSpheat(double r,double Spheat){
	//> 密度、比熱 of メッシュのグローバル変数(=メッシュ固有の値)を更新
	rho = r;
	specificHeat = Spheat;
}

std::vector<Vec2d> PHFemThermo::CalcIntersectionPoint2(unsigned id0,unsigned id1,double r,double R){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	2点を通る直線は1つ	2つの定数を求める
	double constA = 0.0;
	double constB = 0.0;
	///	rと交点
	double constX1 = 0.0;
	double constX1_ = 0.0;
	double constY1 = 0.0;
	///	Rと交点
	double constX2 = 0.0;
	double constX2_ = 0.0;
	double constY2 = 0.0;

	//> 引数の代替処理	関数化したときに、変換する↓
	unsigned vtxId0 = id0;
	unsigned vtxId1 = id1;
	/// 原点に近い順に並び替え
	if(vertexVars[vtxId0].disFromOrigin > vertexVars[vtxId1].disFromOrigin){
		unsigned tempId = vtxId0;
		vtxId0 = vtxId1;
		vtxId1 = tempId;
	}
	/// vtxId0 < vtxId1 が保証されている

	//	2点で交わることが前提
	//> 2点のdisFromOriginをr,Rと比較してどちらと交わるかを判別する。
	//> 円環との交点を求める
	// x-z平面で考えている
	/// constA,B:vtxId0.vtxId1を通る直線の傾きと切片　/// aconsta,constbは正負構わない
	DSTR << "id0: " << id0 << ", id1: " << id1 <<std::endl;
	constA = ( mesh->vertices[vtxId0].pos.z - mesh->vertices[vtxId1].pos.z) / ( mesh->vertices[vtxId0].pos.x - mesh->vertices[vtxId1].pos.x);
	DSTR << "vertices[vtxId0].pos.z: " << mesh->vertices[vtxId0].pos.z <<std::endl;
	DSTR << "vertices[vtxId0].pos.z: " << mesh->vertices[vtxId1].pos.z <<std::endl;
	DSTR << "dz: vertices[vtxId0].pos.z - vertices[vtxId1].pos.z : " << mesh->vertices[vtxId0].pos.z - mesh->vertices[vtxId1].pos.z << std::endl;

	DSTR << "vertices[vtxId0].pos.x: " << mesh->vertices[vtxId0].pos.x << std::endl;
	DSTR << "vertices[vtxId1].pos.x: " << mesh->vertices[vtxId1].pos.x << std::endl;
	DSTR << "dx: vertices[vtxId0].pos.x - vertices[vtxId1].pos.x: " << mesh->vertices[vtxId0].pos.x - mesh->vertices[vtxId1].pos.x << std::endl;

	DSTR << "constA = dz / dx: " << constA << std::endl;
	if(mesh->vertices[vtxId0].pos.z == mesh->vertices[vtxId1].pos.z && mesh->vertices[vtxId0].pos.x == mesh->vertices[vtxId1].pos.x){
		DSTR << "vertices[vtxId0].pos.y: " << mesh->vertices[vtxId0].pos.y << ", vertices[vtxId1].pos.y: " << mesh->vertices[vtxId1].pos.y << std::endl;
		if(mesh->vertices[vtxId0].pos.y == mesh->vertices[vtxId1].pos.y)
			DSTR << "id[" << id0 <<"], id[" << id1 << "] は同じ頂点 !" << std::endl;
	}
	
	constB = mesh->vertices[vtxId0].pos.z - constA * mesh->vertices[vtxId0].pos.x;
	DSTR << "constB = vertices[vtxId0].pos.z - constA * vertices[vtxId0].pos.x : " << mesh->vertices[vtxId0].pos.z - constA * mesh->vertices[vtxId0].pos.x << std::endl;

	///	交点の座標を計算
	if(vertexVars[vtxId0].disFromOrigin < r){		/// 半径rの円と交わるとき
		//CalcYfromXatcross(vtxId0,vtxId1,r);	//関数化しない
		//> 以下、関数化,vtxId0,1,r:引数、constYを返す
		constX1 = (- constA * constB + sqrt(r * r *(constA * constA + 1.0) - constB * constB));
		constX1_ = (- constA * constB - sqrt(r * r *(constA * constA + 1.0) - constB * constB));
		// どちらかが頂点の間にある　大小がわからないので、orで、点1のx座標、2のx座標と、その入れ替えと、作る
		//> 線分の両端の点の間にあるとき
		if( (mesh->vertices[vtxId0].pos.x <= constX1 && constX1 <= mesh->vertices[vtxId1].pos.x) || (mesh->vertices[vtxId1].pos.x <= constX1 && constX1 <= mesh->vertices[vtxId0].pos.x) ){
			constY1 = sqrt(r * r - constX1 * constX1 );
		}else{
			constY1 = sqrt(r * r - constX1_ * constX1_ );
			constX1 = constX1_;		///		点のx座標はconstX_が正しい事がわかった。
		}
	}else if(vertexVars[vtxId0].disFromOrigin < R){		/// 半径Rの円と交わるとき
		constX1 = (- constA * constB + sqrt(R * R *(constA * constA + 1.0) - constB * constB));
		constX1_ = (- constA * constB - sqrt(R * R *(constA * constA + 1.0) - constB * constB));
		// どちらかが頂点の間にある　大小がわからないので、orで、点1のx座標、2のx座標と、その入れ替えと、作る
		//> 線分の両端の点の間にあるとき
		if( (mesh->vertices[vtxId0].pos.x <= constX1 && constX1 <= mesh->vertices[vtxId1].pos.x) || (mesh->vertices[vtxId1].pos.x <= constX1 && constX1 <= mesh->vertices[vtxId0].pos.x) ){
			constY1 = sqrt(R * R - constX1 * constX1 );
		}else{
			constY1 = sqrt(R * R - constX1_ * constX1_ );
			constX1 = constX1_;		///		点のx座標はconstX_が正しい事がわかった。
		}
		
	}
	//> どちらとも交わるとき
	else if(vertexVars[vtxId0].disFromOrigin < r && R < vertexVars[vtxId1].disFromOrigin){
		//> 定数が2つ欲しい
		constX1 = (- constA * constB + sqrt(r * r *(constA * constA + 1.0) - constB * constB));
		constX1_ = (- constA * constB - sqrt(r * r *(constA * constA + 1.0) - constB * constB));
		// どちらかが頂点の間にある　大小がわからないので、orで、点1のx座標、2のx座標と、その入れ替えと、作る
		//> 線分の両端の点の間にあるとき
		if( (mesh->vertices[vtxId0].pos.x <= constX1 && constX1 <= mesh->vertices[vtxId1].pos.x) || (mesh->vertices[vtxId1].pos.x <= constX1 && constX1 <= mesh->vertices[vtxId0].pos.x) ){
			constY1 = sqrt(r * r - constX1 * constX1 );
		}else{
			constY1 = sqrt(r * r - constX1_ * constX1_ );
			constX1 = constX1_;		///		点のx座標はconstX_が正しい事がわかった。
		}
		constX2 = (- constA * constB + sqrt(R * R *(constA * constA + 1.0) - constB * constB));
		constX2_ = (- constA * constB - sqrt(R * R *(constA * constA + 1.0) - constB * constB));
		//> 線分の両端の点の間にあるとき
		if( (mesh->vertices[vtxId0].pos.x <= constX2 && constX2 <= mesh->vertices[vtxId1].pos.x) || (mesh->vertices[vtxId1].pos.x <= constX2 && constX2 <= mesh->vertices[vtxId0].pos.x) ){
			constY2 = sqrt(R * R - constX2 * constX2 );
		}else{
			constY2 = sqrt(R * R - constX2_ * constX2_ );
			constX2 = constX2_;		///		点のx座標はconstX_が正しい事がわかった。
		}
	}
	std::vector<Vec2d> intersection;
	intersection.push_back(Vec2d(constX1,constY1));
	if(constX2 && constY2){
		intersection.push_back(Vec2d(constX2,constY2));
	}
		//Vec4d interSection;
		//interSection[0] = constX1;
		//interSection[1] = constY1;
		//interSection[2] = constX2;
		//interSection[3] = constY2;
		DSTR << __FILE__  << "(" <<  __LINE__ << "):"<< "intersection Vtx: " << intersection[0] << std::endl;
		//":" <<  __TIME__ << 
		//return interSection;
		return intersection;
}

Vec2d PHFemThermo::CalcIntersectionPoint(unsigned id0,unsigned id1,double r,double R){
	PHFemMeshNew* mesh = GetPHFemMesh();

	double constA = 0.0;
	double constB = 0.0;
	double constX = 0.0;
	double constX_ = 0.0;
	double constY = 0.0;

	//> 引数の代替処理	関数化したときに、変換する↓
	unsigned vtxId0 = id0;
	unsigned vtxId1 = id1;
	/// 原点に近い順に並び替え
	if(vertexVars[vtxId0].disFromOrigin > vertexVars[vtxId1].disFromOrigin){
		unsigned tempId = vtxId0;
		vtxId0 = vtxId1;
		vtxId1 = tempId;
	}
	/// vtxId0 < vtxId1 が保証されている

	//> 2点のdisFromOriginをr,Rと比較してどちらと交わるかを判別する。
	if( (r <= vertexVars[vtxId0].disFromOrigin && vertexVars[vtxId0].disFromOrigin <= R) ^ (r <= vertexVars[vtxId1].disFromOrigin && vertexVars[vtxId1].disFromOrigin <= R)){
		//> 円環との交点を求める
		// x-z平面で考えている
		/// constA,B:vtxId0.vtxId1を通る直線の傾きと切片　/// aconsta,constbは正負構わない
		constA = ( mesh->vertices[vtxId0].pos.z - mesh->vertices[vtxId1].pos.z) / ( mesh->vertices[vtxId0].pos.x - mesh->vertices[vtxId1].pos.x);
		constB = mesh->vertices[vtxId0].pos.z - constA * mesh->vertices[vtxId0].pos.x;

		///	交点の座標を計算
		if(vertexVars[vtxId0].disFromOrigin < r){		/// 半径rの円と交わるとき
			//CalcYfromXatcross(vtxId0,vtxId1,r);	//関数化しない
			//> 以下、関数化,vtxId0,1,r:引数、constYを返す
			constX = (- constA * constB + sqrt(r * r *(constA * constA + 1.0) - constB * constB));
			constX_ = (- constA * constB - sqrt(r * r *(constA * constA + 1.0) - constB * constB));
			// どちらかが頂点の間にある　大小がわからないので、orで、点1のx座標、2のx座標と、その入れ替えと、作る
			//> 線分の両端の点の間にあるとき
			if( (mesh->vertices[vtxId0].pos.x <= constX && constX <= mesh->vertices[vtxId1].pos.x) || (mesh->vertices[vtxId1].pos.x <= constX && constX <= mesh->vertices[vtxId0].pos.x) ){
				constY = sqrt(r * r - constX * constX );
			}else{
				constY = sqrt(r * r - constX_ * constX_ );
				constX = constX_;		///		点のx座標はconstX_が正しい事がわかった。
			}
		}else if(r < vertexVars[vtxId0].disFromOrigin && vertexVars[vtxId0].disFromOrigin < R){		/// 半径Rの円と交わるとき
			constX = (- constA * constB + sqrt(r * R *(constA * constA + 1.0) - constB * constB));
			constX_ = (- constA * constB - sqrt(r * R *(constA * constA + 1.0) - constB * constB));
			// どちらかが頂点の間にある　大小がわからないので、orで、点1のx座標、2のx座標と、その入れ替えと、作る
			//> 線分の両端の点の間にあるとき
			if( (mesh->vertices[vtxId0].pos.x <= constX && constX <= mesh->vertices[vtxId1].pos.x) || (mesh->vertices[vtxId1].pos.x <= constX && constX <= mesh->vertices[vtxId0].pos.x) ){
				constY = sqrt(R * R - constX * constX );
			}else{
				constY = sqrt(R * R - constX_ * constX_ );
				constX = constX_;		///		点のx座標はconstX_が正しい事がわかった。
			}
		}
		//> どちらとも交わるとき
		else if(vertexVars[vtxId0].disFromOrigin < r && R < vertexVars[vtxId1].disFromOrigin){
			//> 定数が2つ欲しい
		}
	}
		Vec2d interSection;
		interSection[0] = constX;
		interSection[1] = constY;
		DSTR << __FILE__  << "(" <<  __LINE__ << "):"<< "interSection: " << interSection << std::endl;
		//":" <<  __TIME__ << 
		return interSection;
}

void PHFemThermo::ArrangeFacevtxdisAscendingOrder(int faceID){
	PHFemMeshNew* mesh = GetPHFemMesh();

	///	3点を原点に近い順に並べる		//>	クイックソートにしたいかも？
	int vtxmin[3];		///	通しの頂点番号を入れる
	vtxmin[0] = mesh->faces[faceID].vertexIDs[0];
	vtxmin[1] = 0;
	vtxmin[2] = 0;
	if(vertexVars[mesh->faces[faceID].vertexIDs[1]].disFromOrigin < vertexVars[mesh->faces[faceID].vertexIDs[0]].disFromOrigin ){
		vtxmin[0] = mesh->faces[faceID].vertexIDs[1];
		vtxmin[1] = mesh->faces[faceID].vertexIDs[0];
	}else{
		vtxmin[1] = mesh->faces[faceID].vertexIDs[1];
	}
	if(vertexVars[mesh->faces[faceID].vertexIDs[2]].disFromOrigin < vertexVars[vtxmin[0]].disFromOrigin){
		vtxmin[2] = vtxmin[1];
		vtxmin[1] = vtxmin[0];
		vtxmin[0] = mesh->faces[faceID].vertexIDs[2];
	}else if(vertexVars[vtxmin[0]].disFromOrigin < vertexVars[mesh->faces[faceID].vertexIDs[2]].disFromOrigin && vertexVars[mesh->faces[faceID].vertexIDs[2]].disFromOrigin < vertexVars[vtxmin[1]].disFromOrigin){
		vtxmin[2] = vtxmin[1];
		vtxmin[1] = mesh->faces[faceID].vertexIDs[2];
	}else if(vertexVars[vtxmin[1]].disFromOrigin < vertexVars[mesh->faces[faceID].vertexIDs[2]].disFromOrigin ){
		vtxmin[2] = mesh->faces[faceID].vertexIDs[2];
	}
	//>	小さい順になっていないときは、assert(0)
	if( !(vertexVars[vtxmin[0]].disFromOrigin < vertexVars[vtxmin[1]].disFromOrigin && vertexVars[vtxmin[1]].disFromOrigin < vertexVars[vtxmin[2]].disFromOrigin )
		){	assert(0);}
	/// debug
	//DSTR << "小さい順 ";
	//for(unsigned j=0; j <3; j++){
	//	DSTR << vertices[vtxmin[j]].disFromOrigin;
	//	if(j<2){ DSTR << ", ";}
	//}
	//DSTR << std::endl;
	/// face内の配列にface内での原点から近い順番を格納
	for(unsigned i=0;i<3;i++){
		faceVars[faceID].ascendVtx[i] = vtxmin[i];
	}
	//DSTR << "vtxmin[0~2]:  " << vtxmin[0] <<" ," << vtxmin[1] << " ,"  << vtxmin[2] << std::endl;	
	//vtxmin[0~2]:  11 ,324 ,281 ,vtxmin[0~2]:  353 ,11 ,324 この順に原点に近い

	//	以下、削除
	//> 返す準備
	//Vec3i vtxarray = Vec3i(vtxmin[0],vtxmin[1],vtxmin[2]);
	//	ex	vtxarray: (    11    324    281)	,vtxarray: (   353     11    324)
	//DSTR << "vtxarray: " << vtxarray << std::endl; 
	//return vtxarray;		///	通しの頂点番号を返す
}

Vec2d PHFemThermo::CalcIntersectionOfCircleAndLine(unsigned id0,unsigned id1,double radius){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	x-z平面での半径rの円環と線分の交点の座標を求める
	//	関数が呼ばれる条件：交わることが明白なとき、交わる円弧の半径と、円弧の内・外側の頂点を引受け計算

	// ...2点を通る直線の方程式の傾きと切片を求める
	double constA = 0.0;	//	傾き
	double constB = 0.0;	//	切片
	double constX1 = 0.0;	//	交点のx 座標１
	double constX1_ = 0.0;	//		  x 座標候補２
	double constZ1 = 0.0;	//		  Y 座標
	double constZ1_ = 0.0;	//		  Y 座標候補２
	double constx = 0.0;	//	1次関数にならない場合の定数
	double constz = 0.0;	//　		同上

	//	...頂点番号を原点に近い＝小さい順( id0 < id1 )に並び替え
	if( vertexVars[ id1 ].disFromOrigin	<	vertexVars[ id0 ].disFromOrigin ){
		unsigned farfromOriginId = id0;
		id0 = id1;
		id1 = farfromOriginId;
	}	// id0 < id1 を保証

	// 1次関数にならない場合
	// .. x == const
	if(mesh->vertices[id0].pos.x == mesh->vertices[id1].pos.x && mesh->vertices[id0].pos.z != mesh->vertices[id1].pos.z ){
		//	x == const の式

	}
	// .. z == const

	// ...傾きと切片を求める
	//
	constA = ( mesh->vertices[id0].pos.z - mesh->vertices[id1].pos.z) / ( mesh->vertices[id0].pos.x - mesh->vertices[id1].pos.x);
	constB = mesh->vertices[id0].pos.z - constA * mesh->vertices[id0].pos.x;
	
	
	// ..次の条件は、以降の処理でエラーが出る場合を知らせるための、デバッグ用
	if(!constA && !constB){
		//if( id0 != id1)
		//	vertices[id0].pos.z != 0 && vertices[id1].pos.z!= 0 && vertices[id0].pos.x
		// z == 0という直線の式である。とｘ＝＝０という直線の式の時もあろう。
		//	この条件分岐の意味は、線分の両端の点のx,z座標が(0,0)や、直線の式が１次関数にならないと記を除くことができるときに、用いる。
		DSTR << "直線の式の切片と傾きが共に0" << std::endl;
		DSTR << "id0: " << id0 << ", id1: " << id1 << "radius: " << radius << std::endl;
		DSTR << "vertices[id0].pos.x: " << mesh->vertices[id0].pos.x << ", vertices[id1].pos.x: " << mesh->vertices[id1].pos.x << std::endl;
		DSTR << "vertices[id0].pos.z: " << mesh->vertices[id0].pos.z << ", vertices[id1].pos.z: " << mesh->vertices[id1].pos.z << std::endl;
		assert(0);
	}
	//DSTR << "constA: " << constA << ",  " << "constB: " << constB <<std::endl; 

	//debug
	//DSTR << "id0: " << id0 << ", id1: " << id1 <<std::endl;
	//DSTR << "vertices[id0].pos.z: " << vertices[id0].pos.z <<std::endl;
	//DSTR << "vertices[id0].pos.z: " << vertices[id1].pos.z <<std::endl;
	//DSTR << "dz: vertices[id0].pos.z - vertices[id1].pos.z : " << vertices[id0].pos.z - vertices[id1].pos.z << std::endl;
	//DSTR << "vertices[id0].pos.x: " << vertices[id0].pos.x << std::endl;
	//DSTR << "vertices[id1].pos.x: " << vertices[id1].pos.x << std::endl;
	//DSTR << "dx: vertices[id0].pos.x - vertices[id1].pos.x: " << vertices[id0].pos.x - vertices[id1].pos.x << std::endl;

	// for Check Debug
	//DSTR << "constA = dz / dx: " << constA << std::endl;
	//if(vertices[id0].pos.z == vertices[id1].pos.z && vertices[id0].pos.x == vertices[id1].pos.x){
	//	DSTR << "vertices[id0].pos.y: " << vertices[id0].pos.y << ", vertices[id1].pos.y: " << vertices[id1].pos.y << std::endl;
	//	if(vertices[id0].pos.y == vertices[id1].pos.y)
	//		DSTR << "id[" << id0 <<"], id[" << id1 << "] は同じ頂点 !" << std::endl;
	//}
	//DSTR << "constB = vertices[id0].pos.z - constA * vertices[id0].pos.x : " << vertices[id0].pos.z - constA * vertices[id0].pos.x << std::endl;
	//DSTR << "constA: " << constA << std::endl;
	//DSTR << "constB: " << constB << std::endl;
	//DSTR << std::endl;

	///	.交点の座標を計算
	// .１次関数の場合、x,z軸に平行な直線の場合がある
	if(vertexVars[id0].disFromOrigin <= radius && radius <= vertexVars[id1].disFromOrigin ){		/// 半径rの円と交わるべき
		double radius2 = radius * radius;
		constX1  = (- constA * constB + sqrt(radius2 * (constA * constA + 1.0) - constB * constB )  )  /  (constA * constA + 1);
		constX1_ = (- constA * constB - sqrt(radius2 * (constA * constA + 1.0) - constB * constB )  )  /  (constA * constA + 1);
		// 交点のx座標が線分の両端点のx座標間にあるとき
		if( (mesh->vertices[id0].pos.x <= constX1 && constX1 <= mesh->vertices[id1].pos.x) || (mesh->vertices[id1].pos.x <= constX1 && constX1 <= mesh->vertices[id0].pos.x) ){
			constZ1 = sqrt(radius2 - constX1  * constX1  );
			constZ1_ = - sqrt(radius2 - constX1  * constX1  );
			//	交点のz座標も両端点のz座標間にあるとき
			if( (mesh->vertices[id0].pos.z <= constZ1_ && constZ1_ <= mesh->vertices[id1].pos.z) || (mesh->vertices[id1].pos.z <= constZ1_ && constZ1_ <= mesh->vertices[id0].pos.z) ){
				constZ1 = constZ1_;
			}
		}else{
			constX1 = constX1_;		///		点のx座標はconstX_が正しい事がわかった。
			constZ1  =   sqrt(radius2 - constX1  * constX1  );
			constZ1_ = - sqrt(radius2 - constX1  * constX1  );
			//	交点のz座標も両端点のz座標間にあるとき
			if( (mesh->vertices[id0].pos.z <= constZ1_ && constZ1_ <= mesh->vertices[id1].pos.z) || (mesh->vertices[id1].pos.z <= constZ1_ && constZ1_ <= mesh->vertices[id0].pos.z) ){
				constZ1 = constZ1_;
			}
		}
	}else{
		DSTR << "CalcVtxCircleAndLine()関数のこの頂点組みと円弧は交わりません" << std::endl;
		constX1 = 0.0;
		constZ1 = 0.0;
		DSTR << "(id0, vertices[id0].disFromOrigin): (" << id0 << ", " << vertexVars[id0].disFromOrigin << "), (id1, vertices[id1].disFromOrigin): (" << id1 << ", " << vertexVars[id1].disFromOrigin << "), radius: " << radius << std::endl;  
		assert(0);
	}
	Vec2d intersection = Vec2d(constX1,constZ1);
	//":" <<  __TIME__ << 
	return intersection;
}		//	CalcVtxCircleAndLine() :difinition

void PHFemThermo::ShowIntersectionVtxDSTR(unsigned faceID,unsigned faceVtxNum,double radius){
	PHFemMeshNew* mesh = GetPHFemMesh();

	unsigned i = faceID;
	unsigned j = faceVtxNum;
	DSTR << "ascendVtx[" << j << "]: " << faceVars[i].ascendVtx[j] << ", " << "[ " << (j+1)%3 << "]: " << faceVars[i].ascendVtx[(j+1)%3] << "; ";
	DSTR << " (vertices[" << faceVars[i].ascendVtx[j] << "].pos.x, .z) = ( " <<  mesh->vertices[faceVars[i].ascendVtx[j]].pos.x << ", "<<  mesh->vertices[faceVars[i].ascendVtx[j]].pos.z  << "), " ;
	DSTR << " (vertices[" << faceVars[i].ascendVtx[(j+1)%3] << "].pos.x, .z) : ( " <<  mesh->vertices[faceVars[i].ascendVtx[(j+1)%3]].pos.x << ", "<<  mesh->vertices[faceVars[i].ascendVtx[(j+1)%3]].pos.z << "), " <<std::endl;
	DSTR <<"face[i].[(" << j << "], [" << (j+1)%3 << "]：各々の原点からの距離" << vertexVars[faceVars[i].ascendVtx[j]].disFromOrigin << ", " << vertexVars[faceVars[i].ascendVtx[(j+1)%3]].disFromOrigin << ", "; 
	DSTR << " radius: " << radius <<" と2点で構成される線分との交点は下記"<< std::endl;
	DSTR << __FILE__  << "(" <<  __LINE__ << "):"<< "Intersection Vtx (x,z)= " << CalcIntersectionOfCircleAndLine( faceVars[i].ascendVtx[ j ] , faceVars[i].ascendVtx[ (j+1)%3 ] , radius) << std::endl;
	DSTR << std::endl;
}

//void PHFemThermo::CalcIHarea(double radius,double Radius,double dqdtAll){
//	//	face構造体メンバihareaを計算
//	//、メッシュ全体のihareaの合計を計算
//	//	...dqdtRatio(熱流束率)を総面積に対するiharea面積比から弱火の時に、faceが受け取るべきdqdtを計算して返す
//
//	//	形状関数の計算は、iharea を用いて、CalcVecf2surface()等で行う,face.shapefunkで行う		→クラス化しようか？
//		//	.... 形状関数を格納する		// ここで分かる形状関数は、頂点間の距離から分かる線形補間係数　すなわち、割合　０＜＝～＜＝１で良くて、最後に、行列に入れる前に、割合以外を入れればいいのかな？
//
//	// radius value check
//	if(Radius <= radius){
//		DSTR << "inner radius size is larger than outer Radius " << std::endl;
//		DSTR << "check and set another value" << std::endl;
//		assert(0);
//	}
//	///	内半径と外半径の間の節点に熱流束境界条件を設定
//	//> 四面体面の三角形と円環領域の重複部分の形状・面積を求める当たり判定を計算する。
//	//>	切り取り形状に応じた形状関数を求め、熱流束ベクトルの成分に代入し、計算する
//
//	//> 1.円環領域と重なるface三角形の形状を算出する。領域に含まれる頂点、face三角形の辺との交点を求めてvecteorに格納する
//	//>	2.vectorには、辺0,1,2の順に領域内の頂点や交点が入っているが、これを元に三角形分割を行う。三角形分割ができたら、各三角形を求める。三角形の総和を、このfaceの加熱領域とする。
//	//>	3.vectorの点における形状関数を求めて、擬似体積（重なっている面積×形状関数の値）を使って、四面体内の各点における形状関数の面積分を求める。求めた値は、熱流束ベクトルの成分として要素剛性行列の成分に代入する。
//	//>	4.毎ステップ、同じ熱流束の値をベクトル成分に加える
//	
//	/// debug
//	//unsigned numIHheated0 = 0; 
//	//for(unsigned i=0; i < nSurfaceFace;i++){
//	//	if(faces[i].mayIHheated){	
//	//		DSTR << i << " ; "  << std::endl;
//	//		numIHheated0 +=1;
//	//	}
//	//}
//	//DSTR << "numIHheated0 / nSurfaceFace: " << numIHheated0 << " / " << nSurfaceFace << std::endl;	////	761 / 980	ってほとんどじゃないか！半分位にならないとおかしいはずだが・・・　ローカルy座標値がマイナスのものを選んでいるので
//
//	//	debug	mayIHheatedの確度を上げる前の数を知りたい
//	unsigned numIHheated0 = 0; 
//	for(unsigned i=0; i < nSurfaceFace;i++){
//		if(faces[i].mayIHheated){	
//			//DSTR << i << " ; "  << std::endl;
//			numIHheated0 +=1;
//		}
//	}
//	DSTR << "numIHheated0 / nSurfaceFace: " << numIHheated0 << " / " << nSurfaceFace << std::endl;
//
//	//	face頂点のどれか1つが、円環領域に入っているfaceだけ、trueに、それ以外は、falseに
//	//> raius,RadiusについてmayIHheatedの確度を上げてから、円環領域と重なっている形状を求める
//	for(unsigned i=0;i < nSurfaceFace; i++){
//		if(faces[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点 円環の範囲内に入っているとは限らない
//			for(unsigned j=0;j<3;j++){
//				/// 円環領域内にface頂点が含まれる
//				if(radius <= vertices[faces[i].vertices[j]].disFromOrigin && vertices[faces[i].vertices[j]].disFromOrigin <= Radius){
//					faces[i].mayIHheated = true;
//					break;		//>	見つかったら、判定はtrueのままで良い。最内側のforを抜ける
//				}
//				else{
//					faces[i].mayIHheated = false;
//				}
//				//> （円環領域には含まれず）円環領域より内側と外側にfaceの辺の頂点がある	vertices[j%3] と vertices[(j+1)%3]　で作る辺があるとき
//				if(vertices[faces[i].vertices[j]].disFromOrigin < radius && Radius < vertices[faces[i].vertices[(j+1)%3]].disFromOrigin 
//					|| vertices[faces[i].vertices[(j+1)%3]].disFromOrigin < radius && Radius < vertices[faces[i].vertices[j]].disFromOrigin){
//						faces[i].mayIHheated = true;
//						break;		//>	同上
//				}else{
//					faces[i].mayIHheated = false;
//				}
//				//>	円環領域内にface辺のどちらかの頂点が含まれるとき(r<P1<R<P2,P1<r<P2<R,(とPa1,P2を入れ替えたもの))
//				if(radius <= vertices[faces[i].vertices[j]].disFromOrigin && vertices[faces[i].vertices[j]].disFromOrigin < Radius && Radius < vertices[faces[i].vertices[(j+1)%3]].disFromOrigin
//					|| radius <= vertices[faces[i].vertices[(j+1)%3]].disFromOrigin && vertices[faces[i].vertices[(j+1)%3]].disFromOrigin < Radius && Radius < vertices[faces[i].vertices[j]].disFromOrigin
//					|| vertices[faces[i].vertices[j]].disFromOrigin <= radius && radius < vertices[faces[i].vertices[(j+1)%3]].disFromOrigin && vertices[faces[i].vertices[(j+1)%3]].disFromOrigin < Radius
//					|| vertices[faces[i].vertices[(j+1)%3]].disFromOrigin <= radius && radius < vertices[faces[i].vertices[j]].disFromOrigin && vertices[faces[i].vertices[j]].disFromOrigin < Radius){
//						faces[i].mayIHheated = true;
//						break;		//>	同上
//				}else{
//					faces[i].mayIHheated = false;
//				}
//			}		//	for
//		}		//	if
//	}		//	for
//	//> debug
//	//>	mayIHheatedのフラグが立っているfaceにその面積の形状関数を与えてみる。	重なる面積をきちんと計算と、少しでも引っかかっていれば、加熱面に入れてしまう計算、試す
//	//> CalcIHdqdt3 or 4
//
//	/// debug
//	unsigned numIHheated = 0; 
//	for(unsigned i=0; i < nSurfaceFace;i++){
//		if(faces[i].mayIHheated){	
//			//DSTR << i << " ; "  << std::endl;
//			numIHheated +=1;
//		}
//	}
//	DSTR << "numIHheated / nSurfaceFace: " << numIHheated << " / " << nSurfaceFace << std::endl;		///:	表面faceの内、加熱節点を含むfaceの数、鉄板:264/980　こんなもんかな 
//	
//	//	.. 交点を求め、faces構造体のvectorに領域内の頂点や交点を格納
//	for(unsigned i=0;i < nSurfaceFace; i++){
//		if(faces[i].mayIHheated){		//	may → 「確実」に変化済みのフラグ
//			//	area:face面積を計算されてなければ、計算
//			if(faces[i].area==0) faces[i].area = CalcTriangleArea(faces[i].vertices[0],faces[i].vertices[1],faces[i].vertices[2]);
//			////	face内の頂点のdisFromOriginの値でソート
//			//unsigned nearestvtxnum		=	0;				///	原点に一番近い頂点のface頂点番号(0~2)
//			//for(unsigned j=0;j<3;j++){
//			//	double hikaku = DBL_MAX;
//			//	if(hikaku > vertices[faces[i].vertices[j]].disFromOrigin){	hikaku = vertices[faces[i].vertices[j]].disFromOrigin;	nearestvtxnum = j;}
//			//}
//			
//			// ... 3点を原点に近い順に並べる		vtxOrder:近い順に格納,番目が近い順番、格納値が頂点番号 ==	faces[i].ascendVtx[3]
//			ArrangeFacevtxdisAscendingOrder(i);		///	ArrangeVtxdisAscendingOrder(int faceID,int vtx0,int vtx1,int vtx2)
//			//DSTR <<  "小さい順か確認: " << vertices[vtxOrder[0]].disFromOrigin << ", "<< vertices[vtxOrder[1]].disFromOrigin << ", "<< vertices[vtxOrder[2]].disFromOrigin << std::endl;
//			
//			// ... face内の各頂点が属している領域を判定 0 | 1 | 2	///	faces[i].ascendVtx[0~2]に該当する頂点が　円環領域の前後のどこに存在しているか
//			// ... vtxdiv[0~2]に近い順に並んだ頂点の領域ID(0~2)を割り振り ＝ faces[i].ascendVtx[j] の順と対応
//			unsigned vtxdiv[3];		//	原点から近い順:0~2に並べ替えられた頂点IDに対応する領域内外の区分け　配列
//			//DSTR << "faces[i].ascendVtx[0~2]:" ;
//			for(unsigned j=0;j<3;j++){
//				if( vertices[faces[i].ascendVtx[j]].disFromOrigin < radius){			vtxdiv[j] = 0;
//				/// 円弧上を含み、円弧上も円環領域内と定義する
//				}else if(radius <= vertices[faces[i].ascendVtx[j]].disFromOrigin && vertices[faces[i].ascendVtx[j]].disFromOrigin <= Radius ){	vtxdiv[j] = 1;
//				}else if(Radius < vertices[faces[i].ascendVtx[j]].disFromOrigin){		vtxdiv[j] = 2;	}
//				//DSTR << faces[i].ascendVtx[j] ;
//				//if(j <  2) DSTR << ", " ;
//			}
//			//DSTR << std::endl;
//
//			//> debug
//			//DSTR << "頂点の領域番号: " ;
//			//for(unsigned j =0;j<3;j++){
//			//	DSTR << vtxexistarea[j];
//			//	if(j<2) DSTR << ", ";
//			//}
//			//DSTR << std::endl;
//			//int vtxexistareadebug =0;
//			//... 2012.2.14ここまで...
//
//			//...	配列の成分の値の変化を見て、始点、交点、辺対となる点を順にvectorに格納していく
//			
//			//	vectorに入れる際の注意!!! %%%%%%%%%%%%%%%%%%
//			// %%%	vtxOrder[ 原点から近い頂点の順(0,1,2) ]:その頂点IDの原点に近い順に並べ替えてIDを格納		(例:vtxOrder[0] = (ID)278, [1] = (ID)35, [2] = (ID)76 etc)
//			// %%%	vtxdiv[ 原点から近い頂点の順(0,1,2) ]:その頂点が円環領域の内側（1）か外側(0,2)かを表す
//
//			//>	faceの辺ごとに場合分け
//			///	 j と(隣の) (j+1)%3 とで対を成す辺について
//			for(unsigned j=0;j<3;j++){
//				double f[3]={0.0, 0.0, 0.0};	// 頂点0,1,2,3から見た形状関数 
//				//debug
//				//DSTR <<"j: " << j << ", faces[i].ascendVtx[j]: " << faces[i].ascendVtx[j] << ", faces[i].ascendVtx[(j+1)%3]: " << faces[i].ascendVtx[(j+1)%3] << std::endl;
//				//DSTR << "vertices[faces[i].ascendVtx[j]].pos: (" << vertices[faces[i].ascendVtx[j]].pos.x  << ", "<< vertices[faces[i].ascendVtx[j]].pos.z << ") " << std::endl;
//				//DSTR << "vertices[faces[i].ascendVtx[(j+1)%3]].pos: (" << vertices[faces[i].ascendVtx[(j+1)%3]].pos.x  << ", "<< vertices[faces[i].ascendVtx[(j+1)%3]].pos.z << ") " << std::endl; 
//				//DSTR << std::endl;
//
//				//	0の領域にある辺:
//				if(vtxdiv[j] == 0 && vtxdiv[(j+1)%3] == 0){
//					//	いずれの点をも領域内vectorには入れない
//				}
//				//	内半径とだけ交わる辺(内:0→外:1と外:1→内:0):
//				else if(vtxdiv[j] == 0 && vtxdiv[(j+1)%3] - vtxdiv[j] > 0 || vtxdiv[j] == 1 && vtxdiv[(j+1)%3] - vtxdiv[j] < 0){
//					//	(始点（↑で入れている場合には不要））と内半径とを、対の点に入れる
//					if(vtxdiv[(j+1)%3] - vtxdiv[j] > 0){	//内→外
//						//	内半径との交点を求めて、座標を入れる
//						//%%%	線分を構成する頂点と半径、交点のチェック関数→DSTR表示	%%%//		//faceID,face内節点番号、半径を用いて、隣り合う節点で作る線分と円弧の交点を求めて表示
//						ShowIntersectionVtxDSTR(i,j,radius);
//						//	..内半径との交点のx,z座標を入れる
//						faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , radius) );
//						//	..組対点の座標をintersectionに入れる
//						faces[i].ihvtx.push_back( Vec2d( vertices[ faces[i].ascendVtx[ (j+1)%3 ] ].pos.x, vertices[faces[i].ascendVtx[ (j+1)%3 ] ].pos.z) );
//						//	....この点位置での形状関数を導出
//					}else if(vtxdiv[(j+1)%3] - vtxdiv[j] < 0){		//外→内
//						//	内半径との交点を求め、交点の座標を入れる
//						faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , radius) );
//						//..組対点は、内半径の内側：円環領域外なので、組対点をvectorには入れない
//					}
//				}
//				//	円環領域(:1)内にある辺
//				else if(vtxdiv[j] == 1 && vtxdiv[(j+1)%3] == 1){
//					//	(始点を入れているのなら、)辺対点をvectorに入れる
//					//	始点は入れずとも、最後に入るはず
//					////intersection.push_back(Vec2d(vertices[vtxdiv[(j+1)%3]].pos.x,vertices[vtxdiv[(j+1)%3]].pos.z));
//					faces[i].ihvtx.push_back(Vec2d(vertices[faces[i].ascendVtx[(j+1)%3]].pos.x,	vertices[faces[i].ascendVtx[(j+1)%3]].pos.z));
//				}
//				//	外半径と交わる辺(内→外、外→内)
//				else if(vtxdiv[j] == 1 && vtxdiv[(j+1)%3] == 2 || vtxdiv[j] == 2 && vtxdiv[(j+1)%3] == 1){
//					// 内向きか外向きかを、符号で判定することで、上の	or	のどちらかを判定し、vectorに入れる順番を変える
//					//	外半径との交点を求める
//					//	内→外 2 - 1 = 1 > 0
//					if(vtxdiv[(j+1)%3] - vtxdiv[j] > 0){
//						//	交点を格納
//						faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , Radius) );
//						////	組対点のX,Z座標を格納→格納しちゃダメでしょ！
//						//faces[i].ihvtx.push_back(Vec2d(vertices[faces[i].ascendVtx[(j+1)%3]].pos.x,	vertices[faces[i].ascendVtx[(j+1)%3]].pos.z));
//						////	?→	intersection.push_back(Vec2d(vertices[vtxOrder[(j+1)%3]].pos.x,vertices[vtxOrder[(j+1)%3]].pos.z));
//						
//					}
//					//	外→内 1 - 2 = -1 < 0
//					else if(vtxdiv[(j+1)%3] - vtxdiv[j] < 0){
//						// 外半径との交点の座標を格納する
//						faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , Radius) );
//						// 組対点を格納する
//						faces[i].ihvtx.push_back( Vec2d( vertices[ faces[i].ascendVtx[ (j+1)%3 ] ].pos.x, vertices[faces[i].ascendVtx[ (j+1)%3 ] ].pos.z) );
//					}
//					else if(vtxdiv[(j+1)%3] - vtxdiv[j] == 0) assert(0);
//					//	(始点がvectorに入っていることを確認する)交点をvectorに入れる
//					faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , Radius) );
//				}
//				//	内半径と外半径とのどちらとも交わる辺(内→外、外→内)
//				else if(vtxdiv[j] == 0 && vtxdiv[(j+1)%3] == 2 || vtxdiv[j] == 2 && vtxdiv[(j+1)%3] == 0){
//					// 内向きか外向きかを、符号で判定することで、上の	or	のどちらかを判定し、vectorに入れる順番を変える
//					//%%%	どちらとも交わる条件
//					// 内半径円弧より原点より→外：0→２: 2 - 0 > 0
//					if(vtxdiv[(j+1)%3] - vtxdiv[j] > 0){
//						//内半径と交わり、交点を格納
//						faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , radius) );
//						//外板系と交わり、交点を格納
//						faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , Radius) );
//					}else{	// 外→内半径円弧より原点より：2→０
//						//外半径と交わり、交点を格納
//						faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , Radius) );
//						//内板系と交わり、交点を格納
//						faces[i].ihvtx.push_back( CalcIntersectionOfCircleAndLine( faces[i].ascendVtx[ j ] , faces[i].ascendVtx[ (j+1)%3 ] , radius) );
//					}
//				}
//				//	内半径と外半径と交わる辺(外側から内側へ)
//				//else if(){
//				//}
//					//	if文の中で、差分がプラスかマイナスで判定できそう
//				
//				//	外半径の外側にある辺
//				else if(vtxdiv[j] == 2 && vtxdiv[(j+1)%3] == 2){
//					//	いずれの点をも領域内vectorには入れない
//				}
//				//	内側から外側に行く辺は上で記述できるが、外側から内側に向かう辺をこれで記述できるのか？
//			}	//	for(unsigned j=0;j<3;j++){
//
//			//	..vectorを三角形分割する
//			//	..分割した三角形の面積を各々求める
//			// ..IH面積に追加
//
//			//faces[i].iharea = 
//
//			//	デバッグ項目：faceのihvtx(vector)に、mayihheatedでフラグの立ったface又は節点が全て含まれているか。
//		}	//	if(mayIHheated)
//	}	//	for(faces[i]
//
//
//	//>	Step	1.の実装
//	//>	radius:内半径、Radius:外半径,　dqdtAll:単位時間あたりに入ってくる全熱量
//
//	//%%	手順 
//	//> isHeated(->nearIHHeater)のfacesの内、3つの節点全部について、特定半径の中に入っているものがあるかどうかをチェックする。
//	//>	入っているものを、見つけたら、面積計算をしたいが、ここで、何個の節点が入っているかによって場合分けを行う。
//	//>	３つ:faceの面積をfaceSに代入する	1~2つ:面積を近似計算するために、3節点の内、どの節点と節点で構成された辺が内半径rまたは、外半径Rと交わっているか判定
//	//>	交わっている場合には、その交点を求める。求めた交点と内半径以上外半径以内の領域にあるface内の節点を用いて、三角形を作り、その面積を求める。
//}


//
//void PHFemThermo::CalcIHdqdt2(double r,double R,double dqdtAll,unsigned num){
//	///	内半径と外半径の間の節点に熱流束境界条件を設定
//	//> 円環で区切られる四面体面の領域を三角形で近似する
//
//	//> 加熱する四面体面の面積の総和を求める
//
//	//%%	手順 
//	//> isHeated(->nearIHHeater)のfacesの内、3つの節点全部について、特定半径の中に入っているものがあるかどうかをチェックする。
//	//>	入っているものを、見つけたら、面積計算をしたいが、ここで、何個の節点が入っているかによって場合分けを行う。
//	//>	３つ:faceの面積をfaceSに代入する	1~2つ:面積を近似計算するために、3節点の内、どの節点と節点で構成された辺が内半径rまたは、外半径Rと交わっているか判定
//	//>	交わっている場合には、その交点を求める。求めた交点と内半径以上外半径以内の領域にあるface内の節点を用いて、三角形を作り、その面積を求める。
//
//	double faceS = 0.0;
//	for(unsigned i=0;i < nSurfaceFace; i++){
//		if(faces[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点 円環の範囲内に入っているとは限らない
//			unsigned nObinnerVtx = 0;
//			unsigned inner[3] = {0,0,0};
//			unsigned judge[2] = {0,0};		///	judge[0],[1]の順に原点に近い点の判定結果
//			if(faces[i].area==0) faces[i].area = CalcTriangleArea(faces[i].vertices[0],faces[i].vertices[1],faces[i].vertices[2]);		// 面積計算が済んでなければ計算する
//				//> 置き換えと0,1を入れ替えるだけ(=ポリモーフィズム)で残りの実装も作る
//				// 下記処理を関数化する?。点のIDを入れれば、同じ処理をする関数
//				//> 0-1で交点を作っているとき true false は論演算の排他的論理和XORでtrue時、交点を計算する 00=0,01=10=1,11=0; 片方の点が範囲内で、もう一方が範囲外の場合、trueになる
//			
//				//> 円環の範囲内に入っていたら、mayIHheatedをtrueに、あるいは、falseにする(一応)
//
//				/// faces[i]の頂点が何個、領域内に入っているかを見つける
//				for(unsigned j=0;j<3;j++){
//					if( r <= vertices[faces[i].vertices[j]].disFromOrigin && vertices[faces[i].vertices[j]].disFromOrigin <= R){
//						nObinnerVtx += 1;
//						inner[j] = 1;
//					}
//				}
//
//				//> nObinnerVtxの値で条件分岐
//			
//				///	
//				if(nObinnerVtx == 1){
//					Vec2d vtxXY[3];			/// faceの辺と半径r,Rとの交点のx,z座標　最大３点
//					for(unsigned j=0;j<3;j++){		///	faceを構成する3頂点について
//						unsigned k = 0;
//						unsigned m = 0;
//						k = (j+1)%3;		///	j=0;k=1, j=1;k=2, j=2;k=0 
//						m = (j+2)%3;
//						//vtxXY[j] = CalcIntersectionPoint(faces[i].vertices[j],faces[i].vertices[k],r,R);
//						//DSTR << "vtxXY[" << j << "]: " << vtxXY[j] << std::endl; 
//						if(inner[j] ==1){			/// faces[i]のj番目の節点が円環領域内に入っている
//						/// j番目の頂点とエッジを構成する点(他の２点)を使って、半径r,Rと交わる点を算出する						
//						//> j 0,1,2
//							if(j == 0){	/// 頂点jと辺を作る頂点を使って、辺と交わる交点を求める
//								//k = (j+1)%3;		///	j=0;k=1, j=1;k=2, j=2;k=0 
//							vtxXY[j] = CalcIntersectionPoint(faces[i].vertices[j],faces[i].vertices[k],r,R);
//							vtxXY[k] = CalcIntersectionPoint(faces[i].vertices[j],faces[i].vertices[m],r,R);
//							}
//							else if(j == 1){
//							vtxXY[j] = CalcIntersectionPoint(faces[i].vertices[j],faces[0].vertices[1],r,R);
//							vtxXY[j] = CalcIntersectionPoint(faces[i].vertices[j],faces[2].vertices[2],r,R);
//							}
//							else if(j == 2){
//							vtxXY[j] = CalcIntersectionPoint(faces[i].vertices[j],faces[0].vertices[1],r,R);
//							vtxXY[j] = CalcIntersectionPoint(faces[i].vertices[j],faces[1].vertices[2],r,R);
//							}
//							else{
//								assert(0);
//								DSTR << "Error in PHFemThermo::CalcIHdqdt2(hogehoge) 領域判定にミス" << std::endl;
//							}
//						 }
//					}
//				}
//
//				//for(unsigned j=0;j<3;j++){
//				//	/// face内の節点を順番にチェックする
//				//	unsigned k =0;
//				//	k = (j+1)%3;		///	j=0;k=1, j=1;k=2, j=2;k=0 
//				//	if( r <= vertices[faces[i].vertices[j]].disFromOrigin && vertices[faces[i].vertices[j]].disFromOrigin <= R){
//				//	}
//				//	if( r <= vertices[faces[i].vertices[j]].disFromOrigin && vertices[faces[i].vertices[j]].disFromOrigin <= R){
//				//		//> j,kをfaces[i].vertices[j],faces[i].vertices[k]として代入
//				//		CalcIntersectionPoint(faces[i].vertices[0],faces[i].vertices[1],r,R);
//				//	}
//				//}
//
//				//> r -> radius
//				//> R -> Radius
//			
//				/// 座標(consX ,constY)が円と三角形の辺との交点
//			
//				//unsigned vtxId0 = faces[i].vertices[0];
//				//unsigned vtxId1 = faces[i].vertices[1];
//			}
//			////> 1-2で交点を作っているとき	//
//			//else if( (r <= vertices[faces[i].vertices[1]].disFromOrigin && vertices[faces[i].vertices[1]].disFromOrigin <= R) ^ (vertices[faces[i].vertices[2]].disFromOrigin && vertices[faces[i].vertices[2]].disFromOrigin <= R)){
//			//	//> 円環との交点を求める
//			//	int katoon00 =0;
//			//}
//			////> 0-2で交点を作っているとき
//			//else if( (r <= vertices[faces[i].vertices[0]].disFromOrigin && vertices[faces[i].vertices[0]].disFromOrigin <= R) ^ (vertices[faces[i].vertices[2]].disFromOrigin && vertices[faces[i].vertices[2]].disFromOrigin <= R)){
//			//	//> 円環との交点を求める
//			//	int katoon000 =0;
//			//}
//
//			//for(unsigned j=0;j<3;j++){
//			//	if( (r <= vertices[faces[i].vertices[j]].disFromOrigin && vertices[faces[i].vertices[j]].disFromOrigin <= R) * (r <= vertices[faces[i].vertices[j+1]].disFromOrigin && vertices[faces[i].vertices[j+1]].disFromOrigin <= R)){
//			//		if(r <= vertices[faces[i].vertices[j]].disFromOrigin && vertices[faces[i].vertices[j]].disFromOrigin <= R)
//			//		nObinnerVtx += 1;
//			//	}
//			//}
//			//if( nObinnerVtx == 1)			faces[i].fluxarea = 1.5/3.0 * faces[i].area;//faces[i].fluxarea = 1.0/3.0 * faces[i].area;
//			//else if(nObinnerVtx == 2)		faces[i].fluxarea = 2.8/3.0 * faces[i].area;//faces[i].fluxarea = 2.0/3.0 * faces[i].area;
//			//else if(nObinnerVtx == 3)		faces[i].fluxarea = faces[i].area;
//			//else if(nObinnerVtx == 0)		faces[i].fluxarea = 0;
//
//			//if(faces[i].fluxarea >= 0){	
//			//	faceS += faces[i].fluxarea;
//			//}else{	assert(0);	}		//	faces[i].fluxareaに0未満の数字が入っているのに加算しようとしている
//			//DSTR << "faces[" << i << "].fluxarea: " << faces[i].fluxarea << std::endl;
//		}
//
//	//for(unsigned i=0;i < nSurfaceFace; i++){
//	//	DSTR << "faces[" << i << "].fluxarea: " << faces[i].fluxarea << std::endl;
//	//}
//
//	if(faceS > 0){
//		//> dqdt を単位面積あたりに直す([1/m^2])
//		double dqdtds = dqdtAll / faceS;
////		DSTR << "dqdtds:  " << dqdtds << std::endl;
//		//>	以下、熱流束をfacesに格納する
//		//>	熱流束の面積計算はfluxareaを用いて行う
//		for(unsigned i=0;i < nSurfaceFace; i++){
//			if(faces[i].mayIHheated){
//				faces[i].heatflux[num] = dqdtds * faces[i].fluxarea;		//	熱流束の量をheatfluxの面積から計算
////				DSTR << "faces[" << i <<"].heatflux: " << faces[i].heatflux <<std::endl;
//			}
//		}
//	}
//	//　以上、値は入っているようだ
//	int katoon =0;
//	//↑をつかって、CreateMatk2tをコピーした関数で、Vecf2?を作る基に
//
//	//>	熱量は、dqdtdsを用いる
//
//	//> r <= <= Rの中心から放射状に加熱
//
//	//	節点でdqdtの値を更新する
//
//	//　以下は、ベクトルを作る関数の仕事
//	//	節点の属する表面の面で、計算する
//	//  vertices[].heatFluxValueを基に計算を進める
//	//	ガウスザイデル計算できるように処理など、準備する
//}

Vec2d PHFemThermo::GetIHbandDrawVtx(){	return IHLineVtxX;	}

void PHFemThermo::SetIHbandDrawVtx(double xS, double xE){
	IHLineVtxX = Vec2d(xS,xE);
}

void PHFemThermo::CalcIHdqdtband_(double xS,double xE,double dqdtAll,unsigned num){
	PHFemMeshNew* mesh = GetPHFemMesh();

	///	x座標：xS~xEの間の節点に熱流束境界条件を設定
	// xS,ｘEの間にいずれか一点がある、
	// 
	// mayIHheatedは使わない	:この条件内で、faceの全節点のy座標が負のものについてのみ、facesSに加算

	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;			// initialize
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		//if(faces[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			unsigned nObinnerVtx = 0;
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			//..全節点でy<0なら、fluxareaに加算
			if(mesh->vertices[mesh->faces[i].vertexIDs[0]].pos.y < 0 && mesh->vertices[mesh->faces[i].vertexIDs[1]].pos.y < 0 && mesh->vertices[mesh->faces[i].vertexIDs[2]].pos.y < 0){
				for(unsigned j=0;j<3;j++){
					// ||.. :x, |.|.:o , .|.|:o ,   .||.:o  , ..||:x 
					//. どれか１点がxS~xEの間にある、又は、隣り合う2点がバンドの外側にある、IHareaに算入
					if( xS <= mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x && mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x <= xE){
						faceVars[i].fluxarea[0] = faceVars[i].area;
						break;
					}
					//.頂点対がバンドを挟むとき
					if(mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x <= xS && xE <= mesh->vertices[mesh->faces[i].vertexIDs[(j+1)%3]].pos.x 
						|| mesh->vertices[mesh->faces[i].vertexIDs[(j+1)%3]].pos.x <= xS && xE <= mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x ){
							faceVars[i].fluxarea[0] = faceVars[i].area;
						break;
					}
				}
			}
			if(faceVars[i].fluxarea >= 0){	
				faceS += faceVars[i].fluxarea[0];
			}else{		assert(0);	}		//	faces[i].fluxareaに0未満の数字が入っているのに加算しようとしている
			//DSTR << "faces[" << i << "].fluxarea: " << faces[i].fluxarea << std::endl;
		//}
	}

	//..face面積に応じた熱流束を全体の面積割合より求める
	if(faceS > 0){
		//> dqdt を単位面積あたりに直す([1/m^2])
		//double dqdt_ds = dqdtAll / faceS;
//		DSTR << "dqdtds:  " << dqdtds << std::endl;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].fluxarea){
				faceVars[i].heatflux[num][0] = faceVars[i].fluxarea[0] / faceS * dqdtAll;		//	熱流束の量をheatfluxの面積から計算
//				DSTR << "faces[" << i <<"].heatflux: " << faces[i].heatflux <<std::endl;			
				// debug
				//for(unsigned j = 0; j < 3;++j){
 				//	vertices[faces[i].vertices[j]].temp = 100.0;
				//}
			}
		}
	}

	//	デバッグ・表示用
	SetIHbandDrawVtx(xS,xE);
}

void PHFemThermo::CalcIHdqdtband(double xS,double xE,double dqdtAll,unsigned num){
	PHFemMeshNew* mesh = GetPHFemMesh();

	///	x座標：xS~xEの間の節点に熱流束境界条件を設定

	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			unsigned nObinnerVtx = 0;
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			for(unsigned j=0;j<3;j++){
				if( xS <= mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x && mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x <= xE){
					nObinnerVtx += 1;
				}
			}
			if( nObinnerVtx == 1)			faceVars[i].fluxarea[0] = faceVars[i].area;//faces[i].fluxarea = 1.0/3.0 * faces[i].area;
			else if(nObinnerVtx == 2)		faceVars[i].fluxarea[0] = faceVars[i].area;//faces[i].fluxarea = 2.0/3.0 * faces[i].area;
			else if(nObinnerVtx == 3)		faceVars[i].fluxarea[0] = faceVars[i].area;
			else if(nObinnerVtx == 0)		faceVars[i].fluxarea[0] = 0.0;

			if(faceVars[i].fluxarea >= 0){	
				faceS += faceVars[i].fluxarea[0];
			}else{		assert(0);	}		//	faces[i].fluxareaに0未満の数字が入っているのに加算しようとしている
			//DSTR << "faces[" << i << "].fluxarea: " << faces[i].fluxarea << std::endl;
		}
	}

	//for(unsigned i=0;i < nSurfaceFace; i++){
	//	DSTR << "faces[" << i << "].fluxarea: " << faces[i].fluxarea << std::endl;
	//}



	if(faceS > 0){
		//> dqdt を単位面積あたりに直す([1/m^2])
		double dqdtds = dqdtAll / faceS;
//		DSTR << "dqdtds:  " << dqdtds << std::endl;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].mayIHheated){
				faceVars[i].heatflux[num][0] = dqdtds * faceVars[i].fluxarea[0];		//	熱流束の量をheatfluxの面積から計算
//				DSTR << "faces[" << i <<"].heatflux: " << faces[i].heatflux <<std::endl;
			}
		}
	}
	
	//　以上、値は入っているようだ
	int katoon =0;
}

void PHFemThermo::CalcIHdqdt_decrease_high(double r,double R,double dqdtAll,unsigned mode){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//dqdtAllを単位面積辺り位に直す
	double dqdtdsAll =0.0;

	///	内半径と外半径の間の節点に熱流束境界条件を設定
	//	少しでも領域にかかっていれば、IH加熱に含める
	int cnt = 0;
	//	初期化
	for(unsigned i=0;i<mesh->faces.size();i++){
		faceVars[i].fluxarea[2] = 0.0;
	}
	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			for(unsigned j=0;j<3;j++){
				if( r <= vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin && vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin <= R){
					faceVars[i].fluxarea[2] = faceVars[i].area;
					break;
				}
			}
			faceS += faceVars[i].fluxarea[2];
			cnt+=1;
		}
	}
	double debugS=0.0;
	double debugdq=0.0;
	if(faceS > 0){
		//dqdtdsAll = dqdtAll / faceS;
		//> dqdt を単位面積あたりに直す([1/m^2])
		double dqdtds = dqdtAll / faceS;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].mayIHheated){
				double avgTemp=0.0;
				double ohm = 0.0;
				avgTemp = 1.0 / 3.0 * ( vertexVars[mesh->faces[i].vertexIDs[0]].temp + vertexVars[mesh->faces[i].vertexIDs[1]].temp + vertexVars[mesh->faces[i].vertexIDs[2]].temp);
				if(avgTemp >= 25.0 && avgTemp < 102.0){
					ohm = (avgTemp - 25) * (9.92e-10) + 5.86e-7;
				}
				else if(102.0 <= avgTemp && avgTemp < 202.0){
					ohm = (avgTemp -102.0) * 9.2e-10 + 6.87e-7;
				}
				else if(202.0<= avgTemp && avgTemp < 302.0){
					ohm = (avgTemp -202.0) * 8.3e-10 + 7.79e-7;
				}
				else if(302.0 <= avgTemp && avgTemp <402.0){
					ohm = (avgTemp -302.0) * 7.5e-10 + 8.62e-7;
				}
				if(avgTemp > 25.0){
					faceVars[i].heatflux[mode][0] = dqdtds * (5.86e-7)/(ohm);		//	熱流束の量をheatfluxの面積から計算し、J/m^2に直さなければいけない→行列で計算するために。
				}else{
					faceVars[i].heatflux[mode][0] = dqdtds;
				}
				//debug
				debugdq += dqdtds * faceVars[i].fluxarea[2];
				debugS += faceVars[i].fluxarea[2];
			}
		} 
	}
	if(debugS != faceS){ DSTR << "diff between debugS:" << debugS << ", faceS:" << faceS << std::endl; }
	if( debugdq <= dqdtAll - 1e-8 &&  dqdtAll + 1e-8 <= debugdq){	DSTR << "面積が大体同じではない" <<std::endl;} 	//大体同じではないときに、警告			大体同じときの条件判定　dqdtAll - 1e-8 <= debugdq && debugdq <= dqdtAll + 1e-8: 大体同じ 
}		// /*CalcIHdqdt_decrease*/

void PHFemThermo::CalcIHdqdt_decrease(double r,double R,double dqdtAll,unsigned mode){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//dqdtAllを単位面積辺り位に直す
	double dqdtdsAll =0.0;

	///	内半径と外半径の間の節点に熱流束境界条件を設定
	//	少しでも領域にかかっていれば、IH加熱に含める
	int cnt = 0;
	//	初期化
	for(unsigned i=0;i<mesh->faces.size();i++){
		faceVars[i].fluxarea[2] = 0.0;
	}
	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			for(unsigned j=0;j<3;j++){
				if( r <= vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin && vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin <= R){
					faceVars[i].fluxarea[2] = faceVars[i].area;
					break;
				}
			}
			faceS += faceVars[i].fluxarea[2];
			cnt+=1;
		}
	}
	double debugS=0.0;
	double debugdq=0.0;
	if(faceS > 0){
		//dqdtdsAll = dqdtAll / faceS;
		//> dqdt を単位面積あたりに直す([1/m^2])
		double dqdtds = dqdtAll / faceS;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].mayIHheated){
				faceVars[i].heatflux[mode][2] = -dqdtds;		//	熱流束の量をheatfluxの面積から計算し、J/m^2に直さなければいけない→行列で計算するために。
				//debug
				debugdq += dqdtds * faceVars[i].fluxarea[2];
				debugS += faceVars[i].fluxarea[2];
			}
		} 
	}
	if(debugS != faceS){ DSTR << "diff between debugS:" << debugS << ", faceS:" << faceS << std::endl; }
	if( debugdq <= dqdtAll - 1e-8 &&  dqdtAll + 1e-8 <= debugdq){	DSTR << "面積が大体同じではない" <<std::endl;} 	//大体同じではないときに、警告			大体同じときの条件判定　dqdtAll - 1e-8 <= debugdq && debugdq <= dqdtAll + 1e-8: 大体同じ 
}		// /*CalcIHdqdt_decrease*/


void PHFemThermo::CalcIHdqdt_add_high(double r,double R,double dqdtAll,unsigned mode){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//%%%			[1]について計算
	//dqdtAllを単位面積辺り位に直す
	double dqdtdsAll =0.0;

	///	内半径と外半径の間の節点に熱流束境界条件を設定
	//	少しでも領域にかかっていれば、IH加熱に含める
	int cnt = 0;
	//	初期化
	for(unsigned i=0;i<mesh->faces.size();i++){
		faceVars[i].fluxarea[1] = 0.0;
	}
	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			for(unsigned j=0;j<3;j++){
				if( r <= vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin && vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin <= R){
					faceVars[i].fluxarea[1] = faceVars[i].area;
					break;
				}
			}
			faceS += faceVars[i].fluxarea[1];
			cnt+=1;
		}
	}
	double debugS=0.0;
	double debugdq=0.0;
	if(faceS > 0){
		//dqdtdsAll = dqdtAll / faceS;
		//> dqdt を単位面積あたりに直す([1/m^2])
		double dqdtds = dqdtAll / faceS;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].mayIHheated){
				double avgTemp=0.0;
				double ohm = 0.0;
				avgTemp = 1.0 / 3.0 * ( vertexVars[mesh->faces[i].vertexIDs[0]].temp + vertexVars[mesh->faces[i].vertexIDs[1]].temp + vertexVars[mesh->faces[i].vertexIDs[2]].temp);
				if(avgTemp >= 25.0 && avgTemp < 102.0){
					ohm = (avgTemp - 25) * (9.92e-10) + 5.86e-7;
				}
				else if(102.0 <= avgTemp && avgTemp < 202.0){
					ohm = (avgTemp -102.0) * 9.2e-10 + 6.87e-7;
				}
				else if(202.0<= avgTemp && avgTemp < 302.0){
					ohm = (avgTemp -202.0) * 8.3e-10 + 7.79e-7;
				}
				else if(302.0 <= avgTemp && avgTemp <402.0){
					ohm = (avgTemp -302.0) * 7.5e-10 + 8.62e-7;
				}
				if(avgTemp > 25.0){
					faceVars[i].heatflux[mode][0] = dqdtds * (5.86e-7)/(ohm);		//	熱流束の量をheatfluxの面積から計算し、J/m^2に直さなければいけない→行列で計算するために。
				}else{
					faceVars[i].heatflux[mode][0] = dqdtds;
				}
				//debug
				debugdq += dqdtds * faceVars[i].fluxarea[1];
				debugS += faceVars[i].fluxarea[1];
			}
		} 
	}
	if(debugS != faceS){ DSTR << "diff between debugS:" << debugS << ", faceS:" << faceS << std::endl; }
	if( debugdq <= dqdtAll - 1e-8 &&  dqdtAll + 1e-8 <= debugdq){	DSTR << "面積が大体同じではない" <<std::endl;} 	//大体同じではないときに、警告			大体同じときの条件判定　dqdtAll - 1e-8 <= debugdq && debugdq <= dqdtAll + 1e-8: 大体同じ 
}		// /*CalcIHdqdt_add*/

void PHFemThermo::CalcIHdqdt_add(double r,double R,double dqdtAll,unsigned mode){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//%%%			[1]について計算
	//dqdtAllを単位面積辺り位に直す
	double dqdtdsAll =0.0;

	///	内半径と外半径の間の節点に熱流束境界条件を設定
	//	少しでも領域にかかっていれば、IH加熱に含める
	int cnt = 0;
	//	初期化
	for(unsigned i=0;i<mesh->faces.size();i++){
		faceVars[i].fluxarea[1] = 0.0;
	}
	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			for(unsigned j=0;j<3;j++){
				if( r <= vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin && vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin <= R){
					faceVars[i].fluxarea[1] = faceVars[i].area;
					break;
				}
			}
			faceS += faceVars[i].fluxarea[1];
			cnt+=1;
		}
	}
	double debugS=0.0;
	double debugdq=0.0;
	if(faceS > 0){
		//dqdtdsAll = dqdtAll / faceS;
		//> dqdt を単位面積あたりに直す([1/m^2])
		double dqdtds = dqdtAll / faceS;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].mayIHheated){
				faceVars[i].heatflux[mode][1] = dqdtds;		//	熱流束の量をheatfluxの面積から計算し、J/m^2に直さなければいけない→行列で計算するために。
				//debug
				debugdq += dqdtds * faceVars[i].fluxarea[1];
				debugS += faceVars[i].fluxarea[1];
			}
		} 
	}
	if(debugS != faceS){ DSTR << "diff between debugS:" << debugS << ", faceS:" << faceS << std::endl; }
	if( debugdq <= dqdtAll - 1e-8 &&  dqdtAll + 1e-8 <= debugdq){	DSTR << "面積が大体同じではない" <<std::endl;} 	//大体同じではないときに、警告			大体同じときの条件判定　dqdtAll - 1e-8 <= debugdq && debugdq <= dqdtAll + 1e-8: 大体同じ 
}		// /*CalcIHdqdt_add*/

void PHFemThermo::CalcIHdqdt_atleast_map(Vec2d origin,double dqdtAll,unsigned mode){
	PHFemMeshNew* mesh = GetPHFemMesh();

	double faceSq[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};								//　円環毎の面積保存
	// 10sec時
	//double ratio[10] = {0.016646849,	0.022592152,	0.051129608,	0.087990488,	0.12960761,	0.162901308,	0.180737218,	0.168846611,	0.122473246,	0.057074911};		//	加熱流束分担割合
	// 5sec時
	double ratio[10] = {0.007653061,	0.015306122,	0.043367347,	0.084183673,	0.130102041,	0.168367347,	0.18877551,	0.181122449,	0.130102041,	0.051020408};
	//	最初の4項から0.01ひいて、後ろに加算
	//double ratio[10] = {0.002653061,	0.010306122,	0.038367347,	0.079183673,	0.130102041,	0.168367347,	0.18977551,	0.182122449,	0.131102041,	0.052020408};
	ratio[5] += ratio[0];
	ratio[0] -= ratio[0];
	for(unsigned i=1;i<5;++i){
		ratio[i] = ratio[i] - 0.007;
	}
	for(unsigned i=6;i<10;++i){
		ratio[i] = ratio[i] + 0.007;
	}
	double areaQ[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	for(unsigned i=0;i<10;++i){
		areaQ[i] = ratio[i] * dqdtAll;		//	ここは1sec当たりの値で良い。時間積分で調整するため
	}
	//double rsum=0;
	//double Qsum=0;
	//for(unsigned i=0;i<10;++i){
	//	rsum += ratio[i];
	//	Qsum += areaQ[i];
	//}
	//DSTR << "rsum:"<<rsum<<std::endl;
	//DSTR << "Qsum:"<<Qsum<<std::endl;
	//　計算されていない場合に備えて
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
		}
	}

	//> 表面faceの内、原点から各faceの節点のローカル(x,z)座標系での平面上の距離の計算を、faceの全節点のy座標が負のものに対して、IH加熱の可能性を示すフラグを設定
	for(unsigned i=0;i<mesh->nSurfaceFace;i++){
		//	(x,z)平面におけるmayIHheatedのface全節点の原点からの距離を計算する
		for(unsigned j=0; j<3; j++){
			double dx = mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x - origin[0];
			double dz = mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.z - origin[1];	//	表記はyだが、実質z座標が入っている
			vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin = sqrt( dx * dx + dz * dz);
		}
	}
	//求めた距離に応じて、設定された初期条件を満たすような温度分布を作る
	float r[11];		//	10こめの外側が必要なため
	for(unsigned i=0;i<11;++i){
		r[i] = 0.01 * i;
	}

	//	中心から一定範囲内の面積和を求める	face原点で判断
	for(unsigned id=0;id<mesh->nSurfaceFace;++id){
		if(faceVars[id].mayIHheated){
			//	faceの重心の原点からの距離を求める
			double grvPnt = 1.0 / 3.0 * (vertexVars[mesh->faces[id].vertexIDs[0]].disFromOrigin + vertexVars[mesh->faces[id].vertexIDs[1]].disFromOrigin +vertexVars[mesh->faces[id].vertexIDs[2]].disFromOrigin);
			for(unsigned i=0; i < 10;++i){
				if(i>0){
					if( (r[i-1] + r[i]) / 2.0 < grvPnt && grvPnt <= ( r[i] + r[i+1]) / 2.0 ){
						faceSq[i] += faceVars[id].area;
						faceVars[id].map = i;
						faceVars[id].fluxarea[3] = faceVars[id].area;
						//DSTR << "id:" << id << ", i:" << i << std::endl; 
					} 
				}
				else{
					if(grvPnt <=  (r[i] + r[i+1]) /2.0){
						faceSq[i] += faceVars[id].area;
						faceVars[id].map = i;
						faceVars[id].fluxarea[3] = faceVars[id].area;
						//DSTR << "id:" << id << ", i:" << i << std::endl;
					} 
				}

			}
		}		
	}
	//face節点のうち、1点でも範囲内に入ったら、探索を止めて、その範囲のfaceと確定する。全部のタグ付け後に、当該領域の面積和を求めないとズレそう
	//double debugfaceSum=0;
	//for(unsigned i=0;i<10;i++){
	//	DSTR <<"faceSq["<< i << "]:"<<  faceSq[i] << std::endl;
	//	debugfaceSum += faceSq[i];
	//}
	double debugW=0;
	for(unsigned id=0;id<mesh->nSurfaceFace;++id){
		if(faceVars[id].mayIHheated){
			for(unsigned i=0;i < 10; ++i){
				if(faceSq[i]>0){
					//	加熱流束対象face（範囲内face）のみ
					if(faceVars[id].map<10){
						faceVars[id].heatflux[mode][3] = areaQ[faceVars[id].map] / faceSq[i] ;		// J/m^2に直す。atleastだけのソース参照(熱流束の量をheatfluxの面積から計算し、J/m^2に直さなければいけない→行列で計算するために。)
						//debugW += faceSq[i] * faces[id].heatflux[mode][3];
					}
				}
			}
		}		
	}
	//debug
	//for(unsigned i=0;i<10;++i){
	//	if(faceSq[i]>0){
	//		debugW += areaQ[i] * faceSq[i] / faceSq[i];
	//		DSTR << faceSq[i] / faceSq[i] <<std::endl;
	//	}
	//}
	//DSTR << "面積和[m^2]:"<<debugfaceSum<<std::endl;
	//DSTR << "加熱和[W]:" << debugW <<std::endl;

}		// /*CalcIHdqdt_atleast_hogehoge*/

void PHFemThermo::CalcIHdqdt_atleast_high(double r,double R,double dqdtAll,unsigned mode){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//dqdtAllを単位面積辺り位に直す
	double dqdtdsAll =0.0;

	///	内半径と外半径の間の節点に熱流束境界条件を設定
	//	少しでも領域にかかっていれば、IH加熱に含める
	int cnt = 0;
	//	初期化
	for(unsigned i=0;i<mesh->faces.size();i++){
		faceVars[i].fluxarea[0] = 0.0;
		faceVars[i].fluxarea[1] = 0.0;
		faceVars[i].fluxarea[2] = 0.0;
	}
	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			for(unsigned j=0;j<3;j++){
				if( r <= vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin && vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin <= R){
					//|| vertices[faces[i].vertices[j]].edges[0] ){
					faceVars[i].fluxarea[0] = faceVars[i].area;
					break;
				}
			}
			faceS += faceVars[i].fluxarea[0];
			cnt+=1;
		}
	}
	double debugS=0.0;
	double debugdq=0.0;
	double powerRatio =0.0;
	if(faceS > 0){
		//dqdtdsAll = dqdtAll / faceS;
		//> dqdt を単位面積あたりに直す([1/m^2])
		double dqdtds = dqdtAll / faceS;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].mayIHheated){
				double avgTemp = 0.0;
				double ohm = 0.0;
				avgTemp = 1.0 / 3.0 * ( vertexVars[mesh->faces[i].vertexIDs[0]].temp + vertexVars[mesh->faces[i].vertexIDs[1]].temp + vertexVars[mesh->faces[i].vertexIDs[2]].temp);
				if(avgTemp >= 25.0 && avgTemp < 102.0){
					ohm = (avgTemp - 25) * (9.92e-10) + 5.86e-7;
				}
				else if(102.0 <= avgTemp && avgTemp < 202.0){
					ohm = (avgTemp -102.0) * 9.2e-10 + 6.87e-7;
				}
				else if(202.0<= avgTemp && avgTemp < 302.0){
					ohm = (avgTemp -202.0) * 8.3e-10 + 7.79e-7;
				}
				else if(302.0 <= avgTemp && avgTemp <402.0){
					ohm = (avgTemp -302.0) * 7.5e-10 + 8.62e-7;
				}
				//if(ohm > 5.91e-7){
				//	DSTR << "ohm: " << ohm << std::endl;
				//	DSTR << "avgTemp:" << avgTemp <<std::endl; 
				//	int katton=0;
				//}
				if(avgTemp > 25.0){
					faceVars[i].heatflux[mode][0] = dqdtds * (5.86e-7)/(ohm);		//	熱流束の量をheatfluxの面積から計算し、J/m^2に直さなければいけない→行列で計算するために。
				}else{
					faceVars[i].heatflux[mode][0] = dqdtds;
				}
				//debug
				debugdq += dqdtds * faceVars[i].fluxarea[0];
				debugS += faceVars[i].fluxarea[0];
			}
		} 
	}
	if(debugS != faceS){ DSTR << "diff between debugS:" << debugS << ", faceS:" << faceS << std::endl; }
	if( debugdq <= dqdtAll - 1e-8 &&  dqdtAll + 1e-8 <= debugdq){	DSTR << "面積が大体同じではない" <<std::endl;} 	//大体同じではないときに、警告			大体同じときの条件判定　dqdtAll - 1e-8 <= debugdq && debugdq <= dqdtAll + 1e-8: 大体同じ 
	//　以上、値は入っているようだ
	//DSTR << "face 加熱面数cnt: " << cnt<<std::endl;
}		// /*CalcIHdqdt_atleast*/

void PHFemThermo::CalcIHdqdt_atleast(double r,double R,double dqdtAll,unsigned mode){
	PHFemMeshNew* mesh = GetPHFemMesh();


	//dqdtAllを単位面積辺り位に直す
	double dqdtdsAll =0.0;

	///	内半径と外半径の間の節点に熱流束境界条件を設定
	//	少しでも領域にかかっていれば、IH加熱に含める
	int cnt = 0;
	//	初期化
	for(unsigned i=0;i<mesh->faces.size();i++){
		faceVars[i].fluxarea[0] = 0.0;
		faceVars[i].fluxarea[1] = 0.0;
		faceVars[i].fluxarea[2] = 0.0;
	}
	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			for(unsigned j=0;j<3;j++){
				if( r <= vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin && vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin <= R){
					//|| vertices[faces[i].vertices[j]].edges[0] ){
					faceVars[i].fluxarea[0] = faceVars[i].area;
					break;
				}
			}
			faceS += faceVars[i].fluxarea[0];
			cnt+=1;
		}
	}
	double debugS=0.0;
	double debugdq=0.0;
	if(faceS > 0){
		//dqdtdsAll = dqdtAll / faceS;
		//> dqdt を単位面積あたりに直す([1/m^2])
		double dqdtds = dqdtAll / faceS;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].mayIHheated){
				faceVars[i].heatflux[mode][0] = dqdtds;		//	熱流束の量をheatfluxの面積から計算し、J/m^2に直さなければいけない→行列で計算するために。
				//debug
				debugdq += dqdtds * faceVars[i].fluxarea[0];
				debugS += faceVars[i].fluxarea[0];
			}
		} 
	}
	if(debugS != faceS){ DSTR << "diff between debugS:" << debugS << ", faceS:" << faceS << std::endl; }
	if( debugdq <= dqdtAll - 1e-8 &&  dqdtAll + 1e-8 <= debugdq){	DSTR << "面積が大体同じではない" <<std::endl;} 	//大体同じではないときに、警告			大体同じときの条件判定　dqdtAll - 1e-8 <= debugdq && debugdq <= dqdtAll + 1e-8: 大体同じ 
	//　以上、値は入っているようだ
	//DSTR << "face 加熱面数cnt: " << cnt<<std::endl;
}		// /*CalcIHdqdt_atleast*/

void PHFemThermo::CalcIHdqdt(double r,double R,double dqdtAll,unsigned num){
	PHFemMeshNew* mesh = GetPHFemMesh();

	///	内半径と外半径の間の節点に熱流束境界条件を設定

	//> 加熱する四面体面の面積の総和を求める
	double faceS = 0.0;
	for(unsigned i=0;i < mesh->nSurfaceFace; i++){
		if(faceVars[i].mayIHheated){			// faceの節点のy座標が負の場合→IH加熱の対象節点
			unsigned nObinnerVtx = 0;
			if(faceVars[i].area==0) faceVars[i].area = CalcTriangleArea(mesh->faces[i].vertexIDs[0],mesh->faces[i].vertexIDs[1],mesh->faces[i].vertexIDs[2]);
			for(unsigned j=0;j<3;j++){
				if( r <= vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin && vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin <= R){
					nObinnerVtx += 1;
				}
			}
			if( nObinnerVtx == 1)			faceVars[i].fluxarea[0] = 1.5/3.0 * faceVars[i].area;//faces[i].fluxarea = 1.0/3.0 * faces[i].area;
			else if(nObinnerVtx == 2)		faceVars[i].fluxarea[0] = 2.8/3.0 * faceVars[i].area;//faces[i].fluxarea = 2.0/3.0 * faces[i].area;
			else if(nObinnerVtx == 3)		faceVars[i].fluxarea[0] = faceVars[i].area;
			else if(nObinnerVtx == 0)		faceVars[i].fluxarea[0] = 0;

			if(faceVars[i].fluxarea >= 0){	
				faceS += faceVars[i].fluxarea[0];
			}else{		assert(0);	}		//	faces[i].fluxareaに0未満の数字が入っているのに加算しようとしている
			//DSTR << "faces[" << i << "].fluxarea: " << faces[i].fluxarea << std::endl;
		}
	}

	//for(unsigned i=0;i < nSurfaceFace; i++){
	//	DSTR << "faces[" << i << "].fluxarea: " << faces[i].fluxarea << std::endl;
	//}

	if(faceS > 0){
		//> dqdt を単位面積あたりに直す([1/m^2])
		double dqdtds = dqdtAll / faceS;
//		DSTR << "dqdtds:  " << dqdtds << std::endl;
		//>	以下、熱流束をfacesに格納する
		//>	熱流束の面積計算はfluxareaを用いて行う
		for(unsigned i=0;i < mesh->nSurfaceFace; i++){
			if(faceVars[i].mayIHheated){
				faceVars[i].heatflux[num][0] = dqdtds * faceVars[i].fluxarea[0];		//	熱流束の量をheatfluxの面積から計算
//				DSTR << "faces[" << i <<"].heatflux: " << faces[i].heatflux <<std::endl;
			}
		}
	}
	
	//　以上、値は入っているようだ

	int katoon =0;
	//↑をつかって、CreateMatk2tをコピーした関数で、Vecf2?を作る基に

	//>	熱量は、dqdtdsを用いる

	//> r <= <= Rの中心から放射状に加熱

	//	節点でdqdtの値を更新する

	//　以下は、ベクトルを作る関数の仕事
	//	節点の属する表面の面で、計算する
	//  vertices[].heatFluxValueを基に計算を進める
	//	ガウスザイデル計算できるように処理など、準備する

}
/// face毎に作ってしまうのが良いのか、verticesごとにやるのがいいのか。どっちがいいか分からないので、ひとまず、vertices毎に作ってしまおう

//void PHFemThermo::SetVertexHeatFlux(int id,double heatFlux){
//	vertices[id].heatFluxValue = heatFlux;
//}

//void PHFemThermo::SetVtxHeatFluxAll(double heatFlux){
//	for(unsigned i=0; i < vertices.size() ;i++){
//		SetVertexHeatFlux(i,heatFlux);
//	}
//}


void PHFemThermo::CalcHeatTransDirect2(double dt){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//lapack利用
	int n = (int)mesh->vertices.size();

	double eps =0.5;
	//	係数行列の作成
	keisu.resize(mesh->vertices.size(),mesh->vertices.size());
	keisu.clear();

	PTM::VVector<double> uhen;
	uhen.resize(mesh->vertices.size(),1);
	uhen.clear();

	keisu = eps * matKAll + 1 / dt * matCAll;
	uhen = (- (1.0 - eps) * matKAll - 1 / dt * matCAll ) * TVecAll + vecFAllSum;

	//if(deformed) keisuInv = keisu.inv(); deformed = false;
	//TVecAll = keisuInv * uhen;

	//PTM::VMatrixRow<double> matk;		//keisu
	PTM::VVector<double> x;				//TVecAll
	//PTM::VVector<double> b;			//uhen
	PTM::VVector<int> ip;
	//int n= 5000;
	//matk.resize(n, n, 0.0);
	//b.resize(n);
	ip.resize(n);
	//for(int i=0; i<n; ++i){
	//	for(int j=0; j<n; ++j){
	//		matk[i][j] = rand();
	//	}
	//}
	//for(int i=0; i<n; ++i){
	//	b[i] = i*10+15;
	//}


	typedef double element_type;
	typedef bindings::remove_imaginary<element_type>::type real_type ;
	typedef bindings::remove_imaginary<int>::type int_type ;
	typedef ublas::vector< real_type > vector_type;
	typedef ublas::matrix< element_type, ublas::column_major > matrix_type;
	ublas::vector<int_type> ipiv(n);
	matrix_type mm(n, n);
	vector_type bb(n);
	for(int i=0; i<n; ++i){
		bb[i] = uhen[i];		//b
	}
	for(int i=0; i<n; ++i){
		for(int j=0; j<n; ++j){
			mm.at_element(i, j)=keisu[i][j];		//=matk
		}
	}
	double det = lapack::gesv(mm, ipiv, bb);
	x.resize(n);
	for(int i=0; i<n; ++i){
		x[i] = bb[i];
	}
	for(int i=0; i<n; ++i){
		TVecAll[i] = x[i];
	}
//	DSTR << "TVecAll: " << TVecAll <<  std::endl;

}

void PHFemThermo::CalcHeatTransDirect(double dt){
	PHFemMeshNew* mesh = GetPHFemMesh();

	if(strcmp(mesh->GetName(), "femNsteak") == 0){
		for(unsigned i=0; i < mesh->vertices.size(); i++){
			DSTR << vecFAllSum[i] << std::endl;
		}
		DSTR << std::endl;
	}

	//直接法利用

	double eps =0.5;
	double eps2 = 0.5;
	double eps3 = 1.0;
	eps = 1.0;
	
//	DSTR << "tets[0].volume: " << tets[0].volume << std::endl;
	
	TVecAll2.resize(mesh->vertices.size());
	TVecAll2 = TVecAll;


	PTM::VVector<double> TVecAll3;	
	//// define @.h
	TVecAll3.resize(mesh->vertices.size());
	TVecAll3 = TVecAll;


	//	係数行列の作成
	keisu.resize(mesh->vertices.size(),mesh->vertices.size());
	keisu.clear();

	//DSTR << "tets.size(): " << tets.size() << ", vertices.size(): " << vertices.size() <<", edges.size(): " << edges.size()<<  std::endl;
	//DSTR << "eps: " << eps << std::endl;

	PTM::VVector<double> uhen;
	uhen.resize(mesh->vertices.size(),1);
	uhen.clear();
	//DSTR <<"TVecAll_before: "<< TVecAll <<std::endl;
	double TEMP =0.0;
	for(unsigned i=0; i<mesh->vertices.size();i++){
		TEMP += vecFAllSum[i];
	}
//	DSTR << "TEMP: " << TEMP << std::endl;

	keisu = eps * matKAll + 1 / dt * matCAll;

	////vecfの分割を適用
	//PTM::VVector<double> vecFAllTemp;
	//vecFAllTemp.resize(mesh->vertices.size());
	//vecFAllTemp.clear();
	//for(unsigned i=0; i < mesh->vertices.size(); i++){
	//	for(unsigned j=0; j < mesh->vertices.size(); j++){
	//		vecFAllTemp[j] += vecFAllSum[i] * fpat[j][i];
	//	}
	//}
	//uhen = (- (1.0 - eps) * matKAll + 1 / dt * matCAll ) * TVecAll + vecFAllTemp;

	//createNewVecF();

//	DSTR <<"keisu: " << keisu << std::endl;
	uhen = (- (1.0 - eps) * matKAll + 1 / dt * matCAll ) * TVecAll + vecFAllSum;
//	DSTR << "(- (1.0 - eps) * matKAll + 1 / dt * matCAll ) * TVecAll: " << (- (1.0 - eps) * matKAll + 1 / dt * matCAll ) * TVecAll <<std::endl;
//	DSTR << "vecFAllSum: "  << vecFAllSum <<std::endl;

	if(deformed){ 
		keisuInv = keisu.inv();
		deformed = false;
	}


	//DSTR << "keisuInv: " << keisuInv << std::endl;

	//PTM::VMatrixRow<double> VV;
	//VV.resize(vertices.size(),vertices.size());
	//VV.clear();
	//VV = keisu * keisuInv;

	//DSTR << "keisu * keisuInv" <<std::endl;
	//DSTR << VV << std::endl;
	//DSTR << "VV対角成分" <<std::endl;
	//for(unsigned i=0;i<vertices.size();i++){
	//	DSTR << VV[i][i] <<std::endl;
	//}

	//DSTR <<"keisu.det(): " << keisu.det() << std::endl;
	//DSTR << "keisuInv.det(): " << keisuInv.det() << std::endl;

	//int keisucnt =0;
	//for(int i=0;i<vertices.size();++i){
	//	for(int j=0;j<vertices.size();++j){
	//		if(keisu[i][j] <0){
	//			keisucnt +=1;
	//		} 
	//	}
	//}
//	DSTR <<keisucnt << std::endl;

	//DSTR << "before calculate "<< std::endl;
	//DSTR << "TVecAll:  " << TVecAll << std::endl; 
	//DSTR << "TVecAll2: " << TVecAll2 << std::endl; 
	//DSTR << "TVecAll3: " << TVecAll3 << std::endl; 

	
	//DSTR << "uhen: " << uhen << std::endl;
	//PTM::VMatrixRow<double> keisuInv2;
	//keisuInv2.resize(mesh->vertices.size(), mesh->vertices.size());
	//keisuInv2.clear();
	//
	//for(unsigned i=0; i < mesh->vertices.size(); i++){
	//	for(unsigned j=0; j < mesh->vertices.size(); j++){
	//		if(keisuInv[i][j] < 0){
	//			keisuInv2[i][j] = 0;
	//		}else{
	//			keisuInv2[i][j] = keisuInv[i][j]; 
	//		}
	//	}
	//}

	//TVecAll = keisuInv * (- (1.0 - eps) * (matKAll + 1 / dt * matCAll ) * TVecAll) + (1 / dt * matCAll).inv() * vecFAllSum;
	//TVecAll = keisuInv * ((- (1.0 - eps) * matKAll + 1 / dt * matCAll ) * TVecAll) + keisuInv2 * vecFAllSum;
	TVecAll = keisuInv * uhen;
	//TVecAll = preTVecAll + 2*tdt*matCAll.inv()*(vecFAllSum - matKAll*TVecAll);
	//preTVecAll = TVecAll3;
	//if(makeAFlag){
	//	A.resize(mesh->vertices.size(), mesh->vertices.size());
	//	ImA.resize(mesh->vertices.size(), mesh->vertices.size());
	//	IpAinv.resize(mesh->vertices.size(), mesh->vertices.size());
	//	keisuIA.resize(mesh->vertices.size(), mesh->vertices.size());
	//	
	//	A = 0.5 * tdt * matCAll.inv() * matKAll;
	//	ImA = I-A;
	//	IpAinv = (I + A).inv();
	//	keisuIA = IpAinv * ImA;

	//	makeAFlag = false;
	//}

	//TVecAll = keisuIA * TVecAll;

	////非対角ようその計算後に対角要素を足す
	//PTM::VVector<double> pT;
	//pT.resize(mesh->vertices.size());
	//pT.clear();
	//TVecAll.clear();
	//for(unsigned i=0; i < mesh->vertices.size(); i++){
	//	for(unsigned j=0; j < mesh->vertices.size(); j++){
	//		if(i==j){
	//			pT[i] = keisuInv[i][j]*uhen[j];
	//		}else{
	//			TVecAll[i] += keisuInv[i][j]*uhen[j];
	//		}
	//	}
	//}
	//TVecAll += pT;
	double sumtemp=0.0;

	

	//for(unsigned i=0;i<vertices.size();i++){
	//	for(unsigned j=0; j < vertices[i].tets.size(); j++){
	//		sumtemp += tets[vertices[i].tets[j]].volume * RHO * SPECIFICHEAT * 5 / 20 * TVecAll[i]; 
	//	}
	//}
	//DSTR << "sumtemp: "<< sumtemp <<std::endl;


	//DSTR<< "TVecAll: " << TVecAll <<std::endl;

	//PTM::VMatrixRow<double> keisu2;
	//keisu2.resize(vertices.size(),vertices.size());
	//keisu2.clear();

	//keisu2 = keisuInv.inv();
	//int checkint=0;
	//for(unsigned i=0;i<vertices.size();i++){
	//	for(unsigned j=0;j<vertices.size();j++){
	//		if(keisu2[i][j] != keisu[i][j]){
	//			checkint +=1;
	//		}
	//	}
	//}
	//DSTR << "checkint: " << checkint << std::endl;

	//DSTR <<"keisu: ";
	//DSTR << keisu << std::endl;

	//DSTR << "keisuInvInv";
	//DSTR << keisu2 << std::endl;

	// 係数行列の非0成分が逆逆行列で0になっているかどうかをcheck
	//for(unsigned i=0;i<vertices.size();i++){
	//	for(unsigned j=0;j<vertices.size();j++){
	//		if(keisu[i][j]!=0){
	//			DSTR << keisu[i][j] - keisu2[i][j] << " - " <<  keisu[i][j] << " : "<< keisu2[i][j] << std::endl;
	//		}
	//	}
	//}

	//// ０成分は0か？
	//for(unsigned i=0;i<vertices.size();i++){
	//	for(unsigned j=0;j<vertices.size();j++){
	//		if(keisu[i][j] ==0){
	//			DSTR << keisu[i][j] - keisu2[i][j] << " - " <<  keisu[i][j] << " : "<< keisu2[i][j] << std::endl;
	//		}
	//	}
	//}


	//PTM::VVector<double> uhen2;
	//uhen2.resize(vertices.size(),1);
	//uhen2.clear();
	//PTM::VMatrixRow<double> keisuInv2;
	//keisuInv2.resize(vertices.size(),vertices.size());
	//keisuInv2.clear();

	//keisu2 = eps2 * matKAll + 1 / dt * matCAll;
	//keisuInv2 = keisu2.inv();
	//uhen2 = (- (1.0 - eps2) * matKAll + 1 / dt * matCAll ) * TVecAll2 + vecFAllSum;
	//TVecAll2 = keisuInv2 * uhen2;

	//
	//DSTR << uhen2 << std::endl;
	//DSTR << keisu2 << std::endl;

	//DSTR << keisuInv2 << std::endl;

	//PTM::VMatrixRow<double> keisu3;			//	直接法で計算時のT(t+dt)係数行列
	//PTM::VMatrixRow<double> keisuInv3;
	//PTM::VVector<double> uhen3;
	//keisu3.resize(vertices.size(),vertices.size());
	//keisu3.clear();
	//uhen3.resize(vertices.size(),1);
	//uhen3.clear();

	//keisuInv3.resize(vertices.size(),vertices.size());
	//keisuInv3.clear();


	//keisu3 = eps3 * matKAll + 1 / dt * matCAll;
	//keisuInv3 = keisu3.inv();
	//uhen3 = (- (1.0 - eps3) * matKAll + 1 / dt * matCAll ) * TVecAll3 + vecFAllSum;

	//TVecAll3 = keisuInv3 * uhen3;


	//int diff_keisu=0;
	//int diff_uhen=0;
	//int diff_keisuInv=0;
	//int diff_TVecAll=0;
	//int diff_TVecAll13=0;

	//int dk2 = 0;
	//int du2 = 0;
	//int di2 = 0;


	//for(int i=0;i<vertices.size();i++){
	//	for(int j=0;j<vertices.size();j++){
	//		if(keisu[i][j] != keisu2[i][j]){
	//			diff_keisu += 1;
	//		}
	//		if(keisu[i][j] != keisu3[i][j]){
	//			dk2 += 1;
	//		}
	//		if(keisuInv[i][j] != keisuInv2[i][j]){
	//			diff_keisuInv += 1;
	//		}
	//		if(keisuInv[i][j] != keisuInv3[i][j]){
	//			di2 += 1;
	//		}
	//	}
	//	if(uhen[i] != uhen2[i]){
	//		diff_uhen += 1;
	//	}
	//	if(uhen[i] != uhen3[i]){
	//		du2 += 1;
	//	}
	//	if(TVecAll[i] != TVecAll2[i]){
	//		diff_TVecAll += 1;
	//	}
	//	if(TVecAll[i] != TVecAll2[i] || TVecAll[i] != TVecAll3[i] || TVecAll2[i] != TVecAll3[i] ){
	//		diff_TVecAll13 += 1;
	//		if(i==0)DSTR << "eps: " << eps <<", "<< eps2 << ", " << eps3<<std::endl;
	//		DSTR << TVecAll[i] << ", " << TVecAll2[i] << ", " << TVecAll3[i] << std::endl;
	//	}
	//}
	//DSTR << "diff_keisu: " << diff_keisu << ", diff_uhen: " << diff_uhen <<", diff_keisuInv: " << diff_keisuInv << ", diff_TVecAll: " << diff_TVecAll << std::endl; 
	//DSTR << "diff_keisu(1-3): " << dk2 << ", diff_uhen(1-3): " << du2 <<", diff_keisuInv(1-3): " << di2 << "diff_TVecAll1-3" << diff_TVecAll13 <<std::endl;

	//DSTR <<"TVecAll_calculated: "<< TVecAll <<std::endl;
	//DSTR <<"TVecAll2_calculated: "<< TVecAll2 <<std::endl;
	//DSTR <<"TVecAll3_calculated: "<< TVecAll3 <<std::endl;
	//DSTR << "TVecAll[0]" << TVecAll[0] <<std::endl;
	//DSTR << "TVecAll2[0]" << TVecAll2[0] <<std::endl;
	//DSTR << "TVecAll3[0]" << TVecAll3[0] <<std::endl;

	int debughensu=0;
}

void PHFemThermo::CalcHeatTransUsingScilab(double dt, double eps){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//直接法利用

	//PTM::TMatrixRow<1,1,double> dtMat;
	//PTM::TMatrixRow<1,1,double> epsMat;
	PTM::VMatrixRow<double> uhenScilab;
	PTM::VMatrixRow<double> TAll;

	uhenScilab.resize(mesh->vertices.size(), 1);
	TAll.resize(mesh->vertices.size(), 1);
	uhenScilab.clear();
	TAll.clear();

	//dtMat.clear();
	//epsMat.clear();
	//dtMat[0][0] = dt;


	TVecAll2.resize(mesh->vertices.size());
	TVecAll2 = TVecAll;


	PTM::VVector<double> TVecAll3;	
	TVecAll3.resize(mesh->vertices.size());
	TVecAll3 = TVecAll;


	//	係数行列の作成
	keisu.resize(mesh->vertices.size(),mesh->vertices.size());
	keisu.clear();

	PTM::VVector<double> uhen;
	uhen.resize(mesh->vertices.size(),1);
	uhen.clear();
	double TEMP =0.0;
	for(unsigned i=0; i<mesh->vertices.size();i++){
		TEMP += vecFAllSum[i];
	}

	keisu = eps * matKAll + 1 / dt * matCAll;

	uhen = (- (1.0 - eps) * matKAll + 1 / dt * matCAll ) * TVecAll + vecFAllSum;

	if(deformed){ 
		//keisuInv = keisu.inv();
		ScilabSetMatrix("keisu", keisu);
		ScilabJob("keisuInv = inv(keisu)");
		ScilabGetMatrix(keisuInv, "keisuInv");
		deformed = false;
	}

	//for(unsigned i=0; i < uhen.size(); i++){
	//	uhenScilab[i][0] = uhen[i];
	//}
	//ScilabSetMatrix("uhen", uhenScilab);
	//ScilabJob("TAll = -linsolve(keisu, uhen)");
	//ScilabGetMatrix(TAll, "TAll");

	//for(unsigned i=0; i < mesh->vertices.size(); i++){
	//	TVecAll[i] = TAll[i][0];
	//}

	TVecAll = keisuInv * uhen;
	
	double sumtemp=0.0;
}

//// ガウスザイデル法を使いAx+b>0を解く
//	template <class AD, class XD, class BD>
//	void GaussSeidel(MatrixImp<AD>& a, VectorImp<XD>& x, const VectorImp<BD>& b){
//		int nIter = 15;					// 反復回数の上限
//		double error = 0.0;
//		double errorRange = 10e-8;		// 許容誤差
//		int n = (int)a.height();		// 連立方程式の数(行列aの行数)
//		std::vector< double > lastx;
//		for(int i = 0; i < n; i++){
//			lastx.push_back(x[i]);
//			x[i] = 0;
//		}
//
//		for(int k = 0; k < nIter; k++){		
//			for(int i = 0; i < n; i++){
//				double term1 = 0.0;
//				double term2 = 0.0;
//				for(int j = 0; j < i; j++){
//					term1 += a[i][j] * x[j];
//				}
//				for(int j = i+1; j < n; j++){
//					term2 += a[i][j] * lastx[j];
//				}
//				// xの更新(繰り返し計算の式を使用)
//				x[i] =  (-b[i] - term1 - term2) / a[i][i];
//				if(x[i] < 0) x[i] = 0.0;
//			}
//
//			// (lastx - x)の2乗の総和と誤差範囲を比較
//			error = 0.0;
//			for(int i = 0; i < n; i++){
//				error += pow(x[i] - lastx[i], 2);
//				//DSTR << "iterete" << i << "," << x[i] << std::endl;
//			}
//			if(error < errorRange){
//				//DSTR << "Finish the iteration in admissible error. " << std::endl;
//				//DSTR << k << std::endl;
//				return;
//			}
//
//			// 繰り返し計算のために更新後のxをlastxに保存
//			for(int i = 0; i < n; i++) lastx[i] = x[i];
//		}
//		//nIterで計算が終わらなかったので打ち切り
//		//static int iterError = 0;
//		//iterError += 1;
//		//DSTR << iterError << "Could not converge in iteration steps. Error = " << error << std::endl;
//		//CSVOUT << error << std::endl;
//	}

//void PHFemThermo::CalcHeatTransUsingGaussSeidel2(unsigned NofCyc,double dt,double eps){
//	double error = 0.0;
//	double errorRange = 1e-7;
//	std::vector<double> lastx;
//	for(unsigned i=0;i<vertices.size();i++){
//		lastx.push_back(TVecAll[i]);
//	}
//	
//	double _eps = 1-eps;			// 1-epsの計算に利用
//	bool DoCalc =true;											//初回だけ定数ベクトルbの計算を行うbool		//NofCycが0の時にすればいいのかも
//	for(unsigned i=0; i < NofCyc; i++){							//ガウスザイデルの計算ループ
//		if(DoCalc){												
//			if(deformed || alphaUpdated ){												//D_iiの作成　形状が更新された際に1度だけ行えばよい
//				for(unsigned j =0; j < vertices.size() ; j++){
//					_dMatAll.resize(1,vertices.size());
//					_dMatAll[0][j] = 1.0/ ( eps * dMatKAll[0][j] + 1.0/dt * dMatCAll[0][j] );		//1 / D__ii	を求める
//					//DSTR << "_dMatAll[0][" << j << "] : " << _dMatAll[0][j]  << std::endl;
//					int debughogeshi =0;
//				}
//				deformed = false;
//			}
//			//	 1      1        1  
//			//	--- ( - - [K] + ---[C] ){T(t)} + {F} 
//			//	D_jj    2       ⊿t
//			//
//
//			for(unsigned j =0; j < vertices.size() ; j++){		//初回ループだけ	係数ベクトルbVecAllの成分を計算
//				bVecAll[j][0] = 0.0;							//bVecAll[j][0]の初期化
//				//節点が属すedges毎に　対角成分(j,j)と非対角成分(j,?)毎に計算
//				//対角成分は、vertices[j].k or .c に入っている値を、非対角成分はedges[hoge].vertices[0] or vertices[1] .k or .cに入っている値を用いる
//				//ⅰ)非対角成分について
//				for(unsigned k =0;k < vertices[j].edges.size() ; k++){
//					unsigned edgeId = vertices[j].edges[k];
//					//リファクタリング	以下の条件分岐についてj>edges[edgeId].vertices[0] とそうでない時とで分けたほうが漏れが出る心配はない？
//					if( j != edges[edgeId].vertices[0]){					//節点番号jとedges.vertices[0]が異なる節点番号の時:非対角成分
//						unsigned vtxid0 = edges[edgeId].vertices[0];
//						bVecAll[j][0] += (-_eps * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid0];
//					}
//					else if( j != edges[edgeId].vertices[1] ){			//節点番号jとedges.vertices[1]が異なる節点番号の時:非対角成分
//						unsigned vtxid1 = edges[edgeId].vertices[1];
//						bVecAll[j][0] += (-_eps * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid1];
//
//					}
//					else{
//						//上記のどちらでもない場合、エラー
//						DSTR << "edges.vertex has 3 vertexies or any other problem" <<std::endl;
//					}
//				}
//				//ⅱ)対角成分について
//				bVecAll[j][0] += (-_eps * dMatKAll[0][j] + 1.0/dt * dMatCAll[0][j] ) * TVecAll[j];
//				//  {F}を加算
//				bVecAll[j][0] += vecFAllSum[j];		//Fを加算
//				//D_iiで割る ⇒この場所は、ここで良いの？どこまで掛け算するの？
//				bVecAll[j][0] = bVecAll[j][0] * _dMatAll[0][j];
//			}
//			DoCalc = false;			//初回のループだけで利用
//			int debughogeshi =0;
//		}		//if(DoCalc){...}
//
//#ifdef DEBUG
//		//	念のため、計算前の初期温度を0にしている。
//		if(i == 0){
//				for(unsigned j=0;j <vertices.size() ;j++){
//					TVecAll[j] = 0.0;
//				}
//		}
//#endif
//		//	 1      
//		//	--- [F]{T(t+dt)}
//		//	D_jj 		
//		//[F] = eps(ilon) [K] +1/dt [C] から対角成分を除し(-1)をかけたもの
//		//エッジに入っている成分に-1をかけるのではなく、最後に-1をかける。
//		//
//		for(unsigned j =0; j < vertices.size() ; j++){
//			//T(t+dt) = の式
//			//	まずtempkjを作る
//			double tempkj = 0.0;			//ガウスザイデルの途中計算で出てくるFの成分計算に使用する一時変数
//			for(unsigned k =0;k < vertices[j].edges.size() ; k++){
//				unsigned edgeId = vertices[j].edges[k]; 
//				if( j != edges[edgeId].vertices[0]){					//節点番号jとedges.vertices[0]が異なる節点番号の時:非対角成分		//OK
//					unsigned vtxid0 = edges[edgeId].vertices[0];
//					//DSTR << "TVecAll["<< vtxid0<<"] : " << TVecAll[vtxid0] <<std::endl;
//					//TVecAll[j] +=_dMatAll[j][0] * -(1.0/2.0 * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid0] + bVecAll[j][0]; 
//					//DSTR << "j : " << j << ", vtxid0 : " << vtxid0 <<", edges[edgeId].vertices[0] : " << edges[edgeId].vertices[0] <<  std::endl;
//					tempkj += (eps * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid0];
//				}
//				else if( j != edges[edgeId].vertices[1] ){			//節点番号jとedges.vertices[1]が異なる節点番号の時:非対角成分
//					unsigned vtxid1 = edges[edgeId].vertices[1];
//					//DSTR << "TVecAll["<< vtxid1<<"] : " << TVecAll[vtxid1] <<std::endl;
//					tempkj += (eps * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid1];
//				}
//				else{
//					//上記のどちらでもない場合、エラー
//					DSTR << "edges.vertex has 3 vertexies or any other problem" <<std::endl;
//				}
//			}
//			TVecAll[j] =	_dMatAll[0][j] * ( -1.0 * tempkj) + bVecAll[j][0];			//	-b = D^(-1) [ (-1/2 * K + 1/dt * C ){T(t+dt)} + {F} ]なので、bVecAllはただの加算でよい
//		}
//		int piyopiyoyo =0;
//	}
//}


void PHFemThermo::CalcHeatTransUsingGaussSeidel(unsigned NofCyc,double dt){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//dt = 0.0000000000001 * dt;		//デバッグ用に、dtをものすごく小さくしても、節点0がマイナスになるのか、調べた
	
	//dtはPHFemEngine.cppで取得する動力学シミュレーションのステップ時間
	bool DoCalc =true;											//初回だけ定数ベクトルbの計算を行うbool		//NofCycが0の時にすればいいのかも
	//std::ofstream ofs("log.txt");
	for(unsigned i=0; i < NofCyc; i++){							//ガウスザイデルの計算ループ
		if(DoCalc){		
			if(deformed || alphaUpdated){												//D_iiの作成　形状や熱伝達率が更新された際に1度だけ行えばよい
				for(unsigned j =0; j < mesh->vertices.size() ; j++){
					//for(unsigned k =0;k < vertices.size(); k++){
					//	DSTR << "dMatCAll "<< k << " : " << dMatCAll[0][k] << std::endl;
					//}
					_dMatAll.resize(1,mesh->vertices.size());
					_dMatAll[0][j] = 1.0/ ( 1.0/2.0 * dMatKAll[0][j] + 1.0/dt * dMatCAll[0][j] );		//1 / D__ii	を求める
					//1.0/dt = 500 d
					//DSTR << "dMatKAll : "  << dMatKAll << std::endl;
					//DSTR << "dMatCAll : "  << dMatCAll << std::endl;
					//DSTR << "1.0/dt : " << 1.0/dt <<std::endl;
					//DSTR <<  1.0/dt *dMatCAll[0][j] << std::endl;		//0.001のオーダー
					//DSTR << 1.0/2.0 * dMatKAll[0][j] << std::endl;		//0.0003前後のオーダー
					//値が入っているかをチェック
					//DSTR << "_dMatAll[0][" << j << "] : " << _dMatAll[0][j]  << std::endl;
					int debughogeshi =0;
				}
				deformed = false;
				alphaUpdated =false;
				//	for DEBUG
				//DSTR << "_dMatAll : " << std::endl;
				//for(unsigned j =0; j < vertices.size() ;j++){
				//	DSTR << j << " : " << _dMatAll[0][j] << std::endl;
				//}
				//int hogeshi=0;
			}
			//	 1      1        1  
			//	--- ( - - [K] + ---[C] ){T(t)} + {F} 
			//	D_jj    2       ⊿t
			//

			for(unsigned j =0; j < mesh->vertices.size() ; j++){		//初回ループだけ	係数ベクトルbVecAllの成分を計算
				bVecAll[j][0] = 0.0;							//bVecAll[j][0]の初期化
				bVecAll_IH[j][0] = 0.0;
				//節点が属すedges毎に　対角成分(j,j)と非対角成分(j,?)毎に計算
				//対角成分は、vertices[j].k or .c に入っている値を、非対角成分はedges[hoge].vertices[0] or vertices[1] .k or .cに入っている値を用いる
				//ⅰ)非対角成分について
				for(unsigned k =0;k < mesh->vertices[j].edgeIDs.size() ; k++){
					unsigned edgeId = mesh->vertices[j].edgeIDs[k];
					//リファクタリング	以下の条件分岐についてj>edges[edgeId].vertices[0] とそうでない時とで分けたほうが漏れが出る心配はない？
					if( j != mesh->edges[edgeId].vertexIDs[0]){					//節点番号jとedges.vertices[0]が異なる節点番号の時:非対角成分
						unsigned vtxid0 = mesh->edges[edgeId].vertexIDs[0];
						bVecAll[j][0] += (-1.0/2.0 * edgeVars[edgeId].k + 1.0/dt * edgeVars[edgeId].c ) * TVecAll[vtxid0];
					}
					else if( j != mesh->edges[edgeId].vertexIDs[1] ){			//節点番号jとedges.vertices[1]が異なる節点番号の時:非対角成分
						unsigned vtxid1 = mesh->edges[edgeId].vertexIDs[1];
						bVecAll[j][0] += (-1.0/2.0 * edgeVars[edgeId].k + 1.0/dt * edgeVars[edgeId].c ) * TVecAll[vtxid1];

					}
					else{
						//上記のどちらでもない場合、エラー
						DSTR << "edges.vertex has 3 vertexies or any other problem" <<std::endl;
					}
					//	for Debug
					//DSTR << "edges[" << edgeId << "].vertices[0] : " << edges[edgeId].vertices[0] << std::endl;
					//DSTR << "edges[" << edgeId << "].vertices[1] : " << edges[edgeId].vertices[1] << std::endl;
					//int hogeshi =0;
				}
				//ⅱ)対角成分について
				bVecAll[j][0] += (-1.0/2.0 * dMatKAll[0][j] + 1.0/dt * dMatCAll[0][j] ) * TVecAll[j];
				//ofs << "bVecAll[" << j <<"][0] : " << bVecAll[j][0] << std::endl;			// DSTR
				//{F}を加算
				bVecAll[j][0] += vecFAllSum[j];		//Fを加算
				//DSTR << " vecFAllSum[" << j << "] : "  << vecFAllSum[j] << std::endl;
				//DSTR << std::endl;
				//D_iiで割る ⇒この場所は、ここで良いの？どこまで掛け算するの？
				bVecAll[j][0] = bVecAll[j][0] * _dMatAll[0][j];
				//ofs << "bVecAll[" << j <<"][0] * _dMatAll : " << bVecAll[j][0] << std::endl;
				//	DSTR <<  "bVecAll[" << j <<"][0] * _dMatAll : " << bVecAll[j][0] << std::endl;
				//ofs << "TVecAll[" << j <<"] : " << TVecAll[j] << std::endl;
				//	DSTR << "TVecAll[" << j <<"] : " << TVecAll[j] << std::endl;
			}
			DoCalc = false;			//初回のループだけで利用
			//値が入っているか、正常そうかをチェック
			//DSTR << "bVecAll[j][0] : " << std::endl;
			//for(unsigned j =0;j <vertices.size() ; j++){
			//	DSTR << j << " : "<< bVecAll[j][0] << std::endl;
			//}
			int debughogeshi =0;
		}		//if(DoCalc){...}

#ifdef DEBUG
		//	念のため、計算前の初期温度を0にしている。
		if(i == 0){
				for(unsigned j=0;j <vertices.size() ;j++){
					TVecAll[j] = 0.0;
				}
		}
#endif
		//	 1      
		//	--- [F]{T(t+dt)}
		//	D_jj 		
		//[F] = 1/2 [K] +1/dt [C] から対角成分を除し(-1)をかけたもの
		//エッジに入っている成分に-1をかけるのではなく、最後に-1をかける。
		//
		for(unsigned j =0; j < mesh->vertices.size() ; j++){
			//T(t+dt) = の式
			//	まずtempkjを作る
			double tempkj = 0.0;			//ガウスザイデルの途中計算で出てくるFの成分計算に使用する一時変数
			for(unsigned k =0;k < mesh->vertices[j].edgeIDs.size() ; k++){
				unsigned edgeId = mesh->vertices[j].edgeIDs[k]; 
				if( j != mesh->edges[edgeId].vertexIDs[0]){					//節点番号jとedges.vertices[0]が異なる節点番号の時:非対角成分		//OK
					unsigned vtxid0 = mesh->edges[edgeId].vertexIDs[0];
					//DSTR << "TVecAll["<< vtxid0<<"] : " << TVecAll[vtxid0] <<std::endl;
					//TVecAll[j] +=_dMatAll[j][0] * -(1.0/2.0 * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid0] + bVecAll[j][0]; 
					//DSTR << "j : " << j << ", vtxid0 : " << vtxid0 <<", edges[edgeId].vertices[0] : " << edges[edgeId].vertices[0] <<  std::endl;
					tempkj += (1.0/2.0 * edgeVars[edgeId].k + 1.0/dt * edgeVars[edgeId].c ) * TVecAll[vtxid0];
				}
				else if( j != mesh->edges[edgeId].vertexIDs[1] ){			//節点番号jとedges.vertices[1]が異なる節点番号の時:非対角成分
					unsigned vtxid1 = mesh->edges[edgeId].vertexIDs[1];
					//DSTR << "TVecAll["<< vtxid1<<"] : " << TVecAll[vtxid1] <<std::endl;
					tempkj += (1.0/2.0 * edgeVars[edgeId].k + 1.0/dt * edgeVars[edgeId].c ) * TVecAll[vtxid1];
				}
				else{
					//上記のどちらでもない場合、エラー
					DSTR << "edges.vertex has 3 vertexies or any other problem" <<std::endl;
				}
				//	for Debug
				//DSTR << "TVecAll:"
				//DSTR << "edges[" << edgeId << "].vertices[0] : " << edges[edgeId].vertices[0] << std::endl;
				//DSTR << "edges[" << edgeId << "].vertices[1] : " << edges[edgeId].vertices[1] << std::endl;
				//int hogeshi =0;
			}
			//	TVecAllの計算
			TVecAll[j] =	_dMatAll[0][j] * ( -1.0 * tempkj) + bVecAll[j][0];			//	-b = D^(-1) [ (-1/2 * K + 1/dt * C ){T(t+dt)} + {F} ]なので、bVecAllはただの加算でよい

			////	for DEBUG
			//int hofgeshi =0;
			//if(TVecAll[j] != 0.0){
			//	DSTR << "!=0 TVecAll["<< j<<"] : " << TVecAll[j] <<std::endl;
			//}
			//DSTR << i << "回目の計算、" << j <<"行目のtempkj: " << tempkj << std::endl;
			//tempkj =0.0;

			//ofs << j << std::endl;
			//ofs << "tempkj: "<< tempkj << std::endl;
			//ofs << "DMatAll[0][j] * ( -1.0 * tempkj) :" <<_dMatAll[0][j] * ( -1.0 * tempkj) << std::endl;
			//ofs << "bVecAll[j][0] :  " <<  bVecAll[j][0] << std::endl;
			//ofs << "  TVecAll[j] : " << TVecAll[j] << std::endl;
			//ofs << std::endl;
		}
		////	for Debug
		//for(unsigned j=0;j < vertices.size();j++){
		//	//DSTR << "tempk" << j << " : " << tempkj << std::endl;
		//	int hogeshi__ =0;
		//	//TVecAll[j]の計算結果を代入する
		//	//定数ベクトルbを上で計算、毎行でbVecAllを減算すればよい。
		//	DSTR << i << "回目の計算の " << "TVecAll[" << j << "] : " << TVecAll[j] << std::endl;
		//}
		//for(unsigned j=0;j < vertices.size();j++){
		//	//DSTR << "tempk" << j << " : " << tempkj << std::endl;
		//	int hogeshi__ =0;
		//	//TVecAll[j]の計算結果を代入する
		//	//定数ベクトルbを上で計算、毎行でbVecAllを減算すればよい。
		//	DSTR << i << "回目の計算の " << "bVecAll[" << j << "][0] : " << bVecAll[j][0] << std::endl;
		//}

		//DSTR << i <<  "th Cyc" << std::endl; 
		//DSTR << i << "回目の計算、TVecAll : " <<std::endl;
		//DSTR << TVecAll << std::endl;
		//ofs << i <<  "th Cyc" << std::endl;
		//ofs << i << "回目の計算、TVecAll : " <<std::endl;
		//ofs << TVecAll << std::endl;
		//ofs << "bVecAll: " <<std::endl;
		//ofs << bVecAll << std::endl;
		//ofs << "_dMatAll: " <<std::endl; 
		//ofs << _dMatAll <<std::endl;
		int piyopiyoyo =0;
		//double tempTemp=0.0;
		//for(unsigned j=0;j <vertices.size() ; j++){
		//	tempTemp += TVecAll[j];
		//}
		//	DSTR
		//ofs << i <<"回目の計算時の　全節点の温度の和 : " << tempTemp << std::endl;
		//ofs << std::endl;
	}
//	deformed = true;
}

void PHFemThermo::CalcHeatTransUsingGaussSeidel(unsigned NofCyc,double dt,double eps){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//for(unsigned i=0; i < vertices.size(); i++){
	//	FEMLOG << vertices[i].k <<","<< vertices[i].c << std::endl;
	//}
	//FEMLOG << vecFAllSum << std::endl;
	//dt = 0.0000000000001 * dt;		//デバッグ用に、dtをものすごく小さくしても、節点0がマイナスになるのか、調べた
	double _eps = 1-eps;			// 1-epsの計算に利用
	//dtはPHFemEngine.cppで取得する動力学シミュレーションのステップ時間
	bool DoCalc =true;											//初回だけ定数ベクトルbの計算を行うbool		//NofCycが0の時にすればいいのかも
	//FEMLOG( std::ofstream ofs("log.txt") ) ;
	for(unsigned i=0; i < NofCyc; i++){							//ガウスザイデルの計算ループ
		convTest = TVecAll;
		if(DoCalc){												
			if(deformed || alphaUpdated ){												//D_iiの作成　形状が更新された際に1度だけ行えばよい
				for(unsigned j =0; j < mesh->vertices.size() ; j++){
					//for(unsigned k =0;k < vertices.size(); k++){
					//	DSTR << "dMatCAll "<< k << " : " << dMatCAll[0][k] << std::endl;
					//}
					_dMatAll.resize(1,mesh->vertices.size());
					_dMatAll[0][j] = 1.0/ ( eps * dMatKAll[0][j] + 1.0/dt * dMatCAll[0][j] );		//1 / D__ii	を求める
					//1.0/dt = 500 d
					//DSTR << "dMatKAll : "  << dMatKAll << std::endl;
					//DSTR << "dMatCAll : "  << dMatCAll << std::endl;
					//DSTR << "1.0/dt : " << 1.0/dt <<std::endl;
					//DSTR <<  1.0/dt *dMatCAll[0][j] << std::endl;		//0.001のオーダー
					//DSTR << 1.0/2.0 * dMatKAll[0][j] << std::endl;		//0.0003前後のオーダー
					//値が入っているかをチェック
					//DSTR << "_dMatAll[0][" << j << "] : " << _dMatAll[0][j]  << std::endl;
					int debughogeshi =0;
				}
				deformed = false;
				//	for DEBUG
				//DSTR << "_dMatAll : " << std::endl;
				//for(unsigned j =0; j < vertices.size() ;j++){
				//	DSTR << j << " : " << _dMatAll[0][j] << std::endl;
				//}
				//int hogeshi=0;
			}
			//	 1      1        1  
			//	--- ( - - [K] + ---[C] ){T(t)} + {F} 
			//	D_jj    2       ⊿t
			//

			for(unsigned j =0; j < mesh->vertices.size() ; j++){		//初回ループだけ	係数ベクトルbVecAllの成分を計算
				bVecAll[j][0] = 0.0;							//bVecAll[j][0]の初期化
				//節点が属すedges毎に　対角成分(j,j)と非対角成分(j,?)毎に計算
				//対角成分は、vertices[j].k or .c に入っている値を、非対角成分はedges[hoge].vertices[0] or vertices[1] .k or .cに入っている値を用いる
				//ⅰ)非対角成分について
				for(unsigned k =0;k < mesh->vertices[j].edgeIDs.size() ; k++){
					unsigned edgeId = mesh->vertices[j].edgeIDs[k];
					//リファクタリング	以下の条件分岐についてj>edges[edgeId].vertices[0] とそうでない時とで分けたほうが漏れが出る心配はない？
					if( j != mesh->edges[edgeId].vertexIDs[0]){					//節点番号jとedges.vertices[0]が異なる節点番号の時:非対角成分
						unsigned vtxid0 = mesh->edges[edgeId].vertexIDs[0];
						bVecAll[j][0] += (-_eps * edgeVars[edgeId].k + 1.0/dt * edgeVars[edgeId].c ) * TVecAll[vtxid0];
					}
					else if( j != mesh->edges[edgeId].vertexIDs[1] ){			//節点番号jとedges.vertices[1]が異なる節点番号の時:非対角成分
						unsigned vtxid1 = mesh->edges[edgeId].vertexIDs[1];
						bVecAll[j][0] += (-_eps * edgeVars[edgeId].k + 1.0/dt * edgeVars[edgeId].c ) * TVecAll[vtxid1];

					}
					else{
						//上記のどちらでもない場合、エラー
						DSTR << "edges.vertex has 3 vertexies or any other problem" <<std::endl;
					}
					//	for Debug
					//DSTR << "edges[" << edgeId << "].vertices[0] : " << edges[edgeId].vertices[0] << std::endl;
					//DSTR << "edges[" << edgeId << "].vertices[1] : " << edges[edgeId].vertices[1] << std::endl;
					//int hogeshi =0;
				}
				//ⅱ)対角成分について
				bVecAll[j][0] += (-_eps * dMatKAll[0][j] + 1.0/dt * dMatCAll[0][j] ) * TVecAll[j];

				//  {F}を加算
				bVecAll[j][0] += vecFAllSum[j];		//Fを加算

				//D_iiで割る ⇒この場所は、ここで良いの？どこまで掛け算するの？
				bVecAll[j][0] = bVecAll[j][0] * _dMatAll[0][j];
				//FEMLOG(ofs << "bVecAll[" << j <<"][0] * _dMatAll : " << bVecAll[j][0] << std::endl);
				//FEMLOG(ofs << "TVecAll[" << j <<"] : " << TVecAll[j] << std::endl);
			}
			DoCalc = false;			//初回のループだけで利用
			//値が入っているか、正常そうかをチェック
			//DSTR << "bVecAll[j][0] : " << std::endl;
			//for(unsigned j =0;j <vertices.size() ; j++){
			//	DSTR << j << " : "<< bVecAll[j][0] << std::endl;
			//}
			int debughogeshi =0;
		}		//if(DoCalc){...}

#ifdef DEBUG
		//	念のため、計算前の初期温度を0にしている。
		if(i == 0){
				for(unsigned j=0;j <vertices.size() ;j++){
					TVecAll[j] = 0.0;
				}
		}
#endif
		//	 1      
		//	--- [F]{T(t+dt)}
		//	D_jj 		
		//[F] = eps(ilon) [K] +1/dt [C] から対角成分を除し(-1)をかけたもの
		//エッジに入っている成分に-1をかけるのではなく、最後に-1をかける。
		//
		for(unsigned j =0; j < mesh->vertices.size() ; j++){
			//T(t+dt) = の式
			//	まずtempkjを作る
			double tempkj = 0.0;			//ガウスザイデルの途中計算で出てくるFの成分計算に使用する一時変数
			for(unsigned k =0;k < mesh->vertices[j].edgeIDs.size() ; k++){
				unsigned edgeId = mesh->vertices[j].edgeIDs[k]; 
				if( j != mesh->edges[edgeId].vertexIDs[0]){					//節点番号jとedges.vertices[0]が異なる節点番号の時:非対角成分		//OK
					unsigned vtxid0 = mesh->edges[edgeId].vertexIDs[0];
					//DSTR << "TVecAll["<< vtxid0<<"] : " << TVecAll[vtxid0] <<std::endl;
					//TVecAll[j] +=_dMatAll[j][0] * -(1.0/2.0 * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid0] + bVecAll[j][0]; 
					//DSTR << "j : " << j << ", vtxid0 : " << vtxid0 <<", edges[edgeId].vertices[0] : " << edges[edgeId].vertices[0] <<  std::endl;
					tempkj += (eps * edgeVars[edgeId].k + 1.0/dt * edgeVars[edgeId].c ) * TVecAll[vtxid0];
				}
				else if( j != mesh->edges[edgeId].vertexIDs[1] ){			//節点番号jとedges.vertices[1]が異なる節点番号の時:非対角成分
					unsigned vtxid1 = mesh->edges[edgeId].vertexIDs[1];
					//DSTR << "TVecAll["<< vtxid1<<"] : " << TVecAll[vtxid1] <<std::endl;
					tempkj += (eps * edgeVars[edgeId].k + 1.0/dt * edgeVars[edgeId].c ) * TVecAll[vtxid1];
				}
				else{
					//上記のどちらでもない場合、エラー
					DSTR << "edges.vertex has 3 vertexies or any other problem" <<std::endl;
				}
				//	for Debug
				//DSTR << "TVecAll:"
				//DSTR << "edges[" << edgeId << "].vertices[0] : " << edges[edgeId].vertices[0] << std::endl;
				//DSTR << "edges[" << edgeId << "].vertices[1] : " << edges[edgeId].vertices[1] << std::endl;
				//int hogeshi =0;
			}
			//	TVecAllの計算
			TVecAll[j] =	_dMatAll[0][j] * ( -1.0 * tempkj) + bVecAll[j][0];			//	-b = D^(-1) [ (-1/2 * K + 1/dt * C ){T(t+dt)} + {F} ]なので、bVecAllはただの加算でよい
			//TVecAll[j] =	_dMatAll[0][j] * ( -1.0 * tempkj) + bVecAll[j][0];   // -b = D^(-1) [ (-1/2 * K + 1/dt * C ){T(t+dt)} + {F} ]なので、bVecAllはただの加算でよい
//			TVecAll[j] =	_dMatAll[0][j] * ( -1.0 * tempkj) + bVecAll[j][0];			//この計算式だと、まともそうな値が出るが・・・理論的にはどうなのか、分からない。。。
			////	for DEBUG
			//int hofgeshi =0;
			//if(TVecAll[j] != 0.0){
			//	DSTR << "!=0 TVecAll["<< j<<"] : " << TVecAll[j] <<std::endl;
			//}
			//DSTR << i << "回目の計算、" << j <<"行目のtempkj: " << tempkj << std::endl;
			//tempkj =0.0;

			//FEMLOG(ofs << j << std::endl);
			//FEMLOG(ofs << "tempkj: "<< tempkj << std::endl);
			//FEMLOG(ofs << "DMatAll[0][j] * ( -1.0 * tempkj) :" <<_dMatAll[0][j] * ( -1.0 * tempkj) << std::endl;)
			//FEMLOG(ofs << "bVecAll[j][0] :  " <<  bVecAll[j][0] << std::endl;)
			//FEMLOG(ofs << "  TVecAll[j] : " << TVecAll[j] << std::endl;)
			//FEMLOG(ofs << std::endl;)
		}
		////	for Debug
		//for(unsigned j=0;j < vertices.size();j++){
		//	//DSTR << "tempk" << j << " : " << tempkj << std::endl;
		//	int hogeshi__ =0;
		//	//TVecAll[j]の計算結果を代入する
		//	//定数ベクトルbを上で計算、毎行でbVecAllを減算すればよい。
		//	DSTR << i << "回目の計算の " << "TVecAll[" << j << "] : " << TVecAll[j] << std::endl;
		//}
		//for(unsigned j=0;j < vertices.size();j++){
		//	//DSTR << "tempk" << j << " : " << tempkj << std::endl;
		//	int hogeshi__ =0;
		//	//TVecAll[j]の計算結果を代入する
		//	//定数ベクトルbを上で計算、毎行でbVecAllを減算すればよい。
		//	DSTR << i << "回目の計算の " << "bVecAll[" << j << "][0] : " << bVecAll[j][0] << std::endl;
		//}

		//DSTR << i <<  "th Cyc" << std::endl; 
		//DSTR << i << "回目の計算、TVecAll : " <<std::endl;
		//DSTR << TVecAll << std::endl;

		//FEMLOG(ofs << i <<  "th Cyc" << std::endl;)
		//FEMLOG(ofs << i << "回目の計算、TVecAll : " <<std::endl;)
		//FEMLOG(ofs << TVecAll << std::endl;)
		//FEMLOG(ofs << "bVecAll: " <<std::endl;)
		//FEMLOG(ofs << bVecAll << std::endl;)
		//FEMLOG(ofs << "_dMatAll: " <<std::endl;) 
		//FEMLOG(ofs << _dMatAll <<std::endl;)
		int piyopiyoyo =0;
		//double tempTemp=0.0;
		//for(unsigned j=0;j <vertices.size() ; j++){
		//	tempTemp += TVecAll[j];
		//}
		//	DSTR
		//FEMLOG(ofs << i <<"回目の計算時の　全節点の温度の和 : " << tempTemp << std::endl;)
		//FEMLOG(ofs << std::endl;)
		if(strcmp(mesh->GetName(), "femNsteak") == 0){
			for(unsigned vtxid=0; vtxid < mesh->vertices.size(); vtxid++){
				ggConvOut << pow(TVecAll[vtxid]-convTest[vtxid],2) << ",";
			}
			ggConvOut << std::endl;
		}
	}
	if(strcmp(mesh->GetName(), "femNsteak") == 0){
		ggConvOut.close();
	}
//	deformed = true;
}

void PHFemThermo::UpdateVertexTempAll(){
	for(unsigned i=0;i < GetPHFemMesh()->vertices.size();i++){
		vertexVars[i].temp = TVecAll[i];
	}
}
void PHFemThermo::UpdateVertexTemp(unsigned vtxid){
		vertexVars[vtxid].temp = TVecAll[vtxid];
}

void PHFemThermo::Step(){
	Step(tdt/*10.0*/);
}

void PHFemThermo::Step(double dt){
	PHFemMeshNew* mesh = GetPHFemMesh();


	//// cps表示用
	//static bool bOneSecond = false;
	//{
	//	static DWORD lastTick = GetTickCount();			//DWORD:unsigned long のこと
	//	static int cpsCount = 0;
	//	int ellapsed = GetTickCount() - lastTick;
	//	++cpsCount;
	//	bOneSecond = false;
	//	if (ellapsed > 1000) {
	//		std::cout << "cps : " << cpsCount << std::endl;
	//		cpslog << StepCount<< "," << cpsCount << "," ;	
	//		lastTick = GetTickCount();
	//		cpsCount = 0;
	//		bOneSecond = true;
	//	}
	//	if(cpsCount){	cpstime	= 1 / cpsCount;		}
	//}
	//static DWORD stepStart = GetTickCount();
	////途中時間
	//if (bOneSecond) {
	//	std::cout << "1: " << GetTickCount() - stepStart << std::endl;
	//	cpslog << GetTickCount() - stepStart << ",";
	//} 
	//stepStart = GetTickCount();
	//途中時間
	//if (bOneSecond) { std::cout << "1: " << GetTickCount() - stepStart << std::endl; }
	// %%% CPS表示

	//ScilabTest();									//	Scilabを使うテスト
	//境界条件を設定:温度の設定
	//UsingFixedTempBoundaryCondition(0,200.0);

	//	実験用のコード
	//	a) 時間離散化時に台形公式利用、前進・後退差分で振動する加熱方式
	//if(StepCount == 0){
	//	unsigned texid_ =7;
	//	UsingFixedTempBoundaryCondition(texid_,200.0);
	//}

	//	b) 断熱過程の実験
	//	熱伝達率を0にする。温度固定境界条件で加熱。

	//	UsingFixedTempBoundaryCondition(3,50.0);
	//for(unsigned i=0 ;i<1;i++){
	//	UsingFixedTempBoundaryCondition(i,200.0);
	//}

	//%%%%		熱伝達境界条件		%%%%//
	//	食材メッシュの表面の節点に、周囲の流体温度を与える
	//	周囲の流体温度は、フライパンの表面温度や、食材のUsingFixedTempBoundaryCondition(0,200.0);液体内の温度の分布から、その場所での周囲流体温度を判別する。
	//	位置座標から判別するコードをここに記述
	//UsingHeatTransferBoundaryCondition(unsigned id,double temp);
	//エネルギー保存則より、周囲流体温度の低下や、流体への供給熱量は制限されるべき

	//dt = dt *0.01;		誤差1度程度になる
	//dt = dt;				収束した時の、計算誤差？（マイナスになっている節点温度がそれなりに大きくなる。）
	
	///>	ガウスザイデル法の設定
	//	CalcHeatTransUsingGaussSeidel(20,dt);			//ガウスザイデル法で熱伝導計算を解く　クランクニコルソン法のみを使いたい場合

//	dNdt = 10.0 * dt;

#if 0
	// 解く前にかならず行う
	UpdateVecFAll_frypan(WEEK);				// 引数に加熱強さを与える。(OFF/WEEK/MIDDLE/HIGH)
#endif

	//{F}を使わずに、熱伝導計算実験をするためのテスト
#if 1
	//checkTVecAllout << COUNT * dt << ", "; 
	//for(unsigned i=0;i<mesh->vertices.size();i++){
	//	if(i != mesh->vertices.size() -1){
	//		checkTVecAllout << TVecAll[i] <<", ";
	//	}else{
	//		checkTVecAllout <<  TVecAll[i] << std::endl;
	//	}

	//}
	COUNT +=1;

	if(COUNT * dt == 1 * dt){
		if(tempe.size()){
			DSTR << "初期温度表示：" << std::endl;
			for(unsigned i=0;i<tempe.size();i++){
				Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.005);
				DSTR << i << ":" << GetVtxTempInTets(hyoukaPos) << std::endl;
			}
		}
	}
#ifdef HYOKA
	//	評価実験用
	if(COUNT * dt >= stopTime -0.02  ){
		checkTVecAllout.close();
		DSTR << "STOP:" <<COUNT * dt<<"Sec passed" << std::endl;
		std::cout << "stopTime are set as" << stopTime <<"[sec]" << std::endl;
		//	LMSRを求める
		double LMS[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		std::vector<double> hyouka;
		hyouka.clear();
		//%%%		放熱
		//200
		//hyouka.push_back(207.7);
		//hyouka.push_back(207.5);
		//hyouka.push_back(206.3);
		//hyouka.push_back(203.4);
		//hyouka.push_back(199.4);
		//hyouka.push_back(193.2);
		//hyouka.push_back(184.7);
		//hyouka.push_back(174.6);
		//hyouka.push_back(166.1);
		//hyouka.push_back(155.5);

		//150
		//hyouka.push_back(156.0);
		//hyouka.push_back(155.7);
		//hyouka.push_back(154.2);
		//hyouka.push_back(151.4);
		//hyouka.push_back(148.1);
		//hyouka.push_back(143.7);
		//hyouka.push_back(138.3);
		//hyouka.push_back(132.9);
		//hyouka.push_back(129.2);
		//hyouka.push_back(124.4);
		
		//100
		//hyouka.push_back(99.1);
		//hyouka.push_back(99.1);
		//hyouka.push_back(98.4);
		//hyouka.push_back(97.2);
		//hyouka.push_back(95.9);
		//hyouka.push_back(94.1);
		//hyouka.push_back(91.9);
		//hyouka.push_back(89.9);
		//hyouka.push_back(88.6);
		//hyouka.push_back(86.8);

		//	50.0 10sec
		//hyouka.push_back(49.9);
		//hyouka.push_back(49.9);
		//hyouka.push_back(49.8);
		//hyouka.push_back(49.7);
		//hyouka.push_back(49.5);

		//hyouka.push_back(49.1);
		//hyouka.push_back(48.6);
		//hyouka.push_back(48.3);
		//hyouka.push_back(48.3);
		//hyouka.push_back(47.9);

		//	50.0	更新タイミング 10sec
		//hyouka.push_back(49.9);
		//hyouka.push_back(49.8);
		//hyouka.push_back(49.7);
		//hyouka.push_back(49.6);
		//hyouka.push_back(49.5);

		//hyouka.push_back(49.1);
		//hyouka.push_back(48.6);
		//hyouka.push_back(48.3);
		//hyouka.push_back(48.2);
		//hyouka.push_back(47.8);

		//	50℃ 60sec
		//hyouka.push_back(49.3);
		//hyouka.push_back(49.3);
		//hyouka.push_back(49.3);
		//hyouka.push_back(49.1);
		//hyouka.push_back(48.9);

		//hyouka.push_back(48.6);
		//hyouka.push_back(48.1);
		//hyouka.push_back(47.9);
		//hyouka.push_back(47.8);
		//hyouka.push_back(47.5);

		//%%%	加熱同定　最初から10sec
		//hyouka.push_back(31.1);
		//hyouka.push_back(31.6);
		//hyouka.push_back(33.9);
		//hyouka.push_back(37.2);
		//hyouka.push_back(40.6);

		//hyouka.push_back(43.4);
		//hyouka.push_back(44.9);
		//hyouka.push_back(43.9);
		//hyouka.push_back(40.1);
		//hyouka.push_back(34.6);

		//%%%	加熱同定　最初から60sec
		hyouka.push_back(59.6);
		hyouka.push_back(61.6);
		hyouka.push_back(68.7);
		hyouka.push_back(79.3);
		hyouka.push_back(90.0);

		hyouka.push_back(97.5);
		hyouka.push_back(99.1);
		hyouka.push_back(92.0);
		hyouka.push_back(79.6);
		hyouka.push_back(62.1);

		//%%%	jの外側が低い温度に設定してあるので、jの値は信用出来ない。iも信用出来ないとして、-2
		for(unsigned i=0;i<tempe.size() - 9; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[0] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		for(unsigned i=0;i<tempe.size() - 8; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[1] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		for(unsigned i=0;i<tempe.size() - 7; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[2] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		for(unsigned i=0;i<tempe.size() - 6; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[3] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		for(unsigned i=0;i<tempe.size() - 5; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[4] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		for(unsigned i=0;i<tempe.size() - 4; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[5] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		for(unsigned i=0;i<tempe.size() - 3; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[6] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		for(unsigned i=0;i<tempe.size() - 2; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[7] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		for(unsigned i=0;i<tempe.size() - 1; i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[8] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
		}
		DSTR << "初期設定温度" << std::endl;
		for(unsigned i=0; i < tempe.size(); ++i){
			DSTR << tempe[i] << ",";
		}
		DSTR << std::endl;
		DSTR << "目標温度" << std::endl;
		for(unsigned i=0; i < hyouka.size(); ++i){
			DSTR << hyouka[i] << ",";
		}
		DSTR << std::endl;
		DSTR << "測定点の温度," << std::endl;
		for(unsigned i=0;i<tempe.size(); i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			LMS[9] += (GetVtxTempInTets(hyoukaPos) - hyouka[i] ) * (GetVtxTempInTets(hyoukaPos) - hyouka[i] ); 
			DSTR << GetVtxTempInTets(hyoukaPos) << ", ";
		}
		DSTR << std::endl;
		DSTR << "熱放射率　比例係数,切片" << "Tout," <<  "j以遠温度," << std::endl;
		DSTR << ems <<"," << ems_const <<"," << vertices[0].Tout <<"," << jout << std::endl;
		DSTR << vertices[0].Tc <<"," << vertices[0].Tout <<"," << vertices[0].heatTransRatio << "," << vertices[0].thermalEmissivity << std::endl;
		DSTR << "加熱出力," << "内半径," <<  "外半径," << std::endl;
		DSTR << weekPow_ <<  "," << inr_ << "," << outR_ << std::endl;
		DSTR << "追加出力," << "内半径," <<  "外半径," << std::endl;
		DSTR << weekPow_add <<  "," << inr_add << "," << outR_add << std::endl;
		DSTR << "出力打ち消し," << "内半径," <<  "外半径," << std::endl;
		DSTR << weekPow_decr <<  "," << inr_decr << "," << outR_decr << std::endl;

		for(unsigned i=0;i<10;++i){
			DSTR << "LMS" << i+1 <<":"  << LMS[i] <<std::endl;
		}
		DSTR << "LMS横書き" <<std::endl;
		for(unsigned i=0;i<10;++i){
			DSTR << LMS[i] <<",";
		}
		DSTR << std::endl;
		DSTR << "目標値との差分" << std::endl;
		for(unsigned i=0;i<tempe.size(); i++){
			Vec3d hyoukaPos =Vec3d(i*0.01, 0.0015, -0.001);
			DSTR << hyouka[i] - GetVtxTempInTets(hyoukaPos) << ", ";
		}
		DSTR << std::endl;
		int katoonLMS =0;
		DSTR<<"------------------"<<std::endl;

	}
#endif HYOKA
	if(COUNT * dt >= stopTime + 0.1){
		//assert(0);			//	止めたい

	}
#endif
	doCalc =true;
	
	if(doCalc){
	//ガウスザイデル法で解く
		if(strcmp(mesh->GetName(), "femNsteak") == 0){
			
			//vecFOutToFile();
			//vecFTopOutToFile();
			//vecFBottomOutToFile();

			CalcHeatTransUsingGaussSeidel(NofCyc,dt,epsilonG);			//ガウスザイデル法で熱伝導計算を解く 第三引数は、前進・クランクニコルソン・後退積分のいずれかを数値で選択
			//CalcHeatTransDirect(dt);
			//CalcHeatTransDirect3(dt);
			//CalcHeatTransDirect4(dt);
			//CalcHeatTransDirect2(dt);
			//CalcHeatTransUsingScilab(dt, epsilonG);

		}else{
			CalcHeatTransUsingGaussSeidel(NofCyc,dt,epsilonG);			//ガウスザイデル法で熱伝導計算を解く 第三引数は、前進・クランクニコルソン・後退積分のいずれかを数値で選択
			//CalcHeatTransDirect(dt);
			//CalcHeatTransDirect2(dt);
			//CalcHeatTransUsingScilab(dt, epsilonG);
		}
	}
	doCalc = false;

	if(mesh->GetPHFemPorousWOMove()){
		for(unsigned vtxid=0; vtxid < mesh->surfaceVertices.size(); vtxid++){
			VertexWaterBoiling(vtxid);
		}
	}
//	if(strcmp(mesh->GetName(), "femPan") == 0){
//		for(unsigned vtxid=0; vtxid < mesh->vertices.size(); vtxid++){
//			vecFOuPant << vecFAllSum[vtxid] << ",";
//		}
//		vecFOut << std::endl;
//	}
//	if(strcmp(mesh->GetName(), "femNsteak") == 0){
//		vecFOutSteak << COUNT * dt << ",";
//		for(unsigned vtxid=0; vtxid < mesh->vertices.size(); vtxid++){
//			vecFOutSteak << vecFAllSum[vtxid] << ",";
//		}
//		vecFOutSteak << std::endl;
//	}
	//温度のベクトルから節点へ温度の反映
	UpdateVertexTempAll();
	StepCount += 1;
	if(StepCount >= 1000*1000*1000){
		StepCount = 0;
		StepCount_ += 1;
	}

	/*小野原追加ここから--------------------------------------------*/
	//beCondVtxsとtoofarにFalseをいれておく。初期化
	for(unsigned i=0; i<mesh->surfaceVertices.size(); i++){
		vertexVars[mesh->surfaceVertices[i]].beCondVtxs = vertexVars[mesh->surfaceVertices[i]].toofar = false;
	}
	/*小野原追加ここまで--------------------------------------------*/

//#ifdef UseMatAll
//	if(strcmp(mesh->GetName(), "femNsteak") == 0){
//		//for(unsigned vtxid=0; vtxid < mesh->vertices.size(); vtxid++){
//		//	vecFOutSteak << vecFAllSum[vtxid] << ",";
//		//}
//		//vecFOutSteak << std::endl;
//		std::ofstream Kout;
//		std::ofstream Cout;
//		Kout.open("matKMeatSingleTet2.txt");
//		Cout.open("matCMeatSingleTet2.txt");
//
//		//dtKout << "[";
//		//Cout << "[";
//		for(unsigned i=0; i < mesh->vertices.size(); i++){
//			for(unsigned j=0; j < mesh->vertices.size(); j++){
//				Kout << /*tdt * */matKAll[i][j] << " ";
//				Cout << matCAll[i][j] << " ";
//			}
//			Kout/* << ";" */<< std::endl;
//			Cout/* << ";" */<< std::endl;
//		}
//		//dtKout << "]";
//		//Cout <<"]"; 
//
//		Kout.close();
//		Cout.close();
//	}
//#endif

//#ifdef UseMatAll
//	if(strcmp(mesh->GetName(), "femNsteak") == 0){
//		OutEig();
//	}
//#endif
}



//void PHFemThermo::CreateMatrix(){
//}

void PHFemThermo::InitTcAll(double temp){
	for(unsigned i =0; i < GetPHFemMesh()->vertices.size();i++){
		vertexVars[i].Tc = temp;
	}
}

void PHFemThermo::InitToutAll(double temp){
	for(unsigned i =0; i < GetPHFemMesh()->vertices.size();i++){
		vertexVars[i].Tout = temp;
		//DSTR << vertices[i].Tout << temp <<std::endl;
	}
}

void PHFemThermo::InitCreateMatC(){
	/// MatCについて	//	使用する行列の初期化
	//dMatCAll：対角行列の成分の入った行列のサイズを定義:配列として利用	幅:vertices.size(),高さ:1
	dMatCAll.resize(1,GetPHFemMesh()->vertices.size()); //(h,w)
	dMatCAll.clear();								///	値の初期化
	//matcの初期化は、matcを作る関数でやっているので、省略
	matCAll.resize(GetPHFemMesh()->vertices.size(),GetPHFemMesh()->vertices.size());
	matCAll.clear();
}

void PHFemThermo::InitCreateMatk_(){
	///	MatKについて
	//matkの初期化
	for(unsigned i =0; i < 4 ; i++){
		for(unsigned j =0; j < 4 ; j++){
			matk[i][j] = 0.0;
		}
	}
	dMatKAll.clear();		///	初期化
#ifdef UseMatAll
	//matKAll.resize(vertices.size(),vertices.size());	///	matKAllのサイズの代入
	matKAll.clear();									///	matKAllの初期化
	//DSTR << "matKAll: " << matKAll <<std::endl;
#endif UseMatAll

#ifdef DumK
	//matKAllの初期化
	matKAll.resize(vertices.size(),vertices.size());
	matkAll.clear();					///	初期化、下の初期化コードは不要
#endif


}

void PHFemThermo::InitCreateMatk(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	///	MatKについて
	//matkの初期化
	for(unsigned i =0; i < 4 ; i++){
		for(unsigned j =0; j < 4 ; j++){
			matk[i][j] = 0.0;
		}
	}
	//for(unsigned i=0;i<3;i++){
	//tets.matk[i]
	//}
	//DMatAll：対角行列の成分の入った行列のサイズを定義:配列として利用	幅:vertices.size(),高さ:1
	dMatKAll.resize(1,mesh->vertices.size()); //(h,w)
	dMatKAll.clear();		///	初期化
	////値の初期化
	//for(unsigned i=0;i<vertices.size();i++){
	//	dMatKAll[0][i] = 0.0;
	//}
#ifdef UseMatAll
	
	matKAll.resize(mesh->vertices.size(),mesh->vertices.size());	///	matKAllのサイズの代入
	matKAll.clear();									///	matKAllの初期化
	//for(unsigned i=0;i<vertices.size();i++){
	//	for(unsigned j=0;j<vertices.size();j++){
	//		matKAll[i][j] = 0.0;
	//	}
	//}
#endif UseMatAll

#ifdef DumK
	//matKAllの初期化
	matKAll.resize(vertices.size(),vertices.size());
	matkAll.clear();					///	初期化、下の初期化コードは不要
	//for(unsigned i=0;i<vertices.size();i++){
	//	for(unsigned j=0;j<vertices.size();j++){
	//		matKAll[i][j] = 0.0;
	//	}
	//}
#endif
}

void PHFemThermo::InitCreateVecf_(){
	///	Vecfについて
	//Vecfの初期化
	for(unsigned i =0; i < 4 ; i++){
			vecf[i] = 0.0;
	}
	vecFAllSum.clear();						///	初期化
}

void PHFemThermo::InitVecFAlls(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	for(unsigned i =0; i < 4 ; i++){ vecf[i] = 0.0;}	/// Vecfの初期化
	vecFAllSum.resize(mesh->vertices.size());					///	全体剛性ベクトルFのサイズを規定
	vecFAllSum.clear();									///		〃			の初期化

	for(unsigned i=0;i < HIGH +1 ; i++){				/// IH加熱モードの各ベクトルを初期化
		vecFAll_f2IH[i].resize(mesh->vertices.size(),1);
		vecFAll_f2IH[i].clear();
	}
	// tets.vecf[4]の初期化
	for(unsigned i=0;i<mesh->tets.size();i++){
		for(unsigned j=0; j <4;j++){
			tetVars[i].vecf[j].clear();
		}
	}
	// vecFAllの初期化
	//initialize
	for(unsigned i =0;i<4;i++){
		vecFAll[i].resize(mesh->vertices.size());
		vecFAll[i].clear();
	}
}

void PHFemThermo::UpdateVecF_frypan(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	// modeは必要か？

	//	Initialize
	//InitVecFAlls();		// この中の初期化のすべてが必要か？

//. 1)最初の2つ({F2},{F3})は、F2,F3のどちらかだけ更新すれば良い場合に用いる
#if 0
	// {F2}
	for(unsigned tetsid = 0; tetsid < mesh->tets.size();tetsid++){
		unsigned id = tetsid;
		CreateVecf2surface(id);			// tets[id].vecf[1];に結果格納
		for(unsigned j =0;j < 4; j++){
			int vtxid0 = mesh->tets[id].vertexIDs[j];
			//vecFAll[1][vtxid0] += vecf[j];
			vecFAll[1][vtxid0] += tetVars[id].vecf[1][j];
		}
	}	
#endif

#if 0
	// {F3}
	for(unsigned tetsid = 0; tetsid < mesh->tets.size();tetsid++){
		unsigned id = tetsid;
		CreateVecf3(id);		// tets[id].vecf[2];に結果格納
		//vecf = tets[id].vecf[2];
		for(unsigned j =0;j < 4; j++){
			int vtxid0 = mesh->tets[id].vertexIDs[j];
			//vecFAll[1][vtxid0] += vecf[j];
			vecFAll[2][vtxid0] += tetVars[id].vecf[2][j];
		}
	}	
#endif

//. 2) {F2,F3}の両方共更新して良い場合
#if 1
	for(unsigned tetsid = 0; tetsid < mesh->tets.size();tetsid++){
		unsigned id = tetsid;
		CreateVecf2surface(id);
		CreateVecf4surface(id);
#if 0
		CreateVecf3surface(id);
#endif
#ifndef NOTUSE_HEATTRANS_HERE
		CreateVecf3(id);		// tets[id].vecf[2];に結果格納
#endif
		//vecf = tets[id].vecf[2];
		for(unsigned j =0;j < 4; j++){
			int vtxid0 = mesh->tets[id].vertexIDs[j];
			//vecFAll[1][vtxid0] += vecf[j];
			vecFAll[1][vtxid0] += tetVars[id].vecf[1][j];
			vecFAll[3][vtxid0] -= tetVars[id].vecf[3][j];
			//DSTR << "vtxid0: " << vtxid0 <<", tets[id].vecf[1]["<< j << "]: " << tets[id].vecf[1][j] << std::endl; 
#ifndef NOTUSE_HEATTRANS_HERE
			vecFAll[2][vtxid0] += tets[id].vecf[2][j];
#endif
		}
	}
#endif

	//Σ{F[i]}_{i=1}^{4}
#ifndef NOTUSE_HEATTRANS_HERE
	vecFAllSum = vecFAll[1] + vecFAll[2];
#else

	vecFAllSum += vecFAll[1] + vecFAll[3];

	// VecFAll がweekpowで入れた総熱量と合致するかチェックコード
	//double tempSumVECF=0.0;
	//for(unsigned i=0;i<vertices.size();i++){
	//	tempSumVECF += vecFAllSum[i];
	//}
	//DSTR <<"vecFAllSum の Sum: " << tempSumVECF << std::endl;

#endif
	//DSTR << "vecFAll[1]: " << vecFAll[1] << std::endl;


//%%%% この関数はここまででとりあえず完成 2012.10.09

	//	消去予定
#if 0
//depend on mode, I don't need to use mode state.Because mode state cause different calc result of heatflus.
// I just use the result of IHdqdt Function.
	//vecFAll[1],[2]に代入
	for(unsigned id = 0; id < mesh->vertices.size();id++){
		if(mode == OFF){ 
			// F2は加算しない
			vecFAllSum[id] = vecFAllSum[id]; //F3
		}
		else if(mode == WEEK){
			vecFAllSum[id] =  vecFAll_f2IH[mode][id][0] + vecFAll_f3[id][0];//F2+F3		//mode=0 -> F2のWEEKの強さ
		}
		else if(mode == MIDDLE){
			vecFAllSum[id] = vecFAll_f2IH[mode][id][0];//F2+F3		//mode=1 -> F2のmiddleの強さ
		}
		else if(mode == HIGH){
			vecFAllSum[id] = vecFAll_f2IH[mode][id][0];//F2+F3		//mode=2 -> F2のhighの強さ
		}
	}

#endif

#if 0
	for(unsigned i =0; i< 4;i++){
		vecFAllSum += vecFAll[i];				//全体剛性行列の和を取る
	}
	// F2,F3を加算する
#endif
}


void PHFemThermo::UpdateVecF(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	// 引数にenum modeを入れてモード指定を行う仕様に変える


//. 1)最初の2つ({F2},{F3})は、F2,F3のどちらかだけ更新すれば良い場合に用いる
#if 0
	// {F2}
	for(unsigned tetsid = 0; tetsid < mesh->tets.size();tetsid++){
		unsigned id = tetsid;
		CreateVecf2surface(id);			// tets[id].vecf[1];に結果格納
		for(unsigned j =0;j < 4; j++){
			int vtxid0 = mesh->tets[id].vertexIDs[j];
			//vecFAll[1][vtxid0] += vecf[j];
			vecFAll[1][vtxid0] += tetVars[id].vecf[1][j];
		}
	}	
#endif

#if 0
	// {F3}
	for(unsigned tetsid = 0; tetsid < mesh->tets.size();tetsid++){
		unsigned id = tetsid;
		CreateVecf3(id);		// tets[id].vecf[2];に結果格納
		//vecf = tets[id].vecf[2];
		for(unsigned j =0;j < 4; j++){
			int vtxid0 = mesh->tets[id].vertexIDs[j];
			//vecFAll[1][vtxid0] += vecf[j];
			vecFAll[2][vtxid0] += tetVars[id].vecf[2][j];
		}
	}	
#endif

//. 2) {F2,F3}の両方共更新して良い場合
#if 1
	for(unsigned tetsid = 0; tetsid < mesh->tets.size();tetsid++){
		unsigned id = tetsid;
		CreateVecf2surface(id);
		CreateVecf4surface(id);
#if 0
		CreateVecf3surface(id);
#endif
#ifndef NOTUSE_HEATTRANS_HERE
		CreateVecf3(id);		// tets[id].vecf[2];に結果格納
#endif
		//vecf = tets[id].vecf[2];
		for(unsigned j =0;j < 4; j++){
			int vtxid0 = mesh->tets[id].vertexIDs[j];
			//vecFAll[1][vtxid0] += vecf[j];
			vecFAll[1][vtxid0] += tetVars[id].vecf[1][j];
			vecFAll[3][vtxid0] -= tetVars[id].vecf[3][j];
			//DSTR << "vtxid0: " << vtxid0 <<", tets[id].vecf[1]["<< j << "]: " << tets[id].vecf[1][j] << std::endl; 
#ifndef NOTUSE_HEATTRANS_HERE
			vecFAll[2][vtxid0] += tetVars[id].vecf[2][j];
#endif
		}
	}
#endif

	//Σ{F[i]}_{i=1}^{4}
#ifndef NOTUSE_HEATTRANS_HERE
	vecFAllSum = vecFAll[1] + vecFAll[2];
#else

	vecFAllSum += vecFAll[1] + vecFAll[3];

	// VecFAll がweekpowで入れた総熱量と合致するかチェックコード
	//double tempSumVECF=0.0;
	//for(unsigned i=0;i<vertices.size();i++){
	//	tempSumVECF += vecFAllSum[i];
	//}
	//DSTR <<"vecFAllSum の Sum: " << tempSumVECF << std::endl;

#endif
	//DSTR << "vecFAll[1]: " << vecFAll[1] << std::endl;


//%%%% この関数はここまででとりあえず完成 2012.10.09

	//	消去予定
#if 0
//depend on mode, I don't need to use mode state.Because mode state cause different calc result of heatflus.
// I just use the result of IHdqdt Function.
	//vecFAll[1],[2]に代入
	for(unsigned id = 0; id < vertices.size();id++){
		if(mode == OFF){ 
			// F2は加算しない
			vecFAllSum[id] = vecFAllSum[id]; //F3
		}
		else if(mode == WEEK){
			vecFAllSum[id] =  vecFAll_f2IH[mode][id][0] + vecFAll_f3[id][0];//F2+F3		//mode=0 -> F2のWEEKの強さ
		}
		else if(mode == MIDDLE){
			vecFAllSum[id] = vecFAll_f2IH[mode][id][0];//F2+F3		//mode=1 -> F2のmiddleの強さ
		}
		else if(mode == HIGH){
			vecFAllSum[id] = vecFAll_f2IH[mode][id][0];//F2+F3		//mode=2 -> F2のhighの強さ
		}
	}

#endif

#if 0
	for(unsigned i =0; i< 4;i++){
		vecFAllSum += vecFAll[i];				//全体剛性行列の和を取る
	}
	// F2,F3を加算する
#endif
}

//void PHFemThermo::UpdateMatK(){
//
//	//%%%	初期化類		%%%//
//	//各種メンバ変数の初期化⇒コンストラクタでできたほうがいいかもしれない。
//	///	Edges
//	for(unsigned i =0; i < edges.size();i++){
//		edges[i].c = 0.0;	
//		edges[i].k = 0.0;
//	}
//
//	///	faces
//	for(unsigned i=0;i<faces.size();i++){
//		faces[i].alphaUpdated = true;
//		faces[i].area = 0.0;
//		faces[i].heatTransRatio = 0.0;
//		faces[i].deformed = true;				//初期状態は、変形後とする
//		faces[i].fluxarea =0.0;
//		faces[i].thermalEmissivity =0.0;
//		//faces[i].heatflux.clear();				// 初期化
//		//faces[i].heatflux[hum]の領域確保：配列として、か、vectorとしてのpush_backか、どちらかを行う。配列ならここに記述。
//		for(unsigned mode =0; mode < HIGH +1 ; mode++){			// 加熱モードの数だけ、ベクトルを生成
//			faces[i].heatflux[mode] = 0.0;
//		}
//	}
//
//	//行列の成分数などを初期化
//	bVecAll.resize(vertices.size(),1);
//	TVecAll.resize(vertices.size());
//
//	//	メッシュ節点温度 初期化
//	SetVerticesTempAll(30.0);
//
//	//評価実験用コード
//	//>	LMSのためのコード
//	//	節点温度の初期設定(行列を作る前に行う)
//	//SetVerticesTempAll((158.5 + (30.0 + 158.5) /2.0)/2.0 );
//	//SetVerticesTempAll((158.5+30.0)/2.0);
//	
//	
//	// main.cppで動くように設定する
//	round.clear();
//	tempe.clear();
//	for(unsigned i=0;i<11;++i){
//		round.push_back(i*0.01);
//	}
//	//200
//	//tempe.push_back(211.3);
//	//tempe.push_back(211.2);
//	//tempe.push_back(210.2);
//	//tempe.push_back(207.6);
//	//tempe.push_back(203.9);
//
//	//tempe.push_back(198.0);
//	//tempe.push_back(189.3);
//	//tempe.push_back(178.7);
//	//tempe.push_back(169.8);
//	//tempe.push_back(158.5);
//	
//	// 150
//	//double tempe2[10] = {158.6, 158.3, 156.8, 153.9, 150.4, 146.0, 140.3, 134.8, 130.8, 125.7};
//	//tempe.push_back(158.6);
//	//tempe.push_back(158.3);
//	//tempe.push_back(156.8);
//	//tempe.push_back(153.9);
//	//tempe.push_back(150.4);
//
//	//tempe.push_back(146);
//	//tempe.push_back(140.3);
//	//tempe.push_back(134.8);
//	//tempe.push_back(130.8);
//	//tempe.push_back(125.7);
//
//	//50
//	tempe.push_back(50.0);
//	tempe.push_back(50.0);
//	tempe.push_back(49.9);
//	tempe.push_back(49.8);
//	tempe.push_back(49.7);
//
//	tempe.push_back(49.3);
//	tempe.push_back(48.7);
//	tempe.push_back(48.5);
//	tempe.push_back(48.4);
//	tempe.push_back(48.0);
//
//	SetConcentricHeatMap(round,tempe,Vec2d(0.0, -0.005));		//	-0.001にしても、初期温度は不変だった。
//	DSTR << "入力 check it out" <<std::endl;
//	for(unsigned i=0; i< tempe.size();++i){
//		DSTR << round[i] << "; "<< i*0.01 <<"; " <<  tempe[i] << std::endl;
//	}
//
//	InitTcAll(temp_c);
//	//>	熱放射関係	LMSS
//	InitToutAll(temp_out);
//	SetThermalEmissivityToVerticesAll(ems);	
//
//	//vertices.tempをすべて、TVecAllへ代入する
//	CreateTempVertex();
//	CalcVtxDisFromVertex(Vec2d(0.0, -0.005));		// 鉄板の原点座標を引数に入れ、対称性が合うようになっている。
//	InitCreateMatk();					///	CreateMatKの初期化
//	keisuInv.resize(vertices.size(),vertices.size());
//	keisuInv.clear();
//	///	熱伝達率を各節点に格納
//	SetHeatTransRatioToAllVertex();
//	for(unsigned i=0; i < this->tets.size(); i++){
//
//		/*小野原追加ここから--------------------------------------------*/
//		//表面faceの面積を計算
//		for(unsigned j= 0 ; j < 4; j++){
//			if(tets[i].faces[j] < (int)nSurfaceFace){			///	外殻の面
//				///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
//				faces[tets[i].faces[j]].area = CalcTriangleArea(faces[tets[i].faces[j]].vertices[0], faces[tets[i].faces[j]].vertices[1], faces[tets[i].faces[j]].vertices[2]);
//			}
//		}
//		/*小野原追加ここまで--------------------------------------------*/
//
//		tets[i].volume = CalcTetrahedraVolume2(i);
//
//		//各行列を作って、ガウスザイデルで計算するための係数の基本を作る。Timestepの入っている項は、このソース(SetDesc())では、実現できないことが分かった(NULLが返ってくる)
//		CreateMatkLocal(i);				///	Matk1 Matk2(更新が必要な場合がある)を作る	//ifdefスイッチで全体剛性行列も(表示用だが)生成可能
//	}
//	//頂点の表面面積を計算（小野原追加）
//	calcVerticesArea();
//
//	//カウントの初期化
//	COUNT = 0;
//
//
//}

void PHFemThermo::IfRadiantHeatTrans(){
	for(int id =0;id<NSurfaceVertices();id++){
		if(vertexVars[id].beRadiantHeat){
			vertexVars[id].thermalEmissivity = ems;
			vertexVars[id].thermalEmissivity_const = ems_const;
		}
		else{
			vertexVars[id].thermalEmissivity = 0.0;
			vertexVars[id].thermalEmissivity_const = 0.0;
		}	
	}
}

void PHFemThermo::UpdateMatk_RadiantHeatToAir(){
		for(unsigned i =0; i < GetPHFemMesh()->edges.size();i++){
			edgeVars[i].k = 0.0;
		}
#ifdef UseMatAll
		matKAll.clear();
#endif
		dMatKAll[0].clear(); 
		for(unsigned i=0;i<GetPHFemMesh()->tets.size();++i){
			CreateMatkLocal_update(i);
		}
#ifdef badMatK
		if(strcmp(GetPHFemMesh()->GetName(), "femNsteak") == 0){
			//if(matkupSwitch){
				CalcMatKup();
				//matkupSwitch = false;
			//}
			for(unsigned i=0; i < GetPHFemMesh()->vertices.size(); i++){
				dMatKAll[0][i] += matkupdate[i];
#ifdef UseMatAll
				matKAll[i][i] += matkupdate[i];
#endif
			}
		}
#endif
#ifdef UseMatAll
#ifdef outMatK
		if(strcmp(GetPHFemMesh()->GetName(), "femNsteak") == 0){
			OutputMatKall();
		//}
		matKAllout.open("matKAllout.csv");
		for(unsigned i=0; i<GetPHFemMesh()->vertices.size(); i++){
			for(unsigned j=0; j<GetPHFemMesh()->vertices.size(); j++){
				matKAllout << matKAll[i][j] << ",";
			}
			matKAllout << std::endl;
		}
		matKAllout.close();
		}//
#endif
#endif
}

void PHFemThermo::UpdateIHheat(unsigned heatingMODE){

	//方針：熱流束ベクトル計算のみを含める。（熱伝導や熱容量は共通なのでこの関数には含めない）

	//熱伝導率、密度、比熱、熱伝達率　のパラメーターを設定・代入
		//PHFemThermoのメンバ変数の値を代入 CADThermoより、0.574;//玉ねぎの値//熱伝導率[W/(ｍ・K)]　Cp = 1.96 * (Ndt);//玉ねぎの比熱[kJ/(kg・K) 1.96kJ/(kg K),（玉ねぎの密度）食品加熱の科学p64より970kg/m^3
		//熱伝達率の単位系　W/(m^2 K)⇒これはSI単位系なのか？　25は論文(MEAT COOKING SIMULATION BY FINITE ELEMENTS)のオーブン加熱時の実測値
		//SetInitThermoConductionParam(0.574,970,1.96,25);
	//SetInitThermoConductionParam(0.574,970,0.1960,25 * 0.01);		//> thConduct:熱伝導率 ,roh:密度,	specificHeat:比熱 J/ (K・kg):1960 ,　heatTrans:熱伝達率 W/(m^2・K)
		//これら、変数値は後から計算の途中で変更できるようなSetParam()関数を作っておいたほうがいいかな？

	//.		熱流束の設定
	//..	初期化
	//SetVtxHeatFluxAll(0.0);

	//	CalcIHdqdtの前に必要
	//faces[i].fluxarea = 0.0;
	

	//1.フライパン位置を取ってくる
		//ih加熱円環中心からの同心円状加熱領域を計算し、ihdqdtに当てはめるメッシュ情報を生成
		//　if(フライパンが動いたか)	動いていなければ、vecfも、1step前の値を使えるようにしておきたい。

	

	//2...	face面での熱流束量を計算（フライパン位置又はポインタを引数に代入：毎回フライパンの位置が変化するので、フライパン位置の変化の度に生成する）
	if(heatingMODE == OFF){
		CalcIHdqdt_atleast(0.0,0.0,0.0, OFF);		//	IH加熱行列の係数0となるため、計算されない
	}
	else if(heatingMODE == WEEK){	
#ifdef TempDependHeat
		CalcIHdqdt_atleast_high(inr_,outR_,weekPow_, WEEK);
		CalcIHdqdt_add_high(inr_add,outR_add,weekPow_add, WEEK);
		CalcIHdqdt_decrease_high(inr_decr,outR_decr,weekPow_decr, WEEK);
#else
	#ifdef DISABLE_COIL
		//CalcIHdqdt_atleast_map(Vec2d(0.0, -0.005),weekPow_FULL,WEEK);
		CalcIHdqdt_atleast_map(Vec2d(0.0425, -0.0425),weekPow_FULL,WEEK);
	#else
		CalcIHdqdt_atleast(inr_,outR_,weekPow_, WEEK);		//	API化済み
		CalcIHdqdt_add(inr_add,outR_add,weekPow_add, WEEK);
		CalcIHdqdt_decrease(inr_decr,outR_decr,weekPow_decr, WEEK);
	#endif

	//>個別に呼ぶ仕様に変更	//UpdateMatk_RadiantHeatToAir();				//	熱伝達境界条件で空気への熱伝達項だけ更新する
#endif
	}	
	else if(heatingMODE == MIDDLE){
		//CalcIHdqdt_atleast(inr_,outR_,231.9 * 0.005 * 1e4, MIDDLE);		//
		CalcIHdqdt_atleast(0.11,0.14,231.9 * 0.005 * 1e4, MIDDLE);		//
	}
	else if(heatingMODE == HIGH){
		CalcIHdqdt_atleast(0.11,0.14,231.9 * 0.005 * 1e5, HIGH);		//
	}
	// GaussSeidel計算する
	doCalc = true;

	//3.各面での熱流束量から全体剛性ベクトルを作る。{F}に代入

#if 1			// switch1
	//UpdateVecF_frypan();		//	関数の外に移動2014.3.12
#endif
	//%%	IH加熱のモード切替
	//	ライン状に加熱
	//	CalcIHdqdtband_(0.09,0.10,231.9 * 5e3);		//*0.5*1e4	値を変えて実験	//*1e3　//*1e4 //5e3
	//	円環状に加熱

	//	この後で、熱流束ベクトルを計算する関数を呼び出す
	///	熱伝達率を各節点に格納
	//SetHeatTransRatioToAllVertex();
#if 0			//!switch1
	InitVecFAlls();
	for(unsigned i=0; i < this->tets.size(); i++){
		CreateVecFAll(i);
	}
#endif

#if 0
	CreateVecF2surfaceAll();		//	CreateVecFAll(i);の代わり
	CreateVecF3surfaceAll();		//	CreateVecFAll(i);の代わり
#endif
}

//小野原追加ここから==========================================
void PHFemThermo::UpdateIHheatband(double xS,double xE,unsigned heatingMODE){////x座標：xS~xEの間の節点に熱流束境界条件を設定

	//熱伝導率、密度、比熱、熱伝達率　のパラメーターを設定・代入
		//PHFemThermoのメンバ変数の値を代入 CADThermoより、0.574;//玉ねぎの値//熱伝導率[W/(ｍ・K)]　Cp = 1.96 * (Ndt);//玉ねぎの比熱[kJ/(kg・K) 1.96kJ/(kg K),（玉ねぎの密度）食品加熱の科学p64より970kg/m^3
		//熱伝達率の単位系　W/(m^2 K)⇒これはSI単位系なのか？　25は論文(MEAT COOKING SIMULATION BY FINITE ELEMENTS)のオーブン加熱時の実測値
		//SetInitThermoConductionParam(0.574,970,1.96,25);
	//SetInitThermoConductionParam(0.574,970,0.1960,25 * 0.01);		//> thConduct:熱伝導率 ,roh:密度,	specificHeat:比熱 J/ (K・kg):1960 ,　heatTrans:熱伝達率 W/(m^2・K)
		//これら、変数値は後から計算の途中で変更できるようなSetParam()関数を作っておいたほうがいいかな？

	//.		熱流束の設定
	//..	初期化
	//SetVtxHeatFluxAll(0.0);

	//1.フライパン位置を取ってくる
		//ih加熱円環中心からの同心円状加熱領域を計算し、ihdqdtに当てはめるメッシュ情報を生成
		//　if(フライパンが動いたか)	動いていなければ、vecfも、1step前の値を使えるようにしておきたい。

	//2...	face面での熱流束量を計算（フライパン位置又はポインタを引数に代入：毎回フライパンの位置が変化するので、フライパン位置の変化の度に生成する）
	if(heatingMODE == OFF){
		CalcIHdqdtband(xS,xE,0.0, OFF);		//	IH加熱行列の係数0となるため、計算されない
	}
	else if(heatingMODE == WEEK){	
		CalcIHdqdtband(xS,xE,231.9 * 1e1, WEEK);		//
	}
	else if(heatingMODE == MIDDLE){
		CalcIHdqdtband(xS,xE,231.9 * 0.005 * 1e4, MIDDLE);		//
	}
	else if(heatingMODE == HIGH){
		CalcIHdqdtband(xS,xE,231.9 * 0.005 * 1e5, HIGH);		//
	}

	//3.各面での熱流束量から全体剛性ベクトルを作る。{F}に代入

#if 1			// switch1
	UpdateVecF_frypan();
#endif
	//%%	IH加熱のモード切替
	//	ライン状に加熱
	//	CalcIHdqdtband_(0.09,0.10,231.9 * 5e3);		//*0.5*1e4	値を変えて実験	//*1e3　//*1e4 //5e3
	//	円環状に加熱

	//	この後で、熱流束ベクトルを計算する関数を呼び出す
	///	熱伝達率を各節点に格納
	//SetHeatTransRatioToAllVertex();
#if 0			//!switch1
	InitVecFAlls();
	for(unsigned i=0; i < this->tets.size(); i++){
		CreateVecFAll(i);
	}
#endif

#if 0
	CreateVecF2surfaceAll();		//	CreateVecFAll(i);の代わり
	CreateVecF3surfaceAll();		//	CreateVecFAll(i);の代わり
#endif
}
//小野原追加ここまで==========================================

void PHFemThermo::SetParamAndReCreateMatrix(double thConduct0,double roh0,double specificHeat0){
	// デバッグのため　&&　使われていなかったので、コメントアウト
	DSTR << "この関数はデバッグのため、コメントアウトしています。用いる場合には、実装を良く見て復活させてください。" <<std::endl;

	//for(unsigned i =0; i < edges.size();i++){
	//	edges[i].c = 0.0;
	//	edges[i].k = 0.0;
	//}

	/////	faces
	//for(unsigned i=0;i<faces.size();i++){
	//	faces[i].alphaUpdated = true;
	//	faces[i].area = 0.0;
	//	faces[i].heatTransRatio = 0.0;
	//	faces[i].deformed = true;				//初期状態は、変形後とする
	//	faces[i].fluxarea =0.0;
	//	for(unsigned j =0; j < HIGH +1 ; j++){			// 加熱モードの数だけ、ベクトルを生成
	//		faces[i].heatflux[j] = 0.0;
	//	}
	//}

	////行列の成分数などを初期化
	//bVecAll.resize(vertices.size(),1);

	////節点温度の初期設定(行列を作る前に行う)
	//SetVerticesTempAll(0.0);			///	初期温度の設定

	////周囲流体温度の初期化(temp度にする)
	//InitTcAll(0.0);

	////dmnN 次元の温度の縦（列）ベクトル
	//CreateTempVertex();

	////熱伝導率、密度、比熱、熱伝達率　のパラメーターを設定・代入
	//	//PHFemThermoのメンバ変数の値を代入 CADThermoより、0.574;//玉ねぎの値//熱伝導率[W/(ｍ・K)]　Cp = 1.96 * (Ndt);//玉ねぎの比熱[kJ/(kg・K) 1.96kJ/(kg K),（玉ねぎの密度）食品加熱の科学p64より970kg/m^3
	//	//熱伝達率の単位系　W/(m^2 K)⇒これはSI単位系なのか？　25は論文(MEAT COOKING SIMULATION BY FINITE ELEMENTS)のオーブン加熱時の実測値
	////. 熱伝達する SetInitThermoConductionParam(0.574,970,0.1960,25 * 0.001 );		//> thConduct:熱伝導率 ,roh:密度,	specificHeat:比熱 J/ (K・kg):1960 ,　heatTrans:熱伝達率 W/(m^2・K)
	////. 熱伝達しない
	//SetInitThermoConductionParam(thConduct0,roh0,specificHeat0,0);		// 熱伝達率=0;にしているw
	//
	////> 熱流束の初期化
	////SetVtxHeatFluxAll(0.0);			// 頂点の熱流束の初期化

	////>	熱放射率の設定
	//SetThermalEmissivityToVerticesAll(0.0);				///	暫定値0.0で初期化	：熱放射はしないｗ

	////> IH加熱するfaceをある程度(表面face && 下底面)絞る、関係しそうなface節点の原点からの距離を計算し、face[].mayIHheatedを判定
	//CalcVtxDisFromOrigin();
	////CalcVtxDisFromVertex(0.0,-1.2);
	////>	IHからの単位時間当たりの加熱熱量	//単位時間当たりの総加熱熱量	231.9; //>	J/sec
	//
	////	この後で、熱流束ベクトルを計算する関数を呼び出す
	//InitCreateMatC();					///	CreateMatCの初期化
	//InitVecFAlls();					///	VecFAll類の初期化
	//InitCreateMatk();					///	CreateMatKの初期化

	/////	熱伝達率を各節点に格納
	//SetHeatTransRatioToAllVertex();
	//for(unsigned i=0; i < this->tets.size(); i++){
	//	tets[i].volume = CalcTetrahedraVolume2(i);
	//	//各行列を作って、ガウスザイデルで計算するための係数の基本を作る。Timestepの入っている項は、このソース(SetDesc())では、実現できないことが分かった(NULLが返ってくる)
	//	CreateMatkLocal(i);				///	Matk1 Matk2(更新が必要な場合がある)を作る	//ifdefスイッチで全体剛性行列も(表示用だが)生成可能
	//	CreatedMatCAll(i);
	//	CreateVecFAll(i);
	//}
	//
	//// カウントの初期化
	////Ndt =0;

	////水分蒸発周りの初期化
	//InitMoist();

	//SetVerticesTempAll(0.0);

}

//void PHFemThermo::ReProduceMat_Vec_ThermalRadiation() {	
//
//	//%%%	初期化類		%%%//
//
//	//各種メンバ変数の初期化⇒コンストラクタでできたほうがいいかもしれない。
//	///	Edges
//	for(unsigned i =0; i < edges.size();i++){
//		edges[i].c = 0.0;	
//		edges[i].k = 0.0;
//	}
//
//	///	faces
//	for(unsigned i=0;i<faces.size();i++){
//		faces[i].alphaUpdated = true;
//		faces[i].area = 0.0;
//		faces[i].heatTransRatio = 0.0;
//		faces[i].deformed = true;				//初期状態は、変形後とする
//		faces[i].fluxarea =0.0;
//		faces[i].thermalEmissivity =0.0;
//		for(unsigned mode =0; mode < HIGH +1 ; mode++){			// 加熱モードの数だけ、ベクトルを生成
//			faces[i].heatflux[mode] = 0.0;
//		}
//	}
//
//	//行列の成分数などを初期化
//	bVecAll.resize(vertices.size(),1);	
//	TVecAll.resize(vertices.size());
//
//	SetVerticesTempAll(jout);
//
//	//周囲への熱伝達温度の初期化(temp度にする)　≠　熱輻射
//	InitTcAll(temp_c);
//	//>	熱放射関係	LM SS
//	InitToutAll(temp_out);
//	//SetThermalEmissivityToVerticesAll(radiantHeat);				///	暫定値0.0で初期化	：熱放射はしないｗ	
//	SetThermalEmissivityToVerticesAll(ems);	
//
//	//vertices.tempをすべて、TVecAllへ代入する
//	CreateTempVertex();
//
//	DSTR << "thConduct:" << thConduct << std::endl;
//	
//	CalcVtxDisFromVertex(Vec2d(0.0, -0.005));		// 鉄板の原点座標を引数に入れ、対称性が合うようになっている。
//	InitCreateMatC();					///	CreateMatCの初期化
//	InitVecFAlls();					///	VecFAll類の初期化
//	InitCreateMatk();					///	CreateMatKの初期化
//
//	keisuInv.resize(vertices.size(),vertices.size());
//	keisuInv.clear();
//
//	///	熱伝達率を各節点に格納
//	SetHeatTransRatioToAllVertex();
//	for(unsigned i=0; i < this->tets.size(); i++){
//		tets[i].volume = CalcTetrahedraVolume2(i);
//		CreateMatkLocal(i);				///	Matk1 Matk2(更新が必要な場合がある)を作る	//ifdefスイッチで全体剛性行列も(表示用だが)生成可能
//		CreatedMatCAll(i);
//		CreateVecFAll(i);
//	}
//#if 0
//
//	//	頂点１の担当体積に対し、熱量を加えるために、担当体積換算で熱量を頂点の温度として与える。{F}を使わないので、熱流束を使わない。
//	double rcv=0.0;
//	for(unsigned i=0;i<vertices[0].tets.size();i++){
//		rcv += tets[vertices[0].tets[i]].volume * RHO * SPECIFICHEAT * 5 / 20; 
//	}
//	double kuwae =1.58;	//	加える熱量
//	//vertices[0].temp = kuwae / rcv;
//	SetVertexTemp(0,kuwae / rcv);
//#endif
//	//このtempをTVecAllに設定
//	//C,Kだけの計算をさせて様子を見る
//
//
//	int hogeshidebug =0;
//	//	節点温度推移の書き出し
////	templog.open("templog.csv");
//
//	//matCAllout.open("matCAllout.txt"); 
//	//matKAllout.open("matKAllout.txt");
//	//
//	//matCAllout << matCAll << std::endl;
//	//matKAllout << matKAll << std::endl;
//	//
//	//matCAllout.close();
//
//	//matCAllout.open("matCAll-1out.txt");
//	//matCAllout << matCAll.inv() << std::endl;
//	//matCAllout.close();
//	////scilabを呼ばないで、SPRの機能で、C.inv() K を求める
//	//matCAllout.open("matCAll.inv()xmatKAll.txt");
//	//matCAllout << matCAll.inv() * matKAll << std::endl; 
//	//matCAllout.close();
//
//	//tempMat.resize(vertices.size(),vertices.size());
//	//tempMat.clear();
//	//tempMat = matCAll.inv() * matKAll;
//
//	//std::vector<double> rowval;
//	//double tempval=0;
//	//for(unsigned i=0;i<vertices.size();i++){
//	//	for(unsigned j=0;j<vertices.size();j++){
//	//	tempval += tempMat[i][j];
//	//	}
//	//	rowval.push_back(tempval);
//	//}
//	//matCAllout.open("matCAll.inv()xmatKAllの各列の和.txt");
//	//for(unsigned i=0;i<vertices.size();i++){
//	//	matCAllout << rowval[i] <<std::endl;
//	//}
//	//matCAllout.close();
//
//	//カウントの初期化
//	COUNT = 0;
//
//	//温度変化出力
//	checkTVecAllout.open("checkTVecAllout.csv");
//	checkTVecAllout <<"時間" << COUNT<<", ";
//	for(unsigned i=0; i < vertices.size();i++){
//		if(i != vertices.size() -1){
//			checkTVecAllout << "頂点" << i << ", ";	
//		}
//		else{
//			checkTVecAllout << "頂点" << i << std::endl;
//		}
//	}
//	FEMLOG.open("femLog.csv");
//
//	//	CPSの経時変化を書き出す
//	//cpslog.open("cpslog.csv");
//
//	// カウントの初期化
//	Ndt =0;
//
//	//水分蒸発周りの初期化
//	InitMoist();
//
//}




void PHFemThermo::AfterSetDesc() {	
}

//void PHFemThermo::CreateLocalMatrixAndSet(){
//	//K,C,Fの行列を作る関数を呼び出して、作らせる
//	for(unsigned i = 0; i< tets.size() ; i++){
//		//tetsを引数にいれると、その行列・ベクトルを作ってくれて、できた行列、ベクトルを基に係数を設定しくれる
//		//こうすれば、各要素剛性行列でfor文を回さなくてもよくなる
//		//CreateMatkLocal(tets);
//		//CreateMatcLocal(tets);
//		//CreateVecfLocal(tets);
//
//		//tetsを入れて作らせる
////		SetkcfParam(tets);
//
//	}
//
//}

//void PHFemThermo::SetkcfParam(Tet tets){
//}

void PHFemThermo::OutputMatKall(){
	//matKAllout.open("matKAllout.txt");
	//matKAllout << matKAll << std::endl;
	//matKAllout.close();
#ifdef badMatK
	upmatKAllout.open("upMatKAllout.csv");
	matKAllout.open("matKAllout.csv");
	for(unsigned i=0; i<GetPHFemMesh()->vertices.size(); i++){
		for(unsigned j=0; j<GetPHFemMesh()->vertices.size(); j++){
			if(i==j){
				matKAllout << matKAll[i][j]-matkupdate[i] << ",";
			}else{
				matKAllout << matKAll[i][j] << ",";
			}
			upmatKAllout << matKAll[i][j] << ",";
		}
		matKAllout << std::endl;
		upmatKAllout << std::endl;
	}
	matKAllout.close();
	upmatKAllout.close();
	std::ofstream upVecout;
	upVecout.open("upVecout.csv");
	for(unsigned i=0; i<matkupdate.size(); i++){
		upVecout << matkupdate[i] << std::endl;
	}
	upVecout.close();
#endif
	std::ofstream matkprop;
	matkprop.open("matKprop.txt");
	double maxgap = 0.0;
	//unsigned maxvtx;
	//double maxtotal;
	for(unsigned i=0; i < GetPHFemMesh()->vertices.size(); i++){
		double total = 0.0;
		for(unsigned j=0; j < GetPHFemMesh()->vertices.size(); j++){
			if(i != j){
				total += fabs(matKAll[i][j]);
			}
		}
		if(total >= fabs(matKAll[i][i])){
			matkprop << i << "行目：　対角成分：" << matKAll[i][i] << "　非対角成分の和：" << total << std::endl;
		}else{
			if((total - matKAll[i][i]) > maxgap){
				maxgap = total - matKAll[i][i];
			//	maxvtx = i;
				//maxtotal = total;
			}
		}
	}
	matkprop << "対角成分と非対角成分の和の差の最大値："<< maxgap << std::endl;
	//matkprop << maxvtx << "行目：　対角成分：" << matKAll[maxvtx][maxvtx] << "　非対角成分の和：" << maxtotal;
	matkprop.close();
}

void PHFemThermo::CreateMatc(unsigned id){
	//最後に入れる行列を初期化
	for(unsigned i =0; i < 4 ;i++){
		for(unsigned j =0; j < 4 ;j++){
			matc[i][j] = 0.0;
		}
	}
	//matc に21でできた行列を入れる
	matc = Create44Mat21();
	//matc = Create44Mat50();
	//	for debug
		//DSTR << "matc " << matc << " ⇒ ";
	
	matc = rho * specificHeat * CalcTetrahedraVolume2(id) / 20.0 * matc;
	
	//	debug	//係数の積をとる
		//DSTR << matc << std::endl;
		//int hogemat =0 ;
}

void PHFemThermo::CreatedMatCAll(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//すべての要素について係数行列を作る
		//c
	CreateMatc(id);
	int mathoge=0;
	//	(ガウスザイデルを使った計算時)要素毎に作った行列の成分より、エッジに係数を格納する
	//	or	(ガウスザイデルを使わない計算時)要素ごとの計算が終わるたびに、要素剛性行列の成分だけをエッジや点に作る変数に格納しておく	#ifedefでモード作って、どちらもできるようにしておいても良いけどw
#ifdef UseGaussSeidel
	for(unsigned j=1; j < 4; j++){
		int vtxid0 = mesh->tets[id].vertexIDs[j];
		//	下三角行列部分についてのみ実行
		//	j==1:k=0, j==2:k=0,1, j==3:k=0,1,2
		for(unsigned k = 0; k < j; k++){
			int vtxid1 = mesh->tets[id].vertexIDs[k];
				for(unsigned l =0; l < mesh->vertices[vtxid0].edgeIDs.size(); l++){
					for(unsigned m =0; m < mesh->vertices[vtxid1].edgeIDs.size(); m++){
						if(mesh->vertices[vtxid0].edgeIDs[l] == mesh->vertices[vtxid1].edgeIDs[m]){
							edgeVars[mesh->vertices[vtxid0].edgeIDs[l]].c += matc[j][k];		//同じものが二つあるはずだから半分にする。上三角化下三角だけ走査するには、どういうfor文ｓにすれば良いのか？
							//DSTR << edges[vertices[vtxid0].edges[l]].k << std::endl;
						}
					}
				}
		}
	}
	//対角成分を対角成分の全体剛性行列から抜き出した1×nの行列に代入する
	//j=0~4まで代入(上のループでは、jは対角成分の範囲しかないので、値が入らない成分が出てしまう)
	for(unsigned j =0;j<4;j++){
		dMatCAll[0][mesh->tets[id].vertexIDs[j]] += matc[j][j];
	}
#endif

#ifdef UseMatAll
	//SciLabで使うために、全体剛性行列を作る
	//matkから作る
	for(unsigned j=0; j<4 ; j++){
		for(unsigned k=0; k<4 ;k++){
			matCAll[mesh->tets[id].vertexIDs[j]][mesh->tets[id].vertexIDs[k]] += matc[j][k];
		}
	}
#endif

	////	for debug
	//DSTR << "dMatCAll : " << std::endl;
	//for(unsigned j =0;j < vertices.size();j++){
	//	DSTR << j << "th : " << dMatCAll[0][j] << std::endl;
	//}
	// ネギについて非0成分になった。

	//	調べる
	//dMatKAllの成分のうち、0となる要素があったら、エラー表示をするコードを書く
	// try catch文にする
	//for(unsigned j = 0; j < vertices.size() ; j++){
	//	if(dMatCAll[0][j] ==0.0){
	//		DSTR << "dMatCAll[0][" << j << "] element is blank" << std::endl;
	//	}
	//}
	int piyodebug =0;
}

void PHFemThermo::CreateVecFAll(unsigned id){

	vecf.clear();
	//	注意
	//	f3を使用する場合:周囲流体温度Tcが0の節点の要素は0になるため、温度の設定が必要
	
	//すべての要素について係数行列を作る
	//f1を作る
	//>	熱流束境界条件	vecf2を作る			
	CreateVecf2surface(id);				//四面体の各面について計算し加算（重ねあわせ）する
	
	//DSTR << "tets[id].vecf[2]: " << tets[id].vecf[2] << std::endl;
	CreateVecf4surface(id);			//produce vecf[3]

	//>	熱伝達境界条件	f3を作る
#ifndef NOTUSE_HEATTRANS_HERE
	CreateVecf3(id);			// surface化すべきだよね	//>	tets[id].vecf[2] を初期化,代入		熱伝達率は相加平均、周囲流体温度は節点の形状関数？ごとに求める
	
	CreateVecf3surface(id);			//実装する
#endif
	//CreateVecf3_(id);			//>	tets[id].vecf[2] を初期化,代入		熱伝達率、周囲流体温度を相加平均で求める
	//f4を作る
	//f1:vecf[0],f2:vecf[1],f3:vecf[2],f4:vecf[3]を加算する

#ifndef NOTUSE_HEATTRANS_HERE
	vecf = tets[id].vecf[1] + tets[id].vecf[2] + tets[id].vecf[3];		//>	+ tets[id].vecf[0] +  tets[id].vecf[3] の予定
#else	//	熱輻射や熱伝達は切り替えられるようにするべき。＠アプリケーション
	vecf += tetVars[id].vecf[1] + tetVars[id].vecf[3];
#endif

	//	(ガウスザイデルを使った計算時)要素毎に作った行列の成分より、エッジに係数を格納する
	//	or	(ガウスザイデルを使わない計算時)要素ごとの計算が終わるたびに、要素剛性行列の成分だけをエッジや点に作る変数に格納しておく	#ifedefでモード作って、どちらもできるようにしておいても良いけどw

	//要素の節点番号の場所に、その節点のfの値を入れる
	//j:要素の中の何番目か
	for(unsigned j =0;j < 4; j++){
		int vtxid0 = GetPHFemMesh()->tets[id].vertexIDs[j];
		vecFAllSum[vtxid0] += vecf[j];
		//vecFAll_f2IH[num][vtxid0][0] += vecf[j];
	}
	//	for debug
	//vecFAllSumに値が入ったのかどうかを調べる 2011.09.21全部に値が入っていることを確認した
	//DSTR << "vecFAllSum : " << std::endl;
	//for(unsigned j =0; j < vertices.size() ; j++){
	//	DSTR << j << " ele is :  " << vecFAllSum[j] << std::endl;
	//}

	////	調べる
	////vecFAllSumの成分のうち、0となる要素があったら、エラー表示をするコードを書く
	//// try catch文にする
	//for(unsigned j = 0; j < vertices.size() ; j++){
	//	if(vecFAllSum[j] ==0.0){
	//		DSTR << "vecFAllSum[" << j << "] element is blank" << std::endl;
	//	}
	//}

}


void PHFemThermo::CreateMatkLocal_update(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	四面体要素について係数行列[k]を作る			命名規則	
	//..	k1を作る	k1kでも、k1bでもどちらでも構わない	どちらが速いか調べる
	///....		変形した時だけ生成する
	//if(deformed){	
	//	CreateMatk1k(id);
	//}			//  K第一項（メッシュ内熱伝導）の行列作成	//k理論を根拠に、加筆して、形状関数を導出	
	//if(deformed){	CreateMatk1b(id);}			//	書籍の理論を根拠に、公式を用いて形状関数を導出
	
	//....		熱伝達境界条件に必要なk2
	//CreateMatk2t(id);					///	K2: 熱 伝達 境界条件
	//CreateMatk3t(id);					///	K3: 熱 輻射 境界条件(空気への熱伝達率)
	CreateMatk3t_nonRadiantHeat(id);	//>	上記K3に対し、熱伝達faceでは、

	//k1,k2,k3を加算する(使っている数値だけ)
	matk = tetVars[id].matk[0] + tetVars[id].matk[1] + tetVars[id].matk[2];	
	//<注意>	matk[0]:K1, matk[1]:K2, matk[2]:K3

#ifdef UseGaussSeidel
	//	(ガウスザイデルを使った計算時)要素毎に作った行列の成分より、エッジに係数を格納する
	//	or	(ガウスザイデルを使わない計算時)要素ごとの計算が終わるたびに、要素剛性行列の成分だけをエッジや点に作る変数に格納しておく	#ifedefでモード作って、どちらもできるようにしておいても良いけどw
	//	Edges のこの要素で計算したK行列の成分をkに係数として格納する		
	//matkの対角成分以外で、下三角の部分の値を、edgeのkに代入する
	for(unsigned j=1; j < 4; j++){
		int vtxid0 = mesh->tets[id].vertexIDs[j];
		//	下三角行列部分についてのみ実行
		//	j==1:k=0, j==2:k=0,1, j==3:k=0,1,2

		for(unsigned k = 0; k < j; k++){
			int vtxid1 = mesh->tets[id].vertexIDs[k];
			for(unsigned l =0; l < mesh->vertices[vtxid0].edgeIDs.size(); l++){
				for(unsigned m =0; m < mesh->vertices[vtxid1].edgeIDs.size(); m++){
					if(mesh->vertices[vtxid0].edgeIDs[l] == mesh->vertices[vtxid1].edgeIDs[m]){
						edgeVars[mesh->vertices[vtxid0].edgeIDs[l]].k += matk[j][k];		//同じものが二つあるはずだから半分にする。上三角化下三角だけ走査するには、どういうfor文ｓにすれば良いのか？
						//DSTR << edges[vertices[vtxid0].edges[l]].k << std::endl;
#ifdef DumK
						edgeVars[mesh->vertices[vtxid0].edgeIDs[l]].k = 0.0;
#endif DumK
					}
				}
			}
		}
	}
#endif

#ifdef UseMatAll
	//SciLabで使うために、全体剛性行列を作る
	//matkから作る
	for(unsigned j=0; j<4 ; j++){
		for(unsigned k=0; k<4 ;k++){
			matKAll[mesh->tets[id].vertexIDs[j]][mesh->tets[id].vertexIDs[k]] += matk[j][k];
		}
	}
#endif UseMatAll

#ifdef DumK
	//SciLabで使うために、全体剛性行列を作る
	//matkから作る
	for(unsigned j=0; j<4 ; j++){
		for(unsigned k=0; k<4 ;k++){
			matKAll[tets.vertices[j]][tets.vertices[k]] = 0.0;;
		}
	}
#endif

#ifdef UseGaussSeidel
	//対角成分を対角成分の全体剛性行列から抜き出した1×nの行列に代入する
	//j=0~4まで代入(上のループでは、jは対角成分の範囲しかないので、値が入らない成分が出てしまう)
	for(unsigned j =0;j<4;j++){
		dMatKAll[0][mesh->tets[id].vertexIDs[j]] += matk[j][j];
		int hoge4 =0;
	}
#endif

#ifdef DumK
	for(unsigned j=0;j<4;j++){
		dMatKAll[0][mesh->tets[id].vertexID[j]] = 0.0;
		int hogeshi =0;
	} 
#endif DumK
	
//#ifdef UseMatAll
//#ifdef UseGaussSeidel
//	for(unsigned j =0;j<mesh->vertices.size();j++){
//		if(matKAll[j][j] != dMatKAll[0][j]){
//			DSTR << j <<" 成分の要素はおかしい！調査が必要である。 matKAll：" << matKAll[j][j] << " dMatKAll：" << dMatKAll[0][j] <<std::endl;
//		}
//	}
//#endif
//#endif UseMatAll

}

void PHFemThermo::CreateMatkLocal(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	四面体要素について係数行列[k]を作る			命名規則	
	//..	k1を作る	k1kでも、k1bでもどちらでも構わない	どちらが速いか調べる
	///....		変形した時だけ生成する
	if(deformed){	CreateMatk1k(id);}			//  K第一項（メッシュ内熱伝導）の行列作成	//k理論を根拠に、加筆して、形状関数を導出	
	//if(deformed){	CreateMatk1b(id);}			//	書籍の理論を根拠に、公式を用いて形状関数を導出
	
	//....		熱伝達境界条件に必要なk2
	CreateMatk2t(id);					///	K2: 熱 伝達 境界条件
	CreateMatk3t(id);					///	K3: 熱 輻射 境界条件(空気への熱伝達率)
	//k1,k2,k3を加算する(使っている数値だけ)
	matk = tetVars[id].matk[0] + tetVars[id].matk[1] + tetVars[id].matk[2];	
	//<注意>	matk[0]:K1, matk[1]:K2, matk[2]:K3
	matkcheck << id << std::endl;
	matkcheck << tetVars[id].matk[2] << std::endl;
	//if(id == 0){
	//	DSTR << "Initial Create matk" << std::endl;
	//	DSTR << matk << std::endl;
	//}
	//DSTR << "tets[id].matk[0]" << std::endl;
	//DSTR << tets[id].matk[0] << std::endl;
	//DSTR << "tets[id].matk[1]" << std::endl;
	//DSTR << tets[id].matk[1] << std::endl;
	//DSTR << "tets[id].matk[2]" << std::endl;
	//DSTR << tets[id].matk[2] << std::endl;

#ifdef UseGaussSeidel
	//	(ガウスザイデルを使った計算時)要素毎に作った行列の成分より、エッジに係数を格納する
	//	or	(ガウスザイデルを使わない計算時)要素ごとの計算が終わるたびに、要素剛性行列の成分だけをエッジや点に作る変数に格納しておく	#ifedefでモード作って、どちらもできるようにしておいても良いけどw
	//	Edges のこの要素で計算したK行列の成分をkに係数として格納する
		
	//matkの対角成分以外で、下三角の部分の値を、edgeのkに代入する
	//
	//DSTR << i <<"th tetrahedra element'edges[vertices[vtxid0].edges[l]].k (All edges.k' value): " << std::endl;
	for(unsigned j=1; j < 4; j++){
		int vtxid0 = mesh->tets[id].vertexIDs[j];
		//	下三角行列部分についてのみ実行
		//	j==1:k=0, j==2:k=0,1, j==3:k=0,1,2
		for(unsigned k = 0; k < j; k++){
			int vtxid1 = mesh->tets[id].vertexIDs[k];
			for(unsigned l =0; l < mesh->vertices[vtxid0].edgeIDs.size(); l++){
				for(unsigned m =0; m < mesh->vertices[vtxid1].edgeIDs.size(); m++){
					if(mesh->vertices[vtxid0].edgeIDs[l] == mesh->vertices[vtxid1].edgeIDs[m]){
						edgeVars[mesh->vertices[vtxid0].edgeIDs[l]].k += matk[j][k];		//同じものが二つあるはずだから半分にする。上三角化下三角だけ走査するには、どういうfor文ｓにすれば良いのか？
						//DSTR << edges[vertices[vtxid0].edges[l]].k << std::endl;
#ifdef DumK
						edgeVars[mesh->vertices[vtxid0].edgeIDs[l]].k = 0.0;
#endif DumK
					}
				}
			}
		}
	}
#endif

#ifdef UseMatAll
	//SciLabで使うために、全体剛性行列を作る
	//matkから作る
	for(unsigned j=0; j<4 ; j++){
		for(unsigned k=0; k<4 ;k++){
			matKAll[mesh->tets[id].vertexIDs[j]][mesh->tets[id].vertexIDs[k]] += matk[j][k];
		}
	}

	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	DSTR << "matkall" << std::endl;
	//	for(unsigned j=0; j < mesh->vertices.size(); j++){
	//		for(unsigned k=0; k < mesh->vertices.size(); k++){
	//			DSTR << std::setw(13) << matKAll[j][k] << "\t";
	//		}
	//		DSTR << std::endl;
	//	}
	//	DSTR << std::endl;
	//}

	//matKAllの非対角成分のうち正のものの符号を反転し、行の値の合計が変化しないように対角成分の値で調節する
	if(strcmp(mesh->GetName(), "femNsteak") == 0){
		//matKModif();
	}

	////edgesに入った係数から作る
	//for(unsigned j=1; j < 4; j++){
	//	int vtxid0 = tets[i].vertices[j];
	//	//	下三角行列部分についてのみ実行
	//	//	j==1:k=0, j==2:k=0,1, j==3:k=0,1,2
	//	for(unsigned k = 0; k < j; k++){
	//		int vtxid1 = tets[i].vertices[k];
	//			for(unsigned l =0; l < vertices[vtxid0].edges.size(); l++){
	//				for(unsigned m =0; m < vertices[vtxid1].edges.size(); m++){
	//					if(vertices[vtxid0].edges[l] == vertices[vtxid1].edges[m]){
	//						edges[vertices[vtxid0].edges[l]].k += matk[j][k];		//同じものが二つあるはずだから半分にする。上三角化下三角だけ走査するには、どういうfor文ｓにすれば良いのか？
	//						//DSTR << edges[vertices[vtxid0].edges[l]].k << std::endl;
	//					}
	//				}
	//			}
	//	}
	//}

#endif UseMatAll

#ifdef DumK
	//SciLabで使うために、全体剛性行列を作る
	//matkから作る
	for(unsigned j=0; j<4 ; j++){
		for(unsigned k=0; k<4 ;k++){
			matKAll[tets.vertices[j]][tets.vertices[k]] = 0.0;;
		}
	}
#endif

#ifdef UseGaussSeidel
	//対角成分を対角成分の全体剛性行列から抜き出した1×nの行列に代入する
	//j=0~4まで代入(上のループでは、jは対角成分の範囲しかないので、値が入らない成分が出てしまう)
	for(unsigned j =0;j<4;j++){
		dMatKAll[0][mesh->tets[id].vertexIDs[j]] += matk[j][j];
		//DSTR << "matk[" << j << "][" << j << "] : " << matk[j][j] << std::endl;
		//DSTR << "dMatKAll[0][" << tets[i].vertices[j] << "] : " << dMatKAll[0][tets[i].vertices[j]] << std::endl;
		int hoge4 =0;
	}
#endif
	//DSTR << std::endl;	//改行

	//std::ofstream matKAll("matKAll.csv");
	//for(){
	//	matKAll
	//	}


#ifdef DumK
	for(unsigned j=0;j<4;j++){
		dMatKAll[0][tets.vertices[j]] = 0.0;
		int hogeshi =0;
	} 
#endif DumK

	
	//	for debug
	//要素25の0~3番目の節点が何か表示する
	//if(i == 25){
	//		for(unsigned n=0;n < 4;n++){
	//			DSTR << n << " : " << tets[25].vertices[n] << std::endl;	//要素25の0番目の節点が63である。
	//		}
	//}
	//節点番号が63の点がどの要素に入っているのかを調べる⇒25,57だった
	//for(unsigned j=0;j < vertices[63].tets.size();j++){
	//	DSTR << vertices[63].tets[j] <<std::endl;
	//}
		
	//	調べる
	//dMatKAllの成分のうち、0となる要素があったら、エラーか？　入っていない成分があっても、問題ない気もする
	// try catch文にする
//	for(unsigned j = 0; j < vertices.size() ; j++){
//		if(dMatKAll[0][j] ==0.0){
//			DSTR << "Creating dMatKAll error!? : dMatKAll[0][" << j << "] == 0.0 " << std::endl;
////			DSTR << "If " << j <<" 's blank eroors didn't banished until display simulation scene, I recommened Source Code Check!" <<std::endl;  
//		}
//	}

	//DSTR << "matKAll : " << matKAll <<std::endl;
	//for(unsigned i=0;i<vertices.size();i++){
	//	for(unsigned j=0;j <  vertices.size();j++){
	//		if(matKAll[i][j] !=0){
	//			DSTR << "matKAll[" << i <<"][" << j << "]: " << matKAll[i][j] <<std::endl;
	//		}
	//	}
	//}
	//DSTR << "dMatKAll : " <<dMatKAll << std::endl;
//#ifdef UseMatAll
//#ifdef UseGaussSeidel
//	for(unsigned j =0;j<mesh->vertices.size();j++){
//		if(matKAll[j][j] != dMatKAll[0][j]){
//			DSTR << j <<" 成分の要素はおかしい！調査が必要である。 matKAll：" << matKAll[j][j] << " dMatKAll：" << dMatKAll[0][j] <<std::endl;
//		}
//	}
//#endif
//#endif UseMatAll
	int hoge5 =0;

}

void PHFemThermo::CreateMatk1b(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//yagawa1983を基にノートに式展開した計算式
	unsigned i=0;
	unsigned j=0;
	unsigned k=0;
	unsigned l=0;

	//a_1~a_4, ... , c_4	を作成
	//係数(符号)×行列式の入れ物
	double a[4];
	double b[4];
	double c[4];

	//要素を構成する節点の座標の入れ物
	double x[4];
	double y[4];
	double z[4];
	//要素内の4節点のx,y,z座標を格納
	for(unsigned m=0; m < 4;m++){
		x[m] = mesh->vertices[mesh->tets[id].vertexIDs[m]].pos.x;
		y[m] = mesh->vertices[mesh->tets[id].vertexIDs[m]].pos.y;
		z[m] = mesh->vertices[mesh->tets[id].vertexIDs[m]].pos.z;
	}

	//mata~matc
	//a[i]
	for(unsigned i =0;i<4;i++){
		double fugou =0.0;				// (-1)^i の符号の定義

		//	fugou の符号判定
		if(i == 0 || i == 2){		//0,2の時、(-1)^1,3 = -1
			fugou = -1.0;
		}
		else{					//1,3の時、(-1)^0,2 = 1
			fugou = 1.0;
		}
		
		//i,j,k,lの関係セット⇒配列の要素にしてもいいかも。i[4],if(i[0]=0){i[1](=j)=1, i[2](=k)=2, i[3](=l)=3}	if(i[0]=1){i[1](=j)=2, i[2](=k)=3, i[3](=l)=0}
		if(i==0){		j=1;	k=2;	l=3;	}
		else if(i==1){	j=2;	k=3;	l=0;	}
		else if(i==2){	j=3;	k=0;	l=1;	}
		else if(i==3){	j=0;	k=1;	l=2;	}

		// a_iの作成
		for(unsigned m =0;m<3;m++){						//	1の成分への代入はついで
			mata[m][0] = 1.0;
		}
		mata[0][1] = y[j];
		mata[1][1] = y[k];
		mata[2][1] = y[l];

		mata[0][2] = z[j];
		mata[1][2] = z[k];
		mata[2][2] = z[l];

		a[i] = fugou * mata.det();

		//DSTR << "mata : " <<std::endl;
		//DSTR << mata << std::endl;

		//DSTR << "mata.det() : " <<std::endl;
		//DSTR << mata.det() << std::endl;

		//DSTR << "a[" << i << "] : " <<std::endl;
		//DSTR << a[i] << std::endl;


		// b_iの作成
		matb[0][0]=x[j];
		matb[1][0]=x[k];
		matb[2][0]=x[l];

		for(unsigned m =0;m<3;m++){						//	1の成分への代入はついで
			matb[m][1] = 1.0;
		}

		matb[0][2]=z[j];
		matb[1][2]=z[k];
		matb[2][2]=z[l];

		b[i] = fugou * matb.det();

		//DSTR << "matb : " <<std::endl;
		//DSTR << matb << std::endl;
		//DSTR << "matb.det() : " <<std::endl;
		//DSTR << matb.det() << std::endl;

		//DSTR << "b[" << i << "] : " <<std::endl;
		//DSTR << b[i] << std::endl;

		// c_iの作成
		matcc[0][0]=x[j];
		matcc[1][0]=x[k];
		matcc[2][0]=x[l];

		matcc[0][1]=y[j];
		matcc[1][1]=y[k];
		matcc[2][1]=y[l];

		for(unsigned m =0;m<3;m++){						//	1の成分への代入はついで
			matcc[m][2] = 1.0;
		}

		//DSTR << "matcc : " <<std::endl;
		//DSTR << matcc << std::endl;
		//DSTR << "matcc.det() : " <<std::endl;
		//DSTR << matcc.det() << std::endl;

		c[i] = fugou * matcc.det();
		
		//	for debug　要素ごとのa_i~c_iの算出
		//DSTR << "a["<< i << "] : " << a[i] << std::endl;
		//DSTR << "b["<< i << "] : " << b[i] << std::endl;
		//DSTR << "c["<< i << "] : " << c[i] << std::endl;
		//DSTR << std::endl;
		int debughogeshi =0;
	}
	
	//	matk1の成分にa_i ~ c_iの多項式を代入	きれいすぎるが、下のコードで良い！	対角成分も非対角成分も、全部、下のコード
	//	改善案		下三角と対角成分だけ、計算し、上三角は下三角を代入でもよい。
	for(unsigned i =0;i<4;i++){
		for(unsigned j =0;j<4;j++){
			tetVars[id].matk[0][i][j] = a[i] * a[j] +b[i] * b[j] + c[i] * c[j];
		}
	}

	////	上記よりコストの少ないコード?
	//matk[0][0] = a[0] * a[0] +b[0] * b[0] + c[0] * c[0];
	//matk[1][1] = a[1] * a[1] +b[1] * b[1] + c[1] * c[1];
	//matk[2][2] = a[2] * a[2] +b[2] * b[2] + c[2] * c[2];
	//matk[3][3] = a[3] * a[3] +b[3] * b[3] + c[3] * c[3];

	//matk[0][1] = a[0] * a[1] + b[0] * b[1] + c[0] * c[1];
	//matk[1][0] = matk[0][1];

	//matk[0][2] = a[0] * a[2] + b[0] * b[2] + c[0] * c[2];
	//matk[2][0] = matk[0][2];

	//matk[0][3] = a[0] * a[3] + b[0] * b[3] + c[0] * c[3];
	//matk[3][0] = matk[0][3];

	//	for DEBUG
	//DSTR << "matk1 : " << std::endl;
	//DSTR << matk1 << std::endl;
	//int debughogeshi2 =0;
	
	//係数の積
//	tets[id].matk[0]= thConduct / (36 *  CalcTetrahedraVolume(tets[id])) * tets[id].matk[0];		//理論が間違っていたので、修正
	tetVars[id].matk[0]= thConduct / (36 *  tetVars[id].volume) * tetVars[id].matk[0];		//理論が間違っていたので、修正

	//	for DEBUG
	//DSTR << "係数積後の matk1 : " << std::endl;
	//DSTR << matk1 << std::endl;
	int debughogeshi3 =0;

	DSTR << "Inner Function matk1b _ tets["<< id << "].matk[0] "<< tetVars[id].matk[0] << std::endl;  
	//a~cの多項式をK1に代入
	//matk1(4x4)に代入

}

void PHFemThermo::CreateMatk1k(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//この計算を呼び出すときに、各四面体ごとに計算するため、四面体の0番から順にこの計算を行う
	//四面体を構成する4節点を節点の配列(Tetsには、節点の配列が作ってある)に入っている順番を使って、面の計算を行ったり、行列の計算を行ったりする。
	//そのため、この関数の引数に、四面体要素の番号を取る

	//最後に入れる行列を初期化
	tetVars[id].matk[0].clear();
	//for(unsigned i =0; i < 4 ;i++){
	//	for(unsigned j =0; j < 4 ;j++){
	//		//tets[id].matk1[i][j] = 0.0;
	//		tets[id].matk[0][i][j] = 0.0;
	//	}
	//}

	//	A行列　=	a11 a12 a13
	//				a21 a22 a23
	//				a31 a32 a33
	//を生成
	PTM::TMatrixRow<4,4,double> matk1A;
	FemVertex p[4];
	for(unsigned i = 0; i< 4 ; i++){
		p[i]= mesh->vertices[mesh->tets[id].vertexIDs[i]];
	}
	
	matk1A[0][0] = (p[2].pos.y - p[0].pos.y) * (p[3].pos.z - p[0].pos.z) - (p[2].pos.z - p[0].pos.z) * (p[3].pos.y - p[0].pos.y);
	matk1A[0][1] = (p[1].pos.z - p[0].pos.z) * (p[3].pos.y - p[0].pos.y) - (p[1].pos.y - p[0].pos.y) * (p[3].pos.z - p[0].pos.z);
	matk1A[0][2] = (p[1].pos.y - p[0].pos.y) * (p[2].pos.z - p[0].pos.z) - (p[1].pos.z - p[0].pos.z) * (p[2].pos.y - p[0].pos.y);

	matk1A[1][0] = (p[2].pos.z - p[0].pos.z) * (p[3].pos.x - p[0].pos.x) - (p[2].pos.x - p[0].pos.x) * (p[3].pos.z - p[0].pos.z);
	matk1A[1][1] = (p[1].pos.x - p[0].pos.x) * (p[3].pos.z - p[0].pos.z) - (p[1].pos.z - p[0].pos.z) * (p[3].pos.x - p[0].pos.x);
	matk1A[1][2] = (p[1].pos.z - p[0].pos.z) * (p[2].pos.x - p[0].pos.x) - (p[1].pos.x - p[0].pos.x) * (p[2].pos.z - p[0].pos.z);

	matk1A[2][0] = (p[2].pos.x - p[0].pos.x) * (p[3].pos.y - p[0].pos.y) - (p[2].pos.y - p[0].pos.y) * (p[3].pos.x - p[0].pos.x);
	matk1A[2][1] = (p[1].pos.y - p[0].pos.y) * (p[3].pos.x - p[0].pos.x) - (p[1].pos.x - p[0].pos.x) * (p[3].pos.y - p[0].pos.y);
	matk1A[2][2] = (p[1].pos.x - p[0].pos.x) * (p[2].pos.y - p[0].pos.y) - (p[1].pos.y - p[0].pos.y) * (p[2].pos.x - p[0].pos.x);

	//	for	debug
	//DSTR << matk1A[0][0] << std::endl;
	//DSTR << "matk1A : " << matk1A << std::endl; 
	//int hogeshi =0;

	//a11 ~ a33 を行列に入れて、[N~T] [N] を計算させる
	
	PTM::TMatrixRow<1,4,double> Nx;
	PTM::TMatrixRow<1,4,double> Ny;
	PTM::TMatrixRow<1,4,double> Nz;

	Nx[0][0] = -matk1A[0][0] - matk1A[0][1] -matk1A[0][2];
	Nx[0][1] = matk1A[0][0];
	Nx[0][2] = matk1A[0][1];
	Nx[0][3] = matk1A[0][2];

	Ny[0][0] = -matk1A[1][0] - matk1A[1][1] -matk1A[1][2];
	Ny[0][1] = matk1A[1][0];
	Ny[0][2] = matk1A[1][1];
	Ny[0][3] = matk1A[1][2];

	Nz[0][0] = -matk1A[2][0] - matk1A[2][1] -matk1A[2][2];
	Nz[0][1] = matk1A[2][0];
	Nz[0][2] = matk1A[2][1];
	Nz[0][3] = matk1A[2][2];

	//	Km の算出
	//tets[id].matk1 = Nx.trans() * Nx + Ny.trans() * Ny + Nz.trans() * Nz;
#if 0
	tets[id].matk[0] = Nx.trans() * Nx + Ny.trans() * Ny + Nz.trans() * Nz;
#else
	//	異方性熱伝導率に対応
	//DSTR << "thConduct_x:" << thConduct_x <<"," << "thConduct_y:" << thConduct_y <<"," << "thConduct_z:" << thConduct_z <<std::endl;
	tetVars[id].matk[0] = thConduct_x * Nx.trans() * Nx + thConduct_y * Ny.trans() * Ny + thConduct_z * Nz.trans() * Nz;
#endif

	////Nx,Ny,Nzを出力
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	PTM::TMatrixRow<4,4,double> NxtNx;
	//	PTM::TMatrixRow<4,4,double> NytNy;
	//	PTM::TMatrixRow<4,4,double> NztNz;
	//	NxtNx=Nx.trans() * Nx;
	//	NytNy=Ny.trans() * Ny;
	//	NztNz=Nz.trans() * Nz;

	//	DSTR << id << std::endl;
	//	for(unsigned i=0; i < 3; i++){
	//		if(i == 0){
	//			DSTR << "Nx=" << "\t";
	//		}else if(i == 1){
	//			DSTR << "Ny=" << "\t";
	//		}else if(i == 2){
	//			DSTR << "Nz=" << "\t";
	//		}
	//		for(unsigned j=0; j < 4; j++){
	//			if(i == 0){
	//				DSTR << Nx[0][j] << "\t";
	//			}else if(i == 1){
	//				DSTR << Ny[0][j] << "\t";
	//			}else if(i == 2){
	//				DSTR << Nz[0][j] << "\t";
	//			}
	//		}
	//		DSTR << std::endl;
	//		if(i == 0){
	//			DSTR << "Nxt*Nx=" << std::endl;
	//		}else if(i == 1){
	//			DSTR << "Nyt*Ny=" << std::endl;
	//		}else if(i == 2){
	//			DSTR << "Nzt*Nz=" << std::endl;
	//		}
	//		for(unsigned k=0; k < 4; k++){
	//			for(unsigned l=0; l < 4; l++){
	//				if(i == 0){
	//					DSTR << std::setw(13) << NxtNx[k][l] << "\t";
	//				}else if(i == 1){
	//					DSTR << std::setw(13) << NytNy[k][l] << "\t";
	//				}else if(i == 2){
	//					DSTR << std::setw(13) << NztNz[k][l] << "\t";
	//				}
	//			}
	//			DSTR << std::endl;
	//		}
	//		DSTR << std::endl;
	//	}

	//	DSTR << "matklocal" << std::endl;
	//	for(unsigned i=0; i < 4; i++){
	//		for(unsigned j=0; j < 4; j++){
	//			DSTR << std::setw(13) << tetVars[id].matk[0][i][j] << "\t";
	//		}
	//		DSTR << std::endl;
	//	}
	//	DSTR << std::endl;
	//}

	////	for debug
	//DSTR << "Nx : " << Nx << std::endl;
	//DSTR << "Nx^T : " << Nx.trans() << std::endl;
	//DSTR << "Nx^T * Nx : " << Nx.trans() * Nx << std::endl;
	//DSTR << "Ny^T * Ny : " << Ny.trans() * Ny << std::endl;
	//DSTR << "Nz^T * Nz : " << Nz.trans() * Nz << std::endl;
	//DSTR << "matk1 : " << tets[id].matk[0] << std::endl;
	//int hogehoge =0;

	//	for	DEBUG
	//DSTR << "matk1 : " << matk1 << std::endl;

	//K1
//	matk1 = thConduct / (36 * CalcTetrahedraVolume(tets) ) * matk1;
	
	//tets[id].matk1 = thConduct / (36 * CalcTetrahedraVolume(tets[id]) ) * tets[id].matk1;
	//tets[id].matk[0] = thConduct / (36 * CalcTetrahedraVolume(tets[id]) ) * tets[id].matk[0];
#if 0
	tets[id].matk[0] = thConduct / (36 * tets[id].volume ) * tets[id].matk[0];
#else
	//tets[id].matk[0] =  1.0/ ( 36 * tets[id].volume ) * tets[id].matk[0];
	tetVars[id].matk[0] =  1.0 / ( 36.0 * tetVars[id].volume ) * tetVars[id].matk[0];
	//tets[id].matk[0] *=  1.0 / ( 36.0 * tets[id].volume )
#endif
	//DSTR << "Inner Function _tets[id].matk1 : " << tets[id].matk1 << std::endl;

}

PTM::TMatrixRow<4,4,double> PHFemThermo::GetKMatInTet(unsigned id){
	//for(unsigned id=0;id<tets.size();++id){
	return tetVars[id].matk[0];
	//}
}

///*小野原追加ここから--------------------------------------------*/
/////頂点の担当する面積の計算を予めしておく。（物体が剛体なのでメッシュ情報は変化しない前提）
void PHFemThermo::calcVerticesArea(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	for(unsigned i=0; i<mesh->surfaceVertices.size(); i++){
		vertexVars[mesh->surfaceVertices[i]].area = 0; //これを求める
		for(unsigned j=0; j<mesh->vertices[mesh->surfaceVertices[i]].faceIDs.size(); ++j){
			if(mesh->vertices[mesh->surfaceVertices[i]].faceIDs[j] < (int) mesh->nSurfaceFace){ //もしfaceが表面の面であったら
				unsigned faceId = mesh->vertices[mesh->surfaceVertices[i]].faceIDs[j];
				vertexVars[mesh->surfaceVertices[i]].area += faceVars[faceId].area / 3 ;
			}
		}
	}
}
///*小野原追加ここまで--------------------------------------------*/

void PHFemThermo::CreateVecf2surface(unsigned id,unsigned num){
	PHFemMeshNew* mesh = GetPHFemMesh();

	// 初期化
	tetVars[id].vecf[1].clear();
	//l=0の時f21,1の時:f22, 2の時:f23, 3の時:f24	を生成
	///	..j番目の行列の成分が0のベクトルを作成
	for(unsigned l= 0 ; l < 4; l++){
		vecf2array[l] = Create41Vec1();
		vecf2array[l][l] = 0.0;			//	l行を0に
	}
	for(unsigned l= 0 ; l < 4; l++){
		///	四面体の各面(l = 0 ～ 3) についてメッシュ表面かどうかをチェックする。表面なら、行列を作ってvecf2arrayに入れる
		//faces[tets.faces[i]].sorted;		/// 1,24,58みたいな節点番号が入っている
		///	..行列型の入れ物を用意
		//faces[tets.faces[l]].vertices;
		if(mesh->tets[id].faceIDs[l] < (int)mesh->nSurfaceFace && faceVars[mesh->tets[id].faceIDs[l]].fluxarea > 0 ){			///	外殻の面 且つ 熱伝達率が更新されたら matk2を更新する必要がある
			///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
			if(faceVars[mesh->tets[id].faceIDs[l]].area ==0 || faceVars[mesh->tets[id].faceIDs[l]].deformed ){		///	面積が計算されていない時（はじめ） or deformed(変形した時・初期状態)がtrueの時		///	条件の追加	面積が0か ||(OR) αが更新されたか
				faceVars[mesh->tets[id].faceIDs[l]].area = CalcTriangleArea(mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]);
				faceVars[mesh->tets[id].faceIDs[l]].deformed = false;
			}
			///	計算結果を行列に代入
			///	areaの計算に使っていない点が入っている行を除いたベクトルの積をとる
			///	積分計算を根本から考える
			unsigned vtx = mesh->tets[id].vertexIDs[0] + mesh->tets[id].vertexIDs[1] + mesh->tets[id].vertexIDs[2] + mesh->tets[id].vertexIDs[3];			
			///	area計算に使われていない節点ID：ID
			unsigned ID = vtx -( mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2] );
			for(unsigned j=0;j<4;j++){
				if(mesh->tets[id].vertexIDs[j] == ID){					///	形状関数が１、（すなわち）このfaceに対面する頂点　と一致したら　その時のfaceで面積分する
					///	外殻にないメッシュ面の面積は0で初期化しておく
					///	以下の[]は上までの[l]と異なる。
					///	IDが何番目かによって、形状関数の係数が異なるので、
					for(unsigned i =0;i<3;++i){
						tetVars[id].vecf[1] += faceVars[mesh->tets[id].faceIDs[l]].heatflux[num][i] * (1.0/3.0) * faceVars[mesh->tets[id].faceIDs[l]].area * vecf2array[j];
					}
//					DSTR << vecf2array[j] << std::endl;
					//DSTR << "tets[id].matk2にfaces[tets[id].faces[l]].heatTransRatio * (1.0/12.0) * faces[tets[id].faces[l]].area * matk2array[" << j << "]"<< "を加算: " <<faces[tets[id].faces[l]].heatTransRatio * (1.0/12.0) * faces[tets[id].faces[l]].area * matk2array[j] << std::endl;
					//DSTR << "tets[id].matk2 +=  " << tets[id].matk2 << std::endl;
				}
				//else{
				//	///	IDと一致しない場合には、matk2array[j]には全成分0を入れる
				//	///	としたいところだが、
				//	//matk2array[j] =0.0 * matk2array[j];
				//	//DSTR << "matk2array[" << j << "]: " << matk2array[j] << std::endl;
				//}
			}
		}
		///	SurfaceFaceじゃなかったら、matk2arrayには0を入れる
		//else{
		//	//matk2array[l];
		//}
	}
}

void PHFemThermo::CreateVecf2surface(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	// 初期化
	tetVars[id].vecf[1].clear();
	//l=0の時f21,1の時:f22, 2の時:f23, 3の時:f24	を生成
	///	..j番目の行列の成分が0のベクトルを作成
	for(unsigned l= 0 ; l < 4; l++){
		vecf2array[l] = Create41Vec1();
		vecf2array[l][l] = 0.0;			//	l行を0に
	}
	for(unsigned l= 0 ; l < 4; l++){
		///	四面体の各面(l = 0 ～ 3) についてメッシュ表面かどうかをチェックする。表面なら、行列を作ってvecf2arrayに入れる
		//faces[tets.faces[i]].sorted;		/// 1,24,58みたいな節点番号が入っている
		///	..行列型の入れ物を用意
		//faces[tets.faces[l]].vertices;
		if(mesh->tets[id].faceIDs[l] < (int)mesh->nSurfaceFace && faceVars[mesh->tets[id].faceIDs[l]].fluxarea > 0 ){			///	外殻の面 且つ 熱伝達率が更新されたら matk2を更新する必要がある
			///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
			if(faceVars[mesh->tets[id].faceIDs[l]].area ==0 || faceVars[mesh->tets[id].faceIDs[l]].deformed ){		///	面積が計算されていない時（はじめ） or deformed(変形した時・初期状態)がtrueの時		///	条件の追加	面積が0か ||(OR) αが更新されたか
				faceVars[mesh->tets[id].faceIDs[l]].area = CalcTriangleArea(mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]);
				faceVars[mesh->tets[id].faceIDs[l]].deformed = false;
			}
			///	計算結果を行列に代入
			///	areaの計算に使っていない点が入っている行を除いたベクトルの積をとる
			unsigned vtx = mesh->tets[id].vertexIDs[0] + mesh->tets[id].vertexIDs[1] + mesh->tets[id].vertexIDs[2] + mesh->tets[id].vertexIDs[3];			
			///	area計算に使われていない節点ID：ID
			unsigned ID = vtx -( mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2] );
			for(unsigned j=0;j<4;j++){
				if(mesh->tets[id].vertexIDs[j] == ID){					///	形状関数が１、（すなわち）このfaceに対面する頂点　と一致したら　その時のfaceで面積分する
					///	以下の[]は上までの[l]と異なる。
					///	IDが何番目かによって、形状関数の係数が異なるので、
					//	複数の熱流束に対応:i
#ifdef DISABLE_COIL
					tetVars[id].vecf[1] += faceVars[mesh->tets[id].faceIDs[l]].heatflux[1][3] * (1.0/3.0) * faceVars[mesh->tets[id].faceIDs[l]].fluxarea[3] * vecf2array[j];	//+=:j=0~3のvecf2arrayを加算										
#else 
					for(unsigned i=0;i<3;++i){
						tets[id].vecf[1] += faces[tets[id].faces[l]].heatflux[1][i] * (1.0/3.0) * faces[tets[id].faces[l]].fluxarea[i] * vecf2array[j];	//+=:j=0~3のvecf2arrayを加算
					}
#endif
				}
			}
		}

		//槓子範囲内に入っていなければありうる↓消す
		//else if(tets[id].faces[l] < (int)nSurfaceFace && faces[tets[id].faces[l]].fluxarea <= 0 ){
		//	DSTR << "faces[tets[id].faces[l]].fluxareaが正しく計算されていません" <<std::endl;
		//}

		///	SurfaceFaceじゃなかったら、matk2arrayには0を入れる
		//else{
		//	//matk2array[l];
		//}
	}

	// 式の検算コード
	//double m1=0.0;
	//double m2=0.0;
	//double m3=0.0;
	//m1 = (vertices[tets[id].vertices[1]].pos.y - vertices[tets[id].vertices[0]].pos.y) * (vertices[tets[id].vertices[3]].pos.z - vertices[tets[id].vertices[0]].pos.z)
	//	- (vertices[tets[id].vertices[1]].pos.z - vertices[tets[id].vertices[0]].pos.z) * (vertices[tets[id].vertices[3]].pos.y - vertices[tets[id].vertices[0]].pos.y);
	//m2 = (vertices[tets[id].vertices[3]].pos.x - vertices[tets[id].vertices[0]].pos.x) * (vertices[tets[id].vertices[1]].pos.z - vertices[tets[id].vertices[0]].pos.z)
	//	- (vertices[tets[id].vertices[1]].pos.x - vertices[tets[id].vertices[0]].pos.x) * (vertices[tets[id].vertices[3]].pos.z - vertices[tets[id].vertices[0]].pos.z);
	//m3 = (vertices[tets[id].vertices[1]].pos.x - vertices[tets[id].vertices[0]].pos.x) * (vertices[tets[id].vertices[3]].pos.y - vertices[tets[id].vertices[0]].pos.y)
	//	- (vertices[tets[id].vertices[3]].pos.x - vertices[tets[id].vertices[0]].pos.x) * (vertices[tets[id].vertices[1]].pos.y - vertices[tets[id].vertices[0]].pos.y);
	//double sankaku = 0.0;
	//sankaku = m1 * m1 + m2 * m2 + m3 * m3;
	//sankaku = sqrt(sankaku) * 1/2;
	//double menseki = CalcTriangleArea(tets[id].vertices[0],tets[id].vertices[1],tets[id].vertices[3]);
	//DSTR << "sankaku "<<sankaku << ", menseki " << menseki <<std::endl;
	//int debughensu0=0;
}

void PHFemThermo::CreateVecf3surface(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	// 初期化
	tetVars[id].vecf[2].clear();
	double Tc[4];
	double TcAll =0;
	for(unsigned l=0;l<4;++l){
		TcAll += vertexVars[mesh->tets[id].vertexIDs[l]].Tc;
	}
	//l=0の時f21,1の時:f22, 2の時:f23, 3の時:f24	を生成
	///	..j番目の行列の成分が0のベクトルを作成
	for(unsigned l= 0 ; l < 4; l++){
		vecf3array[l] = Create41Vec1();
		vecf3array[l][l] = 0.0;			//	l行を0に
		Tc[l] =  (TcAll - vertexVars[mesh->tets[id].vertexIDs[l]].Tc) * 1.0/3.0;
	}
	for(unsigned l= 0 ; l < 4; l++){
		///	四面体の各面(l = 0 ～ 3) についてメッシュ表面かどうかをチェックする。表面なら、行列を作ってvecf2arrayに入れる
		//faces[tets.faces[i]].sorted;		/// 1,24,58みたいな節点番号が入っている
		if(mesh->tets[id].faceIDs[l] < (int)mesh->nSurfaceFace && faceVars[mesh->tets[id].faceIDs[l]].heatTransRatios.size() ){		///	外殻の面でのみ実行 && （１つ以上の）熱伝達率が設定されているとき
			///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
			if(faceVars[mesh->tets[id].faceIDs[l]].area ==0 || faceVars[mesh->tets[id].faceIDs[l]].deformed ){		///	面積が計算されていない時（はじめ） or deformed(変形した時・初期状態)がtrueの時		///	条件の追加	面積が0か ||(OR) αが更新されたか
				faceVars[mesh->tets[id].faceIDs[l]].area = CalcTriangleArea(mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]);
				faceVars[mesh->tets[id].faceIDs[l]].deformed = false;
			}
			///	areaの計算に使っていない点が入っている行を除いたベクトルの積をとる
			///	積分計算を根本から考える
			unsigned vtx = mesh->tets[id].vertexIDs[0] + mesh->tets[id].vertexIDs[1] + mesh->tets[id].vertexIDs[2] + mesh->tets[id].vertexIDs[3];			
			///	area計算に使われていない節点ID：ID
			unsigned ID = vtx -( mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2] );
			for(unsigned j=0;j<4;j++){
				if(mesh->tets[id].vertexIDs[j] == ID){					///	形状関数が１、（すなわち）このfaceに対面する頂点　と一致したら　その時のfaceで面積分する
					///	以下の[]は上までの[l]と異なる。
					///	IDが何番目かによって、形状関数の係数が異なるので、
					for(unsigned k =0 ; k < faceVars[mesh->tets[id].faceIDs[l]].heatTransRatios.size(); ++k){
						tetVars[id].vecf[2] +=  faceVars[mesh->tets[id].faceIDs[l]].heatTransRatios[k] * (1.0/3.0) * faceVars[mesh->tets[id].faceIDs[l]].area * vecf3array[j];	//+=:j=0~3のvecf2arrayを加算			
					}
				}
			}
		}
	}
}

void PHFemThermo::CreateVecf4surface(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	// 初期化
	tetVars[id].vecf[3].clear();
	//l=0の時f21,1の時:f22, 2の時:f23, 3の時:f24	を生成
	///	..j番目の行列の成分が0のベクトルを作成
	double Tout[4];
	double ToutAll =0;
	for(unsigned l=0;l<4;++l){
		ToutAll += vertexVars[mesh->tets[id].vertexIDs[l]].Tout;
	}
	for(unsigned l= 0 ; l < 4; l++){
		vecf4array[l] = Create41Vec1();
		vecf4array[l][l] = 0.0;			//	l行を0に
		Tout[l] =  (ToutAll - vertexVars[mesh->tets[id].vertexIDs[l]].Tout) * 1.0/3.0;
	}
	for(unsigned l= 0 ; l < 4; l++){
		///	四面体の各面(l = 0 ～ 3) についてメッシュ表面かどうかをチェックする。表面なら、行列を作ってvecf2arrayに入れる
		//faces[tets.faces[i]].sorted;		/// 1,24,58みたいな節点番号が入っている
		if(mesh->tets[id].faceIDs[l] < (int)mesh->nSurfaceFace){	// && faces[tets[id].faces[l]].thermalEmissivity != 0 ){		///	外殻の面でのみ実行 && （１つ以上の）熱伝達率が設定されているとき
			///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
			if(faceVars[mesh->tets[id].faceIDs[l]].area ==0 || faceVars[mesh->tets[id].faceIDs[l]].deformed ){		///	面積が計算されていない時（はじめ） or deformed(変形した時・初期状態)がtrueの時		///	条件の追加	面積が0か ||(OR) αが更新されたか
				faceVars[mesh->tets[id].faceIDs[l]].area = CalcTriangleArea(mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]);
				faceVars[mesh->tets[id].faceIDs[l]].deformed = false;
			}
			///	areaの計算に使っていない点が入っている行を除いたベクトルの積をとる
			///	積分計算を根本から考える
			unsigned vtx = mesh->tets[id].vertexIDs[0] + mesh->tets[id].vertexIDs[1] + mesh->tets[id].vertexIDs[2] + mesh->tets[id].vertexIDs[3];			
			///	area計算に使われていない節点ID：ID
			unsigned ID = vtx - ( mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2] );
			for(unsigned j=0;j<4;j++){
				if(mesh->tets[id].vertexIDs[j] == ID){					///	形状関数が１、（すなわち）このfaceに対面する頂点　と一致したら　その時のfaceで面積分する
					double ems_temp = ( 	
						vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].thermalEmissivity_const
						+ vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].thermalEmissivity_const
						+ vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].thermalEmissivity_const
						) / 3.0;
					///	以下の[]は上までの[l]と異なる。
					///	IDが何番目かによって、形状関数の係数が異なるので、
					tetVars[id].vecf[3] +=  ems_temp * Tout[j] * (1.0/3.0) * faceVars[mesh->tets[id].faceIDs[l]].area * vecf4array[j];	//+=:j=0~3のvecf2arrayを加算								
				}
			}
		}
	}
}

#if 0
void PHFemThermo::CreateVecF3surfaceAll(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	初期化
	//	弱火、中火、強火について初期化(ベクトルの行数設定、初期化)
	vecFAll_f3.resize(mesh->vertices.size(),1);			//表面だけでなく、全節点について計算しないと、ベクトル×行列の計算が不成立のため。
	vecFAll_f3.clear();
	
	//四面体要素ごとに行列を作り、どこかで合成する
	//idを入れて、再帰的に作っている
	for(unsigned id =0; id < mesh->tets.size();id++){ 
		//行列を作る
		CreateVecf3(id);//;		// f3surfaceではないけれど、いいのか？	//CreateVecf2surface(id,num);	//	この関数も、引数に指定したベクトルに入れられるようにする?
		for(unsigned j =0;j < 4; j++){
			int vtxid0 = mesh->tets[id].vertexIDs[j];
			//vecFAllSum[vtxid0] = vecf[j];			//全体剛性ベクトルを作成：ガウスザイデル計算内でやっている処理・これを行う。ここまでをVecf2でやる。
			vecFAll_f3[vtxid0][0] += tetVars[id].vecf[2][j];		//	+= じゃなくてもいいのか？同様に、元のソースでも += の必要があるのでは？
		}
	}
	//作った後に、ガウスザイデル計算で、VecFAllにセットする関数を作る。
	//vecFAllSumに加算とかすると、どんどん増えてしまうし、逆に、他の、変化しない要素{F_3}など、全体ベクトルも作って、保存しておく必要
	//ガウスザイデルの計算の中で、これまでの計算でFベクトルを使うのに代えて、マイステップで、VecFをF1,F2から作る必要がある。
}

void PHFemThermo::CreateVecF2surfaceAll(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	初期化
	//	弱火、中火、強火について初期化(ベクトルの行数設定、初期化) initVecFAlls()で実行
	//for(unsigned i =0; i < 4 ;i++){
	//	vecFAll_f2IH[i].resize(vertices.size(),1);			//表面だけでなく、全節点について計算しないと、ベクトル×行列の計算が不成立のため。
	//	vecFAll_f2IH[i].clear();
	//}
	
	//四面体要素ごとに行列を作り、どこかで合成する
	//idを入れて、再帰的に作っている
	for(unsigned id =0; id < mesh->tets.size();id++){ 
		//行列を作る
		for(unsigned num =0; num <4 ; num++){	//全火力(OFF/WEEK/MIDDLE/HIGH)について
			CreateVecf2surface(id,num);			//	この関数も、引数に指定したベクトルに入れられるようにする?
			//num毎に、入れ物に入れる。
			for(unsigned j =0;j < 4; j++){
				int vtxid0 = mesh->tets[id].vertexIDs[j];
				//vecFAllSum[vtxid0][0] = vecf[j];			//全体剛性ベクトルを作成：ガウスザイデル計算内でやっている処理・これを行う。ここまでをVecf2でやる。
				//vecFAll_f2IHw[vtxid0][0] = vecf[j];
				vecFAll_f2IH[num][vtxid0][0] += tetVars[id].vecf[1][j];		//f2の[num(火力)]	+= じゃなくてもいいのか？同様に、元のソースでも += の必要があるのでは？
			}
		}

		////要素の節点番号の場所に、その節点のfの値を入れる
		////j:要素の中の何番目か
		//for(unsigned j =0;j < 4; j++){
		//	int vtxid0 = tets[id].vertices[j]
		//	vecFAll_f2IHw[vtxid0][0] = vecf[j];tets[id].vecf[1]//のj要素
		//}

		//
		////入れ物に入れる
		////j:要素の中の何番目か
		//for(unsigned j =0;j < 4; j++){
		//	int vtxid0 = tets[id].vertices[j];
		//	vecFAllSum[vtxid0] = vecf[j];			//全体剛性ベクトルを作成：ガウスザイデル計算内でやっている処理・これを行う。ここまでをVecf2でやる。
		//	vecFAll_f2IHw[vtxid0][0] = vecf[j];
		//	
		//}

	//作った後に、ガウスザイデル計算で、vecFAllSumにセットする関数を作る。
	//vecFAllSumに加算とかすると、どんどん増えてしまうし、逆に、他の、変化しない要素{F_3}など、全体ベクトルも作って、保存しておく必要
	//ガウスザイデルの計算の中で、これまでの計算でFベクトルを使うのに代えて、マイステップで、VecFをF1,F2から作る必要がある。

	}

	//以下、CreateVecfLocalからコピペ　2012.9.25
	//すべての要素について係数行列を作る
	//f1を作る
	//>	熱流束境界条件	vecf2を作る			
}
#endif

//void PHFemThermo::CreateVecf2(unsigned id){
//	//	初期化
//	for(unsigned i =0; i < 4 ;i++){
//		//最後に入れる行列を初期化
//		tets[id].vecf[1][i] =0.0;				//>	f3 = vecf[1] 
//	}	
//	//l=0の時f31,1:f32, 2:f33, 3:f34	を生成
//	for(unsigned l= 0 ; l < 4; l++){
//		//matk2array[l] = matk2temp;
//		vecf2array[l] = Create41Vec1();
//		//	l行を0に
//		vecf2array[l][l] = 0.0;
//
//		//array[n][m][l]	= narray[n],m行l列
//		//	f_3	(vecf3array[0], vecf3array[1],..)
//		// =	| 0 | + | 1 |+...
//		//		| 1 |   | 0 |
//		//		| 1 |   | 1 |
//		//		| 1 |   | 1 |
//
//		//	for debug
//		//DSTR << "vecf3array[" << l << "] : " << std::endl;
//		//DSTR << vecf3array[l] << std::endl;
//
//		//係数の積をとる
//		//この節点で構成される四面体の面積の積をとる
//		//四面体の節点1,2,3(0以外)で作る三角形の面積
//		//l==0番目の時、 123	を代入する
//		//l==1			0 23
//		//l==2			01 3
//		//l==3			012
//		//をCalcTriangleAreaに入れることができるようにアルゴリズムを考える。
//
//		//>	CreateMatk2tのようなアルゴリズムに変更予定
//		//k21
//		if(l==0){
//			//>	三角形面を構成する3頂点の熱流束の相加平均
//			double tempHF = (vertices[tets[id].vertices[1]].heatFluxValue + vertices[tets[id].vertices[2]].heatFluxValue + vertices[tets[id].vertices[3]].heatFluxValue ) / 3.0;		//HTR:HeatTransRatio
//			vecf2array[l] = tempHF * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[1],tets[id].vertices[2],tets[id].vertices[3] ) * vecf2array[l];
//			//DSTR << "vecf2array[" << l << "] : " << vecf2array[l] << std::endl;
//			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
//			
//			////>	不要？
//			//for(unsigned m=0; m<4; m++){
//			//	vecf2array[l][m] = vertices[tets[id].vertices[m]].Tc * vecf2array[l][m];
//			//}
//		}
//		//	k22
//		else if(l==1){
//			double tempHF = (vertices[tets[id].vertices[0]].heatFluxValue + vertices[tets[id].vertices[2]].heatFluxValue + vertices[tets[id].vertices[3]].heatFluxValue ) / 3.0;		//HTR:HeatTransRatio
//			vecf2array[l] = tempHF * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[2],tets[id].vertices[3] ) * vecf2array[l];
//			//vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[2],tets[id].vertices[3] ) * vecf3array[l];
//			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
//			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
//			//for(unsigned m=0; m<4; m++){
//			//	vecf2array[l][m] = vertices[tets[id].vertices[m]].Tc * vecf2array[l][m];
//			//}
//		}
//		//	k23
//		else if(l==2){
//			double tempHF = (vertices[tets[id].vertices[0]].heatFluxValue + vertices[tets[id].vertices[1]].heatFluxValue + vertices[tets[id].vertices[3]].heatFluxValue ) / 3.0;		//HTR:HeatTransRatio
//			vecf2array[l] = tempHF * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[1],tets[id].vertices[3] ) * vecf2array[l];
//			//vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[1],tets[id].vertices[3] ) * vecf3array[l];
//			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
//			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
//			//for(unsigned m=0; m<4; m++){
//			//	vecf2array[l][m] = vertices[tets[id].vertices[m]].Tc * vecf2array[l][m];
//			//}
//		}
//		//	k24
//		else if(l==3){
//			double tempHF = (vertices[tets[id].vertices[0]].heatFluxValue + vertices[tets[id].vertices[1]].heatFluxValue + vertices[tets[id].vertices[2]].heatFluxValue ) / 3.0;		//HTR:HeatTransRatio
//			vecf2array[l] = tempHF * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[1],tets[id].vertices[2] ) * vecf2array[l];
//			//vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[1],tets[id].vertices[2] ) * vecf3array[l];
//			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
//			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
//			//for(unsigned m=0; m<4; m++){
//			//	vecf2array[l][m] = vertices[tets[id].vertices[m]].Tc * vecf2array[l][m];
//			//}
//		}
//		//for debug
//		//DSTR << "vecf3array[" << l << "]の完成版は↓" << std::endl;
//		//DSTR << vecf3array[l] << std::endl;
//		//if(dMatCAll == NULL){
//		//	//DSTR <<"i : "<< i << ", l : " << l << std::endl;
//		//	DSTR << "dMatCAll == NULL" <<std::endl;
//		//	DSTR <<"l : " << l << std::endl;
//		//}
//	}
//
//	//f3 = f31 + f32 + f33 + f34
//	for(unsigned i=0; i < 4; i++){
//		//vecf3 += vecf3array[i];
//		tets[id].vecf[1] += vecf2array[i];
//		//	for debug
//		//DSTR << "vecf3 に vecf3array = f3" << i+1 <<"まで加算した行列" << std::endl;
//		//DSTR << vecf3 << std::endl;
//	}
//	
//	//	f1,f2,f3,f4	を計算する際に、[0][0]成分から[3][0]成分までの非0成分について、先にTcをかけてしまう
//
//
//	//for debug
//	//DSTR << "節点（";
//	//for(unsigned i =0; i < 4; i++){
//	//	DSTR << tets[id].vertices[i] << "," ;
//	//}
//	//DSTR << ")で構成される四面体の" << std::endl;
//	//DSTR << "vecf3 : " << std::endl;
//	//DSTR << vecf3 << std::endl;
//	//int hogeshishi =0;
//}

void PHFemThermo::CreateVecf3_(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	初期化
	for(unsigned i =0; i < 4 ;i++){
		//vecf3[i] = 0.0;		//最後に入れる行列を初期化
		tetVars[id].vecf[2][i] =0.0;
	}	
	//l=0の時f31,1:f32, 2:f33, 3:f34	を生成
	for(unsigned l= 0 ; l < 4; l++){
		//matk2array[l] = matk2temp;
		vecf3array[l] = Create41Vec1();
		//	l行を0に
		//for(int i=0;i<4;i++){
		//	vecf3array[l][l][i] = 0.0;
		//}
		vecf3array[l][l] = 0.0;
		//array[n][m][l]	= narray[n],m行l列
		//	f_3	(vecf3array[0], vecf3array[1],..)
		// =	| 0 | + | 1 |+...
		//		| 1 |   | 0 |
		//		| 1 |   | 1 |
		//		| 1 |   | 1 |
		//	for debug
		//DSTR << "vecf3array[" << l << "] : " << std::endl;
		//DSTR << vecf3array[l] << std::endl;

		//係数の積をとる
		//この節点で構成される四面体の面積の積をとる
		//四面体の節点1,2,3(0以外)で作る三角形の面積
		//l==0番目の時、 123	を代入する
		//l==1			0 23
		//l==2			01 3
		//l==3			012
		//をCalcTriangleAreaに入れることができるようにアルゴリズムを考える。
		//k21
		if(l==0){
			//三角形面を構成する3頂点の熱伝達率の相加平均
			double tempHTR = (vertexVars[mesh->tets[id].vertexIDs[1]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[2]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[3]].heatTransRatio ) / 3.0;		//HTR:HeatTransRatio
			double avgTc = (vertexVars[mesh->tets[id].vertexIDs[1]].Tc + vertexVars[mesh->tets[id].vertexIDs[2]].Tc + vertexVars[mesh->tets[id].vertexIDs[3]].Tc ) / 3.0;
			vecf3array[l] = tempHTR * avgTc * (1.0/3.0) * CalcTriangleArea( mesh->tets[id].vertexIDs[1],mesh->tets[id].vertexIDs[2],mesh->tets[id].vertexIDs[3] ) * vecf3array[l];
			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
			
			//>	↓は本当？
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			//for(unsigned m=0; m<4; m++){
			//	vecf3array[l][m] = vertices[tets[id].vertices[m]] * vecf3array[l][m];
			//}
		}
		//	k22
		else if(l==1){
			double tempHTR = (vertexVars[mesh->tets[id].vertexIDs[0]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[2]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[3]].heatTransRatio ) / 3.0;		//HTR:HeatTransRatio
			double avgTc = (vertexVars[mesh->tets[id].vertexIDs[0]].Tc + vertexVars[mesh->tets[id].vertexIDs[2]].Tc + vertexVars[mesh->tets[id].vertexIDs[3]].Tc ) / 3.0;		//HTR:HeatTransRatio
			vecf3array[l] = tempHTR * avgTc * (1.0/3.0) * CalcTriangleArea( mesh->tets[id].vertexIDs[0],mesh->tets[id].vertexIDs[2],mesh->tets[id].vertexIDs[3] ) * vecf3array[l];
			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			//for(unsigned m=0; m<4; m++){
			//	vecf3array[l][m] = vertices[tets[id].vertices[m]].Tc * vecf3array[l][m];
			//}
		}
		//	k23
		else if(l==2){
			double tempHTR = (vertexVars[mesh->tets[id].vertexIDs[0]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[1]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[3]].heatTransRatio ) / 3.0;		//HTR:HeatTransRatio
			double avgTc = (vertexVars[mesh->tets[id].vertexIDs[0]].Tc + vertexVars[mesh->tets[id].vertexIDs[1]].Tc + vertexVars[mesh->tets[id].vertexIDs[3]].Tc ) / 3.0;		//HTR:HeatTransRatio
			vecf3array[l] = tempHTR * avgTc * (1.0/3.0) * CalcTriangleArea( mesh->tets[id].vertexIDs[0],mesh->tets[id].vertexIDs[1],mesh->tets[id].vertexIDs[3] ) * vecf3array[l];
			//vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[1],tets[id].vertices[3] ) * vecf3array[l];
			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			//for(unsigned m=0; m<4; m++){
			//	vecf3array[l][m] = vertices[tets[id].vertices[m]].Tc * vecf3array[l][m];
			//}
		}
		//	k24
		else if(l==3){
			double tempHTR = (vertexVars[mesh->tets[id].vertexIDs[0]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[1]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[2]].heatTransRatio ) / 3.0;		//HTR:HeatTransRatio
			double avgTc = (vertexVars[mesh->tets[id].vertexIDs[0]].Tc + vertexVars[mesh->tets[id].vertexIDs[1]].Tc + vertexVars[mesh->tets[id].vertexIDs[2]].Tc ) / 3.0;		//HTR:HeatTransRatio
			vecf3array[l] = tempHTR * avgTc * (1.0/3.0) * CalcTriangleArea( mesh->tets[id].vertexIDs[0],mesh->tets[id].vertexIDs[1],mesh->tets[id].vertexIDs[2] ) * vecf3array[l];
			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			//for(unsigned m=0; m<4; m++){
			//	vecf3array[l][m] = vertices[tets[id].vertices[m]].Tc * vecf3array[l][m];
			//}
		}
		//for debug
		//DSTR << "vecf3array[" << l << "]の完成版は↓" << std::endl;
		//DSTR << vecf3array[l] << std::endl;
		//if(dMatCAll == NULL){
		//	//DSTR <<"i : "<< i << ", l : " << l << std::endl;
		//	DSTR << "dMatCAll == NULL" <<std::endl;
		//	DSTR <<"l : " << l << std::endl;
		//}
	}

	//f3 = f31 + f32 + f33 + f34
	for(unsigned i=0; i < 4; i++){
		//vecf3 += vecf3array[i];
		tetVars[id].vecf[2] += vecf3array[i];
		//	for debug
		//DSTR << "vecf3 に vecf3array = f3" << i+1 <<"まで加算した行列" << std::endl;
		//DSTR << vecf3 << std::endl;
	}
	
	//	f1,f2,f3,f4	を計算する際に、[0][0]成分から[3][0]成分までの非0成分について、先にTcをかけてしまう


	//for debug
	//DSTR << "節点（";
	//for(unsigned i =0; i < 4; i++){
	//	DSTR << tets[id].vertices[i] << "," ;
	//}
	//DSTR << ")で構成される四面体の" << std::endl;
	//DSTR << "vecf3 : " << std::endl;
	//DSTR << vecf3 << std::endl;
	//int hogeshishi =0;
}

void PHFemThermo::CreateVecf3(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	初期化
	for(unsigned i =0; i < 4 ;i++){
		//vecf3[i] = 0.0;		//最後に入れる行列を初期化
		tetVars[id].vecf[2][i] =0.0;
	}	
	//l=0の時f31,1:f32, 2:f33, 3:f34	を生成
	for(unsigned l= 0 ; l < 4; l++){
		//matk2array[l] = matk2temp;
		vecf3array[l] = Create41Vec1();
		//	l行を0に
		//for(int i=0;i<4;i++){
		//	vecf3array[l][l][i] = 0.0;
		//}
		vecf3array[l][l] = 0.0;
		//array[n][m][l]	= narray[n],m行l列
		//	f_3	(vecf3array[0], vecf3array[1],..)
		// =	| 0 | + | 1 |+...
		//		| 1 |   | 0 |
		//		| 1 |   | 1 |
		//		| 1 |   | 1 |
		//	for debug
		//DSTR << "vecf3array[" << l << "] : " << std::endl;
		//DSTR << vecf3array[l] << std::endl;

		//係数の積をとる
		//この節点で構成される四面体の面積の積をとる
		//四面体の節点1,2,3(0以外)で作る三角形の面積
		//l==0番目の時、 123	を代入する
		//l==1			0 23
		//l==2			01 3
		//l==3			012
		//をCalcTriangleAreaに入れることができるようにアルゴリズムを考える。
		//k21
		if(l==0){
			//三角形面を構成する3頂点の熱伝達率の相加平均
			double tempHTR = (vertexVars[mesh->tets[id].vertexIDs[1]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[2]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[3]].heatTransRatio ) / 3.0;		//HTR:HeatTransRatio
			vecf3array[l] = tempHTR * (1.0/3.0) * CalcTriangleArea( mesh->tets[id].vertexIDs[1],mesh->tets[id].vertexIDs[2],mesh->tets[id].vertexIDs[3] ) * vecf3array[l];
			//vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets.vertices[1],tets.vertices[2],tets.vertices[3] ) * vecf3array[l];
			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			for(unsigned m=0; m<4; m++){
				vecf3array[l][m] = vertexVars[mesh->tets[id].vertexIDs[m]].Tc * vecf3array[l][m];
			}
		}
		//	k22
		else if(l==1){
			double tempHTR = (vertexVars[mesh->tets[id].vertexIDs[0]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[2]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[3]].heatTransRatio ) / 3.0;		//HTR:HeatTransRatio
			vecf3array[l] = tempHTR * (1.0/3.0) * CalcTriangleArea( mesh->tets[id].vertexIDs[0],mesh->tets[id].vertexIDs[2],mesh->tets[id].vertexIDs[3] ) * vecf3array[l];
			//vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[2],tets[id].vertices[3] ) * vecf3array[l];
			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			for(unsigned m=0; m<4; m++){
				vecf3array[l][m] = vertexVars[mesh->tets[id].vertexIDs[m]].Tc * vecf3array[l][m];
			}
		}
		//	k23
		else if(l==2){
			double tempHTR = (vertexVars[mesh->tets[id].vertexIDs[0]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[1]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[3]].heatTransRatio ) / 3.0;		//HTR:HeatTransRatio
			vecf3array[l] = tempHTR * (1.0/3.0) * CalcTriangleArea( mesh->tets[id].vertexIDs[0],mesh->tets[id].vertexIDs[1],mesh->tets[id].vertexIDs[3] ) * vecf3array[l];
			//vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[1],tets[id].vertices[3] ) * vecf3array[l];
			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			for(unsigned m=0; m<4; m++){
				vecf3array[l][m] = vertexVars[mesh->tets[id].vertexIDs[m]].Tc * vecf3array[l][m];
			}
		}
		//	k24
		else if(l==3){
			double tempHTR = (vertexVars[mesh->tets[id].vertexIDs[0]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[1]].heatTransRatio + vertexVars[mesh->tets[id].vertexIDs[2]].heatTransRatio ) / 3.0;		//HTR:HeatTransRatio
			vecf3array[l] = tempHTR * (1.0/3.0) * CalcTriangleArea( mesh->tets[id].vertexIDs[0],mesh->tets[id].vertexIDs[1],mesh->tets[id].vertexIDs[2] ) * vecf3array[l];
			//vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets[id].vertices[0],tets[id].vertices[1],tets[id].vertices[2] ) * vecf3array[l];
			//DSTR << "vecf3array[" << l << "] : " << vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			for(unsigned m=0; m<4; m++){
				vecf3array[l][m] = vertexVars[mesh->tets[id].vertexIDs[m]].Tc * vecf3array[l][m];
			}
		}
		//for debug
		//DSTR << "vecf3array[" << l << "]の完成版は↓" << std::endl;
		//DSTR << vecf3array[l] << std::endl;
		//if(dMatCAll == NULL){
		//	//DSTR <<"i : "<< i << ", l : " << l << std::endl;
		//	DSTR << "dMatCAll == NULL" <<std::endl;
		//	DSTR <<"l : " << l << std::endl;
		//}
	}

	//f3 = f31 + f32 + f33 + f34
	for(unsigned i=0; i < 4; i++){
		//vecf3 += vecf3array[i];
		tetVars[id].vecf[2] += vecf3array[i];
		//	for debug
		//DSTR << "vecf3 に vecf3array = f3" << i+1 <<"まで加算した行列" << std::endl;
		//DSTR << vecf3 << std::endl;
	}
	
	//	f1,f2,f3,f4	を計算する際に、[0][0]成分から[3][0]成分までの非0成分について、先にTcをかけてしまう


	//for debug
	//DSTR << "節点（";
	//for(unsigned i =0; i < 4; i++){
	//	DSTR << tets[id].vertices[i] << "," ;
	//}
	//DSTR << ")で構成される四面体の" << std::endl;
	//DSTR << "vecf3 : " << std::endl;
	//DSTR << vecf3 << std::endl;
	//int hogeshishi =0;
}

double PHFemThermo::CalcTetrahedraVolume2(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	PTM::TMatrixRow<4,4,double> tempMat44;
	tempMat44.clear();
	for(unsigned i =0; i < 4; i++){
		for(unsigned j =0; j < 4; j++){
			if(i == 0){
				tempMat44[i][j] = 1.0;
			}
			else if(i == 1){
					tempMat44[i][j] = mesh->vertices[mesh->tets[id].vertexIDs[j]].pos.x;
			}
			else if(i == 2){
					tempMat44[i][j] = mesh->vertices[mesh->tets[id].vertexIDs[j]].pos.y;
			}
			else if(i == 3){
					tempMat44[i][j] = mesh->vertices[mesh->tets[id].vertexIDs[j]].pos.z;
			}

			
		}
	}
	//	for debug
	//DSTR << tempMat44 << std::endl;
	//for(unsigned i =0; i < 4 ;i++){
	//	DSTR << vertices[tets.vertices[i]].pos.x << " , " << vertices[tets.vertices[i]].pos.y << " , " << vertices[tets.vertices[i]].pos.z << std::endl; 
	//}
	//DSTR << tempMat44.det() << std::endl;
	//int hogever = 0;
	return tempMat44.det() / 6.0;
}

double PHFemThermo::CalcTetrahedraVolume(FemTet tet){
	PHFemMeshNew* mesh = GetPHFemMesh();

	PTM::TMatrixRow<4,4,double> tempMat44;
	for(unsigned i =0; i < 4; i++){
		for(unsigned j =0; j < 4; j++){
			if(i == 0){
				tempMat44[i][j] = 1.0;
			}
			else if(i == 1){
				tempMat44[i][j] = mesh->vertices[tet.vertexIDs[j]].pos.x;
			}
			else if(i == 2){
				tempMat44[i][j] = mesh->vertices[tet.vertexIDs[j]].pos.y;
			}
			else if(i == 3){
				tempMat44[i][j] = mesh->vertices[tet.vertexIDs[j]].pos.z;
			}
			
		}
	}
	//	for debug
	//DSTR << tempMat44 << std::endl;
	//for(unsigned i =0; i < 4 ;i++){
	//	DSTR << vertices[tets.vertices[i]].pos.x << " , " << vertices[tets.vertices[i]].pos.y << " , " << vertices[tets.vertices[i]].pos.z << std::endl; 
	//}
	//DSTR << tempMat44.det() << std::endl;
	//int hogever = 0;
	return tempMat44.det() / 6.0;
}

double PHFemThermo::CalcTriangleArea(int id0, int id1, int id2){
	PHFemMeshNew* mesh = GetPHFemMesh();

	double area=0.0;								///	要改善	faces[id].areaに値を入れる 

	//行列式の成分を用いて面積を求める
	//triarea =
	//|  1     1     1   |
	//|x2-x1 y2-y1 z2-z1 |
	//|x3-x1 y3-y1 z3-z1 |
	//|
	PTM::TMatrixRow<3,3,double> triarea;		//三角形の面積　= tri + area
	for(unsigned i =0 ; i < 3 ; i++){
		triarea[0][i] = 1.0;
	}
	for(unsigned i =0 ; i < 3 ; i++){
		//					x2(節点2のx(pos第i成分)目)	-	x1(〃)
		// i==0の時	vertices[id1].pos[i]	=>	 pos[0] == pos.x
		triarea[1][i] = mesh->vertices[id1].pos[i] - mesh->vertices[id0].pos[i];
	}
	for(unsigned i =0 ; i < 3 ; i++){
		//					x3(節点3のx(pos第i成分)目)	-	x1(〃)
		triarea[2][i] = mesh->vertices[id2].pos[i] - mesh->vertices[id0].pos[i];
	}
	double m1,m2,m3 = 0.0;
	m1 = triarea[1][1] * triarea[2][2] - triarea[1][2] * triarea[2][1];
	m2 = triarea[2][0] * triarea[1][2] - triarea[1][0] * triarea[2][2];
	m3 = triarea[1][0] * triarea[2][1] - triarea[2][0] * triarea[1][1];

	area = sqrt(m1 * m1 + m2 * m2 + m3 * m3) / 2.0;

	//	for debug
	//DSTR << "三角形の面積は : " << area << std::endl; 

	//0番目の節点は40,1番目の節点は134,2番目の節点は79 の座標で計算してみた
	//三角形を求める行列 : 2.75949e-005 * 1 = 2.75949 × 10-5(byGoogle計算機) [m^2] = 2.75949 × 10-1 [cm^2]なので、ネギのメッシュのスケールなら大体あっているはず

	return area;
}

PTM::TMatrixCol<4,1,double> PHFemThermo::Create41Vec1(){
	PTM::TMatrixCol<4,1,double> Mat1temp;
	for(int i =0; i <4 ; i++){
				Mat1temp[i][0] = 1.0;
	}
	return Mat1temp;
}

PTM::TMatrixRow<4,4,double> PHFemThermo::Create44Mat21(){
	//|2 1 1 1 |
	//|1 2 1 1 |
	//|1 1 2 1 |
	//|1 1 1 2 |	を作る
	PTM::TMatrixRow<4,4,double> MatTemp;
	for(int i =0; i <4 ; i++){
		for(int j=0; j < 4 ; j++){
			if(i==j){
				MatTemp[i][j] = 2.0;
			}else{
				MatTemp[i][j] = 1.0;
			}
		}
	}
	return MatTemp;
}

void PHFemThermo::ActivateVtxbeRadiantHeat(){
	for(int i=0;i<NSurfaceVertices();++i){
		vertexVars[i].beRadiantHeat =true;
	}
}


void PHFemThermo::CreateMatk3t_nonRadiantHeat(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//l=0の時k21,1の時:k22, 2の時:k23, 3の時:k24	を生成
	for(unsigned l= 0 ; l < 4; l++){
		//matk2array[l] = matk2temp;
		matk3array[l] = Create44Mat21();
		//	1行i列を0に
		for(int i=0;i<4;i++){
			matk3array[l][l][i] = 0.0;
		}
		//	i行1列を0に
		for(int i=0;i<4;i++){
			matk3array[l][i][l] = 0.0;
		}
	}
	///	初期化
	tetVars[id].matk[2].clear();
	for(unsigned l= 0 ; l < 4; l++){
		///	四面体の各面(l = 0 ～ 3) についてメッシュ表面かどうかをチェックする。表面なら、行列を作ってmatk2arrayに入れる
		//faces[tets.faces[i]].sorted;		/// 1,24,58みたいな節点番号が入っている

		///	外殻の面 且つ 熱伝達率が更新されたら matk2を更新する必要がある	//2014.3.12加藤追記	熱伝達頂点を含むfaceでは、行列に参入しない
		if(mesh->tets[id].faceIDs[l] < (int)mesh->nSurfaceFace && faceVars[mesh->tets[id].faceIDs[l]].alphaUpdated){
			if(vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].beRadiantHeat && vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].beRadiantHeat 
				&& vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].beRadiantHeat	){

				//最後に入れる行列を初期化
				for(unsigned i =0; i < 4 ;i++){
					for(unsigned j =0; j < 4 ;j++){
						tetVars[id].matk[2][i][j] = 0.0;
					}
				}
				///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
				if(faceVars[mesh->tets[id].faceIDs[l]].area ==0 || faceVars[mesh->tets[id].faceIDs[l]].deformed ){		///	面積が計算されていない時（はじめ） or deformed(変形した時・初期状態)がtrueの時		///	条件の追加	面積が0か ||(OR) αが更新されたか
					faceVars[mesh->tets[id].faceIDs[l]].area = CalcTriangleArea(mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]);
					faceVars[mesh->tets[id].faceIDs[l]].deformed = false;
				}
				///	計算結果を行列に代入
				///	areaの計算に使っていない点が入っている行と列を除いた行列の積をとる
				///	積分計算を根本から考える
				unsigned vtx = mesh->tets[id].vertexIDs[0] + mesh->tets[id].vertexIDs[1] + mesh->tets[id].vertexIDs[2] + mesh->tets[id].vertexIDs[3];
				unsigned ID = vtx -( mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2] );
				for(unsigned j=0;j<4;j++){
					if(mesh->tets[id].vertexIDs[j] == ID){					///	形状関数が１、（すなわち）このfaceに対面する頂点　と一致したら　その時のfaceで面積分する
						///	j番目の行列の成分を0にしたmatk2arrayで計算する
						///	外殻にないメッシュ面の面積は0で初期化しておく
						//faces[tets[id].faces[l]].thermalEmissivity = 
						double ems_temp = ( 	
							vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].thermalEmissivity_const
							+ vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].thermalEmissivity_const
							+ vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].thermalEmissivity_const
							) / 3.0;		///	当該faceの熱伝達率を構成節点での値の相加平均をとる
						///	以下の[]は上までの[l]と異なる。
						///	IDが何番目かによって、形状関数の係数が異なるので、
						tetVars[id].matk[2] += ems_temp * (1.0/12.0) * faceVars[mesh->tets[id].faceIDs[l]].area * matk3array[j];
					}
				}
			}
		}
	}

}

void PHFemThermo::CreateMatk3t(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//l=0の時k21,1の時:k22, 2の時:k23, 3の時:k24	を生成
	for(unsigned l= 0 ; l < 4; l++){
		//matk2array[l] = matk2temp;
		matk3array[l] = Create44Mat21();
		//	1行i列を0に
		for(int i=0;i<4;i++){
			matk3array[l][l][i] = 0.0;
		}
		//	i行1列を0に
		for(int i=0;i<4;i++){
			matk3array[l][i][l] = 0.0;
		}
	}
	///	初期化
	tetVars[id].matk[2].clear();
	for(unsigned l= 0 ; l < 4; l++){
		///	四面体の各面(l = 0 ～ 3) についてメッシュ表面かどうかをチェックする。表面なら、行列を作ってmatk2arrayに入れる
		//faces[tets.faces[i]].sorted;		/// 1,24,58みたいな節点番号が入っている
		if(mesh->tets[id].faceIDs[l] < (int)mesh->nSurfaceFace && faceVars[mesh->tets[id].faceIDs[l]].alphaUpdated ){			///	外殻の面 且つ 熱伝達率が更新されたら matk2を更新する必要がある
			//最後に入れる行列を初期化
			for(unsigned i =0; i < 4 ;i++){
				for(unsigned j =0; j < 4 ;j++){
					tetVars[id].matk[2][i][j] = 0.0;
				}
			}
			///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
			if(faceVars[mesh->tets[id].faceIDs[l]].area ==0 || faceVars[mesh->tets[id].faceIDs[l]].deformed ){		///	面積が計算されていない時（はじめ） or deformed(変形した時・初期状態)がtrueの時		///	条件の追加	面積が0か ||(OR) αが更新されたか
				faceVars[mesh->tets[id].faceIDs[l]].area = CalcTriangleArea(mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]);
				faceVars[mesh->tets[id].faceIDs[l]].deformed = false;
			}
			///	計算結果を行列に代入
			///	areaの計算に使っていない点が入っている行と列を除いた行列の積をとる
			///	積分計算を根本から考える
			unsigned vtx = mesh->tets[id].vertexIDs[0] + mesh->tets[id].vertexIDs[1] + mesh->tets[id].vertexIDs[2] + mesh->tets[id].vertexIDs[3];
			unsigned ID = vtx -( mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2] );
			for(unsigned j=0;j<4;j++){
				if(mesh->tets[id].vertexIDs[j] == ID){					///	形状関数が１、（すなわち）このfaceに対面する頂点　と一致したら　その時のfaceで面積分する
					///	j番目の行列の成分を0にしたmatk2arrayで計算する
					///	外殻にないメッシュ面の面積は0で初期化しておく
					//faces[tets[id].faces[l]].thermalEmissivity = 
					double ems_temp = ( 	
						vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].thermalEmissivity_const
						+ vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].thermalEmissivity_const
						+ vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].thermalEmissivity * vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].temp + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].thermalEmissivity_const
						) / 3.0;		///	当該faceの熱伝達率を構成節点での値の相加平均をとる
					///	以下の[]は上までの[l]と異なる。
					///	IDが何番目かによって、形状関数の係数が異なるので、
					tetVars[id].matk[2] += ems_temp * (1.0/12.0) * faceVars[mesh->tets[id].faceIDs[l]].area * matk3array[j];
				}
			}
		}
	}
}

void PHFemThermo::CreateMatk2t(unsigned id){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//l=0の時k21,1の時:k22, 2の時:k23, 3の時:k24	を生成
	for(unsigned l= 0 ; l < 4; l++){
		//matk2array[l] = matk2temp;
		matk2array[l] = Create44Mat21();
		//	1行i列を0に
		for(int i=0;i<4;i++){
			matk2array[l][l][i] = 0.0;
		}
		//	i行1列を0に
		for(int i=0;i<4;i++){
			matk2array[l][i][l] = 0.0;
		}
	}

	///	初期化
	tetVars[id].matk[1].clear();
	//for(unsigned i =0; i < 4 ;i++){
	//	for(unsigned j =0; j < 4 ;j++){
	//		//tets[id].matk2[i][j] = 0.0;
	//		tets[id].matk[1][i][j] = 0.0;
	//	}
	//}

	//	Check
	//DSTR << "matk2array:" << std::endl;
	//for(unsigned i=0;i<4;i++){
	//	DSTR <<i <<": " << matk2array[i] << std::endl;
	//}
	//DSTR << "++i" <<std::endl;
	//for(unsigned i=0;i<4;++i){
	//	DSTR <<i <<": " << matk2array[i] << std::endl;
	//}


	for(unsigned l= 0 ; l < 4; l++){
		///	四面体の各面(l = 0 ～ 3) についてメッシュ表面かどうかをチェックする。表面なら、行列を作ってmatk2arrayに入れる
		//faces[tets.faces[i]].sorted;		/// 1,24,58みたいな節点番号が入っている
		///	行列型の入れ物を用意

		//faces[tets.faces[l]].vertices;
		if(mesh->tets[id].faceIDs[l] < (int)mesh->nSurfaceFace && faceVars[mesh->tets[id].faceIDs[l]].alphaUpdated ){			///	外殻の面 且つ 熱伝達率が更新されたら matk2を更新する必要がある
			///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
			if(faceVars[mesh->tets[id].faceIDs[l]].area ==0 || faceVars[mesh->tets[id].faceIDs[l]].deformed ){		///	面積が計算されていない時（はじめ） or deformed(変形した時・初期状態)がtrueの時		///	条件の追加	面積が0か ||(OR) αが更新されたか
				faceVars[mesh->tets[id].faceIDs[l]].area = CalcTriangleArea(mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1], mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]);
				faceVars[mesh->tets[id].faceIDs[l]].deformed = false;
			}
			///	計算結果を行列に代入
			///	areaの計算に使っていない点が入っている行と列を除いた行列の積をとる
			///	積分計算を根本から考える
			unsigned vtx = mesh->tets[id].vertexIDs[0] + mesh->tets[id].vertexIDs[1] + mesh->tets[id].vertexIDs[2] + mesh->tets[id].vertexIDs[3];
			//DSTR << "vtx: " << vtx <<std::endl;
			
			///	area計算に使われていない節点ID：ID
			unsigned ID = vtx -( mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1] + mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2] );
			//DSTR << "メッシュ表面の面は次の3頂点からなる。" << std::endl;
			//DSTR << "faces[tets.faces[l]].vertices[0]: " << faces[tets.faces[l]].vertices[0] <<std::endl;
			//DSTR << "faces[tets.faces[l]].vertices[1]: " << faces[tets.faces[l]].vertices[1] <<std::endl;
			//DSTR << "faces[tets.faces[l]].vertices[2]: " << faces[tets.faces[l]].vertices[2] <<std::endl;
			//DSTR << "ID: " << ID <<"のときの節点と対面する面で面積分を計算する"<<std::endl;
			for(unsigned j=0;j<4;j++){
				if(mesh->tets[id].vertexIDs[j] == ID){					///	形状関数が１、（すなわち）このfaceに対面する頂点　と一致したら　その時のfaceで面積分する
					///	j番目の行列の成分を0にしたmatk2arrayで計算する
					///	外殻にないメッシュ面の面積は0で初期化しておく
					faceVars[mesh->tets[id].faceIDs[l]].heatTransRatio = (vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[0]].heatTransRatio + vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[1]].heatTransRatio 
						+ vertexVars[mesh->faces[mesh->tets[id].faceIDs[l]].vertexIDs[2]].heatTransRatio ) / 3.0;		///	当該faceの熱伝達率を構成節点での値の相加平均をとる
					///	以下の[]は上までの[l]と異なる。
					///	IDが何番目かによって、形状関数の係数が異なるので、
					tetVars[id].matk[1] += faceVars[mesh->tets[id].faceIDs[l]].heatTransRatio * (1.0/12.0) * faceVars[mesh->tets[id].faceIDs[l]].area * matk2array[j];		//元はtets[id].matk2 +=
					//DSTR << "tets[id].matk2にfaces[tets[id].faces[l]].heatTransRatio * (1.0/12.0) * faces[tets[id].faces[l]].area * matk2array[" << j << "]"<< "を加算: " <<faces[tets[id].faces[l]].heatTransRatio * (1.0/12.0) * faces[tets[id].faces[l]].area * matk2array[j] << std::endl;
					//DSTR << "tets[id].matk2 +=  " << tets[id].matk2 << std::endl;
				}
				//else{
				//	///	IDと一致しない場合には、matk2array[j]には全成分0を入れる
				//	///	としたいところだが、
				//	//matk2array[j] =0.0 * matk2array[j];
				//	//DSTR << "matk2array[" << j << "]: " << matk2array[j] << std::endl;
				//}
			}
		}
		///	SurfaceFaceじゃなかったら、matk2arrayには0を入れる
		//else{
		//	//matk2array[l];
		//}
	}

	//DSTR << "matk2array:" << std::endl;
	//for(unsigned i=0;i<4;i++){
	//	DSTR <<i <<": " << matk2array[i] << std::endl;
	//}

	//k2 = k21 + k22 + k23 + k24
	//for(unsigned i=0; i < 4; i++){
	//	matk2 += matk2array[i];
	//	//	for debug
	//	//DSTR << "matk2 に matk2array = k2" << i+1 <<"まで加算した行列" << std::endl;
	//	//DSTR << matk2 << std::endl;
	//}
	
	//for debug
	//DSTR << "節点（";
	//for(unsigned i =0; i < 4; i++){
	//	DSTR << tets[id].vertices[i] << "," ;
	//}
	//DSTR << ")で構成される四面体の" << std::endl;
	//DSTR << "matk2 : " << std::endl;
	//DSTR << matk2 << std::endl;
	//int hogeshishi =0;
	
	//DSTR << "Inner Function _ matk2t tets[id].matk2: " <<tets[id].matk2 << std::endl;
}

//void PHFemThermo::CreateMatk2t_(unsigned id){
//
//	//l=0の時k21,1の時:k22, 2の時:k23, 3の時:k24	を生成
//	for(unsigned l= 0 ; l < 4; l++){
//		//matk2array[l] = matk2temp;
//		matk2array[l] = Create44Mat21();
//		//	1行i列を0に
//		for(int i=0;i<4;i++){
//			matk2array[l][l][i] = 0.0;
//		}
//		//	i行1列を0に
//		for(int i=0;i<4;i++){
//			matk2array[l][i][l] = 0.0;
//		}
//	}
//
//	///	初期化
//	tets[id].matk[1].clear();
//	//for(unsigned i =0; i < 4 ;i++){
//	//	for(unsigned j =0; j < 4 ;j++){
//	//		//tets[id].matk2[i][j] = 0.0;
//	//		tets[id].matk[1][i][j] = 0.0;
//	//	}
//	//}
//
//	//	Check
//	//DSTR << "matk2array:" << std::endl;
//	//for(unsigned i=0;i<4;i++){
//	//	DSTR <<i <<": " << matk2array[i] << std::endl;
//	//}
//	//DSTR << "++i" <<std::endl;
//	//for(unsigned i=0;i<4;++i){
//	//	DSTR <<i <<": " << matk2array[i] << std::endl;
//	//}
//
//
//	for(unsigned l= 0 ; l < 4; l++){
//		///	四面体の各面(l = 0 ～ 3) についてメッシュ表面かどうかをチェックする。表面なら、行列を作ってmatk2arrayに入れる
//		//faces[tets.faces[i]].sorted;		/// 1,24,58みたいな節点番号が入っている
//		///	行列型の入れ物を用意
//
//		//faces[tets.faces[l]].vertices;
//		if(tets[id].faces[l] < (int)nSurfaceFace && faces[tets[id].faces[l]].alphaUpdated ){			///	外殻の面 且つ 熱伝達率が更新されたら matk2を更新する必要がある
//			//最後に入れる行列を初期化
//			for(unsigned i =0; i < 4 ;i++){
//				for(unsigned j =0; j < 4 ;j++){
//					//matk2[i][j] = 0.0;
//					//tets[id].matk2[i][j] = 0.0;
//					tets[id].matk[1][i][j] = 0.0;
//				}
//			}
//			///	四面体の三角形の面積を計算		///	この関数の外で面積分の面積計算を実装する。移動する
//			if(faces[tets[id].faces[l]].area ==0 || faces[tets[id].faces[l]].deformed ){		///	面積が計算されていない時（はじめ） or deformed(変形した時・初期状態)がtrueの時		///	条件の追加	面積が0か ||(OR) αが更新されたか
//				faces[tets[id].faces[l]].area = CalcTriangleArea(faces[tets[id].faces[l]].vertices[0], faces[tets[id].faces[l]].vertices[1], faces[tets[id].faces[l]].vertices[2]);
//				faces[tets[id].faces[l]].deformed = false;
//			}
//			///	計算結果を行列に代入
//			///	areaの計算に使っていない点が入っている行と列を除いた行列の積をとる
//			///	積分計算を根本から考える
//			unsigned vtx = tets[id].vertices[0] + tets[id].vertices[1] + tets[id].vertices[2] + tets[id].vertices[3];
//			//DSTR << "vtx: " << vtx <<std::endl;
//			
//			///	area計算に使われていない節点ID：ID
//			unsigned ID = vtx -( faces[tets[id].faces[l]].vertices[0] + faces[tets[id].faces[l]].vertices[1] + faces[tets[id].faces[l]].vertices[2] );
//			//DSTR << "メッシュ表面の面は次の3頂点からなる。" << std::endl;
//			//DSTR << "faces[tets.faces[l]].vertices[0]: " << faces[tets.faces[l]].vertices[0] <<std::endl;
//			//DSTR << "faces[tets.faces[l]].vertices[1]: " << faces[tets.faces[l]].vertices[1] <<std::endl;
//			//DSTR << "faces[tets.faces[l]].vertices[2]: " << faces[tets.faces[l]].vertices[2] <<std::endl;
//			//DSTR << "ID: " << ID <<"のときの節点と対面する面で面積分を計算する"<<std::endl;
//			for(unsigned k=0; k < faces[tets[id].faces[l]].heatTransRatios.size();++k){
//				for(unsigned j=0;j<4;j++){
//						if(tets[id].vertices[j] == ID){					///	形状関数が１、（すなわち）このfaceに対面する頂点　と一致したら　その時のfaceで面積分する
//							///	j番目の行列の成分を0にしたmatk2arrayで計算する
//							///	外殻にないメッシュ面の面積は0で初期化しておく
//							faces[tets[id].faces[l]].heatTransRatio = (vertices[faces[tets[id].faces[l]].vertices[0]].heatTransRatio + vertices[faces[tets[id].faces[l]].vertices[1]].heatTransRatio 
//								+ vertices[faces[tets[id].faces[l]].vertices[2]].heatTransRatio ) / 3.0;		///	当該faceの熱伝達率を構成節点での値の相加平均をとる
//							///	以下の[]は上までの[l]と異なる。
//							///	IDが何番目かによって、形状関数の係数が異なるので、
//							tets[id].matk[1] += faces[tets[id].faces[l]].heatTransRatios[k] * (1.0/12.0) * faces[tets[id].faces[l]].area * matk2array[j];		//元はtets[id].matk2 +=
//							//DSTR << "tets[id].matk2にfaces[tets[id].faces[l]].heatTransRatio * (1.0/12.0) * faces[tets[id].faces[l]].area * matk2array[" << j << "]"<< "を加算: " <<faces[tets[id].faces[l]].heatTransRatio * (1.0/12.0) * faces[tets[id].faces[l]].area * matk2array[j] << std::endl;
//							//DSTR << "tets[id].matk2 +=  " << tets[id].matk2 << std::endl;
//						}
//					//else{
//					//	///	IDと一致しない場合には、matk2array[j]には全成分0を入れる
//					//	///	としたいところだが、
//					//	//matk2array[j] =0.0 * matk2array[j];
//					//	//DSTR << "matk2array[" << j << "]: " << matk2array[j] << std::endl;
//					//}
//				}
//			}
//		}
//		///	SurfaceFaceじゃなかったら、matk2arrayには0を入れる
//		//else{
//		//	//matk2array[l];
//		//}
//	}
//
//	//DSTR << "matk2array:" << std::endl;
//	//for(unsigned i=0;i<4;i++){
//	//	DSTR <<i <<": " << matk2array[i] << std::endl;
//	//}
//
//	//k2 = k21 + k22 + k23 + k24
//	//for(unsigned i=0; i < 4; i++){
//	//	matk2 += matk2array[i];
//	//	//	for debug
//	//	//DSTR << "matk2 に matk2array = k2" << i+1 <<"まで加算した行列" << std::endl;
//	//	//DSTR << matk2 << std::endl;
//	//}
//	
//	//for debug
//	//DSTR << "節点（";
//	//for(unsigned i =0; i < 4; i++){
//	//	DSTR << tets[id].vertices[i] << "," ;
//	//}
//	//DSTR << ")で構成される四面体の" << std::endl;
//	//DSTR << "matk2 : " << std::endl;
//	//DSTR << matk2 << std::endl;
//	//int hogeshishi =0;
//	
//	//DSTR << "Inner Function _ matk2t tets[id].matk2: " <<tets[id].matk2 << std::endl;
//} 



bool PHFemThermo::SetConcentricHeatMap(std::vector<double> r, std::vector<double> temp, Vec2d origin){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//中心からの距離を求める
	double dx = 0.0;
	double dz = 0.0;
	for(unsigned i = 0; i < mesh->vertices.size(); ++i){
		if(vertexVars[i].disFromOrigin == 0){ 
			dx = mesh->vertices[i].pos.x - origin[0];
			dz = mesh->vertices[i].pos.z - origin[1];
			vertexVars[i].disFromOrigin = sqrt(dx * dx + dz * dz);
		}
	}
	//	ここまで
	//> 表面faceの内、原点から各faceの節点のローカル(x,z)座標系での平面上の距離の計算を、faceの全節点のy座標が負のものに対して、IH加熱の可能性を示すフラグを設定
	for(unsigned i=0;i<mesh->nSurfaceFace;i++){
		//> 表面のfaceの全節点のy座標が負ならば、そのfaceをIH加熱のface面と判定し、フラグを与える
		//if(faces[i].mayIHheated = true){
			//	(x,z)平面におけるmayIHheatedのface全節点の原点からの距離を計算する
			for(unsigned j=0; j<3; j++){
				double dx = mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.x - origin[0];
				double dz = mesh->vertices[mesh->faces[i].vertexIDs[j]].pos.z - origin[1];	//	表記はyだが、実質z座標が入っている
				vertexVars[mesh->faces[i].vertexIDs[j]].disFromOrigin = sqrt( dx * dx + dz * dz);
			}
		//}
	}
	//求めた距離に応じて、設定された初期条件を満たすような温度分布を作る

	//インタフェース化して、用いる。
	for(unsigned k=0; k < mesh->vertices.size(); ++k){
		for(size_t i=0; i < r.size()-1;++i){
			if(i>0 ){
				if( (r[i-1] + r[i]) / 2.0 < vertexVars[k].disFromOrigin && vertexVars[k].disFromOrigin <= ( r[i] + r[i+1]) / 2.0 ){
					vertexVars[k].temp = temp[i];
				}
			}
			//else if(i == r.size()-1){
			//	if(r[i] < vertices[k].disFromOrigin){
			//		if(temp.size() == r.size()){
			//			vertices[k].temp = temp[i];
			//		}
			//		else if(temp.size() < r.size()){
			//			vertices[k].temp = temp[temp.size() -1];
			//		}
			//	}
			//}
			else{
				if(vertexVars[k].disFromOrigin <=  (r[i] + r[i+1]) /2.0){
					vertexVars[k].temp = temp[i];
				} 
			}
		} 
	}
	if(r.size() != temp.size()) return -1;
	return 1;
}

void PHFemThermo::SetInitThermoConductionParam(double thConduct0,double roh0,double specificHeat0,double heatTrans0){
	thConduct = thConduct0;
	rho = roh0;
	specificHeat = specificHeat0;
	heatTrans = heatTrans0;
}
double PHFemThermo::Get_thConduct(){
	return thConduct;
}
void PHFemThermo::SetThermalEmissivityToVtx(unsigned id,double thermalEmissivity,double thermalEmissivity_const){
	vertexVars[id].thermalEmissivity = thermalEmissivity;
	vertexVars[id].thermalEmissivity_const = thermalEmissivity_const;
}
void PHFemThermo::SetThermalEmissivityToVerticesAll(double thermalEmissivity,double thermalEmissivity_const){
	for(unsigned i =0; i < GetPHFemMesh()->vertices.size(); i++){
		vertexVars[i].thermalEmissivity = thermalEmissivity;
		vertexVars[i].thermalEmissivity_const = thermalEmissivity_const;
	}
}
void PHFemThermo::SetHeatTransRatioToAllVertex(double heatTransR_){
	for(unsigned i =0; i < GetPHFemMesh()->vertices.size() ; i++){
		vertexVars[i].heatTransRatio = heatTransR_;
	}
	//行列とvecfの熱伝達項を作り直す
	std::cout << "行列の熱伝達項を作りなおせ！"<<std::endl;
	DSTR << "行列の熱伝達項を作りなおせ！"<<std::endl;
}

void PHFemThermo::SetHeatTransRatioToAllVertex(){
	for(unsigned i =0; i < GetPHFemMesh()->vertices.size() ; i++){
		vertexVars[i].heatTransRatio = heatTrans;
	}
}

void PHFemThermo::SetTempAllToTVecAll(unsigned size){
	for(unsigned i =0; i < size;i++){
		TVecAll[i] = vertexVars[i].temp;
	}
}

void PHFemThermo::CreateTempVertex(){		//Resize and Set Temp to TVecAll 
	TVecAll.resize(GetPHFemMesh()->vertices.size());
	SetTempAllToTVecAll((unsigned)GetPHFemMesh()->vertices.size());
}

void PHFemThermo::SetTempToTVecAll(unsigned vtxid){

	TVecAll[vtxid] = vertexVars[vtxid].temp;
}

void PHFemThermo::UpdateheatTransRatio(unsigned id,double heatTransRatio){
	//if(vertices[id].heatTransRatio != heatTransRatio){	//異なっていたら
	//	vertices[id].heatTransRatio = heatTransRatio;	
	//	///	αを含む行列の更新	K2,f3
	//	///	f3
	//	for(unsigned i =0; i < vertices[1].tets.size(); i++){
	//		CreateVecf3(tets[vertices[id].tets[i]]);
	//	}
	//	///	K3

	//}
	///	同じなら何もしない
}

void PHFemThermo::SetLocalFluidTemp(unsigned i,double temp){
	vertexVars[i].Tc = temp;			//節点の周囲流体温度の設定
}

void PHFemThermo::SetVertexTemp(unsigned i,double temp){
	vertexVars[i].temp = temp;
	SetTempToTVecAll(i);
}

void PHFemThermo::SetVerticesTempAll(double temp){
	for(std::vector<unsigned int>::size_type i=0; i < GetPHFemMesh()->vertices.size() ; i++){
		vertexVars[i].temp = temp;
		SetTempToTVecAll((unsigned)i);	// 要検討：Aftersetdeskの中で呼ばれる時、TVecAllの要素の数が固まっておらず、アクセス違反の可能性がある
	}
}

void PHFemThermo::AddvecFAll(unsigned id,double dqdt){
	vecFAllSum[id] += dqdt;		//	+=に変更
	doCalc =true;
}

void PHFemThermo::SetvecFAll(unsigned id,double dqdt){
	vecFAllSum[id] = dqdt;		//	+=に変更すべきで、削除予定
}

void PHFemThermo::InitAllVertexTemp(){
	//	このメッシュの全長点の温度を0にする
	for(unsigned i=0; i < GetPHFemMesh()->vertices.size(); i++){
		vertexVars[i].temp = 0.0;
		//	どのメッシュでリセットかけたか　GetMe()->
	}
}

void PHFemThermo::InitMoist(){
	for(unsigned id =0; id < GetPHFemMesh()->tets.size(); id++){
		tetVars[id].wratio = 0.917;
		double rho = 970;
		if(tetVars[id].volume){
			tetVars[id].tetsMg = tetVars[id].volume * rho;	//質量*密度
			tetVars[id].wmass = tetVars[id].tetsMg * tetVars[id].wratio;
		}else if(tetVars[id].volume < 0.0){
			DSTR << "tets[" << id << "]の体積が計算されていません" << std::endl;
			tetVars[id].volume;
		}
	}
}

void PHFemThermo::DecrMoist_velo(double vel){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//制限速度ver
	for(unsigned id =0; id < mesh->tets.size() ; id++){
		////頂点が100度以上で残水量が０ではないとき
		double tempAdd = 0.0;	//	加算温度
		for(unsigned i=0; i < 4; i++){
			tempAdd += vertexVars[mesh->tets[id].vertexIDs[i]].temp;
		}
		//単位換算は合っているか？
		double wlatheat = 2.26 * 1000;		//水1kg当たりの潜熱(latent heat)[W・s]=[J] 水の潜熱が540cal/gよりJに変換して使用   W=J/s 2.26[kJ/kg]
		tetVars[id].tetsMg = tetVars[id].volume * rho;		//四面体の質量
		//単位時間あたり蒸発量は、dw/dt = A/W * Rc  A:見かけ表面積、W:見かけの無水物質量
		double faceS=0.0;
		for(int ii=0;ii<4;ii++){
			int facet = mesh->tets[id].faceIDs[ii];
			if(facet <= (int)mesh->nSurfaceFace){
				faceS += faceVars[facet].area;
			}
		}
		//double surfaceS = tets[id].faces[  //	四面体に属するfaceがnSurfacefaceより小さい番号のfaceなら、表面のface
		double sokudo = vel;
		double dwdt = faceS / tetVars[id].tetsMg * sokudo;//単位時間あたりなのでdt要素を入れる		//1stepで減る水の量
		//double wlat = (tempAdd / 4.0) - 100.0;	//100度を超えただけ蒸発する。
		//double dw = dwdt * specificHeat *  tets[id].tetsMg / wlatheat;	//	水分蒸発量	//	(温度と沸点100度との差分の熱量)÷水の潜熱で蒸発する水の量が分かる。;		//	水分蒸発量
		double exwater	= 0;	//流出する水の量全体(蒸発 + 流出 + 水分移動)
		//平均温度が100度超過

		//100度未満：蒸気圧差　による蒸発・乾燥
			//質量を減じ
			tetVars[id].tetsMg -= dwdt;
			//熱量を奪う	四面体の質量が持っている熱量から、dwdt分を削る		//	4節点の平均温度でいいのか？	//ここに無理がありそう。
			double tetsheating = rho * specificHeat * tetVars[id].volume * (vertexVars[mesh->tets[id].vertexIDs[0]].temp + vertexVars[mesh->tets[id].vertexIDs[1]].temp + vertexVars[mesh->tets[id].vertexIDs[2]].temp + vertexVars[mesh->tets[id].vertexIDs[3]].temp)/4.0;

			//


		//100度以上：潜熱による蒸発・乾燥


		//変性に依る流出　タンパク質変性・構造変化と水分流出



		//	多分、以下のコードが問題。
		//if( tempAdd / 4.0 >= 100.0){
		//	//dwの分だけ、質量や水分量から引く
		//	//double delw = (dt / 0.01 * 1.444*(0.000235/0.29)  / 10000000)*100;
		//	double delw = (1.444*(0.000235/0.29)  / 10000000)*100;
		//	exwater = delw * 500;
		//	tets[id].tetsMg -= dw - exwater;
		//	if(tets[id].wmass > dw - exwater){
		//		tets[id].wmass -= dw - exwater;
		//	}else{
		//		DSTR << "水分流出量が多すぎます" << std::endl;
		//	}
		//	//検証する:ひとまず、exwaterが０でなければ、音を再生させることにしようか。音を出したら、そのメッシュのexwaterの値を０にしよう。
		//	//wlatの分だけ、温度から引く
		//	for(unsigned j=0; j < 4; j++){
		//		vertices[j].temp -= dwdt;
		//	}
		//}
		//とりあえず、簡単に、水分を減らすコード
		//if(tets[id].wmass >= tets[id].wmass_start *0.01){
		//	tets[id].wmass -= tets[id].wmass_start * 0.01;
		//}
	}
}


void PHFemThermo::DecrMoist_vel(double dt){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//制限速度ver
	for(unsigned id =0; id < mesh->tets.size() ; id++){
		////頂点が100度以上で残水量が０ではないとき
		double tempAdd = 0.0;	//	加算温度
		for(unsigned i=0; i < 4; i++){
			tempAdd += vertexVars[mesh->tets[id].vertexIDs[i]].temp;
		}
		//単位換算は合っているか？
		double wlatheat = 2.26 * 1000;		//水1kg当たりの潜熱(latent heat)[W・s]=[J] 水の潜熱が540cal/gよりJに変換して使用   W=J/s 2.26[kJ/kg]
		tetVars[id].tetsMg = tetVars[id].volume * rho;		//四面体の質量
		//単位時間あたり蒸発量は、dw/dt = A/W * Rc  A:見かけ表面積、W:見かけの無水物質量
		double faceS=0.0;
		for(int ii=0;ii<4;ii++){
			int facet = mesh->tets[id].faceIDs[ii];
			if(facet <= (int)mesh->nSurfaceFace){
				faceS += faceVars[facet].area;
			}
		}
		//double surfaceS = tets[id].faces[  //	四面体に属するfaceがnSurfacefaceより小さい番号のfaceなら、表面のface
		double sokudo = 1.0;
		double dwdt = faceS / tetVars[id].tetsMg * sokudo;//単位時間あたりなのでdt要素を入れる
		//double wlat = (tempAdd / 4.0) - 100.0;	//100度を超えただけ蒸発する。
		double dw = dwdt * specificHeat *  tetVars[id].tetsMg / wlatheat;	//	水分蒸発量	//	(温度と沸点100度との差分の熱量)÷水の潜熱で蒸発する水の量が分かる。;		//	水分蒸発量
		double exwater	= 0;	//流出する水の量全体(蒸発 + 流出 + 水分移動)
		//平均温度が100度超過
		if( tempAdd / 4.0 >= 100.0){
			//dwの分だけ、質量や水分量から引く
			//double delw = (dt / 0.01 * 1.444*(0.000235/0.29)  / 10000000)*100;
			double delw = (1.444*(0.000235/0.29)  / 10000000)*100;
			exwater = delw * 500;
			tetVars[id].tetsMg -= dw - exwater;
			if(tetVars[id].wmass > dw - exwater){
				tetVars[id].wmass -= dw - exwater;
			}else{
				DSTR << "水分流出量が多すぎます" << std::endl;
			}
			//検証する:ひとまず、exwaterが０でなければ、音を再生させることにしようか。音を出したら、そのメッシュのexwaterの値を０にしよう。
			//wlatの分だけ、温度から引く
			for(unsigned j=0; j < 4; j++){
				vertexVars[j].temp -= dwdt;
			}
		}
		//とりあえず、簡単に、水分を減らすコード
		//if(tets[id].wmass >= tets[id].wmass_start *0.01){
		//	tets[id].wmass -= tets[id].wmass_start * 0.01;
		//}
	}
}


void PHFemThermo::DecrMoist(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	for(unsigned id =0; id < mesh->tets.size() ; id++){
		////頂点が100度以上で残水量が０ではないとき
		double tempAdd = 0.0;	//	加算温度
		for(unsigned i=0; i < 4; i++){
			tempAdd += vertexVars[mesh->tets[id].vertexIDs[i]].temp;
		}
		//単位換算は合っているか？
		double wlatheat = 2.26 * 1000;		//水1kg当たりの潜熱(latent heat)[W・s]=[J] 水の潜熱が540cal/gよりJに変換して使用   W=J/s 2.26[kJ/kg]
		tetVars[id].tetsMg = tetVars[id].volume * rho;		//四面体の質量
		//単位時間あたり蒸発量は、
		double wlat = (tempAdd / 4.0) - 100.0;	//100度を超えただけ蒸発する。
		double dw = wlat * specificHeat *  tetVars[id].tetsMg / wlatheat;	//	水分蒸発量	//	(温度と沸点100度との差分の熱量)÷水の潜熱で蒸発する水の量が分かる。;		//	水分蒸発量
		double exwater	= 0;	//流出する水の量全体(蒸発 + 流出 + 水分移動)
		//平均温度が100度超過
		if( tempAdd / 4.0 >= 100.0){
			//dwの分だけ、質量や水分量から引く
			//double delw = (dt / 0.01 * 1.444*(0.000235/0.29)  / 10000000)*100;
			double delw = (1.444*(0.000235/0.29)  / 10000000)*100;
			exwater = delw * 500;
			tetVars[id].tetsMg -= dw - exwater;
			if(tetVars[id].wmass > dw - exwater){
				tetVars[id].wmass -= dw - exwater;
			}else{
				DSTR << "水分流出量が多すぎます" << std::endl;
			}
			//検証する:ひとまず、exwaterが０でなければ、音を再生させることにしようか。音を出したら、そのメッシュのexwaterの値を０にしよう。
			//wlatの分だけ、温度から引く
			for(unsigned j=0; j < 4; j++){
				vertexVars[j].temp -= wlat;
			}
		}
		//とりあえず、簡単に、水分を減らすコード
		//if(tets[id].wmass >= tets[id].wmass_start *0.01){
		//	tets[id].wmass -= tets[id].wmass_start * 0.01;
		//}
	}
}

void PHFemThermo::CalcVertexVolume(unsigned vtxid){
	PHFemMeshNew* mesh = GetPHFemMesh();
	vertexVars[vtxid].vVolume = 0;
	for(unsigned tetid=0; tetid < mesh->vertices[vtxid].tetIDs.size(); tetid++){
		vertexVars[vtxid].vVolume += tetVars[mesh->vertices[vtxid].tetIDs[tetid]].volume / 4;
	}
}

void PHFemThermo::VertexWaterBoiling(unsigned vtxid){
	PHFemMeshNew* mesh = GetPHFemMesh();
	unsigned vtxID = mesh->surfaceVertices[vtxid];
	
	if(TVecAll[vtxID] >= 100){
		double calorieOver100 = specificHeat * rho * vertexVars[vtxID].vVolume * (TVecAll[vtxID] - 100.0);	//100℃を超えている分の熱量
		double maxEvapoWaterMass = calorieOver100 / WaterLatentHeat;	//蒸発させる事ができる水の質量(kg)
		if(mesh->GetPHFemPorousWOMove()->GetVertexMw(vtxID) > maxEvapoWaterMass){
			TVecAll[vtxID] = 100.0;
			mesh->GetPHFemPorousWOMove()->SetVertexMw(vtxID, mesh->GetPHFemPorousWOMove()->GetVertexMw(vtxID) - maxEvapoWaterMass);
		}else if(mesh->GetPHFemPorousWOMove()->GetVertexMw(vtxID) > 0){
			double upTemp = (calorieOver100 - mesh->GetPHFemPorousWOMove()->GetVertexMw(vtxID) * WaterLatentHeat) / specificHeat / rho / vertexVars[vtxID].vVolume;
			mesh->GetPHFemPorousWOMove()->SetVertexMw(vtxID, 0);
			TVecAll[vtxID] = 100.0 + upTemp;
		}
	}
}

float PHFemThermo::calcGvtx(std::string fwfood, int pv, unsigned texture_mode){
	float gvtx = 0.0f;
	//テクスチャの設定
	//焦げテクスチャの枚数
	unsigned kogetex	= 5;
	//水分テクスチャの枚数
	unsigned watex		= 2;
	//サーモテクスチャの枚数
	unsigned thtex		= 6;
	unsigned thcamtex   = 9;		//熱カメラっぽい表示用
	//	ロードテクスチャーが焦げ→水→温度の順	（または）水→温度→焦げ	にも変更可能（ファイル名のリネームが必要）

	//水分テクスチャの枚数(水の空間密度を用いた場合)
	unsigned watex2 = 5;

	// num of texture layers
	if(fwfood == "fwNegi"){		///	テクスチャと温度、水分量との対応表は、Samples/Physics/FEMThermo/テクスチャの色と温度の対応.xls	を参照のこと
		kogetex	= 5;
	}
	else if(fwfood == "fwNsteak"){
		kogetex	= 5;		//7にする
	}
	else if(fwfood == "fwPan"){
		kogetex = 5;
	}

	double dtex =(double) 1.0 / ( kogetex + thtex + watex + thcamtex);		//	テクスチャ奥行座標の層間隔
	if(fwfood == "fwNsteak"){
		dtex =(double) 1.0 / ( kogetex + thtex + watex + thcamtex + watex2);
	}
	double texstart = dtex /2.0;										//	テクスチャ座標の初期値 = 焦げテクスチャのスタート座標
	double wastart = texstart + kogetex * dtex;							//	水分量表示テクスチャのスタート座標
	double thstart = texstart + kogetex * dtex + 1.0 * dtex;			//	サーモのテクスチャのスタート座標 水分テクスチャの2枚目からスタート
	double thcamstart = texstart + (thtex + kogetex + watex) * dtex;	//	

	if(texture_mode == 1){
		if(fwfood == "fwPan"){
			gvtx = texstart;// + dtex;		// ねずみ色の底面
		}else if(fwfood == "fwNegi"){
			// 温度変化と同じで　
			double temp = vertexVars[pv].temp;
			// -50.0~0.0:aqua to blue
			if(temp <= -50.0){
				gvtx = texstart + dtex;
			}
			else if(-50.0 < temp && temp <= 0.0){	
				gvtx = texstart + dtex;//(texstart ) + ((temp + 50.0) * dtex /50.0);
			}
			//	0~50.0:blue to green
			else if(0.0 < temp && temp <= 50.0 ){
				//double green = temp * dtex / 50.0 + thstart;
				gvtx = (temp - 50.0)  * dtex / 50.0 + texstart + dtex; //+     dtex;
			}
			//	50.0~100.0:green to yellow
			else if(50.0 < temp && temp <= 100.0){
				gvtx = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
			}
			//	100.0~150:yellow to orange	
			else if(100.0 < temp && temp <= 150.0){
				gvtx = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
			}
			//	150~200:orange to red
			else if(150.0 < temp && temp <= 200.0){
				double pinkc = (temp - 50.0 ) * dtex / 50.0 + thstart ;
				gvtx = dtex * 4.0 + texstart;//(temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
			}
			//	200~250:red to purple
			else if(200.0 < temp && temp <= 250.0){
				gvtx = dtex * 4.0 + texstart;//(temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
			}
			///	250~:only purple
			else if(250.0 < temp){
				gvtx = dtex * 4.0 + texstart;
				//gvtx[stride*gv + tex + 2] = wastart;			//whit
			}
			else{
				DSTR << "vertexVars[" << pv << "].temp = " << vertexVars[pv].temp << std::endl;
			}
		}
		else if(fwfood == "fwNsteak"){
			// 温度変化と同じで　
			double temp = vertexVars[pv].temp;
			// -50.0~0.0:aqua to blue
			if(temp <= -50.0){
				gvtx = texstart + dtex;
			}
			else if(-50.0 < temp && temp <= 0.0){	
				gvtx = texstart + dtex;//(texstart ) + ((temp + 50.0) * dtex /50.0);
			}
			//	0~50.0:blue to green
			else if(0.0 < temp && temp <= 50.0 ){
				//double green = temp * dtex / 50.0 + thstart;
				gvtx = (temp - 50.0)  * dtex / 50.0 + texstart + dtex; //+     dtex;
			}
			//	50.0~100.0:green to yellow
			else if(50.0 < temp && temp <= 100.0){
				gvtx = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
			}
			//	100.0~150:yellow to orange	
			else if(100.0 < temp && temp <= 150.0){
				gvtx = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
			}
			//	150~200:orange to red
			else if(150.0 < temp && temp <= 200.0){
				double pinkc = (temp - 50.0 ) * dtex / 50.0 + thstart ;
				gvtx = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
			}
			//	200~250:red to purple
			else if(200.0 < temp && temp <= 250.0){
				gvtx = (temp - 50.0 ) * dtex / 50.0 + texstart + dtex;// + 2 * dtex;
			}
			///	250~:only purple
			else if(250.0 < temp){
				gvtx = dtex * 6.0 + texstart;
				//gvtx[stride*gv + tex + 2] = wastart;			//white	 ///	まだらになっちゃう
			}
			else{
				DSTR << "vertexVars[" << pv << "].temp = " << vertexVars[pv].temp << std::endl;
			}
		}
		else if(fwfood == "tPan"){
			DSTR << "tPan are there" << std::endl;
		}
		int phmeshdebug =0;
	}else if(texture_mode == 3){
		//	水分蒸発表示モード
		//	残水率に沿った変化
		gvtx = wastart + 2 * dtex;
		for(unsigned j =0; j < GetPHFemMesh()->tets.size(); j++){
			//	割合直打ちでいいや
			if(0.5 < tetVars[j].wratio && tetVars[j].wratio < 1.0){
				gvtx = wastart + 2 * dtex - ( (tetVars[j].wratio -0.5) * (dtex / 0.5) );
			}
			else if(0.0 < tetVars[j].wratio && tetVars[j].wratio < 0.5){
				gvtx = wastart + 1 * dtex - ( (tetVars[j].wratio -0.5) * (dtex / 0.5) );
			}
		}
	}else if(texture_mode == 2){
		double temp = vertexVars[pv].temp;
		// -50.0~0.0:aqua to blue
		if(temp <= -50.0){
			gvtx = thstart;
		}
		else if(-50.0 < temp && temp <= 0.0){	
			gvtx = (thstart ) + ((temp + 50.0) * dtex /50.0);
		}
		//	0~50.0:blue to green
		else if(0.0 < temp && temp <= 50.0 ){
			//double green = temp * dtex / 50.0 + thstart;
			gvtx = temp * dtex / 50.0 + thstart + dtex;
		}
		//	50.0~100.0:green to yellow
		else if(50.0 < temp && temp <= 100.0){
			gvtx = (temp - 50.0 ) * dtex /	 50.0 + thstart + 2 * dtex;
		}
		//	100.0~150:yellow to orange	
		else if(100.0 < temp && temp <= 150.0){
			gvtx = (temp - 50.0 ) * dtex / 50.0 + thstart + 2 * dtex;
		}
		//	150~200:orange to red
		else if(150.0 < temp && temp <= 200.0){
			double pinkc = (temp - 50.0 ) * dtex / 50.0 + thstart ;
			gvtx = (temp - 50.0 ) * dtex / 50.0 + thstart + 2 * dtex;
		}
		//	200~250:red to purple
		else if(200.0 < temp && temp <= 250.0){
			gvtx = (temp - 50.0 ) * dtex / 50.0 + thstart + 2 * dtex;
		}
		///	250~:only purple
		else if(250.0 < temp){
			gvtx = dtex * 6.0 + thstart;
			//gvtx[stride*gv + tex + 2] = wastart;			//white	 ///	まだらになっちゃう
		}
		else{
			DSTR << "vertexVars[" << pv << "].temp = " << vertexVars[pv].temp << std::endl;
		}
	}else if(texture_mode == 4){
		double temp = vertexVars[pv].temp;
		// -50.0~0.0:aqua to blue => 20 : purple
		if(temp < 20.0){
			gvtx = thstart + 6.0 * dtex; 
		}
		else if(temp == 20.0){
			gvtx = thcamstart;
		}
		else if(20.0 < temp && temp <= 30.0){	
			gvtx = thcamstart + (temp - 20.0) * dtex / 10.0;
		}
		//	0~50.0:blue to green
		else if(30.0 < temp && temp <= 40.0 ){
			//double green = temp * dtex / 50.0 + thstart;
			gvtx = thcamstart + dtex + (temp - 30.0) * dtex / 10.0;
		}
		//	50.0~100.0:green to yellow
		else if(40.0 < temp && temp <= 50.0){
			gvtx = thcamstart + 2 * dtex + (temp - 40.0) * dtex / 10.0;
		}
		//	100.0~150:yellow to orange	
		else if(50.0 < temp && temp <= 60.0){
			gvtx = thcamstart + 3 * dtex + (temp - 50.0) * dtex / 10.0;
		}
		//	150~200:orange to red
		else if(60.0 < temp && temp <= 70.0){
			gvtx = thcamstart + 4 * dtex + (temp - 60.0) * dtex / 10.0;
		}
		//	200~250:red to purple
		else if(70.0 < temp && temp <= 80.0){
			gvtx = thcamstart + 5 * dtex + (temp - 70.0) * dtex / 10.0;
		}
		///	250~:only purple
		else if(80.0 < temp && temp <= 90.0){
			gvtx = thcamstart + 6 * dtex + (temp - 80.0) * dtex / 10.0;
		}
		else if(90.0 < temp && temp <= 100.0){
			gvtx = thcamstart + 7 * dtex + (temp - 90.0) * dtex / 10.0;
		}
		else if(100.0 < temp){
			gvtx = thcamstart + 8 * dtex;
		}
		else{
			DSTR << "vertexVars[" << pv << "].temp = " << vertexVars[pv].temp << std::endl;
		}
	}
	return gvtx;
}

void PHFemThermo::CreateVecFAll(){
	for(unsigned tetid=0; tetid < GetPHFemMesh()->tets.size(); tetid++){
		CreateVecFAll(tetid);
	}
}

void PHFemThermo::CalcFaceNormalAll(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	faceの法線を計算
	//.	表面の頂点に、法線ベクトルを追加
	//.	について再帰的に実行
	Vec3d extp;		//	外向き法線
	Vec3d tempV;	//	外向き判定比較頂点(該当face面上にない頂点序数)
	DSTR << "tets.size(): " << mesh->tets.size() << std::endl;
	for(unsigned tid=0; tid < mesh->tets.size(); tid++){
		//	どの頂点IDでfaceが構成されているのか
		unsigned idsum = 0;
		for(unsigned i=0;i<4;i++){
			idsum += mesh->tets[tid].vertexIDs[i];
		}
		for(unsigned fid = 0; fid < 4; fid++){
			//DSTR << "fid :" << fid <<std::endl;
			extp = (mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[1]].pos - mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[0]].pos)
				% (mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[2]].pos - mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[0]].pos);
			extp = extp / extp.norm();
			Vec3d chkN[2] = {mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[1]].pos - mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[2]].pos
				, mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[2]].pos - mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[1]].pos};
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
				idsumt -= mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[j];
				//DSTR << "faces[" << fid << "].vertices["<<j <<"]: "<< faces[tets[tid].faces[fid]].vertices[j];
			}
			
			//. face重心からface外頂点へのベクトルtempV計算
			Vec3d jushin = mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[0]].pos + mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[1]].pos
				+ mesh->vertices[mesh->faces[mesh->tets[tid].faceIDs[fid]].vertexIDs[2]].pos;
			jushin *= 1.0 / 3.0;
			tempV = mesh->vertices[idsumt].pos - jushin;
			//DSTR << "tempV:" << tempV <<std::endl;
			if(tempV==Vec3d(0.0,0.0,0.0)){
				DSTR <<"ERROR:	for normal calculating, some vertices judging is invalids"<< std::endl;
			}
			if((tempV * extp / (tempV.norm() * extp.norm()) ) < 0.0){
				//extpとtempVが±９０度以上離れている：extpが外向き法線
				faceVars[mesh->tets[tid].faceIDs[fid]].normal = extp / 10.0;		//	長さを１0cmに
			}else{
				//extpとtempVが９０度以内：extpの向きを180度変えて、faces[fid].normalに代入
				faceVars[mesh->tets[tid].faceIDs[fid]].normal = - extp / 10.0;		// 逆ベクトル
			}
			
			int debughogeshi=0;
		}
		//Debug
		//全faceに、外向き法線ベクトルを表示させてみて、様子を見れば、確認できるかな？又は、シンプルなメッシュで表示してみるか
	}

	//	頂点の法線を計算
	//	頂点の属するface面より平均？正規化した頂点法線を求める
	std::vector<Vec3d> faceNormal;
	faceNormal.clear();
	for(unsigned vid = 0; vid < mesh->vertices.size(); vid++ ){
		//unsigned fsize = vertices[vid].faces.size();
		for(unsigned fid = 0; fid < mesh->vertices[vid].faceIDs.size(); fid++ ){
			//.	属するface法線がほぼ同じ方向を向いてるものが見つかった場合は、1つだけ加算して平均をとるように変更する

			//外側の頂点の法線だけ加算			
			if(mesh->vertices[vid].faceIDs[fid] < (int)mesh->nSurfaceFace){
				vertexVars[vid].normal += faceVars[mesh->vertices[vid].faceIDs[fid]].normal;		// このコードに代わって、上記vectorコードと以下の加算コードに置き換え
			}

		}
		vertexVars[vid].normal = vertexVars[vid].normal / vertexVars[vid].normal.norm();		//	単位ベクトル化
	}
}

void PHFemThermo::CalcVertexNormalAll(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	//	頂点の法線を計算
	//	頂点の属するface面より平均？正規化した頂点法線を求める
	std::vector<Vec3d> faceNormal;
	faceNormal.clear();
	for(unsigned vid = 0; vid < mesh->vertices.size(); vid++ ){
		//unsigned fsize = vertices[vid].faces.size();
		for(unsigned fid = 0; fid < mesh->vertices[vid].faceIDs.size(); fid++ ){
			//.	属するface法線がほぼ同じ方向を向いてるものが見つかった場合は、1つだけ加算して平均をとるように変更する

			//外側の頂点の法線だけ加算			
			if(mesh->vertices[vid].faceIDs[fid] < (int)mesh->nSurfaceFace){
				vertexVars[vid].normal += faceVars[mesh->vertices[vid].faceIDs[fid]].normal;		// このコードに代わって、上記vectorコードと以下の加算コードに置き換え
			}

		}
		vertexVars[vid].normal = vertexVars[vid].normal / vertexVars[vid].normal.norm();		//	単位ベクトル化
	}
}

void PHFemThermo::InitFaceNormalAll(){
	for(unsigned faceid=0; faceid < GetPHFemMesh()->faces.size(); faceid++){
		faceVars[faceid].normal = Vec3d(0.0, 0.0, 0.0);
	}
}
void PHFemThermo::InitVertexNormalAll(){
	for(unsigned vtxid=0; vtxid < GetPHFemMesh()->vertices.size(); vtxid++){
		vertexVars[vtxid].normal = Vec3d(0.0, 0.0, 0.0);
	}
}

void PHFemThermo::RevVertexNormalAll(){
	PTM::TMatrixRow<3,3,double> revMat;
	revMat.clear();
	revMat[0][0] = -1.0;
	revMat[1][1] = -1.0;
	revMat[2][2] = -1.0;
	for(unsigned vtxid=0; vtxid < GetPHFemMesh()->vertices.size(); vtxid++){
		vertexVars[vtxid].normal = revMat * vertexVars[vtxid].normal;
	}
}

void PHFemThermo::CalcMatKup(){
#ifdef badMatK
	PHFemMeshNew* mesh = GetPHFemMesh();
	matkupdate.clear();
	for(unsigned i=0; i < mesh->vertices.size(); i++){
		double total = 0.0;
		for(unsigned j=0; j < mesh->vertices.size(); j++){
			if(i!=j){
				total += fabs(matKAll[i][j]);
			}
		}
		if(matKAll[i][i] <= total){
			matkupdate.push_back((total - matKAll[i][i]) + dMatKinf);
		}else{
			matkupdate.push_back(0.0);
		}
	}
#endif
}

void PHFemThermo::CalcEdgeLengthAll(){
	PHFemMeshNew* mesh = GetPHFemMesh();
	for(unsigned edgeid=0; edgeid < mesh->edges.size(); edgeid++){
		double length = 0.0;
		length += pow((mesh->vertices[mesh->edges[edgeid].vertexIDs[0]].pos[0] - mesh->vertices[mesh->edges[edgeid].vertexIDs[1]].pos[0]),2.0);
		length += pow((mesh->vertices[mesh->edges[edgeid].vertexIDs[0]].pos[1] - mesh->vertices[mesh->edges[edgeid].vertexIDs[1]].pos[1]),2.0);
		length += pow((mesh->vertices[mesh->edges[edgeid].vertexIDs[0]].pos[2] - mesh->vertices[mesh->edges[edgeid].vertexIDs[1]].pos[2]),2.0);
		edgeVars[edgeid].length = sqrt(length);
	}
}

double PHFemThermo::GetMaxMinEdgeRatioInTet(unsigned tetId){
	PHFemMeshNew* mesh = GetPHFemMesh();
	double maxL = 0.0;
	double minL = 0.0;
	for(unsigned edgeid=0; edgeid < 6; edgeid++){
		double currentL = edgeVars[mesh->tets[tetId].edgeIDs[edgeid]].length;
		if(edgeid == 0){
			maxL = currentL;
			minL = currentL;
		}else{
			if(currentL > maxL){
				maxL = currentL;
			}
			if(currentL < minL){
				minL = currentL;
			}
		}
	}
	return maxL / minL;
}

void PHFemThermo::OutEig(){
	PHFemMeshNew* mesh = GetPHFemMesh();
	PTM::VMatrixRow<double> eig;

#ifndef UseScilab
	if (!ScilabStart()) DSTR << "Error : ScilabStart \n";
	else DSTR << "ScilabStart \n";
#endif
	ScilabSetMatrix("c", matCAll);
	ScilabSetMatrix("k", tdt*matKAll);
	ScilabJob("eig = spec(inv(k+c)*c)");
	ScilabGetMatrix(eig, "eig");
#ifndef UseScilab
	ScilabEnd();
#endif

	std::ofstream eigOut;
	eigOut.open("eigOut.csv");
	for(unsigned i=0; i < mesh->vertices.size(); i++){
		eigOut << eig[i][0] << std::endl;
	}
	eigOut.close();
}

void PHFemThermo::VecFNegativeCheck(){
	PHFemMeshNew* mesh = GetPHFemMesh();
	bool flag = false;
	for(unsigned i=0; i < mesh->vertices.size(); i++){
		if(vecFAllSum[i] < 0){
			DSTR << i << ":" << vecFAllSum[i] << std::endl;
			flag = true;
		}
	}
	if(flag)
		DSTR << std::endl;
}

void PHFemThermo::SetVerticesPreTempAll(double temp){
	for(unsigned vtxid=0; vtxid < GetPHFemMesh()->vertices.size(); vtxid++){
		preTVecAll[vtxid] = temp;
	}
}

PTM::TMatrixRow<4,4,double> PHFemThermo::Create44Mat50(){
	//|5 0 0 0 |
	//|0 5 0 0 |
	//|0 0 5 0 |
	//|0 0 0 5 |	を作る
	PTM::TMatrixRow<4,4,double> MatTemp;
	for(int i =0; i <4 ; i++){
		for(int j=0; j < 4 ; j++){
			if(i==j){
				MatTemp[i][j] = 5.0;
			}else{
				MatTemp[i][j] = 0.0;
			}
		}
	}
	return MatTemp;
}

void PHFemThermo::matKModif(){
	PHFemMeshNew* mesh = GetPHFemMesh();

	for(unsigned i=0; i < mesh->vertices.size(); i++){
		for(unsigned j=0; j < mesh->vertices.size(); j++){
			if(i!=j){
				if(matKAll[i][j] > 0){
					matKAll[i][i] += 2*matKAll[i][j];
					matKAll[i][j] *= -1;
				}
			}
		}
	}
}

void PHFemThermo::makeFPartition(unsigned num){
	PHFemMeshNew* mesh = GetPHFemMesh();

	fpat.resize(mesh->vertices.size(), mesh->vertices.size());
	fpat.clear();
	double cmin = matCAll[1][1];
	for(unsigned i=0; i < mesh->vertices.size(); i++){
		for(unsigned j=0; j < mesh->vertices.size(); j++){
			if(matCAll[i][j] != 0){
				if(matCAll[i][j] < cmin){
					cmin = matCAll[i][j];
				}
			}
		}
	}
	cratio = 1 / cmin * matCAll;

	for(unsigned i=0; i < mesh->vertices.size(); i++){
		if(num == 0){
			fpat[i][i] = 1.0;
		}else{
			PTM::VVector<double> ftemp;
			ftemp = fPartition(i, 1, num-1);
			for(unsigned j=0; j < mesh->vertices.size(); j++){
				fpat[j][i] = ftemp[j];
			}
		}
	}
}

PTM::VVector<double> PHFemThermo::fPartition(unsigned vid, double f, unsigned num){
	unsigned vsize = (unsigned)GetPHFemMesh()->vertices.size();
	double cratioSum = 0;
	PTM::VVector<double> fsub;
	fsub.resize(vsize);
	fsub.clear();

	for(unsigned i=0; i < vsize; i++){
		cratioSum += cratio[i][vid];
	}

	for(unsigned i=0; i < vsize; i++){
		if(num == 0){
			fsub[i] = f * cratio[i][vid] / cratioSum;
		}else{
			fsub += fPartition(i, f*cratio[i][vid]/cratioSum, num-1);
		}
	}
	return fsub;
}

double PHFemThermo::sumVectorElement(PTM::VVector<double> vec){
	double sum = 0;
	for(unsigned i=0; i < vec.size(); i++){
		sum += vec[i];
	}
	return sum;
}

void PHFemThermo::createNewVecF(){
	if(vecFAllSum.norm() != 0){
		PHFemMeshNew* mesh = GetPHFemMesh();
		double fmin = 0;
		double alpha;
		PTM::VVector<double> vecFratio;
		for(unsigned i=0; i < mesh->vertices.size(); i++){
			if(fmin == 0){
				if(vecFAllSum[i] != 0){
					fmin = vecFAllSum[i];
				}
			}else{
				if(vecFAllSum[i] > 0 && vecFAllSum[i] < fmin){
					fmin = vecFAllSum[i];
				}
			}
		}

		vecFratio = 1 / fmin * vecFAllSum;
		//alpha = (sumVectorElement(vecFAllSum) - sumVectorElement(matKAll*TVecAll)) / sumVectorElement((matKAll+1/tdt*matCAll)*vecFratio);
		alpha = sumVectorElement(vecFAllSum) / sumVectorElement(1/tdt*matCAll*vecFratio);
		//alpha = (sumVectorElement(vecFAllSum) - sumVectorElement(matKAll*TVecAllSub)) / sumVectorElement((matKAll+1/tdt*matCAll)*vecFratio);
		//vecFAllSum = matKAll * TVecAll + alpha * (matKAll + 1 / tdt * matCAll) * vecFratio;
		vecFAllSum = alpha * 1 / tdt * matCAll * vecFratio;
		//vecFAllSum = matKAll * TVecAllSub + alpha * (matKAll + 1 / tdt * matCAll) * vecFratio;

		//for(unsigned i=0; i < mesh->vertices.size(); i++){
		//	DSTR << vecFAllSum[i] << std::endl;
		//}
		//DSTR << std::endl;
	}
}

void PHFemThermo::CalcHeatTransDirect3(double dt){
	PHFemMeshNew* mesh = GetPHFemMesh();
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	for(unsigned i=0; i < mesh->vertices.size(); i++){
	//		DSTR << vecFAllSum[i] << std::endl;
	//	}
	//	DSTR << std::endl;
	//}

	//直接法利用

	double eps =0.5;
	double eps2 = 0.5;
	double eps3 = 1.0;
	eps = 1.0;
	
//	DSTR << "tets[0].volume: " << tets[0].volume << std::endl;
	
	TVecAll2.resize(mesh->vertices.size());
	TVecAll2 = TVecAll;


	PTM::VVector<double> TVecAll3;	
	//// define @.h
	TVecAll3.resize(mesh->vertices.size());
	TVecAll3 = TVecAll;


	//	係数行列の作成
	keisu.resize(mesh->vertices.size(),mesh->vertices.size());
	keisu.clear();

	PTM::VVector<double> uhen;
	uhen.resize(mesh->vertices.size(),1);
	uhen.clear();

	double TEMP =0.0;
	for(unsigned i=0; i<mesh->vertices.size();i++){
		TEMP += vecFAllSum[i];
	}

	double alpha = 0;
	PTM::VVector<double> vecFratio;
	vecFratio.resize(mesh->vertices.size());
	vecFratio.clear();

	if(vecFAllSum.norm() != 0){
		double fmin = 0;
		for(unsigned i=0; i < mesh->vertices.size(); i++){
			if(fmin == 0){
				if(vecFAllSum[i] != 0){
					fmin = vecFAllSum[i];
				}
			}else{
				if(vecFAllSum[i] > 0 && vecFAllSum[i] < fmin){
					fmin = vecFAllSum[i];
				}
			}
		}

		vecFratio = 1 / fmin * vecFAllSum;
		alpha = sumVectorElement(vecFAllSum) / sumVectorElement(1/tdt*matCAll*vecFratio);
		//vecFAllSum = alpha * 1 / tdt * matCAll * vecFratio;

		//for(unsigned i=0; i < mesh->vertices.size(); i++){
		//	DSTR << vecFAllSum[i] << std::endl;
		//}
		//DSTR << std::endl;
	}

	keisu = eps * matKAll + 1 / dt * matCAll; 
	
	if(deformed){ 
		keisuInv = keisu.inv();
		deformed = false;
		rightkeisu = keisuInv*matCAll;
		//for(unsigned i=0; i < mesh->vertices.size(); i++){
		//	for(unsigned j=0; j < mesh->vertices.size(); j++){
		//		if(i != j){
		//			if(rightkeisu[i][j] < 0){
		//				rightkeisu[i][i] += 2 * rightkeisu[i][j];
		//				rightkeisu[i][j] = (-1) * rightkeisu[i][j];
		//			}
		//		}
		//	}
		//}
	}

	TVecAll = 1/dt*rightkeisu*(TVecAll+alpha*vecFratio);
}

void PHFemThermo::CalcHeatTransDirect4(double dt){
	PHFemMeshNew* mesh = GetPHFemMesh();
	//if(strcmp(mesh->GetName(), "femNsteak") == 0){
	//	for(unsigned i=0; i < mesh->vertices.size(); i++){
	//		DSTR << vecFAllSum[i] << std::endl;
	//	}
	//	DSTR << std::endl;
	//}

	//直接法利用

	double eps =0.5;
	double eps2 = 0.5;
	double eps3 = 1.0;
	eps = 1.0;
	
//	DSTR << "tets[0].volume: " << tets[0].volume << std::endl;
	
	TVecAll2.resize(mesh->vertices.size());
	TVecAll2 = TVecAll;


	PTM::VVector<double> TVecAll3;	
	//// define @.h
	TVecAll3.resize(mesh->vertices.size());
	TVecAll3 = TVecAll;


	//	係数行列の作成
	keisu.resize(mesh->vertices.size(),mesh->vertices.size());
	keisu.clear();

	PTM::VVector<double> uhen;
	uhen.resize(mesh->vertices.size(),1);
	uhen.clear();

	double TEMP =0.0;
	for(unsigned i=0; i<mesh->vertices.size();i++){
		TEMP += vecFAllSum[i];
	}

	double alpha = 0;
	PTM::VVector<double> vecFratio;
	vecFratio.resize(mesh->vertices.size());
	vecFratio.clear();

	for(unsigned i=0; i < mesh->vertices.size(); i++){
		vecFratio[i] = vecFAllSum[i] / vertexVars[i].vVolume;
	}

	if(vecFratio.norm() != 0){
		double fmin = 0;
		for(unsigned i=0; i < mesh->vertices.size(); i++){
			if(fmin == 0){
				if(vecFratio[i] != 0){
					fmin = vecFratio[i];
				}
			}else{
				if(vecFratio[i] > 0 && vecFratio[i] < fmin){
					fmin = vecFratio[i];
				}
			}
		}

		vecFratio = 1 / fmin * vecFratio;
		alpha = sumVectorElement(vecFAllSum) / sumVectorElement(1/tdt*matCAll*vecFratio);
		//vecFAllSum = alpha * 1 / tdt * matCAll * vecFratio;

		//for(unsigned i=0; i < mesh->vertices.size(); i++){
		//	DSTR << vecFAllSum[i] << std::endl;
		//}
		//DSTR << std::endl;
	}

	keisu = eps * matKAll + 1 / dt * matCAll; 
	
	if(deformed){ 
		keisuInv = keisu.inv();
		deformed = false;
		rightkeisu = keisuInv*matCAll;
		//for(unsigned i=0; i < mesh->vertices.size(); i++){
		//	for(unsigned j=0; j < mesh->vertices.size(); j++){
		//		if(i != j){
		//			if(rightkeisu[i][j] < 0){
		//				rightkeisu[i][i] += 2 * rightkeisu[i][j];
		//				rightkeisu[i][j] = (-1) * rightkeisu[i][j];
		//			}
		//		}
		//	}
		//}
	}

	TVecAll = 1/dt*rightkeisu*(TVecAll+alpha*vecFratio);
}

void PHFemThermo::vecFOutToDSTR(){
	for(unsigned i=0; i < GetPHFemMesh()->vertices.size(); i++){
		DSTR << vecFAllSum[i] << std::endl;
	}
	DSTR << std::endl;
}

void PHFemThermo::vecFOutToFile(){
	std::ofstream vecFThermo;
	vecFThermo.open("vecFThermo.csv",std::ios::app);
	for(unsigned i=0; i < GetPHFemMesh()->vertices.size(); i++){
		vecFThermo << vecFAllSum[i] << ",";
	}
	vecFThermo << std::endl;
	vecFThermo.close();
}

void PHFemThermo::vecFBottomOutToFile(){
	std::ofstream vecFThermo;
	vecFThermo.open("vecFBottomThermo.csv",std::ios::app);
	for(unsigned i=0; i < bottomVertexNums.size(); i++){
		vecFThermo << vecFAllSum[bottomVertexNums[i]] << ",";
	}
	vecFThermo << std::endl;
	vecFThermo.close();
}

void PHFemThermo::vecFTopOutToFile(){
	std::ofstream vecFThermo;
	vecFThermo.open("vecFTopThermo.csv",std::ios::app);
	for(unsigned i=0; i < topVertexNums.size(); i++){
		vecFThermo << vecFAllSum[topVertexNums[i]] << ",";
	}
	vecFThermo << std::endl;
	vecFThermo.close();
}

//PTM::VMatrixRow<double> PHFemThermo::inv(PTM::VMatrixRow<double> mat){
//	PTM::VMatrixRow<double> result;
//
//	result.resize(mat.height(),mat.width());
//	result.clear();
//
//	int m = mat.height();//行数
//	int n = mat.width();//列数
//	int lda = n;
//	double *A = new double[m*n];
//	int info;
//	int *ipiv = new int[std::min(m,n)];
//	int lwork = m;
//	double *work = new double[m];
//	
//	for(int i=0; i < m; i++){
//		for(int j=0; j < n; j++){
//			A[j*m+i] = mat[i][j];
//		}
//	}
//
//	dgetrf_(&n, &m, A, &lda, ipiv, &info);
//	dgetri_(&m, A, &lda, ipiv, work, &lwork, &info);
//
//	for(int i=0; i < m; i++){
//		for(int j=0; j < n; j++){
//			result[i][j] = A[j*m+i];
//		}
//	}
//
//	delete[] A;
//	delete[] ipiv;
//	delete[] work;
//
//	return result;
//}

}



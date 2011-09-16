/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHFemMeshThermo.h>
#include<Base/Affine.h>


using namespace PTM;

namespace Spr{;


PHFemMeshThermoDesc::PHFemMeshThermoDesc(){
	Init();
}
void PHFemMeshThermoDesc::Init(){
	//ディスクリプタに入れる値の初期化?
	

}

///////////////////////////////////////////////////////////////////
//	PHFemMeshThermo


PHFemMeshThermo::PHFemMeshThermo(const PHFemMeshThermoDesc& desc, SceneIf* s){
	SetDesc(&desc);
	if (s){ SetScene(s); }
}
void PHFemMeshThermo::Step(double dt){
	//	
}
void PHFemMeshThermo::CreateMatrix(){
}


void PHFemMeshThermo::SetDesc(const void* p) {
	PHFemMeshThermoDesc* d = (PHFemMeshThermoDesc*)p;
	PHFemMesh::SetDesc(d);
	
	//時間刻み幅	dtの設定
	PHFemMeshThermo::dt = 0.01;

	//行列を作る
		//行列を作るために必要な節点や四面体の情報は、PHFemMeshThermoの構造体に入っている。
			//PHFemMeshThermoのオブジェクトを作る際に、ディスクリプタに値を設定して作る
		
	//節点温度の初期設定(行列を作る前に行う)
	SetVerticesTemp(0.0);

			
	//節点の初期温度を設定する⇒{T}縦ベクトルに代入
		//{T}縦ベクトルを作成する。以降のK,C,F行列・ベクトルの節点番号は、この縦ベクトルの節点の並び順に合わせる?
		
	//dmnN 次元の温度の縦（列）ベクトル
	CreateTempMatrix();

	//熱伝導率、密度、比熱、熱伝達率　のパラメーターを設定・代入
		//PHFemMEshThermoのメンバ変数の値を代入 CADThermoより、0.574;//玉ねぎの値//熱伝導率[W/(ｍ・K)]　Cp = 1.96 * (Ndt);//玉ねぎの比熱[kJ/(kg・K) 1.96kJ/(kg K),（玉ねぎの密度）食品加熱の科学p64より970kg/m^3
		//熱伝達率の単位系　W/(m^2 K)⇒これはSI単位系なのか？　25は論文(MEAT COOKING SIMULATION BY FINITE ELEMENTS)のオーブン加熱時の実測値
		SetInitThermoConductionParam(0.574,970,1.96,25);
		//これら、変数値は後から計算の途中で変更できるようなSetParam()関数を作っておいたほうがいいかな？

	//計算に用いるマトリクス、ベクトルを作成（メッシュごとの要素剛性行列/ベクトル⇒全体剛性行列/ベクトル）
		//{T}縦ベクトルの節点の並び順に並ぶように、係数行列を加算する。係数行列には、面積や体積、熱伝達率などのパラメータの積をしてしまったものを入れる。
	
	//k2の行列を作る
	//CreateMatk2array();
	CreateMatkLocal();
	
	//CreateMatKall();

	//CreateC();
	//CreateF();
	//PrepareGaussSeidel();
		//ガウスザイデルで計算するために、クランクニコルソンの差分式の形で行列を作る。行列DやF、-bなどを作り、ガウスザイデルで計算ステップを実行直前まで
	//ガウスザイデルの計算を単位ステップ時間ごとに行う
		//ガウスザイデルの計算
		//CalcGaussSeidel();
	//（形状が変わったら、マトリクスやベクトルを作り直す）
	//温度変化・最新の時間の{T}縦ベクトルに記載されている節点温度を基に化学変化シミュレーションを行う
		//SetChemicalSimulation();
		//化学変化シミュレーションに必要な温度などのパラメータを渡す
	//温度変化や化学シミュレーションの結果はグラフィクス表示を行う
	

}


bool PHFemMeshThermo::GetDesc(void* p) const {
	PHFemMeshThermoDesc* d = (PHFemMeshThermoDesc*)p;
	return PHFemMesh::GetDesc(d);
}

void PHFemMeshThermo::CreateMatkLocal(){
	//k1を作る
	//CreateMatk1();

	//k2を作る
	for(unsigned i = 0; i< tets.size() ; i++){
		CreateMatk2(tets[i]);
		int hogehogehoge=0;
	}
}
double PHFemMeshThermo::CalcTriangleArea(int id0, int id1, int id2){
	double area=0.0;

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
		triarea[1][i] = vertices[id1].pos[i] - vertices[id0].pos[i];
	}
	for(unsigned i =0 ; i < 3 ; i++){
		//					x3(節点3のx(pos第i成分)目)	-	x1(〃)
		triarea[2][i] = vertices[id2].pos[i] - vertices[id0].pos[i];
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

void PHFemMeshThermo::CreateMatk2(Tet tets){
	//この計算を呼び出すときに、各四面体ごとに計算するため、四面体の0番から順にこの計算を行う
	//四面体を構成する4節点を節点の配列(Tetsには、節点の配列が作ってある)に入っている順番を使って、面の計算を行ったり、行列の計算を行ったりする。
	//そのため、この関数の引数に、四面体要素の番号を取る

	//最後に入れる行列を初期化
	for(unsigned i =0; i < 4 ;i++){
		for(unsigned j =0; j < 4 ;j++){
			Matk2[i][j] = 0.0;
		}
	}
	//1)	一時使用行列tempにすべて入れてしまう
	//|2 1 1 1 |
	//|1 2 1 1 |
	//|1 1 2 1 |
	//|1 1 1 2 |	を作る
	PTM::TMatrixRow<4,4,double> Matk2temp;
	for(int i =0; i <4 ; i++){
		for(int j=0; j < 4 ; j++){
			if(i==j){
				Matk2temp[i][j] = 2.0;
			}else{
				Matk2temp[i][j] = 1.0;
			}
		}
	}
	//for debug
	//DSTR << Matk2temp << std::endl;
	
	//2)	
	//l=0の時k21,1:k22, 2:k23, 3:k24	を生成
	for(unsigned l= 0 ; l < 4; l++){
		Matk2array[l] = Matk2temp;
		//	1行i列を0に
		for(int i=0;i<4;i++){
			Matk2array[l][l][i] = 0.0;
		}
		//	i行1列を0に
		for(int i=0;i<4;i++){
			Matk2array[l][i][l] = 0.0;
		}
		//	k_21	
		// =	|0 0 0 0 |
		//		|0 2 1 1 |
		//		|0 1 2 1 |
		//		|0 1 1 2 |
		//	for debug
		//DSTR <<"Matk2array[" << l << "] : " << std::endl;
		//DSTR << Matk2array[l] << std::endl;

		//係数の積をとる
		//この節点で構成される四面体の面積の積をとる

		//節点を見てみよう♪
		//for(unsigned i =0; i < 4 ; i++){
		//	DSTR << "k2"<< l << "行列の "<< i <<"番目の節点は" << tets.vertices[i] << std::endl;
		//}
	
		//四面体の節点1,2,3(0以外)で作る三角形の面積
		//l==0番目の時、 123	を代入する
		//l==1			0 23
		//l==2			01 3
		//l==3			012
		//をCalcTriangleAreaに入れることができるようにアルゴリズムを考える。
		//k21
		if(l==0){
			Matk2array[l] = heatTrans * (1.0/12.0) * CalcTriangleArea( tets.vertices[1],tets.vertices[2],tets.vertices[3] ) * Matk2array[l];
		}
		//	k22
		else if(l==1){
			Matk2array[l] = heatTrans * (1.0/12.0) * CalcTriangleArea( tets.vertices[0],tets.vertices[2],tets.vertices[3] ) * Matk2array[l];
		}
		//	k23
		else if(l==2){
			Matk2array[l] = heatTrans * (1.0/12.0) * CalcTriangleArea( tets.vertices[0],tets.vertices[1],tets.vertices[3] ) * Matk2array[l];
		}
		//	k24
		else if(l==3){
			Matk2array[l] = heatTrans * (1.0/12.0) * CalcTriangleArea( tets.vertices[0],tets.vertices[1],tets.vertices[2] ) * Matk2array[l];
		}
		//for debug
		//DSTR << "Matk2array[" << l << "]の完成版は↓" << std::endl;
		//DSTR << Matk2array[l] << std::endl;
	}

	//k2 = k21 + k22 + k23 + k24
	for(unsigned i=0; i < 4; i++){
		Matk2 += Matk2array[i];
		//	for debug
		//DSTR << "Matk2 に Matk2array = k2" << i+1 <<"まで加算した行列" << std::endl;
		//DSTR << Matk2 << std::endl;
	}
	
	DSTR << "節点（";
	for(unsigned i =0; i < 4; i++){
		DSTR << tets.vertices[i] << "," ;
	}
	DSTR << ")で構成される四面体の" << std::endl;
	DSTR << "Matk2 : " << std::endl;
	DSTR << Matk2 << std::endl;
	int hogeshishi =0;
}

void PHFemMeshThermo::SetInitThermoConductionParam(double thConduct0,double roh0,double specificHeat0,double heatTrans0){
	thConduct = thConduct0;
	roh = roh0;
	specificHeat = specificHeat0;
	heatTrans = heatTrans0;
}

void PHFemMeshThermo::CreateTempMatrix(){
	unsigned int dmnN = vertices.size();
	TVecAll.resize(dmnN,1);
	for(std::vector<unsigned int>::size_type i=0; i < dmnN ; i++){
		TVecAll[i][0] = vertices[i].temp;
	}
	//for Debug
	//for(unsigned int i =0; i < dmnN; i++){
	//	DSTR << i <<" : " << TVecAll[i][0] << std::endl;
	//}
	//for debug
	//for(std::vector<unsigned int>::size_type i =0; i < vertices.size(); i++){
	//	DSTR << i << " : " << &vertices[i] << std::endl;
	//}

}

void PHFemMeshThermo::SetVerticesTemp(double temp){
	for(std::vector<unsigned int>::size_type i=0; i < vertices.size() ; i++){
			vertices[i].temp = temp;
		}
}

void PHFemMeshThermo::CreateMatTest(){

	TMatrixRow<4,4,double> mat;              // 2行2列の行が詰った行列を宣言
				                               // TMatrixColだと列が詰った行列になる
	//四面体vectorなどから再帰命令などで入力
    mat[0][0] = 1;  mat[0][1] = 1;	mat[0][2] = 1;	mat[0][3] = 1;          // 0行0列 = 1;  0行1列 = 2;
    mat[1][0] = 3;  mat[1][1] = 4;	mat[1][2] = 2;	mat[1][3] = 2;          // 1行0列 = 3;  1行1列 = 4;
	mat[2][0] = 2;	mat[2][1] = 2;	mat[2][2] = 2;	mat[2][3] = 2;
	mat[3][0] = 2;	mat[3][1] = 2;	mat[3][2] = 2;	mat[3][3] = 2;

    TVector<2,float> vec;                   // 2次元のベクトルを宣言
    vec[0] = 1; vec[1] = 0;
    std::cout << mat;
    std::cout << vec << std::endl;
    std::cout << mat * vec << std::endl;    // 掛け算
    std::cout << mat + mat << std::endl;    // 足し算
    std::cout << mat - mat << std::endl;    // 引き算
    std::cout << mat.trans() << std::endl;  // 転置
    std::cout << mat.inv() << std::endl;    // 逆行列
    //return 0;

	double elemK2[4][4];
	double elemK3[4][4];
	//η=0 三行と三列を0にする。
	double alpha;	//熱伝達率をどこかで定義
	int hogehoge;//ξηζのどれを0にするか⇒どういう条件判定がいいのか知らん。
	for(int i=0; i<4; i++){
		for(int j=0; j<4 ;j++){
			if( i = j){
				elemK2[i][j] = 1/12;
			}
			elemK2[i][j] = 1/24;
			if(j=hogehoge){
				elemK2[i][j] =0.0;	//j列を0に
			}
			if(i=hogehoge){
				elemK2[i][j] =0.0;		//i行を0に
			}
		}
		
	}
}

}

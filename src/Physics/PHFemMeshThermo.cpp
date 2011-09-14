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
	
	//行列を作る
		//総節点数nVtxsOut、総要素数nTetsOut、節点座標(Vec3d* vtxsOut)、要素の構成(tetsOut?)などを登録
			//PHFemMeshThermoのオブジェクトを作る際に、ディスクリプタに値を設定して作る
		
		//節点の温度などの初期設定
			
		//節点の初期温度を設定する⇒{T}縦ベクトルに代入
			//{T}縦ベクトルを作成する。以降のK,C,F行列・ベクトルの節点番号は、この縦ベクトルの節点の並び順に合わせる。
			//CreateMeshTempVec();
			//次元はn
			unsigned int dmnN = vertices.size();
			Tvec.resize(dmnN,1);
			for(std::vector<unsigned int>::size_type i=0; i < dmnN ; i++){
				Tvec[i][0] = vertices[i].temp;
			}


		//熱伝導率、熱伝達率、密度、比熱などのパラメーターを設定・代入
			//PHFemMEshThermoのメンバ変数の値を代入
			//SetThermoConductionParam();
			//これら、変数値は後から計算の途中で変更できるようなSetParam()関数を作っておいたほうがいいかな？

		//計算に用いるマトリクス、ベクトルを作成（メッシュごとの要素剛性行列/ベクトル⇒全体剛性行列/ベクトル）
			//{T}縦ベクトルの節点の並び順に並ぶように、係数行列を加算する。係数行列には、面積や体積、熱伝達率などのパラメータの積をしてしまったものを入れる。
		//CreateK1();
		//CreateK2();
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

void PHFemMeshThermo::CreateK(){

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

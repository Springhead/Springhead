/*
 *  Copyright (c) 2003 - 2011, Fumihiro Kato, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <SciLab/SprSciLab.h>
#include <Physics/PHFemMeshThermo.h>
#include <Base/Affine.h>


using namespace PTM;

namespace Spr{;

#define UseMatAll

PHFemMeshThermoDesc::PHFemMeshThermoDesc(){
	Init();
}
void PHFemMeshThermoDesc::Init(){
	//ディスクリプタの生成時に呼ばれるコンストラクタで呼ばれる
	//ディスクリプタ生成時の初期化したいことを書きこむ
	//以下の関数の中で行っている初期化の類をここでやるのが良さそう
	//ディスクリプタに入れる値の初期化?
	

}

///////////////////////////////////////////////////////////////////
//	PHFemMeshThermo


PHFemMeshThermo::PHFemMeshThermo(const PHFemMeshThermoDesc& desc, SceneIf* s){
	deformed = true;			//変数の初期化、形状が変わったかどうか
	SetDesc(&desc);
	if (s){ SetScene(s); }
}

void PHFemMeshThermo::SetThermalBoundaryCondition(){
	
}

void PHFemMeshThermo::PrepareStep(){
	//ガウスザイデルに必要な、計算式の係数を計算する
	double dt = DCAST(PHSceneIf, GetScene())->GetTimeStep();
	//係数bやDMatAll_などをここで作る
	//bVecAllのリサイズ
	//bVecAllに計算結果を格納
	//ただし、[K],[C]などは全体剛性行列を作っているのではなく、成分ごとにEdges構造体に入っているので、この値を用いる
	//係数行列b生成ループ⇒このループをガウスザイデル計算の最初の一回だけやったほうが、forループが1回少なくなるので、計算速そう。けど、if文が必要
}

void PHFemMeshThermo::CreateMatKAll(){

}

void PHFemMeshThermo::CreateMatCAll(){

}


void PHFemMeshThermo::ScilabTest(){
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

void PHFemMeshThermo::UsingFixedTempBoundaryCondition(unsigned id,double temp){
	//温度固定境界条件
	SetVerticesTemp(id,temp);
	//for(unsigned i =0;i < vertices.size()/3; i++){
	//	SetVerticesTemp(i,temp);
	//}
}

void PHFemMeshThermo::UsingHeatTransferBoundaryCondition(){
	//熱伝達境界条件
	//節点の周囲流体温度の設定(K,C,Fなどの行列ベクトルの作成後に実行必要あり)
	//SetLocalFluidTemp(2,200.0);
}

void PHFemMeshThermo::CalcHeatTransUsingGaussSeidel(unsigned NofCyc,double dt){
	//dt = 0.0000000000001 * dt;		//デバッグ用に、dtをものすごく小さくしても、節点0がマイナスになるのか、調べた
	
	//dtはPHFemEngine.cppで取得する動力学シミュレーションのステップ時間
	bool DoCalc =true;											//初回だけ定数ベクトルbの計算を行うbool		//NofCycが0の時にすればいいのかも
	std::ofstream ofs("log.txt");
	for(unsigned i=0; i < NofCyc; i++){							//ガウスザイデルの計算ループ
		if(DoCalc){												
			if(deformed){												//D_iiの作成　形状が更新された際に1度だけ行えばよい
				for(unsigned j =0; j < vertices.size() ; j++){
					//for(unsigned k =0;k < vertices.size(); k++){
					//	DSTR << "DMatCAll "<< k << " : " << DMatCAll[0][k] << std::endl;
					//}
					_DMatAll.resize(1,vertices.size());
					_DMatAll[0][j] = 1.0/ ( 1.0/2.0 * DMatKAll[0][j] + 1.0/dt * DMatCAll[0][j] );		//1 / D__ii	を求める
					//1.0/dt = 500 
					//DSTR << "DMatKAll : "  << DMatKAll << std::endl;
					//DSTR << "DMatCAll : "  << DMatCAll << std::endl;
					//DSTR << "1.0/dt : " << 1.0/dt <<std::endl;
					//DSTR <<  1.0/dt *DMatCAll[0][j] << std::endl;		//0.001のオーダー
					//DSTR << 1.0/2.0 * DMatKAll[0][j] << std::endl;		//0.0003前後のオーダー
					//値が入っているかをチェック
					//DSTR << "_DMatAll[0][" << j << "] : " << _DMatAll[0][j]  << std::endl;
					int debughogeshi =0;
				}
				deformed = false;
				//	for DEBUG
				//DSTR << "_DMatAll : " << std::endl;
				//for(unsigned j =0; j < vertices.size() ;j++){
				//	DSTR << j << " : " << _DMatAll[0][j] << std::endl;
				//}
				//int hogeshi=0;
			}
			//	 1      1        1  
			//	--- ( - - [K] + ---[C] ){T(t)} + {F} 
			//	D_jj    2       ⊿t
			//

			for(unsigned j =0; j < vertices.size() ; j++){		//初回ループだけ	係数ベクトルbVecAllの成分を計算
				bVecAll[j][0] = 0.0;							//bVecAll[j][0]の初期化
				//節点が属すedges毎に　対角成分(j,j)と非対角成分(j,?)毎に計算
				//対角成分は、vertices[j].k or .c に入っている値を、非対角成分はedges[hoge].vertices[0] or vertices[1] .k or .cに入っている値を用いる
				//ⅰ)非対角成分について
				for(unsigned k =0;k < vertices[j].edges.size() ; k++){
					unsigned edgeId = vertices[j].edges[k];
					//リファクタリング	以下の条件分岐についてj>edges[edgeId].vertices[0] とそうでない時とで分けたほうが漏れが出る心配はない？
					if( j != edges[edgeId].vertices[0]){					//節点番号jとedges.vertices[0]が異なる節点番号の時:非対角成分
						unsigned vtxid0 = edges[edgeId].vertices[0];
						bVecAll[j][0] += (-1.0/2.0 * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid0][0];
					}
					else if( j != edges[edgeId].vertices[1] ){			//節点番号jとedges.vertices[1]が異なる節点番号の時:非対角成分
						unsigned vtxid1 = edges[edgeId].vertices[1];
						bVecAll[j][0] += (-1.0/2.0 * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid1][0];
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
				bVecAll[j][0] += (-1.0/2.0 * DMatKAll[0][j] + 1.0/dt * DMatCAll[0][j] ) * TVecAll[j][0];
				DSTR << "bVecAll[" << j <<"][0] : " << bVecAll[j][0] << std::endl;
				//{F}を加算
				bVecAll[j][0] += VecFAll[j][0];		//Fを加算
				//DSTR << " VecFAll[" << j << "][0] : "  << VecFAll[j][0] << std::endl;
				DSTR << std::endl;
				//D_iiで割る ⇒この場所は、ここで良いの？どこまで掛け算するの？
				bVecAll[j][0] = bVecAll[j][0] * _DMatAll[0][j];
				DSTR << "bVecAll[" << j <<"][0] * _DMatAll : " << bVecAll[j][0] << std::endl;
				DSTR << "TVecAll[" << j <<"][0] : " << TVecAll[j][0] << std::endl;
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
					TVecAll[j][0] = 0.0;
				}
		}
#endif
		//	 1      
		//	--- [F]{T(t+dt)}
		//	D_jj 		
		//[F] = 1/2 [K] +1/dt [C] から対角成分を除し(-1)をかけたもの
		//エッジに入っている成分に-1をかけるのではなく、最後に-1をかける。
		//
		for(unsigned j =0; j < vertices.size() ; j++){
			//T(t+dt) = の式
			//	まずtempkjを作る
			double tempkj = 0.0;			//ガウスザイデルの途中計算で出てくるFの成分計算に使用する一時変数
			for(unsigned k =0;k < vertices[j].edges.size() ; k++){
				unsigned edgeId = vertices[j].edges[k]; 
				if( j != edges[edgeId].vertices[0]){					//節点番号jとedges.vertices[0]が異なる節点番号の時:非対角成分		//OK
					unsigned vtxid0 = edges[edgeId].vertices[0];
					//DSTR << "TVecAll["<< vtxid0<<"][0] : " << TVecAll[vtxid0][0] <<std::endl;
					//TVecAll[j][0] +=_DMatAll[j][0] * -(1.0/2.0 * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid0][0] + bVecAll[j][0]; 
					//DSTR << "j : " << j << ", vtxid0 : " << vtxid0 <<", edges[edgeId].vertices[0] : " << edges[edgeId].vertices[0] <<  std::endl;
					tempkj += (1.0/2.0 * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid0][0];
				}
				else if( j != edges[edgeId].vertices[1] ){			//節点番号jとedges.vertices[1]が異なる節点番号の時:非対角成分
					unsigned vtxid1 = edges[edgeId].vertices[1];
					//DSTR << "TVecAll["<< vtxid1<<"][0] : " << TVecAll[vtxid1][0] <<std::endl;
					tempkj += (1.0/2.0 * edges[edgeId].k + 1.0/dt * edges[edgeId].c ) * TVecAll[vtxid1][0];
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
//			TVecAll[j][0] =	_DMatAll[0][j] * ( -1.0 * tempkj) - bVecAll[j][0];
			TVecAll[j][0] =	_DMatAll[0][j] * ( -1.0 * tempkj) + bVecAll[j][0];
			////	for DEBUG
			//int hofgeshi =0;
			//if(TVecAll[j][0] != 0.0){
			//	DSTR << "!=0 TVecAll["<< j<<"][0] : " << TVecAll[j][0] <<std::endl;
			//}
			//DSTR << i << "回目の計算、" << j <<"行目のtempkj: " << tempkj << std::endl;
			//tempkj =0.0;
			ofs << j << std::endl;
			ofs << "tempkj: "<< tempkj << std::endl;
			ofs << "DMatAll[0][j] * ( -1.0 * tempkj) :" <<_DMatAll[0][j] * ( -1.0 * tempkj) << std::endl;
			ofs << "bVecAll[j][0] :  " <<  bVecAll[j][0] << std::endl;
			ofs << "  TVecAll[j][0] : " << TVecAll[j][0] << std::endl;
			ofs << std::endl;
		}
		////	for Debug
		//for(unsigned j=0;j < vertices.size();j++){
		//	//DSTR << "tempk" << j << " : " << tempkj << std::endl;
		//	int hogeshi__ =0;
		//	//TVecAll[j][0]の計算結果を代入する
		//	//定数ベクトルbを上で計算、毎行でbVecAllを減算すればよい。
		//	DSTR << i << "回目の計算の " << "TVecAll[" << j << "][0] : " << TVecAll[j][0] << std::endl;
		//}
		//for(unsigned j=0;j < vertices.size();j++){
		//	//DSTR << "tempk" << j << " : " << tempkj << std::endl;
		//	int hogeshi__ =0;
		//	//TVecAll[j][0]の計算結果を代入する
		//	//定数ベクトルbを上で計算、毎行でbVecAllを減算すればよい。
		//	DSTR << i << "回目の計算の " << "bVecAll[" << j << "][0] : " << bVecAll[j][0] << std::endl;
		//}
		DSTR << i <<  "th Cyc" << std::endl; 
		DSTR << i << "回目の計算、TVecAll : " <<std::endl;
		DSTR << TVecAll << std::endl;
		ofs << i <<  "th Cyc" << std::endl;
		ofs << i << "回目の計算、TVecAll : " <<std::endl;
		ofs << TVecAll << std::endl;
		ofs << "bVecAll: " <<std::endl;
		ofs << bVecAll << std::endl;
		ofs << "_DMatAll: " <<std::endl; 
		ofs << _DMatAll <<std::endl;
		int piyopiyoyo =0;
		double tempTemp=0.0;
		for(unsigned j=0;j <vertices.size() ; j++){
			tempTemp += TVecAll[j][0];
		}
		DSTR << i <<"回目の計算時の　全節点の温度の和 : " << tempTemp << std::endl;
		DSTR << std::endl;
	}
}

void PHFemMeshThermo::UpdateVertexTempAll(unsigned size){
	for(unsigned i=0;i < size;i++){
		vertices[i].temp = TVecAll[i][0];
	}
}
void PHFemMeshThermo::UpdateVertexTemp(unsigned vtxid){
		vertices[vtxid].temp = TVecAll[vtxid][0];
}


void PHFemMeshThermo::Step(double dt){
	
//	ScilabTest();									//	Scilabを使うテスト
	//境界条件を設定:温度の設定
	UsingFixedTempBoundaryCondition(0,1.0);
	//
	CalcHeatTransUsingGaussSeidel(20,dt);			//ガウスザイデル法で熱伝導計算を解く

	//温度を表示してみる
	//DSTR << "vertices[3].temp : " << vertices[3].temp << std::endl;

	//温度のベクトルから節点へ温度の反映
	UpdateVertexTempAll(vertices.size());


	for(unsigned i =0;i<vertices.size();i++){
		if(vertices[i].temp !=0){
			DSTR << "vertices[" << i << "].temp : " << vertices[i].temp << std::endl;
		}
	}
	int hogehoge=0;


	//DSTR << "TVecAll : " <<std::endl;
	//DSTR << TVecAll << std::endl;
	//DSTR << "bVecAll : " <<std::endl;
	//DSTR << bVecAll << std::endl;


	//	for	DEBUG	節点3とエッジ対を作る節点を表示
	//for(unsigned i =0; i < vertices[3].edges.size();i++){
	//	DSTR << "edges[vertices[3].edges[" << i << "]].vertices[0] : " << edges[vertices[3].edges[i]].vertices[0] << std::endl;
	//	DSTR << "edges[vertices[3].edges[" << i << "]].vertices[1] : " << edges[vertices[3].edges[i]].vertices[1] << std::endl;
	//}
	//for(unsigned i =0; i < vertices[3].edges.size();i++){
	//	DSTR << "vertices[3].edges[" << i << "] : " << vertices[3].edges[i] << std::endl;
	//}
	int hogeshi = 0;
	//	for DEBUG
	//（形状が変わったら、マトリクスやベクトルを作り直す）
	//温度変化・最新の時間の{T}縦ベクトルに記載されている節点温度を基に化学変化シミュレーションを行う
		//SetChemicalSimulation();
		//化学変化シミュレーションに必要な温度などのパラメータを渡す
	//温度変化や化学シミュレーションの結果はグラフィクス表示を行う
}

void PHFemMeshThermo::CreateMatrix(){
}


void PHFemMeshThermo::SetDesc(const void* p) {
	PHFemMeshThermoDesc* d = (PHFemMeshThermoDesc*)p;
	PHFemMesh::SetDesc(d);
	
	////時間刻み幅	dtの設定
	//PHFemMeshThermo::dt = 0.01;

	//シーンから、シーンの時間ステップを取得する⇒以下のコードでは、GetTimeStepしようとすると、NULLが返ってくるので、PHEngineで行う
	//DSTR << DCAST(PHSceneIf, GetScene()) << std::endl;
	//DSTR << DCAST(PHSceneIf, GetScene())->GetTimeStep() << std::endl;
	//int hogeshimitake =0;


	//各種メンバ変数の初期化⇒コンストラクタでできたほうがいいかもしれない。
	//Edges
	for(unsigned i =0; i < edges.size();i++){
		edges[i].c = 0.0;
		edges[i].k = 0.0;
	}

	//行列の成分数などを初期化
	bVecAll.resize(vertices.size(),1);

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
//	CreateLocalMatrixAndSet();

	//各行列を作って、ガウスザイデルで計算するための係数の基本を作る。Timestepの入っている項は、このソース(SetDesc())では、実現できないことが分かった(NULLが返ってくる)
	CreateMatkLocal();
	
	//CreateMatKall();		//CreateMatkLocal();に実装したので、後程分ける。
	CreateMatcLocal();
	CreateVecfLocal();
	int hogeshidebug =0;
}



void PHFemMeshThermo::CreateLocalMatrixAndSet(){
	//K,C,Fの行列を作る関数を呼び出して、作らせる
	for(unsigned i = 0; i< tets.size() ; i++){
		//tetsを引数にいれると、その行列・ベクトルを作ってくれて、できた行列、ベクトルを基に係数を設定しくれる
		//こうすれば、各要素剛性行列でfor文を回さなくてもよくなる
		//CreateMatkLocal(tets);
		//CreateMatcLocal(tets);
		//CreateVecfLocal(tets);

		//tetsを入れて作らせる
//		SetkcfParam(tets);

	}

}
void PHFemMeshThermo::SetkcfParam(Tet tets){
}

bool PHFemMeshThermo::GetDesc(void* p) const {
	PHFemMeshThermoDesc* d = (PHFemMeshThermoDesc*)p;
	return PHFemMesh::GetDesc(d);
}

void PHFemMeshThermo::CreateMatc(Tet tets){
	//最後に入れる行列を初期化
	for(unsigned i =0; i < 4 ;i++){
		for(unsigned j =0; j < 4 ;j++){
			Matc[i][j] = 0.0;
		}
	}
	//Matc に21でできた行列を入れる
	Matc = Create44Mat21();
	//	for debug
		//DSTR << "Matc " << Matc << " ⇒ ";
	Matc = roh * specificHeat * CalcTetrahedraVolume(tets) / 20.0 * Matc;
	//	debug	//係数の積をとる
		//DSTR << Matc << std::endl;
		//int hogemat =0 ;
}

void PHFemMeshThermo::CreateMatcLocal(){
	//	使用する行列の初期化
	//DMatCAll：対角行列の成分の入った行列のサイズを定義:配列として利用	幅:vertices.size(),高さ:1
	DMatCAll.resize(1,vertices.size()); //(h,w)
	//値の初期化
	for(unsigned i=0;i<vertices.size();i++){
		DMatCAll[0][i] = 0.0;
	}

	//Matcの初期化は、Matcを作る関数でやっているので、省略
	//すべての要素について係数行列を作る
	for(unsigned i = 0; i< tets.size() ; i++){
		//c
		CreateMatc(tets[i]);
		int mathoge=0;
		//	(ガウスザイデルを使った計算時)要素毎に作った行列の成分より、エッジに係数を格納する
		//	or	(ガウスザイデルを使わない計算時)要素ごとの計算が終わるたびに、要素剛性行列の成分だけをエッジや点に作る変数に格納しておく	#ifedefでモード作って、どちらもできるようにしておいても良いけどw
		for(unsigned j=1; j < 4; j++){
			int vtxid0 = tets[i].vertices[j];
			//	下三角行列部分についてのみ実行
			//	j==1:k=0, j==2:k=0,1, j==3:k=0,1,2
			for(unsigned k = 0; k < j; k++){
				int vtxid1 = tets[i].vertices[k];
					for(unsigned l =0; l < vertices[vtxid0].edges.size(); l++){
						for(unsigned m =0; m < vertices[vtxid1].edges.size(); m++){
							if(vertices[vtxid0].edges[l] == vertices[vtxid1].edges[m]){
								edges[vertices[vtxid0].edges[l]].c += Matc[j][k];		//同じものが二つあるはずだから半分にする。上三角化下三角だけ走査するには、どういうfor文ｓにすれば良いのか？
								//DSTR << edges[vertices[vtxid0].edges[l]].k << std::endl;
							}
						}
					}
			}
		}
		//対角成分を対角成分の全体剛性行列から抜き出した1×nの行列に代入する
		//j=0~4まで代入(上のループでは、jは対角成分の範囲しかないので、値が入らない成分が出てしまう)
		for(unsigned j =0;j<4;j++){
			DMatCAll[0][tets[i].vertices[j]] += Matc[j][j];
		}
	}

	//	for debug
	//DSTR << "DMatCAll : " << std::endl;
	//for(unsigned j =0;j < vertices.size();j++){
	//	DSTR << j << "th : " << DMatCAll[0][j] << std::endl;
	//}
	// ネギについて非0成分になった。

	//	調べる
	//DMatKAllの成分のうち、0となる要素があったら、エラー表示をするコードを書く
	// try catch文にする
	for(unsigned j = 0; j < vertices.size() ; j++){
		if(DMatCAll[0][j] ==0.0){
			DSTR << "DMatCAll[0][" << j << "] element is blank" << std::endl;
		}
	}
	int piyodebug =0;
}


void PHFemMeshThermo::CreateVecfLocal(){
	//Vecfの初期化
	for(unsigned i =0; i < 4 ; i++){
			Vecf[i][0] = 0.0;
	}
	//全体剛性ベクトルFのサイズを規定
	VecFAll.resize(vertices.size(),1);

	//すべての要素について係数行列を作る
	for(unsigned i = 0; i< tets.size() ; i++){
		//f1を作る
		//f2を作る
		//f3を作る
		CreateVecf3(tets[i]);
		//f4を作る
		int hogehoge=0;
		//f1,f2,f3,f4を加算する
		Vecf = Vecf3;	
		//	for debug
		//DSTR << "Vecf : " << std::endl;
		//DSTR << Vecf << std::endl;
		//DSTR << "Vecf3 : " << std::endl;
		//DSTR << Vecf3 << std::endl;
		//	(ガウスザイデルを使った計算時)要素毎に作った行列の成分より、エッジに係数を格納する
		//	or	(ガウスザイデルを使わない計算時)要素ごとの計算が終わるたびに、要素剛性行列の成分だけをエッジや点に作る変数に格納しておく	#ifedefでモード作って、どちらもできるようにしておいても良いけどw

		//要素の節点番号の場所に、その節点のfの値を入れる
		//j:要素の中の何番目か
		for(unsigned j =0;j < 4; j++){
			int vtxid0 = tets[i].vertices[j];
			VecFAll[vtxid0][0] = Vecf[j][0];
		}
	}
	//	for debug
	//VecFAllに値が入ったのかどうかを調べる 2011.09.21全部に値が入っていることを確認した
	//DSTR << "VecFAll : " << std::endl;
	//for(unsigned j =0; j < vertices.size() ; j++){
	//	DSTR << j << " ele is :  " << VecFAll[j][0] << std::endl;
	//}
	//int hogeshi =0;

	////	調べる
	////DVecFAllの成分のうち、0となる要素があったら、エラー表示をするコードを書く
	//// try catch文にする
	//for(unsigned j = 0; j < vertices.size() ; j++){
	//	if(VecFAll[j][0] ==0.0){
	//		DSTR << "VecFAll[" << j << "][0] element is blank" << std::endl;
	//	}
	//}


	//	注意
	//	f3を使用する場合:周囲流体温度Tcが0の節点の要素は0になるため、温度の設定が必要
	//int hogef =0;
}

void PHFemMeshThermo::CreateMatkLocal(){
	//Matkの初期化
	for(unsigned i =0; i < 4 ; i++){
		for(unsigned j =0; j < 4 ; j++){
			Matk[i][j] = 0.0;
		}
	}
	//DMatAll：対角行列の成分の入った行列のサイズを定義:配列として利用	幅:vertices.size(),高さ:1
	DMatKAll.resize(1,vertices.size()); //(h,w)
		//値の初期化
	for(unsigned i=0;i<vertices.size();i++){
		DMatKAll[0][i] = 0.0;
	}

#ifdef UseMatAll
	//MatKAllの初期化
	MatKAll.resize(vertices.size(),vertices.size());
	for(unsigned i=0;i<vertices.size();i++){
		for(unsigned j=0;j<vertices.size();j++){
			MatKAll[i][j] = 0.0;
		}
	}
#endif


	//すべての要素について係数行列を作る
	for(unsigned i = 0; i< tets.size() ; i++){
		//	k1を作る	体積の求積関数
		CreateMatk1k(tets[i]);
//		CreateMatk1b(tets[i]);
		//k2を作る
		CreateMatk2(tets[i]);
		int hogehogehoge=0;
		//k1,k2,k3を加算する(使っている数値だけ)
		Matk = Matk1 + Matk2;	
		//	for debug
		//DSTR << "Matk1 : " << std::endl;
		//DSTR << Matk1 << std::endl;
		//DSTR << "Matk2 : " << std::endl;
		//DSTR << Matk2 << std::endl;
		//DSTR << "Matk : " << std::endl;
		//DSTR << Matk << std::endl;
		//int hogeshi =0;


		//	(ガウスザイデルを使った計算時)要素毎に作った行列の成分より、エッジに係数を格納する
		//	or	(ガウスザイデルを使わない計算時)要素ごとの計算が終わるたびに、要素剛性行列の成分だけをエッジや点に作る変数に格納しておく	#ifedefでモード作って、どちらもできるようにしておいても良いけどw
		//	Edges のこの要素で計算したK行列の成分をkに係数として格納する
		
		//Matkの対角成分以外で、下三角の部分の値を、edgeのkに代入する
		//
		//DSTR << i <<"th tetrahedra element'edges[vertices[vtxid0].edges[l]].k (All edges.k' value): " << std::endl;
		for(unsigned j=1; j < 4; j++){
			int vtxid0 = tets[i].vertices[j];
			//	下三角行列部分についてのみ実行
			//	j==1:k=0, j==2:k=0,1, j==3:k=0,1,2
			for(unsigned k = 0; k < j; k++){
				int vtxid1 = tets[i].vertices[k];
					for(unsigned l =0; l < vertices[vtxid0].edges.size(); l++){
						for(unsigned m =0; m < vertices[vtxid1].edges.size(); m++){
							if(vertices[vtxid0].edges[l] == vertices[vtxid1].edges[m]){
								edges[vertices[vtxid0].edges[l]].k += Matk[j][k];		//同じものが二つあるはずだから半分にする。上三角化下三角だけ走査するには、どういうfor文ｓにすれば良いのか？
								//DSTR << edges[vertices[vtxid0].edges[l]].k << std::endl;
							}
						}
					}
			}
		}

#ifdef UseMatAll
		//SciLabで使うために、全体剛性行列を作る
		//Matkから作る
		for(unsigned j=0; j<4 ; j++){
			for(unsigned k=0; k<4 ;k++){
				MatKAll[tets[i].vertices[j]][tets[i].vertices[k]] += Matk[j][k];
			}
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
		//						edges[vertices[vtxid0].edges[l]].k += Matk[j][k];		//同じものが二つあるはずだから半分にする。上三角化下三角だけ走査するには、どういうfor文ｓにすれば良いのか？
		//						//DSTR << edges[vertices[vtxid0].edges[l]].k << std::endl;
		//					}
		//				}
		//			}
		//	}
		//}

#endif

		//対角成分を対角成分の全体剛性行列から抜き出した1×nの行列に代入する
		//j=0~4まで代入(上のループでは、jは対角成分の範囲しかないので、値が入らない成分が出てしまう)
		for(unsigned j =0;j<4;j++){
			DMatKAll[0][tets[i].vertices[j]] += Matk[j][j];
			DSTR << "Matk[" << j << "][" << j << "] : " << Matk[j][j] << std::endl;
			DSTR << "DMatKAll[0][" << tets[i].vertices[j] << "] : " << DMatKAll[0][tets[i].vertices[j]] << std::endl;
			int hoge4 =0;
		}
		DSTR << std::endl;	//改行
	}//	四面体のfor文の最後
	
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
	//DMatKAllの成分のうち、0となる要素があったら、エラー表示をするコードを書く
	// try catch文にする
	for(unsigned j = 0; j < vertices.size() ; j++){
		if(DMatKAll[0][j] ==0.0){
			DSTR << "DMatKAll[0][" << j << "] element is blank" << std::endl;
		}
	}

	DSTR << "MatKAll : " << MatKAll <<std::endl;
	DSTR << "DMatKAll : " <<DMatKAll << std::endl;
	for(unsigned j =0;j<vertices.size();j++){
		if(MatKAll[j][j] != DMatKAll[0][j]){
			DSTR << j <<" 成分の要素はおかしい！調査が必要である。 " <<std::endl;
		}
	}
	int hoge5 =0;

}

void PHFemMeshThermo::CreateMatk1b(Tet tets){
	//yagawa1983を基にノートに式展開した計算式
	unsigned i=0;
	unsigned j=0;
	unsigned k=0;
	unsigned l=0;
	//double a[4];
	DSTR << i <<std::endl;
	DSTR << j <<std::endl;
	DSTR << k <<std::endl;
	DSTR << l <<std::endl;
	//double a[i]=0.0;


}

void PHFemMeshThermo::CreateMatk1k(Tet tets){
	//この計算を呼び出すときに、各四面体ごとに計算するため、四面体の0番から順にこの計算を行う
	//四面体を構成する4節点を節点の配列(Tetsには、節点の配列が作ってある)に入っている順番を使って、面の計算を行ったり、行列の計算を行ったりする。
	//そのため、この関数の引数に、四面体要素の番号を取る

	//最後に入れる行列を初期化
	for(unsigned i =0; i < 4 ;i++){
		for(unsigned j =0; j < 4 ;j++){
			Matk1[i][j] = 0.0;
		}
	}

	//	A行列　=	a11 a12 a13
	//				a21 a22 a23
	//				a31 a32 a33
	//を生成
	PTM::TMatrixRow<4,4,double> Matk1A;
	FemVertex p[4];
	for(unsigned i = 0; i< 4 ; i++){
		p[i]= vertices[tets.vertices[i]];
	}
	
	Matk1A[0][0] = (p[2].pos.y - p[0].pos.y) * (p[3].pos.z - p[0].pos.z) - (p[2].pos.z - p[0].pos.z) * (p[3].pos.y - p[0].pos.y);
	Matk1A[0][1] = (p[1].pos.z - p[0].pos.z) * (p[3].pos.y - p[0].pos.y) - (p[1].pos.y - p[0].pos.y) * (p[3].pos.z - p[0].pos.z);
	Matk1A[0][2] = (p[1].pos.y - p[0].pos.y) * (p[2].pos.z - p[0].pos.z) - (p[1].pos.z - p[0].pos.z) * (p[2].pos.y - p[0].pos.y);

	Matk1A[1][0] = (p[2].pos.z - p[0].pos.z) * (p[3].pos.x - p[0].pos.x) - (p[2].pos.x - p[0].pos.x) * (p[3].pos.z - p[0].pos.z);
	Matk1A[1][1] = (p[1].pos.x - p[0].pos.x) * (p[3].pos.z - p[0].pos.z) - (p[1].pos.z - p[0].pos.z) * (p[3].pos.x - p[0].pos.x);
	Matk1A[1][2] = (p[1].pos.z - p[0].pos.z) * (p[2].pos.x - p[0].pos.x) - (p[1].pos.x - p[0].pos.x) * (p[2].pos.z - p[0].pos.z);

	Matk1A[2][0] = (p[2].pos.x - p[0].pos.x) * (p[3].pos.y - p[0].pos.y) - (p[2].pos.y - p[0].pos.y) * (p[3].pos.x - p[0].pos.x);
	Matk1A[2][1] = (p[1].pos.y - p[0].pos.y) * (p[3].pos.x - p[0].pos.x) - (p[1].pos.x - p[0].pos.x) * (p[3].pos.y - p[0].pos.y);
	Matk1A[2][2] = (p[1].pos.x - p[0].pos.x) * (p[2].pos.y - p[0].pos.y) - (p[1].pos.y - p[0].pos.y) * (p[2].pos.x - p[0].pos.x);

	//	for	debug
	//DSTR << Matk1A[0][0] << std::endl;
	//DSTR << "Matk1A : " << Matk1A << std::endl; 
	//int hogeshi =0;

	//a11 ~ a33 を行列に入れて、[N~T] [N] を計算させる
	
	PTM::TMatrixRow<1,4,double> Nx;
	PTM::TMatrixRow<1,4,double> Ny;
	PTM::TMatrixRow<1,4,double> Nz;

	Nx[0][0] = -Matk1A[0][0] - Matk1A[0][1] -Matk1A[0][2];
	Nx[0][1] = Matk1A[0][0];
	Nx[0][2] = Matk1A[0][1];
	Nx[0][3] = Matk1A[0][2];

	Ny[0][0] = -Matk1A[1][0] - Matk1A[1][1] -Matk1A[1][2];
	Ny[0][1] = Matk1A[1][0];
	Ny[0][2] = Matk1A[1][1];
	Ny[0][3] = Matk1A[1][2];

	Nz[0][0] = -Matk1A[2][0] - Matk1A[2][1] -Matk1A[2][2];
	Nz[0][1] = Matk1A[2][0];
	Nz[0][2] = Matk1A[2][1];
	Nz[0][3] = Matk1A[2][2];

	//	Km の算出
	Matk1 = Nx.trans() * Nx + Ny.trans() * Ny + Nz.trans() * Nz;
	
	////	for debug
	//DSTR << "Nx : " << Nx << std::endl;
	//DSTR << "Nx^T : " << Nx.trans() << std::endl;
	//DSTR << "Nx^T * Nx : " << Nx.trans() * Nx << std::endl;
	//DSTR << "Ny^T * Ny : " << Ny.trans() * Ny << std::endl;
	//DSTR << "Nz^T * Nz : " << Nz.trans() * Nz << std::endl;
	//DSTR << "Matk1 : " << Matk1 << std::endl;
	//int hogehoge =0;

	//K1
	Matk1 = thConduct / (36 * CalcTetrahedraVolume(tets) ) * Matk1;
	//DSTR << "Matk1 : " << Matk1 << std::endl;
	//int hogedebug =0;
}

void PHFemMeshThermo::CreateVecf3(Tet tets){
	//	初期化
	for(unsigned i =0; i < 4 ;i++){
		Vecf3[i][0] = 0.0;		//最後に入れる行列を初期化
		vertices[tets.vertices[i]].Tc =0.0;	//
	}	
	//l=0の時f31,1:f32, 2:f33, 3:f34	を生成
	for(unsigned l= 0 ; l < 4; l++){
		//Matk2array[l] = Matk2temp;
		Vecf3array[l] = Create41Vec1();
		//	l行を0に
		//for(int i=0;i<4;i++){
		//	Vecf3array[l][l][i] = 0.0;
		//}
		Vecf3array[l][l][0] = 0.0;
		//array[n][m][l]	= narray[n],m行l列
		//	f_3	(Vecf3array[0], Vecf3array[1],..)
		// =	| 0 | + | 1 |+...
		//		| 1 |   | 0 |
		//		| 1 |   | 1 |
		//		| 1 |   | 1 |
		//	for debug
		//DSTR << "Vecf3array[" << l << "] : " << std::endl;
		//DSTR << Vecf3array[l] << std::endl;

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
			Vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets.vertices[1],tets.vertices[2],tets.vertices[3] ) * Vecf3array[l];
			//DSTR << "Vecf3array[" << l << "] : " << Vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			for(unsigned m=0; m<4; m++){
				Vecf3array[l][m][0] = vertices[tets.vertices[m]].Tc * Vecf3array[l][m][0];
			}
		}
		//	k22
		else if(l==1){
			Vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets.vertices[0],tets.vertices[2],tets.vertices[3] ) * Vecf3array[l];
			//DSTR << "Vecf3array[" << l << "] : " << Vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			for(unsigned m=0; m<4; m++){
				Vecf3array[l][m][0] = vertices[tets.vertices[m]].Tc * Vecf3array[l][m][0];
			}
		}
		//	k23
		else if(l==2){
			Vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets.vertices[0],tets.vertices[1],tets.vertices[3] ) * Vecf3array[l];
			//DSTR << "Vecf3array[" << l << "] : " << Vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			for(unsigned m=0; m<4; m++){
				Vecf3array[l][m][0] = vertices[tets.vertices[m]].Tc * Vecf3array[l][m][0];
			}
		}
		//	k24
		else if(l==3){
			Vecf3array[l] = heatTrans * (1.0/3.0) * CalcTriangleArea( tets.vertices[0],tets.vertices[1],tets.vertices[2] ) * Vecf3array[l];
			//DSTR << "Vecf3array[" << l << "] : " << Vecf3array[l] << std::endl;
			//Vecの節点毎にその節点での周囲流体温度Tcとの積を行う
			for(unsigned m=0; m<4; m++){
				Vecf3array[l][m][0] = vertices[tets.vertices[m]].Tc * Vecf3array[l][m][0];
			}
		}
		//for debug
		//DSTR << "Vecf3array[" << l << "]の完成版は↓" << std::endl;
		//DSTR << Vecf3array[l] << std::endl;
		if(DMatCAll == NULL){
			//DSTR <<"i : "<< i << ", l : " << l << std::endl;
			DSTR <<"l : " << l << std::endl;
		}
	}

	//f3 = f31 + f32 + f33 + f34
	for(unsigned i=0; i < 4; i++){
		Vecf3 += Vecf3array[i];
		//	for debug
		//DSTR << "Vecf3 に Vecf3array = f3" << i+1 <<"まで加算した行列" << std::endl;
		//DSTR << Vecf3 << std::endl;
	}
	
	//	f1,f2,f3,f4	を計算する際に、[0][0]成分から[3][0]成分までの非0成分について、先にTcをかけてしまう


	//for debug
	//DSTR << "節点（";
	//for(unsigned i =0; i < 4; i++){
	//	DSTR << tets.vertices[i] << "," ;
	//}
	//DSTR << ")で構成される四面体の" << std::endl;
	//DSTR << "Vecf3 : " << std::endl;
	//DSTR << Vecf3 << std::endl;
	//int hogeshishi =0;
}


double PHFemMeshThermo::CalcTetrahedraVolume(Tet tets){
	PTM::TMatrixRow<4,4,double> tempMat44;
	for(unsigned i =0; i < 4; i++){
		for(unsigned j =0; j < 4; j++){
			if(i == 0){
				tempMat44[i][j] = 1.0;
			}
			else if(i == 1){
				tempMat44[i][j] = vertices[tets.vertices[j]].pos.x;
			}
			else if(i == 2){
				tempMat44[i][j] = vertices[tets.vertices[j]].pos.y;
			}
			else if(i == 3){
				tempMat44[i][j] = vertices[tets.vertices[j]].pos.z;
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

PTM::TMatrixCol<4,1,double> PHFemMeshThermo::Create41Vec1(){
	PTM::TMatrixCol<4,1,double> Mat1temp;
	for(int i =0; i <4 ; i++){
				Mat1temp[i][0] = 1.0;
	}
	return Mat1temp;
}

PTM::TMatrixRow<4,4,double> PHFemMeshThermo::Create44Mat21(){
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

	//2)	
	//l=0の時k21,1:k22, 2:k23, 3:k24	を生成
	for(unsigned l= 0 ; l < 4; l++){
		//Matk2array[l] = Matk2temp;
		Matk2array[l] = Create44Mat21();
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
	
	//for debug
	//DSTR << "節点（";
	//for(unsigned i =0; i < 4; i++){
	//	DSTR << tets.vertices[i] << "," ;
	//}
	//DSTR << ")で構成される四面体の" << std::endl;
	//DSTR << "Matk2 : " << std::endl;
	//DSTR << Matk2 << std::endl;
	//int hogeshishi =0;
}

void PHFemMeshThermo::SetInitThermoConductionParam(double thConduct0,double roh0,double specificHeat0,double heatTrans0){
	thConduct = thConduct0;
	roh = roh0;
	specificHeat = specificHeat0;
	heatTrans = heatTrans0;
}


void PHFemMeshThermo::SetTempAllToTVecAll(unsigned size){
	for(unsigned i =0; i < size;i++){
		TVecAll[i][0] = vertices[i].temp;
	}
}


void PHFemMeshThermo::CreateTempMatrix(){
	unsigned int dmnN = vertices.size();
	TVecAll.resize(dmnN,1);
	SetTempAllToTVecAll(dmnN);
	//for(std::vector<unsigned int>::size_type i=0; i < dmnN ; i++){
	//	TVecAll[i][0] = vertices[i].temp;
	//}

	//for Debug
	//for(unsigned int i =0; i < dmnN; i++){
	//	DSTR << i <<" : " << TVecAll[i][0] << std::endl;
	//}
	//for debug
	//for(std::vector<unsigned int>::size_type i =0; i < vertices.size(); i++){
	//	DSTR << i << " : " << &vertices[i] << std::endl;
	//}

}


void PHFemMeshThermo::SetTempToTVecAll(unsigned vtxid){
	TVecAll[vtxid][0] = vertices[vtxid].temp;
}

void PHFemMeshThermo::SetLocalFluidTemp(unsigned i,double temp){
	vertices[i].Tc = temp;			//節点の周囲流体温度の設定
}

void PHFemMeshThermo::SetVerticesTemp(unsigned i,double temp){
	vertices[i].temp = temp;
	SetTempToTVecAll(i);
}

void PHFemMeshThermo::SetVerticesTemp(double temp){
	for(std::vector<unsigned int>::size_type i=0; i < vertices.size() ; i++){
			vertices[i].temp = temp;
		}
}


}



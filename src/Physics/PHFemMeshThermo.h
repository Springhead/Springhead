/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHFEMMESHTHERMO_H
#define PHFEMMESHTHERMO_H

#include <Physics/PHFemMesh.h>

namespace Spr{;

///	熱のFEM用のメッシュ
class PHFemMeshThermo: public PHFemMesh{
public:
	SPR_OBJECTDEF(PHFemMeshThermo);

	//	頂点
	struct StateVar{
		double temperature;
	};
	struct Coeff{
	};
	std::vector<StateVar> vertexVars;
	std::vector<Coeff> edgeCoeffs;
	
	PHFemMeshThermo(const PHFemMeshThermoDesc& desc=PHFemMeshThermoDesc(), SceneIf* s=NULL);
	///	デスクリプタのサイズ
	virtual size_t GetDescSize() const { return sizeof(PHFemMeshThermoDesc); };
	///	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* desc) const;
	///デスクリプタの設定。
	virtual void SetDesc(const void* desc);
	///熱伝導シミュレーションでエンジンが用いるステップ
	void Step(double dt);

	//行列の生成　小文字は要素剛性行列、大文字は全体剛性行列
	//節点温度ベクトルの	温度の	TVec:要素（縦）ベクトル(4x1)	TVecall:全体（縦）ベクトル(nx1)
	//熱伝導マトリクスの	k:k1~k3の加算したもの,	k1:体積分項,	面積分の	k2:熱伝達境界条件,	k3:熱輻射境界条件	積分
	//熱容量マトリクスの	c:体積分項
	//熱流束ベクトルの		f:f1~f4の加算したもの,	体積分の	f1:内部発熱による項,	面積分の	f2:熱流束境界条件,	f3:熱伝達境界条件,	f4:熱輻射境界条件
	//体積分の場合:要素1つにつき1つの行列、面積分の場合:要素内の各面ごとに1つで計4つの行列なので配列に入れる
	//kやfに関しては、面ごとに計算した係数行列を格納する配列Mat(k/f)arrayを定義
	//	Col:列単位の行列	Row:行単位の行列

	//節点温度ベクトル
	PTM::TMatrixCol<4,1,double> TVec;			//要素の節点温度ベクトル
	PTM::VMatrixCol<double> TVecAll;			//全体の節点温度ベクトル

	//要素の係数行列
	PTM::TMatrixRow<4,4,double> Matk1;			//CreateMatk1()
	PTM::TMatrixRow<4,4,double> Matk2;			//CreateMatk2()
	//int Matk2array[4];						//matk2が入った配列		//CreateMatk2array()
	PTM::TMatrixRow<4,4,double> Matk1array[4];	//Kmの3つの4×4行列の入れ物　Matk1を作るまでの間の一時的なデータ置場
	PTM::TMatrixRow<4,4,double> Matk2array[4];	//k21,k22,k23,k24の4×4行列の入れ物　Matkを作るまでの間の一時的なデータ置場
	PTM::TMatrixRow<4,4,double> Matk;			//Matk=Matk1+Matk2+Matk3	matk1~3を合成した要素剛性行列	CreateMatkLocal()
	PTM::TMatrixRow<4,4,double> Matc;			//
	PTM::TMatrixCol<4,1,double> Vecf3;			//f3:外側の面に面している面のみ計算する　要注意
	PTM::TMatrixCol<4,1,double> Vecf3array[4];	//f31,f32,f33,f34の4×1ベクトルの入れ物		Matkを作るまでの間の一時的なデータ置場
	PTM::TMatrixCol<4,1,double> Vecf;			//f1~f4を合算した縦ベクトル

	//全体の係数行列
	PTM::VMatrixRow<double> MatKAll;			//[K]の全体剛性行列		//CreateMatKall()
	PTM::VMatrixRow<double> MatCAll;			//[C]
	PTM::VMatrixCol<double> VecFAll;			//{F}の全体剛性ベクトル

	//全体の剛性行列の代わり
	PTM::VMatrixRow<double> DMatKAll;			//全体剛性行列Kの対角成分になるはずの値をそのまま代入		実装中での初期化の仕方	DMatKAll.resize(1,vertices.size());
	PTM::VMatrixRow<double> DMatCAll;			//全体剛性行列Cの対角成分
	PTM::VMatrixRow<double> _DMatAll;			//全体剛性行列KとCの対角成分の定数倍和の逆数をとったもの	ガウスザイデルの計算に利用する
	PTM::VMatrixCol<double> bVecAll;			//ガウスザイデルの計算に用いる定数行列bの縦ベクトル	Rowである必要はあるのか？⇒Colにした
	//double *constb;								//ガウスザイデルの係数bを入れる配列のポインタ	後で乗り換える

	void SetVerticesTemp(double temp);					//（節点温度の行列を作成する前に）頂点の温度を設定する（単位摂氏℃）
	void SetVerticesTemp(unsigned i,double temp);		// 節点iの温度をtemp度に設定し、それをTVEcAllに反映
	void SetLocalFluidTemp(unsigned i,double temp);		//	接点iの周囲の節点温度をtemp度に設定
	
	//熱伝達境界条件の時はすべての引数を満たす　温度固定境界条件を用いたいときには、熱伝達率（最後の引数）を入力しない。また、毎Step実行時に特定節点の温度を一定温度に保つようにする。
	void SetInitThermoConductionParam(double thConduct,double roh,double specificHeat,double heatTrans);		//熱伝導率、密度、比熱、熱伝達率などのパラメーターを設定・代入

	void SetThermalBoundaryCondition();				//	熱伝導境界条件の設定

protected:
	//熱伝導計算本体
	//[K]熱伝導マトリクスを作る関数群
	void CreateMatk1k(Tet tets);				//kimura方式の計算法
	void CreateMatk1b(Tet tets);				//yagawa1983の計算法の3次元拡張した計算法 b:book の意味
	void CreateMatk2(Tet tets);					//四面体ごとに作るので、四面体を引数に取る
	void CreateMatk2array();
	void CreateMatkLocal();
	void CreateMatKall();

	void CreateMatcLocal();
	void CreateMatc(Tet tets);					//cの要素剛性行列を作る関数
	void CreateVecfLocal();						//
	void CreateVecf3(Tet tets);					//
	void CreateTempMatrix();					//節点の温度が入った節点配列から、全体縦ベクトルを作る。	この縦行列の節点の並び順は、i番目の節点IDがiなのかな
	void CreateLocalMatrixAndSet();				//K,C,Fすべての行列・ベクトルについて要素剛性行列を作って、エッジに入れる	又は	全体剛性行列を作る関数
	void CreateMatrix();
	void SetkcfParam(Tet tets);					//エッジや頂点にk,c,fの要素剛性行列の係数を設定する関数	すべての四面体について要素剛性行列を求め、k,c,fに値を入れると、次の要素について処理を実行する	
	double CalcTriangleArea(int id0, int id2, int id3);		//節点IDを入れると、その点で構成された三角形の面積を求める　四面体での面積分で使用
	double CalcTetrahedraVolume(Tet tets);		////四面体のIDを入れると、その体積を計算してくれる関数
	void PrepareStep();							//Step()で必要な変数を計算する関数
	//double CalcbVec(int vtxid,
	void CalcHeatTransUsingGaussSeidel(unsigned NofCyc,double dt);		//	ガウスザイデル法で熱伝導を計算 NofCyc:繰り返し計算回数,dt:ステップ時間

	void SetTempAllToTVecAll(unsigned size);		//	TVecAllに全節点の温度を設定する関数
	void SetTempToTVecAll(unsigned vtxid);			//	TVecAllに特定の節点の温度を設定する関数
	void UpdateVertexTemp(unsigned vtxid);			//	計算結果としての温度をTVecAllから節点に更新する
	void UpdateVertexTempAll(unsigned size);		//	計算結果としての温度をTVecAllから全節点に更新する

	void ScilabTest();								//	Scilabを使ってみる関数
	void UsingFixedTempBoundaryCondition(unsigned id,double temp);			//	温度固定境界条件
	void UsingHeatTransferBoundaryCondition();		//	熱伝達境界条件

	PTM::TMatrixRow<4,4,double> Create44Mat21();	//共通で用いる、4×4の2と1でできた行列を返す関数
	//あるいは、引数を入れると、引数を変えてくれる関数
	PTM::TMatrixCol<4,1,double> Create41Vec1();		//共通で用いる、4×1の1でできた行列(ベクトル)を返す関数

	//熱計算に用いるパラメータ
	double thConduct;				//熱伝導率
	double heatTrans;				//熱伝達率
	double roh;						//密度
	double specificHeat;			//比熱
//	double dt;						//時間刻み幅

	bool deformed;					//形状が変わったかどうか
	
};




}	//	namespace Spr
#endif

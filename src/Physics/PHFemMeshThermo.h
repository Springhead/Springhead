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
	
	//節点温度ベクトル
	PTM::TMatrixCol<4,1,double> TVec;		//要素の節点温度ベクトル
	PTM::VMatrixCol<double> TVecAll;		//全体の節点温度ベクトル

	//要素の係数行列
	PTM::TMatrixRow<4,4,double> Matk1;		//CreateMatk1()
	PTM::TMatrixRow<4,4,double> Matk2;		//CreateMatk2()
	//int Matk2array[4];						//matk2が入った配列		//CreateMatk2array()
	PTM::TMatrixRow<4,4,double> Matk2array[4];	//k21,k22,k23,k24の4×4行列の入れ物
	PTM::TMatrixRow<4,4,double> Matk;		//Matk=Matk1+Matk2+Matk3	matk1~3を合成した要素剛性行列	CreateMatkLocal()
	PTM::TMatrixRow<4,4,double> Matc;		//
	PTM::TMatrixCol<4,1,double> Matf3;		//f3:外側の面に面している面のみ計算する　要注意
	int Matf3array[4];						//
	PTM::TMatrixCol<4,1,double> Matf;		//f1~f4を合算した縦ベクトル

	//全体の係数行列
	PTM::VMatrixRow<double> MatKall;		//[K]の全体剛性行列		//CreateMatKall()
	PTM::VMatrixRow<double> MatCall;		//[C]
	PTM::VMatrixRow<double> MatFall;		//{F}の全体剛性ベクトル

	void SetVerticesTemp(double temp);		//（節点温度の行列を作成する前に）頂点の温度を設定する（単位摂氏℃）
	//熱伝達境界条件の時はすべての引数を満たす　温度固定境界条件を用いたいときには、熱伝達率（最後の引数）を入力しない。また、毎Step実行時に特定節点の温度を一定温度に保つようにする。
	void SetInitThermoConductionParam(double thConduct,double roh,double specificHeat,double heatTrans);		//熱伝導率、密度、比熱、熱伝達率などのパラメーターを設定・代入


protected:
	//熱伝導計算本体
	//[K]熱伝導マトリクスを作る関数群
	void CreateMatk1();
	void CreateMatk2(Tet tet);		//四面体ごとに作るので、四面体を引数に取る
	void CreateMatk2array();
	void CreateMatkLocal();
	void CreateMatKall();

	void CreateMatTest();
	void CreteC();
	void CreateF();
	void CreateTempMatrix();	//節点の温度が入った節点配列から、全体縦ベクトルを作る。	この縦行列の節点の並び順は、i番目の節点IDがiなのかな
	void CreateMatrix();
	double CalcTriangleArea(int id0, int id2, int id3);		//節点IDを入れると、その点で構成された三角形の面積を求める　四面体での面積分で使用
	double CalcTetrahedraVol(int tetra);					//四面体のIDを入れると、その体積を計算してくれる関数

	//熱計算に用いるパラメータ
	double thConduct;		//熱伝導率
	double heatTrans;		//熱伝達率
	double roh;				//密度
	double specificHeat;	//比熱
	double dt;				//時間刻み幅
	
};




}	//	namespace Spr
#endif

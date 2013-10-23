/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/* ScilabとSpringhead2をリンクさせるためのサンプル
動作必要要件：api_scilab.dll, call_scilab.dll(Scilabインストール時にscilab\bin内にあるはず）
scilab-5.4で動作確認、別バージョンを利用したい場合はScilabStub.cpp:StartScilab()内にpathを記述する必要がある。
dllをdynamic linkして利用するため、ScilabSwig.batで中間コードScilabStub.cpp, ScilabStub.hpp, ScilabStubImpl.cxxを生成する必要がある。
また、Releaseモードで実行する場合にはdllの呼び出し規約を_cdeclにする必要がある。
その変更はScilabStub.cpp, ScilabStub.hpp, ScilabStubImpl.cxxで宣言されている関数ポインタを_cdeclで呼び出しにする。
ScilabStub.hpp, ScilabStubImpl.cxxはswigで自動生成されるので、参考ファイルとしてScilabStub.hpp.org, ScilabStubImpl.cxx.orgを参照すること。
*/
#include <SciLab/SprSciLab.h>
#include <Base/Affine.h>
#include <conio.h>
#include <Scilab/SprScilab.h>
using namespace Spr;

int _cdecl main(){
	if (!ScilabStart()){
		std::cout << "Error : ScilabStart \n";
	}
	//	行列の読み書き
	std::cout << "Springheadメモリ空間内に行列を生成" << std::endl;
	Matrix2f A;
	A.Ex() = Vec2f(1,2);
	A.Ey() = Vec2f(3,4);
	std::cout << "A:  " << A ;
	std::cout << "A00:" << A[0][0] << std::endl;	std::cout << "A01:" << A[0][1] << std::endl;
	std::cout << "A10:" << A[1][0] << std::endl;	std::cout << "A11:" << A[1][1] << std::endl;
	
	std::cout << std::endl;
	std::cout << "Scilabメモリ空間内に行列を生成し計算" << std::endl;
	ScilabSetMatrix("A", A);
	ScilabJob("b=[4;5]");
	std::cout << "A=";		ScilabJob("disp(A);");
	std::cout << "b=";		ScilabJob("disp(b);");
	std::cout << "Ax=b" << std::endl;		
	ScilabJob("x=A\\b;");
	std::cout << "x=";		ScilabJob("disp(x);");

	std::cout << std::endl;
	std::cout << "Scilabメモリ空間内の行列をSpringheadメモリ空間へコピー" << std::endl;
	PTM::VMatrixRow<double> b, x;
	ScilabGetMatrix(b, "b");
	ScilabGetMatrix(x, "x");
	std::cout << "A=" << A << std::endl;
	std::cout << "b=" << b << std::endl;
	std::cout << "x=" << x << std::endl;

	std::cout << std::endl;
	std::cout << "Scilabメモリ空間内で固有値計算" << std::endl;
	PTM::VMatrixCol<double> matk;
	matk.resize(5, 5, 0.0);
	matk.item(0, 0) = 200;		matk.item(0, 1) = -100;
	matk.item(1, 0) = -100;	    matk.item(1, 1) = 100;
	matk.item(2, 2) = 20;
	matk.item(3, 3) = 20;
	matk.item(4,4) = 1;

	PTM::VMatrixCol<double> matm;
	matm.resize(5, 5, 0.0);
	matm.item(0, 0) = 3;	matm.item(0, 2) = 2;
	matm.item(1, 1) = 3;	matm.item(1, 3) = 2;
	matm.item(2, 0) = 2;	matm.item(2, 2) = 4;
	matm.item(3, 1) = 2;	matm.item(3, 3) = 4;
	matm.item(4,4) = 1;

	ScilabSetMatrix("K", matk);
	ScilabSetMatrix("M", matm);
	ScilabSetMatrix("P", matk);
	ScilabSetMatrix("D", matm);
	ScilabJob("[P D] = spec(inv(M) * K);");
	ScilabJob("disp(P);");
	ScilabJob("disp(D);");
	/*
		Scilabで演算したものはSpringheadにコピーできない
		バグがあるので一部コメントアウト
	*/
	//PTM::VMatrixCol<double> P, D;
	//ScilabGetMatrix(P, "P");
	//ScilabGetMatrix(D, "D");

	//std::cout << "Scilabメモリ空間内の行列をSpringheadで計算" << std::endl;
	//ScilabJob("y = A;");
	//SCMatrix y = ScilabMatrix("y");
	//y = 2*A;
	//std::cout << "y = 2*A : Done by C++ code" << std::endl;
	//std::cout << "y=";	ScilabJob("disp(y);");
	//ScilabJob("clear;");

	//	グラフ描画
	ScilabJob("t = 0:0.01:2*3.141592653;");
	ScilabJob("x = sin(t);");
	ScilabJob("y = cos(t);");
	ScilabJob("plot2d(x, y);");
	for(int i=0; i<100000; ++i){
		ScilabJob("");
	}

	ScilabEnd();
	if(_getch())	return 0;
}

/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/* ScilabとSpringhead2をリンクさせるためのサンプル
動作必要要件：LibScilab.dll(Scilabインストール時にscilab\bin内にあるはず）
scilab-5.3.3で動作確認、別バージョンを利用したい場合はScilabStub.cpp:StartScilab()内に
pathを記述する必要がある。
また、Releaseモードで実行する場合にはdllの呼び出し規約を_cdeclにする必要がある。
その変更はScilabStub.cpp, ScilabStub.hpp, ScilabStubImpl.cxxで宣言されている関数ポインタを_cdeclで呼び出しにする。
これらのファイルはswigで自動生成されるので、参考ファイルとしてScilabStub.hpp.org, ScilabStubImpl.cxx.orgを参照すること。
*/
#include <SciLab/SprSciLab.h>
#include <Base/Affine.h>
using namespace Spr;

int main(){
	if (!ScilabStart()){
		std::cout << "Error : ScilabStart \n";
	}
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

	ScilabEnd();
	return 0;
}

/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <iostream>
#include <conio.h>
#include <Springhead.h>
#include <Foundation/UTClapack.h>

using namespace Spr;
using namespace PTM;

int _cdecl main()
{

	PTM::VMatrixRow<double> matk;
	matk.resize(5, 5, 0.0);
	matk.item(0, 0) = 200;		matk.item(0, 1) = -100;
	matk.item(1, 0) = -100;	    matk.item(1, 1) = 100;
	matk.item(2, 2) = 20;
	matk.item(3, 3) = 20;
	matk.item(4,4) = 1;

	VMatrixRow<double> matm;
	matm.resize(5, 5, 0.0);
	matm.item(0, 0) = 3;	matm.item(0, 2) = 2;
	matm.item(1, 1) = 3;	matm.item(1, 3) = 2;
	matm.item(2, 0) = 2;	matm.item(2, 2) = 4;
	matm.item(3, 1) = 2;	matm.item(3, 3) = 4;
	matm.item(4,4) = 1;

	int size = matk.height();
	VVector<double> e;
	e.resize(size, 0.0);
	VMatrixRow<double> v;
	v.resize(size, size, 0.0);
	sprsygv(matk, matm, e, v);
	std::cout << "eigen value" << std::endl;
	std::cout << e << std::endl;
	std::cout << "eigen vector" << std::endl;
	std::cout << v << std::endl;

	e.resize(5, 0.0);
	v.resize(size, 5, 0.0);
	sprsygvx(matk, matm, e, v, 0, 5);
	std::cout << "eigen value" << std::endl;
	std::cout << e << std::endl;
	std::cout << "eigen vector" << std::endl;
	std::cout << v << std::endl;

	std::cout << "Press any key to end." << std::endl;
	if(_getch()) return 0;
}
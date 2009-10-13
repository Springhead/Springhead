/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "FWVCSample.h"

FWLDHapticSample LDSample;

int _cdecl main(int argc, char* argv[]){
	LDSample.Init(argc, argv);
	LDSample.StartMainLoop();
	return 0;
}
/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/**
	- マルチウィンドウのデモ
	 - 1つのシーンを2つのウィンドウで表示する．
	 - 1つはデバッグ描画，もう1つは非デバッグ描画
 **/

#include "FWMultiWinSample.h"

FWMultiWinSample app;

#if _MSC_VER <= 1500
#include <GL/glut.h>
#endif
int __cdecl main(int argc, char* argv[]){	
	app.Init(argc, argv);	
	app.StartMainLoop();
	return 0;
}

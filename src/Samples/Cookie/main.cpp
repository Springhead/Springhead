/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/***********************************************************************************************
 *	<<Application>>	Cookie
 *	<<Author>>		T. Tokizaki (Programing)，H. Matsuyama (Modeling) 
 *	<<Date>>		Feb. 23, 2008
 *	<<Overview>>	Press 'm'				: Make a new cookie.
 *					Press '1'				: Zoom in  of the top view.
 *					Press '2'				: Zoom out of the top view.
 *					Press '3'				: Rotate the top view.
 *					Press 'Esc', 'q', 'Q'	: Quit the program.
 ***********************************************************************************************/

#include <Springhead.h>		//	Springheadのインタフェース
#include <GL/glut.h>
#include "MYApp.h"
#include "resource.h"
#include <ctime>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
MYApp app;
int main(int argc, char* argv[]){
	srand((unsigned) time(NULL));
	app.Init(argc, argv);
	Sleep(10);
	app.Start();
	return 0;
}

/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "FWFileLoaderSample.h"
/**
	シンプルなSprfileローダー
	シミュレーションはglutIdleFuncで進める
	q, ESC	: アプリケーション終了
	r		: ファイルのリロード
	w		: カメラ行列の初期化
	d		: デバック表示切り替え
*/

FWFileLoaderSample app;
int __cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

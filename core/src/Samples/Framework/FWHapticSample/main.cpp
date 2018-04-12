/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/*
	力覚インタフェースを使い、バーチャル世界と力覚インタラクションができるサンプル
	キー入力
	1:力覚レンダリングモードをペナルティ
	2:力覚レンダリングモードをプロキシに
	c:インタフェースのキャリブレーション
	f:力覚提示のON/OFF
	v:振動提示のON/OFF
	d:デバック表示
	a:バネ係数を100up
	z:バネ係数を100down
	space:剛体を生成する
*/

#include "FWHapticSample.h"

FWHapticSample app;

#if _MSC_VER <= 1600
#include <GL/glut.h>
#endif
int __cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/*
	シミュレーションの複数ウィンドウ表示
	シーンのGraphicモデルとPhysicモデルを2画面に表示
	q,ESC	: アプリケーション終了
	r		: ファイルの再ロード	
*/

#ifndef FWMULTIWIN_SAMPLE_H
#define FWMULTIWIN_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class FWMultiWinSample : public FWApp{
public:
	FWMultiWinSample();	
	virtual void Init(int argc, char* argv[]);
	virtual void TimerFunc(int id);
	virtual void Keyboard(int key, int x, int y);
};

#endif

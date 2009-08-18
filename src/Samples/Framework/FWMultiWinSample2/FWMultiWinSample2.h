/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWMULTIWIN_SAMPLE_H
#define FWMULTIWIN_SAMPLE_H

#include <Framework/SprFWApp.h>
#include <Springhead.h>
#include <vector>

using namespace Spr;


class FWMultiWinSample2 : public FWApp{
private:
	int				nWin;			//< 作るウィンドウの数
	vector<string>	fileNames;		//< 読み込むファイルの名前
	vector<string>	winNames;		//< Windowの名前
public:
	FWMultiWinSample2();	
	virtual void Init(int argc, char* argv[]);
	void InitCameraView();			// カメラ座標	
//	virtual void Reset();
	virtual void Timer();
	virtual void IdleFunc();
	virtual void CallStep(int i);
	static void _cdecl Step1(int id);
	static void _cdecl Step2(int id);
	static void _cdecl Step3(int id);
	virtual void Display();
	void Keyboard(int key, int x, int y);	
};

#endif
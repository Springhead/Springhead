/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef MYAPP_H
#define MYAPP_H

#include <Framework/SprFWAppGLUT.h>
#include <Springhead.h>
#include <cstdlib>
#include <vector>

using namespace Spr;
using namespace std;

class MYApp : public FWAppGLUT, public UTRefCount{
	
private:
	double					dt;				//< 積分幅
	double					nIter;			//< LCP反復回数
	bool					bTimer;

	int								numWindow;		//< 作るウィンドウの数
	vector< UTRef< FWWin > > 		windows;		//< 作ったウィンドウ
	vector< UTRef< FWSceneIf > >	fwScenes;		//< Physicsのシーングラフ
	vector< string >				winNames;		//< Windowの名前
	vector< string >				fileNames;		//< 読み込むファイルの名前

	vector< Affinef	>				views;			//< カメラ位置の回転・ズームを行う時の行列用の変数

public:
	
	// コンストラクタ
	MYApp();

	// このクラス内で新しく宣言する関数
	void NumOfClassMembers(std::ostream& out);

	// インタフェース関数
	double	GetTimeStep()					{return dt;}
	void	SetTimeStep(double data)		{dt = data;}
	double	GetNumIteration()				{return nIter;}
	void	SetNumIteration(double data)	{nIter = data;}
	
	// 基底のクラスで宣言された関数のうちオーバーロードする関数
	void Init(int argc, char* argv[]);
	void Keyboard(int key, int x, int y);
	void Display();
	void Step();
	
};

#endif
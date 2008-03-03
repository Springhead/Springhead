/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWAPPGLUI_H
#define SPR_FWAPPGLUI_H

#include <Framework/SprFWAppGLUT.h>

class GLUI;

namespace Spr{;

class FWAppGLUIDesc {
public:
	int		fromTop;					//< 別ウィンドウを生成するときの上からのdot数
	int		fromLeft;					//< 別ウィンドウを生成するときの左からのdot数
	int		subPosition;				//< OpenGL描画ウィンドウの中にGUIを組み込んでしまう場合の組み込む場所
	char*	gluiName;					//< 別ウィンドウを作成する場合のウィンドウの名前
	bool	createOtherWindow;			//< GUIを別ウィンドウにするかどうか
	
	//デフォルトコンストラクタ
	FWAppGLUIDesc();
};

/** @brief GLUIを用いるアプリケーションクラス
		   基本的にFWAppGLUIを自分のアプリケーションのクラスに継承させ，
		   DesignGUIを必ずオーバーライドして使用する．
		   必要に応じてDesignGUIから派生するコールバック関数をアプリケーションクラスに作成する必要がある．
		   自分のアプリケーションクラスに作成するコールバックは，呼び出し規約に注意して作成すること．
		   (クラス内の関数定義は必ず__thiscallになる)
*/
class FWAppGLUI : public FWAppGLUT, public FWAppGLUIDesc{
protected:
	std::vector<GLUI*> guis;
public:
	~FWAppGLUI();
	virtual void		 Init(int argc, char* argv[]);
	virtual void __cdecl DesignGUI() = 0;
	virtual void		 Start();						// FWAppGLUTのStart()とは中身が異なるので消さないこと．
	virtual GLUI*		 CreateGUI(int wid = 0, FWAppGLUIDesc desc = FWAppGLUIDesc());
};

}
#endif
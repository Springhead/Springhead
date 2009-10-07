#ifndef FWGLUI_H
#define FWGLUI_H
#include<sstream>
#include<string>
#include<vector>
#include<Springhead.h>
#include<Framework/FWGLUT.h>
#include"Framework/SprFWApp.h"
#include <GL/glui.h>

using namespace std;

namespace Spr{;


class FWGLUIDesc{
public:
	int		fromTop;					//< 別ウィンドウを生成するときの上からのdot数
	int		fromLeft;					//< 別ウィンドウを生成するときの左からのdot数
	int		subPosition;				//< OpenGL描画ウィンドウの中にGUIを組み込んでしまう場合の組み込む場所
	char*	gluiName;					//< 別ウィンドウを作成する場合のウィンドウの名前
	bool	createOtherWindow;			//< GUIを別ウィンドウにするかどうか
	
	//デフォルトコンストラクタ
	FWGLUIDesc();
};

/** @brief GLUIを用いるアプリケーションクラス
		   基本的にFWAppGLUIを自分のアプリケーションのクラスに継承させ，
		   DesignGUIを必ずオーバーライドして使用する.
		   CreateGUIでFWAppGLUIDescの情報を基にGLUI*型の変数を取得する．
		   GLUI*型の変数からツリー状にUIのパーツとコールバックを定義する（DesignGUIの実装）
		   glutTimerFuncをGLUI_Master.set_glutTimerFuncにしないと固まる．
		   あとはglutMainLoop();
		   GLUI*がNULLだとglutと同じ動きになるのでDesignGUIを呼ばないのならCreateGUIを呼ぶ必要も無い．
		   必要に応じてDesignGUIから派生するコールバック関数をアプリケーションクラスに作成する必要がある．
		   自分のアプリケーションクラスに作成するコールバックは，呼び出し規約に注意して作成すること．
		   (クラス内の関数定義は必ず__thiscallになる)   
	*/

class FWGLUI :public FWGLUT, public FWGLUIDesc{
protected:
	std::vector<GLUI*> guis;
public:
	~FWGLUI();
	virtual void SPR_CDECL DesignGUI(){};
	///GLUIによるTimerをスタートする
	virtual void StartMainLoop();			// FWAppGLUTのStartMainLoop()とは中身が異なるので消さないこと．
	virtual GLUI*		 CreateGUI(int wid = 0, FWGLUIDesc desc = FWGLUIDesc());
};

}

#endif
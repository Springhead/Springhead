/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
<<EmbPython用swigを自前のクラスにも使用するサンプル>>

SpringheadのIfクラスだけでなく，アプリケーションにおいて自前で用意したクラスも，
swigを用いてPythonからアクセス可能にすることができます．

0. モジュール名をきめる
- このサンプルでは MyEPApp です．
-- 含まれるクラス名とは必ずしも一致しなくて構いません．

1. 用意するファイル
- クラスヘッダ（モジュール名.h)
- .iファイル（モジュール名.i） %module や %ignore を適切に用意し， 最後に %include "モジュール名.h"
- %module:モジュール名
- %ignore:pythonからアクセスしなくて良いMyEPAppクラスのメソッド or コンパイルが通らないメソッドの除外(UTRefまわりで起きる）

2. 書き換えるファイル
- RunSwig.bat の先頭（MODULEやBINを適切にセットする）

3. ビルドの方法
- RunSwig.batを走らせる
- グルーコード，SprEPモジュール名.h, EPモジュール名.h，EPモジュール名.cppが生成される
- ビルドする

4. 実行前に
-SprEPモジュール名.hとEPモジュール名.hをインクルードする(おそらくmain.cpp)
-main関数内のinterpreter->Initialize();のあとで関数Initモジュール名();を呼ぶ
-この関数はSprEPモジュール名.hで宣言されている
-*Springhead関係はinterpreter->Initialize();で初期化されるが，
	自前のものは初期化されないのでここでやっておく

----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#include "MyEPApp.h"

#include "SprEPMyEPApp.h" 
#include "EPMyEPApp.h"

#pragma  hdrstop

using namespace Spr;
using namespace std;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// マルチスレッド用
#ifdef _WIN32
/// std::maxなどを使うのに必要(windows.hと競合するから)
#define NOMINMAX 
#include <windows.h>
#endif

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// WndProc書き換え用
LRESULT CALLBACK NewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC OldWndProc;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// アプリケーションクラス
void MyEPApp::Init(int argc, char* argv[]) {
	this->argc = argc;
	this->argv = argv;

	SampleApp::Init(argc, argv);
	ToggleAction(MENU_ALWAYS, ID_RUN);
	curScene = 0;

	hiSdk = HISdkIf::CreateSdk();

	// --- --- --- --- ---
	// SpaceNavigator

	HWND hWnd = FindWindow((LPCSTR) "GLUT", (LPCSTR) "Python with Springhead");
	HISpaceNavigatorDesc descSN;
	descSN.hWnd = &hWnd;

	spaceNavigator0 = hiSdk->CreateHumanInterface(HISpaceNavigatorIf::GetIfInfoStatic())->Cast();
	spaceNavigator0->Init(&descSN);
	spaceNavigator0->SetPose(Posef(Vec3f(0,0,5.0), Quaternionf()));

	spaceNavigator1 = hiSdk->CreateHumanInterface(HISpaceNavigatorIf::GetIfInfoStatic())->Cast();
	spaceNavigator1->Init(&descSN);
	spaceNavigator1->SetPose(Posef(Vec3f(0,0,-5.0), Quaternionf()));

	// ウィンドウプロシージャを置き換え
	OldWndProc = (WNDPROC)(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)(NewWndProc));

	GetFWScene()->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);

	GetCurrentWin()->GetTrackball()->SetPosition(Vec3f(6.5,6,20));
}

MyEPApp app;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// ループの中身(何回も呼ばれる
void EPLoop(void* arg) {
	//インタラクティブモードで起動
	PyRun_InteractiveLoop(stdin,"SpringheadPython Console");
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// Python <=> Springhead 変数接続用
#define ACCESS_SPR_FROM_PY(cls, name, obj)							\
	{																\
		PyObject* pyObj = (PyObject*)newEP##cls((obj));				\
		Py_INCREF(pyObj);											\
		PyDict_SetItemString(dict, #name, pyObj);					\
	}																\

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// ループ実行直前に１度だけ呼ばれる
void EPLoopInit(void* arg) {
	MyEPApp* app = (MyEPApp*)arg;

	////Pythonでモジュールの使用宣言
	PyRun_SimpleString("import Spr");
		
	// cのpyobjectをpythonで読めるようにする
	PyObject *m = PyImport_AddModule("__main__");
	PyObject *dict = PyModule_GetDict(m);

	ACCESS_SPR_FROM_PY(MyEPApp,				app,				app										);
	ACCESS_SPR_FROM_PY(FWSceneIf,			fwScene,			app->GetFWScene()						);
	ACCESS_SPR_FROM_PY(HITrackballIf,		hiTrackball,		app->GetCurrentWin()->GetTrackball()	);
	ACCESS_SPR_FROM_PY(HISpaceNavigatorIf,	spaceNavigator0,	app->spaceNavigator0					);
	ACCESS_SPR_FROM_PY(HISpaceNavigatorIf,	spaceNavigator1,	app->spaceNavigator1					);

	if (app->argc == 2) {
		ostringstream loadfile;
		loadfile << "__mainfilename__ ='";
		loadfile << app->argv[1];
		loadfile << "'";
		PyRun_SimpleString("import codecs");
		PyRun_SimpleString(loadfile.str().c_str());
		PyRun_SimpleString("__mainfile__ = codecs.open(__mainfilename__,'r','utf-8')");
		PyRun_SimpleString("exec(compile( __mainfile__.read() , __mainfilename__, 'exec'),globals(),locals())");
		PyRun_SimpleString("__mainfile__.close()");
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 置き換え用WndProc
LRESULT CALLBACK NewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	MSG m;
	m.hwnd		= hWnd;
	m.message	= msg;
	m.wParam	= wParam;
	m.lParam	= lParam;
	if (app.spaceNavigator0->PreviewMessage(&m)) {
		return 0L;
	}
	if (app.spaceNavigator1->PreviewMessage(&m)) {
		return 0L;
	}
	return CallWindowProc(OldWndProc, hWnd, msg, wParam, lParam);
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int __cdecl main(int argc, char *argv[]) {
	app.Init(argc, argv);

	EPInterpreter* interpreter = EPInterpreter::Create();

	interpreter->Initialize();
	initMyEPApp();

	interpreter->EPLoop = EPLoop;
	interpreter->EPLoopInit = EPLoopInit;
	interpreter->Run(&app);

	app.StartMainLoop();
	return 0;
}

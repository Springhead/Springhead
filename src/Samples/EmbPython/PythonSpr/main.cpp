/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "../../SampleApp.h"

#include <EmbPython/EmbPython.h>

#include <sstream>

#include "windows.h"

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
class PythonSprApp : public SampleApp{
public:
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_TOGGLE_RENDER_BACKSCENE,
		ID_CALIB_SPIDAR,
		ID_RESET,
	};

	HISpaceNavigatorIf*		spaceNavigator;

	int argc;
	char** argv;

	PythonSprApp(){
		appName = "Python with Springhead";
		numScenes   = 1;
	}
	~PythonSprApp(){}

	// 初期化
	virtual void Init(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;

		SampleApp::Init(argc, argv);
		ToggleAction(MENU_ALWAYS, ID_RUN);
		curScene = 0;

		HWND hWnd = FindWindow(L"GLUT", L"Python with Springhead");

		HISdkIf* hiSdk = HISdkIf::CreateSdk();

		// --- --- --- --- ---
		// SpaceNavigator

		HISpaceNavigatorDesc descSN;
		descSN.hWnd = &hWnd;
		spaceNavigator = hiSdk->CreateHumanInterface(HISpaceNavigatorIf::GetIfInfoStatic())->Cast();
		spaceNavigator->Init(&descSN);
		spaceNavigator->SetPose(Posef(Vec3f(0,0,5.0), Quaternionf()));

		// ウィンドウプロシージャを置き換え
		OldWndProc = (WNDPROC)(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)(NewWndProc));

		fwScene->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);
	}	

	virtual void OnStep(){
		UTAutoLock critical(EPCriticalSection);
		fwScene->Step();
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void OnDraw(GRRenderIf* render) {
		UTAutoLock critical(EPCriticalSection);
		SampleApp::OnDraw(render);
	}

	virtual void OnAction(int menu, int id){
		UTAutoLock critical(EPCriticalSection);
		SampleApp::OnAction(menu, id);
	}

} app;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// ループの中身(何回も呼ばれる
void EPLoop(void* arg) {
	//インタラクティブモードで起動
	PyRun_InteractiveLoop(stdin,"SpringheadPython Console");
}

PyObject* SprPy_PassFWSdkIf(FWSdkIf *ob);

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
	PythonSprApp* app = (PythonSprApp*)arg;

	////Pythonでモジュールの使用宣言
	PyRun_SimpleString("import Spr");
		
	// cのpyobjectをpythonで読めるようにする
	PyObject *m = PyImport_AddModule("__main__");
	PyObject *dict = PyModule_GetDict(m);

	ACCESS_SPR_FROM_PY(FWSceneIf,			fwScene,		app->fwScene		);
	ACCESS_SPR_FROM_PY(HITrackballIf,		hiTrackball,	app->GetCurrentWin()->GetTrackball() );
	ACCESS_SPR_FROM_PY(HISpaceNavigatorIf,	spaceNavigator,	app->spaceNavigator	);

	ostringstream loadfile;
	loadfile << "__mainfilename__ ='";
	if (app->argc == 2) {
		loadfile << app->argv[1];
	} else {
		loadfile << "main.py";
	}
	loadfile << "'";

	PyRun_SimpleString("import codecs");
	PyRun_SimpleString(loadfile.str().c_str());
	PyRun_SimpleString("__mainfile__ = codecs.open(__mainfilename__,'r','utf-8')");
	PyRun_SimpleString("exec(compile( __mainfile__.read() , __mainfilename__, 'exec'),globals(),locals())");
	PyRun_SimpleString("__mainfile__.close()");
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 置き換え用WndProc
LRESULT CALLBACK NewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	MSG m;
	m.hwnd		= hWnd;
	m.message	= msg;
	m.wParam	= wParam;
	m.lParam	= lParam;
	if (app.spaceNavigator->PreviewMessage(&m)) {
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
int main(int argc, char *argv[]) {
	app.Init(argc, argv);

	EPInterpreter* interpreter = EPInterpreter::Create();

	interpreter->Initialize();
	interpreter->EPLoop = EPLoop;
	interpreter->EPLoopInit = EPLoopInit;
	interpreter->Run(&app);

	app.StartMainLoop();
	return 0;
}

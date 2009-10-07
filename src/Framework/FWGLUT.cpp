#include <windows.h>
#include <GL/glew.h>
#include <Framework/FWGLUT.h>

namespace Spr{;
// FWWinGLUT /////////////////////////////////////////////////////////////////////
void FWWinGLUT::Position(int left, int top){
	glutPositionWindow(left, top); fullscreen = false;
}
void FWWinGLUT::Reshape(int width, int height){
	glutReshapeWindow(width, height); fullscreen = false;
	this->width = width; this->height = height;
}
void FWWinGLUT::SetTitle(UTString t){
	glutSetWindowTitle(t.c_str()); title = t;
}
void FWWinGLUT::FullScreen(){
	glutFullScreen(); fullscreen = true;
}
// FWGLUT /////////////////////////////////////////////////////////////////////
FWGLUTDesc::FWGLUTDesc(){
}

FWGLUT::FWGLUT(){
};

FWGLUT* FWGLUT::instance;

FWGLUT::~FWGLUT(){
	FWGLUT::AtExit();
	instance = NULL;
	
}
/** コールバック関数*///////////////////////////////////////////////////////
void FWGLUT::GlutDisplayFunc(){
	instance->fwApp->CallDisplay();	
}

void FWGLUT::GlutReshapeFunc(int w, int h){
	instance->fwApp->CallReshape(w, h);
}
void FWGLUT::GlutTimerFunc(int id){
	instance->fwApp->CallTimerFunc(id);
}
void FWGLUT::GlutIdleFunc(){
	instance->fwApp->CallIdleFunc();
}

void FWGLUT::GlutKeyboardFunc(unsigned char key, int x, int y){
	instance->fwApp->CallKeyboard((int)key, x, y);
}

void FWGLUT::GlutMouseFunc(int button, int state, int x, int y){
	instance->fwApp->CallMouseButton(button, state, x, y);
}
void FWGLUT::GlutMotionFunc(int x, int y){
	instance->fwApp->CallMouseMove(x, y);
}

void FWGLUT::GlutJoystickFunc(unsigned int buttonMask, int x, int y, int z){
	instance->fwApp->CallJoystick(buttonMask, x, y, z);
}

void FWGLUT::AtExit(){
	if(instance && instance->fwApp->vfBridge)
		instance->fwApp->vfBridge->AtExit();
}

/** Init *////////////////////////////////////////////////////////////////

void FWGLUT::Init(int argc, char* argv[]){
	instance = this;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	atexit(FWGLUT::AtExit);
}

/** タイマ *////////////////////////////////////////////////////////////////
void FWGLUT::StartMainLoop(){
	glutIdleFunc(FWGLUT::GlutIdleFunc);
	glutMainLoop();
}

/** ウィンドウ *////////////////////////////////////////////////////////////////

///	ウィンドウを作成し、ウィンドウ IDを返す
FWWin* FWGLUT::CreateWin(const FWWinDesc& d){
	int wid=0;
	if(d.fullscreen){	//< フルスクリーンの場合のウィンドウ生成
		std::stringstream gameMode;
		gameMode << d.width << "x" << d.height << ":32@60";
		glutGameModeString(gameMode.str().c_str());
#ifdef _MSC_VER
		Sleep(100);
#endif
		wid	= glutEnterGameMode();
	}else{				//< ウィンドウモードの場合の生成
		if (d.parentWindow){
			wid = glutCreateSubWindow(d.parentWindow, d.left, d.top, d.width, d.height);
		}else{
			glutInitWindowSize(d.width, d.height);
			glutInitWindowPosition(d.left, d.top);
			wid = glutCreateWindow(d.title.c_str());
		}
	}
	//	このWindowのglewコンテキストの初期化
	int rv = glewInit();
	//	windowに関連するコールバックの設定
	glutDisplayFunc(FWGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWGLUT::GlutReshapeFunc);
	glutKeyboardFunc(FWGLUT::GlutKeyboardFunc);
	glutMouseFunc(FWGLUT::GlutMouseFunc);
	glutMotionFunc(FWGLUT::GlutMotionFunc);
	int pollInterval = 10;	// int pollInterval : glutJoystickFuncを使うときに使う何か．読み込み時間に関係しているらしい．
	glutJoystickFunc(FWGLUT::GlutJoystickFunc, pollInterval);
	
	FWWin* win = DBG_NEW FWWinGLUT(wid, d, fwApp->GetSdk()->CreateRender());
	win->SetID(glutGetWindow());	//現在のwindowのIDを設定
	fwApp->AssignScene(win);
	fwApp->wins.push_back(win);
	return win;
}
///	ウィンドウを破棄する
void FWGLUT::DestroyWin(FWWin* w){
	if (w->fullscreen){
		glutLeaveGameMode();
	}else{
		glutDestroyWindow(w->GetID());
	}
	Wins::iterator it = std::find(fwApp->wins.begin(), fwApp->wins.end(), w);
	assert(it != fwApp->wins.end());
	fwApp->wins.erase(it);
};
///	カレントウィンドウを設定する
void FWGLUT::SetCurrentWin(FWWin* w){
	glutSetWindow(w->GetID());
};
///	カレントウィンドウを返す。
FWWin* FWGLUT::GetCurrentWin(){
	return fwApp->GetWinFromId(glutGetWindow());
}
///	カレントウィンドウのIDを返す。
int FWGLUT::GetWinFromId(){
	return glutGetWindow();
}
///カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
void FWGLUT::PostRedisplay(){
	return glutPostRedisplay();
};
/// Shift,Ctrl,Altのステートを返す
int FWGLUT::Modifiers(){
	return glutGetModifiers();
};

}
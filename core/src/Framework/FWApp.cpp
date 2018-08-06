/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
//#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWSdk.h>
#include <Framework/SprFWScene.h>
#include <HumanInterface/SprHISdk.h>
#include <Graphics/SprGRSdk.h>

#include <Framework/FWGLUT.h>
#include <Framework/FWGLUI.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#ifdef _WIN32
# include <windows.h>
#endif

#ifdef	__linux__
  #define DWORD unsigned long
  #define WINAPI
#endif

namespace Spr{;

FWAppBase::FWAppBase() {
}

FWAppBase::~FWAppBase() {
	//	タイマーを止める
	for (size_t i = 0; i<timers.size(); ++i) timers[i]->Stop();
}

void FWAppBase::Init() {
	// 最も基本的な初期化処理
	// SDK初期化
	CreateSdk();
	// シーンを作成
	GetSdk()->CreateScene();
	// タイマを作成
	CreateTimer();
}

void FWAppBase::TimerFunc(int id) {
	UserFunc();
	GetSdk()->GetScene()->Step();
}

void FWAppBase::CreateSdk() {
	if (fwSdk)
		return;
	fwSdk = FWSdkIf::CreateSdk();
}

/// UTTimerに登録するコールバック関数
void SPR_CDECL FWAppBase_TimerCallback(int id, void* arg) {
	FWAppBase* app = (FWAppBase*)arg;
	if (!app)
		return;
	app->TimerFunc(id);
}

UTTimerIf*  FWAppBase::CreateTimer(UTTimerIf::Mode mode) {
	/// インスタンスはコンストラクタの中でUTTimerStubに格納される
	UTTimerIf* timer = UTTimerIf::Create();
	timer->SetMode(mode);
	timer->SetCallback(FWAppBase_TimerCallback, this);
	timers.push_back(timer);
	return timer;
}

UTTimerIf* FWAppBase::GetTimer(int i) {
	if ((int)timers.size() > 0) return timers[i];
	return NULL;
}

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

FWApp* FWApp::instance = 0;
UTRef<FWGraphicsHandler> FWGraphicsHandler::instance = 0;

FWApp::FWApp(){
	bThread = false;
	bPostRedisplay = false;
	instance = this;
}

FWApp::~FWApp(){
	//	フルスクリーンモードだったら戻す
	bool hasFullScreen = false;
	for(int i = 0; i < (int)wins.size(); i++){
		if(wins[i]->GetFullScreen()){
			hasFullScreen = true;
			break;
		}
	}
	if (hasFullScreen) {
		if (FWGraphicsHandler::instance)
			FWGraphicsHandler::instance->LeaveGameMode();
	}
}

void FWApp::Init(){ Init(0); }
void FWApp::Init(int argc, char* argv[]){
	// 最も基本的な初期化処理
	// SDK初期化
	CreateSdk();
	// シーンを作成
	GetSdk()->CreateScene();
	// ウィンドウマネジャ初期化
	GRInit(argc, argv);
	// ウィンドウを作成
	FWWinDesc wd;
	wd.title = argc ? argv[0] : "FWApp";
	CreateWin();
	// タイマを作成
	CreateTimer();
}

//---------------------------------------------------------
//	Create new thead, init and start the framework.
//	thread function
class FWAppThreadCall {
public:
	static DWORD WINAPI MainLoop(void* arg) {
		FWApp* app = (FWApp*)arg;
		app->StartInThread();
		return 0;
	}
	static void SPR_CDECL RedisplayCheck(int id, void* arg) {
		FWApp* app = (FWApp*)arg;
		app->CheckAndPostRedisplay();
	}
};
void FWApp::InitInNewThread() {
	unsigned long thread;
#ifdef _WIN32
	CreateThread(NULL, 0, FWAppThreadCall::MainLoop, this, 0, &thread);
#else
#warning Please implement thread creation for other platform 
#endif
}

void FWApp::CheckAndPostRedisplay() {
	if (bThread && bPostRedisplay) {
		bPostRedisplay = false;
		FWGraphicsHandler::instance->PostRedisplay();
	}
}
void FWApp::StartInThread() {
	bThread = true;
	// SDK初期化
	CreateSdk();
	// シーンを作成
	GetSdk()->CreateScene();
	// ウィンドウマネジャ初期化
	GRInit(0, NULL);
	EnableIdleFunc(false);
	// ウィンドウを作成
	FWWinDesc wd;
	wd.title = "FWApp::StartInThread";
	CreateWin();

	UTTimerIf* grTimer = CreateTimer(UTTimerIf::FRAMEWORK);
	grTimer->SetCallback(FWAppThreadCall::RedisplayCheck, this);
	grTimer->SetInterval(20);
	grTimer->Start();
	FWGraphicsHandler::instance->StartMainLoop();
}

void FWApp::Display(){
	GetCurrentWin()->Display();
}

void FWApp::TimerFunc(int id){
	UserFunc();
	GetCurrentWin()->GetScene()->Step();
	PostRedisplay();
}

void FWApp::EnableIdleFunc(bool on){
	FWGraphicsHandler::instance->EnableIdleFunc(on);
}
void FWApp::StartMainLoop() {
	FWGraphicsHandler::instance->StartMainLoop();
}


// 派生クラスで定義することのできる仮想関数/////////////////////////////////
void FWApp::Reshape(int w, int h){
}

void FWApp::MouseButton(int button, int state, int x, int y){
	// ctrl+left カーソルで剛体を動かす
	/*if(fwSdk->GetScene() && (mouseInfo.left && mouseInfo.ctrl)){
		dragInfo.Init(fwSdk->GetScene()->GetPHScene(), cameraInfo.view, fwSdk->GetRender());
		dragInfo.Grab(x, y);
	}

	if(state == BUTTON_UP && button == LEFT_BUTTON)
		dragInfo.Release();*/
}

void FWApp::MouseMove(int x, int y){
	// 視点移動(回転)
	/*if(mouseInfo.left && !mouseInfo.ctrl && !mouseInfo.alt){
		cameraInfo.Rotate(mouseInfo.pos.x - mouseInfo.lastPos.x, mouseInfo.pos.y - mouseInfo.lastPos.y,
			GetSdk()->GetRender()->GetPixelSize());
	}

	// 視点移動(平行移動)
	if(mouseInfo.middle || mouseInfo.left && mouseInfo.alt){
		cameraInfo.Translate(mouseInfo.pos.x - mouseInfo.lastPos.x, mouseInfo.pos.y - mouseInfo.lastPos.y,
			GetSdk()->GetRender()->GetPixelSize());
	}
	
	// ズーム
	if(mouseInfo.right){
		cameraInfo.Zoom(mouseInfo.pos.y - mouseInfo.lastPos.y);
	}

	// 剛体ドラッグ
	if(mouseInfo.left && mouseInfo.ctrl)
		dragInfo.Drag(x, y);
	*/
}

//　FWAppのインタフェース ///////////////////////////////////////////////////////

void FWApp::AssignScene(FWWinIf* win){
	if (win->GetScene()) return;
	
	// 既存のどのウィンドウにも割り当てられていないシーンを探す
	for(int i = GetSdk()->NScene() - 1; i >= 0; --i){
		FWSceneIf* scene = GetSdk()->GetScene(i);
		Wins::iterator it;
		for(it = wins.begin(); it != wins.end(); ++it){
			if ((*it)->GetScene() == scene) break;
		}
		if (it == wins.end()){
			win->SetScene(scene);
			return;
		}
	}
}

FWWinIf* FWApp::CreateWin(const FWWinDesc& desc, FWWinIf* parent){
	FWWinIf* win = FWGraphicsHandler::instance->CreateWin(desc, parent);

	// 自身をキーボード・マウスコールバックに登録
	win->GetKeyMouse()->AddCallback(this);

	HISdkIf* hiSdk = GetSdk()->GetHISdk();
	// トラックボールとドラッガーを作成
	if(desc.useKeyMouse && desc.useTrackball)
		win->SetTrackball(hiSdk->CreateHumanInterface(HITrackballIf::GetIfInfoStatic())->Cast());
	//if(desc.useKeyMouse && desc.useDragger)
	//	win->SetDragger(hiSdk->CreateHumanInterface(HIDraggerIf::GetIfInfoStatic())->Cast());

	wins.push_back(win);

	// シーンの割当て
	AssignScene(win);
	// レンダラ割当て
	GRRenderIf* render = GetSdk()->GetGRSdk()->CreateRender();
	render->SetDevice(FWGraphicsHandler::instance->GetGRDevice());
	// このコンテキストに対してGRDeviceGL::Initを呼ぶ
	render->GetDevice()->Init();
	win->SetRender(render);

	return win;
}

FWWinIf* FWApp::GetWinFromId(int wid){
	for(Wins::iterator i = wins.begin(); i != wins.end(); i++){
		if((*i)->GetID() == wid)
			return *i;
	}
	return NULL;
}

FWWinIf* FWApp::GetWin(int pos){
	if(0 <= pos && pos < NWin())
		return wins[pos];
	return NULL;
}

FWWinIf* FWApp::GetCurrentWin(){
	if(FWGraphicsHandler::instance)
		return GetWinFromId(FWGraphicsHandler::instance->GetCurrentWin());
	DSTR << "Error in FWApp::GetCurrentWin(): FWGraphicsHandler::instance does not exist." << std::endl;
	return NULL;
}

void FWApp::DestroyWin(FWWinIf* win){
	FWGraphicsHandler::instance->DestroyWin(win);
}

void FWApp::SetCurrentWin(FWWinIf* win){
	FWGraphicsHandler::instance->SetCurrentWin(win);
}

void FWApp::PostRedisplay(){
	if (bThread){
		bPostRedisplay = true;
	}
	else {
		FWGraphicsHandler::instance->PostRedisplay();
	}
}

int FWApp::GetModifier(){
	return FWGraphicsHandler::instance->GetModifiers();
}

// 描画パート////////////////////////////////////////////////////////////////////

void FWApp::SetGRHandler(int type){
	switch (type) {
	case TypeNone:
		FWGraphicsHandler::instance = 0;
		break;
	case TypeGLUT:
		FWGraphicsHandler::instance = DBG_NEW FWGLUT();
		break;
	case TypeGLUI:
		FWGraphicsHandler::instance = DBG_NEW FWGLUI();
		break;
	}
}

GRDeviceIf* FWApp::GRInit() { return GRInit(0); }
GRDeviceIf* FWApp::GRInit(int argc, char* argv[], int type){
	if(!FWGraphicsHandler::instance)
		SetGRHandler(type);
	FWGraphicsHandler::instance->Init(argc, argv);
	return FWGraphicsHandler::instance->GetGRDevice();
}

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// FWHapticApp 
// 力覚提示を伴う組込みアプリ

FWHapticApp::FWHapticApp() {
}

FWHapticApp::~FWHapticApp() {
}

void FWHapticApp::CreateTimers() {
	UTTimerIf* physicsTimer = CreateTimer(UTTimerIf::THREAD);
	physicsTimerID = physicsTimer->GetID();

	UTTimerIf* hapticTimer = CreateTimer(UTTimerIf::THREAD);
	hapticTimerID = hapticTimer->GetID();
}

void FWHapticApp::TimerFunc(int id) {
	if (hapticTimerID == id) {
		GetSdk()->GetScene(0)->UpdateHapticPointers();
		GetSdk()->GetScene(0)->GetPHScene()->StepHapticLoop();
		GetSdk()->GetScene(0)->GetPHScene()->StepHapticSync();
	} else {
		// GetSdk()->GetScene(0)->GetPHScene()->GetHapticEngine()->StepPhysicsSimulation();
	}
}

void FWHapticApp::StartTimers() {
	GetTimer(0)->Start();
	GetTimer(1)->Start();
}

void FWHapticApp::SetPHScene(PHSceneIf* phScene) {
	GetSdk()->GetScene(0)->SetPHScene(phScene);
}

}



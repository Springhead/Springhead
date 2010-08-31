/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/SprFWApp.h>

#include "FWScene.h"
#include "FWSdk.h"
#include "FWOldSpringheadNode.h"
#include <Physics/PHSdk.h>
#include <Graphics/GRSdk.h>
#include <Framework/FWGLUT.h>
#include <Framework/FWGLUI.h>
#include <Framework/FWInteractScene.h>
#include <Framework/SprFWGraphicsAdaptee.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWApp* FWApp::instance;

FWApp::FWApp(){
	instance = this;
}

FWApp::~FWApp(){
	ReleaseAllTimer();
	int s = (int)wins.size();
	bool hasGameMode = false;
	for(int i = 0; i < s; i++){
		if(wins[i]->fullscreen == true){
			hasGameMode = true; break;
		}
	}
	if(hasGameMode) glutLeaveGameMode();
}

void FWApp::EnableIdleFunc(bool on){
	if(grAdaptee)
		grAdaptee->EnableIdleFunc(on);
}
void FWApp::StartMainLoop(){
	if(grAdaptee)
		grAdaptee->StartMainLoop();
}


// 派生クラスで定義することのできる仮想関数/////////////////////////////////
void FWApp::Reshape(int w, int h){
	if(GetCurrentWin())
		fwSdk->SwitchRender(GetCurrentWin()->GetRender());
	fwSdk->Reshape(w, h);
}

void FWApp::MouseButton(int button, int state, int x, int y){
	// ctrl+left カーソルで剛体を動かす
	if(fwSdk->GetScene() && (mouseInfo.left && mouseInfo.ctrl)){
		dragInfo.Init(fwSdk->GetScene()->GetPHScene(), cameraInfo.view, fwSdk->GetRender());
		dragInfo.Grab(x, y);
	}

	if(state == BUTTON_UP && button == LEFT_BUTTON)
		dragInfo.Release();
}

void FWApp::MouseMove(int x, int y){
	// 視点移動(回転)
	if(mouseInfo.left && !mouseInfo.ctrl && !mouseInfo.alt){
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

}

//　FWAppのインタフェース ///////////////////////////////////////////////////////

void FWApp::CreateSdk(){
	fwSdk = FWSdkIf::CreateSdk();
}

void FWApp::AssignScene(FWWin* win){
	if (win->GetScene()) return;
	for(int i = GetSdk()->NScene() - 1; i >= 0; --i){
		Wins::iterator it;
		for(it = wins.begin(); it != wins.end(); ++it){
			if ((*it)->GetScene() == GetSdk()->GetScene(i)) break;
		}
		if (it == wins.end()){	//	対応するwindowがないscene
			win->scene = GetSdk()->GetScene(i);
			return;
		}
	}
}

FWWin* FWApp::CreateWin(const FWWinDesc& desc){
	if(!grAdaptee)
		return NULL;
	FWWin* win = grAdaptee->CreateWin(desc);
	wins.push_back(win);
	return win;
}

void FWApp::InitWindow(){
	if(!NWin() && grAdaptee){
		CreateWin();
		wins.back()->SetScene(GetSdk()->GetScene());
	}
}

FWWin* FWApp::GetWinFromId(int wid){
	for(Wins::iterator i = wins.begin(); i != wins.end(); i++){
		if((*i)->GetID() == wid)
			return *i;
	}
	return NULL;
}

FWWin* FWApp::GetWin(int pos){
	if(0 <= pos && pos < NWin())
		return wins[pos];
	return NULL;
}

FWWin* FWApp::GetCurrentWin(){
	if(!grAdaptee)
		return NULL;
	return grAdaptee->GetCurrentWin();
}

void FWApp::DestroyWin(FWWin* win){
	if(grAdaptee)
		grAdaptee->DestroyWin(win);
}

void FWApp::SetCurrentWin(FWWin* win){
	if(grAdaptee)
		grAdaptee->SetCurrentWin(win);
}

void FWApp::PostRedisplay(){
	if(grAdaptee)
		grAdaptee->PostRedisplay();
}

int FWApp::GetModifier(){
	if(!grAdaptee)
		return 0;
	return grAdaptee->GetModifiers();
}

void FWApp::Reset(){
	//dragger.clear();	//剛体ドラッグ情報を初期化
}

void FWApp::Clear(){
	Reset();
	//Timerの初期化
	for(int i = 0; i < (int)fwTimers.size(); i++){
		fwTimers[i]->Clear();
	}
	fwTimers.clear();
}

// 描画パート////////////////////////////////////////////////////////////////////

void FWApp::SetGRAdaptee(grAdapteeType type){
	switch (type) {
		case TypeNone:
			grAdaptee = NULL;
			break;
		case TypeGLUT:
			grAdaptee= new FWGLUT;
			break;
		case TypeGLUI:
			grAdaptee= new FWGLUI;
			break;
	}
	if(grAdaptee)
		grAdaptee->SetAdapter(this);
	//instance = this;
}

void FWApp::GRInit(int argc, char* argv[]){
	if(grAdaptee)
		grAdaptee->Init(argc, argv);
}

/**コールバック関数*/
void FWApp::CallDisplay(){
	if(!vfBridge || !vfBridge->Display())
		Display();
}
void FWApp::CallReshape(int w, int h){
	if(!vfBridge || !vfBridge->Reshape(w, h))
		Reshape(w, h);
}
void FWApp::CallTimerFunc(int id){
	if(!vfBridge || !vfBridge->Idle())
		TimerFunc(id);
}
void FWApp::CallIdleFunc(){
	if(!vfBridge || !vfBridge->Idle())
		IdleFunc();
}
void FWApp::CallKeyboard(int key, int x, int y){
	for(int i = 0; i < NIAScenes(); i++){
		FWInteractScene* iaScene = GetIAScene(i)->Cast();
		iaScene->BeginKeyboard();
	}

	if(!vfBridge || !vfBridge->Keyboard(key, x, y))
		Keyboard(key, x, y);

	for(int i = 0; i < NIAScenes(); i++){
		FWInteractScene* iaScene = GetIAScene(i)->Cast();
		iaScene->EndKeyboard();
	}
}
void FWApp::CallMouseButton(int button, int state, int x, int y){
	mouseInfo.pos.x = mouseInfo.lastPos.x = x;
	mouseInfo.pos.y = mouseInfo.lastPos.y = y;
	if(button == LEFT_BUTTON)
		mouseInfo.left = (state == BUTTON_DOWN);
	if(button == MIDDLE_BUTTON)
		mouseInfo.middle = (state == BUTTON_DOWN);
	if(button == RIGHT_BUTTON)
		mouseInfo.right = (state == BUTTON_DOWN);
	if(state == BUTTON_DOWN)
		mouseInfo.first = true;
	int mod = GetModifier();
	mouseInfo.shift = (mod & ACTIVE_SHIFT) != 0;
	mouseInfo.ctrl  = (mod & ACTIVE_CTRL) != 0;
	mouseInfo.alt   = (mod & ACTIVE_ALT) != 0;

	if(!vfBridge || !vfBridge->MouseButton(button, state, x, y))
		MouseButton(button, state, x, y);
}
void FWApp::CallMouseMove(int x, int y){
	mouseInfo.lastPos.x = mouseInfo.pos.x;
	mouseInfo.lastPos.y = mouseInfo.pos.y;
	mouseInfo.pos.x = x;
	mouseInfo.pos.y = y;

	if(mouseInfo.first){
		mouseInfo.first = false;
		return;
	}

	if(!vfBridge || !vfBridge->MouseMove(x, y))
		MouseMove(x, y);
}

void FWApp::CallJoystick(unsigned int buttonMask, int x, int y, int z){
	if(!vfBridge || !vfBridge->Joystick(buttonMask, x, y, z))
		Joystick(buttonMask, x, y, z);
}

/** FWInteraction */
 ////////////////////////////////////////////////////////////////
void FWApp::CreateHISdk(){
	hiSdk = HISdkIf::CreateSdk();
}

HISdkIf* FWApp::GetHISdk(){
	return hiSdk;
}

void FWApp::AddHI(HIBaseIf* hi){
	humanInterfaces.push_back(hi);
}

HIBaseIf* FWApp::GetHI(int i){
	if((unsigned)i <humanInterfaces.size()) return humanInterfaces[i];
	else						  return NULL;
}

FWInteractSceneIf* FWApp::CreateIAScene(const FWInteractSceneDesc &desc){
	FWInteractScene* iaScene = DBG_NEW FWInteractScene(desc);
	iaScenes.push_back(iaScene->Cast());
	iaScene->CreateIAAdaptee(desc.iaMode);
	if(desc.iaMode == LOCAL_DYNAMICS_3D || desc.iaMode == LOCAL_DYNAMICS_6D){
		iaScene->SetHMode(desc.hMode);
	}
	curIAScene = iaScene->Cast();
	return curIAScene;
}
FWInteractSceneIf* FWApp::GetIAScene(int i){
	if(i == -1) return curIAScene;
	if(0 <= i && i < NIAScenes()) return iaScenes[i];
	return NULL;
}
int FWApp::NIAScenes(){ return (int)iaScenes.size(); }
void FWApp::ClearIAScenes(){ 
	iaScenes.clear();
	curIAScene = NULL;
}

//タイマ///////////////////////////////////////////////////////////////////////////

int FWApp::CreateTimer(FWTimer::TimerType t){
	fwTimers.push_back(DBG_NEW FWTimer((int)fwTimers.size(),t));
	fwTimers.back()->Create();
	return fwTimers.back()->GetTimerId();
}
void FWApp::SetInterval(int id, unsigned ms){
	fwTimers[id]->SetInterval(ms);
}
unsigned FWApp::GetInterval(int id){
	return fwTimers[id]->GetInterval();
}
void FWApp::SetResolution(int id, unsigned r){
	fwTimers[id]->SetResolution(r);
}
unsigned FWApp::GetResolution(int id){
	return fwTimers[id]->GetResolution();
}

void FWApp::ReleaseTimer(int id){
	fwTimers[id]->Release();
}
void FWApp::RecreateTimer(int id){
	fwTimers[id]->Recreate();
}
void FWApp::ReleaseAllTimer(){
	for(unsigned int i=0; i < fwTimers.size() ; i++){
		fwTimers[i]->Release();
	}
}
void FWApp::CreateAllTimer(){
	for(unsigned int i=0; i < fwTimers.size() ; i++){
		fwTimers[i]->Recreate();
	}
}
void FWApp::ClearAllTimer(){
	fwTimers.clear();
}

FWTimer* FWApp::GetTimer(int id){
	return (id < (int) fwTimers.size() ? fwTimers[id] : NULL);
}

}

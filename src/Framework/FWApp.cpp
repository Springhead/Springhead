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

FWApp::~FWApp(){
	ReleaseAllTimer();
	glutLeaveGameMode();
}

// 派生クラスで定義することのできる仮想関数/////////////////////////////////
void FWApp::StartMainLoop(){
	grAdaptee->StartMainLoop();
}

void FWApp::Reshape(int w, int h){
	if(!GetCurrentWin())return;
	fwSdk->SwitchRender(GetCurrentWin()->GetRender());
	fwSdk->Reshape(w, h);
}

void FWApp::BeginKeyboard(){
	for(int i = 0; i < NIAScenes(); i++){
		FWInteractScene* iaScene = GetIAScene(i)->Cast();
		iaScene->BeginKeyboard();
	}
}
void FWApp::EndKeyboard(){
	for(int i = 0; i < NIAScenes(); i++){
		FWInteractScene* iaScene = GetIAScene(i)->Cast();
		iaScene->EndKeyboard();
	}
}

void FWApp::MouseButton(int button, int state, int x, int y){
	mouseInfo.lastPos.x = x, mouseInfo.lastPos.y = y;
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

	// ctrl+left カーソルで剛体を動かす
	// middle    カメラ平行移動
	if(fwSdk->GetScene() && ((mouseInfo.left && mouseInfo.ctrl) || mouseInfo.middle)){
		// カーソル座標をシーン座標に変換
		const GRCameraDesc& cam = fwSdk->GetRender()->GetCamera();
		Vec2f vpSize = fwSdk->GetRender()->GetViewportSize();
		Vec3f cursorPos(
			cam.center.x + ((float)x - vpSize.x / 2.0f) * (cam.size.x / vpSize.x),
			cam.center.y + (vpSize.y / 2.0f - (float)y) * (cam.size.y / vpSize.y),
			-cam.front);
		Vec3f ori, dir;
		ori = cameraInfo.view.Pos();
		dir = cameraInfo.view.Rot() * cursorPos;
		// raycast
		// 現在のシーンにPHRayが無ければ作成
		PHSceneIf* phScene = fwSdk->GetScene()->GetPHScene();
		DragInfo& info = dragInfo[fwSdk->GetScene()];
		if(!info.ray){
			info.ray = phScene->CreateRay();
			info.cursor = phScene->CreateSolid();
			info.cursor->SetDynamical(false);
			phScene->SetContactMode(info.cursor, PHSceneDesc::MODE_NONE);
			CDBoxDesc bd;
			info.cursor->AddShape(fwSdk->GetPHSdk()->CreateShape(bd));
		}
		info.ray->SetOrigin(ori);
		info.ray->SetDirection(dir);
		info.ray->Apply();
		if(info.ray->NHits()){
			PHRaycastHit* hit = info.ray->GetNearest();
			// カーソル剛体をヒット位置に移動
			info.cursor->SetCenterPosition(hit->point);
			// ヒット位置のカメラから見た距離（Z座標）を記憶
			Vec3f pointCamera = cameraInfo.view.inv() * hit->point;
			info.depth = pointCamera.z;
			// ヒットした剛体とカーソル剛体をつなぐバネ
			if(mouseInfo.left && mouseInfo.ctrl){
				PHSpringDesc desc;
				Posed pose;
				pose.Pos() = hit->point;
				desc.poseSocket = hit->solid->GetPose().Inv() * pose;
				info.spring = DCAST(PHSpringIf, phScene->CreateJoint(hit->solid, info.cursor, desc));
				const double K = 100.0, D = 10.0;
				info.spring->SetSpring(Vec3d(K, K, K));
				info.spring->SetDamper(Vec3d(D, D, D));
			}
		}
	}

	if(state == BUTTON_UP){
		if(button == LEFT_BUTTON){
			// ドラッグバネの削除
			DragInfo& info = dragInfo[fwSdk->GetScene()];
			if(info.spring){
				fwSdk->GetScene()->GetPHScene()->DelChildObject(info.spring);
				info.spring = NULL;
			}
		}
	}
}

void FWApp::MouseMove(int x, int y){
	int xrel = x - mouseInfo.lastPos.x, yrel = y - mouseInfo.lastPos.y;
	mouseInfo.lastPos.x = x;
	mouseInfo.lastPos.y = y;
	if(mouseInfo.first){
		mouseInfo.first = false;
		return;
	}
	bool cameraPosChange = false;
	// 左ボタン
	if(mouseInfo.left){
		if(mouseInfo.ctrl){
			DragInfo& info = dragInfo[fwSdk->GetScene()];
			if(!info.spring)
				return;
			// カーソル位置の剛体を動かす
			const GRCameraDesc& cam = fwSdk->GetRender()->GetCamera();
			Vec2f vpSize = fwSdk->GetRender()->GetViewportSize();
			float ratio = info.depth / (-cam.front);
			Vec3f rel(
				 (float)xrel * (cam.size.x / vpSize.x) * ratio,
				-(float)yrel * (cam.size.y / vpSize.y) * ratio,
				 0.0f);
			rel = cameraInfo.view.Rot() * rel;
			info.cursor->SetCenterPosition(info.cursor->GetCenterPosition() + rel);
		}
		else{
			// 視点移動(回転)
			cameraInfo.rot.y += (float)xrel * 0.01f;
			cameraInfo.rot.y =
				Spr::max(cameraInfo.rotRangeY[0], Spr::min(cameraInfo.rot.y, cameraInfo.rotRangeY[1]));
			cameraInfo.rot.x += (float)yrel * 0.01f;
			cameraInfo.rot.x =
				Spr::max(cameraInfo.rotRangeX[0], Spr::min(cameraInfo.rot.x, cameraInfo.rotRangeX[1]));
			cameraPosChange = true;
		}
	}
	// 中ボタン
	if(mouseInfo.middle){
		// 視点移動(平行移動)
		DragInfo& info = dragInfo[fwSdk->GetScene()];
		const GRCameraDesc& cam = fwSdk->GetRender()->GetCamera();
		Vec2f vpSize = fwSdk->GetRender()->GetViewportSize();
		float ratio = info.depth / (-cam.front);
		Vec3f rel(
			-(float)xrel * (cam.size.x / vpSize.x) * ratio,
			 (float)yrel * (cam.size.y / vpSize.y) * ratio,
			 0.0f);
		cameraInfo.target += cameraInfo.view.Rot() * rel;
		cameraPosChange = true;
	}
	// 右ボタン
	if(mouseInfo.right){
		// ズーム
		cameraInfo.zoom *= (float)exp((double)yrel/10.0);
		cameraInfo.zoom = Spr::max(cameraInfo.zoomRange[0], Spr::min(cameraInfo.zoom, cameraInfo.zoomRange[1]));
		cameraPosChange = true;
	}
	if(cameraPosChange) cameraInfo.UpdateView();
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
	FWWin* win = grAdaptee->CreateWin(desc);
	return win;
}

void FWApp::InitWindow(){
	if (!NWin()){
		grAdaptee->CreateWin(FWWinDesc());
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
	return grAdaptee->GetCurrentWin();
}

void FWApp::DestroyWin(FWWin* win){
	grAdaptee->DestroyWin(win);
}

void FWApp::SetCurrentWin(FWWin* win){
	grAdaptee->SetCurrentWin(win);

}

void FWApp::PostRedisplay(){
	grAdaptee->PostRedisplay();
}

int FWApp::GetModifier(){
	return grAdaptee->Modifiers();
}

// 描画パート////////////////////////////////////////////////////////////////////
FWApp* FWApp::instance;

void FWApp::SetGRAdaptee(grAdapteeType type){
	switch (type) {
		case TypeGLUT:
			grAdaptee= new FWGLUT;
			break;
		case TypeGLUI:
			grAdaptee= new FWGLUI;
			break;
	}
	grAdaptee->SetAdapter(this);
	instance = this;
}

void FWApp::GRInit(int argc, char* argv[]){
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
	if(!vfBridge || !vfBridge->Keyboard(key, x, y))
		Keyboard(key, x, y);
}
void FWApp::CallMouseButton(int button, int state, int x, int y){
	if(!vfBridge || !vfBridge->MouseButton(button, state, x, y))
		MouseButton(button, state, x, y);
}
void FWApp::CallMouseMove(int x, int y){
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
	return humanInterfaces[i];
}

FWInteractSceneIf* FWApp::CreateIAScene(const FWInteractSceneDesc &desc){
	FWInteractScene* iaScene = DBG_NEW FWInteractScene(desc);
	iaScenes.push_back(iaScene->Cast());
	iaScene->CreateIAAdaptee(desc.iaMode);
	if(desc.iaMode == LOCAL_DYNAMICS){
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

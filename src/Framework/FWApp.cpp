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

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWApp::~FWApp(){
}

void FWApp::Init(int argc, char* argv[]){
	ProcessArguments(argc, argv);
	fwSdk = FWSdkIf::CreateSdk();
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

void FWApp::Step(){
	for(int i=0; i<GetSdk()->NScene(); ++i){
		GetSdk()->GetScene(i)->Step();
	}
	//if(!GetCurrentWin())return;
	//fwSdk->SwitchScene(GetCurrentWin()->GetScene());
	//fwSdk->Step();
}

void FWApp::Display(){
	if(!GetCurrentWin())return;
	fwSdk->SwitchScene(GetCurrentWin()->GetScene());
	fwSdk->SwitchRender(GetCurrentWin()->GetRender());
	fwSdk->Draw();
}

void FWApp::Reshape(int w, int h){
	if(!GetCurrentWin())return;
	fwSdk->SwitchRender(GetCurrentWin()->GetRender());
	fwSdk->Reshape(w, h);
}

void FWApp::MouseButton(int button, int state, int x, int y){
	mouseInfo.lastPos.x = x, mouseInfo.lastPos.y = y;
	if(button == LEFT_BUTTON)
		mouseInfo.left = (state == BUTTON_DOWN);
	if(button == RIGHT_BUTTON)
		mouseInfo.right = (state == BUTTON_DOWN);
	if(state == BUTTON_DOWN)
		mouseInfo.first = true;
	int mod = GetModifier();
	mouseInfo.shift = mod & ACTIVE_SHIFT;
	mouseInfo.ctrl  = mod & ACTIVE_CTRL;
	mouseInfo.alt   = mod & ACTIVE_ALT;

	// カーソルで剛体を動かす
	if(mouseInfo.left && mouseInfo.ctrl && fwSdk->GetScene()){
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
			// 視点移動
			cameraInfo.rot.y += (float)xrel * 0.01f;
			cameraInfo.rot.y =
				Spr::max(cameraInfo.rotRangeY[0], Spr::min(cameraInfo.rot.y, cameraInfo.rotRangeY[1]));
			cameraInfo.rot.x += (float)yrel * 0.01f;
			cameraInfo.rot.x =
				Spr::max(cameraInfo.rotRangeX[0], Spr::min(cameraInfo.rot.x, cameraInfo.rotRangeX[1]));
			cameraPosChange = true;
		}
	}
	// 右ボタン
	if(mouseInfo.right){
		// ズーム
		cameraInfo.zoom *= (float)exp((double)yrel/10.0);
		cameraInfo.zoom = Spr::max(cameraInfo.zoomRange[0], Spr::min(cameraInfo.zoom, cameraInfo.zoomRange[1]));
		cameraPosChange = true;
	}
	if(cameraPosChange){
		cameraInfo.view  = Affinef();
		cameraInfo.view.Pos() = cameraInfo.target + cameraInfo.zoom * Vec3f(
			cos(cameraInfo.rot.x) * cos(cameraInfo.rot.y),
			sin(cameraInfo.rot.x),
			cos(cameraInfo.rot.x) * sin(cameraInfo.rot.y));
		cameraInfo.view.LookAtGL(cameraInfo.target, Vec3f(0.0f, 100.0f, 0.0f));
	}
}

}

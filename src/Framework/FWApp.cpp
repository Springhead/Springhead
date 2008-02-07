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
	lastMousePos.x = x, lastMousePos.y = y;
	if(button == 0 /*GLUT_LEFT_BUTTON*/ )
		bLeftMouseButton = (state == 0 /*GLUT_DOWN*/);
	if(button == 2 /*GLUT_RIGHT_BUTTON*/)
		bRightMouseButton = (state == 0/*GLUT_DOWN*/);
}
void FWApp::MouseMove(int x, int y){
	static bool bFirst = true;
	int xrel = x - lastMousePos.x, yrel = y - lastMousePos.y;
	lastMousePos.x = x;
	lastMousePos.y = y;
	if(bFirst){
		bFirst = false;
		return;
	}
	// 左ボタン
	if(bLeftMouseButton){
		mouseCameraRot.y += xrel * 0.01;
		mouseCameraRot.y = Spr::max(Rad(-180.0), Spr::min(mouseCameraRot.y, Rad(180.0)));
		mouseCameraRot.x += yrel * 0.01;
		mouseCameraRot.x = Spr::max(Rad(-80.0), Spr::min(mouseCameraRot.x, Rad(80.0)));
	}
	// 右ボタン
	if(bRightMouseButton){
		mouseCameraZoom *= exp(yrel/10.0);
		mouseCameraZoom = Spr::max(0.1, Spr::min(mouseCameraZoom, 100.0));
	}
	if (bLeftMouseButton || bRightMouseButton){
		mouseCameraView = Affinef();
		double yoffset = 10.0;
		mouseCameraView.Pos() = mouseCameraZoom * Vec3f(
			cos(mouseCameraRot.x) * cos(mouseCameraRot.y),
			sin(mouseCameraRot.x),
			cos(mouseCameraRot.x) * sin(mouseCameraRot.y));
		mouseCameraView.PosY() += yoffset;
		mouseCameraView.LookAtGL(Vec3f(0.0, yoffset, 0.0), Vec3f(0.0f, 100.0f, 0.0f));
	}
}



}

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
	if(button == 0 /*GLUT_LEFT_BUTTON*/ )
		mouseInfo.left = (state == 0 /*GLUT_DOWN*/);
	if(button == 2 /*GLUT_RIGHT_BUTTON*/)
		mouseInfo.right = (state == 0/*GLUT_DOWN*/);
	if(state == 0)
		mouseInfo.first = true;
}

void FWApp::MouseMove(int x, int y){
	int xrel = x - mouseInfo.lastPos.x, yrel = y - mouseInfo.lastPos.y;
	mouseInfo.lastPos.x = x;
	mouseInfo.lastPos.y = y;
	if(mouseInfo.first){
		mouseInfo.first = false;
		return;
	}
	// 左ボタン
	if(mouseInfo.left){
		cameraInfo.rot.y += (float)xrel * 0.01f;
		cameraInfo.rot.y =
			Spr::max(cameraInfo.rotRangeY[0], Spr::min(cameraInfo.rot.y, cameraInfo.rotRangeY[1]));
		cameraInfo.rot.x += (float)yrel * 0.01f;
		cameraInfo.rot.x =
			Spr::max(cameraInfo.rotRangeX[0], Spr::min(cameraInfo.rot.x, cameraInfo.rotRangeX[1]));
	}
	// 右ボタン
	if(mouseInfo.right){
		cameraInfo.zoom *= (float)exp((double)yrel/10.0);
		cameraInfo.zoom = Spr::max(cameraInfo.zoomRange[0], Spr::min(cameraInfo.zoom, cameraInfo.zoomRange[1]));
	}
	if (mouseInfo.left || mouseInfo.right){
		cameraInfo.view  = Affinef();
		cameraInfo.view.Pos() = cameraInfo.target + cameraInfo.zoom * Vec3f(
			cos(cameraInfo.rot.x) * cos(cameraInfo.rot.y),
			sin(cameraInfo.rot.x),
			cos(cameraInfo.rot.x) * sin(cameraInfo.rot.y));
		cameraInfo.view.LookAtGL(cameraInfo.target, Vec3f(0.0f, 100.0f, 0.0f));
	}
}

}

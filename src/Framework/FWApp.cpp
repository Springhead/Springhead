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

/*void FWSdk::AssignScene(FWWin* win){
	if (win->scene) return;
	for(Scenes::reverse_iterator s=scenes.rbegin(); s!=scenes.rend(); ++s){
		std::set<FWWin>::iterator w;
		for(w = wins.begin(); w != wins.end(); ++w){
			if (w->scene == *s) break;
		}
		if (w == wins.end()){	//	‘Î‰ž‚·‚éwindow‚ª‚È‚¢scene
			win->scene = *s;
			return;
		}
	}
}*/

void FWApp::Step(){
	//for(int i=0; i<GetSdk()->NScene(); ++i){
	//	GetSdk()->GetScene(i)->Step();
	//}
	if(!GetCurrentWin())return;
	fwSdk->SwitchScene(GetCurrentWin()->GetScene());
	fwSdk->Step();
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

}

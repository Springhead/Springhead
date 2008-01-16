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

void FWApp::Step(){
	fwSdk->Step();
}

void FWApp::Display(){
	fwSdk->Draw(GetWin());
}

void FWApp::Reshape(int w, int h){
	fwSdk->Reshape(GetWin(), w, h);
}

}

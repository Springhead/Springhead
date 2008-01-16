/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/SprFWAppGL.h>
#include "FWScene.h"
#include "FWSdk.h"
#include "FWOldSpringheadNode.h"
#include <Physics/PHSdk.h>
#include <Graphics/GRSdk.h>

#include <GL/glut.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

FWSdkIf::FWWinRender* FWAppGL::CreateRender(int wid){
	FWSdkIf::FWWinRender* win = fwSdk->CreateWin(wid, fwSdk->GetGRSdk()->CreateDebugRender());
	
	GRDeviceIf* dev = fwSdk->GetGRSdk()->CreateDeviceGL();
	dev->Init();
	win->render->SetDevice(dev);

	Affinef view;
	view.Pos() = Vec3f(0.0, 3.0, 3.0);
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
	view = view.inv();	
	
	win->render->SetViewMatrix(view);
	return win;
}


}

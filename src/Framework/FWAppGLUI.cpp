/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/SprFWAppGLUI.h>
#include "FWScene.h"
#include "FWSdk.h"
#include "FWOldSpringheadNode.h"
#include <Physics/PHSdk.h>
#include <Graphics/GRSdk.h>
#include <GL/glui.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWAppGLUIDesc::FWAppGLUIDesc(){
	fromTop				= 50;
	fromLeft			= 30;
	subPosition			= GLUI_SUBWINDOW_RIGHT;
	gluiName			= "Menu";
	createOtherWindow	= true;
}

FWAppGLUI::~FWAppGLUI(){
}

//------------------------------------------------------------------
void FWAppGLUI::Start(){
	instance = this;
	if (!NWin()){
		CreateWin();
		wins.back()->SetScene(GetSdk()->GetScene());
	}
	// gluiを使うならFWAppGLUT::GlutTimerFuncはGLUI_Masterの管理下に置く必要がある
	GLUI_Master.set_glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, 0);
	GLUI_Master.set_glutIdleFunc(FWAppGLUT::GlutIdleFunc);

	glutMainLoop();
}

GLUI* FWAppGLUI::CreateGUI(int wid, FWAppGLUIDesc desc){
	GLUI* glui;
	if(desc.createOtherWindow == false){
		glui = GLUI_Master.create_glui_subwindow(wid, desc.subPosition);		
	}else{
		glui = GLUI_Master.create_glui(gluiName, 0, desc.fromLeft, desc.fromTop);
		glui->set_main_gfx_window(wid);
	}
	guis.push_back(glui);
	return glui;
}

}

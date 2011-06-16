/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/SprFWApp.h>
#include <Framework/FWGLUI.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

/*FWGLUIDescの実装*///---------------------------------------------------------
/*FWGLUIDesc::FWGLUIDesc(){
	fromTop				= 50;
	fromLeft			= 30;
	subPosition			= GLUI_SUBWINDOW_RIGHT;
	gluiName			= "Menu";
	createOtherWindow	= true;
}*/
FWGLUI::~FWGLUI(){
}

/*FWGLUIの実装*///--------------------------------------------------------------
void FWGLUI::StartMainLoop(){
	instance = this;
	// これはInitでやるべきことでは？
	//if (!fwApp->NWin()){
	//	fwApp->CreateWin()->SetScene(fwApp->GetSdk()->GetScene());
	//}

	// gluiを使うならFWAppGLUT::GlutTimerFuncはGLUI_Masterの管理下に置く必要がある
	GLUI_Master.set_glutTimerFunc(1, FWGLUT::GlutTimerFunc, 0);
	GLUI_Master.set_glutIdleFunc(FWGLUT::GlutIdleFunc);

	//glutIdleFunc(FWGLUT::GlutIdleFunc);
	
	FWGLUT::StartMainLoop();
}

GLUI* FWGLUI::CreateGUI(int wid, int top, int left, int subPos, char* name, bool other){
	fromTop		= top;
	fromLeft	= left;
	subPosition = subPos;
	gluiName	= name;
	createOtherWindow = other;

	GLUI* glui;
	if(!createOtherWindow){
		glui = GLUI_Master.create_glui_subwindow(wid, subPosition);		
	}else{
		glui = GLUI_Master.create_glui(gluiName, 0, fromLeft, fromTop);
		glui->set_main_gfx_window(wid);
	}
	guis.push_back(glui);
	return glui;
}
}
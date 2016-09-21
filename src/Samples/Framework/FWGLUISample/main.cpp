/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "../../SampleApp.h"

#pragma hdrstop
using namespace Spr;
using namespace std;
#ifdef DAILYBUILD
  #include "dailybuild_SEH_Handler.h"
#endif

class MyApp : public SampleApp{
public:
	FWWinIf*		mainWin;
	FWDialogIf*		topDlg;

public:
	MyApp(){

	}

	void InitGUI(){
		FWWinIf*	mainWin = GetCurrentWin();
		
		// 埋め込みダイアログ
		FWDialogDesc dlgDesc;
		dlgDesc.dock = true;
		dlgDesc.dockPos = FWDialogDesc::DOCK_TOP;
		FWDialogIf*		topDlg = mainWin->CreateDialog(dlgDesc);
		FWPanelIf* frame = topDlg->CreatePanel("group", FWPanelDesc::EMBOSSED);
		FWPanelIf* group = topDlg->CreateRadioGroup(frame);
		topDlg->CreateRadioButton("1", group);
		topDlg->CreateRadioButton("2", group);
		topDlg->CreateRadioButton("3", group);
		topDlg->CreateColumn(true);
		topDlg->CreateRotationControl("rot");
		topDlg->CreateColumn(false);
		topDlg->CreateTranslationControl("trn");

	}
	
	virtual void Init(int argc, char* argv[]){
		SetGRHandler(TypeGLUI);
		SampleApp::Init(argc, argv);

		InitGUI();
	}
	virtual void Keyboard(int key, int x, int y){
		SampleApp::Keyboard(key, x, y);
	}

	virtual void OnControlUpdate(FWControlIf* ctrl){
		
	}

} app;

int _cdecl main(int argc, char* argv[]){
	SEH_HANDLER_DEF
	SEH_HANDLER_TRY
	app.Init(argc, argv);
	app.StartMainLoop();
	SEH_HANDLER_CATCH
	return 0;
}

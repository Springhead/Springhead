/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "MYApp.h"
#include <windows.h>
#include <Framework/SprFWAppGLUT.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>

#define ESC 27
using namespace std;

//=======================================================================================================
// コンストラクタ・デストラクタ
MYApp::MYApp(){
	instance	= this;
	dt			= 0.05;
	nIter		= 20;
	bTimer		= true;
	numWindow	= 3;
	for(int i = 0; i < numWindow; i++){
		stringstream sout1;
		sout1 << "Window " << i+1;
		winNames.push_back(sout1.str());
		stringstream sout2;
		sout2 << "joint" << i+1 << ".x";
		fileNames.push_back(sout2.str());
		views.push_back(Affinef());
	}
}

//=======================================================================================================
// クラス内の関数定義
void MYApp::NumOfClassMembers(std::ostream& out){
	out << "Show the sizes of te MYApp's vector members" << std::endl;
	out << "numWindow		: " << numWindow		<< std::endl;
	out << "windows.size    : " << windows.size()	<< std::endl;
	out << "fwScenes		: " << fwScenes.size()	<< std::endl;
	out << "winNames		: " << winNames.size()  << std::endl;
	out << "views			: " << views.size()		<< std::endl;
}

//=======================================================================================================
// 上位階層で宣言された関数のオーバーロード

void MYApp::Init(int argc, char* argv[]){
	
	FWAppGLUT::Init(argc, argv);
	GetSdk()->Clear();

	for(int i = 0; i < numWindow ; i++){

		if(GetSdk()->LoadScene(fileNames[i])){
			stringstream sout;
			sout << "fwScene" << i+1;
			if(GetSdk()->FindObject(sout.str()))
				fwScenes.push_back(GetSdk()->FindObject(sout.str())->Cast());
			else{
				DSTR << "[Error] : NO scenes wa have. (MYApp.cpp l.69)" << std::endl;
				exit(0xff);
			}

		}
		else{
			DSTR << "[Error] : Cannot open " << fileNames[i] << ". (MYApp.cpp l.75)" << std::endl;
			exit(0xff);
		}

		FWAppGLUTDesc winDesc;
		{
			winDesc.width			= 480;
			winDesc.height			= 320;
			winDesc.left			= 10 + 500*(i/2);
			winDesc.top				= 30 + 360*(i%2);
			winDesc.parentWindow	= 0;
			winDesc.fullscreen		= false;
			if(winNames[i].size()){
				winDesc.title		= winNames[i];
			}else{
				winDesc.title		= "Window";
			}
		}
		windows.push_back(CreateWin(winDesc));
		windows[i]->scene = fwScenes[i];
		views.back() = GetCameraInfo().view;
	}
	NumOfClassMembers(DSTR);
	return;
}

void MYApp::Keyboard(int key, int x, int y){
	if(key == 'q'){
		exit(0);
	} else if(key == ESC){
		exit(0);
	} else{
	}
}

void MYApp::Display(){
		FWWin* wr = GetCurrentWin();

// Mesh がある場合は#if _DEBUGでくくる (from here)
		GetSdk()->SetDebugMode(true);
		GRDebugRenderIf* r = wr->render->Cast();
		r->SetRenderMode(true, true);
//		r->EnableRenderAxis();
		r->EnableRenderForce();
		r->EnableRenderContact();
//#endif (to here)

		GRCameraIf* cam = wr->scene->GetGRScene()->GetCamera();
		if (cam && cam->GetFrame()){
			//Affinef af = cam->GetFrame()->GetTransform();
			cam->GetFrame()->SetTransform(cameraInfo.view);
		}else{
			wr->render->SetViewMatrix(cameraInfo.view.inv());
		}
		FWAppGLUT::Display();
}

int exitCount = 0;
void MYApp::Step(){
	exitCount ++;
	if (exitCount > 60/dt) exit(0);
	if(bTimer){
		FWApp::Step();
		for(int i = 0; i < numWindow; i++){
			SetCurrentWin(GetWin(i));	
			glutPostRedisplay();	
		}
	}
}

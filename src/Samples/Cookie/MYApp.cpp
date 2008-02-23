/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "MYApp.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <GL/glut.h>

#define ESC 27

//ロードしたいシーンが登録されたXFile名

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//コンストラクタ・デストラクタ
MYApp::MYApp(){
	instance			= this;
	filePass			= "./xFiles/";
	fileName			= "sceneCookie.x";
	dt					= 0.01;
	nIter				= 50;
	bStep				= true;
	flagDebugMode		= true;
	flagUseSpidar		= false;
	camHeight			= -0.2f;
	camTopAngle			= 0.0f;
	hPointerName		= "soHapticPointer";
	phCookieName		= "soCookie";
	grCookieName		= "grFrameCookie";
	fwScene				= NULL;
	phScene				= NULL;
	grRender			= NULL;
	deviceGL			= NULL;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//初期化をする関数実装
void MYApp::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);
	GetSdk()->Clear();
	std::stringstream imputFileName;
	imputFileName << filePass << fileName;
	GetSdk()->LoadScene(imputFileName.str().c_str());
	if (GetSdk()->GetScene()){
		phScene = GetSdk()->GetScene()->GetPHScene();
		if(phScene){
			phScene->SetTimeStep(dt);
			phScene->SetNumIteration(nIter);
		}
	}
	FWWinDesc windowDesc;
	windowDesc.width	= 800;
	windowDesc.height	= 600;
	windowDesc.left		= 5;
	windowDesc.top		= 5;
	windowDesc.title	= "Haptic HDR Rendering";
	window				= CreateWin(windowDesc);
	grRender			= window->render->Cast();

	//view行列の初期化
	cameraInfo.view[0][0] =  0.5;
	cameraInfo.view[0][1] = -0.5;
	cameraInfo.view[0][2] =  0.7;
	cameraInfo.view[0][3] =  0.8;
	cameraInfo.view[1][0] =  0.0;
	cameraInfo.view[1][1] =  0.8;
	cameraInfo.view[1][2] =  0.5;
	cameraInfo.view[1][3] =  0.6;
	cameraInfo.view[2][0] = -0.9;
	cameraInfo.view[2][1] = -0.3;
	cameraInfo.view[2][2] =  0.4;
	cameraInfo.view[2][3] =  0.5;
	cameraInfo.view[3][0] =  0.0;
	cameraInfo.view[3][1] =  0.0;
	cameraInfo.view[3][2] =  0.0;
	cameraInfo.view[3][3] =  1.0;

	if(flagUseSpidar){
		devMan.RPool().Register(DBG_NEW DRUsb20Simple(10));	//< USB2.0版コントローラ 8モータ
		devMan.RPool().Register(DBG_NEW DRUsb20Sh4(0));		//< Sh4版コントローラ 8モータ
		devMan.RPool().Register(DBG_NEW DRUsb20Sh4(1));		//< Sh4版コントローラ 8モータ
		devMan.RPool().Register(DBG_NEW DRUsb20Sh4(2));		//< Sh4版コントローラ 8モータ
		devMan.Init();										//< デバイスの初期化
		std::cout	<< "devMan : " << devMan;				//< 初期化の結果を表示
		DSTR		<< "devMan : " << devMan;				//< 初期化の結果を表示

		spidarG6.Init(devMan, false);						//	SPIDARの初期化，キャリブレーションもする．
		spidarG6.Calib();									//< spidarのキャリブレーションを行う
	}
}

//シミュレーションで毎ステップ呼ばれる関数実装
void MYApp::Step(){
	if(bStep){
		if(!phScene)return;
		if(flagUseSpidar){
			spidarG6.Update(dt);
			PHSolidIf* hPointerSolid	= DCAST(PHSolidIf, GetSdk()->GetScene()->GetPHScene()->FindObject(hPointerName));	// 力覚ポインタ表示用の剛体をとってくる
			hapticPosition				= spidarG6.GetPos() * 50;															// 位置の読み出し Vec3fについては，自動生成マニュアルを参照
			hapticRotation				= spidarG6.GetOri() * 50;															// 回転の読み出し(回転を入れるとなんか物が触れない状態)
			if (hPointerSolid){
				hPointerSolid->SetFramePosition(Vec3d(hapticPosition));															// 力覚ポインタ剛体に力覚ポインタの位置を指定
				hPointerSolid->SetOrientation(Quaterniond(hapticRotation));														// 力覚ポインタ剛体に力覚ポインタの回転を指定			
		
			}
		}
		FWAppGLUT::Step();
		if (phScene){
			phScene->ClearForce();
			phScene->GenerateForce();
			phScene->Integrate();
		}
		glutPostRedisplay();
	}
}

//引数の名前の剛体を探してきて，同じデータのものを複製する関数の実装
bool MYApp::CopyTheCookie(const char* phName, const char* grName){
	if(GetSdk()->GetScene()->GetPHScene() && GetSdk()->GetScene()->GetGRScene()){
		if(GetSdk()->GetScene()->GetPHScene()->FindObject(phName)){
			PHSolidIf* bisSolid = DCAST(PHSolidIf, GetSdk()->GetScene()->GetPHScene()->FindObject(phName));
			PHSolidDesc sDesc;
			bisSolid->GetDesc(&sDesc);

			GRFrameIf* bisFrame = DCAST(GRFrameIf, GetSdk()->GetScene()->GetGRScene()->FindObject(grName));
			GRFrameDesc fDesc;
			bisFrame->GetDesc(&fDesc);

			copiedCookies.push_back(GetSdk()->GetScene()->CreateObject(sDesc, fDesc));

			for (unsigned int i=0; i<bisSolid->NChildObject(); ++i) {
				copiedCookies.back()->GetPHSolid()->AddChildObject(bisSolid->GetChildObject(i));
			}
			for (unsigned int i=0; i<bisFrame->NChildObject(); ++i) {
				copiedCookies.back()->GetGRFrame()->AddChildObject(bisFrame->GetChildObject(i));
			}

			std::stringstream sout;
			sout << "soCopiedCookie" << copiedCookies.size();
			copiedCookies.back()->GetPHSolid()->SetName(sout.str().c_str());
			copiedCookies.back()->GetPHSolid()->SetFramePosition(Vec3d(   (double)rand() / (double)RAND_MAX * 0.1, (double)rand() / (double)RAND_MAX * 0.05 - 0.3, (double)rand() / (double)RAND_MAX * -0.1 ));
			copiedCookies.back()->GetPHSolid()->SetAngularVelocity(Vec3d( (double)rand() / (double)RAND_MAX * 2.9,  (double)rand() / (double)RAND_MAX * 0.7,		(double)rand() / (double)RAND_MAX * 1.7 ));
			copiedCookies.back()->GetPHSolid()->SetOrientation(Quaternionf::Rot(Rad((double)rand() / (double)RAND_MAX) * 2.1, 'x') 
															  * Quaternionf::Rot(Rad((double)rand() / (double)RAND_MAX) * 3.3, 'y')
															  * Quaternionf::Rot(Rad((double)rand() / (double)RAND_MAX) * 7.3, 'z'));
			sout.str("");
			sout << "frCopiedCookie" << copiedCookies.size();
			copiedCookies.back()->GetGRFrame()->SetName(sout.str().c_str());
			DSTR		<< "Copy number of the Cookie : " << copiedCookies.size() << std::endl;
			std::cout	<< "Copy number of the Cookie : " << copiedCookies.size() << std::endl;

			return true;
		}
	}
	DSTR		<< "Failed to copy." << std::endl;
	std::cout	<< "Failed to copy." << std::endl;
	return false;
}

void MYApp::ResetScene(){
	GetSdk()->Clear();
	GetSdk()->LoadScene(fileName);
	phScene = GetSdk()->GetScene()->GetPHScene();
	if(phScene){
		phScene->SetTimeStep(dt);
		phScene->SetNumIteration(nIter);
	}
}


/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
//キーボードで何か押された時にコールバックとして呼ばれる関数実装
void MYApp::Keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'q' :
		case 'Q' :
		case ESC :
			{
				DSTR		<< "Quit the program." << std::endl;
				std::cout	<< "Quit the program." << std::endl;
				exit(0);
			}			
			break;		
		case 'r':
			{
				bStep = true;
				DSTR		<< "Run the simulation." << std::endl;
				std::cout	<< "Run the simulation." << std::endl;
			}
			break;
		case 's':
			{
				bStep = false;
				DSTR		<< "Stop the simulation." << std::endl;
				std::cout	<< "Stop the simulation." << std::endl;
				for(int i=0; i<1; ++i){
					bStep = true;
					Step();
					bStep = false;
				}
			}
			break;
		case ' ':
			{
				if(flagDebugMode)		flagDebugMode = false;
				else if(!flagDebugMode) flagDebugMode = true;
			}
			break;
		case 'c':
			if(flagUseSpidar){
				spidarG6.Calib();
				DSTR		<< "Spidar Calibration" << std::endl;
				std::cout	<< "Spidar Calibration" << std::endl;
			}
			break;
		case 'm':
			CopyTheCookie(phCookieName, grCookieName);
			break;
		case 'n':
			ResetScene();
			break;
		case '1'://トップ視点ズームイン
			{
				camHeight -= 0.05;
				cameraInfo.view.Pos() = Vec3f(0.0f, camHeight, 0.0f);
				cameraInfo.view.LookAtGL(Vec3f(0.0, -5.0, 0.0), Quaternionf::Rot(Rad(camTopAngle), 'y') * Vec3f(100.0, 0.0, 0.0));
				GRFrameIf* cam = GetSdk()->GetScene()->GetGRScene()->FindObject("grFrameWorld")->Cast();
				cam->SetTransform(cameraInfo.view);
				break;
			}
		case '2'://トップ視点ズームアウト
			{	
				camHeight += 0.05;
				cameraInfo.view.Pos() = Vec3f(0.0f, camHeight, 0.0f);
				cameraInfo.view.LookAtGL(Vec3f(0.0, -5.0, 0.0), Quaternionf::Rot(Rad(camTopAngle), 'y') * Vec3f(100.0, 0.0, 0.0));
				GRFrameIf* cam = GetSdk()->GetScene()->GetGRScene()->FindObject("grFrameWorld")->Cast();
				cam->SetTransform(cameraInfo.view);
				break;
			}
		case '3'://トップ視点回転
			{
				camTopAngle += 10.0;
				cameraInfo.view.Pos() = Vec3f(0.0f, camHeight, 0.0f);
				cameraInfo.view.LookAtGL(Vec3f(0.0, -5.0, 0.0), Quaternionf::Rot(Rad(camTopAngle), 'y') * Vec3f(100.0, 0.0, 0.0));
				GRFrameIf* cam = GetSdk()->GetScene()->GetGRScene()->FindObject("grFrameWorld")->Cast();
				cam->SetTransform(cameraInfo.view);
				break;
			}
		case '4':
			{
				DSTR		<< "view:" << std::endl;
				std::cout	<< "view:" << std::endl;
				std::cout	<< cameraInfo.view << std::endl;
				DSTR		<< cameraInfo.view << std::endl;
			}
		default :
			break;
	}
}

void MYApp::Display(){
	grRender->ClearBuffer();
	
	grRender->BeginScene();
	GRCameraIf* cam = GetSdk()->GetScene() ? GetSdk()->GetScene()->GetGRScene()->GetCamera() : NULL;
	if (cam && cam->GetFrame()){
		//Affinef af = cam->GetFrame()->GetTransform();
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		grRender->SetViewMatrix(cameraInfo.view.inv());
	}
	if(GetSdk() && GetSdk()->GetScene())
		GetSdk()->GetScene()->Draw(grRender, flagDebugMode);
	grRender->EndScene();

	glutSwapBuffers();
}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Springhead.h>
#include <Creature/CRCreature.h>
#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <ctime>
#include <gl/glui.h>

#include "myGlobalValue.h"
//関数定義が邪魔だから外に追いやっているだけなので消してはいけない。
//そのうちちゃんとクラスとか構造体を作って入れよう。
//【追記】⇒2008.01.20現在FrameworkにFWAppGLUIクラスが実装されている。
#include "LoadDataFromGUI.h"
#include "SetDataFromGUI.h"
#include "ResetDataFromGUI.h"
#include "DesignOfGUI.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

void step(){

	phScene->ClearForce();
	// 独自の制御はここに入れる
	phScene->GenerateForce();
	phScene->Integrate();
	
}

void GLUT_Idle(){
	glutPostRedisplay();
}

void GLUT_Timer(int id){
	glutSetWindow(mainWindow);
	if (!bTimer) return;
	glutTimerFunc(simulationPeriod, GLUT_Timer, 0);							//ここに書くことでglutTimerFunc()を再帰関数にしている
	step();	
	glutPostRedisplay();
}

void createFloor(){
	
	PHSolidDesc descSolid;													//床の初期化用のディスクリプタの宣言
	{
		descSolid.dynamical = false;										//床だから物理法則を切る
	}
	PHSolidIf* soFloor = phScene->CreateSolid(descSolid);					//剛体インタフェースにディスクリプタの中身を渡した”剛体という概念”を登録する
	soFloor->SetName("Floor");
	CDBoxDesc descBox;														//床の衝突判定用のディスクリプタの宣言
	{
		descBox.boxsize = Vec3f(30, 2, 30);									//箱のサイズの指定
	}
	soFloor->AddShape(phSdk->CreateShape(descBox));							//先ほど登録した”剛体という概念”に衝突判定できる実体を与える
	soFloor->SetFramePosition(Vec3f(0, -1, 0));								//実体を持つ剛体の設置場所を指定する

}	

void CreateBody(){

	// --- --- --- --- ---
	// クリーチャ全体

	CRCreatureDesc descCreature;
	{
		
	}
	
	creature = (DBG_NEW CRCreature(descCreature, phScene))->Cast();

	// --- --- --- --- ---
	// Body

	CRFourLegsAnimalBodyDesc bodyDesc;
	{
		//哺乳類データで変更したい初期値があればここでbodyDesc.******をよんで書き換えることもできる。
		bodyDesc.goalShoulder	 = Quaterniond::Rot(Rad(-160), 'x');
		bodyDesc.goalHip		 = Quaterniond::Rot(Rad(- 30), 'x');
		bodyDesc.originElbow	 = Rad( 130);
		bodyDesc.originStifle	 = Rad(-150);
		bodyDesc.originFrontKnee = Rad(-170);
		bodyDesc.originRearKnee	 = Rad( 170);

	}
    
	animalBody = DCAST(CRFourLegsAnimalBodyIf, creature->CreateBody(bodyDesc));


//test from here

	// --- --- --- --- ---
	// コントローラの登録
	
	// --- --- --- --- ---
	// 初期化

	creature->Init();

	// --- --- --- --- ---
	// Horseの初期位置調整
	Vec3f initialPosition = Vec3f(0, 2.0f, 1.5);
	for (int i=0; i<animalBody->NSolids(); i++) {
		PHSolidIf* so = animalBody->GetSolid(i);
		if (so) {
			so->SetFramePosition(so->GetPose().Pos() + initialPosition);
		}
	}
// to here

	phScene->Print(DSTR);
}

void GLUT_Display(){
	glutSetWindow(mainWindow);
	grRender->ClearBuffer();					//バッファーの中身を消す

	grRender->BeginScene();						//描画するシーンの始まり
	grRender->DrawScene(phScene);				//シーングラフを描画する。（本当はきれいなモデルを動かせるけど剛体で置き換えた世界：phScene）

	//　何か独自に描画したい内容があればここで
	//////////////////////////////////////////////////
	//ここに普通のglBegin()とか書くと重ね描きできる	//
	//////////////////////////////////////////////////
	
	grRender->EndScene();						//描画するシーンの終わり

	glutSwapBuffers();							//バッファーを入れ替えることでスムーズ描画をする
}

void GLUT_Reshape(int w, int h){
	//　Viewportと射影行列を設定
	grRender->Reshape(Vec2f(), Vec2f(w,h));	   //Windowの大きさが変わった時、描画をReshape関数の二次元ベクトルに従って行う
}

void onKey(char key){

	PHBallJointDesc ballD;
	PHHingeJointDesc hingeD;

	switch(key){
	// --- 視点変更
	{
	case '1'://初期設定化
		camAngle = 0.0f;
		camZoom  = 3.0f;
		view.Pos() = Vec3f(6.0f*cos(Rad(camAngle)), 3.0f, 6.0f*sin(Rad(camAngle))) * camZoom;	//カメラの座標を指定する
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));								//カメラが見る座標, 上方向ベクトル 
		grRender->SetViewMatrix(view.inv());													//カメラ視点を登録する
		break;

	case '2'://右回り
		camAngle -= 10.0f;
		if (camAngle < 0) { camAngle -= 360.0f; }
		view.Pos() = Vec3f(6.0f*cos(Rad(camAngle)), 3.0f, 6.0f*sin(Rad(camAngle))) * camZoom;
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
		grRender->SetViewMatrix(view.inv());
		break;

	case '3'://左回り
		camAngle += 10.0f;
		if (camAngle < 0) { camAngle -= 360.0f; }
		view.Pos() = Vec3f(6.0f*cos(Rad(camAngle)), 3.0f, 6.0f*sin(Rad(camAngle))) * camZoom;
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
		grRender->SetViewMatrix(view.inv());
		break;

	case '4'://ズーム
		camZoom -= 0.5;
		if (camZoom < 0.5) { camZoom = 0.5; }
		view.Pos() = Vec3f(6.0f*cos(Rad(camAngle)), 3.0f, 6.0f*sin(Rad(camAngle))) * camZoom;
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
		grRender->SetViewMatrix(view.inv());
		break;

	case '5'://広角
		camZoom += 0.5;
		view.Pos() = Vec3f(6.0f*cos(Rad(camAngle)), 3.0f, 6.0f*sin(Rad(camAngle))) * camZoom;
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
		grRender->SetViewMatrix(view.inv());
		break;
	
	case '6'://現在の重心を表示
		DSTR << "Center Of Mass : " << animalBody->GetCenterOfMass() << endl;
		break;

	case '7'://現在の上体重心を表示
		DSTR << "Center Of Mass Of Upper Part : " << animalBody->GetUpperCenterOfMass() << endl;
		break;
	}

	
	// --- ボディを動かす
	{
	case 'a':
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_CHEST_NECK)->GetDesc(&ballD);
		ballD.goal = Quaterniond::Rot(Rad(90), 'x');
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_CHEST_NECK)->SetDesc(&ballD);
		break;
	case 'A':
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_CHEST_NECK)->GetDesc(&ballD);
		ballD.goal = Quaterniond::Rot(Rad(60), 'x');
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_CHEST_NECK)->SetDesc(&ballD);
		break;
	
	case 'z':
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_SHOULDER)->GetDesc(&ballD);
		ballD.goal = Quaterniond::Rot(Rad(-100), 'x');
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_SHOULDER)->SetDesc(&ballD);

		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_ELBOW)->GetDesc(&hingeD);
		hingeD.origin = Rad(70);
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_ELBOW)->SetDesc(&hingeD);

		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_FRONT_KNEE)->GetDesc(&hingeD);
		hingeD.origin = Rad(-60);
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_FRONT_KNEE)->SetDesc(&hingeD);
		break;

	case 'x':
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_HIP)->GetDesc(&ballD);
		ballD.goal = Quaterniond::Rot(Rad(-30), 'x');
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_HIP)->SetDesc(&ballD);

		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_STIFLE)->GetDesc(&hingeD);
		hingeD.origin = Rad(-70);
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_STIFLE)->SetDesc(&hingeD);

		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_REAR_KNEE)->GetDesc(&hingeD);
		hingeD.origin = Rad(60);
		animalBody->GetJoint(CRFourLegsAnimalBodyDesc::JO_RIGHT_REAR_KNEE)->SetDesc(&hingeD);
		break;
	}

	case 'p': 
		bTimer = false;
		step();
		break;

	case 'P':
		if(bTimer == true){
			bTimer = false;
			step();
		}
		else{
			bTimer = true;
			GLUI_Master.set_glutTimerFunc(simulationPeriod, GLUT_Timer, 0);
		}
		break;

	case 'o':
		bTimer = true;
		GLUI_Master.set_glutTimerFunc(simulationPeriod, GLUT_Timer, 0);
		break;
	}
}

void GLUT_Keyboard(unsigned char key, int x,int y){
	
	glutSetWindow(mainWindow);
	
	int i = 0;
	PHSceneDesc sceneDesc;
	PHSceneIf*	state;

	switch(key){
		case ESC: // exit this program
			PressQuit();
			break;
	
		case ' ': // Reset the scene
			phScene->Clear();
			sceneDesc = PHSceneDesc();
			sceneDesc.timeStep = dt;										//シミュレーションの刻み時間を登録する
			sceneDesc.numIteration = niter;									//LCP反復回数を登録する
			phScene = phSdk->CreateScene(sceneDesc);						//物理シミュレータの中にディスクリプタの変数を入れたシーングラフを作成
			createFloor();													//シーングラフ内に床を作る
			CreateBody();
			guiLoadScene();
			guiLoadInit();
			break;

		case '0':
			phScene->GetState(&state);
			DSTR << state << std::endl;
			break;

		default:
			onKey(key);
			break;	
	}
}

void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 1.0);
	grRender->PushLight(light0);
	grRender->PushLight(light1);

}

void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	setLight();
}

int main(int argc, char* argv[]){
	
	//　SDKの作成　
	phSdk = PHSdkIf::CreateSdk();									//物理シミュレータが要る
	grSdk = GRSdkIf::CreateSdk();									//描画が要る

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//fMax付きsimulator

	//　シーンオブジェクトの作成
	PHSceneDesc sceneDesc;											//シーングラフ（設定環境みたいなもの？）を初期化するディスクリプタ
	sceneDesc.timeStep = dt;										//シミュレーションの刻み時間を登録する
	sceneDesc.numIteration = niter;									//LCP反復回数を登録する
	phScene = phSdk->CreateScene(sceneDesc);						//物理シミュレータの中にディスクリプタの変数を入れたシーングラフを作成
	createFloor();													//シーングラフ内に床を作る
	CreateBody();													//シーングラフ内に体を作る

	glutInit(&argc, argv);											//GLUTを初期化する
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);		//GLUTのディスプレイモードを指定する
	glutInitWindowSize(480, 320);									//OpenGLのWindowサイズを指定する
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//window1 : simulator window
	mainWindow = glutCreateWindow("Horse body Simulator");			//OpenGLのWindowタイトルを指定する
	glutPositionWindow(10, 30);
	///---- GLUIの作成
	glui = GLUI_Master.create_glui("Menu", 0, 510, 30);				//< 別ウィンドウを作成するときはこっち
	//glui = GLUI_Master.create_glui_subwindow(window, GLUI_SUBWINDOW_RIGHT);			//< ウィンドウ内に作成するときはこっち
	guiLoadScene();
	DesignGUI();
	guiLoadInit();

	// GLUTおなじみの関数の登録(GLUI>GLUT>OpenGL)
	GLUI_Master.set_glutDisplayFunc(GLUT_Display);
	GLUI_Master.set_glutIdleFunc(GLUT_Idle);
	GLUI_Master.set_glutTimerFunc(simulationPeriod, GLUT_Timer, 0);
	GLUI_Master.set_glutKeyboardFunc(GLUT_Keyboard);
	GLUI_Master.set_glutReshapeFunc(GLUT_Reshape);
	
	glui->set_main_gfx_window(mainWindow);	

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//Springheadのrenderの登録
	grRender = grSdk->CreateDebugRender();				//GraphicSDKでデバッグ情報レンダラーを作成する
	grDevice = grSdk->CreateDeviceGL();					//OpenGL レンダラー用デバイスを作成する
	grDevice->Init();									//OpenGL レンダラーの初期化をする

	grRender->SetDevice(grDevice);						// 初期化をしたものとGLUTおなじみの関数をデバイスに設定
	
	//	カメラの設定
	GRCameraDesc cam;									//描画するためにはカメラが要るのでディスクリプタの宣言をする
	cam.size = Vec2d(0.05, 0);							//ディスクリプタにカメラの大きさを上書きする
	grRender->SetCamera(cam);							//カメラディスクリプタで設定した情報を登録する。
	
	initialize();										//カメラの使用環境（光源とか）の初期化

	//　視点の設定
	view.Pos() = Vec3f(6.0f*cos(Rad(camAngle)), 3.0f, 6.0f*sin(Rad(camAngle))) * camZoom;	//カメラの座標を指定する
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));								//カメラが見る座標, 上方向ベクトル 
	grRender->SetViewMatrix(view.inv());													//カメラ視点を登録する

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//　OpenGLのメインループ
	cout << "************************" << endl;
	cout << "* horse body simulator *" << endl;
	cout << "************************" << endl;
	cout << endl;
	cout << "< Ver 0.7 beta > Last update: 2008.01.10 Toki" << endl;
	cout << "This program is an easy sample of GLUI." << endl;
	glutMainLoop();													//GLUTのループを回す

	return 0;
}
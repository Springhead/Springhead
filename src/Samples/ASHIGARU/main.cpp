/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/Samples/Vehicles/main.cpp

【概要】Springhead2の総合的なデモ第１段．8足ロボットの歩行
 
【仕様】

*/

#include <Springhead.h>		//	Springheadのインタフェース
#include <GL/glut.h>
#include "robot.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC		27

UTRef<PHSdkIf> phSdk;			// SDK
UTRef<GRSdkIf> grSdk;
UTRef<PHSceneIf> scene;			// Scene
UTRef<GRDebugRenderIf> render;
UTRef<GRDeviceGLIf> device;

Robot robot;

void CreateFloor(){
	PHSolidDesc sd;
	PHSolidIf* soFloor = scene->CreateSolid(sd);
	CDBoxDesc bd;
	bd.boxsize = Vec3f(30.0f, 10.0f, 20.0f);
	soFloor->AddShape(phSdk->CreateShape(bd));
	soFloor->SetFramePosition(Vec3f(0,-5, 0));
	soFloor->SetDynamical(false);					// 床は外力によって動かないようにする
}

/**
 brief     	glutDisplayFuncで指定するコールバック関数．画面の描画を行う．
 param	 	なし
 return 	なし
 */
void Display(){
	Affinef af;
	af.Pos() = Vec3f(0, 3, 4)*1.0;
	af.LookAtGL(Vec3f(0,0,0), Vec3f(0,100,0));
	render->SetViewMatrix(af.inv());	//	視点の設定
	
	render->ClearBuffer();
	render->DrawScene(scene);
	render->EndScene();
	glutSwapBuffers();
}

/**
 brief     	光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 1.0);
	render->PushLight(light0);
	render->PushLight(light1);
}

/**
 brief     	初期化処理
 param	 	なし
 return 	なし
 */
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	setLight();
}

/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		なし
 */
void Reshape(int w, int h){
	// Viewportと射影行列を設定
	render->Reshape(Vec2f(), Vec2f(w,h));
}

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void Keyboard(unsigned char key, int x, int y){
	unsigned int i = 0;
	switch (key) {
	//終了
	case ESC:		
	case 'q':
		exit(0);
		break;
	case 'a':
		robot.Forward();
		break;
	case 's':
		robot.Backward();
		break;
	case 'd':
		robot.TurnLeft();
		break;
	case 'f':
		robot.TurnRight();
		break;
	case 'g':
		robot.Stop();
		break;
	default:
		break;
	}
}	

/**
 brief  	glutTimerFuncで指定したコールバック関数
 param	 	<in/--> id　　 タイマーの区別をするための情報
 return 	なし
 */
void timer(int id){
	glutTimerFunc(50, timer, 0);
	/// 時刻のチェックと画面の更新を行う
	for(int i = 0; i < 1; i++)
		scene->Step();
	glutPostRedisplay();
}

/**
 brief		メイン関数
 */
int main(int argc, char* argv[]){

	// SDKの作成　
	phSdk = PHSdkIf::CreateSdk();
	grSdk = GRSdkIf::CreateSdk();
	// シーンオブジェクトの作成
	PHSceneDesc dscene;
	dscene.timeStep = 0.05;
	dscene.numIteration = 5;
	scene = phSdk->CreateScene(dscene);			// シーンの作成
	
	// シーンの構築
	CreateFloor();								//	床
	Posed pose;
	pose.Pos() = Vec3d(0.0, 2.0, 0.0);
	robot.Build(pose, scene, phSdk);			//	ロボット
	//pose.Pos() = Vec3d(0.0, 1.0, 1.0);

	/*CDBoxDesc box;								//	三つ重なっている箱
	box.boxsize = Vec3f(1.0, 1.0, 2.0);
	CDBoxIf* boxBody = phSdk->CreateShape(box)->Cast();
	PHSolidDesc sd;
	sd.mass *= 0.7;
	sd.inertia *= 0.7;
	PHSolidIf* soBox;
	for(int i=0; i<3;++i){
		soBox = scene->CreateSolid(sd);
		soBox->AddShape(boxBody);
		soBox->SetPose(pose);
		pose.PosY()+=1.0;
		pose.PosX()-=0.1;
	}*/

	scene->SetGravity(Vec3f(0.0, -9.8, 0.0));	//	重力を設定
	
	//	GLUTの初期化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	int window = glutCreateWindow("ASHIGARU");

	render = grSdk->CreateDebugRender();
	device = grSdk->CreateDeviceGL();
	device->Init();
	render->SetDevice(device);	// デバイスの設定

	glutTimerFunc(50, timer, 0);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	
	initialize();

	glutMainLoop();
}

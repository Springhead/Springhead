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

#include <windows.h>	// Window関連
#include <winuser.h>
#include "resource1.h"
#include <stdio.h>
#include <math.h>
#include <Springhead.h>		//	Springheadのインタフェース
#include <GL/glut.h>
#include "robot.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC		27
#define module_max 3

UTRef<PHSdkIf> phSdk;			// SDK
UTRef<GRSdkIf> grSdk;
UTRef<PHSceneIf> scene;			// Scene
UTRef<GRDebugRenderIf> render;
UTRef<GRDeviceGLIf> device;

Robot robot[10];
//vector<Robot> robots;

double zoom = 0.2;

void CreateFloor(){
	PHSolidDesc sd;
	PHSolidIf* soFloor = scene->CreateSolid(sd);
	CDBoxDesc bd;
	bd.boxsize = Vec3f(30.0f, 10.0f, 20.0f);
	soFloor->AddShape(phSdk->CreateShape(bd));
	soFloor->SetFramePosition(Vec3f( 0, -5, 0));
	soFloor->SetDynamical(false);					// 床は外力によって動かないようにする
}

/**
 brief     	glutDisplayFuncで指定するコールバック関数．画面の描画を行う．
 param	 	なし
 return 	なし
 */
void Display(){
	Affinef af;
	af.Pos() = Vec3f(0, 3.5, 4)*zoom;
	af.LookAtGL(Vec3f(0.1,0,0.1), Vec3f(0,10,0));
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
	light0.position = Vec4f(20.0, 40.0, 40.0, 0.1);
	//light1.position = Vec4f(-20.0, 20.0, 20.0, 0.1);
	render->PushLight(light0);
	//render->PushLight(light1);
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
		exit(0);
		break;
	case 'q':
		exit(0);
		break;
	case 'a':
		for(int i=0; i<module_max; i++){robot[i].Forward();}
		break;
	case 'z':
		zoom -= 0.01;
		break;
	case 'x':
		zoom += 0.01;
		break;
	case 's':
		for(int i=0; i<module_max; i++){robot[i].Backward();}
		break;
	/*case 'd':
		robot.TurnLeft();
		break;
	case 'f':
		robot.TurnRight();
		break;*/
	case 'd':
		for(int i=0; i<module_max; i++){robot[i].Stop();}
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
	glutTimerFunc(100, timer, 0);
	/// 時刻のチェックと画面の更新を行う
	for(int i = 0; i < 1; i++)
		scene->Step();
	glutPostRedisplay();
}

/**
 brief		メイン関数
 */
int main(int argc, char* argv[]){

	PHHingeJointDesc Connect;
	//PHTreeNodeIf* node_connect;
	//PHRootNodeIf* root_connect = scene->CreateRootNode(robot[0].soBody);

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
	for(int i=0; i<module_max; i++){
			pose.Pos() = Vec3d(0.3*i, 0.2, 0.0);
			//pose.Pos() = Vec3d(0.0, 0.2, 0.0);
			robot[i].Build(pose, scene, phSdk);			//	ロボット構築
		}

	//３モジュール直列結合/////////////////////////////////////////////////////////////////////////

	//robot[0].leg[0].jntDX1  -> SetSpringOrigin(Rad(-90.0));//結合脚(leg[0])の姿勢を設定
	//robot[0].leg[0].jntDX1  -> SetSpring(1000);
	//robot[0].leg[0].jntDX2  -> SetSpringOrigin(Rad(-90.0));
	//robot[0].leg[0].jntDX2  -> SetSpring(1000);
	//robot[0].leg[0].jntFoot -> SetSpringOrigin(Rad(180.0));
	//robot[0].leg[0].jntFoot -> SetSpring(1000);
	robot[1].leg[0].jntDX1  -> SetSpringOrigin(Rad(-90.0));
	robot[1].leg[0].jntDX1  -> SetSpring(1000);
	robot[1].leg[0].jntDX2  -> SetSpringOrigin(Rad(-90.0));
	robot[1].leg[0].jntDX2  -> SetSpring(1000);
	robot[1].leg[0].jntFoot -> SetSpringOrigin(Rad(180.0));
	robot[1].leg[0].jntFoot -> SetSpring(1000);
	robot[2].leg[0].jntDX1  -> SetSpringOrigin(Rad(-90.0));
	robot[2].leg[0].jntDX1  -> SetSpring(1000);
	robot[2].leg[0].jntDX2  -> SetSpringOrigin(Rad(-90.0));
	robot[2].leg[0].jntDX2  -> SetSpring(1000);
	robot[2].leg[0].jntFoot -> SetSpringOrigin(Rad(180.0));
	robot[2].leg[0].jntFoot -> SetSpring(1000);

	Connect.poseSocket.Ori() = Quaterniond::Rot(Rad(60.0), 'y');//結合部分①構築
	Connect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	Connect.posePlug.Pos() = Vec3d(0.03, 0.025, 0.0);
	robot[1].leg[0].jntConnect[0] = scene->CreateJoint(robot[0].soBody, robot[1].leg[0].soDX2, Connect)->Cast();
	robot[1].leg[0].jntConnect[0]->SetSpring(100);
	robot[1].leg[0].jntConnect[0]->SetDamper(10);
	robot[1].leg[0].jntConnect[0]->SetSpringOrigin(Rad(180.0));
	//node_connect = scene->CreateTreeNode(root_connect, robot[1].soBody);

	Connect.poseSocket.Ori() = Quaterniond::Rot(Rad(60.0), 'y');//結合部分②構築
	Connect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	Connect.posePlug.Pos() = Vec3d(0.03, 0.025, 0.0);
	robot[2].leg[0].jntConnect[0] = scene->CreateJoint(robot[1].soBody, robot[2].leg[0].soDX2, Connect)->Cast();
	//scene->CreateTreeNode(node_connect, robot[2].leg[0].soDX2);
	robot[2].leg[0].jntConnect[0]->SetSpring(100);
	robot[2].leg[0].jntConnect[0]->SetDamper(10);
	robot[2].leg[0].jntConnect[0]->SetSpringOrigin(Rad(180.0));

	///////////////////////////////////////////////////////////////////////////////////////////////

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

	glutTimerFunc(100, timer, 0);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	
	initialize();

	glutMainLoop();
}

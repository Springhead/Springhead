/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/Samples/Joints/main.cpp

【概要】関節機能のデモ
*/

#include <ctime>
#include <string>
#include <GL/glut.h>
#include <sstream>
#include <windows.h>

#include <Springhead.h>		//	Springheadのインタフェース

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC		27

PHSdkIf* phSdk;			// SDKインタフェース
GRSdkIf* grSdk;
PHSceneIf* scene;		// Sceneインタフェース
GRDebugRenderIf* render;
GRDeviceGLIf* device;

double simulationPeriod = 32.0;
int sceneNo;			// シーン番号

PHSolidDesc descFloor;					//床剛体のディスクリプタ
PHSolidDesc descBox;					//箱剛体のディスクリプタ
//CDConvexMeshIf* meshBox;				//箱形状のインタフェース
CDShapeIf* shapeBox;
CDShapeIf* shapeSphere;

std::vector<PHSolidIf*> soBox;			//箱剛体のインタフェース
std::vector<PHJointIf*> jntLink;		//関節のインタフェース

std::vector<PHSolidIf*> soTarget;		


/** 実験用変数 **/
const double dt = 0.05	;					//積分幅
const int niter = 200;					//LCPはんぷくかいすう

bool bTimer = true;			//	タイマーでシミュレーションを進めるかどうか


void step(){
	scene->ClearForce();
	scene->GenerateForce();
	scene->Integrate();
	glutPostRedisplay();
}
void timer(int id){
	if (!bTimer) return;
	glutTimerFunc(simulationPeriod, timer, 0);
	step();
}

// シーン0 : 鎖のデモ。space keyで箱が増える
void BuildScene(){	
	soBox.clear();
	jntLink.clear();
	//	腰
	CDBoxDesc bd;
	bd.boxsize = Vec3f(0.19, 0.077, 0.137);
	CDShapeIf* shape = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);
	//空中に固定する
	soBox.back()->SetFramePosition(Vec3f(0.0, 0.0, 0.0));
	soBox.back()->SetDynamical(false);

	//	腹
	bd.boxsize = Vec3f(0.15, 0.08, 0.108);
	shape = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);

	//	ジョイント
	PHHingeJointDesc jd;
	jd.poseSocket.Pos() = Vec3d(0,0.04, 0);
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
	jd.posePlug.Pos() = Vec3d(0, -0.04, 0);
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'x');
	jd.damper = 60.0;
	jd.spring = 160.0;
	jntLink.push_back(scene->CreateJoint(soBox[1], soBox[0], jd));

	//	胸
	bd.boxsize = Vec3f(0.18, 0.2, 0.1);
	shape = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);

	//	ジョイント
	jd = PHHingeJointDesc();
	jd.poseSocket.Pos() = Vec3d(0, 0.04, 0);
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.posePlug.Pos() = Vec3d(0, -0.1, 0);
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.damper = 60.0;
	jd.spring = 160.0;
	jntLink.push_back(scene->CreateJoint(soBox[2], soBox[1], jd));

	//	継ぎ
	soBox.push_back(scene->CreateSolid(descBox));

	//	肩Z
	jd = PHHingeJointDesc();
	jd.poseSocket.Pos() = Vec3d(0.13, 0.1, 0);
	jd.posePlug.Pos() = Vec3d(0, 0, 0);
	jd.damper = 6.0;
	jd.spring = 10.0;
	jd.origin = Rad(-30);
	jntLink.push_back(scene->CreateJoint(soBox[3], soBox[2], jd));

	//	継ぎ
	soBox.push_back(scene->CreateSolid(descBox));

	//	肩Y
	jd = PHHingeJointDesc();
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'z');
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'z');
	jd.damper = 6.0;
	jd.spring = 150.0;
	jntLink.push_back(scene->CreateJoint(soBox[4], soBox[3], jd));

	//	右上腕
	bd.boxsize = Vec3f(0.049, 0.16, 0.048);
	shape = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);

	//	肩X
	jd = PHHingeJointDesc();
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.posePlug.Pos() = Vec3d(0, 0.09, 0);
	jd.damper = 6.0;
	jd.spring = 10.0;
	jd.origin = Rad(-30);
	jntLink.push_back(scene->CreateJoint(soBox[5], soBox[4], jd));

	//	右前腕
	bd.boxsize = Vec3f(0.046, 0.12, 0.046);
	shape = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);
	//	ひじ
	jd = PHHingeJointDesc();
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.poseSocket.Pos() = Vec3d(0, -0.09, 0);
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.posePlug.Pos() = Vec3d(0, 0.07, 0);
	jd.damper = 4.0;
	jd.spring = 6.0;
	jd.origin = Rad(-90);
	jntLink.push_back(scene->CreateJoint(soBox[6], soBox[5], jd));

	//	頭
	CDSphereDesc sd;
	sd.radius = 0.06;
	shape = phSdk->CreateShape(sd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);
	//	首
	jd = PHHingeJointDesc();
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.poseSocket.Pos() = Vec3d(0, 0.1, 0);
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.posePlug.Pos() = Vec3d(0, -0.08, 0);
	jd.damper = 3.0;
	jd.spring = 3.0;
	jd.origin = Rad(0);
	jntLink.push_back(scene->CreateJoint(soBox[7], soBox[2], jd));

	//	目標位置
	sd.radius = 0.02;
	shape = phSdk->CreateShape(sd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);
	soBox.back()->SetDynamical(false);
	soBox.back()->SetFramePosition(Vec3f(0.2, 0.2, -0.2));
	soTarget.push_back(soBox.back());


	
	//	左手

	//	継ぎ
	soBox.push_back(scene->CreateSolid(descBox));

	//	肩Z
	jd = PHHingeJointDesc();
	jd.poseSocket.Pos() = Vec3d(-0.13, 0.1, 0);
	jd.posePlug.Pos() = Vec3d(0, 0, 0);
	jd.damper = 6.0;
	jd.spring = 10.0;
	jd.origin = Rad(30);
	jntLink.push_back(scene->CreateJoint(soBox[9], soBox[2], jd));

	//	継ぎ
	soBox.push_back(scene->CreateSolid(descBox));

	//	肩Y
	jd = PHHingeJointDesc();
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'z');
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'z');
	jd.damper = 6.0;
	jd.spring = 150.0;
	jntLink.push_back(scene->CreateJoint(soBox[10], soBox[9], jd));

	//	右上腕
	bd.boxsize = Vec3f(0.049, 0.16, 0.048);
	shape = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);

	//	肩X
	jd = PHHingeJointDesc();
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.posePlug.Pos() = Vec3d(0, 0.09, 0);
	jd.damper = 6.0;
	jd.spring = 10.0;
	jd.origin = Rad(-30);
	jntLink.push_back(scene->CreateJoint(soBox[11], soBox[10], jd));

	//	右前腕
	bd.boxsize = Vec3f(0.046, 0.12, 0.046);
	shape = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);
	//	ひじ
	jd = PHHingeJointDesc();
	jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.poseSocket.Pos() = Vec3d(0, -0.09, 0);
	jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
	jd.posePlug.Pos() = Vec3d(0, 0.07, 0);
	jd.damper = 4.0;
	jd.spring = 6.0;
	jd.origin = Rad(-90);
	jntLink.push_back(scene->CreateJoint(soBox[12], soBox[11], jd));

	//	目標位置
	shape = phSdk->CreateShape(sd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shape);
	soBox.back()->SetDynamical(false);
	soBox.back()->SetFramePosition(Vec3f(-0.2, 0.2, -0.2));
	soTarget.push_back(soBox.back());

	//	バネ
	PHSpringDesc spd;
	spd.damper = Vec3f(1,1,1) * 1000;
	spd.spring = Vec3f(1,1,1) * 3000;
	spd.poseSocket.Pos() = Vec3d(0, -0.09, 0);
	jntLink.push_back(scene->CreateJoint(soBox[8], soBox[6], spd));
	//	バネ
	spd.poseSocket.Pos() = Vec3d(0, -0.09, 0);
	jntLink.push_back(scene->CreateJoint(soBox[13], soBox[12], spd));



	// 重力を設定
	scene->SetGravity(Vec3f(0, -9.8, 0));
	//	接触を切る
	scene->SetContactMode(&soTarget[0], PHSceneDesc::MODE_NONE);
	scene->SetContactMode(&soTarget[1], PHSceneDesc::MODE_NONE);
}


void OnKey(char key){
	switch(key){
	case 'z': soTarget[0]->SetFramePosition(Vec3f(0.0, 0.2, 0.0)); break;
	case 'x': soTarget[0]->SetFramePosition(Vec3f(0.1, 0.2, 0.0)); break;
	case 'c': soTarget[0]->SetFramePosition(Vec3f(0.2, 0.2, 0.0)); break;
	case 'v': soTarget[0]->SetFramePosition(Vec3f(0.3, 0.2, 0.0)); break;

	case 'a': soTarget[0]->SetFramePosition(Vec3f(0.0, 0.2, -0.2)); break;
	case 's': soTarget[0]->SetFramePosition(Vec3f(0.1, 0.2, -0.2)); break;
	case 'd': soTarget[0]->SetFramePosition(Vec3f(0.2, 0.2, -0.2)); break;
	case 'f': soTarget[0]->SetFramePosition(Vec3f(0.3, 0.2, -0.2)); break;

	case 'q': soTarget[0]->SetFramePosition(Vec3f(0.0, 0.3, -0.4)); break;
	case 'w': soTarget[0]->SetFramePosition(Vec3f(0.1, 0.3, -0.4)); break;
	case 'e': soTarget[0]->SetFramePosition(Vec3f(0.2, 0.3, -0.4)); break;
	case 'r': soTarget[0]->SetFramePosition(Vec3f(0.3, 0.3, -0.6)); break;


	case '.': soTarget[1]->SetFramePosition(Vec3f(-0.0, 0.2, 0.0)); break;
	case ',': soTarget[1]->SetFramePosition(Vec3f(-0.1, 0.2, 0.0)); break;
	case 'm': soTarget[1]->SetFramePosition(Vec3f(-0.2, 0.2, 0.0)); break;
	case 'n': soTarget[1]->SetFramePosition(Vec3f(-0.3, 0.2, 0.0)); break;

	case 'l': soTarget[1]->SetFramePosition(Vec3f(-0.0, 0.2, -0.2)); break;
	case 'k': soTarget[1]->SetFramePosition(Vec3f(-0.1, 0.2, -0.2)); break;
	case 'j': soTarget[1]->SetFramePosition(Vec3f(-0.2, 0.2, -0.2)); break;
	case 'h': soTarget[1]->SetFramePosition(Vec3f(-0.3, 0.2, -0.2)); break;

	case 'o': soTarget[1]->SetFramePosition(Vec3f(-0.0, 0.3, -0.4)); break;
	case 'i': soTarget[1]->SetFramePosition(Vec3f(-0.1, 0.3, -0.4)); break;
	case 'u': soTarget[1]->SetFramePosition(Vec3f(-0.2, 0.3, -0.4)); break;
	case 'y': soTarget[1]->SetFramePosition(Vec3f(-0.3, 0.3, -0.6)); break;

	case 'p': 
		bTimer = false;
		step();
		break;
	case 'P': 
		bTimer = true;
		break;
	}
}

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);
	render->EndScene();
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	setLight();
}

/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		なし
 */
void reshape(int w, int h){
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
void keyboard(unsigned char key, int x, int y){
	unsigned int i = 0;
	switch (key) {
		//終了
		case ESC:		
			exit(0);
			break;
		default:
			OnKey(key);
			break;
	}
}	


/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){
	// SDKの作成　
	phSdk = PHSdkIf::CreateSdk();
	grSdk = GRSdkIf::CreateSdk();
	// シーンオブジェクトの作成
	PHSceneDesc dscene;
	dscene.timeStep = dt;
	dscene.numIteration = niter;
	scene = phSdk->CreateScene(dscene);				// シーンの作成
	BuildScene();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	int window = glutCreateWindow("Joints");

	render = grSdk->CreateDebugRender();
	device = grSdk->CreateDeviceGL(window);

	// 初期設定
	device->Init();

	glutTimerFunc(20, timer, 0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(idle);
	
	render->SetDevice(device);	// デバイスの設定
	//	カメラの設定
	GRCameraDesc cam;
	cam.size = Vec2d(0.05, 0);
	render->SetCamera(cam);	
	initialize();

	// 視点の設定
	Affinef view;
	view.Pos() = Vec3f(1.0, 0.8, -2.1);								// 目の位置（観察用）
//	view.Pos() = Vec3f(0.0, 3.0,  0.1);								// （確認用）
	view.LookAtGL(Vec3f(0.0, 0.3, 0.0), Vec3f(0.0, 1.0, 0.0));		// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);

	glutMainLoop();
}

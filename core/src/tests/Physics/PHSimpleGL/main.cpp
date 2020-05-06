/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** 
 Springhead2/src/tests/Physics/PHSimpleGL/main.cpp
 
【概要】
  2つの剛体の位置の変化を確認するテストプログラム（位置を出力、GL表示）
  ・剛体の運動を確認する。
  ・位置を出力し、OpenGLでシミュレーションを行う。
 
【終了基準】
  ・赤いティーポットと青いティーポットの10秒後の位置をそれぞれ計算し期待値とする。
  　この期待値とシミュレーション結果を比較して、一致したら正常終了(success)とする。
  
【処理の流れ】
  ・シミュレーションに必要な情報(質量・重心・慣性テンソルなど)を設定する。
    剛体の形状はOpenGLでティーポットを指定する。
  ・シミュレーションが動き始めたら、下記条件で剛体に力を加える。
      左の赤いティーポット：オブジェクトのローカル座標系を原点とし、重心の1m上を右に1Nの力を加える。
      右の青いティーポット：オブジェクトのローカル座標系の原点から1m右に重心をずらし、重心の1m上を右に1Nの力を加える。
  ・与えられた条件により⊿t秒後の位置の変化を積分し、剛体の位置情報を出力と、OpenGLレンダリングを行う。
   
 */
#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#include <GL/glut.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC		27

UTRef<PHSdkIf> sdk;
PHSceneIf* scene;
PHSolidIf* redTeapot, *blueTeapot;	// Solidタイプ
// 光源の設定 
static GLfloat light_position[] = { 15.0, 30.0, 20.0, 1.0 };
static GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 }; 
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// 材質の設定
static GLfloat mat_red[]        = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat mat_blue[]       = { 0.0, 0.0, 1.0, 1.0 };
static GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat mat_shininess[]  = { 120.0 };

static double stepCnt = 0.0;
namespace {
	Vec3d redVel = Vec3d(0.0, 0.0, 0.0);	// 速度
	Vec3d redPos = Vec3d(0.0, 0.0, 0.0);	// 位置
	Vec3d blueVel = Vec3d(0.0, 0.0, 0.0);				
	Vec3d bluePos = Vec3d(0.0, 0.0, 0.0);				
}

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void SPR_CDECL display(){

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// 左の赤いティーポット
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);

	Affined ad;
	glPushMatrix();
	Posed pose = redTeapot->GetPose();
	pose.ToAffine(ad);
	//	redTeapot->GetOrientation().to_matrix(af);
	//	af.Pos() = redTeapot->GetFramePosition();
	// poseでは精度はdoubleなのでmatrixdにする
	glMultMatrixd(ad);
	glutSolidTeapot(1.0);
	glPopMatrix();

	// 右の青いティーポット
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blue);

	glPushMatrix();
	pose = blueTeapot->GetPose();
	ad = Affined(pose);
	glMultMatrixd(ad);
	glutSolidTeapot(1.0);
	glPopMatrix();

	glutSwapBuffers();
}

/**
 brief     	光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);
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

	gluLookAt(4.0, 3.0, 10.0, 
		      4.0, 0.0, 0.0,
		 	  0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	setLight();
}

/**
 brief  	glutReshapeFuncで指定したコールバック関数
 param	 	<in/--> w　　幅
 param  	<in/--> h　　高さ
 return 	なし
 */
void SPR_CDECL reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)(w/h), 1.0, 500);
	glMatrixMode(GL_MODELVIEW);
}

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void SPR_CDECL keyboard(unsigned char key, int x, int y){
	if (key == ESC) exit(0);
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void SPR_CDECL idle(){
	//	剛体の重心の1m上を右に押す．
	Vec3d force = Vec3d(1, 0, 0);
	redTeapot->AddForce( force, Vec3f(0,1,0)+redTeapot->GetCenterPosition());
	blueTeapot->AddForce( force, Vec3f(0,1,0)+blueTeapot->GetCenterPosition());

	scene->Step();
	stepCnt += scene->GetTimeStep();	// 微小時間[msec]
	double dt = scene->GetTimeStep();

	// 速度を導き、微少時間経過後の位置を計算（期待値)
	redVel		+= force * redTeapot->GetMassInv() * dt;	// (force/m)*dt
	redPos		+= redVel * dt;
	blueVel		+= force * blueTeapot->GetMassInv() * dt;	// (force/m)*dt
	bluePos		+= blueVel * dt;

	DSTR << redTeapot->GetCenterPosition() << blueTeapot->GetCenterPosition() << std::endl;
	
	// 10secにシミュレーション結果と期待値を比較
	if (stepCnt >= 10.0) {	
		bool redApprox, blueApprox;
		redApprox	= approx(redPos, redTeapot->GetFramePosition());
		blueApprox	= approx(bluePos, blueTeapot->GetFramePosition());

		if (redApprox && blueApprox) {	// シミュレーション結果は正しい結果となりました。
			std::cout << "\nPHSimpleGL success (redTeapot:success, blueTeapot:success)" << std::endl;
			exit(EXIT_SUCCESS);
		} else if (redApprox) {			// 赤いティーポットは正しい結果が得られましたが、青いティーポットは正しい結果が得られませんでした。
			std::cout << "\nPHSimpleGL failure (redTeapot:success, blueTeapot:failure)" << std::endl;
			exit(EXIT_FAILURE);
		} else if (blueApprox) {		// 青いティーポットは正しい結果が得られましたが、赤いティーポットは正しい結果が得られませんでした。		
			std::cout << "\nPHSimpleGL failure (redTeapot:failure, blueTeapot:success)" << std::endl;
			exit(EXIT_FAILURE);
		} else {						// 青いティーポットと赤いティーポットはともに正しい結果が得られませんでした。
			std::cout << "\nPHSimpleGL failure (redTeapot:failure, blueTeapot:failure)" << std::endl;
			exit(EXIT_FAILURE);
		}
	} 
	
	glutPostRedisplay();
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int SPR_CDECL main(int argc, char* argv[]){
	sdk = PHSdkIf::CreateSdk();					//	SDKの作成
	scene = sdk->CreateScene();				//	シーンの作成
	
	PHSolidDesc desc;						// 左のteapot
	desc.mass = 2.0;						// 質量	
	desc.inertia *=2.0;						// 慣性テンソル
	desc.center = Vec3f(0,0,0);				// 質量中心の位置
	redTeapot = scene->CreateSolid(desc);	// 剛体をdescに基づいて作成
	redTeapot->SetGravity(false);			// 重力を無効にする
	redPos = redTeapot->GetFramePosition();

	desc.center = Vec3f(1,0,0);			//	重心の位置をSolidの原点から1m右にずらす．
	desc.pose.Pos() = Vec3f(3.5, 0.0, 0.0);
	//desc.pose = desc.pose * Posed::Trn(0.0, 0.0, 3.0);	// 手前に平行移動	
	Vec3d trn = Vec3d(0.0, 0.0, 3.0);
	desc.pose = desc.pose * Posed::Trn(trn);				// 手前に平行移動
	blueTeapot = scene->CreateSolid(desc);	
	blueTeapot->SetGravity(false);
	bluePos = blueTeapot->GetFramePosition();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("PHSimpleGL");

	initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
	
	return 0;
}


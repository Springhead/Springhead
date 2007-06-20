/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/*
 Springhead2/src/Samples/ASHIGARU/main.cpp

【概要】３脚モジュラーロボットASHIGARUの歩容生成シミュレータ
 
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
#include "myheader.h"	// 自作関数，構造体

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC		27
#define module_max 5

UTRef<PHSdkIf> phSdk;			// SDK
UTRef<GRSdkIf> grSdk;
UTRef<PHSceneIf> scene;			// Scene
UTRef<GRDebugRenderIf> render;
UTRef<GRDeviceGLIf> device;

Robot robot[10];//とりあえずASHIGARU最大１０体

double zoom = 0.2;
double shift_LR = 0.1;
double shift_UD = 0.1;

double para_X1 = 0.11, para_a1 = 0.05, para_b1 = 0.07, para_H1 = 0.17;
double	L1 = 0.067, L2 = 0.067, L3 = 0.12;	// 関節間長さ

double			NowTime = 0.0;	// 時間
double			s_time = 0.0;					// サンプリングタイム初期化
double			CycleTime = 2.0;
double			Phase = 0.0;

bool stepOnTimer = false;

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
	af.LookAtGL(Vec3f(shift_LR+0.5,0,shift_UD), Vec3f(shift_LR,10,shift_UD));
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
		//robot[module_max-1].leg[0].jntDX2->SetSpringOrigin(Rad(80.0));
		//robot[module_max-1].leg[0].jntFoot->SetSpringOrigin(Rad(10.0));
		break;
	case 's':
		for(int i=0; i<module_max; i++){robot[i].Backward();}
		//robot[module_max-1].leg[0].jntDX2->SetSpringOrigin(Rad(-60.0));
		//robot[module_max-1].leg[0].jntFoot->SetSpringOrigin(Rad(150.0));
		break;
	case 'z':
		zoom -= 0.01;
		glutPostRedisplay();
		break;
	case 'x':
		zoom += 0.01;
		glutPostRedisplay();
		break;
	case 't':
		stepOnTimer = !stepOnTimer;
		break;
	case 'p':
		stepOnTimer = false;
		scene->Step();
		glutPostRedisplay();
		break;
	case 'c':
		shift_LR += 0.01;
		break;
	case 'v':
		shift_LR -= 0.01;
		break;
	case 'k':
		NowTime += 0.05;
		break;
	case 'm':
		NowTime -= 0.05;
		break;
	case 'd':
		for(int i=0; i<module_max; i++){robot[i].Stop();}
		break;
	case 'o':
		for(int i=0; i<module_max; i++){
			robot[i].soBody->SetDynamical(true);
		}
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

	static LARGE_INTEGER	freq, start, now, past;			// 周波数，開始時間，現在時間，前回時間

	// 時間計測準備
	start.QuadPart = now.QuadPart = past.QuadPart = 0;
	QueryPerformanceFrequency( &freq );	// 計算機の周波数計測

	glutTimerFunc(100, timer, 0);

	/// 時刻のチェックと画面の更新を行う
	if (stepOnTimer){
		for(int i = 0; i < 1; i++)
		scene->Step();

		// 位相生成
		/*QueryPerformanceCounter( &now );
		NowTime = (double)( now.QuadPart - start.QuadPart ) / (double)freq.QuadPart;	// 経過時間の計算
		s_time = (double)( now.QuadPart - past.QuadPart ) / (double)freq.QuadPart;
		past.QuadPart = now.QuadPart;*/	

		Phase = (2 * PI / CycleTime) * NowTime;
	
		for(int module_num=0; module_num<module_max; module_num++){
				for(int leg_num = 1; leg_num < 3; leg_num++){
				double	PhaseTemp;
				PhaseTemp = Phase + leg_num * 2 * PI / 2.0 + module_num * 2 * PI / 2.0;
				//PhaseTemp[module_num * 2 + leg_num]= Phase[module_num * 2 + leg_num];
				//PhaseTemp[module_num * 2 + leg_num]= Phase[module_num * 2 + leg_num] + leg_num * 2 * PI / 2.0 + module_num * 2 * PI / 2.0;
				//PhaseTemp[module_num * 2 + leg_num]= Phase[module_num * 2 + leg_num] + leg_num * 2 * PI / 2.0;

				// 足先軌道の計算（楕円軌道）
				double	xx = para_X1;
				//double	yy = pow(-1, leg_num) * para_a1 * cos(PhaseTemp[module_num * 2 + leg_num]);
				double	yy = pow(-1, leg_num) * para_a1 * cos(PhaseTemp);
				//double	yy = pow(-1, module_num) * para_a1 * cos(PhaseTemp[module_num * 2 + leg_num]);
				//double	yy = para_a1 * cos(PhaseTemp[module_num * 2 + leg_num]);
				double	zz;
					if(sin(PhaseTemp) < 0.0)	zz = - para_H1;
					//if(sin(PhaseTemp[module_num * 2 + leg_num]) < 0.0)	zz = - para_H1;
					else zz = para_b1 * sin(PhaseTemp) - para_H1;
					//else zz = para_b1 * sin(PhaseTemp[module_num * 2 + leg_num]) - para_H1;

				// 各関節角度の計算
				double	theta1 = CalcTheta1(xx, yy);
				if(leg_num)theta1 += pow(-1, leg_num) * PI / 6.0;
				double	theta2 = CalcTheta2(xx, yy, zz, L1, L2, L3);
				double	theta3 = CalcTheta3(xx, yy, zz, L1, L2, L3);

				// グラフィック表示のために各値をセット
				robot[module_num].leg[leg_num].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
				robot[module_num].leg[leg_num].jntDX2  ->  SetSpringOrigin(-theta2);
				robot[module_num].leg[leg_num].jntFoot ->  SetSpringOrigin(-theta3);
			}
		}

		glutPostRedisplay();
	}
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
	scene->SetGravity(Vec3f(0.0, -9.8, 0.0));	//	重力を設定
	// シーンの構築
	CreateFloor();								//	床

	Posed pose;
	pose.Ori() = Quaterniond::Rot(Rad(120.0), 'y');
	for(int i=0; i<module_max; i++){			//ASHIGARU構築
		pose.Pos() = Vec3d(0.3*i, 0.17, 0.0);
		robot[i].Build_root(pose, scene, phSdk);
	}


//////モジュール直列結合構築/////////////////////////////////////////////////////////////////////////

	PHHingeJointDesc jdConnect;

	for(int i=0; i<module_max; i++){
		robot[i].leg[1].jntDX1 -> SetSpringOrigin(Rad(-60.0));
		robot[i].leg[2].jntDX1 -> SetSpringOrigin(Rad(-120.0));
	}

	std::vector<PHSolidIf*> allSolids;
	for(int i=0; i<module_max; i++){
		allSolids.push_back(robot[i].soBody);
		for(int j=0; j<3; ++j){
			allSolids.push_back(robot[i].leg[j].soDX1);
			allSolids.push_back(robot[i].leg[j].soDX2);
			allSolids.push_back(robot[i].leg[j].soFoot);
		}
	}
	scene->SetContactMode(&allSolids[0], allSolids.size(), PHSceneDesc::MODE_NONE);

	double K = 10000.0, D = 0.0;
	for(int i=0; i<module_max; i++){
		robot[i].leg[0].jntDX1  -> SetSpringOrigin(Rad(-90.0));//結合脚(leg[0])の姿勢を設定
		robot[i].leg[0].jntDX1  -> SetSpring(K);
		robot[i].leg[0].jntDX1  -> SetDamper(D);
		robot[i].leg[0].jntDX2  -> SetSpringOrigin(Rad(-90.0));
		robot[i].leg[0].jntDX2  -> SetSpring(K);
		robot[i].leg[0].jntDX2  -> SetDamper(D);
		robot[i].leg[0].jntFoot -> SetSpringOrigin(Rad(180.0));
		robot[i].leg[0].jntFoot -> SetSpring(K);
		robot[i].leg[0].jntFoot -> SetDamper(D);
	}

	//結合部分構築
	jdConnect.poseSocket.Ori() = Quaterniond();
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	for(int i=0; i<module_max-1; i++){
		robot[i].leg[0].jntConnect[0] = scene->CreateJoint(robot[i+1].soBody, robot[i].soBody, jdConnect)->Cast();
		robot[i].leg[0].jntConnect[0]->SetSpring(K);
		robot[i].leg[0].jntConnect[0]->SetDamper(D);
		robot[i].leg[0].jntConnect[0]->SetSpringOrigin(Rad(0.0));
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

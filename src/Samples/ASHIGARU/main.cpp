/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/*
 Springhead2/src/Samples/ASHIGARU/main.cpp

【概要】３脚モジュラーロボットASHIGARUの歩行シミュレータ
 
【仕様】結合部分の設定及びモジュールの初期設定を変えることで「3モジュール直列」「3モジュール環状」「4モジュール放射状」に対応

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

//#include "JoyStick.h"		// JoyStickクラス
//JoyStick	myjs;			// JoyStickオブジェクト

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

FILE *fd;					//組合せ吐き出し用

using namespace Spr;

#define ESC		27
#define module_max 4

double K = 1000.0, D = 0.0;//結合部分をガッチガチに

UTRef<PHSdkIf> phSdk;			// SDK
UTRef<GRSdkIf> grSdk;
UTRef<PHSceneIf> scene;			// Scene
UTRef<GRDebugRenderIf> render;
UTRef<GRDeviceGLIf> device;

void GetTrajectory(int module_num, int leg_num, double para_a1, double para_b1, double para_X1, double para_H1, double Phase[], double direction);
void GetLegPosition(int module_num);

INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);	// メインダイアログ

// メインダイアログのハンドル
HWND		g_hDlgMain;		// ハンドル
HINSTANCE	g_hInstMain;	// インスタンス

Robot robot[50];//とりあえずASHIGARU最大１０体

double zoom = 0.2;

double para_X1 = 0.11, para_a1 = 0.03, para_b1 = 0.05, para_H1 = 0.17;
double	L1 = 0.067, L2 = 0.067, L3 = 0.125;	// 関節間長さ
double   direction_theta = 0.0;  //進行方向

double			NowTime = 0.0;	// 時間
double			s_time = 0.0;	// サンプリングタイム初期化
double			CycleTime = 1.2;
double			Phase = 0.0;
double			PhaseTemp[50];

bool stepOnTimer = false;
int turn_flag = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateFloor(){
	PHSolidDesc sd;
	PHSolidIf* soFloor = scene->CreateSolid(sd);
	CDBoxDesc bd;
	bd.material.mu = 5.0;
	bd.material.mu0 = 5.0;
	bd.boxsize = Vec3f(10.0f, 0.3f, 10.0f);
	soFloor->AddShape(phSdk->CreateShape(bd));
	soFloor->SetFramePosition(Vec3f( 0, -0.15, 0));
	soFloor->SetDynamical(false);					// 床は外力によって動かないようにする
}

/* brief     	glutDisplayFuncで指定するコールバック関数．画面の描画を行う．
   param	 	なし
   return 	    なし*/
void Display(){
	Affinef af;
	af.Pos() = Vec3f(0, 3.5, 4)*zoom;
	af.LookAtGL(Vec3f(0.6,0,0.1), Vec3f(0.1,10,0.1));
	render->SetViewMatrix(af.inv());	//	視点の設定
	
	render->ClearBuffer();
	render->DrawScene(scene);
	render->EndScene();
	glutSwapBuffers();
}

/*  brief     	光源の設定
   param	 	なし
   return 	    なし*/
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(20.0, 40.0, 40.0, 0.1);
	//light1.position = Vec4f(-20.0, 20.0, 20.0, 0.1);
	render->PushLight(light0);
	//render->PushLight(light1);
}

/* brief     	初期化処理
   param	 	なし
   return 	    なし*/
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	setLight();
}

/* brief		glutReshapeFuncで指定したコールバック関数
   param		<in/--> w　　幅
   param		<in/--> h　　高さ
   return		なし*/
void Reshape(int w, int h){
	// Viewportと射影行列を設定
	render->Reshape(Vec2f(), Vec2f(w,h));
}

/* brief 		glutKeyboardFuncで指定したコールバック関数 
  param		    <in/--> key　　 ASCIIコード
  param 		<in/--> x　　　 キーが押された時のマウス座標
  param 		<in/--> y　　　 キーが押された時のマウス座標
  return 	なし	*/
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
		NowTime += 0.03;
		//direction_theta += 0.008;
		break;
	case 's':
		NowTime -= 0.03;
		//direction_theta -= 0.008;
		break;
	case 'w':
		turn_flag++ ;
		break;
	case 'z':
		zoom -= 0.01;
		break;
	case 'x':
		zoom += 0.01;
		break;
	case 't':
		stepOnTimer = !stepOnTimer;
		break;
	case 'p':
		stepOnTimer = false;
		scene->Step();
		glutPostRedisplay();
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

/* brief  	    glutTimerFuncで指定したコールバック関数
   param	 	<in/--> id　　 タイマーの区別をするための情報
   return 	    なし */
void timer(int id){

	static LARGE_INTEGER	freq, start, now, past;			// 周波数，開始時間，現在時間，前回時間

	// 時間計測準備
	start.QuadPart = now.QuadPart = past.QuadPart = 0;
	QueryPerformanceFrequency( &freq );	// 計算機の周波数計測
	glutTimerFunc(100, timer, 0);
	direction_theta = NowTime/5;

	/// 時刻のチェックと画面の更新を行う
	if (stepOnTimer){
		
		scene->Step();

		//std::cout << robot[0].soBody->GetFramePosition();
		fprintf(fd,"%3f	%3f	%3f	%3f\n", NowTime , robot[0].soBody->GetFramePosition());

		// 位相生成
		QueryPerformanceCounter( &now );
		//NowTime = (double)( now.QuadPart - start.QuadPart ) / (double)freq.QuadPart;	// 経過時間の計算
		s_time = (double)( now.QuadPart - past.QuadPart ) / (double)freq.QuadPart;
		past.QuadPart = now.QuadPart;

		Phase = (2 * PI / CycleTime) * NowTime;
	
	//3モジュール直列結合の設定////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//robot[0].theta = 0.0;	robot[1].theta = 0.0;	robot[2].theta = 0.0;//モジュールの姿勢設定

		//robot[0].leg[0].Joint_flag = true;	robot[1].leg[0].Joint_flag = true;//結合脚指定

		//PhaseTemp[0] = 0.0;				PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase;//各脚の位相指定
		//PhaseTemp[3] = 0.0;				PhaseTemp[4] = Phase + PI;		PhaseTemp[5] = Phase + PI;
		//PhaseTemp[6] = Phase + PI;	PhaseTemp[7] = Phase;				PhaseTemp[8] = Phase;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//3モジュール環状結合の設定////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//robot[0].theta = 0.0;	robot[1].theta = 2*PI/3;	robot[2].theta = -2*PI/3;//モジュールの姿勢設定

		//robot[0].leg[0].Joint_flag = true;	robot[1].leg[0].Joint_flag = true;	robot[2].leg[0].Joint_flag = true;//結合脚指定

		//PhaseTemp[0] = 0.0;				PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase + PI;//各脚の位相指定
		//PhaseTemp[3] = 0.0;				PhaseTemp[4] = Phase;			PhaseTemp[5] = Phase + PI;
		//PhaseTemp[6] = 0.0;				PhaseTemp[7] = Phase;			PhaseTemp[8] = Phase + PI;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//4モジュール直列結合の設定////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//robot[0].theta = 0.0;	robot[1].theta = 0.0;	robot[2].theta = 0.0;	robot[3].theta = 0.0;//モジュールの姿勢設定

		//robot[0].leg[0].Joint_flag = true;	robot[1].leg[0].Joint_flag = true;	robot[2].leg[0].Joint_flag = true;//結合脚指定

		//PhaseTemp[0] = 0.0;				PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase;//各脚の位相指定(安定余裕のみ)
		//PhaseTemp[3] = 0.0;				PhaseTemp[4] = Phase + PI;		PhaseTemp[5] = Phase + PI;
		//PhaseTemp[6] = 0.0;				PhaseTemp[7] = Phase;			PhaseTemp[8] =Phase;
		//PhaseTemp[9] = Phase;			PhaseTemp[10] = Phase + PI;		PhaseTemp[11] = Phase + PI;

		//PhaseTemp[0] = 0.0;				PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase + PI;//各脚の位相指定(全部考慮)
		//PhaseTemp[3] = 0.0;				PhaseTemp[4] = Phase + PI;		PhaseTemp[5] = Phase;
		//PhaseTemp[6] = 0.0;				PhaseTemp[7] = Phase + PI;		PhaseTemp[8] = Phase + PI;
		//PhaseTemp[9] = Phase + PI;		PhaseTemp[10] = Phase;			PhaseTemp[11] = Phase;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//4モジュールU型結合の設定////////////////////////////////////////////////////////////////////////////////////////////////////////////

		robot[0].theta = 0.0;	robot[1].theta = 0.0;	robot[2].theta = 0.0;	robot[3].theta = 0.0;//モジュールの姿勢設定

		robot[0].leg[0].Joint_flag = true;	robot[2].leg[1].Joint_flag = true;	robot[2].leg[2].Joint_flag = true;//結合脚指定
		robot[1].leg[2].Joint_flag = true;

		PhaseTemp[0] = 0.0;				PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase + PI;//各脚の位相指定(安定余裕のみ)
		PhaseTemp[3] = Phase;			PhaseTemp[4] = Phase + PI;		PhaseTemp[5] = 0.0;
		PhaseTemp[6] = Phase + PI;		PhaseTemp[7] = 0.0;				PhaseTemp[8] =0.0;
		PhaseTemp[9] = Phase;			PhaseTemp[10] = Phase + PI;		PhaseTemp[11] = Phase;

		//PhaseTemp[0] = 0.0;				PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase + PI;//各脚の位相指定(全部考慮)
		//PhaseTemp[3] = Phase;			PhaseTemp[4] = Phase + PI;		PhaseTemp[5] = 0.0;
		//PhaseTemp[6] = Phase + PI;		PhaseTemp[7] = 0.0;				PhaseTemp[8] = 0.0;
		//PhaseTemp[9] = Phase;			PhaseTemp[10] = Phase;			PhaseTemp[11] = Phase + PI;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//4モジュール放射状結合の設定////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//robot[0].theta = 0.0;	robot[1].theta = 0.0;	robot[2].theta = 0.0;	robot[3].theta = 0.0;//モジュールの姿勢設定

		//robot[0].leg[0].Joint_flag = true;	robot[2].leg[1].Joint_flag = true;	robot[3].leg[2].Joint_flag = true;//結合脚指定
		////robot[1].leg[0].Joint_flag = true;	robot[1].leg[1].Joint_flag = true;	robot[1].leg[2].Joint_flag = true;//結合脚指定

		//PhaseTemp[0] = 0.0;					PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase + PI;//各脚の位相指定(安定余裕のみ)
		//PhaseTemp[3] = Phase + PI;			PhaseTemp[4] = Phase;			PhaseTemp[5] = Phase + PI;
		////PhaseTemp[3] = 0.0;					PhaseTemp[4] = 0.0;				PhaseTemp[5] = 0.0;
		//PhaseTemp[6] = Phase + PI;			PhaseTemp[7] = 0.0;				PhaseTemp[8] = Phase;
		//PhaseTemp[9] = Phase;				PhaseTemp[10] = Phase + PI;		PhaseTemp[11] = 0.0;

		//PhaseTemp[0] = 0.0;					PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase + PI;//各脚の位相指定(全部考慮)
		//PhaseTemp[3] = Phase + PI;			PhaseTemp[4] = Phase + PI;			PhaseTemp[5] = Phase;
		////PhaseTemp[3] = 0.0;					PhaseTemp[4] = 0.0;				PhaseTemp[5] = 0.0;
		//PhaseTemp[6] = Phase;			PhaseTemp[7] = 0.0;				PhaseTemp[8] = Phase + PI;
		//PhaseTemp[9] = Phase + PI;				PhaseTemp[10] = Phase;		PhaseTemp[11] = 0.0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//6モジュール不定形の設定////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//robot[0].theta = 0.0;	robot[1].theta = 0.0;	robot[2].theta = 0.0;//モジュールの姿勢設定
		//robot[3].theta = 0.0;	robot[4].theta = 0.0;	robot[5].theta = 0.0;

		//robot[1].leg[0].Joint_flag = true;	robot[1].leg[2].Joint_flag = true;	robot[3].leg[1].Joint_flag = true;//結合脚指定
		//robot[3].leg[2].Joint_flag = true;	robot[4].leg[2].Joint_flag = true;

		//PhaseTemp[0] = Phase;			PhaseTemp[1] = Phase;			PhaseTemp[2] = Phase + PI;//各脚の位相指定(安定余裕＋面積＋分散も考慮したときの最適解)
		//PhaseTemp[3] = 0.0;				PhaseTemp[4] = Phase + PI;		PhaseTemp[5] = 0.0;
		//PhaseTemp[6] = Phase;			PhaseTemp[7] = Phase + PI;		PhaseTemp[8] = Phase;
		//PhaseTemp[9] = Phase + PI;		PhaseTemp[10] = 0.0;			PhaseTemp[11] = 0.0;
		//PhaseTemp[12] = Phase + PI;		PhaseTemp[13] = Phase;			PhaseTemp[14] = 0.0;
		//PhaseTemp[15] = Phase;			PhaseTemp[16] = Phase;			PhaseTemp[17] = Phase + PI;

		//PhaseTemp[0] = Phase;			PhaseTemp[1] = Phase + PI;		PhaseTemp[2] = Phase;//各脚の位相指定(ローテーション法の解)
		//PhaseTemp[3] = 0.0;				PhaseTemp[4] = Phase;			PhaseTemp[5] = 0.0;
		//PhaseTemp[6] = Phase;			PhaseTemp[7] = Phase + PI;		PhaseTemp[8] = Phase + PI;
		//PhaseTemp[9] = Phase;			PhaseTemp[10] = 0.0;			PhaseTemp[11] = 0.0;
		//PhaseTemp[12] = Phase + PI;		PhaseTemp[13] = Phase + PI;		PhaseTemp[14] = 0.0;
		//PhaseTemp[15] = Phase;			PhaseTemp[16] = Phase;			PhaseTemp[17] = Phase + PI;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//6モジュールｒの設定////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//robot[0].theta = 0.0;	robot[1].theta = 0.0;	robot[2].theta = 0.0;//モジュールの姿勢設定
		//robot[3].theta = 0.0;	robot[4].theta = 0.0;	robot[5].theta = 0.0;

		//robot[0].leg[2].Joint_flag = true;	robot[2].leg[1].Joint_flag = true;	robot[3].leg[1].Joint_flag = true;//結合脚指定
		//robot[4].leg[0].Joint_flag = true;	robot[5].leg[0].Joint_flag = true;

		//PhaseTemp[0] = Phase;			PhaseTemp[1] = Phase + PI;			PhaseTemp[2] = 0.0;//各脚の位相指定(安定余裕)
		//PhaseTemp[3] = Phase + PI;				PhaseTemp[4] = Phase;		PhaseTemp[5] = Phase;
		//PhaseTemp[6] = Phase;			PhaseTemp[7] = 0.0;		PhaseTemp[8] = Phase;
		//PhaseTemp[9] = Phase + PI;		PhaseTemp[10] = 0.0;			PhaseTemp[11] = Phase + PI;
		//PhaseTemp[12] = 0.0;		PhaseTemp[13] = Phase + PI;			PhaseTemp[14] = Phase;
		//PhaseTemp[15] = 0.0;			PhaseTemp[16] = Phase + PI;			PhaseTemp[17] = Phase + PI;

		//PhaseTemp[0] = Phase;			PhaseTemp[1] = Phase + PI;		PhaseTemp[2] = 0.0;//各脚の位相指定(全部考慮)
		//PhaseTemp[3] = Phase + PI;		PhaseTemp[4] = Phase + PI;			PhaseTemp[5] = Phase;
		//PhaseTemp[6] = Phase;			PhaseTemp[7] = 0.0;				PhaseTemp[8] = Phase;
		//PhaseTemp[9] = Phase + PI;		PhaseTemp[10] = 0.0;			PhaseTemp[11] = Phase;
		//PhaseTemp[12] = 0.0;			PhaseTemp[13] = Phase;			PhaseTemp[14] = Phase + PI;
		//PhaseTemp[15] = 0.0;			PhaseTemp[16] = Phase;			PhaseTemp[17] = Phase + PI;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//6モジュールブーメランの設定////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//robot[0].theta = 0.0;	robot[1].theta = 0.0;	robot[2].theta = 0.0;//モジュールの姿勢設定
		//robot[3].theta = 0.0;	robot[4].theta = 0.0;	robot[5].theta = 0.0;

		//robot[0].leg[2].Joint_flag = true;	robot[1].leg[2].Joint_flag = true;	robot[3].leg[1].Joint_flag = true;//結合脚指定
		//robot[4].leg[1].Joint_flag = true;	robot[5].leg[1].Joint_flag = true;
		
		//PhaseTemp[0] = Phase;			PhaseTemp[1] = Phase + PI;		PhaseTemp[2] = 0.0;//各脚の位相指定(安定余裕のみ)
		//PhaseTemp[3] = Phase + PI;		PhaseTemp[4] = Phase;			PhaseTemp[5] = 0.0;
		//PhaseTemp[6] = Phase + PI;		PhaseTemp[7] = Phase + PI;		PhaseTemp[8] = Phase;
		//PhaseTemp[9] = Phase ;			PhaseTemp[10] = 0.0;			PhaseTemp[11] = Phase + PI;
		//PhaseTemp[12] = Phase;			PhaseTemp[13] = 0.0;			PhaseTemp[14] = Phase;
		//PhaseTemp[15] = Phase + PI;		PhaseTemp[16] = 0.0;			PhaseTemp[17] = Phase;

		//PhaseTemp[0] = Phase;			PhaseTemp[1] = Phase + PI;		PhaseTemp[2] = 0.0;//各脚の位相指定(全部考慮)
		//PhaseTemp[3] = Phase + PI;		PhaseTemp[4] = Phase;			PhaseTemp[5] = 0.0;
		//PhaseTemp[6] = Phase + PI;		PhaseTemp[7] = Phase + PI;		PhaseTemp[8] = Phase;
		//PhaseTemp[9] = Phase ;			PhaseTemp[10] = 0.0;			PhaseTemp[11] = Phase;
		//PhaseTemp[12] = Phase;			PhaseTemp[13] = 0.0;			PhaseTemp[14] = Phase + PI;
		//PhaseTemp[15] = Phase + PI;		PhaseTemp[16] = 0.0;			PhaseTemp[17] = Phase;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//double K = 1000.0, D = 0.0;//結合部分をガッチガチに

		GetLegPosition(module_max);//各モジュールの姿勢情報を取得
		GetTrajectory(module_max, 3, para_a1, para_b1, para_X1, para_H1, PhaseTemp, direction_theta);//足先軌道の計算

		for(int i=0; i<module_max; i++){//結合脚の姿勢を設定
			for(int j=0; j<3; j++){
				if(robot[i].leg[j].Joint_flag == true){
					robot[i].leg[j].jntDX1  -> SetSpringOrigin(Rad(-90.0));robot[i].leg[j].jntDX1  -> SetSpring(K);robot[i].leg[j].jntDX1  -> SetDamper(D);
					robot[i].leg[j].jntDX2  -> SetSpringOrigin(Rad(-90.0));robot[i].leg[j].jntDX2  -> SetSpring(K);robot[i].leg[j].jntDX2  -> SetDamper(D);
					robot[i].leg[j].jntFoot -> SetSpringOrigin(Rad(180.0));robot[i].leg[j].jntFoot -> SetSpring(K);robot[i].leg[j].jntFoot -> SetDamper(D);
				}
			}
		}
		glutPostRedisplay();
	}
}

/* brief		メイン関数 */
int main(int argc, char* argv[]){

	// SDKの作成　
	phSdk = PHSdkIf::CreateSdk();
	grSdk = GRSdkIf::CreateSdk();

	if ((fd = fopen("data1.txt", "w")) < 0) {printf("Can't open %s", "data1.txt");exit(1);}//テキスト書き込みファイルオープン

	// シーンオブジェクトの作成
	PHSceneDesc dscene;
	dscene.timeStep = 0.05;
	dscene.numIteration = 5;
	scene = phSdk->CreateScene(dscene);			// シーンの作成
	scene->SetGravity(Vec3f(0.0, -9.8, 0.0));	//	重力を設定

	// シーンの構築
	CreateFloor();								//	床

	//ここで結合状態の設定を行う
	Posed pose;

	//初期描画位置/////////////////////////////////////////////////////////////////////////初期描画位置をこれにしないとエラるっぽい

	pose.Ori() = Quaterniond::Rot(Rad(120.0), 'y');
	for(int i=0; i<module_max; i++){			//ASHIGARU構築
		pose.Pos() = Vec3d(0.3*i, 0.17, 0.0);
		robot[i].Build_root(pose, scene, phSdk);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	PHHingeJointDesc jdConnect;

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

//////モジュール直列結合構築/////////////////////////////////////////////////////////////////////////

	//結合部分構築
	/*jdConnect.poseSocket.Ori() = Quaterniond();
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	for(int i=0; i<module_max-1; i++){
		robot[i].jntConnect = scene->CreateJoint(robot[i+1].soBody, robot[i].soBody, jdConnect)->Cast();
		robot[i].jntConnect->SetSpring(K);
		robot[i].jntConnect->SetDamper(D);
		robot[i].jntConnect->SetSpringOrigin(Rad(0.0));
	}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////３モジュール環状結合////////////////////////////////////////////////////////////////////////////////////////////

	//結合部分構築
	/*jdConnect.poseSocket.Ori() = Quaterniond::Rot(Rad(-120.0), 'y');
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	robot[0].jntConnect = scene->CreateJoint(robot[1].soBody, robot[0].soBody, jdConnect)->Cast();
	robot[0].jntConnect->SetSpring(K);
	robot[0].jntConnect->SetDamper(D);
	robot[0].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Ori() = Quaterniond::Rot(Rad(-120.0), 'y');
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	robot[1].jntConnect = scene->CreateJoint(robot[2].soBody, robot[1].soBody, jdConnect)->Cast();
	robot[1].jntConnect->SetSpring(K);
	robot[1].jntConnect->SetDamper(D);
	robot[1].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Ori() = Quaterniond::Rot(Rad(-120.0), 'y');
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);*/
	/*robot[2].jntConnect = scene->CreateJoint(robot[0].soBody, robot[2].soBody, jdConnect)->Cast();//これの設定がおかしい。これなくてもそこそこ様にはなってる。
	robot[2].jntConnect->SetSpring(K);
	robot[2].jntConnect->SetDamper(D);
	robot[2].jntConnect->SetSpringOrigin(Rad(0.0));*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////4モジュールU型結合////////////////////////////////////////////////////////////////////////////////////////////

	//結合部分構築
	jdConnect.poseSocket.Ori() = Quaterniond();
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	robot[0].jntConnect = scene->CreateJoint(robot[1].soBody, robot[0].soBody, jdConnect)->Cast();
	robot[0].jntConnect->SetSpring(K);
	robot[0].jntConnect->SetDamper(D);
	robot[0].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.2, 0.0, 0.0);
	robot[2].jntConnect = scene->CreateJoint(robot[1].soBody, robot[2].soBody, jdConnect)->Cast();
	robot[2].jntConnect->SetSpring(K);
	robot[2].jntConnect->SetDamper(D);
	robot[2].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.1, 0.0, 0.173);
	robot[3].jntConnect = scene->CreateJoint(robot[2].soBody, robot[3].soBody, jdConnect)->Cast();
	robot[3].jntConnect->SetSpring(K);
	robot[3].jntConnect->SetDamper(D);
	robot[3].jntConnect->SetSpringOrigin(Rad(0.0));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////4モジュール放射状結合////////////////////////////////////////////////////////////////////////////////////////////

	//結合部分構築
	/*jdConnect.poseSocket.Ori() = Quaterniond();
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	robot[0].jntConnect = scene->CreateJoint(robot[1].soBody, robot[0].soBody, jdConnect)->Cast();
	robot[0].jntConnect->SetSpring(K);
	robot[0].jntConnect->SetDamper(D);
	robot[0].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.2, 0.0, 0.0);
	robot[2].jntConnect = scene->CreateJoint(robot[1].soBody, robot[2].soBody, jdConnect)->Cast();
	robot[2].jntConnect->SetSpring(K);
	robot[2].jntConnect->SetDamper(D);
	robot[2].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, -0.173);
	robot[3].jntConnect = scene->CreateJoint(robot[1].soBody, robot[3].soBody, jdConnect)->Cast();
	robot[3].jntConnect->SetSpring(K);
	robot[3].jntConnect->SetDamper(D);
	robot[3].jntConnect->SetSpringOrigin(Rad(0.0));*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////6モジュール不定形///////////////////////////////////////////////////////////////////////////////////////////////////

	/*jdConnect.poseSocket.Ori() = Quaterniond();
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.1, 0.0, -0.173);
	robot[2].jntConnect = scene->CreateJoint(robot[1].soBody, robot[2].soBody, jdConnect)->Cast();
	robot[2].jntConnect->SetSpring(K);
	robot[2].jntConnect->SetDamper(D);
	robot[2].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, -0.173);
	robot[1].jntConnect = scene->CreateJoint(robot[0].soBody, robot[1].soBody, jdConnect)->Cast();
	robot[1].jntConnect->SetSpring(K);
	robot[1].jntConnect->SetDamper(D);
	robot[1].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.2, 0.0, 0.0);
	robot[0].jntConnect = scene->CreateJoint(robot[3].soBody, robot[0].soBody, jdConnect)->Cast();
	robot[0].jntConnect->SetSpring(K);
	robot[0].jntConnect->SetDamper(D);
	robot[0].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, -0.173);
	robot[3].jntConnect = scene->CreateJoint(robot[4].soBody, robot[3].soBody, jdConnect)->Cast();
	robot[3].jntConnect->SetSpring(K);
	robot[3].jntConnect->SetDamper(D);
	robot[3].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, -0.173);
	robot[4].jntConnect = scene->CreateJoint(robot[5].soBody, robot[4].soBody, jdConnect)->Cast();
	robot[4].jntConnect->SetSpring(K);
	robot[4].jntConnect->SetDamper(D);
	robot[4].jntConnect->SetSpringOrigin(Rad(0.0));*/
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////6モジュールr///////////////////////////////////////////////////////////////////////////////////////////////////

	/*jdConnect.poseSocket.Ori() = Quaterniond();
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, -0.173);
	robot[0].jntConnect = scene->CreateJoint(robot[1].soBody, robot[0].soBody, jdConnect)->Cast();
	robot[0].jntConnect->SetSpring(K);
	robot[0].jntConnect->SetDamper(D);
	robot[0].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.2, 0.0, 0.0);
	robot[2].jntConnect = scene->CreateJoint(robot[1].soBody, robot[2].soBody, jdConnect)->Cast();
	robot[2].jntConnect->SetSpring(K);
	robot[2].jntConnect->SetDamper(D);
	robot[2].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	robot[4].jntConnect = scene->CreateJoint(robot[1].soBody, robot[4].soBody, jdConnect)->Cast();
	robot[4].jntConnect->SetSpring(K);
	robot[4].jntConnect->SetDamper(D);
	robot[4].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.2, 0.0, 0.0);
	robot[3].jntConnect = scene->CreateJoint(robot[2].soBody, robot[3].soBody, jdConnect)->Cast();
	robot[3].jntConnect->SetSpring(K);
	robot[3].jntConnect->SetDamper(D);
	robot[3].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	robot[5].jntConnect = scene->CreateJoint(robot[4].soBody, robot[5].soBody, jdConnect)->Cast();
	robot[5].jntConnect->SetSpring(K);
	robot[5].jntConnect->SetDamper(D);
	robot[5].jntConnect->SetSpringOrigin(Rad(0.0));*/
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////6モジュールブーメラン///////////////////////////////////////////////////////////////////////////////////////////////////

	/*jdConnect.poseSocket.Ori() = Quaterniond();
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, -0.173);
	robot[0].jntConnect = scene->CreateJoint(robot[1].soBody, robot[0].soBody, jdConnect)->Cast();
	robot[0].jntConnect->SetSpring(K);
	robot[0].jntConnect->SetDamper(D);
	robot[0].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, -0.173);
	robot[1].jntConnect = scene->CreateJoint(robot[2].soBody, robot[1].soBody, jdConnect)->Cast();
	robot[1].jntConnect->SetSpring(K);
	robot[1].jntConnect->SetDamper(D);
	robot[1].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.2, 0.0, 0.0);
	robot[3].jntConnect = scene->CreateJoint(robot[2].soBody, robot[3].soBody, jdConnect)->Cast();
	robot[3].jntConnect->SetSpring(K);
	robot[3].jntConnect->SetDamper(D);
	robot[3].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.2, 0.0, 0.0);
	robot[4].jntConnect = scene->CreateJoint(robot[3].soBody, robot[4].soBody, jdConnect)->Cast();
	robot[4].jntConnect->SetSpring(K);
	robot[4].jntConnect->SetDamper(D);
	robot[4].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(0.2, 0.0, 0.0);
	robot[5].jntConnect = scene->CreateJoint(robot[4].soBody, robot[5].soBody, jdConnect)->Cast();
	robot[5].jntConnect->SetSpring(K);
	robot[5].jntConnect->SetDamper(D);
	robot[5].jntConnect->SetSpringOrigin(Rad(0.0));*/
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//関節角度計算用関数
void GetTrajectory(int module_num, int leg_num, double para_a1, double para_b1, double para_X1, double para_H1, double Phase[], double direction){

	double	PhaseTemp[100];
	double	L1 = 0.067, L2 = 0.067, L3 = 0.12;	// 関節間長さ

	for(int i = 0; i < module_num; i++){
		for(int j = 0; j < leg_num; j++){
			PhaseTemp[i * 3 + j] = Phase[i * 3 + j];
			// 足先軌道の計算（楕円軌道）
			double	xx = 0.0;
			double	yy = - para_a1 * cos(PhaseTemp[i * 3 + j]);
			double	zz;
				if(sin(PhaseTemp[i * 3 + j]) < 0.0)	zz = - para_H1;
				else zz = para_b1 * sin(PhaseTemp[i * 3 + j]) - para_H1;

			double  YY = xx * cos(direction + robot[i].leg[j].direction) - yy * sin(direction + robot[i].leg[j].direction);
			double  XX = xx * sin(direction + robot[i].leg[j].direction) + yy * cos(direction + robot[i].leg[j].direction) + para_X1;

			// 各関節角度の計算
			double	theta1 = atan(YY / XX);
			double	m = sqrt(XX*XX + YY*YY) - L1;
			double	theta2 = atan(zz / m) + acos( (L2*L2 + m*m + zz*zz - L3*L3) / (2 * L2 * sqrt(m*m + zz*zz)) );
			double	theta3 = - PI + acos( (L2*L2 + L3*L3 - m*m - zz*zz) / (2 * L2 * L3) );

			// グラフィック表示のために各値をセット
			if(robot[i].leg[j].Joint_flag == true)continue;
			else{
				robot[i].leg[j].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
				robot[i].leg[j].jntDX2  ->  SetSpringOrigin(-theta2);
				robot[i].leg[j].jntFoot ->  SetSpringOrigin(-theta3);
			}	
		}
	}
}

//脚位置取得
void GetLegPosition(int module_num){
	for(int i=0; i<module_num; i++){
		robot[i].leg[0].direction = robot[i].theta;
		robot[i].leg[1].direction = robot[i].theta + 2*PI/3;
		robot[i].leg[2].direction = robot[i].theta - 2*PI/3;
	}
}

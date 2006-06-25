/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/HIS/HISpidarG6/main.cpp
【概要】
  グラフィックスレンダラークラスの DrawScene APIを使い、シーンを一括でレンダリングする。
  DrawScene API で設定されているマテリアルマテリアルサンプルを用いて、カラフルなボックスをレンダリングする。　
  また、SPIDARとつなぎ、球体及び地面と力覚インタラクションを行う。
  
【終了基準】
  ・5000ステップ後に強制終了。 

 */

#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#include <string>
#include <GL/glut.h>

#include "HIS/HIS.h"
#include <iostream>
#include <stdio.h>

#include <mmsystem.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;

#define ESC				27			// ESC key
#define EXIT_TIMER		5000		// 実行ステップ数
#define WINSIZE_WIDTH	480			// ウィンドウサイズ(width)
#define WINSIZE_HEIGHT	360			// ウィンドウサイズ(height)
#define NUM_SPHERES		2			// sphere数

#define HAPTIC_FREQ		1			// 力覚スレッドの周期(1/msec)
#define POINTER_RADIUS  1.0			// 力覚ポインタの半径

#ifdef _WIN32		//	Win32版(普通はこっち)
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
#include <conio.h>

// virtual couplingの係数
const float K = 60;
const float B = 30;
const float dt = HAPTIC_FREQ;
#endif

// グラフィック用の変数
GRSdkIf* grSdk;
GRDebugRenderIf* render;
GRDeviceGLIf* grDevice;

// 物理計算用の変数
PHSdkIf* phSdk;
PHSceneIf* scene;
PHSolidIf* soFloor;
std::vector<PHSolidIf*> soSphere; 

// SPIDAR用の変数
DVDeviceManager devMan;						//	D/Aやカウンタなどのデバイスの管理
HISpidarG6X3 spidarG6;						//	SPIDARに対応するクラス

// その他の変数
Vec3f spidar_pos = Vec3f();
Vec3f sphere_pos = Vec3f();
Affinef pos_affine;
Affinef view;

bool bforce = false;
int time_counter = 0;

int sec_counter = 0;
const int one_sec = 1000 / HAPTIC_FREQ;

#define NVTX	4			//	衝突判定する点
static Vec3f grip[NVTX]={
		Vec3f( 0.02f, 0.0f, 0.02f),
		Vec3f(-0.02f, 0.0f, 0.02f),
		Vec3f(-0.02f, 0.0f,-0.02f),
		Vec3f( 0.02f, 0.0f,-0.02f)
	};

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);

	// SPIDARの位置を球形で表示する
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	Posef::Unit(spidar_pos).ToAffine(pos_affine);
	glMultMatrixf(pos_affine);
	glutSolidSphere(POINTER_RADIUS, 12, 9);
	glPopMatrix();

	render->EndScene();

	// ビデオレートにあわせるために60Hzにする
	Sleep(16.66);
}

/**
 brief		光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, -20.0, 1.0);
	render->PushLight(light0);
	render->PushLight(light1);
}

/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		 なし
 */
void reshape(int w, int h){
	// Viewportと射影行列を設定
	render->Reshape(Vec2f(), Vec2f(w,h));
}


MMRESULT FTimerId;

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void keyboard(unsigned char key, int x, int y){
	if (key == ESC) 
	{
		timeKillEvent(FTimerId);
		timeEndPeriod(1);
		std::cout << "exit operation from key input" << std::endl;
		exit(0);
	}
	else if(key == 'c')
	{
		// cでキャリブレーション
		spidarG6.Calib();
	}
	else if(key == ' ')
	{
		bforce = !bforce;
	}
}

double dist;
double min_dist = 10000;
int index = 0;
double d = 10000;
Vec3f so_pos;

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	scene->Step();
	glutPostRedisplay();

	// ポインタの最近傍の物体を探す処理
	min_dist = 10000;
	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		so_pos = soSphere[sphereCnt]->GetFramePosition();
/*
		dist = sqrt(
		  pow(spidar_pos.x - so_pos.x, 2) 
		+ pow(spidar_pos.y - so_pos.y, 2) 
		+ pow(spidar_pos.z - so_pos.z, 2)
		);
*/
		dist = (spidar_pos - so_pos).norm();

		if(dist < min_dist)
		{
			// 最近傍の物体を取得
			min_dist = dist;
			sphere_pos = soSphere[sphereCnt]->GetFramePosition();
			index = sphereCnt;
		}
	}

	// 最小の距離を設定
	d = min_dist;

	time_counter++;

	if (++time_counter > EXIT_TIMER) 
	{
		timeKillEvent(FTimerId);
		timeEndPeriod(1);
		std::cout << "exit by count" << std::endl;
		exit(0);
	}
}

Vec3f lastPos = Vec3f();
Vec3f vel = Vec3f();
Vec3f delta = Vec3f();
const float alpha = 0.1f;
Quaternionf qt;
Vec3f force = Vec3f();
Vec3f torque = Vec3f();
const float WALL = -0.0f;
Vec3f lp = Vec3f();
Vec3f p = Vec3f();

void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	spidarG6.Update(dt);							//	USBの通信を行う．
	spidar_pos = spidarG6.GetPos() * 60;			// spidarの位置を格納している変数の更新
	spidar_pos = view.inv().Rot() * spidar_pos;		// SPIDARの座標がワールド座標ではないのでワールド座標にする

#if _DEBUG
	// 一秒ごとにSPIDARの座標を表示する
	if(sec_counter % one_sec == 0)
	{
//		std::cout << "spidar position = " << spidar_pos << std::endl;
//		std::cout << "force = " << force << "torque = " << torque << std::endl;
//		std::cout << "distance = " << d << std::endl;
		sec_counter = 0;	
	}
	sec_counter++;									// カウンターの更新
#endif

	// 速度・姿勢の用意
	delta = (spidar_pos - lastPos) / dt;
	vel = (1-alpha)*vel + alpha*delta;
	lastPos = spidar_pos;
	qt = spidarG6.GetOri();			//	角度の読み出し 4元数 Quaternionf については，自動生成マニュアルを参照

	// 変数の初期化
	force = Vec3f();
	torque = Vec3f();

	// 提示力の計算
	for(int i=0; i<NVTX; ++i){
		lp = qt*grip[i];
		p = lp + spidar_pos;

		// 地面との接触を計算
		if (p.y-POINTER_RADIUS < WALL){
			// virtual couplingで提示力の計算
			double temp_force = (WALL - p.y + POINTER_RADIUS) * K + B * vel.y;
			force.y += temp_force;
			torque += lp ^ Vec3f(0, temp_force, 0);
		}

		// 球体との接触を計算
		if(d < DCAST(CDSphereIf, soSphere[index]->GetShape(0))->GetRadius() + 1.0)
		{
			Vec3f temp_force = (Vec3f(3, 3, 3) + p - sphere_pos) + (vel - soSphere[index]->GetVelocity());
		
//			Vec3f sphere_force = -temp_force;
//			Vec3f sphere_torque = lp ^ (-temp_force);

			force += -temp_force;
			torque += lp ^ (-temp_force);

			soSphere[index]->AddForce(-temp_force, lp ^ (-temp_force));
		}
	}

	// gripに力を発生させる
	if(bforce)spidarG6.SetForce(force, torque);
	else spidarG6.SetForce(Vec3f(), Vec3f());
}


/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){
	phSdk = CreatePHSdk();					// SDKの作成　
	PHSceneDesc sd;
//	sd.ContactMode = PHSceneDesc::ContactMode::MODE_LCP;
	sd.timeStep = 0.017;
	scene = phSdk->CreateScene(sd);				// シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;

	// Solidの作成
	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere.push_back(scene->CreateSolid(desc));		// 剛体をdescに基づいて作成
	}

	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetGravity(false);

	//	形状の作成
	CDBoxIf* floor=NULL;
	CDSphereIf* sphere=NULL;
	{
		CDSphereDesc sd;
		sd.radius = 2.0;
		sphere = DCAST(CDSphereIf, phSdk->CreateShape(sd));

		CDBoxDesc bd;
		bd.boxsize = Vec3f (30.0, 1.0, 30.0);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
	}	

	soFloor->AddShape(floor);
	soFloor->SetFramePosition(Vec3f(0,-0.5,0));

	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere[sphereCnt]->AddShape(sphere);
		soSphere[sphereCnt]->SetFramePosition(Vec3f(0, 15+5*sphereCnt, 0));
	}
	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINSIZE_WIDTH, WINSIZE_HEIGHT);

	int window = glutCreateWindow("GRSimple");
	grSdk = CreateGRSdk();
	render = grSdk->CreateDebugRender();
	grDevice = grSdk->CreateDeviceGL(window);

	// 初期設定
	grDevice->Init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	
	render->SetDevice(grDevice);	// デバイスの設定

	// 視点を設定する
	
	view.Pos() = Vec3f(0.0, 5.0, -20.0);								// eye
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));			// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);
	
	setLight();

	
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(1));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(2));		//	Sh4版コントローラ 8モータ

	devMan.Init();								//	デバイスの初期化
	std::cout << devMan;						//	初期化の結果を表示
	
	spidarG6.Init(devMan, false);			//	SPIDARの初期化，キャリブレーションもする．


	Sleep(1000);					


	spidarG6.Calib();							//	キャリブレーション

	timeBeginPeriod(1);							//分解能の最小値

	// hapticスレッドの生成・開始
	FTimerId = timeSetEvent(HAPTIC_FREQ,    // タイマー間隔[ms]
                        1,   // 時間分解能
                        TimerProc,//コールバック関数
                        (DWORD)&time,//ユーザー定義の値
                        TIME_PERIODIC //単発(TIME_ONESHOT) or 繰り返し(TIME_PERIODIC)
                        );

	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete phSdk;
}

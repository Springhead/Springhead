/**
 Springhead2/src/tests/HIS/HISpidarG6/main.cpp
【概要】
  グラフィックスレンダラークラスの DrawScene APIを使い、シーンを一括でレンダリングする。
  DrawScene API で設定されているマテリアルマテリアルサンプルを用いて、カラフルなボックスをレンダリングする。　
  また、SPIDARとつなぎ、力覚インタラクションを行う（開発中）

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
#define EXIT_TIMER		7000		// 実行ステップ数
#define WINSIZE_WIDTH	480			// ウィンドウサイズ(width)
#define WINSIZE_HEIGHT	360			// ウィンドウサイズ(height)
#define NUM_SPHERES		2			// sphere数

#define HAPTIC_FREQ		20			// 力覚スレッドの周期(msec)

#ifdef _WIN32		//	Win32版(普通はこっち)
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
#include <conio.h>

const float K = 2000;
const float B = 0;
const float dt = 0.016f;
#endif

GRSdkIf* grSdk;
GRDebugRenderIf* render;
GRDeviceGLIf* grDevice;

PHSdkIf* phSdk;
PHSceneIf* scene;
PHSolidIf* soFloor;
std::vector<PHSolidIf*> soSphere; 

HISpidarG6X3 spidarG6;						//	SPIDARに対応するクラス
Vec3f spidar_pos = Vec3f();

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);

	// SPIDARの位置を表示する
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(spidar_pos.x, spidar_pos.y, spidar_pos.z);
	glutSolidSphere(0.01, 18, 9);
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
}	
/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	scene->Step();
	glutPostRedisplay();
	static int count;
	count++;
	if (++count > EXIT_TIMER) 
	{
		timeKillEvent(FTimerId);
		timeEndPeriod(1);
		std::cout << "exit by count" << std::endl;
		exit(0);
	}
}

static int sec_counter = 0;
static int one_sec = 1000 / HAPTIC_FREQ;

void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	spidarG6.Update(dt);							//	USBの通信を行う．

	spidar_pos = spidarG6.GetPos();					// spidarの位置を格納している変数の更新

	// 一秒ごとにSPIDARの座標を表示する
	if(sec_counter % one_sec == 0)
	{
		std::cout << spidar_pos << std::endl;
		sec_counter = 0;
	}
	sec_counter++;									// カウンターの更新

#if 0
		float delta = (pos.Y() - lastPos) / dt;
		float alpha = 0.1f;
		vel = (1-alpha)*vel + alpha*delta;
		lastPos = spidar_pos.Y();
		Quaternionf qt = spidarG6.GetOri();			//	角度の読み出し 4元数 Quaternionf については，自動生成マニュアルを参照
		float force=0;
		Vec3f torque;
		for(int i=0; i<NVTX; ++i){
			Vec3f lp = qt*grip[i];
			Vec3f p = lp + pos;
			const float WALL = -0.001f;
			if (p.Y() < WALL){
				force += (WALL-p.Y())*K + B*vel;
				torque += lp ^ Vec3f(0, force, 0);
			}
		}
		spidarG6.SetForce(Vec3f(0, force, 0), torque);	//	力の取得
#endif
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
	sd.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;
	sd.timeStep = 0.01;
	scene = phSdk->CreateScene(sd);				// シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;

	// Solidの作成
	unsigned int sphereCnt;
	for (sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
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
		sphere = DCAST(CDSphereIf, phSdk->CreateShape(sd));

		CDBoxDesc bd;
		bd.boxsize = Vec3f (30.0, 1.0, 30.0);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
	}	

	soFloor->AddShape(floor);
	soFloor->SetFramePosition(Vec3f(0,-3,0));
	for (sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
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
	Affinef view;
	view.Pos() = Vec3f(0.0, 5.0, -15.0);								// eye
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));			// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);
	
	setLight();

	DVDeviceManager devMan;						//	D/Aやカウンタなどのデバイスの管理

	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(1));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(2));		//	Sh4版コントローラ 8モータ

	devMan.Init();								//	デバイスの初期化
	std::cout << devMan;						//	初期化の結果を表示
	
	spidarG6.Init(devMan, false);			//	SPIDARの初期化，キャリブレーションもする．

#ifdef __sh__
	cyg_thread_delay(10000);
#else
	Sleep(1000);					
#endif
	
		spidarG6.Calib();				//	キャリブレーション

#define NVTX	4					//	衝突判定する点
	Vec3f grip[NVTX]={
		Vec3f( 0.02f, 0.0f, 0.02f),
		Vec3f(-0.02f, 0.0f, 0.02f),
		Vec3f( 0.02f, 0.0f,-0.02f),
		Vec3f(-0.02f, 0.0f,-0.02f)
	};

	float lastPos=0;
	float vel=0;

	timeBeginPeriod(1//分解能の最小値
            );

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
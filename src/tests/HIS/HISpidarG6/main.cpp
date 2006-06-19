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
#define EXIT_TIMER		5000		// 実行ステップ数
#define WINSIZE_WIDTH	480			// ウィンドウサイズ(width)
#define WINSIZE_HEIGHT	360			// ウィンドウサイズ(height)
#define NUM_SPHERES		2			// sphere数

#define HAPTIC_FREQ		8			// 力覚スレッドの周期(msec)

#ifdef _WIN32		//	Win32版(普通はこっち)
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
#include <conio.h>

// virtual couplingの係数
const float K = 10;
const float B = 10;
const float dt = HAPTIC_FREQ;
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
Affinef pos = Affinef();
Vec3f sp_pos_render = Vec3f();

bool bforce = false;
Affinef view;

#define NVTX	4			//	衝突判定する点
static Vec3f grip[NVTX]={
		Vec3f( 0.02f, 0.0f, 0.02f),
		Vec3f(-0.02f, 0.0f, 0.02f),
		Vec3f(-0.02f, 0.0f,-0.02f),
		Vec3f( 0.02f, 0.0f,-0.02f)
	};

Vec3f sphere_pos = Vec3f();

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
	sp_pos_render = -1 * spidar_pos;		
	sp_pos_render.y *= -1.0f;
	Posef::Unit(sp_pos_render).ToAffine(pos);
	glMultMatrixf(pos);
	glutSolidSphere(1.0, 12, 9);
	glPopMatrix();
	
	// SPIDARの位置と球の距離を線で表示する
	glBegin(GL_LINES);
	glVertex3f(spidar_pos.x * (-1), spidar_pos.y, spidar_pos.z * (-1));
	glVertex3f(sphere_pos.x, sphere_pos.y, sphere_pos.z);
	glEnd();

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

int time_counter = 0;
/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	scene->Step();
	// sphereの位置の更新
	sphere_pos = soSphere[0]->GetFramePosition();

	glutPostRedisplay();

	time_counter++;
	if (++time_counter > EXIT_TIMER) 
	{
		timeKillEvent(FTimerId);
		timeEndPeriod(1);
		std::cout << "exit by count" << std::endl;
		exit(0);
	}
}

int sec_counter = 0;
int one_sec = 1000 / HAPTIC_FREQ;

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
	spidar_pos = spidarG6.GetPos() * 50;					// spidarの位置を格納している変数の更新

	double dist = sqrt(
		  pow(-spidar_pos.x - sphere_pos.x, 2) 
		+ pow(spidar_pos.y - sphere_pos.y, 2) 
		+ pow(-spidar_pos.z - sphere_pos.z, 2)
		);

	// 一秒ごとにSPIDARの座標を表示する
	if(sec_counter % one_sec == 0)
	{
//		std::cout << spidar_pos << sp_pos_render << std::endl;
//		if(spidar_pos.y < WALL)std::cout << "in the wall" << std::endl;
		sec_counter = 0;	

		// sphere の場所を表示
//		for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
//			std::cout << soSphere[sphereCnt]->GetFramePosition() << std::endl;
//		}

//		std::cout << "force = " << force << "torque = " << torque << "dist = " << dist << std::endl;

	}
	sec_counter++;									// カウンターの更新

	// 速度・姿勢の用意
	delta = (spidar_pos - lastPos) / dt;
	vel = (1-alpha)*vel + alpha*delta;
	lastPos = spidar_pos;
	qt = spidarG6.GetOri();			//	角度の読み出し 4元数 Quaternionf については，自動生成マニュアルを参照

	// 変数の初期化
	force = Vec3f();
	torque = Vec3f();

	for(int i=0; i<NVTX; ++i){
		lp = qt*grip[i];
		p = lp + spidar_pos;
		
		if (p.Y() < WALL){
			// virtual couplingで提示力の計算
			double temp_force = (WALL-p.y)*K + B*vel.y;
			force.y += temp_force;
			torque += lp ^ Vec3f(0, temp_force, 0);
		}
	}

	// もし二つの球が干渉していたら
	if(dist < DCAST(CDSphereIf, soSphere[0]->GetShape(0))->GetRadius() + 1.0)
	{
		Vec3f temp_force = (sphere_pos - spidar_pos) * 5 + (vel - soSphere[0]->GetVelocity());
		
		// 提示する力を二つの球体の距離に反比例させたいために
		// 得られた力を単位ベクトルとして、係数を別に求めて掛け算する
		Vec3f utemp = temp_force.unit();
		utemp.x = fabs(3 - temp_force.x) * utemp.x;
		utemp.y = fabs(3 - temp_force.y) * utemp.y;
		utemp.z = fabs(3 - temp_force.z) * utemp.z;

		Vec3f sphere_force = utemp;
		Vec3f sphere_torque = lp ^ utemp;

		force += sphere_force;
		torque += sphere_torque;

		soSphere[0]->AddForce(-sphere_force, -sphere_torque);
	}

	force.x *= -1;
	force.z *= -1;

	// gripに力を発生させる
	if(bforce)spidarG6.SetForce(force, torque);
	else spidarG6.SetForce(Vec3f(), Vec3f());

	spidarG6.Update(dt);
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
	sd.timeStep = 0.017;
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
		sd.radius = 2.0;
		sphere = DCAST(CDSphereIf, phSdk->CreateShape(sd));

		CDBoxDesc bd;
		bd.boxsize = Vec3f (30.0, 1.0, 30.0);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
	}	

	soFloor->AddShape(floor);
	soFloor->SetFramePosition(Vec3f(0,-0.5,0));

	for (sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere[sphereCnt]->AddShape(sphere);
		soSphere[sphereCnt]->SetFramePosition(Vec3f(0, 15+5*sphereCnt, 0));
		std::cout << soSphere[sphereCnt]->GetFramePosition() << std::endl;
	}
	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	sphere_pos = Vec3f(0, 15, 0);

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
	
	view.Pos() = Vec3f(0.0, 10.0, -20.0);								// eye
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
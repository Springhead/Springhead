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
#include <iostream>
#include <stdio.h>

#include "HIS/HIS.h"
#include "Physics/PHConstraintEngine.h"
#include "Physics/PHConstraint.h"
#include "Physics/PHContactPoint.h"

#include <mmsystem.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;

#define ESC				27			// ESC key
#define EXIT_TIMER		5000		// 実行ステップ数
#define WINSIZE_WIDTH	480			// ウィンドウサイズ(width)
#define WINSIZE_HEIGHT	360			// ウィンドウサイズ(height)
#define NUM_SPHERES		10			// sphere数

#define HAPTIC_FREQ		10			// 力覚スレッドの周期(1/msec)
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

MMRESULT FTimerId;


#define NVTX	4			//	衝突判定する点
static Vec3f grip[NVTX]={
		Vec3f( 0.02f, 0.0f, 0.02f),
		Vec3f(-0.02f, 0.0f, 0.02f),
		Vec3f(-0.02f, 0.0f,-0.02f),
		Vec3f( 0.02f, 0.0f,-0.02f)
	};

int index = 0;
double global_collision_counter = 0;

std::map<PHConstraint*, int> process_map;

/**
 brief     	calculate_surround_effectから呼ばれる再帰的に情報を計算する関数
 param		なし
 return 	とりあえず今はVec3fを返している（変更予定）
*/
Vec3f calculate_counteractive_object_accel(PHConstraints cs, int depth, PHSolid* solid)
{
	// すべての接触について処理
	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
	{
		// 現在の値がすでに処理されたかチェック
		std::map<PHConstraint*, int>::iterator t = process_map.find(*it);

		// まだ処理されていない
		if(t == process_map.end())
		{
			// この接触の片方がうけとった剛体である場合再帰的にこの関数を呼び処理を行う
			if((*it)->solid[0]->solid == solid)
			{
				//　すでに処理した接触をマップに登録する
				process_map.insert(std::pair<PHConstraint*, int>(*it, depth));

				// 再帰的に次の処理に渡す
				calculate_counteractive_object_accel(cs, depth+1, (*it)->solid[1]->solid);
			}
			else if((*it)->solid[1]->solid == solid)
			{
				//　すでに処理した接触をマップに登録する
				process_map.insert(std::pair<PHConstraint*, int>(*it, depth));

				// 再帰的に次の処理に渡す
				calculate_counteractive_object_accel(cs, depth+1, (*it)->solid[0]->solid);
			}
		}
	}

	// ここに具体的な処理を追加する----------------------------------------------------------

	return Vec3f();
}

/**
 brief     	接触点の情報から周囲の情報による影響を計算する関数
 param		なし
 return 	周囲の影響を行列として返す
*/
Matrix3d calculate_surround_effect(PHConstraints cs)
{
	if(cs.size() > 0)
	{
		// 最近傍物体の取得
		PHSolidIf *nearest = soSphere[index];

		// depth変数の宣言
		int depth = 1;

		// 処理チェックに使うマップを初期化
		process_map.clear();

		// すべての衝突についてチェック
		for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
		{
			// 現在の値がすでに処理されたかチェック
			std::map<PHConstraint*, int>::iterator t = process_map.find(*it);
	
			// まだ処理されていない
			if(t == process_map.end())
			{
				// solid1が最近傍物体
				if(((*it)->solid[0]->solid) == nearest)
				{
					//　すでに処理した接触点をマップに登録する
					process_map.insert(std::pair<PHConstraint*, int>(*it, depth));

					// 再帰的に次の処理に渡す	
					calculate_counteractive_object_accel(cs, depth+1, (*it)->solid[1]->solid);
//					DSTR << "solid1 is nearest" << std::endl;
				}
				// solid2が最近傍物体
				else if(((*it)->solid[1]->solid) == nearest)
				{
					//　すでに処理した接触点をマップに登録する
					process_map.insert(std::pair<PHConstraint*, int>(*it, depth));

					// 再帰的に次の処理に渡す
					calculate_counteractive_object_accel(cs, depth+1, (*it)->solid[0]->solid);
//					DSTR << "solid2 is nearest" << std::endl;
				}
				else
				{
					//　最近傍物体がない場合のデバッグ出力
//					DSTR << "no object is nearest " << std::endl;
				}
			}
		}
	}

	// ここに具体的な処理を追加する----------------------------------------------------------

	return Matrix3d();
}

/**
 brief     	物体の接触点の情報を表示する関数
 param		なし
 return 	なし
*/
void show_collision_info(PHConstraints cp)
{
	// 接触点が存在すれば処理する
	if(cp.size() > 0)
	{
		int counter = 0;
		DSTR << "----------NEW COLLISION DATA No." << global_collision_counter++ << "-----------" << std::endl;
		
		// すべての衝突に対して処理
		for(PHConstraints::iterator it = cp.begin(); it != cp.end(); it++)
		{
			// ループ内のカウンターを更新
			counter++;
			DSTR << "COLLISION" << counter << ": " << std::endl;


			// 最近傍物体のアドレスを取得
			PHSolidIf* nearest_address = &(*soSphere[index]);

			// PHContactPointにdynamic castした処理のチェック
			PHContactPoint* contact = DCAST(PHContactPoint, (*it));
			DSTR << "collision pos = " << contact->pos << std::endl;
//			DSTR << "address of solid1 = " << &(*contact->solid[0]->solid) << std::endl;
//			DSTR << "address of solid2 = " << &(*contact->solid[1]->solid) << std::endl;
			DSTR << std::endl;

			// PHConstraintの情報の表示
			// solid1
			DSTR << "solid1.mass = " << (*it)->solid[0]->solid->mass << std::endl;
			DSTR << "solid1.inertia = " << std::endl << (*it)->solid[0]->solid->inertia;
			DSTR << "solid1.velocity = " << (*it)->solid[0]->solid->GetVelocity()<< std::endl;
			DSTR << "solid1.angular_velocity = " << (*it)->solid[0]->solid->GetAngularVelocity() << std::endl;
			DSTR << "solid1.position = " << (*it)->solid[0]->solid->GetFramePosition() << std::endl;
			DSTR << "vector to working point from center = " << (contact->pos - (*it)->solid[0]->solid->GetCenterPosition()).unit() << std::endl;
//			DSTR << "solid1.address = " << ((*it)->solid[0]->solid) << std::endl;
			if(nearest_address == ((*it)->solid[0]->solid))DSTR << " --this solid is nearest object!!" << std::endl;

			DSTR << std::endl;

			// solid2
			DSTR << "solid2.mass = " << (*it)->solid[1]->solid->mass << std::endl;
			DSTR << "solid2.inertia = " << std::endl << (*it)->solid[1]->solid->inertia;
			DSTR << "solid2.velocity = " << (*it)->solid[1]->solid->GetVelocity()<< std::endl;
			DSTR << "solid2.angular_velocity = " << (*it)->solid[1]->solid->GetAngularVelocity() << std::endl;
			DSTR << "solid2.position = " << (*it)->solid[1]->solid->GetFramePosition() << std::endl;
			DSTR << "vector to working point from center = " << (contact->pos - (*it)->solid[1]->solid->GetCenterPosition()).unit() << std::endl;
//			DSTR << "solid2.address = " << ((*it)->solid[1]->solid) << std::endl;
			if(nearest_address == ((*it)->solid[1]->solid))DSTR << " --this solid is nearest object!!" << std::endl;

			DSTR << std::endl;

//			DSTR << "index = " << index << "; address = " << &(*soSphere[index]) << std::endl;

			DSTR << std::endl;
		}
	}
}

/**
 brief     	物体の接触点を取得する関数（APIに移動予定）
 param		なし
 return 	接触点の集合
*/
PHConstraints GetContactPoints()
{
	return scene->GetConstraintEngine()->GetContactPoints();
}


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
	// toggle force output
	else if(key == ' ')
	{
		bforce = !bforce;
	}
}

double dist;
double min_dist;
double d = 10000;
Vec3f so_pos;

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	scene->Step();

	// ポインタの最近傍の物体を探す処理
	min_dist = 10000;

	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		so_pos = soSphere[sphereCnt]->GetFramePosition();

		dist = (spidar_pos - so_pos).norm();

		if(dist < min_dist)
		{
			// 最近傍の物体を取得
			min_dist = dist;
			sphere_pos = so_pos; 
			index = sphereCnt;
		}
	}

	// 最小の距離を設定
	d = min_dist;

	// 衝突点を取得
	PHConstraints cs = GetContactPoints();

	// 衝突点情報を表示
//	show_collision_info(cs);
	
	// 衝突点から周囲の影響を計算
	calculate_surround_effect(cs);

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

Vec3f lastPos = Vec3f();
Vec3f vel = Vec3f();
Vec3f delta = Vec3f();
const float alpha = 0.1f;
Quaternionf qt;
Vec3f force;
Vec3f torque;
const float WALL = -0.0f;
Vec3f lp;
Vec3f p;

/**
 brief  	提示力の計算とHaptic Device	へ反映. multimedia timerによって呼び出される
 param	 	なし
 return 	なし
*/
void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	spidarG6.Update(dt);							//	USBの通信を行う．
	spidar_pos = view.inv().Rot() * spidarG6.GetPos() * 60;		// SPIDARの座標がワールド座標ではないのでワールド座標にする

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

	// 提示力の計算 --> solidを使った方法に変える
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
 brief  	device managerの初期化を行う関数
 param	 	なし
 return 	なし
 */
void InitDeviceManager()
{
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(1));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(2));		//	Sh4版コントローラ 8モータ

	devMan.Init();								//	デバイスの初期化
	std::cout << devMan;						//	初期化の結果を表示	
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

	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetDynamical(false);

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

	InitDeviceManager();
	spidarG6.Init(devMan, false);			//	SPIDARの初期化，キャリブレーションもする．

	timeBeginPeriod(1);							//分解能の最小値
/*
	// hapticスレッドの生成・開始
	FTimerId = timeSetEvent(HAPTIC_FREQ,    // タイマー間隔[ms]
                        1,   // 時間分解能
                        TimerProc,//コールバック関数
                        (DWORD)&time,//ユーザー定義の値
                        TIME_PERIODIC //単発(TIME_ONESHOT) or 繰り返し(TIME_PERIODIC)
                        );
*/
	// openGL main loop
	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete phSdk;
}
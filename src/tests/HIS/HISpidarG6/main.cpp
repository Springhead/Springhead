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
#define EXIT_TIMER		10000		// 実行ステップ数
#define WINSIZE_WIDTH	480			// ウィンドウサイズ(width)
#define WINSIZE_HEIGHT	360			// ウィンドウサイズ(height)
#define NUM_SPHERES		1			// sphere数

// 力覚スレッドの周期Hz(1000/HAPTIC_FREQ)
#ifdef _DEBUG
#define HAPTIC_FREQ		100 
#elif _OPT
#define HAPTIC_FREQ		2
#elif _WINDOWS
#define HAPTIC_FREQ		3
#endif

// 提示力のバネダンパ係数
#define K_force			100
#define B_force			0.5

// SPIDAR更新幅
const float dt = 0.001f * HAPTIC_FREQ;

// 一秒で処理する回数
const int one_sec = 1000 / HAPTIC_FREQ;

#ifdef _WIN32		//	Win32版(普通はこっち)
	#include <Device/DRUsb20Simple.h>
	#include <Device/DRUsb20Sh4.h>
	#include <conio.h>

	// virtual couplingの係数
	#ifdef _DEBUG
		const float K = 2000;
		const float B = 120;
	#elif _OPT
		const float K = 5;
		const float B = 5;
	#elif _WINDOWS
		const float K = 20;
		const float B = 25;
	#endif
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
PHSolidIf* soPointer;

// SPIDAR用の変数
DVDeviceManager devMan;						//	D/Aやカウンタなどのデバイスの管理
HISpidarG6X3 spidarG6;						//	SPIDARに対応するクラス

// その他の変数
Vec3f spidar_pos = Vec3f();
Affinef view;
Matrix3f view_rot;
bool bforce = false;
MMRESULT FTimerId;

// 周囲の影響の計算に使う変数
Matrix3d effect[NUM_SPHERES+1];
Vec3d constant[NUM_SPHERES+1];

// 再帰計算をした情報を格納するテーブル
//std::map<PHConstraint*, int> process_map;
PHConstraint* process_map[NUM_SPHERES+1];
int num_process = 0;

// ポインタと接触したソリッド
PHSolid* nearest_solids[NUM_SPHERES+1];
int num_collisions = 0;

PHConstraint* nearest_constraints[NUM_SPHERES+1];

/**
 brief     	calculate_surround_effectから呼ばれ、接触力を再帰的に計算する関数
 param		なし
 return 	呼び出し元の剛体と接触する剛体の接触力を返す
*/
void calculate_counteractive_object_force(PHConstraints cs, int depth, PHSolid* solid, Vec3d original_r, Vec3d *output_force)
{
	std::vector<Vec3d> forces;
	std::vector<Vec3d> vec_r;

	// すべての接触について処理
	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
	{
		bool process = false;

		// 現在の値がすでに処理されたかチェック
		for(int i = 0; i < num_process; i++)
		{
			if(process_map[i] == *it)process = true;
		}
//		std::map<PHConstraint*, int>::iterator t = process_map.find(*it);

		// まだ処理されていない
		if(process == false)
		{
			// この接触の片方がうけとった剛体である場合再帰的にこの関数を呼び処理を行う
			if((*it)->solid[0]->solid == solid)
			{
				//　すでに処理した接触をマップに登録する
//				process_map.insert(std::pair<PHConstraint*, int>(*it, depth));
				process_map[num_process++] = *it;

				// 剛体の中心から力の作用点までのベクトルを計算する
				PHContactPoint* contact = DCAST(PHContactPoint, (*it));
				Vec3d f = Vec3d();
				Vec3d r = contact->pos - (*it)->solid[1]->solid->GetCenterPosition();

				// 再帰的に次の処理に渡す
				calculate_counteractive_object_force(cs, depth+1, (*it)->solid[1]->solid, r, &f);

				// 得られた情報の保存
				forces.push_back(f);
				vec_r.push_back(r);
			}
			else if((*it)->solid[1]->solid == solid)
			{
				//　すでに処理した接触をマップに登録する
//				process_map.insert(std::pair<PHConstraint*, int>(*it, depth));
				process_map[num_process++] = *it;

				// 剛体の中心から力の作用点までのベクトルを計算する
				PHContactPoint* contact = DCAST(PHContactPoint, (*it));
				Vec3d f = Vec3d();
				Vec3d r = contact->pos - (*it)->solid[0]->solid->GetCenterPosition();

				// 再帰的に次の処理に渡す
				calculate_counteractive_object_force(cs, depth+1, (*it)->solid[0]->solid, r, &f);

				// 得られた情報の保存
				forces.push_back(f);
				vec_r.push_back(r);
			}
		}
	}

	// 変数の用意
	Vec3d sum_r = Vec3d();
	Vec3d sum_force = Vec3d();
	Vec3d sum_r_out_f = Vec3d();

	for(int i = 0; i < (int)forces.size(); i++) 
	{
		Vec3d r = (Vec3d)vec_r.at(i);
		Vec3d f = (Vec3d)forces.at(i);

		// 得られた値の合計値を計算
		// 物体に加わる力を正とし、
		// 前の剛体から得られた力は作用させる力なので負にする
		sum_r = sum_r + r;
		sum_force = sum_force + (- f) ;

		//　力と発生源からの外積を計算、それらの合計値を計算する
		sum_r_out_f = sum_r_out_f + r ^ (- f);
	}

	// sum_rだけは返す力に対応するｒを加える
	sum_r = sum_r + original_r;

	// まとめられる部分はまとめる
	Vec3d A = solid->GetAcceleration() - (solid->GetInertiaInv() * sum_r_out_f) ^ sum_r + solid->GetTorque() * solid->GetTorque() * sum_r - solid->GetMassInv() * sum_force;

	Matrix3d T = solid->GetInertiaInv() * Matrix3d(0, - original_r.z, original_r.y, original_r.z, 0, - original_r.x, -original_r.y, original_r.x, 0);
	Matrix3d M = solid->GetMassInv() * Matrix3d().Unit() + Matrix3d(T.Ex() ^ sum_r, T.Ey() ^ sum_r, T.Ez() ^ sum_r);

	// 導かれる力を返す
	*output_force = M.inv() * A;
}

/**
 brief     	接触点の情報から周囲の情報による影響を計算する関数
 param		その時点での衝突情報PHConstraintsと結果格納用のMatrix3dとVec3dをとる。
			周囲の影響をEffectに、定数項をConstantに格納する
			また、PHSolid* solids[]にポインタが接している剛体のリストが入る
 return 	pointerに接している物体の個数を返す
*/
int calculate_surround_effect(PHConstraints cs, Matrix3d *Effect, Vec3d *Constant, PHSolid* solids[NUM_SPHERES+1]) 
{
	std::vector<Vec3d> forces;
	std::vector<Vec3d> vec_r;

	Vec3d vec_r_pointer[NUM_SPHERES+1];
	int num_solids = 0;

	// 衝突の数をチェック
	if(cs.size() > 0)
	{
		// 深さの初期化
		int depth = 1;

		// 処理済マップの初期化
//		process_map.clear();
		for(int i = 0; i < num_process; i++)
		{
			process_map[i] = NULL;
		}
		num_process = 0;

		// pointerに接している剛体を検索し、取得する
		for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
		{
			// 接触の片方の剛体がポインタ
			if((*it)->solid[0]->solid == soPointer)
			{
				// その剛体を保存
				solids[num_solids] = (*it)->solid[1]->solid;
				nearest_constraints[num_solids] = *it;

				// その剛体の中心から力の作用点までのベクトルを保存
				PHContactPoint* contact = DCAST(PHContactPoint, (*it));
				vec_r_pointer[num_solids++] = contact->pos - (*it)->solid[1]->solid->GetCenterPosition();

				// 処理済マップに登録
//				process_map.insert(std::pair<PHConstraint*, int>(*it, depth));
				process_map[num_process++] = *it;
			}
			// 接触の片方の剛体がポインタ
			else if((*it)->solid[1]->solid == soPointer)
			{
				// その剛体を保存
				solids[num_solids] = (*it)->solid[0]->solid;
				nearest_constraints[num_solids] = *it;

				// その剛体の中心から力の作用点までのベクトルを保存
				PHContactPoint* contact = DCAST(PHContactPoint, (*it));
				vec_r_pointer[num_solids++] = contact->pos - (*it)->solid[0]->solid->GetCenterPosition();

				// 処理済マップに登録
//				process_map.insert(std::pair<PHConstraint*, int>(*it, depth));
				process_map[num_process++] = *it;
			}
		}

		// もしポインタに接する剛体がなければ
		// 力覚に影響がないのでリターンする
		if(num_solids == 0)
		{
			return 0;
		}

		// pointerに接している剛体が接している剛体を検索し、
		// それぞれ処理していく
		for(int j = 0; j < num_solids; j++)
		{
			// 先ほど登録した剛体を取得
			PHSolidIf* solid = (PHSolidIf*)solids[j];

			// この剛体について衝突のリストから検索
			for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
			{
				// 処理済みかどうか検索
//				std::map<PHConstraint*, int>::iterator t = process_map.find(*it);

				bool process = false;

				for(int i = 0; i < num_process; i++)
				{
					if(process_map[i] == *it)process = true;
				}

				// 処理済みマップからは見つからなかった
				if(process == false)
				{
					// この剛体が含まれた接触
					if((*it)->solid[0]->solid == solid)
					{
						// この接触点を処理済マップに登録する
//						process_map.insert(std::pair<PHConstraint*, int>(*it, depth));
						process_map[num_process++] = *it;

						// 剛体の中心から力の作用点までのベクトルを計算する
						PHContactPoint* contact = DCAST(PHContactPoint, (*it));
						Vec3d f = Vec3d();
						Vec3d r = contact->pos - (*it)->solid[1]->solid->GetCenterPosition();

						// 再帰的に次の処理に渡す	
						calculate_counteractive_object_force(cs, depth+1, (*it)->solid[1]->solid, r, &f);

						// 得られた値の保存
						forces.push_back(f);
						vec_r.push_back(r);
					}
					// この剛体が含まれた接触
					else if((*it)->solid[1]->solid == solid)
					{
						//　この接触点を処理済マップに登録する
//						process_map.insert(std::pair<PHConstraint*, int>(*it, depth));
						process_map[num_process++] = *it;

						// 剛体の中心から力の作用点までのベクトルを計算する
						PHContactPoint* contact = DCAST(PHContactPoint, (*it));
						Vec3d f = Vec3d();
						Vec3d r = contact->pos - (*it)->solid[0]->solid->GetCenterPosition();

						// 再帰的に次の処理に渡す
						calculate_counteractive_object_force(cs, depth+1, (*it)->solid[0]->solid, r, &f);

						// 得られた値の保存
						forces.push_back(f);
						vec_r.push_back(r);
					}
				}
			}

			// 変数の用意
			Vec3d sum_r = Vec3d();
			Vec3d sum_force = Vec3d();
			Vec3d sum_r_out_f = Vec3d();

			for(int i = 0; i < (int)forces.size(); i++) 
			{
				Vec3d r = (Vec3d)vec_r.at(i);
				Vec3d f = (Vec3d)forces.at(i);

				// 得られた値の合計値を計算 
				// 物体に加わる力を正とし、
				// 前の剛体から得られた力は作用させる力なので負にする
				sum_r = sum_r + r;
				sum_force = sum_force + (- f);

				//　力と発生源からの外積を計算、それらの合計値を計算する
				sum_r_out_f = sum_r_out_f + r ^ (- f);
			}

			// 力覚ポインタと最近傍物体との接触点
			Vec3d original_r = vec_r_pointer[j];

			// sum_rだけは返す力に対応するｒを加える
			sum_r = sum_r + original_r;

			// まとめられる部分はまとめる
			// 定数項
			// A = -w^2 * sum(r) + (I^(-1) * sum(r x f)) x sum(r) + sum(f) / m
			Vec3d A = (((PHSolid *)soPointer)->GetInertiaInv() * sum_r_out_f) ^ sum_r - ((PHSolid *)soPointer)->GetTorque() * ((PHSolid *)soPointer)->GetTorque() * sum_r + ((PHSolid *)soPointer)->GetMassInv() * sum_force;

			// T = I^(-1) * (r x f)のrを外積から行列にして外積を排除したもの
			Matrix3d T = ((PHSolid *)soPointer)->GetInertiaInv() * Matrix3d(0, - original_r.z, original_r.y, original_r.z, 0, - original_r.x, -original_r.y, original_r.x, 0);
			
			// 求める行列
			// M = 1/m * E + (T1 x sum(r) T2 x sum(r) T3 x sum(r))
			Matrix3d M = ((PHSolid *)soPointer)->GetMassInv() * Matrix3d().Unit() + Matrix3d(T.Ex() ^ sum_r, T.Ey() ^ sum_r, T.Ez() ^ sum_r);

			// 結果の格納
			Effect[j] = M;
			Constant[j] = A;
		}
		return num_solids;
	}
	// 衝突がなかったので０を返してリターン
	else
	{
		return 0;
	}
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
		// 衝突を表示するときの個数を数えるカウンター
		static double global_collision_counter = 0;
		int counter = 0;
		DSTR << "----------NEW COLLISION DATA No." << global_collision_counter++ << "-----------" << std::endl;
		
		// すべての衝突に対して処理
		for(PHConstraints::iterator it = cp.begin(); it != cp.end(); it++)
		{
			// ループ内のカウンターを更新
			counter++;
			DSTR << "COLLISION" << counter << ": " << std::endl;

			// PHContactPointにdynamic castした処理のチェック
			PHContactPoint* contact = DCAST(PHContactPoint, (*it));
			DSTR << "collision pos = " << contact->pos << std::endl;
			DSTR << std::endl;

			// PHConstraintの情報の表示
			// solid1
			DSTR << "solid1.mass = " << (*it)->solid[0]->solid->mass << std::endl;
			DSTR << "solid1.inertia = " << std::endl << (*it)->solid[0]->solid->inertia;
			DSTR << "solid1.velocity = " << (*it)->solid[0]->solid->GetVelocity()<< std::endl;
			DSTR << "solid1.angular_velocity = " << (*it)->solid[0]->solid->GetAngularVelocity() << std::endl;
			DSTR << "solid1.position = " << (*it)->solid[0]->solid->GetFramePosition() << std::endl;
			DSTR << "vector to working point from center = " << (contact->pos - (*it)->solid[0]->solid->GetCenterPosition()).unit() << std::endl;

			for(int i = 0; i < num_collisions; i++)
			{
				// 最近傍物体のアドレスを取得
				PHSolidIf* nearest_address = &(*nearest_solids[i]);
				if(nearest_address == ((*it)->solid[0]->solid))DSTR << " --this solid is nearest object!!" << std::endl;
			}
			DSTR << std::endl;

			// solid2
			DSTR << "solid2.mass = " << (*it)->solid[1]->solid->mass << std::endl;
			DSTR << "solid2.inertia = " << std::endl << (*it)->solid[1]->solid->inertia;
			DSTR << "solid2.velocity = " << (*it)->solid[1]->solid->GetVelocity()<< std::endl;
			DSTR << "solid2.angular_velocity = " << (*it)->solid[1]->solid->GetAngularVelocity() << std::endl;
			DSTR << "solid2.position = " << (*it)->solid[1]->solid->GetFramePosition() << std::endl;
			DSTR << "vector to working point from center = " << (contact->pos - (*it)->solid[1]->solid->GetCenterPosition()).unit() << std::endl;

			for(int i = 0; i < num_collisions; i++)
			{
				// 最近傍物体のアドレスを取得
				PHSolid* nearest_address = &(*nearest_solids[i]);
				if(nearest_address == ((*it)->solid[1]->solid))DSTR << " --this solid is nearest object!!" << std::endl;
			}

			DSTR << std::endl;
		}
	}
}

/**
 brief  	提示力の計算とHaptic Device	へ反映. multimedia timerによって呼び出される
 param	 	なし
 return 	なし
*/
void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	// pointerの位置の更新
	static Vec3d old_pos = Vec3d();
	static Vec3d last_force = Vec3d();
	static Vec3d last_last_force = Vec3d();

	spidarG6.Update(dt);//	USBの通信を行う．
	spidar_pos = view_rot * spidarG6.GetPos();		// SPIDAR座標からワールド座標にする

	// pointerの速度を計算
	Vec3d PointerVel = (spidar_pos - old_pos) / dt;
	old_pos = spidar_pos;

	// spidar座標までのベクトルを作成
	Vec3d goal = spidar_pos - soPointer->GetFramePosition();

	// VR空間のポインタとSPIDARをvirtual couplingでつなげる
	soPointer->AddForce(K * goal + B * (PointerVel - soPointer->GetVelocity()), Vec3f());

	// --- 複数衝突の場合はここから変更する ---
	// 加える力を計算 ---> 見直しが必要
	
	Vec3d force = Vec3d();
	Vec3d pointer_torque = Vec3d();

	for(int i = 0; i < num_collisions; i++)
	{
		PHConstraint *constraint = nearest_constraints[i];
		if(constraint == NULL)break;
		PHContactPoint* cp = DCAST(PHContactPoint, constraint);
		
		Vec3d local_force = (cp->pos - spidar_pos);
		Vec3d local_torque = (cp->pos - nearest_solids[i]->GetCenterPosition()) ^ local_force;
		nearest_solids[i]->AddForce(local_force, local_torque);

		force = force + local_force;
		pointer_torque = pointer_torque + (cp->pos - soPointer->GetCenterPosition()) ^ (-local_force);
	}

	// spidarに力を加える
	if(num_collisions > 0)
	{
		// 前二つの提示力と平均を取ってつぶを取る
		if(bforce)spidarG6.SetForce(-(0.6 * force + 0.25 * last_force + 0.15 * last_last_force), pointer_torque);
		else spidarG6.SetForce(Vec3d(), Vec3d());

		// 提示力を保存
		last_last_force = last_force;
		last_force = force;
	}
	else
	{
		// 衝突がないので提示力をリセットする
		spidarG6.SetForce(Vec3d());
		last_last_force = last_force;
		last_force = Vec3d();
	}

#if 0
//#if _DEBUG | _WINDOWS
	static int sec_counter = 0;
	// 一秒ごとにSPIDARの座標を表示する
	if(sec_counter % one_sec == 0)
	{
//		std::cout << "spidar position = " << spidar_pos << std::endl;
//		std::cout << "spidar velocity = " << PointerVel << std::endl;
		std::cout << "force = " << force << "torque = " << pointer_torque << std::endl;
		sec_counter = 0;	
	}
	sec_counter++;									// カウンターの更新
#endif
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
 brief		特定のソリッドから重力をなくす
 param		重力をなくすソリッド
 return		なし
*/
void RemoveGravity(PHSolidIf* solid)
{
	scene->RemoveGravity(solid);
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	scene->Step();

	// 衝突点を取得
	PHConstraints cs = GetContactPoints();

	// 衝突点情報を表示
//	show_collision_info(cs);

	// 衝突点から周囲の影響を計算
	num_collisions = calculate_surround_effect(cs, effect, constant, nearest_solids);

	// 再描画
	glutPostRedisplay();

/*
	// 経過時間をチェック
	static int time_counter = 0;
	time_counter++;
	if (time_counter > EXIT_TIMER) 
	{
		timeKillEvent(FTimerId);
		timeEndPeriod(1);
		std::cout << "exit by count" << std::endl;
		exit(0);
	}
*/
}

/**
 brief		解析法の影響を受けない剛体を登録する。この剛体が生む接触はすべて処理されない
 param		登録する剛体
 return		なし
*/
void AddInteractiveSolid(PHSolidIf* soPointer)
{
	scene->GetConstraintEngine()->AddInteractiveSolid(soPointer);
}

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);

//#if _DEBUG | _WINDOWS
	// SPIDARの軌跡をデバッグ出力
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	static Affinef pos_affine;
	Posef::Unit(spidar_pos).ToAffine(pos_affine);
	glMultMatrixf(pos_affine);
	
	GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, white);

	glutSolidSphere(0.3, 12, 9);
	glPopMatrix();
//#endif

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
	else if(key == 'i')
	{
		soPointer->SetFramePosition(Vec3d(0, 0, 0));
		spidarG6.Calib();
	}
}

void InitScene();
void InitRendering(int*, char**);
void InitDeviceManager();

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){

	// object, solidなどの初期化
	InitScene();

	// rendering関係の初期化
	InitRendering(&argc, argv);

	// device managerの初期化
	InitDeviceManager();

	// deviceの初期化
	spidarG6.Init(devMan, false);			//	SPIDARの初期化，キャリブレーションもする．

	timeBeginPeriod(1);							//分解能の最小値

	// hapticスレッドの生成・開始
	FTimerId = timeSetEvent(HAPTIC_FREQ,    // タイマー間隔[ms]
	                        1,   // 時間分解能
		                    TimerProc,//コールバック関数
		                    (DWORD)&time,//ユーザー定義の値
		                    TIME_PERIODIC //単発(TIME_ONESHOT) or 繰り返し(TIME_PERIODIC)
		                    );

	// openGL main loop
	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete phSdk;
}

/**
 brief  	sceneの初期化を行う関数
 param	 	なし
 return 	なし
 */
void InitScene()
{
	phSdk = PHSdkIf::CreateSdk();					// SDKの作成　
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

	soPointer = scene->CreateSolid(desc);

	//	形状の作成
	CDBoxIf* floor=NULL;
	CDSphereIf* sphere=NULL;

	{
		CDSphereDesc sd;
		sd.radius = 1.0;
		sphere = DCAST(CDSphereIf, phSdk->CreateShape(sd));
		soPointer->AddShape(sphere);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));

		sd.radius = 2.0;
		sphere = DCAST(CDSphereIf,phSdk->CreateShape(sd));

		CDBoxDesc bd;
		bd.boxsize = Vec3f (30.0, 1.0, 30.0);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soFloor->AddShape(floor);
		soFloor->SetFramePosition(Vec3f(0,-2.5,0));
	}	

	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere[sphereCnt]->AddShape(sphere);
		soSphere[sphereCnt]->SetFramePosition(Vec3f(0, 15+5*sphereCnt, 0));
	}
	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	// pointerの重力を無効にする
	RemoveGravity(soPointer);

	// pointerと接触する剛体との解析法による計算をやめ、
	// 力覚で処理するようにする
	AddInteractiveSolid(soPointer);

	for(int i = 0; i < NUM_SPHERES+1; i++) nearest_constraints[i] = NULL;
}

/**
 brief  	rendering, windowまわりの初期化を行う関数
 param	 	mainの引数argc, argv
 return 	なし
 */
void InitRendering(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINSIZE_WIDTH, WINSIZE_HEIGHT);

	int window = glutCreateWindow("GRSimple");
	grSdk = GRSdkIf::CreateSdk();
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

	/// SPIDAR座標をビュー座標に直す変数
	view_rot = view.inv().Rot() * 60;
	
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
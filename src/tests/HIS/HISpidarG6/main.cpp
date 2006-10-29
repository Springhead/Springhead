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
  DrawScene API で設定されているマテリアルサンプルを用いて、カラフルな球体をレンダリングする。　
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
#define NUM_SPHERES		2			// sphere数
#define SPIDAR_SCALE	70			// SPIDARのVE内での動作スケール


#ifdef _DEBUG
	#define SIMULATION_FREQ	100         // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		500			// 力覚スレッドの周期Hz
	float K = 1500;						// virtual couplingの係数
	float B = 150;

#elif _WINDOWS
	#define SIMULATION_FREQ	150         // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		1000		// 力覚スレッドの周期Hz
	float K = 1450;						// virtual couplingの係数
	float B = 200;
#endif

// 提示力と剛体に提示する力を直接変化させる定数
double FORCE_COEFF=		0.1;

#ifdef _WIN32		//	Win32版(普通はこっち)
	#include <Device/DRUsb20Simple.h>
	#include <Device/DRUsb20Sh4.h>
	#include <conio.h>
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
Matrix3f view_rot;
Matrix3f view_haptic;

bool bforce = false;
MMRESULT FTimerId1;
MMRESULT FTimerId2;

// 再帰計算をした情報を格納するテーブル
PHConstraint* process_map[NUM_SPHERES+1];
int num_process = 0;

// 力覚計算に必要なデータを集めた構造体
typedef struct {
	// collision solid data
	PHSolid* nearest_solids[NUM_SPHERES+1];
	Vec3d solid_velocity[NUM_SPHERES+1];
	Vec3d solid_angular_velocity[NUM_SPHERES+1];
	Matrix3d solid_inertiainv[NUM_SPHERES+1];

	// collision data
	Vec3d col_positions[NUM_SPHERES+1];
	Vec3d col_normals[NUM_SPHERES+1];
	int num_collisions;
	
	Vec3d original_col_positions[NUM_SPHERES+1];
	Vec3d original_col_normals[NUM_SPHERES+1];

	// 周囲の影響の結果を格納する変数
	Matrix3d vel_effect[NUM_SPHERES+1];
	Vec3d vel_constant[NUM_SPHERES+1];

//	Matrix3d ang_effect[NUM_SPHERES+1];
	Vec3d ang_constant[NUM_SPHERES+1];

	// pointer data
	Vec3d pointer_pos;
	Vec3d pointer_vel;
	double pointer_massinv;
} Haptic_info;

// １と２を用意するのはスレッドで必要な排他アクセスを避け（待ちが発生するため）、
// 各プロセスの更新速度を保つため
Haptic_info info1;
Haptic_info info2;

// trueのときは構造体１を参照
// falseのときは構造体２を参照
bool current_valid_data = true; 

// 周囲の影響を有効にするかどうかを決めるフラグ
bool bSurroundEffect = false;

// SPIDARの位置を表示するかどうか
bool bDisplayPointer = true;

// 引数のsolidがすでに処理されているか調べる関数
bool checkProcessed(PHConstraint* target)
{
	for(int i = 0; i < num_process; i++)
	{
		if(process_map[i] == target)return true;
	}
	return false;
}

// 引数のsolidを処理済に登録する
void addProcessed(PHConstraint* constraint)
{
	process_map[num_process++] = constraint;
}

// 処理済リストを初期化
void initializeProcessed()
{
	// 処理済マップの初期化
	for(int i = 0; i < num_process; i++)
	{
		process_map[i] = NULL;
	}
	num_process = 0;
}

// 与えられた接触で、引数のsolidに接している剛体をしらべ、あれば返す
PHSolid* getAdjacentSolid(PHConstraint* constraint, PHSolid* solid, int* sign = NULL)
{
	if(constraint->solid[0]->solid == solid)
	{
		if(sign != NULL) *sign = -1;
		return constraint->solid[1]->solid;
	}
	else if(constraint->solid[1]->solid == solid)
	{
		if(sign != NULL) *sign = 1;
		return constraint->solid[0]->solid;
	}
	else return NULL;
}

// 提案手法の計算ポインタ用
void calculate_pointer_effect(std::vector<Vec3d> *forces, std::vector<Vec3d> *vec_r, Vec3d original_r, PHSolid* solid, Vec3d *C, Matrix3d *M, Vec3d *D, Matrix3d *N)
{
	// 変数の用意
	Vec3d sum_force = Vec3d();
	Vec3d sum_r_out_f = Vec3d();

	for(int i = 0; i < (int)forces->size(); i++) 
	{
		Vec3d r = (Vec3d)vec_r->at(i);
		Vec3d f = (Vec3d)forces->at(i);

		// 得られた値の合計値を計算 
		sum_force = sum_force + f;

		//　力と発生源からの外積を計算、それらの合計値を計算する
		sum_r_out_f = sum_r_out_f + r ^ f;
	}

	// まとめられる部分はまとめる
	// 定数項
	// C = sum(Fi)/m + g + w x (w x r) + (I^{-1} sum(ri x Fi)) x r
	*D = solid->GetInertiaInv() * sum_r_out_f;
	*C = Vec3d(sum_force * solid->GetMassInv() + scene->GetGravity() + solid->GetAngularVelocity() ^ (solid->GetAngularVelocity() ^ original_r) + *D ^ original_r);

	// T = I^(-1) * (r x F)のrを外積から行列にして外積を排除したもの。Fは含まない
	// T(ri) = I^{-1} X(ri) 
	*N = solid->GetInertiaInv() * Matrix3d(0, - original_r.z, original_r.y, original_r.z, 0, - original_r.x, -original_r.y, original_r.x, 0);
	
	// 求める行列
	// M = 1/m * E + (T1 x r T2 x r T3 x r)
	*M = Matrix3d(solid->GetMassInv() * Matrix3d().Unit() + Matrix3d(N->Ex() ^ original_r, N->Ey() ^ original_r, N->Ez() ^ original_r));
}

// 剛体が接触している剛体に与える力の計算
void calculate_solid_effect(std::vector<Vec3d> *forces, std::vector<Vec3d> *vec_r, PHSolid* solid, Vec3d original_r, Vec3d *output_force)
{
	// 変数の用意
	Vec3d sum_force = Vec3d();
	Vec3d sum_r_out_f = Vec3d();

	for(int i = 0; i < (int)forces->size(); i++) 
	{
		Vec3d r = (Vec3d)vec_r->at(i);
		Vec3d f = (Vec3d)forces->at(i);

		// 得られた値の合計値を計算
		sum_force = sum_force + f;

		//　力と発生源からの外積を計算、それらの合計値を計算する
		sum_r_out_f = sum_r_out_f + r ^ f;
	}

	// まとめられる部分はまとめる
	// C = (v_p)' - (sum(Fi) / m + w x (w x r) + I^{-1} sum(ri x Fi) x r
	// point_accelの計算がこれでよいのか確認する必要がある
	Vec3d point_accel = solid->GetAcceleration() + ((solid->GetAngularVelocity() - solid->GetOldAngularVelocity()) / scene->GetTimeStep()) ^ original_r;
	Vec3d C = point_accel - ((sum_force * solid->GetMassInv()) + scene->GetGravity() + solid->GetAngularVelocity() ^ (solid->GetAngularVelocity() ^ original_r) + (solid->GetInertiaInv() * sum_r_out_f) ^ original_r);

	// T = I^(-1) * (r x F)のrを外積から行列にして外積を排除したもの。Fは含まない
	// T(ri) = I^{-1} X(ri) 	
	Matrix3d T = solid->GetInertiaInv() * Matrix3d(0, - original_r.z, original_r.y, original_r.z, 0, - original_r.x, -original_r.y, original_r.x, 0);

	// M = 1/m * E + (T1 x r T2 x r T3 x r)
	Matrix3d M = solid->GetMassInv() * Matrix3d().Unit() + Matrix3d(T.Ex() ^ original_r, T.Ey() ^ original_r, T.Ez() ^ original_r);

	// 導かれる力を返す
	*output_force = M.inv() * C;
}

void calculate_counteractive_object_force(PHConstraints cs, int depth, PHSolid* solid, Vec3d original_r, Vec3d *output_force);

// 次の接してる剛体に進むときに必要な処理を行う
void gotoNextSolid(PHConstraints cs, PHConstraint* constraint, int depth, PHSolid* solid, std::vector<Vec3d> *vec_r, std::vector<Vec3d> *force, PHSolid* origin)
{
	// 処理済マップに登録する
	addProcessed(constraint);

	// 剛体の中心から力の作用点までのベクトルを計算する
	PHContactPoint* contact = DCAST(PHContactPoint, constraint);
	Vec3d f = Vec3d();
	Vec3d r = contact->pos - solid->GetCenterPosition();

	// 再帰的に次の処理に渡す
	calculate_counteractive_object_force(cs, depth, solid, r, &f);

	// 得られた情報の保存
	force->push_back(f);
	vec_r->push_back(contact->pos - origin->GetCenterPosition());
}

// 提示力の計算および提案手法に必要な情報を集めて構造体を作成する
void makeInfo(PHSolid* nearest, PHConstraint* constraint, Haptic_info* info, int sign, Vec3d C, Vec3d D, Matrix3d M, Matrix3d N)
{	
	// その剛体の中心から力の作用点までのベクトルを保存
	PHContactPoint* contact = DCAST(PHContactPoint, constraint);
	info->col_positions[info->num_collisions] = contact->pos;
	info->original_col_positions[info->num_collisions] = contact->pos;

	// 剛体からポインタへの接触面の法線を保存
	info->col_normals[info->num_collisions] = sign * contact->shapePair->normal;
	info->original_col_normals[info->num_collisions] = sign * contact->shapePair->normal;
	
	// 近傍剛体の現時点の速度と角速度を保存。
	// 別な変数を用意するのは、書き換えしやすいようにするため
	info->solid_velocity[info->num_collisions] = nearest->GetVelocity() +  nearest->GetAngularVelocity() ^ (info->col_positions[info->num_collisions] - nearest->GetCenterPosition());
	info->solid_angular_velocity[info->num_collisions] = nearest->GetAngularVelocity();
	info->solid_inertiainv[info->num_collisions] = nearest->GetInertiaInv();

	// 計算で得られた周囲の影響を表す変数をコピーする
	info->vel_constant[info->num_collisions] = C;
	info->ang_constant[info->num_collisions] = D;
	info->vel_effect[info->num_collisions] = M;
//	info->ang_effect[info->num_collisions] = N;

	// 接している剛体自身を登録
	info->nearest_solids[info->num_collisions] = nearest;

	// 衝突数をひとつ増やす
	info->num_collisions++;
}


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
		// 現在の値がすでに処理されたかチェック		
		bool process = checkProcessed(*it);

		// まだ処理されていない
		if(process == false)
		{
			// solidに接している剛体を取得
			PHSolid* nearest = getAdjacentSolid(*it, solid);

			// この剛体を含む接触があった
			if(nearest != NULL)
			{
				// その剛体に移動して再帰的に接触力の計算をする
				gotoNextSolid(cs, *it, depth+1, nearest, &vec_r, &forces, solid);
			}
		}
	}
	// 接している剛体に与える接触力の計算
	calculate_solid_effect(&forces, &vec_r, solid, original_r, output_force);
}

/**
 brief     	接触点の情報から周囲の情報による影響を計算する関数
 param		その時点での衝突情報PHConstraintsと衝突情報を格納する構造体Haptic_info型の変数を取る
 return 	なし
*/
void calculate_surround_effect(PHConstraints cs, Haptic_info *info)
{
	// 衝突の数をチェック
	if(cs.size() > 0)
	{
		PHSolid* nearest_solids[NUM_SPHERES+1];
		int signs[NUM_SPHERES+1];
		PHConstraint* consts[NUM_SPHERES+1];

		Vec3d vec_r_pointer[NUM_SPHERES+1];
		int num_solids = 0;

		// 処理済マップの初期化
		initializeProcessed();

		// pointerに接している剛体を検索し、取得する
		for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
		{
			// 法線の向きを修正する符号
			int sign = 1;

			// ポインタに接する剛体を取得
			PHSolid* nearest = getAdjacentSolid(*it, (PHSolid*)soPointer, &sign);

			// ポインタを含む剛体があったので処理
			if(nearest != NULL)
			{
				// 近傍剛体を保存
				nearest_solids[num_solids] = nearest;
				signs[num_solids] = sign;
				consts[num_solids] = *it;

				PHContactPoint* contact = DCAST(PHContactPoint, *it);	
				vec_r_pointer[num_solids++] = contact->pos - nearest->GetCenterPosition();
			}
		}

		// 初期化
		info->num_collisions = 0;

		// もしポインタに接する剛体がなければ
		// 力覚に影響がないのでリターンする
		if(num_solids == 0) return;

		// pointerに接している剛体が接している剛体を検索し、
		// それぞれ処理していく
		for(int j = 0; j < num_solids; j++)
		{
			// 並進用の計算結果を格納する変数
			Vec3d C;
			Matrix3d M;

			// 回転用の計算結果を格納する変数
			Vec3d D;
			Matrix3d N;

			std::vector<Vec3d> forces;
			std::vector<Vec3d> vec_r;

			// 先ほど登録した剛体(=ポインタに接している剛体)を取得			
			PHSolidIf* solid = (PHSolidIf*)nearest_solids[j];

			// すべての衝突を調査
			for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
			{
				bool process = checkProcessed(*it);

				// 処理済みマップからは見つからなかった(= 未処理 = 目的の剛体の可能性あり）
				if(process == false)
				{
					PHSolid* nearest = getAdjacentSolid(*it, (PHSolid*)solid);

					// ポインタに接する剛体に接する剛体があればそちらに進む
					if(nearest != NULL && nearest != soPointer)
					{						
						// 深さの初期化
						int depth = 1;

						// 接する剛体に移動し、再帰的に接触力を計算する
						gotoNextSolid(cs, *it, depth+1, nearest, &vec_r, &forces, (PHSolid*)solid);
					}
				}
			}
			
			// 提案手法の計算
			calculate_pointer_effect(&forces, &vec_r, vec_r_pointer[j], (PHSolid*)solid, &C, &M, &D, &N);

			// 計算結果を元にデータを作成
			makeInfo(nearest_solids[j], consts[j], info, signs[j], C, D, M, N);
		}
		return;
	}
	// 衝突がなかったので０を返してリターン
	else
	{
		info->num_collisions = 0;
		return;
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

			if(current_valid_data == true)
			{
				for(int i = 0; i < info1.num_collisions; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = &(*info1.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[0]->solid))DSTR << " --this solid is nearest object!!" << std::endl;
				}
			}
			else 
			{
				for(int i = 0; i < info2.num_collisions; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = &(*info2.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[0]->solid))DSTR << " --this solid is nearest object!!" << std::endl;
				}

			}
			DSTR << std::endl;

			// solid2
			DSTR << "solid2.mass = " << (*it)->solid[1]->solid->mass << std::endl;
			DSTR << "solid2.inertia = " << std::endl << (*it)->solid[1]->solid->inertia;
			DSTR << "solid2.velocity = " << (*it)->solid[1]->solid->GetVelocity()<< std::endl;
			DSTR << "solid2.angular_velocity = " << (*it)->solid[1]->solid->GetAngularVelocity() << std::endl;
			DSTR << "solid2.position = " << (*it)->solid[1]->solid->GetFramePosition() << std::endl;
			DSTR << "vector to working point from center = " << (contact->pos - (*it)->solid[1]->solid->GetCenterPosition()).unit() << std::endl;

			if(current_valid_data == true)
			{
				for(int i = 0; i < info1.num_collisions; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info1.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]->solid))DSTR << " --this solid is nearest object!!" << std::endl;
				}
			}
			else
			{
				for(int i = 0; i < info2.num_collisions; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info2.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]->solid))DSTR << " --this solid is nearest object!!" << std::endl;
				}
			}
			DSTR << std::endl;
		}
	}
}

/**
 brief  	提示力の計算とHaptic Device	へ反映. multimedia timerによって呼び出される
			内部的にはポインタとSPIDARをvirtual couplingでつなぎ、ポインタも更新している
 param	 	なし
 return 	なし
*/
void CALLBACK HapticRendering(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	// SPIDARの情報を格納する変数
	static Vec3d old_pos = Vec3d();
//	static Vec3d last_force = Vec3d();
//	static Vec3d last_torque = Vec3d();

	// SPIDAR更新幅
	static const float dt = (float)1.0f / HAPTIC_FREQ;

	// シミュレーションから得られる情報を格納する変数
	Haptic_info* info = NULL;

	// 状態によって参照先を切り替える
	if(current_valid_data)
	{
		info = &info1;
	}
	else
	{
		info = &info2;
	}

	// SPIDARの位置の更新
	spidarG6.Update(dt);//	USBの通信を行う．
	spidar_pos = view_rot * spidarG6.GetPos();		// SPIDAR座標からワールド座標にする

	// SPIDARの速度を計算
	Vec3d PointerVel = (spidar_pos - old_pos) * dt;
	old_pos = spidar_pos;

	// SPIDAR座標までのベクトルを作成
	Vec3d goal = spidar_pos - info->pointer_pos;

	// VR空間のポインタとSPIDARをvirtual couplingでつなげる
	Vec3d VCforce = K * goal + B * (PointerVel - info->pointer_vel);

	// soPointerの速度を更新
	info->pointer_vel = info->pointer_vel + info->pointer_massinv * VCforce * dt;

	// ポインタに加える力・トルクを格納する変数
	Vec3d pointer_force = Vec3d();

	if(info->num_collisions > 0)
	{
		// ポインタに生じたすべての接触について計算
		for(int i = 0; i < info->num_collisions; i++)
		{
			Vec3d feedback_force = - FORCE_COEFF * dot(VCforce, info->col_normals[i]) * info->col_normals[i];

			// 衝突対象のトルクを計算
			Vec3d solid_torque_vector = (info->col_positions[i] - info->nearest_solids[i]->GetCenterPosition()) ^ (-feedback_force);

			Vec3d accel;
			Vec3d ang_accel;

			if(bSurroundEffect == false || info->nearest_solids[i]->IsDynamical() == false)
			{
//				info->nearest_solids[i]->AddForce(-feedback_force, solid_torque_vector);
				accel = info->nearest_solids[i]->GetMassInv() * (-feedback_force);

				ang_accel = info->solid_inertiainv[i] * (solid_torque_vector);
			}

			// 周囲の影響を考慮
			else
			{
				// 加速度を計算
				accel = info->vel_effect[i] * (-feedback_force) + info->vel_constant[i];

				// 角加速度を計算 ang_effectを使うよりもsolid_torque_vectorより与えるトルクがわかっているのでそれを使ったほうがより正確なはず
	//			Vec3d ang_accel = info->ang_effect[i] * (-feedback_force) + info->ang_constant[i];
				ang_accel = info->solid_inertiainv[i] * (solid_torque_vector) + info->ang_constant[i];

			}

			// 速度を計算し、衝突点を更新する
			info->solid_velocity[i] = info->solid_velocity[i] + accel * dt;

			// 衝突点の移動距離を計算
			Vec3d dx = info->solid_velocity[i] * dt;

			// 衝突点を更新
			info->col_positions[i] = info->col_positions[i] + dx;

			// ポインタもそちらに動いたとして、位置を更新する
			// これはベクトルなので、すべての衝突点について足されると最終的に目的のベクトルが得られる
			info->pointer_pos = info->pointer_pos + dx;

			// 角速度を計算し、法線の向きを更新する
			info->solid_angular_velocity[i] = info->solid_angular_velocity[i] + ang_accel * dt;

			Vec3d dth = info->solid_angular_velocity[i] * dt;
			info->col_normals[i] = info->col_normals[i] + dth;
			info->col_normals[i] = info->col_normals[i].unit();

			if(fabs(feedback_force.x) > 300 || fabs(feedback_force.y) > 300 || fabs(feedback_force.z) > 300)
			{
				std::cout << "********" << std::endl;
				std::cout << i << ":force is " << feedback_force << std::endl;
				std::cout << "pointer_pos = " << info->pointer_pos << std::endl;
				std::cout << "dx = " << dx << std::endl;
				std::cout << "vel effect = " << info->vel_effect[i] << std::endl;
				std::cout << "vel const = " << info->vel_constant[i] << std::endl;
				std::cout << "accel = " << accel << std::endl;
				std::cout << "VCforce = " << VCforce << std::endl;
				std::cout << "goal = " << goal << std::endl;
				std::cout << "relative vel = " << PointerVel - info->pointer_vel << std::endl;
				std::cout << "normal = " << info->col_normals[i] << std::endl;
				std::cout << "dth = " << dth << std::endl;
				std::cout << "ang accel = " << ang_accel << std::endl;
				std::cout << std::endl;
				pointer_force = Vec3d();
			}

			
			// 提示力を前の値に追加
			pointer_force = pointer_force + feedback_force;
		}

		// spidarに力を加える
		if(bforce)
		{
			// SPIDARの空間と見ている空間が違うので行列を掛けて射影する
			Vec3d f = view_haptic * pointer_force;

			if(fabs(f.x) > 300 || fabs(f.y) > 300 || fabs(f.z) > 300)
			{
				std::cout << "force is " << f << std::endl;
				f = Vec3d();
			}
				spidarG6.SetForce(f, Vec3d());	
		}
		else spidarG6.SetForce(Vec3d(), Vec3d());
	}
	else
	{
		// 衝突がないので提示力をリセットする
		spidarG6.SetForce(Vec3d());

		// soPointerの位置を更新
		info->pointer_pos = info->pointer_pos + info->pointer_vel * dt;
	}

#if 0
	static int sec_counter = 0;
	// 一秒ごとにSPIDARの座標を表示する
	if(sec_counter % HAPTIC_FREQ == 0)
	{
//		std::cout << "spidar position = " << spidar_pos << std::endl;
//		std::cout << "spidar velocity = " << PointerVel << std::endl;
//		std::cout << " force = " << pointer_force << std::endl;
//		std::cout << "num collisions = " << info->num_collisions << std::endl;
//		std::cout << "pointer = " << soPointer->GetFramePosition() << std::endl;
//		std::cout << std::endl;
//		std::cout << "orientation = " << soPointer->GetOrientation() << std::endl;

		for(int i = 0; i < info->num_collisions; i++)
		{
			std::cout << "col pos = " << info->col_positions[i] << std::endl;
		}

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

// 提案手法によって生じた剛体の位置の誤差を修正する関数
void ErrorCorrection()
{
	Haptic_info* info = NULL;

	// 状態によって参照先を切り替える
	if(current_valid_data)
	{
		info = &info1;
	}
	else
	{
		info = &info2;
	}
	// ポインタが接しているすべての剛体について処理
	for(int i = 0; i < info->num_collisions; i++)
	{
		// 誤差をあらわすベクトルを計算
		Vec3d pos_error = info->col_positions[i] - info->original_col_positions[i];
		Vec3d ori_error = info->col_normals[i] - info->original_col_normals[i];

		if(info->nearest_solids[i]->IsDynamical())
		{
			// 誤差の分平行移動する
			info->nearest_solids[i]->SetFramePosition(info->nearest_solids[i]->GetFramePosition() + pos_error);
			info->nearest_solids[i]->SetOrientation(info->nearest_solids[i]->GetOrientation() * Quaterniond::Rot(ori_error));

			info->nearest_solids[i]->SetVelocity(info->solid_velocity[i]);
			info->nearest_solids[i]->SetAngularVelocity(info->solid_angular_velocity[i]);

			// 衝突点情報の上書き
			info->original_col_positions[i] = info->col_positions[i];
			info->original_col_normals[i] = info->col_normals[i];
		}
	}
}

void keyboard(unsigned char, int, int);

/**
 brief		VR内のプロキシの位置を更新する関数。
			この関数と力覚レンダリングを組み合わせることでポインタの更新をHAPTIC_FREQの周波数で更新できる。
 param		なし
 return		なし
 */
void UpdatePointer()
{
	Haptic_info* info = NULL;

	// 状態によって参照先を切り替える
	if(current_valid_data)
	{
		info = &info1;
	}
	else
	{
		info = &info2;
	}

	soPointer->SetFramePosition(info->pointer_pos);
	soPointer->SetVelocity(info->pointer_vel);
}

void CALLBACK StepSimulation(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	// ポインタの位置を更新
	UpdatePointer();

	// 局所的動力学計算によって生じた誤差を修正する
	ErrorCorrection();

	// シミュレーションを１ステップ進める
	scene->Step();

	// soPointerの角速度を０にする
	keyboard('p', 0, 0);

	// 衝突点を取得
	PHConstraints cs = GetContactPoints();

	// 衝突点情報を表示
//	show_collision_info(cs);

	// 衝突点から周囲の影響を計算
	// １を参照中。２を更新
	if(current_valid_data == true)
	{
		calculate_surround_effect(cs, &info2);
		info2.pointer_pos = soPointer->GetFramePosition();
		info2.pointer_vel = soPointer->GetVelocity();
		info2.pointer_massinv = soPointer->GetMassInv();
	}
	// ２を参照中。１を更新
	else
	{
		calculate_surround_effect(cs, &info1);
		info1.pointer_pos = soPointer->GetFramePosition();
		info1.pointer_vel = soPointer->GetVelocity();
		info1.pointer_massinv = soPointer->GetMassInv();
	}

	// 力覚スレッドのシミュレーションの変数の参照先を変更
	current_valid_data = current_valid_data?false:true;
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	// 再描画
	glutPostRedisplay();
}

/**
 brief		解析法の影響を受けない剛体を登録する。この剛体が生む接触は相手に接触力を与えない
 param		登録する剛体
 return		なし
*/
void AddInactiveSolid(PHSolidIf* soSolid)
{
	scene->GetConstraintEngine()->AddInactiveSolid(soSolid);
}

// SPIDARの位置と原点をあらわす点を描画する関数
void displayPointer()
{
	GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat orange[] = {1.0, 0.2, 0.0, 1.0};
	GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};

	GLfloat *color;

	// 状態によってSPIDARの球の色を変える
	if(bforce)
	{
		if(bSurroundEffect)color = orange;
		else color = red;
	}
	else
	{
		if(bSurroundEffect)color = blue;
		else color = white;
	}

	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		static Affinef pos_affine;
		Posef::Unit(spidar_pos).ToAffine(pos_affine);
		glMultMatrixf(pos_affine);
	
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, color);

		glutSolidSphere(1.0, 12, 9);
	glPopMatrix();

	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		Vec3f te = Vec3f();
		Affinef af = Affinef();
		Posef::Unit(te).ToAffine(af);
		glMultMatrixf(af);
		glutSolidSphere(1.0, 12, 9);
	glPopMatrix();
}

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);

	// フラグがONのときSPIDARの軌跡をデバッグ出力
	if(bDisplayPointer) displayPointer();

	render->EndScene();
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
		timeKillEvent(FTimerId1);
		timeKillEvent(FTimerId1);
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

		// メッセージを出力
		std::cout << "force ";
		if(bforce) std::cout << "on";
		else std::cout << "off";
		std::cout << std::endl;
	}
	// soPointerとSPIDARの場所を初期値に戻し、キャリブレーションを行う(virtual coupling用）
	else if(key == 'i')
	{
		soPointer->SetVelocity(Vec3d());
		soPointer->SetAngularVelocity(Vec3d());
		soPointer->SetPose(Posed());
		spidarG6.Calib();
	}
	// sphereの場所をリセット
	else if(key == 'r')
	{
		for(int i = 0; i < NUM_SPHERES; i++)
		{
			soSphere[i]->SetPose(Posed());
			soSphere[i]->SetFramePosition(Vec3f(0, 15+5*(i+1), 0));
			soSphere[i]->SetAngularVelocity(Vec3d());
			soSphere[i]->SetVelocity(Vec3d());
		}
	}
	// 周囲の影響を有効にするかどうか
	else if(key == 's')
	{
		bSurroundEffect = !bSurroundEffect;

		// メッセージを出力
		std::cout << "surrounding effect ";
		if(bSurroundEffect)std::cout << "on";
		else std::cout << "off";
		std::cout << std::endl;
	}
	else if(key == 'p')
	{
		soPointer->SetAngularVelocity(Vec3d());
	}
	// 現在のバーチャルカップリングのKとBの値を表示する
	else if(key == 'l')
	{
		std::cout << "k = " << K << " b = " << B << std::endl;
	}
	// バーチャルカップリングの係数のKを1増加して現在の状態を表示する
	else if(key == 'k')
	{
		K += 1;
		keyboard('l', 0, 0);
	}
	// バーチャルカップリングの係数のBを1増加して現在の状態を表示する
	else if(key == 'b')
	{
		B += 1;
		keyboard('l', 0, 0);
	}
	// バーチャルカップリングの係数のKを1減少して現在の状態を表示する
	else if(key == 'j')
	{
		K -= 1;
		keyboard('l', 0, 0);
	}
	// バーチャルカップリングの係数のBを1減少して現在の状態を表示する
	else if(key == 'v')
	{
		B -= 1;
		keyboard('l', 0, 0);
	}
	// 提示力を調節する値を0.1増加する
	else if(key == 'm')
	{
		FORCE_COEFF += 0.1;
		std::cout << "coeff = " << FORCE_COEFF << std::endl;
	}
	// 提示力を調節する値を0.1減少する
	else if(key == 'n')
	{
		FORCE_COEFF -= 0.1;
		std::cout << "coeff = " << FORCE_COEFF << std::endl;
	}
	// SPIDARの位置と原点をあらわす球の表示をON/OFFする
	else if(key == 'd')
	{
		bDisplayPointer = !bDisplayPointer;
		std::cout << "pointer is ";
		if(!bDisplayPointer)std::cout << "not ";
		std::cout << "displayed" << std::endl;
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
	FTimerId1 = timeSetEvent(1000 / HAPTIC_FREQ,    // タイマー間隔[ms]
	                        1,   // 時間分解能
		                    HapticRendering,//コールバック関数
		                    (DWORD)&time,//ユーザー定義の値
		                    TIME_PERIODIC //単発(TIME_ONESHOT) or 繰り返し(TIME_PERIODIC)
		                    );

	// simulationスレッドの生成・開始
	FTimerId2 = timeSetEvent(1000 / SIMULATION_FREQ,    // タイマー間隔[ms]
	                        1,   // 時間分解能
		                    StepSimulation,//コールバック関数
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
	sd.timeStep = (double)1.0 / SIMULATION_FREQ;
	scene = phSdk->CreateScene(sd);				// シーンの作成
	PHSolidDesc desc;
	desc.mass = 0.5;
	desc.inertia *= 2.0;

	// Solidの作成
	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere.push_back(scene->CreateSolid(desc));		// 剛体をdescに基づいて作成
	}

	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetDynamical(false);

	desc.mass = 2.0;
	soPointer = scene->CreateSolid(desc);

	//	形状の作成
	CDBoxIf* floor=NULL;
	CDSphereIf* sphere=NULL;

	{
		CDSphereDesc sd;
		sd.radius = 1.25;
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
	AddInactiveSolid(soPointer);
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

	int window = glutCreateWindow("HapticTest");
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
	Affinef view;
	view.Pos() = Vec3f(0.0, 5.0, -20.0);								// eye
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));			// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);
	
	setLight();

	/// SPIDAR座標をビュー座標に直す変数
	view_rot = view.inv().Rot() * SPIDAR_SCALE;
	view_haptic = view.Rot();
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
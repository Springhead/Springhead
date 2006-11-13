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
#include <GL/glut.h>
#include <iostream>

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


#define LIMIT_DEPTH 100				// 予測シミュレーションを行う剛体取得の深さ上限
#define NUM_PREDICT_ITERATE 1		// 予測シミュレーションのイテレート回数


#ifdef _DEBUG
	#define SIMULATION_FREQ	100         // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		500			// 力覚スレッドの周期Hz
	float Km = 1000;					// virtual couplingの係数
	float Bm = 100;						// 並進

	float Kr = 40;						// 回転
	float Br = 15;

#elif _WINDOWS
	#define SIMULATION_FREQ	100         // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		1000		// 力覚スレッドの周期Hz
	float Km = 2000;					// virtual couplingの係数
	float Bm = 100;

	float Kr = 40;						// 回転
	float Br = 15;
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

// 仮想世界で扱う剛体
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

Quaterniond spidar_ori;

bool bforce = false;
MMRESULT FTimerId1;
MMRESULT FTimerId2;

/*
// 再帰計算をした情報を格納するテーブル
PHConstraint* process_map[NUM_SPHERES+1];
int num_process = 0;
*/

// 力覚計算に必要なデータを集めた構造体
// 力覚計算を高速で行えるように
// 関数呼び出しのオーバーヘッドを避けるために
// シミュレーションでデータを格納しておく
typedef struct {
	// collision solid data
	PHSolid* nearest_solids[NUM_SPHERES+1];
	Vec3d solid_velocity[NUM_SPHERES+1];
	Vec3d solid_angular_velocity[NUM_SPHERES+1];
	Vec3d solid_center_position[NUM_SPHERES+1];
	double solid_massinv[NUM_SPHERES+1];
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

	Quaterniond pointer_ori;
	Vec3d pointer_angvel;
	Matrix3d pointer_inertiainv;
	Vec3d pointer_center;
} HapticInfo;

// １と２を用意するのはスレッドで必要な排他アクセスを避け（待ちが発生するため）、
// 各プロセスの更新速度を保つため
HapticInfo info1;
HapticInfo info2;

// trueのときは構造体１を参照
// falseのときは構造体２を参照
bool current_valid_data = true; 

// 周囲の影響を有効にするかどうかを決めるフラグ
bool bSurroundEffect = false;

// SPIDARの位置を表示するかどうか
bool bDisplayPointer = true;

// KとBの値どちらの変更を有効にするか
// trueの場合並進
// falseの場合回転
bool bMode = true;

/*
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
*/

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

// 提示力の計算および提案手法に必要な情報を集めて構造体を作成する
void MakeHapticInfo(HapticInfo *info, PHConstraints pointer_consts, std::vector<std::pair<PHSolid *, Vec3d> > pointer_solids, std::vector<Matrix3d> matrices, std::vector<Vec3d> vecs)
{
	info->num_collisions = 0;

	for(int i = 0; i < (int)pointer_consts.size(); i++)
	{
		PHContactPoint* contact = DCAST(PHContactPoint, pointer_consts.at(i));
		info->col_positions[info->num_collisions] = contact->pos;
//		info->original_col_positions[info->num_collisions] = info->col_positions[info->num_collisions];

		PHSolid* so = pointer_solids.at(i).first;

		info->solid_velocity[info->num_collisions] = so->GetVelocity() + so->GetAngularVelocity() ^ (info->col_positions[info->num_collisions] - so->GetCenterPosition());
		info->solid_angular_velocity[info->num_collisions] = so->GetAngularVelocity();
		info->solid_center_position[info->num_collisions] = so->GetCenterPosition();
		info->solid_massinv[info->num_collisions] = so->GetMassInv();
		info->solid_inertiainv[info->num_collisions] = so->GetInertiaInv();
		
		info->nearest_solids[info->num_collisions] = so;

		info->col_normals[info->num_collisions] = pointer_solids.at(i).second.unit();
//		info->original_col_normals[info->num_collisions] = info->col_normals[info->num_collisions];

		info->num_collisions++;
	}

	/*
	// 計算で得られた周囲の影響を表す変数をコピーする
	info->vel_constant[info->num_collisions] = C;
	info->ang_constant[info->num_collisions] = D;
	info->vel_effect[info->num_collisions] = M;
//	info->ang_effect[info->num_collisions] = N;
	*/
}

/**
 brief     	物体の接触点を取得する関数
 param		なし
 return 	接触点の集合
*/
PHConstraints GetContactPoints(PHSceneIf* scene1)
{
	return scene1->GetConstraintEngine()->GetContactPoints();
}

// 再帰的に接している剛体を取得する関数
void RecursiveSolidRetrieval(PHSolid* solid, set<PHConstraint *>* relative_solid_consts, std::set<PHSolidInfoForLCP *>* relative_solids, int depth)
{
	PHConstraints cs = GetContactPoints(scene);

	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
	{
		// 与えられた剛体から、その剛体に接触する剛体を取得する
		PHSolid* col_solid = getAdjacentSolid(*it, solid);

		// もしその剛体が存在し、ポインタではない場合
		// ポインタの場合はGetSolidsFromPointerで取得する
		if(col_solid != NULL && col_solid != soPointer)
		{
			// 接触自体を追加
			PHConstraint* c = *it;
			pair<set<PHConstraint *>::iterator, bool> p = relative_solid_consts->insert(c);

			// 今追加したデータは重複してなかった
			if(p.second != false)
			{
				relative_solids->insert(c->solid[0]);
				relative_solids->insert(c->solid[1]);

				// 動かない剛体ではなく、かつ取得上限まで達していなかった
				if(col_solid->IsDynamical() == true && depth < LIMIT_DEPTH)
				{
					// 次の剛体にすすむ
					RecursiveSolidRetrieval(col_solid, relative_solid_consts, relative_solids, depth++);
				}
			}
		}
	}
}

// ポインタからスタートして必要な剛体をすべて取得してくる関数
void GetSolidsFromPointer(std::vector<std::pair<PHSolid *, Vec3d> >* pointer_solids, PHConstraints* pointer_consts, std::set<PHSolidInfoForLCP *>* relative_solids, set<PHConstraint *>* relative_solid_consts)
{
	// 衝突点を取得
	PHConstraints cs = GetContactPoints(scene);

	// すべての衝突点について調査
	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
	{
		int sign = 1;

		// ポインタを含む接触を取得
		PHSolid* col_solid = getAdjacentSolid(*it, (PHSolid*)soPointer, &sign);
		
		// 発見できた
		if(col_solid != NULL)
		{
			// 剛体からポインタに向かう法線を計算
			PHContactPoint* cp = DCAST(PHContactPoint, *it);
			Vec3d normal = sign *  cp->shapePair->normal;

			// 引数で与えられた配列に接触する剛体と法線のペアを格納
			pointer_solids->push_back(std::pair<PHSolid*, Vec3d>(col_solid, normal));

			// ポインタと接している剛体のPHSolidInfoForLCPを保存
			if(col_solid == (*it)->solid[0]->solid)
			{
				relative_solids->insert((*it)->solid[0]);
			}
			else
			{
				relative_solids->insert((*it)->solid[1]);
			}

			// 接触自体を格納
			pointer_consts->push_back(*it);
			relative_solid_consts->insert(*it);

			// 次の剛体にすすむ
			if(col_solid->IsDynamical() == true)RecursiveSolidRetrieval(col_solid, relative_solid_consts, relative_solids, 0);
		}
	}
}

// 先送りシミュレーションをする関数
std::vector<Vec3d> PredictSimulation(PHConstraints pointer_consts, set<PHConstraint *> relative_solid_consts, std::vector<std::pair<PHSolid *, Vec3d> > pointer_solids, std::set<PHSolidInfoForLCP *> all_solids, Vec3d force_vec, int index = -1)
{
	double dt = scene->GetTimeStep();

	// 単位力を加える
	if(index != -1)
	{
		PHConstraint *constraint = pointer_consts.at(index);
		PHContactPoint* contact = DCAST(PHContactPoint, constraint);

		std::pair<PHSolid*, Vec3d> p = pointer_solids.at(index);
		PHSolid* so = p.first;

		so->AddForce(force_vec, contact->pos);
	}

	// すべての剛体をセットアップ
	for(std::set<PHSolidInfoForLCP *>::iterator it = all_solids.begin(); it != all_solids.end(); it++)
	{
		PHSolidInfoForLCP* solid = *it;
		solid->SetupDynamics(dt);
	}

	// 接触をセットアップ
	for(set<PHConstraint *>::iterator it = relative_solid_consts.begin(); it != relative_solid_consts.end(); it++)
	{
		(*it)->SetupDynamics(dt, scene->GetConstraintEngine()->correctionRate, scene->GetConstraintEngine()->shrinkRate);
	}

	for(int i = 0; i < NUM_PREDICT_ITERATE; i++)
	{
		// すべての接触をiterate
		for(set<PHConstraint *>::iterator it = relative_solid_consts.begin(); it != relative_solid_consts.end(); it++)
		{
			(*it)->IterateDynamics();
		}
	}
	
	std::vector<Vec3d> effects;

	// 影響を観測
	for(std::vector<std::pair<PHSolid *, Vec3d> >::iterator it = pointer_solids.begin(); it != pointer_solids.end(); it++)
	{
		Vec3d dv;
		Vec3d dw;

		std::pair<PHSolid*, Vec3d> p = *it;
		PHSolidInfoForLCP* si = DCAST(PHSolidInfoForLCP, (*it).first);

		// ここではdvを見ているが、変更の可能性あり
		dv = si->dv + si->dv0;
		dw = si->dw + si->dw0;

		effects.push_back(dv);
		effects.push_back(dw);
	}

	// 加えた単位力を元にもどす
	if(index != -1)
	{
		PHConstraint *constraint = pointer_consts.at(index);
		PHContactPoint* contact = DCAST(PHContactPoint, constraint);

		std::pair<PHSolid*, Vec3d> p = pointer_solids.at(index);
		PHSolid* so = p.first;
		so->nextForce -= force_vec;
		so->nextTorque -= (contact->pos - so->GetCenterPosition()) ^ force_vec;
	}

	// 影響をreturn
	return effects;
}

// 提案手法のために先送りシミュレーションを計画的に行ってくれる関数
void PredictSimulations(PHConstraints pointer_consts, set<PHConstraint *> relative_solid_consts, std::vector<std::pair<PHSolid *, Vec3d> > pointer_solids, std::set<PHSolidInfoForLCP *> all_solids, std::vector<Matrix3d> *mat, std::vector<Vec3d> *vec)
{
	std::vector<Matrix3d> effects;

	// 加える力を０ベクトルとして定数項を取得
	std::vector<Vec3d> b = PredictSimulation(pointer_consts, relative_solid_consts, pointer_solids, all_solids, Vec3d());

	// 接触数＝列数だけ繰り返す
	for(int i = 0; i < (int)pointer_consts.size(); i++)
	{
		std::vector<Vec3d> vec_x;
		std::vector<Vec3d> vec_y;
		std::vector<Vec3d> vec_z;

		// ある単位ベクトルを加えてその結果をあらわすベクトルを取得
		vec_x = PredictSimulation(pointer_consts, relative_solid_consts, pointer_solids, all_solids, Vec3d(1, 0, 0), i);
		vec_y = PredictSimulation(pointer_consts, relative_solid_consts, pointer_solids, all_solids, Vec3d(0, 1, 0), i);
		vec_z = PredictSimulation(pointer_consts, relative_solid_consts, pointer_solids, all_solids, Vec3d(0, 0, 1), i);

		// このループで縦ベクトルを計算し格納
		for(int j = 0; j < (int)b.size(); j += 2)
		{
			Matrix3d v = Matrix3d(vec_x[j] - b[j], vec_y[j] - b[j], vec_z[j] - b[j]);
			Matrix3d w = Matrix3d(vec_x[j+1] - b[j+1], vec_y[j+1] - b[j+1], vec_z[j+1] - b[j+1]);

			effects.push_back(v);
			effects.push_back(w);
		}
	}

	*mat = effects;
	*vec = b;
}

// 周囲の影響を計算し、引数の変数に格納する関数
void calculate_surround_effect(HapticInfo* info)
{
	std::vector<std::pair<PHSolid *, Vec3d> > pointer_solids;
	PHConstraints pointer_consts;

	std::set<PHSolidInfoForLCP *> relative_solids;
	set<PHConstraint *> relative_solid_consts;

	// ポインタからスタートして必要な剛体をすべて取得してくる関数
	GetSolidsFromPointer(&pointer_solids, &pointer_consts, &relative_solids, &relative_solid_consts);

	std::vector<Matrix3d> matrices;
	std::vector<Vec3d> vecs;

	// 力を加えてみて動かし、影響を観測する関数
	PredictSimulations(pointer_consts, relative_solid_consts, pointer_solids, relative_solids, &matrices, &vecs);

	// 接触の情報を計算する
	MakeHapticInfo(info, pointer_consts, pointer_solids, matrices, vecs);
}

/**
 brief     	物体の接触点の情報を表示する関数
 param		なし
 return 	なし
*/
void show_collision_info()
{
	// 衝突点を取得
	PHConstraints cp = GetContactPoints(scene);

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
	// SPIDAR更新幅
	static const double dt = (double)1.0 / HAPTIC_FREQ;

	// シミュレーションから得られる情報を格納する変数
	HapticInfo* info = NULL;

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

	// SPIDARの情報を格納する変数
	static Vec3d old_pos = spidar_pos;

	// SPIDARの速度を計算
	// 速度の定義では v = dx / dt だが
	// dtで割らないほうが安定みたいなので
	// 差分だけを使うことにする
	Vec3d SPIDARVel = spidar_pos - old_pos;
	old_pos = spidar_pos;

	// SPIDARの位置までのベクトルを作成
	Vec3d goal = spidar_pos - info->pointer_pos;

	// VR空間のポインタとSPIDARをvirtual couplingでつなげる
	Vec3d VCforce = Km * goal + Bm * (SPIDARVel - info->pointer_vel);

	// soPointerの速度を更新
	info->pointer_vel = info->pointer_vel + info->pointer_massinv * VCforce * dt;


	// SPIDARの姿勢を更新
	Quaterniond qv;
	qv.FromMatrix(view_rot);
	spidar_ori = qv * spidarG6.GetOri();

	static Quaterniond old_ori = spidar_ori;

	// SPIDARの回転速度を計算
	// 前の姿勢から現在の姿勢に変換する４元数を計算
	Quaterniond SPIDARAngVel = spidar_ori * old_ori.Inv();
	old_ori = spidar_ori;

	// 現在のポインタの姿勢からSPIDARの姿勢までの回転を表す４元数を計算
	Quaterniond ang_goal = spidar_ori * info->pointer_ori.Inv();

	// 回転についてのバーチャルカップリング
	Vec3d VCTorque = Kr * ang_goal.Rotation() + Br * (SPIDARAngVel.Rotation() - info->pointer_angvel);

	// 角速度を更新
	info->pointer_angvel = info->pointer_angvel + info->pointer_inertiainv * VCTorque * dt;

	if(info->num_collisions > 0)
	{
		// ポインタに加える力・トルクを格納する変数
		Vec3d pointer_force = Vec3d();
		Vec3d pointer_torque = Vec3d();

		// 接触によって動いた量の総和を格納する変数
		Vec3d pointer_dx = Vec3d();
//		Vec3d pointer_dth = Vec3d();

		// ポインタに生じたすべての接触について計算
		for(int i = 0; i < info->num_collisions; i++)
		{
			// 提示力および提示トルクの計算
			// 衝突点での法線の逆方向にカップリング力を射影
			// normalは単位ベクトル前提
			Vec3d feedback_force = - FORCE_COEFF * dot(VCforce, info->col_normals[i]) * info->col_normals[i];
			Vec3d feedback_torque = (info->col_positions[i] - info->pointer_center) ^ feedback_force;

			// 衝突対象のトルクを計算
			Vec3d solid_torque_vector = (info->col_positions[i] - info->solid_center_position[i]) ^ (-feedback_force);

			Vec3d accel;
			Vec3d ang_accel;

			// ここで加速度の計算を行う
			// 加速度の計算にどちらを使うかはそのときのモードによって変わる
			// IsDynamical == falseな剛体は移動しないので加速度はない
			if(info->nearest_solids[i]->IsDynamical() == false)
			{
				accel = Vec3d();
				ang_accel = Vec3d();
			}
				// 周囲の影響を考慮しない局所的動力学計算
				// ポインタに接する剛体のみ局所的に高速で更新
				else if(bSurroundEffect == false)
				{
					// 毎回AddForceをするとCorrectionする必要がない
					// シミュレーションでは周囲の状況を考慮した更新が行えるが、
					// HapticRenderingでの高速な仮想壁の更新には加速度が反映されない
					// さらにユーザが実際に力覚を感じるのはこの状況であるため
					// 重要であると思う
					info->nearest_solids[i]->AddForce(-feedback_force, solid_torque_vector);
					accel = info->solid_massinv[i] * (-feedback_force);
					ang_accel = info->solid_inertiainv[i] * (solid_torque_vector);
				}
				// 周囲の影響を考慮する
				// ポインタが接触している仮想壁の加速度は
				// シミュレーションから得られた値を元に計算される
				else
				{
					// 加速度を計算
					accel = info->vel_effect[i] * (-feedback_force) + info->vel_constant[i];

					// 角加速度を計算 ang_effectを使うよりもsolid_torque_vectorより与えるトルクがわかっているのでそれを使ったほうがより正確なはず
//					Vec3d ang_accel = info->ang_effect[i] * (-feedback_force) + info->ang_constant[i];
					ang_accel = info->solid_inertiainv[i] * (solid_torque_vector) + info->ang_constant[i];
				}

			// 速度を計算し、衝突点を更新する
			info->solid_velocity[i] = info->solid_velocity[i] + accel * dt;

			// 衝突点の移動距離を計算
			Vec3d dx = info->solid_velocity[i] * dt;

			// 衝突点を更新
			info->col_positions[i] = info->col_positions[i] + dx;
			
			// 剛体の重心を更新
			// 回転計算のrベクトルのため
			info->solid_center_position[i] = info->solid_center_position[i] + dx;

			// ポインタの移動のために仮想壁の移動量を保存しておく
			pointer_dx += dx;

			// 角速度を更新
			info->solid_angular_velocity[i] = info->solid_angular_velocity[i] + ang_accel * dt;

			// 移動量を計算 theta = (w * dt) x r
			Vec3d dth = (info->solid_angular_velocity[i] * dt) ^ (info->col_positions[i] - info->solid_center_position[i]);

			// ベクトルの加算による法線の更新
			info->col_normals[i] = (info->col_normals[i] + dth).unit();

#if 0
			// 提示力が大きすぎる場合提示力を減らす
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
				feedback_force /= 300;
			}
#endif
			
			// 提示力を前の値に追加
			pointer_force = pointer_force + feedback_force;
			pointer_torque = pointer_torque + feedback_torque;
		}

		// spidarに力を加える
		if(bforce)
		{
			// SPIDARの空間と見ている空間が違うので行列を掛けて射影する
			Vec3d f = view_haptic * pointer_force;
//			Vec3d t = view_haptic * pointer_torque;

			// 提示力が大きすぎる場合は小さくする
			if(fabs(f.x) > 300 || fabs(f.y) > 300 || fabs(f.z) > 300)
			{
				std::cout << "force  is " << f;
				f /= 300;
				std::cout << "; changed to " << f << std::endl;
//				std::cout << "torque is " << t;
//				t /= 300;
//				std::cout << "; changed to " << t << std::endl;
			}
			spidarG6.SetForce(f,- VCTorque);	
		}
		else spidarG6.SetForce(Vec3d(), Vec3d());

		// 仮想壁の移動に沿ってポインタもそちらに動いたとして、位置を更新する
		// 本来速度によって進むベクトルの方向に射影し
		// その射影したベクトルと速度から得られるベクトルのノルムの小さいほうをとる
		Vec3d temp = info->pointer_vel * dt;
		pointer_dx = dot(pointer_dx, temp) * temp / temp.norm();

		// 仮想壁の移動の方が大きかった
		if(pointer_dx.norm() >= temp.norm())
		{
			// 速度で動いた量だけ更新
			info->pointer_pos += temp;
			info->pointer_center += temp;

//			info->pointer_ori = info->pointer_ori * Quaterniond::Rot(info->pointer_angvel * dt);
		}
		// 速度の移動の方が大きかった
		else
		{
			// 仮想壁で動いた分だけ更新
			info->pointer_pos += pointer_dx;
			info->pointer_center += pointer_dx;

//			info->pointer_ori = info->pointer_ori * Quaterniond::Rot(info->pointer_angvel * dt);
			// 速度は運動量と力積の関係より受けた力の分だけ弱くなる
			// 式から導出できる
			info->pointer_vel += info->pointer_massinv * pointer_force * dt;
		}
	}
	else
	{
		// 衝突がないので提示力をリセットする
		spidarG6.SetForce(Vec3d());

		// soPointerの位置を更新
		Vec3d dx = info->pointer_vel * dt;
		info->pointer_pos = info->pointer_pos + dx;
		info->pointer_center += info->pointer_pos + dx;

		info->pointer_ori = Quaterniond::Rot(info->pointer_angvel * dt) * info->pointer_ori;
		info->pointer_ori = info->pointer_ori.unit();
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

//		std::cout << "orientation = " << info->pointer_ori << std::endl;
//		std::cout << "angvel = " << info->pointer_angvel << std::endl;

		std::cout << "inertia = " << info->pointer_inertiainv << std::endl;

		for(int i = 0; i < info->num_collisions; i++)
		{
			std::cout << "normal = " << info->col_normals[i] << std::endl;
		}

		sec_counter = 0;	
	}
	sec_counter++;									// カウンターの更新
#endif
}

/**
 brief		特定のソリッドから重力をなくす
 param		重力をなくすソリッド
 return		なし
*/
void RemoveGravity(PHSolidIf* solid)
{
	solid->SetGravity(false);
}
/*
// 提案手法によって生じた剛体の位置の誤差を修正する関数
void ErrorCorrection()
{
	HapticInfo* info = NULL;

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
*/

void keyboard(unsigned char, int, int);

/**
 brief		VR内のプロキシの位置を更新する関数。
			この関数と力覚レンダリングを組み合わせることでポインタの更新をHAPTIC_FREQの周波数で更新できる。
 param		なし
 return		なし
 */
void UpdatePointer()
{
	HapticInfo* info = NULL;

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

	soPointer->SetOrientation(info->pointer_ori);
	soPointer->SetAngularVelocity(info->pointer_angvel);
}

void CALLBACK StepSimulation(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	// ポインタの位置を更新
	UpdatePointer();

	// 局所的動力学計算によって生じた誤差を修正する
//	ErrorCorrection();

	// シミュレーションを１ステップ進める
	scene->Step();

	// soPointerの角速度を０にする
//	keyboard('p', 0, 0);

	// 衝突点情報を表示
//	show_collision_info();

	// 衝突点から周囲の影響を計算
	// １を参照中。２を更新
	if(current_valid_data == true)
	{
		calculate_surround_effect(&info2);
		info2.pointer_pos = soPointer->GetFramePosition();
		info2.pointer_vel = soPointer->GetVelocity();
		info2.pointer_massinv = soPointer->GetMassInv();
		info2.pointer_angvel = soPointer->GetAngularVelocity();
		info2.pointer_inertiainv = soPointer->GetInertiaInv();
		info2.pointer_ori = soPointer->GetOrientation();
		info2.pointer_center = soPointer->GetCenterPosition();
	}
	// ２を参照中。１を更新
	else
	{
		calculate_surround_effect(&info1);
		info1.pointer_pos = soPointer->GetFramePosition();
		info1.pointer_vel = soPointer->GetVelocity();
		info1.pointer_massinv = soPointer->GetMassInv();
		info1.pointer_angvel = soPointer->GetAngularVelocity();
		info1.pointer_inertiainv = soPointer->GetInertiaInv();
		info1.pointer_ori = soPointer->GetOrientation();
		info1.pointer_center = soPointer->GetCenterPosition();
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
void AddInactiveSolid(PHSceneIf* scene1, PHSolidIf* soSolid)
{
	scene1->GetConstraintEngine()->AddInactiveSolid(soSolid);
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
	/*
	else if(key == 's')
	{
		bSurroundEffect = !bSurroundEffect;

		// メッセージを出力
		std::cout << "surrounding effect ";
		if(bSurroundEffect)std::cout << "on";
		else std::cout << "off";
		std::cout << std::endl;
	}
	*/
	else if(key == 'p')
	{
		soPointer->SetAngularVelocity(Vec3d());
	}
	// 現在のバーチャルカップリングのKとBの値を表示する
	else if(key == 'l')
	{
		std::cout << "Km = " << Km << " Bm = " << Bm << std::endl;
		std::cout << "Kr = " << Kr << " Br = " << Br << std::endl;
	}
	// バーチャルカップリングの係数のKを1増加して現在の状態を表示する
	else if(key == 'k')
	{
		if(bMode)Km += 1;
		else Kr += 1;
		keyboard('l', 0, 0);
	}
	// バーチャルカップリングの係数のBを1増加して現在の状態を表示する
	else if(key == 'b')
	{
		if(bMode)Bm += 1;
		else Br += 1;
		keyboard('l', 0, 0);
	}
	// バーチャルカップリングの係数のKを1減少して現在の状態を表示する
	else if(key == 'j')
	{
		if(bMode)Km -= 1;
		else Kr -= 1;
		keyboard('l', 0, 0);
	}
	// バーチャルカップリングの係数のBを1減少して現在の状態を表示する
	else if(key == 'v')
	{
		if(bMode)Bm -= 1;
		else Br -= 1;
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
	else if(key == 'e')
	{
		bMode = !bMode;
		if(bMode)std::cout << "movement ";
		else std::cout << "rotation ";
		std::cout << "edit mode" << std::endl;
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

	DWORD time = 0;

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

		// 球形のポインタ
		/*
		soPointer->AddShape(sphere);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));
		*/

		sd.radius = 2.0;
		sphere = DCAST(CDSphereIf,phSdk->CreateShape(sd));

		CDBoxDesc bd;
		bd.boxsize = Vec3f (30.0, 1.0, 30.0);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soFloor->AddShape(floor);
		soFloor->SetFramePosition(Vec3f(0,-2.5,0));

		bd.boxsize = Vec3f(2, 2, 2);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soPointer->AddShape(floor);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));
	}	

	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere.at(sphereCnt)->AddShape(sphere);
		soSphere.at(sphereCnt)->SetFramePosition(Vec3f(0, 15+5*sphereCnt, 0));
	}
	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	// pointerの重力を無効にする
	RemoveGravity(soPointer);

	// pointerと接触する剛体との解析法による計算をやめ、
	// 力覚で処理するようにする
	AddInactiveSolid(scene, soPointer);
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

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
#include <time.h>
#include <stdio.h>

#include "HIS/HIS.h"
#include "Physics/PHConstraintEngine.h"
#include "Physics/PHConstraint.h"
#include "Physics/PHContactPoint.h"
#include <Physics/PHSpatial.h>

#include <mmsystem.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;

#define ESC				27			// ESC key
//#define EXIT_TIMER		10000		// 実行ステップ数
#define WINSIZE_WIDTH	480			// ウィンドウサイズ(width)
#define WINSIZE_HEIGHT	360			// ウィンドウサイズ(height)
#define NUM_SPHERES		2			// sphere数
#define SPIDAR_SCALE	70			// SPIDARのVE内での動作スケール


#define LIMIT_DEPTH 100				// 予測シミュレーションを行う剛体取得の深さ上限
#define NUM_PREDICT_ITERATE 1		// 予測シミュレーションのイテレート回数

#define NUM_COLLISIONS 10			// ポインタへの許容接触数

#ifdef _DEBUG
	#define SIMULATION_FREQ	80         // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		100			// 力覚スレッドの周期Hz
	float Km = 400;						// virtual couplingの係数
	float Bm = 30;						// 並進

	float Kr = 40;						// 回転
	float Br = 13;

#elif _WINDOWS
	#define SIMULATION_FREQ	80         // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		1000		// 力覚スレッドの周期Hz
//	float Km = 2000;					// virtual couplingの係数
	float Km = 400;
	float Bm = 30;

	float Kr = 40;						// 回転
	float Br = 13;
#endif

// 提示力と剛体に提示する力を直接変化させる定数
double FORCE_COEFF =		1.0;

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
vector<PHSolidIf*> soSphere; 
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


// 力覚計算に必要なデータを集めた構造体
// 力覚計算を高速で行えるように
// シミュレーションでデータを格納しておく
typedef struct {
	// collision solid data
	PHSolid* nearest_solids[NUM_COLLISIONS];
	Vec3d solid_velocity[NUM_COLLISIONS];
	Vec3d solid_angular_velocity[NUM_COLLISIONS];
	Vec3d solid_center_position[NUM_COLLISIONS];
	double solid_massinv[NUM_COLLISIONS];
	Matrix3d solid_inertiainv[NUM_COLLISIONS];

	// collision data
	Vec3d col_positions[NUM_COLLISIONS];
	Vec3d col_normals[NUM_COLLISIONS];
	int num_collisions;
	
	Vec3d original_col_positions[NUM_COLLISIONS];
	Vec3d original_col_normals[NUM_COLLISIONS];

	// 周囲の影響の結果を格納する変数
	Matrix3d vel_effect[NUM_COLLISIONS];
	Vec3d vel_constant[NUM_COLLISIONS];

	Matrix3d ang_effect[NUM_COLLISIONS];
	Vec3d ang_constant[NUM_COLLISIONS];

	// pointer data
	Vec3d pointer_pos;
	Vec3d pointer_vel;
	double pointer_massinv;

	Quaterniond pointer_ori;
	Vec3d pointer_angvel;
	Matrix3d pointer_inertiainv;

	bool bCollision;
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

// 与えられた接触で、引数のsolidに接している剛体をしらべ、あれば返す
PHSolid* getAdjacentSolid(PHConstraint* constraint, PHSolid* solid, int* sign = NULL)
{
	if(constraint->solid[0] == solid)
	{
		if(sign != NULL) *sign = -1;
		return constraint->solid[1];
	}
	else if(constraint->solid[1] == solid)
	{
		if(sign != NULL) *sign = 1;
		return constraint->solid[0];
	}
	else return NULL;
}

// 提示力の計算および提案手法に必要な情報を集めて構造体を作成する
void MakeHapticInfo(HapticInfo *info, vector<pair<PHConstraint *, int> > pointer_consts, vector<Matrix3d> matrices, vector<SpatialVector> vecs, bool bCollision)
{
	int num_dynamical = 0;

	// 力覚レンダリングに必要なデータの作成
	for(int i = 0; i < (int)pointer_consts.size(); i++)
	{
		PHConstraint* constraint = pointer_consts.at(num_dynamical).first;
		int sign = pointer_consts.at(num_dynamical).second;
		PHSolid* so;

		if(sign > 0)
		{
			so = pointer_consts.at(num_dynamical).first->solid[0];
		}
		else
		{
			so = pointer_consts.at(num_dynamical).first->solid[1];
		}

		PHContactPoint* contact = DCAST(PHContactPoint, constraint);
		info->col_positions[num_dynamical] = contact->pos;
		info->original_col_positions[num_dynamical] = info->col_positions[num_dynamical];

		info->solid_velocity[num_dynamical] = so->GetVelocity() + so->GetAngularVelocity() ^ (contact->pos - so->GetCenterPosition());
		info->solid_angular_velocity[num_dynamical] = so->GetAngularVelocity();
		info->solid_center_position[num_dynamical] = so->GetCenterPosition();
		info->solid_massinv[num_dynamical] = so->GetMassInv();
		info->solid_inertiainv[num_dynamical] = so->GetInertiaInv();

		info->nearest_solids[num_dynamical] = so;

		info->col_normals[num_dynamical] = contact->shapePair->normal * sign;
		info->original_col_normals[num_dynamical] = info->col_normals[num_dynamical];

		/*
		// 計算で得られた周囲の影響を表す変数をコピーする
		info->vel_constant[num_dynamical] = C;
		info->ang_constant[num_dynamical] = D;
		info->vel_effect[num_dynamical] = M;
	//	info->ang_effect[num_dynamical] = N;
		*/

		num_dynamical++;
	}

	info->num_collisions = num_dynamical > NUM_COLLISIONS ? NUM_COLLISIONS : num_dynamical;
	info->bCollision = bCollision;

	// 剛体ポインタの情報を格納
	info->pointer_pos = soPointer->GetFramePosition();
	info->pointer_vel = soPointer->GetVelocity();
	info->pointer_massinv = soPointer->GetMassInv();
	info->pointer_angvel = soPointer->GetAngularVelocity();
	info->pointer_inertiainv = soPointer->GetInertiaInv();
	info->pointer_ori = soPointer->GetOrientation();
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
void RecursiveSolidRetrieval(PHSolid* solid, set<PHConstraint *>* relative_solid_consts, set<PHSolid *>* relative_solids, int depth)
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
				// PHSolidを保存
				// 重複している場合はsetなので自動的に排除される
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
void GetSolidsFromPointer(vector<pair<PHConstraint *, int> >* pointer_consts, set<PHSolid *>* relative_solids, set<PHConstraint *>* relative_solid_consts, bool* bCollision)
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
			// 接触が存在した
			*bCollision = true;

			// ポインタと接している剛体のPHSolidを保存
			if(sign > 0)
			{
				relative_solids->insert((*it)->solid[0]);
			}
			else
			{
				relative_solids->insert((*it)->solid[1]);
			}

			// もし動ける剛体であれば、接触自体を格納
			// 動けない剛体は追加せず、その代わりにbCollisionでまかなう
			// こうするとデータを作成する剛体の数が減るので高速化できる
			if(col_solid->IsDynamical() == true)pointer_consts->push_back(pair<PHConstraint *, int>(*it, sign));

// 周囲の影響を計算するためのデータ作成処理
#if 1
			relative_solid_consts->insert(*it);

			// この剛体のと生じた接触が次の剛体に影響を及ぼす可能性がある場合は、次の剛体にすすむ
			if(col_solid->IsDynamical() == true)RecursiveSolidRetrieval(col_solid, relative_solid_consts, relative_solids, 0);
#endif
		}
	}
}

map<PHConstraint *, SpatialVector> lambda;

// ラムダの値を保存しておく
void SaveLambda(set<PHConstraint *> relative_solid_consts)
{
	for(set<PHConstraint *>::iterator it = relative_solid_consts.begin(); it != relative_solid_consts.end(); it++)
	{
		// ラムダの値(fv, fw)を接触とペアで保存しておく
		lambda.insert(pair<PHConstraint *, SpatialVector>(*it, (*it)->f));
	}
}

// 先送りシミュレーションをする関数
vector<SpatialVector> PredictSimulation(vector<pair<PHConstraint *, int> > pointer_consts, set<PHConstraint *> relative_solid_consts, set<PHSolid *> relative_solids, Vec3d force_vec, map<PHSolid *, Vec3d> coeff, int index = -1)
{
	double dt = scene->GetTimeStep();

	// 一時的な保存用変数
	SpatialVector f_save;
	PHSolid* si;

	// ポインタが接する剛体のうちindex番目に単位力を加える
	if(index != -1)
	{
		PHConstraint *constraint = pointer_consts.at(index).first;
		PHContactPoint* contact = DCAST(PHContactPoint, constraint);
			
		if(pointer_consts.at(index).second > 0)
		{
			si = DCAST(PHSolid, constraint->solid[0]);
		}
		else
		{
			si = DCAST(PHSolid, constraint->solid[1]);
		}

		f_save = si->f;

		// 事前に計算しておいた値を参照
		map<PHSolid *, Vec3d>::iterator cef = coeff.find(si);

		// 引数で与えられた単位力を拘束座標に変換して加える
		si->dv.v += si->GetMassInv() * force_vec * dt;
		si->dv.w += si->GetInertiaInv() * (si->GetOrientation().Conjugated() * ((contact->pos - si->GetCenterPosition()) ^ force_vec) - contact->shapePair->normal * pointer_consts.at(index).second) * dt;
	}

	// 接触のSetupDynamicsのcompjacobian以外の部分をやる
	// 別に行う理由は計算量の問題と依存関係によるdv(dw)はdv0(dw0)に依存
	for(set<PHConstraint *>::iterator it = relative_solid_consts.begin(); it != relative_solid_consts.end(); it++)
	{
		(*it)->SetupDynamicsForPrediction();
	}

	for(int i = 0; i < NUM_PREDICT_ITERATE; i++)
	{
		// すべての接触をiterate
		for(set<PHConstraint *>::iterator it = relative_solid_consts.begin(); it != relative_solid_consts.end(); it++)
		{
			(*it)->IterateDynamics();
		}
	}
	
	vector<SpatialVector> effects;

	// 影響を観測
	for(vector<pair<PHConstraint *, int> >::iterator it = pointer_consts.begin(); it != pointer_consts.end(); it++)
	{
		PHConstraint* c = (*it).first;
		int sign = (*it).second;

		PHSolid* s;

		if(sign > 0)
		{
			s = c->solid[0];
		}
		else
		{
			s = c->solid[1];
		}

		effects.push_back(s->dv);
	}

	// 加えた単位力の部分を元にもどす
	if(index != -1)
	{		
		si->f = f_save;
	}

	// fvとfwの値を元にもどす
	for(set<PHConstraint *>::iterator it = relative_solid_consts.begin(); it != relative_solid_consts.end(); it++)
	{
		map<PHConstraint *, SpatialVector>::iterator p = lambda.find(*it);
		(*it)->f = p->second;
	}

	// 影響をreturn
	return effects;
}

// 予測シミュレーションをセットアップする
void SetupPredictSimulation(set<PHConstraint *> relative_solid_consts, set<PHSolid *> relative_solids, map<PHSolid *, Vec3d> *coeff)
{
	double dt = scene->GetTimeStep();

	// すべての剛体をセットアップ
	for(set<PHSolid *>::iterator it = relative_solids.begin(); it != relative_solids.end(); it++)
	{
		PHSolid* solid = *it;
		solid->SetupDynamics(dt);

		// 単位力を加える処理のところで使う値(=加えた力をワールド座標から拘束座標に変換するもの)を今のうちに計算しておく
		Vec3d w = solid->GetOrientation().Conjugated() * solid->GetAngularVelocity();
		Vec3d calc = w % (solid->GetInertia() * w);
		coeff->insert(pair<PHSolid *, Vec3d>(*it, calc));
	}

	// すべての接触についてヤコビアンを計算
	for(set<PHConstraint *>::iterator it = relative_solid_consts.begin(); it != relative_solid_consts.end(); it++)
	{
		(*it)->CompResponseMatrix();
	}

	// lambdaを初期化
	lambda.clear();

	// ラムダを保存する
	SaveLambda(relative_solid_consts);
}


// 提案手法のために先送りシミュレーションを計画的に行ってくれる関数
void PredictSimulations(vector<pair<PHConstraint *, int> > pointer_consts, set<PHConstraint *> relative_solid_consts, set<PHSolid *> relative_solids, vector<Matrix3d> *mat, vector<SpatialVector> *vec)
{
	map<PHSolid *, Vec3d> coeff;

	// 必要な情報の準備
	SetupPredictSimulation(relative_solid_consts, relative_solids, &coeff);

	vector<Matrix3d> effects;

	// 加える力を０ベクトルとして定数項を取得
	vector<SpatialVector> b = PredictSimulation(pointer_consts, relative_solid_consts, relative_solids, Vec3d(), coeff);

	// 接触数＝列数だけ繰り返す
	for(int i = 0; i < (int)pointer_consts.size(); i++)
	{
		// ある単位ベクトルを加えてその結果をあらわすベクトルを取得
		vector<SpatialVector> vec_x = PredictSimulation(pointer_consts, relative_solid_consts, relative_solids, Vec3d(1, 0, 0), coeff, i);
		vector<SpatialVector> vec_y = PredictSimulation(pointer_consts, relative_solid_consts, relative_solids, Vec3d(1, 1, 0), coeff, i);
		vector<SpatialVector> vec_z = PredictSimulation(pointer_consts, relative_solid_consts, relative_solids, Vec3d(1, 0, 1), coeff, i);

		// このループで縦ベクトルを計算し格納
		for(int j = 0; j < (int)b.size(); j++)
		{
			Matrix3d v = Matrix3d(vec_x[j].v - b[j].v, vec_y[j].v - vec_x[j].v - b[j].v, vec_z[j].v - vec_x[j].v - b[j].v);
			Matrix3d w = Matrix3d(vec_x[j].w - b[j].w, vec_y[j].w - vec_x[j].w - b[j].w, vec_z[j].w - vec_x[j].w - b[j].w);

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
	vector<pair<PHConstraint *, int> > pointer_consts;

	set<PHSolid *> relative_solids;
	set<PHConstraint *> relative_solid_consts;

	bool bCollision = false;

	// ポインタからスタートして必要な剛体をすべて取得してくる関数
	GetSolidsFromPointer(&pointer_consts, &relative_solids, &relative_solid_consts, &bCollision);

	vector<Matrix3d> matrices;
	vector<SpatialVector> vecs;

	// 力を加えてみて動かし、影響を観測する関数
	if(bCollision == true)PredictSimulations(pointer_consts, relative_solid_consts, relative_solids, &matrices, &vecs);

	// 接触の情報を計算する
	MakeHapticInfo(info, pointer_consts, matrices, vecs, bCollision);
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
		DSTR << "----------NEW COLLISION DATA No." << global_collision_counter++ << "-----------" << endl;
		
		// すべての衝突に対して処理
		for(PHConstraints::iterator it = cp.begin(); it != cp.end(); it++)
		{
			// ループ内のカウンターを更新
			counter++;
			DSTR << "COLLISION" << counter << ": " << endl;

			// PHContactPointにdynamic castした処理のチェック
			PHContactPoint* contact = DCAST(PHContactPoint, (*it));
			DSTR << "collision pos = " << contact->pos << endl;
			DSTR << endl;

			// PHConstraintの情報の表示
			// solid1
			DSTR << "solid1.mass = " << (*it)->solid[0]->mass << endl;
			DSTR << "solid1.inertia = " << endl << (*it)->solid[0]->inertia;
			DSTR << "solid1.velocity = " << (*it)->solid[0]->GetVelocity()<< endl;
			DSTR << "solid1.angular_velocity = " << (*it)->solid[0]->GetAngularVelocity() << endl;
			DSTR << "solid1.position = " << (*it)->solid[0]->GetFramePosition() << endl;
			DSTR << "vector to working point from center = " << (contact->pos - (*it)->solid[0]->GetCenterPosition()).unit() << endl;

			if(current_valid_data == true)
			{
				for(int i = 0; i < info1.num_collisions; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = &(*info1.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[0]))DSTR << " --this solid is nearest object!!" << endl;
				}
			}
			else 
			{
				for(int i = 0; i < info2.num_collisions; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = &(*info2.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[0]))DSTR << " --this solid is nearest object!!" << endl;
				}

			}
			DSTR << endl;

			// solid2
			DSTR << "solid2.mass = " << (*it)->solid[1]->mass << endl;
			DSTR << "solid2.inertia = " << endl << (*it)->solid[1]->inertia;
			DSTR << "solid2.velocity = " << (*it)->solid[1]->GetVelocity()<< endl;
			DSTR << "solid2.angular_velocity = " << (*it)->solid[1]->GetAngularVelocity() << endl;
			DSTR << "solid2.position = " << (*it)->solid[1]->GetFramePosition() << endl;
			DSTR << "vector to working point from center = " << (contact->pos - (*it)->solid[1]->GetCenterPosition()).unit() << endl;

			if(current_valid_data == true)
			{
				for(int i = 0; i < info1.num_collisions; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info1.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]))DSTR << " --this solid is nearest object!!" << endl;
				}
			}
			else
			{
				for(int i = 0; i < info2.num_collisions; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info2.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]))DSTR << " --this solid is nearest object!!" << endl;
				}
			}
			DSTR << endl;
		}
	}
}

static clock_t start, end;
static int temp_counter = 0;

void t_start()
{
	start = clock();
}

void t_end()
{
	// 一周にかかる時間をチェック
	end = clock();

	double time = (double)(end - start) / CLOCKS_PER_SEC;
	if(time > 0)printf("%d: time =  %f\n", temp_counter++, time);
}
/**
 brief  	提示力の計算とHaptic Device	へ反映. multimedia timerによって呼び出される
			内部的にはポインタとSPIDARをvirtual couplingでつなぎ、ポインタも更新している
 param	 	なし
 return 	なし
*/
void CALLBACK HapticRendering(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
//	t_start();

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

	spidarG6.Update(dt);//	USBの通信を行う．


	/////////////////////////////////////////////////////////////////////////////////////////
	// Virtual Coupling
	// 並進
	// SPIDARの位置の更新
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
	Vec3d VCForce = Km * goal + Bm * (SPIDARVel - info->pointer_vel);

	// soPointerの速度を更新
	info->pointer_vel = info->pointer_vel + info->pointer_massinv * VCForce * dt;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Virtual Coupling
	// 回転
	// SPIDARの姿勢を更新
	Quaterniond qv;
	qv.FromMatrix(view_rot);

	static Quaterniond spidar_ori;
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


	//////////////////////////////////////////////////////////////////////////////////////////
	// 接触点の更新
	if(info->bCollision)
	{
		// 接触によって動いた量の総和を格納する変数
		Vec3d pointer_dx = Vec3d();

		for(int i = 0; i < info->num_collisions; i++)
		{
			Vec3d accel;
			Vec3d ang_accel;

			// 局所的な動力学計算のみ
			if(bSurroundEffect == false)
			{
				info->nearest_solids[i]->AddForce(VCForce, info->col_positions[i]);
				accel = info->solid_massinv[i] * VCForce;
				ang_accel = info->solid_inertiainv[i] * ((info->col_positions[i] - info->solid_center_position[i]) ^ (VCForce));
			}
			// 局所的な動力学計算＋周囲の影響を考慮
			else
			{
				// 加速度を計算
				accel = info->vel_effect[i] * VCForce + info->vel_constant[i];
				ang_accel = info->solid_inertiainv[i] * VCForce + info->ang_constant[i];
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
			// 局所的な動力学計算を６自由度で行うのであれば必要
			info->col_normals[i] = (info->col_normals[i] + dth).unit();
		}

		// spidarに力を加える
		if(bforce)
		{
			// SPIDARの空間と見ている空間が違うので行列を掛けて射影する
			Vec3d f = - view_haptic * VCForce;
			Vec3d t = - view_haptic * VCTorque;

			spidarG6.SetForce(f, t);	
		}
		else spidarG6.SetForce(Vec3d(), Vec3d());

		if(pointer_dx != Vec3d())
		{
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
			}
			// 速度の移動の方が大きかった
			else
			{
				info->pointer_pos += pointer_dx;
			}

			info->pointer_ori = Quaterniond::Rot(info->pointer_angvel * dt) * info->pointer_ori;
			info->pointer_ori = info->pointer_ori.unit();
		}
		// 剛体は動かなかったので位置は更新しない
		else
		{
			// 速度は運動量と力積の関係より受けた力の分だけ弱くなる
			// 式から導出できる
			info->pointer_vel += info->pointer_massinv * VCForce * dt;
			info->pointer_ori = Quaterniond::Rot(info->pointer_angvel * dt) * info->pointer_ori;
			info->pointer_ori = info->pointer_ori.unit();
		}
	}
	else
	{
		// 衝突がないので提示力をリセットする
		spidarG6.SetForce(Vec3d());

		// soPointerの位置を更新
		Vec3d dx = info->pointer_vel * dt;
		info->pointer_pos = info->pointer_pos + dx;

		info->pointer_ori = Quaterniond::Rot(info->pointer_angvel * dt) * info->pointer_ori;
		info->pointer_ori = info->pointer_ori.unit();
	}

/*
	if(info->num_collisions > 0)
	{
		// ポインタに加える力・トルクを格納する変数
		Vec3d pointer_force = Vec3d();
		Vec3d pointer_torque = Vec3d();

		// 接触によって動いた量の総和を格納する変数
		Vec3d pointer_dx = Vec3d();

		// ポインタに生じたすべての接触について計算
		for(int i = 0; i < info->num_collisions; i++)
		{
			// 提示力および提示トルクの計算
			// 衝突点での法線の逆方向にカップリング力を射影
			// normalは単位ベクトル前提
			Vec3d feedback_force = - FORCE_COEFF * dot(VCforce, info->col_normals[i]) * info->col_normals[i];
			Vec3d feedback_torque = (info->col_positions[i] - info->pointer_center) ^ feedback_force;

			// ここで加速度の計算を行う
			// 加速度の計算にどちらを使うかはそのときのモードによって変わる
			// IsDynamical == falseな剛体は移動しないので加速度はない
			if(info->nearest_solids[i]->IsDynamical() == true)
			{
				Vec3d accel;
				Vec3d ang_accel;

				// 衝突対象のトルクを計算
				Vec3d solid_torque_vector = (info->col_positions[i] - info->solid_center_position[i]) ^ (-feedback_force);

				// 周囲の影響を考慮しない局所的動力学計算
				// ポインタに接する剛体のみ局所的に高速で更新
				if(bSurroundEffect == false)
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
			}
	
#if 0
			// 提示力が大きすぎる場合提示力を減らす
			if(fabs(feedback_force.x) > 300 || fabs(feedback_force.y) > 300 || fabs(feedback_force.z) > 300)
			{
				cout << "********" << endl;
				cout << i << ":force is " << feedback_force << endl;
				cout << "pointer_pos = " << info->pointer_pos << endl;
				cout << "dx = " << dx << endl;
				cout << "vel effect = " << info->vel_effect[i] << endl;
				cout << "vel const = " << info->vel_constant[i] << endl;
				cout << "accel = " << accel << endl;
				cout << "VCforce = " << VCforce << endl;
				cout << "goal = " << goal << endl;
				cout << "relative vel = " << PointerVel - info->pointer_vel << endl;
				cout << "normal = " << info->col_normals[i] << endl;
				cout << "dth = " << dth << endl;
				cout << "ang accel = " << ang_accel << endl;
				cout << endl;
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
			Vec3d t = view_haptic * pointer_torque;

#if 0
			// 提示力が大きすぎる場合は小さくする
			if(fabs(f.x) > 300 || fabs(f.y) > 300 || fabs(f.z) > 300)
			{
				cout << "force  is " << f;
				f /= 300;
				cout << "; changed to " << f << endl;
				cout << "torque is " << t;
				t /= 300;
				cout << "; changed to " << t << endl;
			}
#endif
			spidarG6.SetForce(f,- VCTorque + t);	
		}
		else spidarG6.SetForce(Vec3d(), Vec3d());

		if(pointer_dx != Vec3d())
		{
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

				info->pointer_ori = Quaterniond::Rot(info->pointer_angvel * dt) * info->pointer_ori;
				info->pointer_ori = info->pointer_ori.unit();
			}
		}
		// 速度の移動の方が大きかった
		else
		{
			// 仮想壁で動いた分だけ更新
			info->pointer_pos += pointer_dx;
			info->pointer_center += pointer_dx;
		
			// 速度は運動量と力積の関係より受けた力の分だけ弱くなる
			// 式から導出できる
			info->pointer_vel += info->pointer_massinv * pointer_force * dt;

			info->pointer_ori = Quaterniond::Rot(info->pointer_angvel * dt) * info->pointer_ori;
			info->pointer_ori = info->pointer_ori.unit();
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
//		cout << "pointer = " << info->pointer_pos << endl;
		cout << "num collisions = " << info->num_collisions << endl;
		sec_counter = 0;	
	}
	sec_counter++;									// カウンターの更新
#endif
*/
//	t_end();
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
		Quaterniond ori_error = Quaterniond::Rot(info->col_normals[i]) * Quaterniond::Rot(info->original_col_normals[i]).Inv();

		// 誤差の分平行移動する
		info->nearest_solids[i]->SetFramePosition(info->nearest_solids[i]->GetFramePosition() + pos_error);
		info->nearest_solids[i]->SetOrientation(info->nearest_solids[i]->GetOrientation() * ori_error);

		info->nearest_solids[i]->SetCenterPosition(info->solid_center_position[i]);

		info->nearest_solids[i]->SetVelocity(info->solid_velocity[i]);
		info->nearest_solids[i]->SetAngularVelocity(info->solid_angular_velocity[i]);
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

	// 衝突点情報を表示
//	show_collision_info();

	// 衝突点から周囲の影響を計算
	// １を参照中。２を更新
	if(current_valid_data == true)
	{
		calculate_surround_effect(&info2);
	}
	// ２を参照中。１を更新
	else
	{
		calculate_surround_effect(&info1);
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

	// SPIDARの位置を描画
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

	// 原点を描画
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
		cout << "exit operation from key input" << endl;
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
		cout << "force ";
		if(bforce) cout << "on";
		else cout << "off";
		cout << endl;
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
		cout << "surrounding effect ";
		if(bSurroundEffect)cout << "on";
		else cout << "off";
		cout << endl;
	}
	*/
	else if(key == 'p')
	{
		soPointer->SetAngularVelocity(Vec3d());
	}
	// 現在のバーチャルカップリングのKとBの値を表示する
	else if(key == 'l')
	{
		cout << "Km = " << Km << " Bm = " << Bm << endl;
		cout << "Kr = " << Kr << " Br = " << Br << endl;
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
		FORCE_COEFF += 0.01;
		cout << "coeff = " << FORCE_COEFF << endl;
	}
	// 提示力を調節する値を0.1減少する
	else if(key == 'n')
	{
		FORCE_COEFF -= 0.01;
		cout << "coeff = " << FORCE_COEFF << endl;
	}
	// SPIDARの位置と原点をあらわす球の表示をON/OFFする
	else if(key == 'd')
	{
		bDisplayPointer = !bDisplayPointer;
		cout << "pointer is ";
		if(!bDisplayPointer)cout << "not ";
		cout << "displayed" << endl;
	}
	else if(key == 'e')
	{
		bMode = !bMode;
		if(bMode)cout << "movement ";
		else cout << "rotation ";
		cout << "edit mode" << endl;
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

		// 四角のポインタ
		bd.boxsize = Vec3f(2, 2, 2);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soPointer->AddShape(floor);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));
	}	

	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere.at(sphereCnt)->AddShape(floor);
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
	cout << devMan;						//	初期化の結果を表示	
}

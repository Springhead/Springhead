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

// debug出力用
#include <fstream>

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

#define NUM_COLLISIONS 20			// ポインタへの許容接触数
#define NUM_COL_SOLIDS 20			// ポインタへの許容接触剛体数　NUM_COLLISIONSと区別するのはプログラムを読みやすくするため。実質的な存在意義はない

#ifdef _DEBUG
	#define SIMULATION_FREQ	30         // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		500			// 力覚スレッドの周期Hz
	float Km = 400;						// virtual couplingの係数
	float Bm = 30;						// 並進

	float Kr = 40;						// 回転
	float Br = 13;

#elif _WINDOWS
	#define SIMULATION_FREQ	30          // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		1000		// 力覚スレッドの周期Hz
	float Km = 400;						// virtual couplingの係数
	float Bm = 30;						// 並進

	float Kr = 40;						// 回転
	float Br = 13;
#endif

// 提示力と剛体に提示する力を直接変化させる定数
double FORCE_COEFF =		0.5;

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


// debug出力用fstream
ofstream ofs;
bool bOutput = false;

// 力覚計算に必要なデータを集めた構造体
// 力覚計算を高速で行えるように
// シミュレーションでデータを格納しておく
typedef struct {
	// collision data
	PHConstraint* constraint[NUM_COLLISIONS];

	// collision solid data
	PHSolid* nearest_solids[NUM_COL_SOLIDS];

	// map from physical data to rigid body itself
	map<int, int> ColToSol;
	multimap<int, int> SolToCol;
	int num_solids;

	// physical data
	Vec3d solid_velocity[NUM_COL_SOLIDS];
	Vec3d solid_angular_velocity[NUM_COL_SOLIDS];

	Vec3d solid_current_center_positions[NUM_COL_SOLIDS];
	Vec3d solid_future_center_positions[NUM_COL_SOLIDS];
	Quaterniond solid_orientations[NUM_COL_SOLIDS];

	double solid_massinvs[NUM_COL_SOLIDS];
	Matrix3d solid_inertiainvs[NUM_COL_SOLIDS];

	// collision data
	Vec3d future_col_positions[NUM_COLLISIONS];
	Vec3d future_col_normals[NUM_COLLISIONS];
	int num_collisions;
	
	Vec3d current_col_positions[NUM_COLLISIONS];
	Vec3d current_col_normals[NUM_COLLISIONS];

	Vec3d pointer_current_col_positions[NUM_COLLISIONS];
	Vec3d pointer_future_col_positions[NUM_COLLISIONS];

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
void MakeHapticInfo(HapticInfo *info, HapticInfo *prev_info, vector<pair<PHConstraint *, int> > pointer_consts, vector<Matrix3d> matrices, vector<SpatialVector> vecs, vector<pair<PHConstraint *, int> > pointer_static_consts)
{
	// 初期化処理
	// 前々回のデータはもう必要ないので初期化する
	for(int i = 0; i < info->num_solids; i++) info->nearest_solids[i] = NULL;
	info->num_solids = 0;
	info->ColToSol.clear();
	info->SolToCol.clear();

	int size = (int)pointer_consts.size();

	// 力覚レンダリングに必要なデータの作成
	for(int i = 0; i < (int)(pointer_consts.size() + pointer_static_consts.size()); i++)
	{
		// 許容数を超える場合は打ち切り
		if(i > NUM_COLLISIONS) break;

		PHConstraint* constraint;
		int sign;
		PHSolid* so;

		// 動く剛体を含む接触
		if(size > i)
		{
			constraint = pointer_consts[i].first;
			sign = pointer_consts[i].second;

			if(sign > 0)
			{
				so = pointer_consts[i].first->solid[0];
			}
			else
			{
				so = pointer_consts[i].first->solid[1];
			}

			/*
			// 計算で得られた周囲の影響を表す変数をコピーする
			info_local->vel_constant[num_dynamical] = C;
			info_local->ang_constant[num_dynamical] = D;
			info_local->vel_effect[num_dynamical] = M;
		//	info_local->ang_effect[num_dynamical] = N;
			*/
		}
		// 動かない剛体を含む接触
		else
		{
			constraint = pointer_static_consts[i - size].first;
			sign = pointer_static_consts[i - size].second;

			if(sign > 0)
			{
				so = pointer_static_consts[i - size].first->solid[0];
			}
			else
			{
				so = pointer_static_consts[i - size].first->solid[1];
			}

			/*
			info_local->vel_constant[num_dynamical] = Matrix3d;
			info_local->ang_constant[num_dynamical] = Matrix3d;
			info_local->vel_effect[num_dynamical] = Vec3d();
		//	info_local->ang_effect[num_dynamical] = Vec3d();
			*/
		}


		int counter = 0;
		// 配列にPHSolidを追加
		// 剛体と接触が１対１ではないため、接触から剛体へ添え字を変換するマップを作成する
		while(1)
		{
			// まだ追加されていない剛体は追加する
			if(!info->nearest_solids[counter])
			{
				info->nearest_solids[counter] = so;
				info->ColToSol.insert(pair<int, int>(i, counter));
				info->SolToCol.insert(pair<int, int>(counter, i));
				info->num_solids = counter+1;

				// 速度等も剛体ごとに追加
				info->solid_velocity[counter] = so->GetVelocity();
				info->solid_angular_velocity[counter] = so->GetAngularVelocity();
				info->solid_massinvs[counter] = so->GetMassInv();
				info->solid_inertiainvs[counter] = so->GetInertiaInv();

				info->solid_orientations[counter] = so->GetOrientation();

				bool current_exists = false;
				// 前回もこの剛体とポインタの接触が存在したかチェックし、
				// 存在した場合は徐々に戻すようにデータを格納する
				for(int j = 0; j < prev_info->num_solids; j++)
				{
					if(prev_info->nearest_solids[j] == so && so->IsDynamical())
					{
						info->solid_current_center_positions[counter] = prev_info->solid_current_center_positions[j];
						info->solid_future_center_positions[counter] = so->GetCenterPosition();
						current_exists = true;				
						break;
					}
				}

				// 前回は存在しなかった
				if(!current_exists)
				{
					info->solid_current_center_positions[counter] = so->GetCenterPosition();
					info->solid_future_center_positions[counter] = Vec3d();
				}
				break;
			}
			// もしすでに追加済みの場合は追加しない
			// 添え字変換表のみ作成
			else if(info->nearest_solids[counter] == so)
			{
				info->ColToSol.insert(pair<int, int>(i, counter));
				info->SolToCol.insert(pair<int, int>(counter, i));
				break;
			}
			counter++;
		}

		info->constraint[i] = constraint;

		PHContactPoint* contact = DCAST(PHContactPoint, constraint);

		bool current_exists = false;

		// 同じ接触が前回存在したかチェック
		for(int j = 0; j < prev_info->num_collisions; j++)
		{
			// 見つかった場合前回の法線と接触点を保存し
			// 今回のデータを目標として徐々に更新を行う
			if(prev_info->constraint[j] == constraint)
			{
				info->current_col_positions[i] = prev_info->current_col_positions[j];
				info->future_col_positions[i] = contact->pos;

				info->current_col_normals[i] = prev_info->current_col_normals[j];
				info->future_col_normals[i] = contact->shapePair->normal * sign;

				info->pointer_current_col_positions[i] = prev_info->pointer_current_col_positions[j];
				info->pointer_future_col_positions[i] = contact->pos;

				current_exists = true;
				break;
			}
		}

		// 前回同じ接触が存在しなかったので
		// 目標の接触の法線と接触点はなし
		if(!current_exists)
		{
			info->current_col_positions[i] = contact->pos;
			info->future_col_positions[i] = Vec3d();

			info->current_col_normals[i] = contact->shapePair->normal * sign;
			info->future_col_normals[i] = Vec3d();

			// ポインタの接触候補点は接触点と同じ
			info->pointer_current_col_positions[i] = info->current_col_positions[i];
			info->pointer_future_col_positions[i] = Vec3d();
		}
	}

	info->num_collisions = size + (int)pointer_static_consts.size();

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
void RecursiveSolidRetrieval(map<PHConstraint *, bool> csm, PHSolid* solid, set<PHConstraint *>* relative_solid_consts, set<PHSolid *>* relative_solids, int depth)
{
	for(map<PHConstraint *, bool>::iterator it = csm.begin(); it != csm.end(); it++)
	{
		// 処理済の場合は次にすすむ
		if(it->second) continue;

		// 与えられた剛体から、その剛体に接触する剛体を取得する
		PHSolid* col_solid = getAdjacentSolid(it->first, solid);

		// もしその剛体が存在し、ポインタではない場合
		// ポインタの場合はGetSolidsFromPointerで取得する
		if(col_solid && col_solid != DCAST(PHSolid, soPointer))
		{
			// 処理済みにする
			it->second = true;

			// 接触自体を追加
			PHConstraint* c = it->first;
			pair<set<PHConstraint *>::iterator, bool> p = relative_solid_consts->insert(c);

			// 今追加したデータは重複してなかった
			if(p.second)
			{
				// PHSolidを保存
				// 重複している場合はsetなので自動的に排除される
				relative_solids->insert(c->solid[0]);
				relative_solids->insert(c->solid[1]);

				// 動かない剛体ではなく、かつ取得上限まで達していなかった
				if(col_solid->IsDynamical() && depth < LIMIT_DEPTH)
				{
					// 次の剛体にすすむ
					RecursiveSolidRetrieval(csm, col_solid, relative_solid_consts, relative_solids, depth++);
				}
			}
		}
	}
}

// ポインタからスタートして必要な剛体をすべて取得してくる関数
void GetSolidsFromPointer(vector<pair<PHConstraint *, int> >* pointer_consts, set<PHSolid *>* relative_solids, set<PHConstraint *>* relative_solid_consts, vector<pair<PHConstraint *, int> >* pointer_static_consts)
{
	// 衝突点を取得
	PHConstraints cs = GetContactPoints(scene);
	map<PHConstraint *, bool> csm;

	// 計算時間を短縮するためにマップに処理済フラグをつけて保存する
	// 接触数が少ない場合は無駄が多いが、
	// 接触数が多くなったときの伸びはこちらのほうが断然少ないはず
	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
	{
		csm.insert(pair<PHConstraint *, bool>(*it, false));
	}

	// すべての衝突点について調査
	for(map<PHConstraint *, bool>::iterator it = csm.begin(); it !=csm.end(); it++)
	{
		// 処理済の場合は次にすすむ
		if(it->second) continue;

		int sign = 1;

		// ポインタを含む接触を取得
		PHSolid* col_solid = getAdjacentSolid(it->first, DCAST(PHSolid, soPointer), &sign);
		
		// 発見できた
		if(col_solid)
		{
			it->second = true;

			// ポインタと接している剛体のPHSolidを保存
			if(sign > 0)
			{
				relative_solids->insert(it->first->solid[0]);
			}
			else
			{
				relative_solids->insert(it->first->solid[1]);
			}

			// もし動ける剛体であれば、接触自体を格納
			// 動けない剛体は別なベクトルに追加
			if(col_solid->IsDynamical())pointer_consts->push_back(pair<PHConstraint *, int>(it->first, sign));
			else pointer_static_consts->push_back(pair<PHConstraint *, int>(it->first, sign));

// 周囲の影響を計算するためのデータ作成処理
#if 1
			relative_solid_consts->insert(it->first);

			// この剛体と生じた接触が次の剛体に影響を及ぼす可能性がある場合は、次の剛体にすすむ
			if(col_solid->IsDynamical())RecursiveSolidRetrieval(csm, col_solid, relative_solid_consts, relative_solids, 0);
#endif
		}
	}
}

map<PHConstraint *, SpatialVector> lambda;

// ラムダの値を保存しておく関数
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
		PHConstraint *constraint = pointer_consts[index].first;
		PHContactPoint* contact = DCAST(PHContactPoint, constraint);
			
		if(pointer_consts[index].second > 0)
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
		si->dv.w += si->GetInertiaInv() * (si->GetOrientation().Conjugated() * ((contact->pos - si->GetCenterPosition()) ^ force_vec) - contact->shapePair->normal * pointer_consts[index].second) * dt;
	}

	// 接触のSetupDynamicsのcompjacobian以外の部分をやる
	// 別に行う理由は計算量の問題と変数の依存関係による
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
		PHConstraint* c = it->first;
		int sign = it->second;

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
	// ヤコビアンの計算は一度だけでよい
	for(set<PHConstraint *>::iterator it = relative_solid_consts.begin(); it != relative_solid_consts.end(); it++)
	{
		(*it)->CompResponseMatrix();
	}

	// lambdaを初期化
	lambda.clear();

	// シミュレーションで上書きしてしまう変数を今のうちに保存する
	SaveLambda(relative_solid_consts);
}


// 提案手法のために先送りシミュレーションを計画的に行ってくれる関数
void PredictSimulations(vector<pair<PHConstraint *, int> > pointer_consts, set<PHConstraint *> relative_solid_consts, set<PHSolid *> relative_solids, vector<Matrix3d> *mat, vector<SpatialVector> *vec)
{
	map<PHSolid *, Vec3d> coeff;

	// 必要な情報の準備
	SetupPredictSimulation(relative_solid_consts, relative_solids, &coeff);

	// 加える力を０ベクトルとして定数項を取得
	vector<SpatialVector> b = PredictSimulation(pointer_consts, relative_solid_consts, relative_solids, Vec3d(), coeff);

	vector<Matrix3d> effects;

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
void calculate_surround_effect(HapticInfo* info, HapticInfo* prev_info)
{
	vector<pair<PHConstraint *, int> > pointer_consts;
	vector<pair<PHConstraint *, int> > pointer_static_consts;

	set<PHSolid *> relative_solids;
	set<PHConstraint *> relative_solid_consts;

	// ポインタからスタートして必要な剛体をすべて取得してくる関数
	GetSolidsFromPointer(&pointer_consts, &relative_solids, &relative_solid_consts, &pointer_static_consts);

	vector<Matrix3d> matrices;
	vector<SpatialVector> vecs;

	// 力を加えてみて動かし、影響を観測する関数
	PredictSimulations(pointer_consts, relative_solid_consts, relative_solids, &matrices, &vecs);

	// 接触の情報を計算する
	MakeHapticInfo(info, prev_info, pointer_consts, matrices, vecs, pointer_static_consts);
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
		cout << "----------NEW COLLISION DATA No." << global_collision_counter++ << "-----------" << endl;
		
		// すべての衝突に対して処理
		for(PHConstraints::iterator it = cp.begin(); it != cp.end(); it++)
		{
			// ループ内のカウンターを更新
			counter++;
			cout << "COLLISION" << counter << ": " << endl;

			// PHContactPointにdynamic castした処理のチェック
			PHContactPoint* contact = DCAST(PHContactPoint, (*it));
			cout << "collision pos = " << contact->pos << endl;
			cout << endl;

			// PHConstraintの情報の表示
			// solid1
			cout << "solid1.mass = " << (*it)->solid[0]->mass << endl;
			cout << "solid1.inertia = " << endl << (*it)->solid[0]->inertia;
			cout << "solid1.velocity = " << (*it)->solid[0]->GetVelocity()<< endl;
			cout << "solid1.angular_velocity = " << (*it)->solid[0]->GetAngularVelocity() << endl;
			cout << "solid1.position = " << (*it)->solid[0]->GetFramePosition() << endl;
			cout << "vector to working point from center = " << (contact->pos - (*it)->solid[0]->GetCenterPosition()).unit() << endl;

			if(current_valid_data)
			{
				for(int i = 0; i < info1.num_solids; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = (PHSolidIf *)&(*info1.nearest_solids[i]);
					if(nearest_address == (PHSolidIf *)((*it)->solid[0]))cout << " --this solid is nearest object!!" << endl;
				}
			}
			else 
			{
				for(int i = 0; i < info2.num_solids; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = (PHSolidIf *)&(*info2.nearest_solids[i]);
					if(nearest_address == (PHSolidIf *)((*it)->solid[0]))cout << " --this solid is nearest object!!" << endl;
				}

			}
			cout << endl;

			// solid2
			cout << "solid2.mass = " << (*it)->solid[1]->mass << endl;
			cout << "solid2.inertia = " << endl << (*it)->solid[1]->inertia;
			cout << "solid2.velocity = " << (*it)->solid[1]->GetVelocity()<< endl;
			cout << "solid2.angular_velocity = " << (*it)->solid[1]->GetAngularVelocity() << endl;
			cout << "solid2.position = " << (*it)->solid[1]->GetFramePosition() << endl;
			cout << "vector to working point from center = " << (contact->pos - (*it)->solid[1]->GetCenterPosition()).unit() << endl;

			if(current_valid_data)
			{
				for(int i = 0; i < info1.num_solids; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info1.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]))cout << " --this solid is nearest object!!" << endl;
				}
			}
			else
			{
				for(int i = 0; i < info2.num_solids; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info2.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]))cout << " --this solid is nearest object!!" << endl;
				}
			}
			cout << endl;
		}
	}
}



// 処理にかかる時間を計測。一時的なもの
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
	
	// シミュレーションと力覚レンダリングの更新周期の比率をあらわす変数
	static const double ratio = (double)SIMULATION_FREQ / (10 * HAPTIC_FREQ);

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


	////////////////////////////////////////////////////////////////////////////////////////
	// 力覚計算
	// ユーザへのフォースフィードバックと接触剛体の加速度更新

	// 登録された接触があっても、現在本当に接触しているかはわからないのでフラグを用意
	bool feedback = false;

	for(int i = 0; i < info->num_collisions; i++)
	{
		// 面の法線と、ポインタ上の点から剛体上の点までを結んだベクトルの内積を計算
		// これが０以上なら（ゼロベクトルも含む。ちょうど接している）接触がある
		if(dot(info->current_col_positions[i] - info->pointer_current_col_positions[i], info->current_col_normals[i]) >= 0)
		{
			// 力を加えてよい対象かチェック
			// 力の向きと剛体の接する側のチェック
			if(dot(info->current_col_normals[i], -VCForce) >= 0)
			{
				// ユーザに力覚を提示
				feedback = true;

				// 剛体の添え字
				int sol_index = info->ColToSol.find(i)->second;

				// 外力で動く剛体であれば加速度が変化するはずなので加速度を計算して速度を更新する
				if(info->nearest_solids[sol_index]->IsDynamical())
				{
					Vec3d accel;
					Vec3d ang_accel;

					// 局所的な動力学計算のみ
					if(!bSurroundEffect)
					{
//						info->nearest_solids[sol_inex]->AddForce(FORCE_COEFF * VCForce, info->col_positions[i]);
						accel = info->solid_massinvs[sol_index] * FORCE_COEFF * VCForce;
						ang_accel = info->solid_inertiainvs[sol_index] * ((info->current_col_positions[i] - info->solid_current_center_positions[sol_index]) ^ (VCForce));
					}
					// 局所的な動力学計算＋周囲の影響を考慮
					else
					{
						// 加速度を計算
						accel = info->vel_effect[i] * VCForce + info->vel_constant[i];
						ang_accel = info->solid_inertiainvs[i] * VCForce + info->ang_constant[i];
					}

					// 得られた加速度で速度を更新
					// 座標は後で更新する
					info->solid_velocity[sol_index] += accel * dt;
								
					// 角速度を更新
					info->solid_angular_velocity[sol_index] += ang_accel * dt;
				}
			}
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// ユーザへの力の提示
	if(feedback && bforce)
	{
		// SPIDARの空間と見ている空間が違うので行列を掛けて射影する
		Vec3d f = - view_haptic * VCForce;
		Vec3d t = - view_haptic * VCTorque;

		spidarG6.SetForce(FORCE_COEFF * f, FORCE_COEFF * t);

		// debug出力処理
		if(bOutput)
		{
			ofs << FORCE_COEFF * f << " " << FORCE_COEFF * t << endl;
		}
	}
	else spidarG6.SetForce(Vec3d(), Vec3d());
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// バーチャルカップリングに従ってポインタの位置・姿勢更新
	info->pointer_vel += info->pointer_massinv * VCForce * dt;
	Vec3d pointer_dx = info->pointer_vel * dt;
	info->pointer_pos += pointer_dx;

	info->pointer_angvel += info->pointer_inertiainv * VCTorque * dt;
	Vec3d pointer_dth = info->pointer_angvel * dt;
	info->pointer_ori = Quaterniond::Rot(pointer_dth) * info->pointer_ori;
	info->pointer_ori = info->pointer_ori.unit();

	Vec3d max_penetration = Vec3d();

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 局所的シミュレーションによる接触する剛体の位置・姿勢を更新
	for(int i = 0; i < info->num_solids; i++)
	{
		// 速度による移動・回転
		Vec3d dx_local = info->solid_velocity[i] * dt;
		info->solid_current_center_positions[i] += dx_local;

		Vec3d dth_local = info->solid_angular_velocity[i] * dt;
		info->solid_orientations[i] = Quaterniond::Rot(dth_local) * info->solid_orientations[i];
//#define GRADUAL_REFLECTION
#ifdef GRADUAL_REFLECTION
		info->solid_current_center_positions[i] += ratio * info->solid_future_center_positions[i];
#endif
		
		// 剛体上で起こった接触を検索する		
		// 検索に時間をかけたくないので、添え字を剛体から接触に変換するmultimapを使って検索を行う
		pair<multimap<int, int>::iterator, multimap<int, int>::iterator> it = info->SolToCol.equal_range(i);
		for(; it.first != it.second; it.first++)
		{
			// この剛体上にある接触を剛体の移動に伴って更新する
			int col_index = (it.first)->second;
			info->current_col_positions[col_index] += dx_local;
			info->current_col_normals[col_index] += dth_local ^ (info->current_col_positions[col_index] - info->solid_current_center_positions[i]);
			info->current_col_normals[col_index] = info->current_col_normals[col_index].unit();

			// ポインタの接触点更新
			// ポインタの移動量に従って平行移動
			// この情報はポインタが持つのではなく接触がそれぞれ持っている
			info->pointer_current_col_positions[col_index] += pointer_dx;
			info->pointer_current_col_positions[col_index] += pointer_dth ^ (info->pointer_current_col_positions[col_index] - info->pointer_pos);

// 目的地に徐々に近づけていく
#ifdef GRADUAL_REFLECTION
			info->pointer_current_col_positions[col_index] += ratio * info->pointer_future_col_positions[col_index];
			info->current_col_positions[col_index] += ratio * info->future_col_positions[col_index];
			info->current_col_normals[col_index] += ratio * info->future_col_normals[col_index];
#endif

#define CORRECT_PENETRATION

#ifdef CORRECT_PENETRATION
			// めり込み解消処理
			// ここで最大のめり込みを調べる
			Vec3d col_vector = info->current_col_positions[col_index] - info->pointer_current_col_positions[col_index];
			Vec3d col_normal = dot(col_vector, info->current_col_normals[col_index]) * info->current_col_normals[col_index].unit();

			// 最大のベクトルを更新
			if(col_normal.norm() > max_penetration.norm())
			{
				max_penetration = col_normal;
			}
#endif
		}
	}

// ポインタの位置補正。めり込みを解消
#ifdef CORRECT_PENETRATION
	// ポインタ自身の位置を移動
	info->pointer_pos += ratio * max_penetration;

	// すべての接触もその方向に移動
	for(int i = 0; i < info->num_collisions; i++)
	{
		info->pointer_current_col_positions[i] += 0.1 * max_penetration;
	}
#endif
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

	// 力覚レンダリングからシミュレーションへの局所的動力学計算の結果反映処理
	// すべての剛体について行う
	for(int i = 0; i < info->num_solids; i++)
	{
		// 誤差の分平行移動する
		Vec3d pos_error = info->solid_current_center_positions[i] - info->nearest_solids[i]->GetCenterPosition();
		Quaterniond ori_error = info->solid_orientations[i] * info->nearest_solids[i]->GetOrientation().Inv();

		info->nearest_solids[i]->SetFramePosition(info->nearest_solids[i]->GetFramePosition() + pos_error);
		info->nearest_solids[i]->SetOrientation(info->nearest_solids[i]->GetOrientation() * ori_error);

		info->nearest_solids[i]->SetCenterPosition(info->solid_current_center_positions[i]);

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

	// 局所的動力学計算によって生じた差を修正する
	ErrorCorrection();

	// シミュレーションを１ステップ進める
	scene->Step();

	// 衝突点情報を表示
//	show_collision_info();

	// 衝突点から周囲の影響を計算
	// １を参照中。２を更新
	if(current_valid_data)
	{
		// 基本的に前回のデータを引き継ぐ
		calculate_surround_effect(&info2, &info1);
	}
	// ２を参照中。１を更新
	else
	{
		calculate_surround_effect(&info1, &info2);
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
	else if(key == 'p')
	{
		bOutput = !bOutput;

		cout << "file output is ";
		if(bOutput)cout << "enabled" << endl;
		else cout << "disabled" << endl;
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

	ofs.open("log.txt");

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
		soSphere[sphereCnt]->AddShape(floor);
		soSphere[sphereCnt]->SetFramePosition(Vec3f(0, 15+5*sphereCnt, 0));
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

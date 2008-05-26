#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <Springhead.h>
#include <Physics/PHSolid.h>
#include <Base/TMatrix.h>

using namespace Spr;
using namespace PTM;

/// 近傍物体の情報を入れた構造体
struct PHNeighborObject {
	PHSolidIf* phSolidIf;	///< シーンがもつ近傍物体へのポインタ
	PHSolid phSolid;			///< phSolidIf*の中身のコピー
	bool blocal;				///< 力覚プロセスでシミュレーションしているかどうか
	bool bfirstlocal;			///< 初めて近傍物体になったかどうか
	Vec3d closestPoint;		///< 近傍物体最近点（ローカル座標）
	Vec3d pointerPoint;		///< 力覚ポインタ最近点（ローカル座標）
	Vec3d face_normal;		///< 近傍物体の面の法線（の予定）
	Vec3d direction;			///< 近傍物体から力覚ポインタへのベクトル // いまはこれを面の法線としてやってる 
	TMatrixRow<6, 3, double> A;	///< LocalDynamicsで使う運動係数（ユーザの入力により変化）
	SpatialVector b;						///< LocalDynamicsで使う運動係数（重力等の定数項）
};

#endif
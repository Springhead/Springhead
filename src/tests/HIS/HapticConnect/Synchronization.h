#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <Springhead.h>
#include <Physics/PHSolid.h>

using namespace Spr;

/// 近傍物体の情報を入れた構造体
struct PHNeighborObject {
	PHSolidIf* phSolidIf;	///< シーンがもつ近傍物体へのポインタ
	PHSolid phSolid;			///< phSolidIf*の中身のコピー
	bool lastDynamical;		///< 初期のdynamical　// 今は使ってない．すでに近傍物体だったらfalseになってしまうため．
	bool lastFrozen;			///< 初期のfrozen		// 今は使ってない
	bool blocal;				///< 力覚プロセスでシミュレーションしているかどうか
	bool bfirstlocal;
	Vec3d closestPoint;		///< 近傍物体最近点（ローカル座標）
	Vec3d pointerPoint;		///< 力覚ポインタ最近点（ローカル座標）
	Vec3d face_normal;		///< 近傍物体の面の法線（の予定）
	Vec3d direction;			///< 近傍物体から力覚ポインタへのベクトル // いまはこれを面の法線としてやってる 
	
};

#endif
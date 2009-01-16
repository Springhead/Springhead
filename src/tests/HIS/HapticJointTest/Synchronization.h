#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <Springhead.h>
#include <Base/TMatrix.h>
#include <Physics/PHSolid.h>

using namespace Spr;
using namespace PTM;

/// 近傍物体の情報を入れた構造体
struct PHNeighborObject {
	PHSolidIf* phSolidIf;				///< シーンがもつ近傍物体へのポインタ
	PHSolid phSolid;					///< phSolidIf*の中身のコピー
	bool bneighbor;
	bool blocal;						///< 力覚プロセスでシミュレーションしているかどうか
	bool bfirstlocal;					///< 初めて近傍物体になったかどうか
	Vec3d closestPoint;					///< 近傍物体最近点(ローカル座標)
	Vec3d pointerPoint;					///< 力覚ポインタ最近点(ローカル座標)
	Vec3d face_normal;					///< 近傍物体の提示面の法線(近傍物体から力覚ポインタへのベクトル)
	Vec3d last_face_normal;				///< 前回の近傍物体の提示面の法線
	double test_force_norm;				///< 予測シミュレーションで単位力の大きさ
	TMatrixRow<6, 3, double> A;			///< LocalDynamicsで使う運動係数（ユーザの入力により変化）
	SpatialVector b;					///< 予測シミュレーションで求めた運動係数（重力等の定数項）
	SpatialVector curb;					///< 通常シミュレーションででた定数項
	SpatialVector lastb;				///< 前回の予測シミュレーションで求めた運動係数の定数項
	SpatialVector lastvel;

	PHNeighborObject(){
		phSolidIf = NULL;	
		bneighbor = false;
		blocal = false;							
		bfirstlocal = false;						
		closestPoint = Vec3d(0.0, 0.0, 0.0);					
		pointerPoint = Vec3d(0.0, 0.0, 0.0);						
		face_normal = Vec3d(0.0, 0.0, 0.0);			
		last_face_normal = Vec3d(0.0, 0.0, 0.0);
		test_force_norm = 0.0;		
	}; 
};

#endif
#ifndef FW_EXPANDED_PHSOLID_H
#define FW_EXPANDED_PHSOLID_H

#include <Springhead.h>
#include <Physics/PHSolid.h>
#include <Base/TMatrix.h>

using namespace Spr;
using namespace PTM;

struct MotionCoeff{
	TMatrixRow<6, 3, double> A;			///< LocalDynamicsで使う運動係数（ユーザの入力により変化）
	SpatialVector b;					///< 予測シミュレーションで求めた運動係数（重力等の定数項）
	SpatialVector curb;					///< 通常シミュレーションででた定数項
	SpatialVector lastb;				///< 前回の予測シミュレーションで求めた運動係数の定数項
};

struct Flag{
	bool bneighbor;
	bool blocal;						///< 力覚プロセスでシミュレーションしているかどうか
	bool bfirstlocal;					///< 初めて近傍物体になったかどうか
	Flag(){
		bneighbor = false;
		blocal = false;							
		bfirstlocal = false;	
	}
};

struct NeighborPoint{
	Posed last_Pose;						///< 近傍物体の前回のPose
	Vec3d closestPoint;					///< 近傍物体最近点(ローカル座標)
	Vec3d last_closestPoint;			///< 前回の近傍物体の最近点(ローカル座標)
	Vec3d pointerPoint;					///< 力覚ポインタ最近点(ローカル座標)
	Vec3d face_normal;					///< 近傍物体の提示面の法線(近傍物体から力覚ポインタへのベクトル)
	Vec3d last_face_normal;			///< 前回の近傍物体の提示面の法線
	double test_force_norm;			///< 予測シミュレーションで単位力の大きさ
	Vec3d impulse;						///< 物理プロセスが1ステップ終わるまでに力覚ポインタが加えた力積
	NeighborPoint(){
		closestPoint = Vec3d(0.0, 0.0, 0.0);					
		pointerPoint = Vec3d(0.0, 0.0, 0.0);						
		face_normal = Vec3d(0.0, 0.0, 0.0);			
		last_face_normal = Vec3d(0.0, 0.0, 0.0);
		test_force_norm = 0.0;
		impulse = Vec3d();
	}
};

struct SyncInfo{
	struct MotionCoeff motionCoeff;
	struct NeighborPoint neighborPoint;
};

struct ExpandedPHSolid{
	PHSolidIf* phSolidIf;
	PHSolid haSolid;
	struct SyncInfo syncInfo;
	struct Flag flag;
	ExpandedPHSolid(){
		phSolidIf = NULL;
	}
};


#endif
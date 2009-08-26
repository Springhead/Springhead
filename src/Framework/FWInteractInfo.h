#ifndef FWINTERACT_INFO_H
#define FWINTERACT_INFO_H

#include <Springhead.h>
#include <Physics/PHSolid.h>
#include <Base/TMatrix.h>
#include <vector>

using namespace Spr;
using namespace PTM;


/** Local Dynamicsに必要な変数 */
struct Mobility{
	Vec3d force;
	TMatrixRow<6, 3, double> A;		///< LocalDynamicsで使うモビリティ（ユーザの入力により変化）
};
/** ポインタとソリッドの近傍点及び法線 */
struct NeighborInfo{
	Vec3d closest_point;			///< 近傍物体最近点(ローカル座標)
	Posed last_pose;				///< 近傍物体の前回のPose
	Vec3d last_closest_point;		///< 前回の近傍物体の最近点(ローカル座標)
	Vec3d pointer_point;			///< 力覚ポインタ最近点(ローカル座標)
	Vec3d face_normal;				///< 近傍物体の提示面の法線(近傍物体から力覚ポインタへのベクトル)
	Vec3d last_face_normal;			///< 前回の近傍物体の提示面の法線
	double test_force_norm;			///< 予測シミュレーションで単位力の大きさ
	Vec3d test_force;				///< 予測シミュレーションで使うテスト力
	Vec3d impulse;					///< 物理プロセスが1ステップ終わるまでに力覚ポインタが加えた力積
	NeighborInfo(){
		closest_point = Vec3d(0.0, 0.0, 0.0);					
		pointer_point = Vec3d(0.0, 0.0, 0.0);						
		face_normal = Vec3d(0.0, 0.0, 0.0);			
		last_face_normal = Vec3d(0.0, 0.0, 0.0);
		test_force_norm = 0.0;
		impulse = Vec3d();
	}
};
struct SyncInfo{
};
/** ポインタ近傍かどうかのフラグ */
struct Flag{
	bool bneighbor;
	bool blocal;			///< 力覚プロセスでシミュレーションしているかどうか
	bool bfirstlocal;		///< 初めて近傍物体になったかどうか
	Flag(){
		bneighbor = false;
		blocal = false;							
		bfirstlocal = false;	
	}
};

struct FWInteractInfo{
	Flag flag;
	struct Mobility mobility;
	struct NeighborInfo neighborInfo;

};

typedef std::vector<FWInteractInfo> AllInteractInfo;

#endif

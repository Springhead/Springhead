#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <Springhead.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>

namespace Spr{;

class PHConstraintEngine: public PHEngine{
	OBJECTDEF(PHConstraintEngine);

	/// 1つの接触点
	struct PHContactPoint{
		Vec3d point;		/// 接触点の位置
		Matrix3d J[2][2];	/// LCPを構成するヤコビ行列

	};
	typedef std::vector<PHContactPoint> PHContactPoints;

	/// Solid同士の交差
	struct PHContactVolume{
		int solids[2];				/// 接触している剛体
		int shapes[2];				/// 接触している形状
		CDShape* intersection;		/// 交差形状
		Vec3d normal;				/// 法線
		Vec3d center;				/// 交差形状の重心
		PHContactPoints	points;		/// normalに直交する平面へ射影した接触多面体の頂点配列
	};
	typedef std::vector<PHContactVolume> PHContactVolumes;
	

protected:
	PHSolids			solids;		//拘束力計算の対象となる剛体
	PHContactVolumes	contacts;	//剛体同士の接触情報

	//UTCombination<UTRef<PHSolidPair> > solidPairs;
public:
	void Add(PHSolid* s);
	void Init();
	///
	int GetPriority() const {return 0/*SGBP_CONSTRAINTENGINE*/;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	virtual void Clear(PHScene* s){ solids.clear(); }
};

}	//	namespace Spr
#endif

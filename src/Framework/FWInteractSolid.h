#ifndef FWINTERACT_SOLID_H
#define FWINTERACT_SOLID_H

#include <Foundation/Object.h>
#include <Physics/PHSolid.h>

namespace Spr{;

struct FWInteractSolid : public SceneObject{
public:
	bool bfirstSim;
	bool bSim;
	PHSolid* sceneSolid;
	PHSolid copiedSolid;
	SpatialVector b;				///< 予測シミュレーションで求めたモビリティ（重力等の定数項）
	SpatialVector curb;				///< 通常シミュレーションででた定数項
	SpatialVector lastb;			///< 前回の予測シミュレーションで求めた定数項
	FWInteractSolid(){
		bSim = false;
		sceneSolid = NULL;
	}
}; 
typedef std::vector<FWInteractSolid> FWInteractSolids;

}

#endif
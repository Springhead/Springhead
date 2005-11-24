#ifndef PHPENALTYENGINE_H
#define PHPENALTYENGINE_H

#include <Springhead.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>

namespace Spr{;

class PHPenaltyEngine: public PHEngine{
	OBJECTDEF(PHPenaltyEngine);

	class PHSolidPair: public UTRefCount{
	public:
		PHSolidPair(PHSolid* s0, PHSolid* s1){
			solid[0] = s0;
			solid[1] = s1;
		}
		PHSolid* solid[2];
		UTCombination<UTRef< CDShapePair> > shapePairs;
		void Init();

		bool Detect(PHPenaltyEngine* engine);
	};
protected:
	PHSolids solids;
	UTCombination<UTRef<PHSolidPair> > solidPairs;
public:
	void Add(PHSolid* s);
	void Init();
	///
	int GetPriority() const {return SGBP_PENALTYENGINE;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	virtual void Clear(PHScene* s){ solids.clear(); }
};

}	//	namespace Spr
#endif

#ifndef FWINTERACT_POINTERIF_H
#define FWINTERACT_POINTERIF_H

#include <Foundation/SprObject.h>
#include <Physics/SprPHSolid.h>
#include <HumanInterface/SprHIBase.h>

namespace Spr{;

struct FWInteractPointerDesc{
public:
	PHSolidIf*	pointerSolid;
	HIBaseIf*	humanInterface; 
	double		springK;
	double		damperD;
	double		posScale;
	double		localRange;
	FWInteractPointerDesc(){ Init(); }
	void Init(){
		pointerSolid  = NULL;
		humanInterface = NULL;
		springK = 0.0;
		damperD = 0.0;
		posScale = 1.0;
		localRange = 0.1;
	}
};

struct FWInteractPointerIf : public SceneObjectIf{
public:
	SPR_IFDEF(FWInteractPointer);

	void		SetPointerSolid(PHSolidIf* solid);
	PHSolidIf*	GetPointerSolid();
	void		SetHI(HIBaseIf* hi);
	HIBaseIf*	GetHI();
	void		SetPosScale(double s);
	double		GetPosScale();
	void		SetLocalRange(double r);
	double		GetLocalRange();
};

//typedef UTRef<FWInteractPointerIf> UTRef_FWInteractPointerIf;
typedef std::vector< UTRef<FWInteractPointerIf> > FWInteractPointers;

}
#endif
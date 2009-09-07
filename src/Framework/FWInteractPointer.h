#ifndef FWINTERACT_POINTER_H
#define FWINTERACT_POINTER_H

#include <Foundation/Object.h>
#include <Framework/SprFWInteractPointer.h>
#include <Physics/PHSolid.h>
#include <Framework/FWInteractInfo.h>

namespace Spr{;

class FWInteractPointer : public SceneObject, public FWInteractPointerDesc{
public:
	SPR_OBJECTDEF(FWInteractPointer);
	PHSolid			hiSolid;
	AllInteractInfo interactInfo;
	bool			bForce;
	bool			bVibration;
	std::vector<bool>	bContact;

	FWInteractPointer();
	FWInteractPointer(const FWInteractPointerDesc& desc);

	void		Init();
	void		SetPointerSolid(PHSolidIf* solid);
	PHSolidIf*	GetPointerSolid();
	void		SetHI(HIBaseIf* hi);
	HIBaseIf*	GetHI();
	void		SetPosScale(double s);
	double		GetPosScale();
	void		SetForceScale(double s);
	double		GetForceScale();
	void		SetLocalRange(double r);
	double		GetLocalRange();
	void		SetDefaultPosition(Posed p);
	Posed		GetDefaultPosition();
	void		EnableForce(bool b = true);
	void		EnableVibration(bool b = true);
	void		Calibration();

	/*std::vector<  UTRef< FWObjectIf >  >	copiedBiscuits;*/
};

}
#endif
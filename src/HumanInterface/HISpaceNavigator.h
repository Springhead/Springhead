#ifndef HISPACENAVIGATOR_H
#define HISPACENAVIGATOR_H

#include <HumanInterface/HIBase.h>

namespace Spr{;

class HISpaceNavigator: public HIPose{
public:
	SPR_OBJECTDEF(HISpaceNavigator);

	HISpaceNavigator(const HISpaceNavigatorDesc& desc = HISpaceNavigatorDesc()) {}

	virtual bool			Init(const void* desc);
	virtual bool			Calibration();
	virtual bool			SetPose(Posef pose);
	virtual Vec3f			GetPosition();
	virtual Quaternionf		GetOrientation();
	virtual Posef			GetPose();
	virtual Affinef			GetAffine();

	virtual bool			PreviewMessage(void *m);

private:
	Posef currPose;
	void *hWnd;

	void Translate(Vec3f trn);
	void Rotate(Vec3f rot);
};

}

#endif//HISPACENAVIGATOR_H
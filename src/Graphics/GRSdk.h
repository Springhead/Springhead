#ifndef GRSDK_H
#define GRSDK_H
#include <Graphics/GRRender.h>


namespace Spr {;

class SPR_DLL GRSdk:public GRSdkIf, public Object{
protected:
public:
	OBJECTDEF(GRSdk);
	~GRSdk();
	void Print(std::ostream& os) const {}
	BASEIMP_OBJECT(Object);
	GRRenderIf* CreateRender();
	GRDeviceIf* CreateDevice();
public:
};

}
#endif

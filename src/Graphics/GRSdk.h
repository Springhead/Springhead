#ifndef GRSDK_H
#define GRSDK_H
#include <SprGraphics.h>


namespace Spr {;

class SPR_DLL GRSdk:public GRSdkIf, public Object{
protected:
	std::vector< UTRef<Object> > objects;
public:
	OBJECTDEF(GRSdk);
	~GRSdk();
	void Print(std::ostream& os) const {}
	BASEIMP_OBJECT(Object);
	GRDebugRenderIf* CreateDebugRender();
	GRDeviceGLIf* CreateDeviceGL();
public:
};

}
#endif

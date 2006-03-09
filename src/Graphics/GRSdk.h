/**
 *	@file GRSdk.h
 *	@brief グラフィックスSDK
 */
#ifndef GRSDK_H
#define GRSDK_H
#include <SprGraphics.h>


namespace Spr {;

/**	@class	GRSdk
    @brief	グラフィックスSDK　 */
class SPR_DLL GRSdk:public InheritObject<GRSdkIf, Object>{
protected:
	std::vector< UTRef<Object> > objects;
public:
	OBJECTDEF(GRSdk);
	~GRSdk();
	void Print(std::ostream& os) const {}
	GRDebugRenderIf* CreateDebugRender();
	GRDeviceGLIf* CreateDeviceGL(int window);
};

}
#endif

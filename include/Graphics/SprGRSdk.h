#ifndef SPR_GRSDK_H
#define SPR_GRSDK_H

#include <Springhead.h>

namespace Spr{;

struct GRDeviceGLIf;
struct GRDebugRenderIf;

/**	グラフィックスSDK	*/
struct GRSdkIf: public ObjectIf{
	IF_DEF(GRSdk);
	virtual GRDebugRenderIf* CreateDebugRender()=0;
	virtual GRDeviceGLIf* CreateDeviceGL(int window)=0;
};
GRSdkIf* _cdecl CreateGRSdk();

}
#endif

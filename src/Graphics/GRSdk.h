/**
 *	@file GRSdk.h
 *	@brief グラフィックスSDK
 */
#ifndef GRSDK_H
#define GRSDK_H
#include <SprGraphics.h>


namespace Spr {;
struct GRSceneIf;

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
	GRSceneIf* GRSdk::CreateScene();
	virtual size_t NChildObject() const { return objects.size(); }
	virtual ObjectIf* GetChildObject(size_t i);
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc);
};

}
#endif

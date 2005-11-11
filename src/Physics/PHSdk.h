#ifndef PHSDK_H
#define PHSDK_H
#include <Foundation/Scene.h>
#include <Physics/SprPHSdk.h>

namespace Spr {;

class SPR_DLL PHSdk:public PHSdkIf, public Object{
	OBJECTDEF(PHSdk);
	void Print(std::ostream& os) const {}
	BASEIMP_OBJECT(Object);
	PHSceneIf* CreateScene();
public:
};

}
#endif

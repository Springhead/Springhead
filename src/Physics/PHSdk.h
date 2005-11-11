#ifndef PHSDK_H
#define PHSDK_H
#include <Foundation/Scene.h>
#include <Physics/SprPHSdk.h>

namespace Spr {;

class SPR_DLL PHSdk:public PHSdkIf, public Object{
protected:
	typedef std::vector< UTRef<PHSceneIf> > Scenes;
	Scenes scenes;
public:
	OBJECTDEF(PHSdk);
	~PHSdk();
	void Print(std::ostream& os) const {}
	BASEIMP_OBJECT(Object);
	PHSceneIf* CreateScene();
public:
};

}
#endif

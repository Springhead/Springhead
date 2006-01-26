#ifndef PHSDK_H
#define PHSDK_H
#include <Foundation/Scene.h>
#include <Physics/SprPHSdk.h>

namespace Spr {;

class SPR_DLL PHSdk:public PHSdkIf, public NameManager{
protected:
	//	scene
	typedef std::vector< UTRef<PHSceneIf> > Scenes;
	Scenes scenes;
	//	shape
	typedef std::vector< UTRef<CDShapeIf> > Shapes;
	Shapes shapes;

public:
	OBJECTDEF(PHSdk);
	~PHSdk();
	void Print(std::ostream& os) const {}
	BASEIMP_OBJECT(Object);
	virtual PHSceneIf* CreateScene();
	virtual int GetNScene();
	virtual PHSceneIf** GetScenes();

	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc);
	virtual int GetNShape();
	virtual CDShapeIf** GetShapes();
public:
};

}
#endif

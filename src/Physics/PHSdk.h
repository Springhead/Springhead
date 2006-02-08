#ifndef PHSDK_H
#define PHSDK_H
#include <Foundation/Scene.h>
#include <Physics/SprPHSdk.h>

namespace Spr {;

class SPR_DLL PHSdk:public InheritNameManager<PHSdkIf, NameManager>{
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
	void Print(std::ostream& os) const;
	virtual PHSceneIf* CreateScene();
	virtual PHSceneIf* CreateScene(const PHSceneDesc& desc);
	virtual int GetNScene();
	virtual PHSceneIf** GetScenes();

	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc);
	virtual int GetNShape();
	virtual CDShapeIf** GetShapes();
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
public:
};

}
#endif

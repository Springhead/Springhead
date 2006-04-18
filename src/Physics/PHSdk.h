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
	virtual PHSceneIf* CreateScene();
	virtual PHSceneIf* CreateScene(const PHSceneDesc& desc);
	virtual int NScene();
	virtual PHSceneIf** GetScenes();

	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc);
	virtual int NShape();
	virtual CDShapeIf** GetShapes();
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual size_t NChildObject() const { return scenes.size()+shapes.size(); }
	virtual ObjectIf* GetChildObject(size_t i);
public:
};

}
#endif

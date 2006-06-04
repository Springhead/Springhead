#ifndef PHSDK_H
#define PHSDK_H
#include <Foundation/Scene.h>
#include <Physics/SprPHSdk.h>

namespace Spr {;

class SPR_DLL PHSdk:public InheritNameManager<PHSdkIf, NameManager>{
protected:
	//	scene
	typedef std::vector< UTRef<PHSceneIf> > Scenes;
	///	シーン
	Scenes scenes;
	//	shape
	typedef std::vector< UTRef<CDShapeIf> > Shapes;
	///	形状
	Shapes shapes;
	///	他のオブジェクトたち
	Objects objects;

public:
	OBJECT_DEF(PHSdk);
	PHSdk();
	~PHSdk();
	virtual PHSceneIf* CreateScene();
	virtual PHSceneIf* CreateScene(const PHSceneDesc& desc);
	virtual int NScene();
	virtual PHSceneIf** GetScenes();

	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc);
	virtual int NShape();
	virtual CDShapeIf* GetShape(int i);
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual size_t NChildObject() const { return scenes.size()+shapes.size(); }
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
public:
};

}
#endif

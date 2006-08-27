/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSDK_H
#define PHSDK_H
#include <Foundation/Foundation.h>
#include <Physics/SprPHSdk.h>

namespace Spr {;

PHSdkIf* SPR_CDECL CreatePHSdk();
class PHSdkFactory : public SdkFactoryBase {
public:
	const IfInfo* GetIfInfo() const {
		return PHSdkIf::GetIfInfoStatic();
	}
	ObjectIf* Create(const void* desc){
		return CreatePHSdk();
	}
};

class SPR_DLL PHSdk:public InheritNameManager<PHSdkIf, NameManager>{
protected:
	//	型情報DB
	static UTRef<UTTypeDescDb> typeDb;
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
	static UTTypeDescDb* GetTypeDb();
	virtual PHSceneIf* CreateScene();
	virtual PHSceneIf* CreateScene(const PHSceneDesc& desc);
	virtual int NScene();
	virtual PHSceneIf* GetScene(size_t i);

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

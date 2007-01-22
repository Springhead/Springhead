/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWSDK_H
#define FWSDK_H
#include <Foundation/Foundation.h>
#include <Framework/SprFWSdk.h>
#include <Graphics/SprGRRender.h>

namespace Spr {;

class FWSdkFactory : public FactoryBase {
public:
	const IfInfo* GetIfInfo() const {
		return FWSdkIf::GetIfInfoStatic();
	}
	ObjectIf* Create(const void* desc, const ObjectIf*){
		return FWSdkIf::CreateSdk();
	}
};

class SPR_DLL FWSdk:public Sdk, public FWSdkIfInit{
protected:
	//	å^èÓïÒDB
	static UTRef<UTTypeDescDb> typeDb, oldSpringheadTypeDb;
	//	scene
	typedef std::vector< UTRef<FWSceneIf> > Scenes;
	///	ÉVÅ[Éì
	Scenes scenes;
	// SDKs
	UTRef<PHSdkIf> phSdk;
	UTRef<GRSdkIf> grSdk;
	UTRef<FISdkIf> fiSdk;
	// Graphics
	UTRef<GRRenderIf> grRender;
	UTRef<GRDeviceIf> grDevice;

public:
	OBJECTDEF(FWSdk, Sdk);
	FWSdk();
	~FWSdk();
	virtual FWSceneIf* CreateScene(const FWSceneDesc& desc);
	virtual int NScene() const;
	virtual FWSceneIf* GetScene(size_t i);

	virtual size_t NChildObject() const { return NScene(); }
	virtual ObjectIf* GetChildObject(size_t i){ return GetScene(i); }
	virtual bool AddChildObject(ObjectIf* o);
	PHSdkIf* GetPHSdk(){ return phSdk; }
	GRSdkIf* GetGRSdk(){ return grSdk; }
	FISdkIf* GetFISdk(){ return fiSdk; }
	
public:
	virtual void ClearObjects();
	virtual void Step();
	virtual void Draw();
	virtual void Reshape(int w, int h);
	virtual void Keyboard(unsigned char key, int x, int y);

protected:
	void CreateSdks();
};

}
#endif

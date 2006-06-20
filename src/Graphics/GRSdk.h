/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file GRSdk.h
 *	@brief グラフィックスSDK
 */
#ifndef GRSDK_H
#define GRSDK_H
#include <SprGraphics.h>
#include <Graphics/GRScene.h>


namespace Spr {;
struct GRSceneIf;

/**	@class	GRSdk
    @brief	グラフィックスSDK　 */
class SPR_DLL GRSdk:public InheritObject<GRSdkIf, NameManager>, public GRSdkDesc{
protected:
	typedef std::vector< UTRef<Object> > Objects;
	typedef std::vector< UTRef<GRScene> > GRScenes;
	Objects objects;
	GRScenes scenes;
public:
	OBJECT_DEF(GRSdk);
	GRSdk(const GRSdkDesc& = GRSdkDesc());
	~GRSdk();
	virtual void Print(std::ostream& os) const {}
	virtual GRDebugRenderIf* CreateDebugRender();
	virtual GRDeviceGLIf* CreateDeviceGL(int window);
	virtual GRSceneIf* GetScene(size_t i);
	virtual size_t NScene(){ return scenes.size(); }
	virtual size_t NChildObject() const { return scenes.size() + objects.size(); }
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual GRSceneIf* CreateScene();
};

}
#endif

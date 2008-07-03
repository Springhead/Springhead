/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

class SPR_DLL FWSdk:public Sdk{
protected:
	///	シーン
	typedef std::vector< UTRef<FWSceneIf> > FWScenes;	//< GRRenderにもScenesがあってScenes型typedefでは上手く動かない．
	FWScenes scenes;
	
	/// レンダラ
	typedef std::vector< UTRef<GRRenderIf> > Renders;
	Renders renders;

	/// アクティブシーンとレンダラ
	FWSceneIf*	curScene;
	GRRenderIf*	curRender;

	// SDKs
	UTRef<PHSdkIf> phSdk;
	UTRef<GRSdkIf> grSdk;
	UTRef<FISdkIf> fiSdk;
	
	bool debugMode;
public:
	SPR_OBJECTDEF(FWSdk);
	FWSdk();
	~FWSdk();

	virtual FWSceneIf* CreateScene(const PHSceneDesc& phdesc = PHSceneDesc(), const GRSceneDesc& grdesc = GRSceneDesc());
	virtual bool LoadScene(UTString filename);
	virtual bool SaveScene(UTString filename);
	virtual int NScene() const{	return (int)scenes.size(); }
	virtual void SwitchScene(FWSceneIf* scene){ curScene = scene; }
	virtual FWSceneIf* GetScene(int i = -1);
	virtual void MergeScene(FWSceneIf* scene0, FWSceneIf* scene1);

	virtual GRRenderIf*	CreateRender();
	virtual int NRender() const{return (int)renders.size();}
	virtual GRRenderIf* GetRender(int index = -1);
	virtual void SwitchRender(GRRenderIf* render){ curRender = render; }

	virtual bool GetDebugMode(){return debugMode;}
	virtual void SetDebugMode(bool debug = true){debugMode = debug;}

	virtual size_t NChildObject() const { return NScene(); }
	virtual ObjectIf* GetChildObject(size_t i){ return GetScene((int)i); }
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual PHSdkIf* GetPHSdk(){ return phSdk; }
	virtual GRSdkIf* GetGRSdk(){ return grSdk; }
	virtual FISdkIf* GetFISdk(){ return fiSdk; }

	virtual void Clear();
	virtual void Step();
	virtual void Draw();
	virtual void Reshape(int w, int h);
	
protected:
	void CreateSdks();
};
}
#endif

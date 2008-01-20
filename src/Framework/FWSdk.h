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

class SPR_DLL FWSdk:public Sdk, public FWSdkIfInit{
protected:
	//	scene
	typedef std::vector< UTRef<FWSceneIf> > Scenes;
	///	シーン
	Scenes scenes;
	/// アクティブシーン
	FWSceneIf* fwScene;
	// SDKs
	UTRef<PHSdkIf> phSdk;
	UTRef<GRSdkIf> grSdk;
	UTRef<FISdkIf> fiSdk;
	// Graphics
	std::set<FWWin> wins;
	bool debugMode;
public:
	OBJECTDEF(FWSdk, Sdk);
	FWSdk();
	~FWSdk();
	virtual FWSceneIf* CreateScene(const PHSceneDesc& phdesc = PHSceneDesc(), const GRSceneDesc& grdesc = GRSceneDesc());
	virtual FWWin* CreateWin(int wid, GRRenderIf* r);
	virtual bool LoadScene(UTString filename);
	virtual bool SaveScene(UTString filename);
	virtual int NScene() const;
	virtual void SwitchScene(FWSceneIf* scene){ fwScene = scene; }
	virtual FWSceneIf* GetScene(int i = -1);
	virtual void MergeScene(FWSceneIf* scene0, FWSceneIf* scene1);
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
	virtual void Draw(FWWin* cur);
	virtual void Reshape(FWWin* cur, int w, int h);
	virtual FWWin* GetWinFromId(int wid);
	virtual int NWin(){ return (int)wins.size(); }
	virtual FWWin* GetWin(int pos);
	virtual void AssignScene(FWWin* win);

protected:
	void CreateSdks();
};
}
#endif

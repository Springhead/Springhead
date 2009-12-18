/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWINTERACT_SCENEIF_H
#define SPR_FWINTERACT_SCENEIF_H

#include <Foundation/SprScene.h>
#include <Framework/SprFWScene.h>
#include <Framework/SprFWInteractPointer.h>


namespace Spr{;

class FWInteractAdaptee;

typedef enum FWInteractMode{
	NONE,
	LOCAL_DYNAMICS,
	VIRTUAL_COUPLING
}IAMode;

typedef enum FWHapticMode{
	PENALTY,
	PENALTY6D,
	CONSTRAINT,
	PROXY,
	PROXYSIMULATION
}HMode;

struct FWInteractSceneDesc{
public:
	FWSceneIf* fwScene;
	FWInteractMode iaMode;
	FWHapticMode hMode;
	double hdt;
	FWInteractSceneDesc(){Init();}
	void Init(){
		fwScene = NULL;
		iaMode = NONE;
		hMode = PENALTY;
		hdt = 0.001;
	}
};

struct FWInteractSceneIf : public SceneIf{
public:
	SPR_IFDEF(FWInteractScene);


	FWSceneIf* GetScene();
	void CreateIAAdaptee(FWInteractMode mode);
	FWInteractAdaptee*		GetIAAdaptee();
	FWInteractPointerIf*	CreateIAPointer(const FWInteractPointerDesc& desc);
	FWInteractPointerIf*	GetIAPointer(int i = -1);
	int						NIAPointers();
	FWInteractMode			GetIAMode();
	FWHapticMode			GetHMode();
	void SetHMode(FWHapticMode hMode);

	/// à»â∫ÇÃä÷êîÇÕadapteeÇÇ∆Ç®ÇµÇƒé¿ëïÇ™åƒÇŒÇÍÇÈ
	void Init();
	void Clear();
	void Step();
	void CallBackHapticLoop();
	Vec3d* GetProxyPoint();
	Vec3d* GetForce();
	bool GetContactFlag();
	Vec3d GetPicCenterPosition();
};

typedef UTRef<FWInteractSceneIf> UTRef_FWInteractSceneIf;
typedef std::vector<UTRef_FWInteractSceneIf> FWInteractScenes;

}
#endif

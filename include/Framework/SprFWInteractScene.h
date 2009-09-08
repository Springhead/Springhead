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
#include <Framework/FWInteractAdaptee.h>

namespace Spr{;

class FWInteractAdaptee;

typedef enum FWInteractMode{
	NONE,
	LOCAL_DYNAMICS
}INMode;

typedef enum FWHapticMode{
	NOMAL,
	PROXY,
	PRO_SIMU
}HMode;

struct FWInteractSceneDesc{
public:
	FWSceneIf* fwScene;
	FWInteractMode mode;
	FWHapticMode hmode;
	double hdt;
	FWInteractSceneDesc(){Init();}
	void Init(){
		fwScene = NULL;
		mode = NONE;
		hmode = NOMAL;
		hdt = 0.001;
	}
};

struct FWInteractSceneIf : public SceneIf{
public:
	SPR_IFDEF(FWInteractScene);


	FWSceneIf* GetScene();
	void CreateINAdaptee(FWInteractMode inMode);
	FWInteractAdaptee*		GetINAdaptee();
	FWInteractPointerIf*	CreateINPointer(const FWInteractPointerDesc& desc);
	FWInteractPointerIf*	GetINPointer(int i = -1);
	int						NINPointers();
	FWInteractMode			GetINMode();
	FWHapticMode			GetHMode();

	/// à»â∫ÇÃä÷êîÇÕadapteeÇÇ∆Ç®ÇµÇƒé¿ëïÇ™åƒÇŒÇÍÇÈ
	void Init();
	void Clear();
	void Step();
	void CallBackHapticLoop();
};

typedef UTRef<FWInteractSceneIf> UTRef_FWInteractSceneIf;
typedef std::vector<UTRef_FWInteractSceneIf> FWInteractScenes;

}
#endif

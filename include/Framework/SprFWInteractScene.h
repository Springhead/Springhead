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

enum FWInteractMode{
	NONE,
	LOCAL_DYNAMICS
};

struct FWInteractSceneDesc{
public:
	FWSceneIf* fwScene;
	FWInteractMode mode;
	double hdt;
	FWInteractSceneDesc(){Init();}
	void Init(){
		fwScene = NULL;
		mode = NONE;
		hdt = 0.001;
	}
};

struct FWInteractSceneIf : public SceneIf{
public:
	SPR_IFDEF(FWInteractScene);

	FWSceneIf* GetScene();
	void CreateInteractAdaptee(FWInteractMode iMode);
	FWInteractPointerIf*	CreateInteractPointer(const FWInteractPointerDesc& desc);
	FWInteractPointerIf*	GetInteractPointer(int i = -1);
	int						NInteractPointers();
	FWInteractMode			GetInteractMode();

	/// à»â∫ÇÃä÷êîÇÕadapteeÇÇ∆Ç®ÇµÇƒé¿ëïÇ™åƒÇŒÇÍÇÈ
	void Init();
	void Step();
	void Clear();
	void Reset();
	void CallBackHapticLoop();
};

}
#endif

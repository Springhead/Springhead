/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWINTERACT_SCENE_H
#define FWINTERACT_SCENE_H

#include <Framework/SprFWInteractScene.h>
#include <Foundation/Scene.h>
#include <Framework/FWInteractPointer.h>
#include <Framework/FWInteractSolid.h>
#include <Framework/FWInteractAdaptee.h>

namespace Spr{;

class SPR_DLL FWInteractScene : public Scene, public FWInteractSceneDesc{
protected:
	FWInteractPointerIf*		curIAPointer;
	FWInteractPointers			interactPointers;
	FWInteractSolids			interactSolids;
	UTRef<FWInteractAdaptee>	interactAdaptee;
public:
	SPR_OBJECTDEF(FWInteractScene);
	FWInteractScene();
	FWInteractScene(const FWInteractSceneDesc& desc);

	FWSceneIf* GetScene();

	/// Adapteeä÷åW
	void					CreateIAAdaptee(FWInteractMode mode);
	FWInteractAdaptee*		GetIAAdaptee();
	FWInteractMode			GetIAMode();
	FWHapticMode			GetHMode();
	void					SetHMode(FWHapticMode hMode);

	/// IAPointerä÷åW
	FWInteractPointerIf*	CreateIAPointer(const FWInteractPointerDesc& desc);
	FWInteractPointerIf*	GetIAPointer(int i = -1);
	FWInteractPointers*		GetIAPointers();
	int						NIAPointers();

	/// IASolidä÷åW
	FWInteractSolid*		GetIASolid(int i);
	FWInteractSolids*		GetIASolids();
	int						NIASolids();


	/// à»â∫ÇÃä÷êîÇÕadapteeÇÇ∆Ç®ÇµÇƒé¿ëïÇ™åƒÇŒÇÍÇÈ
	void Init();
	void Clear();
	void Step();
	void CallBackHapticLoop();
	void BeginKeyboard();
	void EndKeyboard();
	//Vec3d* GetProxyPoint();
	//Vec3d* GetForce();
	bool GetContactFlag();
	Vec3d GetPicCenterPosition();
};


}
#endif
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
	FWInteractPointerIf*		curINPointer;
	FWInteractPointers			interactPointers;
	FWInteractSolids			interactSolids;
	UTRef<FWInteractAdaptee>	interactAdaptee;
public:
	SPR_OBJECTDEF(FWInteractScene);
	FWInteractScene();
	FWInteractScene(const FWInteractSceneDesc& desc);

	FWSceneIf* GetScene();

	/// Adapteeä÷åW
	void					CreateINAdaptee(FWInteractMode inMode);
	FWInteractAdaptee*		GetINAdaptee();
	FWInteractMode			GetINMode();
	FWHapticMode			GetHMode();

	/// INPointerä÷åW
	FWInteractPointerIf*	CreateINPointer(const FWInteractPointerDesc& desc);
	FWInteractPointerIf*	GetINPointer(int i = -1);
	FWInteractPointers*		GetINPointers();
	int						NINPointers();

	/// INSolidä÷åW
	FWInteractSolid*		GetINSolid(int i);
	FWInteractSolids*		GetINSolids();
	int						NINSolids();


	/// à»â∫ÇÃä÷êîÇÕadapteeÇÇ∆Ç®ÇµÇƒé¿ëïÇ™åƒÇŒÇÍÇÈ
	void Init();
	void Step();
	void Clear();
	void Reset();
	void CallBackHapticLoop();

};


}
#endif
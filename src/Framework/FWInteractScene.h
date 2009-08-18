#ifndef FWINTERACT_SCENE_H
#define FWINTERACT_SCENE_H

#include <Framework/SprFWInteractScene.h>
#include <Foundation/Scene.h>
#include <Framework/FWInteractPointer.h>
#include <Framework/FWInteractAdaptee.h>

namespace Spr{;


class SPR_DLL FWInteractScene : public Scene, public FWInteractSceneDesc{
protected:
	FWInteractPointerIf*		curiPointer;
	FWInteractPointers			interactPointers;
	UTRef<FWInteractAdaptee>	iAdaptee;
public:
	SPR_OBJECTDEF(FWInteractScene);
	FWInteractScene();
	FWInteractScene(const FWInteractSceneDesc& desc);

	FWSceneIf* GetScene();
	void CreateInteractAdaptee(FWInteractMode iMode);
	FWInteractAdaptee* GetInteractAdaptee();
	FWInteractPointerIf*	CreateInteractPointer(const FWInteractPointerDesc& desc);
	FWInteractPointerIf*	GetInteractPointer(int i = -1);
	FWInteractPointers*		GetInteractPointers();
	int						NInteractPointers();
	FWInteractMode			GetInteractMode(){ return mode; }

	/// à»â∫ÇÃä÷êîÇÕadapteeÇÇ∆Ç®ÇµÇƒé¿ëïÇ™åƒÇŒÇÍÇÈ
	void Init();
	void Step();
	void Clear();
	void Reset();
	void CallBackHapticLoop();

};
typedef UTRef<FWInteractSceneIf> UTRef_FWInteractSceneIf;
typedef std::vector<UTRef_FWInteractSceneIf> FWInteractScenes;

}
#endif
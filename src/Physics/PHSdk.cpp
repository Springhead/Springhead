#include <Springhead.h>
#include <Foundation/Object.h>
#include "PHSdk.h"
#include "PHScene.h"

#pragma hdrstop


namespace Spr{;

//----------------------------------------------------------------------------
//	PHScene
OBJECTIMP(PHSdk, Object);
PHSceneIf* PHSdk::CreateScene(){
	return new PHScene;
}

PHSdkIf* CreatePHSdk(){
	return new PHSdk;
}

}

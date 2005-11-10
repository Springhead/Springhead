#include <FileIO/FIDocScene.h>
#pragma hdrstop

namespace Spr{;
void SPR_DLL RegisterLoaderForCollision(FILoadScene* l){
	REGISTER_LOADER(l, CDMesh);
	REGISTER_LOADER(l, CDSphere);
	REGISTER_LOADER(l, CDPhysicalMaterial);
}

//-------------------------------------------------------------------
//	Saver
//-------------------------------------------------------------------

void SPR_DLL RegisterSaverForCollision(FISaveScene* l){
	REGISTER_SAVER(l, CDPhysicalMaterial);
}
}

void RegisterLoaderForCollision(Spr::FILoadScene* l){
	Spr::RegisterLoaderForCollision(l);
}
void SPR_DLL RegisterSaverForCollision(Spr::FISaveScene* s){
	Spr::RegisterSaverForCollision(s);
}




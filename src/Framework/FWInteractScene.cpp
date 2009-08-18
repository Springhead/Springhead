#include <Framework/FWInteractScene.h>
#include <Framework/FWLDHaptic.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
FWInteractScene::FWInteractScene(){}
FWInteractScene::FWInteractScene(const FWInteractSceneDesc &desc) : FWInteractSceneDesc(desc){}

FWSceneIf* FWInteractScene::GetScene(){
	return fwScene; 
}
void FWInteractScene::CreateInteractAdaptee(FWInteractMode iMode){
	FWInteractAdaptee* ia;
	switch(mode){
	case NONE:
		ia = NULL;
		break;
	case LOCAL_DYNAMICS:
		ia = DBG_NEW FWLDHaptic();
		ia->SetInteractScene(this);
		break;
	default:
		ia = NULL;
		break;
	}
	iAdaptee = ia;
	iAdaptee->Init();
}
FWInteractAdaptee* FWInteractScene::GetInteractAdaptee(){
	return iAdaptee;
}
FWInteractPointerIf* FWInteractScene::CreateInteractPointer(const FWInteractPointerDesc& desc){
	FWInteractPointer* iPointer = DBG_NEW FWInteractPointer(desc); 
	interactPointers.push_back(iPointer->Cast()); 
	curiPointer = iPointer->Cast();
	return curiPointer;
}
FWInteractPointerIf* FWInteractScene::GetInteractPointer(int i){ 
	if(i == -1) return curiPointer;
	if(0 <= i && i < NInteractPointers()) return interactPointers[i];
	return NULL;
}
FWInteractPointers*	FWInteractScene::GetInteractPointers(){
	return &interactPointers;
}
int FWInteractScene::NInteractPointers(){ 
	return (int)interactPointers.size(); 
}


void FWInteractScene::Init(){
	GetInteractAdaptee()->Init();
}
void FWInteractScene::Step(){
	GetInteractAdaptee()->Step();
}
void FWInteractScene::Clear(){
	GetInteractAdaptee()->Clear();	
}
void FWInteractScene::Reset(){
	GetInteractAdaptee()->Reset();
}
void FWInteractScene::CallBackHapticLoop(){
	GetInteractAdaptee()->CallBackHapticLoop();
}




}
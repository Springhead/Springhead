#include <Framework/FWInteractScene.h>
#include <Framework/FWLDHaptic.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
FWInteractScene::FWInteractScene(){}
FWInteractScene::FWInteractScene(const FWInteractSceneDesc &desc) : FWInteractSceneDesc(desc){}

FWSceneIf* FWInteractScene::GetScene(){ return fwScene; }
/// AdapteeŠÖŒW
void FWInteractScene::CreateINAdaptee(FWInteractMode inMode){
	FWInteractAdaptee* ia;
	switch(mode){
	case NONE:
		ia = NULL;
		break;
	case LOCAL_DYNAMICS:
		ia = DBG_NEW FWLDHaptic();
		ia->SetINScene(this);
		break;
	default:
		ia = NULL;
		break;
	}
	interactAdaptee = ia;
	interactAdaptee->Init();
}
FWInteractAdaptee* FWInteractScene::GetINAdaptee(){ return interactAdaptee; }
FWInteractMode FWInteractScene::GetINMode(){ return mode; }
FWHapticMode FWInteractScene::GetHMode(){ return hmode; }

/// INPointerŠÖŒW
FWInteractPointerIf* FWInteractScene::CreateINPointer(const FWInteractPointerDesc& desc){
	FWInteractPointer* inPointer = DBG_NEW FWInteractPointer(desc); 
	interactPointers.push_back(inPointer->Cast()); 
	curINPointer = inPointer->Cast();
	return curINPointer;
}
FWInteractPointerIf* FWInteractScene::GetINPointer(int i){ 
	if(i == -1) return curINPointer;
	if(0 <= i && i < NINPointers()) return interactPointers[i];
	return NULL;
}
FWInteractPointers*	FWInteractScene::GetINPointers(){ return &interactPointers; }
int FWInteractScene::NINPointers(){ return (int)interactPointers.size(); }

/// INSolidŠÖŒW
FWInteractSolid* FWInteractScene::GetINSolid(int i){
	if(0 <= i && i < NINSolids()) return &interactSolids[i];
	return NULL;
}
FWInteractSolids* FWInteractScene::GetINSolids(){ return &interactSolids; }
int FWInteractScene::NINSolids(){ return (int)interactSolids.size(); }

/// 
void FWInteractScene::Init(){
	GetINAdaptee()->Init();
}
void FWInteractScene::Step(){
	GetINAdaptee()->Step();
}
void FWInteractScene::Clear(){
	GetINAdaptee()->Clear();	
}
void FWInteractScene::Reset(){
	GetINAdaptee()->Reset();
}
void FWInteractScene::CallBackHapticLoop(){
	GetINAdaptee()->CallBackHapticLoop();
}
void FWInteractScene::BeginKeyboard(){
	GetINAdaptee()->BeginKeyboard();
}
void FWInteractScene::EndKeyboard(){
	GetINAdaptee()->EndKeyboard();
}

}
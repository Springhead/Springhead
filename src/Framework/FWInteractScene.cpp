/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWInteractScene.h>
#include <Framework/FWLDHaptic.h>
#include <Framework/FWVirtualCoupling.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
FWInteractScene::FWInteractScene(){}
FWInteractScene::FWInteractScene(const FWInteractSceneDesc &desc) : FWInteractSceneDesc(desc){
}

FWSceneIf* FWInteractScene::GetScene(){ return fwScene; }
/// IAAdapteeŠÖŒW
void FWInteractScene::CreateIAAdaptee(FWInteractMode iaMode){
	FWInteractAdaptee* ia;
	switch(iaMode){
	case NONE:
		ia = NULL;
		break;
	case LOCAL_DYNAMICS:
		ia = DBG_NEW FWLDHaptic();
		ia->SetIAScene(this);
		break;
	case VIRTUAL_COUPLING:
		ia = DBG_NEW FWVirtualCoupling();
		ia->SetIAScene(this);
		break;
	default:
		ia = NULL;
		break;
	}
	interactAdaptee = ia;
	interactAdaptee->Init();
}
FWInteractAdaptee* FWInteractScene::GetIAAdaptee(){ return interactAdaptee; }
FWInteractMode FWInteractScene::GetIAMode(){ return iaMode; }
FWHapticMode FWInteractScene::GetHMode(){ return hMode; }
void FWInteractScene::SetHMode(FWHapticMode hMode){
	GetIAAdaptee()->SetHMode(hMode);
}

/// IAPointerŠÖŒW
FWInteractPointerIf* FWInteractScene::CreateIAPointer(const FWInteractPointerDesc& desc){
	FWInteractPointer* iPointer = DBG_NEW FWInteractPointer(desc); 
	FWInteractPointerIf* iPointerIf = iPointer->Cast();
	interactPointers.push_back(iPointerIf); 
	curIAPointer = iPointer->Cast();
	switch(iaMode){
		case FWInteractMode::NONE:
			break;
		case FWInteractMode::LOCAL_DYNAMICS:
			{
				GetScene()->GetPHScene()->SetContactMode(iPointer->pointerSolid, PHSceneDesc::MODE_NONE);
				iPointer->pointerSolid->SetDynamical(false);
				iPointer->pointerSolid->SetIntegrate(false);
				break;
			}
		case FWInteractMode::VIRTUAL_COUPLING:
			{
				FWVirtualCoupling* ip = (FWVirtualCoupling*)GetIAAdaptee();
				ip->CreatePointerSolid();
				break;
			}
		default:
			break;
	}
	return curIAPointer;
}
FWInteractPointerIf* FWInteractScene::GetIAPointer(int i){ 
	if(i == -1) return curIAPointer;
	if(0 <= i && i < NIAPointers()) return interactPointers[i];
	return NULL;
}
FWInteractPointers*	FWInteractScene::GetIAPointers(){ return &interactPointers; }
int FWInteractScene::NIAPointers(){ return (int)interactPointers.size(); }

/// IASolidŠÖŒW
FWInteractSolid* FWInteractScene::GetIASolid(int i){
	if(0 <= i && i < NIASolids()) return &interactSolids[i];
	return NULL;
}
FWInteractSolids* FWInteractScene::GetIASolids(){ return &interactSolids; }
int FWInteractScene::NIASolids(){ return (int)interactSolids.size(); }

/// 
void FWInteractScene::Init(){
	GetIAAdaptee()->Init();
}
void FWInteractScene::Step(){
	GetIAAdaptee()->Step();
}
void FWInteractScene::Clear(){
	fwScene = NULL;
	interactAdaptee = NULL;
	curIAPointer = NULL;
	interactPointers.clear();
	interactSolids.clear();
}
void FWInteractScene::CallBackHapticLoop(){
	GetIAAdaptee()->CallBackHapticLoop();
}
void FWInteractScene::BeginKeyboard(){
	GetIAAdaptee()->BeginKeyboard();
}
void FWInteractScene::EndKeyboard(){
	GetIAAdaptee()->EndKeyboard();
}
Vec3d* FWInteractScene::GetProxyPoint(){
	return GetIAAdaptee()->GetProxyPoint();
}
Vec3d* FWInteractScene::GetForce(){
	return GetIAAdaptee()->GetForce();
}
bool FWInteractScene::GetContactFlag(){
	return GetIAAdaptee()->GetContactFlag();
}
Vec3d FWInteractScene::GetPicCenterPosition(){
	return GetIAAdaptee()->GetPicCenterPosition();
}
}
#include <Framework/FWInteractPointer.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWInteractPointer::FWInteractPointer(){ Init(); }
FWInteractPointer::FWInteractPointer(const FWInteractPointerDesc& desc) : FWInteractPointerDesc(desc){ Init(); }
void FWInteractPointer::Init(){
	bForce = false;
	bVibration = false;
}
void FWInteractPointer::SetPointerSolid(PHSolidIf* solid){
	pointerSolid = solid;
}
PHSolidIf* FWInteractPointer::GetPointerSolid(){
	return pointerSolid; 
}
void FWInteractPointer::SetHI(HIBaseIf* hi){
	humanInterface  = hi;
}
HIBaseIf* FWInteractPointer::GetHI(){
	return humanInterface; 
}
void FWInteractPointer::SetPosScale(double s){
	posScale = s; 
}
double FWInteractPointer::GetPosScale(){
	return posScale; 
}
void FWInteractPointer::SetForceScale(double s){
	forceScale = s; 
}
double FWInteractPointer::GetForceScale(){
	return forceScale; 
}
void FWInteractPointer::SetLocalRange(double r){
	localRange = r; 
}
double FWInteractPointer::GetLocalRange(){
	return localRange; 
}
void  FWInteractPointer::SetDefaultPosition(Posed p){
	defaultPosition = p;
}

Posed  FWInteractPointer::GetDefaultPosition(){
	return defaultPosition; 
}
void FWInteractPointer::EnableForce(bool b){
	bForce = b;
}
void FWInteractPointer::EnableVibration(bool b){
	bVibration = b;
}
void FWInteractPointer::Calibration(){
	GetHI()->Calibration();
}

}
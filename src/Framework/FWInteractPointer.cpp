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
void FWInteractPointer::SetLocalRange(double r){
	localRange = r; 
}
double FWInteractPointer::GetLocalRange(){
	return localRange; 
}
void  FWInteractPointer::SetPosition(Posed p){
	position = p;
}

Posed  FWInteractPointer::GetPosition(){
	return position; 
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
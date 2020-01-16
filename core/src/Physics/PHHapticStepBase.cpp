#include <Physics/PHHapticStepBase.h>
#include <Physics/PHHapticEngine.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticStepBase
double PHHapticStepBase::GetPhysicsTimeStep(){
	return engine->GetScene()->GetTimeStep();
}
double PHHapticStepBase::GetHapticTimeStep(){
	return engine->GetScene()->GetHapticTimeStep();
}
void PHHapticStepBase::SetHapticTimeStep(double dt) {
	return engine->GetScene()->SetHapticTimeStep(dt);
}
int PHHapticStepBase::NHapticPointers(){
	return (int)engine->hapticPointers.size();
}
int PHHapticStepBase::NHapticSolids(){
	return (int)engine->hapticSolids.size();
}
PHHapticPointer* PHHapticStepBase::GetHapticPointer(int i){
	return engine->hapticPointers[i];
}
PHSolidForHaptic* PHHapticStepBase::GetHapticSolid(int i){
	return engine->hapticSolids[i];
}
void PHHapticStepBase::StepPhysicsSimulation(){
	engine->GetScene()->Step();
}

bool PHHapticStepBase::SetCallbackBeforeStep(PHHapticEngineIf::Callback f, void* arg) {
	return true;
}

bool PHHapticStepBase::SetCallbackAfterStep(PHHapticEngineIf::Callback f, void* arg) {
	return true;
}


}

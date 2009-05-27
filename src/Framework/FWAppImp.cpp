#include "FWAppImp.h"
#include "FWAppAdapter.h"

FWAppImp::FWAppImp(){
	fwscene = NULL;
};

void FWAppImp::SetAdapter(FWAppAdapter* a){ adapter = a; }
FWSdkIf* FWAppImp::GetSdk(){ return adapter->GetSdk(); }
void FWAppImp::SetScene(FWSceneIf* s){ fwscene =s; }
FWSceneIf* FWAppImp::GetScene(){ return fwscene; } 
void FWAppImp::SetSpidar(HISpidarGIf* s){ spidarG6 = s; }
HISpidarGIf* FWAppImp::GetSpidar(){ return spidarG6; }
string FWAppImp::GetSceneName(){ return sceneName; }

void FWAppImp::Init(){
	SetScene(GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc()));		// Scene‚Ìì¬CÝ’è
	ResetScene();
}
void FWAppImp::ResetScene(){
	GetScene()->GetPHScene()->Clear();
	GetScene()->GetPHScene()->SetGravity(gravity);				
	GetScene()->GetPHScene()->SetTimeStep(pdt);
	GetScene()->GetPHScene()->SetNumIteration(nIter);
	BuildScene();
}
void FWAppImp::BuildScene(){}
void FWAppImp::Idle(){}
void FWAppImp::Step(){}
void FWAppImp::UpdateHapticPointer(){}
void FWAppImp::CallBack(){}
void FWAppImp::DebugMode(bool bDebug, GRDebugRenderIf* render){}
void FWAppImp::Keyboard(int key, int x, int y){}

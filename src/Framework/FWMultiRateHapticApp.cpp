#include <Framework/FWMultiRateHapticApp.h>
#include <sstream>

#ifdef USE_HDRSTOP
#pragma  hdrstop
#endif

namespace Spr{;

FWMultiRateHapticApp::FWMultiRateHapticApp(){
	bSync = false;
	bCalcPhys = true;
}

void FWMultiRateHapticApp::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWMultiRateHapticApp::Idle(){
	Step();
}

void FWMultiRateHapticApp::SyncHapticProcess(){
		FWHapticBase* hprocess = GetHapticProcess();
	if(bSync){
		// 力覚ポインタの同期
		UpdateHapticPointer(GetHapticPointer(), GetHapticInterface());

		// 力覚プロセス->物理プロセス
		// 力覚プロセスでの近傍物体のシミュレーション結果を物理プロセスに反映させる
		for(unsigned i = 0; i < hprocess->expandedPHSolids.size(); i++){
		// blocalがtrue，blocalがな場合は結果を反映させる
			if(expandedPHSolids[i].flag.blocal && !expandedPHSolids[i].flag.bfirstlocal){
				SpatialVector b = (expandedPHSolids[i].syncInfo.motionCoeff.b + 
					(expandedPHSolids[i].syncInfo.motionCoeff.curb - expandedPHSolids[i].syncInfo.motionCoeff.lastb)) *  GetPhysicTimeStep();
				Vec3d v = hprocess->expandedPHSolids[i].haSolid.GetVelocity() + b.v();
				hprocess->expandedPHSolids[i].phSolidIf->SetVelocity(v);
				Vec3d w = hprocess->expandedPHSolids[i].haSolid.GetAngularVelocity() + b.w();
				hprocess->expandedPHSolids[i].phSolidIf->SetAngularVelocity(w);
				hprocess->expandedPHSolids[i].phSolidIf->SetCenterPosition(hprocess->expandedPHSolids[i].haSolid.GetCenterPosition());
				hprocess->expandedPHSolids[i].phSolidIf->SetOrientation(hprocess->expandedPHSolids[i].haSolid.GetOrientation());
				expandedPHSolids[i].syncInfo.neighborPoint.test_force_norm = hprocess->expandedPHSolids[i].syncInfo.neighborPoint.test_force_norm;
			}
		}

		// 物理プロセス->力覚プロセス
		// シーンで新しく生成された剛体を格納
		for(int i = hprocess->expandedPHSolids.size(); i < expandedPHSolids.size(); i++){
			hprocess->expandedPHSolids.resize(hprocess->expandedPHSolids.size() + 1);
			hprocess->expandedPHSolids.back() = expandedPHSolids[i];
		}
		for(unsigned i = 0; i < hprocess->expandedPHSolids.size(); i++){
			// 初めて取得した近傍物体のみ行う
			if(expandedPHSolids[i].flag.bfirstlocal){
				hprocess->expandedPHSolids[i].haSolid = expandedPHSolids[i].haSolid;
				expandedPHSolids[i].flag.bfirstlocal = false;
			}
			hprocess->expandedPHSolids[i].flag.blocal = expandedPHSolids[i].flag.blocal;
			hprocess->expandedPHSolids[i].syncInfo = expandedPHSolids[i].syncInfo;
		}

		// 物理プロセスで使用する刻み時間
		//GetPHScene()->SetTimeStep(0.001f * (float)hcount);
		//dt = 0.001f * (float)hcount;
		hprocess->loopCounter = 0;

		// 同期終了のフラグ
		bSync = false;
	}
#define COUNT_MAX 300
	if(hprocess->loopCounter > COUNT_MAX) {
		DSTR << "too many step" << std::endl;
		hprocess->loopCounter = 0;
		bSync = false;
	}
	hprocess->loopCounter++;
	hapticcount++;
}

FWHapticBase* FWMultiRateHapticApp::GetHapticProcess(){
	return &hapticProcess;
}


}
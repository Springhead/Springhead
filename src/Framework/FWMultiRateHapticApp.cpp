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

//void FWMultiRateHapticApp::SyncHapticProcess(FWHapticBase* hprocess){
//	if(bSync){
//		// 力覚ポインタの同期
//		UpdateHapticPointer(GetHapticPointer(), GetHapticInterface());
//
//		// 力覚プロセス->物理プロセス
//		// 力覚プロセスでの近傍物体のシミュレーション結果を物理プロセスに反映させる
//		for(unsigned i = 0; i < hprocess.expandedObjects.size(); i++){
//		// blocalがtrue，blocalがな場合は結果を反映させる
//			if(expandedObjects[i].flag.blocal && !expandedObjects[i].flag.bfirstlocal){
//				SpatialVector b = (expandedObjects[i].syncInfo.motionCoeff.b + 
//					(expandedObjects[i].syncInfo.motionCoeff.curb - expandedObjects[i].syncInfo.motionCoeff.lastb)) *  pdt;
//				Vec3d v = hprocess.expandedObjects[i].phSolid.GetVelocity() + b.v();
//				hprocess.expandedObjects[i].phSolidIf->SetVelocity(v);
//				Vec3d w = hprocess.expandedObjects[i].phSolid.GetAngularVelocity() + b.w();
//				hprocess.expandedObjects[i].phSolidIf->SetAngularVelocity(w);
//				hprocess.expandedObjects[i].phSolidIf->SetCenterPosition(hprocess.expandedObjects[i].phSolid.GetCenterPosition());
//				hprocess.expandedObjects[i].phSolidIf->SetOrientation(hprocess.expandedObjects[i].phSolid.GetOrientation());
//				expandedObjects[i].syncInfo.neighborPoint.test_force_norm = hprocess.expandedObjects[i].syncInfo.neighborPoint.test_force_norm;
//			}
//		}
//
//		// 物理プロセス->力覚プロセス
//		// シーンで新しく生成された剛体を格納
//		for(size_t i = hprocess.expandedObjects.size(); i < expandedObjects.size(); i++){
//			hprocess.expandedObjects.resize(hprocess.expandedObjects.size() + 1);
//			hprocess.expandedObjects.back() = expandedObjects[i];
//		}
//		for(unsigned i = 0; i < hprocess.expandedObjects.size(); i++){
//			// 初めて取得した近傍物体のみ行う
//			if(expandedObjects[i].flag.bfirstlocal){
//				hprocess.expandedObjects[i].phSolid = expandedObjects[i].phSolid;
//				expandedObjects[i].flag.bfirstlocal = false;
//			}
//			hprocess.expandedObjects[i].flag.blocal = expandedObjects[i].flag.blocal;
//			hprocess.expandedObjects[i].syncInfo = expandedObjects[i].syncInfo;
//		}
//
//		// 物理プロセスで使用する刻み時間
//		//GetPHScene()->SetTimeStep(0.001f * (float)hcount);
//		//dt = 0.001f * (float)hcount;
//		hprocess.hcount = 0;
//
//		// 同期終了のフラグ
//		bsync = false;
//	}
//	if(hprocess.hcount > COUNT_MAX) {
//		DSTR << "too many step" << endl;
//		hprocess.hcount = 0;
//		bsync = false;
//	}
//	hprocess.hcount++;
//	hapticcount++;
//}



}
#include <Framework/FWMultiRateHapticApp.h>
#include <sstream>

#ifdef USE_HDRSTOP
#pragma  hdrstop
#endif

namespace Spr{;

FWMultiRateHapticApp::FWMultiRateHapticApp(){}

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

void FWMultiRateHapticApp::SyncHapticProcess(){}



}

#include <Springhead.h>		//	Springheadのインタフェース
#include <Physics/PHConstraintEngine.h>
#include <ctime>
#include <GL/glut.h>
#include <Springhead.h>
#include <fstream>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;
void main(){
	std::ifstream file;
/*
	file.open("ContFindCommonPointSaveParam.txt");
	PHSdkIf* sdk = PHSdkIf::CreateSdk();
	ContFindCommonPointCall(file, sdk);
*/
	file.open("DetectContinuouslySaveParam.txt");
	PHSdkIf* sdk = PHSdkIf::CreateSdk();
	CallDetectContinuously(file, sdk);
}

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
int main(){
	std::ifstream file;
#if 0
	file.open("ContFindCommonPointSaveParam.txt");
	PHSdkIf* sdk = PHSdkIf::CreateSdk();
	ContFindCommonPointCall(file, sdk);
#elif 1
	file.open("DetectContinuouslySaveParam.txt");
	PHSdkIf* sdk = PHSdkIf::CreateSdk();
	CallDetectContinuously(file, sdk);
#else
	extern bool bGJKDebug;
	bGJKDebug = true;
	CDSphereDesc sdA;
	sdA.radius = 1;
	CDSphereDesc sdB;
	sdB.radius = 2;
	UTRef<CDConvex> cA = new CDSphere(sdA);
	UTRef<CDConvex> cB = new CDSphere(sdB);
	Posed poseA, poseB;
	double rr = sdA.radius + sdB.radius;
/*
	poseA.pz = rr-0.1;
	poseA.px = 0.1;
/*/
	poseA.pz = sqrt(rr)-0.1;
	poseA.px = sqrt(rr);
/*/
	poseA.pz = sqrt(3.0)/2*rr-0.1;
	poseA.px = 0.5*rr;
*/
	Vec3d dir(0,0,1);
	Vec3d normal, pa, pb;
	double dist = 0;
	for(int i=0; i<10; ++i){
		int rt = ContFindCommonPoint(cA, cB, poseA, poseB, dir, -10, 10, normal, pa, pb, dist);
		DSTR << rt;
		DSTR << " normal:" << normal;
		DSTR << " dist:" << dist;
		DSTR << " pa pb:" << pa << pb << std::endl;
		DSTR << "---------------------------------------------------------------------" << std::endl;
		dir = -normal;
	}
#endif
	return 0;
}

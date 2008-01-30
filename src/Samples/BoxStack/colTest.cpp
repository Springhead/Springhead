
#include <Springhead.h>		//	Springheadのインタフェース
#include <Physics/PHConstraintEngine.h>
#include <ctime>
#include <GL/glut.h>
#include <Springhead.h>
#include <fstream>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
namespace Spr{
int FASTCALL ContFindCommonPoint(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, Vec3d& range, Vec3d& normal, 
	Vec3d& pa, Vec3d& pb, double& dist);
void ContFindCommonPointSaveParam(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, Vec3d& range, Vec3d& normal, 
	Vec3d& pa, Vec3d& pb, double& dist);
void ContFindCommonPointCall(std::istream& file, PHSdkIf* sdk);
}

using namespace Spr;
void main(){
	std::ifstream file;
	file.open("ContFindCommonPointSaveParam.txt");
	PHSdkIf* sdk = PHSdkIf::CreateSdk();
	ContFindCommonPointCall(file, sdk);
}
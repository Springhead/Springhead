#include "HIS.h"
#pragma hdrstop
//#include "HISpidarG6Jun.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarG6Jun, HISpidarG6);
HISpidarG6Jun::HISpidarG6Jun() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/

bool HISpidarG6Jun::Init(DVDeviceManager& dev){
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	(0.235f/2)		//	x方向の辺の長さ/2
	#define PY	(0.225f/2)		//	y方向の辺の長さ/2
	#define PZ	(0.235f/2)		//	z方向の辺の長さ/2
	//	糸のグリップへの取り付け位置
	#define GX	(0.0555f/2)		//	x方向の辺の長さ/2
	#define GY	(0.0555f/2)		//	y方向の辺の長さ/2
	Vec3f motorPos[8][2] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ), Vec3f(-GX,  0, 0)},
		{Vec3f( PX, PY,-PZ), Vec3f(  0, GY, 0)},
		{Vec3f(-PX,-PY,-PZ), Vec3f(  0,-GY, 0)},
		{Vec3f( PX,-PY, PZ), Vec3f( GX,  0, 0)},
		{Vec3f( PX, PY, PZ), Vec3f( GX,  0, 0)},
		{Vec3f(-PX, PY,-PZ), Vec3f(  0, GY, 0)},
		{Vec3f( PX,-PY,-PZ), Vec3f(  0,-GY, 0)},
		{Vec3f(-PX,-PY, PZ), Vec3f(-GX,  0, 0)}
	};
	if (!HISpidarG6::Init(dev, 8, motorPos, 0.3f, 2.924062107079e-5f, 1.0f, 20.0f)){
		return false;
	}
	for(int i=4; i<8; ++i){
		motor[i].lengthPerPulse *= -1;
	}
	Calib();
	return true;
}

}//	namespace Spr

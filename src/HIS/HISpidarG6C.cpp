#include "HIS.h"
#pragma hdrstop
//#include "HISpidarG6C.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarG6C, HISpidarG6);

HISpidarG6C::HISpidarG6C() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/

bool HISpidarG6C::Init(DVDeviceManager& dev){
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	const float PX = 0.20f/2;		//	x方向の辺の長さ/2
	const float PY = 0.20f/2;		//	y方向の辺の長さ/2
	const float PZ = -0.20f/2;		//	z方向の辺の長さ/2
	//	糸のグリップへの取り付け位置
	const float GX = 0.065f/2;		//	x方向の辺の長さ/2
	const float GY = 0.065f/2;		//	y方向の辺の長さ/2

	Vec3f motorPos[8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{Vec3f(-PX,-PY, PZ), Vec3f(0.0f,  -GY, 0.0f)},
		{Vec3f(-PX,-PY,-PZ), Vec3f( -GX, 0.0f, 0.0f)},
		{Vec3f( PX,-PY,-PZ), Vec3f(  GX, 0.0f, 0.0f)},
		{Vec3f( PX,-PY, PZ), Vec3f(0.0f,  -GY, 0.0f)},
		{Vec3f(-PX, PY, PZ), Vec3f(0.0f,   GY, 0.0f)},
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, 0.0f, 0.0f)},
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, 0.0f, 0.0f)},
		{Vec3f( PX, PY, PZ), Vec3f(0.0f,   GY, 0.0f)}
	};
/*
	//キャリブレーション位置を中心からずらす
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/
	if( HISpidarG6::Init(dev, 8, motorPos, 0.3f, (float)-2.03086921e-5, 1.0f, 20.0f) == false ){
		return false;
	}
//	for(int i=0; i<8; ++i){
//		motor[i].lengthPerPulse *= -1;
//	}
	Calib();
	return true;
}

}//	namespace Spr

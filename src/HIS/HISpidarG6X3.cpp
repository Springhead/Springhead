#include "HIS.h"
#pragma hdrstop
//#include "HISpidarG6X3.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarG6X3, HISpidarG6);
HISpidarG6X3::HISpidarG6X3() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/

bool HISpidarG6X3::Init(DVDeviceManager& dev, bool bLeft=false){
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	const float PX = 0.12f/2;		//	x方向の辺の長さ/2
	const float PY = 0.14f/2;		//	y方向の辺の長さ/2
	const float PZ = 0.12f/2;		//	z方向の辺の長さ/2
	//	糸のグリップへの取り付け位置．グリップはピンポン玉を採用しています．
	const float GX = 0.045f/2;		//	x方向の辺の長さ/2
	const float GY = 0.045f/2;		//	y方向の辺の長さ/2
	Matrix3f rotR = Matrix3f::Rot((float)Rad(-45), 'y');
	Matrix3f rotL = Matrix3f::Rot((float)Rad(-45), 'y');

	Vec3f motorPos[2][8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{
			{rotR*Vec3f(-PX,-PY, PZ), rotR*Vec3f( -GX, 0.0f, 0.0f)},
			{rotR*Vec3f( PX,-PY, PZ), rotR*Vec3f(  GX, 0.0f, 0.0f)},
			{rotR*Vec3f( PX,-PY,-PZ), rotR*Vec3f(0.0f,  -GY, 0.0f)},
			{rotR*Vec3f(-PX,-PY,-PZ), rotR*Vec3f(0.0f,  -GY, 0.0f)},
			{rotR*Vec3f(-PX, PY, PZ), rotR*Vec3f( -GX, 0.0f, 0.0f)},
			{rotR*Vec3f( PX, PY, PZ), rotR*Vec3f(  GX, 0.0f, 0.0f)},
			{rotR*Vec3f( PX, PY,-PZ), rotR*Vec3f(0.0f,   GY, 0.0f)},
			{rotR*Vec3f(-PX, PY,-PZ), rotR*Vec3f(0.0f,   GY, 0.0f)}
		},
		{
			{rotL*Vec3f( PX, PY,-PZ), rotL*Vec3f(0, 0.0f,  -GX)},
			{rotL*Vec3f( PX, PY, PZ), rotL*Vec3f(0, 0.0f,   GX)},
			{rotL*Vec3f(-PX, PY, PZ), rotL*Vec3f(0,   GY, 0.0f)},
			{rotL*Vec3f(-PX, PY,-PZ), rotL*Vec3f(0,   GY, 0.0f)},
			{rotL*Vec3f( PX,-PY,-PZ), rotL*Vec3f(0, 0.0f,  -GX)},
			{rotL*Vec3f( PX,-PY, PZ), rotL*Vec3f(0, 0.0f,   GX)},
			{rotL*Vec3f(-PX,-PY, PZ), rotL*Vec3f(0,  -GY, 0.0f)},
			{rotL*Vec3f(-PX,-PY,-PZ), rotL*Vec3f(0,  -GY, 0.0f)},
		}
	};
/*
	//キャリブレーション位置を中心からずらす
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/

	if( HISpidarG6::Init(dev, 8, motorPos[bLeft?1:0], 4.0f/35.1f, (float)1.66555e-5, 1.0f, 20.0f) == false ){
		return false;
	}
	if (bLeft){
		motor[2].lengthPerPulse *= -1;
		motor[3].lengthPerPulse *= -1;
		motor[4].lengthPerPulse *= -1;
		motor[5].lengthPerPulse *= -1;

	}else{
		motor[2].lengthPerPulse *= -1;
		motor[3].lengthPerPulse *= -1;
		motor[4].lengthPerPulse *= -1;
		motor[5].lengthPerPulse *= -1;
	}

	Calib();
	return true;
}

}//	namespace Spr

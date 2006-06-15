#include "HIS.h"
#pragma hdrstop
//#include "HISpidarG6WB.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarG6WB, HISpidarG6);

HISpidarG6WB::HISpidarG6WB() : HISpidarG6() {
}

bool HISpidarG6WB::Init(DVDeviceManager& dev){
	//	The coordinates where the string is connected on the grip.
	const float GY = 0.115f/2;		//	length of grip in y-direction / 2
	const float GZ = 0.115f/2;		//	length of grip in x-direction / 2
	//	The position of the motor where the string comes from
	//	and the position of the connection point on the grip.
#if 0
	Vec3f motorPos[8][2] = {
		{Vec3f(-0.945f, 1.2f, 1.1f)			/*Vec3f(-PX, PY, PZ)*/, Vec3f(  0.0f,   GY, 0.0f)},
		{Vec3f( 1.316f, 1.314f, -0.937f)	/*Vec3f( PX, PY,-PZ)*/, Vec3f(  0.0f, 0.0f,  -GZ)},
		{Vec3f(-1.302f,-0.096f, -0.935f)	/*Vec3f(-PX,-PY,-PZ)*/, Vec3f(  0.0f,  -GY, 0.0f)},
		{Vec3f( 0.93f, 0.045f, 1.15f)		/*Vec3f( PX,-PY, PZ)*/, Vec3f(  0.0f, 0.0f,   GZ)},

		{Vec3f( 0.945f, 1.22f, 1.1f)		/*Vec3f( PX, PY, PZ)*/, Vec3f(  0.0f, 0.0f,   GZ)},
		{Vec3f(-1.316f, 1.212f, -0.937f)	/*Vec3f(-PX, PY,-PZ)*/, Vec3f(  0.0f,   GY, 0.0f)},
		{Vec3f( 1.302f,-0.098f, -0.935f)	/*Vec3f( PX,-PY,-PZ)*/, Vec3f(  0.0f, 0.0f,  -GZ)},
		{Vec3f(-0.93f, 0.047f, 1.15f)		/*Vec3f(-PX,-PY, PZ)*/, Vec3f(  0.0f,  -GY, 0.0f)},
	};
#else
	Vec3f motorPos[8][2] = {
		{Vec3f( 0.945f, 1.2f,  1.1f)		/*Vec3f(-PX, PY, PZ)*/, Vec3f(  0.0f,  GY,   0.0f)},
		{Vec3f(-1.316f, 1.314f, -0.937f)	/*Vec3f( PX, PY,-PZ)*/, Vec3f(  0.0f,   0.0f, -GZ)},
		{Vec3f(1.302f, -0.096f, -0.935f)	/*Vec3f(-PX,-PY,-PZ)*/, Vec3f(  0.0f, -GY,   0.0f)},
		{Vec3f(-0.93f, 0.045f,  1.15f)		/*Vec3f( PX,-PY, PZ)*/, Vec3f(  0.0f,   0.0f,  GZ)},

		{Vec3f(-0.945f, 1.22f, 1.1f)		/*Vec3f( PX, PY, PZ)*/, Vec3f(  0.0f,   0.0f,  GZ)},
		{Vec3f(1.316f, 1.212f, -0.937f)		/*Vec3f(-PX, PY,-PZ)*/, Vec3f(  0.0f,  GY,   0.0f)},
		{Vec3f(-1.302f,-0.098f, -0.935f)	/*Vec3f( PX,-PY,-PZ)*/, Vec3f(  0.0f,   0.0f, -GZ)},
		{Vec3f(0.93f, 0.047f, 1.15f)		/*Vec3f(-PX,-PY, PZ)*/, Vec3f(  0.0f, -GY,   0.0f)},
	};
#endif
/*
	//	To make some offset for the calibration position.
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/
	if( HISpidarG6::Init(dev, 8, motorPos, 0.3f, 2.924062107079e-5f, 0.3f, 10.0f) == false ){
		return false;
	}
	for(int i=4; i<8; ++i){
		motor[i].lengthPerPulse *= -1;
	}
	Calib();
	return true;
}

}//	namespace Spr

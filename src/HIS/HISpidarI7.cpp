#include "HIS.h"
#ifdef _WIN32
 #pragma hdrstop
#endif
//#include "HISpidarI7.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarI7, HISpidarG6);

HISpidarI7::HISpidarI7() : HISpidarG6() {
}

bool HISpidarI7::Init(DVDeviceManager& dev){
	Vec3f motorPos[7][2] = {		//	モータの取り付け位置(中心を原点とするOpenGL座標系（右がX,上がY,奥がZ）)
		{ Vec3f(   0.00f,  25.00f, -4.00f )/1000.0f , Vec3f(   0.00f,  45.04f, -21.72f )/1000.0f } ,
		{ Vec3f(   3.46f,  25.00f,  2.00f )/1000.0f , Vec3f(  18.81f,  45.04f,  10.86f )/1000.0f } ,
		{ Vec3f(  -3.46f,  25.00f,  2.00f )/1000.0f , Vec3f( -18.81f,  45.04f,  10.86f )/1000.0f } ,
		{ Vec3f(  25.00f,   0.00f, -3.00f )/1000.0f , Vec3f(  39.00f, -22.52f, -18.81f )/1000.0f } ,
		{ Vec3f(  25.00f,   0.00f,  3.00f )/1000.0f , Vec3f(  39.00f, -22.52f,  18.81f )/1000.0f } ,
		{ Vec3f( -25.00f,   0.00f,  3.00f )/1000.0f , Vec3f( -39.00f, -22.52f,  18.81f )/1000.0f } ,
		{ Vec3f( -25.00f,   0.00f, -3.00f )/1000.0f , Vec3f( -39.00f, -22.52f, -18.81f )/1000.0f } ,
  };

	//HISpidarG6::Init(DVDeviceManager& dev, int nMotor, const Vec3f(* motorPos)[2], float vpn, float lpp, float minF, float maxF)
	if( HISpidarG6::Init(dev, 7, motorPos, 0.2279f, 0.008f*M_PI/500.0f/4.0f, 0.4f, 3.0f) == false ){
		return false;
	}
	Calib();
	return true;
}

}//	namespace Spr

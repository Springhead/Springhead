#include "HumanInterface.h"
#include "HIHaptic.h"

#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr {;

HIForceInterface6D::HIForceInterface6D(){
	alpha = 0.8f;
}
void HIForceInterface6D::Update(float dt){
	HIPose::Update(dt);
	Vec3f pos = GetPosition();
	Quaternionf ori = GetOrientation();
	Vec3f v = (pos - lastPos) / dt;
	Vec3f av = (ori * lastOri.Inv()).Rotation() / dt;
	vel = alpha*vel + (1-alpha)*v;
	angVel = alpha*angVel + (1-alpha)*av;
	lastPos = pos;
	lastOri = ori;
}

HIForceInterface3D::HIForceInterface3D(){
	alpha = 0.8f;
}
void HIForceInterface3D::Update(float dt){
	HIPosition::Update(dt);
	Vec3f pos = GetPosition();
	Vec3f v = (pos - lastPos) / dt;
	vel = alpha*vel + (1-alpha)*v;
	lastPos = pos;
}

}	//	namespace Spr

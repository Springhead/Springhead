#include "HumanInterface.h"
#include "HIHaptic.h"

#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr {;
HIForceInterface6D::HIForceInterface6D(){
}
void HIForceInterface6D::Update(float dt){
	HIPose::Update(dt);
	Vec3f pos = GetPos();
	Quaternionf ori = GetOri();
	Vec3f v = (pos - lastPos) / dt;
	Vec3f av = (ori * lastOri.Inv()).Rotation() / dt;
	vel = alpha*vel + (1-alpha)*v;
	angVel = alpha*angVel + (1-alpha)*av;
	lastPos = pos;
	lastOri = ori;
}

}	//	namespace Spr

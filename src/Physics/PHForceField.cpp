#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "PHForceField.h"

using namespace std;
namespace Spr{

OBJECT_IMP(PHGravityEngine, PHEngine);

void PHGravityEngine::Step(){
	PHSolids::iterator it;
	PHSolid* solid;
	for(it = solids.begin(); it != solids.end(); it++){
		solid = *it;
		solid->AddForce((float)solid->GetMass() * accel);
	}
}

bool PHGravityEngine::AddChildObject(Object* o){
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	return false;
}

}

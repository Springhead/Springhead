#include "Physics.h"
#pragma hdrstop
#include "PHForceField.h"

using namespace std;
namespace Spr{

OBJECTIMP(PHGravityEngine, PHEngine);

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

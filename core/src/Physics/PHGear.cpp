/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHGear.h>
#include <Physics/PHJoint.h>
#include <Physics/PHTreeNode.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHGear
PHGear::PHGear(const PHGearDesc& desc){
	joint[0] = joint[1] = NULL;
	SetDesc(&desc);

	bArticulated = false;
}

bool PHGear::AddChildObject(ObjectIf* o){
	PH1DJoint* j = DCAST(PH1DJoint, o);
	if(j){
		if(!joint[0]){
			joint[0] = j;
			return true;
		}
		if(!joint[1]){
			joint[1] = j;
			return true;
		}
	}
	return false;
}

bool PHGear::DelChildObject(ObjectIf* o){
	PH1DJoint* j = DCAST(PH1DJoint, o);
	if(j){
		if(j == joint[0]){
			joint[0] = NULL;
			return true;
		}
		if(j == joint[1]){
			joint[1] = NULL;
			return true;
		}
	}
	return false;
}

ObjectIf* PHGear::GetChildObject(size_t pos){
	return joint[pos]->Cast();
}

bool PHGear::IsFeasible(){
	return joint[0]->IsEnabled() && joint[0]->IsFeasible() && 
		   joint[1]->IsEnabled() && joint[1]->IsFeasible();
}

bool PHGear::IsArticulated(){
	return bArticulated;
}

bool PHGear::IsCyclic(){
	return joint[0]->IsCyclic() || joint[1]->IsCyclic();
}

void PHGear::SetupAxisIndex(){
	axes.Clear();
	axes.Enable(0);
	joint[0]->targetAxes.Enable(joint[0]->movableAxes[0]);
	joint[1]->targetAxes.Enable(joint[1]->movableAxes[0]);
}

void PHGear::Setup(){
	f *= engine->shrinkRate;
	
	// LCPのA行列の対角成分を計算
	A   [0] = ratio*ratio * joint[0]->A[joint[0]->movableAxes[0]] + joint[1]->A[joint[1]->movableAxes[0]];
	Ainv[0] = 1.0 / A[0];

	// bを計算
	b[0] = ratio * joint[0]->velocity[0] - joint[1]->velocity[0];
	if(mode == PHGearDesc::MODE_POS){
		double e = ratio * joint[0]->position[0] + offset - joint[1]->position[0];
		if(joint[0]->IsCyclic() || joint[1]->IsCyclic()){
			while(e > M_PI)
				e -= 2.0*M_PI;
			while(e < -M_PI)
				e += 2.0*M_PI;
		}
		db[0] = engine->velCorrectionRate * e;
	}
	else db[0] = 0.0;
}

bool PHGear::Iterate(){
	bool updated = false;
	int i0 = joint[0]->movableAxes[0];
	int i1 = joint[1]->movableAxes[0];
	
	joint[0]->dv[i0] = joint[0]->J[0].row(i0) * joint[0]->solid[0]->dv + joint[0]->J[1].row(i0) * joint[0]->solid[1]->dv;
	joint[1]->dv[i1] = joint[1]->J[0].row(i1) * joint[1]->solid[0]->dv + joint[1]->J[1].row(i1) * joint[1]->solid[1]->dv;

	dv  [0] = ratio * joint[0]->dv[i0] - joint[1]->dv[i1];
	res [0] = b[0] + db[0] + dv[0];
	fnew[0] = f[0] - engine->accelSOR * Ainv[0] * res[0];
	df  [0] = fnew[0] - f[0];
	f   [0] = fnew[0];

	if(df[0] > engine->dfEps){
		updated = true;
		CompResponse(df[0], 0);
	}
	return updated;
}

void PHGear::CompResponse(double df, int i){
	joint[0]->CompResponse(ratio * df, joint[0]->movableAxes[0]);
	joint[1]->CompResponse(-df       , joint[1]->movableAxes[0]);
}

}

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
	f = A = Ainv = b = 0.0;
	joint[0] = joint[1] = NULL;
	bArticulated = false;
	SetDesc(&desc);
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

void PHGear::CompResponse(double f){
	double fc;
	SpatialVector fs;
	PHSolid* s;
	for(int i = 0; i < 2; i++){
		fc = (i == 0 ? ratio * f : -f);
		for(int j = 0; j < 2; j++){
			s = joint[i]->solid[j];
			if(!s->IsDynamical())continue;
			if(s->treeNode){
				(Vec6d&)fs = joint[i]->J[j].row(joint[i]->movableAxes[0]) * fc;
				s->treeNode->CompResponse(fs, true, false);
			}
			else s->dv += joint[i]->T[j].row(joint[i]->movableAxes[0]) * fc;
		}
	}
}

void PHGear::SetupLCP(){
	if(bArticulated)return;
	f *= engine->shrinkRate;
	
	// LCPのA行列の対角成分を計算
	A = ratio * ratio * joint[0]->A[joint[0]->movableAxes[0]] + joint[1]->A[joint[1]->movableAxes[0]];
	Ainv = 1.0 / A;

	// 拘束力初期値による速度変化量を計算
	CompResponse(f);
}

void PHGear::IterateLCP(){
	if(bArticulated)return;
	double fnew;
	double b[2];
	for(int i = 0; i < 2; i++){
		int iaxis = joint[i]->movableAxes[0];
		b[i] = joint[i]->b[iaxis]
			+ joint[i]->J[0].row(iaxis) * joint[i]->solid[0]->dv
			+ joint[i]->J[1].row(iaxis) * joint[i]->solid[1]->dv;
	}
	fnew = f - Ainv * (ratio * b[0] - b[1]);
	CompResponse(fnew - f);
	f = fnew;
}

}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRTrajectoryController.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRCreature.h>
#include <Physics/SprPHScene.h>

namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRTrajectoryController::Step() {
	PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();

	if (crBody) {
		bool enable = false;

		for (int i=0; i<crBody->NSolids(); ++i) {
			if (CRIKSolidIf *cso = crBody->GetSolid(i)->Cast()) {
				if (cso->GetIKEndEffector()->IsEnabled()) {
					enable = true;
				}
			}
		}

		if (enable) {
			phScene->GetIKEngine()->Enable(true);
			/*
			phScene->GetIKEngine()->Enable(true);
			CompJointAngle();
			phScene->GetIKEngine()->Enable(false);
			CompOffsetForce();
			phScene->GetIKEngine()->Enable(false);
			*/
		}

	} else {
		crBody = DCAST(CRCreatureIf,GetScene())->GetBody(0);
	}
}

// --- --- --- --- --- --- ---

void CRTrajectoryController::Init() {
	states = ObjectStatesIf::Create();
	crBody = NULL;
}

void CRTrajectoryController::CompJointAngle() {
	PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();

	states->SaveState(phScene);

	jointTargetValues.clear();

	for (int i=0; i<crBody->NJoints(); ++i) {
		if (DCAST(CRIKJointIf,crBody->GetJoint(i))->GetIKActuator()->IsEnabled()) {
			PHBallJointIf* bj = crBody->GetJoint(i)->GetPHJoint()->Cast();
			if (bj) {
				JointTargetValue jtv;
				jtv.joint = bj->Cast();
				jtv.spring = bj->GetSpring();
				jtv.damper = bj->GetDamper();
				jointTargetValues.push_back(jtv);
			}

			PHHingeJointIf* hj = crBody->GetJoint(i)->GetPHJoint()->Cast();
			if (hj) {
				JointTargetValue jtv;
				jtv.joint = hj->Cast();
				jtv.spring = hj->GetSpring();
				jtv.damper = hj->GetDamper();
				jointTargetValues.push_back(jtv);
			}
		}
	}

	phScene->GetIKEngine()->Enable(true);

	for (int n=0; n<2; ++n) {
		phScene->Step();
	}

	phScene->GetIKEngine()->Enable(false);

	for (int i=0; i<crBody->NJoints(); ++i) {
		PHBallJointIf* bj = crBody->GetJoint(i)->GetPHJoint()->Cast();
		if (bj) {
			jointTargetValues[i].ballPosition = bj->GetPosition();
			jointTargetValues[i].ballVelocity = bj->GetVelocity();
		}

		PHHingeJointIf* hj = crBody->GetJoint(i)->GetPHJoint()->Cast();
		if (hj) {
			jointTargetValues[i].hingePosition = hj->GetPosition();
			jointTargetValues[i].hingeVelocity = hj->GetVelocity();
		}
	}

	states->LoadState(phScene);
}

void CRTrajectoryController::CompOffsetForce() {
	PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();

	for (unsigned i=0; i<jointTargetValues.size(); ++i) {
		PHBallJointIf* bj = jointTargetValues[i].joint->Cast();
		if (bj) {
			bj->SetSpring(0);
			bj->SetDamper(DBL_MAX);
			bj->SetTargetVelocity(jointTargetValues[i].ballVelocity);
		}

		PHHingeJointIf* hj = jointTargetValues[i].joint->Cast();
		if (hj) {
			hj->SetSpring(0);
			hj->SetDamper(DBL_MAX);
			hj->SetTargetVelocity(jointTargetValues[i].hingeVelocity);
		}
	}

	phScene->Step();

	for (unsigned i=0; i<jointTargetValues.size(); ++i) {
		PHBallJointIf* bj = jointTargetValues[i].joint->Cast();
		if (bj) {
			jointTargetValues[i].ballOffset = bj->GetOffsetForce();
		}

		PHHingeJointIf* hj = jointTargetValues[i].joint->Cast();
		if (hj) {
			jointTargetValues[i].hingeOffset = hj->GetOffsetForce();
		}
	}

	states->LoadState(phScene);

	for (unsigned i=0; i<jointTargetValues.size(); ++i) {
		PHBallJointIf* bj = jointTargetValues[i].joint->Cast();
		if (bj) {
			bj->SetTargetPosition(jointTargetValues[i].ballPosition);
			bj->SetTargetVelocity(jointTargetValues[i].ballVelocity);
			bj->SetOffsetForce(jointTargetValues[i].ballOffset);
			bj->SetSpring(jointTargetValues[i].spring);
			bj->SetDamper(jointTargetValues[i].damper);
			// std::cout << jointTargetValues[i].ballPosition << std::endl;
		}

		PHHingeJointIf* hj = jointTargetValues[i].joint->Cast();
		if (hj) {
			hj->SetTargetPosition(jointTargetValues[i].hingePosition);
			hj->SetTargetVelocity(jointTargetValues[i].hingeVelocity);
			hj->SetOffsetForce(jointTargetValues[i].hingeOffset);
			hj->SetSpring(jointTargetValues[i].spring);
			hj->SetDamper(jointTargetValues[i].damper);
			// std::cout << jointTargetValues[i].hingePosition << std::endl;
		}
	}
}

}
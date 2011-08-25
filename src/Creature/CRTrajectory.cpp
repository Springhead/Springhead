/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRTrajectory.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRBodyPart.h>
#include <Creature/SprCRCreature.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHIK.h>

namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRTrajectory::Start() {
	time = 0;
	bEnabled = true;
	posStart = solid->GetPHSolid()->GetPose()*posInSolid;
	velStart = solid->GetPHSolid()->GetVelocity() + PTM::cross(solid->GetPHSolid()->GetAngularVelocity(), posInSolid);
	CalcTrajectory();
	currentPosition = GetPosition();
	currentOrientation = GetOrientation();

	solid->GetIKEndEffector()->Enable(true);
}

void CRTrajectory::Step() {
	if (soCur==NULL) {
		/*
		PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();
		PHSolidDesc descSo;
		descSo.dynamical = false;
		soCur = phScene->CreateSolid(descSo);
		CDBoxDesc descBox;
		descBox.boxsize = Vec3f(0.2, 0.2, 0.2);
		soCur->AddShape(phScene->GetSdk()->CreateShape(descBox));
		phScene->SetContactMode(soCur, PHSceneDesc::MODE_NONE);
		*/
	}

	if (bEnabled) {
		if (posEnabled) {
			// restart if too far
			if ((GetPosition() - solid->GetPHSolid()->GetPose()*posInSolid).norm() > 2.0) {
				// Start();
			}

			// restart if jump
			if ((GetPosition() - currentPosition).norm() > 0.5) {
				Start();
			}
		}

		currentPosition = GetPosition();
		currentOrientation = GetOrientation();

		time += DCAST(CRCreatureIf,GetScene())->GetPHScene()->GetTimeStep();

		// std::cout << time << " : " << GetPosition() << std::endl;

		// std::cout << posInSolid << std::endl;
		solid->GetIKEndEffector()->SetTargetPosition(GetPosition(), posInSolid);
		if (soCur) {
			soCur->SetFramePosition(GetPosition());
		}
	}
}

bool CRTrajectory::AddChildObject(ObjectIf* o) {
	CRIKSolidIf* so = o->Cast();
	if (so) {
		solid = so;
		return true;
	}
	return false;
}

ObjectIf* CRTrajectory::GetChildObject(size_t pos) {
	if (pos == 0) {
		return solid->Cast();
	}
	return NULL;
}

size_t CRTrajectory::NChildObject() const {
	if (solid != NULL) {
		return 1;
	}
	return 0;
}

}
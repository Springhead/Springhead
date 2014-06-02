/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/FWSkeletonSensor.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

void FWSkeletonSensor::Update() {
	if (sensor) {
		FWSceneIf* fwScene = GetScene()->Cast();
		sensor->Update(fwScene->GetPHScene()->GetTimeStep());

		PrepareSkeleton(sensor->NSkeletons());
		for (int i=0; i<sensor->NSkeletons(); ++i) {
			HISkeletonIf* hiSkel = sensor->GetSkeleton(i);
			if (i<phSkeletons.size()) {
				PrepareBone(phSkeletons[i], hiSkel->NBones());
				for (int j=0; j<hiSkel->NBones(); ++j) {
					if (j<phSkeletons[i]->NBones()) {
						HIBoneIf* hiBone = hiSkel->GetBone(j);
						PHBoneIf* phBone = phSkeletons[i]->GetBone(j);
						phBone->SetPosition(  hiBone->GetPosition()  );
						phBone->SetDirection( hiBone->GetDirection() );
						phBone->SetLength(    hiBone->GetLength() )  ;
					}
				}
			}
		}
	}
}

void FWSkeletonSensor::PrepareSkeleton(int n) {
	PHSceneIf* phScene = DCAST(FWSceneIf,GetScene())->GetPHScene();
	for (int i=phSkeletons.size(); i<n; ++i) {
		PHSkeletonIf* phSkel = phScene->CreateSkeleton();
		phSkel->SetName("skelLeap");
		phSkeletons.push_back(phSkel);
	}
}

void FWSkeletonSensor::PrepareBone(PHSkeletonIf* phSkel, int n) {
	PHSceneIf* phScene = DCAST(FWSceneIf,GetScene())->GetPHScene();
	for (int i=phSkel->NBones(); i<n; ++i) {
		PHBoneIf* phBone = phSkel->CreateBone(NULL);
		if (bCreatePHSolid) {
			PHSolidIf* so = phScene->CreateSolid();
			so->SetName((std::string("so") + std::string(phSkel->GetName())).c_str());
			so->SetDynamical(false);
			phBone->SetSolid(so);
			if (bCreateCDShape) {
				CDRoundConeDesc descRC;
				descRC.length = 1e-2;
				descRC.radius = this->radius;
				so->AddShape( phScene->GetSdk()->CreateShape(descRC) );

				for (int j=0; j<phSkel->NBones(); ++j) {
					PHSolidIf* so2 = phSkel->GetBone(j)->GetSolid();
					if (so!=so2) {
						phScene->SetContactMode(so, so2, PHSceneDesc::MODE_NONE);
					}
				}
			}
		}
	}
}

}

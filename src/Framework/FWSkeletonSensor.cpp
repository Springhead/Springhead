/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/FWSkeletonSensor.h>
#include <Physics/PHSkeleton.h>
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
			if (i<(int)phSkeletons.size()) {
				PrepareBone(phSkeletons[i], hiSkel->NBones());
				for (int j=0; j<hiSkel->NBones(); ++j) {
					if (j<phSkeletons[i]->NBones()) {
						HIBoneIf* hiBone = hiSkel->GetBone(j);
						PHBoneIf* phBone = phSkeletons[i]->GetBone(j);
						phBone->SetDirection( hiBone->GetDirection() );
						phBone->SetPosition(  hiBone->GetPosition()  );
						phBone->SetLength(    hiBone->GetLength() )  ;
					}
				}
				((PHSkeleton*)phSkeletons[i])->grabStrength = hiSkel->GetGrabStrength();
			}
		}
	}
}

void FWSkeletonSensor::PrepareSkeleton(int n) {
	PHSceneIf* phScene = DCAST(FWSceneIf,GetScene())->GetPHScene();
	for (int i=(int)phSkeletons.size(); i<n; ++i) {
		PHSkeletonIf* phSkel = phScene->CreateSkeleton();
		phSkel->SetName("skelLeap");
		phSkeletons.push_back(phSkel);

		for (int j = 0; j<phSkel->NBones(); ++j) {
			PHSolidIf* so1 = (bCreatePHSpring ? phSkel->GetBone(j)->GetProxySolid() : phSkel->GetBone(j)->GetSolid());
			for (int k = 0; k < phSkel->NBones(); ++k) {
				if (j != k) {
					PHSolidIf* so2 = (bCreatePHSpring ? phSkel->GetBone(k)->GetProxySolid() : phSkel->GetBone(k)->GetSolid());
					phScene->SetContactMode(so1, so2, PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
}

void FWSkeletonSensor::PrepareBone(PHSkeletonIf* phSkel, int n) {
	PHSceneIf* phScene = DCAST(FWSceneIf,GetScene())->GetPHScene();
	for (int i=phSkel->NBones(); i<n; ++i) {
		PHBoneIf* phBone = phSkel->CreateBone(NULL);
		if (bCreatePHSolid) {
			PHSolidIf* so = phScene->CreateSolid();
			so->SetName((std::string("soInput") + std::string(phSkel->GetName())).c_str());
			so->SetDynamical(false);
			so->SetMass(0.1);
			so->SetInertia(Matrix3d::Unit() * 0.1);
			phBone->SetSolid(so);

			if (bCreatePHSpring) {
				PHSolidIf* soProxy = phScene->CreateSolid();
				soProxy->SetName((std::string("soProxy") + std::string(phSkel->GetName())).c_str());
				soProxy->SetDynamical(true);
				soProxy->SetMass(0.1);
				soProxy->SetInertia(Matrix3d::Unit() * 0.1);
				phBone->SetProxySolid(soProxy);

				PHSpringDesc descSpring;
				PHSpringIf* spring = phScene->CreateJoint(so, soProxy, descSpring)->Cast();
				spring->SetSpring(Vec3d(1, 1, 1) * 300);
				spring->SetDamper(Vec3d(1, 1, 1) * 3);
				spring->SetSpringOri(300);
				spring->SetDamperOri(3);
			}

			if (bCreateCDShape) {
				CDRoundConeDesc descRC;
				descRC.length = 1e-2f;
				descRC.radius = this->radius;
				CDShapeIf* shape = phScene->GetSdk()->CreateShape(descRC);
				shape->SetStaticFriction(1.0);  // <!!>
				shape->SetDynamicFriction(1.0); // <!!>

				PHSolidIf* shapedSolid;
				if (bCreatePHSpring) {
					shapedSolid = phBone->GetProxySolid();
				} else {
					shapedSolid = phBone->GetSolid();
				}
				shapedSolid->AddShape(shape);

				for (int k = 0; k<phScene->NSolids(); ++k) {
					PHSolidIf* so2 = phScene->GetSolids()[k];
					if ((shapedSolid != so2) && !(so2->IsDynamical())) {
						phScene->SetContactMode(shapedSolid, so2, PHSceneDesc::MODE_NONE);
					}
				}
			}
		}
	}
}

}

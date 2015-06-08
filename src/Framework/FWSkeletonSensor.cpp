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

		// Invisible Countの更新 (まず全て++しておいて、VisibleなものはProcessSkeleton内で0にする)
		for (int j = 0; j < skeletons.size(); ++j) {
			skeletons[j].invisibleCnt++;
		}

		// 入力の処理
		for (int i = 0; i < sensor->NSkeletons(); ++i) {
			if (!(sensor->GetSkeleton(i)->IsTracked())) { continue; } // トラッキングされていない場合はスキップ

			bool bProcessed = false;
			for (int j = 0; j < skeletons.size(); ++j) {
				if (skeletons[j].id == sensor->GetSkeleton(i)->GetTrackID()) {
					ProcessSkeleton(sensor->GetSkeleton(i), j);
					bProcessed = true;
					break;
				}
			}
			if (!bProcessed) {
				// 空きスケルトンを探す
				for (int j = 0; j < skeletons.size(); ++j) {
					if (skeletons[j].id < 0) {
						skeletons[j].id = sensor->GetSkeleton(i)->GetTrackID();
						ProcessSkeleton(sensor->GetSkeleton(i), j);
						UnfreezeSkeleton(j); // Unfreezeは位置の反映(Process)のあとで
						bProcessed = true;
						break;
					}
				}
				if (!bProcessed) {
					// 空きが無かったので追加する
					AddSkeleton();
					ProcessSkeleton(sensor->GetSkeleton(i), skeletons.size()-1);
				}
			}
		}

		// Invisible Countが一定値を超えたら消滅処理
		for (int j = 0; j < skeletons.size(); ++j) {
			if (skeletons[j].invisibleCnt > 1) { // 見えない 即 消失
				FreezeSkeleton(j);
			}
		}
	}
}

void FWSkeletonSensor::ProcessSkeleton(HISkeletonIf* hiSkel, int i) {
	PHSceneIf* phScene = DCAST(FWSceneIf, GetScene())->GetPHScene();

	PrepareBone(skeletons[i].phSkeleton, hiSkel->NBones());
	for (int j = 0; j < hiSkel->NBones(); ++j) {
		if (j < skeletons[i].phSkeleton->NBones()) {
			HIBoneIf* hiBone = hiSkel->GetBone(j);
			PHBoneIf* phBone = skeletons[i].phSkeleton->GetBone(j);
			phBone->SetLength(hiBone->GetLength());

			if (bCreatePHJoint && hiBone->GetParent()) {
				int n=0; bool bFound=false;
				for (; n < hiSkel->NBones(); ++n) { if (hiSkel->GetBone(n) == hiBone->GetParent()){ bFound = true; break; } }
				if (bFound) {
					HIBoneIf* hiParentBone = hiSkel->GetBone(n);
					PHBoneIf* phParentBone = skeletons[i].phSkeleton->GetBone(n);

					// まずphBoneとphParentBoneをつなぐ関節が有るかチェック、なければ作る
					PHBallJointIf* jo = phBone->GetJoint()->Cast();
					if (!jo) {
						PHBallJointDesc desc;
						desc.spring = 1e+3;
						desc.damper = 1e+1;
						desc.poseSocket.Pos().Z() = -hiParentBone->GetLength() / 2.0;
						desc.posePlug.Pos().Z()   =  hiBone->GetLength() / 2.0;
						jo = phScene->CreateJoint(phParentBone->GetSolid(), phBone->GetSolid(), desc)->Cast();
						phBone->SetJoint(jo);
					}

					// 次にhiBoneとhiParentBoneの姿勢差に基いて関節の目標角度をセット
					Quaterniond qP; qP.RotationArc(Vec3d(0, 0, 1), hiParentBone->GetDirection().unit());
					Quaterniond qC; qC.RotationArc(Vec3d(0, 0, 1), hiBone->GetDirection().unit());
					Quaterniond q = qP.Inv() * qC;
					Vec3d w = (q * (DCAST(PHBone, phBone)->lastJointPose.Inv())).RotationHalf() / phScene->GetTimeStep();
					jo->SetTargetPosition(q);
					jo->SetTargetVelocity(w);
					DCAST(PHBone, phBone)->lastJointPose = q;

					// 子剛体のDynamicalをONにする
					phBone->GetSolid()->SetDynamical(true);

					jo->Enable(true);
				}

				// <!!> おおむね指の太さの10倍（でいいのか？）程度の誤差が出たら強制同期
				if ((phBone->GetSolid()->GetPose().Pos() - hiBone->GetPosition()).norm() > (radius[0]*10)) {
					phBone->SetDirection(hiBone->GetDirection());
					phBone->SetPosition(hiBone->GetPosition());
					phBone->GetSolid()->SetVelocity(Vec3d());
					phBone->GetSolid()->SetAngularVelocity(Vec3d());
				}
			} else {
				if (PHJointIf* jo = phBone->GetJoint()) { jo->Enable(false); }
				phBone->SetDirection(hiBone->GetDirection());
				phBone->SetPosition(hiBone->GetPosition());
			}
		}
	}
	(DCAST(PHSkeleton, (skeletons[i].phSkeleton)))->grabStrength = hiSkel->GetGrabStrength();
	skeletons[i].invisibleCnt = 0;
}

void FWSkeletonSensor::AddSkeleton() {
	PHSceneIf* phScene = DCAST(FWSceneIf, GetScene())->GetPHScene();

	FWSkeletonInfo si;
	si.phSkeleton = phScene->CreateSkeleton();
	si.phSkeleton->SetName("skelLeap");
	skeletons.push_back(si);

	for (int j = 0; j<si.phSkeleton->NBones(); ++j) {
		PHSolidIf* so1 = (bCreatePHSpring ? si.phSkeleton->GetBone(j)->GetProxySolid() : si.phSkeleton->GetBone(j)->GetSolid());
		for (int k = 0; k < si.phSkeleton->NBones(); ++k) {
			if (j != k) {
				PHSolidIf* so2 = (bCreatePHSpring ? si.phSkeleton->GetBone(k)->GetProxySolid() : si.phSkeleton->GetBone(k)->GetSolid());
				phScene->SetContactMode(so1, so2, PHSceneDesc::MODE_NONE);
			}
		}
	}
}

void FWSkeletonSensor::FreezeSkeleton(int i) {
	PHSceneIf* phScene = DCAST(FWSceneIf, GetScene())->GetPHScene();

	skeletons[i].invisibleCnt = 0;
	skeletons[i].id = -1;
	PHSkeletonIf* skel = skeletons[i].phSkeleton;
	for (int j = 0; j < skel->NBones(); ++j) {
		PHSolidIf* so = (bCreatePHSpring ? skel->GetBone(j)->GetProxySolid() : skel->GetBone(j)->GetSolid());

		// 接触を切る
		phScene->SetContactMode(so, PHSceneDesc::MODE_NONE);

		// Dynamicalをオフにする
		so->SetDynamical(false);

		// 遠くへ飛ばす
		so->SetFramePosition(Vec3d(9999, 9999, 9999));
	}
}

void FWSkeletonSensor::UnfreezeSkeleton(int i) {
	PHSceneIf* phScene = DCAST(FWSceneIf, GetScene())->GetPHScene();

	skeletons[i].invisibleCnt = 0;
	PHSkeletonIf* skel = skeletons[i].phSkeleton;
	for (int j = 0; j < skel->NBones(); ++j) {
		PHSolidIf* so = (bCreatePHSpring ? skel->GetBone(j)->GetProxySolid() : skel->GetBone(j)->GetSolid());

		// 接触を入れる
		phScene->SetContactMode(so, PHSceneDesc::MODE_LCP);

		// Dynamicalを（必要なら）オンにする
		if (bCreatePHSpring) { so->SetDynamical(true); }
	}

	// 自己接触は切る
	for (int j = 0; j < skel->NBones(); ++j) {
		for (int k = 0; k < skel->NBones(); ++k) {
			if (j != k) {
				PHSolidIf* so1 = (bCreatePHSpring ? skel->GetBone(j)->GetProxySolid() : skel->GetBone(j)->GetSolid());
				PHSolidIf* so2 = (bCreatePHSpring ? skel->GetBone(k)->GetProxySolid() : skel->GetBone(k)->GetSolid());
				phScene->SetContactMode(so1, so2, PHSceneDesc::MODE_NONE);
			}
		}
	}

	// Non Dynamical との接触は切る
	for (int j = 0; j < phScene->NSolids(); ++j) {
		for (int k = 0; k < skel->NBones(); ++k) {
			PHSolidIf* so1 = phScene->GetSolids()[j];
			PHSolidIf* so2 = (bCreatePHSpring ? skel->GetBone(k)->GetProxySolid() : skel->GetBone(k)->GetSolid());
			if ((so1 != so2) && !(so1->IsDynamical()) && (so1->NShape()>0)) {
				phScene->SetContactMode(so1, so2, PHSceneDesc::MODE_NONE);
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

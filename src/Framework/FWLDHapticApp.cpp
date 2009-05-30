/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWLDHapticApp.h>

// FWLDHapticProcessの実装
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FWLDHapticProcess::Step(){}

void FWLDHapticProcess::LocalDynamics(){
	FWExpandedPHSolid** esolids = GetFWExpandedPHSolids();
	int Nesolids = GetNExpandedPHSolids();
	double hdt = GetHapticTimeStep();
	double pdt = GetPhysicTimeStep();

	for(int i = 0; i < Nesolids; i++){
		if(!esolids[i]->flag.blocal) continue;
		SpatialVector vel;																																					// 剛体の速度（ワールド座標系）
		vel.v() = esolids[i]->haSolid.GetVelocity();
		vel.w() = esolids[i]->haSolid.GetAngularVelocity();
		if(GetLoopCount() == 1) vel += (esolids[i]->syncInfo.motionCoeff.curb - esolids[i]->syncInfo.motionCoeff.lastb) *  pdt;	// 衝突の影響を反映
		vel += (esolids[i]->syncInfo.motionCoeff.A * esolids[i]->haSolid.nextForce + esolids[i]->syncInfo.motionCoeff.b) * hdt;	// 力覚ポインタからの力による速度変化
		esolids[i]->haSolid.SetVelocity(vel.v());																		
		esolids[i]->haSolid.SetAngularVelocity(vel.w());
		esolids[i]->haSolid.SetCenterPosition(esolids[i]->haSolid.GetCenterPosition() + vel.v() * hdt);
		esolids[i]->haSolid.SetOrientation(( Quaterniond::Rot(vel.w() * hdt) * esolids[i]->haSolid.GetOrientation()).unit());
 		esolids[i]->haSolid.SetUpdated(true);
		esolids[i]->haSolid.Step();
	}
}


// FWLDHapticAppの実装
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FWLDHapticApp::FWLDHapticApp(){
	hapticProcess = &hprocess;
}

void FWLDHapticApp::CallBack(){
	if(hprocess.GetLoopCount() > 300) return;
	hprocess.Step();
	SyncHapticProcess();
}

void FWLDHapticApp::ResetScene(){
	expandedPHSolids.clear();
	hprocess.expandedPHSolids.clear();
	bSync = false;
	bCalcPhys = true;
	hapticcount = 1;
	hprocess.ResetHapticProcess();
	states = ObjectStatesIf::Create();
	states2 = ObjectStatesIf::Create();
	PHSceneIf* phscene = GetScene()->GetPHScene();
	phscene->Clear();
	phscene->SetTimeStep(pdt);
	phscene->SetNumIteration(nIter);
	BuildScene();
}

void FWLDHapticApp::Step(){
	if (bSync) return;
	if (bCalcPhys){
		UpdateHapticPointer();
		int Nesolids = GetNExpandedPHSolids();
		FWExpandedPHSolid** esolids = GetFWExpandedPHSolids();
		std::vector<SpatialVector> lastvel;
		for(int i = 0; i < Nesolids; i++){
			if(!esolids[i]->flag.blocal) continue;
			lastvel.resize(i + 1);
			lastvel.back().v() = esolids[i]->phSolidIf->GetVelocity();
			lastvel.back().w() = esolids[i]->phSolidIf->GetAngularVelocity();
		}
		if(bStep) GetSdk()->GetScene()->GetPHScene()->Step();
		else if (bOneStep){
			GetSdk()->GetScene()->GetPHScene()->Step();
			bOneStep = false;
		}
		for(int i = 0; i < Nesolids; i++){
			if(!esolids[i]->flag.blocal) continue;
			SpatialVector curvel;
			curvel.v() = esolids[i]->phSolidIf->GetVelocity();
			curvel.w() = esolids[i]->phSolidIf->GetAngularVelocity();
			esolids[i]->syncInfo.motionCoeff.curb = (curvel - lastvel[i]) / pdt;
		}
		ExpandPHSolidInfo();
		FindNearestObjectFromHapticPointer(GetHapticPointer());	// 近傍物体の取得
		TestSimulation();
		bCalcPhys = false;
	}
	if (hapticcount < pdt/hdt) return;
	hapticcount -= (int)(pdt/hdt);
	bSync = true;
	bCalcPhys = true;
}


void FWLDHapticApp::TestSimulation(){
	// expandedPHSolidsのblocalがtrueの物体に対して単位力を加え，接触しているすべての物体について，運動係数を計算する
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* hpointer = GetHapticPointer();
	FWExpandedPHSolid** esolids = GetFWExpandedPHSolids(); 
	int Nesolids = GetNExpandedPHSolids();

#ifdef DIVIDE_STEP
	states2->SaveState(phscene);			// 予測シミュレーションのために現在の剛体の状態を保存する
	//	LCPの直前までシミュレーションしてその状態を保存
	phscene->ClearForce();
	phscene->GenerateForce();
	phscene->IntegratePart1();
#endif
	states->SaveState(phscene);			// 予測シミュレーションのために現在の剛体の状態を保存する

	for(int i = 0; i < Nesolids; i++){
		if(!esolids[i]->flag.blocal) continue;
		// 現在の速度を保存
		SpatialVector currentvel, nextvel; 
		currentvel.v() = esolids[i]->phSolidIf->GetVelocity();																	// 現在の速度
		currentvel.w() = esolids[i]->phSolidIf->GetAngularVelocity();														// 現在の角速度									
		Vec3d cPoint = esolids[i]->phSolidIf->GetPose() * esolids[i]->syncInfo.neighborPoint.closestPoint;	// 力を加える点
		const float minTestForce = 0.5;																									// 最小テスト力
		if(esolids[i]->syncInfo.neighborPoint.test_force_norm < minTestForce){
			esolids[i]->syncInfo.neighborPoint.test_force_norm = minTestForce;											// テスト力が0なら1にする 
		}

		// 拘束座標系を作るための準備
		Vec3d rpjabs, vpjabs;
		rpjabs = cPoint - hpointer->GetCenterPosition();																				//力覚ポインタの中心から接触点までのベクトル
		vpjabs = hpointer->GetVelocity() + hpointer->GetAngularVelocity() % rpjabs;										//接触点での速度
		Vec3d rjabs, vjabs;
		rjabs = cPoint - esolids[i]->phSolidIf->GetCenterPosition();																//剛体の中心から接触点までのベクトル
		vjabs = esolids[i]->phSolidIf->GetVelocity() + esolids[i]->phSolidIf->GetAngularVelocity() % rjabs;		//接触点での速度

		//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
		Vec3d n, t[2], vjrel, vjrelproj;
		n = -esolids[i]->syncInfo.neighborPoint.face_normal;
		vjrel = vjabs - vpjabs;															// 相対速度
		vjrelproj = vjrel - (n * vjrel) * n;											// 相対速度ベクトルを法線に直交する平面に射影したベクトル
		double vjrelproj_norm = vjrelproj.norm();
		if(vjrelproj_norm < 1.0e-10){												// 射影ベクトルのノルムが小さいとき
			t[0] = n % Vec3d(1.0, 0.0, 0.0);										// t[0]を法線とVec3d(1.0, 0.0, 0.0)の外積とする
			if(t[0].norm() < 1.0e-10)													// それでもノルムが小さかったら
				t[0] = n % Vec3d(0.0, 1.0, 0.0);									// t[0]を法線とVec3d(0.0, 1.0, 0.0)の外積とする
			t[0].unitize();																	// t[0]を単位ベクトルにする
		}
		else{
			t[0] = vjrelproj / vjrelproj_norm;										// ノルムが小さくなかったら，射影ベクトルのまま
		}
		t[1] = n % t[0];																	// t[1]は法線とt[0]の外積できまる

		// 何も力を加えないでシミュレーションを1ステップ進める
#ifdef DIVIDE_STEP
		phscene->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = esolids[i]->phSolidIf->GetVelocity();
		nextvel.w() = esolids[i]->phSolidIf->GetAngularVelocity();
		esolids[i]->syncInfo.motionCoeff.lastb = esolids[i]->syncInfo.motionCoeff.b;
		esolids[i]->syncInfo.motionCoeff.b = (nextvel - currentvel) / GetPhysicTimeStep();

		TMatrixRow<6, 3, double> u;
		TMatrixRow<3, 3, double> force;
		// 法線方向に力を加える
		states->LoadState(phscene
			);
		force.col(0) = esolids[i]->syncInfo.neighborPoint.test_force_norm * n;
		esolids[i]->phSolidIf->AddForce(force.col(0), cPoint);
#ifdef DIVIDE_STEP
		GetPHScene()->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = esolids[i]->phSolidIf->GetVelocity();
		nextvel.w() = esolids[i]->phSolidIf->GetAngularVelocity();
		u.col(0) = (nextvel - currentvel) /GetPhysicTimeStep() - esolids[i]->syncInfo.motionCoeff.b;

		// n + t[0]方向に力を加える
		states->LoadState(phscene);
		force.col(1) = esolids[i]->syncInfo.neighborPoint.test_force_norm * (n + t[0]);
		esolids[i]->phSolidIf->AddForce(force.col(1), cPoint);
		phscene->IntegratePart2();
		nextvel.v() = esolids[i]->phSolidIf->GetVelocity();
		nextvel.w() = esolids[i]->phSolidIf->GetAngularVelocity();
		u.col(1) = (nextvel - currentvel) /GetPhysicTimeStep() - esolids[i]->syncInfo.motionCoeff.b;

		// n+t[1]方向力を加える
		states->LoadState(phscene);
		force.col(2) = esolids[i]->syncInfo.neighborPoint.test_force_norm * (n + t[1]);
		esolids[i]->phSolidIf->AddForce(force.col(2), cPoint);
#ifdef DIVIDE_STEP
		GetPHScene()->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = esolids[i]->phSolidIf->GetVelocity();
		nextvel.w() = esolids[i]->phSolidIf->GetAngularVelocity();
		u.col(2) = (nextvel - currentvel) /GetPhysicTimeStep() - esolids[i]->syncInfo.motionCoeff.b;
		
		esolids[i]->syncInfo.motionCoeff.A = u  * force.inv();				// 運動係数Aの計算
		states->LoadState(phscene);													// 元のstateに戻しシミュレーションを進める
	}
#ifdef DIVIDE_STEP
		states2->LoadState(GetPHScene());										// 元のstateに戻しシミュレーションを進める
#endif
}
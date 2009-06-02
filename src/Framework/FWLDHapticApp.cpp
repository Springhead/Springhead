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

void FWLDHapticProcess::Step(){
	UpdateHumanInterface();
}

void FWLDHapticProcess::LocalDynamics(){
	FWExpandedPHSolids* esolids = GetFWExpandedPHSolids();
	int Nesolids = GetNExpandedPHSolids();
	double hdt = GetHapticTimeStep();
	double pdt = GetPhysicTimeStep();

	for(int i = 0; i < Nesolids; i++){
		if(!esolids->at(i).flag.blocal) continue;
		SpatialVector vel;																																					// 剛体の速度（ワールド座標系）
		vel.v() = esolids->at(i).haSolid.GetVelocity();
		vel.w() = esolids->at(i).haSolid.GetAngularVelocity();
		if(GetLoopCount() == 1) vel += (esolids->at(i).syncInfo.motionCoeff.curb - esolids->at(i).syncInfo.motionCoeff.lastb) *  pdt;	// 衝突の影響を反映
		vel += (esolids->at(i).syncInfo.motionCoeff.A * esolids->at(i).haSolid.nextForce + esolids->at(i).syncInfo.motionCoeff.b) * hdt;	// 力覚ポインタからの力による速度変化
		esolids->at(i).haSolid.SetVelocity(vel.v());																		
		esolids->at(i).haSolid.SetAngularVelocity(vel.w());
		esolids->at(i).haSolid.SetCenterPosition(esolids->at(i).haSolid.GetCenterPosition() + vel.v() * hdt);
		esolids->at(i).haSolid.SetOrientation(( Quaterniond::Rot(vel.w() * hdt) * esolids->at(i).haSolid.GetOrientation()).unit());
 		esolids->at(i).haSolid.SetUpdated(true);
		esolids->at(i).haSolid.Step();
	}
}


// FWLDHapticAppの実装
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FWLDHapticApp::FWLDHapticApp(){
	hapticProcess = &hprocess;
}

void FWLDHapticApp::CallBack(){
//	cout << GetHapticProcess()->GetLoopCount() << endl;
///	if(GetHapticProcess()->GetLoopCount() > 300) return;
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
	phscene->SetTimeStep(GetPhysicTimeStep());
	phscene->SetNumIteration(nIter);
	GetHapticProcess()->SetHapticTimeStep(GetHapticTimeStep());
	GetHapticProcess()->SetPhysicTimeStep(GetPhysicTimeStep()); 
	DSTR << GetHapticProcess()->GetHapticTimeStep() << endl; 
	BuildScene();
}

void FWLDHapticApp::Step(){
	if (bSync) return;
	if (bCalcPhys){
		UpdateHapticPointer();
		int Nesolids = GetNExpandedPHSolids();
		FWExpandedPHSolids* esolids = GetFWExpandedPHSolids();
		std::vector<SpatialVector> lastvel;
		for(int i = 0; i < Nesolids; i++){	
			if(!esolids->at(i).flag.blocal) continue;
			lastvel.resize(i + 1);
			lastvel.back().v() = esolids->at(i).phSolidIf->GetVelocity();
			lastvel.back().w() = esolids->at(i).phSolidIf->GetAngularVelocity();
		}
		if(bStep) GetScene()->GetPHScene()->Step();
		else if (bOneStep){
			GetScene()->GetPHScene()->Step();
			bOneStep = false;
		}
		for(int i = 0; i < Nesolids; i++){
			if(!esolids->at(i).flag.blocal) continue;
			SpatialVector curvel;
			curvel.v() = esolids->at(i).phSolidIf->GetVelocity();
			curvel.w() = esolids->at(i).phSolidIf->GetAngularVelocity();
			esolids->at(i).syncInfo.motionCoeff.curb = (curvel - lastvel[i]) / pdt;
		}
		ExpandPHSolidInfo();
		FindNearestObjectFromHapticPointer();	// 近傍物体の取得
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
	FWExpandedPHSolids* esolids = GetFWExpandedPHSolids(); 
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
		if(!esolids->at(i).flag.blocal) continue;
		// 現在の速度を保存
		SpatialVector currentvel, nextvel; 
		currentvel.v() = esolids->at(i).phSolidIf->GetVelocity();																	// 現在の速度
		currentvel.w() = esolids->at(i).phSolidIf->GetAngularVelocity();														// 現在の角速度									
		Vec3d cPoint = esolids->at(i).phSolidIf->GetPose() * esolids->at(i).syncInfo.neighborPoint.closestPoint;	// 力を加える点
		const float minTestForce = 0.5;																									// 最小テスト力
		if(esolids->at(i).syncInfo.neighborPoint.test_force_norm < minTestForce){
			esolids->at(i).syncInfo.neighborPoint.test_force_norm = minTestForce;											// テスト力が0なら1にする 
		}

		// 拘束座標系を作るための準備
		Vec3d rpjabs, vpjabs;
		rpjabs = cPoint - hpointer->GetCenterPosition();																				//力覚ポインタの中心から接触点までのベクトル
		vpjabs = hpointer->GetVelocity() + hpointer->GetAngularVelocity() % rpjabs;										//接触点での速度
		Vec3d rjabs, vjabs;
		rjabs = cPoint - esolids->at(i).phSolidIf->GetCenterPosition();																//剛体の中心から接触点までのベクトル
		vjabs = esolids->at(i).phSolidIf->GetVelocity() + esolids->at(i).phSolidIf->GetAngularVelocity() % rjabs;		//接触点での速度

		//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
		Vec3d n, t[2], vjrel, vjrelproj;
		n = -esolids->at(i).syncInfo.neighborPoint.face_normal;
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
		nextvel.v() = esolids->at(i).phSolidIf->GetVelocity();
		nextvel.w() = esolids->at(i).phSolidIf->GetAngularVelocity();
		esolids->at(i).syncInfo.motionCoeff.lastb = esolids->at(i).syncInfo.motionCoeff.b;
		esolids->at(i).syncInfo.motionCoeff.b = (nextvel - currentvel) / GetPhysicTimeStep();

		TMatrixRow<6, 3, double> u;
		TMatrixRow<3, 3, double> force;
		// 法線方向に力を加える
		states->LoadState(phscene
			);
		force.col(0) = esolids->at(i).syncInfo.neighborPoint.test_force_norm * n;
		esolids->at(i).phSolidIf->AddForce(force.col(0), cPoint);
#ifdef DIVIDE_STEP
		GetPHScene()->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = esolids->at(i).phSolidIf->GetVelocity();
		nextvel.w() = esolids->at(i).phSolidIf->GetAngularVelocity();
		u.col(0) = (nextvel - currentvel) /GetPhysicTimeStep() - esolids->at(i).syncInfo.motionCoeff.b;

		// n + t[0]方向に力を加える
		states->LoadState(phscene);
		force.col(1) = esolids->at(i).syncInfo.neighborPoint.test_force_norm * (n + t[0]);
		esolids->at(i).phSolidIf->AddForce(force.col(1), cPoint);
		phscene->IntegratePart2();
		nextvel.v() = esolids->at(i).phSolidIf->GetVelocity();
		nextvel.w() = esolids->at(i).phSolidIf->GetAngularVelocity();
		u.col(1) = (nextvel - currentvel) /GetPhysicTimeStep() - esolids->at(i).syncInfo.motionCoeff.b;

		// n+t[1]方向力を加える
		states->LoadState(phscene);
		force.col(2) = esolids->at(i).syncInfo.neighborPoint.test_force_norm * (n + t[1]);
		esolids->at(i).phSolidIf->AddForce(force.col(2), cPoint);
#ifdef DIVIDE_STEP
		GetPHScene()->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = esolids->at(i).phSolidIf->GetVelocity();
		nextvel.w() = esolids->at(i).phSolidIf->GetAngularVelocity();
		u.col(2) = (nextvel - currentvel) /GetPhysicTimeStep() - esolids->at(i).syncInfo.motionCoeff.b;
		
		esolids->at(i).syncInfo.motionCoeff.A = u  * force.inv();				// 運動係数Aの計算
		states->LoadState(phscene);													// 元のstateに戻しシミュレーションを進める
	}
#ifdef DIVIDE_STEP
		states2->LoadState(GetPHScene());										// 元のstateに戻しシミュレーションを進める
#endif
}
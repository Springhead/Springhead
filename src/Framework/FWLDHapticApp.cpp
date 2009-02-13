/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWLDHapticApp.h>

void FWLDHapticProcess::Step(){}

void FWLDHapticProcess::LocalDynamics(){
	FWExpandedPHSolid** esolids = GetFWExpandedPHSoilds();
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

FWExpandedPHSolid** FWLDHapticProcess::GetFWExpandedPHSoilds(){
	return expandedPHSolids.empty() ? NULL : (FWExpandedPHSolid**)&*expandedPHSolids.begin();
}

int FWLDHapticProcess::GetNExpandedPHSolids(){
	return expandedPHSolids.size();
}

void FWLDHapticApp::CallBack(){
	if(hprocess.GetLoopCount() > 300) return;
	hprocess.Step();
	SyncHapticProcess();
}
/*
void FWLDHapticApp::Step(){
	if (bsync) return;
	if (calcPhys){
		UpdateHapticPointer();
		vector<SpatialVector> lastvel;
		for(unsigned int i = 0; i < expandedObjects.size(); i++){
			if(!expandedObjects[i].flag.blocal) continue;
			lastvel.resize(i + 1);
			lastvel.back().v() = expandedObjects[i].phSolidIf->GetVelocity();
			lastvel.back().w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		}
		if(bStep) GetPHScene()->Step();
		else if (bOneStep){
			GetPHScene()->Step();
			bOneStep = false;
		}

		for(unsigned i = 0; i < expandedObjects.size(); i++){
			if(!expandedObjects[i].flag.blocal) continue;
			SpatialVector curvel;
			curvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
			curvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
			expandedObjects[i].syncInfo.motionCoeff.curb = (curvel - lastvel[i]) / pdt;
		}
		ExpandSolidInfo();
		FindNearestObject();	// 近傍物体の取得
		PredictSimulation();
		calcPhys = false;
	}
	if (hapticcount < pdt/hdt) return;
	hapticcount -= pdt/hdt;
	bsync = true;
	calcPhys = true;
}*/
/*
void TFWLDHapticApp::TestSimulation(){
	// neighborObjetsのblocalがtrueの物体に対して単位力を加え，接触しているすべての物体について，運動係数を計算する
#ifdef DIVIDE_STEP
	states2->SaveState(GetPHScene());			// 予測シミュレーションのために現在の剛体の状態を保存する
	//	LCPの直前までシミュレーションしてその状態を保存
	GetPHScene()->ClearForce();
	GetPHScene()->GenerateForce();
	GetPHScene()->IntegratePart1();
#endif
	states->SaveState(GetPHScene());			// 予測シミュレーションのために現在の剛体の状態を保存する

	for(unsigned i = 0; i < expandedObjects.size(); i++){
		if(!expandedObjects[i].flag.blocal) continue;
		
		// 現在の速度を保存
		SpatialVector currentvel, nextvel; 
		currentvel.v() = expandedObjects[i].phSolidIf->GetVelocity();											// 現在の速度
		currentvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();									// 現在の角速度									
		Vec3d cPoint = expandedObjects[i].phSolidIf->GetPose() * expandedObjects[i].syncInfo.neighborPoint.closestPoint;	// 力を加える点
		const float minTestForce = 0.5;
		if(expandedObjects[i].syncInfo.neighborPoint.test_force_norm < minTestForce){
			expandedObjects[i].syncInfo.neighborPoint.test_force_norm = minTestForce;		// テスト力が0なら1にする 
		}

		// 拘束座標系を作るための準備
		Vec3d rpjabs, vpjabs;
		rpjabs = cPoint - soPointer->GetCenterPosition();																							//力覚ポインタの中心から接触点までのベクトル
		vpjabs = soPointer->GetVelocity() + soPointer->GetAngularVelocity() % rpjabs;													//接触点での速度
		Vec3d rjabs, vjabs;
		rjabs = cPoint - expandedObjects[i].phSolidIf->GetCenterPosition();																	//剛体の中心から接触点までのベクトル
		vjabs = expandedObjects[i].phSolidIf->GetVelocity() + expandedObjects[i].phSolidIf->GetAngularVelocity() % rjabs;	//接触点での速度

		//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
		Vec3d n, t[2], vjrel, vjrelproj;
		n = -expandedObjects[i].syncInfo.neighborPoint.face_normal;
		vjrel = vjabs - vpjabs;										// 相対速度
		vjrelproj = vjrel - (n * vjrel) * n;						// 相対速度ベクトルを法線に直交する平面に射影したベクトル
		double vjrelproj_norm = vjrelproj.norm();
		if(vjrelproj_norm < 1.0e-10){							// 射影ベクトルのノルムが小さいとき
			t[0] = n % Vec3d(1.0, 0.0, 0.0);					// t[0]を法線とVec3d(1.0, 0.0, 0.0)の外積とする
			if(t[0].norm() < 1.0e-10)								// それでもノルムが小さかったら
				t[0] = n % Vec3d(0.0, 1.0, 0.0);				// t[0]を法線とVec3d(0.0, 1.0, 0.0)の外積とする
			t[0].unitize();												// t[0]を単位ベクトルにする
		}
		else{
			t[0] = vjrelproj / vjrelproj_norm;					// ノルムが小さくなかったら，射影ベクトルのまま
		}
		t[1] = n % t[0];												// t[1]は法線とt[0]の外積できまる

		// 何も力を加えないでシミュレーションを1ステップ進める
#ifdef DIVIDE_STEP
		GetPHScene()->IntegratePart2();
#else
		GetPHScene()->Step();
#endif
		nextvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		expandedObjects[i].syncInfo.motionCoeff.lastb = expandedObjects[i].syncInfo.motionCoeff.b;
		expandedObjects[i].syncInfo.motionCoeff.b = (nextvel - currentvel) / pdt;

		TMatrixRow<6, 3, double> u;
		TMatrixRow<3, 3, double> force;
		// 法線方向に力を加える
		states->LoadState(GetPHScene());
		force.col(0) = expandedObjects[i].syncInfo.neighborPoint.test_force_norm * n;
		expandedObjects[i].phSolidIf->AddForce(force.col(0), cPoint);
#ifdef DIVIDE_STEP
		GetPHScene()->IntegratePart2();
#else
		GetPHScene()->Step();
#endif
		nextvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		u.col(0) = (nextvel - currentvel) /pdt - expandedObjects[i].syncInfo.motionCoeff.b;

		// n + t[0]方向に力を加える
		states->LoadState(GetPHScene());
		force.col(1) = expandedObjects[i].syncInfo.neighborPoint.test_force_norm * (n + t[0]);
		expandedObjects[i].phSolidIf->AddForce(force.col(1), cPoint);
		GetPHScene()->IntegratePart2();
		nextvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		u.col(1) = (nextvel - currentvel) /pdt - expandedObjects[i].syncInfo.motionCoeff.b;

		// n+t[1]方向力を加える
		states->LoadState(GetPHScene());
		force.col(2) = expandedObjects[i].syncInfo.neighborPoint.test_force_norm * (n + t[1]);
		expandedObjects[i].phSolidIf->AddForce(force.col(2), cPoint);
#ifdef DIVIDE_STEP
		GetPHScene()->IntegratePart2();
#else
		GetPHScene()->Step();
#endif
		nextvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		u.col(2) = (nextvel - currentvel) /pdt - expandedObjects[i].syncInfo.motionCoeff.b;
		
		expandedObjects[i].syncInfo.motionCoeff.A = u  * force.inv();				// 運動係数Aの計算
		states->LoadState(GetPHScene());								// 元のstateに戻しシミュレーションを進める
	}
#ifdef DIVIDE_STEP
		states2->LoadState(GetPHScene());								// 元のstateに戻しシミュレーションを進める
#endif
}*/
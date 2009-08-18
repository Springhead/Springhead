/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWLDHaptic.h>
#include <Framework/SprFWApp.h>

// FWLDHapticLoopの実装
//////////////////////////////////////////////////////////////////////////////////////////////
FWLDHapticLoop::FWLDHapticLoop(){}
void FWLDHapticLoop::Step(){
	UpdateInterface();
	HapticRendering();
	LocalDynamics();
}

void FWLDHapticLoop::UpdateInterface(){
	int N = NInteractPointers();
	for(int i = 0; i < N; i++){
		FWInteractPointer* iPointer =GetInteractPointer(i)->Cast();
		HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
		double s = iPointer->GetPosScale();
		hif->Update((float)hdt);
		PHSolid* hiSolid = &iPointer->hiSolid;
		hiSolid->SetVelocity((Vec3d)hif->GetVelocity() * s);
		hiSolid->SetAngularVelocity((Vec3d)hif->GetAngularVelocity());
		hiSolid->SetFramePosition((Vec3d)hif->GetPosition() * s);
		hiSolid->SetOrientation(hif->GetOrientation());
	}
}


void FWLDHapticLoop::HapticRendering(){
	for(int j = 0; j < NInteractPointers(); j++){
		FWInteractPointer* iPointer = GetInteractPointer(j)->Cast();
		HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
		static double vibT = 0;
		static bool vibFlag = false;
		Vec3d vibV = (Vec3d)hif->GetVelocity() * iPointer->GetPosScale();
		static Vec3d vibVo = vibV;
		double vibforce = 0;
		static Vec3d proxy[100];
		bool noContact = true;

		SpatialVector outForce = SpatialVector();
		for(int i = 0; i < NInteractSolids(); i++){
			FWInteractSolid* iSolid = GetInteractSolid(i);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			if(!iInfo->flag.blocal) continue;
			NeighborInfo* nInfo = &iInfo->neighborInfo;
			PHSolid* cSolid = &iSolid->copiedSolid;
			Vec3d cPoint = cSolid->GetPose() * nInfo->closest_point;			// 剛体の近傍点のワールド座標系
			Vec3d pPoint = iPointer->hiSolid.GetPose() * nInfo->pointer_point;	// 力覚ポインタの近傍点のワールド座標系
			Vec3d force_dir = pPoint - cPoint;
			Vec3d interpolation_normal;											// 提示力計算にしようする法線（前回の法線との間を補間する）

			// 剛体の面の法線補間
			// 前回の法線と現在の法線の間を補間しながら更新
			double syncCount = pdt / hdt;						// プロセスの刻み時間の比
			interpolation_normal = (loopCount * nInfo->face_normal + 
				(syncCount - (double)loopCount) * nInfo->last_face_normal) / syncCount;															
			if(loopCount > syncCount)	interpolation_normal = nInfo->face_normal;

			double f = force_dir * interpolation_normal;		// 剛体の面の法線と内積をとる
			if(f < 0.0){										// 内積が負なら力を計算
				Vec3d ortho = f * interpolation_normal;			// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
				Vec3d dv = cSolid->GetPointVelocity(cPoint) - iPointer->hiSolid.GetPointVelocity(pPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;
				
				static Vec3d addforce = Vec3d(0,0,0);
				double K = iPointer->springK;
				double D = iPointer->damperD;
	//			if(!bproxy){
					addforce = -K * ortho + D * dvortho;// * ortho.norm();								// 提示力計算 (*ダンパの項にorthoのノルムをかけてみた)
	//			}else{
	//				addforce = -K * (pPoint - (proxy[i]+phSolid->GetCenterPosition())) + D * dvortho;	// 提示力計算(proxy)
	//			}
				//Vec3d addtorque = (pPoint - hpointer.GetCenterPosition()) % addforce ;

				if(!vibFlag){
					vibT = 0;
					vibVo = vibV - cSolid->GetVelocity() ;
				}
				vibFlag = true;
	//			if(vhaptic){
					double vibA = cSolid->GetShape(0)->GetVibA();
					double vibB = cSolid->GetShape(0)->GetVibB();
					double vibW = cSolid->GetShape(0)->GetVibW();
					vibforce = vibA * (vibVo * 0.003 * addforce.unit()) * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT);	//振動計算
	//			}			

				// proxy法での摩擦の計算
				Vec3d posVec = pPoint - (proxy[i] + cSolid->GetCenterPosition());
				double posDot = dot(nInfo->face_normal,posVec);
				Vec3d tVec = posDot * nInfo->face_normal;
				Vec3d tanjent = posVec - tVec;
				double mu0 = cSolid->GetShape(0)->GetStaticFriction();
				double mu1 = cSolid->GetShape(0)->GetDynamicFriction();
				if(tanjent.norm() > abs(mu0 * posDot)){
					proxy[i] += (tanjent.norm() - abs(mu1 * posDot)) * tanjent.unit();
	//				proxyPos += (tanjent.norm() - abs(1.0 * posDot)) * tanjent.unit();
				}
				outForce.v() += addforce + (vibforce * addforce.unit());	// ユーザへの提示力		
	//			outForce.w.() += addtorque;										 
				iPointer->interactInfo[i].mobility.force = -1 * addforce;						// 計算した力を剛体に加える
				nInfo->test_force_norm = addforce.norm();
				noContact = false;
			}
		}
		if (noContact) vibFlag = false;
		vibT += hdt;
		#ifdef TORQUE
		if(bDisplayforce) fInterface->SetForce(displayforce, displaytorque);
		#else
	//	if(bDisplayforce) 
//			hif->SetForce(Vec3f(), Vec3f());			// 発振が怖いのでとりあえず出力なしで，あとでフラグをつくります
			hif->SetForce(outForce.v(), Vec3d());					
		#endif
	}
}

void FWLDHapticLoop::LocalDynamics(){
	for(int i = 0; i < NInteractSolids(); i++){
		FWInteractSolid* iSolid = FWHapticLoopBase::GetInteractSolid(i);
		if(!iSolid->bSim) continue;
		SpatialVector vel;
		vel.v() = iSolid->copiedSolid.GetVelocity();
		vel.w() = iSolid->copiedSolid.GetAngularVelocity();
		if(GetLoopCount() == 1) vel += (iSolid->curb - iSolid->lastb) *  pdt;	// 衝突の影響を反映
		for(int j = 0; j < NInteractPointers(); j++){
			FWInteractPointer* iPointer = GetInteractPointer(j);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			vel += (iInfo->mobility.A * iInfo->mobility.force) * hdt;			// 力覚ポインタからの力による速度変化

			iInfo->mobility.force = Vec3d();
		}
		vel += iSolid->b * hdt;
		iSolid->copiedSolid.SetVelocity(vel.v());																		
		iSolid->copiedSolid.SetAngularVelocity(vel.w());
		iSolid->copiedSolid.SetCenterPosition(iSolid->copiedSolid.GetCenterPosition() + vel.v() * hdt);
		iSolid->copiedSolid.SetOrientation(( Quaterniond::Rot(vel.w() * hdt) * iSolid->copiedSolid.GetOrientation()).unit());
 		iSolid->copiedSolid.SetUpdated(true);
		iSolid->copiedSolid.Step();
	}
}

// FWLDHapticAppの実装
//////////////////////////////////////////////////////////////////////////////////////////////
FWLDHaptic::FWLDHaptic(){}
void FWLDHaptic::Init(){
	FWMultiRateHaptic::Init();
	hapticLoop = &LDHapticLoop;
	double pdt = GetInteractScene()->GetScene()->GetPHScene()->GetTimeStep();
	double hdt = GetInteractScene()->hdt;
	hapticLoop->Init(pdt, hdt);
	states = ObjectStatesIf::Create();
	states2 = ObjectStatesIf::Create();
}
void FWLDHaptic::Clear(){
	FWMultiRateHaptic::Clear();
	states->Clear();
	states2->Clear();
}
void FWLDHaptic::CallBackHapticLoop(){
	GetHapticLoop()->Step();
	Sync();
}
void FWLDHaptic::Step(){
	if (bSync) return;
	if (bCalcPhys){
		UpdatePointer();
		PhysicsStep();
		UpdateSolidList();
		NeighborObjectFromPointer();
		TestSimulation();
		bCalcPhys = false;
	}
	double pdt = GetInteractScene()->GetScene()->GetPHScene()->GetTimeStep();
	double hdt = GetInteractScene()->hdt;
	if (hapticcount < pdt / hdt) return;
	hapticcount -= (int)(pdt/hdt);
	bSync = true;
	bCalcPhys = true;
}
void FWLDHaptic::PhysicsStep(){
	FWInteractSolids* iSolids = GetInteractSolids();
	std::vector<SpatialVector> lastvel;
	for(int i = 0; i < (int)iSolids->size(); i++){
		FWInteractSolid* iSolid = &iSolids->at(i);
		if(!iSolid->bSim) continue;
		lastvel.resize(i + 1);
		lastvel.back().v() = iSolid->sceneSolid->GetVelocity();
		lastvel.back().w() = iSolid->sceneSolid->GetAngularVelocity();
	}
//	if(bStep) GetFWApp()->GeSdk()->GetScene()->GetPHScene()->Step();
//	else if (bOneStep){
		GetInteractScene()->GetScene()->GetPHScene()->Step();
//		bOneStep = false;
//	}
	for(int i = 0; i < (int)iSolids->size(); i++){
		FWInteractSolid* iSolid = &iSolids->at(i);
		if(!iSolid->bSim) continue;
		SpatialVector curvel;
		curvel.v() = iSolid->sceneSolid->GetVelocity();
		curvel.w() = iSolid->sceneSolid->GetAngularVelocity();
		double pdt = GetInteractScene()->GetScene()->GetPHScene()->GetTimeStep();
		iSolid->curb = (curvel - lastvel[i]) / pdt;
	}
}
void FWLDHaptic::UpdatePointer(){
	int N = GetInteractScene()->NInteractPointers();
	for(int i = 0; i < N; i++){	
		if(GetHapticLoop()->NInteractPointers() == 0) return; 
		FWInteractPointer* piPointer = GetInteractScene()->GetInteractPointer(i)->Cast();
		FWInteractPointer* hiPointer = GetHapticLoop()->GetInteractPointer(i)->Cast();
		PHSolidIf* soPointer = piPointer->pointerSolid;
		PHSolid* hiSolid = &hiPointer->hiSolid;
		soPointer->SetVelocity(hiSolid->GetVelocity());
		soPointer->SetAngularVelocity(hiSolid->GetAngularVelocity());
		soPointer->SetFramePosition(hiSolid->GetFramePosition());
		soPointer->SetOrientation(hiSolid->GetOrientation());
		soPointer->SetDynamical(false);
	}
}
void FWLDHaptic::TestSimulation(){
	/** FWInteractSolidsのblocalがtrueの物体に対してテスト力を加え，
		接触しているすべての物体について，モビリティを計算する */
	PHSceneIf* phScene = GetInteractScene()->GetScene()->GetPHScene();

	#ifdef DIVIDE_STEP
	/// テストシミュレーションのために現在の剛体の状態を保存する
	states2->SaveState(phScene);		
	///	LCPの直前までシミュレーションしてその状態を保存
	phScene->ClearForce();
	phScene->GenerateForce();
	phScene->IntegratePart1();
	#endif
	/// テストシミュレーションのために現在の剛体の状態を保存する
	states->SaveState(phScene);			

	/// テストシミュレーション実行
	FWInteractSolids* iSolids = GetInteractSolids();
	for(int i = 0; i < (int)iSolids->size(); i++){
		FWInteractSolid* iSolid = GetInteractSolid(i);
		PHSolid* phSolid = iSolid->sceneSolid;
		if(!iSolid->bSim) continue;

		/// 現在の速度を保存
		SpatialVector curvel, nextvel; 
		curvel.v() = phSolid->GetVelocity();			// 現在の速度
		curvel.w() = phSolid->GetAngularVelocity();		// 現在の角速度									

		/// 何も力を加えないでシミュレーションを1ステップ進める
		#ifdef DIVIDE_STEP
		phScene->IntegratePart2();
		#else
		phScene->Step();
		#endif
		nextvel.v() = phSolid->GetVelocity();
		nextvel.w() = phSolid->GetAngularVelocity();
		/// モビリティbの算出
		iSolid->lastb = iSolid->b;
		double pdt = phScene->GetTimeStep();
		iSolid->b = (nextvel - curvel) / pdt;
		states->LoadState(phScene);						// 現在の状態に戻す

		/// InteractPointerの数だけ力を加えるテストシミュレーションを行う
		int N = GetInteractScene()->NInteractPointers();
		for(int j = 0; j < N; j++){
			FWInteractPointer* iPointer = GetInteractScene()->GetInteractPointer(j)->Cast();
			PHSolidIf* soPointer = iPointer->pointerSolid;
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			Vec3d cPoint = phSolid->GetPose() * iInfo->neighborInfo.closest_point;		// 力を加える点(ワールド座標)

			/// 拘束座標系を作るための準備
			Vec3d rpjabs, vpjabs;
			rpjabs = cPoint - soPointer->GetCenterPosition();									//力覚ポインタの中心から接触点までのベクトル
			vpjabs = soPointer->GetVelocity() + soPointer->GetAngularVelocity() % rpjabs;		//接触点での速度
			Vec3d rjabs, vjabs;
			rjabs = cPoint - phSolid->GetCenterPosition();										//剛体の中心から接触点までのベクトル
			vjabs = phSolid->GetVelocity() + phSolid->GetAngularVelocity() % rjabs;				//接触点での速度

			/// 拘束座標変換行列の計算
			Vec3d n, t[2], vjrel, vjrelproj;						//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
			n = -1* iInfo->neighborInfo.face_normal;
			vjrel = vjabs - vpjabs;									// 相対速度
			vjrelproj = vjrel - (n * vjrel) * n;					// 相対速度ベクトルを法線に直交する平面に射影したベクトル
			double vjrelproj_norm = vjrelproj.norm();
			if(vjrelproj_norm < 1.0e-10){							// 射影ベクトルのノルムが小さいとき
				t[0] = n % Vec3d(1.0, 0.0, 0.0);					// t[0]を法線とVec3d(1.0, 0.0, 0.0)の外積とする
				if(t[0].norm() < 1.0e-10)							// それでもノルムが小さかったら
					t[0] = n % Vec3d(0.0, 1.0, 0.0);				// t[0]を法線とVec3d(0.0, 1.0, 0.0)の外積とする
				t[0].unitize();										// t[0]を単位ベクトルにする
			}
			else{
				t[0] = vjrelproj / vjrelproj_norm;					// ノルムが小さくなかったら，射影ベクトルのまま
			}
			t[1] = n % t[0];										// t[1]は法線とt[0]の外積できまる

			/// 接触点に加える力
			const float minTestForce = 0.5;										// 最小テスト力
			/// 通常テスト力が最小テスト力を下回る場合
			if(iInfo->neighborInfo.test_force_norm < minTestForce){
				iInfo->neighborInfo.test_force_norm = minTestForce;					 
			}

			TMatrixRow<6, 3, double> u;			// 剛体の機械インピーダンス
			TMatrixRow<3, 3, double> force;		// 加える力

			/// 法線方向に力を加える
			force.col(0) = iInfo->neighborInfo.test_force_norm * n;
			phSolid->AddForce(force.col(0), cPoint);
			#ifdef DIVIDE_STEP
			phScene->IntegratePart2();
			#else
			phScene->Step();
			#endif
			nextvel.v() = phSolid->GetVelocity();
			nextvel.w() = phSolid->GetAngularVelocity();
			u.col(0) = (nextvel - curvel) /pdt - iSolid->b;
			states->LoadState(phScene);

			/// n + t[0]方向に力を加える
			force.col(1) = iInfo->neighborInfo.test_force_norm * (n + t[0]);
			phSolid->AddForce(force.col(1), cPoint);
			#ifdef DIVIDE_STEP
			phScene->IntegratePart2();
			#else
			phScene->Step();
			#endif
			nextvel.v() = phSolid->GetVelocity();
			nextvel.w() = phSolid->GetAngularVelocity();
			u.col(1) = (nextvel - curvel) /pdt - iSolid->b;
			states->LoadState(phScene);

			/// n+t[1]方向力を加える
			force.col(2) = iInfo->neighborInfo.test_force_norm * (n + t[1]);
			phSolid->AddForce(force.col(2), cPoint);
			#ifdef DIVIDE_STEP
			phScene->IntegratePart2();
			#else
			phScene->Step();
			#endif
			nextvel.v() = phSolid->GetVelocity();
			nextvel.w() = phSolid->GetAngularVelocity();
			u.col(2) = (nextvel - curvel) /pdt - iSolid->b;
			
			iInfo->mobility.A = u  * force.inv();			// モビリティAの計算
			states->LoadState(phScene);								// 元のstateに戻しシミュレーションを進める
		}
	}
	///--------テストシミュレーション終了--------
	#ifdef DIVIDE_STEP
		states2->LoadState(phScene);							// 元のstateに戻しシミュレーションを進める
	#endif
}
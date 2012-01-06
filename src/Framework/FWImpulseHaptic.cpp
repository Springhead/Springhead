/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/FWImpulseHaptic.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

// FWImpulseHapticの実装
///////////////////////////////////////////////
FWImpulseHapticLoop::FWImpulseHapticLoop(){}
void FWImpulseHapticLoop::Step(){
	UpdateInterface();
	switch(hmode){
		case PENALTY3D:
			HapticRendering();
			break;
		case CONSTRAINT:
			ConstraintBasedRendering();
			break;
		default:
			HapticRendering();
			break;
	}
}

Vec3d FWImpulseHapticLoop::Vibration(FWInteractSolid* iSolid, FWInteractPointer* iPointer, int n){
	Vec3d vibforce = Vec3d(0,0,0);

	if(!iPointer->bContact[n]){	// この近傍点と初接触かどうか
		vibV = iPointer->hiSolid.GetVelocity() - iSolid->copiedSolid.GetVelocity();
		iSolid->sceneSolid->GetShape(0)->SetVibT(0);
	}
	vibT = iSolid->sceneSolid->GetShape(0)->GetVibT();		// material.vibTへのポインタ
	iPointer->bContact[n] = true;

	double vibA = iSolid->copiedSolid.GetShape(0)->GetVibA();
	double vibB = iSolid->copiedSolid.GetShape(0)->GetVibB();
	double vibW = iSolid->copiedSolid.GetShape(0)->GetVibW();

	vibforce = vibA * (vibV*0.03) * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT);		//振動計算
	iSolid->sceneSolid->GetShape(0)->SetVibT((float)(vibT+hdt));		// 接触時間の更新
	return vibforce;
}

void FWImpulseHapticLoop::HapticRendering(){
	for(int j = 0; j < NIAPointers(); j++){
		FWInteractPointer* iPointer = GetIAPointer(j)->Cast();

		if((int)iPointer->bContact.size() < NIASolids()){
			for(int i=(int)iPointer->bContact.size(); i<NIASolids(); i++){
				iPointer->bContact.push_back(false);
				// contactFlag[0].push_back(false);
				// contactFlag[1].push_back(false);
				// oVibForce.push_back(Vec3d(0,0,0));
			}
		}
		HIHapticIf* hif = iPointer->GetHI()->Cast();
		
		SpatialVector outForce = SpatialVector();

		for(int i = 0; i < NIASolids(); i++){
			FWInteractSolid* iSolid = GetIASolid(i);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			if(!iInfo->flag.blocal) continue;
			ToHaptic* th = &iInfo->toHaptic;
			ToPhysic* tp = &iInfo->toPhysic;
			PHSolid* cSolid = &iSolid->copiedSolid;
			Vec3d cPoint = cSolid->GetPose() * th->closest_point;			// 剛体の近傍点のワールド座標系
			Vec3d pPoint = iPointer->hiSolid.GetPose() * th->pointer_point;	// 力覚ポインタの近傍点のワールド座標系
			Vec3d force_dir = pPoint - cPoint;

			// 剛体の面の法線補間
			// 前回の法線と現在の法線の間を補間しながら更新
			double syncCount = pdt / hdt;						// プロセスの刻み時間の比
			Vec3d interpolation_normal = (loopCount * th->face_normal + 
				(syncCount - (double)loopCount) * th->last_face_normal) / syncCount;															

			// 接触点の補間
			Vec3d interpolation_cPoint = (loopCount * cPoint + 
				(syncCount - (double)loopCount) * th->last_closest_point) / syncCount;

			force_dir = pPoint - interpolation_cPoint;

			// 同期カウントを越えたら現在の法線，接触点を使う
			if(loopCount > syncCount){
				interpolation_normal = th->face_normal;
				force_dir = pPoint - cPoint;
			}

			double f = force_dir * interpolation_normal;		// 剛体の面の法線と内積をとる
			if(f < 0.0){										// 内積が負なら力を計算
				Vec3d pVibForce = Vec3d(0,0,0);

				Vec3d ortho = f * interpolation_normal;			// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
				Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;

				/// 抗力の計算
				double K = iPointer->correctionSpringK;
				double D = iPointer->correctionDamperD;
				Vec3d addforce = -1 * (K * ortho + D * dvortho);
		
				double ws4 = pow(iPointer->GetWorldScale(), 4);
				outForce.v() += addforce / ws4;	
				outForce.w() = Vec3d();

				///振動の計算
				if(iPointer->bVibration){
					pVibForce = Vibration(iSolid, iPointer, i);
					// if(bPic){ pVibForce += oVibForce[i]; }
				}

				outForce.v() += pVibForce;	

				/// 計算した力を剛体に加える//
				tp->impulse = -1 * addforce;	
			}else{
				iPointer->bContact[i] = false;
				// contactFlag[j][i] = false;
				// proxy[j][i] = poseSolid.Inv() * pPoint;
			}
		}
		/// インタフェースへ力を出力
		SetRenderedForce(iPointer->GetHI(), iPointer->bForce, outForce * iPointer->GetForceScale());
	}
}

void FWImpulseHapticLoop::ConstraintBasedRendering(){}


// FWImpulseHapticの実装
///////////////////////////////////////////////
FWImpulseHaptic::FWImpulseHaptic(){
	hapticLoop = &impulseLoop;
}

void FWImpulseHaptic::Init(){
	FWMultiRateHaptic::Init();
	double pdt = GetPHScene()->GetTimeStep();
	double hdt = GetIAScene()->hdt;
	InitIAAdaptee();
	hapticLoop->Init(pdt, hdt);
}

void FWImpulseHaptic::Clear(){
	FWMultiRateHaptic::Clear();
}

void FWImpulseHaptic::CallBackHapticLoop(){
	GetHapticLoop()->Step();
	Sync();
}

void FWImpulseHaptic::SyncHaptic2Physic(){
	/// HapticLoop--->PhysicsLoop
	double hdt = GetHapticLoop()->hdt;
	int lc = GetHapticLoop()->GetLoopCount(); 
	for(int i = 0; i < GetHapticLoop()->NIAPointers(); i++){
		FWInteractPointer* hip = GetHapticLoop()->GetIAPointer(i);

		FWInteractSolids* hiss = GetHapticLoop()->GetIASolids();
		for(int j = 0; j < (int)hiss->size(); j++){
			GetIAPointer(i)->interactInfo[j].toPhysic = hip->interactInfo[j].toPhysic;

			FWInteractInfo* iInfo = &hip->interactInfo[j];
			// bSim = ture かつ bfirstSim = falseなら結果を反映させる
			if(!iInfo->flag.blocal || iInfo->flag.bfirstlocal) continue;
			Vec3d cPoint = iInfo->toHaptic.pose * iInfo->toHaptic.closest_point;	// 近傍物体の接触点
			Vec3d force = iInfo->toPhysic.impulse;							// 近傍物体に加える力
			iInfo->toPhysic.impulse = Vec3d();								// 近傍物体に加わる力積の初期化
			PHSolid* ps = GetIASolid(j)->sceneSolid;
			ps->AddForce(force, cPoint);						// 近傍物体の接触点に力覚ポインタからの力を加える
		}
	}
}

void FWImpulseHaptic::SyncPhysic2Haptic(){
	/// PhysicsLoop--->HapticLoop ///
	// 1. シーンで新しく生成された分を拡張
	// 1.1. 力覚ポインタの増加分
	std::vector<FWInteractPointer>* hips= GetHapticLoop()->GetIAPointers();
	for(int i = (int)hips->size(); i < NIAPointers(); i++){
		hips->resize(i+1);
		hips->back() = *GetIAPointer(i);
		hips->back().Sync();
	}
	// 1.2. Solidの増加分
	FWInteractSolids* hiss = GetHapticLoop()->GetIASolids();
	for(int i = (int)hiss->size(); i < (int)NIASolids(); i++){
		hiss->resize(i + 1);
		hiss->back() = *GetIASolid(i);
		/// ポインタが持つ情報についても拡張
		for(int j = 0; j < NIAPointers(); j++){
			FWInteractPointer* hip = GetHapticLoop()->GetIAPointer(j);
			hip->interactInfo.resize(i + 1);
			hip->interactInfo.back() = GetIAPointer(j)->interactInfo[i];
		}
	}
	// 2. 情報の同期
	for(unsigned i = 0; i < hiss->size(); i++){
		FWInteractSolid* pis = GetIASolid(i);
		FWInteractSolid* his = GetHapticLoop()->GetIASolid(i);
		his->copiedSolid = *pis->sceneSolid;

		for(int j = 0; j < NIAPointers(); j++){
			FWInteractPointer* hip = GetHapticLoop()->GetIAPointer(j)->Cast();
			FWInteractPointer* pip = GetIAPointer(j);
			hip->interactInfo[i].toHaptic = pip->interactInfo[i].toHaptic;
			hip->interactInfo[i].flag = pip->interactInfo[i].flag;
			hip->bForce = pip->bForce;
			hip->bVibration = pip->bVibration;

			// 同期後の後処理
			pip->interactInfo[i].toHaptic.last_closest_point
				= pis->sceneSolid->GetPose() * pip->interactInfo[i].toHaptic.closest_point;
		}
	}
}

void FWImpulseHaptic::Step(){
	if (bSync) return;
	if (bCalcPhys){
		GetPHScene()->Step();
		UpdateSolidList();
		NeighborObjectFromPointer();
		bCalcPhys = false;
	}
	double pdt = GetPHScene()->GetTimeStep();
	double hdt = GetIAScene()->hdt;
	if (hapticcount < pdt / hdt) return;
	hapticcount -= (int)(pdt/hdt);
	bSync = true;
	bCalcPhys = true;	
}

void FWImpulseHaptic::UpdatePointer(){
	for(int i = 0; i < NIAPointers(); i++){	
		if(GetHapticLoop()->NIAPointers() == 0) return; 
		PHSolidIf* soPointer = GetIAPointer(i)->pointerSolid;
		FWInteractPointer* hiPointer = GetHapticLoop()->GetIAPointer(i)->Cast();
		PHSolid* hiSolid = &hiPointer->hiSolid;
		soPointer->SetVelocity(hiSolid->GetVelocity());
		soPointer->SetAngularVelocity(hiSolid->GetAngularVelocity());
		soPointer->SetFramePosition(hiSolid->GetFramePosition());
		soPointer->SetOrientation(hiSolid->GetOrientation());
		soPointer->SetDynamical(false);
	}
}

void FWImpulseHaptic::BeginKeyboard(){}
void FWImpulseHaptic::EndKeyboard(){}

}
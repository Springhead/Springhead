/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "FWImpulseHaptic.h"

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

void FWImpulseHapticLoop::HapticRendering(){
	for(int j = 0; j < NIAPointers(); j++){
		FWInteractPointer* iPointer = GetIAPointer(j)->Cast();
		if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
			HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
		}else{
			HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
		}

		SpatialVector outForce = SpatialVector();

		for(int i = 0; i < NIASolids(); i++){
			FWInteractSolid* iSolid = GetIASolid(i);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			if(!iInfo->flag.blocal) continue;
			NeighborInfo* nInfo = &iInfo->neighborInfo;
			PHSolid* cSolid = &iSolid->copiedSolid;
			Posed poseSolid = cSolid->GetPose();
			Vec3d last_cPoint = nInfo->last_closest_point;
			Vec3d cPoint = cSolid->GetPose() * nInfo->closest_point;			// 剛体の近傍点のワールド座標系
			Vec3d pPoint = iPointer->hiSolid.GetPose() * nInfo->pointer_point;	// 力覚ポインタの近傍点のワールド座標系
			Vec3d force_dir = pPoint - cPoint;
			Vec3d interpolation_normal;											// 提示力計算にしようする法線（前回の法線との間を補間する）
			Vec3d interpolation_cPoint;

			// 剛体の面の法線補間
			// 前回の法線と現在の法線の間を補間しながら更新
			double syncCount = pdt / hdt;						// プロセスの刻み時間の比
			interpolation_normal = (loopCount * nInfo->face_normal + 
				(syncCount - (double)loopCount) * nInfo->last_face_normal) / syncCount;															

			// 接触点の補間（剛体側の接触点を補間する）
			interpolation_cPoint = (loopCount * cPoint + 
				((double)syncCount - loopCount) * last_cPoint) / (double)syncCount;
			force_dir = pPoint - interpolation_cPoint;
		
			// 同期カウントを越えたら現在の法線，接触点を使う
			if(loopCount > syncCount){
				interpolation_normal = nInfo->face_normal;
				force_dir = pPoint - cPoint;
			}

			double f = force_dir * interpolation_normal;		// 剛体の面の法線と内積をとる
			if(f < 0.0){										// 内積が負なら力を計算
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

				/// 計算した力を剛体に加える//
				iPointer->interactInfo[i].mobility.force = nInfo->test_force = -1 * addforce;	
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

void FWImpulseHaptic::Step(){
	if (bSync) return;
	if (bCalcPhys){
		UpdatePointer();
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
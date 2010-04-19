/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "FWImpulseHaptic.h"

// FWImpulseHaptic�̎���
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
			ToHaptic* th = &iInfo->toHaptic;
			ToPhysic* tp = &iInfo->toPhysic;
			PHSolid* cSolid = &iSolid->copiedSolid;
			Posed poseSolid = cSolid->GetPose();
			Vec3d last_cPoint = th->last_closest_point;
			Vec3d cPoint = cSolid->GetPose() * th->closest_point;			// ���̂̋ߖT�_�̃��[���h���W�n
			Vec3d pPoint = iPointer->hiSolid.GetPose() * th->pointer_point;	// �͊o�|�C���^�̋ߖT�_�̃��[���h���W�n
			Vec3d force_dir = pPoint - cPoint;
			Vec3d interpolation_normal;											// �񎦗͌v�Z�ɂ��悤����@���i�O��̖@���Ƃ̊Ԃ��Ԃ���j
			Vec3d interpolation_cPoint;

			// ���̖̂ʂ̖@�����
			// �O��̖@���ƌ��݂̖@���̊Ԃ��Ԃ��Ȃ���X�V
			double syncCount = pdt / hdt;						// �v���Z�X�̍��ݎ��Ԃ̔�
			interpolation_normal = (loopCount * th->face_normal + 
				(syncCount - (double)loopCount) * th->last_face_normal) / syncCount;															

			// �ڐG�_�̕�ԁi���̑��̐ڐG�_���Ԃ���j
			interpolation_cPoint = (loopCount * cPoint + 
				((double)syncCount - loopCount) * last_cPoint) / (double)syncCount;
			force_dir = pPoint - interpolation_cPoint;
		
			// �����J�E���g���z�����猻�݂̖@���C�ڐG�_���g��
			if(loopCount > syncCount){
				interpolation_normal = th->face_normal;
				force_dir = pPoint - cPoint;
			}

			double f = force_dir * interpolation_normal;		// ���̖̂ʂ̖@���Ɠ��ς��Ƃ�
			if(f < 0.0){										// ���ς����Ȃ�͂��v�Z
				Vec3d ortho = f * interpolation_normal;			// �ߖT�_����͊o�|�C���^�ւ̃x�N�g���̖ʂ̖@���ւ̐��ˉe
				Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;

				/// �R�͂̌v�Z
				double K = iPointer->correctionSpringK;
				double D = iPointer->correctionDamperD;
				Vec3d addforce = -1 * (K * ortho + D * dvortho);
		
				double ws4 = pow(iPointer->GetWorldScale(), 4);
				outForce.v() += addforce / ws4;	
				outForce.w() = Vec3d();

				/// �v�Z�����͂����̂ɉ�����//
				iPointer->interactInfo[i].mobility.force = tp->test_force = -1 * addforce;	
			}
		}
		/// �C���^�t�F�[�X�֗͂��o��
		SetRenderedForce(iPointer->GetHI(), iPointer->bForce, outForce * iPointer->GetForceScale());
	}
}

void FWImpulseHapticLoop::ConstraintBasedRendering(){}


// FWImpulseHaptic�̎���
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

void FWImpulseHaptic::SyncHaptic2Phsyic(Spr::FWInteractSolid *h, Spr::FWInteractSolid *p){
	//Vec3d cPoint = hprocess.expandedObjects[i].syncInfo.neighborPoint.closestPoint;	// �ߖT���̂̐ڐG�_(���[���h���W�n)
	//Vec3d force = hprocess.expandedObjects[i].syncInfo.neighborPoint.impulse / (hprocess.hcount * hdt);											// �ߖT���̂ɉ������
	//hprocess.expandedObjects[i].syncInfo.neighborPoint.impulse = Vec3d();																						// �ߖT���̂ɉ����͐ς̏�����
	//expandedObjects[i].phSolidIf->AddForce(force, cPoint);
}

void FWImpulseHaptic::SyncPhsyic2Haptic(Spr::FWInteractSolid *h, Spr::FWInteractSolid *p){}

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
/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWVirtualCoupling.h>
#include <Framework/SprFWApp.h>
#include <Physics/PHConstraintEngine.h>
#include <sstream>

namespace Spr{;

FWVirtualCoupling::FWVirtualCoupling(){
}
void FWVirtualCoupling::CreateVCPointer(){
	if(vcSolid.size()== 0 ){
		int N = NIAPointers();
		for(int i = 0; i < N; i++){
			PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
			
			//vcSolidの作成(インタフェースに同期して動くshapeのないポインタ)
			PHSolidDesc desc;
			desc.mass = 1e20f;
			desc.inertia *= 1e30f;
			vcSolid.push_back( GetPHScene()->CreateSolid(desc) );
			vcSolid[i]->SetGravity(false);
			vcSolid[i]->SetDynamical(false);

			//vcJointの作成
			PHSpringDesc jointDesc;
			{
				jointDesc.poseSocket.Pos()	= Vec3f(0.0f, 0.0f, 0.0f);
				jointDesc.posePlug.Pos()	= Vec3f(0.0f, 0.0f, 0.0f);
				jointDesc.spring			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->correctionSpringK;
				jointDesc.damper			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->correctionDamperD;
				jointDesc.springOri			= GetIAPointer(i)->springK *1000;
				jointDesc.damperOri			= GetIAPointer(i)->damperD *1000;
			}
			vcJoint.push_back( GetPHScene()->CreateJoint(vcSolid[i], pSolid, jointDesc) );
		}
	}
}
void FWVirtualCoupling::GrabSolid(){
	int N = NIAPointers();
	for(int i = 0; i < N; i++){
		if(GetIAPointer(i)->GetGrabFlag()==1){
			PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
			int NSolids = GetPHScene()->NSolids();
			////Pointerに接触している剛体の取得
			PHSolidPairForLCPIf* pair;						// 剛体のペアをいれる変数pair
			int contact;									// 接触判定の結果0よりも大きい場合接触
			PHSolidIf* grabSolid;							// 接触している剛体
			//ポインタ剛体と接触している剛体のペアを検索
			for(int m=0; m< NSolids; m++){
				for(int n=m+1; n<NSolids; n++){
					// (m, n)番目の剛体ペアへのポインタを取得して，自分で作ったpairに代入
					pair =GetPHScene()->GetSolidPair(m, n);
					
					// ポインタ剛体との接触を判定
					for(int pairN = 0; pairN<2 ;pairN++){
						if(pSolid == pair->GetSolid(pairN)){
							// (m, n)番目の剛体ペアの接触判定　contact>0のとき接触
							for(int si=0; si<pair->GetSolid(0)->NShape(); si++){
								for(int sl=0; sl<pair->GetSolid(1)->NShape(); sl++){
									contact = pair->GetContactState(si, sl);
									if(contact>0){
										grabSolid = pair->GetSolid(1-pairN);
										continue;
									}
								}
							}
						}
					}
				}
			}
			//掴んだ時点のPoseを取得し，キャリブレーションを行う．
			//GetIAPointer(i)->grabPose = grabSolid->GetPose();

			//vcJointの作成
			PHSpringDesc jointDesc;
			{
				jointDesc.poseSocket.Pos()	= Vec3f(0.0f, 0.0f, 0.0f);
				jointDesc.poseSocket.Ori()	= grabSolid->GetPose().Ori();
				jointDesc.posePlug.Pos()	= Vec3f(0.0f, 0.0f, 0.0f);
				jointDesc.spring			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->correctionSpringK;
				jointDesc.damper			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->correctionDamperD;
				jointDesc.springOri			= GetIAPointer(i)->springK *1000;
				jointDesc.damperOri			= GetIAPointer(i)->damperD *1000;
			}
			grabJoint.push_back( GetPHScene()->CreateJoint(vcSolid[i], grabSolid, jointDesc) );

			//Pointer剛体の接触判定解除
			GetPHScene()->SetContactMode(GetIAPointer(i)->GetPointerSolid(),PHSceneDesc::MODE_NONE);
			//Jointの作成終了
			GetIAPointer(i)->SetGrabFlag(2);
		}else if(GetIAPointer(i)->GetGrabFlag()==3){
			//Pointer剛体の接触判定を復活
			GetPHScene()->SetContactMode(GetIAPointer(i)->GetPointerSolid(),PHSceneDesc::MODE_LCP);
			for(int i=0;i<grabJoint.size() ;i++){
				grabJoint[i]->Enable(false);
				grabJoint[i]->Clear();
			}
			grabJoint.clear();
			GetIAPointer(i)->SetGrabFlag(0);
		}

	}
}
void FWVirtualCoupling::UpdateInterface(){
	int N = NIAPointers();
		double pdt = GetPHScene()->GetTimeStep();
		for(int i = 0; i < N; i++){
			FWInteractPointer* iPointer = GetIAPointer(i)->Cast();
			double s = iPointer->GetWorldScale() * iPointer->GetPosScale();
			if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
				//6自由度インタフェースの場合
				HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
				hif->Update((float)pdt);
				PHSolid* hiSolid = &iPointer->hiSolid;
				hiSolid->SetVelocity((Vec3d)hif->GetVelocity() * s);
				hiSolid->SetAngularVelocity((Vec3d)hif->GetAngularVelocity());
				Posed hifPose;
				hifPose.Pos()=(Vec3d)hif->GetPosition() * s;
				hifPose.Ori()=hif->GetOrientation();
				Posed hiSolidPose = hifPose * GetIAPointer(i)->GetDefaultPosition()*GetIAPointer(i)->GetPointersCalibPosition();
				hiSolid->SetPose(hiSolidPose);
			}else{
				//3自由度インタフェースの場合
				HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
				hif->Update((float)pdt);
				PHSolid* hiSolid = &iPointer->hiSolid;
				hiSolid->SetVelocity((Vec3d)hif->GetVelocity() * s);
				Posed hifPose;
				hifPose.Pos()=(Vec3d)hif->GetPosition() * s;
				Posed hiSolidPose = hifPose * GetIAPointer(i)->GetDefaultPosition()*GetIAPointer(i)->GetPointersCalibPosition();
				hiSolid->SetPose(hiSolidPose);
			}
		}
}
void FWVirtualCoupling::UpdatePointerDirect(){
	//直接ポインタを更新する
	int N = NIAPointers();
	for(int i = 0; i<N ; i++){
		PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
		FWInteractPointer* iPointer = GetIAPointer(i)->Cast();
		PHSolid* hiSolid = &iPointer->hiSolid;					//インタフェースの位置を格納したPHSolid
		pSolid->SetPose(hiSolid->GetPose());

	}
}

void FWVirtualCoupling::UpdatePointer(){
	//バーチャルカップリングでポインタを更新する
	CreateVCPointer(); //VCポインタが作成されていない場合作成する．
	int N = NIAPointers();
	for(int i = 0; i<N ; i++){
		PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
		FWInteractPointer* iPointer = GetIAPointer(i)->Cast();
		PHSolid* hiSolid = &iPointer->hiSolid;					//インタフェースの位置を格納したPHSolid
		vcSolid[i]->SetPose(hiSolid->GetPose());				//vcSolidの位置をインタフェースの位置にする

		//ジョイントに働く力(world座標系)を取得
		SpatialVector outForce = SpatialVector();
		Posed jPose; vcJoint[i]->GetPlugPose(jPose);
		vcJoint[i]->GetConstraintForce(outForce.v(),outForce.w());
		double s = iPointer->GetForceScale();
		outForce.v() = jPose * outForce.v() * (-s);
		outForce.w() = jPose * outForce.w() * (-s);

		/// インタフェースへ力を出力
		if(iPointer->bForce){
			if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
				HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(outForce.v(), outForce.w());
			}else{
				HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(outForce.v());
			}
		}else{
			if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
				HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(Vec3d(), Vec3d());
			}else{
				HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(Vec3d());
			}		
		}

	}
}

void FWVirtualCoupling::Step(){
	UpdateInterface();
	UpdatePointer();
	//UpdatePointerDirect();
	//GetPHScene()->Step();　//LDHapticと併用できるようにするため，Step()は自分で呼ぶことにする
}

void FWVirtualCoupling::Clear(){
	bSync = false;
	bCalcPhys = true;
	hapticcount = 1;
	vcSolid.clear();
	vcJoint.clear();
}


}

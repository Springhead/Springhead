/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWGrabCoupling.h>
#include <Framework/SprFWApp.h>
#include <Physics/PHConstraintEngine.h>
#include <sstream>

namespace Spr{;

FWGrabCoupling::FWGrabCoupling(){
	cluchFlag = false;
	cluchTrans = Posed();
}
void FWGrabCoupling::CreatePointerSolid(){
	bool grabFlag = true;

	if(grabFlag){
		if(vcSolid.size()== 0 ){
			int N = NIAPointers();
			for(int i = 0; i < N; i++){
				PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
				pSolid->SetMass(1e30f);
				pSolid->SetInertia(1e30f*Matrix3d().Unit());
				pSolid->SetGravity(false);
				pSolid->SetIntegrate(false);
				pSolid->SetDynamical(false);
				GetPHScene()->SetContactMode(pSolid,PHSceneDesc::MODE_NONE);

			}
		}
	}else{
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
				vcSolid[i]->SetIntegrate(false);
				vcSolid[i]->SetDynamical(false);

				//vcJointの作成
				PHSpringDesc jointDesc;
				{
					jointDesc.poseSocket.Pos()	= Vec3f(0.0f, 0.0f, 0.0f);
					jointDesc.posePlug.Pos()	= Vec3f(0.0f, 0.0f, 0.0f);
					jointDesc.spring			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->springK;
					jointDesc.damper			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->damperD;
					jointDesc.springOri			= GetIAPointer(i)->springOriK;
					jointDesc.damperOri			= GetIAPointer(i)->damperOriD;
				}
				vcJoint.push_back( GetPHScene()->CreateJoint(vcSolid[i], pSolid, jointDesc) );
			}
		}
	}
}
void FWGrabCoupling::GrabSolid(){
	//ポインタの接触判定から判定する方法
	int N = NIAPointers();
	for(int i = 0; i < N; i++){
		if(GetIAPointer(i)->GetGrabFlag()==1){
			PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
			int NSolids = GetPHScene()->NSolids();
			////Pointerに接触している剛体の取得
			PHSolidPairForLCPIf* pair;						// 剛体のペアをいれる変数pair
			int contact;									// 接触判定の結果0よりも大きい場合接触
			PHSolidIf* grabSolid;							// 接触している剛体
			Vec3d cPoint;									// 剛体が接触している位置(World座標系)
			Vec3d cPointG;									// 掴む剛体座標系での接触点
			Vec3d cPointP;									// ポインタ座標系での接触点
			PHConstraintIf* param;
			bool createdFlag =false;

			////ポインタ剛体と接触している剛体のペアを検索
			for(int m=0; m< NSolids&&!createdFlag; m++){
				for(int n=m+1; n<NSolids&&!createdFlag; n++){
					// (m, n)番目の剛体ペアへのポインタを取得して，自分で作ったpairに代入
					pair =GetPHScene()->GetSolidPair(m, n);
					// ポインタと剛体の接触を判定
					for(int pairN = 0; pairN<2&&!createdFlag ;pairN++){     //ポインタがm番目or n番目ににあるので2回判定する.
						if(pSolid == pair->GetSolid(pairN)){  //m番目,n番目のどちらかの剛体がポインタの場合判定をおこなう
							// (m, n)番目の剛体ペアの接触判定　contact>0のとき接触
							for(int si=0; si<pair->GetSolid(0)->NShape()&&!createdFlag; si++){
								for(int sl=0; sl<pair->GetSolid(1)->NShape() &&!createdFlag; sl++){
									contact = pair->GetContactState(si, sl);
									param = GetPHScene()->GetConstraintEngine()->GetContactPoints()->FindBySolidPair(pair->GetSolid(0), pair->GetSolid(1));
									if(contact>0&&param){
										DSTR<<"CreateGrabPointer---------------------------"<<std::endl;
										DSTR<<pair->GetSolid(0)->GetName()<<"::"<<pair->GetSolid(1)->GetName()<<std::endl;
										cPoint = pair->GetCommonPoint(si, sl);
										DSTR<<"cPoint:"<<cPoint<<std::endl;
										grabSolid = pair->GetSolid(1-pairN);
										if(pairN==0){ //1番目がポインタの場合
											cPointG = grabSolid->GetShapePose(sl)* Vec3d(0,0,0);//掴む剛体座標系でのPlugの位置
											cPointP = pSolid->GetShapePose(si) * Vec3d(0,0,0);  //ポインタ座標系でのSoketの位置
										}
										if(pairN==1){ //2番目がポインタの場合
											cPointG = grabSolid->GetShapePose(si)* Vec3d(0,0,0);//掴む剛体座標系でのPlugの位置
											cPointP = pSolid->GetShapePose(sl) * Vec3d(0,0,0);  //ポインタ座標系でのSoketの位置
										}
										DSTR<<"cPointG"<<cPointG<<std::endl;
										//vcJointの作成
										PHSpringDesc jointDesc;
										double coefficient = 1; //バネを接続した瞬間は係数を高く設定しないと発散してしまう
										{
											jointDesc.poseSocket.Pos()	= Vec3d(0,0,0);
											jointDesc.poseSocket.Ori()	= grabSolid->GetPose().Ori();
											jointDesc.posePlug.Pos()	= cPointG;

											jointDesc.spring			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->springK * coefficient;
											jointDesc.damper			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->damperD* coefficient;
											jointDesc.springOri			= GetIAPointer(i)->springOriK* coefficient;
											jointDesc.damperOri			= GetIAPointer(i)->damperOriD* coefficient;
										}
										grabJoint.push_back( GetPHScene()->CreateJoint(vcSolid[i], grabSolid, jointDesc) );
										grabJoint[0]->SetName("grabJoint");
										//掴んだ剛体の位置をポインタの位置に移動
										grabSolid->SetPose(pSolid->GetPose());
										//Pointer剛体の接触判定解除
										GetPHScene()->SetContactMode(GetIAPointer(i)->GetPointerSolid(),PHSceneDesc::MODE_NONE);
										createdFlag = true;
										continue;
									}
								}
							}
						}
					}
				}
			}
			//Jointの作成終了
			if(createdFlag)GetIAPointer(i)->SetGrabFlag(2);
			else GetIAPointer(i)->SetGrabFlag(0);

		}else if(GetIAPointer(i)->GetGrabFlag()==4){
			//Pointer剛体の接触判定を復活
			GetPHScene()->SetContactMode(GetIAPointer(i)->GetPointerSolid(),PHSceneDesc::MODE_LCP);
			for(size_t i=0;i<grabJoint.size() ;i++){
				grabJoint[i]->Enable(false);
				grabJoint[i]->Clear();
			}
			grabJoint.clear();
			GetIAPointer(i)->SetGrabFlag(0);
		}

	}
}

void FWGrabCoupling::GrabSolid2(){
	//ポインタの位置情報から判定する方法
	int N = NIAPointers();
	for(int i = 0; i < N; i++){
		if(GetIAPointer(i)->GetGrabFlag()==1){
			PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
			int NSolids = GetPHScene()->NSolids();
			////Pointerに接触している剛体の取得
			PHSolidIf* grabSolid = NULL;							// 接触している剛体
			Posed pose = pSolid->GetPose();
			bool createdFlag =false;

			double length =1e30;
			////ポインタのレンジ内の剛体を検索
			PHSolidIf** solids = GetPHScene()->GetSolids();
			for(int n=0; n< NSolids&&!createdFlag; n++){
				//---
				std::string centerName;
				centerName= "soCenter";
				if(solids[n]->GetName() == centerName)continue;
				//---
				//if(!solids[n]->IsDynamical())continue;
				Posed shapePose = solids[n]->GetShapePose(0);
				Posed spose = solids[n]->GetPose() * shapePose;
				Vec3d lPose = spose.Pos() - pose.Pos();
				if(length > lPose.norm()) {
					length = lPose.norm();
					if(length <GetIAPointer(i)->localRange && solids[n]!=pSolid){
						grabSolid =solids[n]; //ポインタ近傍の剛体
					}
				}
			}
			
			///ジョイントの作成
			if(grabSolid){
				vcSolid.push_back(grabSolid);
				DSTR<<"grabSolid: "<<grabSolid->GetName()<<std::endl;
				grabSolid->GetDesc(&grabSolidDesc);
				grabSolid->SetDynamical(true);
				grabSolid->SetIntegrate(true);
				

				//Axis表示用の座標変換Poseを保存
				grabPointerPose = pSolid->GetPose().Inv() * grabSolid->GetPose();
				Posed shapePose = grabSolid->GetShapePose(0);
				PHSpringDesc jointDesc;
				double coefficient = 1; //バネを接続した瞬間は係数を高く設定しないと発散してしまう
				{
					jointDesc.poseSocket.Pos()	= Vec3d(0,0,0);
					jointDesc.poseSocket.Pos()	= Vec3d(0,0,0);
					jointDesc.poseSocket.Ori()	= pSolid->GetPose().Ori().Inv();
					//jointDesc.posePlug.Pos()	= shapePose * Vec3d(0,0,0);
					Posed plug =grabSolid->GetPose().Inv()*pSolid->GetPose();
					jointDesc.posePlug.Pos()	= plug.Pos();
					jointDesc.posePlug.Ori()	= grabSolid->GetShapePose(0).Ori().Inv() * grabSolid->GetPose().Ori().Inv();
					jointDesc.spring			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->springK * coefficient;
					jointDesc.damper			= Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(i)->damperD * coefficient;
					jointDesc.springOri			= GetIAPointer(i)->springOriK* coefficient;
					jointDesc.damperOri			= GetIAPointer(i)->damperOriD* coefficient;
					jointDesc.fMax				= 15.0;
				}
				//DSTR<<grabSolid->GetName()<<std::endl;
				std::string name[8];
				name[0]= "soRightUpperArm";
				name[1]= "soLeftUpperArm";
				name[2]= "soRightUpperKnee";
				name[3]= "soLeftUpperKnee";
				name[4]= "soWaist";
				name[5]= "soBreast";
				name[6]= "soHead";
				for(int i=0; i<6; i++){
					if(grabSolid->GetName() == name[i]){
						jointDesc.springOri			= 0.0;
						jointDesc.damperOri			= 0.0;
					}
				}
				grabJoint.push_back( GetPHScene()->CreateJoint(GetIAPointer(i)->GetPointerSolid(), grabSolid, jointDesc) );
				//掴んだ剛体の位置をポインタの位置に移動
				pSolid->SetPose(grabSolid->GetPose());

				
				//Pointer剛体の接触判定解除
				GetPHScene()->SetContactMode(GetIAPointer(i)->GetPointerSolid(),PHSceneDesc::MODE_NONE);
				createdFlag = true;
				//ポインタのShapeを削除
				//pointerShape = pSolid->GetShape(0);
				//pSolid->ClearShape();
				//continue;
			}

			//Jointの作成終了
			if(createdFlag)GetIAPointer(i)->SetGrabFlag(2);
			else GetIAPointer(i)->SetGrabFlag(0);

		}else if(GetIAPointer(i)->GetGrabFlag()==4){
			for(int i=0;i<grabJoint.size() ;i++){
				grabJoint[i]->Enable(false);
				grabJoint[i]->Clear();
			}
			for(int i=0;i<vcSolid.size() ;i++){
				//vcSolid[i]->SetDynamical(grabSolidDesc.dynamical);
				//vcSolid[i]->SetIntegrate(grabSolidDesc.integrate);
				vcSolid[i]->SetVelocity(Vec3d(0.0,0.0,0.0));
				vcSolid[i]->SetAngularVelocity(Vec3d(0.0,0.0,0.0));
			}
			vcSolid.clear();
			grabJoint.clear();
			GetIAPointer(i)->SetGrabFlag(0);
			//GetIAPointer(i)->GetPointerSolid()->AddShape(pointerShape);
		}

	}
}
void FWGrabCoupling::AdjustSpring(double n){
///// 剛体とポインタ間でジョイントを作成した瞬間はバネ係数を高く設定しないと発散してしまう．
///// そこで，接続後に設定したバネ係数に変更を行う
///// 現在は単一のポインタのみに対応
//
//	for(int i =0; i < grabJoint.size(); i++){
//		PHSpringIf* j = grabJoint[i]->Cast();
//		j->SetSpring(Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(0)->springK * n);
//		j->SetDamper(Vec3f(1.0f, 1.0f, 1.0f) * GetIAPointer(0)->damperD * n);
//		j->SetSpringOri(GetIAPointer(0)->springOriK * n);
//		j->SetDamperOri(GetIAPointer(0)->damperOriD * n);
//	}
//
//	for(int i =0; i < vcSolid.size(); i++){
//		PHSolidIf* s = vcSolid[i]->Cast();
//		s->SetDynamical(true);
//	}
//
//	
}

void FWGrabCoupling::UpdateInterface(){

	
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
				Posed hifPose;

				hifPose.Pos() = (Vec3d)hif->GetPosition() * s;
				hifPose.Ori() = hif->GetOrientation();
				Posed newCameraPose; 
				newCameraPose.Ori() = GetIAPointer(i)->GetCameraOri();

				//Posed hiSolidPose = GetIAPointer(i)->GetDefaultPosition()*hifPose;
				Posed origin = GetIAPointer(i)->GetCameraOrigin();
				
				Posed hiSolidPose = GetIAPointer(i)->GetDefaultPosition() * GetIAPointer(i)->cameraPose * hifPose * origin;
				origin  = hifPose.Inv() * newCameraPose.Inv() * GetIAPointer(i)->cameraPose * hifPose * origin;
				GetIAPointer(i)->SetCameraOrigin(origin);
				GetIAPointer(i)->cameraPose = newCameraPose;

				//hiSolidPose =	cameraPose*hiSolidPose; //カメラの移動に対して移動方向を変更
				if(!cluchFlag){
					hiSolid->SetPose(cluchTrans * hiSolidPose);
					hiSolid->SetVelocity((Vec3d)hif->GetVelocity() * s);
					hiSolid->SetAngularVelocity((Vec3d)hif->GetAngularVelocity());
					cluchPose[0] = cluchTrans * hiSolidPose;
				}else{
					//クラッチ時
					cluchPose[1] = hiSolidPose;
					cluchTrans = cluchPose[0] * cluchPose[1].Inv();
				}
			}else{
				//3自由度インタフェースの場合
				HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
				hif->Update((float)pdt);
				PHSolid* hiSolid = &iPointer->hiSolid;
				hiSolid->SetVelocity((Vec3d)hif->GetVelocity() * s);
				Posed hifPose;
				hifPose.Pos()=(Vec3d)hif->GetPosition() * s;
				Posed hiSolidPose = GetIAPointer(i)->GetDefaultPosition()*GetIAPointer(i)->GetPointersCalibPosition() * hifPose;
				hiSolid->SetPose(hiSolidPose);
			}
		}
}
void FWGrabCoupling::UpdatePointerDirect(){
	//直接ポインタを更新する
	int N = NIAPointers();
	
	for(int i = 0; i<N ; i++){
		PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
		FWInteractPointer* iPointer = GetIAPointer(i)->Cast();
		PHSolid* hiSolid = &iPointer->hiSolid;					//インタフェースの位置を格納したPHSolid

		pSolid->SetPose(hiSolid->GetPose());
			
	}
}

void FWGrabCoupling::UpdateGrabPointer(){
	//バーチャルカップリングでポインタを更新する
	int N = NIAPointers();
	SpatialVector outForce;
	FWInteractPointer* iPointer;
	for(int i = 0; i<N ; i++){
			PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
			iPointer = GetIAPointer(i)->Cast();
			PHSolid* hiSolid = &iPointer->hiSolid;					//インタフェースの位置を格納したPHSolid
			hiSolid->SetPose(hiSolid->GetPose());				//vcSolidの位置をインタフェースの位置にする

			//ジョイントに働く力(world座標系)を取得
			outForce = SpatialVector();
			Posed jPose;
			Posed cPose = iPointer->GetCameraOrigin();
		if(GetIAPointer(i)->GetGrabFlag()==3){
			//GrabSolidの場合
			grabJoint[i]->GetSocketPose(jPose);
			grabJoint[i]->GetConstraintForce(outForce.v(),outForce.w());

			double fs = iPointer->GetForceScale();
			double ts = iPointer->GetTorqueScale();
			outForce.v() = jPose * cPose* outForce.v() * (-fs);
			outForce.w() = jPose * cPose*  outForce.w() * (-ts);
		}else{
			outForce.v() = Vec3d(0.0,0.0,0.0);
			outForce.w() = Vec3d(0.0,0.0,0.0);
		}

		double fRange = 1.0;

		if(outForce.v().norm() > fRange){
			for(int i=0; i<3 ; i++){
				outForce.v()[i] = outForce.v()[i] * fRange / outForce.v().norm();
			}
		}

		double fRotRange = 1.0;
		if(outForce.w().norm() > fRotRange){
			for(int i=0; i<3 ; i++){
				outForce.w()[i] = outForce.w()[i] * fRotRange / outForce.w().norm();
			}
		}
		//outForce.v() = Vec3d(0.0,0.0,0.0);

		//DSTR<<outForce<<std::endl;

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
	UpdatePointerDirect();
}

void FWGrabCoupling::UpdatePointer(){
	//バーチャルカップリングでポインタを更新する
	//CreateVCPointer(); //VCポインタが作成されていない場合作成する．
	int N = NIAPointers();
	for(int i = 0; i<N ; i++){
		PHSolidIf* pSolid = GetIAPointer(i)->GetPointerSolid(); //物理シミュレーションのポインタ
		FWInteractPointer* iPointer = GetIAPointer(i)->Cast();
		PHSolid* hiSolid = &iPointer->hiSolid;					//インタフェースの位置を格納したPHSolid
		vcSolid[i]->SetPose(hiSolid->GetPose());				//vcSolidの位置をインタフェースの位置にする

		//ジョイントに働く力(world座標系)を取得
		SpatialVector outForce = SpatialVector();
		Posed jPose;
		Posed cPose ;cPose.Ori()= iPointer->GetCameraOri();

		if(GetIAPointer(i)->GetGrabFlag()==3){
			//GrabSolidの場合
			grabJoint[i]->GetSocketPose(jPose);
			grabJoint[i]->GetConstraintForce(outForce.v(),outForce.w());
		}else{
			vcJoint[i]->GetSocketPose(jPose);
			vcJoint[i]->GetConstraintForce(outForce.v(),outForce.w());
		}
		double s = iPointer->GetForceScale();
		outForce.v() = cPose.Inv() *jPose * outForce.v() * (-s);
		outForce.w() = cPose.Inv() *jPose * outForce.w() * (-s);

		DSTR<<jPose * outForce.v() * (-s)<<" 変換後 "<<cPose *jPose * outForce.v() * (-s)<<std::endl;

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

void FWGrabCoupling::Step(){
	UpdateInterface();
	//UpdatePointer();
	UpdateGrabPointer();
	//UpdatePointerDirect();
	//GetPHScene()->Step();　//LDHapticと併用できるようにするため，Step()は自分で呼ぶことにする
}

void FWGrabCoupling::Clear(){
	bSync = false;
	bCalcPhys = true;
	hapticcount = 1;
	vcSolid.clear();
	vcJoint.clear();
}


}

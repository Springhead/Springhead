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
FWLDHapticLoop::FWLDHapticLoop(){
	proK = 50;
	proD = 0.005;
	proM = 7.5*10e-7;
	bPic = false;
	picNum = 0;
	for(int i=0;i<2;i++){
		DisplayProxy[i] = Vec3d(5,0,0);
		DisplayForce[i] = Vec3d(0,0,0);
		//contactFlag[i].push_back(false);
		//proxy[i].push_back(Vec3d(3,0,0));
		//proVel[i].push_back(Vec3d(0,0,0));
		//pLastPoint[i].push_back(Vec3d(0,0,0));
	}
}
void FWLDHapticLoop::Step(){
	UpdateInterface();
	switch(hmode){
		case PENALTY:
			HapticRendering();
			break;
		case PROXY:
			Proxy();
			break;
		case PROXYSIMULATION:
			ProxySimulation();
			break;
		default:
			Proxy();
			break;
	}
	LocalDynamics();
}

void FWLDHapticLoop::UpdateInterface(){
	int N = NIAPointers();
	for(int i = 0; i < N; i++){
		FWInteractPointer* iPointer = GetIAPointer(i)->Cast();
		double s = iPointer->GetPosScale();
		if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
			//6自由度インタフェースの場合
			HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
			hif->Update((float)hdt);
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
			hif->Update((float)hdt);
			PHSolid* hiSolid = &iPointer->hiSolid;
			hiSolid->SetVelocity((Vec3d)hif->GetVelocity() * s);
			Posed hifPose;
			hifPose.Pos()=(Vec3d)hif->GetPosition() * s;
			Posed hiSolidPose = hifPose * GetIAPointer(i)->GetDefaultPosition()*GetIAPointer(i)->GetPointersCalibPosition();
			hiSolid->SetPose(hiSolidPose);
		}
	}
}

void FWLDHapticLoop::HapticRendering(){
	bPic = false;
	for(int j = 0; j < NIAPointers(); j++){
		FWInteractPointer* iPointer = GetIAPointer(j)->Cast();
		if((int)iPointer->bContact.size() < NIASolids()){
			for(int i=(int)iPointer->bContact.size(); i<NIASolids(); i++){
				iPointer->bContact.push_back(false);
				contactFlag[0].push_back(false);
				contactFlag[1].push_back(false);
				oVibForce.push_back(Vec3d(0,0,0));
			}
		}		
		if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
			HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
		}else{
			HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
		}

		SpatialVector outForce = SpatialVector();
//		Vibration(NIASolids());
		for(int i = 0; i < NIASolids(); i++){
			FWInteractSolid* iSolid = GetIASolid(i);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			if(!iInfo->flag.blocal) continue;
			NeighborInfo* nInfo = &iInfo->neighborInfo;
			PHSolid* cSolid = &iSolid->copiedSolid;
			Posed poseSolid = cSolid->GetPose();
			Vec3d cPoint = cSolid->GetPose() * nInfo->closest_point;			// 剛体の近傍点のワールド座標系
			Vec3d pPoint = iPointer->hiSolid.GetPose() * nInfo->pointer_point;	// 力覚ポインタの近傍点のワールド座標系
			Vec3d force_dir = pPoint - cPoint;
			Vec3d interpolation_normal;											// 提示力計算にしようする法線（前回の法線との間を補間する）
			if((int)proxy[j].size() <= NIASolids()){
				for(int k=(int)proxy[j].size(); k<NIASolids(); k++){
					proxy[j].push_back(poseSolid.Inv()*pPoint);
					proVel[j].push_back(Vec3d(0,0,0));
					pLastPoint[j].push_back(Vec3d(0,0,0));
				}
			}

			// 剛体の面の法線補間
			// 前回の法線と現在の法線の間を補間しながら更新
			double syncCount = pdt / hdt;						// プロセスの刻み時間の比
			interpolation_normal = (loopCount * nInfo->face_normal + 
				(syncCount - (double)loopCount) * nInfo->last_face_normal) / syncCount;															
			if(loopCount > syncCount)	interpolation_normal = nInfo->face_normal;

			double f = force_dir * interpolation_normal;		// 剛体の面の法線と内積をとる
			if(f < 0.0){										// 内積が負なら力を計算
				contactFlag[j][i] = true;
				if(contactFlag[0][i]&&contactFlag[1][i]) bPic = true;	// 物体を把持しているかどうか
				Vec3d pVibforce = Vec3d(0,0,0);
				Vec3d oVibforce = Vec3d(0,0,0);
				Vec3d ortho = f * interpolation_normal;			// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
				Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;

				Vec3d friction_dir = -1 * (force_dir - ortho);	// 摩擦力がポインタに加わる方向
				Vec3d friction_normal = friction_dir.unit();
				double friction_rate = friction_dir.norm() / ortho.norm();
				PTM::TMatrixRow<1,3,double> fdtrans;
				fdtrans[0][0] = friction_normal[0];
				fdtrans[0][1] = friction_normal[1];
				fdtrans[0][2] = friction_normal[2];
				Vec3d dvortho2 = dvortho - (fdtrans * dvortho)[0] * friction_normal;

				/// 抗力の計算
				double K = iPointer->springK;
				double D = iPointer->damperD;
				Vec3d addforce = -1 * (K * ortho + D * dvortho2);
				Vec3d addtorque = (pPoint - cSolid->GetCenterPosition()) % addforce ;

				///振動の計算
				if(iPointer->bVibration)	pVibforce = Vibration(iSolid, iPointer, i);

				outForce.v() += addforce + pVibforce;	
				outForce.w() += addtorque;

				DisplayForce[j] = outForce.v();

				/// 計算した力を剛体に加える//
				iPointer->interactInfo[i].mobility.force = -1 * addforce * iPointer->GetForceScale();	
				nInfo->test_force_norm = addforce.norm() * iPointer->GetForceScale();
				nInfo->test_force = addforce * iPointer->GetForceScale();
				//if(iPointer->bForce)	DSTR << vibforce << endl;
			}else{
				iPointer->bContact[i] = false;
				contactFlag[j][i] = false;
			}
		}

		/// インタフェースへ力を出力
		if(iPointer->bForce){
			if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
				HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(outForce.v(), Vec3d());
				#ifdef TORQUE
					hif->SetForce(outForce.v(), outForce.w());
				#endif
			}else{
				HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(outForce.v());
			}
		}else{
			if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
				HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(Vec3d(), Vec3d());
				#ifdef TORQUE
					hif->SetForce(Vec3d(), Vec3d());
				#endif
			}else{
				HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(Vec3d());
			}		
		}
	}
}

void FWLDHapticLoop::LocalDynamics(){
	for(int i = 0; i < NIASolids(); i++){
		FWInteractSolid* iSolid = FWHapticLoopBase::GetIASolid(i);
		if(!iSolid->bSim) continue;
		SpatialVector vel;
		vel.v() = iSolid->copiedSolid.GetVelocity();
		vel.w() = iSolid->copiedSolid.GetAngularVelocity();
		if(GetLoopCount() == 1){
			vel += (iSolid->curb - iSolid->lastb) *  pdt;	// 衝突の影響を反映

		}
		for(int j = 0; j < NIAPointers(); j++){
			FWInteractPointer* iPointer = GetIAPointer(j);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			if(!iInfo->flag.blocal) continue;
			vel += (iInfo->mobility.A * iInfo->mobility.force) * hdt;			// 力覚ポインタからの力による速度変化
			iInfo->mobility.force = Vec3d();
		}
		vel += iSolid->b * hdt;
		iSolid->copiedSolid.SetVelocity(vel.v());		
		iSolid->copiedSolid.SetAngularVelocity(vel.w());
		iSolid->copiedSolid.SetCenterPosition(iSolid->copiedSolid.GetCenterPosition() + vel.v() * hdt);
		iSolid->copiedSolid.SetOrientation(( Quaterniond::Rot(vel.w() * hdt) * iSolid->copiedSolid.GetOrientation()).unit());

		//Debug
		//DSTR<<"Vel:"<<iSolid->copiedSolid.GetVelocity()<<std::endl;
		//DSTR<<"AngVel:"<<iSolid->copiedSolid.GetAngularVelocity()<<std::endl;
		//DSTR<<"Pos:"<<iSolid->copiedSolid.GetCenterPosition()<<std::endl;
		//DSTR<<"Ori:"<<iSolid->copiedSolid.GetOrientation()<<std::endl;
		//DSTR<<"----------------------------"<<std::endl;

 		iSolid->copiedSolid.SetUpdated(true);
		iSolid->copiedSolid.Step();
	}
}

Vec3d FWLDHapticLoop::Vibration(FWInteractSolid* iSolid, FWInteractPointer* iPointer, int n){
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
#if 1
Vec3d FWLDHapticLoop::Vibration(int nSolids){
	PHSceneIf* phScene = GetIAAdaptee()->GetPHScene();
	
	int sceneCnt = phScene->GetCount();
	std::vector<Vec3d> vel;
	if((int)vel.size() < nSolids){
		for(int i=vel.size(); i<nSolids; i++){
			vel.push_back(Vec3d(0,0,0));
		}
	}

	PHSolidPairForLCPIf* solidPair;
	PHShapePairForLCPIf* shapePair;

	for (int i=0; i<nSolids; ++i) {
		for (int j=i+1; j<nSolids; ++j) {
			PHSolidIf *so1, *so2;
			so1 = phScene->GetSolids()[i];
			so2 = phScene->GetSolids()[j];

			solidPair = phScene->GetSolidPair(i, j);
			PHConstraintIf* constraint = phScene->GetConstraintEngine()->GetContactPoints()->FindBySolidPair(so1, so2);

			if (!solidPair)  { continue; }
			if (!constraint) { continue; }

			for (int s=0; s<so1->NShape(); ++s) {
				for (int t=0; t<so2->NShape(); ++t) {

					// この方法だと同じ剛体の異なるShapeについての接触は
					// 異なるContactとなる。
					// それを剛体ごとにまとめるには皮膚感覚の加算についての知見が必要。
					// とりあえずあとまわし (mitake, 09/02/07)

					shapePair = solidPair->GetShapePair(s, t);

					if (!shapePair) { continue; }

					int			contactStat	= solidPair->GetContactState(s, t);
					unsigned	lastContCnt	= solidPair->GetLastContactCount(s, t);

					if (contactStat == 1 || (contactStat == 2 && (lastContCnt == sceneCnt-1))) {
						if(contactStat ==1){
							vel[i] = so1->GetVelocity() - so2->GetVelocity();
							vel[j] = so2->GetVelocity() - so1->GetVelocity();
							so1->GetShape(0)->SetVibT(0.0);
							so2->GetShape(0)->SetVibT(0.0);
						}

						double vibT1 = so1->GetShape(0)->GetVibT();
						double vibA1 = so1->GetShape(0)->GetVibA();
						double vibB1 = so1->GetShape(0)->GetVibB();
						double vibW1 = so1->GetShape(0)->GetVibW();

						double vibT2 = so2->GetShape(0)->GetVibT();
						double vibA2 = so2->GetShape(0)->GetVibA();
						double vibB2 = so2->GetShape(0)->GetVibB();
						double vibW2 = so2->GetShape(0)->GetVibW();

						oVibForce[i] += vibA1 * (vel[i]*0.03) * exp(-vibB1 * vibT1) * sin(2 * M_PI * vibW1 * vibT1);		//振動計算
						oVibForce[j] += vibA2 * (vel[j]*0.03) * exp(-vibB2 * vibT2) * sin(2 * M_PI * vibW2 * vibT2);		//振動計算

						so1->GetShape(0)->SetVibT(vibT1+hdt);
						so2->GetShape(0)->SetVibT(vibT2+hdt);
					}
				}
			}
		}
	}
}
#endif
void FWLDHapticLoop::Proxy(){
	static bool friFlag = false;
	bPic = false;

	for(int j = 0; j < NIAPointers(); j++){
		FWInteractPointer* iPointer = GetIAPointer(j)->Cast();
		if((int)iPointer->bContact.size() < NIASolids()){
			for(int i=(int)iPointer->bContact.size(); i<NIASolids(); i++){
				iPointer->bContact.push_back(false);
				contactFlag[0].push_back(false);
				contactFlag[1].push_back(false);
			}
		}
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
			Vec3d cPoint = cSolid->GetPose() * nInfo->closest_point;			// 剛体の近傍点のワールド座標系
			Vec3d pPoint = iPointer->hiSolid.GetPose() * nInfo->pointer_point;	// 力覚ポインタの近傍点のワールド座標系
			Vec3d force_dir = pPoint - cPoint;
			Vec3d interpolation_normal;											// 提示力計算にしようする法線（前回の法線との間を補間する）
			if((int)proxy[j].size() <= NIASolids()){
				for(int k=(int)proxy[j].size(); k<NIASolids(); k++){
					proxy[j].push_back(poseSolid.Inv()*pPoint);
					proVel[j].push_back(Vec3d(0,0,0));
					pLastPoint[j].push_back(Vec3d(0,0,0));
				}
			}

			// 剛体の面の法線補間
			// 前回の法線と現在の法線の間を補間しながら更新
			double syncCount = pdt / hdt;						// プロセスの刻み時間の比
			interpolation_normal = (loopCount * nInfo->face_normal + 
				(syncCount - (double)loopCount) * nInfo->last_face_normal) / syncCount;															
			if(loopCount > syncCount)	interpolation_normal = nInfo->face_normal;

			double f = force_dir * interpolation_normal;		// 剛体の面の法線と内積をとる
			if(f < 0.0){										// 内積が負なら力を計算
				contactFlag[j][i] = true;
				Vec3d vibforce = Vec3d(0,0,0);
				Vec3d ortho = f * interpolation_normal;			// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
				Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;

				/// 抗力の計算
				double K = iPointer->springK;
				double D = iPointer->damperD;
				Vec3d addforce = -K * (pPoint - (poseSolid * proxy[j][i])) - D * dvortho;
				Vec3d addtorque = (pPoint - cSolid->GetCenterPosition()) % addforce ;

				///振動の計算
				if(iPointer->bVibration)	vibforce = Vibration(iSolid, iPointer, i);

				// 摩擦の計算
				Vec3d wproxy = poseSolid * proxy[j][i];
				Vec3d posVec = pPoint - (wproxy);
				double posDot = dot(interpolation_normal,posVec);
				Vec3d tVec = posDot * interpolation_normal;
				Vec3d tanjent = posVec - tVec;
				double mu0 = cSolid->GetShape(0)->GetStaticFriction();
				double mu1 = cSolid->GetShape(0)->GetDynamicFriction();
				if(!friFlag){
					if(tanjent.norm() > abs(mu0 * posDot)) friFlag = true;
				}
				if(friFlag){
					if(tanjent.norm() > abs(mu1 * posDot)){
						dproxy = (tanjent.norm() - abs(mu1 * posDot)) * tanjent.unit();
						wproxy += dproxy;
						proxy[j][i] = poseSolid.Inv() * wproxy;
					}
				}

				outForce.v() += addforce + vibforce;	
				outForce.w() += addtorque;

				DisplayForce[j] = outForce.v();

				/// 計算した力を剛体に加える
				iPointer->interactInfo[i].mobility.force = -1 * addforce* iPointer->GetForceScale();						
				nInfo->test_force_norm = addforce.norm()* iPointer->GetForceScale();;
				nInfo->test_force = addforce* iPointer->GetForceScale();;

			}else{
				iPointer->bContact[i] = false;
				contactFlag[j][i] = false;
				proxy[j][i] = poseSolid.Inv() * pPoint;
			}
			DisplayProxy[0] = poseSolid * proxy[0][i];
//			DisplayProxy[1] = poseSolid * proxy[1][i];
		}

		/// インタフェースへ力を出力
		if(iPointer->bForce){
			if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
				HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(outForce.v(), Vec3d());
				#ifdef TORQUE
					hif->SetForce(outForce.v(), outForce.w());
				#endif
			}else{
				HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(outForce.v());
			}
		}else{
			if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
				HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(Vec3d(), Vec3d());
				#ifdef TORQUE
					hif->SetForce(Vec3d(), Vec3d());
				#endif
			}else{
				HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
				hif->SetForce(Vec3d());
			}		
		}
	}
	for(int k=0;k<NIASolids();k++){
		if(contactFlag[0][k]&&contactFlag[1][k]){
			picNum = k;
			bPic = true;
		}
	}
}

void FWLDHapticLoop::ProxySimulation(){
	static bool friFlag = false;
	Vec3d dproVel = Vec3d(0,0,0);
	bPic = false;

	for(int j = 0; j < NIAPointers(); j++){
		FWInteractPointer* iPointer = GetIAPointer(j)->Cast();
		if((int)iPointer->bContact.size() < NIASolids()){
			for(int i=(int)iPointer->bContact.size(); i<NIASolids(); i++){
				iPointer->bContact.push_back(false);
				contactFlag[0].push_back(false);
				contactFlag[1].push_back(false);
			}
		}
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
			Vec3d cPoint = cSolid->GetPose() * nInfo->closest_point;			// 剛体の近傍点のワールド座標系
			Vec3d pPoint = iPointer->hiSolid.GetPose() * nInfo->pointer_point;	// 力覚ポインタの近傍点のワールド座標系
			Vec3d force_dir = pPoint - cPoint;
			Vec3d interpolation_normal;											// 提示力計算にしようする法線（前回の法線との間を補間する）
			if((int)proxy[j].size() < NIASolids()){
				for(int k=(int)proxy[j].size(); k<NIASolids(); k++){
					proxy[j].push_back(poseSolid.Inv()*pPoint);
					proVel[j].push_back(Vec3d(0,0,0));
					pLastPoint[j].push_back(Vec3d(0,0,0));
				}
			}

			// 剛体の面の法線補間
			// 前回の法線と現在の法線の間を補間しながら更新
			double syncCount = pdt / hdt;						// プロセスの刻み時間の比
			interpolation_normal = (loopCount * nInfo->face_normal + 
				(syncCount - (double)loopCount) * nInfo->last_face_normal) / syncCount;															
			if(loopCount > syncCount)	interpolation_normal = nInfo->face_normal;

			double f = force_dir * interpolation_normal;		// 剛体の面の法線と内積をとる
			if(f < 0.0){										// 内積が負なら力を計算
				contactFlag[j][i] = true;
				Vec3d vibforce = Vec3d(0,0,0);
				Vec3d ortho = f * interpolation_normal;			// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
				Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;

				/// 抗力の計算
				double K = iPointer->springK;
				double D = iPointer->damperD;
				Vec3d addforce;
				Vec3d addtorque;
				if(dvortho.norm() < 0){
					addforce = -K * (pPoint - (poseSolid * proxy[j][i])) - D * dvortho;
					addtorque = (pPoint - cSolid->GetCenterPosition()) % addforce ;
				}else{
					addforce = -K * (pPoint - (poseSolid * proxy[j][i]));
					addtorque = (pPoint - cSolid->GetCenterPosition()) % addforce ;
				}

				///振動の計算
				if(iPointer->bVibration){
					vibforce = Vibration(iSolid, iPointer, i);
				}

				// 摩擦の計算
				Vec3d pvel = iPointer->hiSolid.GetPointVelocity(pPoint);
				Vec3d wproxy = poseSolid * proxy[j][i];
				Vec3d posVec = pLastPoint[j][i] - (wproxy);
				double posDot = dot(interpolation_normal,posVec);
				Vec3d tVec = posDot * interpolation_normal;
				Vec3d tanjent = posVec - tVec;
				Vec3d velVec = pvel - proVel[j][i];
				double velDot = dot(interpolation_normal,velVec);
				Vec3d velTan = velVec - (velDot * interpolation_normal);

				double mu0 = cSolid->GetShape(0)->GetStaticFriction();
				double mu1 = cSolid->GetShape(0)->GetDynamicFriction();
				if(!friFlag){
					if(tanjent.norm() >= abs(mu0 * posDot))		friFlag = true;
				}
				if(friFlag){
					Vec3d dPOS;
					Vec3d dVEL;
					Vec3d dFRI;
					if((tanjent.norm() == 0)&&(velTan.norm() == 0)){
						dPOS = Vec3d(0,0,0);
						dVEL = Vec3d(0,0,0);
						dFRI = Vec3d(0,0,0);
					}else if(tanjent.norm() == 0){
						dPOS = - hdt * proVel[j][i];
						dVEL = dot((pvel - proVel[j][i]),velTan.unit()) * velTan.unit();
						dFRI = Vec3d(0,0,0);
					}else if(velTan.norm() == 0){
						dPOS = dot(pPoint-wproxy,tanjent.unit()) * tanjent.unit();
						dVEL = Vec3d(0,0,0);
						dFRI = abs(posDot*proK * mu1)*tanjent.unit();
					}else{
						dPOS = dot(pPoint-wproxy,tanjent.unit()) * tanjent.unit() - hdt * proVel[j][i];
						dVEL = dot((pvel - proVel[j][i]),velTan.unit()) * velTan.unit();
						dFRI = abs(posDot*proK * mu1)*tanjent.unit();
					}
					if(proVel[j][i].norm() < 0){
						dproVel = (proK * dPOS + proD * dVEL + dFRI) * (hdt/(proM + proD*hdt + proK*hdt*hdt));
					}else{
						dproVel = (proK * dPOS + proD * dVEL - dFRI) * (hdt/(proM + proD*hdt + proK*hdt*hdt));
					}
					proVel[j][i] += dproVel;
					wproxy += proVel[j][i] * hdt;
					proxy[j][i] = poseSolid.Inv() * wproxy;

					if(proVel[j][i].norm() < 1){
						proVel[j][i] = Vec3d(0,0,0);
						friFlag = false;
					}
				}

				outForce.v() += addforce + vibforce;	
				outForce.w() += addtorque;

				DisplayForce[j] = outForce.v();

				/// 計算した力を剛体に加える
				iPointer->interactInfo[i].mobility.force = -1 * addforce* iPointer->GetForceScale();;						
				nInfo->test_force_norm = addforce.norm()* iPointer->GetForceScale();;
				nInfo->test_force = addforce* iPointer->GetForceScale();;
			}else{
				iPointer->bContact[i] = false;
				contactFlag[j][i] = false;
				proxy[j][i] = poseSolid.Inv() * pPoint;
				proVel[j][i] = Vec3d(0,0,0);
			}
			DisplayProxy[0] = poseSolid * proxy[0][i];
//			DisplayProxy[1] = poseSolid * proxy[1][i];
			pLastPoint[j][i] = pPoint;

			/// インタフェースへ力を出力
			if(iPointer->bForce){
				if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
					HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
					hif->SetForce(outForce.v(), Vec3d());
					#ifdef TORQUE
						hif->SetForce(outForce.v(), outForce.w());
					#endif
				}else{
					HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
					hif->SetForce(outForce.v());
				}
			}else{
				if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
					HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
					hif->SetForce(Vec3d(), Vec3d());
					#ifdef TORQUE
					hif->SetForce(Vec3d(), Vec3d());
					#endif
				}else{
					HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
					hif->SetForce(Vec3d());
				}		
			}
		}
	}
	for(int k=0;k<NIASolids();k++){
		if(contactFlag[0][k]&&contactFlag[1][k]){
			picNum = k;
			bPic = true;
		}
	}
}

Vec3d* FWLDHapticLoop::GetProxyPoint(){
	return DisplayProxy;
};

Vec3d* FWLDHapticLoop::GetForce(){
	return DisplayForce;
}

bool FWLDHapticLoop::GetContactFlag(){
	return bPic;
};
#if 0
int* FWLDHapticLoop::ContactStat(int nPic){
	PHSceneIf* phScene = GetPHScene();
	int	contactStat[phScene->NSolids()];

	PHSolidPairForLCPIf* solidPair;
	PHShapePairForLCPIf* shapePair;

	for(int i=0; i<phScene->NSolids(); ++i){
		PHSolidIf *so1, *so2;
		so1	= phScene->GetSolids()[i];
		so2	= phScene->GetSolids()[nPic];
		solidPair = phScene->GetSolidPair(i,nPic);
		PHConstraintIf* constraint = phScene->GetConstraintEngine()->GetContactPoints()->FindBySolidPair(so1, so2);

		if (!solidPair){
			int contactStat[i] = 0;
			continue;
		}
		if (!constraint){
			int contactStat[i] = 0;
			continue;
		}

		for (int s=0; s<so1->NShape(); ++s){
			for (int t=0; t<so2->NShape(); ++t){
				shapePair = solidPair->GetShapePair(s, t);

				if (!shapePair){ 
					int contactStat[i] = 0;
					continue;
				}
				int	contactStat[i]	= solidPair->GetContactState(s, t);
			}
		}
	}
	return contactStat;
}
#endif
/*
int GetPicTime();
int GetPenetrate();
*/
// FWLDHapticAppの実装
//////////////////////////////////////////////////////////////////////////////////////////////
FWLDHaptic::FWLDHaptic(){}
void FWLDHaptic::Init(){
	FWMultiRateHaptic::Init();
	hapticLoop = &LDHapticLoop;
	double pdt = GetPHScene()->GetTimeStep();
	double hdt = GetIAScene()->hdt;
	InitIAAdaptee();
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
	double pdt = GetPHScene()->GetTimeStep();
	double hdt = GetIAScene()->hdt;
	if (hapticcount < pdt / hdt) return;
	hapticcount -= (int)(pdt/hdt);
	bSync = true;
	bCalcPhys = true;
}
void FWLDHaptic::PhysicsStep(){
	std::vector<SpatialVector> lastvel;
	for(int i = 0; i < NIASolids(); i++){
		if(!GetIASolid(i)->bSim) continue;
		lastvel.resize(i + 1);
		PHSolid* phSolid = GetIASolid(i)->sceneSolid;
		lastvel.back().v() = phSolid->GetVelocity();
		lastvel.back().w() = phSolid->GetAngularVelocity();
	}
	/// シミュレーションを進める

	//DSTR<<"Physicsシミュレーション"<<std::endl;
	GetPHScene()->Step();
	for(int i = 0; i < NIASolids(); i++){
		if(!GetIASolid(i)->bSim) continue;
		SpatialVector curvel;
		PHSolid* phSolid = GetIASolid(i)->sceneSolid;
		curvel.v() = phSolid->GetVelocity();
		curvel.w() = phSolid->GetAngularVelocity();
		double pdt = GetPHScene()->GetTimeStep();
		GetIASolid(i)->curb = (curvel - lastvel[i]) / pdt;
		//DSTR << "naga"<<curvel.w() << std::endl;
//		DSTR << GetIASolid(i)->curb << std::endl;
	}
}

void FWLDHaptic::UpdatePointer(){
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

void FWLDHaptic::TestSimulation(){
	/** FWInteractSolidsのblocalがtrueの物体に対してテスト力を加え，
		接触しているすべての物体について，モビリティを計算する */
	PHSceneIf* phScene = GetPHScene();

	#ifdef DIVIDE_STEP
	/// テストシミュレーションのために現在の剛体の状態を保存する
	states2->SaveState(phScene);		
	///	LCPの直前までシミュレーションしてその状態を保存
	phScene->ClearForce();
	phScene->GenerateForce();
	phScene->IntegratePart1();
	#endif
	/// テストシミュレーションのために現在の剛体の状態を保存する
	phScene->GetConstraintEngine()->SetBSaveConstraints(true);
	states->SaveState(phScene);	

	/// テストシミュレーション実行
	for(int i = 0; i < NIASolids(); i++){
		if(!GetIASolid(i)->bSim) continue;
		FWInteractSolid* inSolid = GetIASolid(i);
		PHSolid* phSolid = GetIASolid(i)->sceneSolid;
		/// 現在の速度を保存
		SpatialVector curvel, nextvel; 
		curvel.v() = phSolid->GetVelocity();			// 現在の速度
		curvel.w() = phSolid->GetAngularVelocity();		// 現在の角速度		

		//DSTR<<" 力を加えないで1ステップ進める--------------------"<<std::endl;
		/// 何も力を加えないでシミュレーションを1ステップ進める
		#ifdef DIVIDE_STEP
		phScene->IntegratePart2();
		#else
		phScene->Step();
		#endif
		nextvel.v() = phSolid->GetVelocity();
		nextvel.w() = phSolid->GetAngularVelocity();
		/// モビリティbの算出
		inSolid->lastb = inSolid->b;
		double pdt = phScene->GetTimeStep();
		inSolid->b = (nextvel - curvel) / pdt;
		states->LoadState(phScene);						// 現在の状態に戻す

		/// InteractPointerの数だけ力を加えるテストシミュレーションを行う
		for(int j = 0; j < NIAPointers(); j++){
			FWInteractPointer* iPointer = GetIAPointer(j);
			if(iPointer->interactInfo[i].flag.blocal==false)continue;
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
			#ifdef FORCE_ELEMENT
			for(int i = 0; i < 3; i++){
				if(iInfo->neighborInfo.test_force[i] < minTestForce)
					iInfo->neighborInfo.test_force[i] = minTestForce;
			}
			#endif
			TMatrixRow<6, 3, double> u;			// 剛体の機械インピーダンス
			TMatrixRow<3, 3, double> force;		// 加える力

			#ifdef FORCE_ELEMENT
			force.col(0) = Vec3d(iInfo->neighborInfo.test_force[0], 0, 0);
			force.col(1) = Vec3d(0, iInfo->neighborInfo.test_force[1], 0);
			force.col(2) = Vec3d(0, 0, iInfo->neighborInfo.test_force[2]);
			#else
			force.col(0) = iInfo->neighborInfo.test_force_norm * n;
			force.col(1) = iInfo->neighborInfo.test_force_norm * (n + t[0]);
			force.col(2) = iInfo->neighborInfo.test_force_norm * (n + t[1]);
			#endif
			//DSTR<<force<<std::endl;

			//DSTR<<" 法線--------------------"<<std::endl;
			/// 法線方向に力を加える
			phSolid->AddForce(force.col(0), cPoint); 
			#ifdef DIVIDE_STEP
			phScene->IntegratePart2();
			#else
			phScene->Step();
			#endif
			nextvel.v() = phSolid->GetVelocity();
			nextvel.w() = phSolid->GetAngularVelocity();
			u.col(0) = (nextvel - curvel) /pdt - inSolid->b;
			states->LoadState(phScene);
			
			//DSTR<<" n + t[0]--------------------"<<std::endl;
			/// n + t[0]方向に力を加える
			phSolid->AddForce(force.col(1), cPoint);
			#ifdef DIVIDE_STEP
			phScene->IntegratePart2();
			#else
			phScene->Step();
			#endif
			nextvel.v() = phSolid->GetVelocity();
			nextvel.w() = phSolid->GetAngularVelocity();
			u.col(1) = (nextvel - curvel) /pdt - inSolid->b;
			states->LoadState(phScene);

			//DSTR<<" n + t[1]--------------------"<<std::endl;
			/// n+t[1]方向力を加える
			phSolid->AddForce(force.col(2), cPoint);
			#ifdef DIVIDE_STEP
			phScene->IntegratePart2();
			#else
			phScene->Step();
			#endif
			nextvel.v() = phSolid->GetVelocity();
			nextvel.w() = phSolid->GetAngularVelocity();
			u.col(2) = (nextvel - curvel) /pdt - inSolid->b;
			
			iInfo->mobility.A = u  * force.inv();			// モビリティAの計算
//			DSTR <<  iInfo->mobility.A << std::endl;
			states->LoadState(phScene);						// 元のstateに戻しシミュレーションを進める
		}
	}
	///--------テストシミュレーション終了--------
	#ifdef DIVIDE_STEP
		states2->LoadState(phScene);							// 元のstateに戻しシミュレーションを進める
	#endif
}

void FWLDHaptic::BeginKeyboard(){
	PHSceneIf* phScene = GetIAScene()->GetScene()->GetPHScene();
	states->ReleaseState(phScene);
	states2->ReleaseState(phScene);
}
void FWLDHaptic::EndKeyboard(){
	PHSceneIf* phScene = GetIAScene()->GetScene()->GetPHScene();
	for(int i = 0; i < NIAPointers(); i++){
		PHSolidIf* pointer = GetIAPointer(i)->pointerSolid;
		phScene->SetContactMode(pointer,PHSceneDesc::MODE_NONE);
	}
}

void FWLDHaptic::ReleaseState(PHSceneIf* p){
	states->ReleaseState(p);
	states2->ReleaseState(p);
}

Vec3d* FWLDHaptic::GetProxyPoint(){
	return LDHapticLoop.GetProxyPoint();
}

Vec3d* FWLDHaptic::GetForce(){
	return LDHapticLoop.GetForce();
}

bool FWLDHaptic::GetContactFlag(){
	return LDHapticLoop.GetContactFlag();
}

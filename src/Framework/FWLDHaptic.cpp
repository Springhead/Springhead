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
	proK = 10;
	proD = 0.001;
	proM = 7.5*10e-6;
	bPic = false;
	picNum = 0;
	for(int i=0;i<2;i++){
//		DisplayProxy[i] = Vec3d(5,0,0);
		DisplayForce[i] = Vec3d(0,0,0);
	}
}
void FWLDHapticLoop::Step(){
	UpdateInterface();
	switch(hmode){
		case PENALTY3D:
			HapticRendering3D();
			LocalDynamics();
			break;
		case PENALTY:
			HapticRendering();
			LocalDynamics();
			break;
		case PENALTY6D:
			HapticRendering6D();
			LocalDynamics();
			//LocalDynamics6D();
			break;
		case CONSTRAINT:
			ConstraintBasedRendering();
			//LocalDynamics();
			LocalDynamics6D();
			break;
		case PROXY:
			Proxy();
			LocalDynamics();
			break;
		case PROXYSIMULATION:
			ProxySimulation();
			LocalDynamics();
			break;
		default:
			Proxy();
			LocalDynamics();
			break;
	}
}

void FWLDHapticLoop::UpdateInterface(){
	int N = NIAPointers();
	for(int i = 0; i < N; i++){
		FWInteractPointer* iPointer = GetIAPointer(i)->Cast();
		double s = iPointer->GetWorldScale() * iPointer->GetPosScale();
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
			hiSolid->SetPose(GetIAPointer(i)->GetDefaultPosition() * hifPose);
		}else{
			//3自由度インタフェースの場合
			HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
			hif->Update((float)hdt);
			PHSolid* hiSolid = &iPointer->hiSolid;
			hiSolid->SetVelocity((Vec3d)hif->GetVelocity() * s);
			Posed hifPose;
			hifPose.Pos()=(Vec3d)hif->GetPosition() * s;
			hiSolid->SetPose(GetIAPointer(i)->GetDefaultPosition() * hifPose);
		}
	}
}

void FWLDHapticLoop::HapticRendering3D(){
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
				Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;

				/// 抗力の計算
				double K = iPointer->correctionSpringK;
				double D = iPointer->correctionDamperD;
				Vec3d addforce = -1 * (K * ortho + D * dvortho);
				Vec3d addtorque = (pPoint - cSolid->GetCenterPosition()) % addforce ;

				double ws4 = pow(iPointer->GetWorldScale(), 4);
				outForce.v() += addforce / ws4;	

				DisplayForce[j] = outForce.v();

				/// 計算した力を剛体に加える//
				iPointer->interactInfo[i].mobility.force = -1 * addforce;	
				nInfo->test_force_norm = addforce.norm();
				nInfo->test_force = addforce;
			}
		}
		/// インタフェースへ力を出力
		SetRenderedForce(iPointer->GetHI(), iPointer->bForce, outForce * iPointer->GetForceScale());
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
		PicVibration(NIASolids());

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
				Vec3d pVibForce = Vec3d(0,0,0);
				Vec3d ortho = f * interpolation_normal;			// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
				Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;

				/// 抗力の計算
				double K = iPointer->correctionSpringK;
				double D = iPointer->correctionDamperD;
				Vec3d addforce = -1 * (K * ortho + D * dvortho);
				Vec3d addtorque = (pPoint - cSolid->GetCenterPosition()) % addforce ;

				///振動の計算
				if(iPointer->bVibration){
					pVibForce = Vibration(iSolid, iPointer, i);
					if(bPic){ pVibForce += oVibForce[i]; }
				}

				outForce.v() += addforce + pVibForce;	
				//outForce.w() += addtorque;

				DisplayForce[j] = outForce.v();

				/// 計算した力を剛体に加える//
				iPointer->interactInfo[i].mobility.force = -1 * addforce;	
				nInfo->test_force_norm = addforce.norm();
				nInfo->test_force = addforce;
				//if(iPointer->bForce)	DSTR << vibforce << endl;
			}else{
				iPointer->bContact[i] = false;
				contactFlag[j][i] = false;
			}
		}
		/// インタフェースへ力を出力
		SetRenderedForce(iPointer->GetHI(), iPointer->bForce, outForce * iPointer->GetForceScale());
	}
	for(int k=0;k<NIASolids();k++){
		if(contactFlag[0][k]&&contactFlag[1][k]){
			picNum = k;
			bPic = true;
		}
	}
}

void FWLDHapticLoop::HapticRendering6D(){
	for(int j = 0; j < NIAPointers(); j++){
		FWInteractPointer* iPointer = GetIAPointer(j)->Cast();
		SpatialVector outForce = SpatialVector();

		for(int i = 0; i < NIASolids(); i++){
			FWInteractSolid* iSolid = GetIASolid(i);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			if(!iInfo->flag.blocal) continue;
			NeighborInfo* nInfo = &iInfo->neighborInfo;
			PHSolid* cSolid = &iSolid->copiedSolid;

			/// 剛体の面の法線補間
			double syncCount = pdt / hdt;						// プロセスの刻み時間の比
			// 提示力計算にしようする法線（前回の法線との間を補間する）
			Vec3d interpolation_normal = (loopCount * nInfo->face_normal + 
				(syncCount - (double)loopCount) * nInfo->last_face_normal) / syncCount;															
			// カウンタが規定の同期カウントを上回る場合は現在の法線にする
			if(loopCount > syncCount)	interpolation_normal = nInfo->face_normal;
			
			std::vector < Vec3d > psection = nInfo->pointer_section;
			std::vector < Vec3d > ssection = nInfo->solid_section;
			int pSecSize = (int)psection.size();
			for(int k = 0; k < pSecSize; k++){
				Vec3d pPoint = iPointer->hiSolid.GetPose() * psection[k];	// 力覚ポインタの接触点(ワールド座標)
				Vec3d cPoint = cSolid->GetPose() * ssection[k];				// 剛体の接触点(ワールド座標)
				Vec3d force_dir = pPoint - cPoint;
				// 剛体の接触点から力覚ポインタの接触点へのベクトル(ワールド系)
				double f = force_dir * interpolation_normal;				// 剛体の面の法線と内積をとる
				if(f < 0.0){												// 内積が負なら力を計算
					Vec3d ortho = f * interpolation_normal;					// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
					Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
					Vec3d dvortho = dv.norm() * interpolation_normal;

					/// 抗力の計算
					double K = iPointer->correctionSpringK / psection.size();
					double D = iPointer->correctionDamperD / psection.size();
					double s = iPointer->GetWorldScale() * iPointer->GetPosScale();
					Vec3d addforce = -1 * (K * ortho + D * dvortho);
					Vec3d addtorque = (pPoint - cSolid->GetCenterPosition()) % addforce / s;

					outForce.v() += addforce;	
					outForce.w() += addtorque;

					/// 計算した力を剛体に加える//
					iPointer->interactInfo[i].mobility.force = -1 * addforce;	
					nInfo->test_force_norm = addforce.norm();
					nInfo->test_force = addforce;
				}
			}
		}
		/// インタフェースへ力を出力
		SetRenderedForce(iPointer->GetHI(), iPointer->bForce, outForce * iPointer->GetForceScale());
	}
}

// ガウスザイデル法を使いAx+b>0を解く
template <class AD, class XD, class BD>
void GaussSeidel(MatrixImp<AD>& a, VectorImp<XD>& x, const VectorImp<BD>& b){
	int nIter = 20;					// 反復回数の上限
	double errorRange = 10e-8;		// 許容誤差
	int n = (int)a.height();		// 連立方程式の数(行列aの行数)
	std::vector< double > lastx;
	for(int i = 0; i < n; i++){
		lastx.push_back(x[i]);
		x[i] = 0;
	}

	for(int k = 0; k < nIter; k++){		
		for(int i = 0; i < n; i++){
			double term1 = 0.0;
			double term2 = 0.0;
			for(int j = 0; j < i; j++){
				term1 += a[i][j] * x[j];
			}
			for(int j = i+1; j < n; j++){
				term2 += a[i][j] * lastx[j];
			}
			// xの更新(繰り返し計算の式を使用)
			x[i] =  (-b[i] - term1 - term2) / a[i][i];
			if(x[i] < 0) x[i] = 0.0;
		}

		// (lastx - x)の2乗の総和と誤差範囲を比較
		double error = 0.0;
		for(int i = 0; i < n; i++){
			error += pow(x[i] - lastx[i], 2);
		}
		if(error < errorRange){
			//DSTR <<　"Finish the iteration in admissible error. " << std::endl;
			return;
		}

		// 繰り返し計算のために更新後のxをlastxに保存
		for(int i = 0; i < n; i++) lastx[i] = x[i];
	}
	// nIterで計算が終わらなかったので打ち切り
	//DSTR << "Could not convergent the variable. << std::endl; 
	//DTR << "Abort the iteration." << std::endl;
}

struct SolidVertex{
	int iaSolidID;
	Vec3d normal;
	Vec3d r;
	double d;
	Vec3d force;
};
typedef std::vector< SolidVertex > SolidVertices;

//#define SIMPLEX
#ifdef SIMPLEX 
#include "C:\Project\Experiments\HapticInteraction\FWLDHapticTest\Simplex.h"
#endif
void FWLDHapticLoop::ConstraintBasedRendering(){
	for(int j = 0; j < NIAPointers(); j++){
		FWInteractPointer* iPointer = GetIAPointer(j)->Cast();
		SpatialVector outForce = SpatialVector();
		SolidVertices sv;
		for(int i = 0; i < NIASolids(); i++){
			FWInteractSolid* iSolid = GetIASolid(i);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			iInfo->mobility.f.clear();
			if(!iInfo->flag.blocal) continue;
			NeighborInfo* nInfo = &iInfo->neighborInfo;
			PHSolid* cSolid = &iSolid->copiedSolid;

			/// 剛体の面の法線補間
			double syncCount = pdt / hdt;									// プロセスの刻み時間の比
			// 提示力計算にしようする法線（前回の法線との間を補間する）
			Vec3d interpolation_normal = (loopCount * nInfo->face_normal + 
				(syncCount - (double)loopCount) * nInfo->last_face_normal) / syncCount;															
			// カウンタが規定の同期カウントを上回る場合は現在の法線にする
			if(loopCount > syncCount)	interpolation_normal = nInfo->face_normal;			
#if 0
			std::vector < Vec3d > psection = nInfo->pointer_section;
			std::vector < Vec3d > ssection = nInfo->solid_section;
			double ptemp;
			double ctemp;
			for(size_t k = 0; k < psection.size(); k++){
				Vec3d pPoint = iPointer->hiSolid.GetPose() * psection[k];	// 力覚ポインタの接触点(ワールド座標)
				Vec3d cPoint = cSolid->GetPose() * ssection[k];				// 剛体の接触点(ワールド座標)
				ptemp = pPoint.y;
				ctemp = cPoint.y;
				Vec3d force_dir =  pPoint - cPoint;							// 剛体の接触点から力覚ポインタの接触点へのベクトル(ワールド系)
				double f = force_dir * interpolation_normal;				// 剛体の面の法線と内積をとる
				if(f < 0.0){												// 内積が負なら力を計算
					Vec3d ortho = f * interpolation_normal;					// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
					sv.resize(sv.size() + 1);
					sv.back().iaSolidID = i;
					sv.back().normal = interpolation_normal;
					sv.back().r = pPoint - iPointer->hiSolid.GetCenterPosition();
					sv.back().d = ortho.norm();
				}
			}
#else
			nInfo->solid_section.clear();
			std::vector < Vec3d > ivs = nInfo->intersection_vertices;
			Vec3d pPoint = iPointer->hiSolid.GetPose() * nInfo->pointer_point;	// 力覚ポインタの接触点(ワールド座標)
			Vec3d cPoint = cSolid->GetPose() * nInfo->closest_point;			// 剛体の接触点(ワールド座標)
			Vec3d force_dir =  pPoint - cPoint;									// 剛体の接触点から力覚ポインタの接触点へのベクトル(ワールド系)
			double f = force_dir * interpolation_normal;						// 剛体の面の法線と内積をとる
			//DSTR << "--------------------" << std::endl;
			if(f < 0.0){														// 内積が負なら力を計算
				// 接触点毎に接触点から補間平面までの距離を計算
				for(size_t k = 0; k < ivs.size(); k++){
					Vec3d wivs = iPointer->hiSolid.GetPose() * ivs[k];
					double dot = (wivs - cPoint) * interpolation_normal;
					if(dot >= 0)	continue;
					Vec3d ortho = dot * interpolation_normal;		// 剛体の近傍点から接触点までのベクトルを面法線へ射影
					sv.resize(sv.size() + 1);
					sv.back().iaSolidID = i;
					sv.back().normal = interpolation_normal;
					sv.back().r = wivs - iPointer->hiSolid.GetCenterPosition();
					sv.back().d = ortho.norm();
					Vec3d onPlane = wivs - ortho;
					nInfo->solid_section.push_back(cSolid->GetPose().Inv() * onPlane);
				}
			}
	
#endif
		}
		/// 連立不等式を計算するための行列を作成
		if(sv.size() > 0){
			int l = sv.size();	// 接触点の数
			VMatrixRow<double> a;
			a.resize(l, l);
			VVector<double> d;
			d.resize(l);
			VVector<double> fnorm;
			fnorm.resize(l+1);
			for(int m = 0; m <= l; m++){
				if(m == 0)	fnorm[m] = 0.0;
				else		fnorm[m] = 1.0;
			}
			for(int m = 0; m < l; m++){
				for(int n = 0; n < l; n++){
					Vec3d rm = sv[m].r;
					Vec3d nm = sv[m].normal;
					Vec3d rn = sv[n].r; 
					Vec3d nn = sv[n].normal;
					a[m][n] = (rm % nm) * iPointer->hiSolid.GetInertia().inv() * (rn % nn) + nm * nn / iPointer->hiSolid.GetMass();
				}
				d[m] = sv[m].d;
			}
#ifdef SIMPLEX
			Inequality ineq;
			ineq.resize(l);
			for(int m = 0; m < l; m++){
				ineq[m] = '>';
			}
			Simplex simplex;
			simplex.SetTargetFunction(fnorm);
			simplex.SetLinearInequality(a, d, ineq);
			VVector<double> f = simplex.Start();
			double ftemp = 0;
#else
			VVector<double> f = VVector<double>();
			f.resize(l);
			GaussSeidel(a,f,-d);
#endif
			// ポインタと剛体に拘束を与える
			Vec3d dr = Vec3d();
			Vec3d dtheta = Vec3d();
			for(int m = 0; m < l; m++){
				if(f[m] < 0) f[m] = 0.0;
				// ポインタへの拘束
				dr += f[m] * sv[m].normal / iPointer->hiSolid.GetMass();							// 位置拘束
				dtheta += f[m] * iPointer->hiSolid.GetInertia().inv() * (sv[m].r % sv[m].normal);	// 回転拘束
			}

			/// 力覚インタフェースに出力する力の計算
			double ws4 = pow(iPointer->GetWorldScale(), 4);

			outForce.v() = (iPointer->correctionSpringK * dr - iPointer->correctionDamperD * (dr/hdt)) /ws4;
			outForce.w() = (iPointer->correctionSpringK * dtheta / 20) / ws4;
			//CSVOUT << l << "," << sv[0].d << "," << outForce.v().y << endl;
			//CSVOUT << outForce.v().x << "," << outForce.v().y << "," << outForce.v().z << "," << outForce.w().x << "," << outForce.w().y << "," << outForce.w().z << endl;

			/// 剛体へ力を加える
			for(int m = 0; m < l; m++){
				Vec3d addforce = -1 * f[m] * sv[m].normal * iPointer->correctionSpringK;
				iPointer->interactInfo[sv[m].iaSolidID].mobility.f.push_back(addforce);	

			}
		}
		/// インタフェースへ力を出力
		SetRenderedForce(iPointer->GetHI(), iPointer->bForce, outForce * iPointer->GetForceScale());
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

void FWLDHapticLoop::LocalDynamics6D(){
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
			SpatialVector f;
			for(int k = 0; k < iInfo->mobility.f.size(); k++){
				f.v() += iInfo->mobility.f[k];
				Vec3d r = (iSolid->copiedSolid.GetPose() * iInfo->neighborInfo.solid_section[k]) - iSolid->copiedSolid.GetCenterPosition(); 
				f.w() += r % iInfo->mobility.f[k];
			}
			vel += (iInfo->mobility.Minv * f) * hdt;			// 力覚ポインタからの力による速度変化
			iInfo->neighborInfo.test_force_norm = f.v().norm();
			iInfo->neighborInfo.test_torque_norm = f.w().norm();
			iInfo->neighborInfo.test_force = f.v();
			iInfo->neighborInfo.test_torque = f.w();
			//DSTR << f.v().norm() << std::endl;
			iInfo->mobility.f.clear();
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
void FWLDHapticLoop::PicVibration(int nSolids){
	PHSceneIf* phScene = GetIAAdaptee()->GetPHScene();
	
	int sceneCnt = phScene->GetCount();
	static std::vector<Vec3d> vel;
	if((int)vel.size() < nSolids){
		for(int i=(int)vel.size(); i<nSolids; i++){
			vel.push_back(Vec3d(0,0,0));
		}
	}

	PHSolidPairForLCPIf* solidPair;
	PHShapePairForLCPIf* shapePair;
	for(int i=0; i<nSolids; i++){ oVibForce[i] = Vec3d(0,0,0); }

	for (int i=0; i<nSolids; ++i) {
		for (int j=i+1; j<nSolids; ++j) {
			PHSolidIf *so1, *so2;
			so1 = phScene->GetSolids()[i];
			so2 = phScene->GetSolids()[j];

			solidPair = phScene->GetSolidPair(i, j);
			//PHConstraintIf* constraint = phScene->GetConstraintEngine()->GetContactPoints()->FindBySolidPair(so1, so2);

			if (!solidPair)  { continue; }
			//if (!constraint) { continue; }

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
				oVibForce.push_back(Vec3d(0,0,0));
			}
		}
		if(DCAST(HIForceInterface6DIf, iPointer->GetHI())){
			HIForceInterface6DIf* hif = iPointer->GetHI()->Cast();
		}else{
			HIForceInterface3DIf* hif = iPointer->GetHI()->Cast();
		}

		SpatialVector outForce = SpatialVector();
		PicVibration(NIASolids());

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
				Vec3d pVibForce = Vec3d(0,0,0);
				Vec3d ortho = f * interpolation_normal;			// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
				Vec3d dv =  iPointer->hiSolid.GetPointVelocity(pPoint) - cSolid->GetPointVelocity(cPoint);
				Vec3d dvortho = dv.norm() * interpolation_normal;

				/// 抗力の計算
				double K = iPointer->correctionSpringK;
				double D = iPointer->correctionDamperD;
				Vec3d addforce = (-K * (pPoint - (poseSolid * proxy[j][i])) - D * dvortho);
				Vec3d addtorque = (pPoint - cSolid->GetCenterPosition()) % addforce ;

				///振動の計算
				if(iPointer->bVibration){
					pVibForce = Vibration(iSolid, iPointer, i);
					if(bPic)	pVibForce += oVibForce[i];		// 物体を把持していたらその剛体の振動を手に伝える
				}


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

				double ws = iPointer->GetWorldScale();
				double ws4 = ws * ws * ws * ws;
				outForce.v() += addforce/ws4 + pVibForce;	
				outForce.w() += addtorque;

				DisplayForce[j] = outForce.v();

				/// 計算した力を剛体に加える
				iPointer->interactInfo[i].mobility.force = -1 * addforce;						
				nInfo->test_force_norm = addforce.norm();
				nInfo->test_force = addforce;

			}else{
				iPointer->bContact[i] = false;
				contactFlag[j][i] = false;
				proxy[j][i] = poseSolid.Inv() * pPoint;
			}
//			DisplayProxy[0] = poseSolid * proxy[0][i];
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
		if((*contactFlag).size()>0){
			if(contactFlag[0][k]&&contactFlag[1][k]){
				picNum = k;
				bPic = true;
			}
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
				double K = iPointer->correctionSpringK;
				double D = iPointer->correctionDamperD;
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
//			DisplayProxy[0] = poseSolid * proxy[0][i];
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
		if((*contactFlag).size()>0){
			if(contactFlag[0][k]&&contactFlag[1][k]){
				picNum = k;
				bPic = true;
			}
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

Vec3d FWLDHapticLoop::GetPicCenterPosition(){
	return GetIASolid(picNum)->copiedSolid.GetCenterPosition();
}
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
#if 0
		TestSimulation6D();
#else
		TestSimulation();
#endif
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
			if(iPointer->interactInfo[i].flag.blocal==false) continue;
			PHSolid* soPointer = iPointer->pointerSolid->Cast();
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			Vec3d cPoint = phSolid->GetPose() * iInfo->neighborInfo.closest_point;		// 力を加える点(ワールド座標)

			Vec3d n = -iInfo->neighborInfo.face_normal;
			Matrix3d local = CalcConstraintCoordinateSystem(soPointer, phSolid, 
				iInfo->neighborInfo.pointer_point, iInfo->neighborInfo.closest_point, n);

			/// 接触点に加える力
			float minTestForce = 0.5;		// 最小テスト力
			/// 通常テスト力が最小テスト力を下回る場合
			if(iInfo->neighborInfo.test_force_norm < minTestForce){
				iInfo->neighborInfo.test_force_norm = minTestForce;					 
			}

			TMatrixRow<6, 3, double> u;			// 剛体の機械インピーダンス
			TMatrixRow<3, 3, double> force;		// 加える力
#define NORMAL
			#ifdef NORMAL
			force.col(0) = iInfo->neighborInfo.test_force_norm * n;
			force.col(1) = iInfo->neighborInfo.test_force_norm * (n + local.col(1));
			force.col(2) = iInfo->neighborInfo.test_force_norm * (n + local.col(2));
            #else
			force.col(0) = iInfo->neighborInfo.test_force;
			Vec3d base1 = iInfo->neighborInfo.test_force.unit();
			Vec3d base2 = Vec3d(1,0,0) - (Vec3d(1,0,0)*base1)*base1;
			if (base2.norm() > 0.1){
				base2.unitize();
			}else{
				base2 = Vec3d(0,1,0) - (Vec3d(0,1,0)*base1)*base1;
				base2.unitize();
			}
			Vec3d base3 = base1^base2;
			force.col(1) = iInfo->neighborInfo.test_force.norm() * (base1 + base2);
			force.col(2) = iInfo->neighborInfo.test_force.norm() * (base1 + base3);
            #endif

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
			
			for(int k = 0; k < 3; k++){
				phSolid->AddForce(force.col(k), cPoint);
				#ifdef DIVIDE_STEP
				phScene->IntegratePart2();
				#else
				phScene->Step();
				#endif
				nextvel.v() = phSolid->GetVelocity();
				nextvel.w() = phSolid->GetAngularVelocity();
				u.col(k) = (nextvel - curvel) /pdt - inSolid->b;
				states->LoadState(phScene);			
			}

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

void FWLDHaptic::TestSimulation6D(){
	/* 6DoF Haptic Rendering のための多点接触テストシミュレーション*/
	/*	力覚ポインタが剛体に加える力の点が複数
		点はFWInteractAdaptee::FindSectionVertex()で取得し，
		vector< Vec3d > FWInteractInfo::neighborInfo::sectionに保存されている
	*/
	
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
			if(iPointer->interactInfo[i].flag.blocal==false) continue;
			PHSolid* soPointer = iPointer->pointerSolid->Cast();
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			Vec3d cPoint = phSolid->GetPose() * iInfo->neighborInfo.closest_point;		// 力を加える点(ワールド座標)
			
			/// 拘束座標系の作成
			Vec3d n = -iInfo->neighborInfo.face_normal;
			Matrix3d local = CalcConstraintCoordinateSystem(soPointer, phSolid, 
				iInfo->neighborInfo.pointer_point, iInfo->neighborInfo.closest_point, n);

			/// 接触点に加える力
			const float minTestForce = 0.5;										// 最小テスト力
			/// 通常テスト力が最小テスト力を下回る場合
			if(iInfo->neighborInfo.test_force_norm < minTestForce)		iInfo->neighborInfo.test_force_norm = minTestForce;
			if(iInfo->neighborInfo.test_torque_norm < minTestForce)		iInfo->neighborInfo.test_torque_norm = minTestForce;					 
			
			TMatrixRow<6,6,double> u;		// 剛体の機械インピーダンス
			TMatrixRow<6,6,double> f;		// 加える力,トルク
			u.resize(6,6);
			f.resize(6,6);
			for(int k = 0; k<6; k++){
				for(int l = 0; l<6; l++){
					u[k][l] = 0;
					f[k][l] = 0;
				}
			}
			
			SpatialVector force[6];
			for(int k = 0; k < 5; k++){
				force[k] = SpatialVector();
			}
			force[0].v() = iInfo->neighborInfo.test_force_norm * n;
			force[1].v() = iInfo->neighborInfo.test_force_norm * (n + local.col(1));
			force[2].v() = iInfo->neighborInfo.test_force_norm * (n + local.col(2));
			force[3].w() = iInfo->neighborInfo.test_torque_norm * n;
			force[4].w() = iInfo->neighborInfo.test_torque_norm * (n + local.col(1));
			force[5].w() = iInfo->neighborInfo.test_torque_norm * (n + local.col(2));

			for(int k = 0; k < 5; k++){
				f.col(k) = force[k];
			}

			/// 力を加える
			for(int k = 0; k < 3; k++){
				phSolid->AddForce((Vec3d)force[k].v()); 
				#ifdef DIVIDE_STEP
				phScene->IntegratePart2();
				#else
				phScene->Step();
				#endif
				nextvel.v() = phSolid->GetVelocity();
				nextvel.w() = phSolid->GetAngularVelocity();
				u.col(k) = (nextvel - curvel) /pdt - inSolid->b;
				states->LoadState(phScene);
			}
			/// トルクを加える
			for(int k = 3; k < 6; k++){
				phSolid->AddTorque((Vec3d)force[k].w()); 
				#ifdef DIVIDE_STEP
				phScene->IntegratePart2();
				#else
				phScene->Step();
				#endif
				nextvel.v() = phSolid->GetVelocity();
				nextvel.w() = phSolid->GetAngularVelocity();
				u.col(k) = (nextvel - curvel) /pdt - inSolid->b;
				states->LoadState(phScene);
			}
			
			iInfo->mobility.Minv = u  * f.inv() * -1;			// モビリティAの計算
		}
	}
	///--------テストシミュレーション終了--------
	#ifdef DIVIDE_STEP
		states2->LoadState(phScene);							// 元のstateに戻しシミュレーションを進める
	#endif
}

Matrix3d FWLDHaptic::CalcConstraintCoordinateSystem(PHSolid* pha, PHSolid* phb, Vec3d pa, Vec3d pb, Vec3d normal){
			/// 拘束座標系を作るための準備
			Vec3d rajabs, vajabs;
			rajabs = pha->GetPose() * pa - pha->GetCenterPosition();				//剛体の中心から接触点までのベクトル
			vajabs = pha->GetVelocity() + pha->GetAngularVelocity() % rajabs;		//接触点での速度
			Vec3d rbjabs, vbjabs;
			rbjabs = phb->GetPose() * pb - phb->GetCenterPosition();				//力覚ポインタの中心から接触点までのベクトル
			vbjabs = phb->GetVelocity() + phb->GetAngularVelocity() % rbjabs;		//接触点での速度

			/// 拘束座標変換行列の計算
			Vec3d n, t[2], vjrel, vjrelproj;						//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
			n = normal;
			vjrel = vbjabs - vajabs;								// 相対速度(剛体a->剛体b)
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
			Matrix3d local;
			local.col(0) = n;
			local.col(1) = t[0];
			local.col(2) = t[1];
			return local;

			/* 改良版（実装中）			
			/// 拘束座標系の作成
			Vec3d v0 = phSolid->GetPointVelocity(cPoint);
			Vec3d v1 = soPointer->GetPointVelocity(pPoint);
			Matrix3d local;	//	contact coodinate system 接触の座標系
			Vec3d normal = iInfo->neighborInfo.face_normal; 
			local.Ex() = normal;
			local.Ey() = v1-v0;
			local.Ey() -= local.Ey() * normal * normal;
			if (local.Ey().square() > 1e-6){
				local.Ey().unitize(); 
			}else{
				if (Square(normal.x) < 0.5) local.Ey()= (normal ^ Vec3f(1,0,0)).unit();
				else local.Ey() = (normal ^ Vec3f(0,1,0)).unit();
			}
			local.Ez() =  local.Ex() ^ local.Ey();
			*/
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

Vec3d FWLDHaptic::GetPicCenterPosition(){
	return LDHapticLoop.GetPicCenterPosition();
}

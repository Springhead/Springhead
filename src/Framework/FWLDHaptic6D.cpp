/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWLDHaptic6D.h>
#include <Framework/SprFWApp.h>


// FWLDHaptic6DLoopの実装
//////////////////////////////////////////////////////////////////////////////////////////////
FWLDHaptic6DLoop::FWLDHaptic6DLoop(){}

void FWLDHaptic6DLoop::Step(){
	UpdateInterface();
	ConstraintBasedRendering();
	LocalDynamics6D();
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
	static int iterError = 0;
	iterError += 1;
	DSTR << iterError << "Could not converge in iteration steps." << std::endl; 
}

struct SolidVertex{
	int iaSolidID;
	Vec3d normal;
	Vec3d r;
	double d;
	Vec3d force;
};
typedef std::vector< SolidVertex > SolidVertices;


std::vector<string> saveDeviceForce;
std::vector<string> saveDevicePosition;
std::vector<string> saveProxyPosition;


//#define SIMPLEX
#ifdef SIMPLEX 
#include "C:\Projects\Experiments\HapticInteraction\FWLDHapticTest\Simplex.h"
#endif
void FWLDHaptic6DLoop::ConstraintBasedRendering(){
	for(int j = 0; j < NIAPointers(); j++){
		FWInteractPointer* iPointer = GetIAPointer(j)->Cast();
		SpatialVector outForce = SpatialVector();
		SolidVertices sv;
		for(int i = 0; i < NIASolids(); i++){
			FWInteractSolid* iSolid = GetIASolid(i);
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			iInfo->mobility.f.clear();
			if(!iInfo->flag.blocal) continue;
			ToHaptic* th = &iInfo->toHaptic;
			PHSolid* cSolid = &iSolid->copiedSolid;

			th->solid_section.clear();
			Vec3d pPoint = iPointer->hiSolid.GetPose() * th->pointer_point;	// 力覚ポインタの接触点(ワールド座標)
			Vec3d cPoint = cSolid->GetPose() * th->closest_point;			// 剛体の接触点(ワールド座標)


			/// 剛体の面の法線補間
			double syncCount = pdt / hdt;									// プロセスの刻み時間の比
			// 提示力計算にしようする法線（前回の法線との間を補間する）
			Vec3d interpolation_normal = (loopCount * th->face_normal + 
				(syncCount - (double)loopCount) * th->last_face_normal) / syncCount;															
			// カウンタが規定の同期カウントを上回る場合は現在の法線にする
			if(loopCount > syncCount){
				interpolation_normal = th->face_normal;			
			}

			Vec3d force_dir =  pPoint - cPoint;								// 剛体の接触点から力覚ポインタの接触点へのベクトル(ワールド系)
			double f = force_dir * interpolation_normal;					// 剛体の面の法線と内積をとる
		
			std::vector < Vec3d > ivs = th->intersection_vertices;

			if(f < 0.0){													// 内積が負なら力を計算
#if 0
				// 接触点毎に接触点から補間平面までの距離を計算
				double max_dot = 0;
				int temp = 100;
				// 最侵入点を見つける
				for(size_t k = 0; k < ivs.size(); k++){
					Vec3d wivs = iPointer->hiSolid.GetPose() * ivs[k];
					Vec3d h = pPoint - wivs; 
					if(h.norm() > 0.01) continue;
					max_dot = (wivs - cPoint) * interpolation_normal;	// 剛体の点から中間面上の点へのベクトルのノルム
					Vec3d ortho = max_dot * interpolation_normal;		// 剛体の近傍点から接触点までのベクトルを面法線へ射影
					sv.resize(sv.size() + 1);
					sv.back().iaSolidID = i;
					sv.back().normal = interpolation_normal;
					sv.back().r = wivs - iPointer->hiSolid.GetCenterPosition();
					sv.back().d = ortho.norm();
					Vec3d onPlane = wivs - ortho;
					th->solid_section.push_back(cSolid->GetPose().Inv() * onPlane);
					temp = k;
				}

				// それ以外
				for(size_t k = 0; k < ivs.size(); k++){
					if(k == temp) continue;	// 最侵入点なのでスキップ
					Vec3d wivs = iPointer->hiSolid.GetPose() * ivs[k];
					double dot = (wivs - cPoint) * interpolation_normal;	// 剛体の点から中間面上の点へのベクトルのノルム
					if(abs(dot - max_dot) != 0)	continue;
					Vec3d ortho = dot * interpolation_normal;		// 剛体の近傍点から接触点までのベクトルを面法線へ射影
					sv.resize(sv.size() + 1);
					sv.back().iaSolidID = i;
					sv.back().normal = interpolation_normal;
					sv.back().r = wivs - iPointer->hiSolid.GetCenterPosition();
					sv.back().d = ortho.norm();
					Vec3d onPlane = wivs - ortho;
					th->solid_section.push_back(cSolid->GetPose().Inv() * onPlane);
				}
#else
				for(size_t k = 0; k < ivs.size(); k++){
					Vec3d wivs = iPointer->hiSolid.GetPose() * ivs[k];
					double dot = (wivs - cPoint) * interpolation_normal;	// 剛体の点から中間面上の点へのベクトルのノルム
					if(dot > 0.0)	continue;
					Vec3d ortho = dot * interpolation_normal;		// 剛体の近傍点から接触点までのベクトルを面法線へ射影
					sv.resize(sv.size() + 1);
					sv.back().iaSolidID = i;
					sv.back().normal = interpolation_normal;
					sv.back().r = wivs - iPointer->hiSolid.GetCenterPosition();
					sv.back().d = ortho.norm();
					Vec3d onPlane = wivs - ortho;
					th->solid_section.push_back(cSolid->GetPose().Inv() * onPlane);
				}
#endif
			}
	
		}
		Vec3d dr = Vec3d();
		Vec3d dtheta = Vec3d();
		/// 連立不等式を計算するための行列を作成
		iPointer->last_proxy_pose = iPointer->proxy_pose;
		int l = 0;
		if(sv.size() > 0){
			l = (int)sv.size();	// 接触点の数
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
					
					// 並進拘束
					double trans = nm * nn / iPointer->hiSolid.GetMass();
					// 回転拘束
					double rotate = (rm % nm) * iPointer->hiSolid.GetInertia().inv() * (rn % nn);
				
					a[m][n] = trans + rotate;
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
			dr = Vec3d();
			dtheta = Vec3d();
			for(int m = 0; m < l; m++){
				if(f[m] < 0) f[m] = 0.0;
				// ポインタへの拘束
				dr += f[m] * sv[m].normal / iPointer->hiSolid.GetMass();							// 位置拘束
				dtheta += f[m] * iPointer->hiSolid.GetInertia().inv() * (sv[m].r % sv[m].normal);	// 回転拘束
			}

			/// 力覚インタフェースに出力する力の計算
			double ws4 = pow(iPointer->GetWorldScale(), 4);
			outForce.v() = (iPointer->correctionSpringK * dr - iPointer->correctionDamperD * (dr/hdt)) /ws4;
			outForce.w() = (iPointer->correctionSpringK * dtheta) / ws4;


			/// 剛体へ力を加える
			for(int m = 0; m < l; m++){
				Vec3d addforce = -1 * f[m] * sv[m].normal * iPointer->correctionSpringK /ws4;
				iPointer->interactInfo[sv[m].iaSolidID].mobility.f.push_back(addforce);
			}
		}

		/// インタフェースへ力を出力
		SetRenderedForce(iPointer->GetHI(), iPointer->bForce, outForce * iPointer->GetForceScale());
		// プロキシの位置（デバイスの目標位置）
		Posed p = iPointer->hiSolid.GetPose();
		p.Pos() += dr;
		p.Ori() = ( Quaterniond::Rot(dtheta) * p.Ori()).unit();
		iPointer->proxy_pose =  p;

#if 1
		// デバイスへの力をセーブ
		std::stringstream str;
		str << outForce.v().x << "\t" << outForce.v().y << "\t" << outForce.v().z << "\t" << outForce.w().x << "\t" << outForce.w().y << "\t" << outForce.w().z;
		saveDeviceForce.push_back(str.str());
		str.str("");
		str.clear();

		// デバイス位置のセーブ
		Vec3d devpos = iPointer->hiSolid.GetCenterPosition();
		Vec3d devori, devtemp;
		iPointer->hiSolid.GetOrientation().ToEuler(devtemp);
		devori.x = Deg(devtemp.z);
		devori.y = Deg(devtemp.x);
		devori.z = Deg(devtemp.y);
		str << devpos.x << "\t" << devpos.y << "\t" << devpos.z << "\t" << devori.x << "\t" << devori.y << "\t" << devori.z; 
		saveDevicePosition.push_back(str.str());
		str.str("");
		str.clear();

		// プロキシ位置のセーブ
		Vec3d proxypos = iPointer->proxy_pose.Pos();
		Vec3d proxyori, proxytemp;
		iPointer->proxy_pose.Ori().ToEuler(proxytemp);
		proxyori.x = Deg(proxytemp.z);
		proxyori.y = Deg(proxytemp.x);
		proxyori.z = Deg(proxytemp.y);
		str << proxypos.x << "\t" << proxypos.y << "\t" << proxypos.z << "\t" << proxyori.x << "\t" << proxyori.y << "\t" << proxyori.z << "\t" << l; 
		saveProxyPosition.push_back(str.str());
		str.str("");
		str.clear();

#endif
	}
}


std::vector<string> saveSolidForce;
std::vector<string> saveSolidPosition;
void FWLDHaptic6DLoop::LocalDynamics6D(){
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
			SpatialVector f = SpatialVector();
			for(int k = 0; k < (int)iInfo->mobility.f.size(); k++){
				f.v() += iInfo->mobility.f[k];
				Vec3d r = (iSolid->copiedSolid.GetPose() * iInfo->toHaptic.solid_section[k]) - iSolid->copiedSolid.GetCenterPosition(); 
				f.w() += r % iInfo->mobility.f[k];
			}
			vel += (iInfo->mobility.Minv * f) * hdt;				// 力覚ポインタからの力による速度変化
			//DSTR << iInfo->mobility.Minv * f << std::endl;
			iInfo->toPhysic.test_force = f.v();					// テスト力の保存
			iInfo->toPhysic.test_torque = f.w();				// テストトルクの保存
			iInfo->toHaptic.solid_section.clear();
			iInfo->mobility.f.clear();
		}
		vel += iSolid->b * hdt;

		//solid　forceをセーブするための計算
		SpatialVector dvel = SpatialVector();
		dvel.v() = vel.v() - iSolid->copiedSolid.GetVelocity();
		dvel.w() = vel.w() - iSolid->copiedSolid.GetAngularVelocity();
		SpatialVector accel = dvel / hdt;		// 加速度
		SpatialVector sf = SpatialVector();	// 力
		sf.v() = iSolid->copiedSolid.GetMass() * accel.v() ;
		sf.w() = iSolid->copiedSolid.GetInertia() * accel.w();

		/// 状態の更新
		iSolid->copiedSolid.SetVelocity(vel.v());		
		iSolid->copiedSolid.SetAngularVelocity(vel.w());
		iSolid->copiedSolid.SetCenterPosition(iSolid->copiedSolid.GetCenterPosition() + vel.v() * hdt);
		iSolid->copiedSolid.SetOrientation(( Quaterniond::Rot(vel.w() * hdt) * iSolid->copiedSolid.GetOrientation()).unit());
		/// 更新完了のフラグを立てる
 		iSolid->copiedSolid.SetUpdated(true);
		iSolid->copiedSolid.Step();

#if 0
		if(i != 1) continue;
		//Solid Position, Forceのセーブ
		std::stringstream str;
		str << sf.v().x << "\t" << sf.v().y << "\t" << sf.v().z << "\t" << sf.w().x << "\t" << sf.w().y << "\t" << sf.w().z;
		saveSolidForce.push_back(str.str());
		str.str("");
		str.clear();

		//位置のファイルセーブ
		Vec3d sVel = iSolid->copiedSolid.GetVelocity();
		Vec3d sAVel = iSolid->copiedSolid.GetAngularVelocity();
		Vec3d pos = iSolid->copiedSolid.GetCenterPosition();
		Vec3d ori, temp;
		iSolid->copiedSolid.GetOrientation().ToEuler(temp);
		ori.x = Deg(temp.z);
		ori.y = Deg(temp.x);
		ori.z = Deg(temp.y);
		str << sVel.x << "\t" << sVel.y << "\t" << sVel.z << "\t" << sAVel.x << "\t" << sAVel.y << "\t" << sAVel.z << "\t";
		str << pos.x << "\t" << pos.y << "\t" << pos.z << "\t" << ori.x << "\t" << ori.y << "\t" << ori.z;
		//DSTR << str.str() << std::endl;
		saveSolidPosition.push_back(str.str());
#endif
	}
}



// FWLDHapticAppの実装
//////////////////////////////////////////////////////////////////////////////////////////////
FWLDHaptic6D::FWLDHaptic6D(){
	hapticLoop = &ldLoop6D;
}

void FWLDHaptic6D::Step(){
	if (bSync) return;
	if (bCalcPhys){
		UpdatePointer();
		PhysicsStep();
		UpdateSolidList();
		NeighborObjectFromPointer();
		TestSimulation6D();
		bCalcPhys = false;
	}
	double pdt = GetPHScene()->GetTimeStep();
	double hdt = GetIAScene()->hdt;
	if (hapticcount < pdt / hdt) return;
	hapticcount -= (int)(pdt/hdt);
	bSync = true;
	bCalcPhys = true;
}

void FWLDHaptic6D::TestSimulation6D(){
	/* 6DoF Haptic Rendering のためのテストシミュレーション*/
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
			if(iPointer->interactInfo[i].flag.blocal == false) continue;
			PHSolid* soPointer = iPointer->pointerSolid->Cast();
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			Vec3d n = -iInfo->toHaptic.face_normal;

			const float minTestForce = 1;		// 最小テスト力
			const float minTestTorque = 0.;

			SpatialVector f[6];
			for(int k = 0; k < 5; k++)	f[k] = SpatialVector();
			f[0].v() = iInfo->toPhysic.test_force;
			f[0].w() = iInfo->toPhysic.test_torque;
			iInfo->toPhysic.test_force = Vec3d();
			iInfo->toPhysic.test_torque = Vec3d();
			/// テスト力が0の場合の処理
			//テストトルクも必ず0になる
			//接触していないので，1点だけに力を加えるようにする
			if(f[0].v().norm() == 0){
				f[0].v() = minTestForce * n;
				Vec3d cPoint = phSolid->GetPose() * iInfo->toHaptic.closest_point;		// 力を加える点(ワールド座標)
				Vec3d center = phSolid->GetCenterPosition();
				f[0].w() = (cPoint - center) % f[0].v();
			}
			/// テストトルクが0の場合の処理
			if(f[0].w().norm() == 0){
				f[0].w() = minTestTorque * Vec3d(1, 0, 0);	// とりあえず適当なベクトルを入れておく
				DSTR << "test torque is Zero" << std::endl;
			}

#if 1
			f[3].w() = f[0].w();
			f[0].w() = Vec3d();
			/// 力f[0].v()と垂直な力2本
			Vec3d base1 = f[0].v().unit();
			Vec3d base2 = Vec3d(1,0,0) - (Vec3d(1,0,0)*base1)*base1;
			if(base2.norm() > 0.1)	base2.unitize();
			else{
				base2 = Vec3d(0,1,0) - (Vec3d(0,1,0)*base1)*base1;
				base2.unitize();
			}
			Vec3d base3 = base1^base2;
			f[1].v() = f[0].v().norm() * (base1 + base2);
			f[2].v() = f[0].v().norm() * (base1 + base3);

			/// トルクf[0].w()と垂直なトルク2本
			base1 = f[3].w().unit();
			base2 = Vec3d(1,0,0) - (Vec3d(1,0,0)*base1)*base1;
			if(base2.norm() > 0.1)	base2.unitize();
			else{
				base2 = Vec3d(0,1,0) - (Vec3d(0,1,0)*base1)*base1;
				base2.unitize();
			}
			base3 = base1^base2;
			f[4].v() = f[5].v() = f[0].v();
			f[4].w() = f[3].w().norm() * (base1 + base2);
			f[5].w() = f[3].w().norm() * (base1 + base3);
#else
			/// テスト力すべてをf[0]にする
			for(int k = 1; k < 6; k++) f[k] = f[0];

			/// 力f[0].v()と垂直な力2本
			Vec3d base1 = f[0].v().unit();
			Vec3d base2 = Vec3d(1,0,0) - (Vec3d(1,0,0)*base1)*base1;
			if(base2.norm() > 0.1)	base2.unitize();
			else{
				base2 = Vec3d(0,1,0) - (Vec3d(0,1,0)*base1)*base1;
				base2.unitize();
			}
			Vec3d base3 = base1^base2;
			f[1].v() = f[0].v().norm() * (base1 + base2);
			f[2].v() = f[0].v().norm() * (base1 + base3);

			/// トルクf[0].w()と垂直なトルク2本
			base1 = f[0].w().unit();
			base2 = Vec3d(1,0,0) - (Vec3d(1,0,0)*base1)*base1;
			if(base2.norm() > 0.1)	base2.unitize();
			else{
				base2 = Vec3d(0,1,0) - (Vec3d(0,1,0)*base1)*base1;
				base2.unitize();
			}
			base3 = base1^base2;
			f[3].w() = f[0].w().norm() * (base1 + base2);
			f[4].w() = f[0].w().norm() * (base1 + base3);

			/// f[0]と垂直になるベクトル svbase2 = (a * f[0].v() b*f[0].w())^{t}
			/// a*f[0].v()*f[0].v() + b*f[0].w()*f[0].w() = 0となるa, bをみつける
			SpatialVector svbase1 = SpatialVector();
			svbase1.v() = f[0].v();
			svbase1.w() = f[0].w();
			svbase1.unitize();
			double f_ip = f[0].v()*f[0].v();
			double t_ip = f[0].w()*f[0].w();
			double c = f_ip / t_ip;
			double a = 1;
			double b = - a * c;
			//DSTR << c << std::endl;
			SpatialVector svbase2 = SpatialVector();
			svbase2.v() = a * f[0].v();
			svbase2.w() = b * f[0].w();
			svbase2.unitize();
			//f[5] = f[0].w().norm() * (svbase1 + svbase2);
			f[5] = svbase1 + svbase2;
			f[5].v() = f[0].v().norm() * f[5].v();
			f[5].w() = f[0].w().norm() * f[5].w();

#endif

			TMatrixRow<6,6,double> u = TMatrixRow<6,6,double>();		// 剛体の機械インピーダンス
			TMatrixRow<6,6,double> F = TMatrixRow<6,6,double>();		// 加える力,トルク行列
			for(int k = 0; k < 6; k++)	F.col(k) = f[k];				// テスト力，テストトルクを行列に詰める

			/// テスト力，テストトルクを加えてテストシミュレーション実行
			for(int k = 0; k < 6; k++){
				phSolid->AddForce(f[k].v()); 
				phSolid->AddTorque(f[k].w());
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
			//DSTR << F.det() << std::endl; 
			//DSTR << u << std::endl; 
			iInfo->mobility.Minv = u  * F.inv();			// モビリティAの計算
#if 0
			DEBUG
			DSTR << "------------------------" << std::endl;
			DSTR << "b:" << std::endl;	DSTR << inSolid->b << std::endl;
			DSTR << "F:" << std::endl;	DSTR << F << std::endl;
			DSTR << "Minv:" << std::endl;	DSTR << iInfo->mobility.Minv << std::endl;
#if 0
			TMatrixRow<6, 6, double> M = TMatrixRow<6, 6, double>();
			if(det(u) == 0)	M = F * u; 
			else			M = F * u.inv(); 
			DSTR << "M:" << std::endl;	DSTR << M << std::endl;
#endif
#endif
		}
	}
	///--------テストシミュレーション終了--------
	#ifdef DIVIDE_STEP
		states2->LoadState(phScene);							// 元のstateに戻しシミュレーションを進める
	#endif
}

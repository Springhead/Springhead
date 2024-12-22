#include <Physics/PHHapticStepLocalDynamicsDev.h>
#include <Physics/PHHapticEngine.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticStepLocalDynamicsDev
void PHHapticStepLocalDynamicsDev::StepHapticLoop() {
	UpdateHapticPointer();
	engine->HapticRendering(this->Cast());
	LocalDynamics6D();
}
void PHHapticStepLocalDynamicsDev::LocalDynamics6D() {
	double pdt = GetPhysicsTimeStep();
	double hdt = GetHapticTimeStep();
	for (int i = 0; i < NHapticSolids(); i++) {
		PHSolidForHaptic* hsolid = (PHSolidForHaptic*)GetSolidInHaptic(i);
		if (hsolid->doSim == 0) continue;
		if (hsolid->GetLocalSolid()->IsDynamical() == false) continue;
		PHSolid* localSolid = (PHSolid*)hsolid->GetLocalSolid();
		SpatialVector vel;
		vel.v() = localSolid->GetVelocity();
		vel.w() = localSolid->GetAngularVelocity();
		if (loopCount == 1) {
			vel += (hsolid->curb - hsolid->lastb) * pdt;	// 衝突の影響を反映
		}
		for (int j = 0; j < NHapticPointers(); j++) {
			PHHapticPointer* pointer = (PHHapticPointer*)GetPointerInHaptic(j);
			PHSolidPairForHaptic* sp = (PHSolidPairForHaptic*)GetSolidPairInHaptic(i, pointer->GetPointerID());
			if (sp->inLocal == 0) continue;
			SpatialVector force;
			force.v() = sp->force;
			force.w() = sp->torque;
			vel += (sp->A6D * force) * hdt;			// 力覚ポインタからの力による速度変化
													//CSVOUT << force[0] << "," << force[1] << "," << force[2] << "," << force[3] << "," << force[4] << "," << force[5] << "," <<std::endl;
													//DSTR << force << std::endl;
													//DSTR << sp->A6D << std::endl;
		}
		vel += hsolid->b * hdt;
		//DSTR << vel << std::endl;
		//CSVOUT << vel.w().y << std::endl;
		localSolid->SetVelocity(vel.v());
		localSolid->SetAngularVelocity(vel.w());
		localSolid->SetOrientation((Quaterniond::Rot(vel.w() * hdt) * localSolid->GetOrientation()).unit());
		//localSolid->SetOrientation(( localSolid->GetOrientation() * Quaterniond::Rot(vel.w() * hdt)).unit());
		localSolid->SetCenterPosition(localSolid->GetCenterPosition() + vel.v() * hdt);

		localSolid->SetUpdated(true);
		localSolid->Step();
	}
}

PHHapticStepLocalDynamicsDev::PHHapticStepLocalDynamicsDev(){
	states = ObjectStatesIf::Create();
}

void PHHapticStepLocalDynamicsDev::Step1(){
	lastvels.clear();
	for(int i = 0; i < NHapticSolids(); i++){
		SpatialVector vel;
		vel.v() = GetHapticSolid(i)->GetSceneSolid()->GetVelocity();
		vel.w() = GetHapticSolid(i)->GetSceneSolid()->GetAngularVelocity();
		lastvels.push_back(vel);
	}
}
void PHHapticStepLocalDynamicsDev::Step2(){
	// 更新後の速度、前回の速度差から定数項を計算
	for(int i = 0; i < NHapticSolids(); i++){
		// 近傍の剛体のみ
		if(((PHSolidForHaptic*)GetHapticSolid(i))->doSim == 0) continue;
		PHSolidIf* solid = GetHapticSolid(i)->GetSceneSolid();
		SpatialVector dvel;
		dvel.v() = solid->GetVelocity();
		dvel.w() = solid->GetAngularVelocity();
		((PHSolidForHaptic*)GetHapticSolid(i))->curb = (dvel - lastvels[i]) / GetPhysicsTimeStep();
	}

	engine->StartDetection();
	PredictSimulation6D();
}

void PHHapticStepLocalDynamicsDev::PredictSimulation6D(){
	engine->bPhysicStep = false;
	/** PHSolidForHapticのdosim > 0の物体に対してテスト力を加え，
		すべての近傍物体について，アクセレランスを計算する */
	PHSceneIf* phScene = engine->GetScene();

	//#define DIVIDE_STEP
	#ifdef DIVIDE_STEP
	/// 予測シミュレーションのために現在の剛体の状態を保存する
	states2->SaveState(phScene);		
	///	LCPの直前までシミュレーションしてその状態を保存
	phScene->ClearForce();
	phScene->GenerateForce();
	phScene->IntegratePart1();
	#endif
	/// 予測シミュレーションのために現在の剛体の状態を保存する
	phScene->GetConstraintEngine()->SetBSaveConstraints(true);
	states->Clear();
	states->SaveState(phScene);	

	/// テストシミュレーション実行
	for(int i = 0; i < NHapticSolids(); i++){
		if( engine->hapticSolids[i]->doSim == 0) continue;
		PHSolidForHaptic* hsolid = engine->hapticSolids[i];
		PHSolid* phSolid = hsolid->sceneSolid;
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

		/// アクセレランスbの算出
		hsolid->lastb = hsolid->b;
		double pdt = phScene->GetTimeStep();
		SpatialVector preb = (nextvel - curvel)/pdt;
		hsolid->b = preb;


		states->LoadState(phScene);					// 現在の状態に戻す

		//if(i == 1){
		//	DSTR << "-------" << std::endl;
		//	//DSTR << dot << std::endl;
		//	//DSTR << hsolid->sceneSolid->GetLastVelocity().y << "," << "curvel" << curvel.v().y << "    " << "nextvel" << nextvel.v().y << std::endl;  
		//	DSTR << hsolid->curb << "," << hsolid->b << "," << hsolid->bimpact << std::endl;
		//	//CSVOUT << hsolid->sceneSolid->GetFramePosition().y << "," << curvel.v().y << std::endl;
		//}

		/// HapticPointerの数だけ力を加える予測シミュレーション
		for(int j = 0; j < NHapticPointers(); j++){
			PHHapticPointer* pointer = engine->hapticPointers[j];
			PHSolidPairForHaptic* solidPair = engine->GetSolidPairImp(i, pointer->GetPointerID());
			if(solidPair->inLocal == 0) continue;
			PHShapePairForHaptic* sp = solidPair->GetShapePair(0, 0)->Cast();	// 1形状のみ対応
			Vec3d cPoint = sp->shapePoseW[0] * sp->closestPoint[0];		// 力を加える点(ワールド座標)
			Vec3d normal = -1 * sp->normal;

			float minTestForce = 0.5;		// 最小テスト力
			float minTestTorque = 0.5;

			// テスト力、テストトルクの作成
			SpatialVector testForce;
			testForce.v() = solidPair->force;
			testForce.w() = solidPair->torque;
			solidPair->force  = Vec3d();
			solidPair->torque = Vec3d();

			/// テスト力が0の場合の処理
			//テストトルクも必ず0になる
			//接触していないので，1点だけに力を加えるようにする
			if(testForce.v().norm() < 1e-5){
				testForce.v() = minTestForce * normal;
				Vec3d cPoint = sp->shapePoseW[0] * sp->closestPoint[0];		// 力を加える点(ワールド座標)
				Vec3d center = phSolid->GetCenterPosition();
				testForce.w() = (cPoint - center) % testForce.w();
				//DSTR << testForce.w() << std::endl;
			}
			///// テストトルクが0の場合の処理
			//if(testForce.w().norm() < 1e-5){
			//	testForce.w() = minTestTorque * Vec3d(1, 0, 0);	// とりあえず適当なベクトルを入れておく
			//	DSTR << "test torque is Zero" << std::endl;
			//}

			SpatialVector f[6];
#if 1
			/// 力f[0].v()と垂直な力2本
			f[0].v() = testForce.v();
			Vec3d base1 = f[0].v().unit();
			Vec3d base2 = Vec3d(1,0,0) - (Vec3d(1,0,0)*base1)*base1;
			if(base2.norm() > 0.1)	base2.unitize();
			else{
				base2 = Vec3d(0,1,0) - (Vec3d(0,1,0)*base1)*base1;
				base2.unitize();
			}
			Vec3d base3 = base1^base2;
			f[1].v() = f[0].v().norm() * (base1 + base2).unit();
			f[2].v() = f[0].v().norm() * (base1 + base3).unit();

			/// トルクf[0].w()と垂直なトルク2本
			f[3].w() = testForce.w();
			base1 = f[3].w().unit();
			base2 = Vec3d(1,0,0) - (Vec3d(1,0,0)*base1)*base1;
			if(base2.norm() > 0.1)	base2.unitize();
			else{
				base2 = Vec3d(0,1,0) - (Vec3d(0,1,0)*base1)*base1;
				base2.unitize();
			}
			base3 = base1^base2;
			f[4].v() = f[0].v();
			f[5].v() = f[0].v();
			f[4].w() = f[3].w().norm() * (base1 + base2).unit();
			f[5].w() = f[3].w().norm() * (base1 + base3).unit();

#else
			/// テスト力すべてをf[0]にする
			for(int k = 1; k < 6; k++) f[k] = f[0];

			/// 力f[0].v()と垂直な力2本
			f[0].v() = testForce.v();
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

			/// f[0]と垂直になるベクトル svbase2 = (a * f[0].v(), b*f[0].w())^{t}
			/// a*f[0].v()*f[0].v() + b*f[0].w()*f[0].w() = 0となるa, bをみつける
			SpatialVector svbase1 = SpatialVector();
			svbase1.v() = f[0].v();
			svbase1.w() = f[0].w();
			svbase1.unitize();
			double f_ip = f[0].v().square();
			double t_ip = f[0].w().square();
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

			//TMatrixRow<6,6,double> u = TMatrixRow<6,6,double>();		// 剛体の機械インピーダンス
			//TMatrixRow<6,6,double> F = TMatrixRow<6,6,double>();		// 加える力,トルク行列
			SpatialMatrix u, F;
			for(int k = 0; k < 6; k++)	F.col(k) = f[k];				// テスト力，テストトルクを行列に詰める

			/// テスト力，テストトルクを加えてテストシミュレーション実行
			for(int k = 0; k < 6; k++){
				phSolid->AddForce (f[k].v()); 
				phSolid->AddTorque(f[k].w());
				#ifdef DIVIDE_STEP
				phScene->IntegratePart2();
				#else
				phScene->Step();
				#endif
				nextvel.v() = phSolid->GetVelocity();
				nextvel.w() = phSolid->GetAngularVelocity();
				u.col(k) = (nextvel - curvel) /pdt - hsolid->b;
				states->LoadState(phScene);

				//DSTR << "force" << std::endl;
				//DSTR << f[k] << std::endl;
				//DSTR << "nextvel" << std::endl;
				//DSTR << nextvel << std::endl;
			}
			//DSTR << F.det() << std::endl; 
			//DSTR << u << std::endl; 
			SpatialMatrix Finv;
			(Matrix6d&)Finv = F.inv();
			solidPair->A6D = u * Finv;			// モビリティAの計算
#if 0
			DSTR << "------------------------" << std::endl;
			DSTR << "u" << std::endl; DSTR << u << std::endl;
			DSTR << "b:" << std::endl;	DSTR << hsolid->b << std::endl;
			DSTR << "F:" << std::endl;	DSTR << F << std::endl;
			DSTR << "Minv:" << std::endl;	DSTR << solidPair->A6D << std::endl;
#if 1
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
	engine->bPhysicStep = true;
}


void PHHapticStepLocalDynamicsDev::SyncHaptic2Physic(){
#if 1
	// physics <------ haptic
	// PHSolidForHapticの同期
	// PHSolidPairForHaptic(力覚ポインタと近傍の物体)の各種情報の同期
	for(size_t i = 0; i < hapticModel.hapticPointers.size(); i++){
		PHHapticPointer* hpointer = hapticModel.hapticPointers[i];
		int hpointerID = hpointer->GetPointerID();
		int nNeighbors = (int)hpointer->neighborSolidIDs.size();
		// 近傍物体であるペアだけ同期
		for(int j = 0; j < nNeighbors; j++){
			int solidID = hpointer->neighborSolidIDs[j];
			PHSolidPairForHaptic* hpair = hapticModel.GetSolidPair(solidID, hpointerID);
			PHSolidPairForHaptic* ppair = engine->GetSolidPairImp(solidID, hpointerID);
			PHSolidPairForHapticVars* hVars = (PHSolidPairForHapticVars*)hpair;
			PHSolidPairForHapticVars* pVars = (PHSolidPairForHapticVars*)ppair;
			*pVars = *hVars;	// haptic側で保持しておくべき情報を同期
		}
	}
	// LocalDynamicsSimulationの結果をシーンに反映
	for(int i = 0; i < (int)hapticModel.hapticSolids.size(); i++){
		PHSolidForHaptic* hsolid = hapticModel.hapticSolids[i];
		if(hsolid->bPointer) continue;		// ポインタの場合
		if(hsolid->doSim <= 1) continue;	// 局所シミュレーション対象でない場合

		//アクセレランス定数項で反映速度を作る
		double pdt = GetPhysicsTimeStep();
		PHSolid* localSolid = &hsolid->localSolid;
		PHSolidForHaptic* psolid = (PHSolidForHaptic*)GetHapticSolid(i);
		SpatialVector b = (psolid->b + (psolid->curb - psolid->lastb)) * pdt;	// モビリティ定数項
		//b = SpatialVector();
		Vec3d v = localSolid->GetVelocity()        + b.v();			// 反映速度
		Vec3d w = localSolid->GetAngularVelocity() + b.w();		// 反映角速度

		// 状態の反映
		PHSolid* sceneSolid = hsolid->sceneSolid;
		Vec3d svel = sceneSolid->GetVelocity();
		Vec3d spos = sceneSolid->GetFramePosition();
		sceneSolid->SetVelocity(v);
		sceneSolid->SetAngularVelocity(w);
		sceneSolid->SetPose(localSolid->GetPose());
		//DSTR << phvel << ", " << v << std::endl;
		//if(i == 1){
		//	DSTR << "physicsvel" << svel.y << "," << "hapticvel" << localSolid->GetVelocity().y << std::endl;
		//	DSTR << "mergevel" << sceneSolid->GetVelocity().y << std::endl;
		//	DSTR << "nextb" << psolid->b << std::endl;
		//	DSTR << b << std::endl;
		//	CSVOUT << svel.y << "," << spos.y << "," <<  localSolid->GetVelocity().y << "," << localSolid->GetFramePosition().y << "," << v.y << std::endl;
		//}

		PHSolidForHapticSt* hst = (PHSolidForHapticSt*)hsolid;
		PHSolidForHapticSt* pst = (PHSolidForHapticSt*)GetHapticSolid(i);
		*pst = *hst;
	}
#endif
}

void PHHapticStepLocalDynamicsDev::SyncPhysic2Haptic(){
#if 1
	// haptic <------ physics
	// PHSolidForHapticの同期
	for(int i = 0; i < NHapticSolids(); i++){
		PHSolidForHaptic* psolid = (PHSolidForHaptic*)GetHapticSolid(i);
		if(psolid->bPointer) continue;
		PHSolidForHaptic* hsolid = hapticModel.hapticSolids[i];
		PHSolidForHapticSt2* pst2 = (PHSolidForHapticSt2*)psolid;
		PHSolidForHapticSt2* hst2 = (PHSolidForHapticSt2*)hsolid;
		*hst2 = *pst2;
		if(psolid->doSim == 1) hsolid->localSolid = psolid->localSolid;		
	}
	// solidpair, shapepairの同期
	// 近傍物体のみ同期させる
	for(int i = 0; i < NHapticPointers(); i++){
		PHHapticPointer* ppointer = (PHHapticPointer*)GetHapticPointer(i);
		const int ppointerID = ppointer->GetPointerID();
		const int nNeighbors = (int)ppointer->neighborSolidIDs.size();
		for(int j = 0; j < nNeighbors; j++){
			const int solidID = ppointer->neighborSolidIDs[j];
			PHSolidPairForHaptic* hpair = hapticModel.GetSolidPair(solidID, ppointerID);
			PHSolidPairForHaptic* ppair = engine->GetSolidPairImp(solidID, ppointerID);

			hpair->CopyFromPhysics(ppair);
			//DSTR << hpair->A << std::endl;
		}
	}
#endif
}

void PHHapticStepLocalDynamicsDev::ReleaseState(PHSceneIf* scene) {
	states->ReleaseState(scene);
}


}

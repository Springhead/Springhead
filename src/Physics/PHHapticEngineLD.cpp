#include <Physics/PHHapticEngineLD.h>

namespace Spr{;
//----------------------------------------------------------------------------
// PHHapticLoopLD
void PHHapticLoopLD::Step(){
	UpdateInterface();
	HapticRendering();
	LocalDynamics();
}
void PHHapticLoopLD::HapticRendering(){
	PHHapticRenderInfo hri;
	hri.pointers = GetHapticPointers();
	hri.hsolids = GetHapticSolids();
	hri.sps = GetSolidPairsForHaptic();
	hri.hdt = GetHapticTimeStep();
	hri.pdt = GetPhysicsTimeStep();
	hri.loopCount = loopCount;
	hri.bInterpolatePose = false;
	hri.bMultiPoints = false;
	//PenaltyBasedRendering(hri);
	//ConstraintBasedRendering(hri);
	PHHapticRenderBase hapticRender;
	hapticRender.ConstraintBasedRendering(hri);
}

void PHHapticLoopLD::LocalDynamics(){
	double pdt = GetPhysicsTimeStep();
	double hdt = GetHapticTimeStep();
	for(int i = 0; i < NHapticSolids(); i++){
		PHSolidForHaptic* hsolid = GetHapticSolid(i);
		if(hsolid->doSim == 0) continue;
		PHSolid* localSolid = &hsolid->localSolid;
		SpatialVector vel;
		vel.v() = localSolid->GetVelocity();
		vel.w() = localSolid->GetAngularVelocity();
		if(loopCount == 1){
			vel += (hsolid->curb - hsolid->lastb) *  pdt;	// 衝突の影響を反映
		}
		for(int j = 0; j < NHapticPointers(); j++){
			PHHapticPointer* pointer = GetHapticPointer(j);
			PHSolidPairForHaptic* sp = GetSolidPairForHaptic(i, pointer->GetPointerID());
			if(sp->inLocal == 0) continue;
			vel += (sp->A * sp->force) * hdt;			// 力覚ポインタからの力による速度変化
			//DSTR << sp->A * sp->force << std::endl;
		}
		vel += hsolid->b * hdt;
		localSolid->SetVelocity(vel.v());		
		localSolid->SetAngularVelocity(vel.w());
		localSolid->SetOrientation(( Quaterniond::Rot(vel.w() * hdt) * localSolid->GetOrientation()).unit());
		localSolid->SetCenterPosition(localSolid->GetCenterPosition() + vel.v() * hdt);

 		localSolid->SetUpdated(true);
		localSolid->Step();
		if(i == 1){
			CSVOUT << localSolid->GetFramePosition().y << "," << vel.v().y << "," << hsolid->b.v().y * hdt << std::endl;
			//CSVOUT << localSolid->GetVelocity().y << ","
			//<< (hsolid->curb - hsolid->lastb).v().y *  pdt << ","
			//<< hsolid->b.v().y * hdt << std::endl;
		}
	}
}

//----------------------------------------------------------------------------
// PHHapticEngineLD
PHHapticEngineLD::PHHapticEngineLD(){
	hapticLoop = &hapticLoopLD;
	hapticLoop->engineImp = this;
	states = ObjectStatesIf::Create();
	states2 = ObjectStatesIf::Create();
}

void PHHapticEngineLD::Step1(){
}
void PHHapticEngineLD::Step2(){
	// 更新後の速度、前回の速度差から定数項を計算
	for(int i = 0; i < NHapticSolids(); i++){
		// 近傍の剛体のみ
		if(GetHapticSolid(i)->doSim == 0) continue;
		PHSolid* solid = GetHapticSolid(i)->sceneSolid;
		SpatialVector curvel, lastvel;
		curvel.v() = solid->GetVelocity();
		curvel.w() = solid->GetAngularVelocity();
		lastvel.v() = solid->GetLastVelocity();
		lastvel.w() = solid->GetLastAngularVelocity();
		GetHapticSolid(i)->curb = (curvel - lastvel) / GetPhysicsTimeStep();
	}

	engine->StartDetection();
	PredictSimulation3D();
}

/// 1対1のshapeで、1点の接触のみ対応
void PHHapticEngineLD::PredictSimulation3D(){
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
#if 1
	/// テストシミュレーション実行
	for(int i = 0; i < NHapticSolids(); i++){
		if(GetHapticSolid(i)->doSim == 0) continue;
		PHSolidForHaptic* hsolid = GetHapticSolid(i);
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
		hsolid->b = (nextvel - curvel) / pdt;
		states->LoadState(phScene);						// 現在の状態に戻す

		/// HapticPointerの数だけ力を加える予測シミュレーション
		for(int j = 0; j < NHapticPointers(); j++){
			PHHapticPointer* pointer = GetHapticPointer(j);
			PHSolidPairForHaptic* solidPair = GetSolidPairForHaptic(i, pointer->GetPointerID());
			if(solidPair->inLocal == 0) continue;
			PHShapePairForHaptic* sp = solidPair->shapePairs.item(0, 0);	// 1形状のみ対応
			Vec3d cPoint = sp->shapePoseW[0] * sp->closestPoint[0];		// 力を加える点(ワールド座標)
			Vec3d normal = -1 * sp->normal;

			TMatrixRow<6, 3, double> u;			// 剛体の機械インピーダンス?
			TMatrixRow<3, 3, double> force;		// 加える力
			u.clear(0.0);
			force.clear(0.0);

			float minTestForce = 0.5;		// 最小テスト力

			// 相対座標系
			if(solidPair->force.norm() == 0){
				force.col(0) = minTestForce * normal;
			}else{
				force.col(0) = solidPair->force;
				solidPair->force = Vec3d();
			}
			Vec3d base1 = force.col(0).unit();
			Vec3d base2 = Vec3d(1, 0, 0) - (Vec3d(1, 0, 0) * base1) * base1;
			if (base2.norm() > 0.1){
				base2.unitize();
			}else{
				base2 = Vec3d(0, 1, 0) - (Vec3d(0, 1, 0) * base1) * base1;
				base2.unitize();
			}
			Vec3d base3 = base1^base2;
			force.col(1) = force.col(0).norm() * (base1 + base2).unit();
			force.col(2) = force.col(0).norm() * (base1 + base3).unit();

			/// テスト力を3方向に加える	
			for(int m = 0; m < 3; m++){
				phSolid->AddForce(force.col(m), cPoint);
				#ifdef DIVIDE_STEP
				phScene->IntegratePart2();
				#else
				phScene->Step();
				#endif
				nextvel.v() = phSolid->GetVelocity();
				nextvel.w() = phSolid->GetAngularVelocity();
				u.col(m) = (nextvel - curvel) / pdt - hsolid->b;
				//DSTR << force.col(m) << force.col(m).norm() << std::endl;
				//DSTR << nextvel - curvel << std::endl;
				states->LoadState(phScene);			
			}

			solidPair->A = u  * force.inv();	// m/(Ns2)
			//DSTR << solidPair->A << std::endl;
			//if(i == 1) CSVOUT << hsolid->b.v().y << std::endl;
		}
	}
#endif
	///--------テストシミュレーション終了--------
#ifdef DIVIDE_STEP
	states2->LoadState(phScene);							// 元のstateに戻しシミュレーションを進める
#endif
	engine->bPhysicStep = true;
}

void PHHapticEngineLD::SyncHaptic2Physic(){
#if 1
	// physics <------ haptic
	// PHSolidForHapticの同期
	// PHSolidPairForHaptic(力覚ポインタと近傍の物体)の各種情報の同期
	for(int i = 0; i < hapticLoop->NHapticPointers(); i++){
		PHHapticPointer* hpointer = hapticLoop->GetHapticPointer(i);
		int hpointerID = hpointer->GetPointerID();
		int nNeighbors = hpointer->neighborSolidIDs.size();
		// 近傍物体であるペアだけ同期
		for(int j = 0; j < nNeighbors; j++){
			int solidID = hpointer->neighborSolidIDs[j];
			PHSolidPairForHaptic* hpair = hapticLoop->GetSolidPairForHaptic(solidID, hpointerID);
			PHSolidPairForHaptic* ppair = GetSolidPairForHaptic(solidID, hpointerID);
			PHSolidPairForHapticSt* hst = (PHSolidPairForHapticSt*)hpair;
			PHSolidPairForHapticSt* pst = (PHSolidPairForHapticSt*)ppair;
			*pst = *hst;	// haptic側で保持しておくべき情報を同期
		}
	}
	// LocalDynamicsSimulationの結果をシーンに反映
	for(int i = 0; i < (int)hapticLoop->NHapticSolids(); i++){
		PHSolidForHaptic* hsolid = hapticLoop->GetHapticSolid(i);
		//DSTR << hsolid->doSim << std::endl;
		if(hsolid->bPointer) continue;		// ポインタの場合
		if(hsolid->doSim <= 1) continue;	// 局所シミュレーション対象でない場合

		//アクセレランス定数項で反映速度を作る
		double pdt = GetPhysicsTimeStep();
		PHSolid* localSolid = hsolid->GetLocalSolid();
		PHSolidForHaptic* psolid = GetHapticSolid(i);
		SpatialVector b = (psolid->b + (psolid->curb - psolid->lastb)) * pdt;	// モビリティ定数項
		Vec3d v = localSolid->GetVelocity() + b.v();			// 反映速度
		Vec3d w = localSolid->GetAngularVelocity() + b.w();		// 反映角速度

		//if(i == 1) CSVOUT << localSolid->GetVelocity().y << "," << v.y << std::endl;

		// 状態の反映
		PHSolid* sceneSolid = hsolid->sceneSolid;
		sceneSolid->SetVelocity(v);
		sceneSolid->SetAngularVelocity(w);
		sceneSolid->SetPose(localSolid->GetPose());
		//CSVOUT << sceneSolid->GetPose().Pos().y << std::endl;

		PHSolidForHapticSt* hst = (PHSolidForHapticSt*)hsolid;
		PHSolidForHapticSt* pst = (PHSolidForHapticSt*)GetHapticSolid(i);
		*pst = *hst;
	}
#endif
}

void PHHapticEngineLD::SyncPhysic2Haptic(){
#if 1
	// haptic <------ physics
	// PHSolidForHapticの同期
	for(int i = 0; i < NHapticSolids(); i++){
		PHSolidForHaptic* psolid = GetHapticSolid(i);
		if(psolid->bPointer) continue;
		PHSolidForHaptic* hsolid = hapticLoop->GetHapticSolid(i);
		PHSolidForHapticSt2* pst2 = (PHSolidForHapticSt2*)psolid;
		PHSolidForHapticSt2* hst2 = (PHSolidForHapticSt2*)hsolid;
		*hst2 = *pst2;
		if(psolid->doSim == 1) hsolid->localSolid = psolid->localSolid;		
	}
	// solidpair, shapepairの同期
	// 近傍物体のみ同期させる
	for(int i = 0; i < NHapticPointers(); i++){
		PHHapticPointer* ppointer = GetHapticPointer(i);
		const int ppointerID = ppointer->GetPointerID();
		const int nNeighbors = ppointer->neighborSolidIDs.size();
		for(int j = 0; j < nNeighbors; j++){
			const int solidID = ppointer->neighborSolidIDs[j];
			PHSolidPairForHaptic* hpair = hapticLoop->GetSolidPairForHaptic(solidID, ppointerID);
			PHSolidPairForHaptic* ppair = GetSolidPairForHaptic(solidID, ppointerID);
			*hpair = PHSolidPairForHaptic(*ppair);
		}
	}
#endif
}

}
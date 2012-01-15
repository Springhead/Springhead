#include <Physics/PHHapticEngine.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticPointer
void PHHapticPointer::UpdateInterface(float dt){
	if(bDebugControl) return;
	double s = GetWorldScale() * GetPosScale();
	HIHapticIf* hif = DCAST(HIHapticIf, humanInterface);
	hif->Update(dt);
	hiSolid->SetVelocity((Vec3d)hif->GetVelocity() * s);
	hiSolid->SetAngularVelocity((Vec3d)hif->GetAngularVelocity());
	Posed pose;
	pose.Pos() = (Vec3d)hif->GetPosition() * s;
	pose.Ori() = hif->GetOrientation();
	hiSolid->SetPose(GetDefaultPose() * pose);
}

void PHHapticPointer::UpdateHapticPointer(){
	SetVelocity(hiSolid->GetVelocity());
	SetAngularVelocity(hiSolid->GetAngularVelocity());
	SetFramePosition(hiSolid->GetFramePosition());
	SetOrientation(hiSolid->GetOrientation());
}

void PHHapticPointer::SetForce(SpatialVector f){
	HIHapticIf* hif = DCAST(HIHapticIf, humanInterface);
	if(bForce)
		 hif->SetForce(f.v(), f.w());
	else hif->SetForce(Vec3f(), Vec3f());	
}

// 1/11力覚レンダリング
// 実装予定
// 球型ポインタのみが使えるProxy3DoF(池田くんが作った奴）
// proxyに質量を与え、stick-slip frictionが提示できるproxy simulation
// 凸形状が使えるmultiple point intermediate representation
// 凸形状が使えるconstratint based rendering
// 凸形状が使えるvirtual coupling


// PHSolidForHapticのsolidはポインタなので、physics側への剛体のポインタになってる
// 複製する必要がある
void PHHapticPointer::HapticRendering(PHSolidsForHaptic* hsolids, PHSolidPairsForHaptic* sps, double loopCount){		
		//SpatialVector outForce = SpatialVector();
		//int nNeighbors = neighborSolidIDs.size();
		//for(int i = 0; i < nNeighbors; i++){
		//	int a = solidID;	int b = neighborSolidIDs[i];
		//	int p = 0;			int s = 1;
		//	if(a > b){
		//		std::swap(a, b);
		//		p = 1;
		//		s = 0;
		//	}
		//	PHSolid* hsolid = &hsolids->at(b)->localSolid; 
		//	PHSolidPairForHaptic* solidPair = sps->item(a, b);
		//	for(int j = 0; j < solidPair->solid[0]->NShape(); j++){
		//		for(int k = 0; k < solidPair->solid[1]->NShape(); k++){
		//			PHShapePairForHaptic* sp;
		//			sp = solidPair->shapePairs.item(j, k);
		//			Vec3d sPoint = sp->shapePoseW[s] * sp->closestPoint[s];	// 剛体側の近傍点
		//			Vec3d pPoint = pose * sp->closestPoint[p];	// ポインタ側の近傍点
		//			Vec3d last_sPoint = sp->lastShapePoseW[s] * sp->lastClosestPoint[s];
		//			Vec3d normal = sp->normal;
		//			Vec3d lastNormal = sp->lastNormal;
		//			if(s > p){
		//				normal *= -1;
		//				lastNormal *= -1;
		//			}

		//			// 剛体の面の法線補間
		//			// 前回の法線と現在の法線の間を補間しながら更新
		//			double pdt = scene->GetTimeStep();
		//			double hdt = scene->GetHapticTimeStep();
		//			double syncCount = pdt / hdt;
		//			double t = loopCount / syncCount;
		//			Vec3d interpolation_normal = interpolate(t, lastNormal, normal);
		//			Vec3d interpolation_sPoint = interpolate(t, last_sPoint, sPoint); 
		//			Vec3d dir = pPoint - interpolation_sPoint;
		//			
		//			double f = dir * interpolation_normal;
		//			if(f < 0.0){
		//				Vec3d ortho = f * interpolation_normal;
		//				Vec3d dv = GetPointVelocity(pPoint) - hsolid->GetPointVelocity(interpolation_sPoint);
		//				Vec3d dvortho = dv.norm() * interpolation_normal;

		//				double K = 100;
		//				double D = 0.1;
		//				Vec3d addforce = -1 * (K * ortho + D * dvortho);
		//				double ws4 = pow(GetWorldScale(), 4);
		//				outForce.v() += addforce / ws4;
		//				outForce.w() = Vec3d();
		//				
		//				ImpulsePoint ip;
		//				ip.contactPointW = interpolation_sPoint;
		//				ip.impulse = -1 * addforce;
		//				solidPair->impulsePoints.push_back(ip);
		//			}
		//		}
		//	}
		//}
		//SetForce(outForce);
}


void PHHapticPointer::MultiPointRendering(PHSolidsForHaptic* hsolids, PHSolidPairsForHaptic* sps, 
											double loopCount, double pdt, double hdt){		
	
	PHIrs irs;
	irs.clear();
	int nNeighbors = neighborSolidIDs.size();
	for(int i = 0; i < nNeighbors; i++){
		int solidID = neighborSolidIDs[i];
		PHSolid* hsolid = &hsolids->at(solidID)->localSolid; 
		PHSolidPairForHaptic* solidPair = sps->item(solidID, pointerID);
		for(int j = 0; j < solidPair->solid[0]->NShape(); j++){
			for(int k = 0; k < solidPair->solid[1]->NShape(); k++){
				PHShapePairForHaptic* sp = solidPair->shapePairs.item(j, k);
				Vec3d sPoint = sp->shapePoseW[0] * sp->closestPoint[0];	// solidの近傍点
				Vec3d pPoint = pose * sp->closestPoint[1];	// pointerの近傍点
				Vec3d last_sPoint = sp->lastShapePoseW[0] * sp->lastClosestPoint[0];
				Vec3d normal = sp->normal;
				Vec3d lastNormal = sp->lastNormal;
				Posed shapePoseW = sp->shapePoseW[0];
				Posed lastShapePoseW = sp->lastShapePoseW[0];

				// 剛体の面の法線補間
				// 前回の法線と現在の法線の間を補間しながら更新
				double syncCount = pdt / hdt;
				double t = loopCount / syncCount;
				Vec3d interpolation_normal = interpolate(t, lastNormal, normal);
				Vec3d interpolation_sPoint = interpolate(t, last_sPoint, sPoint);
				Posed interpolation_pose = interpolate(t, lastShapePoseW, shapePoseW);
				Vec3d dir = pPoint - interpolation_sPoint;
					
				double dot = dir * interpolation_normal;
				if(dot < 0.0){
					for(int l = 0; l < (int)sp->intersectionVertices.size(); l++){
						Vec3d iv = sp->intersectionVertices[l];
						Vec3d wiv = hiSolid->GetPose() * iv; 	// ポインタの侵入点(world)
						dot = (wiv - sPoint) * interpolation_normal;	// デバイスの侵入点から中間面上の点へのベクトルのノルム（デバイスの侵入量）
						if(dot > 0.0)	continue;
						Vec3d ortho = dot * interpolation_normal; // 剛体の近傍点からデバイス侵入点までのベクトルを面法線へ射影
						PHIr ir;
						ir.solidID = solidID;
						ir.normal = interpolation_normal;
						ir.r = wiv - hiSolid->GetCenterPosition();
						ir.vertex = wiv;
						ir.solidPoint = wiv - ortho;		// solidの接触点(world)
						ir.ortho = ortho;
						ir.interpolation_pose = interpolation_pose;
						irs.push_back(ir);
					}
				}
			}
		}
	}
	SpatialVector outForce = SpatialVector();
	int NIrs = irs.size();
	if(NIrs > 0){
		double K = 100;
		double D = 0.1;
		K /= (double)NIrs;
		D /= (double)NIrs;
		for(int i = 0; i < NIrs; i++){
			PHIr ir = irs[i];
			PHSolid* hsolid = &hsolids->at(ir.solidID)->localSolid;
			Vec3d ortho = ir.ortho;
			Vec3d wiv = ir.vertex;
			Vec3d solidPoint = ir.solidPoint;
			Vec3d dv = GetPointVelocity(wiv) - hsolid->GetPointVelocity(solidPoint);
			Vec3d dvortho = dv.norm() * ir.normal;

			Vec3d addforce = -1 * (K * ortho + D * dvortho);
			double ws4 = pow(GetWorldScale(), 4);
			outForce.v() += addforce / ws4;
			outForce.w() = Vec3d();
		
	/*		ImpulsePoint ip;
			ip.contactPointW = interpolation_sPoint;
			ip.impulse = -1 * addforce;
			solidPair->impulsePoints.push_back(ip);*/	
		}
	}
		
	SetForce(outForce);
}

}
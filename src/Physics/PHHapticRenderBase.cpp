#include <Physics/PHHapticRenderBase.h>

namespace Spr{;

PHIrs PHHapticRenderBase::CompIntermediateRepresentation(PHHapticPointer* pointer, PHHapticRenderInfo hri){
	PHSolidsForHaptic* hsolids = hri.hsolids;
	PHSolidPairsForHaptic* sps = hri.sps;
	double pdt = hri.pdt;
	double hdt = hri.hdt;
	double loopCount = (double)hri.loopCount;
	bool bInterpolatePose = hri.bInterpolatePose;
	bool bMultiPoints = hri.bMultiPoints;

	const double syncCount = pdt / hdt;
	double t = loopCount / syncCount;
	if(t > 1.0) t = 1.0;
	
	PHIrs irs;
	int nNeighbors = (int)pointer->neighborSolidIDs.size();
	for(int i = 0; i < nNeighbors; i++){
		int solidID = pointer->neighborSolidIDs[i];
		PHSolidPairForHaptic* sp = sps->item(solidID, pointer->GetPointerID());
		PHSolid* curSolid[2];
		curSolid[0] = hsolids->at(solidID)->GetLocalSolid();
		curSolid[1] = DCAST(PHSolid, pointer);
		PHIrs tempIrs = sp->CompIntermediateRepresentation(curSolid, t, bInterpolatePose, bMultiPoints);
		if(tempIrs.size() == 0) continue;
		irs.insert(irs.end(), tempIrs.begin(), tempIrs.end());
	}
	return irs;
}

void PHHapticRenderBase::PenaltyBasedRendering(PHHapticRenderInfo hri){
	for(int p = 0; p < (int)hri.pointers->size(); p++){
		PHHapticPointer* pointer = hri.pointers->at(p);
		PHIrs irs = CompIntermediateRepresentation(pointer, hri);
		SpatialVector outForce = SpatialVector();
		int NIrs = irs.size();
		if(NIrs > 0){
			double K = 100;
			double D = 0.1;
			for(int i = 0; i < NIrs; i++){
				PHIr* ir = irs[i];
				Vec3d ortho = ir->depth * ir->normal;
				Vec3d dv = ir->pointerPointVel - ir->contactPointVel;
				Vec3d dvortho = dv.norm() * ir->normal;

				Vec3d addforce = K * ortho + D * dvortho;
				outForce.v() += addforce;
				outForce.w() += Vec3d();
			
				irs[i]->force = -1 * addforce * hri.hdt / hri.pdt;
				hri.hsolids->at(irs[i]->solidID)->AddForce(irs[i]->force, irs[i]->contactPointW);
			}
		}
		pointer->SetForce(outForce);
	}
}

void PHHapticRenderBase::ConstraintBasedRendering(PHHapticRenderInfo hri){
	//DSTR << "----------haptic rendering" << std::endl;
	for(int p = 0; p < (int)hri.pointers->size(); p++){
		PHHapticPointer* pointer = hri.pointers->at(p);

		// 中間表現を求める。摩擦状態を更新
		PHIrs irs = CompIntermediateRepresentation(pointer, hri);
		
		SpatialVector outForce = SpatialVector();
		int Nirs = irs.size();
		if(Nirs > 0){
			// プロキシ姿勢計算のための連立不等式
			VMatrixRow< double > c;
			c.resize(Nirs, Nirs);
			c.clear(0.0);
			VVector< double > d;
			d.resize(Nirs);
			d.clear(0.0);
			double massInv = pointer->GetMassInv();
			Matrix3d rotationMatInv =  (pointer->GetInertia() * pointer->GetRotationalWeight()).inv();
			for(int i = 0; i < Nirs; i++){
				for(int j = 0; j < Nirs; j++){
					Vec3d ri = irs[i]->r;
					Vec3d ni = irs[i]->normal;
					Vec3d rj = irs[j]->r;
					Vec3d nj = irs[j]->normal;

					// 並進拘束
					double trans = ni * nj * massInv;
					// 回転拘束
					double rotate = (ri % ni) *	rotationMatInv * (rj % nj);
					c[i][j] = trans + rotate;
				}
				d[i] = irs[i]->depth;
			}
			VVector< double > f;
			f.resize(Nirs);
			f.clear(0.0);

			// 連立不等式を解く
			// 球（1点接触）で摩擦ありの場合だと侵入解除に回転が
			// 含まれる。解は正しいが、プロキシの更新がうまくいかなくなるので、
			// 回転の重み行列をなるべく大きくする必要がある。
			GaussSeidel(c, f, -d);

			double k  = 50;
			double b = 0.0;

			// ポインタ移動量を求める
			Vec3d dr = Vec3d();
			Vec3d dtheta = Vec3d();
			Vec3d allDepth = Vec3d();
			for(int i = 0; i < Nirs; i++){
				f[i] = std::max(f[i], 0.0);
				// 並進量
				Vec3d tmpdr = f[i] * irs[i]->normal * massInv;
				dr += tmpdr;
				// 回転量
				Vec3d tmpdtheta = f[i] * rotationMatInv * (irs[i]->r % irs[i]->normal);
				dtheta += tmpdtheta;
				
				allDepth += -1 * irs[i]->normal * irs[i]->depth;
			}
			// プロキシ位置姿勢更新（目標位置姿勢解除状態）
			pointer->targetProxy.Ori() = ( Quaterniond::Rot(dtheta) * pointer->GetOrientation() ).unit();
			pointer->targetProxy.Pos() = pointer->GetFramePosition() + dr;

			// 相対位置計算用のプロキシ位置姿勢
			pointer->proxyPose.Pos() = pointer->targetProxy.Pos();
		
			/// 力覚インタフェースに出力する力の計算
			double hdt = hri.hdt;
			Vec3d last_dr = pointer->last_dr;
			Vec3d last_dtheta = pointer->last_dtheta;

			outForce.v() = k * dr  + b * (dr - last_dr)/hdt;
			//outForce.w() = (pointer->springOriK * dtheta + pointer->damperOriD * ((dtheta - last_dtheta)/hdt));
			pointer->last_dr = dr;
			pointer->last_dtheta = dtheta; 

			// 剛体に加える力を計算
			Vec3d ratio;
			double epsilon = 1e-10;
			for(int i = 0; i < 3; i++){
				ratio[i] = outForce.v()[i] / allDepth[i];
				if(abs(allDepth[i]) < epsilon) ratio[i] = 0.0;
			}
			for(int i = 0; i < Nirs; i++){
				Vec3d pointForce = Vec3d();	// 各接触点に働く力
				Vec3d dir = irs[i]->normal * irs[i]->depth;
				for(int j = 0; j < 3; j++){
					pointForce[j] = ratio[j] * dir[j] *  hri.hdt / hri.pdt;
				}
				irs[i]->force = pointForce;
				hri.sps->item(irs[i]->solidID, pointer->GetPointerID())->force = pointForce;
				hri.hsolids->at(irs[i]->solidID)->AddForce(pointForce, irs[i]->contactPointW);
			}
		}
		pointer->SetForce(outForce);
		//CSVOUT << outForce.v().y << std::endl;
	}
}

void PHHapticRenderBase::ReflectForce2Solid(PHSolidsForHaptic* hsolids, double hdt, double pdt){
//	DSTR << "----------" << std::endl;
	for(int i = 0; i < (int)hsolids->size(); i++){
		if(hsolids->at(i)->bPointer) continue;
		PHSolidForHaptic* hsolid = hsolids->at(i);
		PHSolid* sceneSolid = hsolid->sceneSolid;
		sceneSolid->AddForce(hsolid->force);
		sceneSolid->AddTorque(hsolid->torque);
		hsolid->force.clear();
		hsolid->torque.clear();
	}
}

void PHHapticRenderBase::VirtualCoupling(PHHapticPointer* pointer){
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


}
#include <Physics/PHHapticRenderBase.h>

namespace Spr{;

void PHHapticRenderBase::HapticRendering(PHHapticRenderInfo hri){
	for(int i = 0; i < (int)hri.pointers->size(); i++){
		PHHapticPointer* pointer = hri.pointers->at(i);
		PenaltyBasedRendering(pointer, hri);
	}
}

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
		curSolid[0] = &hsolids->at(solidID)->localSolid;
		curSolid[1] = DCAST(PHSolid, pointer);
		PHIrs tempIrs = sp->CompIntermediateRepresentation(curSolid, t, bInterpolatePose, bMultiPoints);
		if(tempIrs.size() == 0) continue;
		irs.insert(irs.end(), tempIrs.begin(), tempIrs.end());
	}
	return irs;
}

void PHHapticRenderBase::PenaltyBasedRendering(PHHapticPointer* pointer, PHHapticRenderInfo hri){

	PHIrs irs = CompIntermediateRepresentation(pointer, hri);

	SpatialVector outForce = SpatialVector();
	int NIrs = irs.size();
	if(NIrs > 0){
		double K = 100;
		double D = 0.1;
		for(int i = 0; i < NIrs; i++){
			PHIr* ir = &irs[i];
			Vec3d ortho = ir->depth * ir->normal;
			Vec3d dv = ir->pointerPointVel - ir->contactPointVel;
			Vec3d dvortho = dv.norm() * ir->normal;

			Vec3d addforce = -1 * (K * ortho + D * dvortho);
			double ws4 = pow(pointer->GetWorldScale(), 4);
			outForce.v() += addforce / ws4;
			outForce.w() += Vec3d();
		
			ir->force = -1 * addforce;
		}
		pointer->allIrs.insert(pointer->allIrs.end(), irs.begin(), irs.end());
	}
	pointer->SetForce(outForce);
}

void PHHapticRenderBase::ConstraintBasedRendering(PHHapticRenderInfo hri){
	DSTR << "----------haptic rendering" << std::endl;
	for(int p = 0; p < (int)hri.pointers->size(); p++){
		PHHapticPointer* pointer = hri.pointers->at(p);

		// 中間表現を求める。摩擦状態を更新
		PHIrs irs = CompIntermediateRepresentation(pointer, hri);
		
		SpatialVector outForce = SpatialVector();
		int Nirs = irs.size();
		if(Nirs > 0){
#if 1
			Vec3d debug;
			// 摩擦
			for(int i = 0; i < Nirs; i++){
				double mu = irs[i].mu;	// 動摩擦係数
				mu = 0.5;
				
				double l = mu * irs[i].depth; // 摩擦円錐半径
				Vec3d vps = irs[i].pointerPointW;
				Vec3d vq = irs[i].solidPair->relativePose * irs[i].pointerPointW;
				Vec3d dq = (vq - vps) * irs[i].normal * irs[i].normal;
				Vec3d vqs = vq - dq;
				Vec3d tangent = vqs - vps;

				DSTR << "vps" << vps << std::endl;
				DSTR << "vq" << vq << std::endl;
				DSTR << "tangent " << tangent << tangent.norm() << std::endl;

				PHIr ir;
				ir = irs[i];
				double epsilon = 1e-5;
				if(tangent.norm() < epsilon){
					// 静止状態
					DSTR << "rest" << std::endl;
				}
				if(epsilon < tangent.norm() && tangent.norm() <= l){
					 //静摩擦（静止摩擦半径内）
					ir.normal = tangent.unit();
					ir.depth = tangent.norm();
					irs.push_back(ir);
					DSTR << "static friction" << std::endl;
				}
				if(epsilon < l && l < tangent.norm()){
					// 動摩擦
					ir.normal = tangent.unit();
					ir.depth = l;
					irs.push_back(ir);
					DSTR << "dynamic friction" << std::endl;
				}
			}
#endif
			// プロキシ姿勢計算のための連立不等式
			Nirs = irs.size();
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
					Vec3d ri = irs[i].r;
					Vec3d ni = irs[i].normal;
					Vec3d rj = irs[j].r;
					Vec3d nj = irs[j].normal;

					// 並進拘束
					double trans = ni * nj * massInv;
					// 回転拘束
					double rotate = (ri % ni) *	rotationMatInv * (rj % nj);
					c[i][j] = trans + rotate;
				}
				d[i] = irs[i].depth;
			}
			VVector< double > f;
			f.resize(Nirs);
			f.clear(0.0);

			// 連立不等式を解く
			// 球（1点接触）で摩擦ありの場合だと侵入解除に回転が
			// 含まれる。解は正しいが、プロキシの更新がうまくいかなくなるので、
			// 回転の重み行列をなるべく大きくする必要がある。
			GaussSeidel(c, f, -d);

			// ポインタ移動量を求める
			Vec3d dr = Vec3d();
			Vec3d dtheta = Vec3d();
			double fall = 0.0;
			for(int i = 0; i < Nirs; i++){
				f[i] = std::max(f[i], 0.0);
				// 並進量
				Vec3d tmpdr = f[i] * irs[i].normal * massInv;
				dr += tmpdr;
				// 回転量
				Vec3d tmpdtheta = f[i] * rotationMatInv * (irs[i].r % irs[i].normal);
				dtheta += tmpdtheta;
				// 力覚計算の比率
				fall += f[i];
			}
			// プロキシ位置姿勢更新（目標位置姿勢解除状態）
			pointer->targetProxy.Ori() = ( Quaterniond::Rot(dtheta) * pointer->GetOrientation() ).unit();
			pointer->targetProxy.Pos() = pointer->GetFramePosition() + dr;

			// 相対位置計算用のプロキシ位置姿勢
			pointer->proxyPose.Pos() = pointer->targetProxy.Pos();
		

			/// 力覚インタフェースに出力する力の計算
			double ws4 = pow(pointer->GetWorldScale(), 4);
			double hdt = hri.hdt;
			Vec3d last_dr = pointer->last_dr;
			Vec3d last_dtheta = pointer->last_dtheta;

			outForce.v() = 50 * dr  + 0.0 * (dr - last_dr)/hdt;
			//outForce.w() = (pointer->springOriK * dtheta + pointer->damperOriD * ((dtheta - last_dtheta)/hdt));
			pointer->last_dr = dr;
			pointer->last_dtheta = dtheta; 

			// 剛体に加える力を計算
			for(int i = 0; i < Nirs; i++){
				f[i] = std::max(f[i], 0.0);
				irs[i].force = -1 * outForce.v().norm() * irs[i].normal.unit() * f[i] / fall;
			}
		}
		pointer->SetForce(outForce);
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
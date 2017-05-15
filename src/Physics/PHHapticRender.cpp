#include <Physics/PHHapticRender.h>
#include <Physics/PHHapticEngine.h>
#include <Foundation/QuadProg++.hh>

namespace Spr{;
PHHapticRenderInfo::PHHapticRenderInfo() {
	loopCount = 0;
	pdt = 0.02f;
	hdt = 0.001f;
	bInterpolatePose = true;
}

PHHapticRender::PHHapticRender(){
}

void PHHapticRender::HapticRendering(PHHapticRenderInfo info){
	*(PHHapticRenderInfo*)this = info;
	for(int i = 0; i < (int)pointers->size(); i++){
		PHHapticPointer* pointer = pointers->at(i);
		pointer->hapticForce.clear();
		switch (pointer->renderMode){
			case PHHapticPointerDesc::PENALTY:
				PenaltyBasedRendering(pointer);
				break;
			case PHHapticPointerDesc::CONSTRAINT:
				ConstraintBasedRendering(pointer);
				break;
			case PHHapticPointerDesc::DYNAMICS_CONSTRAINT:
				DynamicsConstraintRendering(pointer);
				break;
		}
 		VibrationRendering(pointer);
	}
}

PHIrs PHHapticRender::CompIntermediateRepresentation(PHHapticPointer* pointer){
	PHIrs irs;
	int nNeighbors = (int)pointer->neighborSolidIDs.size();
	for(int i = 0; i < nNeighbors; i++){
		int solidID = pointer->neighborSolidIDs[i];
		PHSolidPairForHaptic* sp = sps->item(solidID, pointer->GetPointerID());
		PHIrs tempIrs = sp->CompIntermediateRepresentation(this, hsolids->at(solidID)->GetLocalSolid(), pointer);
		if(tempIrs.size() == 0) continue;
		irs.insert(irs.end(), tempIrs.begin(), tempIrs.end());
	}
	return irs;
}

// 力覚レンダリング
// （とりあえず欲しい）
// 凸形状が使えるPenaltyBasedRendering(multiple point intermediate representation)
// 凸形状が使えるconstratint based rendering
// 凸形状が使えるvirtual coupling
// （余力があれば）
// 球型ポインタのみが使えるProxy3DoF(池田くんが作った奴）
// proxyに質量を与え、stick-slip frictionが提示できるproxy simulation


// 各接触頂点の侵入量にバネ係数をかけて合計したものを出力
// バネ係数が高いと不安定になるので低めに（1000N/m)程度に抑える必要あり
void PHHapticRender::PenaltyBasedRendering(PHHapticPointer* pointer){
	// 摩擦は追加できないので、強制的にfalse
	const bool bFric = pointer->bFriction;
	if(pointer->bFriction){
		pointer->EnableFriction(false);
	}
	PHIrs irs = CompIntermediateRepresentation(pointer);
	SpatialVector outForce = SpatialVector();
	int NIrs = (int)irs.size();
	if(NIrs > 0){
		for(int i = 0; i < NIrs; i++){
			PHIr* ir = irs[i];
			Vec3d ortho = ir->depth * ir->normal;
			Vec3d dv = ir->pointerPointVel - ir->contactPointVel;
			Vec3d dvortho = dv.norm() * ir->normal;

			float K  = pointer->reflexCoeff.spring / pointer->GetPosScale();
			float D = pointer->reflexCoeff.damper / pointer->GetPosScale();

			Vec3d addforce = K * ortho + D * dvortho;
			outForce.v() += addforce;
			
			irs[i]->force = -1 * addforce;
			hsolids->at(irs[i]->solidID)->AddForce(irs[i]->force, irs[i]->contactPointW);
			PHSolid* localSolid = &hsolids->at(irs[i]->solidID)->localSolid;
			PHSolidPairForHaptic* sp = sps->item(irs[i]->solidID, pointer->GetPointerID());
			sp->force += irs[i]->force;	// あるポインタが剛体に加える力
			sp->torque += (irs[i]->contactPointW - localSolid->GetCenterPosition()) ^ irs[i]->force;
		}
	}
	pointer->AddHapticForce(outForce);
	pointer->EnableFriction(bFric);
}

void PHHapticRender::SolveProxyPose(Vec3d& dr, Vec3d& dtheta, Vec3d& allDepth, PHHapticPointer* pointer, const PHIrs& irs) {
#if 1//	変数をdrとdThetaにして、QPを解く
	double mass = pointer->GetMass();
	Matrix3d rotationMat = pointer->GetInertia() * pointer->GetRotationalWeight();
	static VMatrixRow<double> G;
	G.resize(6, 6);
	G.clear();
	G[0][0] = G[1][1] = G[2][2] = mass;
	G.vsub_matrix(3, 3, 3, 3) = rotationMat;

	static VMatrixRow< double > c;
	c.resize(6, irs.size());
	c.clear(0.0);
	static VVector< double > d;
	d.resize(irs.size());
	d.clear(0.0);

	for (size_t i = 0; i < irs.size(); i++) {
		c.col(i).v_range(0, 3) = irs[i]->normal;
		c.col(i).v_range(3, 3) = irs[i]->r ^ irs[i]->normal;
		d[i] = -irs[i]->depth;
	}
	VVector<double> x;
	x.resize(6);
	x.clear(0.0);
	//	min 0.5 * x G x + g0 x
	//		s.t.
	//		CE^T x + ce0 = 0
	//		CI^T x + ci0 >= 0
	//		double solve_quadprog(G, g0, CE, ce0, CI, ci0, x);
	static VVector<double> g0, ce0, ci0;
	static VMatrixRow<double> CE;
	g0.resize(6);
	g0.clear(0.0);
	ce0.resize(0);
	CE.resize(6, 0);
	solve_quadprog(G, g0, CE, ce0, c, d, x);
/*	DSTR << "-----------------------------" << std::endl;
	DSTR << c << std::endl;
	DSTR << x << std::endl;
	DSTR << d << std::endl;
*/
	dr = x.v_range(0, 3);
	dtheta = x.v_range(3, 3);
	for (size_t i = 0; i < irs.size(); i++) {
		allDepth += -1 * irs[i]->normal * irs[i]->depth;
	}
#else
	// プロキシ姿勢計算のための連立不等式
	VMatrixRow< double > c;
	c.resize(irs.size(), irs.size());
	c.clear(0.0);
	VVector< double > d;
	d.resize(irs.size());
	d.clear(0.0);
	double massInv = pointer->GetMassInv();
	Matrix3d rotationMatInv = (pointer->GetInertia() * pointer->GetRotationalWeight()).inv();
	for (int i = 0; i < irs.size(); i++) {
		for (int j = 0; j < irs.size(); j++) {
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
	VVector<double> f;
	f.resize(irs.size());
	f.clear(0.0);

	// 連立不等式を解く
	// 球（1点接触）で摩擦ありの場合だと侵入解除に回転が
	// 含まれる。解は正しいが、プロキシの更新がうまくいかなくなるので、
	// 回転の重み行列をなるべく大きくする必要がある。
	// 回転の重み行列を大きくするとプロキシの回転移動がなくなるため、回転摩擦がでなくなる
	GaussSeidel(c, f, -d);

	for (int i = 0; i < irs.size(); i++) {
		f[i] = std::max(f[i], 0.0);
		// 並進量
		Vec3d tmpdr = f[i] * irs[i]->normal * massInv;
		dr += tmpdr;
		// 回転量
		Vec3d tmpdtheta = f[i] * rotationMatInv * (irs[i]->r % irs[i]->normal);
		dtheta += tmpdtheta;

		allDepth += -1 * irs[i]->normal * irs[i]->depth;
	}
#endif
}

Vec3d GetOrthogonalVector(Vec3d n) {
	double max=0;
	int i1, i2;
	for (int i = 0; i < 3; ++i) {
		if (std::abs(n[i]) > max) {
			max = std::abs(n[i]);
			i1 = i;
		}
	}
	if (n[(i1 + 1) % 3] > n[(i1 + 2) % 3]) i2 = (i1 + 1) % 3;
	else i2 = (i1 + 2) % 3;
	Vec3d u = n;
	std::swap(u[i1], u[i2]);
	u -= (u * n) * n;
	return u.unit();
}

void PHHapticRender::ConstraintBasedRendering(PHHapticPointer* pointer){
	// プロキシの状態の保存と更新
	pointer->lastProxyPose = Posed(pointer->proxyPose.Pos(), pointer->GetOrientation());

	// 中間表現を求める。摩擦状態を更新
	PHIrs irs = CompIntermediateRepresentation(pointer);
	
	SpatialVector outForce = SpatialVector();
	if(irs.size() > 0){
		// ポインタ移動量を求める
		Vec3d dr, dtheta, allDepth;
		SolveProxyPose(dr, dtheta, allDepth, pointer, irs);

		// プロキシ位置姿勢更新（目標位置姿勢解除状態）
		pointer->proxyPose.Ori() = ( Quaterniond::Rot(dtheta) * pointer->GetOrientation() ).unit();
		pointer->proxyPose.Pos() = pointer->GetFramePosition() + dr;
		
		/// 力覚インタフェースに出力する力の計算
		Vec3d last_dr = pointer->last_dr;
		Vec3d last_dtheta = pointer->last_dtheta;

		float K  = pointer->reflexCoeff.spring / pointer->GetPosScale();
		float D = pointer->reflexCoeff.damper / pointer->GetPosScale();
		float KOri = pointer->reflexCoeff.rotationSpring * pointer->GetRotationalWeight();
		float DOri = pointer->reflexCoeff.rotationDamper * pointer->GetRotationalWeight();

		outForce.v() = K * dr  + D * (dr - last_dr)/hdt;
		outForce.w() = KOri * dtheta + DOri * ((dtheta - last_dtheta)/hdt);
		pointer->last_dr = dr;
		pointer->last_dtheta = dtheta; 

		// 剛体に加える力を計算
		// レンダリングした力から各接触点に働く力を逆算
		// うまくいってない可能性がある
		Vec3d ratio;
		double epsilon = 1e-10;
		for(int i = 0; i < 3; i++){
			ratio[i] = outForce.v()[i] / allDepth[i];
			if(abs(allDepth[i]) < epsilon) ratio[i] = 0.0;
		}
		//DSTR << "all" << outForce << std::endl;
		//DSTR << "ratio" << ratio << std::endl;
		//DSTR << "NIrs" << irs.size() << std::endl;
		for(size_t i = 0; i < irs.size(); i++){
			Vec3d dir = irs[i]->normal * irs[i]->depth;
			for(int j = 0; j < 3; j++){
				irs[i]->force[j] = ratio[j] * dir[j];
			}
			//DSTR << "pos" << irs[i]->contactPointW << std::endl;
			//DSTR << "depth" << irs[i]->depth << std::endl;
			//DSTR << "irs[i]->force" << irs[i]->force << std::endl;
			hsolids->at(irs[i]->solidID)->AddForce(irs[i]->force, irs[i]->contactPointW);	// 各ポインタが剛体に加えた全ての力
			PHSolid* localSolid = &hsolids->at(irs[i]->solidID)->localSolid;
			PHSolidPairForHaptic* sp = sps->item(irs[i]->solidID, pointer->GetPointerID());
			sp->force += irs[i]->force;	// あるポインタが剛体に加える力
			sp->torque += (irs[i]->contactPointW - localSolid->GetCenterPosition()) ^ irs[i]->force;
			//DSTR << sp->force << std::endl;
			//DSTR << irs[i]->contactPointW << " " << localSolid->GetCenterPosition() << std::endl;
			//DSTR << irs[i]->contactPointW - localSolid->GetCenterPosition() << std::endl;
		}
	}
	else {
		pointer->proxyPose = pointer->GetPose();
	}
	pointer->AddHapticForce(outForce);
	//DSTR << pointer->GetFramePosition() << std::endl;
	//DSTR << "render" << outForce << std::endl;
	//CSVOUT << outForce[0] << "," << outForce[1] << "," << outForce[2] << "," << outForce[3] << "," << outForce[4] << "," << outForce[5] << "," <<std::endl;
}


PHIrs PHHapticRender::CompIntermediateRepresentationShapeLevel(PHSolid* solid0, PHHapticPointer* pointer, 
	PHSolidPairForHaptic* so, PHShapePairForHaptic* sh, Posed curShapePoseW[2], double t, bool bInterpolatePose, bool bPoints) {
	Vec3d sPoint = curShapePoseW[0] * sh->closestPoint[0];	// 今回のsolidの近傍点（World)
	Vec3d pPoint = curShapePoseW[1] * sh->closestPoint[1];	// 今回のpointerの近傍点（World)
	Vec3d last_sPoint = sh->lastShapePoseW[0] * sh->lastClosestPoint[0]; // 前回の剛体近傍点（World)
	Vec3d interpolation_normal = sh->normal;				// 補間法線
	Vec3d interpolation_sPoint = sPoint;					// 補間剛体近傍点
	// 剛体の面の法線補間　前回の法線と現在の法線の間を補間
	interpolation_normal = interpolate(t, sh->lastNormal, sh->normal);
	Vec3d dir = pPoint - interpolation_sPoint;
	double dot = dir * interpolation_normal;
	//DSTR << sPoint << std::endl;
	//DSTR << curShapePoseW[0] << "," << closestPoint[0] << std::endl;

	sh->irs.clear();
	if (dot >= 0.0) return PHIrs();
	if (bPoints) {
		for (size_t i = 0; i < sh->intersectionVertices.size(); i++) {
			Vec3d iv = sh->intersectionVertices[i];
			Vec3d wiv = curShapePoseW[1] * iv; 	// ポインタの侵入点(world)
			dot = (wiv - interpolation_sPoint) * interpolation_normal;	// デバイスの侵入点から中間面上の点へのベクトルのノルム（デバイスの侵入量）
			if (dot > 0.0)	continue;
			PHIr* ir = DBG_NEW PHIr(so, so->solidID[0], sh);
			ir->normal = interpolation_normal;
			ir->pointerPointW = wiv;
			Vec3d ortho = dot * interpolation_normal; // 剛体の近傍点からデバイス侵入点までのベクトルを面法線へ射影
			ir->contactPointW = wiv - ortho;		// solidの接触点(world)
			ir->depth = ortho.norm();
			ir->interpolation_pose = curShapePoseW[0];
			sh->irs.push_back(ir);
		}
	}
	else {
		PHIr* ir = DBG_NEW PHIr(so, so->solidID[0], sh);
		ir->normal = interpolation_normal;
		ir->pointerPointW = pPoint;
		Vec3d ortho = dot * interpolation_normal;
		ir->contactPointW = pPoint - ortho;
		ir->depth = ortho.norm();
		ir->interpolation_pose = curShapePoseW[0];
		sh->irs.push_back(ir);
	}
	for (size_t i = 0; i < sh->irs.size(); i++) {
		PHIr* ir = sh->irs[i];
		ir->r = ir->pointerPointW - pointer->GetCenterPosition();
		ir->contactPointVel = solid0->GetPointVelocity(ir->contactPointW);
		ir->pointerPointVel = pointer->GetPointVelocity(ir->pointerPointW);
	}
	return PHIrs(sh->irs.begin(), sh->irs.end());
}

PHIrs PHHapticRender::CompIntermediateRepresentationForDynamicsConstraint(PHHapticPointer* pointer) {
	PHIrs irsAll;
	int nNeighbors = (int)pointer->neighborSolidIDs.size();
	for (int i = 0; i < nNeighbors; i++) {
		int solidID = pointer->neighborSolidIDs[i];
		PHSolidPairForHaptic* sp = sps->item(solidID, pointer->GetPointerID());

		//--------------------------------------------------
		/* 力覚安定化のための補間
		// Impulseの場合は相手の剛体のPoseの補間が必要。
		// LocalDynamicsの場合は法線の補間のみでよい。
		// 法線の補間はPHShapePairForHapticでやる。h
		*/
		PHSolid* solid0 = hsolids->at(solidID)->GetLocalSolid();
		const double syncCount = pdt / hdt;
		double t = loopCount / syncCount;
		if (t > 1.0) t = 1.0;

		sp->force.clear();
		sp->torque.clear();
		sp->lastInterpolationPose = sp->interpolationPose;
		sp->interpolationPose = solid0->GetPose();
		if (bInterpolatePose) {
			Posed cur = solid0->GetPose();
			double dt = ((PHScene*)solid0->GetScene())->GetTimeStep();
			Posed last;
			last.Pos() = cur.Pos() - (solid0->GetVelocity() * dt + solid0->GetOrientation() * solid0->dV.v());
			last.Ori() = (cur.Ori() * Quaterniond::Rot(-solid0->v.w() * dt + -solid0->dV.w())).unit();
			sp->interpolationPose = interpolate(t, last, cur);
		}
		// 接触したとして摩擦計算のための相対位置を計算
		// 相対摩擦
		if (sp->frictionState == PHSolidPairForHaptic::FREE) {
			sp->frictionState = PHSolidPairForHaptic::STATIC;
			sp->contactCount = 0;
			sp->fricCount = 0;
			sp->initialRelativePose = pointer->GetPose() * sp->interpolationPose.Inv();
		}
		else {
			sp->contactCount++;
			sp->initialRelativePose = pointer->lastProxyPose * sp->lastInterpolationPose.Inv();
		}
		sp->relativePose = sp->initialRelativePose * sp->interpolationPose * pointer->GetPose().Inv();
		//DSTR << "pose" << pointer->GetPose() << std::endl;
		//DSTR << "lastProxy" << pointer->lastProxyPose << std::endl;
		//DSTR << "ini" << initialRelativePose << std::endl;
		//DSTR << "relativePose" << relativePose << std::endl;

		// 中間表現の作成
		PHIrs irs;
		for (int i = 0; i < solid0->NShape(); i++) {
			for (int j = 0; j < pointer->NShape(); j++) {
				PHShapePairForHaptic* spHaptic = sp->GetShapePair(i, j);
				Posed curShapePoseW[2];
				curShapePoseW[0] = sp->interpolationPose * solid0->GetShapePose(i);
				curShapePoseW[1] = pointer->GetPose() * pointer->GetShapePose(j);
				PHIrs sirs = CompIntermediateRepresentationShapeLevel(solid0, pointer, sp, spHaptic, curShapePoseW, t, bInterpolatePose, pointer->bMultiPoints);
				irs.insert(irs.end(), sirs.begin(), sirs.end());
			}
		}
		if (irs.size() == 0) {
			// 接触なし
			sp->frictionState = PHSolidPairForHaptic::FREE;
			sp->initialRelativePose = Posed();
			sp->relativePose = Posed();
		}
		//--------------------------------------------------
		if (irs.size()) {
			irsAll.insert(irsAll.end(), irs.begin(), irs.end());
		}
	}
	return irsAll;
}

void PHHapticRender::SolveProxyVelocity(PHHapticPointer* pointer, const PHIrs& irs) {
	double mass = pointer->GetMass();
	Matrix3d rotationMat = pointer->GetInertia() * pointer->GetRotationalWeight();
	static VMatrixRow<double> G;
	G.resize(6, 6);
	G.clear();
	G[0][0] = G[1][1] = G[2][2] = mass;
	G.vsub_matrix(3, 3, 3, 3) = rotationMat;

	static VMatrixRow< double > c;
	c.resize(6, irs.size());
	c.clear(0.0);
	static VVector< double > d;
	d.resize(irs.size());
	d.clear(0.0);

	for (size_t i = 0; i < irs.size(); i++) {
		c.col(i).v_range(0, 3) = irs[i]->normal;
		c.col(i).v_range(3, 3) = irs[i]->r % irs[i]->normal;
		d[i] = (pointer->proxyVelocity.v() + (pointer->proxyVelocity.w() % (irs[i]->pointerPointW - pointer->GetCenterPosition())) - irs[i]->contactPointVel) * irs[i]->normal;
	}
	VVector<double> x;
	x.resize(6);
	x.clear(0.0);
	//	min 0.5 * x G x + g0 x
	//		s.t.
	//		CE^T x + ce0 = 0
	//		CI^T x + ci0 >= 0
	//		double solve_quadprog(G, g0, CE, ce0, CI, ci0, x);
	static VVector<double> g0, ce0, ci0;
	static VMatrixRow<double> CE;
	g0.resize(6);
	g0.clear(0.0);
	ce0.resize(0);
	CE.resize(6, 0);
	solve_quadprog(G, g0, CE, ce0, c, d, x);
/*	DSTR << "-----------------------------" << std::endl;
	DSTR << c << std::endl;
	DSTR << x << std::endl;
	DSTR << d << std::endl;*/
	pointer->proxyVelocity += x;
}

void PHHapticRender::SolveFrictionForce(PHHapticPointer* pointer, const PHIrs& irs) {
	if (!pointer->bFriction) return;
	//	摩擦
	// 0 < f dt < μN dt のとき、速度0、それ以外の時 fdt = μNdt で 速度 > 0
	//	lambdaは力積
	//	LCPでfを解く
	//	fから剛体の速度変化を求める
	//	Mv(t+1) = f(t+1)*dt + Mv(t)
	//	v(t+1) = M^-1 f(t+1)*dt + v(t)
	//	J v(t+1) = J M-^1 J^T lambda(t+1) * dt + J v(t)
	//	w(t+1) = dt * A lambda(t+1) + w(t), -μNdt < lambda(t+1) < μNdt
	//	摩擦力の向き = u, w = u*v + u*(ω x p) = u*v + u*(ω x p)
	//	ω x p = (ωy pz - ωz py,  ωz px - ωx pz,  ωx py - ωy px) 
	//	u*(ω x p) = ux ωy pz - ux ωz py + uy ωz px - uy ωx pz + uz ωx py - uz ωy px
	//			  = (py uz - pz uy)wx +  (pz ux - px uz)ωy + (px uy - py ux)wz 
	//	J = (ux, uy, uz, p x u)
	//	A = J M^-1 J^T
	double massInv = pointer->GetMassInv();
	Matrix3d rotationMatInv = (1.0/pointer->GetRotationalWeight()) * pointer->GetInertiaInv();
	std::vector<Vec3d> rVel(irs.size()), u(irs.size()), v(irs.size());
	std::vector<double> rVelNorm(irs.size());
	for (size_t i = 0; i < irs.size(); ++i) {
		SpatialVector pVel = pointer->lastProxyVelocity;
		rVel[i] = pVel.v() + (pVel.w() ^ (irs[i]->pointerPointW - pointer->GetCenterPosition()))			
			- irs[i]->contactPointVel;							//	相対速度
		rVel[i] -= (rVel[i] * irs[i]->normal) * irs[i]->normal;	//	接線成分を削除
#if _DEBUG
		if (isnan(rVel[i].x)) {
			DSTR << rVel[i];
			assert(0);
		}
#endif
		rVelNorm[i] = rVel[i].norm();
		//	接線２方向を計算
		u[i] = (rVelNorm[i] > 1e-12) ? rVel[i] / rVelNorm[i] : GetOrthogonalVector(irs[i]->normal);
		v[i] = u[i] % irs[i]->normal;
	}

	VMatrixRow< double > A;
	A.resize(irs.size() * 2, irs.size() * 2);
	A.clear(0.0);
	VVector< double > w, minFt, maxFt;
	w.resize(irs.size() * 2);	minFt.resize(irs.size() * 2);	maxFt.resize(irs.size() * 2);
	w.clear();	minFt.clear(); maxFt.clear();
	PHShapePairForHaptic* sh = NULL;

	for (size_t i = 0; i < irs.size(); i++) {
		//	摩擦係数の計算
		double mu = 0;
		if (irs[i]->shapePair != sh) {
			sh = irs[i]->shapePair;
			if (pointer->bTimeVaryFriction) {	//	時変摩擦を使う場合
				if (irs[i]->solidPair->frictionState == PHSolidPairForHapticSt::STATIC) {	//	静止摩擦
					mu = sh->mu + sh->mu*(sh->timeVaryFrictionA * log(1 + sh->timeVaryFrictionB * irs[i]->solidPair->fricCount * hdt));
				}
				else {							//	動摩擦
					mu = sh->mu;
				}
			}
			else {	//	時不変の定数を使う場合
				mu = sh->mu;
				if (irs[i]->solidPair->frictionState == PHSolidPairForHapticSt::STATIC) mu = sh->mu0;
			}
		}
		//	minFt, maxFtの計算
		maxFt[i * 2] = maxFt[i * 2 + 1] = mu * irs[i]->force.norm() * hdt;
		minFt[i * 2] = minFt[i * 2 + 1] = -maxFt[i*2];
#if 0
		if (irs[i]->force.norm() > 0.1){
			DSTR << "irs[" << i << "]->force : " << irs[i]->force << " ";
			DSTR << "penetration: " << pointer->proxyPose.Pos() - pointer->GetPose().Pos() << std::endl;
		}
#endif
		//	A行列の計算
		for (size_t j = 0; j < irs.size(); j++) {
			//	J = (ux, uy, uz, p x u)
			// 並進成分
			double trans11 = u[i] * u[j] * massInv;
			double trans12 = v[i] * u[j] * massInv;
			double trans21 = u[i] * v[j] * massInv;
			double trans22 = v[i] * v[j] * massInv;
			// 回転成分
			double rotate11 = (irs[i]->r % u[i]) * rotationMatInv * (irs[j]->r % u[j]);
			double rotate12 = (irs[i]->r % u[i]) * rotationMatInv * (irs[j]->r % v[j]);
			double rotate21 = (irs[i]->r % v[i]) * rotationMatInv * (irs[j]->r % u[j]);
			double rotate22 = (irs[i]->r % v[i]) * rotationMatInv * (irs[j]->r % v[j]);
			A[i * 2][j * 2] = trans11 + rotate11;
			A[i * 2 + 1][j * 2] = trans21 + rotate21;
			A[i * 2][j * 2 + 1] = trans12 + rotate12;
			A[i * 2 + 1][j * 2 + 1] = trans22 + rotate22;
		}
		//	wベクトル
		w[i * 2] = rVelNorm[i];
	}
	VVector< double > ft;
	ft.resize(irs.size() * 2);
	GaussSeidelMinMax(ft, minFt, maxFt, A, w);

#if 0
	DSTR << "SolveFrictionForce--------------------" << std::endl;
	DSTR << "A=" << A << std::endl;
	DSTR << "w=" << w << std::endl;
	DSTR << "ft=" << ft << std::endl;
	DSTR << "minFt=" << minFt << std::endl;
	DSTR << "maxFt=" << maxFt << std::endl;
	DSTR << "--------------------------------------" << std::endl;
#endif

	//	proxyの速度と摩擦状態の更新、剛体への摩擦力の追加
	Vec3d proxyFt, proxyTt;
	PHSolidPairForHaptic* sp = irs[0]->solidPair;
	bool bProj = false;
	for (size_t i = 0; i < irs.size(); ++i) {
		//	摩擦状態の更新
		if (maxFt[2 * i] - minFt[2 * i] > 1e-8) {
			if (ft[2 * i] == minFt[2 * i] || ft[2 * i] == maxFt[2 * i]) bProj = true;
		}
		if (maxFt[2 * i + 1] - minFt[2 * i + 1] > 1e-8) {
			if (ft[2 * i + 1] == minFt[2 * i + 1] || ft[2 * i + 1] == maxFt[2 * i + 1]) bProj = true;
		}
		if (irs[i]->solidPair != sp || i == irs.size()-1) {
			sp->fricCount++;
			if (bProj) {
				if (sp->frictionState != PHSolidPairForHapticSt::DYNAMIC) {
					DSTR << sp->fricCount << " ";
					sp->fricCount = 0;
					sp->frictionState = PHSolidPairForHapticSt::DYNAMIC;
				}
			}
			else {
				if (sp->frictionState != PHSolidPairForHapticSt::STATIC) {
					sp->fricCount = 0;
					sp->frictionState = PHSolidPairForHapticSt::STATIC;
				}
			}
			sp = irs[i]->solidPair;
			bProj = false;
		}
		//	ポインタへの力の追加
		Vec3d pft = u[i] * ft[2 * i];
		Vec3d ptt = (irs[i]->pointerPointW - pointer->GetCenterPosition()) % pft;
		proxyFt += pft;
		proxyTt += ptt;
		pft = v[i] * ft[2 * i + 1];
		ptt = (irs[i]->pointerPointW - pointer->GetCenterPosition()) % pft;
		proxyFt += pft;
		proxyTt += ptt;
	}
	pointer->lastProxyVelocity.v() += massInv * proxyFt;
	pointer->lastProxyVelocity.w() += rotationMatInv * proxyTt;
	//DSTR << "lastProxyVelocity@AfterFric: " << pointer->lastProxyVelocity << std::endl;
}

void PHHapticRender::DynamicsConstraintRendering(PHHapticPointer* pointer) {
	//DSTR << "lastProxyVelocity@Start: " << pointer->lastProxyVelocity << std::endl;
	// 中間表現を求める。摩擦状態を更新
	PHIrs irs = CompIntermediateRepresentationForDynamicsConstraint(pointer);
	SpatialVector outForce = SpatialVector();
	if (irs.size() == 0) {
		pointer->proxyPose = pointer->GetPose();
		pointer->proxyVelocity.v() = pointer->GetVelocity();
		pointer->proxyVelocity.w() = pointer->GetAngularVelocity();
		pointer->bLastContactState = false;
	}else{
		// ポインタ移動量を求める
		Vec3d dr, dtheta, allDepth;
		SolveProxyPose(dr, dtheta, allDepth, pointer, irs);
		// ポインタを中間表現の外に追い出した点を、proxyPoseとする。
		pointer->proxyPose.Ori() = (Quaterniond::Rot(dtheta) * pointer->GetOrientation()).unit();
		pointer->proxyPose.Pos() = pointer->GetFramePosition() + dr;
		//	初回接触時のみ、 lastProxyをProxy合わせる。
		if (!pointer->bLastContactState) {
			pointer->lastProxyPose = pointer->proxyPose;
			pointer->lastProxyVelocity = pointer->proxyVelocity;
			pointer->bLastContactState = true;
		}
		//DSTR << "lastProxyVelocity@AfterFirstInit: " << pointer->lastProxyVelocity << std::endl;
		//	proxyの速度＝ポインタの速度を中間表現法線向きに制限したもの を求める
		SolveProxyVelocity(pointer, irs);

		//	lastProxy と proxyの間にバネダンパを考えて、lastProxyの速度を更新する。
		float KF = pointer->frictionCoeff.spring / pointer->GetPosScale();
		float DF = pointer->frictionCoeff.damper / pointer->GetPosScale();
		float KOriF = pointer->frictionCoeff.rotationSpring;
		float DOriF = pointer->frictionCoeff.rotationDamper;
		Vec3f f = KF * (pointer->proxyPose.Pos() - pointer->lastProxyPose.Pos())
			+ DF * (pointer->proxyVelocity.v() - pointer->lastProxyVelocity.v());
		Vec3f t = KOriF * (pointer->proxyPose.Ori() * pointer->lastProxyPose.Ori().Inv()).Rotation()
			+ DOriF * (pointer->proxyVelocity.w() - pointer->lastProxyVelocity.w());
		pointer->lastProxyVelocity.v() += pointer->GetMassInv() * f * hdt;
		pointer->lastProxyVelocity.w() += pointer->GetInertiaInv() * t * hdt;

		//DSTR << "lastProxyVelocity@AfterSpring: " << pointer->lastProxyVelocity << std::endl;


		/// 力覚インタフェースに出力する力の計算
		Vec3d last_dr = pointer->last_dr;
		Vec3d last_dtheta = pointer->last_dtheta;

		float K = pointer->reflexCoeff.spring / pointer->GetPosScale();
		float D = pointer->reflexCoeff.damper / pointer->GetPosScale();
		float KOri = pointer->reflexCoeff.rotationSpring * pointer->GetRotationalWeight();
		float DOri = pointer->reflexCoeff.rotationDamper * pointer->GetRotationalWeight();

		outForce.v() = K * dr + D * (dr - last_dr) / hdt;
		outForce.w() = KOri * dtheta + DOri * ((dtheta - last_dtheta) / hdt);
		pointer->last_dr = dr;
		pointer->last_dtheta = dtheta;

		// 剛体に加える力を計算
		// レンダリングした力から各接触点に働く力を逆算
		Vec3d ratio;
		double epsilon = 1e-10;
		for (int i = 0; i < 3; i++) {
			ratio[i] = outForce.v()[i] / allDepth[i];
			if (abs(allDepth[i]) < epsilon) ratio[i] = 0.0;
		}
		for (size_t i = 0; i < irs.size(); i++) {
			Vec3d penetration = irs[i]->normal * irs[i]->depth;
			for (int j = 0; j < 3; j++) {
				irs[i]->force[j] = ratio[j] * penetration[j];
			}
		}
		//	lastProxyの速度で摩擦力を求め、lastProxyの速度を更新し、中間表現へ力を加える
		SolveFrictionForce(pointer, irs);
		
		//	lastProxyの位置を更新する
		pointer->lastProxyPose.Pos() += hdt * pointer->lastProxyVelocity.v();
		pointer->lastProxyPose.Ori() = Quaterniond::Rot(hdt * pointer->lastProxyVelocity.w())  * pointer->lastProxyPose.Ori();

		//	中間表現から、剛体に力を加える
		for (size_t i = 0; i < irs.size(); i++) {
			hsolids->at(irs[i]->solidID)->AddForce(irs[i]->force, irs[i]->contactPointW);	// 各ポインタが剛体に加えた全ての力
			PHSolid* localSolid = &hsolids->at(irs[i]->solidID)->localSolid;
			PHSolidPairForHaptic* sp = sps->item(irs[i]->solidID, pointer->GetPointerID());
			assert(sp == irs[i]->solidPair);
			sp = irs[i]->solidPair;
			sp->force += irs[i]->force;	// あるポインタが剛体に加える力
			sp->torque += (irs[i]->contactPointW - localSolid->GetCenterPosition()) ^ irs[i]->force;
		}
	}
	pointer->AddHapticForce(outForce);
	//DSTR << pointer->GetFramePosition() << std::endl;
	//DSTR << "render" << outForce << std::endl;
	//CSVOUT << outForce[0] << "," << outForce[1] << "," << outForce[2] << "," << outForce[3] << "," << outForce[4] << "," << outForce[5] << "," <<std::endl;
}

void PHHapticRender::VibrationRendering(PHHapticPointer* pointer){
	if(!pointer->bVibration) return;
	int Nneigbors = (int)pointer->neighborSolidIDs.size();
	for(int j = 0; j < (int)Nneigbors; j++){
		int solidID = pointer->neighborSolidIDs[j];
		PHSolidPairForHaptic* sp = sps->item(solidID, pointer->GetPointerID());
		PHSolid* solid = hsolids->at(solidID)->GetLocalSolid(); 
		if(sp->frictionState == sp->FREE) continue;
		if(sp->contactCount == 0){
			sp->vibrationVel = pointer->GetVelocity() - solid->GetVelocity();
		}

		Vec3d vibV = sp->vibrationVel;
		double vibA = solid->GetShape(0)->GetVibA();
		double vibB = solid->GetShape(0)->GetVibB();
		double vibW = solid->GetShape(0)->GetVibW();
		double vibT = sp->contactCount * hdt;

		SpatialVector vibForce;
		// 法線方向に射影する必要がある？
		vibForce.v() = vibA * vibV * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT) / pointer->GetPosScale();		//振動計算
		if (sp->frictionState == sp->DYNAMIC) {
			Vec3d vibV = sp->totalFrictionForce;
			double vibT = sp->fricCount * hdt;
			vibForce.v() += 1000 * vibA * vibV * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT) / pointer->GetPosScale();		//振動計算
		}

		pointer->AddHapticForce(vibForce);
		//CSVOUT << vibForce.v().x << "," << vibForce.v().y << "," << vibForce.v().z << std::endl;
	}
}

}
#include <Physics/PHHapticRender.h>
#include <Physics/PHHapticEngine.h>
#include <Foundation/QuadProg++.hh>

#if 1
void MyBreak() {
	DSTR << "Should break here" << std::endl;
}
#define NANCHECK(x)										\
{														\
	double lpn = x.norm();								\
	if (isnan(lpn) || lpn > 1e8) {						\
		DSTR << #x << ": "<< x << std::endl;			\
		MyBreak();										\
	}													\
}

#define NANCHECKLP										\
{														\
double lpn = pointer->lastProxyVelocity.norm();			\
	if (isnan(lpn) || lpn > 1e8) {						\
		DSTR << "pointer->lastProxyVelocity: " << pointer->lastProxyVelocity << std::endl;	\
		MyBreak();																		\
	}																						\
}
#else
#define NANCHECKLP
#endif

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
	// プロキシ姿勢計算のための連立不等式
	VMatrixRow< double > c;
	c.resize(irs.size(), irs.size());
	c.clear(0.0);
	VVector< double > d;
	d.resize(irs.size());
	d.clear(0.0);
	double massInv = pointer->GetMassInv();
	Matrix3d rotationMatInv = (pointer->GetInertia() * pointer->GetRotationalWeight()).inv();
	for (size_t i = 0; i < irs.size(); i++) {
		for (size_t j = 0; j < irs.size(); j++) {
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

	for (size_t i = 0; i < irs.size(); i++) {
		f[i] = std::max(f[i], 0.0);
		// 並進量
		Vec3d tmpdr = f[i] * irs[i]->normal * massInv;
		dr += tmpdr;
		// 回転量
		Vec3d tmpdtheta = f[i] * rotationMatInv * (irs[i]->r % irs[i]->normal);
		dtheta += tmpdtheta;

		allDepth += -1 * irs[i]->normal * irs[i]->depth;
	}
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


bool PHHapticRender::CompIntermediateRepresentationShapeLevel(PHSolid* solid0, PHHapticPointer* pointer, 
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

	sh->irs.clear();
	if (dot >= 0.0) return false;
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
	return true;
}
bool PHHapticRender::CompFrictionIntermediateRepresentation(PHHapticPointer* pointer, PHSolidPairForHaptic* sp, PHShapePairForHaptic* sh) {
	static std::ofstream fricLog("fricLog.txt");
	static int count;
	count++;
	int Nirs = sh->irs.size();
	if (Nirs == 0) return false;
	bool bDynamic = false;
	double mu = 0;
	if (pointer->bTimeVaryFriction) {
		if (sp->frictionState == sp->STATIC) {
			mu = sh->mu + sh->mu*(sh->timeVaryFrictionA * log(1 + sh->timeVaryFrictionB * (sp->fricCount + 1) * hdt));
		}
	}
	else {
		mu = sh->mu;
		if (sp->frictionState == sp->STATIC) mu = sh->mu0;
	}
	sp->totalFrictionForce = Vec3d();

//	pointer->lastProxyPose.Pos().X() = pointer->GetPose().PosX() - 0.01;
//	pointer->lastProxyVelocity.v() = Vec3d(1,0,0);

	for (int i = 0; i < Nirs; i++) {
		PHIr* ir = sh->irs[i];
		if (pointer->bTimeVaryFriction && sp->frictionState == sp->DYNAMIC) {
			double v = (ir->pointerPointVel - ir->contactPointVel).norm();
			v = std::max(v, sh->timeVaryFrictionC / hdt);
			//	速度と粘性摩擦を含める
			mu = sh->mu + sh->timeVaryFrictionA * log(1 + sh->timeVaryFrictionB * sh->timeVaryFrictionC / v)
				+ sh->frictionViscosity * v;
		}
		double l = mu * ir->depth;							//	摩擦円錐半径
		Vec3d vps = ir->pointerPointW;						//	接触判定した際の、ポインタ侵入点の位置
		Vec3d vq = sp->relativePose * ir->pointerPointW;	//	現在の(ポインタの移動分を反映した)、位置
		Vec3d dq = (vq - vps) * ir->normal * ir->normal;	//	移動の法線成分
		Vec3d vqs = vq - dq;								//	法線成分の移動を消した現在の位置
		Vec3d tangent = vqs - vps;							//	移動の接線成分

		//DSTR << "vps" << vps << std::endl;
		//DSTR << "vq" << vq << std::endl;
		//DSTR << "tangent " << tangent << tangent.norm() << std::endl;

		//	Pointer側の速度
		Vec3d proxyPointVel = pointer->lastProxyVelocity.v() + (pointer->lastProxyVelocity.w() % (ir->pointerPointW - pointer->GetPose().Pos()));
		Vec3d relVel = proxyPointVel - ir->contactPointVel;
		relVel -= (relVel * ir->normal) * ir->normal;
		DSTR << "relVel:" << relVel.norm();
		if (relVel.norm() > 0.01) {
//			bDynamic = true;				// 一つでも、静止摩擦を越えたら、連鎖して滑るので、全体を動摩擦にする
		}
#if 1
		Posed lastProxyFromDevice = pointer->lastProxyPose * pointer->GetPose().Inv();
		//Vec3d lastProxyPointFromDevice = lastProxyFromDevice.Pos() + lastProxyFromDevice.Ori().Rotation() % (ir->pointerPointW - pointer->GetPose().Pos());

//		DSTR << "lastProxyPointFromDevice: " << lastProxyPointFromDevice << "  vel:" << pointer->lastProxyVelocity.v() << std::endl;

		double epsilon = 1e-5;
		double tangentNorm = tangent.norm();
		Vec3d fricDir;
		if (tangentNorm > 1e-5) fricDir = tangent / tangentNorm;
		/* 摩擦の向きは速度だと、速度0で破綻することが多い
		Vec3d velTangent = relVel - (relVel * ir->normal) * ir->normal;
		double velTagentNorm = velTangent.norm();
		if (velTagentNorm > epsilon) fricDir = - velTangent / velTagentNorm;
		else if (tangentNorm > epsilon) fricDir = tangent / tangentNorm;
		*/
		//	DSTR << "tan:" << tangent / tangentNorm << "  vel:" << - proxyPointVel << std::endl;
		if (fricDir.norm() > 0) {
			double lastProxy = tangent * fricDir;
			double predict = lastProxy + (proxyPointVel * fricDir) * hdt;	//	pr = r(t) + v dt;
			const double alpha = 0.4;
			double fullFric = predict -  alpha * (predict - l);				//	r(t+1) = pr - alpha * (pr - l)
			PHIr* fricIr = DBG_NEW PHIr();
			*fricIr = *ir;
			fricIr->normal = fricDir;
			DSTR << (sp->frictionState == sp->STATIC ? "S":"D") << (lastProxy > fullFric ? "d " : "s ") << "LP:" << lastProxy << "  FF:" << fullFric << std::endl;
			fricIr->depth = std::min(lastProxy, fullFric);
			if (lastProxy > fullFric) {
				bDynamic = true;				// 一つでも、静止摩擦を越えたら、連鎖して滑るので、全体を動摩擦にする
			}
			sh->irs.push_back(fricIr);
			sp->totalFrictionForce += fricIr->depth * fricIr->normal;
			//DSTR << "lastPx:" << lastProxy <<  "  wall:" << fricIr->depth << "  vel:" << pointer->lastProxyVelocity.v().x << std::endl;
		}
#else
		double epsilon = 1e-5;
		double tangentNorm = tangent.norm();
		if (tangentNorm > epsilon) {	//	ずれが0のときは、向きも分からないので摩擦拘束は入れない
			PHIr* fricIr = DBG_NEW PHIr();
			*fricIr = *ir;
			fricIr->normal = tangent / tangentNorm;
			fricIr->depth = std::min(tangentNorm, l);
			sh->irs.push_back(fricIr);
			sp->totalFrictionForce += fricIr->depth * fricIr->normal;
		}
#endif
	}
	sp->fricCount++;
	if (bDynamic) {
		if (sp->frictionState != sp->DYNAMIC) {
			std::cout << " S:" << sp->fricCount;
			sp->fricCount = 0;
			sp->frictionState = sp->DYNAMIC;
		}
	}
	else {
		if (sp->frictionState != sp->STATIC) {
			std::cout << " D:" << sp->fricCount << std::endl;
			sp->fricCount = 0;
			sp->frictionState = sp->STATIC;
		}
	}
	fricLog << count << "\t" << mu << std::endl;
	return true;
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
				PHShapePairForHaptic* sh = sp->GetShapePair(i, j);
				Posed curShapePoseW[2];
				curShapePoseW[0] = sp->interpolationPose * solid0->GetShapePose(i);
				curShapePoseW[1] = pointer->GetPose() * pointer->GetShapePose(j);
				CompIntermediateRepresentationShapeLevel(solid0, pointer, sp, sh, curShapePoseW, t, bInterpolatePose, pointer->bMultiPoints);
				if (pointer->bFriction) {
					CompFrictionIntermediateRepresentation(pointer, sp, sh);
				}
				irs.insert(irs.end(), sh->irs.begin(), sh->irs.end());
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


void PHHapticRender::DynamicsConstraintRendering(PHHapticPointer* pointer) {
	NANCHECKLP
	// 中間表現を求める。摩擦状態を更新
	PHIrs irs = CompIntermediateRepresentationForDynamicsConstraint(pointer);
	SpatialVector outForce = SpatialVector();
	if (irs.size() == 0) {
		pointer->proxyPose = pointer->GetPose();
		pointer->proxyVelocity.v() = pointer->GetVelocity();
		pointer->proxyVelocity.w() = pointer->GetAngularVelocity();
		pointer->bLastContactState = false;
	}else{
		//	初回接触時のみ、 lastProxyをProxy合わせる。
		if (!pointer->bLastContactState) {
			pointer->lastProxyPose = pointer->proxyPose;
			pointer->lastProxyVelocity = pointer->proxyVelocity;
			pointer->bLastContactState = true;
		}
		// ポインタ移動量を求める
		Vec3d dr, dtheta, allDepth;
		SolveProxyPose(dr, dtheta, allDepth, pointer, irs);
		// ポインタを中間表現の外に追い出した点を、proxyPoseとする。
		pointer->proxyPose.Ori() = (Quaterniond::Rot(dtheta) * pointer->GetOrientation()).unit();
		pointer->proxyPose.Pos() = pointer->GetFramePosition() + dr;
		Vec3d dProxPos = pointer->proxyPose.Pos() - pointer->lastProxyPose.Pos();
		Quaterniond dProxRot = pointer->proxyPose.Ori() * pointer->lastProxyPose.Ori().Inv();
		pointer->lastProxyVelocity.v() = dProxPos / hdt;
		pointer->lastProxyVelocity.w() = dProxRot.Rotation() / hdt;
		pointer->lastProxyPose = pointer->proxyPose;
		NANCHECKLP

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
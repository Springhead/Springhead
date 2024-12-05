#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticStepMulti.h>
#include <Physics/LuGre/coordinateConverter.h>
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
#if 0
PHHapticRenderInfo::PHHapticRenderInfo() {
	loopCount = 0;
	pdt = 0.02f;
	hdt = 0.001f;
	bInterpolatePose = true;
}
#endif

void PHHapticEngine::HapticRendering(PHHapticStepBase* he){
	for(int i = 0; i < he->NPointersInHaptic(); i++){
		PHHapticPointer* pointer = he->GetPointerInHaptic(i);
		pointer->hapticForce.clear();
		if (pointer->renderMode == PHHapticPointerDesc::PENALTY) {
			PenaltyBasedRendering(he, pointer);
		}else{
			DynamicProxyRendering(he, pointer);
		}
		VibrationRendering(he, pointer);
	}
}
#if 0
PHIrs PHHapticEngine::CompIntermediateRepresentation(PHHapticStepBase* he, PHHapticPointer* pointer){
	PHIrs irs;
	int nNeighbors = (int)pointer->neighborSolidIDs.size();
	for(int i = 0; i < nNeighbors; i++){
		int solidID = pointer->neighborSolidIDs[i];
		PHSolidPairForHaptic* sp = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(solidID, pointer->GetPointerID());
		PHIrs tempIrs = sp->CompIntermediateRepresentation(this, he->GetSolidInHaptic(solidID)->GetLocalSolid(), pointer);
		if(tempIrs.size() == 0) continue;
		irs.insert(irs.end(), tempIrs.begin(), tempIrs.end());
	}
	return irs;
}
#endif
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
void PHHapticEngine::PenaltyBasedRendering(PHHapticStepBase* he, PHHapticPointer* pointer){
	// 摩擦は追加できないので、強制的にfalse
	const bool bFric = pointer->bFriction;
	if(pointer->bFriction){
		pointer->EnableFriction(false);
	}
	PHIrs irs;
	CompIntermediateRepresentationForDynamicProxy(he, irs, irs, pointer);
	SpatialVector outForce = SpatialVector();
	int NIrs = (int)irs.size();
	if(NIrs > 0){
		for(int i = 0; i < NIrs; i++){
			PHIr* ir = irs[i];
			Vec3d ortho = ir->depth * ir->normal;
			Vec3d dv = ir->pointerPointVel - ir->contactPointVel;
			Vec3d dvortho = dv.norm() * ir->normal;

			float K  = pointer->reflexSpring / pointer->GetPosScale();
			float D = pointer->reflexDamper / pointer->GetPosScale();

			Vec3d addforce = K * ortho + D * dvortho;
			outForce.v() += addforce;
			
			irs[i]->force = -1 * addforce;
			he->GetSolidInHaptic(irs[i]->solidID)->AddForce(irs[i]->force, irs[i]->contactPointW);
			PHSolid* localSolid = &he->GetSolidInHaptic(irs[i]->solidID)->localSolid;
			PHSolidPairForHaptic* sp = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(irs[i]->solidID, pointer->GetPointerID());
			sp->force += irs[i]->force;	// あるポインタが剛体に加える力
			sp->torque += (irs[i]->contactPointW - localSolid->GetCenterPosition()) ^ irs[i]->force;
		}
	}
	pointer->AddHapticForce(outForce);
	pointer->EnableFriction(bFric);
}

void PHHapticEngine::SolveProxyPose(Vec3d& dr, Vec3d& dtheta, Vec3d& allDepth, PHHapticPointer* pointer, const PHIrs& irs) {
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
		assert(c[i][i] > 0);
		assert(f[i] >= 0);
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
	int i1 = 0, i2;
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

bool PHHapticEngine::CompIntermediateRepresentationShapeLevel(PHSolid* solid0, PHHapticPointer* pointer, 
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
	sh->nIrsNormal = (int)sh->irs.size();
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
			//assert(std::isfinite(ir->depth));
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
		assert(std::isfinite(ir->depth));
	}
	for (size_t i = 0; i < sh->irs.size(); i++) {
		PHIr* ir = sh->irs[i];
		ir->r = ir->pointerPointW - pointer->GetCenterPosition();
		ir->contactPointVel = solid0->GetPointVelocity(ir->contactPointW);
		ir->pointerPointVel = pointer->GetPointVelocity(ir->pointerPointW);
	}
	sh->nIrsNormal = (int)sh->irs.size();
	return true;
}

bool PHHapticEngine::CompLuGreFrictionIntermediateRepresentation(PHHapticStepBase* he, PHHapticPointer* pointer, PHSolidPairForHaptic* sp, PHShapePairForHaptic* sh) {
	
	int Nirs = (int)sh->NIrs();//接触の数

	if (Nirs > 1) return false;//1つのShapePairについて複数の接触があることはないはず

	//現在接触していない場合
	if (Nirs == 0) {
		sh->hasContact = false;
		return false;
	}
	
	//以下は現在接触している場合
	PHIr* ir = sh->irs[0];//考えるべき接触の拘束条件

	//ちょうど今接触し始めたとき
	if (!sh->hasContact) {
		sh->hasContact = true;
		sh->avgBristlesDeflection = Vec2d();										//剛毛の平均変位を初期化
		sh->avgBristlesDeflectionVel = Vec2d();										//剛毛の平均変位の微分値を初期化
		sh->avgStickingTime = Vec2d();												//平均固着時間を初期化
		sh->LuGreFunctionG = Vec2d(sh->LuGreParameterA, sh->LuGreParameterA);		//関数g(T)を初期化
		sh->contactSurfacePose = getWorldToPlanePose(ir->normal, ir->pointerPointW);//接触面の座標系を用意(ポインタの侵入位置を接触面上の座標系の原点とする)
		sh->proxyPos = Vec3d();														//プロキシの位置は接触開始場所=接触面の原点にしておく
		sh->frictionForce = Vec2d();												//摩擦力の初期化

		return true;
	}
	
	//必要な変数などの用意
	double spring = pointer->GetFrictionSpring();												//ハプティックポインタとプロキシの間のバネ係数
	double damper = pointer->GetFrictionDamper();												//ハプティックポインタとプロキシの間のダンパ係数
	double mass = pointer->GetMass();															//プロキシの質量
	sh->contactSurfacePose = updateWorldToPlanePose(sh->contactSurfacePose, ir->normal, ir->pointerPointW);	//接触面上の座標系の更新(ポインタの侵入位置を接触面上の座標系の原点とする)
	double hdt = he->GetHapticTimeStep();														//摩擦計算の時間間隔
	Vec3d objectPos = sp->GetSolid(0)->GetCenterOfMass();										//接触している相手の物体の現在の重心の位置(World座標)
	Vec3d objectVel = sp->GetSolid(0)->GetVelocity();											//接触している相手の物体の現在の速度(World座標)
	Vec3d objectVelOnSurface = convertWorldToPlaneVec(objectVel, sh->contactSurfacePose);			//接触している相手の物体の現在の速度(接触面上の座標)
	Vec3d lastObjectVel = sh->objectVel;														//接触している相手の物体の前回の速度(World座標)
	Vec3d lastObjectVelOnSurface = convertWorldToPlaneVec(lastObjectVel, sh->contactSurfacePose);	//接触している相手の物体の前回の速度(接触面上の座標)
	Vec3d lastPointerRelativePos = sh->pointerPos - objectPos;									//ハプティックポインタの前回の相対位置(World座標)
	Vec3d lastPointerRelativePosOnSurface = convertPlaneToWorldPos(lastPointerRelativePos, sh->contactSurfacePose);	//ハプティックポインタの前回の相対位置(接触面上の座標)
	Vec3d pointerRelativePos = sp->relativePose * ir->pointerPointW - objectPos;									//ハプティックポインタの現在の相対位置(World座標)
	Vec3d pointerRelativePosOnSurface = convertWorldToPlanePos(pointerRelativePos, sh->contactSurfacePose);			//ハプティックポインタの現在の相対位置(接触面上の座標)
	Vec3d lastProxyPos = sh->proxyPos;																				//プロキシの前回の絶対位置(World座標)
	Vec3d lastProxyRelativePos = lastProxyPos - objectPos;															//プロキシの前回の相対位置(World座標)
	Vec3d lastProxyRelativePosOnSurface = convertWorldToPlanePos(lastProxyRelativePos, sh->contactSurfacePose);		//プロキシの前回の相対位置(接触面上の座標)
	Vec2d lastRelativeVelOnSurface = sh->relativeVelOnSurface;									//前回の時点のプロキシと接触している物体の相対速度(接触面上の座標)
	Vec2d lastAvgBristlesDeflection = sh->avgBristlesDeflection;								//剛毛の平均変位 (接触面上の座標)
	
	//接触面上の座標系のx軸方向, y軸方向のそれぞれについて連立方程式を立てて解いていく
	double A = spring * (-pointerRelativePosOnSurface.z) + damper * (-pointerRelativePosOnSurface.z + lastPointerRelativePosOnSurface.z) / hdt;
	for (int i = 0; i < 2; i++) {
		//接触面上のx方向について

		//平均固着時間、g(T)の更新
		sh->avgStickingTime[i] = std::min(sh->avgStickingTime[i], sh->LuGreFunctionG[i] / (sh->bristlesSpringK * std::abs(lastRelativeVelOnSurface[i])));
		sh->LuGreFunctionG[i] = sh->LuGreParameterA + sh->LuGreParameterB * std::log(sh->LuGreParameterC * sh->avgStickingTime[i] + 1);

		//連立方程式の用意(連立方程式はWx = wとする)
		double B = spring * (pointerRelativePosOnSurface[i] - lastProxyRelativePosOnSurface[i])
			+ damper * (pointerRelativePosOnSurface[i] - lastPointerRelativePosOnSurface[i]) / hdt
			- mass * (objectVelOnSurface[i] - lastObjectVelOnSurface[i]) / hdt
			+ mass * lastRelativeVelOnSurface[i] / hdt;
		double S = sh->bristlesSpringK * lastRelativeVelOnSurface[i] * hdt / sh->LuGreFunctionG[i];
		double T = sh->bristlesSpringK * lastAvgBristlesDeflection[i] / sh->LuGreFunctionG[i];

		//連立方程式の左辺の行列を作成
		Matrix3d W[2];//W[0]はプロキシと物体の相対速度が正になる場合、W[1]は相対速度が負になる場合の行列を表す
		//まずは相対速度が正になる場合を埋める
		W[0][0][0] = A * (sh->bristlesSpringK + hdt + sh->bristlesDamperD);
		W[0][0][1] = sh->bristlesViscosityV * A;
		W[0][0][2] = -1;
		W[0][1][0] = 0;
		W[0][1][1] = -spring * hdt - damper - mass / hdt;
		W[0][1][2] = -1;
		W[0][2][0] = -S - 1;
		W[0][2][1] = 1 - T;
		W[0][2][2] = 0;
		//相対速度が負になる場合
		//(ほとんどが相対速度正の場合と同じなので、コピーしてから違う部分だけ書き換える)
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				W[1][j][k] = W[0][j][k];
			}
		}
		W[1][2][0] = S - 1;
		W[1][2][1] = 1 + T;

		//連立方程式の右辺のベクトルを作成
		Vec3d w;
		w[0] = -A * sh->bristlesSpringK * lastAvgBristlesDeflection[i];
		w[1] = -B;
		w[2] = 0;

		//連立方程式の解を入れるベクトルを用意
		Vec3d x[2];//x[0]がプロキシの相対速度正の場合の結果、x[1]は相対速度負の場合の結果

		//連立方程式を解く
		for (int j = 0; j < 2; j++) {//j = 0はプロキシの相対速度が正の場合の計算、j = 1は相対速度が負の場合の計算

			//前回の値で初期化
			x[i] = Vec3d(sh->avgBristlesDeflectionVel[i], lastRelativeVelOnSurface[i], sh->frictionForce[i]);

			//連立方程式を解く
			GaussSeidelEquation(W[j], x[i], w);
		}

		//連立方程式を解いた結果、x[0]とx[1]のどちらを採用するか決める
		bool conditionStisfied[2];//プロキシの相対速度が正の場合と、負の場合について、それぞれ条件が満たされているかを表す
		conditionStisfied[0] = ( x[0][1] >= 0 );
		conditionStisfied[1] = ( x[1][1] <= 0 );

		int selectedIndex = 0;//x[0]とx[1]のどちらを採用するか
		if (conditionStisfied[0]) {
			if (conditionStisfied[1]) {
				//どちらも条件を満たしている場合
				printf("プロキシの相対速度が正の場合も負の場合も条件を満たしています\n");
			}
			//プロキシの相対速度が正の場合のみ条件を満たしているというケース
		} else if (conditionStisfied[1]) {
			//プロキシの相対速度が負の場合のみ条件を満たしているというケース
			selectedIndex = 1;
		} else {
			//どちらも条件を満たしていない場合
			printf("プロキシの相対速度が正の場合も負の場合も条件を満たしていません\n");
		}
		
		//計算結果を保存
		sh->relativeVelOnSurface[i] = x[selectedIndex][1];											//プロキシの相対速度(接触面上の座標)
		sh->avgBristlesDeflection[i] = lastAvgBristlesDeflection[i] + x[selectedIndex][0] * hdt;	//剛毛の平均変位(接触面上の座標)
		sh->avgBristlesDeflectionVel[i] = x[selectedIndex][0];										//剛毛の平均変位(接触面上の座標)の微分
		sh->frictionForce[i] = x[selectedIndex][2];													//摩擦力(接触面上の座標)					
	}

	//計算結果の保存
	sh->hasContact = true;								//接触していることを記録
	sh->pointerPos = pointerRelativePos + objectPos;	//ハプティックポインタの位置(World座標) 
	sh->objectVel = objectVel;							//接触している相手の物体の速度(World座標)
	//プロキシの位置の保存
	Vec3d relativeVelOnSurface3d = Vec3d(sh->relativeVelOnSurface.x, sh->relativeVelOnSurface.y, 0);
	Vec3d proxyRelativePosOnSurface = lastProxyRelativePosOnSurface + relativeVelOnSurface3d * hdt;	//プロキシの現在の相対位置(接触面上の座標)
	Vec3d proxyRelativePos = convertPlaneToWorldPos(proxyRelativePosOnSurface, sh->contactSurfacePose);	//プロキシの現在の相対位置(World座標)
	sh->proxyPos = proxyRelativePos + objectPos;														//プロキシの現在の絶対位置(World座標)

	return true;
	
	//拘束条件の生成
	PHIr* fricIr = DBG_NEW PHIr();
	*fricIr = *ir;
	Vec3d proxyPosDelta = sh->proxyPos - lastProxyPos;	//プロキシの位置の移動量(World座標)
	fricIr->normal = proxyPosDelta.unit();
	fricIr->depth = proxyPosDelta.norm();
	//sh->irs.push_back(fricIr);

	//デバッグ用
	printf("(%f, %f, %f,   %f, %f, %f)\n", sh->contactSurfacePose.PosX(), sh->contactSurfacePose.PosY(), sh->contactSurfacePose.PosZ(), sh->contactSurfacePose.OriX(), sh->contactSurfacePose.OriY(), sh->contactSurfacePose.OriZ());
	printf("摩擦力: (%f, %f) => %f\n", sh->frictionForce[0], sh->frictionForce[1], sh->frictionForce.norm());
	printf("normal: (%f, %f, %f),  depth: %f\n\n", fricIr->normal.x, fricIr->normal.y, fricIr->normal.z, fricIr->depth);
	
	return true;
}

bool PHHapticEngine::CompFrictionIntermediateRepresentation(PHHapticStepBase* he, PHHapticPointer* pointer, PHSolidPairForHaptic* sp, PHShapePairForHaptic* sh) {
	
	//LuGreモデルの場合の分岐
	if (pointer->frictionType == PHFrictionType::LuGre) {
		return CompLuGreFrictionIntermediateRepresentation(he, pointer, sp, sh);
	}

	int Nirs = (int)sh->irs.size();
	if (Nirs == 0) return false;
	bool bStatic = false;
	double hdt = he->GetHapticTimeStep();
	//	Proxyを動力学で動かすときの、バネの伸びに対する移動距離の割合 0.5くらいが良い感じ
	//double alpha = hdt * hdt * pointer->GetMassInv() * pointer->reflexSpring;
	double alpha = hdt * hdt * pointer->GetMassInv() * pointer->frictionSpring;

	//	摩擦係数の計算
	if (pointer->bTimeVaryFriction) {
		if (sp->frictionState == PHSolidPairForHapticIf::STATIC) {
			sh->muCur = sh->mu + sh->mu*(sh->timeVaryFrictionA * log(1 + sh->timeVaryFrictionB * (sp->fricCount + 1) * hdt));
		}
	}
	else {
		sh->muCur = sh->mu;
		if (sp->frictionState == PHSolidPairForHapticIf::STATIC) sh->muCur = sh->mu0;
	}
	
	for (int i = 0; i < Nirs; i++) {
		PHIr* ir = sh->irs[i];
		if (pointer->bTimeVaryFriction && sp->frictionState == PHSolidPairForHapticIf::DYNAMIC) {
			double v = (ir->pointerPointVel - ir->contactPointVel).norm();
			v = std::max(v, sh->timeVaryFrictionC / hdt);
			//stribeck効果を含める
			
			sh->muCur = sh->mu + (sh->mu - sh->stribeckmu)*(exp(-v / sh->stribeckVelocity)-1.0);

			/*
			//	速度と粘性摩擦を含める
			sh->muCur = sh->mu + sh->timeVaryFrictionA * log(1 + sh->timeVaryFrictionB * sh->timeVaryFrictionC / v)
				+ sh->frictionViscosity * v;
				*/
		}
		double l = sh->muCur * ir->depth;					//	摩擦円錐半径
		Vec3d vps = ir->pointerPointW;						//	接触判定した際の、ポインタ侵入点の位置
		Vec3d vq = sp->relativePose * ir->pointerPointW;	//	現在の(ポインタの移動分を反映した)、位置
		Vec3d dq = (vq - vps) * ir->normal * ir->normal;	//	移動の法線成分
		Vec3d vqs = vq - dq;								//	法線成分の移動を消した現在の位置
		Vec3d tangent = vqs - vps;							//	移動の接線成分
		//DSTR << "vps" << vps << std::endl;
		//DSTR << "vq" << vq << std::endl;
		//DSTR << "tangent " << tangent << tangent.norm() << std::endl;
		
		//	TODO hase: 上の計算法(relativePose)が本当に良いか要検討
		//Posed lastProxyFromDevice = pointer->lastProxyPose * pointer->GetPose().Inv();
		//Vec3d lastProxyPointFromDevice = lastProxyFromDevice.Pos() + lastProxyFromDevice.Ori().Rotation() % (ir->pointerPointW - pointer->GetPose().Pos());
		//DSTR << "lastProxyPointFromDevice: " << lastProxyPointFromDevice << "  vel:" << pointer->lastProxyVelocity.v() << std::endl;


		//	Pointer側の速度
		Vec3d proxyPointVel = pointer->lastProxyVelocity.v() + (pointer->lastProxyVelocity.w() % (ir->pointerPointW - pointer->GetPose().Pos()));

		double epsilon = 1e-5;
		double tangentNorm = tangent.norm();
		if (tangentNorm > 1e-12) {
			PHIr* fricIr = DBG_NEW PHIr();
			*fricIr = *ir;
			fricIr->normal = tangent / tangentNorm;
			//	現在のProxy位置と摩擦力の限界位置を計算
			double proxyPos, frictionLimit;
			if (pointer->renderMode == PHHapticPointer::DYNAMIC_PROXY) {
				proxyPos = tangentNorm;
				double vdt = (proxyPointVel * fricIr->normal) * hdt;
				double predict = proxyPos + vdt - (pointer->GetFrictionDamper() * (vdt - (pointer->velocity * fricIr->normal)*hdt));	//	pr = r(t) + v dt;
				frictionLimit = predict - alpha * (predict - l);
			}
			else {
				proxyPos = tangentNorm;
				frictionLimit = l;
			}
			//	結果をfricIrに反映
			//std::cout << proxyPos << " : " << frictionLimit << std::endl;
			if (proxyPos <= frictionLimit) {
				fricIr->depth = proxyPos;
				assert(std::isfinite(fricIr->depth));
				bStatic = true;				// 一つでも、静止摩擦ならば、それが持ちこたえると考える。
			}
			else {
				fricIr->depth = frictionLimit;
				assert(std::isfinite(fricIr->depth));
			}
			sh->irs.push_back(fricIr);
		}
	}
	sp->fricCount++;
	if (!bStatic) {
		if (sp->frictionState != PHSolidPairForHapticIf::DYNAMIC) {
			std::cout << " S:" << sp->fricCount;
			sp->fricCount = 0;
			sp->frictionState = PHSolidPairForHapticIf::DYNAMIC;
		}
	}
	else {
		if (sp->frictionState != PHSolidPairForHapticIf::STATIC) {
			std::cout << " D:" << sp->fricCount << std::endl;
			sp->fricCount = 0;
			sp->frictionState = PHSolidPairForHapticIf::STATIC;
		}
	}
	return true;
}

void PHHapticEngine::CompIntermediateRepresentationForDynamicProxy(PHHapticStepBase* he, PHIrs& irsNormal, PHIrs& irsFric, PHHapticPointer* pointer){
	int nNeighbors = (int)pointer->neighborSolidIDs.size();
	for (int i = 0; i < nNeighbors; i++) {
		int solidID = pointer->neighborSolidIDs[i];
		PHSolidPairForHaptic* sp = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(solidID, pointer->GetPointerID());

		//--------------------------------------------------
		/* 力覚安定化のための補間
		// Impulseの場合は相手の剛体のPoseの補間が必要。
		// LocalDynamicsの場合は法線の補間のみでよい。
		// 法線の補間はPHShapePairForHapticでやる。h
		*/
		PHSolid* solid0 = he->GetSolidInHaptic(solidID)->GetLocalSolid();
		
		const double syncCount = he->GetPhysicsTimeStep() / he->GetHapticTimeStep();
		double t = he->GetLoopCount() / syncCount;
		if (t > 1.0) t = 1.0;

		sp->force.clear();
		sp->torque.clear();
		sp->lastInterpolationPose = sp->interpolationPose;
		sp->interpolationPose = solid0->GetPose();
		if (he->IsInterporate()) {
			Posed cur = solid0->GetPose();
			double dt = ((PHScene*)solid0->GetScene())->GetTimeStep();
			Posed last;
			last.Pos() = cur.Pos() - (solid0->GetVelocity() * dt + solid0->GetOrientation() * solid0->dV.v());
			last.Ori() = (cur.Ori() * Quaterniond::Rot(-solid0->v.w() * dt + -solid0->dV.w())).unit();
			sp->interpolationPose = interpolate(t, last, cur);
		}
		// 接触したとして摩擦計算のための相対位置を計算
		// 相対摩擦
		if (sp->frictionState == PHSolidPairForHapticIf::FREE) {
			sp->frictionState = PHSolidPairForHapticIf::STATIC;
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
		bool bContact = false;
		for (int i = 0; i < solid0->NShape(); i++) {
			for (int j = 0; j < pointer->NShape(); j++) {
				PHShapePairForHaptic* sh = sp->GetShapePair(i, j)->Cast();
				Posed curShapePoseW[2];
				curShapePoseW[0] = sp->interpolationPose * solid0->GetShapePose(i);
				curShapePoseW[1] = pointer->GetPose() * pointer->GetShapePose(j);
				CompIntermediateRepresentationShapeLevel(solid0, pointer, sp, sh, curShapePoseW, t, he->IsInterporate(), pointer->bMultiPoints);
				if (sh->irs.size()) {
					bContact = true;
					irsNormal.insert(irsNormal.end(), sh->irs.begin(), sh->irs.end());
				}
				if (pointer->bFriction) {
					CompFrictionIntermediateRepresentation(he, pointer, sp, sh);
					irsFric.insert(irsFric.end(), sh->irs.begin()+sh->nIrsNormal, sh->irs.end());
				}
			}
		}
		if (!bContact) {
			// 接触なし
			sp->frictionState = PHSolidPairForHapticIf::FREE;
			sp->initialRelativePose = Posed();
			sp->relativePose = Posed();
		}
	}
}

void PHHapticEngine::DynamicProxyRendering(PHHapticStepBase* he, PHHapticPointer* pointer) {
	double hdt = he->GetHapticTimeStep();

	NANCHECKLP
	pointer->lastProxyPose = pointer->proxyPose;
	NANCHECKLP
		// 中間表現を求める。摩擦状態を更新
	PHIrs irsNormal, irsFric, irsAll;

	CompIntermediateRepresentationForDynamicProxy(he, irsNormal, irsFric, pointer);

	irsAll = irsNormal;
	irsAll.insert(irsAll.end(), irsFric.begin(), irsFric.end());
	SpatialVector outForce = SpatialVector();
	if (irsNormal.size() == 0) {
		pointer->proxyPose = pointer->GetPose();
		if (pointer->renderMode == pointer->DYNAMIC_PROXY) {
			pointer->lastProxyVelocity.v() = pointer->GetVelocity();
			pointer->lastProxyVelocity.w() = pointer->GetAngularVelocity();
		}
	}else{
		// 追い出しのためのポインタ移動量を求める
		Vec3d dr, dtheta, allDepth;
		SolveProxyPose(dr, dtheta, allDepth, pointer, irsAll);
		// ポインタを中間表現の外に追い出した点を、proxyPoseとする。
		pointer->proxyPose.Ori() = Quaterniond::Rot(dtheta) * pointer->GetOrientation();
		pointer->proxyPose.Pos() = pointer->GetFramePosition() + dr;
		
		//GMS用
	//	pointer->proxyPose.Pos() = pointer->GetFramePosition() + dr + ( pointer->totalZ/(double)pointer->proxyN);

		if (pointer->renderMode == pointer->DYNAMIC_PROXY) {
			//	Proxyの速度の計算
			Vec3d dProxPos = pointer->proxyPose.Pos() - pointer->lastProxyPose.Pos();
			Quaterniond dProxRot = pointer->proxyPose.Ori() * pointer->lastProxyPose.Ori().Inv();
			pointer->lastProxyVelocity.v() = dProxPos / hdt;
			pointer->lastProxyVelocity.w() = dProxRot.Rotation() / hdt;
			NANCHECKLP
		}
		/// 力覚インタフェースに出力する力の計算
		Vec3d last_dr = pointer->last_dr;
		Vec3d last_dtheta = pointer->last_dtheta;

		float K = pointer->reflexSpring / pointer->GetPosScale();
		float D = pointer->reflexDamper / pointer->GetPosScale();
		float KOri = pointer->rotationReflexSpring * pointer->GetRotationalWeight();
		float DOri = pointer->rotationReflexDamper * pointer->GetRotationalWeight();

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
		for (size_t i = 0; i < irsAll.size(); i++) {
			Vec3d penetration = irsAll[i]->normal * irsAll[i]->depth;
			for (int j = 0; j < 3; j++) {
				irsAll[i]->force[j] = ratio[j] * penetration[j];
			}
		}		
		//	中間表現から、剛体に力を加える
		for (size_t i = 0; i < irsAll.size(); i++) {
			he->GetSolidInHaptic(irsAll[i]->solidID)->AddForce(irsAll[i]->force, irsAll[i]->contactPointW);	// 各ポインタが剛体に加えた全ての力
			PHSolid* localSolid = &he->GetSolidInHaptic(irsAll[i]->solidID)->localSolid;
			PHSolidPairForHaptic* sp = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(irsAll[i]->solidID, pointer->GetPointerID());
			assert(sp == irsAll[i]->solidPair);
			sp = irsAll[i]->solidPair;
			sp->force += irsAll[i]->force;	// あるポインタが剛体に加える力
			sp->torque += (irsAll[i]->contactPointW - localSolid->GetCenterPosition()) ^ irsAll[i]->force;
		}
		//	摩擦振動提示の大きさに使うため、SolidPairに摩擦力の合計を計算して記録する
		if (irsFric.size()) {
			PHSolidPairForHaptic* sp = irsFric[0]->solidPair;
			if (sp->frictionState == PHSolidPairForHapticIf::STATIC) {
				sp->lastStaticFrictionForce.clear();
				for (size_t i = 0; i < irsFric.size(); i++) {
					sp->lastStaticFrictionForce += irsFric[i]->force;
					if (sp != irsFric[i]->solidPair) {
						sp = irsFric[i]->solidPair;
						sp->lastStaticFrictionForce.clear();
					}
				}
			}
		}
	}
	pointer->AddHapticForce(outForce);
	//DSTR << pointer->GetFramePosition() << std::endl;
	//DSTR << "render" << outForce << std::endl;
	//CSVOUT << outForce[0] << "," << outForce[1] << "," << outForce[2] << "," << outForce[3] << "," << outForce[4] << "," << outForce[5] << "," <<std::endl;
}

void PHHapticEngine::VibrationRendering(PHHapticStepBase* he, PHHapticPointer* pointer) {
	if (!pointer->bVibration) return;
	double hdt = he->GetHapticTimeStep();
	int Nneigbors = (int)pointer->neighborSolidIDs.size();
	for (int j = 0; j < (int)Nneigbors; j++) {
		int solidID = pointer->neighborSolidIDs[j];
		PHSolidPairForHaptic* sp = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(solidID, pointer->GetPointerID());
		PHSolid* solid = he->GetSolidInHaptic(solidID)->GetLocalSolid();
		if (sp->frictionState == PHSolidPairForHapticIf::FREE) continue;
		if (sp->contactCount == 0) {
			sp->contactVibrationVel = pointer->GetVelocity() - solid->GetVelocity();
		}
		Vec3d vibV = sp->contactVibrationVel;
		double vibA = solid->GetShape(0)->GetVibA();
		double vibB = solid->GetShape(0)->GetVibB();
		double vibW = solid->GetShape(0)->GetVibW();
		double vibT = sp->contactCount * hdt;

		SpatialVector vibForce;
		// 法線方向に射影する必要がある？
		vibForce.v() = vibA * vibV * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT) / pointer->GetPosScale();		//振動計算
		if (sp->frictionState == PHSolidPairForHapticIf::DYNAMIC) {
			Vec3d vibV = sp->lastStaticFrictionForce * hdt * pointer->GetMassInv() * 0.3;	//	0.3は謎係数。ないと接触の振動に対して強すぎてしまう。
			double vibT = sp->fricCount * hdt;
			vibForce.v() += vibA * vibV * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT) / pointer->GetPosScale();		//振動計算

		}
		pointer->AddHapticForce(vibForce);
		//CSVOUT << vibForce.v().x << "," << vibForce.v().y << "," << vibForce.v().z << std::endl;
	}
}

}

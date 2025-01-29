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

void PHHapticEngine::HapticRendering(PHHapticStepBaseIf* heIf){
	PHHapticStepBase* he = (PHHapticStepBase*)heIf;
	for(int i = 0; i < he->NPointersInHaptic(); i++){
		PHHapticPointer* pointer = (PHHapticPointer*)he->GetPointerInHaptic(i);
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
		PHSolidPairForHaptic* sph = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(solidID, pointer->GetPointerID());
		PHIrs tempIrs = sph->CompIntermediateRepresentation(this, he->GetSolidInHaptic(solidID)->GetLocalSolid(), pointer);
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
			PHSolidIf* localSolid = he->GetSolidInHaptic(irs[i]->solidID)->GetLocalSolid();
			PHSolidPairForHaptic* sph = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(irs[i]->solidID, pointer->GetPointerID());
			sph->force += irs[i]->force;	// あるポインタが剛体に加える力
			sph->torque += (irs[i]->contactPointW - localSolid->GetCenterPosition()) ^ irs[i]->force;
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
	//DSTR << "Nirs:" << Nirs << "sh:" << (int)sh << std::endl;

	//1つのShapePairについて複数の接触があることはないはず
	if (Nirs > 1) {
		sh->hasContact = false;
		return false;
	}

	//現在接触していない場合
	if (Nirs == 0) {
		sh->hasContact = false;
		return false;
	}

	//以下は現在接触している場合
	PHIr* ir = sh->irs[0];//考えるべき接触の拘束条件
	
	//LuGreのパラメータの更新(注意: 毎フレーム更新することになってしまうので、この部分はやり方を変えたほうがよい)
	sh->UpdateCache();

	//ちょうど今接触し始めたとき
	if (!sh->hasContact) {
		sh->hasContact = true;														//接触していることを記録
		sh->avgStickingTime = 0;													//平均固着時間を初期化
		sh->LuGreFunctionG = sh->LuGreParameterA;									//関数g(T)を初期化
		sh->avgBristlesDeflection = Vec3d();										//剛毛の平均変位を初期化
		sh->pointerPos = ir->pointerPointW - ir->r;									//現在のハプティックポインタの位置(World座標)
		sh->pointerDepth = 0;														//現在のハプティックポインタの深さ(侵入量)
		sh->proxyPos = ir->pointerPointW + ir->depth * ir->normal - ir->r;			//プロキシの現在の位置はハプティックポインタの上部(かつ他の物体に侵入しない位置)で、プロキシの重心の位置にとる
		sh->lastProxyPos = sh->proxyPos;											//プロキシの前回の位置もプロキシの現在の位置と同じ位置とする
		sh->objectPos = sp->GetSolid(0)->GetCenterOfMass();							//接触している相手の物体の現在の位置(World座標)
		sh->lastObjectPos = sh->objectPos;											//接触している相手の物体の前回の位置(World座標)は、現在の位置と同じとしておく
		sh->frictionForce = Vec3d();												//摩擦力の初期化
		sp->frictionState = PHSolidPairForHapticIf::STATIC;							//接触したときは静止しているものとする
		sp->fricCount = 0;															//静止状態 or 動いている状態が継続しているステップ数(fricCount)を0にする
		//sh->contactSurfacePose = getWorldToPlanePose(ir->normal, sh->lastProxyPos);	//接触面の座標系を用意(前回のプロキシの位置を接触面上の座標系の原点とする)
		sh->contactSurfacePose = getWorldToPlanePose(ir->normal, Vec3d(1.0, 0.0, 0.0), sh->lastProxyPos);
		DSTR << "接触開始" << std::endl;
		return true;
	}

	//接触面上の座標系(ローカル座標系)を更新する
	//ただし、z軸方向が新しい接触面の法線方向に、x軸が前回の速度方向に近い方向になるようにする
	Vec3d lastRelativeVel = (sh->proxyPos - sh->lastProxyPos - sh->objectPos + sh->lastObjectPos) / he->GetHapticTimeStep();	//前回の速度方向(World座標)
	//Posed contactSurfacePose = getWorldToPlanePose(ir->normal, lastRelativeVel, sh->proxyPos);									//接触面上の座標系の更新(前回のプロキシの位置を座標系の原点とする)
	Posed contactSurfacePose = sh->contactSurfacePose;

	//必要な変数などの用意(おもに現在の接触面上の座標系での座標などを用意する)
	double spring = pointer->GetFrictionSpring();													//ハプティックポインタとプロキシの間のバネ係数
	double damper = pointer->GetFrictionDamper();													//ハプティックポインタとプロキシの間のダンパ係数
	double mass = pointer->GetMass();																//プロキシの質量
	double hdt = he->GetHapticTimeStep();															//摩擦計算の時間間隔
	Vec3d lastAvgBristlesDeflectionOnSurface = convertWorldToPlaneVec(sh->avgBristlesDeflection, contactSurfacePose);	//前回のzの値(接触面上の座標)
	Vec3d pointerPos = ir->pointerPointW - ir->r;													//ハプティックポインタの現在の位置(World座標)
	Vec3d pointerPosOnSurface = convertWorldToPlanePos(pointerPos, contactSurfacePose);				//ハプティックポインタの現在の位置(接触面上の座標)
	Vec3d lastPointerPosOnSurface = convertWorldToPlanePos(sh->pointerPos, contactSurfacePose);		//ハプティックポインタの前回の位置(接触面上の座標)
	double pointerDepth = ir->depth;																//ハプティックポインタの現在の深さ(侵入量)
	double lastPointerDepth = sh->pointerDepth;														//ハプティックポインタの前回の深さ(侵入量)
	Vec3d lastProxyPosOnSurface = convertWorldToPlanePos(sh->proxyPos, contactSurfacePose);			//プロキシの前回の位置(接触面上の座標)
	Vec3d secondLastProxyPosOnSurface = convertWorldToPlanePos(sh->lastProxyPos, contactSurfacePose);	//プロキシの前々回の位置(接触面上の座標)
	Vec3d objectPos = sp->GetSolid(0)->GetCenterOfMass();											//接触相手の物体の現在の位置(World座標)
	Vec3d objectPosOnSurface = convertWorldToPlanePos(objectPos, contactSurfacePose);				//接触相手の物体の現在の位置(接触面上の座標)
	Vec3d lastObjectPosOnSurface = convertWorldToPlanePos(sh->objectPos, contactSurfacePose);		//接触相手の物体の前回の位置(接触面上の座標)

	//平均固着時間、g(T)の更新
	double avgStickingTimeIfNotMoving = sh->avgStickingTime + hdt;
	double avgStickingTimeIfMoving = sh->LuGreFunctionG / (sh->bristlesSpringK * lastRelativeVel.norm());
	sh->avgStickingTime = std::min(avgStickingTimeIfNotMoving, avgStickingTimeIfMoving);
	sh->LuGreFunctionG = sh->LuGreParameterA + sh->LuGreParameterB * std::log(sh->LuGreParameterC * sh->avgStickingTime + 1);

	//静止状態 or 動いている状態が継続しているステップ数(fricCount)の更新
	PHSolidPairForHapticIf::FrictionState newFrictionState;//現在 静止 or 動いている のどちらとみなせる状態か
	newFrictionState = (avgStickingTimeIfNotMoving <= avgStickingTimeIfMoving) ? PHSolidPairForHapticIf::STATIC : PHSolidPairForHapticIf::DYNAMIC;
	if (sp->frictionState != newFrictionState) {
		sp->fricCount = 0;
	}
	else {
		sp->fricCount++;
	}
	sp->frictionState = newFrictionState;


	//接触面上の座標系のx軸方向, y軸方向のそれぞれについて連立方程式を立てて解いていく
	//連立方程式を解いた結果を入れるための場所を用意
	Vec3d proxyPosOnSurface = Vec3d();				//現在のプロキシの位置(接触面上の座標)
	Vec3d avgBristlesDeflectionOnSurface = Vec3d();	//現在のzの値(接触面上の座標系での値)
	Vec3d frictionForceOnSurface = Vec3d();			//現在の摩擦力(接触面上の座標系での値)
	//連立方程式を解くための準備
	double fPerp = spring * pointerDepth + (damper / hdt) * (pointerDepth - lastPointerDepth);	//垂直抗力
	for (int i = 0; i < 2; i++) {
		//接触面上のx方向について

		//連立方程式の用意(連立方程式はWx = wとする)
		double P = sh->bristlesSpringK * lastAvgBristlesDeflectionOnSurface[i] / sh->LuGreFunctionG;
		double Q = sh->bristlesSpringK * (objectPosOnSurface[i] - lastObjectPosOnSurface[i]) / sh->LuGreFunctionG;
		double R = -lastAvgBristlesDeflectionOnSurface[i] + lastProxyPosOnSurface[i] + objectPosOnSurface[i] - lastObjectPosOnSurface[i];
		double S = sh->bristlesSpringK * lastAvgBristlesDeflectionOnSurface[i] * lastProxyPosOnSurface[i] / sh->LuGreFunctionG;
		double U = sh->bristlesDamperD * lastAvgBristlesDeflectionOnSurface[i]
			+ sh->bristlesViscosityV * (lastProxyPosOnSurface[i] + objectPosOnSurface[i] - lastObjectPosOnSurface[i]);
		double V = mass * (2 * lastProxyPosOnSurface[i] - secondLastProxyPosOnSurface[i]) / (hdt * hdt)
			+ spring * pointerPosOnSurface[i]
			+ damper * (pointerPosOnSurface[i] - lastPointerPosOnSurface[i] + lastProxyPosOnSurface[i]) / hdt;

		//連立方程式の左辺の行列を作成
		Matrix3d W[2];//W[0]はプロキシと物体の相対速度が正になる場合、W[1]は相対速度が負になる場合の行列を表す
		//まずは相対速度が正になる場合を埋める
		W[0][0][0] = 1 - P;
		W[0][0][1] = -1 + Q;
		W[0][0][2] = 0;
		W[0][1][0] = sh->bristlesViscosityV * fPerp;
		W[0][1][1] = fPerp * (sh->bristlesSpringK * hdt + sh->bristlesDamperD);
		W[0][1][2] = hdt;
		W[0][2][0] = mass / (hdt * hdt) + spring + damper / hdt;
		W[0][2][1] = 0;
		W[0][2][2] = -1;
		//相対速度が負になる場合
		//(ほとんどが相対速度正の場合と同じなので、コピーしてから違う部分だけ書き換える)
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				W[1][j][k] = W[0][j][k];
			}
		}
		W[1][0][0] = 1 + P;
		W[1][0][1] = -1 - Q;

		//連立方程式の右辺のベクトルを作成
		Vec3d w[2];//w[0]はプロキシと物体の相対速度が正になる場合、w[1]は相対速度が負になる場合の行列を表す
		w[0][0] = R - S;
		w[0][1] = fPerp * U;
		w[0][2] = V;
		w[1][0] = R + S;
		w[1][1] = fPerp * U;
		w[1][2] = V;
		
		//連立方程式の解を入れるベクトルを用意
		Vec3d x[2];//x[0]がプロキシの相対速度正の場合の結果、x[1]は相対速度負の場合の結果

		//連立方程式を解く
		for (int j = 0; j < 2; j++) {//j = 0はプロキシの相対速度が正の場合の計算、j = 1は相対速度が負の場合の計算
			int ip[3];
			gauss(W[j], x[j], w[j], ip);
		}
		
		//連立方程式を解いた結果得られる相対速度を求める
		double relativeVelOnSurface[2];//1つ目の値が相対速度が正の場合の計算結果で、2つ目の値が相対速度が負の場合の計算結果
		for (int j = 0; j < 2; j++) {
			relativeVelOnSurface[j] = (w[j][0] - lastProxyPosOnSurface[i] - objectPosOnSurface[i] + lastObjectPosOnSurface[i]) / hdt;//w[j][0]は現在のプロキシの位置(接触面上の座標)
		}

		//連立方程式を解いた結果、x[0]とx[1]のどちらを採用するか決める
		bool conditionSatisfied[2];//プロキシの相対速度が正の場合と、負の場合について、それぞれ条件が満たされているかを表す
		conditionSatisfied[0] = ( relativeVelOnSurface[0] >= 0 );
		conditionSatisfied[1] = ( relativeVelOnSurface[1] < 0 );

		int selectedIndex = 0;//相対速度が正の場合(0)と負の場合(1)のどちらを採用するか
		if (conditionSatisfied[0]) {
			if (conditionSatisfied[1]) {
				//どちらも条件を満たしている場合
				//if (i == 0) DSTR << "プロキシの相対速度が正の場合も負の場合も条件を満たしています: " << x[0][1] << ", " << x[1][1] << std::endl;
				
				//printf("(%f, %f, %f) (%f, %f, %f)       (lastZ: %f), det(%f, %f)\n", x[0][0], x[0][1], x[0][2], x[1][0], x[1][1], x[1][2], lastAvgBristlesDeflection[i], det(W[0]), det(W[1]));
			}
			//プロキシの相対速度が正の場合のみ条件を満たしているというケース
			//if (i == 0) DSTR << "正の場合のみ" << std::endl;
		} else if (conditionSatisfied[1]) {
			//プロキシの相対速度が負の場合のみ条件を満たしているというケース
			//if (i == 0) DSTR << "負の場合のみ" << std::endl;
			selectedIndex = 1;
		} else {
			//どちらも条件を満たしていない場合
			//DSTR << "プロキシの相対速度が正の場合も負の場合も条件を満たしていません" << std::endl;
		}
		
		//連立方程式を解いた結果を保存
		proxyPosOnSurface[i] = x[selectedIndex][0];
		avgBristlesDeflectionOnSurface[i] = x[selectedIndex][1];
		frictionForceOnSurface[i] = x[selectedIndex][2];
	}

	//計算結果の保存
	sh->hasContact = true;																					//接触していることを記録
	sh->contactSurfacePose = contactSurfacePose;															//接触面上の座標系を表すPosed
	sh->avgBristlesDeflection = convertPlaneToWorldVec(avgBristlesDeflectionOnSurface, contactSurfacePose);	//現在のzの値(World座標)
	sh->pointerPos = pointerPos;																			//現在のハプティックポインタの位置(World座標)
	sh->pointerDepth = pointerDepth;																		//現在のハプティックポインタの深さ(侵入量)
	sh->lastProxyPos = sh->proxyPos;																		//前回のプロキシの位置(World座標)
	sh->proxyPos = convertPlaneToWorldPos(proxyPosOnSurface, contactSurfacePose);							//現在のプロキシの位置(World座標)
	sh->lastObjectPos = sh->objectPos;																		//前回の接触相手の物体の位置(World座標)
	sh->objectPos = objectPos;																				//現在の接触相手の物体の位置(World座標)
	sh->frictionForce = convertPlaneToWorldVec(frictionForceOnSurface, contactSurfacePose);					//現在の摩擦力(World座標)
	
	//拘束条件の生成(ここで生成する拘束条件は、接触面に対して平行にはたらく位置の拘束条件)
	PHIr* fricIr = DBG_NEW PHIr();
	*fricIr = *ir;
	Vec3d offsetVector = sh->proxyPos - pointerPos;										//現在のハプティックポインタの位置から現在のプロキシの位置への移動量(World座標)
	Vec3d tangentialVector = offsetVector - dot(offsetVector, ir->normal) * ir->normal;	//offsetVectorのうち、平面に対して平行な成分のみを抽出したもの
	fricIr->normal = tangentialVector.unit();
	fricIr->depth = tangentialVector.norm();
	sh->irs.push_back(fricIr);
	
	//デバッグ用
	//DSTR << "z : (" << sh->avgBristlesDeflection.x << ", " << sh->avgBristlesDeflection.y  << ")" << std::endl;
	//DSTR << "T : " << sh->avgStickingTime[0] << std::endl;
	//DSTR << sh->pointerPos.x << "," << sh->pointerPos.z << "," << sh->pointerDepth << std::endl;
	DSTR << sh->pointerPos.x << "," << sh->pointerDepth << "," << sh->avgStickingTime << ", " << avgStickingTimeIfMoving  << "," << sh->LuGreFunctionG << "," << sh->avgBristlesDeflection.x << "," << sh->proxyPos.x << std::endl;
	return true;
}


bool PHHapticEngine::CompFrictionIntermediateRepresentation(PHHapticStepBase* he, PHHapticPointer* pointer, PHSolidPairForHaptic* sph, PHShapePairForHaptic* sh) {

	//LuGreモデルの場合の分岐
	if (pointer->frictionType == PHFrictionType::LuGre) {
		return CompLuGreFrictionIntermediateRepresentation(he, pointer, sph, sh);
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
		if (sph->frictionState == PHSolidPairForHapticIf::STATIC) {
			sh->muCur = sh->mu + sh->mu*(sh->timeVaryFrictionA * log(1 + sh->timeVaryFrictionB * (sph->fricCount + 1) * hdt));
		}
	}
	else {
		sh->muCur = sh->mu;
		if (sph->frictionState == PHSolidPairForHapticIf::STATIC) sh->muCur = sh->mu0;
	}
	
	for (int i = 0; i < Nirs; i++) {
		PHIr* ir = sh->irs[i];
		if (pointer->bTimeVaryFriction && sph->frictionState == PHSolidPairForHapticIf::DYNAMIC) {
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
		Vec3d vq = sph->relativePose * ir->pointerPointW;	//	現在の(ポインタの移動分を反映した)、位置
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
	sph->fricCount++;
	if (!bStatic) {
		if (sph->frictionState != PHSolidPairForHapticIf::DYNAMIC) {
			std::cout << " S:" << sph->fricCount;
			sph->fricCount = 0;
			sph->frictionState = PHSolidPairForHapticIf::DYNAMIC;
		}
	}
	else {
		if (sph->frictionState != PHSolidPairForHapticIf::STATIC) {
			std::cout << " D:" << sph->fricCount << std::endl;
			sph->fricCount = 0;
			sph->frictionState = PHSolidPairForHapticIf::STATIC;
		}
	}
	return true;
}

void PHHapticEngine::CompIntermediateRepresentationForDynamicProxy(PHHapticStepBase* he, PHIrs& irsNormal, PHIrs& irsFric, PHHapticPointer* pointer){
	int nNeighbors = (int)pointer->neighborSolidIDs.size();
	for (int i = 0; i < nNeighbors; i++) {
		int solidID = pointer->neighborSolidIDs[i];
		PHSolidPairForHaptic* sph = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(solidID, pointer->GetPointerID());

		//--------------------------------------------------
		/* 力覚安定化のための補間
		// Impulseの場合は相手の剛体のPoseの補間が必要。
		// LocalDynamicsの場合は法線の補間のみでよい。
		// 法線の補間はPHShapePairForHapticでやる。
		*/
		PHSolid* solid0 = (PHSolid*)he->GetSolidInHaptic(solidID)->GetLocalSolid();
		
		const double syncCount = he->GetPhysicsTimeStep() / he->GetHapticTimeStep();
		double t = he->GetLoopCount() / syncCount;
		if (t > 1.0) t = 1.0;

		sph->force.clear();
		sph->torque.clear();
		sph->lastInterpolationPose = sph->interpolationPose;
		sph->interpolationPose = solid0->GetPose();
		if (he->IsInterporate()) {
			Posed cur = solid0->GetPose();
			double dt = ((PHScene*)solid0->GetScene())->GetTimeStep();
			Posed last;
			last.Pos() = cur.Pos() - (solid0->GetVelocity() * dt + solid0->GetOrientation() * solid0->dV.v());
			last.Ori() = (cur.Ori() * Quaterniond::Rot(-solid0->v.w() * dt + -solid0->dV.w())).unit();
			sph->interpolationPose = interpolate(t, last, cur);
		}
		// 接触したとして摩擦計算のための相対位置を計算
		// 相対摩擦
		if (sph->frictionState == PHSolidPairForHapticIf::FREE) {
			sph->frictionState = PHSolidPairForHapticIf::STATIC;
			sph->contactCount = 0;
			sph->fricCount = 0;
			sph->initialRelativePose = pointer->GetPose() * sph->interpolationPose.Inv();
		}
		else {
			sph->contactCount++;
			sph->initialRelativePose = pointer->lastProxyPose * sph->lastInterpolationPose.Inv();
		}
		sph->relativePose = sph->initialRelativePose * sph->interpolationPose * pointer->GetPose().Inv();
		//DSTR << "pose" << pointer->GetPose() << std::endl;
		//DSTR << "lastProxy" << pointer->lastProxyPose << std::endl;
		//DSTR << "ini" << initialRelativePose << std::endl;
		//DSTR << "relativePose" << relativePose << std::endl;

		// 中間表現の作成
		bool bContact = false;
		for (int i = 0; i < solid0->NShape(); i++) {
			for (int j = 0; j < pointer->NShape(); j++) {
				PHShapePairForHaptic* sh = sph->GetShapePair(i, j)->Cast();
				Posed curShapePoseW[2];
				curShapePoseW[0] = sph->interpolationPose * solid0->GetShapePose(i);
				curShapePoseW[1] = pointer->GetPose() * pointer->GetShapePose(j);
				CompIntermediateRepresentationShapeLevel(solid0, pointer, sph, sh, curShapePoseW, t, he->IsInterporate(), pointer->bMultiPoints);
				if (sh->irs.size()) {
					bContact = true;
					irsNormal.insert(irsNormal.end(), sh->irs.begin(), sh->irs.end());
				}
				if (pointer->bFriction) {
					CompFrictionIntermediateRepresentation(he, pointer, sph, sh);
					irsFric.insert(irsFric.end(), sh->irs.begin()+sh->nIrsNormal, sh->irs.end());
				}
			}
		}
		if (!bContact) {
			// 接触なし
			sph->frictionState = PHSolidPairForHapticIf::FREE;
			sph->initialRelativePose = Posed();
			sph->relativePose = Posed();
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
			PHSolidIf* localSolid = he->GetSolidInHaptic(irsAll[i]->solidID)->GetLocalSolid();
			PHSolidPairForHaptic* sph = (PHSolidPairForHaptic*)he->GetSolidPairInHaptic(irsAll[i]->solidID, pointer->GetPointerID());
			assert(sph == irsAll[i]->solidPair);
			sph = irsAll[i]->solidPair;
			sph->force += irsAll[i]->force;	// あるポインタが剛体に加える力
			sph->torque += (irsAll[i]->contactPointW - localSolid->GetCenterPosition()) ^ irsAll[i]->force;
		}
		//	摩擦振動提示の大きさに使うため、SolidPairに摩擦力の合計を計算して記録する
		if (irsFric.size()) {
			PHSolidPairForHaptic* sph = irsFric[0]->solidPair;
			if (sph->frictionState == PHSolidPairForHapticIf::STATIC) {
				sph->lastStaticFrictionForce.clear();
				for (size_t i = 0; i < irsFric.size(); i++) {
					sph->lastStaticFrictionForce += irsFric[i]->force;
					if (sph != irsFric[i]->solidPair) {
						sph = irsFric[i]->solidPair;
						sph->lastStaticFrictionForce.clear();
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
		PHSolidPairForHaptic* sph = (PHSolidPairForHaptic*)&*he->GetSolidPairInHaptic(solidID, pointer->GetPointerID());
		PHSolid* solid = he->GetSolidInHaptic(solidID)->GetLocalSolid()->Cast();
		if (sph->frictionState == PHSolidPairForHapticIf::FREE) continue;
		if (sph->contactCount == 0) {
			sph->contactVibrationVel = pointer->GetVelocity() - solid->GetVelocity();
		}
		Vec3d vibV = sph->contactVibrationVel;
		double vibA = solid->GetShape(0)->GetVibA();
		double vibB = solid->GetShape(0)->GetVibB();
		double vibW = solid->GetShape(0)->GetVibW();
		double vibT = sph->contactCount * hdt;

		SpatialVector vibForce;
		// 法線方向に射影する必要がある？
		vibForce.v() = vibA * vibV * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT) / pointer->GetPosScale();		//振動計算
		if (sph->frictionState == PHSolidPairForHapticIf::DYNAMIC) {
			Vec3d vibV = sph->lastStaticFrictionForce * hdt * pointer->GetMassInv() * 0.3;	//	0.3は謎係数。ないと接触の振動に対して強すぎてしまう。
			double vibT = sph->fricCount * hdt;
			vibForce.v() += vibA * vibV * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT) / pointer->GetPosScale();		//振動計算

		}
		pointer->AddHapticForce(vibForce);
		//CSVOUT << vibForce.v().x << "," << vibForce.v().y << "," << vibForce.v().z << std::endl;
	}
}

}

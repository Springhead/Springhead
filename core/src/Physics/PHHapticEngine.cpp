#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticStepMulti.h>
#include <Physics/PHHapticStepImpulse.h>
#include <Physics/PHHapticStepSingle.h>
#include <Physics/PHHapticStepLocalDynamics.h>
#include <Physics/LuGre/coordinateConverter.h>
#include <Physics/PHHapticStepLocalDynamicsDev.h>

namespace Spr {;

//----------------------------------------------------------------------------
// PHSolidForHaptic

PHSolidForHaptic::PHSolidForHaptic() {
	bPointer = false;
	doSim = 0;
	NLocalFirst = 0;
	NLocal = 0;
}
void PHSolidForHaptic::AddForce(Vec3d f) {
	force += f;
}
void PHSolidForHaptic::AddForce(Vec3d f, Vec3d r) {
	torque += (r - localSolid.pose * localSolid.center) ^ f;
	force += f;
}
void PHSolidForHaptic::CopyFromPhysics(PHSolidForHaptic* phys) {
	*(PHSolidForHapticSt*)this = *phys;
	*(PHSolidForHapticSt2*)this = *phys;
	localSolid = phys->localSolid;
	NLocal = phys->NLocal;
	NLocalFirst = phys->NLocalFirst;
}
void PHSolidForHaptic::CopyFromHaptics(PHSolidForHaptic* haptics) {
	PHSolidForHapticSt* hst = (PHSolidForHapticSt*)haptics;
	PHSolidForHapticSt* pst = (PHSolidForHapticSt*)this;
	*pst = *hst;
}

//----------------------------------------------------------------------------
// PHShapePairForHaptic
PHShapePairForHaptic::PHShapePairForHaptic() {
	springK = 0;
	damperD = 0;
	mu = 0;
	mu0 = 0;
	timeVaryFrictionA = 0;
	timeVaryFrictionB = 0;
	timeVaryFrictionC = 0;
	frictionViscosity = 0;
	stribeckVelocity = 0;
	stribeckmu = 0;
	muCur = 0;
	nIrsNormal = 0;

	hasContact = false;
	bristlesSpringK = 0;
	bristlesDamperD = 0;
	bristlesViscosityV = 0;
	avgBristlesDeflection = Vec2d();
	avgBristlesDeflectionVel = Vec2d();
	contactSurfacePose = Posed();
	pointerPos = Vec3d();
	proxyPos = Vec3d();
	objectVel = Vec3d();
	relativeVelOnSurface = Vec2d();
	avgStickingTime = Vec2d();
	frictionForce = Vec2d();
	LuGreParameterA = 0.5;
	LuGreParameterB = 2.0;
	LuGreParameterC = 10.0;
	LuGreFunctionG = Vec2d(LuGreParameterA, LuGreParameterA);
}

void PHShapePairForHaptic::CopyFromPhysics(const PHShapePairForHaptic* src) {
	*(CDShapePairState*)this = *src;
	state = src->state;
	for (int i = 0; i < 2; ++i) {
		closestPoint[i] = src->closestPoint[i];
		shape[i] = src->shape[i];
		shapePoseW[i] = src->shapePoseW[i];
	}

	commonPoint = src->commonPoint;
	center = src->center;
	iNormal = src->iNormal;

	lastNormal = src->lastNormal;			///< 前回の近傍物体の提示面の法線
}

void PHShapePairForHaptic::CopyFromHaptics(const PHShapePairForHaptic* src) {
}

void PHShapePairForHaptic::Init(PHSolidPair* sp, PHFrame* fr0, PHFrame* fr1) {
	PHShapePair::Init(sp, fr0, fr1);
	UpdateCache();
}

void PHShapePairForHaptic::UpdateCache() {
	springK = (shape[0]->GetReflexSpring() + shape[1]->GetReflexSpring()) * 0.5;
	damperD = (shape[0]->GetReflexDamper() + shape[1]->GetReflexDamper()) * 0.5;
	mu = (shape[0]->GetDynamicFriction() + shape[1]->GetDynamicFriction()) * 0.5;
	mu0 = (shape[0]->GetStaticFriction() + shape[1]->GetStaticFriction()) * 0.5;
	timeVaryFrictionA = (shape[0]->GetMaterial().timeVaryFrictionA + shape[1]->GetMaterial().timeVaryFrictionA) * 0.5;
	timeVaryFrictionB = (shape[0]->GetMaterial().timeVaryFrictionB + shape[1]->GetMaterial().timeVaryFrictionB) * 0.5;
	timeVaryFrictionC = (shape[0]->GetMaterial().timeVaryFrictionC + shape[1]->GetMaterial().timeVaryFrictionC) * 0.5;
	frictionViscosity = (shape[0]->GetMaterial().frictionViscosity + shape[1]->GetMaterial().frictionViscosity) * 0.5;
	stribeckVelocity = (shape[0]->GetMaterial().stribeckVelocity + shape[1]->GetMaterial().stribeckVelocity) * 0.5;
	stribeckmu = (shape[0]->GetMaterial().stribeckmu + shape[1]->GetMaterial().stribeckmu) * 0.5;

	bristlesSpringK = ((double)(shape[0]->GetMaterial().bristlesSpringK) + (double)(shape[1]->GetMaterial().bristlesSpringK)) * 0.5;			///< LuGreモデルにおける剛毛のバネ係数
	bristlesDamperD = ((double)(shape[0]->GetMaterial().bristlesDamperD) + (double)(shape[1]->GetMaterial().bristlesDamperD)) * 0.5;			///< LuGreモデルにおける剛毛のダンパ係数
	bristlesViscosityV = ((double)(shape[0]->GetMaterial().bristlesViscosityV) + (double)(shape[1]->GetMaterial().bristlesViscosityV)) * 0.5;	///< LuGreモデルにおける剛毛にはたらく粘性抵抗の係数
}

bool PHShapePairForHaptic::Detect(unsigned ct, const Posed& pose0, const Posed& pose1){
	// 0:剛体, 1:力覚ポインタ
	// 前回の状態を保存
	for(int i = 0; i < 2; i++){
		lastShapePoseW[i] = shapePoseW[i];
		lastClosestPoint[i] = closestPoint[i];	
	}
	lastNormal = normal;

	// 今回の状態を保存
	shapePoseW[0] = pose0;
	shapePoseW[1] = pose1;

	// 最近傍点対を見つける
	Vec3d sep;
	double dist = FindClosestPoints(shape[0], shape[1], shapePoseW[0], shapePoseW[1],
									sep, closestPoint[0], closestPoint[1]);
	// 接触
	Vec3d w0 = shapePoseW[0] * closestPoint[0];
	Vec3d w1 = shapePoseW[1] * closestPoint[1];
	normal = (w1 - w0);
	if (normal.norm() > 1e-20) {
		normal = normal.unit();		// 剛体->力覚ポインタへの法線ベクトル
	}
	else {
		return false;
	}
	commonPoint = (w0 + w1) * 0.5;	// 共有点

	if(dist > 1e-3){
		// 接触していない
		state = NONE;
	}else{							
		// 接触
		if (lastContactCount == unsigned(ct-1))	state = CONTINUE;
		else state = NEW;
		lastContactCount = ct;
	}
	return true;
}

int PHShapePairForHaptic::OnDetect(unsigned ct, const Vec3d& center0){
	Vec3d dir = -1 * lastNormal;	// 力覚ポインタから見た剛体の移動方向
	if(dir == Vec3d()){
		dir = commonPoint - center0;
		DSTR << "dir==(0,0,0) n=" << normal << " use " << dir <<  " in PHShapePariForHaptic::OnDetect()" << std::endl;
	}
	double dist = 0.0;
	int cp = ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir, 
							-DBL_MAX, 1, normal, closestPoint[0], closestPoint[1], dist);
	
	/// GJKが失敗した時の処理
	if(cp != 1){
		static int cont = 0;
		ContFindCommonPointSaveParam(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir, 
							-DBL_MAX, 1, normal, closestPoint[0], closestPoint[1], dist);
		DSTR << cont << "ContFindCommonPoint do not find contact point" << std::endl;
		cont++;
		ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir,
			-DBL_MAX, 1, normal, closestPoint[0], closestPoint[1], dist);
	}
	return cp;
}

bool PHShapePairForHaptic::CompIntermediateRepresentation(Posed curShapePoseW[2], double t, bool bInterpolatePose, bool bPoints){
	irs.clear();
	nIrsNormal = 0;
	Vec3d sPoint = curShapePoseW[0] * closestPoint[0];	// 今回のsolidの近傍点（World)
	Vec3d pPoint = curShapePoseW[1] * closestPoint[1];	// 今回のpointerの近傍点（World)
	Vec3d last_sPoint = lastShapePoseW[0] * lastClosestPoint[0]; // 前回の剛体近傍点（World)
	Vec3d interpolation_normal = normal;		// 補間法線
	Vec3d interpolation_sPoint = sPoint;		// 補間剛体近傍点

	// 剛体の面の法線補間　前回の法線と現在の法線の間を補間
	interpolation_normal = interpolate(t, lastNormal, normal);

	Vec3d dir = pPoint - interpolation_sPoint;			
	double dot = dir * interpolation_normal;
	
	//DSTR << sPoint << std::endl;
	//DSTR << curShapePoseW[0] << "," << closestPoint[0] << std::endl;

	if(dot >= 0.0) return false;
	if(bPoints){
		for(int i = 0; i < (int)intersectionVertices.size(); i++){
			Vec3d iv = intersectionVertices[i];
			Vec3d wiv = curShapePoseW[1] * iv; 	// ポインタの侵入点(world)
			dot = (wiv - interpolation_sPoint) * interpolation_normal;	// デバイスの侵入点から中間面上の点へのベクトルのノルム（デバイスの侵入量）
			if(dot > 0.0)	continue;
			PHIr* ir = DBG_NEW PHIr();
			ir->normal = interpolation_normal;
			ir->pointerPointW = wiv;
			Vec3d ortho = dot * interpolation_normal; // 剛体の近傍点からデバイス侵入点までのベクトルを面法線へ射影
			ir->contactPointW = wiv - ortho;		// solidの接触点(world)
			ir->depth = ortho.norm();
			ir->interpolation_pose = curShapePoseW[0];
			irs.push_back(ir);
		}
	}else{
		PHIr* ir = DBG_NEW PHIr();
		ir->normal = interpolation_normal;
		ir->pointerPointW = pPoint;
		Vec3d ortho = dot * interpolation_normal;
		ir->contactPointW = pPoint - ortho;
		ir->depth = ortho.norm();
		ir->interpolation_pose = curShapePoseW[0];
		irs.push_back(ir);
	}
	nIrsNormal = (int)irs.size();
	return true;
}


//----------------------------------------------------------------------------
// PHSolidPairForHaptic

PHSolidPairForHaptic::PHSolidPairForHaptic(){
	solidID[0] = -1;
	solidID[1] = -1;
}
void PHSolidPairForHaptic::CopyForDisplay(const PHSolidPairForHaptic* src) {
	CopyFromPhysics(src);
}

void PHSolidPairForHaptic::CopyFromPhysics(const PHSolidPairForHaptic* phys) {
	//	Copy except shapePairs and listeners.
	const PHSolidPairForHaptic* src = phys;
	*(PHSolidPairForHapticVars*)this = *(PHSolidPairForHapticVars*)src;
	*(PHSolidPairVars*)this = *(PHSolidPairVars*)src;

	//	Update shapePairs
	this->shapePairs.resize(src->shapePairs.height(), src->shapePairs.width());
	for (int i = 0; i < src->shapePairs.height(); i++) {
		for (int j = 0; j < src->shapePairs.width(); j++) {
			const PHShapePairForHaptic* srcPair = src->GetShapePair(i, j)->Cast();
			PHShapePairForHaptic* destPair = GetShapePair(i, j)->Cast();
			if (destPair) {
				destPair->CopyFromPhysics(srcPair);
			}
			else {
				PHShapePairForHaptic* destPair = DBG_NEW PHShapePairForHaptic(); 
				destPair->Init(srcPair->solidPair, srcPair->frame[0], srcPair->frame[1]);
				destPair->CopyFromPhysics(srcPair);
				shapePairs.item(i, j) = destPair;
			}
		}
	}
}
void PHSolidPairForHaptic::CopyFromHaptics(const PHSolidPairForHaptic* hpair) {
	PHSolidPairForHapticVarsBase* hVars = (PHSolidPairForHapticVarsBase*)hpair;
	PHSolidPairForHaptic* ppair = this;
	PHSolidPairForHapticVarsBase* pVars = (PHSolidPairForHapticVarsBase*)ppair;
	*pVars = *hVars;	// haptic側で保持しておくべき情報を同期
	//	ShapePairの情報のコピー
	for (int h = 0; h < hpair->shapePairs.height(); ++h) {
		for (int w = 0; w < hpair->shapePairs.width(); ++w) {
			if (h < ppair->shapePairs.height() && w < ppair->shapePairs.width()) {
				((PHShapePairForHaptic*)ppair->GetShapePair(h, w))->CopyFromHaptics((PHShapePairForHaptic*)hpair->GetShapePair(h, w));
			}
		}
	}
}

void PHSolidPairForHaptic::OnDetect(PHShapePair* _sp, unsigned ct, double dt){
	if(_sp == NULL) assert(0);
	PHSolidPair::OnDetect(_sp, ct, dt);
	
	PHShapePairForHaptic* sp = (PHShapePairForHaptic*)_sp;
	sp->intersectionVertices.clear();
	
	PHHapticPointerIf* pointer = GetSolid(1)->Cast();
	
	if(sp->state == CDShapePair::NEW || sp->state == CDShapePair::CONTINUE){
		sp->OnDetect(ct, body[1]->GetCenterPosition());	// CCDGJKで近傍点対を再取得
		sp->intersectionVertices.push_back(sp->closestPoint[1]);
	}else{
		// 接触していない場合、近傍点を侵入領域の頂点とする
		sp->intersectionVertices.push_back(sp->closestPoint[1]);
	}

	// はじめて近傍の場合
	if(inLocal == 1){
		//DSTR << "To be in Local at first" << std::endl;
		for(int i = 0; i < 2; i++){
			sp->lastShapePoseW  [i] = sp->shapePoseW  [i];
			sp->lastClosestPoint[i] = sp->closestPoint[i];
		}
		sp->lastNormal = sp->normal;
	}	

	//CSVOUT << (sp->shapePoseW[0] * sp->closestPoint[0]).y << "," << (sp->shapePoseW[1] * sp->closestPoint[1]).y << std::endl;
}

//----------------------------------------------------------------------------
// PHHapticEngine
PHHapticEngineDesc::PHHapticEngineDesc(){
}

PHHapticEngine::PHHapticEngine(){
	bEnabled = false;
	bPhysicStep   = true;
	hapticStep = DBG_NEW PHHapticStepImpulse();
	hapticStep->engine = this;
	hapticStepMode = MULTI_THREAD;
}
void PHHapticEngine::Step() { if (bEnabled && bPhysicStep) hapticStep->Step1(); }
void PHHapticEngine::Step2() { if (bEnabled && bPhysicStep) hapticStep->Step2(); }
void PHHapticEngine::StepHapticLoop() { if (bEnabled) hapticStep->StepHapticLoop(); }
void PHHapticEngine::StepHapticSync() { if (bEnabled) hapticStep->StepHapticSync(); }
void PHHapticEngine::StepPhysicsSimulation() { hapticStep->StepPhysicsSimulation(); }
bool PHHapticEngine::SetCallbackBeforeStep(PHHapticEngineIf::Callback f, void* arg) { return hapticStep->SetCallbackBeforeStep(f, arg); }
bool PHHapticEngine::SetCallbackAfterStep(PHHapticEngineIf::Callback f, void* arg) { return hapticStep->SetCallbackAfterStep(f, arg); }

int PHHapticEngine::NSolidsInHaptic() {
	if (!hapticStep) return 0;
	return hapticStep->NSolidsInHaptic();
}
int PHHapticEngine::NPointersInHaptic() {
	if (!hapticStep) return 0;
	return hapticStep->NPointersInHaptic();
}

PHHapticPointerIf* PHHapticEngine::GetPointerInHaptic(int i) {
	if (!hapticStep) return NULL;
	return (PHHapticPointerIf*)hapticStep->GetPointerInHaptic(i);
}
PHSolidPairForHapticIf* PHHapticEngine::GetSolidPairInHaptic(int i, int j) {
	if (!hapticStep) return NULL;
	return (PHSolidPairForHapticIf*) hapticStep->GetSolidPairInHaptic(i, j);
}

void PHHapticEngine::SetHapticStepMode(HapticStepMode mode){
	hapticStepMode = mode;
	switch(hapticStepMode){
		case SINGLE_THREAD:		
			hapticStep = DBG_NEW PHHapticStepSingle();
			break;		
		case MULTI_THREAD:		
			hapticStep = DBG_NEW PHHapticStepImpulse();
			break;
		case LOCAL_DYNAMICS:
			hapticStep = DBG_NEW PHHapticStepLocalDynamics();
			break;
		case LOCAL_DYNAMICS6DOF:
			hapticStep = DBG_NEW PHHapticStepLocalDynamicsDev();
			break;
		default:
			assert(0);
			return;
	}
	hapticStep->engine = this;
}

void PHHapticEngine::StartDetection(){
	// 物理シーンでのHapticPointerの接触判定の無効化
	DisablePointerContactDetectionInPhysics();
	// AABBの更新
	UpdateEdgeList();
	// 力覚ポインタごとに近傍物体を見つける
	for(int i = 0; i < (int)hapticPointers.size(); i++){
		Detect(hapticPointers[i]);
	}
	// ローカルでシミュレーションするかどうかのフラグを設定
	for(int i = 0; i < (int)hapticSolids.size(); i++){
		PHSolidForHaptic* h = hapticSolids[i];
		if(DCAST(PHHapticPointer, h->sceneSolid)) continue;
		if(h->NLocal == 0 && h->NLocalFirst > 0){
			h->doSim = 1;			// はじめて近傍になった
		}else if(h->NLocal > 0){
			h->doSim = 2;			// 近傍状態を継続
		}else{
			h->doSim = 0;			// 近傍でない
		}
		h->NLocal = 0; 
		h->NLocalFirst = 0;
	}
}

void PHHapticEngine::UpdateEdgeList(){
	edges.clear();
	Vec3f dir[3] = { Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1) };
	for(int i = 0; i < (int)bodies.size(); i++){
		Edge e;
		for(int j = 0; j < 3; j++){
			float min, max = 0.0f;
			bodies[i]->GetBBoxSupport(dir[j], min, max);
			e.min[j] = min; e.max[j] = max;
		}
		edges.push_back(e);
	}
}

void PHHapticEngine::Detect(PHHapticPointer* pointer){
	int ct = GetScene()->GetCount();
	int dt = GetScene()->GetTimeStep();
	const int pointerSolidID = pointer->GetSolidID();
	Vec3f range3d = Vec3f(1.0, 1.0, 1.0) * pointer->GetLocalRange(); // 検出閾値
	// pointerのBBoxをrange3d分だけ拡げる
	edges[pointerSolidID].min -= range3d;
	edges[pointerSolidID].max += range3d;
	
	Vec3f pMin = edges[pointerSolidID].min;
	Vec3f pMax = edges[pointerSolidID].max;
	int N = (int)hapticSolids.size();
	pointer->neighborSolidIDs.clear();
	for(int i = 0; i < N; i++){
		if(i == pointerSolidID) continue;	// ポインタと剛体が同じ場合
		Vec3f soMin = edges[i].min;
		Vec3f soMax = edges[i].max;
		int nAxes = 0;		// いくつの軸で交差しているかどうか
		for(int j = 0; j < 3; j++){
			int inAxis = 0;	// その軸で交差しているかどうか
			// pointerのBBox内にsolidのBBoxがあったら交差
			if(pMin[j] <= soMin[j] && soMin[j] <= pMax[j]) inAxis++; 
			if(pMin[j] <= soMax[j] && soMax[j] <= pMax[j]) inAxis++; 
			// ソリッドのBox内にクエリのBBoxがあったら交差
			if(soMin[j] <= pMin[j] && pMin[j] <= soMax[j]) inAxis++;
			if(soMin[j] <= pMax[j] && pMax[j] <= soMax[j]) inAxis++;
			// inが1以上ならその軸で交差
			if(inAxis > 0) nAxes++;
#if 0
				DSTR << i << " pMin[i] = " << pMin[i] << "  soMin[i] = " << soMin[i] << "  pMax[i] = " << pMax[i] << std::endl;
				DSTR << i << " pMin[i] = "  << pMin[i] << "  soMax[i] = " << soMax[i] << "  pMax[i] = " << pMax[i] << std::endl;
				DSTR << i << " soMin[i] = " << soMin[i] << "  pMin[i] = " << pMin[i] << "  soMax[i] = " << soMax[i] << std::endl;
				DSTR << i << " soMin[i] = " << soMin[i] << "  pMax[i] = " << pMax[i] << "  soMax[i] = " << soMax[i] << std::endl;
			}
			DSTR << "nAxes" << nAxes <<  std::endl;
			DSTR << "------------------------" << std::endl;
#else
		}
#endif
#if 1
		// 2.近傍物体と判定
		const int pointerID = pointer->GetPointerID();
		PHSolidPairForHaptic* solidPair = GetSolidPairImp(i, pointerID);
		if(DCAST(PHHapticPointer, solidPair->body[0])) continue;	// 剛体がポインタの場合
		if (!solidPair->bEnabled) continue;
		if(nAxes == 3){
			// 形状毎の近傍点探索、接触解析
			int ns0 = solidPair->body[0]->NShape();
			int ns1 = solidPair->body[1]->NShape();
			for(int is0 = 0; is0 < ns0; is0++)for(int is1 = 0; is1 < ns1; is1++){
				solidPair->Detect(solidPair->GetShapePair(is0, is1)->Cast(), ct, dt, false);
			}
			pointer->neighborSolidIDs.push_back(i);
			PHSolidForHaptic* h = hapticSolids[i];
			h->localSolid = *h->sceneSolid;	// 近傍と判定されたのでコピー
			assert(std::isfinite(h->localSolid.pose.px));
			if(solidPair->inLocal == 0){
				// 初めて近傍になった
				solidPair->inLocal = 1;	
				h->NLocalFirst += 1;
			}else{
				// 継続して近傍である
				solidPair->inLocal = 2;
				h->NLocal += 1;
			}
			// グラフィクス表示用にコピーをとる
			*solidPairsTemp.item(i, pointerID) = *solidPair;
		}else{
			// 近傍でない
			solidPair->inLocal = 0;
		}
#endif
	}
}

bool PHHapticEngine::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s && std::find(bodies.begin(), bodies.end(), s) == bodies.end()){
		// bodiesにsolidを保存(pointerも含まれる)
		bodies.push_back(s);				
		PHSolidForHaptic* h = DBG_NEW PHSolidForHaptic();
		h->sceneSolid = s;
		h->localSolid = *s;
		hapticSolids.push_back(h);

		int NSolids = (int)bodies.size();
		int NPointers = (int)hapticPointers.size();

		// PHSolidPairFoHapticを追加 行 solid, 列 pointer
		// PHSolidがPHHapticPointerの場合
		// 列を追加
		PHHapticPointer* p = DCAST(PHHapticPointer, o);
		if(p){
			NPointers += 1;
			int pointerID = NPointers - 1;
			p->SetPointerID(pointerID);
			p->SetSolidID(NSolids - 1);
			hapticPointers.push_back(p);
			h->bPointer = true;
			solidPairs    .resize(NSolids, NPointers);
			solidPairsTemp.resize(NSolids, NPointers);
			for(int i = 0; i < NSolids; i++){
				PHSolidPairForHaptic* solidPair = (PHSolidPairForHaptic*)CreateSolidPair();
				solidPair->Init(this, bodies[i], s);
				solidPair->solidID[0] = i;
				solidPair->solidID[1] = NSolids - 1;
				solidPairs.item(i, pointerID) = solidPair;
				// fwSceneで描画するための一時領域を確保
				PHSolidPairForHaptic* solidPairTemp = (PHSolidPairForHaptic*)CreateSolidPair();
				solidPairTemp->CopyForDisplay(solidPair);
				solidPairsTemp.item(i, pointerID) = solidPairTemp;
			}
			if(s->NShape())	UpdateShapePairs(s);
		}

		// PHSolidの場合
		// 行のみ追加
		solidPairs.resize    (NSolids, NPointers);
		solidPairsTemp.resize(NSolids, NPointers);
		for(int i = 0; i < NPointers; i++){
			PHSolidPairForHaptic* solidPair = (PHSolidPairForHaptic*)CreateSolidPair();
			solidPair->Init(this, bodies[NSolids - 1], hapticPointers[i]);	
			solidPair->solidID[0] = NSolids - 1;	
			solidPair->solidID[1] = hapticPointers[i]->GetSolidID();
			solidPairs.item(NSolids - 1, i) = solidPair;
			PHSolidPairForHaptic* solidPairTemp = (PHSolidPairForHaptic*)CreateSolidPair();
			solidPairTemp->CopyForDisplay(solidPair);
			solidPairsTemp.item(NSolids - 1, i) = solidPairTemp;
		}
		if(s->NShape())	UpdateShapePairs(s);

		return true;
	}
	return false;
}

// 未完成
bool PHHapticEngine::DelChildObject(ObjectIf* o){
	//PHSolid* s = DCAST(PHSolid, o);
	//if(s){
	//	PHSolids::iterator is = find(bodies.begin(), bodies.end(), s);
	//	if(is != bodies.end() && bodies.size() > 0){
	//		int idx = (int)(is - bodies.begin());
	//		bodies.erase(is);
	//		solidPairs.erase_col(idx);
	//		PHSolidsForHaptic::iterator it;
	//		for(it = hapticSolids.begin(); it < hapticSolids.end(); it++){
	//			if(DCAST(PHSolid, (*it)->sceneSolid) == s) hapticSolids.erase(it);
	//		}
	//		PHHapticPointer* p = DCAST(PHHapticPointer, o);
	//		if(p){
	//			PHHapticPointers::iterator ip = find(hapticPointers.begin(), hapticPointers.end(), p);
	//			if(ip != hapticPointers.end() && hapticPointers.size() > 0){
	//				hapticPointers.erase(ip);
	//			}
	//		}
	//	}
	//	return true;
	//}
	return false;
}

void PHHapticEngine::UpdateShapePairs(PHBody* solid){
	PHBodies::iterator it = std::find(bodies.begin(), bodies.end(), solid);
	if(it == bodies.end())
		return;
	int isolid = (int)(it - bodies.begin());
	int i, j;
	PHSolidPairForHaptic* sp;
	PHBody* s[2];
	// solidの場合(行の更新）
	for(i = 0; i < NPointers(); i++){
		sp = GetSolidPairImp(isolid, i);
		s[0] = solid;
		s[1] = sp->body[1];
		sp->shapePairs.resize(s[0]->NShape(), s[1]->NShape());
		for(j = 0; j < s[1]->NShape(); j++){
			PHShapePairForHaptic* n = sp->CreateShapePair();
			n->Init(sp, s[0]->GetFrame(s[0]->NShape() - 1)->Cast(), s[1]->GetFrame(j)->Cast());
			sp->shapePairs.item(s[0]->NShape() - 1, j) = n;
		}
	}

	PHHapticPointer* pointer = DCAST(PHHapticPointer, solid);
	if(!pointer) return;
	// PHHapticPointerの場合（列の更新）
	int pointerID = pointer->GetPointerID();
	int pointerSolidID = pointer->GetSolidID();
	for(i = 0; i < (int)bodies.size(); i++){
		if(i == pointerSolidID) continue;
		sp = GetSolidPairImp(i, pointerID);
		s[0] = sp->body[0];
		s[1] = solid;
		sp->shapePairs.resize(s[0]->NShape(), s[1]->NShape());
		for(j = 0; j < s[0]->NShape(); j++){
			PHShapePairForHaptic* n = sp->CreateShapePair();
			n->Init(sp, s[0]->GetFrame(j)->Cast(), s[1]->GetFrame(s[1]->NShape() - 1)->Cast());
			sp->shapePairs.item(j, s[1]->NShape()-1) = n;
		}
	}
}

void PHHapticEngine::DisablePointerContactDetectionInPhysics(){
	// 力覚ポインタをシーンの接触から切る
	for(int i = 0; i < (int)hapticPointers.size(); i++){
		GetScene()->SetContactMode(hapticPointers[i]->Cast(), PHSceneDesc::MODE_NONE);
	}
}

PHHapticEngineDesc::HapticStepMode PHHapticEngine::GetHapticStepMode(){
	return hapticStepMode;
}

void PHHapticEngine::ReleaseState(){
	hapticStep->ReleaseState(GetScene());
}

void PHHapticEngine::EnableContact(PHSolidIf* lhs, PHSolidIf* rhs, bool bEnable){
	PHSolidIf *soLatter = NULL;
	for (int i = 0; i < solidPairs.height(); ++i) {
		for (int j = 0; j < solidPairs.width(); ++j) {
			PHSolidIf *s0 = solidPairs.item(i, j)->body[0]->Cast(), *s1 = solidPairs.item(i, j)->body[1]->Cast();
			if ( ((s0 == lhs) && (s1 == rhs)) || ((s0 == rhs) && (s1 == lhs)) ) {
				solidPairs.item(i, j)->bEnabled = bEnable;
			}
		}
	}
}

void PHHapticEngine::EnableContact(PHSolidIf** group, size_t length, bool bEnable){
	// <!!> Not Implemented Yet
	/*
	std::vector<int> idx;
	PHSolids::iterator it;
	for (int i = 0; i < (int)length; i++){
		it = find(bodies.begin(), bodies.end(), (PHSolid*)(group[i]->Cast()));
		if (it != bodies.end())
			idx.push_back((int)(it - bodies.begin()));
	}
	sort(idx.begin(), idx.end());
	for (int i = 0; i < (int)idx.size(); i++){
		for (int j = i + 1; j < (int)idx.size(); j++){
			solidPairs.item(idx[i], idx[j])->bEnabled = bEnable;
		}
	}
	*/
}

void PHHapticEngine::EnableContact(PHSolidIf* solid, bool bEnable){
	for (int i = 0; i < solidPairs.height(); ++i) {
		for (int j = 0; j < solidPairs.width(); ++j) {
			PHSolidIf *s0 = solidPairs.item(i, j)->body[0]->Cast(), *s1 = solidPairs.item(i, j)->body[1]->Cast();
			if (s0==solid || s1==solid) {
				solidPairs.item(i, j)->bEnabled = bEnable;
			}
		}
	}
}

void PHHapticEngine::EnableContact(bool bEnable){
	for (int i = 0; i<solidPairs.height(); ++i) {
		for (int j=0; j<solidPairs.width(); ++j) {
			solidPairs.item(i, j)->bEnabled = bEnable;
		}
	}
}

} // namespace Spr

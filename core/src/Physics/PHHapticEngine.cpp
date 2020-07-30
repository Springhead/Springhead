#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticStepMulti.h>
#include <Physics/PHHapticStepImpulse.h>
#include <Physics/PHHapticStepSingle.h>
#include <Physics/PHHapticStepLocalDynamics.h>

namespace Spr {
	;

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

		mus = {0,0,0,0,0};
		mu0s = { 0,0,0,0,0};
	
		timeVaryFrictionAs = { 0,0,0,0,0 };
		timeVaryFrictionBs = { 0,0,0,0,0 };
		timeVaryFrictionCs = { 0,0,0,0,0,0 };
		stribeckVelocitys = { 0,0,0,0,0 };
		stribeckmus = { 0,0,0,0,0 };
		timeVaryFrictionDs = { 0,0,0,0,0 };
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

		for (int i = 0; i < shape[0]->GetMaterial().timeVaryFrictionAs.size(); i++) {
			mus[i] = (shape[0]->GetMaterial().mus[i] + shape[1]->GetMaterial().mus[i]) * 0.5;
			mu0s[i] = (shape[0]->GetMaterial().mu0s[i] + shape[1]->GetMaterial().mu0s[i]) * 0.5;
			timeVaryFrictionAs[i] = (shape[0]->GetMaterial().timeVaryFrictionAs[i] + shape[1]->GetMaterial().timeVaryFrictionAs[i]) * 0.5;
			timeVaryFrictionBs[i] = (shape[0]->GetMaterial().timeVaryFrictionBs[i] + shape[1]->GetMaterial().timeVaryFrictionBs[i]) * 0.5;
			stribeckVelocitys[i] = (shape[0]->GetMaterial().stribeckVelocitys[i] + shape[1]->GetMaterial().stribeckVelocitys[i]) * 0.5;
			stribeckmus[i] = (shape[0]->GetMaterial().stribeckmus[i] + shape[1]->GetMaterial().stribeckmus[i]) * 0.5;
		}


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

#define SELECTION 1 // 中間表現の面に載っている点は接触点としない
extern bool bUseContactVolume;
bool PHShapePairForHaptic::AnalyzeContactRegion(){
	bUseContactVolume = true;
	static CDContactAnalysis analyzer;
	analyzer.FindIntersection(this->Cast());
	bUseContactVolume = false;

	// 侵入領域の頂点の取得
	for(CDQHPlane< CDContactAnalysisFace >* it = analyzer.planes.begin; it != analyzer.planes.end; ++it){
		if(it->deleted) continue;
		Vec3d point = it->normal/it->dist + commonPoint;	// 双対変換（面から点へ）	

		// 0:剛体, 1:力覚ポインタ
		if(SELECTION){
			Vec3d w0 = shapePoseW[0] * closestPoint[0];	// 中間表現面上の点（剛体の近傍点）
			double dot = (point - w0) * normal;
			if(dot < -5e-008)	intersectionVertices.push_back(shapePoseW[1].Inv() * point);
		}else{
			intersectionVertices.push_back(shapePoseW[1].Inv() * point);
		} 
	}
	// 面が見つからなかった、shapeがconvex or boxではなかった場合
	if(intersectionVertices.size() == 0){
		intersectionVertices.push_back(closestPoint[1]);
		return false;
	}
	return true;
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
PHSolidPairForHaptic::PHSolidPairForHaptic(const PHSolidPairForHaptic& s){
	*this = s;
	for(int i = 0; i < s.shapePairs.height(); i++){
		for(int j = 0; j < s.shapePairs.width(); j++){
			const PHShapePairForHaptic* src = s.GetShapePair(i, j)->Cast();
			if (src) shapePairs.item(i, j) = DBG_NEW PHShapePairForHaptic(*src);
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
		if(pointer->IsMultiPoints()){
			sp->AnalyzeContactRegion();		// 侵入領域の頂点を取得
		}else{
			sp->intersectionVertices.push_back(sp->closestPoint[1]);
		}
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
	hapticSteps.push_back(hapticStep);
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
			for(int i = 0; i < (int)hapticSteps.size(); i++){
				if(DCAST(PHHapticStepSingle, hapticSteps[i])){
					hapticStep = hapticSteps[i];
					return;
				}
			}
			hapticStep = DBG_NEW PHHapticStepSingle();
			break;		
		case MULTI_THREAD:		
			for(int i = 0; i < (int)hapticSteps.size(); i++){
				if(DCAST(PHHapticStepImpulse, hapticSteps[i])){
					hapticStep = hapticSteps[i];
					return;
				}
			}
			hapticStep = DBG_NEW PHHapticStepImpulse();
			break;
		case LOCAL_DYNAMICS:
			for(int i = 0; i < (int)hapticSteps.size(); i++){
				if(DCAST(PHHapticStepLocalDynamics, hapticSteps[i])){
					hapticStep = hapticSteps[i];
					return;
				}
			}
			hapticStep = DBG_NEW PHHapticStepLocalDynamics();
			break;
		default:
			assert(0);
			return;
	}
	hapticStep->engine = this;
	hapticSteps.push_back(hapticStep);
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
		PHSolidPairForHaptic* solidPair = GetSolidPair(i, pointerID)->Cast();
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
			*h->GetLocalSolid() = *h->sceneSolid;	// 近傍と判定されたのでコピー
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
		*h->GetLocalSolid() = *s;
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
				*solidPairTemp = *solidPair;
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
			*solidPairTemp = *solidPair;
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
		sp = GetSolidPair(isolid, i)->Cast();
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
		sp = GetSolidPair(i, pointerID)->Cast();
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

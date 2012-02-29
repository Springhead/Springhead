#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticRender.h>
#include <Physics/PHHapticEngineImpulse.h>
#include <Physics/PHHapticEngineSingleBase.h>
#include <Physics/PHHapticEngineLD.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHSolidForHaptic

PHSolidForHaptic::PHSolidForHaptic(){
	bPointer = false;	
	doSim = 0;	
	NLocalFirst = 0;
	NLocal = 0;
}
void PHSolidForHaptic::AddForce(Vec3d f){
	force += f;
}
void PHSolidForHaptic::AddForce(Vec3d f, Vec3d r){
	torque += (r - localSolid.pose * localSolid.center) ^ f;
	force += f;
}
//----------------------------------------------------------------------------
// PHSapePairForHaptic
PHShapePairForHaptic::PHShapePairForHaptic(){}
PHShapePairForHaptic::PHShapePairForHaptic(const PHShapePairForHaptic& s){
	*this = s;
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
	Vec3d w0 = shapePoseW[0] * closestPoint[0];
	Vec3d w1 = shapePoseW[1] * closestPoint[1];
	normal = (w1 - w0).unit();		// 剛体->力覚ポインタへの法線ベクトル
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
		DSTR << "dir = Vec3d() in PHShapePariForHaptic::OnOnDetectClosestPoints" << std::endl;
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
			if(dot < -1e-5)	intersectionVertices.push_back(shapePoseW[1].Inv() * point);
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
	PHIr irtemp;
	irtemp.springK = (shape[0]->GetReflexSpring() + shape[1]->GetReflexSpring()) * 0.5;
	irtemp.damperD = (shape[0]->GetReflexDamper() + shape[1]->GetReflexDamper()) * 0.5;
	irtemp.mu = (shape[0]->GetDynamicFriction() + shape[1]->GetDynamicFriction()) * 0.5;
	irtemp.mu0 = (shape[0]->GetStaticFriction() + shape[1]->GetStaticFriction()) * 0.5;

	if(bPoints){
		for(int i = 0; i < (int)intersectionVertices.size(); i++){
			Vec3d iv = intersectionVertices[i];
			Vec3d wiv = curShapePoseW[1] * iv; 	// ポインタの侵入点(world)
			dot = (wiv - interpolation_sPoint) * interpolation_normal;	// デバイスの侵入点から中間面上の点へのベクトルのノルム（デバイスの侵入量）
			if(dot > 0.0)	continue;
			PHIr* ir = DBG_NEW PHIr();
			*ir = irtemp;
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
		*ir = irtemp;
		ir->normal = interpolation_normal;
		ir->pointerPointW = pPoint;
		Vec3d ortho = dot * interpolation_normal;
		ir->contactPointW = pPoint - ortho;
		ir->depth = ortho.norm();
		ir->interpolation_pose = curShapePoseW[0];
		irs.push_back(ir);
	}
	//std::sort(irs.begin, irs.end()):
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
	const int height = s.shapePairs.height();
	const int width = s.shapePairs.width();
	shapePairs.resize(height, width);
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			shapePairs.item(i, j) = DBG_NEW PHShapePairForHaptic(*s.shapePairs.item(i, j));
		}
	}
}


bool PHSolidPairForHaptic::Detect(engine_type* engine, unsigned int ct, double dt){
	return PHSolidPair::Detect(engine, ct, dt);
}

void PHSolidPairForHaptic::OnDetect(PHShapePairForHaptic* sp, PHHapticEngine* engine, unsigned ct, double dt){
	sp->intersectionVertices.clear();
	if(sp->state == sp->NEW || sp->state == sp->CONTINUE){
		sp->OnDetect(ct, solid[1]->GetCenterPosition());	// CCDGJKで近傍点対を再取得
		sp->AnalyzeContactRegion();		// 侵入領域の頂点を取得
	}else{
		// 接触していない場合、近傍点を侵入領域の頂点とする
		sp->intersectionVertices.push_back(sp->closestPoint[1]);
	}

	// はじめて近傍の場合
	if(inLocal == 1){
		for(int i = 0; i < 2; i++){
			sp->lastShapePoseW[i] = sp->shapePoseW[i];
			sp->lastClosestPoint[i] = sp->closestPoint[i];
		}
		sp->lastNormal = sp->normal;
	}	

	//CSVOUT << (sp->shapePoseW[0] * sp->closestPoint[0]).y << "," << (sp->shapePoseW[1] * sp->closestPoint[1]).y << std::endl;
}

PHIrs PHSolidPairForHaptic::CompIntermediateRepresentation(PHSolid* curSolid[2], double t, bool bInterpolatePose, bool bMultiPoints){
	/* 力覚安定化のための補間
	// Impulseの場合は相手の剛体のPoseの補間が必要。
	// LocalDynamicsの場合は法線の補間のみでよい。
	// 法線の補間はPHShapePairForHapticでやる。
	*/
	force.clear();
	torque.clear();
	lastInterpolationPose = interpolationPose;
	interpolationPose = curSolid[0]->GetPose();
	if(bInterpolatePose){
		interpolationPose = interpolate(t, curSolid[0]->GetLastPose(), curSolid[0]->GetPose());
	}

	// 接触したとして摩擦計算のための相対位置を計算
	PHHapticPointer* pointer = DCAST(PHHapticPointer, curSolid[1]);
#if 1
	// 相対摩擦
	if(frictionState == FREE){
		frictionState = FIRST;
		contactCount = 0;
		initialRelativePose =  pointer->GetPose() * interpolationPose.Inv();
	}else{
		frictionState = STATIC;
		contactCount += 1;
		initialRelativePose =  pointer->lastProxyPose * lastInterpolationPose.Inv();
	}
	relativePose = initialRelativePose * interpolationPose * pointer->GetPose().Inv();		
#else
	// 絶対摩擦
	if(frictionState == FREE){
		frictionState = STATIC;
		initialRelativePose = Posed();
	}else{
		initialRelativePose =  pointer->lastProxyPose * pointer->GetPose().Inv();
	}
	relativePose = initialRelativePose;
#endif

	//DSTR << "pose" << pointer->GetPose() << std::endl;
	//DSTR << "lastProxy" << pointer->lastProxyPose << std::endl;
	//DSTR << "ini" << initialRelativePose << std::endl;
	//DSTR << "relativePose" << relativePose << std::endl;

	// 中間表現の作成
	PHIrs irs;
	for(int i = 0; i < curSolid[0]->NShape(); i++){
		for(int j = 0; j < curSolid[1]->NShape(); j++){
			PHShapePairForHaptic* sp = shapePairs.item(i, j);
			Posed curShapePoseW[2];
			curShapePoseW[0] = interpolationPose * curSolid[0]->GetShapePose(i);
			curShapePoseW[1] = curSolid[1]->GetPose() * curSolid[1]->GetShapePose(j);
			sp->CompIntermediateRepresentation(curShapePoseW, t, bInterpolatePose, bMultiPoints);
			for(int k = 0; k < (int)sp->irs.size(); k++){
				PHIr* ir = sp->irs[k];
				ir->solidID = solidID[0];
				ir->solidPair = this;
				ir->r = ir->pointerPointW - curSolid[1]->GetCenterPosition();
				ir->contactPointVel = curSolid[0]->GetPointVelocity(ir->contactPointW);
				ir->pointerPointVel = curSolid[1]->GetPointVelocity(ir->pointerPointW);	
			}
			if(pointer->bFriction) CompFrictionIntermediateRepresentation(sp);
			for(int k = 0; k < (int)sp->irs.size(); k++){
				irs.push_back(sp->irs[k]);
			}
		}
	}

	if(irs.size() == 0){
		// 接触なし
		frictionState = FREE;
		initialRelativePose = Posed();
		relativePose = Posed();
	}
	return irs;
}

bool PHSolidPairForHaptic::CompFrictionIntermediateRepresentation(PHShapePairForHaptic* sp){
	// 摩擦
	int Nirs = sp->irs.size();
	if(Nirs == 0) return false;
	for(int i = 0; i < Nirs; i++){
		PHIr* ir = sp->irs[i];
		double mu = ir->mu;	// 動摩擦係数
				
		double l = mu * ir->depth;		// 摩擦円錐半径

		Vec3d vps = ir->pointerPointW;
		Vec3d vq = relativePose * ir->pointerPointW;
		Vec3d dq = (vq - vps) * ir->normal * ir->normal;
		Vec3d vqs = vq - dq;
		Vec3d tangent = vqs - vps;

		//DSTR << "vps" << vps << std::endl;
		//DSTR << "vq" << vq << std::endl;
		//DSTR << "tangent " << tangent << tangent.norm() << std::endl;

		PHIr* fricIr = DBG_NEW PHIr();
		*fricIr = *ir;
		double epsilon = 1e-5;
		if(tangent.norm() < epsilon){
			// 静止状態
			delete fricIr;
			//DSTR << "rest" << std::endl;
		}
		if(epsilon < tangent.norm() && tangent.norm() <= l){
			//静摩擦（静止摩擦半径内）
			fricIr->normal = tangent.unit();
			fricIr->depth = tangent.norm();
			sp->irs.push_back(fricIr);
			//DSTR << "static friction" << std::endl;
		}

		if(epsilon < l && l < tangent.norm()){
			// 動摩擦
			fricIr->normal = tangent.unit();
			fricIr->depth = l;
			sp->irs.push_back(fricIr);
			//DSTR << "dynamic friction" << std::endl;
		}
	}
	return true;
}

bool PHSolidPairForHaptic::CompFrictionIntermediateRepresentation2(PHShapePairForHaptic* sp){
	// 摩擦

	int Nirs = sp->irs.size();
	if(Nirs == 0) return false;
	for(int i = 0; i < Nirs; i++){
		PHIr* ir = sp->irs[i];
		double mu = ir->mu;	// 動摩擦係数				
		double l = mu * ir->depth;		// 摩擦円錐半径

		Vec3d vps = ir->pointerPointW;
		Vec3d vq = relativePose * ir->pointerPointW;
		Vec3d dq = (vq - vps) * ir->normal * ir->normal;
		Vec3d vqs = vq - dq;
		Vec3d tangent = vqs - vps;

		//DSTR << "vps" << vps << std::endl;
		//DSTR << "vq" << vq << std::endl;
		//DSTR << "tangent " << tangent << tangent.norm() << std::endl;

		PHIr* fricIr = DBG_NEW PHIr();
		*fricIr = *ir;
		double epsilon = 1e-5;
		if(tangent.norm() < epsilon){
			// 静止状態
			delete fricIr;
			//DSTR << "rest" << std::endl;
		}
		if(epsilon < tangent.norm() && tangent.norm() <= l){
			//静摩擦（静止摩擦半径内）
			fricIr->normal = tangent.unit();
			fricIr->depth = tangent.norm();
			sp->irs.push_back(fricIr);
			//DSTR << "static friction" << std::endl;
		}

		if(epsilon < l && l < tangent.norm()){
			// 動摩擦
			fricIr->normal = tangent.unit();
			fricIr->depth = l;
			sp->irs.push_back(fricIr);
			//DSTR << "dynamic friction" << std::endl;
		}
	}
	return true;
}
//----------------------------------------------------------------------------
// PHHapticEngineImp
double PHHapticEngineImp::GetPhysicsTimeStep(){
	return engine->GetScene()->GetTimeStep();
}
double PHHapticEngineImp::GetHapticTimeStep(){
	return engine->GetScene()->GetHapticTimeStep();
}
int PHHapticEngineImp::NHapticPointers(){
	return (int)engine->hapticPointers.size();
}
int PHHapticEngineImp::NHapticSolids(){
	return (int)engine->hapticSolids.size();
}
PHHapticPointer* PHHapticEngineImp::GetHapticPointer(int i){
	return engine->hapticPointers[i];
}
PHSolidForHaptic* PHHapticEngineImp::GetHapticSolid(int i){
	return engine->hapticSolids[i];
}
PHSolidPairForHaptic* PHHapticEngineImp::GetSolidPairForHaptic(int i, int j){
	return engine->solidPairs.item(i, j);
}
PHHapticPointers* PHHapticEngineImp::GetHapticPointers(){
	return &engine->hapticPointers;
}
PHSolidsForHaptic* PHHapticEngineImp::GetHapticSolids(){
	return &engine->hapticSolids;
}
PHSolidPairsForHaptic* PHHapticEngineImp::GetSolidPairsForHaptic(){
	return (PHSolidPairsForHaptic*)&engine->solidPairs;
}
PHHapticRender* PHHapticEngineImp::GetHapticRender(){
	return engine->hapticRender->Cast();
}
void PHHapticEngineImp::StepSimulation(){
	engine->GetScene()->Step();
}

//----------------------------------------------------------------------------
// PHHapticEngine
PHHapticEngineDesc::PHHapticEngineDesc(){

}

PHHapticEngine::PHHapticEngine(){
	bHapticEngine = false;
	bPhysicStep = true;
	engineImp = DBG_NEW PHHapticEngineImpulse();
	engineImp->engine = this;
	engineMode = MULTI_THREAD;
	engineImps.push_back(engineImp);
	hapticRender = DBG_NEW PHHapticRender();
}

void PHHapticEngine::SetHapticEngineMode(HapticEngineMode mode){
	engineMode = mode;
	switch(engineMode){
		case SINGLE_THREAD:		
			for(int i = 0; i < (int)engineImps.size(); i++){
				if(DCAST(PHHapticEngineSingleBase, engineImps[i])){
					engineImp = engineImps[i];
					return;
				}
			}
			engineImp = DBG_NEW PHHapticEngineSingleBase();
			break;		
		case MULTI_THREAD:		
			for(int i = 0; i < (int)engineImps.size(); i++){
				if(DCAST(PHHapticEngineImpulse, engineImps[i])){
					engineImp = engineImps[i];
					return;
				}
			}
			engineImp = DBG_NEW PHHapticEngineImpulse();
			break;
		case LOCAL_DYNAMICS:
			for(int i = 0; i < (int)engineImps.size(); i++){
				if(DCAST(PHHapticEngineLD, engineImps[i])){
					engineImp = engineImps[i];
					return;
				}
			}
			engineImp = DBG_NEW PHHapticEngineLD();
			break;
		default:
			assert(0);
			return;
	}
	engineImp->engine = this;
	engineImps.push_back(engineImp);
}

PHHapticRenderIf* PHHapticEngine::GetHapticRender(){ 
	return hapticRender->Cast(); 
}

void PHHapticEngine::UpdateHapticPointer(){};

void PHHapticEngine::StartDetection(){
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
	for(int i = 0; i < (int)solids.size(); i++){
		Edge e;
		for(int j = 0; j < 3; j++){
			float min, max = 0.0f;
			solids[i]->GetBBoxSupport(dir[j], min, max);
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
	int N = hapticSolids.size();
	pointer->neighborSolidIDs.clear();
	for(int i = 0; i < N; i++){
		if(i == pointerSolidID) continue;
		Vec3f soMin = edges[i].min;
		Vec3f soMax = edges[i].max;
		int nAxes = 0;		// いくつの軸で交差しているかどうか
		for(int i = 0; i < 3; i++){
			int inAxis = 0;	// その軸で交差しているかどうか
			// pointerのBBox内にsolidのBBoxがあったら交差
			if(pMin[i] <= soMin[i] && soMin[i] <= pMax[i]) inAxis++; 
			if(pMin[i] <= soMax[i] && soMax[i] <= pMax[i]) inAxis++; 
			// ソリッドのBox内にクエリのBBoxがあったら交差
			if(soMin[i] <= pMin[i] && pMin[i] <= soMax[i]) inAxis++;
			if(soMin[i] <= pMax[i] && pMax[i] <= soMax[i]) inAxis++;
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
		PHSolidPairForHaptic* sp = solidPairs.item(i, pointerID);
		if(nAxes == 3){
			sp->Detect(this, ct, dt);	// 形状毎の近傍点探索、接触解析
			pointer->neighborSolidIDs.push_back(i);
			PHSolidForHaptic* h = hapticSolids[i];
			*h->GetLocalSolid() = *h->sceneSolid;	// 近傍と判定されたのでコピー
			if(sp->inLocal == 0){
				// 初めて近傍になった
				sp->inLocal = 1;	
				h->NLocalFirst += 1;
			}else{
				// 継続して近傍である
				sp->inLocal = 2;
				h->NLocal += 1;
			}
		}else{
			// 近傍でない
			sp->inLocal = 0;
		}
#endif
	}
}

bool PHHapticEngine::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s && std::find(solids.begin(), solids.end(), s) == solids.end()){
		// solidsにsolidを保存(pointerも含まれる)
		solids.push_back(s);				
		PHSolidForHaptic* h = DBG_NEW PHSolidForHaptic();
		h->sceneSolid = s;
		*h->GetLocalSolid() = *s;
		hapticSolids.push_back(h);

		int NSolids = (int)solids.size();
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
			solidPairs.resize(NSolids, NPointers);
			for(int i = 0; i < NSolids; i++){
				solidPairs.item(i, pointerID) = DBG_NEW PHSolidPairForHaptic();
				solidPairs.item(i, pointerID)->Init(solids[i], s);
				solidPairs.item(i, pointerID)->solidID[0] = i;
				solidPairs.item(i, pointerID)->solidID[1] = NSolids - 1;
			}
			if(s->NShape())	UpdateShapePairs(s);
		}

		// PHSolidの場合
		// 行のみ追加
		solidPairs.resize(NSolids, NPointers);
		for(int i = 0; i < NPointers; i++){
			solidPairs.item(NSolids - 1, i) = DBG_NEW PHSolidPairForHaptic();
			solidPairs.item(NSolids - 1, i)->Init(solids[NSolids - 1], hapticPointers[i]);	
			solidPairs.item(NSolids - 1, i)->solidID[0] = NSolids - 1;	
			solidPairs.item(NSolids - 1, i)->solidID[1] = hapticPointers[i]->GetSolidID();	
		}
		if(s->NShape())	UpdateShapePairs(s);
		return true;

		// 力覚ポインタをシーンの接触から切る
		for(int i = 0; i < NPointers; i++){
			GetScene()->SetContactMode(hapticPointers[i]->Cast(), PHSceneDesc::MODE_NONE);
		}
	}
	return false;
}

// 未完成
bool PHHapticEngine::DelChildObject(ObjectIf* o){
	//PHSolid* s = DCAST(PHSolid, o);
	//if(s){
	//	PHSolids::iterator is = find(solids.begin(), solids.end(), s);
	//	if(is != solids.end() && solids.size() > 0){
	//		int idx = (int)(is - solids.begin());
	//		solids.erase(is);
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

void PHHapticEngine::UpdateShapePairs(PHSolid* solid){
	PHSolids::iterator it = std::find(solids.begin(), solids.end(), solid);
	if(it == solids.end())
		return;
	int isolid = (int)(it - solids.begin());
	int i, j;
	PHSolidPairForHaptic* sp;
	PHSolid* s[2];
	// solidの場合(行の更新）
	for(i = 0; i < NHapticPointers(); i++){
		sp = solidPairs.item(isolid, i);
		s[0] = solid;
		s[1] = sp->solid[1];
		sp->shapePairs.resize(s[0]->NShape(), s[1]->NShape());
		for(j = 0; j < s[1]->NShape(); j++){
			PHShapePairForHaptic* n = DBG_NEW PHShapePairForHaptic();
			n->shape[0] = s[0]->GetShape(s[0]->NShape() - 1)->Cast();
			n->shape[1] = s[1]->GetShape(j)->Cast();
			sp->shapePairs.item(s[0]->NShape() - 1, j) = n;
		}
	}

	PHHapticPointer* pointer = DCAST(PHHapticPointer, solid);
	if(!pointer) return;
	// PHHapticPointerの場合（列の更新）
	int pointerID = pointer->GetPointerID();
	int pointerSolidID = pointer->GetSolidID();
	for(i = 0; i < (int)solids.size(); i++){
		if(i == pointerSolidID) continue;
		sp = solidPairs.item(i, pointerID);
		s[0] = sp->solid[0];
		s[1] = solid;
		sp->shapePairs.resize(s[0]->NShape(), s[1]->NShape());
		for(j = 0; j < s[0]->NShape(); j++){
			PHShapePairForHaptic* n = DBG_NEW PHShapePairForHaptic();
			n->shape[0] = s[0]->GetShape(j)->Cast();
			n->shape[1] = s[1]->GetShape(s[1]->NShape()-1)->Cast(); 
			sp->shapePairs.item(j, s[1]->NShape()-1) = n;
		}
	}
}

} // namespace Spr
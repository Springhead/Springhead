#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticRenderMulti.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHSapePairForHaptic
bool PHShapePairForHaptic::Detect(unsigned ct, const Posed& pose0, const Posed& pose1){
	for(int i = 0; i < 2; i++){
		lastShapePoseW[i] = shapePoseW[i];
		lastClosestPoint[i] = closestPoint[i];	
	}
	lastNormal = normal;
	return DetectClosestPoints(ct, pose0, pose1);
}

bool PHShapePairForHaptic::DetectClosestPoints(unsigned ct, const Posed& pose0, const Posed& pose1){
	shapePoseW[0] = pose0;
	shapePoseW[1] = pose1;
	
	Vec3d sep;
	bool rv = FindClosestPoints(shape[0], shape[1], shapePoseW[0], shapePoseW[1],
									sep, closestPoint[0], closestPoint[1]);
	double dist = (GetClosestPointOnWorld(1) - GetClosestPointOnWorld(0)).norm();
	if(rv){
		if(dist > 1e-5)	state = NONE;	// 接触していない
		else{							// 接触
			commonPoint = shapePoseW[0] * closestPoint[0];
			if (lastContactCount == unsigned(ct-1))	state = CONTINUE;
			else state = NEW;
			lastContactCount = ct;
		}
	}else state = NONE;
	return rv;
}

int PHShapePairForHaptic::OnDetectClosestPoints(unsigned ct, const Vec3d& center0){
		Vec3d dir = normal;
		if(dir = Vec3d()) dir = -1 * (closestPoint[0] - center0); 
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


//----------------------------------------------------------------------------
// PHSolidPairForHaptic

bool PHSolidPairForHaptic::Detect(engine_type* engine, unsigned int ct, double dt){
	
	return PHSolidPair::Detect(engine, ct, dt);
}

void PHSolidPairForHaptic::OnDetect(PHShapePairForHaptic* sp, PHHapticEngineImp* engine, unsigned ct, double dt){
	if(sp->state == CDShapePair::State::NEW || sp->state == CDShapePair::State::CONTINUE){
		sp->OnDetectClosestPoints(ct, solid[0]->GetCenterPosition());
	}
	if(inLocal == 1){
		for(int i = 0; i < 2; i++){
			sp->lastShapePoseW[i] = sp->shapePoseW[i];
			sp->lastClosestPoint[i] = sp->closestPoint[i];
		}
		sp->lastNormal = sp->normal;
	}	
}


//----------------------------------------------------------------------------
// PHHapticEngineImp

void PHHapticEngineImp::Step(){
	UpdateHapticPointer();
	StartDetection();
}

void PHHapticEngineImp::UpdateHapticPointer(){};

void PHHapticEngineImp::StartDetection(){
	UpdateEdgeList();
	for(int i = 0; i < (int)hapticPointers.size(); i++){
		Detect(hapticPointers[i]);
	}
	for(int i = 0; i < (int)hapticSolids.size(); i++){
		PHSolidForHaptic* h = hapticSolids[i];
		if(DCAST(PHHapticPointer, h->solid)) continue;
		if(h->NLocal == 0 && h->NLocalFirst > 0)	h->doSim = 1;
		else if(h->NLocal > 0)	h->doSim = 2;
		else	h->doSim = 0;
		h->NLocal = 0; h->NLocalFirst = 0;
	}
}

void PHHapticEngineImp::UpdateEdgeList(){
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

void PHHapticEngineImp::Detect(PHHapticPointer* q){
	int ct = GetScene()->GetCount();
	int dt = GetScene()->GetTimeStep();
	const int pointerID = q->GetID();
	Vec3f range3d = Vec3f(1.0, 1.0, 1.0) * q->GetLocalRange(); // 検出閾値
	// pointerのBBoxをrange3d分だけ拡げる
	edges[pointerID].min -= range3d;
	edges[pointerID].max += range3d;
	
	Vec3f pMin = edges[pointerID].min;
	Vec3f pMax = edges[pointerID].max;
	int N = hapticSolids.size();
	q->neighborSolidIDs.clear();
	for(int i = 0; i < N; i++){
		if(i == pointerID) continue;
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
		// 2.近傍物体と判定
		int a, b;
		a = pointerID;
		b = i;
		if(a > b) std::swap(a, b);
		PHSolidPairForHaptic* sp = solidPairs.item(a, b);
		PHSolidForHaptic* h = hapticSolids[i];
		if(nAxes == 3){
			sp->Detect(this, ct, dt);
			q->neighborSolidIDs.push_back(i);
			if(sp->inLocal == 0){
				sp->inLocal = 1;
				h->NLocalFirst += 1;
			}else{
				sp->inLocal = 2;
				h->NLocal += 1;
			}
		}else{
			sp->inLocal = 0;
		}
	}
}

bool PHHapticEngineImp::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s && std::find(solids.begin(), solids.end(), s) == solids.end()){
		solids.push_back(s);				// solidsにsolidを保存(pointerも含まれる)
		hapticSolids.resize(hapticSolids.size() + 1);
		PHSolidForHaptic* h = DBG_NEW PHSolidForHaptic();
		hapticSolids.back() = h;
		h->solid = s;
		int N = (int)solids.size();	
		assert(solidPairs.height() == N-1 && solidPairs.width() == N-1);
		solidPairs.resize(N, N);
		for(int i = 0; i < N-1; i++){
			solidPairs.item(i, N-1) = DBG_NEW PHSolidPairForHaptic();
			solidPairs.item(i, N-1)->Init(solids[i], solids[N-1]);	
		}
		if(s->NShape())	UpdateShapePairs(s);

		// PHSolidがPHHapticPointerの場合
		PHHapticPointer* p = DCAST(PHHapticPointer, o);
		if(p){
			p->SetID(N-1);
			hapticPointers.push_back(p);
			h->bPointer = true;
		}
		return true;
	}
	return false;
}

bool PHHapticEngineImp::DelChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s){
		PHSolids::iterator is = find(solids.begin(), solids.end(), s);
		if(is != solids.end() && solids.size() > 0){
			int idx = (int)(is - solids.begin());
			solids.erase(is);
			solidPairs.erase_col(idx);
			PHSolidsForHaptic::iterator it;
			for(it = hapticSolids.begin(); it < hapticSolids.end(); it++){
				if((*it)->solid = s) hapticSolids.erase(it);
			}
			PHHapticPointer* p = DCAST(PHHapticPointer, o);
			if(p){
				PHHapticPointers::iterator ip = find(hapticPointers.begin(), hapticPointers.end(), p);
				if(ip != hapticPointers.end() && hapticPointers.size() > 0){
					hapticPointers.erase(ip);
				}
			}
		}
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
// PHHapticEngine

void PHHapticEngine::SetRenderMode(RenderMode r){
	renderMode = r;
	switch(renderMode){
		case NONE:
			 renderImp = DBG_NEW PHHapticRenderMulti();
			break;
		case IMPULSE:
			//render = DBG_NEW 
			break;
		default:
			break;
	}
}
} // namespace Spr
/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHContactDetector.h>
#include <Foundation/UTQPTimer.h>

#include <unordered_set>

using namespace PTM;
using namespace std;

//#define REPORT_TIME 1

namespace Spr{;

Spr::UTQPTimer ptimerForCd;
Spr::UTQPTimer ptimerForGjk;
UTLongLong& broadTime = UTPerformanceMeasureIf::GetInstance("Collision")->Count("broad");
UTLongLong& narrowTime = UTPerformanceMeasureIf::GetInstance("Collision")->Count("narrow");
extern UTLongLong& coltimePhase1;

///////////////////////////////////////////////////////////////////////////////////////////////////
// PHShapePair

void PHShapePair::Init(PHSolidPair* sp, PHFrame* fr0, PHFrame* fr1){
	solidPair = sp;
	frame[0] = fr0;
	frame[1] = fr1;
	shape[0] = fr0->shape->Cast();
	shape[1] = fr1->shape->Cast();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// PHSolidPair

void PHSolidPair::Init(PHContactDetector* d, PHBody* s0, PHBody* s1){
	detector = d;
	body[0] = s0;
	body[1] = s1;
	int ns0 = s0->NShape();
	int ns1 = s1->NShape();
	shapePairs.resize(ns0, ns1);
	for(int i = 0; i < ns0; i++)for(int j = 0; j < ns1; j++){
		PHShapePair* sp = shapePairs.item(i, j);
		if (!sp){
			sp = CreateShapePair();
			shapePairs.item(i, j) = sp;
		}
		sp->Init(this, body[0]->frames[i], body[1]->frames[j]);
	}
	bEnabled = true;
}

bool PHSolidPair::Detect(unsigned int ct, double dt){
	if(!bEnabled)return false;

	// いずれかのSolidに形状が割り当てられていない場合は接触なし
	if(body[0]->NShape() == 0 || body[1]->NShape() == 0) return false;

	// 全てのshape pairについて交差を調べる
	bool found = false;
	PHShapePair* sp;
	for(int i = 0; i < body[0]->NShape(); i++)for(int j = 0; j < body[1]->NShape(); j++){
		sp = shapePairs.item(i, j);
		//このshape pairの交差判定/法線と接触の位置を求める．
		ptimerForGjk.Count();
		if(sp->Detect(
			ct,
			body[0]->GetPose() * body[0]->GetShapePose(i), body[1]->GetPose() * body[1]->GetShapePose(j))){
			found = true;
			ptimerForGjk.Accumulate(coltimePhase1);
			OnDetect(sp, ct, dt);
		}
		else {
			ptimerForGjk.Accumulate(coltimePhase1);
		}
	}
	return found;
}
bool PHSolidPair::ContDetect(unsigned int ct, double dt){
	if(!bEnabled)return false;

	// いずれかのSolidに形状が割り当てられていない場合は接触なし
	if(body[0]->NShape() == 0 || body[1]->NShape() == 0) return false;
	// 両方ともフリーズ状態の場合は接触なし
	if(body[0]->IsFrozen() && body[1]->IsFrozen())
		return false;

	static std::vector<Posed> shapePose[2];
	static std::vector<Vec3d> shapeCenter[2];
	for(int i=0; i < 2; i++){
		shapePose[i].resize(body[i]->NShape());
		shapeCenter[i].resize(body[i]->NShape());
		for(int j=0; j < body[i]->NShape(); j++){
			CDConvex* convex = DCAST(CDConvex, body[i]->GetShape(j));
			if(convex){
				Posed lp = body[i]->GetShapePose(j);
				shapeCenter[i][j] = lp * convex->GetCenterOfMass();
				shapePose[i][j] = body[i]->GetPose() * lp;
			}else{
				body[i]->DelChildObject(body[i]->GetFrame(j));
				shapePose[i].resize(body[i]->NShape());
				shapeCenter[i].resize(body[i]->NShape());
				j--;
			}
		}
	}
	Vec3d vt[2] = {body[0]->GetVelocity() * dt, body[1]->GetVelocity() * dt};
	Vec3d wt[2] = {body[0]->GetAngularVelocity() * dt, body[1]->GetAngularVelocity() * dt};
	Vec3d delta[2] = {vt[0] - (wt[0]^body[0]->GetCenterOfMass()), vt[1] - (wt[1] ^  body[1]->GetCenterOfMass())};

	// 全てのshape pairについて交差を調べる
	bool found = false;
	PHShapePair* sp;
	for(int i = 0; i < body[0]->NShape(); i++)for(int j = 0; j < body[1]->NShape(); j++){
		sp = shapePairs.item(i, j);
		//このshape pairの交差判定/法線と接触の位置を求める．
		if (sp->ContDetect(ct, shapePose[0][i], shapePose[1][j],
			//	剛体ではなく、形状の移動量なので、形状の中心位置で移動量を補正する。
			delta[0] + (wt[0]^shapeCenter[0][i]),  delta[1] + (wt[1]^shapeCenter[1][j]), dt)){
			ptimerForCd.Accumulate(narrowTime);
			assert(0.9 < sp->normal.norm() && sp->normal.norm() < 1.1);
			found = true;
			OnContDetect(sp, ct, dt);
			ptimerForCd.Count();
		}
	}
	// フリーズの解除
	if(found){
		if(body[0]->IsDynamical() && !body[1]->IsFrozen()){
			body[0]->SetFrozen(false);
		}
		else if(body[1]->IsDynamical() && !body[0]->IsFrozen()){
			body[1]->SetFrozen(false);
		}
	}
	return found;
}

bool PHSolidPair::Detect(PHShapePair* shapePair, unsigned ct, double dt, bool continuous){
	PHFrame* fr0 = shapePair->frame[0];
	PHFrame* fr1 = shapePair->frame[1];
#if 0	// check PHFrame
	for(int i=0; i<2; ++i){
			int j;
			for(j=0; j< body[i]->frames.size(); ++j){
				if (body[i]->GetFrame(j) ==  shapePair->frame[i]->Cast()) break;
			}
			if (j >= body[i]->frames.size()){
				DSTR << body[i]->GetName() << " has wrong frame " <<  shapePair->frame[i]->GetName() << std::endl;
			}
	}
#endif

	bool found = false;
	if(continuous){	
		const double alpha = 2.0;
		if (shapePair->ContDetect(ct, body[0]->GetPose() * fr0->GetPose(), body[1]->GetPose() * fr1->GetPose(), alpha * fr0->delta, alpha * fr1->delta, dt)){
			double n = shapePair->normal.norm();
			assert(0.9 < n && n < 1.1);
			found = true;
			OnContDetect(shapePair, ct, dt);
		}		
	}
	else{
		if (shapePair->Detect(ct, body[0]->GetPose() * fr0->GetPose(), body[1]->GetPose() * fr1->GetPose())){
			found = true;
			OnDetect(shapePair, ct, dt);
		}
	}

	// フリーズの解除
	if(found){
		if(fr0->body->IsDynamical() && !fr1->body->IsFrozen()){
			fr0->body->SetFrozen(false);
		}
		else if(fr1->body->IsDynamical() && !fr0->body->IsFrozen()){
			fr1->body->SetFrozen(false);
		}
	}
	return found;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// PHContactDetector

PHContactDetector::PHContactDetector(){
	SetDetectionRange(Vec3f(), Vec3f(), 1, 1, 1);
}

void PHContactDetector::Clear(){
	bodies.clear();
	inactiveBodies.clear();
	solidPairs.clear();
}

void PHContactDetector::AddInactiveSolid(PHBodyIf* body){
	inactiveBodies.push_back(body->Cast());
}

bool PHContactDetector::IsInactiveSolid(PHBodyIf* body){
	for(PHBodies::iterator it = inactiveBodies.begin(); it != inactiveBodies.end(); it++){
		if((*it) == body->Cast())
			return true;
	}
	return false;
}

bool PHContactDetector::AddChildObject(ObjectIf* o){
	PHBody* s = DCAST(PHBody, o);
	if(s && std::find(bodies.begin(), bodies.end(), s) == bodies.end()){
		bodies.push_back(s);
		int N = (int)bodies.size();
		assert(solidPairs.height() == N-1 && solidPairs.width() == N-1);
		solidPairs.resize(N, N);
		for(int i = 0; i < N-1; i++){
			solidPairs.item(i, N-1) = CreateSolidPair();
			solidPairs.item(i, N-1)->Init(this, bodies[i], bodies[N-1]);
		}
		if(s->NShape())
			UpdateShapePairs(s);
		return true;
	}
	return false;
}

bool PHContactDetector::DelChildObject(ObjectIf* o){
	PHBody* s = DCAST(PHBody, o);
	if(!s)return false;
	
	PHBodies::iterator is;
	is = find(bodies.begin(), bodies.end(), s);
	if(is != bodies.end()){
		int idx = (int)(is - bodies.begin());
		bodies.erase(is);
		solidPairs.erase_row(idx);
		solidPairs.erase_col(idx);
		return true;
	}
	is = find(inactiveBodies.begin(), inactiveBodies.end(), s);
	if(is != inactiveBodies.end())
		inactiveBodies.erase(is);

	return false;
}

int PHContactDetector::NSolidPairs() const{
	int n = (int)bodies.size();
	return n * (n - 1) / 2;
}
int PHContactDetector::NShapePairs() const{
	int n = (int)bodies.size(), N = 0;
	for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
		N += (int)(solidPairs.item(i,j)->shapePairs.size());
	return N;
}
int PHContactDetector::NActiveSolidPairs() const{
	int n = (int)bodies.size(), N = 0;
	for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
		N += (int)solidPairs.item(i,j)->bEnabled;
	return N;
}
int PHContactDetector::NActiveShapePairs() const{
	int n = (int)bodies.size(), N = 0;
	for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
		if(solidPairs.item(i,j)->bEnabled)
			N += (int)solidPairs.item(i,j)->shapePairs.size();
	return N;
}
	
size_t PHContactDetector::GetStateSize() const {
	PHContactDetectorSt s(NSolidPairs(), NShapePairs());
	return s.GetSize();
}
void PHContactDetector::ConstructState(void* m) const {
	new (m) PHContactDetectorSt(NSolidPairs(), NShapePairs());
}
void PHContactDetector::DestructState(void* m) const {
	((PHContactDetectorSt*)m)->~PHContactDetectorSt();
}
bool PHContactDetector::GetState(void* s) const {
	PHContactDetectorSt* es = ((PHContactDetectorSt*)s);
	es->nSolidPair = (size_t)NSolidPairs();
	es->nShapePair = (size_t)NShapePairs();
	PHSolidPairSt* solidStates = es->SolidStates();
	CDShapePairState* shapeStates = es->ShapeStates();
	//	solidPairs.item(i,j)　の i<j部分を使っているのでそこだけ保存
	int solidPos = 0;
	int shapePos = 0;
	PHSolidPair* solidPair;
	PHShapePair* shapePair;

	for(int j = 1; j < solidPairs.width(); ++j){
		for(int i = 0; i < j; ++i){
			solidPair = solidPairs.item(i, j);
			solidPair->GetSt(solidStates[solidPos]);				

			for(int r = 0; r < solidPair->shapePairs.height(); ++r){
				for(int c = 0; c < solidPair->shapePairs.width(); ++c){
					shapePair = solidPair->shapePairs.item(r, c);
					shapePair->GetSt(shapeStates[shapePos]);
					++shapePos;
				}
			}
			++solidPos;
		}
	}
	
	return true;
}
void PHContactDetector::SetState(const void* s){
	PHContactDetectorSt* es = (PHContactDetectorSt*)s;
	assert(es->nSolidPair == (size_t)NSolidPairs());
	assert(es->nShapePair == (size_t)NShapePairs());
	PHSolidPairSt* solidStates = es->SolidStates();
	CDShapePairState* shapeStates = es->ShapeStates();
	//	solidPairs.item(i,j)　の i<j部分を使っているのでそこだけ保存
	int solidPos = 0;
	int shapePos = 0;
	PHSolidPair* solidPair;
	PHShapePair* shapePair;

	for(int j = 1; j < solidPairs.width(); ++j){
		for(int i = 0; i < j; ++i){
			solidPair = solidPairs.item(i, j);
			solidPair->SetSt(solidStates[solidPos]);
	
			for(int r = 0; r < solidPair->shapePairs.height(); ++r){
				for(int c = 0; c < solidPair->shapePairs.width(); ++c){
					shapePair = solidPair->shapePairs.item(r, c);
					shapePair->SetSt(shapeStates[shapePos]);
					++shapePos;
				}
			}
			++solidPos;
		}
	}

}

void PHContactDetector::UpdateShapePairs(PHBody* body){
	PHBodies::iterator it = std::find(bodies.begin(), bodies.end(), body);
	if(it == bodies.end())
		return;
	int isolid = (int)(it - bodies.begin());
	int i, j;
	PHSolidPair* solidPair;
	PHBody *slhs, *srhs;
	for(i = 0; i < isolid; i++){
		solidPair = solidPairs.item(i, isolid);
		slhs = solidPair->body[0];
		srhs = body;
		solidPair->shapePairs.resize(slhs->NShape(), srhs->NShape());
		for(j = 0; j < slhs->NShape(); j++){
			PHShapePair* shapePair = solidPair->CreateShapePair();
			shapePair->Init(solidPair, slhs->frames[j], srhs->frames[srhs->NShape()-1]);
			solidPair->shapePairs.item(j, srhs->NShape()-1) = shapePair;
		}
	}
	for(i = isolid+1; i < (int)bodies.size(); i++){
		solidPair = solidPairs.item(isolid, i);
		slhs = body;
		srhs = solidPair->body[1];
		solidPair->shapePairs.resize(body->NShape(), srhs->NShape());
		for(j = 0; j < srhs->NShape(); j++){
			PHShapePair* shapePair = solidPair->CreateShapePair();
			shapePair->Init(solidPair, slhs->frames[slhs->NShape()-1], srhs->frames[j]);
			solidPair->shapePairs.item(slhs->NShape()-1, j) = shapePair;
		}
	}
}

void PHContactDetector::DelShapePairs(PHBody* body, int iBegin, int iEnd){
	PHBodies::iterator it = std::find(bodies.begin(), bodies.end(), body);
	if(it == bodies.end())
		return;
	int isolid = (int)(it - bodies.begin());
	int i, j;
	PHBody *slhs, *srhs;
	PHSolidPair* solidPair;
	for(i = 0; i < isolid; i++){
		solidPair = solidPairs.item(i, isolid);
		slhs = solidPair->body[0];
		srhs = body;
		//	消えたShapeに対応する行を詰める
		for(j = 0; j < slhs->NShape(); j++){
			for(int k=iEnd; k < srhs->NShape() + (iEnd-iBegin); ++k)
				solidPair->shapePairs.item(j, k - (iEnd-iBegin)) = solidPair->shapePairs.item(j, k);
		}
		//	サイズの更新
		solidPair->shapePairs.resize(slhs->NShape(), srhs->NShape());
	}
	for(i = isolid+1; i < (int)bodies.size(); i++){
		solidPair = solidPairs.item(isolid, i);
		slhs = body;
		srhs = solidPair->body[1];
		//	消えたShapeに対応する行を詰める
		for(j = 0; j < srhs->NShape(); j++) {
			for(int k=iEnd; k < slhs->NShape() + (iEnd-iBegin); ++k){
				solidPair->shapePairs.item(k - (iEnd-iBegin), j) = solidPair->shapePairs.item(k, j);
			}
		}
		//	サイズの更新
		solidPair->shapePairs.resize(slhs->NShape(), srhs->NShape());
#if 0		//確認
		for(j = 0; j < slhs->NShape(); j++) {
			for(int k=0; k<srhs->NShape(); ++k) {
				if (solidPair->shapePairs.item(j,k)->frame[0] != solidPair->body[0]->frames[j]){
					DSTR << solidPair->body[0]->GetName() << "-" << solidPair->body[1]->GetName() << std::endl;
					DSTR << "sp" << j << " - " << k << " " << solidPair->shapePairs.item(j,k)->frame[0]->GetName() << "-" << solidPair->body[0]->frames[j]->GetName();
					DSTR << std::endl;
					DSTR << "collision";
				}
				if (solidPair->shapePairs.item(j,k)->frame[1] != solidPair->body[1]->frames[k]){
					DSTR << solidPair->body[0]->GetName() << "-" << solidPair->body[1]->GetName() << std::endl;
				}
			}
		}
#endif
	}
}

void PHContactDetector::EnableContact(PHSolidIf* lhs, PHSolidIf* rhs, bool bEnable){
	PHBodies::iterator ilhs, irhs;
	ilhs = find(bodies.begin(), bodies.end(), (PHBody*)(lhs->Cast()));
	irhs = find(bodies.begin(), bodies.end(), (PHBody*)(rhs->Cast()));
	if(ilhs == bodies.end() || irhs == bodies.end())
		return;
	int i = (int)(ilhs - bodies.begin()), j = (int)(irhs - bodies.begin());
	if(i > j)std::swap(i, j);
	assert(i < solidPairs.height() && j < solidPairs.width());
	solidPairs.item(i, j)->bEnabled = bEnable;
}

void PHContactDetector::EnableContact(PHSolidIf** group, size_t length, bool bEnable){
	std::vector<int> idx;
	PHBodies::iterator it;
	for(int i = 0; i < (int)length; i++){
		it = find(bodies.begin(), bodies.end(), (PHBody*)(group[i]->Cast()));
		if(it != bodies.end())
			idx.push_back((int)(it - bodies.begin()));
	}
	sort(idx.begin(), idx.end());
	for(int i = 0; i < (int)idx.size(); i++){
		for(int j = i+1; j < (int)idx.size(); j++){
			solidPairs.item(idx[i], idx[j])->bEnabled = bEnable;
		}
	}
}

void PHContactDetector::EnableContact(PHSolidIf* body, bool bEnable){
	PHBodies::iterator it = find(bodies.begin(), bodies.end(), (PHBody*)(body->Cast()));
	if(it == bodies.end())
		return;
	int idx = (int)(it - bodies.begin());
	for(int i = 0; i < idx; i++)
		solidPairs.item(i, idx)->bEnabled = bEnable;
	for(int i = idx+1; i < (int)bodies.size(); i++)
		solidPairs.item(idx, i)->bEnabled = bEnable;
}

void PHContactDetector::EnableContact(bool bEnable){
	int n = (int)bodies.size();
	for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
		solidPairs.item(i, j)->bEnabled = bEnable;
}

void PHContactDetector::SetDetectionRange(Vec3f center, Vec3f extent, int nx, int ny, int nz){
	numDivision = Vec3i(nx, ny, nz);
	regionBBox.SetBBoxCenterExtent(center, extent);
	
	Vec3d cellExtent;
	for(int k = 0; k < 3; k++)
		cellExtent[k] = extent[k] / (double)numDivision[k];
	cellBBox.SetBBoxCenterExtent(Vec3d(), cellExtent);

	cells.resize(nx * ny * nz);
	Vec3d w = 2.0 * cellExtent;
	double x, y, z;
	Vec3d cellmin, cellmax;
	x = center.x - extent.x;
	for(int ix = 0; ix < nx; ix++, x += w.x){
		y = center.y - extent.y;
		for(int iy = 0; iy < ny; iy++, y += w.y){
			z = center.z - extent.z;
			for(int iz = 0; iz < nz; iz++, z += w.z){
				cellmin = Vec3d(x      , y      , z      );
				cellmax = Vec3d(x + w.x, y + w.y, z + w.z);
				GetCell(ix, iy, iz).bbox.SetBBoxMinMax(cellmin, cellmax);
			}
		}
	}
}

bool PHContactDetector::DetectPair(
	PHContactDetector::ShapeIndex sh0,
	PHContactDetector::ShapeIndex sh1, unsigned ct, double dt, bool continuous){

	// 同じ剛体同士はスキップ
	if(sh0.idxBody == sh1.idxBody)
		return false;

	// 両方の形状とも中心がセルの外ならスキップ
	if(!sh0.center && !sh1.center)
		return false;

	if(sh0.idxBody > sh1.idxBody)
		swap(sh0, sh1);

	PHSolidPair* solidPair = solidPairs.item(sh0.idxBody, sh1.idxBody);
	
	// 無効な剛体対はスキップ
	if(!solidPair->bEnabled)
		return false;
	
	// 両方frozenならスキップ
	if(solidPair->body[0]->IsFrozen() && solidPair->body[1]->IsFrozen())
		return false;
	
	// 両方non-dynamicalならスキップ
	if(!solidPair->body[0]->IsDynamical() && !solidPair->body[1]->IsDynamical())
		return false;

	nBroad++;
	
	PHShapePair* shapePair = solidPair->shapePairs.item(sh0.idxShape, sh1.idxShape);
	PHFrame* fr0 = shapePair->frame[0];
	PHFrame* fr1 = shapePair->frame[1];

	// ShapeのBBox同士の判定で除外できなければnarrow phaseへ
	if(PHBBox::Intersect(fr0->bbWorld[continuous], fr1->bbWorld[continuous])){
		solidPair->Detect(shapePair, ct, dt, continuous);
		nNarrow++;
		return true;
	}

	return false;
};

bool PHContactDetector::Detect(unsigned ct, double dt, int mode, bool continuous){
	if(NActiveSolidPairs() == 0) return false;
	bool found = false;
	
	ptimerForCd.Count();
	//	Sort and prune mode --------------------------------------------------------------------
	if( mode >= PHSceneDesc::MODE_SORT_AND_SWEEP_X &&
	    mode <= PHSceneDesc::MODE_SORT_AND_SWEEP_Z ){
		int idx = mode - PHSceneDesc::MODE_SORT_AND_SWEEP_X;
		Vec3d dir(0,0,0);
		dir[idx] = 1;
		edges.resize(2 * bodies.size());
		std::vector<Edge>::iterator eit = edges.begin();
		for(int i = 0; i < (int)bodies.size(); i++){
			bodies[i]->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
			if (continuous){
				PHScene* s = DCAST(PHScene, bodies[i]->GetNameManager());
				double dt = s->GetTimeStep();
				Quaterniond rot = Quaterniond::Rot(bodies[i]->GetAngularVelocity() * dt);
				Vec3d prevDir = rot * dir;
				float pe0, pe1;
				bodies[i]->GetBBoxSupport(prevDir, pe0, pe1);
				Vec3d delta = dt * bodies[i]->GetVelocity();
				float ofs = delta * prevDir;
				if (ofs < 0) pe0 += ofs;
				else pe1 += ofs;
				eit[0].edge = std::min(eit[0].edge, pe0);
				eit[1].edge = std::max(eit[1].edge, pe1);
			}
			eit[0].index = i; eit[0].bMin = true;
			eit[1].index = i; eit[1].bMin = false;
			eit += 2;
		}				
		std::sort(edges.begin(), edges.end());

		//端から見ていって，接触の可能性があるノードの判定をする．
		typedef std::set<int> SolidSet;
		SolidSet cur;							//	現在のSolidのセット
#ifdef _DEBUG
		nMaxOverlapObject = 0;
#endif
		for(std::vector<Edge>::iterator it = edges.begin(); it != edges.end(); ++it){
			if (it->bMin){						//	初端だったら，リスト内の物体と判定
				for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
					int f1 = it->index;
					int f2 = *itf;
					if (f1 > f2) std::swap(f1, f2);
					//2. SolidとSolidの衝突判定
					ptimerForCd.Accumulate(broadTime);
					if (continuous){
						found |= solidPairs.item(f1, f2)->ContDetect(ct, dt);
					}else{
						found |= solidPairs.item(f1, f2)->Detect(ct, dt);
					}
					ptimerForCd.Accumulate(narrowTime);
				}
				cur.insert(it->index);
#ifdef _DEBUG
				if (nMaxOverlapObject < (int)cur.size()) nMaxOverlapObject = (int)cur.size();
#endif
			}else{
				cur.erase(it->index);			//	終端なので削除．
			}
		}
		ptimerForCd.Accumulate(broadTime);
	//	Cell mode --------------------------------------------------------------------
	}else{
		// 各形状のAABBを計算
		int nsolids = (int)bodies.size();
		for(int i = 0; i < nsolids; ++i){
			PHBody* body = bodies[i];
			body->CalcAABB();
		}
		// 各形状を領域に分類
		cellOutside.shapes.clear();
		for(int c = 0; c < (int)cells.size(); c++)
			cells[c].shapes.clear();

		Vec3d regionMin  = regionBBox.GetBBoxMin();
		Vec3d regionMax  = regionBBox.GetBBoxMax();
		Vec3d cellExtent = cellBBox  .GetBBoxExtent();
		Vec3d cellmin, cellmax;
		Vec3d bbmin, bbmax, bbcenter, bbextent;
		for(int i = 0; i < nsolids; i++){
			int nshapes = bodies[i]->NShape();
			for(int j = 0; j < nshapes; j++){
				PHBBox& bb = bodies[i]->frames[j]->bbWorld[continuous];
				bbmin    = bb.GetBBoxMin   ();
				bbmax    = bb.GetBBoxMax   ();
				bbcenter = bb.GetBBoxCenter();
				bbextent = bb.GetBBoxExtent();
				bool large = false;
				for(int k = 0; k < 3; k++)
					large |= (bbextent[k] > cellExtent[k]);
				// セルよりも大きな形状は外側扱い
				if(large){
					cellOutside.Add(i,j,true);
					continue;
				}
				
				Vec3i idx;
				bool outside = false;
				for(int k = 0; k < 3; k++){
					idx[k] = (int)floor((bbcenter[k] - regionMin[k]) / (2.0 * cellExtent[k]));
					outside |= (idx[k] < 0 || idx[k] >= numDivision[k]);
				}
				// 領域よりも外側の形状は外側扱い
				if(outside){
					cellOutside.Add(i,j,true);
					continue;
				}

				// それ以外は中心が含まれるセルおよび（かかっていれば）その隣接セルへ登録
				Cell& cell = GetCell(idx.x, idx.y, idx.z);
				cellmin = cell.bbox.GetBBoxMin();
				cellmax = cell.bbox.GetBBoxMax();
				bool flag[3][3];
				for(int k = 0; k < 3; k++){
					flag[k][0] = (bbmin[k] < cellmin[k] && idx[k] > 0);
					flag[k][1] = true;
					flag[k][2] = (bbmax[k] > cellmax[k] && idx[k] < numDivision[k]-1);
				}
				for(int jx = -1; jx <= 1; jx++)for(int jy = -1; jy <= 1; jy++)for(int jz = -1; jz <= 1; jz++){
					if(flag[0][jx+1] && flag[1][jy+1] && flag[2][jz+1])
						GetCell(idx.x + jx, idx.y + jy, idx.z + jz).Add(i,j,(jx == 0 && jy == 0 && jz == 0));
				}
			}
		}
		nBroad  = 0;
		nNarrow = 0;
		// 各セルについて交差判定
		int ix, iy, iz;
		for(ix = 0; ix < numDivision.x; ix++)for(iy = 0; iy < numDivision.y; iy++)for(iz = 0; iz < numDivision.z; iz++){
			Cell& cell = GetCell(ix, iy, iz);
			int n = (int)cell.shapes.size();
			for(int s0 = 1; s0 < n; s0++)for(int s1 = 0; s1 < s0; s1++)
				found |= DetectPair(cell.shapes[s0], cell.shapes[s1], ct, dt, continuous);
		}
		// 外側とセル間
		int noutside = (int)cellOutside.shapes.size();
		for(int c = 0; c < (int)cells.size(); c++){
			int n = (int)cells[c].shapes.size();
			for(int s0 = 0; s0 < noutside; s0++)for(int s1 = 0; s1 < n; s1++)
				found |= DetectPair(cellOutside.shapes[s0], cells[c].shapes[s1], ct, dt, continuous);
		}
		// 外側同士
		for(int s0 = 1; s0 < noutside; s0++)for(int s1 = 0; s1 < s0; s1++)
			found |= DetectPair(cellOutside.shapes[s0], cellOutside.shapes[s1], ct, dt, continuous);
	}
	return found;

}

}

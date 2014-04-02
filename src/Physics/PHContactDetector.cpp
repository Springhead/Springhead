/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHContactDetector.h>
#include <Foundation/UTPreciseTimer.h>

#include <unordered_set>

using namespace PTM;
using namespace std;

//#define REPORT_TIME 1

namespace Spr{;

#ifdef REPORT_TIME
Spr::UTPreciseTimer ptimerForCd;
#endif

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

void PHSolidPair::Init(PHContactDetector* d, PHSolid* s0, PHSolid* s1){
	detector = d;
	solid[0] = s0;
	solid[1] = s1;
	int ns0 = s0->NShape();
	int ns1 = s1->NShape();
	shapePairs.resize(ns0, ns1);
	for(int i = 0; i < ns0; i++)for(int j = 0; j < ns1; j++){
		PHShapePair* sp = shapePairs.item(i, j);
		if (!sp){
			sp = CreateShapePair();
			shapePairs.item(i, j) = sp;
		}
		sp->Init(this, solid[0]->frames[i], solid[1]->frames[j]);
	}
	bEnabled = true;
}

bool PHSolidPair::Detect(PHShapePair* shapePair, unsigned ct, double dt, bool continuous){
	PHFrame* fr0 = shapePair->frame[0];
	PHFrame* fr1 = shapePair->frame[1];

	bool found = false;
	if(continuous){	
		const double alpha = 2.0;
		if(shapePair->ContDetect(ct, fr0->pose_abs, fr1->pose_abs, alpha * fr0->delta, alpha * fr1->delta, dt)){
			double n = shapePair->normal.norm();
			assert(0.9 < n && n < 1.1);
			found = true;
			OnContDetect(shapePair, ct, dt);
		}		
	}
	else{
		if(shapePair->Detect(ct, fr0->pose_abs, fr1->pose_abs)){
			found = true;
			OnDetect(shapePair, ct, dt);
		}
	}

	// フリーズの解除
	if(found){
		if(fr0->solid->IsDynamical() && !fr1->solid->IsFrozen()){
			fr0->solid->SetFrozen(false);
		}
		else if(fr1->solid->IsDynamical() && !fr0->solid->IsFrozen()){
			fr1->solid->SetFrozen(false);
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
	solids        .clear();
	inactiveSolids.clear();
	solidPairs    .clear();
}

void PHContactDetector::AddInactiveSolid(PHSolidIf* solid){
	inactiveSolids.push_back(solid->Cast());
}

bool PHContactDetector::IsInactiveSolid(PHSolidIf* solid){
	for(PHSolids::iterator it = inactiveSolids.begin(); it != inactiveSolids.end(); it++){
		if((*it) == solid->Cast())
			return true;
	}
	return false;
}

bool PHContactDetector::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s && std::find(solids.begin(), solids.end(), s) == solids.end()){
		solids.push_back(s);
		int N = (int)solids.size();
		assert(solidPairs.height() == N-1 && solidPairs.width() == N-1);
		solidPairs.resize(N, N);
		for(int i = 0; i < N-1; i++){
			solidPairs.item(i, N-1) = CreateSolidPair();
			solidPairs.item(i, N-1)->Init(this, solids[i], solids[N-1]);
		}
		if(s->NShape())
			UpdateShapePairs(s);
		return true;
	}
	return false;
}

bool PHContactDetector::DelChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(!s)return false;
	
	PHSolids::iterator is;
	is = find(solids.begin(), solids.end(), s);
	if(is != solids.end()){
		int idx = (int)(is - solids.begin());
		solids.erase(is);
		solidPairs.erase_row(idx);
		solidPairs.erase_col(idx);
		return true;
	}
	is = find(inactiveSolids.begin(), inactiveSolids.end(), s);
	if(is != inactiveSolids.end())
		inactiveSolids.erase(is);

	return false;
}

int PHContactDetector::NSolidPairs() const{
	int n = (int)solids.size();
	return n * (n - 1) / 2;
}
int PHContactDetector::NShapePairs() const{
	int n = (int)solids.size(), N = 0;
	for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
		N += (int)(solidPairs.item(i,j)->shapePairs.size());
	return N;
}
int PHContactDetector::NActiveSolidPairs() const{
	int n = (int)solids.size(), N = 0;
	for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
		N += (int)solidPairs.item(i,j)->bEnabled;
	return N;
}
int PHContactDetector::NActiveShapePairs() const{
	int n = (int)solids.size(), N = 0;
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
	CDShapePairSt* shapeStates = es->ShapeStates();
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
	CDShapePairSt* shapeStates = es->ShapeStates();
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

void PHContactDetector::UpdateShapePairs(PHSolid* solid){
	PHSolids::iterator it = std::find(solids.begin(), solids.end(), solid);
	if(it == solids.end())
		return;
	int isolid = (int)(it - solids.begin());
	int i, j;
	PHSolidPair* solidPair;
	PHSolid *slhs, *srhs;
	for(i = 0; i < isolid; i++){
		solidPair = solidPairs.item(i, isolid);
		slhs = solidPair->solid[0];
		srhs = solid;
		solidPair->shapePairs.resize(slhs->NShape(), srhs->NShape());
		for(j = 0; j < slhs->NShape(); j++){
			PHShapePair* shapePair = solidPair->CreateShapePair();
			shapePair->Init(solidPair, slhs->frames[j], srhs->frames[srhs->NShape()-1]);
			solidPair->shapePairs.item(j, srhs->NShape()-1) = shapePair;
		}
	}
	for(i = isolid+1; i < (int)solids.size(); i++){
		solidPair = solidPairs.item(isolid, i);
		slhs = solid;
		srhs = solidPair->solid[1];
		solidPair->shapePairs.resize(solid->NShape(), srhs->NShape());
		for(j = 0; j < srhs->NShape(); j++){
			PHShapePair* shapePair = solidPair->CreateShapePair();
			shapePair->Init(solidPair, slhs->frames[slhs->NShape()-1], srhs->frames[j]);
			solidPair->shapePairs.item(slhs->NShape()-1, j) = shapePair;
		}
	}
}

void PHContactDetector::DelShapePairs(PHSolid* solid, int iBegin, int iEnd){
	PHSolids::iterator it = std::find(solids.begin(), solids.end(), solid);
	if(it == solids.end())
		return;
	int isolid = (int)(it - solids.begin());
	int i, j;
	PHSolid *slhs, *srhs;
	PHSolidPair* solidPair;
	for(i = 0; i < isolid; i++){
		solidPair = solidPairs.item(i, isolid);
		slhs = solidPair->solid[0];
		srhs = solid;
		//	消えたShapeに対応する行を詰める
		for(j = 0; j < slhs->NShape(); j++){
			for(int k=iEnd; k<srhs->NShape(); ++k)
				solidPair->shapePairs.item(j, k - (iEnd - iBegin)) = solidPair->shapePairs.item(j, k);
		}
		//	サイズの更新
		solidPair->shapePairs.resize(solid->NShape(), srhs->NShape());
	}
	for(i = isolid+1; i < (int)solids.size(); i++){
		solidPair = solidPairs.item(isolid, i);
		slhs = solid;
		srhs = solidPair->solid[1];
		//	消えたShapeに対応する行を詰める
		for(j = 0; j < srhs->NShape(); j++) {
			for(int k=iEnd; k<slhs->NShape(); ++k) {
				solidPair->shapePairs.item(k - (iEnd - iBegin), j) = solidPair->shapePairs.item(k, j);
			}
		}
		//	サイズの更新
		solidPair->shapePairs.resize(solid->NShape(), srhs->NShape());
	}
}

void PHContactDetector::EnableContact(PHSolidIf* lhs, PHSolidIf* rhs, bool bEnable){
	PHSolids::iterator ilhs, irhs;
	ilhs = find(solids.begin(), solids.end(), (PHSolid*)(lhs->Cast()));
	irhs = find(solids.begin(), solids.end(), (PHSolid*)(rhs->Cast()));
	if(ilhs == solids.end() || irhs == solids.end())
		return;
	int i = (int)(ilhs - solids.begin()), j = (int)(irhs - solids.begin());
	if(i > j)std::swap(i, j);
	assert(i < solidPairs.height() && j < solidPairs.width());
	solidPairs.item(i, j)->bEnabled = bEnable;
}

void PHContactDetector::EnableContact(PHSolidIf** group, size_t length, bool bEnable){
	std::vector<int> idx;
	PHSolids::iterator it;
	for(int i = 0; i < (int)length; i++){
		it = find(solids.begin(), solids.end(), (PHSolid*)(group[i]->Cast()));
		if(it != solids.end())
			idx.push_back((int)(it - solids.begin()));
	}
	sort(idx.begin(), idx.end());
	for(int i = 0; i < (int)idx.size(); i++){
		for(int j = i+1; j < (int)idx.size(); j++){
			solidPairs.item(idx[i], idx[j])->bEnabled = bEnable;
		}
	}
}

void PHContactDetector::EnableContact(PHSolidIf* solid, bool bEnable){
	PHSolids::iterator it = find(solids.begin(), solids.end(), (PHSolid*)(solid->Cast()));
	if(it == solids.end())
		return;
	int idx = (int)(it - solids.begin());
	for(int i = 0; i < idx; i++)
		solidPairs.item(i, idx)->bEnabled = bEnable;
	for(int i = idx+1; i < (int)solids.size(); i++)
		solidPairs.item(idx, i)->bEnabled = bEnable;
}

void PHContactDetector::EnableContact(bool bEnable){
	int n = (int)solids.size();
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
	if(sh0.idxSolid == sh1.idxSolid)
		return false;

	// 両方の形状とも中心がセルの外ならスキップ
	if(!sh0.center && !sh1.center)
		return false;

	if(sh0.idxSolid > sh1.idxSolid)
		swap(sh0, sh1);

	PHSolidPair* solidPair = solidPairs.item(sh0.idxSolid, sh1.idxSolid);
	
	// 無効な剛体対はスキップ
	if(!solidPair->bEnabled)
		return false;
	
	// 両方frozenならスキップ
	if(solidPair->solid[0]->IsFrozen() && solidPair->solid[1]->IsFrozen())
		return false;
	
	// 両方non-dynamicalならスキップ
	if(!solidPair->solid[0]->IsDynamical() && !solidPair->solid[1]->IsDynamical())
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
	if(NActiveSolidPairs() == 0)
		return false;
	
	// 各形状のAABBを計算
#ifdef REPORT_TIME
	ptimerForCd.CountUS();
#endif
	int nsolids = (int)solids.size();
	for(int i = 0; i < nsolids; ++i){
		PHSolid* solid = solids[i];
		solid->CalcAABB();
	}
#ifdef REPORT_TIME
	DSTR << "  aabb:" << ptimerForCd.CountUS();
#endif

#ifdef REPORT_TIME
	ptimerForCd.CountUS();
#endif

	if( mode >= PHSceneDesc::MODE_SORT_AND_SWEEP_X &&
	    mode <= PHSceneDesc::MODE_SORT_AND_SWEEP_Z ){
		int idx = mode - PHSceneDesc::MODE_SORT_AND_SWEEP_X;
		
		edges.clear();
		std::vector<Edge>::iterator eit = edges.begin();
		Vec3d bbmin, bbmax;
		for(int i = 0; i < nsolids; i++){
			int nshapes = solids[i]->NShape();
			for(int j = 0; j < nshapes; j++){
				PHBBox& bb = solids[i]->frames[j]->bbWorld[continuous];
				bbmin = bb.GetBBoxMin();
				bbmax = bb.GetBBoxMax();
				edges.push_back(Edge(i, j, bbmin[idx], true ));
				edges.push_back(Edge(i, j, bbmax[idx], false));
			}
		}
		std::sort(edges.begin(), edges.end());
	}
	else{
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
			int nshapes = solids[i]->NShape();
			for(int j = 0; j < nshapes; j++){
				PHBBox& bb = solids[i]->frames[j]->bbWorld[continuous];
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
	}

#ifdef REPORT_TIME
	DSTR << "  assign:" << ptimerForCd.CountUS();
	DSTR << endl;
#endif

#ifdef REPORT_TIME
	ptimerForCd.CountUS();
#endif
	nBroad  = 0;
	nNarrow = 0;
	bool found = false;

	if( mode >= PHSceneDesc::MODE_SORT_AND_SWEEP_X &&
		mode <= PHSceneDesc::MODE_SORT_AND_SWEEP_Z ){
		//端から見ていって，接触の可能性があるノードの判定をする．
		typedef std::set<ShapeIndex> ShapeIndexSet;
		ShapeIndexSet cur;

		for(std::vector<Edge>::iterator it = edges.begin(); it != edges.end(); ++it){
			// 初端だったら，リスト内の物体と判定
			if (it->bMin){
				for(ShapeIndexSet::iterator itf = cur.begin(); itf != cur.end(); ++itf){
					found |= DetectPair(it->index, *itf, ct, dt, continuous);
				}
				cur.insert(it->index);
			}
			// 終端なので削除．
			else{
				cur.erase(it->index);
			}
		}

	}
	else{
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
#ifdef REPORT_TIME
	DSTR << "  check:" << ptimerForCd.CountUS();
	DSTR << "total: " << NActiveShapePairs() << " broad : " << nBroad << "  narrow: " << nNarrow << std::endl;
#endif

	return found;
}

}

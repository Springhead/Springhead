/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHConstraintEngine.h>
#include <Physics/PHHingeJoint.h>
#include <Physics/PHSliderJoint.h>
#include <Physics/PHBallJoint.h>
#include <Physics/PHPathJoint.h>
#include <Physics/PHSpring.h>
#include <Physics/PHContactPoint.h>
#include <Physics/PHContactSurface.h>

using namespace PTM;
using namespace std;
namespace Spr{;

void PHSolidPairForLCP::OnContDetect(PHShapePairForLCP* sp, PHConstraintEngine* engine, unsigned ct, double dt){
	//	交差する2つの凸形状を接触面で切った時の切り口の形を求める
	//int start = engine->points.size();
	sp->EnumVertex(engine, ct, solid[0], solid[1]);
	//int end = engine->points.size();

	//	HASE_REPORT
/*	DSTR << "st:" << sp->state << " depth:" << sp->depth;
	DSTR << " n:" << sp->normal;
	DSTR << " p:" << sp->center;
	DSTR << " r:" << end-start;
	DSTR << std::endl;
	DSTR << "  ring " << end-start << ":";
	for(start; start!=end; ++start){
		PHContactPoint* p = ((PHContactPoint*)&*engine->points[start]);
		DSTR << p->pos << " ";
	}
	DSTR << sp->center;
	DSTR << std::endl;
*/		
}

// 接触解析．接触部分の切り口を求めて，切り口を構成する凸多角形の頂点をengineに拘束として追加する．	
void PHShapePairForLCP::EnumVertex(PHConstraintEngine* engine, unsigned ct, PHSolid* solid0, PHSolid* solid1){
	//	center と normalが作る面と交差する面を求めないといけない．
	//	面の頂点が別の側にある面だけが対象．
	//	quick hull は n log r だから，線形時間で出来ることはやっておくべき．

	//	各3角形について，頂点がどちら側にあるか判定し，両側にあるものを対象とする．
	//	交線を，法線＋数値の形で表現する．
	//	この処理は凸形状が持っていて良い．
	//	＃交線の表現形式として，2次曲線も許す．その場合，直線は返さない
	//	＃2次曲線はMullar＆Preparataには入れないで別にしておく．

	//	相対速度をみて2Dの座標系を決める。
	FPCK_FINITE(solid0->pose);
	FPCK_FINITE(solid1->pose);
	Vec3d v0 = solid0->GetPointVelocity(center);
	Vec3d v1 = solid1->GetPointVelocity(center);
	Matrix3d local;	//	contact coodinate system 接触の座標系
	local.Ex() = normal;
 	local.Ey() = v1-v0;
	local.Ey() -= local.Ey() * normal * normal;
	if (local.Ey().square() > 1e-6){
		local.Ey().unitize(); 
	}else{
		if (Square(normal.x) < 0.5) local.Ey()= (normal ^ Vec3f(1,0,0)).unit();
		else local.Ey() = (normal ^ Vec3f(0,1,0)).unit();
	}
	local.Ez() =  local.Ex() ^ local.Ey();
	if (local.det() < 0.9) {
		DSTR << "Error: local coordinate error." << std::endl;
		DSTR << local;
		Vec3d a = Vec3d(local[0][0], local[1][0], local[2][0]);
		Vec3d b = Vec3d(local[0][1], local[1][1], local[2][1]);
		Vec3d c = Vec3d(local[0][2], local[1][2], local[2][2]);
		DSTR << "dot(local[0], local[1]) :" << dot(a, b) << std::endl;
		DSTR << "dot(local[1], local[2]) :" << dot(b, c) << std::endl;
		DSTR << "dot(local[2], local[0]) :" << dot(c, a) << std::endl;
		DSTR << "norm a :" << a.norm() << std::endl;
		DSTR << "norm b :" << b.norm() << std::endl;
		DSTR << "norm c :" << c.norm() << std::endl;
		DSTR << "normal:" << normal << std::endl;
		DSTR << "v1-v0:" << v1-v0 << std::endl;
		assert(0);
	}

	//	面と面が触れる場合があるので、接触が凸多角形や凸形状になることがある。
	//	切り口を求める。まず、それぞれの形状の切り口を列挙
	CDCutRing cutRing(commonPoint, local);	//	commonPointならば、それを含む面で切れば、必ず切り口の中になる。
	int nPoint = (int)engine->points.size();
	//	両方に切り口がある場合．(球などないものもある)
	bool found = shape[0]->FindCutRing(cutRing, shapePoseW[0]);
	int nLine0 = (int)cutRing.lines.size();
	if (found) found = shape[1]->FindCutRing(cutRing, shapePoseW[1]);
	int nLine1 = (int)cutRing.lines.size() - nLine0;
	if (found){
		//	2つの切り口のアンドをとって、2物体の接触面の形状を求める。
		cutRing.MakeRing();		
		section.clear();
		std::vector<Vec3d>	local_section;	//接触座標系での接触面の頂点(面接触用)
		local_section.clear();		
		if (cutRing.vtxs.begin != cutRing.vtxs.end && !(cutRing.vtxs.end-1)->deleted){
			CDQHLine<CDCutLine>* vtx = cutRing.vtxs.end-1;
			do{
				assert(finite(vtx->dist));
#ifdef _DEBUG
				if (vtx->dist < 1e-200){
					DSTR << "Error:  PHShapePairForLCP::EnumVertex() :  distance too small." << std::endl;
					DSTR << vtx->dist << vtx->normal << std::endl;
					DSTR << cutRing.local << std::endl;
					
					DSTR << "Lines:(" << nLine0 << "+" << nLine1 << ")" << std::endl;
					for(unsigned i=0; i<cutRing.lines.size(); ++i){
						DSTR << cutRing.lines[i].dist << "\t" << cutRing.lines[i].normal << "\t";
						Vec2d pos = cutRing.lines[i].dist * cutRing.lines[i].normal;
						DSTR << pos.X() << "\t" << pos.Y() << std::endl;
					}

					DSTR << "Vertices in dual space:" << std::endl;
					for(CDQHLine<CDCutLine>* vtx = cutRing.vtxs.begin; vtx!=cutRing.vtxs.end; ++vtx){
						if (vtx->deleted) continue;
						DSTR << vtx->dist << "\t" << vtx->normal << "\t";
						double d = vtx->dist;
						if (d==0) d=1e-100;
						Vec2d pos = vtx->normal * d;
						DSTR << pos.X() << "\t" << pos.Y() << std::endl;
					}
					cutRing.lines.clear();
					shape[0]->FindCutRing(cutRing, shapePoseW[0]);
					shape[1]->FindCutRing(cutRing, shapePoseW[1]);
					continue;
				}
#endif
				Vec3d pos;
				pos.sub_vector(1, Vec2d()) = vtx->normal / vtx->dist;
				if(engine->bUseContactSurface == true){
					local_section.push_back(pos);
				}
				pos = cutRing.local * pos;
				section.push_back(pos);

				if(engine->bUseContactSurface == false){
					PHContactPoint *point = DBG_NEW PHContactPoint(local, this, pos, solid0, solid1);
					point->SetScene(engine->GetScene());
					point->engine = engine;

					if(engine->IsInactiveSolid(solid0->Cast())) point->SetInactive(1, false);
					if(engine->IsInactiveSolid(solid1->Cast())) point->SetInactive(0, false);
					engine->points.push_back(point);
				}
				vtx = vtx->neighbor[0];
			} while (vtx!=cutRing.vtxs.end-1);

			if(engine->bUseContactSurface == true){
				Vec3d pos(0.0, 0.0, 0.0);//拘束点は接触座標系の原点とする
				pos = cutRing.local * pos;
				PHContactSurface *point = DBG_NEW PHContactSurface(local, this, pos, solid0, solid1, local_section);
				point->SetScene(engine->GetScene());
				point->engine = engine;

				if(engine->IsInactiveSolid(solid0->Cast())) point->SetInactive(1, false);
				if(engine->IsInactiveSolid(solid1->Cast())) point->SetInactive(0, false);
				engine->points.push_back(point);
			}
		}
	}
	if (nPoint == (int)engine->points.size()){	//	ひとつも追加していない＝切り口がなかった or あってもConvexHullが作れなかった．
		//	きっと1点で接触している．
		PHContactPoint *point = DBG_NEW PHContactPoint(local, this, center, solid0, solid1);
		point->SetScene(engine->GetScene());
		point->engine = engine;

		if(engine->IsInactiveSolid(solid0->Cast())) point->SetInactive(1, false);
		if(engine->IsInactiveSolid(solid1->Cast())) point->SetInactive(0, false);

		engine->points.push_back(point);
	//	section.clear();
	//	section.push_back(center);
	}
}

void PHSolidPairForLCP::OnDetect(PHShapePairForLCP* sp, PHConstraintEngine* engine, unsigned ct, double dt){
	//	法線を求める
	sp->CalcNormal();
	//	交差する2つの凸形状を接触面で切った時の切り口の形を求める
	sp->EnumVertex(engine, ct, solid[0], solid[1]);
}			

//----------------------------------------------------------------------------
// PHConstraintEngine
PHConstraintEngineDesc::PHConstraintEngineDesc(){
	numIter					 = 15;
	numIterCorrection		 = 0;
	numIterContactCorrection = 0;
	velCorrectionRate		 = 0.3;
	posCorrectionRate		 = 0.3;
	contactCorrectionRate	 = 0.1;
	shrinkRate				 = 0.7;
	shrinkRateCorrection	 = 0.7;
	freezeThreshold			 = 0.0;
	accelSOR				 = 1.0;
	bGearNodeReady	 = false;
	bSaveConstraints = false;
	bUpdateAllState	 = true;
	bUseContactSurface = false;
}
PHConstraintEngine::PHConstraintEngine(){
	bContactDetectionEnabled = true;
}

PHConstraintEngine::~PHConstraintEngine(){
	
}

void PHConstraintEngine::Clear(){
	points.clear();
	joints.clear();
}

PHJoint* PHConstraintEngine::CreateJoint(const IfInfo* ii, const PHJointDesc& desc, PHSolid* lhs, PHSolid* rhs){
	if(std::find(solids.begin(), solids.end(), lhs) == solids.end())
		return 0;
	if(std::find(solids.begin(), solids.end(), rhs) == solids.end())
		return 0;

	PHJoint* joint = NULL;
	if(ii == PHHingeJointIf::GetIfInfoStatic())
		joint = DBG_NEW PHHingeJoint();
	else if(ii == PHSliderJointIf::GetIfInfoStatic())
		joint = DBG_NEW PHSliderJoint();
	else if(ii == PHBallJointIf::GetIfInfoStatic())
		joint = DBG_NEW PHBallJoint();
	else if(ii == PHPathJointIf::GetIfInfoStatic())
		joint = DBG_NEW PHPathJoint();
	else if(ii == PHSpringIf::GetIfInfoStatic())
		joint = DBG_NEW PHSpring();
	else assert(false);
	joint->SetScene(GetScene());
	joint->SetDesc(&desc);
	joint->solid[0] = lhs;
	joint->solid[1] = rhs;
	return joint;
}

PHRootNode* PHConstraintEngine::CreateRootNode(const PHRootNodeDesc& desc, PHSolid* solid){
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++)
		if((*it)->FindBySolid(solid))
			return NULL;
	if(find(solids.begin(), solids.end(), solid) == solids.end())
		return NULL;
	
	PHRootNode* root = DBG_NEW PHRootNode();
	root->AddChildObject(solid->Cast());
	return root;
}
PHTreeNode* PHConstraintEngine::CreateTreeNode(const PHTreeNodeDesc& desc, PHTreeNode* parent, PHSolid* solid){
	PHTreeNode* node;
	
	//既存のツリーに含まれていないかチェック
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++)
		if((*it)->FindBySolid(solid))
			return NULL;
	//parentが既存のツリーのノードかチェック（念のため）
	bool found = false;
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++){
		if((*it)->Includes(parent)){
			found = true;
			break;
		}
	}
	if(!found)return NULL;

	//parentに対応する剛体とsolidで指定された剛体とをつなぐ拘束を取得
	PHJoint* joint = DCAST(PHJoint, joints.FindBySolidPair(parent->GetSolid(), solid));
	if(!joint)return NULL;
	//拘束の種類に対応するノードを作成
	node = joint->CreateTreeNode();
	node->AddChildObject(joint->Cast());
	parent->AddChildObject(node->Cast());

	node->Enable(parent->IsEnabled());

	return node;
}

PHPath* PHConstraintEngine::CreatePath(const PHPathDesc& desc){
	PHPath* path = DBG_NEW PHPath(desc);
	return path;
}

PHGear* PHConstraintEngine::CreateGear(const PHGearDesc& desc, PH1DJoint* lhs, PH1DJoint* rhs){
	PHGear* gear = DBG_NEW PHGear();
	gear->joint[0] = lhs;
	gear->joint[1] = rhs;
	gear->SetDesc(&desc);
	return gear;
}

bool PHConstraintEngine::AddChildObject(ObjectIf* o){
	if(Detector::AddChildObject(o))
		return true;

	PHConstraint* con = DCAST(PHConstraint, o);
	if(con){
		con->engine = this;
		joints.push_back(con);
		return true;
	}
	PHRootNode* root = DCAST(PHRootNode, o);
	if(root){
		root->Prepare(GetScene()->Cast(), this);
		trees.push_back(root);
		bGearNodeReady = false;
		return true;
	}
	PHGear* gear = DCAST(PHGear, o);
	if(gear){
		gear->engine = this;
		gears.push_back(gear);
		bGearNodeReady = false;
		return true;
	}
	PHPath* path = DCAST(PHPath, o);
	if(path){
		paths.push_back(path);
		return true;
	}
	return false;
}

void PHConstraintEngine::UpdateGearNode(){
	for(int i = 0; i < (int)trees.size(); i++)
		trees[i]->ResetGearNode();

	for(int i = 0; i < (int)gears.size(); i++){
		PHGear* gear = gears[i];
		gear->bArticulated = false;
		PHTreeNode1D *nodeL, *nodeR;
		for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++){
			nodeL = DCAST(PHTreeNode1D, (*it)->FindByJoint(gear->joint[0]));
			nodeR = DCAST(PHTreeNode1D, (*it)->FindByJoint(gear->joint[1]));
			if(!nodeL || !nodeR)continue;
			if(nodeL->GetParent() == nodeR)
				nodeR->AddGear(gear, nodeL);
			else if(nodeR->GetParent() == nodeL)
				nodeL->AddGear(gear, nodeR);
			else if(nodeL->GetParent() == nodeR->GetParent())
				nodeL->AddGear(gear, nodeR);
		}
	}
}

bool PHConstraintEngine::DelChildObject(ObjectIf* o){
	// ＊相互依存するオブジェクトの削除が必要だが未実装
	if(Detector::DelChildObject(o))
		return true;
	
	PHJoint* joint = DCAST(PHJoint, o);
	if(joint){
		PHConstraints::iterator it = find(joints.begin(), joints.end(), joint);
		if(it != joints.end()){
			joints.erase(it);
			return true;
		}
		return false;
	}
	PHRootNode* root = DCAST(PHRootNode, o);
	if(root){
		PHRootNodes::iterator it = find(trees.begin(), trees.end(), root);
		if(it != trees.end()){
			trees.erase(it);
			bGearNodeReady = false;
			return true;
		}
		return false;
	}
	PHGear* gear = DCAST(PHGear, o);
	if(gear){
		PHGears::iterator it = find(gears.begin(), gears.end(), gear);
		if(it != gears.end()){
			gears.erase(it);
			bGearNodeReady = false;
			return true;
		}
		return false;
	}
	PHPath* path = DCAST(PHPath, o);
	if(path){
		PHPaths::iterator it = find(paths.begin(), paths.end(), path);
		if(it != paths.end()){
			paths.erase(it);
			return true;
		}
		return false;
	}
	return false;
}

void PHConstraintEngine::SetupLCP(){
	/* 相互に依存関係があるので呼び出し順番には注意する */
	
	// ツリー構造の前処理(ABA関係)
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++)
		(*it)->SetupABA();

	// 接触拘束の前処理
	for(PHConstraints::iterator it = points.begin(); it != points.end(); it++)
		(*it)->SetupLCP();
	
	// 関節拘束の前処理
	for(PHConstraints::iterator it = joints.begin(); it != joints.end(); it++)
		(*it)->SetupLCP();
	
	// ギア拘束の前処理
	for(PHGears::iterator it = gears.begin(); it != gears.end(); it++)
		(*it)->SetupLCP();

}
void PHConstraintEngine::SetupCorrectionLCP(){
 	if(numIterCorrection)
		for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++)
			(*it)->SetupCorrectionABA();

	if(numIterContactCorrection)
		for(PHConstraints::iterator it = points.begin(); it != points.end(); it++)
			(*it)->SetupCorrectionLCP();

	if(numIterCorrection)
		for(PHConstraints::iterator it = joints.begin(); it != joints.end(); it++)
			(*it)->SetupCorrectionLCP();
}

void PHConstraintEngine::IterateLCP(){
	int count = 0;
	while(true){
		if(count == numIter)
			break;
		for(PHConstraints::iterator it = points.begin(); it != points.end(); it++)
			(*it)->IterateLCP();
		for(PHConstraints::iterator it = joints.begin(); it != joints.end(); it++)
			(*it)->IterateLCP();
		for(PHGears::iterator it = gears.begin(); it != gears.end(); it++)
			(*it)->IterateLCP();
		count++;
	}
}

void PHConstraintEngine::IterateCorrectionLCP(){
	int end = max(numIterCorrection, numIterContactCorrection);
	for(int i=0; i!=end; ++i){
		if (i<numIterContactCorrection)
			for(PHConstraints::iterator it = points.begin(); it != points.end(); it++)
				(*it)->IterateCorrectionLCP();
		if (i<numIterCorrection)
			for(PHConstraints::iterator it = joints.begin(); it != joints.end(); it++)
				(*it)->IterateCorrectionLCP();
	}
}

void PHConstraintEngine::UpdateSolids(bool bVelOnly){
	double dt = GetScene()->GetTimeStep();

	// ツリーに属さない剛体の更新
	for(PHSolids::iterator is = solids.begin(); is != solids.end(); is++){
		if(!(*is)->IsArticulated() && !(*is)->IsUpdated()){
			(*is)->UpdateVelocity(dt);
			if(!bVelOnly)
				(*is)->UpdatePosition(dt);
			(*is)->SetUpdated(true);
		}
	}

	// ツリーに属する剛体の更新
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++){
		(*it)->UpdateVelocity(dt);
		if(!bVelOnly)
			(*it)->UpdatePosition(dt);
	}
}

#ifdef REPORT_TIME
}
#include <Foundation/UTPreciseTimer.h>
namespace Spr{
UTPreciseTimer ptimer;
UTPreciseTimer ptimerForCd;
#endif


void PHConstraintEngine::StepPart1(){
	//	DSTR << "nContact:" <<  points.size() << std::endl;
	unsigned int ct = GetScene()->GetCount();
	double dt = GetScene()->GetTimeStep();

	// 必要ならばギアノードの更新
	if(!bGearNodeReady){
		UpdateGearNode();
		bGearNodeReady = true;
	}

	//交差を検知
	points.clear();
#ifdef REPORT_TIME
	ptimer.CountUS();
#endif
	if(bContactEnabled && bContactDetectionEnabled){
		//Detect(ct, dt);
		ContDetect(ct, dt);
		if (renderContact) UpdateContactInfoQueue();
#ifdef _DEBUG
//		DSTR << "nMaxOverlapObject = " << nMaxOverlapObject << std::endl;
#endif
	}
#ifdef REPORT_TIME
	DSTR << " col:" << ptimer.CountUS();
#endif
#ifdef REPORT_TIME
	ptimer.CountUS();
#endif
}

void PHConstraintEngine::StepPart2(){
	double dt = GetScene()->GetTimeStep();

	// 前回のStep以降に別の要因によって剛体の位置・速度が変化した場合
	// ヤコビアン等の再計算
	// 各剛体の前処理
	for(PHSolids::iterator it = solids.begin(); it != solids.end(); it++)
		(*it)->UpdateCacheLCP(dt);
	for(PHConstraints::iterator it = points.begin(); it != points.end(); it++)
		(*it)->UpdateState();
	for(PHConstraints::iterator it = joints.begin(); it != joints.end(); it++)
		(*it)->UpdateState();
	
	SetupLCP();
#ifdef REPORT_TIME
	DSTR << " sup:" << ptimer.CountUS();
#endif
	IterateLCP();

#ifdef REPORT_TIME
	DSTR << " ite:" << ptimer.CountUS() << std::endl;
#endif
	SetupCorrectionLCP();
	IterateCorrectionLCP();
	// 位置・速度の更新
	UpdateSolids(!bUpdateAllState);

	for(PHConstraints::iterator it = joints.begin(); it != joints.end(); it++)
		(*it)->bProhibitUpdateSolidCacheLCP = false;
}
	
void PHConstraintEngine::Step(){
	StepPart1();	// 接触判定
	StepPart2();	// 拘束力計算，積分
}

PHConstraintsIf* PHConstraintEngine::GetContactPoints(){
	return DCAST(PHConstraintsIf, &points);
}


//	state関係の実装
size_t PHConstraintEngine::GetStateSize() const{
	return Detector::GetStateSize() + 
		(bSaveConstraints ? sizeof(PHConstraintsSt) : 0);
}
void PHConstraintEngine::ConstructState(void* m) const{
	Detector::ConstructState(m);
	char* p = (char*)m;
	if (bSaveConstraints){
		p += Detector::GetStateSize();
		new (p) PHConstraintsSt;
	}
}
void PHConstraintEngine::DestructState(void* m) const {
	Detector::DestructState(m);
	char* p = (char*)m;
	if (bSaveConstraints){
		p += Detector::GetStateSize();
		((PHConstraintsSt*)p)->~PHConstraintsSt();
	}
}
bool PHConstraintEngine::GetState(void* s) const {
	Detector::GetState(s);
	char* p = (char*)s;
	if (bSaveConstraints){
		PHConstraintsSt* st = (PHConstraintsSt*)(p + Detector::GetStateSize());
		st->joints.resize(joints.size());
		for(size_t i=0; i<joints.size(); ++i){
			joints[i]->GetState(&st->joints[i]);
		}
		st->gears.resize(gears.size());
		for(size_t i=0; i<gears.size(); ++i){
			gears[i]->GetState(&st->gears[i]);
		}
	}
	return true;
}
void PHConstraintEngine::SetState(const void* s){
	Detector::SetState(s);
	char* p = (char*)s;
	if (bSaveConstraints){
		PHConstraintsSt* st = (PHConstraintsSt*)(p + Detector::GetStateSize());
		joints.resize(st->joints.size());
		for(size_t i=0; i<joints.size(); ++i){
			joints[i]->SetState(&st->joints[i]);
		}
		gears.resize(st->gears.size());
		for(size_t i=0; i<gears.size(); ++i){
			gears[i]->SetState(&st->gears[i]);
		}
	}
}

void PHConstraintEngine::EnableRenderContact(bool enable){
	renderContact = enable;
}
void PHConstraintEngine::UpdateContactInfoQueue(){
	int writing = contactInfoQueue.wrote + 1;
	if (writing > 2) writing = 0;
	if (contactInfoQueue.reading == writing) return;

	contactInfoQueue.queue[writing].Clear();
	contactInfoQueue.queue[writing].points = points;
	contactInfoQueue.queue[writing].sections.resize(points.size());
	for(unsigned i=0; i<points.size(); ++i){
		PHContactPoint* point = (PHContactPoint*) &*points[i];
		std::vector<Vec3d>& section = point->shapePair->section;
		contactInfoQueue.queue[writing].sections[i].resize(section.size());
		std::copy(section.begin(), section.end(), contactInfoQueue.queue[writing].sections[i].begin());			
	}
	contactInfoQueue.wrote = writing;
}
void PHConstraintEngine::ContactInfo::Clear(){
	sections.clear();
	points.clear();
}

PHConstraintEngine::ContactInfoQueue::ContactInfoQueue():reading(0),wrote(0){
}



}	//	namespace Spr

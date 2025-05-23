﻿/*
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
#include <Physics/PHFixJoint.h>
#include <Physics/PHGenericJoint.h>
#include <Physics/PHSpring.h>
#include <Physics/PHContactPoint.h>
#include <Physics/PHContactSurface.h>

#include <iomanip>
#include <fstream>

using namespace PTM;
using namespace std;

namespace Spr{;

/* 時間計測
   bReportをtrueにすると処理時間を計測して
   DSTRに出すと同時にcsvファイルに書き出す
   
   * DSTRやファイルに書き出すのに1ms程度かかるので注意

 */
const char*    reportFilename = "PHConstraintEngineReport.csv";

int		colcounter; //サポート探索回数のカウント

void PHSolidPairForLCP::OnDetect(PHShapePair* _sp, unsigned ct, double dt){
	PHSolidPair::OnDetect(_sp, ct, dt);
	PHShapePairForLCP* sp = (PHShapePairForLCP*)_sp;
	//	法線を求める
	sp->CalcNormal();
	//	交差する2つの凸形状を接触面で切った時の切り口の形を求める
	PHSolid* solid0 = body[0]->Cast();
	PHSolid* solid1 = body[1]->Cast();
	if (solid0 && solid1) {
		sp->EnumVertex(ct, solid0, solid1);
	}
}			

void PHSolidPairForLCP::OnContDetect(PHShapePair* _sp, unsigned ct, double dt){
	PHSolidPair::OnContDetect(_sp, ct, dt);
	PHShapePairForLCP* sp = (PHShapePairForLCP*)_sp;

	//	交差する2つの凸形状を接触面で切った時の切り口の形を求める
	PHSolid* solid0 = body[0]->Cast();
	PHSolid* solid1 = body[1]->Cast();
	if (solid0 && solid1) {
		sp->EnumVertex(ct, solid0, solid1);
	}
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
void PHShapePairForLCP::EnumVertex(unsigned ct, PHSolid* solid0, PHSolid* solid1){
	//	center と normalが作る面と交差する面を求めないといけない．
	//	面の頂点が別の側にある面だけが対象．
	//	quick hull は n log r だから，線形時間で出来ることはやっておくべき．

	//	各3角形について，頂点がどちら側にあるか判定し，両側にあるものを対象とする．
	//	交線を，法線＋数値の形で表現する．
	//	この処理は凸形状が持っていて良い．
	//	＃交線の表現形式として，2次曲線も許す．その場合，直線は返さない
	//	＃2次曲線はMullar＆Preparataには入れないで別にしておく．

	PHConstraintEngine* engine = (PHConstraintEngine*)solidPair->detector;

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
				Vec3d pos(0.0, 0.0, 0.0);
				//// 拘束点は接触座標系の原点とする
				//拘束点は断面の中心とする
				for(int i = 0; i < (int)local_section.size(); i++)
					pos += local_section[i];
				pos /= local_section.size();

				//断面も中心基準にする
				for(int i = 0; i < (int)local_section.size(); i++)
					local_section[i] -= pos;

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
	}
}

//----------------------------------------------------------------------------
// PHConstraintEngine
PHConstraintEngineDesc::PHConstraintEngineDesc() {
	method = PHSceneDesc::SOLVER_GS;	//	This initial value will override by PHSceneDesc::method
	numIter = 15;
	numIterCorrection = 0;
	numIterContactCorrection = 0;
	velCorrectionRate = 0.3;
	posCorrectionRate = 0.3;
	contactCorrectionRate = 0.1;
	shrinkRate = 0.7;
	shrinkRateCorrection = 0.7;
	freezeThreshold = 0.0;
	accelSOR = 1.0;
	dfEps = 0.0;
	//	拘束力計算を安定化させる係数、大きくすると拘束力が弱くなり、すり抜けるなどの問題が起きる。
	//	A coefficient to stabilize the constraint force calculation; The larger regularization makes constraint forces weaker and problems such as tunneling could occur.
	regularization = 1e-6;
	bUpdateAllState = true;
	bUseContactSurface = false;
	bReport = false;
}

PHConstraintEngine::PHConstraintEngine(UTPerformanceMeasureIf* pm):
	timeCollision(pm->Count("collision")), 
	timeSetup(pm->Count("setup")),
	timeIterate(pm->Count("iterate"))
{
	dfEps         = 1.0e-12;
	renderContact = true;
}

PHConstraintEngine::~PHConstraintEngine(){
}

void PHConstraintEngine::Clear(){
	points      .clear();
	joints      .clear();
	trees       .clear();
	gears       .clear();
	paths       .clear();
	cons        .clear();
	cons_base   .clear();
}

PHJoint* PHConstraintEngine::CreateJoint(const IfInfo* ii, const PHJointDesc& desc, PHSolid* lhs, PHSolid* rhs){
	if(std::find(bodies.begin(), bodies.end(), lhs) == bodies.end())
		return 0;
	if(std::find(bodies.begin(), bodies.end(), rhs) == bodies.end())
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
	else if(ii == PHGenericJointIf::GetIfInfoStatic())
		joint = DBG_NEW PHGenericJoint();
	else if(ii == PHSpringIf::GetIfInfoStatic())
		joint = DBG_NEW PHSpring();
	else if(ii == PHFixJointIf::GetIfInfoStatic())
		joint = DBG_NEW PHFixJoint();
	else if(ii == PHPointToPointMateIf::GetIfInfoStatic())
		joint = DBG_NEW PHPointToPointMate();
	else if(ii == PHPointToLineMateIf::GetIfInfoStatic())
		joint = DBG_NEW PHPointToLineMate();
	else if(ii == PHPointToPlaneMateIf::GetIfInfoStatic())
		joint = DBG_NEW PHPointToPlaneMate();
	else if(ii == PHLineToLineMateIf::GetIfInfoStatic())
		joint = DBG_NEW PHLineToLineMate();
	else if(ii == PHPlaneToPlaneMateIf::GetIfInfoStatic())
		joint = DBG_NEW PHPlaneToPlaneMate();
	else assert(false);
	joint->SetScene(GetScene());
	joint->SetDesc(&desc);
	joint->solid[0] = lhs;
	joint->solid[1] = rhs;
	return joint;
}

PHRootNode* PHConstraintEngine::CreateRootNode(const PHRootNodeDesc& desc, PHSolid* solid){
	// コンテナに含まれない剛体はNG
	if(find(bodies.begin(), bodies.end(), solid) == bodies.end())
		return NULL;

	// 既存のツリーに含まれる剛体
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++){
		// 非ルートノードになっている剛体をあらたにルートノードにはできない
		if((*it)->FindBySolid(solid))
			return NULL;
	}
	
	PHRootNode* root = DBG_NEW PHRootNode();
	root->engine = this;
	root->AddChildObject(solid->Cast());
	return root;
}

PHTreeNode* PHConstraintEngine::CreateTreeNode(const PHTreeNodeDesc& desc, PHTreeNode* parent, PHSolid* solid){
	//既存のツリーに含まれていないかチェック
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++){
		if((*it)->FindBySolid(solid)){
			DSTR << "CreateTreeNode: solid is already a part of existing tree" << std::endl;
			return NULL;
		}
	}
	//parentが既存のツリーのノードかチェック（念のため）
	bool found = false;
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++){
		if((*it)->Includes(parent)){
			found = true;
			break;
		}
	}
	if(!found){
		DSTR << "CreateTreeNode: parent node must be a part of existing tree" << std::endl;
		return NULL;
	}

	//parentに対応する剛体とsolidで指定された剛体とをつなぐ拘束を取得
	PHJoint* joint = DCAST(PHJoint, joints.FindBySolidPair(parent->GetSolid()->Cast(), solid));
	if(!joint){
		DSTR << "CreateTreeNode: joint not found" << std::endl;
		return NULL;
	}
	
	//拘束の種類に対応するノードを作成
	PHTreeNode* node = joint->CreateTreeNode();
	if(!node){
		DSTR << "CreateTreeNode: this joint does not support tree node" << std::endl;
		return NULL;
	}

	node  ->AddChildObject(joint->Cast());
	parent->AddChildObject(node ->Cast());
	node  ->Enable(parent->IsEnabled());
	
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
	if(PHContactDetector::AddChildObject(o))
		return true;

	PHConstraint* con = DCAST(PHConstraint, o);
	if(con){
		con->engine = this;
		joints.push_back(con);
		return true;
	}
	PHRootNode* root = DCAST(PHRootNode, o);
	if(root){
		trees.push_back(root);
		return true;
	}
	PHGear* gear = DCAST(PHGear, o);
	if(gear){
		gear->engine = this;
		gears.push_back(gear);
		// ギアトレイン更新のためツリーをinvalidate
		for(int i = 0; i < (int)trees.size(); i++)
			trees[i]->bReady = false;

		return true;
	}
	PHPath* path = DCAST(PHPath, o);
	if(path){
		paths.push_back(path);
		return true;
	}
	return false;
}

bool PHConstraintEngine::DelChildObject(ObjectIf* o){
	PHContactDetector::DelChildObject(o);

	// 剛体
	PHSolid* solid = DCAST(PHSolid, o);
	if(solid){
		// 接続している関節と接触を削除
		for(int i = 0; i < (int)joints.size(); ){
			PHConstraint* jnt = joints[i];
			if(jnt->solid[0] == solid || jnt->solid[1] == solid)
				 DelChildObject(jnt->Cast());
			else i++;
		}
		for(int i = 0; i < (int)points.size(); ){
			PHConstraint* pnt = points[i];
			if(pnt->solid[0] == solid || pnt->solid[1] == solid)
				 DelChildObject(pnt->Cast());
			else i++;
		}
		// ルートノードがあれば削除
		if(solid->treeNode)
			DelChildObject(solid->treeNode->Cast());
	}
	
	// 関節
	PHJoint* joint = DCAST(PHJoint, o);
	if(joint){
		PHConstraints::iterator it = find(joints.begin(), joints.end(), joint);
		if(it != joints.end()){
			// ツリーノードがあればそれも削除
			if(joint->treeNode)
				DelChildObject(joint->treeNode->Cast());

			joints.erase(it);
			return true;
		}
		return false;
	}

	// 接触
	PHContactPoint* point = DCAST(PHContactPoint, o);
	if(point){
		PHConstraints::iterator it = find(points.begin(), points.end(), point);
		if(it != points.end()){
			points.erase(it);
			return true;
		}
		return false;
	}

	// ツリーノード
	PHTreeNode* node = DCAST(PHTreeNode, o);
	if(node){
		// 子ノードを先に削除する
		while(!node->Children().empty())
			DelChildObject(node->Children()[0]->Cast());
	
		PHRootNode* root = DCAST(PHRootNode, o);
		// ルートノード
		if(root){
			// 剛体からの切り離し
			if(root->solid)
				root->solid->treeNode = 0;

			trees.erase(find(trees.begin(), trees.end(), root));
		}
		// ルート以外のツリーノード
		else{
			// ルートをinvalidate
			node->root->bReady = false;

			// 関節と剛体から切り離す
			if(node->joint)
				node->joint->treeNode = 0;
			if(node->solid)
				node->solid->treeNode = 0;

			// 親から切り離す（同時にdeleteされる）
			node->SetParent(0);
		}
		return true;
	}
	// ギア
	PHGear* gear = DCAST(PHGear, o);
	if(gear){
		PHGears::iterator it = find(gears.begin(), gears.end(), gear);
		if(it != gears.end()){
			gears.erase(it);
			// ツリーをinvalidate
			for(int i = 0; i < (int)trees.size(); i++)
				trees[i]->bReady = false;

			return true;
		}
		return false;
	}
	// パス
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

void PHConstraintEngine::Setup(){
	//< ツリー構造の前処理(ABA関係)
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp for
    #endif
	for(int i = 0; i < (int)trees.size(); i++){
		if(!trees[i]->IsEnabled())
			continue;
		trees[i]->Setup();
	}
		
	// 反復計算用拘束配列の作成
	// cons      : PHConstraint派生クラス
	// cons_base : PHConstraintBase派生クラス：PHConstraint派生クラスが先頭に来るように
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp single
    #endif
	{
		cons     .clear();
		cons_base.clear();
		
		// 接触
		for(int i = 0; i < (int)points.size(); i++){
			if(points[i]->IsEnabled() && points[i]->IsFeasible()){
				cons     .push_back(points[i]);
				cons_base.push_back(points[i]);
			}
		}
		// 関節
		for(int i = 0; i < (int)joints.size(); i++){
			PHFixJoint* fixJoint = joints[i]->Cast();
			if (fixJoint != NULL) {
				continue;
			}
			if(joints[i]->IsEnabled() && joints[i]->IsFeasible()){
				cons     .push_back(joints[i]);
				cons_base.push_back(joints[i]);
			}
		}
		// 可動範囲，モータ
		for(int i = 0; i < (int)joints.size(); i++){
			if(joints[i]->IsEnabled() && joints[i]->IsFeasible()){
				PH1DJoint* jnt1D = joints[i]->Cast();
				if(jnt1D){
					// Motorを先に入れないとMotorに対してLimitがかからない
					for (size_t j = 0; j < jnt1D->motors.size(); j++){
						if (jnt1D->motors[j]){
							cons_base.push_back(jnt1D->motors[j]);
						}
					}
					/*
					if (jnt1D->motor)
						cons_base.push_back(jnt1D->motor);
					*/
					if(jnt1D->limit && jnt1D->limit->IsEnabled())
						cons_base.push_back(jnt1D->limit);
				}
				PHBallJoint* ball = joints[i]->Cast();
				if(ball){
					// Motorを先に入れないとMotorに対してLimitがかからない
					for (size_t j = 0; j < ball->motors.size(); j++){
						if (ball->motors[j]){
							cons_base.push_back(ball->motors[j]);
						}
					}
					/*
					if(ball->motor)
						cons_base.push_back(ball->motor);
					*/
					if(ball->limit && ball->limit->IsEnabled())
						cons_base.push_back(ball->limit);
				}
				PHSpring* spring = joints[i]->Cast();
				if(spring){
					if(spring->motor)
						cons_base.push_back(spring->motor);
				}
			}
		}
		// ギア
		for(int i = 0; i < (int)gears.size(); i++){
			if(gears[i]->IsEnabled() && gears[i]->IsFeasible() && !gears[i]->IsArticulated())
				cons_base.push_back(gears[i]);
		}

		// FixJointによる固定を正確にするためにガウス・ザイデル法での計算順が最後に来るようにする
		for(int i = 0; i < (int)joints.size(); i++){
			PHFixJoint* fixJoint = joints[i]->Cast();
			if (fixJoint != NULL && joints[i]->IsEnabled() && joints[i]->IsFeasible()) {
				cons     .push_back(joints[i]);
				cons_base.push_back(joints[i]);
			}
		}
		
		// 拘束自由度の決定
		for(int i = 0; i < (int)cons_base.size(); i++)
			cons_base[i]->SetupAxisIndex();
		
	} //< omp single

	// 拘束毎の前処理（J, b, db, dA, ...）
    #ifdef USE_OPENMP_PHYSICS
	# pragma omp for
    #endif
	for(int i = 0; i < (int)cons_base.size(); i++)
		cons_base[i]->Setup();
		
	// 拘束力初期値による速度変化を計算 (dv = A * f)
    #ifdef USE_OPENMP_PHYSICS
	# pragma omp single
    #endif
	for(int i = 0; i < (int)cons_base.size(); i++){
		PHConstraintBase* con = cons_base[i];
		for(int n = 0; n < (int)con->axes.size(); n++){
			int j = con->axes[n];
			con->CompResponse(con->f[j], j);
		}
	}
}

void PHConstraintEngine::SetupCorrection(){
    #ifdef USE_OPENMP_PHYSICS
	# pragma omp for
    #endif
 	for(int i = 0; i < (int)cons_base.size(); i++)
		cons_base[i]->SetupCorrection();
}

void PHConstraintEngine::Iterate(){
	int n;
	for(n = 0; n < numIter; n++){
		int nupdated = 0;
        for(int i = 0; i < (int)cons_base.size(); i++)
			nupdated += (int)cons_base[i]->Iterate();

		if(nupdated == 0)
			break;
	}
}

void PHConstraintEngine::IterateCorrection(){
	for(int n = 0; n != numIterCorrection; ++n){
        int nupdated = 0;
        for(int i = 0; i < (int)cons_base.size(); i++)
			nupdated += cons_base[i]->IterateCorrection();

		if(nupdated == 0)
			break;
	}
}

void PHConstraintEngine::UpdateSolids(bool bVelOnly){
	double dt;

	// 速度の更新 (dtを渡すので並列化しない）
	dt = GetScene()->GetTimeStep();
	for(int i = 0; i < (int)bodies.size(); i++){
		PHSolid* s = bodies[i]->Cast();
		if (!s || s->IsArticulated()) continue;
		s->UpdateVelocity(&dt);
	}
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++)
		(*it)->UpdateJointVelocities(&dt);

	if(bVelOnly)
		return;

	// 位置の更新
	//# pragma omp for
	for(int i = 0; i < (int)bodies.size(); i++){
		PHSolid* s = bodies[i]->Cast();
		if(!s || s->IsArticulated())
			continue;
		s->UpdatePosition(dt);
	}
	//# pragma omp for
	for(int i = 0; i < (int)trees.size(); i++)
		trees[i]->UpdatePosition(dt);

	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++)
		(*it)->UpdateSolidVelocity(&dt);
}

void PHConstraintEngine::StepPart1(){
	#ifdef USE_OPENMP_PHYSICS
	# pragma omp single
    #endif
	{
		//交差を検知
		points.clear();

		PHSceneIf* scene = GetScene();
		if(scene->IsContactDetectionEnabled()){
			colcounter = 0;
			ptimer.CountUS();
			Detect(scene->GetCount(), scene->GetTimeStep(), scene->GetBroadPhaseMode(), scene->IsCCDEnabled());
			timeCollision = ptimer.CountUS();
			if (bReport) {
				DSTR << " col:" << timeCollision;
			}
			if (renderContact) UpdateContactInfoQueue();
		}
	}
}
void PHConstraintEngine::UpdateForStateDebug() {
	double dt = GetScene()->GetTimeStep();
	for (int i = 0; i < GetScene()->NSolids(); ++i) {
		PHSolid* solid = (PHSolid*)GetScene()->GetSolid(i);
		solid->UpdateCacheLCP(dt);
		if (solid->IsDynamical() && !solid->IsStationary() && !solid->IsFrozen()) {
			solid->dv0.v() = solid->minv * solid->f.v() * dt;
			solid->dv0.w() = solid->Iinv * (solid->f.w() - solid->v.w() % (solid->GetInertia() * solid->v.w())) * dt;
		}
		else {
			solid->dv0.clear();
		}
		solid->dv.clear();
		solid->dV.clear();
	}
	for (int i = 0; i < (int)points.size(); i++) points[i]->UpdateState();
	for (int i = 0; i < (int)joints.size(); i++) joints[i]->UpdateState();
	for (int i = 0; i < (int)cons_base.size(); i++) {
		cons_base[i]->ClearVars();
	}
	Setup();
}

void PHConstraintEngine::StepPart2(){
    #ifdef USE_OPENMP_PHYSICS
	# pragma omp single
    #endif
	{
		if(bReport)
			ptimer.CountUS();
	}

	double dt = GetScene()->GetTimeStep();

	// 前回のStep以降に別の要因によって剛体の位置・速度が変化した場合
	// ヤコビアン等の再計算
	// 各剛体の前処理
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp for
    #endif
	for (int i = 0; i < (int)bodies.size(); i++) {
		PHSolid* s = bodies[i]->Cast();
		if (s) s->UpdateCacheLCP(dt);
	}
    
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp for
    #endif
	for(int i = 0; i < (int)points.size(); i++)
		points[i]->UpdateState();
    
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp for
    #endif
	for (int i = 0; i < (int)joints.size(); i++) {
		joints[i]->UpdateState();
	}

	// 速度LCP
	Setup();

    #ifdef USE_OPENMP_PHYSICS
    # pragma omp single
    #endif
	{
		if(bReport){
			timeSetup = ptimer.CountUS();
			DSTR << " sup:" << timeSetup;
			ptimer.CountUS();
		}
	}

	#ifdef USE_OPENMP_PHYSICS
	# pragma omp single
    #endif
	{
		Iterate();
	}
	
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp single
    #endif
	{
		if(bReport){
			timeIterate = ptimer.CountUS();
			DSTR << " ite:" << timeIterate << std::endl;
		}
	}

	// 位置LCP
	SetupCorrection();

	#ifdef USE_OPENMP_PHYSICS
	# pragma omp single
    #endif
	{
		IterateCorrection();
	}

	// 位置・速度の更新
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp single
    #endif
	{
		UpdateSolids(!bUpdateAllState);
	}
}
	
void PHConstraintEngine::Step(){
    #ifdef USE_OPENMP_PHYSICS
	# pragma omp parallel
    #endif
	{
		StepPart1();	// 接触判定
		StepPart2();	// 拘束力計算，積分
	}
}

PHConstraintsIf* PHConstraintEngine::GetContactPoints(){
	return DCAST(PHConstraintsIf, &points);
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

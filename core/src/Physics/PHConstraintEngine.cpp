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

//coltime
int		coltimePhase1;
int		coltimePhase2;
int		coltimePhase3;
int		colcounter;
UTPreciseTimer *p_timer;

void PHSolidPairForLCP::OnDetect(PHShapePair* _sp, unsigned ct, double dt){
	PHShapePairForLCP* sp = (PHShapePairForLCP*)_sp;
	//	法線を求める
	sp->CalcNormal();
	//	交差する2つの凸形状を接触面で切った時の切り口の形を求める
	sp->EnumVertex(ct, solid[0], solid[1]);
}			

void PHSolidPairForLCP::OnContDetect(PHShapePair* _sp, unsigned ct, double dt){
	PHShapePairForLCP* sp = (PHShapePairForLCP*)_sp;

	//	交差する2つの凸形状を接触面で切った時の切り口の形を求める
	sp->EnumVertex(ct, solid[0], solid[1]);
	
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

PHConstraintEngine::PHConstraintEngine(){
	dfEps      = 1.0e-12;
	reportFile = 0;
}

PHConstraintEngine::~PHConstraintEngine(){
	EnableReport(false);
}

void PHConstraintEngine::EnableReport(bool on){
	if(!on && reportFile){
		fclose(reportFile);
		reportFile = nullptr;
	}
	if(on && !reportFile){
		reportFile = fopen(reportFilename, "w");
		if(reportFile)
			fprintf(reportFile, "col, sup, ite\n");
	}

	bReport = on;
	renderContact = true;

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
	if(find(solids.begin(), solids.end(), solid) == solids.end())
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
		
		// 拘束自由度の決定
		for(int i = 0; i < (int)cons_base.size(); i++)
			cons_base[i]->SetupAxisIndex();
		
		// 拘束間の A = J M^-1 J^T を計算
		CompResponseMatrix();
		
	} //< omp single

	// 拘束毎の前処理（J, b, db, dA, ...）
    #ifdef USE_OPENMP_PHYSICS
	# pragma omp for
    #endif
	for(int i = 0; i < (int)cons_base.size(); i++)
		cons_base[i]->Setup();
		
	// 拘束力初期値による速度変化を計算 (dv = A * f)
    #ifdef USE_OPENMP_PHYSICS
	# pragma omp for
    #endif
	for(int i = 0; i < (int)cons_base.size(); i++){
		PHConstraintBase* con = cons_base[i];
		for(int n = 0; n < (int)con->axes.size(); n++){
			int j = con->axes[n];
			con->CompResponseDirect(con->f[j], j);
		}
	}
}

inline double QuadForm(const double* v1, const double* M, const double* v2){
	double y = 0.0;
	int k = 0;
	for(int i = 0; i < 6; i++)for(int j = 0; j < 6; j++, k++)
		y += v1[i] * M[k] * v2[j];
	return y;
}

void PHConstraintEngine::CompResponseMatrix(){
	struct Aux{
		int i;	//< 拘束のインデックス
		int k;	//< solid[0] or solid[1]
		Aux(){}
		Aux(int _i, int _k):i(_i), k(_k){}
	};
	struct Auxs : vector<Aux>{
		int num;
		void Add(const Aux& a){
			at(num) = a;
			num++;
		}
	};
	static vector<Auxs>	solidMap;	//< 各剛体を拘束している拘束の配列
	static vector<Auxs>	treeMap;    //< 各ツリーに属する剛体を拘束している拘束の配列

	UTPreciseTimer p, pQ;
	int TQ = 0;
	p.CountUS();

	solidMap.resize(solids.size());
	treeMap .resize(trees .size());

	// 剛体とツリーにシリアル番号を振る
	for(int i = 0; i < (int)solids.size(); i++){
		solidMap[i].resize(2*cons.size());
		solidMap[i].num = 0;
		solids[i]->id = i;
	}
	for(int i = 0; i < (int)trees.size(); i++){
		treeMap[i].resize(2*cons.size());
		treeMap[i].num = 0;
		trees[i]->treeId = i;
	}

	// 拘束をつながっている剛体/ツリー別に分類
	for(int i = 0; i < (int)cons.size(); i++){
		PHConstraint* con = cons[i];
		for(int k = 0; k < 2; k++){
			PHSolid* s = con->solid[k];
			if(!s->IsDynamical())
				continue;
			if(s->IsArticulated())
				 treeMap [s->treeNode->root->treeId].Add(Aux(i, k));
			else solidMap[s->id]                    .Add(Aux(i, k));
		}
	}

	// A行列計算
	PHConstraint *con0, *con1;
	PHSolid      *s0,   *s1;
	SpatialMatrix A;

	vector<int> idx;
				
	p.CountUS();
	for(int i0 = 0; i0 < (int)cons.size(); i0++){
		if(idx.size() < cons.size())
			idx.resize(cons.size());
		con0 = cons[i0];
		con0->adj.num = 0;
		fill(idx.begin(), idx.end(), -1);

		for(int k0 = 0; k0 < 2; k0++){
			s0 = con0->solid[k0];
			if(!s0->IsDynamical())
				continue;

			Auxs* auxs;
			if(s0->IsArticulated())
				 auxs = &treeMap [s0->treeNode->root->treeId];
			else auxs = &solidMap[s0->id];
				
			for(int j = 0; j < auxs->num; j++){
				int i1 = (*auxs)[j].i;
				int k1 = (*auxs)[j].k;
				con1   = cons[i1];
				s1     = con1->solid[k1];

				const double* Minv;
				if(s0->IsArticulated())
				     Minv = (const double*)&s1->treeNode->dZdv_map[s0->treeNode->id];
				else Minv = (const double*)&s0->Minv;
				
				A.clear();
				for(int n0 = 0; n0 < con0->targetAxes.size(); n0++)for(int n1 = 0; n1 < con1->targetAxes.size(); n1++){
					int j0 = con0->targetAxes[n0];
					int j1 = con1->targetAxes[n1];
					const double* J0 = (const double*)&con0->J[k0].row(j0);
					const double* J1 = (const double*)&con1->J[k1].row(j1);
					A[j1][j0] = QuadForm(J1, Minv, J0);
					
					if(s0->IsArticulated())
						A[j1][j0] = -A[j1][j0];
				}
				
				if(idx[i1] == -1){
					con0->adj.Add(con1, A);
					idx[i1] = (int)(con0->adj.num-1);
				}
				else{
					con0->adj[idx[i1]].A += A;
				}
			}
		}		
	}

	// Aの対角成分とその逆数
	double Ad_eps    = /*1.0e-20;*/1.0e-10;		///< 対角成分の許容最小値
	double Ad_ratio  = /*1.0e-10;*/1.0e-5;		///< 対角成分の最小/最大の許容値
	double And_ratio = /*1.0e-10;*/1.0e-1;		///< 対角成分/非対角成分の許容値

	double And_max;			// 非対角成分の最大値
	double Ad_max  = 0.0;	// 対角成分の最大値

	for(int i0 = 0; i0 < (int)cons.size(); i0++){
		PHConstraint* con0 = cons[i0];
		for(int n0 = 0; n0 < con0->targetAxes.size(); n0++) {
			int j0 = con0->targetAxes[n0];
		
			And_max = 0.0;
				
			for(int i1 = 0; i1 < (int)con0->adj.num; i1++){
				PHConstraint::Adjacent& adj = con0->adj[i1];
				PHConstraint* con1 = adj.con;
				SpatialMatrix& A   = adj.A;
				for(int n1 = 0; n1 < con1->targetAxes.size(); n1++){
					int j1 = con1->targetAxes[n1];
		
					if(con0 == con1 && j0 == j1){
						con0->A[j0] = A[j1][j0];
					}
					else{
						And_max = std::max(And_max, std::abs(A[j1][j0]));
					}
				}
			}

			con0->A[j0] = std::max(con0->A[j0], Ad_eps);
			con0->A[j0] = std::max(con0->A[j0], And_ratio * And_max);
			
			Ad_max = std::max(Ad_max, con0->A[j0]);
		}
	}

	for(int i = 0; i < (int)cons.size(); i++){
		PHConstraint* con = cons[i];
		for(int n = 0; n < con->targetAxes.size(); n++) {
			int j = con->targetAxes[n];
			con->A[j] = std::max(con->A[j], Ad_ratio * Ad_max);
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
        #ifdef USE_OPENMP_PHYSICS
		# pragma omp for
        #endif
		for(int i = 0; i < (int)cons_base.size(); i++)
			nupdated += (int)cons_base[i]->Iterate();

		if(nupdated == 0)
			break;

		for(int i = 0; i < (int)cons_base.size(); i++){
			for(int j = 0; j < 6; j++){
				cons_base[i]->dv_changed[j] = cons_base[i]->dv_changed_next[j];
				cons_base[i]->dv_changed_next[j] = false;
			}
		}
	}
}

void PHConstraintEngine::IterateCorrection(){
	for(int n = 0; n != numIterCorrection; ++n){
        #ifdef USE_OPENMP_PHYSICS
		# pragma omp for
        #endif
		for(int i = 0; i < (int)cons_base.size(); i++)
			cons_base[i]->IterateCorrection();
	}
}

void PHConstraintEngine::UpdateSolids(bool bVelOnly){
	double dt;

	// 拘束力に対する速度変化を計算
	for(int i = 0; i < (int)cons_base.size(); i++){
		PHConstraintBase* con = cons_base[i];
		for(int n = 0; n < (int)con->axes.size(); n++){
			int j = con->axes[n];
			con->CompResponse(con->f[j], j);
		}
	}

	// 速度の更新 (dtを渡すので並列化しない）
	dt = GetScene()->GetTimeStep();
	for(int i = 0; i < (int)solids.size(); i++){
		if(solids[i]->IsArticulated())
			continue;
		solids[i]->UpdateVelocity(&dt);
	}
	for(PHRootNodes::iterator it = trees.begin(); it != trees.end(); it++)
		(*it)->UpdateVelocity(&dt);

	if(bVelOnly)
		return;

	// 位置の更新
	//# pragma omp for
	for(int i = 0; i < (int)solids.size(); i++){
		if(solids[i]->IsArticulated())
			continue;
		solids[i]->UpdatePosition(dt);
	}
	//# pragma omp for
	for(int i = 0; i < (int)trees.size(); i++)
		trees[i]->UpdatePosition(dt);

}

void PHConstraintEngine::StepPart1(){
    #ifdef USE_OPENMP_PHYSICS
	# pragma omp single
    #endif
	{
		//交差を検知
		points.clear();

		if(bReport)
			ptimer.CountUS();
			coltimePhase1 = 0;
			coltimePhase2 = 0;
			coltimePhase3 = 0;
			colcounter = 0;
			p_timer = &ptimer2;
		PHSceneIf* scene = GetScene();
		if(scene->IsContactDetectionEnabled()){
			Detect(scene->GetCount(), scene->GetTimeStep(), scene->GetBroadPhaseMode(), scene->IsCCDEnabled());
			if (renderContact) UpdateContactInfoQueue();
		}

		if(bReport){
			timeCollision = ptimer.CountUS();
			DSTR << " col:" << timeCollision;
		}
	}
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
	for(int i = 0; i < (int)solids.size(); i++)
		solids[i]->UpdateCacheLCP(dt);
    
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp for
    #endif
	for(int i = 0; i < (int)points.size(); i++)
		points[i]->UpdateState();
    
    #ifdef USE_OPENMP_PHYSICS
    # pragma omp for
    #endif
	for(int i = 0; i < (int)joints.size(); i++)
		joints[i]->UpdateState();

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

	Iterate();
	
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
	IterateCorrection();
	
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

	if(bReport && reportFile){
		fprintf(reportFile, "%d, %d, %d, %d\n", timeCollision, timeSetup, timeIterate,coltimePhase1);
	}
}

PHConstraintsIf* PHConstraintEngine::GetContactPoints(){
	return DCAST(PHConstraintsIf, &points);
}

//	state関係の実装
size_t PHConstraintEngine::GetStateSize() const{
	return PHContactDetector::GetStateSize() + (bSaveConstraints ? sizeof(PHConstraintsSt) : 0);
}
void PHConstraintEngine::ConstructState(void* m) const{
	PHContactDetector::ConstructState(m);
	if (bSaveConstraints){
		char* p = (char*)m;
		p += PHContactDetector::GetStateSize();
		new (p) PHConstraintsSt;
	}
}
void PHConstraintEngine::DestructState(void* m) const {
	PHContactDetector::DestructState(m);
	char* p = (char*)m;
	if (bSaveConstraints){
		p += PHContactDetector::GetStateSize();
		((PHConstraintsSt*)p)->~PHConstraintsSt();
	}
}
bool PHConstraintEngine::GetState(void* s) const {
	PHContactDetector::GetState(s);
	char* p = (char*)s;
	if (bSaveConstraints){
		PHConstraintsSt* st = (PHConstraintsSt*)(p + PHContactDetector::GetStateSize());
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
	PHContactDetector::SetState(s);
	char* p = (char*)s;
	if (bSaveConstraints){
		PHConstraintsSt* st = (PHConstraintsSt*)(p + PHContactDetector::GetStateSize());
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

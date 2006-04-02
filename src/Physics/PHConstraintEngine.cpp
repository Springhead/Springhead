#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <float.h>
#include <Collision/CDDetectorImp.h>
#include <Collision/CDQuickHull2D.h>
#include <Collision/CDQuickHull2DImp.h>
#include <Collision/CDCutRing.h>

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
//	PHSolidPair
void PHConstraintEngine::PHSolidPair::Init(PHSolidAux* s0, PHSolidAux* s1){
	solid[0] = s0;
	solid[1] = s1;
	int ns0 = solid[0]->solid->shapes.size(), ns1 = solid[1]->solid->shapes.size();
	shapePairs.resize(ns0, ns1);
	for(int i = 0; i < ns0; i++)for(int j = 0; j < ns1; j++){
		PHShapePair& sp = shapePairs.item(i, j);
		sp.shape[0] = solid[0]->solid->shapes[i];
		sp.shape[1] = solid[1]->solid->shapes[j];
	}
	bEnabled = true;
}

class ContactVertex: public Vec3d{
public:
	static Vec3d ex, ey;
	ContactVertex(){}
	ContactVertex(const Vec3d& v):Vec3d(v){}
	Vec2d GetPos(){
		return Vec2d( (*(Vec3d*)this)*ex, (*(Vec3d*)this)*ey );
	}
};
Vec3d ContactVertex::ex;
Vec3d ContactVertex::ey;

void PHShapePair::EnumVertex(PHConstraintEngine* engine, unsigned ct, PHSolidAux* solid0, PHSolidAux* solid1){
	//	center と normalが作る面と交差する面を求めないといけない．
	//	面の頂点が別の側にある面だけが対象．
	//	quick hull は n log r だから，線形時間で出来ることはやっておくべき．

	//	各3角形について，頂点がどちら側にあるか判定し，両側にあるものを対象とする．
	//	交線を，法線＋数値の形で表現する．
	//	この処理は凸形状が持っていて良い．
	//	＃交線の表現形式として，2次曲線も許す．その場合，直線は返さない
	//	＃2次曲線はMullar＆Preparataには入れないで別にしておく．
	CDConvex* conv[2] = { (CDConvex*)shape[0], (CDConvex*)shape[1], };


	//2Dへの変換がいる．どうする？
	//	適当に速度？
	Vec3d v0 = solid0->solid->GetPointVelocity(center);
	Vec3d v1 = solid1->solid->GetPointVelocity(center);
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
	if (local.det() < 0.99) {
		DSTR << "Error: local coordinate error." << std::endl;
		assert(0);
	}

	//	切り口を求める１：切り口を構成する線分の列挙
	CDCutRing cutRing(center, local);
	conv[0]->FindCutRing(cutRing, shapePoseW[0]);
	conv[1]->FindCutRing(cutRing, shapePoseW[1]);
	//	切り口を求める２：線分をつないで輪を作る
	cutRing.MakeRing();
//	cutRing.Print(DSTR);
//	DSTR << "contact center:" << center << " normal:" << normal << "  vtxs:" << std::endl;
	for(CDQHLine<CDCutLine>* vtx = cutRing.vtxs.begin; vtx!=cutRing.vtxs.end; ++vtx){
		if (vtx->deleted) continue;
		Vec3d pos;
		pos.sub_vector(1, Vec2d()) = vtx->normal / vtx->dist;
		pos = cutRing.local * pos;
		Matrix3d local;
		cutRing.local.Ori().ToMatrix(local);
		engine->points.push_back(DBG_NEW PHContactPoint(local, this, pos, solid0, solid1));
//		DSTR << "  " << pos << std::endl;
	}
}
//#define USE_VOLUME

#ifndef USE_VOLUME
bool PHShapePair::Detect(unsigned ct, PHSolidAux* solid0, PHSolidAux* solid1){
	UpdateShapePose(solid0->solid->GetPose() * shape[0]->GetPose(), solid1->solid->GetPose() * shape[1]->GetPose());
	CDConvex* conv[2] = { (CDConvex*)shape[0], (CDConvex*)shape[1], };
	Vec3d sep;
	bool r = FindCommonPoint(conv[0], conv[1], shapePoseW[0], shapePoseW[1], sep, closestPoint[0], closestPoint[1]);
	const double depthEpsilon = 0.01;
	if (r){
		commonPoint = shapePoseW[0] * closestPoint[0];
		if (lastContactCount == unsigned(ct-1)) state = CONTINUE;
		else{
			state = NEW;
			DSTR << "NEW" << endl;
		}
		lastContactCount = ct;

		//	法線を求める
		if (state == NEW){
			normal = solid0->solid->GetPointVelocity(commonPoint) - solid1->solid->GetPointVelocity(commonPoint);
			double norm = normal.norm();
			if (norm<1e-10){
				normal = solid1->solid->GetCenterPosition() - solid0->solid->GetCenterPosition();
				double norm = normal.norm();
				if (norm<1e-10){
					normal = Vec3d(1,0,0);
				}
			}
			normal.unitize();
			depth = depthEpsilon;
/*
			if (normal.y < 0.8){
				DSTR << std::endl;
				DSTR << (state == NEW ? "new" : "cont");
				DSTR << normal << std::endl;
			} 
*/
		}
		//	前回の法線の向きに動かして，最近傍点を求める
		if (depth < depthEpsilon) depth = depthEpsilon;
		Posed trans;
		Vec3f n;
		while(1) {
			depth *= 2;							//	余裕を見て，深さの2倍動かす
			trans = shapePoseW[1];				//	動かす行列
			trans.Pos() += depth * normal;
			FindClosestPoints(conv[0], conv[1], shapePoseW[0], trans, closestPoint[0], closestPoint[1]);
			center = shapePoseW[0] * closestPoint[0];
			n = trans * closestPoint[1] - center;
			if (n.square() > 1e-5) break;
		}
		depth = depth - n.norm();		//	動かした距離 - 2点の距離
		normal = n.unit();
		center -= 0.5f*depth*normal;
//		DSTR << "Normal:" << normal << " center:" << center << " depth:" << depth << std::endl;
		#ifdef _DEBUG
		if (!finite(normal.norm())){
			//DSTR << "Error: Wrong normal:" << normal << std::endl;
			//DSTR << trans;
			//DSTR << closestPoint[0] << closestPoint[1] << std::endl;
			FindClosestPoints(conv[0], conv[1], shapePoseW[0], trans, closestPoint[0], closestPoint[1]);
		}
		#endif
	}
	return r;
}
#else
bool PHShapePair::Detect(unsigned ct, PHSolidAux* solid0, PHSolidAux* solid1){
	return CDShapePair::Detect(ct);
}
#endif
//--------------------------------------------------------------------------
#ifndef USE_VOLUME	//	体積を使わない接触判定
bool PHConstraintEngine::PHSolidPair::Detect(PHConstraintEngine* engine){
	if(!bEnabled)return false;

	unsigned ct = OCAST(PHScene, engine->GetScene())->GetCount();
	// いずれかのSolidに形状が割り当てられていない場合は接触なし
	PHSolid *s0 = solid[0]->solid, *s1 = solid[1]->solid;
	if(s0->NShape() == 0 || s1->NShape() == 0) return false;

	// 全てのshape pairについて交差を調べる
	bool found = false;
	for(int i = 0; i < (int)(s0->shapes.size()); i++){
		for(int j = 0; j < (int)(s1->shapes.size()); j++){
			PHShapePair& sp = shapePairs.item(i, j);
			//このshape pairの交差判定/法線と接触の位置を求める．
			if(sp.Detect(ct, solid[0], solid[1])){
				found = true;
				//	交差する2つの凸形状を接触面で切った時の切り口の形を求める
				sp.EnumVertex(engine, ct, solid[0], solid[1]);
				//DSTR << sp.normal << endl;
			}
		}
	}
	return found;
}

//--------------------------------------------------------------------------
#else	//	体積を使う接触判定
bool PHConstraintEngine::PHSolidPair::Detect(PHConstraintEngine* engine){
	if(!bEnabled)return false;
	static CDContactAnalysis analyzer;

	unsigned ct = OCAST(PHScene, engine->GetScene())->GetCount();
	
	// いずれかのSolidに形状が割り当てられていない場合はエラー
	if(solid[0]->solid->NShape() == 0 || solid[1]->solid->NShape() == 0)
		return false;

	// 全てのshape pairについて交差を調べる
	bool found = false;
	for(int i = 0; i < (int)(solid[0]->solid->shapes.size()); i++)for(int j = 0; j < (int)(solid[1]->solid->shapes.size()); j++){
		PHShapePair& sp = shapePairs.item(i, j);
		sp.UpdateShapePose(solid[0]->solid->GetPose(), solid[1]->solid->GetPose());

		if(sp.Detect(ct, solid[0], solid[1])){
			found = true;
			analyzer.FindIntersection(&sp);			//交差形状の計算
			analyzer.CalcNormal(&sp);				//交差の法線と中心を得る

			//接触点の作成：
			//交差形状を構成する頂点はanalyzer.planes.beginからendまでの内deleted==falseのもの
			typedef CDQHPlanes<CDContactAnalysisFace>::CDQHPlane Plane;
			static std::vector<ContactVertex> isVtxs;
			isVtxs.clear();
			for(Plane* p = analyzer.planes.begin; p != analyzer.planes.end; p++){
				if(p->deleted) continue;
				isVtxs.push_back(p->normal / p->dist);
			}
			ContactVertex::ex = (-0.1 < sp.normal.z && sp.normal.z < 0.1) ?
				sp.normal ^ Vec3f(0,0,1) : sp.normal ^ Vec3f(1,0,0);
			ContactVertex::ex.unitize();
			ContactVertex::ey = sp.normal ^ ContactVertex::ex;

			//	すべての接触点を含む最小の凸多角形
			static CDQHLines<ContactVertex> supportConvex(1000);
			supportConvex.Clear();
			supportConvex.epsilon = 0.01f;

			static std::vector<ContactVertex*> isVtxPtrs;
			isVtxPtrs.clear();
			isVtxPtrs.resize(isVtxs.size());
			for(size_t i=0; i<isVtxPtrs.size(); ++i) isVtxPtrs[i] = &isVtxs[i];
			supportConvex.CreateConvexHull(&isVtxPtrs.front(), &isVtxPtrs.back()+1);
			
			typedef CDQHLines<ContactVertex>::CDQHLine Line;
//#define DEBUG_CONTACTOUT
#ifdef DEBUG_CONTACTOUT
			int n = engine->points.size();
#endif
			for(Line* l = supportConvex.begin; l!=supportConvex.end; ++l){
				//if (l->deleted) continue;
				Vec3d v = *l->vtx[0]+sp.commonPoint;
				engine->points.push_back(DBG_NEW PHContactPoint(&sp, v, solid[0], solid[1]));
			}
#ifdef DEBUG_CONTACTOUT
			//DSTR << engine->points.size()-n << " contacts:";
			for(unsigned i=n; i<engine->points.size(); ++i){
				//DSTR << engine->points[i].pos;
			}
			//DSTR << std::endl;
#endif
		}
	}
	return found;
}
#endif

//----------------------------------------------------------------------------
// PHConstraintEngine
OBJECTIMP(PHConstraintEngine, PHEngine);

PHConstraintEngine::PHConstraintEngine(){
	max_iter_dynamics = 10;//5;
	max_iter_correction = 20;//6;
	//step_size = 1.0;
	//converge_criteria = 0.00000001;
	max_error = 1.0;
}

PHConstraintEngine::~PHConstraintEngine(){

}

void PHConstraintEngine::Clear(PHScene* s){
	points.clear();
	joints.clear();
	solidPairs.clear();
	solids.clear();
}

void PHConstraintEngine::Add(PHSolid* s){
	if(solids.Find(s) == solids.end()){
		solids.push_back(DBG_NEW PHSolidAux(s));
		
		int N = solids.size();
		assert(solidPairs.height() == N-1 && solidPairs.width() == N-1);
		solidPairs.resize(N, N);
		for(int i = 0; i < N-1; i++)
			solidPairs.item(i, N-1).Init(solids[i], solids[N-1]);
	}
}

void PHConstraintEngine::Remove(PHSolid* s){
	PHSolidAuxs::iterator is = solids.Find(s);
	if(is != solids.end()){
		int idx = is - solids.begin();
		solids.erase(is);
		solidPairs.erase_row(idx);
		solidPairs.erase_col(idx);
	}
}

PHJoint* PHConstraintEngine::AddJoint(PHSolid* lhs, PHSolid* rhs, const PHJointDesc& desc){
	PHSolidAuxs::iterator islhs, isrhs;
	islhs = solids.Find(lhs);
	isrhs = solids.Find(rhs);
	if(islhs == solids.end() || isrhs == solids.end())
		return NULL;
	
	PHJoint* joint = NULL;
	switch(desc.type){
	case PHJointDesc::JOINT_HINGE:
		joint = DBG_NEW PHHingeJoint();
		joint->Init(*islhs, *isrhs, desc);
		break;
	}
	joints.push_back(joint);

	//関節でつなげられた剛体間の接触は無効化
	//EnableContact(lhs, rhs, false);

	return joint;
}

// solidPairs, shapePairsの更新処理
//	solid  : AddShapeが呼ばれたSolid. 新規追加されたShapeはsolid->shapes.back()で取得されるもの.
void PHConstraintEngine::UpdateShapePairs(PHSolid* solid){
	int isolid = (solids.Find(solid) - solids.begin());
	int i, j;
	for(i = 0; i < isolid; i++){
		PHSolidPair& sp = solidPairs.item(i, isolid);
		PHSolid* slhs = sp.solid[0]->solid;
		sp.shapePairs.resize(sp.shapePairs.height(), solid->NShape());
		for(j = 0; j < slhs->NShape(); j++){
			sp.shapePairs.item(j, solid->NShape()-1).shape[0] = slhs->shapes[j];
			sp.shapePairs.item(j, solid->NShape()-1).shape[1] = solid->shapes.back();
		}
	}
	for(i = isolid+1; i < (int)solids.size(); i++){
		PHSolidPair& sp = solidPairs.item(isolid, i);
		PHSolid* srhs = sp.solid[1]->solid;
		sp.shapePairs.resize(solid->NShape(), sp.shapePairs.width());
		for(j = 0; j < srhs->NShape(); j++){
			sp.shapePairs.item(solid->NShape()-1, j).shape[0] = solid->shapes.back();
			sp.shapePairs.item(solid->NShape()-1, j).shape[1] = srhs->shapes[j];
		}
	}
}

void PHConstraintEngine::EnableContact(PHSolid* lhs, PHSolid* rhs, bool bEnable){
	PHSolidAuxs::iterator ilhs = solids.Find(lhs), irhs = solids.Find(rhs);
	if(ilhs == solids.end() || irhs == solids.end())
		return;
	int i = ilhs - solids.begin(), j = irhs - solids.begin();
	if(i > j)swap(i, j);
	assert(i < solidPairs.height() && j < solidPairs.width());
	solidPairs.item(i, j).bEnabled = bEnable;
}

// AABBでソートするための構造体
struct Edge{
	float edge;				///<	端の位置(グローバル系)
	int	index;				///<	元の solidの位置
	bool bMin;				///<	初端ならtrue
	bool operator < (const Edge& s) const { return edge < s.edge; }
};
typedef std::vector<Edge> Edges;

bool PHConstraintEngine::DetectPenetration(){
	/* 以下の流れで交差を求める
		1. SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外
		2. 各Solidの組について
			2a. ShapeのBBoxレベルでの交差判定 (未実装)
			2b. 各Shapeの組についてGJKで交差形状を得る
			2c. 交差形状から法線を求め、法線に関して形状を射影し，その頂点を接触点とする
			2d. 得られた接触点情報をPHContactPointsに詰めていく
	 */
	points.clear();
	int N = solids.size();

	//1. BBoxレベルの衝突判定
	Vec3f dir(0,0,1);
	Edges edges;
	edges.resize(2 * N);
	Edges::iterator eit = edges.begin();
	for(int i = 0; i < N; ++i){
		solids[i]->solid->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		eit[0].index = i; eit[0].bMin = true;
		eit[1].index = i; eit[1].bMin = false;
		eit += 2;
	}
	std::sort(edges.begin(), edges.end());
	//端から見ていって，接触の可能性があるノードの判定をする．
	typedef std::set<int> SolidSet;
	SolidSet cur;							//	現在のSolidのセット
	bool found = false;
	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	初端だったら，リスト内の物体と判定
			for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->index;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				//2. SolidとSolidの衝突判定
				found |= solidPairs.item(f1, f2).Detect(this);
			}
			cur.insert(it->index);
		}else{
			cur.erase(it->index);			//	終端なので削除．
		}
	}
	return found;
}
void PHConstraintEngine::SetupDynamics(double dt){
	solids.SetupDynamics(dt);
	points.SetupDynamics(dt);
	joints.SetupDynamics(dt);
}
void PHConstraintEngine::SetupCorrection(double dt){
	solids.SetupCorrection();
	points.SetupCorrection(dt, max_error);
	joints.SetupCorrection(dt, max_error);
}
void PHConstraintEngine::IterateDynamics(){
	double dfsum = 0.0;
	int count = 0;
	while(true){
		if(count == max_iter_dynamics){
			//DSTR << "max count." << " iteration count: " << count << " dfsum: " << dfsum << endl;
			break;
		}
		points.IterateDynamics();
		joints.IterateDynamics();

		count++;
		//終了判定
		/*if(dfsum < converge_criteria){
			DSTR << "converged." << " iteration count: " << count << " dfsum: " << dfsum << endl;
			break;
		}*/
	}
}
void PHConstraintEngine::IterateCorrection(){
	double dFsum = 0.0;
	int count = 0;
	while(true){
		if(count == max_iter_correction){
			//DSTR << "max count." << " iteration count: " << count << " dFsum: " << dFsum << endl;
			break;
		}
		dFsum = 0.0;
		
		points.IterateCorrection();
		joints.IterateCorrection();

		count++;
		//終了判定
		/*if(dFsum < converge_criteria){
			DSTR << "converged." << " iteration count: " << count << " dFsum: " << dFsum << endl;
			break;
		}*/
	}
}

void PHConstraintEngine::UpdateSolids(double dt){
	PHSolidAuxs::iterator is;
	PHSolidAux* aux;
	PHSolid* solid;
	Vec3d vnew, wnew;
	for(is = solids.begin(); is != solids.end(); is++){
		aux = *is;
        solid = aux->solid;
		//velocity update
		vnew = aux->v + aux->dv0 + aux->dv;
		wnew = aux->w + aux->dw0 + aux->dw;
		solid->SetVelocity       (solid->GetOrientation() * vnew);
		solid->SetAngularVelocity(solid->GetOrientation() * wnew);
		//position update
		solid->SetCenterPosition(solid->GetCenterPosition() + solid->GetVelocity() * dt + solid->GetOrientation() * aux->dV);
		solid->SetOrientation(
			(solid->GetOrientation() * Quaterniond::Rot(wnew * dt + aux->dW)).unit()
		);
		//solid->SetOrientation((solid->GetOrientation() + solid->GetOrientation().Derivative(solid->GetOrientation() * is->dW)).unit());
		solid->SetOrientation((solid->GetOrientation() * Quaterniond::Rot(/*solid->GetOrientation() * */aux->dW)).unit());
		solid->SetUpdated(true);
	}
}

//#include <windows.h>

void PHConstraintEngine::Step(){
	//LARGE_INTEGER freq, val[2];
	//QueryPerformanceFrequency(&freq);
	
	//交差を検知
	//QueryPerformanceCounter(&val[0]);
	DetectPenetration();
	//QueryPerformanceCounter(&val[1]);
	//DSTR << "cd " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;

	double dt = OCAST(PHScene, GetScene())->GetTimeStep();

	//QueryPerformanceCounter(&val[0]);
	SetupDynamics(dt);
	//QueryPerformanceCounter(&val[1]);
	//DSTR << "sd " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;

	//QueryPerformanceCounter(&val[0]);
	IterateDynamics();
	//DSTR << points.size() << endl;
	//for(PHConstraints::iterator it = points.begin(); it != points.end(); it++)
	//	DSTR << (*it)->fv << (*it)->fw << endl;
	//QueryPerformanceCounter(&val[1]);
	//DSTR << "id " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;

	//QueryPerformanceCounter(&val[0]);
	SetupCorrection(dt);
	//QueryPerformanceCounter(&val[1]);
	//DSTR << "sc " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;

	//QueryPerformanceCounter(&val[0]);
	IterateCorrection();
	//QueryPerformanceCounter(&val[1]);
	//DSTR << "ic " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;

	UpdateSolids(dt);

}

#undef SUBMAT
#undef SUBVEC

}


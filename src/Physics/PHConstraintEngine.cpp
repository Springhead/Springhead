#include "Physics.h"
#pragma hdrstop
#include <float.h>
#include <Collision/CDDetectorImp.h>
#include <Collision/CDQuickHull2D.h>
#include <Collision/CDQuickHull2DImp.h>


using namespace PTM;
using namespace std;
namespace Spr{;

// AABBでソートするための構造体
struct Edge{
	float edge;				///<	端の位置(グローバル系)
	int	index;				///<	元の solidの位置
	bool bMin;				///<	初端ならtrue
	bool operator < (const Edge& s) const { return edge < s.edge; }
};
typedef std::vector<Edge> Edges;

//----------------------------------------------------------------------------
// PHContact

//----------------------------------------------------------------------------
// PHConstraintEngine

//----------------------------------------------------------------------------
//	PHSolidPair

void PHConstraintEngine::PHSolidPair::Init(PHSolid* s0, PHSolid* s1){
	int ns0 = s0->shapes.size(), ns1 = s1->shapes.size();
	shapePairs.resize(ns0, ns1);
	for(int i = 0; i < ns0; i++)for(int j = 0; j < ns1; j++){
		CDShapePair& sp = shapePairs.item(i, j);
		sp.shape[0] = s0->shapes[i];
		sp.shape[1] = s1->shapes[j];
	}
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

bool PHConstraintEngine::PHSolidPair::Detect(int is0, int is1, PHConstraintEngine* engine){
	// ＊ここでShapeについてBBoxレベル判定をすれば速くなるかも？
	static CDContactAnalysis analyzer;

	unsigned ct = OCAST(PHScene, engine->GetScene())->GetCount();
	
	// いずれかのSolidに形状が割り当てられていない場合はエラー
	PHSolid *s0 = engine->solids[is0], *s1 = engine->solids[is1];
	if(s0->NShape() == 0 || s1->NShape() == 0)
		return false;

	// 全てのshape pairについて交差を調べる
	bool found = false;
	for(int i = 0; i < (int)(s0->shapes.size()); i++)for(int j = 0; j < (int)(s1->shapes.size()); j++){
		CDShapePair& sp = shapePairs.item(i, j);
		sp.UpdateShapePose(s0->GetPose(), s1->GetPose());
		//このshape pairの交差判定
		if(sp.Detect(ct)){
			found = true;
			//交差形状の計算
			analyzer.FindIntersection(&sp);
			//交差の法線と中心を得る
			analyzer.CalcNormal(&sp);

			//接触を作成
			PHContact con;
			con.shape[0] = i;
			con.shape[1] = j;
			con.solid[0] = is0;
			con.solid[1] = is1;
			con.normal = sp.normal;
			con.depth = sp.depth;
			//摩擦係数は両者の静止摩擦係数の平均とする
			con.mu = (sp.shape[0]->material.mu0 + sp.shape[1]->material.mu0) * 0.5;
			
			//接触点の作成：
			//交差形状を構成する頂点はanalyzer.planes.beginからendまでの内deleted==falseのもの
			typedef CDQHPlanes<CDContactAnalysisFace>::CDQHPlane Plane;
			static std::vector<ContactVertex> isVtxs;
			isVtxs.clear();
			for(Plane* p = analyzer.planes.begin; p != analyzer.planes.end; p++){
				if(p->deleted) continue;
				isVtxs.push_back(p->normal / p->dist);
			}
			ContactVertex::ex = (-0.1<con.normal.z && con.normal.z < 0.1) ?
				con.normal ^ Vec3f(0,0,1) : con.normal ^ Vec3f(1,0,0);
			ContactVertex::ex.unitize();
			ContactVertex::ey = con.normal ^ ContactVertex::ex;

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
				engine->points.push_back(PHContactPoint(engine->contacts.size(), v));
			}
#ifdef DEBUG_CONTACTOUT
			DSTR << engine->points.size()-n << " contacts:";
			for(unsigned i=n; i<engine->points.size(); ++i){
				DSTR << engine->points[i].pos;
			}
			DSTR << std::endl;
#endif
			engine->contacts.push_back(con);
		}
	}
	return found;
}

//----------------------------------------------------------------------------

OBJECTIMP(PHConstraintEngine, PHEngine);

PHConstraintEngine::PHConstraintEngine(){
	ready = false;
	max_iter_dynamics = 10;
	max_iter_correction = 10;
	step_size = 1.0;
	converge_criteria = 0.00000001;
}

PHConstraintEngine::~PHConstraintEngine(){

}

void PHConstraintEngine::Add(PHSolid* s){
	if(solids.Find(s) == 0){
		solids.push_back(s);
		Invalidate();
	}
}

void PHConstraintEngine::Remove(PHSolid* s){
	if(solids.Erase(s))
		Invalidate();
}

void PHConstraintEngine::Init(){
	int N = solids.size();

	solidAuxs.resize(N);
	for(int i = 0; i < N; i++){
		solidAuxs[i].minv = solids[i]->GetMassInv();
		solidAuxs[i].Iinv = solids[i]->GetInertiaInv();
	}

	//登録されているSolidの数に合わせてsolidPairsとshapePairsをresize
	solidPairs.resize(N, N);
	for(int i = 0; i < N; i++)for(int j = i+1; j < N; j++){
		PHSolidPair& sp = solidPairs.item(i, j);
		sp.Init(solids[i], solids[j]);
	}

	ready = true;
}

bool PHConstraintEngine::Detect(){
	/* 以下の流れで交差を求める
		1. SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外
		2. 各Solidの組について
			2a. ShapeのBBoxレベルでの交差判定 (未実装)
			2b. 各Shapeの組についてGJKで交差形状を得る
			2c. 交差形状から法線を求め、法線に関して形状を射影し，その頂点を接触点とする
			2d. 得られた接触点情報をPHContactPointsに詰めていく
	 */

	contacts.clear();
	points.clear();
	int N = solids.size();

	//1. BBoxレベルの衝突判定
	Vec3f dir(0,0,1);
	Edges edges;
	edges.resize(2 * N);
	Edges::iterator eit = edges.begin();
	for(int i = 0; i < N; ++i){
		solids[i]->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
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
				found |= solidPairs.item(f1, f2).Detect(f1, f2, this);
			}
			cur.insert(it->index);
		}else{
			cur.erase(it->index);			//	終端なので削除．
		}
	}
	return found;
}

/*void PHConstraintEngine::PrintContacts(){
	PHContactPoints::iterator ip;
	int icon = -1;
	for(ip = points.begin(); ip != points.end(); ip++){
		if(icon != ip->contact){
			icon = ip->contact;
			PHContact& con = contacts[icon];
			DSTR << "contact: " << icon << " normal: " << con.normal << " center: " << con.center << endl;
		}
		DSTR << "point: " << ip->pos <<
			" normal: " << ip->Jlin[0].row(0) << "tangent0: " << ip->Jlin[0].row(1) << endl;
	}
}*/

//LCP構築
void PHConstraintEngine::SetupDynamics(double dt){
	//各Solidに関係する変数
	{
		Quaterniond q;
		Vec3d v, w, f, t;
		for(int i = 0; i < (int)(solids.size()); i++){
			q = solids[i]->GetOrientation();
			v = q.Conjugated() * solids[i]->GetVelocity();
			w = q.Conjugated() * solids[i]->GetAngularVelocity();
			f = q.Conjugated() * solids[i]->nextForce;
			t = q.Conjugated() * solids[i]->nextTorque;
			solidAuxs[i].Vlin0 = v + solidAuxs[i].minv * f * dt;
			solidAuxs[i].Vang0 = w + solidAuxs[i].Iinv * (t - w % (solids[i]->GetInertia() * w)) * dt;
			solidAuxs[i].dVlin.clear();
			solidAuxs[i].dVang.clear();
		}
	}
	//各Contactに関係する変数
	Vec3d n, p, r[2], v[2], vrel, vrelproj, t[2];
	Matrix3d rcross[2], R[2];
	Posed q[2];
	PHSolid* solid[2];
	PHSolidAux* solidaux[2];
	int icon = -1;
	for(PHContactPoints::iterator ip = points.begin(); ip != points.end(); ip++){
		//接触のインデックスを必要なら更新
		if(icon != ip->contact){
			icon = ip->contact;
			PHContact& con = contacts[icon];
			n = con.normal;	//法線
			for(int i = 0; i < 2; i++){
				solid[i]    =  solids[con.solid[i]];
				solidaux[i] = &solidAuxs[con.solid[i]];
				q[i] = solid[i]->GetPose();
				q[i].Ori().ToMatrix(R[i]);
			}
		}

		for(int i = 0; i < 2; i++){
			r[i] = ip->pos - q[i].Pos();	//剛体の中心から接触点までのベクトル
			rcross[i] = Matrix3d::Cross(r[i]);
			v[i] = solid[i]->GetVelocity() + solid[i]->GetAngularVelocity() % r[i];	//接触点での速度
		}
		//接線ベクトルt[0], t[1]
		// *t[0]は相対速度ベクトルに平行になるようにする(といいらしい)
		vrel = v[1] - v[0];
		vrelproj = vrel - (n * vrel) * n;	//相対速度ベクトルを法線に直交する平面に射影したベクトル
		double vrelproj_norm = vrelproj.norm();
		if(vrelproj_norm < 1.0e-10){
			t[0] = n % Vec3d(1.0, 0.0, 0.0);	
			if(t[0].norm() < 1.0e-10)
				t[0] = n % Vec3d(0.0, 1.0, 0.0);
			t[0].unitize();
		}
		else{
			t[0] = vrelproj / vrelproj_norm;
		}
		t[1] = t[0] % n;
		for(int i = 0; i < 2; i++){
			// J行列
			ip->Jlin[i].row(0) = n;
			ip->Jlin[i].row(1) = t[0];
			ip->Jlin[i].row(2) = t[1];
			ip->Jang[i] = ip->Jlin[i] * (-rcross[i]);
			ip->Jlin[i] = ip->Jlin[i] * solid[i]->GetRotation();
			ip->Jang[i] = ip->Jang[i] * solid[i]->GetRotation();
			if(i == 0){
				ip->Jlin[i] *= -1.0;
				ip->Jang[i] *= -1.0;
			}
			// T行列
			ip->Tlin[i] = solidaux[i]->minv * ip->Jlin[i].trans();
			ip->Tang[i] = solidaux[i]->Iinv * ip->Jang[i].trans();
		}
		// A行列
		ip->A = ip->Jlin[0] * ip->Tlin[0] + ip->Jang[0] * ip->Tang[0] +
				ip->Jlin[1] * ip->Tlin[1] + ip->Jang[1] * ip->Tang[1];
		ip->Ainv = ip->A.inv();
		// bベクトル
		ip->b = ip->Jlin[0] * (solidaux[0]->Vlin0) + 
				ip->Jang[0] * (solidaux[0]->Vang0) +
				ip->Jlin[1] * (solidaux[1]->Vlin0) +
				ip->Jang[1] * (solidaux[1]->Vang0);
		ip->b = ip->Ainv * ip->b;
		ip->Jlin[0] = ip->Ainv * ip->Jlin[0];
		ip->Jang[0] = ip->Ainv * ip->Jang[0];
		ip->Jlin[1] = ip->Ainv * ip->Jlin[1];
		ip->Jang[1] = ip->Ainv * ip->Jang[1];
		ip->f.clear();
		// Jlin, Jang, bをAの対角要素で割る
		/*double diag_inv;
		for(int i = 0; i < 3; i++){
			//0割りチェックは？
			diag_inv = 1.0 / ip->A[i][i];
			ip->b[i] *= diag_inv;
			ip->Jlin[0].row(i) *= diag_inv;
			ip->Jlin[1].row(i) *= diag_inv;
			ip->Jang[0].row(i) *= diag_inv;
			ip->Jang[1].row(i) *= diag_inv;
		}*/
	}
}

void PHConstraintEngine::SetupCorrection(){
	PHContactPoints::iterator ip;
	PHContact* con;
	PHSolidAux* solidaux[2];
	Vec3d Vlin[2], Vang[2];
	//Dynamicsの影響を考慮した上での各接触点での交差深度
	int icon = -1;
	for(ip = points.begin(); ip != points.end(); ip++){
		if(icon != ip->contact){
			icon = ip->contact;
			con = &contacts[icon];
			for(int i = 0; i < 2; i++){
				solidaux[i] = &solidAuxs[con->solid[i]];
				Vlin[i] = solidaux[i]->Vlin0 + solidaux[i]->dVlin;
				Vang[i] = solidaux[i]->Vang0 + solidaux[i]->dVang;
			}
		}
		ip->B = -con->depth +
			ip->Jlin[0].row(0) * Vlin[0] + ip->Jang[0].row(0) * Vang[0] + ip->Jlin[1].row(0) * Vlin[1] + ip->Jang[1].row(0) * Vang[1];
		ip->B *= (0.1 / ip->A[0][0]);
		ip->F = 0.0;
		//ip->b = ip->Ainv * ip->b;
	}
	for(PHSolidAuxs::iterator is = solidAuxs.begin(); is != solidAuxs.end(); is++){
		is->dVlin.clear();
		is->dVang.clear();
	}
}

void PHConstraintEngine::IterateDynamics(){
	PHContactPoints::iterator ip;
	PHContact* con;
	PHSolidAux* solidaux[2];
	Vec3d fnew;
	double dfsum;
	int count = 0;
	while(true){
		if(count == max_iter_dynamics){
			DSTR << "max count." << " iteration count: " << count << " dfsum: " << dfsum << endl;
			break;
		}
		dfsum = 0.0;
		//反復
		//接触力fの更新
		int icon = -1;
		for(ip = points.begin(); ip != points.end(); ip++){
			if(icon != ip->contact){
				icon = ip->contact;
				con = &contacts[icon];
				for(int i = 0; i < 2; i++)
					solidaux[i] = &solidAuxs[con->solid[i]];
			}

			fnew = ip->f - step_size * (ip->b +
				ip->Jlin[0] * solidaux[0]->dVlin + ip->Jang[0] * solidaux[0]->dVang +
				ip->Jlin[1] * solidaux[1]->dVlin + ip->Jang[1] * solidaux[1]->dVang);

			//垂直抗力 >= 0の制約
			fnew[0] = Spr::max(0.0, fnew[0]);
			
			//|摩擦力| <= 最大静止摩擦の制約
			//	・摩擦力の各成分が最大静止摩擦よりも小さくても合力は超える可能性があるので本当はおかしい。
			//	・静止摩擦と動摩擦が同じ値でないと扱えない。
			double flim = con->mu * fnew[0];		//最大静止摩擦
			fnew[1] = Spr::min(Spr::max(-flim, fnew[1]), flim);
			fnew[2] = Spr::min(Spr::max(-flim, fnew[2]), flim);		

			ip->df = fnew - ip->f;
			dfsum += ip->df.square();
			ip->f = fnew;
			
			solidaux[0]->dVlin += (ip->Tlin[0] * ip->df);
			solidaux[0]->dVang += (ip->Tang[0] * ip->df);
			solidaux[1]->dVlin += (ip->Tlin[1] * ip->df);
			solidaux[1]->dVang += (ip->Tang[1] * ip->df);

		}
		count++;
		//終了判定
		if(dfsum < converge_criteria){
			DSTR << "converged." << " iteration count: " << count << " dfsum: " << dfsum << endl;
			break;
		}
	}
}

void PHConstraintEngine::IterateCorrection(){
	PHContactPoints::iterator ip;
	PHContact* con;
	PHSolidAux* solidaux[2];
	double Fnew, dFsum;
	int count = 0;
	while(true){
		if(count == max_iter_correction){
			DSTR << "max count." << " iteration count: " << count << " dFsum: " << dFsum << endl;
			break;
		}
		dFsum = 0.0;
		
		int icon = -1;
		for(ip = points.begin(); ip != points.end(); ip++){
			if(icon != ip->contact){
				icon = ip->contact;
				con = &contacts[icon];
				for(int i = 0; i < 2; i++)
					solidaux[i] = &solidAuxs[con->solid[i]];
			}

			Fnew = ip->F - step_size * (ip->B +
				ip->Jlin[0].row(0) * solidaux[0]->dVlin + ip->Jang[0].row(0) * solidaux[0]->dVang +
				ip->Jlin[1].row(0) * solidaux[1]->dVlin + ip->Jang[1].row(0) * solidaux[1]->dVang);

			//垂直抗力 >= 0の制約
			Fnew = Spr::max(0.0, Fnew);
			
			ip->dF = Fnew - ip->F;
			dFsum += ip->dF * ip->dF;
			ip->F = Fnew;
			
			solidaux[0]->dVlin += (ip->Tlin[0].col(0) * ip->dF);
			solidaux[0]->dVang += (ip->Tang[0].col(0) * ip->dF);
			solidaux[1]->dVlin += (ip->Tlin[1].col(0) * ip->dF);
			solidaux[1]->dVang += (ip->Tang[1].col(0) * ip->dF);

		}
		count++;
		//終了判定
		if(dFsum < converge_criteria){
			DSTR << "converged." << " iteration count: " << count << " dFsum: " << dFsum << endl;
			break;
		}
	}
}

void PHConstraintEngine::UpdateDynamics(double dt){
	PHSolids::iterator is;
	PHSolidAuxs::iterator isaux;
	for(is = solids.begin(), isaux = solidAuxs.begin(); is != solids.end(); is++, isaux++){
		PHSolid* s = *is;
		//velocity update
		s->SetVelocity       (s->GetOrientation() * (isaux->Vlin0 + isaux->dVlin));
		s->SetAngularVelocity(s->GetOrientation() * (isaux->Vang0 + isaux->dVang));
		//position update
		s->SetCenterPosition(s->GetCenterPosition() + s->GetVelocity() * dt);
		s->SetOrientation(s->GetOrientation() + s->GetOrientation().Derivative(s->GetAngularVelocity()) * dt);

		DSTR << s->GetVelocity() << " ; " << s->GetCenterPosition() << endl;

		(*is)->SetUpdated(true);
	}
}

void PHConstraintEngine::UpdateCorrection(){
	PHSolids::iterator is;
	PHSolidAuxs::iterator isaux;
	for(is = solids.begin(), isaux = solidAuxs.begin(); is != solids.end(); is++, isaux++){
		PHSolid* s = *is;
		//position update
		s->SetCenterPosition(s->GetCenterPosition() + s->GetOrientation() * isaux->dVlin);
		s->SetOrientation(s->GetOrientation() + s->GetOrientation().Derivative(s->GetOrientation() * isaux->dVang));

		(*is)->SetUpdated(true);
	}
}

void PHConstraintEngine::Step(){
	if(!ready)
		Init();

	//交差を検知
	if(!Detect())
		return;

	double dt = OCAST(PHScene, GetScene())->GetTimeStep();

	//PrintContacts();

	DSTR << "dynamics: " << endl;
	SetupDynamics(dt);
	IterateDynamics();
	UpdateDynamics(dt);
	
	DSTR << "correction: " << endl;
	SetupCorrection();
	IterateCorrection();
	UpdateCorrection();

}

}

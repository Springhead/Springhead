#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <float.h>
#include <Collision/CDDetectorImp.h>
#include <Collision/CDQuickHull2D.h>
#include <Collision/CDQuickHull2DImp.h>

using namespace PTM;
using namespace std;
namespace Spr{;

#define SUBMAT(r, c, h, w) sub_matrix(TSubMatrixDim<r, c, h, w>())
#define SUBVEC(o, l) sub_vector(TSubVectorDim<o, l>())

inline double rowtimes(const Matrix3d& M, int k, const Vec3d& v){
	return M.item(k, 0) * v[0] + M.item(k, 1) * v[1] + M.item(k, 2) * v[2];
}

//----------------------------------------------------------------------------
// PHSolidAux
void PHSolidAuxs::Init(){
	for(iterator it = begin(); it != end(); it++){
		PHSolidAux* s = *it;
		s->minv = s->solid->GetMassInv();
		s->Iinv = s->solid->GetInertiaInv();
	}
}
void PHSolidAux::SetupDynamics(double dt){
	Quaterniond q;
	Vec3d v, w, f, t;
	q = solid->GetOrientation();
	v = q.Conjugated() * solid->GetVelocity();
	w = q.Conjugated() * solid->GetAngularVelocity();
	if(solid->IsDynamical()){
		f = q.Conjugated() * solid->nextForce;
		t = q.Conjugated() * solid->nextTorque;
		v0 = v + minv * f * dt;
		w0 = w + Iinv * (t - w % (solid->GetInertia() * w)) * dt;
	}
	else{
		v0 = v;
		w0 = w;
	}
	dv.clear();
	dw.clear();
}
void PHSolidAux::SetupCorrection(){
	dV.clear();
	dW.clear();
}

//----------------------------------------------------------------------------
// PHConstraint
PHConstraint::PHConstraint(PHJointDesc::JointType t){
	type = t;
	fv.clear();
	fw.clear();
	Fv.clear();
	Fq.clear();
}
void PHConstraint::CompJacobian(bool bCompAngular){
	Quaterniond qrel;
	Matrix3d	R[2];
	Matrix3d	Rjabs[2];
	Vec3d		r[2];
	R[0] = solid[0]->solid->GetRotation();
	R[1] = solid[1]->solid->GetRotation();
	r[0] = solid[0]->solid->GetCenterPosition();
	r[1] = solid[1]->solid->GetCenterPosition();
	Rjabs[0] = R[0] * Rj[0];
	Rjabs[1] = R[1] * Rj[1];
	Rjrel = Rjabs[0].trans() * Rjabs[1];
	rjrel = Rjabs[0].trans() * ((R[1] * rj[1] + r[1]) - (R[0] * rj[0] + r[0]));
	Jvv[0] = -Rj[0].trans();
	Jvw[0] = -Rj[0].trans() * (-Matrix3d::Cross(rj[0]));
	Jvv[1] =  Rjabs[0].trans() * R[1];
	Jvw[1] =  Jvv[1] * (-Matrix3d::Cross(rj[1]));
	
	if(bCompAngular){
		Jwv[0].clear();
		Jww[0] = Jvv[0];
		Jwv[1].clear();
		Jww[1] = Jvv[1];
		
		qrel.FromMatrix(Rjrel);
		Matrix3d E(
			 qrel.W(),  qrel.Z(), -qrel.Y(),
			-qrel.Z(),  qrel.W(), qrel.X(),
			 qrel.Y(), -qrel.X(), qrel.W());
		E *= 0.5;
		Jqv[0].clear();
		Jqw[0] = E * Jww[0];
		Jqv[1].clear();
		Jqw[1] = E * Jww[1];
	}
}

void PHConstraint::SetupDynamics(double dt){
	bFeasible = solid[0]->solid->IsDynamical() || solid[1]->solid->IsDynamical();
	if(!bFeasible)return;

	CompJacobian(type != PHJointDesc::JOINT_CONTACT);		//接触拘束の場合は回転関係のヤコビ行列は必要ない
	
	int i, j, k;
	Av.clear();
	Aw.clear();
	Aq.clear();
	for(i = 0; i < 2; i++){
		if(solid[i]->solid->IsDynamical()){
			for(j = 0; j < dim_v; j++){
				k = idx_v[j];
				Tvv[i].row(k) = Jvv[i].row(k) * solid[i]->minv;
				Tvw[i].row(k) = Jvw[i].row(k) * solid[i]->Iinv;
				Av[k] += Jvv[i].row(k) * Tvv[i].row(k) + Jvw[i].row(k) * Tvw[i].row(k);
			}
			for(j = 0; j < dim_w; j++){
				k = idx_w[j];
				Twv[i].row(k) = Jwv[i].row(k) * solid[i]->minv;
				Tww[i].row(k) = Jww[i].row(k) * solid[i]->Iinv;
				Aw[k] += Jwv[i].row(k) * Twv[i].row(k) + Jww[i].row(k) * Tww[i].row(k);
			}
			for(j = 0; j < dim_q; j++){
				k = idx_q[j];
				Tqv[i].row(k) = Jqv[i].row(k) * solid[i]->minv;
				Tqw[i].row(k) = Jqw[i].row(k) * solid[i]->Iinv;
				Aq[k] += Jqv[i].row(k) * Tqv[i].row(k) + Jqw[i].row(k) * Tqw[i].row(k);
			}
		}
	}
	if(dim_v)for(i = 0; i < 3; i++)
		Av[i] = 1.0 / Av[i];
	if(dim_w)for(i = 0; i < 3; i++)
		Aw[i] = 1.0 / Aw[i];
	if(dim_q)for(i = 0; i < 3; i++)
		Aq[i] = 1.0 / Aq[i];
	
	for(j = 0; j < dim_v; j++){
		k = idx_v[j];
		bv[k] = rowtimes(Jvv[0], k, solid[0]->v0) + rowtimes(Jvw[0], k, solid[0]->w0) +
				rowtimes(Jvv[1], k, solid[1]->v0) + rowtimes(Jvw[1], k, solid[1]->w0);
		bv[k] *= Av[k];
		Jvv[0].row(k) *= Av[k];
		Jvw[0].row(k) *= Av[k];
		Jvv[1].row(k) *= Av[k];
		Jvw[1].row(k) *= Av[k];
	}
	for(int j = 0; j < dim_w; j++){
		k = idx_w[j];
		bw[k] = rowtimes(Jwv[0], k, solid[0]->v0) + rowtimes(Jww[0], k, solid[0]->w0) +
				rowtimes(Jwv[1], k, solid[1]->v0) + rowtimes(Jww[1], k, solid[1]->w0);
		bw[k] *= Aw[k];
		Jwv[0].row(k) *= Aw[k];
		Jww[0].row(k) *= Aw[k];
		Jwv[1].row(k) *= Aw[k];
		Jww[1].row(k) *= Aw[k];
	}
	//fv.clear();
	//fw.clear();
}
void PHConstraint::CompError(){
	Bv = rjrel;
	Quaterniond qrel;
	qrel.FromMatrix(Rjrel);
	Bq = qrel.V();
}
void PHConstraint::SetupCorrection(double dt){
	if(!bFeasible)return;

	int j, k;
	Vec3d v[2], w[2];
	for(int i = 0; i < 2; i++){
		v[i] = solid[i]->v0 + solid[i]->dv;
		w[i] = solid[i]->w0 + solid[i]->dw;
	}
	CompError();
	// velocity updateによる影響を加算
	for(j = 0; j < dim_v; j++){
		k = idx_v[j];
		Bv[k] *= Av[k];
		Bv[k] += rowtimes(Jvv[0], k, v[0]) + rowtimes(Jvw[0], k, w[0]) +
				 rowtimes(Jvv[1], k, v[1]) + rowtimes(Jvw[1], k, w[1]);
	}
	for(j = 0; j < dim_q; j++){
		k = idx_w[j];
		Bq[k] *= Aq[k];
		Bq[k] += rowtimes(Jqv[0], k, v[0]) + rowtimes(Jqw[0], k, w[0]) +
				 rowtimes(Jqv[1], k, v[1]) + rowtimes(Jqw[1], k, w[1]);
	}

	Bv *= 0.1;	//一度に誤差を0にしようとするとやや振動的になるので適当に誤差を小さく見せる
	Bq *= 0.1;
	//DSTR << B.norm() << endl;
	//Fv.clear();
	//Fq.clear();
}
void PHConstraint::IterateDynamics(){
	if(!bFeasible)return;

	Vec3d fvnew, fwnew, dfv, dfw;
	int i, j, k;
	for(j = 0; j < dim_v; j++){
		k = idx_v[j];
		fvnew[k] = fv[k] - (bv[k] + 
			rowtimes(Jvv[0], k, solid[0]->dv) + rowtimes(Jvw[0], k, solid[0]->dw) +
			rowtimes(Jvv[1], k, solid[1]->dv) + rowtimes(Jvw[1], k, solid[1]->dw));
		Projectionfv(fvnew[k], k);
		dfv[k] = fvnew[k] - fv[k];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dv += Tvv[i].row(k) * dfv[k];
				solid[i]->dw += Tvw[i].row(k) * dfv[k];
			}
		}
	}
	for(j = 0; j < dim_w; j++){
		k = idx_w[j];
		fwnew[k] = fw[k] - (bw[k] + 
			rowtimes(Jwv[0], k, solid[0]->dv) + rowtimes(Jww[0], k, solid[0]->dw) +
			rowtimes(Jwv[1], k, solid[1]->dv) + rowtimes(Jww[1], k, solid[1]->dw));
		Projectionfw(fwnew[k], k);
		dfw[k] = fwnew[k] - fw[k];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dv += Twv[i].row(k) * dfw[k];
				solid[i]->dw += Tww[i].row(k) * dfw[k];
			}
		}
	}
}
void PHConstraint::IterateCorrection(){
	Vec3d Fvnew, Fqnew, dFv, dFq;
	int i, j, k;
	for(j = 0; j < dim_v; j++){
		k = idx_v[j];
		Fvnew[k] = Fv[k] - (Bv[k] + 
			rowtimes(Jvv[0], k, solid[0]->dV) + rowtimes(Jvw[0], k, solid[0]->dW) +
			rowtimes(Jvv[1], k, solid[1]->dV) + rowtimes(Jvw[1], k, solid[1]->dW));
		ProjectionFv(Fvnew[k], k);
		dFv[k] = Fvnew[k] - Fv[k];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dV += Tvv[i].row(k) * dFv[k];
				solid[i]->dW += Tvw[i].row(k) * dFv[k];
			}
		}
	}
	for(j = 0; j < dim_q; j++){
		k = idx_q[j];
		Fqnew[k] = Fq[k] - (Bq[k] + 
			rowtimes(Jqv[0], k, solid[0]->dV) + rowtimes(Jqw[0], k, solid[0]->dW) +
			rowtimes(Jqv[1], k, solid[1]->dV) + rowtimes(Jqw[1], k, solid[1]->dW));
		ProjectionFq(Fqnew[k], k);
		dFq[k] = Fqnew[k] - Fq[k];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dV += Tqv[i].row(k) * dFq[k];
				solid[i]->dW += Tqw[i].row(k) * dFq[k];
			}
		}
	}
}

//----------------------------------------------------------------------------
// PHContactPoint
PHContactPoint::PHContactPoint(CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1):
	PHConstraint(PHJointDesc::JOINT_CONTACT), shapePair(sp), pos(p)
{
	solid[0] = s0, solid[1] = s1;

	dim_v = 3;
	dim_w = 0;
	dim_q = 0;
	idx_v[0] = 0, idx_v[1] = 1; idx_v[2] = 2;
	
	Vec3d rjabs[2], vjabs[2];
	for(int i = 0; i < 2; i++){
		rjabs[i] = pos - solid[i]->solid->GetCenterPosition();	//剛体の中心から接触点までのベクトル
		vjabs[i] = solid[i]->solid->GetVelocity() + solid[i]->solid->GetAngularVelocity() % rjabs[i];	//接触点での速度
	}
	//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
	Vec3d n, t[2], vjrel, vjrelproj;
	n = shapePair->normal;
	vjrel = vjabs[1] - vjabs[0];
	vjrelproj = vjrel - (n * vjrel) * n;		//相対速度ベクトルを法線に直交する平面に射影したベクトル
	double vjrelproj_norm = vjrelproj.norm();
	if(vjrelproj_norm < 1.0e-10){
		t[0] = n % Vec3d(1.0, 0.0, 0.0);	
		if(t[0].norm() < 1.0e-10)
			t[0] = n % Vec3d(0.0, 1.0, 0.0);
		t[0].unitize();
	}
	else{
		t[0] = vjrelproj / vjrelproj_norm;
	}
	t[1] = t[0] % n;
	Matrix3d Rjabs;
	// 接触点の関節フレームはx軸, y軸を接線，z軸を法線とする
	Rjabs.col(0) = n;
	Rjabs.col(1) = t[0];
	Rjabs.col(2) = t[1];
	
	for(int i = 0; i < 2; i++){
		Rj[i] = solid[i]->solid->GetRotation().trans() * Rjabs;
		rj[i] = solid[i]->solid->GetRotation().trans() * rjabs[i];
	}
}
void PHContactPoint::Projectionfv(double& f, int k){
	static double flim;
	if(k == 0){	//垂直抗力 >= 0の制約
		f = Spr::max(0.0, f);
		flim = 0.5 * (shapePair->shape[0]->material.mu0 + shapePair->shape[1]->material.mu0) * f;	//最大静止摩擦
	}
	else{
		//|摩擦力| <= 最大静止摩擦の制約
		//	・摩擦力の各成分が最大静止摩擦よりも小さくても合力は超える可能性があるので本当はおかしい。
		//	・静止摩擦と動摩擦が同じ値でないと扱えない。
		//摩擦係数は両者の静止摩擦係数の平均とする
		f = Spr::min(Spr::max(-flim, f), flim);
	}
}
void PHContactPoint::CompError(){
	Bv = Vec3d(-shapePair->depth, 0.0, 0.0);
}
void PHContactPoint::ProjectionFv(double& F, int k){
	//垂直抗力 >= 0の制約
	if(k == 0)
		 F = Spr::max(0.0, F);
	else F = 0.0;
}

//----------------------------------------------------------------------------
// PHHingeJoint
PHHingeJoint::PHHingeJoint():PHConstraint(PHJointDesc::JOINT_HINGE){
	dim_v = 3;
	dim_w = 2;
	dim_q = 2;
	idx_v[0] = 0, idx_v[1] = 1, idx_v[2] = 2;
	idx_w[0] = 0, idx_w[1] = 1;
	idx_q[0] = 0, idx_q[1] = 1;
}

//----------------------------------------------------------------------------
// PHSliderJoint
PHSliderJoint::PHSliderJoint():PHConstraint(PHJointDesc::JOINT_SLIDER){
	dim_v = 2;
	dim_w = 3;
	dim_q = 3;
	idx_v[0] = 0, idx_v[1] = 1;
	idx_w[0] = 0, idx_w[1] = 1, idx_w[2] = 2;
	idx_q[0] = 0, idx_q[1] = 1, idx_q[2] = 2;
}

//----------------------------------------------------------------------------
// PHBallJoint
PHBallJoint::PHBallJoint():PHConstraint(PHJointDesc::JOINT_BALL){
	dim_v = 3;
	dim_w = 0;
	dim_q = 0;
	idx_v[0] = 0, idx_v[1] = 1, idx_v[2] = 2;
}

//----------------------------------------------------------------------------
//	PHSolidPair
void PHConstraintEngine::PHSolidPair::Init(PHSolidAux* s0, PHSolidAux* s1){
	solid[0] = s0;
	solid[1] = s1;
	int ns0 = solid[0]->solid->shapes.size(), ns1 = solid[1]->solid->shapes.size();
	shapePairs.resize(ns0, ns1);
	for(int i = 0; i < ns0; i++)for(int j = 0; j < ns1; j++){
		CDShapePair& sp = shapePairs.item(i, j);
		sp.shape[0] = solid[0]->solid->shapes[i];
		sp.shape[1] = solid[1]->solid->shapes[j];
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

bool PHConstraintEngine::PHSolidPair::Detect(PHConstraintEngine* engine){
	// ＊ここでShapeについてBBoxレベル判定をすれば速くなるかも？
	static CDContactAnalysis analyzer;

	unsigned ct = OCAST(PHScene, engine->GetScene())->GetCount();
	
	// いずれかのSolidに形状が割り当てられていない場合はエラー
	if(solid[0]->solid->NShape() == 0 || solid[1]->solid->NShape() == 0)
		return false;

	// 全てのshape pairについて交差を調べる
	bool found = false;
	for(int i = 0; i < (int)(solid[0]->solid->shapes.size()); i++)for(int j = 0; j < (int)(solid[1]->solid->shapes.size()); j++){
		CDShapePair& sp = shapePairs.item(i, j);
		sp.UpdateShapePose(solid[0]->solid->GetPose(), solid[1]->solid->GetPose());

		if(sp.Detect(ct)){
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
			DSTR << engine->points.size()-n << " contacts:";
			for(unsigned i=n; i<engine->points.size(); ++i){
				DSTR << engine->points[i].pos;
			}
			DSTR << std::endl;
#endif
		}
	}
	return found;
}

//----------------------------------------------------------------------------
// PHConstraintEngine
OBJECTIMP(PHConstraintEngine, PHEngine);

PHConstraintEngine::PHConstraintEngine(){
	ready = false;
	max_iter_dynamics = 5;
	max_iter_correction = 5;
	step_size = 1.0;
	converge_criteria = 0.00000001;
}

PHConstraintEngine::~PHConstraintEngine(){

}

void PHConstraintEngine::Add(PHSolid* s){
	if(solids.Find(s) == solids.end()){
		solids.push_back(DBG_NEW PHSolidAux());
		solids.back()->solid = s;
		Invalidate();
	}
}

void PHConstraintEngine::Remove(PHSolid* s){
	PHSolidAuxs::iterator is = solids.Find(s);
	if(is != solids.end()){
		solids.erase(is);
		Invalidate();
	}
}

void PHConstraintEngine::AddJoint(PHSolid* lhs, PHSolid* rhs, const PHJointDesc& desc){
	PHSolidAuxs::iterator islhs, isrhs;
	islhs = solids.Find(lhs);
	isrhs = solids.Find(rhs);
	if(islhs == solids.end() || isrhs == solids.end())
		return;
	
	PHConstraint* joint = NULL;
	switch(desc.type){
	case PHJointDesc::JOINT_HINGE:
		joint = DBG_NEW PHHingeJoint();
		joint->Init(*islhs, *isrhs, desc);
		break;
	}
	joints.push_back(joint);

	//関節でつなげられた剛体間の接触は無効化
	//EnableContact(lhs, rhs, false);
}

void PHConstraintEngine::EnableContact(PHSolid* lhs, PHSolid* rhs, bool bEnable){
	//solidPairs.
}

void PHConstraintEngine::Init(){
	solids.Init();

	//登録されているSolidの数に合わせてsolidPairsとshapePairsをresize
	int N = solids.size();
	solidPairs.resize(N, N);
	for(int i = 0; i < N; i++)for(int j = i+1; j < N; j++){
		PHSolidPair& sp = solidPairs.item(i, j);
		sp.Init(solids[i], solids[j]);
	}

	ready = true;
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
	points.SetupCorrection(dt);
	joints.SetupCorrection(dt);
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
	for(is = solids.begin(); is != solids.end(); is++){
		aux = *is;
        solid = aux->solid;
		//velocity update
		solid->SetVelocity       (solid->GetOrientation() * (aux->v0 + aux->dv));
		solid->SetAngularVelocity(solid->GetOrientation() * (aux->w0 + aux->dw));
		//position update
		solid->SetCenterPosition(solid->GetCenterPosition() + solid->GetVelocity() * dt + solid->GetOrientation() * aux->dV);
		solid->SetOrientation(
			(solid->GetOrientation() * Quaterniond::Rot((aux->w0 + aux->dw) * dt + aux->dW)).unit()
		);
		//solid->SetOrientation((solid->GetOrientation() + solid->GetOrientation().Derivative(solid->GetOrientation() * is->dW)).unit());
		solid->SetOrientation((solid->GetOrientation() * Quaterniond::Rot(/*solid->GetOrientation() * */aux->dW)).unit());
		solid->SetUpdated(true);
	}
}

//#include <windows.h>

void PHConstraintEngine::Step(){
	if(!ready)
		Init();
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


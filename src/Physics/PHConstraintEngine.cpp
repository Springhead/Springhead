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

//----------------------------------------------------------------------------
// PHSolidAux
void PHSolidAux::SetupDynamics(double dt){
	Quaterniond q;
	Vec3d v, w, f, t;
	q = solid->GetOrientation();
	v = q.Conjugated() * solid->GetVelocity();
	w = q.Conjugated() * solid->GetAngularVelocity();
	f = q.Conjugated() * solid->nextForce;
	t = q.Conjugated() * solid->nextTorque;
	v0 = v + minv * f * dt;
	w0 = w + Iinv * (t - w % (solid->GetInertia() * w)) * dt;
	dv.clear();
	dw.clear();
}
void PHSolidAux::SetupCorrection(){
	dV.clear();
	dW.clear();
}

//----------------------------------------------------------------------------
// PHConstraint
void PHConstraint::CompRelativeVelJacobian(){
	//写像元：[v1, w1, v2, w2]	(vi, wi)は剛体i (i=0,1) の速度，角速度（ローカル座標）
	//写像先：[vrel, wrel]		剛体0の関節フレームと剛体1の関節フレームの相対速度（剛体0関節フレームローカル）
	Matrix3d	R[2];
	Vec3d		r[2];
	R[0] = solid[0]->solid->GetRotation();
	R[1] = solid[1]->solid->GetRotation();
	r[0] = solid[0]->solid->GetCenterPosition();
	r[1] = solid[1]->solid->GetCenterPosition();
	Matrix3d	Rjabs[2];
	Rjabs[0] = R[0] * Rj[0];
	Rjabs[1] = R[1] * Rj[1];
	Rjrel = Rjabs[0].trans() * Rjabs[1];
	rjrel = Rjabs[0].trans() * ((R[1] * rj[1] + r[1]) - (R[0] * rj[0] + r[0]));
	Jvrel_v[0] = -Rj[0].trans();
	Jvrel_w[0] = -Rj[0].trans() * (-Matrix3d::Cross(rj[0]));
	Jvrel_v[1] =  Rjabs[0].trans() * R[1];
	Jvrel_w[1] =  Jvrel_v[1] * (-Matrix3d::Cross(rj[1]));
	//Jwrel_v[0].clear();
	//Jwrel_w[0] = Jvrel_v[0];
	//Jwrel_v[1].clear();
	//Jwrel_w[1] = Jvrel_v[1];
}

//----------------------------------------------------------------------------
// PHConstraintND
template<int N>
void PHConstraintND<N>::CompJacobian(){
	CompRelativeVelJacobian();
	CompJointJacobian();
	Jv[0] = Jvrel * Jvrel_v[0];
	Jw[0] = Jvrel * Jvrel_w[0] + Jwrel * Jvrel_v[0];
	Jv[1] = Jvrel * Jvrel_v[1];
	Jw[1] = Jvrel * Jvrel_w[1] + Jwrel * Jvrel_v[1];
}
template<int N>
void PHConstraintND<N>::SetupDynamics(double dt){
	CompJacobian();
	
	Tv[0] = solid[0]->minv * Jv[0].trans();
	Tw[0] = solid[0]->Iinv * Jw[0].trans();
	Tv[1] = solid[1]->minv * Jv[1].trans();
	Tw[1] = solid[1]->Iinv * Jw[1].trans();
	
	A = Jv[0] * Tv[0] + Jw[0] * Tw[0] + Jv[1] * Tv[1] + Jw[1] * Tw[1];
	
	b = Jv[0] * (solid[0]->v0) + Jw[0] * (solid[0]->w0) + Jv[1] * (solid[1]->v0) + Jw[1] * (solid[1]->w0);
	double tmp;
	for(int i = 0; i < N; i++){
		tmp = 1.0 / A[i][i];
		b[i] *= tmp;
		Jv[0].row(i) *= tmp;
		Jw[0].row(i) *= tmp;
		Jv[1].row(i) *= tmp;
		Jw[1].row(i) *= tmp;
	}
	/*Ainv = A.inv();
	b = Ainv * b;
	Jv[0] = Ainv * Jv[0];
	Jw[0] = Ainv * Jw[0];
	Jv[1] = Ainv * Jv[1];
	Jw[1] = Ainv * Jw[1];*/

	f.clear();
}
template<int N>
void PHConstraintND<N>::SetupCorrection(double dt){
	CompError();
	for(int i = 0; i < N; i++)
		B[i] /= A[i][i];
	//B = Ainv * B;
	Vec3d v[2], w[2];
	for(int i = 0; i < 2; i++){
		v[i] = solid[i]->v0 + solid[i]->dv;
		w[i] = solid[i]->w0 + solid[i]->dw;
	}
	// velocity updateによる影響を加算
	B += (Jv[0] * v[0] + Jw[0] * w[0] + Jv[1] * v[1] + Jw[1] * w[1]) * dt;
	B *= 0.1;	//一度に誤差を0にしようとするとやや振動的になるので適当に誤差を小さく見せる
	DSTR << B.norm() << endl;
	F.clear();
}
template<int N>
void PHConstraintND<N>::IterateDynamics(){
	//dfsum = 0.0;
	//反復
	//接触力fの更新
	VecNd fnew, df;
	
	/*fnew = f - (b + Jv[0] * solid[0]->dv + Jw[0] * solid[0]->dw + Jv[1] * solid[1]->dv + Jw[1] * solid[1]->dw);
	ProjectionDynamics(fnew);
	VecNd df = fnew - f;
	//dfsum += df.square();
	f = fnew;
	//DSTR << "f : " << f << endl;
	solid[0]->dv += Tv[0] * df;
	solid[0]->dw += Tw[0] * df;
	solid[1]->dv += Tv[1] * df;
	solid[1]->dw += Tw[1] * df;*/

	for(int i = 0; i < N; i++){
		fnew[i] = f[i] - (b[i] + Jv[0].row(i) * solid[0]->dv + Jw[0].row(i) * solid[0]->dw + Jv[1].row(i) * solid[1]->dv + Jw[1].row(i) * solid[1]->dw);
		ProjectionDynamics(fnew);
		df[i] = fnew[i] - f[i];
		//dfsum += df.square();
		f[i] = fnew[i];
		//DSTR << "f : " << f << endl;
		solid[0]->dv += Tv[0].col(i) * df[i];
		solid[0]->dw += Tw[0].col(i) * df[i];
		solid[1]->dv += Tv[1].col(i) * df[i];
		solid[1]->dw += Tw[1].col(i) * df[i];	
	}
}
template<int N>
void PHConstraintND<N>::IterateCorrection(){
	VecNd Fnew, dF;
	
	/*Fnew = F - (B + Jv[0] * solid[0]->dV + Jw[0] * solid[0]->dW + Jv[1] * solid[1]->dV + Jw[1] * solid[1]->dW);
	ProjectionCorrection(Fnew);
	VecNd dF = Fnew - F;
	//dFsum += ip->dF * ip->dF;
	F = Fnew;
	//DSTR << "F : " << F << endl;
	solid[0]->dV += Tv[0] * dF;
	solid[0]->dW += Tw[0] * dF;
	solid[1]->dV += Tv[1] * dF;
	solid[1]->dW += Tw[1] * dF;*/

	for(int i = 0; i < N; i++){
		Fnew[i] = F[i] - (B[i] + Jv[0].row(i) * solid[0]->dV + Jw[0].row(i) * solid[0]->dW + Jv[1].row(i) * solid[1]->dV + Jw[1].row(i) * solid[1]->dW);
		ProjectionCorrection(Fnew);
		dF[i] = Fnew[i] - F[i];
		//dFsum += ip->dF * ip->dF;
		F[i] = Fnew[i];
		//DSTR << "F : " << F << endl;
		solid[0]->dV += Tv[0].col(i) * dF[i];
		solid[0]->dW += Tw[0].col(i) * dF[i];
		solid[1]->dV += Tv[1].col(i) * dF[i];
		solid[1]->dW += Tw[1].col(i) * dF[i];
	}
}

//----------------------------------------------------------------------------
// PHContactPoint
void PHContactPoint::CompJacobian(){

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

	CompRelativeVelJacobian();
	Jv[0] = Jvrel_v[0];
	Jw[0] = Jvrel_w[0];
	Jv[1] = Jvrel_v[1];
	Jw[1] = Jvrel_w[1];
}
void PHContactPoint::ProjectionDynamics(VecNd& f){
	//垂直抗力 >= 0の制約
	f[0] = Spr::max(0.0, f[0]);
	
	//|摩擦力| <= 最大静止摩擦の制約
	//	・摩擦力の各成分が最大静止摩擦よりも小さくても合力は超える可能性があるので本当はおかしい。
	//	・静止摩擦と動摩擦が同じ値でないと扱えない。
	//摩擦係数は両者の静止摩擦係数の平均とする
	double flim = 0.5 * (shapePair->shape[0]->material.mu0 + shapePair->shape[1]->material.mu0) * f[0];	//最大静止摩擦
	f[1] = Spr::min(Spr::max(-flim, f[1]), flim);
	f[2] = Spr::min(Spr::max(-flim, f[2]), flim);
}
void PHContactPoint::CompError(){
	B = Vec3d(-shapePair->depth, 0.0, 0.0);
}
void PHContactPoint::ProjectionCorrection(VecNd& F){
	//垂直抗力 >= 0の制約
	F[0] = Spr::max(0.0, F[0]);
	F[1] = F[2] = 0.0;
}

//----------------------------------------------------------------------------
// PHHingeJoint
PHHingeJoint::PHHingeJoint(){
	Matrix3d unit = Matrix3d::Unit();
	Jvrel.SUBMAT(0, 0, 3, 3) = unit;
	Jvrel.SUBMAT(3, 0, 2, 3) = unit.SUBMAT(0, 0, 2, 3);
	Jwrel.SUBMAT(0, 0, 3, 3).clear();
}
void PHHingeJoint::CompJointJacobian(){
	//写像元：[vrel, wrel]
	//写像先：y	[y[0], y[1], y[2]] = vrel,
	//		    [y[3], y[4]] = 剛体0関節フレームから見た剛体1関節フレーム上の[0,0,1]の速度のx, y成分
	Jwrel.SUBMAT(3, 0, 2, 3) = -Matrix3d::Cross(Rjrel * Vec3d(0.0, 0.0, 1.0)).SUBMAT(0, 0, 2, 3);
}
void PHHingeJoint::CompError(){
	B.SUBVEC(0, 3) = rjrel;
	B.SUBVEC(3, 2) = (Rjrel * Vec3d(0.0, 0.0, 1.0)).SUBVEC(0, 2);
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
		solids.push_back(PHSolidAux());
		solids.back().solid = s;
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
	
	PHConstraint* joint;
	switch(desc.type){
	case PHJointDesc::JOINT_HINGE:
		joint = DBG_NEW PHHingeJoint();
		joint->Init(&*islhs, &*isrhs, desc);
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
	int N = solids.size();
	for(int i = 0; i < N; i++){
		solids[i].minv = solids[i].solid->GetMassInv();
		solids[i].Iinv = solids[i].solid->GetInertiaInv();
	}

	//登録されているSolidの数に合わせてsolidPairsとshapePairsをresize
	solidPairs.resize(N, N);
	for(int i = 0; i < N; i++)for(int j = i+1; j < N; j++){
		PHSolidPair& sp = solidPairs.item(i, j);
		sp.Init(&solids[i], &solids[j]);
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
		solids[i].solid->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
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
			DSTR << "max count." << " iteration count: " << count << " dfsum: " << dfsum << endl;
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
			DSTR << "max count." << " iteration count: " << count << " dFsum: " << dFsum << endl;
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
	for(is = solids.begin(); is != solids.end(); is++){
		PHSolid* solid = is->solid;
		//velocity update
		solid->SetVelocity       (solid->GetOrientation() * (is->v0 + is->dv));
		solid->SetAngularVelocity(solid->GetOrientation() * (is->w0 + is->dw));
		//position update
		solid->SetCenterPosition(solid->GetCenterPosition() + solid->GetVelocity() * dt + solid->GetOrientation() * is->dV);
		solid->SetOrientation(
			(solid->GetOrientation() * Quaterniond::Rot((is->w0 + is->dw) * dt + is->dW)).unit()
		);
		//solid->SetOrientation((solid->GetOrientation() + solid->GetOrientation().Derivative(solid->GetOrientation() * is->dW)).unit());
		solid->SetOrientation((solid->GetOrientation() * Quaterniond::Rot(/*solid->GetOrientation() * */is->dW)).unit());
		solid->SetUpdated(true);
	}
}

void PHConstraintEngine::Step(){
	if(!ready)
		Init();

	//交差を検知
	DetectPenetration();
	
	double dt = OCAST(PHScene, GetScene())->GetTimeStep();

	//PrintContacts();

	//DSTR << "dynamics: " << endl;
	SetupDynamics(dt);
	IterateDynamics();
	
	//DSTR << "correction: " << endl;
	SetupCorrection(dt);
	IterateCorrection();

	UpdateSolids(dt);

}

#undef SUBMAT
#undef SUBVEC

}


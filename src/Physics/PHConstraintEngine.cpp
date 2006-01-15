#include "Physics.h"
#pragma hdrstop
#include <float.h>
#include <Collision/CDDetectorImp.h>


using namespace PTM;
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

bool PHConstraintEngine::PHSolidPair::Detect(int is0, int is1, PHConstraintEngine* engine){
	// ＊ここでShapeについてBBoxレベル判定をすれば速くなるかも？
	static CDContactAnalysis analyzer;

	unsigned ct = ((PHScene*)(engine->GetScene()))->GetCount();
	
	PHSolid *s0 = engine->solids[is0], *s1 = engine->solids[is1];

	// 全てのshape pairについて交差を調べる
	bool found = false;
	for(int i = 0; i < s0->shapes.size(); i++)for(int j = 0; j < s1->shapes.size(); j++){
		CDShapePair& sp = shapePairs.item(i, j);
		sp.UpdateShapePose(s0->GetPose(), s1->GetPose());
		//このshape pairの交差判定
		if(sp.Detect(ct)){
			found = true;
			//交差形状の計算
			analyzer.FindIntersection(&sp);
			//交差の法線と中心を得る
			analyzer.CalcNormal(&sp);

			//摩擦係数は両者の静止摩擦係数の平均とする
			double mu = (sp.shape[0]->material.mu0 + sp.shape[1]->material.mu0) * 0.5;
			//接触を作成
			engine->contacts.push_back(PHContact(is0, is1, i, j, sp.normal, sp.center, mu));

			//接触点の作成：
			//交差形状を構成する頂点はanalyzer.planes.beginからendまでの内deleted==falseのもの
			typedef CDQHPlanes<CDContactAnalysisFace>::CDQHPlane Plane;
			Vec3d v, vproj;
			for(Plane* p = analyzer.planes.begin; p != analyzer.planes.end; p++){
				if(p->deleted)
					continue;
				//sp.centerを通りsp.normalを法線とする平面上に頂点を射影
				//法線は正規化されているとする
				v = p->normal;
                vproj = v - ((v - sp.center) * sp.normal) * sp.normal;

				engine->points.push_back(PHContactPoint(engine->contacts.size() - 1, vproj));
			}
		}
	}
	return found;
}

//----------------------------------------------------------------------------

OBJECTIMP(PHConstraintEngine, PHEngine);

PHConstraintEngine::PHConstraintEngine(){
	bReady = false;
}

PHConstraintEngine::~PHConstraintEngine(){

}

void PHConstraintEngine::AddSolid(PHSolid* s){
	if(solids.Find(s) == 0){
		solids.push_back(s);
		bReady = false;
	}
}

void PHConstraintEngine::RemoveSolid(PHSolid* s){
	if(solids.Erase(s))
		bReady = false;
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

	bReady = true;
}

void PHConstraintEngine::Detect(){
	/* 以下の流れで交差を求める
		1. SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外
		2. 各Solidの組について
			2a. ShapeのBBoxレベルでの交差判定 (未実装)
			2b. 各Shapeの組についてGJKで交差形状を得る
			2c. 交差形状から法線を求め、法線に関して形状を射影し，その頂点を接触点とする
			2d. 得られた接触点情報をPHContactPointsに詰めていく
	 */

	contacts.clear();
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
	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	初端だったら，リスト内の物体と判定
			for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->index;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				//2. SolidとSolidの衝突判定
				solidPairs.item(f1, f2).Detect(f1, f2, this);
			}
			cur.insert(it->index);
		}else{
			cur.erase(it->index);			//	終端なので削除．
		}
	}
}


void PHConstraintEngine::SetupLCP(){
	double dt = ((PHSceneIf*)GetScene())->GetTimeStep();

	//LCP構築
	//各Solidに関係する変数
	for(int i = 0; i < solids.size(); i++){
		solidAuxs[i].dVlin_nc = solids[i]->GetVelocity() + solidAuxs[i].minv * solids[i]->GetForce() * dt;
		Vec3d w = solids[i]->GetAngularVelocity();
		solidAuxs[i].dVang_nc = w + solidAuxs[i].Iinv * (solids[i]->GetTorque() - w % (solids[i]->GetInertia() * w)) * dt;
	}

	//各Contactに関係する変数
	Vec3d n, c, r[2], v[2], vrel, t[2];
	Matrix3d rcross[2];
	Posed q[2];
	PHSolid* solid[2];
	PHSolidAux* solidaux[2];
	int icon = 0;
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
			}
		}
		c = ip->pos;	//接触点
		for(int i = 0; i < 2; i++){
			r[i] = c - q[i].pos;	//剛体の中心から接触点までのベクトル
			rcross[i] = Matrix3d::Cross(r[i]);
			v[i] = solid[i]->GetVelocity() + solid[i]->GetAngularVelocity() % r[i];	//接触点での速度
		}
		//接線ベクトルt[0], t[1]
		// *t[0]は相対速度ベクトルに平行になるようにする(といいらしい)
		vrel = v[1] - v[0];
		//t[0] = (n % vrel) % n より変形
		t[0] = vrel - (n * vrel) * n;
		t[1] = t[0] % n;

		for(int i = 0; i < 2; i++){
			ip->Jlin[i].row(0) = n;
			ip->Jlin[i].row(1) = t[0];
			ip->Jlin[i].row(2) = t[1];
			ip->Jang[i].row(0) = rcross[0] * n;
			ip->Jang[i].row(1) = rcross[0] * t[0];
			ip->Jang[i].row(2) = rcross[0] * t[1];
			if(i == 0){
				ip->Jlin[i] *= -1.0;
				ip->Jang[i] *= -1.0;
			}
			ip->Tlin[i] = solidaux[i]->minv * ip->Jlin[i].trans();
			ip->Tang[i] = solidaux[i]->Iinv * ip->Jang[i].trans();
		}
		ip->b = ip->Jlin[0] * solidaux[0]->dVlin_nc + ip->Jang[0] * solidaux[0]->dVang_nc +
				ip->Jlin[1] * solidaux[1]->dVlin_nc + ip->Jang[1] * solidaux[1]->dVang_nc;
	}

	//A行列の設定
	/*
	δ(lhs(i),lhs(j)) * (1/m_lhs(i) J_lin(i,lhs(i)) J_lin(j,lhs(j))' + J_ang(i,lhs(i)) I_lhs(i)^-1 J_ang(j,lhs(j))') +
	δ(lhs(i),rhs(j)) * (1/m_lhs(i) J_lin(i,lhs(i)) J_lin(j,rhs(j))' + J_ang(i,lhs(i)) I_lhs(i)^-1 J_ang(j,rhs(j))') +
	δ(rhs(i),lhs(j)) * (1/m_rhs(i) J_lin(i,rhs(i)) J_lin(j,lhs(j))' + J_ang(i,rhs(i)) I_rhs(i)^-1 J_ang(j,lhs(j))')
	δ(rhs(i),rhs(j)) * (1/m_rhs(i) J_lin(i,rhs(i)) J_lin(j,rhs(j))' + J_ang(i,rhs(i)) I_rhs(i)^-1 J_ang(j,rhs(j))') +
	*/
	//Aは対称行列なので対角および上三角要素のみ計算
	A.resize(points.size(), points.size());
	PHContactPoints::iterator ip_row, ip_col;
	PHLCPMatrix::iterator iA;
	int icon_row, icon_col, solid_row[2], solid_col[2];
	icon_row = 0;
	for(ip_row = points.begin(); ip_row != points.end(); ip_row++){
		if(ip_row->contact != icon_row){
			icon_row = ip_row->contact;
			PHContact& con = contacts[icon_row];
			solid_row[0] = con.solid[0];
			solid_row[1] = con.solid[1];
		}
		int i = ip_row - points.begin();
		iA = &A.item(i, i);
		icon_col = icon_row;
		for(ip_col = ip_row; ip_col != points.end(); ip_col++){
			if(ip_col->contact != icon_col){
				icon_col = ip_col->contact;
				PHContact& con = contacts[icon_col];
				solid_col[0] = con.solid[0];
				solid_col[1] = con.solid[1];
			}		
			iA->clear();
			if(solid_row[0] == solid_col[0])
				*iA += ip_row->Jlin[0] * ip_col->Tlin[0] + ip_row->Jang[0] * ip_col->Tang[0];
			if(solid_row[0] == solid_col[1])
				*iA += ip_row->Jlin[0] * ip_col->Tlin[1] + ip_row->Jang[0] * ip_col->Tang[1];
			if(solid_row[1] == solid_col[0])
				*iA += ip_row->Jlin[1] * ip_col->Tlin[0] + ip_row->Jang[1] * ip_col->Tang[0];
			if(solid_row[1] == solid_col[1])
				*iA += ip_row->Jlin[1] * ip_col->Tlin[1] + ip_row->Jang[1] * ip_col->Tang[1];
			iA++;
		}
	}

	//各接触点のbとJlin, JangをAの対角要素で割る
	iA = A.begin();
	for(PHContactPoints::iterator ip = points.begin(); ip != points.end(); ip++){
		for(int i = 0; i < 3; i++){
			//0割りチェックは？
			double diag_inv = 1.0 / (*iA)[i][i];
			ip->b[i] *= diag_inv;
			ip->Jlin[0].row(i) *= diag_inv;
			ip->Jlin[1].row(i) *= diag_inv;
			ip->Jang[0].row(i) *= diag_inv;
			ip->Jang[1].row(i) *= diag_inv;
		}
		iA += points.size() + 1;	//次の対角要素へ
	}
}

void PHConstraintEngine::SetInitialValue(){
	//速度変化量は無負荷の場合で初期化
	for(PHSolidAuxArray::iterator is = solidAuxs.begin(); is != solidAuxs.end(); is++){
		is->dVlin = is->dVlin_nc;
		is->dVang = is->dVang_nc;
	}
	//接触力は0で初期化
	for(PHContactPoints::iterator ip = points.begin(); ip != points.end(); ip++){
		ip->f.clear();
	}
}

bool PHConstraintEngine::CheckConvergence(){
	double e = 0.0;
	for(PHContactPoints::iterator ip = points.begin(); ip != points.end(); ip++)
		e += ip->df.norm();
	return e < 0.001;
}

void PHConstraintEngine::UpdateLCP(){
	PHContactPoints::iterator ip;
	PHContact* con;
	PHSolidAux* solidaux[2];
	//接触力fの更新
	Vec3d fnew;
	int icon = 0;
	for(ip = points.begin(); ip != points.end(); ip++){
		if(icon != ip->contact){
			icon = ip->contact;
			con = &contacts[icon];
			for(int i = 0; i < 2; i++)
				solidaux[i] = &solidAuxs[con->solid[i]];
		}
		fnew = ip->f - (ip->b +
			ip->Jlin[0] * solidaux[0]->dVlin + ip->Jang[0] * solidaux[0]->dVang +
			ip->Jlin[1] * solidaux[1]->dVlin + ip->Jang[1] * solidaux[1]->dVang);

		//fmin = {  0, -mu * f[0], -mu * f[0]};
		//fmax = {Inf,  mu * f[0],  mu * f[0]};
		//fnew[i] = min(max(fmin[i], f[i]), fmax[i]);
		//＊メモ：
		//	・摩擦力の各成分が最大静止摩擦よりも小さくても合力は超える可能性があるので本当はおかしい。
		//	・静止摩擦と動摩擦が同じ値でないと扱えない。
		
		//垂直抗力 >= 0の制約
		fnew[0] = Spr::max(0.0, fnew[0]);
		
		//|摩擦力| <= 最大静止摩擦の制約
		double flim = con->mu * fnew[0];		//最大静止摩擦
		fnew[1] = Spr::min(Spr::max(-flim, fnew[1]), flim);
		fnew[2] = Spr::min(Spr::max(-flim, fnew[2]), flim);		

		ip->df = fnew - ip->f;
		ip->f = fnew;
	}
	//速度変化dVの更新
	icon = 0;
	for(ip = points.begin(); ip != points.end(); ip++){
		if(icon != ip->contact){
			icon = ip->contact;
			PHContact& con = contacts[icon];
			for(int i = 0; i < 2; i++)
				solidaux[i] = &solidAuxs[con.solid[i]];
		}
		solidaux[0]->dVlin = ip->Tlin[0] * ip->f;
		solidaux[0]->dVang = ip->Tang[0] * ip->f;
		solidaux[1]->dVlin = ip->Tlin[1] * ip->f;
		solidaux[1]->dVang = ip->Tang[1] * ip->f;
	}
}

void PHConstraintEngine::Step(){
	if(!bReady)
		Init();

	//交差を検知
	Detect();

	//LCPを設定
	SetupLCP();

	//決定変数の初期値を設定
	SetInitialValue();

	while(true){
		//terminate condition
		if(CheckConvergence())
			break;
		
		UpdateLCP();
	}

}

}

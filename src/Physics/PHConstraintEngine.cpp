#include "Physics.h"
#pragma hdrstop
#include <float.h>
#include <Collision/CDDetectorImp.h>


using namespace PTM;
namespace Spr{;

/*
//	衝突検出と接触力の計算
bool PHConstraintEngine::PHSolidPair::Detect(PHConstraintEngine* engine){
	bool rv = false;
	int ct = ((PHScene*)engine->GetScene())->GetCount();
	double dt = ((PHScene*)engine->GetScene())->GetTimeStep();

	//	Shape同士の接触判定
	typedef std::vector<PHShapePair*> Contacts;
	Contacts contacts;
	for(int i=0; i<shapePairs.height(); ++i){
		for(int j=0; j<shapePairs.width(); ++j){
			PHShapePair* shapePair = shapePairs.item(i,j);
			shapePair->UpdateShapePose(solid[0].solid->GetPose(), solid[1].solid->GetPose());
			if ( shapePair->Detect(ct) ){
				rv = true;
				contacts.push_back(shapePair);
				static CDContactAnalysis analyzer;
				analyzer.FindIntersection(shapePair);	//	接触形状の解析
				analyzer.CalcNormal(shapePair);			//	法線ベクトルの計算

			}
		}
	}

	return rv;
}
*/

//----------------------------------------------------------------------------
OBJECTIMP(PHConstraintEngine, PHEngine);

void PHConstraintEngine::Add(PHSolid* s){
	solids.push_back(s);
	Init();
}

void PHConstraintEngine::Init(){
}

/**	Solidの端の位置をもち，ソートされるもの	*/
struct PHSolidEdge{
	float edge;				///<	端の位置(グローバル系)
	int solid;				///<	元の solidの位置
	bool bMin;				///<	初端ならtrue
	bool operator < (const PHSolidEdge& s) const { return edge < s.edge; }
};

void PHConstraintEngine::Step(){
/*
	//BBoxレベルの衝突判定
	Vec3f dir(0,0,1);
	typedef std::vector<PHSolidEdge> Edges;
	Edges edges;
	edges.resize(solids.size()*2);
	Edges::iterator eit = edges.begin();
	for(unsigned int i = 0; i < solids.size(); ++i){
		solids[i]->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		if (!_finite(eit[0].edge) || !_finite(eit[1].edge)){
			DSTR << solids[i]->GetName() << " min:" << eit[0].edge << " max:" << eit[1].edge << std::endl;
			solids[i]->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		}
		eit[0].solid = i; eit[0].bMin = true;
		eit[1].solid = i; eit[1].bMin = false;
		eit += 2;
	}
	if (edges.size() > 200){
		DSTR << "strange edges" << std::endl;
		DSTR << (unsigned)&*edges.begin() << (unsigned)&*edges.end();
	}
	std::sort(edges.begin(), edges.end());
	//端から見ていって，接触の可能性があるノードの判定をする．
	typedef std::set<int> SolidSet;
	SolidSet cur;							//	現在のSolidのセット
	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	初端だったら，リスト内の物体と判定
			for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->solid;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				PHSolidPair* pair =  solidPairs.item(f1, f2);
				//	SolidとSolidの衝突判定
				pair->Detect(this);	//	再帰的に衝突判定(GJKを使用)
			}
			cur.insert(it->solid);
		}else{
			cur.erase(it->solid);			//	終端なので当該フレームを削除．
		}
	}
*/
	//ここまでの処理でPHContactVolumeの配列を得る.
	//これをもとにLCP構築
	//ヤコビ行列に値を詰める
	Vec3d n, c, r[2], v[2], vrel, t[2];
	Matrix3d rcross[2];
	Posed q[2];
	PHSolid* solid[2];
	for(PHContactVolumes::iterator iv = contacts.begin(); iv != contacts.end(); iv++){
		n = iv->normal;	//法線
		for(PHContactPoints::iterator ip = iv->points.begin(); ip != iv->points.end(); ip++){
			c = ip->point;	//接触点
			for(int i = 0; i < 2; i++){
				solid[i] = solids[iv->solids[i]];
                q[i] = solid[i]->GetPose();
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

			ip->J[0][0].row(0) = -n;
			ip->J[0][0].row(1) = -t[0];
			ip->J[0][0].row(2) = -t[1];
			ip->J[0][1].row(0) = -rcross[0] * n;
			ip->J[0][1].row(1) = -rcross[0] * t[0];
			ip->J[0][1].row(2) = -rcross[0] * t[1];
			ip->J[1][0].row(0) =  n;
			ip->J[1][0].row(1) =  t[0];
			ip->J[1][0].row(2) =  t[1];
			ip->J[1][1].row(0) =  rcross[1] * n;
			ip->J[1][1].row(1) =  rcross[1] * t[0];
			ip->J[1][1].row(2) =  rcross[1] * t[1];
		}
	}

	//LCPを解く
	//precomputation

	while(true){
		//terminate condition

		//linear equation

		//projection

	}

}

}

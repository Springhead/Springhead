#include "Physics.h"
#pragma hdrstop
#include <float.h>
#include <Collision/CDDetectorImp.h>

using namespace PTM;
namespace Spr{;

void PHPenaltyEngine::PHSolidPair::Init(){
	shapePairs.resize(solid[0]->shapes.size(), solid[1]->shapes.size());
	for(unsigned i=0; i<solid[0]->shapes.size(); ++i){
		for(unsigned j=i+1; i<solid[1]->shapes.size(); ++j){
			shapePairs.item(i, j) = new CDShapePair(solid[0]->shapes[i], solid[1]->shapes[j]);
		}		
	}
}

bool PHPenaltyEngine::PHSolidPair::Detect(PHPenaltyEngine* engine){
	bool rv = false;
	for(int i=0; i<shapePairs.height(); ++i){
		for(int j=i; j<shapePairs.width(); ++j){
			rv = rv || shapePairs.item(i,j)->Detect(solid[0]->GetPose(), solid[1]->GetPose(), 
							((PHScene*)engine->GetScene())->GetCount() );
		}
	}
	return rv;
}

void PHPenaltyEngine::Init(){
	solidPairs.resize(solids.size(), solids.size());
	for(unsigned i=0; i<solids.size(); ++i){
		for(unsigned j=i+1; j<solids.size(); ++j){
			solidPairs.item(i, j) = new PHSolidPair(solids[i], solids[j]);
			solidPairs.item(i, j)->Init();
		}
	}
}

/**	Solidの端の位置をもち，ソートされるもの	*/
struct PHSolidEdge{
	float edge;				///<	端の位置(グローバル系)
	int solid;				///<	元の solidの位置
	bool bMin;				///<	初端ならtrue
	bool operator < (const PHSolidEdge& s) const { return edge < s.edge; }
};

void PHPenaltyEngine::Step(){
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
	//	端から見ていって，接触の可能性があるノードの判定をする．
	typedef std::set<int> SolidSet;
	SolidSet cur;							//	現在のSolidのセット
	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	初端だったら，リスト内の物体と判定
//			DSTR << "Col " << it->solid << " - ";
			for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->solid;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				PHSolidPair* pair =  solidPairs.item(f1, f2);
				//	SolidとSolidの衝突判定
				if (pair->Detect(this)){	//	再帰的に衝突判定(GJKを使用)
/*
					listeners.Before(scene, pair);
					for(CDIntersections::iterator it = pair->intersections.begin(); it != pair->intersections.end(); ++it){
						timerQhull.Start();
						analyzer.FindIntersection(*it->convexPair, it->geometryPair->posture);	//	接触形状の解析
						timerQhull.Stop();
						timerNormal.Start();
						analyzer.CalcNormal(*(CDConvexPairWithNormal*)it->convexPair, it->geometryPair->posture);		//	法線ベクトルの計算
						timerNormal.Stop();
						listeners.Analyzed(scene, pair, it->geometryPair, (CDConvexPairWithRecord*) it->convexPair, &analyzer);
					}
					listeners.After(scene, pair);
*/				}
//				DSTR << " " << *itf;
			}
//			DSTR << std::endl;
			cur.insert(it->solid);
		}else{
			cur.erase(it->solid);			//	終端なので当該フレームを削除．
		}
	}
	
}


}

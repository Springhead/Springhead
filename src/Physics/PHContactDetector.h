/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONTACTDETECTOR_H
#define PHCONTACTDETECTOR_H

//#include <SprPhysics.h>
#include <vector>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Physics/PHSolid.h>

namespace Spr{;

///剛体に関する付加情報
class PHSolidInfo : public Object{
public:
	PHSolid*	solid;		///< PHSolidへの参照
	PHSolidInfo(PHSolid* s):solid(s){}
};

template<class TSolidInfo>
class PHSolidInfos : public std::vector< UTRef<TSolidInfo> >{
public:
	typedef std::vector< UTRef<TSolidInfo> > base;
	typedef typename base::iterator iterator;
	iterator Find(PHSolid* s){
		iterator is;
		for(is = base::begin(); is != base::end(); is++)
			if((*is)->solid == s)
				break;
		return is;
	};
};


/// 剛体の組の状態
struct PHSolidPairState{
	bool bEnabled;
};

/// 剛体の組
template<class TSolidInfo, class TShapePair, class TEngine>
class PHSolidPair : public PHSolidPairState, public UTRefCount{
public:
	typedef TShapePair shapepair_type;
	typedef TEngine engine_type;

	TSolidInfo* solid[2];
	UTCombination< UTRef<TShapePair> > shapePairs;

	virtual void Init(TSolidInfo* s0, TSolidInfo* s1){
		solid[0] = s0;
		solid[1] = s1;
		int ns0 = s0->solid->NShape(), ns1 = s1->solid->NShape();
		shapePairs.resize(ns0, ns1);
		bEnabled = true;
	}

	virtual void OnDetect(shapepair_type* sp, engine_type* engine, unsigned ct, double dt){}	///< 交差が検知されたときの処理
	
	bool Detect(TEngine* engine, unsigned int ct, double dt){
		if(!bEnabled)return false;

		// いずれかのSolidに形状が割り当てられていない場合は接触なし
		PHSolid *s0 = solid[0]->solid, *s1 = solid[1]->solid;
		if(s0->NShape() == 0 || s1->NShape() == 0) return false;

		// 全てのshape pairについて交差を調べる
		bool found = false;
		TShapePair* sp;
		for(int i = 0; i < s0->NShape(); i++)for(int j = 0; j < s1->NShape(); j++){
			sp = shapePairs.item(i, j);
			//sp.UpdateShapePose(solid[0]->solid->GetPose(), solid[1]->solid->GetPose());
			//このshape pairの交差判定/法線と接触の位置を求める．
			if(sp->Detect(
				ct,
				DCAST(CDConvex, s0->GetShape(i)), DCAST(CDConvex, s1->GetShape(j)),
				s0->GetPose() * s0->GetShapePose(i), s1->GetPose() * s1->GetShapePose(j)))
			{
				found = true;
				OnDetect(sp, engine, ct, dt);
			}
		}
		return found;
	}

	void SetState(const PHSolidPairState& s){
		*((PHSolidPairState*)this) = s;
	}
};

///	PHContactDetectorの状態
struct PHContactDetectorState{
	size_t nSolidPair;	///<	SolidPairの数
	size_t nShapePair;	///<	ShapePairの数
	PHSolidPairState* SolidStates(){
		char* ptr = ((char*)this) + sizeof(*this);
		return (PHSolidPairState*)ptr;
	}
	CDShapePairState* ShapeStates(){
		char* ptr = ((char*)this) + sizeof(*this) + nSolidPair*sizeof(PHSolidPairState);
		return (CDShapePairState*)ptr;
	}
	size_t GetSize(){
		return sizeof(*this) + nSolidPair*sizeof(PHSolidPairState)
			+ nShapePair*sizeof(CDShapePairState);
	}
	PHContactDetectorState(int n, int m):nSolidPair(n), nShapePair(m){}

};

template<class TSolidInfo, class TShapePair, class TSolidPair, class TEngine>
class PHContactDetector : public PHEngine{

	// AABBでソートするための構造体
	struct Edge{
		float edge;				///<	端の位置(グローバル系)
		int	index;				///<	元の solidの位置
		bool bMin;				///<	初端ならtrue
		bool operator < (const Edge& s) const { return edge < s.edge; }
	};
	typedef std::vector<Edge> Edges;

public:
	typedef TShapePair shapepair_type;
	typedef TSolidPair solidpair_type;
	typedef TEngine engine_type;
	typedef UTCombination< UTRef<TSolidPair> > PHSolidPairs;

	bool						bContactEnabled;///< 接触が有効化された剛体の組が1つでも存在すればtrue
	PHSolidInfos<TSolidInfo>	solids;			///< 剛体の配列
	PHSolidPairs				solidPairs;		///< 剛体の組の配列

	virtual void Clear(){
		solidPairs.clear();
		solids.clear();
	}
	
	///< 剛体の追加
	virtual bool AddChildObject(ObjectIf* o){
		PHSolid* s = DCAST(PHSolid, o);
		if(s && solids.Find(s) == solids.end()){
			solids.push_back(DBG_NEW TSolidInfo(s));
			int N = (int)solids.size();
			assert(solidPairs.height() == N-1 && solidPairs.width() == N-1);
			solidPairs.resize(N, N);
			for(int i = 0; i < N-1; i++){
				solidPairs.item(i, N-1) = DBG_NEW TSolidPair();
				solidPairs.item(i, N-1)->Init(solids[i], solids[N-1]);
			}
			return true;
		}
		return false;
	}

	///< 剛体の削除
	virtual bool DelChildObject(ObjectIf* o){
		PHSolid* s = DCAST(PHSolid, o);
	 	if(!s)return false;
		typename PHSolidInfos<TSolidInfo>::iterator is = solids.Find(s);
		if(is != solids.end()){
			int idx = is - solids.begin();
			solids.erase(is);
			solidPairs.erase_row(idx);
			solidPairs.erase_col(idx);
			return true;
		}
		return false;
	}

	int NSolidPairs() const{
		int n = (int)solids.size();
		return n * (n - 1) / 2;
	}
	int NShapePairs() const{
		int n = (int)solids.size(), N = 0;
		for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
			N += (int)(solidPairs.item(i,j)->shapePairs.size());
		return N;
	}

	virtual size_t GetStateSize() const {
		PHContactDetectorState s(NSolidPairs(), NShapePairs());
		return s.GetSize();
	}
	virtual void ConstructState(void* m) const {
		new (m) PHContactDetectorState(NSolidPairs(), NShapePairs());
	}
	virtual void DestructState(void* m) const {
		((PHContactDetectorState*)m)->~PHContactDetectorState();
	}
	virtual bool GetState(void* s) const {
		PHContactDetectorState* es = ((PHContactDetectorState*)s);
		PHSolidPairState* solidStates = es->SolidStates();
		CDShapePairState* shapeStates = es->ShapeStates();
		//	solidPairs.item(i,j)　の i<j部分を使っているのでそこだけ保存
		int solidPos=0;
		int shapePos=0;
		TSolidPair* sp;
		for(int j=0; j<solidPairs.width(); ++j){
			for(int i=0; i<j; ++i){
				sp = solidPairs.item(i, j);
				solidStates[solidPos] = *(PHSolidPairState*)sp;
				++solidPos;
				for(int r = 0; r < sp->shapePairs.height(); ++r){
					for(int c = 0; c < sp->shapePairs.width(); ++c){
						shapeStates[shapePos] = *(CDShapePairState*)(sp->shapePairs.item(r, c));
						++shapePos;
					}
				}
			}
		}
		return true;
	}
	virtual void SetState(const void* s){
		PHContactDetectorState* es = (PHContactDetectorState*)s;
		PHSolidPairState* solidStates = es->SolidStates();
		CDShapePairState* shapeStates = es->ShapeStates();
		//	solidPairs.item(i,j)　の i<j部分を使っているのでそこだけ保存
		int solidPos=0;
		int shapePos=0;
		TSolidPair* sp;
		for(int j=0; j<solidPairs.width(); ++j){
			for(int i=0; i<j; ++i){
				sp = solidPairs.item(i, j);
				sp->SetState(solidStates[solidPos]);
				++solidPos;
				for(int r = 0; r < sp->shapePairs.height(); ++r){
					for(int c = 0; c < sp->shapePairs.width(); ++c){
						sp->shapePairs.item(r, c)->SetState(shapeStates[shapePos]);
						++shapePos;
					}
				}
			}
		}
	}

	///< SolidにShapeが追加されたときにSolidから呼ばれる
	void UpdateShapePairs(PHSolid* solid){
		int isolid = (solids.Find(solid) - solids.begin());
		int i, j;
		TSolidPair* sp;
		PHSolid *slhs, *srhs;
		for(i = 0; i < isolid; i++){
			sp = solidPairs.item(i, isolid);
			slhs = sp->solid[0]->solid;
			sp->shapePairs.resize(slhs->NShape(), solid->NShape());
			for(j = 0; j < slhs->NShape(); j++)
				sp->shapePairs.item(j, solid->NShape()-1) = DBG_NEW TShapePair();
		}
		for(i = isolid+1; i < (int)solids.size(); i++){
			sp = solidPairs.item(isolid, i);
			srhs = sp->solid[1]->solid;
			sp->shapePairs.resize(solid->NShape(), srhs->NShape());
			for(j = 0; j < srhs->NShape(); j++)
				sp->shapePairs.item(solid->NShape()-1, j) = DBG_NEW TShapePair();
		}
	}

	/// bEnabledフラグがtrueなSolidPairが存在するか
	bool ActiveSolidPairExists(){
		bool yes = false;
		int n = (int)solids.size();
		for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
			yes |= solidPairs.item(i, j)->bEnabled;
		return yes;
	}

	virtual void EnableContact(PHSolidIf* lhs, PHSolidIf* rhs, bool bEnable){
		typename PHSolidInfos<TSolidInfo>::iterator ilhs = solids.Find((PHSolid*)lhs), irhs = solids.Find((PHSolid*)rhs);
		if(ilhs == solids.end() || irhs == solids.end())
			return;
		int i = ilhs - solids.begin(), j = irhs - solids.begin();
		if(i > j)std::swap(i, j);
		assert(i < solidPairs.height() && j < solidPairs.width());
		solidPairs.item(i, j)->bEnabled = bEnable;
		bContactEnabled = ActiveSolidPairExists();
	}

	virtual void EnableContact(PHSolidIf** group, size_t length, bool bEnable){
		std::vector<int> idx;
		typename PHSolidInfos<TSolidInfo>::iterator it;
		for(int i = 0; i < (int)length; i++){
			it = solids.Find((PHSolid*)group[i]);
			if(it != solids.end())
				idx.push_back(it - solids.begin());
		}
		sort(idx.begin(), idx.end());
		for(int i = 0; i < (int)idx.size(); i++){
			for(int j = i+1; j < (int)idx.size(); j++){
				solidPairs.item(idx[i], idx[j])->bEnabled = bEnable;
			}
		}
		bContactEnabled = ActiveSolidPairExists();
	}

	virtual void EnableContact(PHSolidIf* solid, bool bEnable){
		typename PHSolidInfos<TSolidInfo>::iterator it = solids.Find((PHSolid*)solid);
		if(it == solids.end())
			return;
		int idx = it - solids.begin();
		for(int i = 0; i < idx; i++)
			solidPairs.item(i, idx)->bEnabled = bEnable;
		for(int i = idx+1; i < (int)solids.size(); i++)
			solidPairs.item(idx, i)->bEnabled = bEnable;
		bContactEnabled = ActiveSolidPairExists();
	}

	virtual void EnableContact(bool bEnable){
		int n = (int)solids.size();
		for(int i = 0; i < n; i++)for(int j = i+1; j < n; j++)
			solidPairs.item(i, j)->bEnabled = bEnable;
		bContactEnabled = bEnable;
	}

	///< 全体の交差の検知
	bool Detect(unsigned int ct, double dt){
		/* 以下の流れで交差を求める
			1. SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外
			2. 各Solidの組について
				2a. ShapeのBBoxレベルでの交差判定 (未実装)
				2b. 各Shapeの組についてGJKで交差形状を得る
				2c. 交差形状から法線を求め、法線に関して形状を射影し，その頂点を接触点とする
				2d. 得られた接触点情報をPHContactPointsに詰めていく
		*/		
		int N = solids.size();

		//1. BBoxレベルの衝突判定
		Vec3f dir(0,0,1);
		Edges edges;
		edges.resize(2 * N);
		typename Edges::iterator eit = edges.begin();
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
		for(typename Edges::iterator it = edges.begin(); it != edges.end(); ++it){
			if (it->bMin){						//	初端だったら，リスト内の物体と判定
				for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
					int f1 = it->index;
					int f2 = *itf;
					if (f1 > f2) std::swap(f1, f2);
					//2. SolidとSolidの衝突判定
					found |= solidPairs.item(f1, f2)->Detect((TEngine*)this, ct, dt);
				}
				cur.insert(it->index);
			}else{
				cur.erase(it->index);			//	終端なので削除．
			}
		}
		return found;
	}

};

}

#endif

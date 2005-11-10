#ifndef CDCOLLISIONENGINE_H
#define CDCOLLISIONENGINE_H

#pragma once
#include <SceneGraph/SGBehaviorEngine.h>
#include <Collision/CDFramePair.h>
#include <Collision/CDDetectorImp.h>
#include <WinBasis/WBPreciseTimer.h>

namespace Spr{;


///	衝突時のリスナー(コールバック)クラス．
class CDCollisionListener{
public:
	///	すべての前に一度呼び出させるコールバック．
	virtual void BeforeAll(SGScene* scene){}

	///	衝突判定後，解析前に呼び出されるコールバック．接触しているフレーム対の数だけ呼び出される．
	virtual void Before(SGScene* scene, CDFramePairWithRecord* fr){}
	///	解析直後，交差部の面や頂点が有効なときに，接触している凸形状対の数だけ呼び出されるコールバック．
	virtual void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){}
	///	解析後に呼び出されるコールバック．接触しているフレーム対の数だけ呼び出される．
	virtual void After(SGScene* scene, CDFramePairWithRecord* fr){}

	///	すべての後に一度呼び出させるコールバック．
	virtual void AfterAll(SGScene* scene){}
};
///	リスナのvector
class CDCollisionListeners:public std::vector<CDCollisionListener*>{
public:
	///	衝突判定前に一度呼び出させるコールバック．
	void BeforeAll(SGScene* scene){
		for(unsigned  i=0; i<size(); ++i){
			at(i)->BeforeAll(scene);
		}
	}
	///	衝突判定後，解析前に呼び出されるコールバック．接触しているフレーム対の数だけ呼び出される．
	void Before(SGScene* scene, CDFramePairWithRecord* fr){
		for(unsigned  i=0; i<size(); ++i){
			if (fr->IsActive(i)) begin()[i]->Before(scene, fr);
		}
	}
	///	解析直後，交差部の面や頂点が有効なときに，接触している凸形状対の数だけ呼び出されるコールバック．
	void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){
		for(unsigned  i=0; i<size(); ++i){
			if (fr->IsActive(i)) begin()[i]->Analyzed(scene, fr, geom, conv, analyzer);
		}
	}
	///	解析後に呼び出されるコールバック．接触しているフレーム対の数だけ呼び出される．
	void After(SGScene* scene, CDFramePairWithRecord* fr){
		for(unsigned  i=0; i<size(); ++i){
			if (fr->IsActive(i)) begin()[i]->After(scene, fr);
		}
	}
	void AfterAll(SGScene* scene){
		for(unsigned  i=0; i<size(); ++i){
			at(i)->AfterAll(scene);
		}
	}
};

/**	衝突判定エンジン．
	衝突しているペアを見つけ，衝突判定を行い，コールバックを呼び出す．	*/
class CDCollisionEngine: public SGBehaviorEngine
{
protected:
	///	アクティブ／非アクティブなリスナの指定
	struct ActivePair{
		int pos;		///<	リスナの番号
		int frame[2];	///<	フレームのペア
	};
	typedef std::vector<ActivePair> TActivePairs;
	TActivePairs activeList;	///<	衝突判定を行う（リスナを呼び出す）ペアのリスト(デフォルトでは行わない)
	TActivePairs inactiveList;	///<	衝突判定を行わない（リスナを呼びさない）ペアのリスト(デフォルトでは行う)
	std::vector<bool> defaults;	///<	デフォルトで衝突判定を行う（リスナを呼び出す）かどうかのフラグ
	
	///	衝突判定の一覧．item(i,j) (i<j) でアクセスする．
	CDFramePairWithRecords pairs;
	///	接触解析エンジン．交差部の面や頂点の情報を持っている．
	CDContactAnalysis analyzer;
	///	フレームごとのユーザレコードの数
	int nFrameRecords;
	///	フレームペアごとのユーザレコードの数
	int nFramePairRecords;
	///	凸形状のペアごとのユーザレコードの数
	int nConvexPairRecords;
	///	判定対象フレームのコンテナ型
	typedef std::vector< UTRef<CDFrame> > Frames;
	///	衝突判定対象のフレーム
	Frames frames;
	///	衝突のリスナ
	CDCollisionListeners listeners;
	///	フレームとフレーム番号の対応表

	void Render(GRRender* render, SGScene* scene);
	
	std::map<SGFrame*, int> frameMap;

public:
	///	フレーム対のためのイタレータ
	typedef CDFramePairWithRecords::iterator TFrameIt;

	struct GetGeom{
		CDGeometryPairs* operator()(TFrameIt it){ return *it ? &(*it)->geometryPairs : NULL; }
	};
	///	ジオメトリ対のためのイタレータ
	class TGeometryIt: public UTComposedIterator<CDGeometryPair*, TFrameIt, CDGeometryPairs::iterator, GetGeom>{
		friend class CDCollisionEngine;
		friend class TConvexIt;
	public:
		TGeometryIt(){}
		TGeometryIt(CDFramePairWithRecords::iterator b, CDFramePairWithRecords::iterator e, CDGeometryPairs::iterator g){
			Init(b, e, g);
		}
	};

	
	struct GetConv{
		CDConvexPairs* operator()(TGeometryIt it){ return &it->convexPairs; }
	};
	///	凸形状対のためのイタレータ
	class TConvexIt: public UTComposedIterator<CDConvexPairWithRecord*, TGeometryIt, CDConvexPairs::iterator, GetConv>{
		friend class CDCollisionEngine;
		TConvexIt(){}
		TConvexIt(TGeometryIt b, TGeometryIt e, CDConvexPairs::iterator c){
			Init(b, e, c);
		}
	};

public:
	WBPreciseTimer timerGjk, timerQhull, timerNormal;
	///
	SGOBJECTDEF(CDCollisionEngine);
	///
	CDCollisionEngine();

	///@name	BehaviorEngineをオーバーライド
	//{
	///
	int GetPriority() const { return SGBP_COLLISIONENGINE; }
	///	時間を dt 進める
	virtual void Step(SGScene* s);
	///	判定対象のフレームをクリアする．
	virtual void Clear(SGScene* s);
	///	初期化(frames から pairsを作る)
	void Init();
	//}

	//@name	リスナや判定対象のフレームと関連付けられたデータのアクセス
	//@{
	///	リスナを返す．
	CDCollisionListener* GetListener(int pos){ return listeners[pos]; }
	///	リスナの数
	size_t NListener(){ return listeners.size(); }

	///	フレームの番号を返す．
	int GetFrameID(SGFrame* fr){ return frameMap[fr]; }
	///	フレーム対を返す．f1 < f2 でなければならない．
	CDFramePairWithRecord* GetFramePair(int f1, int f2){
		return pairs.item(f1, f2);
	}
	///	フレーム対を返す．(f1,f2)と(f2,f1)どちらは向こう．
	CDFramePairWithRecord* GetFramePair(SGFrame* f1, SGFrame*f2){ return GetFramePair(GetFrameID(f1), GetFrameID(f2)); }
	///	フレームIDからフレームを返す．
	SGFrame* GetFrame(int id){ return frames[id]->frame; }
	///	フレームIDとレコードの位置(ReserveFrameRecord()の返り値)からレコードを返す．
	CDUserRecord* GetFrameRecord(int id, int pos){ return frames[id]->records[pos]; }
#if defined _MSC_VER && _MSC_VER >= 1300
	///	フレーム対のイタレータの始点
	TFrameIt FramePairBegin() const { return (TFrameIt&)pairs.begin(); }
	///	フレーム対のイタレータの終点
	TFrameIt FramePairEnd() const { return (TFrameIt&)pairs.end(); }
#else
	///	フレーム対のイタレータの始点
	TFrameIt FramePairBegin() const { return (TFrameIt)pairs.begin(); }
	///	フレーム対のイタレータの終点
	TFrameIt FramePairEnd() const { return (TFrameIt)pairs.end(); }
#endif
	///	ジオメトリ対のイタレータの始点
	TGeometryIt GeometryPairBegin() const{
		TGeometryIt rv;
		rv.InitAsBegin(FramePairBegin(), FramePairEnd());
		return rv;
	}
	///	ジオメトリ対のイタレータの終点
	TGeometryIt GeometryPairEnd() const{
		TGeometryIt rv;
		rv.InitAsEnd(FramePairBegin(), FramePairEnd());
		return rv;
	}
	///	凸形状対のイタレータの始点
	TConvexIt ConvexPairBegin() const {
		TConvexIt rv;
		rv.InitAsBegin(GeometryPairBegin(), GeometryPairEnd());
		return rv;
	}
	///	凸形状対のイタレータの終点
	TConvexIt ConvexPairEnd() const{
		TConvexIt rv;
		rv.InitAsEnd(GeometryPairBegin(), GeometryPairEnd());
		return rv;
	}
	//}
	
	///@name	登録，追加系のメソッド
	//{
	///	判定対象フレームを追加．すべての判定対象フレームは同階層になければならない．
	void AddFrame(SGFrame* frame, CDUserRecord* rec=NULL, int pos=-1);
	///	フレームの数
	size_t NFrame(){ return frames.size(); }
	///	リスナーの登録
	int AddListener(CDCollisionListener* l);
	///	衝突判定をしないペアを登録（デフォルトでは衝突判定する）
	bool AddInactive(int f1, int f2, int pos);
	///	衝突判定をしないペアを登録（デフォルトでは衝突判定する）
	bool AddInactive(SGFrame* f1, SGFrame* f2, int pos){ return AddInactive(GetFrameID(f1), GetFrameID(f2), pos); }
	///	posのリスナについて判定しないように登録したペア(InactiveList)をクリアする
	int ClearInactive(int pos);
	///	衝突判定をするペアを登録（pos のリスナについては，デフォルトでは判定しなくなる．）
	bool AddActive(int f1, int f2, int pos);
	///	衝突判定をするペアを登録（pos のリスナについては，デフォルトでは判定しなくなる．）
	bool AddActive(SGFrame* f1, SGFrame* f2, int pos){ return AddActive(GetFrameID(f1), GetFrameID(f2), pos); }
	///	posのリスナについて判定するように登録したペア(activeList)をクリアする
	int ClearActive(int pos);
	///	フレーム(CDFrame) ごとのユーザレコード(records) の場所取り．
	int ReserveFrameRecord();
	///	フレームのペア(CDFramePairWithRecord) ごとのユーザレコード(records) の場所取り．
	int ReserveFramePairRecord();
	///	凸形状のペア(CDConvexPairWithRecord) ごとのユーザレコードの場所取り．
	int ReserveConvexPairRecord();
	//}

	void RayCheck(Vec3f from, Vec3f dir);
	
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
};

}

#endif


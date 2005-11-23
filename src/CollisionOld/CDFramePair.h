#ifndef CDDETECTOR_H
#define CDDETECTOR_H
#include "CDConvex.h"
#include <SceneGraph/SGFrame.h>
#include <Base/Combination.h>
#include "CDQuickHull3D.h"

namespace Spr {;

template <class T, class ITI, class ITJ, class GetContainer>
class UTComposedIterator{
public:
	typedef UTComposedIterator<T, ITI, ITJ, GetContainer> self_type;
protected:
	ITI iti, iEnd;
	ITJ itj;
public:
	self_type(){}
	T operator->(){ return (T&)*itj; }
	T operator*(){ return (T&)*itj; }
	self_type& operator ++(int){ Next(); return *this; }
	self_type operator ++(){ self_type rv(*this); Next(); return rv; }
	bool operator ==(const self_type& i) const{ return i.iti == iti && i.itj == itj; }
	bool operator !=(const self_type& i) const {return !(i==*this);}
	void Init(ITI b, ITI e, ITJ j){
		while (b!=e && !GetContainer()(b)) ++b;
		iti = b;
		iEnd = e;
		itj = j;
	}
	void InitAsBegin(ITI b, ITI e){
		Init(b, e, NULL);
		if (b!=e) itj = GetContainer()(iti)->begin();
		else return;
		while (itj==GetContainer()(iti)->end()){
			do{
				++iti;
				if (iti == iEnd){
					itj=NULL;
					break;
				}
			} while(!GetContainer()(iti));
			itj=GetContainer()(iti)->begin();
		}
	}
	void InitAsEnd(ITI b, ITI e){
		Init(e, e, 0);
	}
	void Next(){
		assert(itj != NULL);
		++itj;
		if (itj == GetContainer()(iti)->end()){
			do{
				++iti;
			}while(iti != iEnd && (!GetContainer()(iti) || !GetContainer()(iti)->size()));
			if (iti != iEnd){
				itj = GetContainer()(iti)->begin();
			}else{
				itj = NULL;
			}
		}
	}
};


///	凸形状のペア
class CDConvexPair:public UTRefCount{
public:
	virtual ~CDConvexPair(){}
	UTRef<CDConvex> convex[2];
	Vec3d closestPoint[2]; // ローカル座標系
	Vec3d commonPoint;     // グローバル座標系
	unsigned int lastContactCount;
	enum State{
		NEW,
		CONTINUE,
	} state;
	void Set(CDConvex* c1, CDConvex* c2){
		convex[0] = c1;
		convex[1] = c2;
	}
	CDConvexPair():lastContactCount(-2){}
};
///	凸形状ペアの全組み合わせ
class CDConvexPairs:public UTCombination< UTRef<CDConvexPair> >{
public:
};
class CDFramePair;
///	ジオメトリのペア
class CDGeometryPair:public UTRefCount{
public:
	UTRef<CDGeometry> geometry[2];	///<	ジオメトリ
	UTRef<SGFrame> frame[2];		///<	ジオメトリが属すフレーム
	Affinef posture[2];				///<	フレームの基準フレームからの姿勢(ワークエリア)
	CDConvexPairs convexPairs;		///<	2つのジオメトリを構成する凸形状のすべてのペア
	void Set(CDFramePair* fp, CDGeometry* g1, SGFrame* f1, CDGeometry* g2, SGFrame* f2);
	virtual ~CDGeometryPair(){}
};
///	ジオメトリのペアの全組み合わせ
class CDGeometryPairs:public UTCombination< UTRef<CDGeometryPair> >{
public:
};

///	衝突判定結果
class CDIntersection{
public:
	CDConvexPair* convexPair;
	CDGeometryPair* geometryPair;
	CDIntersection(CDConvexPair* cp, CDGeometryPair* gp): convexPair(cp), geometryPair(gp){}
};
///	衝突判定結果の配列
	class CDIntersections:public std::vector<CDIntersection>{
};

	
///	ユーザレコードの基本クラス
typedef SGObject CDUserRecord;

///	ユーザレコードのコンテナ型
class CDUserRecords:public std::vector< UTRef<CDUserRecord> >{
public:
};
///	衝突判定対象フレーム．	フレームとユーザレコードを持つ
class CDFrame:public UTRefCount{
public:
	CDFrame(){}
	CDFrame(SGFrame* f): frame(f){}
	CDUserRecords records;
	UTRef<SGFrame> frame;
};
/**	衝突判定対象のフレームのペア.
フレームは同じ階層でなければならない(親やそのPostureは考慮しない)．

	フレームの所有関係
	SGFrame
	 +-子SGFrame
	 +-CDMesh
		    +-CDPolyhedron
	フレーム(SGFrame)は，複数のジオメトリ(CDGeometries)と子フレームを持つ．
	ジオメトリ(CDGeometry) の中には，複数のジオメトリを持つもの(CDMesh)
	と単純な凸形状(CDPolyhedron)がある．
	CDMesh は複数のConvexを持つ．
*/
class CDFramePair{	
public:
	///	リスナがアクティブかどうかのフラグ．
	class CDIsActive: public std::vector<bool>{
		bool bOr;
	public:
		void Update(){
			bOr = false;
			for(unsigned i=0; i<size(); ++i){
				bOr |= at(i);
			}
		}
		bool GetOr(){ return bOr; }
	};
	struct GetConv{
		CDConvexPairs* operator()(CDGeometryPairs::iterator it){ return *it ? &(*it)->convexPairs : NULL; }
	};
	///	凸形状のペアを列挙するためのイタレータ
	struct CDConvexPairIt: UTComposedIterator<CDConvexPair*, CDGeometryPairs::iterator, CDConvexPairs::iterator, GetConv>{
		friend class CDCollisionEngine;
		CDConvexPairIt(){}
		CDConvexPairIt(CDGeometryPairs::iterator b, CDGeometryPairs::iterator e, CDConvexPairs::iterator c){
			Init(b, e, c);
		}
	};

	///	最後に接触した時刻
	unsigned lastContactCount;
	///	アクティブかどうかのフラグたち．CDCollisionEngineのリスナの数だけフラグがある．
	CDIsActive isActive;
	///	判定対象フレーム
	UTRef<CDFrame> frame[2];
	///	衝突したペアだけを並べた配列
	CDIntersections intersections;
	///	ジオメトリのペアの全組み合わせ
	CDGeometryPairs geometryPairs;	

	///
	CDFramePair();
	///	フレームのセット
	void Set(CDFrame* f1, CDFrame* f2);
	///	全体がアクティブかどうか
	bool IsActive(){ return isActive.GetOr(); }
	///	リスナ i がアクティブかどうか
	bool IsActive(int i){ return isActive[i]; }
	///	クリア
	void Clear(){
		frame[0] = frame[1] = NULL;
		geometryPairs.clear();
	}
	///	共有点を探す
	bool Detect(int count);
	///	接触しているかどうか
	bool IsContact(SGScene* scene);

	///	凸形状対の列挙
	CDConvexPairIt ConvexPairBegin(){
		CDConvexPairIt rv;
		rv.InitAsBegin(geometryPairs.begin(), geometryPairs.end());
		return rv;
	}
	CDConvexPairIt ConvexPairEnd(){
		CDConvexPairIt rv;
		rv.InitAsEnd(geometryPairs.begin(), geometryPairs.end());
		return rv;
	}
protected:

	///	接触時のコールバック
	virtual void Found(CDConvexPair& cp, CDGeometryPair& gp){
		intersections.push_back(CDIntersection(&cp, &gp));
	}
	/**	CDConvexPairの派生クラスを使うための仕組み．
		このクラスを継承して，この関数をオーバーライドすることで，
		CDConvexPairの代わりに派生クラスを使用できる．	*/
	virtual CDConvexPair* CreateConvexPair(){
		return new CDConvexPair;
	}
	/**	CDGeometryPairの派生クラスを使うための仕組み．
		このクラスを継承して，この関数をオーバーライドすることで，
		CDGeometryPairの代わりに派生クラスを使用できる．	*/
	virtual CDGeometryPair* CreateGeometryPair(){
		return new CDGeometryPair;
	}
private:
	///	フレームが持つジオメトリを列挙．
	void EnumGeometry(SGFrames& frames, CDGeometries& geoms, SGFrame* f);
	friend class CDGeometryPair;
};
///	法線情報つきのConvexPair
class CDConvexPairWithNormal: public CDConvexPair{
public:
	Vec3d normal;				///<	衝突の法線(0から1へ) (Global)
	Vec3d iNormal;				///<	積分による法線
	Vec3d center;				///<	2つの最侵入点の中間の点，CDContactAnalysis::CalcNormal が更新する．
	double depth;				///<	衝突の深さ：最近傍点を求めるために，2物体を動かす距離．
};
///	法線情報つきのFramePair
class CDFramePairWithNormal :public CDFramePair{
protected:
	virtual CDConvexPair* CreateConvexPair(){
		return new CDConvexPairWithNormal;
	}
};

///	ユーザレコードと法線を持つConvexPair
class CDConvexPairWithRecord: public CDConvexPairWithNormal{
public:
	CDUserRecords records;
};
///	CDConvexPairWithRecordのためのCDFramePair レコードを持つ．
class CDFramePairWithRecord :public CDFramePairWithNormal, public UTRefCount{
public:
	CDUserRecords records;
protected:
	virtual CDConvexPair* CreateConvexPair(){
		return new CDConvexPairWithRecord;
	}
};
///	衝突判定の対象のフレーム対．
class CDFramePairWithRecords: public UTCombination< UTRef<CDFramePairWithRecord> >{
public:
};

}
#endif

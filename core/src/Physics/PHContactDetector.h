/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONTACTDETECTOR_H
#define PHCONTACTDETECTOR_H

#include <vector>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Physics/PHSolid.h>

namespace Spr{;

class PHContactDetector;
class PHSolidPair;

class PHShapePair : public CDShapePair{
public:
	SPR_OBJECTDEF_ABST_NOIF(PHShapePair);

	PHSolidPair* solidPair;
	PHFrame*     frame[2];

public:
	void Init (PHSolidPair* sp, PHFrame* fr0, PHFrame* fr1);
	void SetSt(const CDShapePairState& s){ *((CDShapePairState*)this) = s; }
	void GetSt(      CDShapePairState& s){ s = *this; }
	PHFrameIf* GetFrame(int i) { return (PHFrameIf*)frame[i]; }
	PHShapePair& operator = (const PHShapePair& src);
};


/// 剛体の組の状態
struct PHSolidPairSt{
	bool bEnabled;
	PHSolidPairSt():bEnabled(false){}
};

struct PHSolidPairVars: PHSolidPairSt {
	PHContactDetector* detector;
	PHBody* body[2];
};

/// 剛体の組
class PHSolidPair : public PHSolidPairVars, public Object{
public:
	SPR_OBJECTDEF_ABST_NOIF(PHSolidPair);
	typedef UTCombination< UTRef<PHShapePair> > PHShapePairs;
	PHShapePairs shapePairs;
	typedef std::vector<PHCollisionListener*> Listeners;
	Listeners listeners;

	virtual ~PHSolidPair(){}
	
	/// 派生クラスが実装する関数
	virtual PHShapePair* CreateShapePair() = 0;
	virtual void OnDetect(PHShapePair* sp, unsigned ct, double dt) {	///< 交差が検知されたときの処理
		for (Listeners::iterator it = listeners.begin(); it != listeners.end(); ++it) {
			(*it)->OnDetect((PHSolidPairIf*)this, (CDShapePairIf*)sp, ct, dt);
		}
	}
	virtual void OnContDetect(PHShapePair* sp, unsigned ct, double dt) {	///< 交差が検知されたときの処理
		for (Listeners::iterator it = listeners.begin(); it != listeners.end(); ++it) {
			(*it)->OnContDetect((PHSolidPairIf*)this, (CDShapePairIf*)sp, ct, dt);
		}
	}
	void Init  (PHContactDetector* d, PHBody* b0, PHBody* b1);
	bool Detect(unsigned int ct, double dt);
	bool ContDetect(unsigned int ct, double dt);
	bool Detect(PHShapePair* shapePair, unsigned ct, double dt, bool continuous);

	void SetSt(const PHSolidPairSt& s){ *((PHSolidPairSt*)this) = s; }
	void GetSt(      PHSolidPairSt& s){ s = *this; }

	PHSolidIf* GetSolid(int i) { return body[i]->Cast(); }			///< 指定したsolidを返す
	PHBodyIf* GetBody(int i) { return body[i]->Cast(); }			///< 指定したbodyを返す

	/// 剛体同士の接触が有効かどうかを取得・設定する
	bool IsContactEnabled()         { return bEnabled;   }
	void EnableContact(bool enable) { bEnabled = enable; }

	///	Listener
	int NListener() { return (int) listeners.size();  }
	PHCollisionListener* GetListener(int i) { return listeners[i];  }
	void RemoveListener(int i) { listeners.erase(listeners.begin() + i); }
	void AddListener(PHCollisionListener*l, int pos=-1){
		if (pos == -1) listeners.push_back(l);
		else listeners.insert(listeners.begin() + pos, l);
	}
};

///	PHContactDetectorの状態
struct PHContactDetectorSt{
	size_t nSolidPair;	///<	SolidPairの数
	size_t nShapePair;	///<	ShapePairの数

	PHSolidPairSt* SolidStates(){
		char* ptr = ((char*)this) + sizeof(*this);
		return (PHSolidPairSt*)ptr;
	}
	CDShapePairState* ShapeStates(){
		char* ptr = ((char*)this) + sizeof(*this) + nSolidPair*sizeof(PHSolidPairSt);
		return (CDShapePairState*)ptr;
	}
	size_t GetSize(){
		return sizeof(*this) + nSolidPair*sizeof(PHSolidPairSt) + nShapePair*sizeof(CDShapePairState);
	}
	PHContactDetectorSt(int n=0, int m=0):nSolidPair(n), nShapePair(m){}
};


class PHContactDetector : public PHEngine{
public:
	typedef UTCombination< UTRef<PHSolidPair> > PHSolidPairs;

	struct ShapeIndex{
		int		idxBody;
		int		idxShape;
		bool	center;		///< 中心を含むか

		ShapeIndex(){}
		ShapeIndex(int i, int j, bool c):idxBody(i), idxShape(j), center(c){}
	};
	
	struct Edge{
		float      edge;		///< 端の位置
		bool       bMin;		///< 初端か 
		int		index;		///< 剛体のID
		
		bool operator < (const Edge& s) const { return edge < s.edge; }

		Edge(){}
		Edge(int i, float e, bool _min): edge(e), bMin(_min), index(i){}
	};
	
	struct Cell{
		PHBBox		bbox;

		std::vector<ShapeIndex>	shapes;
		
		void Add(int i, int j, bool c){
			shapes.push_back(ShapeIndex(i,j,c));
		}
	};

	Vec3i				numDivision;
	PHBBox				regionBBox;
	PHBBox				cellBBox;
	Cell				cellOutside;
	std::vector<Cell>	cells;
	std::vector<Edge>   edges;
	
	PHBodies			bodies;				///< 物体の配列
	PHBodies			inactiveBodies;		///< 接触判定しない物体の集合
	PHSolidPairs		solidPairs;			///< 剛体の組の配列	
	int					nBroad;
	int					nNarrow;
#ifdef _DEBUG
	int nMaxOverlapObject;
#endif

	Cell&	GetCell(int ix, int iy, int iz){
		return cells[(ix * numDivision.y + iy) * numDivision.z + iz];
	}
	
public:
	PHContactDetector();

	/// Objectの仮想関数
	virtual void   Clear();
	virtual bool   AddChildObject(ObjectIf* o);		///< 剛体の追加
	virtual bool   DelChildObject(ObjectIf* o);		///< 剛体の削除
	virtual size_t GetStateSize  ()        const;
	virtual void   ConstructState(void* m) const;
	virtual void   DestructState (void* m) const;
	virtual bool   GetState      (void* s) const;
	virtual void   SetState      (const void* s);

	/// 派生クラスが実装する関数
	virtual PHSolidPair* CreateSolidPair(){ return NULL; }

	void AddInactiveSolid(PHBodyIf* body);	///< 解析法を適用しない剛体の追加
	bool IsInactiveSolid (PHBodyIf* body);	///< 解析法を適用しない剛体の検索

	int NSolidPairs      () const;
	int NShapePairs      () const;
	int NActiveSolidPairs() const;
	int NActiveShapePairs() const;
	
	void UpdateShapePairs(PHBody* body);							///< 形状追加時の処理
	void DelShapePairs   (PHBody* body, int iBegin, int iEnd);	///< 形状削除時の処理
	
	void SetDetectionRange(Vec3f center, Vec3f extent, int nx, int ny, int nz);		///< 衝突判定の対象範囲

	/// 剛体同士の衝突判定に有効/無効を設定
	void EnableContact(PHSolidIf*  lhs, PHSolidIf* rhs, bool bEnable);
	void EnableContact(PHSolidIf** group, size_t length, bool bEnable);
	void EnableContact(PHSolidIf*  solid, bool bEnable);
	void EnableContact(bool bEnable);

	/// 形状対の交差の検知
	bool DetectPair   (ShapeIndex sh0, ShapeIndex sh1, unsigned ct, double dt, bool continuous);

	/// 全体の交差の検知
	bool Detect       (unsigned ct, double dt, int mode, bool continuous);
	
};

inline bool operator<(const PHContactDetector::ShapeIndex& lhs, const PHContactDetector::ShapeIndex& rhs){
	return lhs.idxBody < rhs.idxBody || (lhs.idxBody == rhs.idxBody && lhs.idxShape < rhs.idxShape);
}

}

#endif

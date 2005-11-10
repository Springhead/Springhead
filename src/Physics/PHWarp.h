#ifndef PHWARP_H
#define PHWARP_H

#include <Collision/CDFramePair.h>
#include <Collision/CDCollisionEngine.h>
#include "PHSolid.h"

namespace Spr{;

/**	指定した剛体（複数指定可）がワープゾーンに衝突したときワープさせる．
    例えば，ワープゾーンフレームを床の下にひろーく作っておけば，
    床から落ちたオブジェクトを床の上方にワープさせるということが可能．
	PHWarpEngine が持つ．	*/
class PHWarp:public SGObject{
	SGOBJECTDEF(PHWarp);
protected:

public:
	Vec3f position;					///<	ワープ先の位置								
	Vec3f velocity;					///<	ワープ先での速度
	Vec3f angVel;					///<	ワープ先での角速度
	UTRef<SGFrame> warpZoneFrame;	///<	ワープゾーン
	PHSolids solids;				///<	ワープさせる剛体たち

	///	クリア
	void Clear();
	///	
	bool AddChildObject(SGObject* o, SGScene* s);

};

///	PHWarpの配列
class PHWarps:public std::vector< UTRef<PHWarp> >{
};


/** ワープエンジン */
class PHWarpEngine:public SGBehaviorEngine, CDCollisionListener {
	SGOBJECTDEF(PHWarpEngine);

public:
	/// フレーム対ごとのワープデータ
	class FramePairRecord:public CDUserRecord{
	public:
		Vec3f position;				///<	ワープ先の位置								
		Vec3f velocity;				///<	ワープ先での速度
		Vec3f angVel;				///<	ワープ先での角速度
		UTRef<PHSolid> solid[2];	///<	ワープさせる剛体
		int frameID[2];				///<	フレーム対のID
	};


public:
	/// PHWarpEngine の呼ばれる優先順位
	int GetPriority() const { return SGBP_WARPENGINE; }
	///
	bool AddChildObject(SGObject* o, SGScene* s);
	/// 接触時に呼ばれるコールバック．ワープをさせる処理を行う．
	void After(SGScene* scene, CDFramePairWithRecord* fr);
	///	時間を dt 進める（何もしない，Listener の Collide がすべての処理をする）
	virtual void Step(SGScene* s){}
	/// クリア
	virtual void Clear(SGScene* s);
	/// ロード完了時の処理，CollisionEngine にコールバックを登録する．CollisionEngine がなければ作る．
	virtual void Loaded(SGScene* scene);
	/// FramePairRecord の登録
	void AddFramePairRecord(PHWarp* w, int f1, int f2, PHSolid* s1, PHSolid* s2=NULL);

	PHWarps warps;				///<	ワープの情報
protected:
	int listenerPos;			///< リスナの位置
	int framePairRecordPos;		///< ワープエンジン用の FramePairRecord の位置
	std::vector< UTRef<FramePairRecord> > framePairRecords;	///< フレーム対ごとのワープデータ
};


};
#endif

#pragma once
#include "PHSolid.h"
#include <SceneGraph/SGScene.h>
#include <Physics/PHConvexPair.h>
#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>

namespace Spr{;


class PHStick: public SGObject{
public:
	SGOBJECTDEF(PHStick);
	UTRef<SGFrame> frame[2];
	float force;
	bool AddChildObject(SGObject* o, SGScene* s);
};
class PHContactEngine;
/**	くっつきエンジン	*/
class PHStickEngine:public SGBehaviorEngine, CDCollisionListener{
public:
protected:
	int listenerPos;
	int framePairRecordPos;
	PHContactEngine* contactEngine;
public:	
	///	くっつくリスト
	typedef std::vector< UTRef<PHStick> > Sticks;
	Sticks sticks;

	SGOBJECTDEF(PHStickEngine);
	bool AddChildObject(SGObject* o, SGScene* s);
	void After(SGScene* scene, CDFramePairWithRecord* fr);
	///
	int GetPriority() const { return SGBP_STICKENGINE; }
	///	時間を dt 進める．(何もしない，Listener の Collide がすべての処理をする．)
	virtual void Step(SGScene* s){}
	///	sticksをクリアする．
	virtual void Clear(SGScene* s);
	///	ロード完了時の処理，CollisionEngine にコールバックを登録する．CollisionEngineがなければ作る．
	virtual void Loaded(SGScene* scene);
};


}

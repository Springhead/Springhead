#include "Physics.h"
#pragma hdrstop
#include "PHWarp.h"

namespace Spr{;

//----------------------------------------------------------------------------
//	PHWarp
//

SGOBJECTIMP(PHWarp, SGObject);

void PHWarp::Clear(){
	warpZoneFrame = NULL;
	solids.clear();
	position.clear();
	velocity.clear();
	angVel.clear();
}

bool PHWarp::AddChildObject(SGObject* o, SGScene* scene){
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	if (DCAST(SGFrame, o)){
		assert(!warpZoneFrame);
		warpZoneFrame = (SGFrame*)o;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
//	PHWarpEngine
//

SGOBJECTIMP(PHWarpEngine, SGBehaviorEngine);

void PHWarpEngine::Clear(SGScene* s){
	listenerPos = -1;
	framePairRecordPos = -1;
	warps.clear();
}

bool PHWarpEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHWarp, o)){
		warps.push_back((PHWarp*)o);
		return true;
	}
	return false;
}

void PHWarpEngine::AddFramePairRecord(PHWarp* w, int f1, int f2, PHSolid* s1, PHSolid* s2){
	FramePairRecord* fpr = new FramePairRecord;
	fpr->position = w->position;
	fpr->velocity = w->velocity;
	fpr->angVel = w->angVel;
	fpr->frameID[0] = f1;
	fpr->frameID[1] = f2;
	fpr->solid[0] = s1;
	fpr->solid[1] = s2;
	framePairRecords.push_back(fpr);
}

void PHWarpEngine::Loaded(SGScene* scene){
	UTRef<CDCollisionEngine> ce;
	scene->GetBehaviors().Find(ce);
	// CDCollisionEngine がない場合は作る
	if (!ce){
		ce = new CDCollisionEngine;
		ce->SetName("theCollisionEngine", scene);
		scene->GetBehaviors().Add(ce);
	}

	// このワープエンジンをリスナーに登録する
	listenerPos = ce->AddListener(this);
	// フレーム対ごとのワープデータを格納するための場所をあらかじめ確保しておく
	framePairRecordPos = ce->ReserveFramePairRecord();

	// CDCollisionEngine の frames にワープ判定対象フレームがない場合追加し，
	// フレーム対ごとに activeList に登録する
	for(unsigned i=0; i<warps.size(); ++i){
		UTRef<PHWarp> warp = warps[i];
		PHSolids sols = warp->solids;
		int f1, f2;
		unsigned f2Start;
		if (warp->warpZoneFrame){
			ce->AddFrame(warp->warpZoneFrame);
			f1 = ce->GetFrameID(warp->warpZoneFrame);
			f2Start = 0;
		} else {
			ce->AddFrame(sols[0]->GetFrame());
			f1 = ce->GetFrameID(sols[0]->GetFrame());
			f2Start = 1;
		}
		for(unsigned j=f2Start; j<sols.size(); ++j){
			ce->AddFrame(sols[j]->GetFrame());
			f2 = ce->GetFrameID(sols[j]->GetFrame());
			ce->AddActive(f1, f2, listenerPos);
			if (f2Start == 0) AddFramePairRecord(warp, f1, f2, sols[j]);
			else AddFramePairRecord(warp, f1, f2, sols[j], sols[0]);
		}
	}
	
	//	初期化(frames から pairsを作る)
	ce->Init();

	// pairs の records にワープデータを持たせる
	for(unsigned i=0; i<framePairRecords.size(); ++i){
		FramePairRecord *fpr = framePairRecords[i];
		CDFramePairWithRecord* cfpr = ce->GetFramePair(fpr->frameID[0], fpr->frameID[1]);
		if (!cfpr) cfpr = ce->GetFramePair(fpr->frameID[1], fpr->frameID[0]);
		assert(!cfpr->records[framePairRecordPos]);
		cfpr->records[framePairRecordPos] = fpr;
/*
		if(cfpr.IsActive()){
			DSTR << "ActivePairs in this WarpEngine. << std::endl;
			//DSTR << fpr->frameID[0] << "-" << fpr->frameID[1] << std::endl;
			DSTR << cfpr.frame[0]->frame->GetName() << " - " << cfpr.frame[1]->frame->GetName() << std::endl;
		}
*/
	}
}

void PHWarpEngine::After(SGScene* scene, CDFramePairWithRecord* fp){
	// 前ステップでも衝突していたらワープさせない（2物体ともワープする場合のワープ先での衝突判定を解除）
	for(CDIntersections::iterator it = fp->intersections.begin(); it != fp->intersections.end(); ++it){
		CDConvexPair* cp = (CDConvexPair*) it->convexPair;
		if (cp->state == CDConvexPair::CONTINUE) {
			return;
		}
	}

	// レコード(中身はPHWarpEngine::FramePairRecord)をとってくる
	FramePairRecord* fpr = (FramePairRecord*)&*fp->records[framePairRecordPos];
	
	fpr->solid[0]->SetFramePosition(fpr->position);
	fpr->solid[0]->SetVelocity(fpr->velocity);
	fpr->solid[0]->SetAngularVelocity(fpr->angVel);
	fpr->solid[0]->SetRotation(Matrix3d());
	fpr->solid[0]->ClearForce();

	if(fpr->solid[1]){
		fpr->solid[1]->SetFramePosition(fpr->position);
		fpr->solid[1]->SetVelocity(fpr->velocity);
		fpr->solid[1]->SetAngularVelocity(fpr->angVel);
		fpr->solid[1]->SetRotation(Matrix3d());
		fpr->solid[1]->ClearForce();
	}	
}

//----------------------------------------------------------------------------
//	PHWarpLoader
//

class PHWarpLoader:public FIObjectLoader<PHWarp>{
	virtual bool LoadData(FILoadScene* ctx, PHWarp* w){
		ctx->docs.Top()->GetData(w->position, "position");
		ctx->docs.Top()->GetData(w->velocity, "velocity");
		ctx->docs.Top()->GetData(w->angVel, "angularVelocity");
		return true;
	}
};

//----------------------------------------------------------------------------
//	PHWarpSaver
//

class PHWarpSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWarp"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg);

	struct WarpInfo {
		Vec3f position;									
		Vec3f velocity;		
		Vec3f angVel;	
	};
};

void PHWarpSaver::Save(FISaveScene* ctx, SGObject* arg){
	PHWarp* warp = (PHWarp*)arg;
	FIDocNodeBase* doc = ctx->CreateDocNode("Warp", warp);
	ctx->docs.back()->AddChild(doc);
	WarpInfo info;
	info.position = warp->position;
	info.velocity = warp->velocity;
	info.angVel = warp->angVel;
	doc->SetWholeData(info);
	if (warp->warpZoneFrame){
		doc->AddChild(ctx->CreateDocNode("REF", warp->warpZoneFrame));
	}
	for(unsigned i=0; i<warp->solids.size(); ++i){
		doc->AddChild(ctx->CreateDocNode("REF", warp->solids[i]));
	}
}

//----------------------------------------------------------------------------
//	PHWarpEngineLoader
//

class PHWarpEngineLoader : public FIObjectLoader<PHWarpEngine>{
	virtual bool LoadData(FILoadScene* ctx, PHWarpEngine* we){
		return true;
	}
};

//----------------------------------------------------------------------------
//	PHWarpEngineSaver
//

class PHWarpEngineSaver : public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWarpEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWarpEngine* we = (PHWarpEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("WarpEngine", we);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(PHWarps::iterator it = we->warps.begin(); it != we->warps.end(); ++it){
			ctx->SaveRecursive(*it);
		}
		ctx->docs.pop_back();
	}
};
DEF_REGISTER_BOTH(PHWarp);
DEF_REGISTER_BOTH(PHWarpEngine);

};
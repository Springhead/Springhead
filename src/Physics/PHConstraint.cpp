#include "Physics.h"
#pragma hdrstop

using namespace PTM;
namespace Spr{;

//////////////////////////////////////////////////////////
SGOBJECTIMP(PHConstraint, SGObject);
SGObject* PHConstraint::ChildObject(size_t i){
	return solid[i];
}

size_t PHConstraint::NChildObjects(){
	return 2;
}

bool PHConstraint::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		if(solid[0] == NULL)
			solid[0] = (PHSolid*)o;
		else if(solid[1] == NULL)
			solid[1] = (PHSolid*)o;
		else return false;
		return true;
	}
	return false;
}

bool PHConstraint::DelChildObject(SGObject* o, SGScene* s){
	/*PHSolid* s = DCAST(PHSolid, o);
	if(lhs == s){
		lhs = NULL;
		return true;
	}
	if(rhs == s){
		rhs = NULL;
		return true;
	}*/
	return false;
}

const UTTypeInfo** PHConstraint::ChildCandidates(){
	static const UTTypeInfo* rv[] = {NULL};
	return rv;
}

////////////////////////////////////////////////////////////////////

SGOBJECTIMP(PHConstraintEngine, SGBehaviorEngine);

void PHConstraintEngine::Loaded(SGScene* scene){
	//Solidに対応するConstrainedStateオブジェクトを作成し、結びつける
	for(int i = 0; i < cons.size(); i++){
		for(int j = 0; j < 2; j++){
			for(int k = 0; k < objs.size(); k++){
				//既に対応するConstrainedStateオブジェクトが存在した場合
				if(states[k]->solid == cons[i]->solid[j]){
					cons[i]->state[j] = states[k];
					cons[i]->idx[j] = states[k]->nConstraints;
					states[k]->nConstraints++;
				}
				//まだ無かった場合
				else{
					PHConstrainedState* state = new PHConstrainedState;
					state->solid = cons[i]->solid[j];
					cons[i]->state[j] = state;
					cons[i]->idx[j] = 0;
					state->nConstraints = 1;
					states.push_back(state);
				}
			}
		}
	}
}

void PHConstraintEngine::Step(SGScene* s){
	//拘束を解いて、
	SolveConstraint();
	//Solidに反映
	UpdateState();
}

//拘束を満たすSolidの位置・姿勢を計算
void PHConstraintEngine::SolveConstraint(){
	//解析的解法：
	
	//反復解法：
	double error = 0.0;
	const double error_max = 1.0e-5;	//許容拘束誤差
	const int itr_max = 20;				//最大反復回数
	while(itr_max--){
		//各拘束について、被拘束Solidの目標位置と拘束誤差を計算
		for(int i = 0; i < cons.size(); i++){
			error += cons[i]->CompDesiredPosture();
		}
		//終了判定
		if(error < error_max)
			return;
		//目標位置と拘束誤差に基づいてSolidの位置を計算
		for(int i = 0; i < states.size(); i++)
			states[i]->UpdatePosture();
	}
}

SGObject* PHConstraintEngine::ChildObject(size_t i){
	return cons[i];
}

size_t PHConstraintEngine::NChildObjects(){
	return cons.size();
}

bool PHConstraintEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHConstraint, o)){
		cons.push_back((PHConstraint*)o);
		return true;
	}
	return false;
}

bool PHConstraintEngine::DelChildObject(SGObject* o, SGScene* s){
	return false; 
}

const UTTypeInfo** PHConstraintEngine::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHConstraint::GetTypeInfoStatic(), NULL
	};
	return rv;
}

//////////////////////////////////////////////////////////////////////

class PHConstraintEngineLoader : public FIObjectLoader<PHConstraintEngine>{
public:
	void LoadData(FILoadScene* ctx, PHConstraintEngine* engine){
	}
};

class PHConstraintEngineSaver : public FIBaseSaver
{
public:
	UTString GetType() const{ return "PHConstraintEngine";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHConstraintEngine* ce = (PHConstraintEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("ConstraintEngine", ce);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(unsigned i=0; i<ce->cons.size(); ++i){
			ctx->SaveRecursive(ce->cons[i]);
		}
		ctx->docs.pop_back();
	}
};

///////////////////////////////////////////////////////////////////////

class PHPointConstraintLoader : public FIObjectLoader<PHPointConstraint>{
	virtual void LoadData(FILoadScene* ctx, PHPointConstraint* con){
		ctx->docs.Top()->GetData(con->info);
	}
};

class PHPointConstraintSaver:public FIBaseSaver{
	virtual UTString GetType() const{return "PHPointConstraint";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHPointConstraint* con = (PHSolid*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PointConstraint", con);
		ctx->docs.back()->AddChild(doc);
		doc->SetData(con->info);
		if(con->lhs)
			doc->AddChild(ctx->CreateDocNode("Solid", s->lhs, true));
		if(con->rhs)
			doc->AddChild(ctx->CreateDocNode("Solid", s->rhs, true));
	}
};

DEF_REGISTER_BOTH(PHPointConstraint);
DEF_REGISTER_BOTH(PHConstraintEngine);

}

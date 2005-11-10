#include "Physics.h"
#pragma hdrstop
#include "PHJointPid.h"
#include <iomanip>

using namespace PTM;
namespace Spr{;

////////////////////////////////////////////////////////////////////////////////
//ツリー操作
PHJointBase* PHJointBase::Search(PHSolid* s)
{
	if(solid == s)return this;
	PHJointBase* pNode;
	array_type::const_iterator it;
	for(it = Children().begin(); it != Children().end(); it++){
		pNode = (*it)->Search(s);
		if(pNode)return pNode;
	}
	return 0;
}
//////////////////////////////////////////////////////////
//	初期化
SGOBJECTIMPABST(PHJointBase, SGObject);

PHJointBase::PHJointBase():solid(0), frame(0){
	intType = SYMPLETIC;
//	intType = ANALYTIC;
}

void PHJointBase::Loaded(SGScene* scene)
{
	//solidもframeも無指定の場合の処理
	if(!solid && !frame){
		solid = new PHSolid;
		solid->SetMass(0.1f);
		solid->SetInertia(0.1f * Matrix3d());
		solid->SetFrame(new SGFrame);
	}
	if(solid) solid->Loaded(scene);
	//	PHSolidContainerからSolidを削除
	PHSolidContainer* sc = NULL;
	scene->GetBehaviors().Find(sc);
	if (sc) sc->DelChildObject(solid, scene);
	
	//諸変数の初期化
	UpdateSolid();
	if(GetParent()){
		CompJointAxis();
	}else{
		if(solid){
			p = solid->GetCenterPosition();
			R = solid->GetRotation();
			v_abs = solid->GetVelocity();
			w_abs = solid->GetAngularVelocity();
		}else{
			p = frame->GetPosition();
			R = frame->GetRotation();
			v_abs.clear();
			w_abs.clear();
		}		
	}
	Ia.clear();
	Za.clear();

	Reset();

	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->Loaded(scene);
	}
}

//////////////////////////////////////////////////////////
//・木構造
SGObject* PHJointBase::ChildObject(size_t i){
	return Children()[i];
}
SGObject* PHJointBase::ReferenceObject(size_t i){
	if (i!=0) return NULL;
	if (solid) return solid;
	return frame;
}
size_t PHJointBase::NReferenceObjects(){
	if (frame || solid) return 1; 
	return 0;
}
size_t PHJointBase::NChildObjects(){
	return Children().size();
}

bool PHJointBase::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		solid = (PHSolid*)o;
		solid->SetIntegrationMode(PHINT_NONE);
		return true;
	}
	if (DCAST(PHJointBase, o)){
		((PHJointBase*)o)->SetParent(this);
		return true;
	}
	return s->AddChildObject(o, s);
}

bool PHJointBase::DelChildObject(SGObject* o, SGScene* s){
	PHJointBase* j = DCAST(PHJointBase, o);
	for(unsigned i=0; i<Children().size(); ++i){
		if (Children()[i] == j){
			j->SetParent(NULL);
			return true;
		}
	}
	return false;
}

const UTTypeInfo** PHJointBase::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHJointBase::GetTypeInfoStatic(), NULL
	};
	return rv;
}

void PHJointBase::UpdateSolid(){
	double m = solid ? solid->GetMass() : 0.0;
	Matrix3d I = solid ? solid->GetInertia(): Matrix3d();
	Ii.clear();
	smitem(Ii, 0, 1) = Matrix3d::Diag(m, m, m);
	smitem(Ii, 1, 0) = I;
}
void PHJointBase::UpdateJointPosture(){
	CompJointAxis();
	CompRelativePosition();
	//TODO:ここで下位ノードの位置をすべて更新
}
void PHJointBase::CompArticulatedInertia(double dt){
	//	Ia, Zaをクリア
	Ia.clear();
	Za.clear();

	//子ノードから先に計算
	for(array_type::const_iterator it = Children().end(); it != Children().begin();)
		(*--it)->CompArticulatedInertia(dt);

	//よく使う出てくる式をキャッシュ
	rcross = Matrix3d::Cross(prc);
	rpcross = Matrix3d::Cross(pRc * prc);
	rcross_cRp = rcross * cRp;
	rpcross_pRc = rpcross * pRc;
		
	//Za
	if(solid){
		svitem(Za, 0) += -R.trans() * solid->GetForce();
		svitem(Za, 1) += -R.trans() * solid->GetTorque() + Vec3d(
			(smitem(Ii, 1, 0)[2][2] - smitem(Ii, 1, 0)[1][1]) * w.Y() * w.Z(),
			(smitem(Ii, 1, 0)[0][0] - smitem(Ii, 1, 0)[2][2]) * w.Z() * w.X(),
			(smitem(Ii, 1, 0)[1][1] - smitem(Ii, 1, 0)[0][0]) * w.X() * w.Y());
	}
	//Ia
	Ia += Ii;
	Ia_c = Ia * c;
	Z_plus_Ia_c = Za + Ia_c;
}
void PHJointBase::CompCoriolisAccelRecursive(double dt){
	if (GetParent()) CompCoriolisAccel();
	for(array_type::const_iterator it = Children().end(); it != Children().begin();){
		(*--it)->CompCoriolisAccelRecursive(dt);
	}
}

void PHJointBase::ClearTorqueRecursive(){
	for(array_type::iterator it = Children().begin(); it != Children().end(); ++it){
		(*it)->ClearTorqueRecursive();
	}
}
void PHJointBase::ClearForce(){
	if (solid) solid->ClearForce();
	for(array_type::iterator it = Children().begin(); it != Children().end(); ++it){
		(*it)->ClearForce();
	}
}

void PHJointBase::Integrate(SGScene* scene){
	//	Solidに速度・位置を反映させる．
	solid->SetVelocity(v_abs);
	solid->SetAngularVelocity(w_abs);
	solid->SetRotation(R);
	solid->SetCenterPosition(p);
	//	子ジョイントの計算
	for(array_type::const_iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->Integrate(scene);
}

void PHJointBase::PropagateState()
{
	//親ノードとの相対位置、姿勢を計算
	CompRelativePosition();

	//相対位置から絶対位置を計算
	R = GetParent()->R * pRc;
	p = GetParent()->p + R * prc;

	//親ノードの速度・角速度＋関節速度　→　速度・角速度
	w = cRp * GetParent()->w;
	v = cRp * GetParent()->v + cross(w, prc);
	CompRelativeVelocity();
	v += pvc;
	w += pwc;
	v_abs = R * v;
	w_abs = R * w;
}
void PHJointBase::Reset(){
	PropagateState();
	solid->SetCenterPosition(p);
	solid->SetRotation(R);
}

void PHJointBase::LoadState(const SGBehaviorStates& states){
	Reset();
	//	子ジョイントをロード
	if (solid) solid->LoadState(states);
	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->LoadState(states);
	}
}

void PHJointBase::SaveState(SGBehaviorStates& states) const{
	if (solid) solid->SaveState(states);
	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->SaveState(states);
	}
}
void PHJointBase::SaveX(XJointBase& x) const{
	x.crj = crj;
	x.prj = prj;
	x.cRj = cRj;
	x.pRj = pRj;
}
void PHJointBase::LoadX(const XJointBase& x){
	crj = x.crj;
	prj = x.prj;
	cRj = x.cRj;
	pRj = x.pRj;
}


//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointRoot, PHJointBase);
void PHJointRoot::Reset(){
	quat.from_matrix(R);
	v = R.trans() * v_abs;
	w = R.trans() * w_abs;
}

class PHJointStateRoot: public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointStateRoot);
	//ルートノードの状態
	Vec3d p;
	Matrix3d R;
	Vec3d v_abs;
	Vec3d w_abs;
};
SGOBJECTIMP(PHJointStateRoot, SGBehaviorState);
void PHJointRoot::LoadState(const SGBehaviorStates& states){
	PHJointStateRoot* js = DCAST(PHJointStateRoot, states.GetNext());	
	//ロード
	p = js->p;
	R = js->R;
	v_abs = js->v_abs;
	w_abs = js->w_abs;
	PHJointBase::LoadState(states);
}
void PHJointRoot::SaveState(SGBehaviorStates& states) const{
	UTRef<PHJointStateRoot> js = new PHJointStateRoot;
	states.push_back(js);
	js->p = p;
	js->R = R;
	js->v_abs = v_abs;
	js->w_abs = w_abs;
	PHJointBase::SaveState(states);
}
void PHJointRoot::CalcAccel(double dt){
	if(solid){	//	ルートノードが剛体の場合
		//加速度を計算
		a = (Ia.inv() * Za) * -1;
	}else{	//if(frame)
		a.clear();
	}
}
void PHJointRoot::Integrate(SGScene* scene){
	double dt = scene->GetTimeStep();
	//physical
	if(solid){
		//加速度を計算
		a = (Ia.inv() * Za) * -1;
		//速度変化量
		Vec3d dv_abs = R * svitem(a, 1) * dt;
		//角速度変化量
		Vec3d dw_abs = R * svitem(a, 0) * dt;

		//位置を更新
		p += (v_abs + 0.5 * dv_abs) * dt;
		//回転量を更新
		quat += quat.derivative(w_abs + 0.5 * dw_abs) * dt;
		quat.unitize();
		quat.to_matrix(R);

		//速度を更新
		v_abs += dv_abs;
		w_abs += dw_abs;
		v = quat.conjugated() * v_abs;
		w = quat.conjugated() * w_abs;

		Vec3d a_rot = R * svitem(a, 0);
		Vec3d a_trn = R * svitem(a, 1);

		v_abs *= scene->GetVelocityLossPerStep();
		w_abs *= scene->GetVelocityLossPerStep();
		PHJointBase::Integrate(scene);
	}else{		//non-physical
		a.clear();
		for(array_type::const_iterator it = Children().begin(); it != Children().end(); it++)
			(*it)->Integrate(scene);
	}
}

//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointClearForce, SGBehaviorEngine);
void PHJointClearForce::Step(SGScene* s){
	je->root->ClearTorqueRecursive();
}


SGOBJECTIMP(PHJointEngine, SGBehaviorEngine);
void PHJointEngineEnumSolid(PHJointBase* j, PHJointEngine* e){
	e->solids.insert(j->solid);
}
void PHJointEngine::Loaded(SGScene* scene){
	root->Loaded(scene);
	root->Traverse(&PHJointEngineEnumSolid, this);
}
void PHJointEngine::Step(SGScene* scene)
{
	timer.Start();
	//	featherstone's algorithm
	double dt = scene->GetTimeStep();
	root->CompCoriolisAccelRecursive(dt);
	root->CompArticulatedInertia(dt);
	root->Integrate(scene);
	timer.Stop();
}

bool PHJointEngine::Has(SGObject* o){
	PHSolid* solid = DCAST(PHSolid, o);
	if (solid){
		if (solids.find(solid) != solids.end()) return true;
	}
	PHJointBase* joint = DCAST(PHJointBase, o);
	while (joint){
		if (joint == root) return true;
		joint = joint->GetParent();
	}
	return false;
}
SGObject* PHJointEngine::ChildObject(size_t i){
	return root;
}
size_t PHJointEngine::NChildObjects(){
	return root ? 1 : 0;
}
bool PHJointEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		root->solid = (PHSolid*)o;
		return true;
	}
	if (DCAST(SGFrame, o)){
		root->frame= (SGFrame*)o;
		return true;
	}
	if (DCAST(PHJointBase, o)){
		((PHJointBase*)o)->SetParent(root);
		return true;
	}
	return false;
}
bool PHJointEngine::DelChildObject(SGObject* o, SGScene* s){
	if (root == o){
		root = NULL;
		return true;
	}
	return false; 
}
const UTTypeInfo** PHJointEngine::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHJointBase::GetTypeInfoStatic(), NULL
	};
	return rv;
}
void PHJointEngine::LoadState(const SGBehaviorStates& states){
	root->LoadState(states);
}
void PHJointEngine::SaveState(SGBehaviorStates& states) const{
	root->SaveState(states);
}

//////////////////////////////////////////////////////////////////////
class PHJointEngineLoader : public FIObjectLoader<PHJointEngine>{
public:
	bool LoadData(FILoadScene* ctx, PHJointEngine* engine){
		//	JointEngineの初期化
		engine->root = new PHJointRoot;
		PHJointClearForce* jcf = new PHJointClearForce;
		jcf->je = engine;
		ctx->scene->AddChildObject(jcf, ctx->scene);
		
		//	clearForce への登録
		UTRef<PHSolidClearForce> clearForce;
		ctx->scene->GetBehaviors().Find(clearForce);
		if(!clearForce){
			clearForce= new PHSolidClearForce;
			ctx->scene->GetBehaviors().Add(clearForce);
		}
		clearForce->solvers.push_back(engine);
		
		return true;
	}
};

class PHJointEngineSaver : public FIBaseSaver
{
public:
	UTString GetType() const{ return "PHJointEngine";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHJointEngine* je = (PHJointEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("JointEngine", je);
		//接続対象を保存
		if (je->root->solid){
			doc->AddChild(ctx->CreateDocNode("REF", je->root->solid));
		}else if(je->root->frame){
			doc->AddChild(ctx->CreateDocNode("REF", je->root->frame));
		}

		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(unsigned i=0; i<je->root->Children().size(); ++i){
			ctx->SaveRecursive(je->root->Children()[i]);
		}
		ctx->docs.pop_back();
	}
};


//関節の種類
enum PHJointType{
	TYPE_HINGE,			//回転
	TYPE_SLIDER,		//直動
	//TYPE_BALLJOINT,	//ボールジョイント
};
//入力の種類
enum PHInputType{
	TYPE_TORQUE,		//トルク
	TYPE_ACCEL,			//加速度
	TYPE_VELOCITY,		//速度
	TYPE_POSITION		//位置
};
///関節情報．そのままファイルのJointタグのエントリと同じ
struct PHJointInfo{
	DWORD		nType;				///<	関節種類 @see PHJointType
	Vec3f		prj;	///<	親剛体のフレームから見た関節位置
	Matrix3f	pRj;	///<	親剛体のフレームから見た関節姿勢
	Vec3f		crj;	///<	子剛体のフレームから見た関節位置
	Matrix3f	cRj;	///<	子剛体のフレームから見た関節姿勢

	float		fPosition;			///<	変位
	float		fVelocity;			///<	速度
	float		fMaxTorque;			///<	最大トルク
	float		fMinPosition;		///<	可動範囲(最大値)
	float		fMaxPosition;		///<	可動範囲(最小値)
	DWORD		nInputType;			///<	入力の種類 @see PHJointInputType
	float		fInput;				///<	入力値の初期値
	float		fPValue;			///<	PID制御のPの値
	float		fIValue;			///<	PID制御のIの値
	float		fDValue;			///<	PID制御のDの値
};

class PHJointLoader: public FIBaseLoader
{
protected:
	virtual UTString GetNodeType() const {
		return "Joint";
	}
	void Load(FILoadScene* ctx){
		//関節の種類とパラメータ
		PHJointInfo info;
		ctx->docs.Top()->GetWholeData(info);
		//	左手系→右手系変換
		info.cRj.ExZ() *= -1;
		info.cRj.EyZ() *= -1;
		info.cRj.EzX() *= -1;
		info.cRj.EzY() *= -1;
		info.crj.Z() *= -1;
		info.pRj.ExZ() *= -1;
		info.pRj.EyZ() *= -1;
		info.pRj.EzX() *= -1;
		info.pRj.EzY() *= -1;
		info.prj.Z() *= -1;

		UTRef<PHJoint1D> joint=NULL;
		if (info.nType == TYPE_HINGE){
			joint = new PHJointHinge;
		}else if(info.nType == TYPE_SLIDER){
			joint = new PHJointSlider;
		}else{
			DSTR << "Unknown joint type " << info.nType << std::endl;
		}
		joint->SetName(ctx->docs.Top()->GetName().c_str(), ctx->scene);
		ctx->AddObject(joint);
		ctx->AddContainer(joint);
		
		joint->cRj = info.cRj;
		joint->pRj = info.pRj;
		joint->crj = info.crj;
		joint->prj = info.prj;
		joint->position = info.fPosition;
		joint->velocity = info.fVelocity;
		joint->maxPosition = info.fMaxPosition;
		joint->minPosition = info.fMinPosition;
		
		//	PIDの分離
		UTRef<PHJointPid> pid = new PHJointPid;
		std::string name = joint->GetName();
		if (name[0] == 'j') name = name.substr(1);
		if (name[0] == 'o') name = name.substr(1);
		name = std::string("pid").append(name);
		pid->SetName(name.c_str(), ctx->scene);
		ctx->scene->AddChildObject(pid, ctx->scene);
		pid->proportional = info.fPValue;
		pid->differential = info.fDValue;
		pid->integral = info.fIValue;
		pid->goal = info.fInput;
		pid->joint = joint;
		pid->type = (info.nInputType == TYPE_VELOCITY) ? 1 : 0;
	}
};

DEF_REGISTER_LOADER(PHJoint);
DEF_REGISTER_BOTH(PHJointEngine);

}

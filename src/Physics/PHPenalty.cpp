#include "Physics.h"
#pragma hdrstop

#include "PHPenalty.h"

namespace Spr{;

const float SPRING = 0.1f;
const float DAMPER = 0.002f;
//const float SPRING = 0.1f;
//const float DAMPER = 0.004f;

//----------------------------------------------------------------------------
//	PHPenalty
SGOBJECTIMP(PHPenalty, SGObject);

void PHPenalty::Init(){
}
void PHPenalty::Loaded(SGScene* scene){
	for(int i=0; i<2; ++i){
		if (!solid[i]){
			solid[i] = new PHSolid;
			solid[i]->SetMass(FLT_MAX);
			solid[i]->SetInertia(Matrix3d()*FLT_MAX);
			solid[i]->SetFrame(frame[i]->frame);
		}
	}
	Set(solid[0], solid[1]);
}
void PHPenalty::Draw(SGScene* scene){
	Affinef afw = frame[0]->frame->GetParent()->GetWorldPosture();
	for(CDGeometryPairs::iterator it = geometryPairs.begin(); it != geometryPairs.end(); ++it){
		CDGeometryPair& gp = **it;
		for(CDConvexPairs::iterator itc = gp.convexPairs.begin();
		itc != gp.convexPairs.end(); ++itc){
			PHConvexPair& cp = (PHConvexPair&)**itc;
			
			Vec3f ap[3];
			ap[0] = cp.reflexForcePoint - cp.frictionForce/convertedMass / 10;
			ap[2] = cp.reflexForcePoint + cp.frictionForce/convertedMass / 10;
			ap[1] = cp.reflexForcePoint;
			ap[0] = (ap[0] - ap[1]) + ap[1];
			ap[2] = (ap[2] - ap[1]) + ap[1];
			//	描画
			GRRender* render;
			scene->GetRenderers().Find(render);
			if (render->CanDraw()){
				render->SetModelMatrix(afw);
				GRMaterialData mat1(
					Vec4f(1, 0, 0, 1),
					Vec4f(1, 0, 0, 1),
					Vec4f(1, 0, 0, 1),
					Vec4f(1, 0, 0, 1), 0);
				GRMaterialData mat2(
					Vec4f(0, 1, 0, 1),
					Vec4f(0, 1, 0, 1),
					Vec4f(0, 1, 0, 1),
					Vec4f(0, 1, 0, 1),0);
				render->SetDepthTest(false);
				render->SetLineWidth(8);
				render->SetMaterial(mat1);
				render->DrawDirect(GRRender::LINES, ap, ap+1);
				render->SetMaterial(mat2);
				render->DrawDirect(GRRender::LINES, ap+1, ap+2);
				render->SetDepthTest(true);
			}
		}
	}
}
void PHPenalty::Clear(){
	solid[0] = NULL;
	solid[1] = NULL;
	CDFramePair::Clear();
}

int detectTime;
int analyzeTime;
int refTime;
int fricTime;
WBPreciseTimer ptimer;

void PHPenalty::Step(SGScene* scene){
	area=0;	//	衝突部分の面積
	dt = (float)scene->GetTimeStep();
	//	共有点を見つける．
	ptimer.CountUS();
	bool bContact = Detect(scene->GetCount());
	detectTime += ptimer.CountUS();
	if (!bContact) return;	//	交叉部の共有点を列挙する．同じ交叉を見つけた場合は，期限切れの印を消す．

	//	運動に関するパラメータを取ってくる
	Vec3f cog[2], vel[2], angVel[2], pos[2];	//	重心，速度，角速度，位置
	Quaternionf ori[2];
	for(int i=0; i<2; ++i){
		if (solid[i]){
			vel[i] = solid[i]->GetVelocity();
			angVel[i] = solid[i]->GetAngularVelocity();
			pos[i] = solid[i]->GetFramePosition();
			ori[i] = solid[i]->GetOrientation();
			cog[i] = ori[i] * solid[i]->GetCenter() + pos[i];
		}else{
			cog[i] = pos[i];
		}
	}

	//	接触解析を行い，反力を計算する．
	for(CDIntersections::iterator it = intersections.begin(); it != intersections.end(); ++it){
		PHConvexPair& cp = (PHConvexPair&)*it->convexPair;
		PHGeometryPair& gp = (PHGeometryPair&)*it->geometryPair;

		//	バネ係数とダンパ係数を決める
		float rkk[2], rbb[2], fkk[2], fbb[2];
		for(int i=0; i<2; ++i){
			if (cp.convex[i]->pmaterial != NULL){
				rkk[i] = cp.convex[i]->pmaterial->pMatData.reflexSpring;
				rbb[i] = cp.convex[i]->pmaterial->pMatData.reflexDamper;
				fkk[i] = cp.convex[i]->pmaterial->pMatData.frictionSpring;
				fbb[i] = cp.convex[i]->pmaterial->pMatData.frictionDamper;
			}else{
				rkk[i] = fkk[i] = SPRING;
				rbb[i] = fbb[i] = DAMPER;
			}
		}
		cp.rk = (rkk[0] + rkk[1])/2 * convertedMass / (dt*dt); ////rkk[0]とrkk[1]の関係は平均でいいのか？
		cp.rb = (rbb[0] + rbb[1])/2 * convertedMass / (dt*dt);
		cp.fk = (fkk[0] + fkk[1])/2 * convertedMass / (dt*dt);
		cp.fb = (fbb[0] + fbb[1])/2 * convertedMass / (dt*dt);

		//	各交差について，交差部の形状を求め，ペナルティ，速度，面積を積分する．
		ptimer.CountUS();
		cp.FindIntersection(cp, gp.posture);            	//	交差部分の形状を求める．
		analyzeTime += ptimer.CountUS();
#ifdef _DEBUG
		Affinef afw;
		if (solid[0]->GetFrame()->GetParent()) afw = solid[0]->GetFrame()->GetParent()->GetWorldPosture();
		cp.Draw(cp, afw, scene);					        //	交差部を描画
#endif
		ptimer.CountUS();
		cp.CalcNormal(cp, gp.posture);   	//	衝突の法線を求める．
		cp.CalcReflexForce(cog, vel, angVel, cp.rk, cp.rb, cp.fk, cp.fb);	//	ペナルティ，速度を積分. 抗力と摩擦力の係数を別々に分けた
		area += cp.area;													//	全体の面積を求める
		refTime += ptimer.CountUS();
	}

	//	抗力とその作用点を求め，摩擦を計算し，抗力と摩擦力を物体に加える．
	for(CDIntersections::iterator it = intersections.begin(); it != intersections.end(); ++it){
		PHConvexPair& cp = (PHConvexPair&) *it->convexPair;
		//	積分したペナルティと速度を面積で割る
		cp.reflexSpringForce /= area;
		cp.reflexDamperForce /= area;
		cp.reflexSpringTorque /= area;
		cp.reflexDamperTorque /= area;
		cp.dynaFric /= area;
		cp.dynaFricMom /= area;
		DEBUG_EVAL(
			if ( !_finite( cp.reflexSpringForce.norm() )
				|| !_finite( cp.frictionForce.norm() )
				|| !_finite( cp.frictionTorque.norm() ) ){
				DSTR << "Error: forces: " << cp.reflexSpringForce << cp.frictionForce << cp.frictionTorque << std::endl;
			}
		)
		ptimer.CountUS();
		//	摩擦力を計算する
		cp.CalcFriction(ori, pos, cog, vel, angVel, cp.fk, cp.fb);	//	抗力と摩擦力の係数が違う場合
		fricTime += ptimer.CountUS();
		//	力を加える．
		Vec3d force = cp.reflexSpringForce + cp.reflexDamperForce;
		Vec3d torque = cp.reflexSpringTorque + cp.reflexDamperTorque;
		float sign = 1;
		for(int i=0; i<2; ++i){
			/// 抗力を加える．
			solid[i]->AddForce(sign*force);
			solid[i]->AddTorque(sign*(torque +(-cog[i]^force) ));
			/// 摩擦力を加える．
			solid[i]->AddForce(sign*cp.frictionForce, cp.reflexForcePoint);
			solid[i]->AddTorque(sign*cp.frictionTorque);
			sign *= -1.0f;
		}
	}
	DEBUG_EVAL( Draw(scene) );
}

void PHPenalty::Set(PHSolid* s1, PHSolid* s2){
	solid[0] = s1;
	solid[1] = s2;
	if (s1) frame[0] = new CDFrame(s1->GetFrame());
	if (s2) frame[1] = new CDFrame(s2->GetFrame());
	CalcConvertedMass();
	CDFramePair::Set(frame[0], frame[1]);
}

void PHPenalty::CalcConvertedMass(){
	float m=1.0f;
	if (solid[0]->GetMass() < 1e10f && solid[1]->GetMass() < 1e10f){
		float m0 = (float)solid[0]->GetMass();
		float m1 = (float)solid[1]->GetMass();
		m = m0 * m1 / (m0+m1);
	}else if (solid[0]->GetMass() < 1e10f){
		m = (float)solid[0]->GetMass();
	}else if (solid[1]->GetMass() < 1e10f){
		m = (float)solid[1]->GetMass();
	}
	convertedMass = m;
}

bool PHPenalty::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(SGFrame, o)){
		if (frame[0] && frame[1]) frame[0] = frame[1];
		frame[frame[0] ? 1 : 0] = new CDFrame((SGFrame*)o);
		return true;
	}
	if (DCAST(PHSolid, o)){
		if (solid[0] && solid[1]) solid[0] = solid[1];
		solid[solid[0] ? 1 : 0] = (PHSolid*)o;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
//	PHPenaltyEngine
//
SGOBJECTIMP(PHPenaltyEngine, SGBehaviorEngine);
bool PHPenaltyEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHPenalty, o)){
		penalties.push_back((PHPenalty*)o);
		return true;
	}
	return false;
}
void PHPenaltyEngine::Step(SGScene* s){
	for(PHPenalties::iterator it = penalties.begin(); it != penalties.end(); ++it){
		(*it)->Step(s);
	}
}

void PHPenaltyEngine::Clear(SGScene* s){
	for(PHPenalties::iterator it = penalties.begin(); it != penalties.end(); ++it){
		(*it)->Clear();
	}
	penalties.clear();
}

class PHPenaltyLoader:public FIObjectLoader<PHPenalty>{
	virtual bool LoadData(FILoadScene* ctx, PHPenalty* p){ return true; }
};
class PHPenaltySaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHPenalty"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHPenalty* pen = (PHPenalty*)arg;
		FIDocNodeBase* docPen = ctx->CreateDocNode("Penalty", pen);
		ctx->docs.back()->AddChild(docPen);
		for(int i=0; i<2; ++i){
			if (pen->frame[i]){
				assert(strlen(pen->frame[i]->frame->GetName()));
				docPen->AddChild(ctx->CreateDocNode("REF", pen->frame[i]->frame));
			}
		}
		for(int i=0; i<2; ++i){
			if (pen->solid[i] && strlen(pen->solid[i]->GetName())){
				docPen->AddChild(ctx->CreateDocNode("REF", pen->solid[i]));
			}
		}
	}
};
DEF_REGISTER_BOTH(PHPenalty);

class PHPenaltyEngineLoader:public FIObjectLoader<PHPenaltyEngine>{
	virtual bool LoadData(FILoadScene* ctx, PHPenaltyEngine* pc){ return true; }
};
class PHPenaltyEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHPenaltyEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHPenaltyEngine* pc = (PHPenaltyEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PenaltyEngine", pc);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(PHPenalties::iterator it = pc->penalties.begin(); it != pc->penalties.end(); ++it){
			ctx->SaveRecursive(*it);
		}
		ctx->docs.pop_back();
	}
};
DEF_REGISTER_BOTH(PHPenaltyEngine);


};

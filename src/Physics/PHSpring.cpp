#include "Physics.h"
#pragma hdrstop
#include "PHSpring.h"

namespace Spr{;

SGOBJECTIMP(PHSpring, SGBehaviorEngine);

PHSpring::PHSpring(){
}
bool PHSpring::AddChildObject(SGObject* o, SGScene* s){
	PHSolid* so = DCAST(PHSolid, o);
	if (so){
		if(!solid[0] || solid[0]==so) solid[0] = so;
		else if(!solid[1] || solid[1]==so) solid[1] = so;
		else return false;
	}else{
		return false;
	}
	return true;
}
void PHSpring::Step(SGScene* s){
	Vec3f posW[2];
	Quaternionf oriW[2];
	for(int i=0; i<2; ++i){
		posW[i] = solid[i]->GetOrientation() * pos[i] + solid[i]->GetFramePosition();
		oriW[i] = solid[i]->GetOrientation() * ori[i];
	}
	Vec3f dPos = posW[1] - posW[0];
	Vec3f f = kt;
	PTM::multi_each(f, dPos);
	solid[0]->AddForce( f, posW[0]);
	solid[1]->AddForce(-f, posW[1]);

	Quaternionf dOri = oriW[1] * oriW[0].inv();
	Vec3f dAng = dOri.rotation();
	
	Vec3f t = kr;
	PTM::multi_each(t, dAng);
	solid[0]->AddTorque( t);
	solid[1]->AddTorque(-t);

	Vec3f dVel = solid[1]->GetVelocity() - solid[0]->GetVelocity();
	Vec3f v = bt;
	PTM::multi_each(v, dVel);
	solid[0]->AddForce(v, posW[0]);
	solid[1]->AddForce(-v, posW[1]);
	
	Vec3f dAngVel = solid[1]->GetAngularVelocity() - solid[0]->GetAngularVelocity();
	Vec3f av = br;
	PTM::multi_each(av, dAngVel);
	solid[0]->AddTorque(av);
	solid[1]->AddTorque(-av);
}

typedef Vec3f			Vector;
typedef Quaternionf		Quaternion;
typedef Matrix3f		Matrix3x3;
DEF_RECORD(XSpring,{
	GUID Guid(){ return WBGuid("ABC87118-0C78-40fd-9072-9C54A8BE8ADD"); }
	Vector		pos1;
	Matrix3x3	rot1;
	Vector		pos2;
	Matrix3x3	rot2;
	Vector	kt;
	Vector	bt;
	Vector	kr;
	Vector	br;
});


class PHSpringLoader:public FIObjectLoader<PHSpring>{
public:
	PHSpringLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Vector);
		db->REG_FIELD(Matrix3x3);
		db->REG_RECORD_PROTO(XSpring);
	}
	virtual bool LoadData(FILoadScene* ctx, PHSpring* s){
		XSpring spr;
		ctx->docs.Top()->GetWholeData(spr);
		//	¶ŽèŒn¨‰EŽèŒn•ÏŠ·
		spr.pos1.Z() *= -1;
		spr.pos2.Z() *= -1;
		spr.rot1.ExZ() *= -1;
		spr.rot1.EyZ() *= -1;
		spr.rot1.EzX() *= -1;
		spr.rot1.EzY() *= -1;
		s->kt = spr.kt;
		s->bt = spr.bt;
		s->kr = spr.kr;
		s->br = spr.br;
		s->pos[0] = spr.pos1;
		s->pos[1] = spr.pos2;
		s->ori[0].from_matrix(spr.rot1);
		s->ori[1].from_matrix(spr.rot2);
		return true;
	}
};

class PHSpringSaver:public FIObjectSaver<PHSpring>{
	virtual UTString GetType() const{ return "PHSpring"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, PHSpring* s){
		XSpring spr;
		spr.kt = s->kt;
		spr.bt = s->bt;
		spr.kr = s->kr;
		spr.br = s->br;
		spr.pos1 = s->pos[0];
		spr.pos2 = s->pos[1];
		s->ori[0].to_matrix(spr.rot1);
		s->ori[1].to_matrix(spr.rot2);
		//	‰EŽèŒn¨¶ŽèŒn•ÏŠ·
		spr.pos1.Z() *= -1;
		spr.pos2.Z() *= -1;
		spr.rot1.ExZ() *= -1;
		spr.rot1.EyZ() *= -1;
		spr.rot1.EzX() *= -1;
		spr.rot1.EzY() *= -1;
		doc->SetWholeData(spr);
	}
};
DEF_REGISTER_BOTH(PHSpring);

}	//	namespace Spr
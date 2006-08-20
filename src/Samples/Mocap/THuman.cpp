// THuman.cpp: THuman クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "THuman.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace Spr;
extern bool bExplicit;
TSpring::TSpring(Spr::PHSolid* s, Spr::Vec3f p):solid(s), pos(p){
	if (!bExplicit){
		PHScene* scene = DCAST(PHScene, solid->GetScene());
		anchor = DCAST(PHSolid, scene->CreateSolid(PHSolidDesc()));
		anchor->SetDynamical(false);
		PHSpringDesc desc;
		desc.poseSocket.Pos() = pos;
		spring = DCAST(PHSpring, scene->CreateJoint(anchor, solid, desc));
	}
}

Vec3f TSpring::GetPos(){
	if (solid) return solid->GetPose() * pos;
	return Vec3f();
}
Vec3f TSpring::GetVel(){
	if (solid){
		Vec3f gPos = solid->GetOrientation() * pos;
		Vec3f vel = solid->GetVelocity() + (solid->GetAngularVelocity() ^ gPos);
		return vel;
	};
	return Vec3f();
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

THuman::THuman(){
	soBody  = NULL;
	soHead  = NULL;
	soRUArm = NULL;
	soLUArm = NULL;
	soRFArm = NULL;
	soLFArm = NULL;
	soRHand = NULL;
	soLHand = NULL;
	soWaist = NULL;
	soRULeg = NULL;
	soLULeg = NULL;
	soRLLeg = NULL;
	soLLLeg = NULL;
	soRFoot = NULL;
	soLFoot = NULL;
	
	joWaist1 = NULL;
	joWaist2 = NULL;
	joWaist3 = NULL;
	joNeck1 = NULL;
	joNeck2 = NULL;
	joNeck3 = NULL;
	joRShoulder1 = NULL;
	joRShoulder2 = NULL;
	joRShoulder3 = NULL;
	joRElbow1 = NULL;
	joRWrist1 = NULL;
	joRWrist2 = NULL;
	joLShoulder1 = NULL;
	joLShoulder2 = NULL;
	joLShoulder3 = NULL;
	joLElbow1 = NULL;
	joLWrist1 = NULL;
	joLWrist2 = NULL;
	joRHip1 = NULL;
	joRHip2 = NULL;
	joRHip3 = NULL;
	joRKnee1 = NULL;
	joRAnkle1 = NULL;
	joLHip1 = NULL;
	joLHip2 = NULL;
	joLHip3 = NULL;
	joLKnee1 = NULL;
	joLAnkle1 = NULL;

	joLShin = NULL;
	joRShin = NULL;

	bLoaded = false;
}

void THuman::ConnectSolid(PHSolid*& solid, const char* name, PHScene* scene){
	scene->FindObject(solid, name);
	bLoaded &= (solid != NULL);
	solids.push_back(solid);
}
void THuman::ConnectJoint(PHHingeJoint*& joint, const char* name, PHScene* scene){
	PHJoint1D* j;
	ConnectJoint(j, name, scene);
	joint = DCAST(PHHingeJoint, j);
}
void THuman::ConnectJoint(PHSliderJoint*& joint, const char* name, PHScene* scene){
	PHJoint1D* j;
	ConnectJoint(j, name, scene);
	joint = DCAST(PHSliderJoint, j);
}
void THuman::ConnectJoint(PHJoint1D*& joint, const char* name, PHScene* scene){
	ObjectIf* obj = scene->FindObject(name);
	joint = DCAST(PHJoint1D, obj);
	bLoaded &= (joint != NULL);
	joints.push_back(joint);

	UTString n = name;
	if (n[0] == 'j') n = n.substr(1);
	if (n[0] == 'o') n = n.substr(1);
	n = std::string("pid").append(n);
}
bool THuman::Connect(PHScene* scene){
	solids.clear();
	joints.clear();
	bLoaded = true;
	ConnectSolid(soBody, "soBody", scene);
	ConnectSolid(soHead, "soHead", scene);
	ConnectSolid(soRUArm, "soRUArm", scene);
	ConnectSolid(soLUArm, "soLUArm", scene);
	ConnectSolid(soRFArm, "soRFArm", scene);
	ConnectSolid(soLFArm, "soLFArm", scene);
	ConnectSolid(soRHand, "soRHand", scene);
	ConnectSolid(soLHand, "soLHand", scene);
	ConnectSolid(soWaist, "soWaist", scene);
	ConnectSolid(soRULeg, "soRULeg", scene);
	ConnectSolid(soLULeg, "soLULeg", scene);
	ConnectSolid(soRLLeg, "soRLLeg", scene);
	ConnectSolid(soLLLeg, "soLLLeg", scene);
	ConnectSolid(soRFoot, "soRFoot", scene);
	ConnectSolid(soLFoot, "soLFoot", scene);

	ConnectJoint(joWaist1, "joWaist1", scene);
	ConnectJoint(joWaist2, "joWaist2", scene);
	ConnectJoint(joWaist3, "joWaist3", scene);
	ConnectJoint(joNeck1, "joNeck1", scene);
	ConnectJoint(joNeck2, "joNeck2", scene);
	ConnectJoint(joNeck3, "joNeck3", scene);
	ConnectJoint(joLShoulder1, "joLShoulder1", scene);
	ConnectJoint(joLShoulder2, "joLShoulder2", scene);
	ConnectJoint(joLShoulder3, "joLShoulder3", scene);
	ConnectJoint(joLElbow1, "joLElbow1", scene);
	ConnectJoint(joLWrist1, "joLWrist1", scene);
	ConnectJoint(joLWrist2, "joLWrist2", scene);
	ConnectJoint(joRShoulder1, "joRShoulder1", scene);
	ConnectJoint(joRShoulder2, "joRShoulder2", scene);
	ConnectJoint(joRShoulder3, "joRShoulder3", scene);
	ConnectJoint(joRElbow1, "joRElbow1", scene);
	ConnectJoint(joRWrist1, "joRWrist1", scene);
	ConnectJoint(joRWrist2, "joRWrist2", scene);
	ConnectJoint(joLHip1, "joLHip1", scene);
	ConnectJoint(joLHip2, "joLHip2", scene);
	ConnectJoint(joLHip3, "joLHip3", scene);
	ConnectJoint(joLKnee1, "joLKnee1", scene);
	ConnectJoint(joLAnkle1, "joLAnkle1", scene);
//	ConnectJoint(joLAnkle2, "joLAnkle2", scene);
	ConnectJoint(joRHip1, "joRHip1", scene);
	ConnectJoint(joRHip2, "joRHip2", scene);
	ConnectJoint(joRHip3, "joRHip3", scene);
	ConnectJoint(joRKnee1, "joRKnee1", scene);
	ConnectJoint(joRAnkle1, "joRAnkle1", scene);
//	ConnectJoint(joRAnkle2, "joRAnkle2", scene);
	ConnectJoint(joLShin, "joLShin", scene);
	ConnectJoint(joRShin, "joRShin", scene);
	return bLoaded;
}

THuman::~THuman(){
}
void THuman::SaveOneAngle(TAngle& angle, PHJoint1D* j){
	angle.angle[angle.nAngle] = (float)j->GetPosition();
	angle.angVel[angle.nAngle] = (float)j->GetVelocity();
	angle.nAngle ++;
}
void THuman::SaveJointAngle(){
	TAngle angle;
	angle.bodyOri = soBody->GetOrientation();
	angle.bodyAngVel = soBody->GetAngularVelocity();
	angle.bodyVel = soBody->GetVelocity();
	for(unsigned i=0; i<joints.size(); ++i){
		SaveOneAngle(angle, joints[i]);
	}
	angles.push_back(angle);
}

void THuman::LoadOneAngle(TAngle& a, int c, PHJoint1D* j){
	j->origin = a.angle[c];
}
void THuman::SaveOneTorque(TAngle& a, int c, PHJoint1D* j){
	//	TODO	SaveTorque
	//	a.jointTorque[c] = j->GetTorque();
}
void THuman::SaveForce(float time, float dt){
	int c = (int)(time / dt);
	if (c >= (int)angles.size()) return;
	TAngle& angle = angles[c];
	angle.force = Vec3f();
	angle.torque = Vec3f();
	angle.force += soLFoot->GetForce();
//	angle.force += soRFoot->GetForce();
	angle.torque += soLFoot->GetTorque();
//	angle.torque += soRFoot->GetTorque();
	angle.torque += soLFoot->GetCenterPosition() ^ soLFoot->GetForce();
//	angle.torque += soRFoot->GetCenterPosition() ^ soRFoot->GetForce();
	for(unsigned i=0; i<joints.size(); ++i){
		SaveOneTorque(angle, i, joints[i]);
	}
}
bool THuman::LoadJointAngle(float time, float dt){
	int c = (int)(time / dt);
	if (c >= (int)angles.size()) return false;
	TAngle& angle = angles[c];
	for(unsigned i=0; i<joints.size(); ++i){
		LoadOneAngle(angle, i, joints[i]);
	}
	const float SAFETYRATE = 0.2f;			//	安全率 小さいほど安定でやわらかく
	const float SPRING = 0.2f*SAFETYRATE;		//	バネ
	const float DAMPER = 0.6f*SAFETYRATE;		//	ダンパ
	float mass = (float)soBody->GetMass();
	float spring = SPRING * mass / (2*dt*dt);
	float damper = DAMPER * mass / (dt);
	Quaternionf dOri = angle.bodyOri * soBody->GetOrientation().Inv();
	Vec3f dAng = dOri.Rotation();
	Vec3f dAVel = angle.bodyAngVel - soBody->GetAngularVelocity();
	soBody->AddTorque(spring* dAng + damper* dAVel);
	Vec3f dVel = angle.bodyVel - soBody->GetVelocity();
	if (dVel.Y() > 0) dVel.Y() = 0;
	if (time < 0.1f) soBody->AddForce(0.5f * damper * dVel);
	return true;
}
inline float GetChildMass(PHJoint1D* j){
	if (strlen(j->solid[0]->solid->GetName()) != 0){
		return j->solid[0]->solid->GetMass();
	}else{	//	名前をもつ子ノードを探す
		float rv = 0;
		for(int i=0; i<j->children.size(); ++i){
			PHJoint1D* c = j->children[i];
			if (c) rv += GetChildMass(c);
		}
		return rv;
	}
}
void THuman::JointPIDMul(PHJoint1D* jo, float mul){
	jo->spring *= mul;
//	jo->integral *= mul;
	jo->damper *= mul;
}
void THuman::SetSpring(float dt){
	const float SAFETYRATE = 0.01f;
	float k = 0.2f * SAFETYRATE;
	float b = 0.6f * SAFETYRATE;
	for(unsigned i=0; i<joints.size(); ++i){
		float mass = GetChildMass(joints[i]);
		joints[i]->spring = k * mass / (2*dt*dt);
		joints[i]->damper = b * mass / dt;
//		joints[i]->integral = k * mass / (2*dt*dt) / 5000.0f;
	}
	/*
	const float AnkleMul = 0.1f;
	JointPIDMul(joLAnkle1, AnkleMul);
	JointPIDMul(joRAnkle1, AnkleMul);
*/
	const float HIPMUL = 2.0f;
	JointPIDMul(joLHip1, HIPMUL);
	JointPIDMul(joLHip2, HIPMUL);
	JointPIDMul(joLHip3, HIPMUL);
	JointPIDMul(joRHip1, HIPMUL);
	JointPIDMul(joRHip2, HIPMUL);
	JointPIDMul(joRHip3, HIPMUL);

	const float WaistMul = 0.3f;
	JointPIDMul(joWaist1, WaistMul);
	JointPIDMul(joWaist2, WaistMul);
	JointPIDMul(joWaist3, WaistMul);
	
	const float ShinMul = 10.0f;
	JointPIDMul(joLShin, ShinMul);
	JointPIDMul(joRShin, ShinMul);
}
void THuman::SetTotalMass(float totalMass){
	for(unsigned i=0; i<solids.size(); ++i){
		solids[i]->SetMass(solids[i]->GetMass()*totalMass);
		solids[i]->SetInertia(solids[i]->GetInertia()*totalMass);
	}
}
void THuman::SetOneInertia(Spr::PHSolid* solid, float longAxis, float rate){
	float shortAxis = longAxis*rate;
	float longMorment;
	float shortMorment;
	Matrix3f solidInertia;
	
	longMorment = (float)(solid->GetMass()*(shortAxis*shortAxis+shortAxis*shortAxis))/5;
	shortMorment = (float)(solid->GetMass()*(longAxis*longAxis+shortAxis*shortAxis))/5;

	if(strcmp(solid->GetName(),"soWaist" )== 0){
		solidInertia[0][0] = longMorment;
		solidInertia[1][1] = shortMorment;
		solidInertia[2][2] = longMorment;
	}else if(strcmp(solid->GetName(), "soRFoot")==0 || strcmp(solid->GetName(), "soLFoot")== 0){
		solidInertia[0][0] = shortMorment;
		solidInertia[1][1] = shortMorment;
		solidInertia[2][2] = longMorment;
	}else{
		solidInertia[0][0] = shortMorment;
		solidInertia[1][1] = longMorment;
		solidInertia[2][2] = shortMorment;
	}
	solid->SetInertia(solidInertia);
}

#define CHILD 0
#define PARENT 1
void THuman::SetScale(Spr::Vec2f head, Spr::Vec2f shoulder, Spr::Vec2f body, Spr::Vec2f hip,
	float armLen, float forearmLen, float handLen, float thighLen, float shinLen,
	Spr::Vec3f foot, Spr::Vec3f pos6, Spr::Vec3f pos7){
	
	Vec3f org[40][2];
	for(int i=0; i<joints.size(); ++i){
		org[i][0] = joints[i]->rj[0];
		org[i][1] = joints[i]->rj[1];
	}

	springs[0].resize(13);
	springs[1].resize(13);
/*
	solids[0]->SetDynamical(false);
	Posed p = solids[0]->GetPose();
	p.Pos().Y() += 2;
	solids[0]->SetPose(p);
*/

	float neckOffset = shoulder.Y()*2/3;
	joNeck3->rj[CHILD].Y() = -shoulder.Y()/2-neckOffset;
	joNeck3->rj[CHILD].Z() = shoulder.Y()/2;
	springs[0][0] = TSpring(soHead, Vec3f(0, head.Y() -neckOffset, 0));
	springs[1][0] = TSpring(soHead, Vec3f(0, head.Y() -neckOffset, 0));
	springs[0][1] = TSpring(soHead, Vec3f(-head.X()/2, -neckOffset, 0));
	springs[1][1] = TSpring(soHead, Vec3f( head.X()/2, -neckOffset, 0));	
	joNeck1->rj[PARENT].Y() = body.Y()/2 + shoulder.Y()/2;
	joRShoulder1->rj[PARENT].Y() = body.Y()/2;
	joRShoulder1->rj[PARENT].X() = shoulder.X()/2;
	joLShoulder1->rj[PARENT].Y() = body.Y()/2;
	joLShoulder1->rj[PARENT].X() = -shoulder.X()/2;
	springs[0][2] = TSpring(soBody, Vec3f(-shoulder.X()/2, body.Y()/2, 0));
	springs[1][2] = TSpring(soBody, Vec3f( shoulder.X()/2, body.Y()/2, 0));
	joWaist3->rj[CHILD].Y() = -body.Y()/2;
	springs[0][3] = TSpring(soWaist, Vec3f(-body.X()/2, 0, 0));
	springs[1][3] = TSpring(soWaist, Vec3f( body.X()/2, 0, 0));
	joLHip1->rj[PARENT].Y() = -hip.Y();
	joLHip1->rj[PARENT].X() = -hip.X()/3;
	joRHip1->rj[PARENT].Y() = -hip.Y();
	joRHip1->rj[PARENT].X() = hip.X()/3;
	springs[0][4] = TSpring(soWaist, Vec3f(-hip.X()/2, -hip.Y(),0));
	springs[1][4] = TSpring(soWaist, Vec3f(+hip.X()/2, -hip.Y(),0));
	//	腿の長さ
	joRHip3->rj[CHILD].Y() = thighLen/2;
	joLHip3->rj[CHILD].Y() = thighLen/2;
	joRKnee1->rj[PARENT].Y() = -thighLen/2;
	joLKnee1->rj[PARENT].Y() = -thighLen/2;
	springs[0][5] = TSpring(soLULeg, Vec3f(-hip.X()/6,-thighLen/2,0));
	springs[1][5] = TSpring(soRULeg, Vec3f(+hip.X()/6,-thighLen/2,0));
	//	脛の長さ
	joRKnee1->rj[CHILD].Y() = shinLen/2;
	joLKnee1->rj[CHILD].Y() = shinLen/2;
	joRAnkle1->rj[PARENT].Y() = -shinLen/2;
	joLAnkle1->rj[PARENT].Y() = -shinLen/2;
	//	8と9を結ぶ線を足の向きとする．
	//	8と9の中点を原点，8の向きをZ+，右向きをX+として右足の6と7の位置を求める
	Affinef af;
	af.Ex() *= foot.X() / 2;
	af.Ey() *= foot.Y() / 2;
	af.Ez() *= foot.Z() / 2;
/*	TODO 足の向きの設定
	SGFrame* frL;
	soLFoot->GetFrame()->FindObject(frL);
	frL->FindObject(frL);
	frL->SetPosture(af);	
	SGFrame* frR;
	soRFoot->GetFrame()->FindObject(frR);
	frR->FindObject(frR);
	frR->SetPosture(af);
*/
	//	剛体の中心から見た位置に変換
	Vec3f footOffset(-pos6.X()/2, -foot.Y()/2, 0);	//	剛体の中心から見た，89の中点
	pos6 += footOffset;
	pos7 += footOffset;
	
	joLAnkle1->rj[CHILD] = Vec3f(-pos6.X()+foot.X()/2, pos6.Y(), pos6.Z());
	joRAnkle1->rj[CHILD] = Vec3f( pos6.X()-foot.X()/2, pos6.Y(), pos6.Z());
	springs[0][6] = TSpring(soLFoot, Vec3f(-pos6.X(), pos6.Y(), pos6.Z()));
	springs[1][6] = TSpring(soRFoot, Vec3f( pos6.X(), pos6.Y(), pos6.Z()));
	springs[0][7] = TSpring(soLFoot, Vec3f(-pos7.X(), pos7.Y(), pos7.Z()));
	springs[1][7] = TSpring(soRFoot, Vec3f( pos7.X(), pos7.Y(), pos7.Z()));
	springs[0][8] = TSpring(soLFoot, Vec3f(-footOffset.X(), footOffset.Y(), foot.Z()/2));
	springs[1][8] = TSpring(soRFoot, Vec3f( footOffset.X(), footOffset.Y(), foot.Z()/2));
	springs[0][9] = TSpring(soLFoot, Vec3f(-footOffset.X(), footOffset.Y(),-foot.Z()/2));
	springs[1][9] = TSpring(soRFoot, Vec3f( footOffset.X(), footOffset.Y(),-foot.Z()/2));

	//	上腕
	joLShoulder3->rj[CHILD].Y() = armLen/2;
	joRShoulder3->rj[CHILD].Y() = armLen/2;
	joLElbow1->rj[PARENT].Y() = -armLen/2;
	joRElbow1->rj[PARENT].Y() = -armLen/2;
	springs[0][10] = TSpring(soLUArm, Vec3f(0,-armLen/2,0));
	springs[1][10] = TSpring(soRUArm, Vec3f(0,-armLen/2,0));
	//	前腕
	joLElbow1->rj[CHILD].Y() = forearmLen/2;
	joRElbow1->rj[CHILD].Y() = forearmLen/2;
	joLWrist1->rj[PARENT].Y() = -forearmLen/2;
	joRWrist1->rj[PARENT].Y() = -forearmLen/2;
	springs[0][11] = TSpring(soLFArm, Vec3f(0,-forearmLen/2,0));
	springs[1][11] = TSpring(soRFArm, Vec3f(0,-forearmLen/2,0));
	//	手
	joLWrist2->rj[CHILD].Y() = handLen/2;
	joRWrist2->rj[CHILD].Y() = handLen/2;
	springs[0][12] = TSpring(soLHand, Vec3f(0,-handLen/2,0));
	springs[1][12] = TSpring(soRHand, Vec3f(0,-handLen/2,0));

	//	質量
	SetTotalMass(60.0f);
	//	慣性モーメント
	SetOneInertia(soHead, head.Y(), 1.0f/2.0f);
	SetOneInertia(soBody, body.Y(), 2.0f/3.0f);

	SetOneInertia(soLUArm, armLen, 1.0f/3.0f);
	SetOneInertia(soRUArm, armLen, 1.0f/3.0f);
	SetOneInertia(soLFArm, forearmLen, 1.0f/3.0f);
	SetOneInertia(soRFArm, forearmLen, 1.0f/3.0f);
	SetOneInertia(soLHand, handLen, 1.0f);
	SetOneInertia(soRHand, handLen, 1.0f);

	SetOneInertia(soWaist, hip.Y(), 3.0f/2.0f);

	SetOneInertia(soLULeg, thighLen, 5.0f/8.0f);
	SetOneInertia(soRULeg, thighLen, 5.0f/8.0f);
	SetOneInertia(soLLLeg, shinLen, 1.0f/2.0f);
	SetOneInertia(soRLLeg, shinLen, 1.0f/2.0f);
	SetOneInertia(soLFoot, foot.Z(), 1.0f/3.0f);
	SetOneInertia(soRFoot, foot.Z(), 1.0f/3.0f);
/*
	for(int i=0; i<joints.size(); ++i){
		DSTR << joints[i]->GetName() << std::endl;
		DSTR << i << " " << org[i][0];
		DSTR << org[i][1] << std::endl;
		DSTR << i << " " << joints[i]->rj[0];
		DSTR << joints[i]->rj[1] << std::endl;
	}
*/
}

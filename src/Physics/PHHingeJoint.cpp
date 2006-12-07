/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHHingeJoint
IF_OBJECT_IMP(PHHingeJoint, PHJoint1D)

PHHingeJoint::PHHingeJoint(){
	axisIndex[0] = 5;
	for(int i = 0; i < 6; i++)
		axis[i] = (i == axisIndex[0]);
}

void PHHingeJoint::UpdateJointState(){
	//Ž²•ûŒü‚ÌS‘©‚Í‡’v‚µ‚Ä‚¢‚é‚à‚Ì‚Æ‰¼’è‚µ‚ÄŠp“x‚ðŒ©‚é
	position[0] = qjrel.Theta();
	if(qjrel.Axis().Z() < 0.0)
		position = -position;
	velocity[0] = vjrel.w.z;
}

void PHHingeJoint::CompBias(){
	double dtinv = 1.0 / scene->GetTimeStep();
	//	hase	‚±‚ê‚ð“ü‚ê‚½‚Ù‚¤‚ªAŽáŠ±ˆÀ’è‰»‚·‚éB
	//	–{—ˆ‘¬“x‚ð‚»‚Ì“s“xC³‚µ‚Ä‚¢‚é‚Ì‚ÅAPD§Œä‚Å‚Í‚È‚¢‚Ì‚¾‚¯‚ÇA
	//	shrinkRatio ‚ª‚‚¢ê‡AS‘©—Í‚ªŽc‚é‚½‚ßAD‚ð“ü‚ê‚é‚ÆˆÀ’è‰»‚·‚éB
	//	tazz ’Ç‹LD—v‚·‚é‚ÉŒ»Ý‚ÌŒë·‚Å‚Í‚È‚­ŽŸŽž‚Ì—\‘ªŒë·‚ð0‚É‚·‚é‚æ‚¤‚É‚·‚é
	db.v = (Xjrel.r * dtinv + vjrel.v);
	db.w = (qjrel.AngularVelocity((qjrel - Quaterniond()) * dtinv) + vjrel.w);
	db.w.z = 0.0;
	db *= engine->correctionRate;

	double diff;
	if(mode == MODE_VELOCITY){
		db.w.z = -vel_d;
	}
	else if(spring != 0.0 || damper != 0.0){
		diff = GetPosition() - origin;
		while(diff >  M_PI) diff -= 2 * M_PI;
		while(diff < -M_PI) diff += 2 * M_PI;
		double tmp = 1.0 / (damper + spring * scene->GetTimeStep());
		dA.w.z = tmp * dtinv;
		db.w.z = spring * (diff) * tmp;
	}
}

/*void PHHingeJoint::CompError(double dt){
	B.SUBVEC(0, 3) = rjrel;
	B.SUBVEC(3, 3) = qjrel.V();
}*/

/*void PHHingeJoint::ProjectionCorrection(double& F, int k){
	if(k == 5){
		if(on_lower)
			F = max(0.0, F);
		if(on_upper)
			F = min(0.0, F);
	}
}*/

//-----------------------------------------------------------------------------
//OBJECT_IMP(PHHingeJointNode, PHTreeNode1D);

void PHHingeJointNode::CompJointJacobian(){
	J[0].v.clear();
	J[0].w = Vec3d(0.0, 0.0, 1.0);
	PHTreeNode1D::CompJointJacobian();
}
void PHHingeJointNode::CompJointCoriolisAccel(){
	cj.clear();
}
void PHHingeJointNode::CompRelativeVelocity(){
	PHJoint1D* j = GetJoint();
	j->vjrel.v.clear();
	j->vjrel.w = Vec3d(0.0, 0.0, j->velocity[0]);
}
void PHHingeJointNode::CompRelativePosition(){
	PHJoint1D* j = GetJoint();
	j->Xjrel.R = Matrix3d::Rot(j->position[0], 'z');
	j->Xjrel.r.clear();
}
void PHHingeJointNode::CompBias(){
	PHJoint1D* j = GetJoint();
	double diff;
	double dt = scene->GetTimeStep(), dtinv = 1.0 / dt;
	if(j->mode == PHJoint::MODE_VELOCITY){
		db[0] = -j->vel_d;
	}
	else if(j->spring != 0.0 || j->damper != 0.0){
		diff = j->GetPosition() - j->origin;
		while(diff >  M_PI) diff -= 2 * M_PI;
		while(diff < -M_PI) diff += 2 * M_PI;
		double tmp = 1.0 / (j->damper + j->spring * dt);
		dA[0] = tmp * dtinv;
		db[0] = j->spring * (diff) * tmp;
	}
	else dA[0] = db[0] = 0.0;
}

}

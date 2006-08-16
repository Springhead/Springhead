// THuman.h: THuman クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>
#include <Physics/PHHingeJoint.h>
#include <Physics/PHSliderJoint.h>

#if !defined(AFX_THUMAN_H__FF8DFEAD_290E_4F9C_A0F7_06A5639A6B0D__INCLUDED_)
#define AFX_THUMAN_H__FF8DFEAD_290E_4F9C_A0F7_06A5639A6B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SOLID_NUM 15
#define JOINT_NUM 28

class TSpring{
public:
	Spr::PHSolid* solid;
	Spr::Vec3f pos;
	TSpring():solid(NULL){}
	TSpring(Spr::PHSolid* s, Spr::Vec3f p):solid(s), pos(p){}
	Spr::Vec3f GetPos();
	Spr::Vec3f GetVel();
	void AddForce(Spr::Vec3f f){
		if (solid) solid->AddForce(f, GetPos());
	}
};
class TSprings:public std::vector<TSpring>{
};
class THuman  
{
public:
	class TAngle{
	public:
		Spr::Vec3f bodyVel;
		Spr::Quaternionf bodyOri;
		Spr::Vec3f bodyAngVel;
		Spr::Vec3f force;
		Spr::Vec3f torque;
		unsigned nAngle;
		float angle[30];
		float angVel[30];
		float jointTorque[30];
		TAngle():nAngle(0){}
	};
	class TAngles: public std::vector<TAngle>{
	};
public:
	TSprings springs[2];
	TAngles angles;
	std::vector<Spr::PHSolid*> solids;
	std::vector<Spr::PHJoint1D*> joints;
	//PHSolid* solids[SOLID_NUM];
	Spr::PHSolid *soBody, *soHead, *soRUArm, *soLUArm, *soRFArm, *soLFArm, *soRHand, *soLHand,
				*soWaist, *soRULeg, *soLULeg, *soRLLeg, *soLLLeg, *soRFoot, *soLFoot;
	//PHJoint1D* joints[JOINT_NUM];
	Spr::PHHingeJoint *joWaist1, *joWaist2, *joWaist3, *joNeck1, *joNeck2, *joNeck3,
				*joRShoulder1, *joRShoulder2, *joRShoulder3,
				*joRElbow1, *joRWrist1, *joRWrist2,
				*joLShoulder1, *joLShoulder2, *joLShoulder3,
				*joLElbow1, *joLWrist1, *joLWrist2,
				*joRHip1, *joRHip2, *joRHip3, *joRKnee1, *joRAnkle1,
				*joLHip1, *joLHip2, *joLHip3, *joLKnee1, *joLAnkle1;
	Spr::PHSliderJoint			*joLShin, *joRShin;
	bool bLoaded;

	
	THuman();
	virtual ~THuman();
	bool Connect(Spr::PHScene* scene);
	void ConnectSolid(Spr::PHSolid*& solid, const char* name, Spr::PHScene* scene);
	void ConnectJoint(Spr::PHJoint1D*& solid, const char* name, Spr::PHScene* scene);

	bool IsLoaded(){ return bLoaded; }
	void SaveJointAngle();
	bool LoadJointAngle(float time, float dt);
	void SaveForce(float time, float dt);
	void SetSpring(float dt);
	
	void SetScale(Spr::Vec2f head, Spr::Vec2f shoulder, Spr::Vec2f body, Spr::Vec2f hip,
		float armLen, float forearmLen, float handLen, float thighLen, float shinLen,
		Spr::Vec3f foot, Spr::Vec3f pos6, Spr::Vec3f pos7);

	void SetTotalMass(float tm);
	void SetOneInertia(Spr::PHSolid* solid, float longAxis, float rate);

protected:
	void SaveOneAngle(TAngle& a, Spr::PHJoint1D* j);
	void LoadOneAngle(TAngle& a, int c, Spr::PHJoint1D* j);
	void SaveOneTorque(TAngle& a, int c, Spr::PHJoint1D* j);
	void JointPIDMul(Spr::PHJoint1D* jo, float mul);
};

#endif // !defined(AFX_THUMAN_H__FF8DFEAD_290E_4F9C_A0F7_06A5639A6B0D__INCLUDED_)

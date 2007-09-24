#include ".\crwcfootforce.h"
#include <Base/TMatrix.h>
#include <Base/TVector.h>

using namespace PTM;

CRWCFootForce::CRWCFootForce(double ts, Vec3d g, bool lf)
{
    LF = lf;
    TimeStep = ts;
	Gravity = g;
}

CRWCFootForce::~CRWCFootForce(void)
{
}

void CRWCFootForce::Init(){
	footMoveTermFlag = true;
}

void CRWCFootForce::UpdateState(Vec3d cls, Vec3d nls, Vec3d ph, Vec3d vh, double tl, bool la, bool ec){
	CurrentLandingSite = cls;
	NextLandingSite = nls;
	PositionOfHip = ph;
	VelocityOfHip = vh;
	timeleft = tl;
	LandAble = la;
	EarthConnection = ec;
}

void CRWCFootForce::ChangeSupportLegs(bool lf){

	if(lf) {
		LF = false;
		RF = true;
	}
	else {
		LF = true;
		RF = false;
	}
}


//—¼‹r‚Ì§Œä
void CRWCFootForce::FootMove(){

	double kp = 500.0;
	double kv = 80.0;
	double LengthSwing;
	double LengthSupport;
	double CrossSwing;
	double CrossSupport;
	double CrossVelocitySwing;
	double CrossVelocitySupport;
	Vec3d ForceSwing = Vec3d(0.0,0.0,0.0);
	Vec3d ForceSupport = Vec3d(0.0,0.0,0.0);
	Vec3d Rotleft;
	Vec3d Rotright;
	Vec3d AVleft;
	Vec3d AVright;
	Vec3d HipToSwingFoot;
	Vec3d HipToSupportFoot;
	Vec3d VelocityHipSwing;
	Vec3d VelocityHipSupport;


    if(LandAble == true) ForceSwing = CalcSwingFootForce();
	if(EarthConnection == true) ForceSupport = CalcSupportFootForce();

	if(LF == true){
		LengthSwing = (PositionOfHip - FootLeft->GetCenterPosition()).norm();
		LengthSupport = (PositionOfHip - FootRight->GetCenterPosition()).norm();
		HipToSwingFoot = FootLeft->GetCenterPosition() - PositionOfHip;
		HipToSupportFoot = FootRight->GetCenterPosition() - PositionOfHip;
		VelocityHipSwing = FootLeft->GetVelocity() - VelocityOfHip;
		VelocityHipSupport = FootRight->GetVelocity() - VelocityOfHip;
	} else {
		LengthSwing = (PositionOfHip - FootRight->GetCenterPosition()).norm();
		LengthSupport = (PositionOfHip - FootLeft->GetCenterPosition()).norm();
		HipToSwingFoot = FootRight->GetCenterPosition() - PositionOfHip;
		HipToSupportFoot = FootLeft->GetCenterPosition() - PositionOfHip;
		VelocityHipSwing = FootRight->GetVelocity() - VelocityOfHip;
		VelocityHipSupport = FootLeft->GetVelocity() - VelocityOfHip;
	}

	if(LengthSwing > MaxFootLength) {
		if(CrossSwing = HipToSwingFoot.dot(ForceSwing) > 0.0){
			ForceSwing = ForceSwing - (CrossSwing/LengthSwing) * HipToSwingFoot.unit();
		}
        CrossVelocitySwing = VelocityHipSwing.dot(HipToSwingFoot);
		ForceSwing = ForceSwing + kp*((MaxFootLength/LengthSwing)*HipToSwingFoot - HipToSwingFoot) + kv*((-CrossVelocitySwing/LengthSwing)*HipToSwingFoot.unit());
	}


	if(LengthSupport > MaxFootLength) {
		if(CrossSupport = HipToSupportFoot.dot(ForceSupport) > 0.0){
			ForceSupport = ForceSupport - (CrossSupport/LengthSupport) * HipToSupportFoot.unit();
		}
        CrossVelocitySupport = VelocityHipSupport.dot(HipToSupportFoot);
		ForceSupport = ForceSupport + kp*((MaxFootLength/LengthSupport)*HipToSupportFoot - HipToSupportFoot) + kv*((-CrossVelocitySupport/LengthSupport)*HipToSupportFoot.unit());
	}

	if(LengthSwing < MinFootLength) {
        CrossVelocitySwing = VelocityHipSwing.dot(HipToSwingFoot);
		ForceSwing = kp*((MinFootLength/LengthSwing)*HipToSwingFoot - HipToSwingFoot) + kv*((-CrossVelocitySwing/LengthSwing)*HipToSwingFoot.unit());
	}

	if(LengthSupport < MinFootLength) {
        CrossVelocitySupport = VelocityHipSupport.dot(HipToSupportFoot);
		ForceSupport = kp*((MinFootLength/LengthSupport)*HipToSupportFoot - HipToSupportFoot) + kv*((-CrossVelocitySupport/LengthSupport)*HipToSupportFoot.unit());
	}

	//DSTR << "ForceSwing = " << ForceSwing << std::endl; 

	if(LF == true){
		FootLeft->AddForce(ForceSwing);
		FootRight->AddForce(ForceSupport);
	} else {
		FootRight->AddForce(ForceSwing);
		FootLeft->AddForce(ForceSupport);
	}

	Rotleft = (FootLeft->GetOrientation()).Rotation();
	Rotright = (FootRight->GetOrientation()).Rotation();
	AVleft = FootLeft->GetAngularVelocity();
	AVright = FootRight->GetAngularVelocity();

	FootLeft->AddTorque(-kp * Rotleft - kv * AVleft);
    FootRight->AddTorque(-kp * Rotright - kv * AVright);
}

//ŽxŽ‹r‚Ì“®‚«‚ð¶¬‚·‚é—Í‚ÌŒvŽZ
Vec3d CRWCFootForce::CalcSwingFootForce(void){

	Vec3d Force = Vec3d(0.0,0.0,0.0);
	double kp = 500.0;
    double kv = 80.0;
	double pdx;
	double vdx;
	double pdy;
	double vdy;
	double pdz;
	double vdz;
	double cx;
	double cvx;
	double cy;
	double cvy;
	double cz;
	double cvz;
	double tl;

	if(LF == true){
		cx = FootLeft->GetCenterPosition().x;
		cvx = FootLeft->GetVelocity().x;
		cy = FootLeft->GetCenterPosition().y;
		cvy = FootLeft->GetVelocity().y;
		cz = FootLeft->GetCenterPosition().z;
		cvz = FootLeft->GetVelocity().z;
	} else {
		cx = FootRight->GetCenterPosition().x;
		cvx = FootRight->GetVelocity().x;
		cy = FootRight->GetCenterPosition().y;
		cvy = FootRight->GetVelocity().y;
		cz = FootRight->GetCenterPosition().z;
	    cvz = FootRight->GetVelocity().z;
	}

	if(timeleft == 0.0) tl = TimeStep;
	else tl = timeleft;

	vdx = (NextLandingSite.x - cx) / tl;
	pdx = cx + vdx * TimeStep;

	vdz = (NextLandingSite.z - cz) / tl;
	pdz = cz + vdz * TimeStep;
	
	if(timeleft > T0 / 2.0 && footMoveTermFlag == true) {
		vdy = (FootSize/2.0 + 2.0 * footheight - cy) / tl;            /////////
		if(cy > FootSize/2.0 + footheight) footMoveTermFlag = false;  /////////
	    }
	else {
		vdy = (FootSize/2.0 - cy) / tl;                               /////////
	}
    pdy = cy + TimeStep * vdy;

	//DSTR << "vdx = " << vdx << " vdz = " << vdz << " vdy = " << vdy << std::endl;

	Force = Vec3d(kp * (pdx - cx) + kv * (vdx - cvx) , 0.0 , 0.0);
	Force = Force + Vec3d(0.0 , 0.0 , kp * (pdz - cz) + kv * (vdz - cvz));
	Force = Force + FootRight->GetMass() * (-1) * Gravity;
    Force = Force + Vec3d(0.0, kp * (pdy - cy) + kv * (vdy - cvy) , 0.0);

	return Force;
}

//ŽxŽ‹r‚Ì“®‚«‚ð¶¬‚·‚é—Í‚ÌŒvŽZ
Vec3d CRWCFootForce::CalcSupportFootForce(void){

	double kp = 500.0;
    double kv = 80.0;
	Vec3d Force;

	if(LF == true){
		    Force = Vec3d(kp * (CurrentLandingSite.x - FootRight->GetCenterPosition().x) - kv * (FootRight->GetVelocity().x) , 0.0, 0.0);
            Force = Force + Vec3d(0.0, 0.0, kp * (CurrentLandingSite.z - FootRight->GetCenterPosition().z) - kv * (FootRight->GetVelocity().z));
	} else {
		Force = Vec3d(kp * (CurrentLandingSite.x - FootLeft->GetCenterPosition().x) - kv * (FootLeft->GetVelocity().x) , 0.0, 0.0);
	    Force = Force + Vec3d(0.0, 0.0, kp * (CurrentLandingSite.z - FootLeft->GetCenterPosition().z) - kv * (FootLeft->GetVelocity().z));
	}

	return Force;
}

//—¼‹rŽxŽŠúŠÔ‚ÌØ‚è‘Ö‚¦‘O‚Ì‘«‚Ì“®‚«‚ð¶¬‚·‚é—Í‚ÌŒvŽZ
Vec3d CRWCFootForce::CalcDoubleSupportPreFootForce(void){

	double kp = 1100.0;
	double kv = 70.0;
	Vec3d Force;

	if(LF == true){
		Force = Vec3d(kp * (CurrentLandingSite.x - FootRight->GetCenterPosition().x) - kv * (FootRight->GetVelocity().x) , 0.0, 0.0);
		Force = Force + Vec3d(0.0, 0.0, kp * (CurrentLandingSite.z - FootRight->GetCenterPosition().z) - kv * (FootRight->GetVelocity().z));
	} else {
		Force = Vec3d(kp * (CurrentLandingSite.x - FootLeft->GetCenterPosition().x) - kv * (FootLeft->GetVelocity().x) , 0.0, 0.0);
		Force = Force + Vec3d(0.0, 0.0, kp * (CurrentLandingSite.z - FootLeft->GetCenterPosition().z) - kv * (FootLeft->GetVelocity().z));
	}

	return Force;
}

//—¼‹rŽxŽŠúŠÔ‚ÌØ‚è‘Ö‚¦Œã‚Ì‘«‚Ì“®‚«‚ð¶¬‚·‚é—Í‚ÌŒvŽZ
Vec3d CRWCFootForce::CalcDoubleSupportNextFootForce(void){

	double kp = 1100.0;
	double kv = 70.0;
	Vec3d Force;

	if(LF == true){
		Force = Vec3d(kp * (NextLandingSite.x - FootLeft->GetCenterPosition().x) - kv * (FootLeft->GetVelocity().x) , 0.0, 0.0);
		Force = Force + Vec3d(0.0, 0.0, kp * (NextLandingSite.z - FootLeft->GetCenterPosition().z) - kv * (FootLeft->GetVelocity().z));
	} else {
		Force = Vec3d(kp * (NextLandingSite.x - FootRight->GetCenterPosition().x) - kv * (FootRight->GetVelocity().x) , 0.0, 0.0);
		Force = Force + Vec3d(0.0, 0.0, kp * (NextLandingSite.z - FootRight->GetCenterPosition().z) - kv * (FootRight->GetVelocity().z));
	}

	return Force;
}
//—¼‹rŽxŽŠúŠÔ‚Ì‹r‚Ì“®‚«
void CRWCFootForce::FootDoubleSupport(void){

	double kp = 1100.0;
	double kv = 70.0;
    double LengthPre;
	double LengthNext;
	double CrossPre;
	double CrossNext;
	double CrossVelocityPre;
	double CrossVelocityNext;
	Vec3d ForcePre = Vec3d(0.0,0.0,0.0);
	Vec3d ForceNext = Vec3d(0.0,0.0,0.0);
	Vec3d Rotleft;
	Vec3d Rotright;
	Vec3d AVleft;
	Vec3d AVright;
	Vec3d HipToPreFoot;
	Vec3d HipToNextFoot;
	Vec3d VelocityHipPre;
	Vec3d VelocityHipNext;


	footMoveTermFlag = true;

	if(EarthConnection == true) ForcePre = CalcDoubleSupportPreFootForce();
	if(LandAble == true) ForceNext = CalcDoubleSupportNextFootForce();

	if(LF == true){
		LengthNext = (PositionOfHip - FootLeft->GetCenterPosition()).norm();
		LengthPre = (PositionOfHip - FootRight->GetCenterPosition()).norm();
		HipToNextFoot = FootLeft->GetCenterPosition() - PositionOfHip;
		HipToPreFoot = FootRight->GetCenterPosition() - PositionOfHip;
		VelocityHipNext = FootLeft->GetVelocity() - VelocityOfHip;
		VelocityHipPre = FootRight->GetVelocity() - VelocityOfHip;
	} else {
		LengthNext = (PositionOfHip - FootRight->GetCenterPosition()).norm();
		LengthPre = (PositionOfHip - FootLeft->GetCenterPosition()).norm();
		HipToNextFoot = FootRight->GetCenterPosition() - PositionOfHip;
		HipToPreFoot = FootLeft->GetCenterPosition() - PositionOfHip;
		VelocityHipNext = FootRight->GetVelocity() - VelocityOfHip;
		VelocityHipPre = FootLeft->GetVelocity() - VelocityOfHip;
	}

	if(LengthNext > MaxFootLength) {
		if(CrossNext = HipToNextFoot.dot(ForceNext) > 0.0){
			ForceNext = ForceNext - (CrossNext/LengthNext) * HipToNextFoot.unit();
		}
        CrossVelocityNext = VelocityHipNext.dot(HipToNextFoot);
		ForceNext = ForceNext + kp*((MaxFootLength/LengthNext)*HipToNextFoot - HipToNextFoot) + kv*((-CrossVelocityNext/LengthNext)*HipToNextFoot.unit());
	}

	if(LengthPre > MaxFootLength) {
		if(CrossPre = HipToPreFoot.dot(ForcePre) > 0.0){
			ForcePre = ForcePre - (CrossPre/LengthPre) * HipToPreFoot.unit();
		}
        CrossVelocityPre = VelocityHipPre.dot(HipToPreFoot);
		ForcePre = ForcePre + kp*((MaxFootLength/LengthPre)*HipToPreFoot - HipToPreFoot) + kv*((-CrossVelocityPre/LengthPre)*HipToPreFoot.unit());
	}

	if(LengthNext < MinFootLength) {
        CrossVelocityNext = VelocityHipNext.dot(HipToNextFoot);
		ForceNext = kp*((MinFootLength/LengthNext)*HipToNextFoot - HipToNextFoot) + kv*((-CrossVelocityNext/LengthNext)*HipToNextFoot.unit());
	}

	if(LengthPre < MinFootLength) {
        CrossVelocityPre = VelocityHipPre.dot(HipToPreFoot);
		ForcePre = kp*((MinFootLength/LengthPre)*HipToPreFoot - HipToPreFoot) + kv*((-CrossVelocityPre/LengthPre)*HipToPreFoot.unit());
	}

	if(LF == true){
		FootRight->AddForce(ForcePre);
		FootLeft->AddForce(ForceNext);
	} else {
		FootLeft->AddForce(ForcePre);
		FootRight->AddForce(ForceNext);
	}
	
	Rotleft = (FootLeft->GetOrientation()).Rotation();
	Rotright = (FootRight->GetOrientation()).Rotation();
	AVleft = FootLeft->GetAngularVelocity();
	AVright = FootRight->GetAngularVelocity();

	FootLeft->AddTorque(-kp * Rotleft - kv * AVleft);
    FootRight->AddTorque(-kp * Rotright - kv * AVright);

	FootLeft->AddForce(Vec3d(0.0, kp*(FootSize/2.0 - FootLeft->GetCenterPosition().y) - kv*FootLeft->GetVelocity().y, 0.0)); 
	FootRight->AddForce(Vec3d(0.0, kp*(FootSize/2.0 - FootRight->GetCenterPosition().y) - kv*FootRight->GetVelocity().y, 0.0)); 

	//DSTR << "footleft.y = " << FootLeft->GetCenterPosition().y << std::endl;
	//DSTR << "footrihgt.y = " << FootRight->GetCenterPosition().y << std::endl;
}

/*
//—¼‹r‚Ì§Œä
void FootForce::FootMove(){

	double kp = 0.6;
	double kv = 0.8;
	Vec3d Rotleft;
	Vec3d Rotright;
	Vec3d AVleft;
	Vec3d AVright;

	if(LF == true) {
			if(LandAble == true) FootMoveLeft();
		    FootRight->AddForce(Vec3d(kp * (CurrentLandingSite.x - FootRight->GetCenterPosition().x) - kv * (FootRight->GetVelocity().x) , 0.0, 0.0));
            FootRight->AddForce(Vec3d(0.0, 0.0, kp * (CurrentLandingSite.z - FootRight->GetCenterPosition().z) - kv * (FootRight->GetVelocity().z)));
	}
	else {
		if(LandAble == true) FootMoveRight();
        FootLeft->AddForce(Vec3d(kp * (CurrentLandingSite.x - FootLeft->GetCenterPosition().x) - kv * (FootLeft->GetVelocity().x) , 0.0, 0.0));
	    FootLeft->AddForce(Vec3d(0.0, 0.0, kp * (CurrentLandingSite.z - FootLeft->GetCenterPosition().z) - kv * (FootLeft->GetVelocity().z)));
	}      


	Rotleft = (FootLeft->GetOrientation()).Rotation();
	Rotright = (FootRight->GetOrientation()).Rotation();
	AVleft = FootLeft->GetAngularVelocity();
	AVright = FootRight->GetAngularVelocity();

	FootLeft->AddTorque(-kp * Rotleft - kv * AVleft);
    FootRight->AddTorque(-kp * Rotright - kv * AVright);
}

//¶‹r‚Ì—V‹r§Œä
void FootForce::FootMoveLeft(){
	double kp = 0.6;
    double kv = 0.9;
	double pdx;
	double vdx;
	double pdy;
	double vdy;
	double pdz;
	double vdz;
	double cx = FootLeft->GetCenterPosition().x;
	double cvx = FootLeft->GetVelocity().x;
	double cy = FootLeft->GetCenterPosition().y;
	double cvy = FootLeft->GetVelocity().y;
	double cz = FootLeft->GetCenterPosition().z;
	double cvz = FootLeft->GetVelocity().z;
	double tl;

	if(timeleft == 0.0) tl = TimeStep;
	else tl = timeleft;

	vdx = (NextLandingSite.x - cx) / tl;
	pdx = cx + vdx * TimeStep;

	vdz = (NextLandingSite.z - cz) / tl;
	pdz = cz + vdz * TimeStep;

	
	if(timeleft > T0 / 2.0 && footMoveTermFlag == true) {
		vdy = (FootSize/2.0 + 2.0 * footheight - cy) / tl;
		if(cy > FootSize/2.0 + footheight) footMoveTermFlag = false;
	    }
	else {
		vdy = (FootSize/2.0 - cy) / tl;
	}
    pdy = cy + TimeStep * vdy;

	FootLeft->AddForce(Vec3d(kp * (pdx - cx) + kv * (vdx - cvx) , 0.0 , 0.0));
	FootLeft->AddForce(Vec3d(0.0, 0.0,kp * (pdz - cz) + kv * (vdz - cvz)));

	FootLeft->AddForce(FootLeft->GetMass() * (-1) * Gravity);
	FootLeft->AddForce(Vec3d(0.0, kp * (pdy - cy) + kv * (vdy - cvy) , 0.0));

	DSTR << kp * (pdx - cx) + kv * (vdx - cvx) << std::endl;
} 

//‰E‹r‚Ì—V‹r§Œä
void FootForce::FootMoveRight(){

	double kp = 0.6;
    double kv = 0.9;
	double pdx;
	double vdx;
	double pdy;
	double vdy;
	double pdz;
	double vdz;
	double cx = FootRight->GetCenterPosition().x;
	double cvx = FootRight->GetVelocity().x;
	double cy = FootRight->GetCenterPosition().y;
	double cvy = FootRight->GetVelocity().y;
	double cz = FootRight->GetCenterPosition().z;
	double cvz = FootRight->GetVelocity().z;
    double tl;

	if(timeleft == 0.0) tl = TimeStep;
	else tl = timeleft;

	vdx = (NextLandingSite.x - cx) / tl;
	pdx = cx + vdx * TimeStep;

	vdz = (NextLandingSite.z - cz) / tl;
	pdz = cz + vdz * TimeStep;

	if(timeleft > T0 / 2.0 && footMoveTermFlag == true) {
		vdy = (FootSize/2.0 + 2.0 * footheight - cy) / tl;
		if(cy > FootSize/2.0 + footheight) footMoveTermFlag = false;
	} else {
		vdy = (FootSize/2.0 - cy) / tl;
	}
    pdy = cy + TimeStep * vdy;

	FootRight->AddForce(Vec3d(kp * (pdx - cx) + kv * (vdx - cvx) , 0.0 , 0.0));
	FootRight->AddForce(Vec3d(0.0 , 0.0 , kp * (pdz - cz) + kv * (vdz - cvz)));

	FootRight->AddForce(FootRight->GetMass() * (-1) * Gravity);
    FootRight->AddForce(Vec3d(0.0, kp * (pdy - cy) + kv * (vdy - cvy) , 0.0));
}
*/
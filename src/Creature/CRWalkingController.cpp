/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRWalkingController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
IF_OBJECT_IMP(CRWalkingController, CRController);

/////////////遊脚制御の実験に使用している変数////
/////////////////////////////////////////////////

//ベクトルobjects内に含まれるソリッドを,一つの剛体と見た重心位置を返す
Vec3d CRWalkingController::GetCenterOfBlocks(std::vector<PHSolidIf*> objects){
	
	Vec3d CenterOfBlock = Vec3d(0,0,0);
	double TotalWeight = 0;

	for(int i = 0;i < (int)objects.size(); i++){
		CenterOfBlock = CenterOfBlock + objects[i]->GetCenterPosition() * objects[i]->GetMass();
		TotalWeight = TotalWeight + objects[i]->GetMass(); 
	}

	return CenterOfBlock / TotalWeight;
}


Vec3d CRWalkingController::CalcCenterVelocity(std::vector<PHSolidIf*> objs){

	Vec3d velocity = Vec3d(0,0,0);
	double TotalWeight = 0;

	for(int i = 0;i < (int)objs.size(); i++){
		velocity = velocity + objs[i]->GetVelocity() * objs[i]->GetMass();
		TotalWeight = TotalWeight + objs[i]->GetMass(); 
	}

	return velocity / TotalWeight;
}

double CRWalkingController::TotalMass(std::vector<PHSolidIf*> objects){

	double TotalMass = 0.0;

	for(int i = 0;i < (int)objects.size(); i++){
         TotalMass = TotalMass + objects[i]->GetMass();
	}

	return TotalMass;
}

//歩行パラメータから基本歩行周期を計算
double CRWalkingController::CalcBasicCycle(){
	double T;

    Tc = sqrt(height / abs(phScene->GetGravity().y));

	if((paramVelocityX - paramHalfStrideX / Tc) <= 0){
		DSTR << "inadequate parameter VelocityX,height,paramHalfStrideX" << std::endl;
		exit(0);
	}

	T = Tc * log((paramVelocityX + paramHalfStrideX/Tc) / (paramVelocityX - paramHalfStrideX/Tc));

    return T;
}

//Z方向のパラメータを決定する
double CRWalkingController::CalcZParamV(double hz, double restT){

	double Tc;
    double sinh;
	double cosh;
	double v;

    Tc = sqrt(height / abs(phScene->GetGravity().y));
    sinh = exp(restT / Tc) / 2 - exp(- restT / Tc) / 2;
	cosh = exp(restT / Tc) / 2 + exp(- restT / Tc) / 2;

    v = hz*sinh / (Tc*(1+cosh));

	return v;
}

//Z方向のパラメータを決定する
double CRWalkingController::CalcZParamH(double v, double restT){

	double Tc;
    double sinh;
	double cosh;
	double h;

    Tc = sqrt(height / abs(phScene->GetGravity().y));
    sinh = exp(restT / Tc) / 2 - exp(- restT / Tc) / 2;
	cosh = exp(restT / Tc) / 2 + exp(- restT / Tc) / 2;

    h = v*(1+cosh)*Tc / sinh;

	return h;
}

double CRWalkingController::CalcLocalX(double xb, double zb, double xt, double zt, double theta){

	double vari0;
	double vari1;
	double relativeX;

	if(abs(tan(theta)) > 100000.0) {
		vari0 = xt;
		vari1 = zb;
	}
	else if(abs(1/tan(theta)) > 100000.0) {
		vari0 = xb;
		vari1 = zt;
	}
	else {
		vari0 = ((zt - zb) + tan(theta)*xt + xb/tan(theta)) / (tan(theta) + 1/tan(theta));
		vari1 = ((xt - xb) + tan(theta)*zb + zt/tan(theta)) / (tan(theta) + 1/tan(theta));
	}

	if(abs(theta) < pi/2.0){
		if(vari0 < xt) relativeX = sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
		else relativeX = -sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
	} else if(abs(theta) > pi/2.0 && abs(theta) < 3.0*pi/2.0) {
		if(vari0 < xt) relativeX = -sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
		else relativeX = sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
	} else if(abs(theta) > 3.0/2.0*pi){
		if(vari0 < xt) relativeX = sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
		else relativeX = -sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
    } else {
		if(theta > 0.0) {
			if(theta < pi) relativeX = zb - zt;
			else  relativeX = zt - zb;
		}
		else {
			if(theta > -pi) relativeX = zt - zb;
			else relativeX = zb - zt;
		}
	}

	return relativeX;
}

double CRWalkingController::CalcLocalZ(double xb, double zb, double xt, double zt, double theta){

	double vari0;
	double vari1;
	double relativeZ;

	if(abs(tan(theta)) > 100000.0) {
		vari0 = xt;
		vari1 = zb;
	}
	else if(abs(1/tan(theta)) > 100000.0) {
		vari0 = xb;
		vari1 = zt;
	}
	else {
		vari0 = ((zt - zb) + tan(theta)*xt + xb/tan(theta)) / (tan(theta) + 1/tan(theta));
		vari1 = ((xt - xb) + tan(theta)*zb + zt/tan(theta)) / (tan(theta) + 1/tan(theta));
	}

	if(abs(theta) < pi/2.0){
		if(vari1 > zb) relativeZ = sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
		else relativeZ = -sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
	} else if(abs(theta) > pi/2.0 && abs(theta) < 3.0*pi/2.0) {
		if(vari1 > zb) relativeZ = -sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
		else relativeZ = sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
	} else if(abs(theta) > 3.0/2.0*pi){
		if(vari1 > zb) relativeZ = sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
		else relativeZ = -sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
	} else {
		if(theta > 0.0) {
			if(theta < pi) relativeZ = xt - xb;
			else relativeZ = xb - xt;
		}
		else {
			if(theta > -pi) relativeZ = xb - xt;
			else relativeZ = xt - xb;
		}
	}

	return relativeZ;
}

double CRWalkingController::CalcLocalVX(double vx, double vz, double theta){

	double LocalVX;

	LocalVX = cos(theta)*vx - sin(theta)*vz;

	return LocalVX;
}

double CRWalkingController::CalcLocalVZ(double vx, double vz, double theta){

	double LocalVZ;

	LocalVZ = cos(theta)*vz + sin(theta)*vx;

	return LocalVZ;
}

void CRWalkingController::CalcCurrentDirection(){

	double angle;

	/*
	if(abs(CenterVelocity.z) > 0.00001 && abs(CenterVelocity.x) > 0.00001) {
		if(CenterVelocity.x > 0.0 && CurrentDirection >= -1.49*pi && CurrentDirection <= 1.49*pi) angle = atan(-CenterVelocity.z / CenterVelocity.x);
		else if(CenterVelocity.x > 0.0 && CurrentDirection > 1.49*pi) angle = atan(-CenterVelocity.z / CenterVelocity.x) + 2*pi;
		else if(CenterVelocity.x > 0.0 && CurrentDirection < -1.49*pi) angle = atan(-CenterVelocity.z / CenterVelocity.x) - 2*pi;
		else if(CurrentDirection > 0.0) angle = atan(-CenterVelocity.z / CenterVelocity.x) + pi;
		else angle = atan(-CenterVelocity.z / CenterVelocity.x) - pi;
	} else if(abs(CenterVelocity.z) > 0.00001 && abs(CenterVelocity.x) <= 0.00001){
		angle = (-CenterVelocity.z / abs(CenterVelocity.z))*pi/2;
	} else angle = 0.0;*/


	//angle = TargetDirection;

	if(angle > 2.0*pi) angle = angle - 2.0*pi;
	if(angle < -2.0*pi) angle = angle + 2.0*pi;

	//DSTR << "CurrentDirection = " << CurrentDirection << std::endl;
	//CurrentDirection = angle;
	CurrentDirection = TargetDirection;
}

void CRWalkingController::CalcTargetAngle(){

	//double paramAngle = 0.3;
	//double CurrentRot;

	//CurrentRot = (soCenterObject->GetOrientation()).Rotation().y;
	/*
	if(flag == 1) {
		flag = 0;
		TargetDirection = TargetDirection + 0.25;
		if(TargetDirection > 2.0*pi) TargetDirection = TargetDirection - 2.0*pi;
	    if(TargetDirection < -2.0*pi) TargetDirection = TargetDirection + 2.0*pi;
		DSTR << "TargetDirection = " << TargetDirection << std::endl;
	} 
	if(flag2 == 1) {
		flag2 = 0;
		TargetDirection = TargetDirection - 0.25;
		if(TargetDirection > 2.0*pi) TargetDirection = TargetDirection - 2.0*pi;
	    if(TargetDirection < -2.0*pi) TargetDirection = TargetDirection + 2.0*pi;
		DSTR << "TargetDirection = " << TargetDirection << std::endl;
	} 
	*/
	
	if(totalStep < 250) TargetDirection = 0.0;
	if(totalStep >= 250 && totalStep < 450) TargetDirection = -pi/12.0;
	if(totalStep >= 450 && totalStep < 650) TargetDirection = -pi/6.0;
	if(totalStep >= 650 && totalStep < 850) TargetDirection = -pi/4.0;
	if(totalStep >= 850 && totalStep < 1050) TargetDirection = -5*pi/12.0;
	if(totalStep >= 1050 && totalStep < 1250) TargetDirection = -pi/2.0;
	if(totalStep >= 1250 && totalStep < 1450) TargetDirection = -7*pi/12.0; 
	if(totalStep >= 1450 && totalStep < 1650) TargetDirection = -8*pi/12.0; 
	if(totalStep >= 1650 && totalStep < 1850) TargetDirection = -9*pi/12.0; 
	if(totalStep >= 1850 && totalStep < 2050) TargetDirection = -10*pi/12.0; 
	if(totalStep >= 2050 && totalStep < 2250) TargetDirection = -11*pi/12.0; 
	if(totalStep >= 2250 && totalStep < 2450) TargetDirection = -pi; 
	if(totalStep >= 2450 && totalStep < 2650) TargetDirection = -13*pi/12.0; 
	if(totalStep >= 2650 && totalStep < 2850) TargetDirection = -14*pi/12.0; 
	if(totalStep >= 2850 && totalStep < 3050) TargetDirection = -15*pi/12.0; 
	if(totalStep >= 3050 && totalStep < 3250) TargetDirection = -16*pi/12.0; 
	if(totalStep >= 3250 && totalStep < 3450) TargetDirection = -17*pi/12.0; 
	if(totalStep >= 3450 && totalStep < 3650) TargetDirection = -18.0*pi/12.0; 
	if(totalStep >= 3650 && totalStep < 3850) TargetDirection = -19*pi/12.0; 
	if(totalStep >= 3850 && totalStep < 4050) TargetDirection = -20*pi/12.0; 
	if(totalStep >= 4050 && totalStep < 4250) TargetDirection = -21*pi/12.0; 
	if(totalStep >= 4250 && totalStep < 4450) TargetDirection = -22*pi/12.0; 
	if(totalStep >= 4450 && totalStep < 4650) TargetDirection = -23.0*pi/12.0; 
	if(totalStep >= 4650 && totalStep < 4850) TargetDirection = -24*pi/12.0; 
	if(totalStep >= 4850 && totalStep < 5050) TargetDirection = -25*pi/12.0; 
	if(totalStep >= 5050 && totalStep < 5250) TargetDirection = -26*pi/12.0; 
	if(totalStep >= 5250 && totalStep < 5450) TargetDirection = -27*pi/12.0; 
	//TargetDirection = CurrentRot + 0.1;
	//TargetDirection = 0.0;
	//TargetDirection = 0.0;
	//DSTR << "TargetDirection = " << TargetDirection << std::endl;
	//if(abs(TargetDirection - ((soCenterObject->GetOrientation()).Rotation()).y) > paramAngle)
	if(TargetDirection > 2.0*pi) TargetDirection = TargetDirection - 2.0*pi;
    if(TargetDirection < -2.0*pi) TargetDirection = TargetDirection + 2.0*pi;
}

//歩行中に速度を変える
void CRWalkingController::VelocityXChange(double vx){

	paramVelocityX = vx;
	T0 = CalcBasicCycle();
	paramVelocityZ = CalcZParamV(paramHalfStrideZ, T0);

	tl->SetParamT0(T0);
	tl->SetParamVelocityX(paramVelocityX);
	tl->SetParamVelocityZ(paramVelocityZ);

    ls->SetParamT0(T0);
	ls->SetParamVelocityX(paramVelocityX);
	ls->SetParamVelocityZ(paramVelocityZ);

	ff->SetParamT0(T0);

	gf->SetParamT0(T0);
	gf->SetParamT0ds(paramLdx/paramVelocityX);
	gf->SetParamVX(paramVelocityX);
	gf->SetParamVZ(paramVelocityZ);
}

//歩行中に歩幅を変える
void CRWalkingController::HalfStrideXChange(double sx){

	paramHalfStrideX = sx;
	T0 = CalcBasicCycle();
	paramVelocityZ = CalcZParamV(paramHalfStrideZ, T0);
                        
	tl->SetParamT0(T0);
	tl->SetParamVelocityZ(paramVelocityZ);

    ls->SetParamT0(T0);
	ls->SetParamVelocityZ(paramVelocityZ);
	ls->SetParamStrideX(paramHalfStrideX);

	ff->SetParamT0(T0);

	gf->SetParamT0(T0);
	gf->SetParamVZ(paramVelocityZ);
}
//歩行中に横方向の速度を変える
void CRWalkingController::VelocityZChange(double vz){

	paramVelocityZ = vz;
	paramHalfStrideZ = CalcZParamV(paramVelocityZ, T0);

	tl->SetParamVelocityZ(paramVelocityZ);
	ls->SetParamVelocityZ(paramVelocityZ);
	gf->SetParamVZ(paramVelocityZ);

	ls->SetParamStrideZ(paramHalfStrideZ);
}

//歩行中に重心の上下動を変える
void CRWalkingController::AmplitudeChange(double amp){
	amplitude = amp;
	gf->SetParamAmplitude(amplitude);
}

//上げる足の高さを変える
void CRWalkingController::FootHeightChange(double fh){
	footHeight = fh;
	ff->SetParamFootHeight(fh);
}

//現在の支持脚が維持できるか判定する
//条件は上体角度, 上体と脚の間の角度, 現在の支持脚の長さから決定する
bool CRWalkingController::CalcSustainable(void){

	Quaterniond qua;
	double MaxRoLandingSite;
	double LocalHipX;
	double LocalHipZ;
	double vari;
	Vec3d Bvec;        //上体の姿勢の単位ベクトル
	Vec3d Lvec;        //腰と着地点を結んだベクトル
	Vec3d dvec = Vec3d(0.0, -1.0 , 0.0);
	Vec3d RoGround;     //地面と上体の間の角度
	Vec3d RoLandingSite;//着地点と上体の間の角度

	LocalHipX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, soHip->GetCenterPosition().x, soHip->GetCenterPosition().z, CurrentDirection);
	LocalHipZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, soHip->GetCenterPosition().x, soHip->GetCenterPosition().z, CurrentDirection);

	Bvec = (soHip->GetCenterPosition() - PositionOfUpperBody).unit();
	Lvec = Vec3d(LocalHipX, (-1)*soHip->GetCenterPosition().y , LocalHipZ);

    qua.RotationArc(Bvec, dvec);
	RoGround = qua.Rotation();
	qua.RotationArc(Bvec, Lvec);
	RoLandingSite = qua.Rotation();

	vari = (1 - abs(atan(LocalHipZ/LocalHipX)) / (pi*2.0)) * (1 - abs(atan(LocalHipZ/LocalHipX)) / (pi*2.0));

	if(LocalHipX > 0.0) MaxRoLandingSite = (1-vari) * MaxRoLandingSiteSide + vari * MaxRoLandingSiteFront;
	else MaxRoLandingSite = (1-vari) * MaxRoLandingSiteSide + vari * MaxRoLandingSiteBack;

	//DSTR << "RoGround.norm()= " << RoGround.norm() << " RoLandingSite.norm() = " << RoLandingSite.norm() << " Lvec.norm() = "<< Lvec.norm() << std::endl;
	//DSTR << currentlandingsite << std::endl;
	//DSTR << nextlandingsite << std::endl;
	if(RoGround.norm() > MaxRoGround || RoLandingSite.norm() > MaxRoLandingSite || Lvec.norm() > MaxFootLength){
		DSTR << "Fall : Supportleg cannot sustain" << std::endl;
		return false;}
	else return true;
}


//身体的制約によって発生する力をオブジェクトに与える
void CRWalkingController::ConstraintForce(void){
 
	double kp = 3.0;
	double kv = 1.0;
	double param0 = 50.0;
	//double param1 = 3.0;
	//double MaxConstraintVelocity = 25.0;
	double LengthLeft;
	double LengthRight;
	double CrossVelocityLeft;
	double CrossVelocityRight;
	double HousenVelocityFootLeft;
    double HousenVelocityFootRight;
	double HousenVelocityBodyLeft;
	double HousenVelocityBodyRight;
	Vec3d ForceLeft = Vec3d(0.0,0.0,0.0);
	Vec3d ForceRight = Vec3d(0.0,0.0,0.0);
	Vec3d HipToLeftFoot;
	Vec3d HipToRightFoot;
	Vec3d Bvec;        //上体の姿勢の単位ベクトル
	Vec3d Leftvec;        //腰と左脚を結んだベクトル
	Vec3d Rightvec;       //腰と右脚を結んだベクトル
	Vec3d dvec = Vec3d(0.0, -1.0 , 0.0);
	Vec3d RoLeft;         //左足と上体の間の角度
	Vec3d RoRight;         //右足と上体の間の角度
	Vec3d HousenFootLeft = Vec3d(0.0,0.0,0.0);
	Vec3d HousenBodyLeft = Vec3d(0.0,0.0,0.0);
	Vec3d HousenFootRight = Vec3d(0.0,0.0,0.0);
	Vec3d HousenBodyRight = Vec3d(0.0,0.0,0.0);
	Vec3d ForceFootLeft;
	Vec3d ForceBodyLeft;
	Vec3d ForceFootRight;
	Vec3d ForceBodyRight;
	Vec3d VelocityHipFootLeft = footleft->GetVelocity() - soHip->GetVelocity();
	Vec3d VelocityHipFootRight = footright->GetVelocity() - soHip->GetVelocity();
	Vec3d VelocityHipBody = soHead->GetVelocity() - soHip->GetVelocity();
	Quaterniond qua;
	
	Bvec = soHip->GetCenterPosition() - soHead->GetCenterPosition();
	Leftvec = Vec3d(footleft->GetCenterPosition().x - soHip->GetCenterPosition().x , - soHip->GetCenterPosition().y , footleft->GetCenterPosition().z - soHip->GetCenterPosition().z);
    Rightvec = Vec3d(footright->GetCenterPosition().x - soHip->GetCenterPosition().x , - soHip->GetCenterPosition().y , footright->GetCenterPosition().z - soHip->GetCenterPosition().z);

	qua.RotationArc(Bvec, Leftvec);
	RoLeft = qua.Rotation();
	qua.RotationArc(Bvec, Rightvec);
	RoRight = qua.Rotation();

	//左足の法線方向を計算
	if(Bvec.dot(Leftvec) != 0.0){
		HousenFootLeft = (Leftvec - (Leftvec.dot(Leftvec)/Bvec.dot(Leftvec)) * Bvec).unit();
		if(HousenFootLeft.dot(Bvec) < 0.0) HousenFootLeft = -HousenFootLeft;
		HousenBodyLeft = (Bvec - (Bvec.dot(Bvec)/Bvec.dot(Leftvec)) * Leftvec).unit();
        if(HousenBodyLeft.dot(Leftvec) > 0.0) HousenBodyLeft = -HousenBodyLeft;
	}
	
	//右足の法線方向を計算
	if(Bvec.dot(Rightvec) != 0.0){
		HousenFootRight = (Rightvec - (Rightvec.dot(Rightvec)/Bvec.dot(Rightvec)) * Bvec).unit();
		if(HousenFootRight.dot(Bvec) < 0.0) HousenFootRight = -HousenFootRight;
		HousenBodyRight = (Bvec - (Bvec.dot(Bvec)/Bvec.dot(Rightvec)) * Rightvec).unit();
        if(HousenBodyRight.dot(Rightvec) > 0.0) HousenBodyRight = -HousenBodyRight;
	}

	//腰に対する脚の速さの法線方向の成分
	HousenVelocityFootLeft = VelocityHipFootLeft.dot(HousenFootLeft);
    HousenVelocityFootRight = VelocityHipFootRight.dot(HousenFootRight);
	HousenVelocityBodyLeft = VelocityHipBody.dot(HousenBodyLeft);
    HousenVelocityBodyRight = VelocityHipBody.dot(HousenBodyRight);

	/*
	if(HousenVelocityFootLeft > MaxConstraintVelocity){
		ForceFootLeft = -(param1*HousenVelocityFootLeft) * HousenFootLeft;
		ForceBodyLeft = -(param1*HousenVelocityFootLeft) * (Leftvec.norm()/Bvec.norm()) * HousenBodyLeft;

        solid3->AddForce(ForceBodyLeft);
		solid1->AddForce(-ForceBodyLeft);
		solid1->AddForce(-ForceFootLeft);
		footleft->AddForce(ForceFootLeft);
	}

	
	if(HousenVelocityFootRight > MaxConstraintVelocity){
		ForceFootRight = -(param1*HousenVelocityFootRight) * HousenFootRight;
		ForceBodyRight = -(param1*HousenVelocityFootRight) * (Rightvec.norm()/Bvec.norm()) * HousenBodyRight;

		solid3->AddForce(ForceBodyRight);
		solid1->AddForce(-ForceBodyRight);
		solid1->AddForce(-ForceFootRight);
		footright->AddForce(ForceFootRight);
	}

	
	if(HousenVelocityBodyLeft > MaxConstraintVelocity){
		ForceFootLeft = -(param1*HousenVelocityBodyLeft) * HousenFootLeft;
        ForceBodyLeft = -(param1*HousenVelocityBodyLeft) * (Leftvec.norm()/Bvec.norm()) * HousenBodyLeft;

		solid3->AddForce(ForceBodyLeft);
		solid1->AddForce(-ForceBodyLeft);
		solid1->AddForce(-ForceFootLeft);
		footleft->AddForce(ForceFootLeft);
	}

	if(HousenVelocityBodyRight > MaxConstraintVelocity){
		ForceFootRight = -(param1*HousenVelocityBodyRight) * HousenFootLeft;
        ForceBodyRight = -(param1*HousenVelocityBodyRight) * (Rightvec.norm()/Bvec.norm()) * HousenBodyRight;

		solid3->AddForce(ForceBodyRight);
		solid1->AddForce(-ForceBodyRight);
		solid1->AddForce(-ForceFootRight);
		footright->AddForce(ForceFootRight);
	}*/

	LengthLeft = (soHip->GetCenterPosition() - footleft->GetCenterPosition()).norm();
	LengthRight = (soHip->GetCenterPosition() - footright->GetCenterPosition()).norm();
	HipToLeftFoot = footleft->GetCenterPosition() - soHip->GetCenterPosition();
	HipToRightFoot = footright->GetCenterPosition() - soHip->GetCenterPosition();

	//脚の長さがMaxFootLengthを超えた場合に腰にかける力
	if(LengthLeft > MaxFootLength) {
		DSTR << "path " << std::endl;
        CrossVelocityLeft = VelocityHipFootLeft.dot(HipToLeftFoot);
		ForceLeft = -kp*((MaxFootLength/LengthLeft)*HipToLeftFoot - HipToLeftFoot) - kv*((-CrossVelocityLeft/LengthLeft)*HipToLeftFoot.unit());
		soHip->AddForce(ForceLeft);
	}

	//脚の長さがMaxFootLengthを超えた場合に腰にかける力
	if(LengthRight > MaxFootLength) {
        CrossVelocityRight = VelocityHipFootRight.dot(HipToRightFoot);
		ForceRight = -kp*((MaxFootLength/LengthRight)*HipToRightFoot - HipToRightFoot) - kv*((-CrossVelocityRight/LengthRight)*HipToRightFoot.unit());
		soHip->AddForce(ForceRight);
	}

	//脚の長さがMinFootLengthを下回った場合に腰にかける力
	if(LengthLeft < MinFootLength) {
        CrossVelocityLeft = VelocityHipFootLeft.dot(HipToLeftFoot);
		ForceLeft = -kp*((MinFootLength/LengthLeft)*HipToLeftFoot - HipToLeftFoot) + kv*((CrossVelocityLeft/LengthLeft)*HipToLeftFoot.unit());
		soHip->AddForce(ForceLeft);
	}

	//脚の長さがMinFootLengthを下回った場合に腰にかける力
	if(LengthRight < MinFootLength) {
        CrossVelocityRight = VelocityHipFootRight.dot(HipToRightFoot);
		ForceRight = -kp*((MinFootLength/LengthRight)*HipToRightFoot - HipToRightFoot) + kv*((CrossVelocityRight/LengthRight)*HipToRightFoot.unit());
		soHip->AddForce(ForceRight);
	}

    //脚と上体の角度がMaxRoConstraintを超えた場合に脚と腰にかける力
	if(RoLeft.norm() > MaxRoConstraint) {
		ForceFootLeft = (param0*RoLeft.norm()-MaxRoConstraint) * HousenFootLeft;
		ForceBodyLeft = (param0*RoLeft.norm()-MaxRoConstraint) * (Leftvec.norm()/Bvec.norm()) * HousenBodyLeft;

		soHead->AddForce(ForceBodyLeft);
		soHip->AddForce(-ForceBodyLeft);
		soHip->AddForce(-ForceFootLeft);
		footleft->AddForce(ForceFootLeft);
		}

	//脚と上体の角度がMaxRoConstraintを超えた場合に脚と腰にかける力
	if(RoRight.norm() > MaxRoConstraint) {
		ForceFootRight = (param0*RoRight.norm()-MaxRoConstraint) * HousenFootRight;
		ForceBodyRight = (param0*RoRight.norm()-MaxRoConstraint) * (Rightvec.norm()/Bvec.norm()) * HousenBodyRight;
		
		soHead->AddForce(ForceBodyRight);
	    soHip->AddForce(-ForceBodyRight);   //原因不明.何故かこれを入れるとおかしくなる
		soHip->AddForce(-ForceFootRight);
		footright->AddForce(ForceFootRight);
		}
}

//歩行が継続可能か判定する
//次の着地点が不適切な場合, 腰の位置が一定値を下回った場合, 重心の加速度が一定値を超えた場合, 
//上体の角度がMaxRoGroundを超えた場合, 腰に対する頭の速度, 脚の速度が一定値を超えた場合
//歩行継続を不可能とする
void CRWalkingController::completeFall(){

	double param0 = 0.02;   //quit reason1に関わるパラメータ
	double param1 = 1.3;    //quit reason1に関わるパラメータ
	double param2 = 0.3;    //quit reason1に関わるパラメータ
	double param3 = -0.25;   //quit reason2に関わるパラメータ
	double param4 = -0.17;   //quit reason3に関わるパラメータ
	double param5 = 0.85*MinFootLength;    //これより腰の位置が落ちると歩行継続不可
	double param6 = 18.0;  //これより重心位置の加速度があると歩行継続不可
	double MaxCompleteFallVelocity = 100.0;  //これより腰に対する頭の速度と脚の速度があると歩行継続不可
	double LocalBodyX;
	double LocalBodyZ;
	double LocalNextLandingSiteX;
	double LocalNextLandingSiteZ;
	double LocalBodyVelocityX;
	double LocalBodyVelocityZ;
	Quaterniond qua;
	Vec3d Bvec;        //上体の姿勢の単位ベクトル
	Vec3d dvec = Vec3d(0.0, -1.0 , 0.0);
	Vec3d RoGround;     //地面と上体の間の角度
	Vec3d VelocityHipFootLeft = footleft->GetVelocity() - soHip->GetVelocity();
	Vec3d VelocityHipFootRight = footright->GetVelocity() - soHip->GetVelocity();
	Vec3d VelocityHipBody = soHead->GetVelocity() - soHip->GetVelocity();

	Bvec = (soHip->GetCenterPosition() - PositionOfUpperBody).unit();
    qua.RotationArc(Bvec, dvec);
	RoGround = qua.Rotation();

	LocalBodyX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, PositionOfUpperBody.x, PositionOfUpperBody.z, CurrentDirection);
	LocalBodyZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, PositionOfUpperBody.x, PositionOfUpperBody.z, CurrentDirection);
	LocalNextLandingSiteX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection);
	LocalNextLandingSiteZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection);
	LocalBodyVelocityX = CalcLocalVX(CenterVelocity.x, CenterVelocity.z, CurrentDirection);
	LocalBodyVelocityZ = CalcLocalVZ(CenterVelocity.x, CenterVelocity.z, CurrentDirection);

	if(abs(timeleft) < param0 && LocalBodyVelocityX > param1*paramVelocityX && LocalNextLandingSiteX < LocalBodyX - param2) {completefall = true; 
	DSTR << "quit reason 1 " << std::endl;}
	else if(abs(timeleft) < param0 && LocalBodyVelocityX < -param1*paramVelocityX && LocalNextLandingSiteX > LocalBodyX - param3) {completefall = true;
	DSTR << "quit reason 2 " << std::endl;}
	else if(LF == true && abs(timeleft) < param0 && LocalBodyVelocityZ < 0.0 && LocalNextLandingSiteZ > LocalBodyZ - param4) {completefall = true;
    DSTR << "quit reason 3 " << std::endl;}
	else if(RF == true && abs(timeleft) < param0 && LocalBodyVelocityZ > 0.0 && LocalNextLandingSiteZ < LocalBodyZ + param4) {completefall = true;
    DSTR << "quit reason 4 " << std::endl;}
	else if(soHip->GetCenterPosition().y < param5) {completefall = true;
	DSTR << "quit reason 5 " << std::endl;}
	else if((PreviousV-CenterVelocity).norm()/TimeStep > param6) {completefall = true;
	DSTR << "quit reason 6 " << std::endl;}
	else if(RoGround.norm() > MaxRoGround) {completefall = true;
	DSTR << "quit reason 7 " << std::endl;}
	else if(VelocityHipFootLeft.norm() > MaxCompleteFallVelocity || VelocityHipFootRight.norm() > MaxCompleteFallVelocity || VelocityHipBody.norm() > MaxCompleteFallVelocity) {completefall = true;
	DSTR << "quit reason 8 " << std::endl;
	//DSTR << VelocityHipFootLeft.norm() << " " << VelocityHipFootRight.norm() << " " << VelocityHipBody.norm() << std::endl;
	}
	else completefall = false;
	//DSTR << "hipPosition = " << solid1->GetCenterPosition().y << std::endl;
}

//歩行継続不可となった場合に体のパーツにかける力
void CRWalkingController::fallForce(){

	double kp = 3.0;
	double kv = 1.8;
	double param0 = 50.0;
	double param1 = 3.0;
	double LengthLeft;
	double LengthRight;
	double CrossVelocityLeft;
	double CrossVelocityRight;
	double HousenVelocityFootLeft;
    double HousenVelocityFootRight;
	double HousenVelocityBodyLeft;
	double HousenVelocityBodyRight;
	Vec3d ForceLeft = Vec3d(0.0,0.0,0.0);
	Vec3d ForceRight = Vec3d(0.0,0.0,0.0);
	Vec3d HipToLeftFoot;
	Vec3d HipToRightFoot;
	Vec3d VelocityHipLeft;
	Vec3d VelocityHipRight;
	Vec3d Bvec;        //上体の姿勢の単位ベクトル
	Vec3d Leftvec;        //腰と左脚を結んだベクトル
	Vec3d Rightvec;       //腰と右脚を結んだベクトル
	Vec3d dvec = Vec3d(0.0, -1.0 , 0.0);
	Vec3d RoLeft;         //左足と上体の間の角度
	Vec3d RoRight;         //右足と上体の間の角度
	Vec3d HousenFootLeft = Vec3d(0.0,0.0,0.0);
	Vec3d HousenBodyLeft = Vec3d(0.0,0.0,0.0);
	Vec3d HousenFootRight = Vec3d(0.0,0.0,0.0);
	Vec3d HousenBodyRight = Vec3d(0.0,0.0,0.0);
	Vec3d ForceFootLeft;
	Vec3d ForceBodyLeft;
	Vec3d ForceFootRight;
	Vec3d ForceBodyRight;
	Vec3d VelocityHipFootLeft = footleft->GetVelocity() - soHip->GetVelocity();
	Vec3d VelocityHipFootRight = footright->GetVelocity() - soHip->GetVelocity();
	Vec3d VelocityHipBody = soHead->GetVelocity() - soHip->GetVelocity();
	Vec3d Pd;
	Quaterniond qua;


	
	Bvec = soHip->GetCenterPosition() - soHead->GetCenterPosition();
	Leftvec = Vec3d(footleft->GetCenterPosition().x - soHip->GetCenterPosition().x , - soHip->GetCenterPosition().y , footleft->GetCenterPosition().z - soHip->GetCenterPosition().z);
    Rightvec = Vec3d(footright->GetCenterPosition().x - soHip->GetCenterPosition().x , - soHip->GetCenterPosition().y , footright->GetCenterPosition().z - soHip->GetCenterPosition().z);

	qua.RotationArc(Bvec, Leftvec);
	RoLeft = qua.Rotation();
	qua.RotationArc(Bvec, Rightvec);
	RoRight = qua.Rotation();

	if(Bvec.dot(Leftvec) != 0.0){
		HousenFootLeft = (Leftvec - (Leftvec.dot(Leftvec)/Bvec.dot(Leftvec)) * Bvec).unit();
		if(HousenFootLeft.dot(Bvec) < 0.0) HousenFootLeft = -HousenFootLeft;
		HousenBodyLeft = (Bvec - (Bvec.dot(Bvec)/Bvec.dot(Leftvec)) * Leftvec).unit();
        if(HousenBodyLeft.dot(Leftvec) > 0.0) HousenBodyLeft = -HousenBodyLeft;
	}
	
	if(Bvec.dot(Rightvec) != 0.0){
		HousenFootRight = (Rightvec - (Rightvec.dot(Rightvec)/Bvec.dot(Rightvec)) * Bvec).unit();
		if(HousenFootRight.dot(Bvec) < 0.0) HousenFootRight = -HousenFootRight;
		HousenBodyRight = (Bvec - (Bvec.dot(Bvec)/Bvec.dot(Rightvec)) * Rightvec).unit();
        if(HousenBodyRight.dot(Rightvec) > 0.0) HousenBodyRight = -HousenBodyRight;
	}

	if(soHip->GetCenterPosition().y < 0.3){
		soHead->AddForce(-1.0*soHead->GetVelocity());
	    soCenterObject->AddForce(-1.0*soCenterObject->GetVelocity());
	    soHip->AddForce(-1.0*soHip->GetVelocity());
	    soHip->AddTorque(2*(RoLeft+RoRight));
	}
   
	/*
	ForceFootLeft = (param0*RoLeft.norm()) * HousenFootLeft;
	ForceBodyLeft = (param0*RoLeft.norm()) * (Leftvec.norm()/Bvec.norm()) * HousenBodyLeft;

	solid3->AddForce(ForceBodyLeft);
	solid1->AddForce(-ForceBodyLeft);
	solid1->AddForce(-ForceFootLeft);
	footleft->AddForce(ForceFootLeft);
	
	ForceFootRight = (param0*RoRight.norm()) * HousenFootRight;
	ForceBodyRight = (param0*RoRight.norm()) * (Rightvec.norm()/Bvec.norm()) * HousenBodyRight;
		
	solid3->AddForce(ForceBodyRight);
	solid1->AddForce(-ForceBodyRight);
	solid1->AddForce(-ForceFootRight);
	footright->AddForce(ForceFootRight);
	*/

	LengthLeft = (soHip->GetCenterPosition() - footleft->GetCenterPosition()).norm();
	LengthRight = (soHip->GetCenterPosition() - footright->GetCenterPosition()).norm();
	HipToLeftFoot = footleft->GetCenterPosition() - soHip->GetCenterPosition();
	HipToRightFoot = footright->GetCenterPosition() - soHip->GetCenterPosition();
	VelocityHipLeft = footleft->GetVelocity() - soHip->GetVelocity();
	VelocityHipRight = footright->GetVelocity() - soHip->GetVelocity();
	
	HousenVelocityFootLeft = VelocityHipFootLeft.dot(HousenFootLeft);
    HousenVelocityFootRight = VelocityHipFootRight.dot(HousenFootRight);
	HousenVelocityBodyLeft = VelocityHipBody.dot(HousenBodyLeft);
    HousenVelocityBodyRight = VelocityHipBody.dot(HousenBodyRight);
	

	/*
	ForceFootLeft = -(param1*HousenVelocityFootLeft) * HousenFootLeft;
	ForceBodyLeft = -(param1*HousenVelocityFootLeft) * (Leftvec.norm()/Bvec.norm()) * HousenBodyLeft;
    solid3->AddForce(ForceBodyLeft);
	solid1->AddForce(-ForceBodyLeft);
	solid1->AddForce(-ForceFootLeft);
	footleft->AddForce(ForceFootLeft);

	ForceFootRight = -(param1*HousenVelocityFootRight) * HousenFootRight;
	ForceBodyRight = -(param1*HousenVelocityFootRight) * (Rightvec.norm()/Bvec.norm()) * HousenBodyRight;
	solid3->AddForce(ForceBodyRight);
	solid1->AddForce(-ForceBodyRight);
	solid1->AddForce(-ForceFootRight);
	footright->AddForce(ForceFootRight);
	

	ForceFootLeft = -(param1*HousenVelocityBodyLeft) * HousenFootLeft;
    ForceBodyLeft = -(param1*HousenVelocityBodyLeft) * (Leftvec.norm()/Bvec.norm()) * HousenBodyLeft;
	solid3->AddForce(ForceBodyLeft);
	solid1->AddForce(-ForceBodyLeft);
	solid1->AddForce(-ForceFootLeft);
	footleft->AddForce(ForceFootLeft);
	

	ForceFootRight = -(param1*HousenVelocityBodyRight) * HousenFootLeft;
    ForceBodyRight = -(param1*HousenVelocityBodyRight) * (Rightvec.norm()/Bvec.norm()) * HousenBodyRight;
	solid3->AddForce(ForceBodyRight);
	solid1->AddForce(-ForceBodyRight);
	solid1->AddForce(-ForceFootRight);
	footright->AddForce(ForceFootRight);
	*/

	Pd = FootLength*Bvec.unit() + soHip->GetCenterPosition();

	footleft->AddForce(kp*(Pd - footleft->GetCenterPosition()) - kv*footleft->GetVelocity());
	footright->AddForce(kp*(Pd - footright->GetCenterPosition()) - kv*footright->GetVelocity());

	/*
	if(LengthLeft > MaxFootLength){
        CrossVelocityLeft = VelocityHipLeft.dot(HipToLeftFoot);
		ForceLeft = -kp*((MaxFootLength/LengthLeft)*HipToLeftFoot - HipToLeftFoot) - kv*((-CrossVelocityLeft/LengthLeft)*HipToLeftFoot.unit());
		soHip->AddForce(ForceLeft);
		footleft->AddForce(-ForceLeft);
	}

	if(LengthRight > MaxFootLength) {
        CrossVelocityRight = VelocityHipRight.dot(HipToRightFoot);
		ForceRight = -kp*((MaxFootLength/LengthRight)*HipToRightFoot - HipToRightFoot) - kv*((-CrossVelocityRight/LengthRight)*HipToRightFoot.unit());
		soHip->AddForce(ForceRight);
		footright->AddForce(-ForceRight);
	}

	if(LengthLeft < MinFootLength) {
        CrossVelocityLeft = VelocityHipLeft.dot(HipToLeftFoot);
		ForceLeft = -kp*((MinFootLength/LengthLeft)*HipToLeftFoot - HipToLeftFoot) + kv*((CrossVelocityLeft/LengthLeft)*HipToLeftFoot.unit());
		soHip->AddForce(ForceLeft);
		footleft->AddForce(-ForceLeft);
	}

	if(LengthRight < MinFootLength) {
        CrossVelocityRight = VelocityHipRight.dot(HipToRightFoot);
		ForceRight = -kp*((MinFootLength/LengthRight)*HipToRightFoot - HipToRightFoot) + kv*((CrossVelocityRight/LengthRight)*HipToRightFoot.unit());
		soHip->AddForce(ForceRight);
		footright->AddForce(-ForceRight);
	}

	    //脚と上体の角度がMaxRoConstraintを超えた場合に脚と腰にかける力
	if(RoLeft.norm() > MaxRoConstraint) {
		ForceFootLeft = (param0*RoLeft.norm()-MaxRoConstraint) * HousenFootLeft;
		ForceBodyLeft = (param0*RoLeft.norm()-MaxRoConstraint) * (Leftvec.norm()/Bvec.norm()) * HousenBodyLeft;

		soHead->AddForce(ForceBodyLeft);
	    soHip->AddForce(-ForceBodyLeft);
		soHip->AddForce(-ForceFootLeft);
		footleft->AddForce(ForceFootLeft);
		}

	//脚と上体の角度がMaxRoConstraintを超えた場合に脚と腰にかける力
	if(RoRight.norm() > MaxRoConstraint) {
		ForceFootRight = (param0*RoRight.norm()-MaxRoConstraint) * HousenFootRight;
		ForceBodyRight = (param0*RoRight.norm()-MaxRoConstraint) * (Rightvec.norm()/Bvec.norm()) * HousenBodyRight;
		
		soHead->AddForce(ForceBodyRight);
	    soHip->AddForce(-ForceBodyRight); 
		soHip->AddForce(-ForceFootRight);
		footright->AddForce(ForceFootRight);
		}
		*/
}

/*
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 1.0);
	render->PushLight(light0);
	render->PushLight(light1);
}

void display(){
	render->ClearBuffer();
	render->Drawscene(scene);
	render->EndScene();
	glutSwapBuffers();
}
*/

double CRWalkingController::square(double a){
	double square;
	square = a*a;

	return square;
}

//changeの計算
Vec3d CRWalkingController::CalcChange(void){
	cac->UpdateState(PositionOfUpperBody,CenterVelocity,soHead->GetCenterPosition(),CurrentDirection);
	return cac->CalcChangeAroundCenter();
}

//timeleftの計算
double CRWalkingController::CalcTimeLeft(void){
	double legLength = (soHip->GetCenterPosition() - currentlandingsite).norm();

	if(LF == true) tl->UpdateState(timeleft, et, legLength, change, CenterVelocity, currentlandingsite, nextlandingsite, footleft->GetCenterPosition(), LF, CurrentDirection);
	else tl->UpdateState(timeleft, et, legLength, change, CenterVelocity, currentlandingsite, nextlandingsite, footright->GetCenterPosition(), LF, CurrentDirection);
	if(DSterm == true) return tl->CalcNextStepTimeLeft(PositionOfUpperBody.x ,PositionOfUpperBody.z, SingleSupportEndX, SingleSupportEndZ, ls->GetDoubleSupportDistanceX());                 //次に着地するまでの時間
	else return tl->CalcNextStepTimeLeft();
}

//目標着地点の計算
Vec3d CRWalkingController::CalcNextLandingSite(void){
	if(DSterm == false){
		if(LF == true) ls->UpdateState(change, PositionOfUpperBody, soHip->GetCenterPosition() , CenterVelocity, timeleft, currentlandingsite, footleft->GetCenterPosition(), footleft->GetVelocity(), TargetDirection, CurrentDirection);
		else ls->UpdateState(change, PositionOfUpperBody, soHip->GetCenterPosition() , CenterVelocity, timeleft, currentlandingsite, footright->GetCenterPosition(), footright->GetVelocity(), TargetDirection, CurrentDirection);
		return ls->CalcNextLandingSite();
	} else return nextlandingsite;
}

//TimeHalfCycleの計算
double CRWalkingController::CalcTimeHalfCycle(void){

    double firsthalfDStime;   //両脚支持期間を２分割した際の前半にかかる時間
	double secondhalfDStime;  //両脚支持期間を２分割した際の後半にかかる時間
	double LocalBodyX;
	double LocalBodyZ;
	double LocalBodyVelocityX;
	double LocalBodyVelocityZ;
	double MidPointX;
	double MidPointZ;
	double CurrentStrideX;
	double CurrentStrideZ;
	double LocalPreX;
	double LocalPreZ;
	double LocalSX;
	double LocalSZ;
	double thc;

	if(DSterm == true) secondhalfDStime = tl->CalcSecondHalfDoubleSupportTime(PositionOfUpperBody.x ,SingleSupportEndX, CenterVelocity.x, ls->GetDoubleSupportDistanceX(), PositionOfUpperBody.z, SingleSupportEndZ, CenterVelocity.z);
	else secondhalfDStime = 0.0;
	if(DSterm == false) firsthalfDStime = tl->CalcFirstHalfDoubleSupportTimeSingle(PositionOfUpperBody.x ,CenterVelocity.x, ls->GetDoubleSupportDistanceX(), PositionOfUpperBody.z, CenterVelocity.z);
	else firsthalfDStime = tl->CalcFirstHalfDoubleSupportTimeDouble(PositionOfUpperBody.x ,SingleSupportEndX, CenterVelocity.x, ls->GetDoubleSupportDistanceX(), PositionOfUpperBody.z, SingleSupportEndZ, CenterVelocity.z);

	timeDSleft = firsthalfDStime + secondhalfDStime;
    thc = timeleft + firsthalfDStime + secondhalfDStime;

	LocalBodyX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, PositionOfUpperBody.x, PositionOfUpperBody.z, CurrentDirection);
	LocalBodyZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, PositionOfUpperBody.x, PositionOfUpperBody.z, CurrentDirection);
	LocalBodyVelocityX = CalcLocalVX(CenterVelocity.x, CenterVelocity.z, CurrentDirection);
	LocalBodyVelocityZ = CalcLocalVZ(CenterVelocity.x, CenterVelocity.z, CurrentDirection);
	MidPointX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection) / 2.0;
	MidPointZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection) / 2.0;
	CurrentStrideX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection);
	CurrentStrideZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection);
    LocalPreX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, previousPX, previousPZ, CurrentDirection);
	LocalPreZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, previousPX, previousPZ, CurrentDirection);
	LocalSX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, SingleSupportEndX, SingleSupportEndZ, CurrentDirection);
	LocalSZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, SingleSupportEndX, SingleSupportEndZ, CurrentDirection);

	if(LocalBodyVelocityX > 0.0) {                             //サイクルの切り替わり時にtimehalfcycleを０にする必要がある
		if(abs(change.z) < LimitChange && LocalPreX < LocalSX + DSDX/2 && LocalBodyX > LocalSX + DSDX/2 || LocalBodyVelocityX > 0.0 && abs(change.z) > LimitChange && LocalPreX < MidPointX + DoubleSupportLimitRate*abs(CurrentStrideX) && LocalBodyX > MidPointX + DoubleSupportLimitRate*abs(CurrentStrideX))
			thc = 0.0;
	} else {
		if(abs(change.z) < LimitChange && LocalPreX > LocalSX + DSDX/2 && LocalBodyX < LocalSX + DSDX/2 || LocalBodyVelocityX < 0.0 && abs(change.z) > LimitChange && LocalPreX > MidPointX - DoubleSupportLimitRate*abs(CurrentStrideX) && LocalBodyX < MidPointX - DoubleSupportLimitRate*abs(CurrentStrideX))
			thc = 0.0;
	}

	if(abs(change.x) > LimitChange && LocalPreZ > MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ) && LocalBodyZ < MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ)) thc = 0.0;
    if(abs(change.x) > LimitChange && LocalPreZ < MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ) && LocalBodyZ > MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ)) thc = 0.0;

	//DSTR << "firsthalfDStime " << firsthalfDStime  << " secondhalfDStime " << secondhalfDStime << " timeleft " << timeleft << " timehalfcycle " << thc << std::endl;
	//DSTR << "CenterVelocity.x " << CenterVelocity.x << std::endl;

	return thc;
}

//床反力の計算
void CRWalkingController::GenerateCenterForce(void){

	double positionrate;
	double LocalBodyX;
	double LocalSX;

	LocalBodyX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, PositionOfUpperBody.x, PositionOfUpperBody.z, CurrentDirection);
	LocalSX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, SingleSupportEndX, SingleSupportEndZ, CurrentDirection);

	if(DSDX > 0) positionrate = (LocalBodyX - LocalSX) / DSDX;
	else positionrate = - (LocalBodyX - (LocalSX + DSDX)) / DSDX; 
	if(positionrate > 1.0) positionrate = 1.0;
	if(positionrate < 0.0) positionrate = 0.0;

	if(DSterm == false)gf->UpdateState(PositionOfUpperBody , CenterVelocity , soHip->GetCenterPosition(), timehalfcycle , change , currentlandingsite ,nextlandingsite,0.0, DSterm, CurrentDirection);
	else gf->UpdateState(PositionOfUpperBody , CenterVelocity , soHip->GetCenterPosition(), timehalfcycle , change , currentlandingsite ,nextlandingsite,positionrate, DSterm, CurrentDirection);
    
	if(EarthConnection == true) gf->GeneCenterForce();              //床反力の計算
}

//脚の動きを生成
void CRWalkingController::GenerateFootForce(void){
	if(katoki < gaitbegin) katoki = katoki + 1;   //何故か脚の動きを遅らせないと重心の運動がおかしくなる
	else {
		ff->UpdateState(currentlandingsite, nextlandingsite, soHip->GetCenterPosition(), soHip->GetVelocity(), timeleft, LandAble, EarthConnection, TargetDirection);
		if(DSterm == true) ff->FootDoubleSupport();
		else ff->FootMove(); //支持脚および遊脚の制御
	}
}

//歩行を行う
void CRWalkingController::gait(void){
    //std::vector<PHSolidIf*> WholeBody;
    //std::vector<PHSolidIf*> UpperBody;
	double LocalBodyX;
	double LocalBodyZ;
	double LocalBodyVelocityX;
	double LocalBodyVelocityZ;
	double MidPointX; 
	double MidPointZ;
	double CurrentStrideX;
	double CurrentStrideZ;
	double LocalSX;
	double LocalSZ;
	double leglength = (soHip->GetCenterPosition() - currentlandingsite).norm();  //solid1 -> Hip
	Vec3d AMAC;     //重心周りの角運動量
	Vec3d RoDiff;   //上体姿勢のズレ 
    
	//WholeBody.push_back(solid1);
    //WholeBody.push_back(solid2);
    //WholeBody.push_back(solid3);

	//UpperBody.push_back(solid2);
	//UpperBody.push_back(solid3);

	//PositionOfWholeBody = GetCenterOfBlocks(WholeBody);       //重心位置
	PositionOfUpperBody = GetCenterOfBlocks(UpperBody);         //上体位置
	//PHSolid* so = XCAST(solid2);
	CenterVelocity = CalcCenterVelocity(UpperBody);  //未定義
	CalcTargetAngle();                               //再度考慮 　　 目標進行方向の計算
	CalcCurrentDirection();                          //再度考慮　　　現在の進行方向の計算

	

	change = CalcChange();                   //changeの計算
	timeleft = CalcTimeLeft();               //timeleftの計算
	if(timeleft < 0.0) timeleft = 0.0;       //timeleftがマイナスとなった場合修正
	nextlandingsite = CalcNextLandingSite(); //目標着地点の計算
    LandAble = ls->GetLandAble();            //着地できるかの判断
	EarthConnection = CalcSustainable();     //接地しているかの判断

	DSDX = ls->GetDoubleSupportDistanceX();  //両脚支持期間に重心の進む距離

	timehalfcycle = CalcTimeHalfCycle(); //timehalfcycleの計算
	GenerateCenterForce();               //支持脚から重心にかかる力
	GenerateFootForce();                 //脚の動きを制御する ダイナミクスとは直接関係ない

    ConstraintForce();  //構造的制約により上半身に掛かる力

	//DSTR << "CenterVelocity = " << CenterVelocity << std::endl;
	//DSTR << "PositionOfUpperBody = " << PositionOfUpperBody << std::endl;
	//DSTR << "timeleft = " << timeleft << std::endl;
	//DSTR << "change = " << change << std::endl;
    //DSTR << "timehalfcycle = " << timehalfcycle << std::endl;
	//DSTR << "currentlandingsite = " << currentlandingsite << " nextlandingsite = " << nextlandingsite << std::endl;
	//DSTR << "CurrentDirection = " << CurrentDirection << std::endl;
	//DSTR << "PositionOfWholeBody = " << PositionOfWholeBody << std::endl;
	//DSTR << soHead->GetCenterPosition() << std::endl;
	//DSTR << "geneforce = " << gf->GetCenterForce() << std::endl;
	//DSTR << "totalStep = " << totalStep << std::endl;
	//DSTR << "TargetDirection = " << TargetDirection << std::endl;
	

///////途中で撃力を加える//////////
	if(totalStep > 700){
		if(forcestep1 < 5) {
			//solid1->AddForce(Vec3d(10.0 ,0.0 ,0.0));
			//solid2->AddForce(Vec3d(400.0 ,0.0 ,0.0));
			//soHead->AddForce(Vec3d(0.0 ,70.0 ,0.0));
			//soHip->AddForce(Vec3d(10.0, 0.0, -50.0));
			//soCenterObject->AddTorque(Vec3d(0.0,1500.0,0.0));
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
			DSTR << " " << std::endl;
		}
		forcestep1 = forcestep1 + 1;
	}

	if(flag3 == 1) {
        soHead->AddForce(Vec3d(1000.0 ,0.0 ,0.0));
        flag3 = 0;
	}
///////////////////////////////////

	//if(totalStep < 50 && totalStep > 35) solid2->AddForce(Vec3d(20,0.0,0.0) , PositionOfWholeBody);  //開始時の動きを良くするための力 いずれ直していく必要がある
	double vari2;

	Quaterniond Quat0;
	Quaterniond Quat1;

	Quat0 = (footright->GetPose().Ori() + footleft->GetPose().Ori())/2.0;
	Quat1 = soCenterObject->GetPose().Ori();
	vari2 = (soCenterObject->GetAngularVelocity()).y;

	Quaterniond dQuat = Quat0 * Quat1.Inv();
    //Vec3f torque = springOri * dQuat.Rotation() + damperOri * dAngV;

	soCenterObject->AddTorque(Vec3d(0.0, 5*(dQuat.Rotation().y) + (-4.0)*vari2 ,0.0)); //激力が加わった時にy方向に回転しないためのトルク

	LocalBodyVelocityX = CalcLocalVX(CenterVelocity.x, CenterVelocity.z, CurrentDirection);
	LocalBodyVelocityZ = CalcLocalVZ(CenterVelocity.x, CenterVelocity.z, CurrentDirection);

	//条件を満たすとき単脚支持期間を終了する
	if(timeleft == 0.0 && DSterm == false){
		
		//DSTR << "CenterVelocity = " << CenterVelocity << std::endl;
		DSTR << "step = " << CalcLocalX(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, currentlandingsite.z, CurrentDirection) << std::endl;
        DSTR << "LocalVX begin = " << LocalBodyVelocityX << std::endl;

		SingleSupportEndX = PositionOfUpperBody.x;
		SingleSupportEndZ = PositionOfUpperBody.z;
		et = 0.0;
		timeDSleft = T0*0.2;
		DSterm = true;
	}

	if(DSterm == true) {
	    //DSTR << "LocalVX = " << LocalBodyVelocityX << std::endl;
		//DSTR << "CenterVelocity.x = " << CenterVelocity.x << std::endl;
	}

	LocalBodyX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, PositionOfUpperBody.x, PositionOfUpperBody.z, CurrentDirection);
	LocalBodyZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, PositionOfUpperBody.x, PositionOfUpperBody.z, CurrentDirection);
	MidPointX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection) / 2.0;
	MidPointZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection) / 2.0;
	CurrentStrideX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection);
	CurrentStrideZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, nextlandingsite.z, CurrentDirection);
	LocalSX = CalcLocalX(currentlandingsite.x, currentlandingsite.z, SingleSupportEndX, SingleSupportEndZ, CurrentDirection);
	LocalSZ = CalcLocalZ(currentlandingsite.x, currentlandingsite.z, SingleSupportEndX, SingleSupportEndZ, CurrentDirection);

	//条件を満たすときtimeDSleftが0となり両脚支持期間を終了する
	if(LocalBodyVelocityX > 0.0 && DSDX > 0.0 && LocalBodyX > LocalSX + DSDX && DSterm == true) timeDSleft = 0.0;
	else if(LocalBodyVelocityX > 0.0 && DSDX < 0.0 && LocalBodyX > LocalSX && DSterm == true) timeDSleft = 0.0;
	else if(abs(change.z) > LimitChange && LocalBodyVelocityX > 0.0 && LocalBodyX > MidPointX + DoubleSupportLimitRate*abs(CurrentStrideX) && DSterm == true) timeDSleft = 0.0;
	else if(abs(change.x) > LimitChange && LocalBodyVelocityZ < 0.0 && LocalBodyZ < MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ) && DSterm == true) timeDSleft = 0.0;
	else if(abs(change.x) > LimitChange && LocalBodyVelocityZ > 0.0 && LocalBodyZ > MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ) && DSterm == true) timeDSleft = 0.0;
	//else if(abs(change.z) > LimitChange && CenterVelocity.x < 0.0 && PositionOfWholeBody.x < MidPointX - DoubleSupportLimitRate*abs(CurrentStrideX) && DSterm == true) timeDSleft = 0.0;
	else if(CenterVelocity.x < 0.0) timeDSleft = 0.0;
	else true;
		
	//条件を満たすとき両脚支持期間を終了する
	if(timeDSleft == 0.0 && DSterm == true){
		DSterm = false;

		DSTR << "CenterVelocity end = " << CenterVelocity << std::endl;
		ls->ChangeSupportLegs(LF);
		ff->ChangeSupportLegs(LF);

		if(LF == true) {
			LF = false;
			RF = true;
		} else {
			LF = true;
			RF = false;
		}
		et = 0.0;
		timeleft = T0;
		currentlandingsite = nextlandingsite;

		if(nextSpeed > 0.0){
			WCSetSpeed(nextSpeed);
			nextSpeed = -1;
		}
		if(stop == true){
			WCStop();
			stop = false;
		}
		if(reverse == true){
			WCReverse();
			reverse = false;
		}

		if(LF == true) DSTR << "RIGHT " << std::endl;
		else DSTR << "LEFT " << std::endl;

		DSTR << "currentlandingsite = " << currentlandingsite << std::endl;
		DSTR << "StrideLocalX = " << CalcLocalX(PositionOfUpperBody.x, PositionOfUpperBody.z, currentlandingsite.x, currentlandingsite.z, CurrentDirection) << std::endl;
		DSTR << "LocalVX end = " << LocalBodyVelocityX << std::endl;
		DSTR << "norm = " << (PositionOfUpperBody - currentlandingsite).norm() << std::endl;
	}
	else et = et + TimeStep;
	previousPX = PositionOfUpperBody.x;
	previousPZ = PositionOfUpperBody.z;
	PreviousV = CenterVelocity;

	completeFall();
	totalStep = totalStep + 1;
	//DSTR << "force = " << soCenterObject->GetForce() << std::endl;
}


/*
void standToWalk()
{}
*/

void CRWalkingController::func(){
	totalStep = totalStep + 1;

	//if(DSterm == true) DSTR << "force solid1 = " << solid1->GetForce() << " force solid2 = " << solid2->GetForce() << "force solid3 = " << solid3->GetForce() << std::endl;
	//DSTR << "Position solid1 = " << solid1->GetCenterPosition().y << std::endl;
	if(completefall == false)gait();
	else fallForce();
}

void CRWalkingController::Init(){

	CRController::Init();

	body = NULL;
	for (int i=0; i<creature->NBodies(); ++i) {
		if (!body) {
			body = DCAST(CRTrunkFootHumanBodyIf, creature->GetBody(i));
		}
	}

	CreateUpperBody();
	AssignFoot();  
	AssignCenterObject(); 
	AssignHip();        
	AssignHead();    

	///初期パラメータの設定/////
    paramVelocityX = 0.8; 
    paramHalfStrideX = 0.2;
	paramHalfStrideZ = 0.1;         
	footHeight = 0.05;                                  
    amplitude = 0.0;   

	footsize = 0.0619;
	////////////////////////////

	SetTimeParams();
	paramVelocityZ = CalcZParamV(paramHalfStrideZ,T0); //Z方向のパラメータの計算
    AssignInitialLandingSite();

	///大域変数の初期化/////
	et = 0.0;     
	TargetDirection = pi/2; 
	CurrentDirection = pi/2;
	LF = true;     
	RF = false;    
	DSterm = false; 
	completefall = false; 
	totalStep = 0;   
	katoki = 0;      
	gaitbegin = 30;  
	forcestep1 = 0;  
	flag = 0;       
	forcestep2 = 0;  
	flag2 = 0;      
	flag3 = 0;   

	nextSpeed = -1;
	reverse = false;
	stop = false;

	Xs = 0.0;//////////////////////////////////////////////////////////////////////
	Zs = 0.0;//////////////////////////////////////////////////////////////////////

	CreateCRWCTimeLeft();
	CreateCRWCChangeAroundCenter();
	CreateCRWCLandingSite();
	CreateCRWCFootForce();
	CreateCRWCGeneForce();

	DSTR << "BasicCycle = " << T0 << std::endl;

	/// 依存する他のオブジェクトの取得方法（もし必要なら）．

	/// < Body >
	/*
	この方法は今となっては使えない．bodyが複数ある状況に対応したため．
	body = DCAST(CRHingeHumanBodyIf, creature->GetBody());
	*/

	/// < PHScene >
	/*
	phScene = DCAST(PHSceneIf, creature->GetScene());
	*/

	/// < InternalScene >
	/*
	internalScene = creature->GetInternalScene();
	*/

	/// < ほかのController > 
	/*
	for (int i=0; i<creature->NControllers(); i++) {
		if (!gazeCtrl) {
			gazeCtrl = DCAST(CRGazeControllerIf, creature->GetController(i))
		}
	}
	*/

	// < ほかのSensor >
	/*
	for (int i=0; i<creature->NSensors(); i++) {
		if (!opticalSensor) {
			opticalSensor = DCAST(CROpticalSensorIf, creature->GetSensor(i))
		}
	}
	*/
}

	
void CRWalkingController::Step(){
	totalStep = totalStep + 1;
	CRController::Step();

	if(totalStep > 200 && completefall == false)gait();
	else if(completefall == false) stand();
	else fallForce();

	if(totalStep == 400) SetSpeed(0.3);
	if(totalStep == 600) SetSpeed(0.2);
	//if(totalStep == 800) Reverse();
	/*if(totalStep == 800) SetSpeed(0.3);
	if(totalStep == 1000) SetSpeed(0.1);
	if(totalStep == 1200) Stop();*/
}

void CRWalkingController::CreateUpperBody(void){
	UpperBody.push_back(body->GetSolid(CRTrunkFootHumanBodyDesc::SO_WAIST));
	UpperBody.push_back(body->GetSolid(CRTrunkFootHumanBodyDesc::SO_CHEST));
	UpperBody.push_back(body->GetSolid(CRTrunkFootHumanBodyDesc::SO_HEAD));
}

void CRWalkingController::AssignFoot(void){
	footleft = body->GetSolid(CRTrunkFootHumanBodyDesc::SO_LEFT_FOOT);
	footright = body->GetSolid(CRTrunkFootHumanBodyDesc::SO_RIGHT_FOOT);
}

void CRWalkingController::AssignCenterObject(void){
	soCenterObject = body->GetSolid(CRTrunkFootHumanBodyDesc::SO_CHEST);
	//soCenterObject->SetMass(0.44);
}

void CRWalkingController::AssignHip(void){
	soHip = body->GetSolid(CRTrunkFootHumanBodyDesc::SO_WAIST);
	//soHip->SetMass(0.17);
}

void CRWalkingController::AssignHead(void){
	soHead = body->GetSolid(CRTrunkFootHumanBodyDesc::SO_HEAD);
}

void CRWalkingController::SetTimeParams(){
	T0 = CalcBasicCycle();          //歩行の基本周期の計算 
	timeleft = T0; 
	TimeStep = phScene->GetTimeStep();  
}

void CRWalkingController::AssignInitialLandingSite(){
	////////////////////////あとで変更
	currentlandingsite = Vec3d(footright->GetFramePosition().x, 0.0, footright->GetFramePosition().z); 	//最初は右足が支持脚 
    ////////////////////////
}

void CRWalkingController::CreateCRWCTimeLeft(){
	tl = new CRWCTimeLeft(TimeStep);   
	tl->Init();
	tl->SetParamT0(T0);
	tl->SetParamCycleRate(minCycleRate, maxDSRate);
	tl->SetParamMaxFootLength(MaxFootLength);
	tl->SetParamDoubleSupportLimitRate(DoubleSupportLimitRate);
	tl->SetParamLimitChange(LimitChange);
	tl->SetParamVelocityX(paramVelocityX);
	tl->SetParamVelocityZ(paramVelocityZ);
	tl->SetParamFootSize(footsize);
	tl->SetParamMaxFootSpeedY(2.5);
}

void CRWalkingController::CreateCRWCChangeAroundCenter(){
	cac = new CRWCChangeAroundCenter(UpperBody);
	cac->Init();
}

void CRWalkingController::CreateCRWCLandingSite(){
	ls = new CRWCLandingSite(currentlandingsite , LF);
	ls->Init();
	ls->SetParamLd(paramLdx, paramLdz);
    ls->SetParamT0(T0);
	ls->SetParamMaxStride(maxHalfStride);
	ls->SetParamTc(Tc);
	ls->SetParamTimeStep(TimeStep);
	ls->SetParamVelocityX(paramVelocityX);
	ls->SetParamVelocityZ(paramVelocityZ);
	ls->SetParamStrideX(paramHalfStrideX);
	ls->SetParamStrideZ(paramHalfStrideZ);
	ls->SetParamFootLength(FootLength);
	ls->SetParamMaxFootSpeeds(MaxFootSpeedFront,MaxFootSpeedSide,MaxFootSpeedBack);
	ls->SetParamMaxFootAccerelations(MaxFootAccelerationFront,MaxFootAccelerationSide,MaxFootAccelerationBack);
	ls->SetParamMaxFootLength(MaxFootLength);
	ls->SetParamMaxRoGround(MaxRoGround);
	ls->SetParamMaxRoLandingSite(MaxRoLandingSiteFront,MaxRoLandingSiteSide,MaxRoLandingSiteBack);
}

void CRWalkingController::CreateCRWCFootForce(){
	ff = new CRWCFootForce(TimeStep, phScene->GetGravity(),LF);    //scene->GetGravity()
    ff->Init();
    ff->SetFoots(footleft, footright);
	ff->SetParamT0(T0);
	ff->SetParamFootHeight(footHeight);
	ff->SetParamMaxFootLength(MaxFootLength);
	ff->SetParamMinFootLength(MinFootLength);
	ff->SetParamFootSize(footsize);
}

void CRWalkingController::CreateCRWCGeneForce(){
	gf = new CRWCGeneForce((-1) * phScene->GetGravity() * TotalMass(UpperBody) , TimeStep);  //scene->GetGravity()
    gf->Init();
	gf->SetCenterObject(soCenterObject);    //centerObjectの指定
	gf->SetParamAmplitude(amplitude);
	gf->SetParamheight(height);
	gf->SetParamT0(T0);
	gf->SetParamT0ds(paramLdx/paramVelocityX);
	gf->SetParamVX(paramVelocityX);
	gf->SetParamVZ(paramVelocityZ);
	gf->SetParamMaxFootLength(MaxFootLength);
}

void CRWalkingController::stand(){
	soCenterObject->AddForce(-1*phScene->GetGravity()*TotalMass(UpperBody));
	soCenterObject->AddForce(10*(Vec3d(Xs,height,Zs) - GetCenterOfBlocks(UpperBody)) - 5*CalcCenterVelocity(UpperBody));
}

void CRWalkingController::SetSpeed(double v){
	if(v < 0) {
		DSTR << "SetSpeed;;Input Positive Value" << std::endl;
		exit(1);
	} else if(v > 1.2){
		DSTR << "SetSpeed;;Too Fast Speed Parametor" << std::endl;
		exit(1);
	}

	nextSpeed = v;
}

void CRWalkingController::WCSetSpeed(double v){

	if(v > 0.8){
		HalfStrideXChange(0.18*v);
		VelocityXChange(v);
    	AmplitudeChange(0.01);
		FootHeightChange(0.06*v);
		if(v < 0.7) VelocityZChange(0.3*v);
		else VelocityZChange(0.21);
    } else if(v > 0.4){
		HalfStrideXChange(0.18*v);
		VelocityXChange(v);
    	AmplitudeChange(v*0.02);
		FootHeightChange(0.06*v);
		if(v < 0.7) VelocityZChange(0.3*v);
		else VelocityZChange(0.21);
	} else {
		HalfStrideXChange(0.15*v);
		VelocityXChange(v);
    	AmplitudeChange(v*0.01);
		FootHeightChange(0.06*v);
		VelocityZChange(0.4*v);
	}
}

void CRWalkingController::Stop(){
	stop = true;
}

void CRWalkingController::WCStop(){
    HalfStrideXChange(0.0001);
	VelocityXChange(0.0004);
	AmplitudeChange(0.0);
	ff->SetParamFootHeight(0.00005);
	VelocityZChange(0.07);
}

void CRWalkingController::Reverse(void){
	reverse = true;
}

void CRWalkingController::WCReverse(void){
	ls->SetParamStrideX(-1.0);
	ls->SetParamVelocityX(-0.35);
	AmplitudeChange(0.005);
}

void CRWalkingController::SetRotationAngle(double r){
	TargetDirection = (soCenterObject->GetOrientation()).Rotation().y + r + pi/2;
	if(TargetDirection > 2.0*pi) TargetDirection = TargetDirection - 2.0*pi;
    if(TargetDirection < -2.0*pi) TargetDirection = TargetDirection + 2.0*pi;
}

void CRWalkingController::SetRotationWorldCoordinate(double r){
	if(r > 2*pi || r < -2*pi){
		DSTR << "SetRotationWorldCoordinate;;Inadequate Input" << std::endl;
		exit(1);
	}
	TargetDirection = r + pi/2;
}

}
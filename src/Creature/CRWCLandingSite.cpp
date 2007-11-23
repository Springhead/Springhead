#include ".\crwcLandingsite.h"

CRWCLandingSite::CRWCLandingSite(Vec3d InitialLanding , bool lf)
{
	CurrentLandingSite = InitialLanding;
	
	LF = lf;
	if(LF) RF = false;
	else RF = true;
}

CRWCLandingSite::~CRWCLandingSite(void)
{
}

void CRWCLandingSite::Init(void){
	kx = 6.5;
	kz = 6.5;

	ax = 1.0;
	bx = 10.0;
	cx = 1000.0;
	az = 1.0;
	bz = 2.0;
	cz = 1000.0;

	pi = 3.1415926535;
	LandAble = true;
}

void CRWCLandingSite::UpdateState(Vec3d changea , Vec3d PositionOfWholeBodya , Vec3d ph, Vec3d VelocityOfWholeBodya , double timelefta, Vec3d cls, Vec3d psf, Vec3d vsf, double theta, double cd){
	change = changea;
	PositionOfWholeBody = PositionOfWholeBodya;
	PositionOfHip = ph;
    VelocityOfWholeBody = VelocityOfWholeBodya;
	timeleft = timelefta;
	CurrentLandingSite = cls;
	PositionOfSwingFoot = psf;
	VelocityOfSwingFoot = vsf;
	TargetAngle = theta;
	CurrentDirection = cd;
}

void CRWCLandingSite::ChangeSupportLegs(bool lf){

	if(lf) {
		LF = false;
		RF = true;
	}
	else {
		LF = true;
		RF = false;
	}
}


//身体、構造的制約条件なしの次の着地点
void CRWCLandingSite::CalcNextLandingSiteWithoutConstraint(){

	double BLX;
	double LX;
	double BLZ;
	double LZ;

	LocalX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfWholeBody.x, PositionOfWholeBody.z, TargetAngle);
    LocalZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfWholeBody.x, PositionOfWholeBody.z, TargetAngle);
	LocalBodyVelocityX = CalcLocalVX(VelocityOfWholeBody.x, VelocityOfWholeBody.z, TargetAngle);
	LocalBodyVelocityZ = CalcLocalVZ(VelocityOfWholeBody.x, VelocityOfWholeBody.z, TargetAngle);


	//DSTR << "PositionOfWholeBody.x - CurrentLandingSite.x = " << PositionOfWholeBody.x - CurrentLandingSite.x << " LocalX = " << LocalX << std::endl;
	npx = CalcPosition(0.0, LocalX, LocalBodyVelocityX);
	nvx = CalcVelocity(0.0, LocalX, LocalBodyVelocityX);

	npz = CalcPosition(0.0, LocalZ, LocalBodyVelocityZ);
	nvz = CalcVelocity(0.0, LocalZ, LocalBodyVelocityZ);

	npx = npx + 2.0*nvx*TimeStep;     //ivrc用の特殊な補正
	npz = npz + 2.0*nvz*TimeStep;     //ivrc用の特殊な補正

	//DSTR << "nvx = " << nvx << " nvz = " << nvz << std::endl;

	BLX = CalcBasicLandingSiteX(npx, nvx);
	LX = CalcLandingSiteX(npx, nvx,change.z, BLX);

	//DSTR << "BLX = " << BLX << " LX = " << LX << std::endl;
	//DSTR << "VelocityX = " << VelocityX << " StrideX = " << StrideX << " BLX = " << BLX << " LX = " << LX << " LocalX = " << LocalX << " PositionOfWholeBody.x = " << PositionOfWholeBody.x << " VelocityOfWholeBody.x = " << VelocityOfWholeBody.x << " TargetAngle = " << TargetAngle << std::endl;
	//DSTR << "VelocityX = " << VelocityX << " StrideX = " << StrideX << " BLX = " << BLX << " LX = " << LX << " nvx = " << nvx << " npx = " << npx << " nextV = "  << CalcNextVelocity(LX,npx,nvx)  << std::endl;
	//DSTR << "npx = " << npx << " nvx = " << nvx << std::endl;
	//DSTR << CalcNextVelocity(LX, npx, nvx) << std::endl;
	//DSTR << "nvx = " << CalcVelocity(0.0, -0.0989374,  0.589449) << std::endl;
	//DSTR << "LocalX " << LocalX  << " LocalBodyVelocityX " << LocalBodyVelocityX << std::endl;
	//DSTR << "npx - LX = " << npx - LX << " PositionOfWholeBody.x - CurrentLandingSite.x - LX = " << PositionOfWholeBody.x - CurrentLandingSite.x - LX << " LocalX = " << LocalX << std::endl;

	if(LF == true) {
		BLZ = CalcBasicLandingsiteZL(npz,nvz);
		LZ = CalcLandingsiteZL(npz, nvz, change.x, BLZ);
	}
	else {
		BLZ = CalcBasicLandingsiteZR(npz,nvz);
		LZ = CalcLandingsiteZR(npz, nvz, change.x, BLZ);
	}

	LX = LX + CalcDoubleSupportDistanceX(LX);
	LZ = LZ + CalcDoubleSupportDistanceZ(LZ);

	//DSTR << "1 LX = " << LX << " LZ = " << LZ << std::endl;
	//DSTR << "VelocityZ = " << VelocityZ << std::endl;
	//DSTR << npz << nvz << std::endl;
	//DSTR << "BLZ = " << BLZ << " LZ = " << LZ << std::endl;
	NextLandingSiteWithoutConstraint = Vec3d(LX, 0.0 , LZ);
}

//身体的、構造的制約を負荷した着地点の決定
Vec3d CRWCLandingSite::CalcNextLandingSite(){

	double LX;
	double LZ;
	double LocalHipX;
	double LocalHipZ;
	double LocalFootX;
	double LocalFootZ;
	double LocalFootVX;
	double LocalFootVZ;
	double CDLocalHipX;
	double CDLocalHipZ;
	double CDLX;
	double CDLZ;
	double CDLocalBodyVelocityX;
	double CDLocalBodyVelocityZ;
	double PossibleLandingSite;
	double t0;
	double t1;
	double vari0;
	double vari1;
	double vari2;
	double paramBackStep = 0.6;
	double MaxRoLandingSite;
	Quaterniond qua;
	Quaterniond qua2;
	Vec3d Bvec;        //上体の姿勢の単位ベクトル
	Vec3d Lvec;        //腰と着地点を結んだ単位ベクトル
	Vec3d dvec = Vec3d(0.0, -1.0 , 0.0);
	Vec3d RoGround;     //地面と上体の間の角度
	Vec3d RoLandingSite;//着地点と上体の間の角度
	Vec3d vec0;
	Vec3d CLvec;
	//double MaxRoGround = 3.14 / 2.0;      //60度
	//double MaxRoLandingSite = 3.14 / 2.5; //60度


    //DSTR << "prenextlandingsite = " << NextLandingSiteWithoutConstraint << std::endl;

	CalcNextLandingSiteWithoutConstraint();

    LX = NextLandingSiteWithoutConstraint.x;
	LZ = NextLandingSiteWithoutConstraint.z; 

	//DSTR << "1 LX = " << LX <<" LZ = " << LZ << std::endl;

	LocalHipX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfHip.x, PositionOfHip.z, TargetAngle);
	LocalHipZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfHip.x, PositionOfHip.z, TargetAngle);
	CDLocalHipX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfHip.x, PositionOfHip.z, CurrentDirection);
	CDLocalHipZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfHip.x, PositionOfHip.z, CurrentDirection);
	CDLX = LX*cos(TargetAngle-CurrentDirection) + LZ*sin(TargetAngle-CurrentDirection);
	CDLZ = LZ*cos(TargetAngle-CurrentDirection) - LX*sin(TargetAngle-CurrentDirection);
	CDLocalBodyVelocityX = CalcLocalVX(VelocityOfWholeBody.x, VelocityOfWholeBody.z, CurrentDirection);
	CDLocalBodyVelocityZ = CalcLocalVZ(VelocityOfWholeBody.x, VelocityOfWholeBody.z, CurrentDirection);


	Bvec = (PositionOfHip - PositionOfWholeBody).unit();
	Lvec = (Vec3d(CDLX - (CDLocalHipX + CDLocalBodyVelocityX*timeleft) , - PositionOfHip.y , CDLZ - (CDLocalHipZ + CDLocalBodyVelocityZ * timeleft))).unit();

    qua.RotationArc(Bvec, dvec);
	RoGround = qua.Rotation();
	qua.RotationArc(Bvec, Lvec);
	RoLandingSite = qua.Rotation();

	vari2 = (1 - abs(atan((CDLZ-CDLocalHipZ)/(CDLX-CDLocalHipX)))/(pi*2.0)) * (1 - abs(atan((CDLZ-CDLocalHipZ)/(CDLX-CDLocalHipX)))/(pi*2.0));

	//DSTR << "CDLX = "<< CDLX << " CDLZ = " << CDLZ << " abs(atan) = " << abs(atan(CDLZ/CDLX)) << std::endl;

	if(CDLX > 0.0) MaxRoLandingSite = (1-vari2) * MaxRoLandingSiteSide + vari2 * MaxRoLandingSiteFront;
	else MaxRoLandingSite = (1-vari2) * MaxRoLandingSiteSide + vari2 * MaxRoLandingSiteBack;

	//DSTR << "MaxRoLandingSite = " << MaxRoLandingSite << std::endl;

	if(RoLandingSite.norm() > MaxRoLandingSite) {
        vec0 = (0.85*MaxRoLandingSite/RoLandingSite.norm()) * RoLandingSite;
	} else vec0 = RoLandingSite;

	qua.RotationArc(Bvec, Bvec);
	qua = qua.Rot(vec0);
	CLvec = qua*Bvec;

	t0 = -PositionOfHip.y / CLvec.y;              
	if((0.9)*MaxFootLength > PositionOfHip.y) t1 = sqrt(((0.9)*(0.9)*MaxFootLength*MaxFootLength - PositionOfHip.y*PositionOfHip.y) / (CLvec.x*CLvec.x + CLvec.z*CLvec.z));
	else t1 = 0.0;

	t0 = min(t0,t1);
	
	LX = LocalHipX + LocalBodyVelocityX*timeleft + t0*CLvec.x;
	LZ = LocalHipZ + LocalBodyVelocityZ*timeleft + t0*CLvec.z;

	//DSTR << "2 LX = " << LX <<" LZ = " << LZ << std::endl;

	//DSTR << "LX = " << LX << " LZ = " << LZ << std::endl;

	if(RoGround.norm() > MaxRoGround || MaxFootLength < PositionOfHip.y) {  //腰の位置が脚の長さよりも高くなった場合
		LandAble = false;                                                   //もしくは上体の姿勢がMaxRoGroundを超えた場合は
     	NextLandingSite = Vec3d(0.0,0.0,0.0);                               //着地できないという条件
		return NextLandingSite;
	}

	//////////着地点に関する構造的制約の条件//////////////////////

	LocalFootX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfSwingFoot.x, PositionOfSwingFoot.z, TargetAngle);
	LocalFootZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfSwingFoot.x, PositionOfSwingFoot.z, TargetAngle);
    LocalFootVX = CalcLocalVX(VelocityOfSwingFoot.x, VelocityOfSwingFoot.z, TargetAngle);
	LocalFootVZ = CalcLocalVZ(VelocityOfSwingFoot.x, VelocityOfSwingFoot.z, TargetAngle);

	if(timeleft > 0.0){    ///遊脚の最高速度・加速度を考慮した条件
		if(LX < LocalFootX) { 
			MaxFootAcceleration = MaxAcBack * cos(TargetAngle-CurrentDirection) + MaxAcSide * sin(TargetAngle-CurrentDirection);
			MaxFootSpeed = MaxSpBack * cos(TargetAngle-CurrentDirection) + MaxSpSide * sin(TargetAngle-CurrentDirection);
			if(LocalFootVX - MaxFootAcceleration*timeleft < -MaxFootSpeed){
				t0 = (LocalFootVX + MaxFootSpeed) / MaxFootAcceleration;
				t1 = timeleft - t0;
			} else {
				t0 = timeleft;
				t1 = 0.0;
			}
			PossibleLandingSite = -MaxFootAcceleration/2*(t0*t0) + LocalFootVX*(t0) + LocalFootX;
			PossibleLandingSite = PossibleLandingSite - MaxFootSpeed * t1;

			if(LX < PossibleLandingSite) LX = PossibleLandingSite;
		} else {
			MaxFootAcceleration = MaxAcFront * cos(TargetAngle-CurrentDirection) + MaxAcSide * sin(TargetAngle-CurrentDirection);
			MaxFootSpeed = MaxSpFront * cos(TargetAngle-CurrentDirection) + MaxSpSide * sin(TargetAngle-CurrentDirection);
			if(LocalFootVX + MaxFootAcceleration*timeleft > MaxFootSpeed){
				t0 = (MaxFootSpeed - LocalFootVX) / MaxFootAcceleration;
				t1 = timeleft - t0;
			} else {
				t0 = timeleft;
				t1 = 0.0;
			}
			PossibleLandingSite = MaxFootAcceleration/2*(t0*t0) + LocalFootVX*(t0) + LocalFootX;
			PossibleLandingSite = PossibleLandingSite + MaxFootSpeed * t1;

			if(LX > PossibleLandingSite) LX = PossibleLandingSite;
		}
	} else LX = LocalFootX;

	if(timeleft > 0.0){    ///遊脚の最高速度・加速度を考慮した条件
		if(LZ < LocalFootZ) {     
			MaxFootAcceleration = MaxAcSide * cos(TargetAngle-CurrentDirection) + MaxAcBack * sin(TargetAngle-CurrentDirection);
			MaxFootSpeed = MaxSpSide * cos(TargetAngle-CurrentDirection) + MaxSpBack * sin(TargetAngle-CurrentDirection);
			if(LocalFootVZ - MaxFootAcceleration*timeleft < -MaxFootSpeed){
				t0 = (LocalFootVZ + MaxFootSpeed) / MaxFootAcceleration;
				t1 = timeleft - t0;
			} else {
				t0 = timeleft;
				t1 = 0.0;
			}
			PossibleLandingSite = -MaxFootAcceleration/2*(t0*t0) + LocalFootVZ*(t0) + LocalFootZ;
			PossibleLandingSite = PossibleLandingSite - MaxFootSpeed * t1;

			if(LZ < PossibleLandingSite) LZ = PossibleLandingSite;
		} else {
			MaxFootAcceleration = MaxAcSide * cos(TargetAngle-CurrentDirection) + MaxAcFront * sin(TargetAngle-CurrentDirection);
			MaxFootSpeed = MaxSpSide * cos(TargetAngle-CurrentDirection) + MaxSpFront * sin(TargetAngle-CurrentDirection);
			if(LocalFootVZ + MaxFootAcceleration*timeleft > MaxFootSpeed){
				t0 = (MaxFootSpeed - LocalFootVZ) / MaxFootAcceleration;
				t1 = timeleft - t0;
			} else {
				t0 = timeleft;
				t1 = 0.0;
			}
			PossibleLandingSite = MaxFootAcceleration/2*(t0*t0) + LocalFootVZ*(t0) + LocalFootZ;
			PossibleLandingSite = PossibleLandingSite + MaxFootSpeed * t1;

			if(LZ > PossibleLandingSite) LZ = PossibleLandingSite;
		}
	} else LZ = LocalFootZ;

	//DSTR << "3 LX = " << LX << " LZ = " << LZ << std::endl;
   

	/////脚の長さに対する制約///////
	if(LX > 0.0){
		if(LX > MaxStride)
			LX = MaxStride;
	} else {
		if(LX <  -MaxStride)
            LX = -MaxStride;
	}

	/////脚の長さに対する制約///////
	if(LZ > 0.0){
		if(LZ > MaxStride)
			LZ = MaxStride;
	} else {
		if(LZ < -MaxStride)
            LZ = -MaxStride;
	}

	if(LF == true){
		if(LX < 0.24 && LX > -0.24){
			if(LZ < 0.1 && LZ > -0.1) LZ = -0.1;
		}
	} else {
		if(LX < 0.24 && LX > -0.24){
			if(LZ < 0.1 && LZ > -0.1) LZ = 0.1;
		}
	}

	/*
	if(LF == true) {
		if(LZ > 0.0) LZ = 0.0;
	} else {
		if(LZ < 0.0) LZ = 0.0;
	}*/

	/*
	if(LF == true){
		limitZ = VelocityOfWholeBody.z * timeleft - MaxStride + PositionOfHip.z;
		if(CurrentLandingSite.z < LZ) LZ = CurrentLandingSite.z;
		else if(LZ < limitZ) LZ = limitZ;
	} else {
		limitZ = VelocityOfWholeBody.z * timeleft + MaxStride + PositionOfHip.z;
		if(LZ < CurrentLandingSite.z) LZ = CurrentLandingSite.z;
		else if(limitZ < LZ) LZ = limitZ;
	}*/

	//DSTR << "4 LX = " << LX <<" LZ = " << LZ << std::endl;

	vari0 = LX*cos(TargetAngle) + LZ*sin(TargetAngle) + CurrentLandingSite.x;
	vari1 = LZ*cos(TargetAngle) - LX*sin(TargetAngle) + CurrentLandingSite.z;
	LX = vari0;
	LZ = vari1;

	LandAble = true;
	NextLandingSite = Vec3d(LX, 0.0 , LZ);

    //DSTR << "nextlandingsite = " << NextLandingSite << std::endl;

	return NextLandingSite;
}


//線形倒立振子において, 切り替え時間, 着地点, 現在位置・速度が与えられたときの、切り替え時の位置
double CRWCLandingSite::CalcPosition(double landingsite, double cx, double cv){
	double px;
	double sinh;
	double cosh;

    sinh = exp(timeleft / Tc) / 2 - exp(- timeleft / Tc) / 2;
	cosh = exp(timeleft / Tc) / 2 + exp(- timeleft / Tc) / 2;

	px = (cx - landingsite) * cosh + Tc * cv * sinh + landingsite;

	return px;
}

//線形倒立振子において, 切り替え時間, 着地点, 現在位置・速度が与えられたときの、切り替え時の速度
double CRWCLandingSite::CalcVelocity(double landingsite, double cx, double cv){
	double vx;
	double sinh;
	double cosh;

    sinh = exp(timeleft / Tc) / 2 - exp(- timeleft / Tc) / 2;
	cosh = exp(timeleft / Tc) / 2 + exp(- timeleft / Tc) / 2;

	vx = (cx - landingsite)/Tc * sinh + cv * cosh;

	return vx;
}

//バランス状態に応じて着地点関数にかかる重み
double CRWCLandingSite::CalcWeightForBalanceX(double changeZ){
	double R;

    if(changeZ > 0.0) R = changeZ / kx;
	else R = 2.0*changeZ / kx;

	//DSTR << "R = " << R << "LocalBodyVelocityX = " << LocalBodyVelocityX << std::endl;

    if(R < -2.0)R = -2.0;
    if(R > 2.0) R = 2.0;

	return R;
}

double CRWCLandingSite::CalcWeightForBalanceZ(double changeX){
	double R;

	R = (-1) * changeX / kz;
	if(R < -1.0)R = -1.0;
	if(R > 1.0) R = 1.0;

	return R;
}

//バランスを考慮しない場合の着地点(X方向)
double CRWCLandingSite::CalcBasicLandingSiteX(double cpx, double cvx){
	double landingsite;
	double sinh;
	double cosh;
	double localvari;

    sinh = exp(T0 / Tc) / 2 - exp(- T0 / Tc) / 2;
	cosh = exp(T0 / Tc) / 2 + exp(- T0 / Tc) / 2;
 
	localvari = ax*cosh*(-StrideX + cosh*cpx + Tc*sinh*cvx) + bx*sinh/Tc*(-VelocityX + sinh*cpx/Tc + cosh*cvx);
    landingsite = localvari / (ax*cosh*cosh + bx*sinh*sinh/Tc/Tc);
    
    return landingsite;
}

//バランスを考慮した場合の着地点(X方向)
double CRWCLandingSite::CalcLandingSiteX(double cpx, double cvx, double changeZ, double normallanding){
    double landingsite;
	double sinh;
	double cosh;
	double R;          //バランスの重み関数の値
	double localvari;

    sinh = exp(T0 / Tc) / 2 - exp(- T0 / Tc) / 2;
	cosh = exp(T0 / Tc) / 2 + exp(- T0 / Tc) / 2;
	R = CalcWeightForBalanceX(changeZ);

	//DSTR << "changeZ = " << changeZ << " R = " << R << std::endl;
	//DSTR << "normallanding + R*StrideX = " << normallanding + R*StrideX << std::endl;

	localvari = ax*cosh*(-StrideX + cosh*cpx + Tc*sinh*cvx) + bx*sinh/Tc*(-VelocityX + sinh*cpx/Tc + cosh*cvx) + cx*(normallanding + R*StrideX);
    landingsite = localvari / (ax*cosh*cosh + bx*sinh*sinh/Tc/Tc + cx);

    return landingsite;
}

double CRWCLandingSite::CalcBasicLandingsiteZL(double cpz, double cvz){

	double landingsite;
	double sinh;
	double cosh;
	double localvari;

    sinh = exp(T0 / Tc) / 2 - exp(- T0 / Tc) / 2;
	cosh = exp(T0 / Tc) / 2 + exp(- T0 / Tc) / 2;
 
	localvari = az*cosh*(-StrideZ + cosh*cpz + Tc*sinh*cvz) + bz*sinh/Tc*(-VelocityZ + sinh*cpz/Tc + cosh*cvz);
    landingsite = localvari / (az*cosh*cosh + bz*sinh*sinh/Tc/Tc);
    
    return landingsite;
}

double CRWCLandingSite::CalcBasicLandingsiteZR(double cpz, double cvz){

	double landingsite;
	double sinh;
	double cosh;
	double localvari;

    sinh = exp(T0 / Tc) / 2 - exp(- T0 / Tc) / 2;
	cosh = exp(T0 / Tc) / 2 + exp(- T0 / Tc) / 2;
 
	localvari = az*cosh*(StrideZ + cosh*cpz + Tc*sinh*cvz) + bz*sinh/Tc*(VelocityZ + sinh*cpz/Tc + cosh*cvz);
    landingsite = localvari / (az*cosh*cosh + bz*sinh*sinh/Tc/Tc);
    
    return landingsite;
}

double CRWCLandingSite::CalcLandingsiteZL(double cpz, double cvz, double changeX, double normallanding){

	double landingsite;
	double sinh;
	double cosh;
	double R;
	double localvari;

    sinh = exp(T0 / Tc) / 2 - exp(- T0 / Tc) / 2;
	cosh = exp(T0 / Tc) / 2 + exp(- T0 / Tc) / 2;
	R = CalcWeightForBalanceZ(changeX);
 
	localvari = az*cosh*(-StrideZ + cosh*cpz + Tc*sinh*cvz) + bz*sinh/Tc*(-VelocityZ + sinh*cpz/Tc + cosh*cvz) + cz*(normallanding + R*StrideZ);
    landingsite = localvari / (az*cosh*cosh + bz*sinh*sinh/Tc/Tc + cz);

    return landingsite;
}

double CRWCLandingSite::CalcLandingsiteZR(double cpz, double cvz, double changeX, double normallanding){

	double landingsite;
	double sinh;
	double cosh;
	double R;
	double localvari;

    sinh = exp(T0 / Tc) / 2 - exp(- T0 / Tc) / 2;
	cosh = exp(T0 / Tc) / 2 + exp(- T0 / Tc) / 2;
	R = CalcWeightForBalanceZ(changeX);
 
	localvari = az*cosh*(StrideZ + cosh*cpz + Tc*sinh*cvz) + bz*sinh/Tc*(VelocityZ + sinh*cpz/Tc + cosh*cvz) + cz*(normallanding + R*StrideZ);
    landingsite = localvari / (az*cosh*cosh + bz*sinh*sinh/Tc/Tc + cz);
	
    return landingsite;
}

//両足支持期間にX方向に進む距離を計算
double CRWCLandingSite::CalcDoubleSupportDistanceX(double nextSingleSupportLandingSiteX){
		
    double npx;
	double SingleSupportDistanceX;
	double LocalX;
	double LocalBodyVelocityX;

	LocalX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfWholeBody.x, PositionOfWholeBody.z, TargetAngle);
	LocalBodyVelocityX = CalcLocalVX(VelocityOfWholeBody.x, VelocityOfWholeBody.z, TargetAngle);

	npx = CalcPosition(0.0, LocalX, LocalBodyVelocityX);
    SingleSupportDistanceX = nextSingleSupportLandingSiteX - npx;

	DoubleSupportDistanceX = (SingleSupportDistanceX / StrideX) * Ldx;

	return DoubleSupportDistanceX;
};

//両足支持期間にZ方向に進む距離を計算
double CRWCLandingSite::CalcDoubleSupportDistanceZ(double nextSingleSupportLandingSiteZ){

	double npz;
	double SingleSupportDistanceZ;
	double LocalZ;
	double LocalBodyVelocityZ;

	LocalZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfWholeBody.x, PositionOfWholeBody.z, TargetAngle);
	LocalBodyVelocityZ = CalcLocalVZ(VelocityOfWholeBody.x, VelocityOfWholeBody.z, TargetAngle);

	npz = CalcPosition(0.0, LocalZ, LocalBodyVelocityZ);
	SingleSupportDistanceZ = nextSingleSupportLandingSiteZ - npz;

	DoubleSupportDistanceZ = (SingleSupportDistanceZ / StrideZ) * Ldz;

	return DoubleSupportDistanceZ;
};

double CRWCLandingSite::CalcNextVelocity(double nextlandingsite, double np, double nv){
	double nnv;
	double sinh;
	double cosh;

    sinh = exp(T0 / Tc) / 2 - exp(- T0 / Tc) / 2;
	cosh = exp(T0 / Tc) / 2 + exp(- T0 / Tc) / 2;

	nnv = (np - nextlandingsite)/Tc * sinh + nv * cosh;

	return nnv;
}

double CRWCLandingSite::CalcLocalX(double xb, double zb, double xt, double zt, double theta){

	double vari0;
	double vari1;
	double relativeX;

	if(abs(tan(theta)) > 10000000000.0) {
		vari0 = xt;
		vari1 = zb;
	}
	else if(abs(1/tan(theta)) > 10000000000.0) {
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

double CRWCLandingSite::CalcLocalZ(double xb, double zb, double xt, double zt, double theta){

	double vari0;
	double vari1;
	double relativeZ;

	if(abs(tan(theta)) > 10000000000.0) {
		vari0 = xt;
		vari1 = zb;
	}
	else if(abs(1/tan(theta)) > 10000000000.0) {
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

double CRWCLandingSite::CalcLocalVX(double vx, double vz, double theta){

	double LocalVX;

	LocalVX = cos(theta)*vx - sin(theta)*vz;

	return LocalVX;
}

double CRWCLandingSite::CalcLocalVZ(double vx, double vz, double theta){

	double LocalVZ;

	LocalVZ = cos(theta)*vz + sin(theta)*vx;

	return LocalVZ;
}
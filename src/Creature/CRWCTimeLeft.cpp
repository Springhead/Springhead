#include ".\crwctimeleft.h"

CRWCTimeLeft::CRWCTimeLeft(double t)
{
	TimeStep = t;
}

CRWCTimeLeft::~CRWCTimeLeft(void)
{
}

void CRWCTimeLeft::Init(){
	Tkc = 400.0;
	Tkvx = 3.0;
	Tkvz = 3.0;
	Tkl = 6.0; 

	pi = 3.1415926535;
}

void CRWCTimeLeft::UpdateState(double tl, double elat, double cfl, Vec3d ch, Vec3d v, Vec3d cls, Vec3d nls, Vec3d cfp, bool lf, double cd){
	timeleft = tl;
	et = elat;
	CurrentFootLength = cfl;
	change = ch;
	VelocityOfWholeBody = v;
	CurrentLandingSite = cls;
	NextLandingSite = nls;
	CurrentFootPosition = cfp;
	CurrentDirection = cd;
	if(lf == true) {
		LF = true;
		RF = false;
	} else {
		LF = false;
		RF = true;
	}
}

double CRWCTimeLeft::CalcNextStepTimeLeft(void){

     double tl;
	 double vari0;
	 double vari1;
	 double vari2;
	 double param0 = -0.07;
	 double param1 = 1.1;
	 double LocalBodyVelocityX;
	 double LocalBodyVelocityZ;
	 double abschange;
	 double aveRate;

	 LocalBodyVelocityX = CalcLocalVX(VelocityOfWholeBody.x, VelocityOfWholeBody.z, CurrentDirection);
     LocalBodyVelocityZ = CalcLocalVZ(VelocityOfWholeBody.x, VelocityOfWholeBody.z, CurrentDirection);

	 if(CurrentFootLength < MaxFootLength && CurrentFootLength > MaxFootLength + param0) vari0 = 1/(MaxFootLength - CurrentFootLength)/Tkl;
	 else vari0 = 0.0;
	 if(LocalBodyVelocityX > param1*VelocityX) vari1 = (LocalBodyVelocityX - VelocityX) / Tkvx;
	 else if(LocalBodyVelocityX < 0.0) vari1 = -LocalBodyVelocityX / Tkvx;
	 else vari1 = 0.0;
	 if(LocalBodyVelocityZ > param1*VelocityZ) vari2 = (LocalBodyVelocityZ - VelocityZ) / Tkvz;
	 else if(LocalBodyVelocityZ < -param1*VelocityZ) vari2 = (-VelocityZ - LocalBodyVelocityZ) / Tkvz;
	 else vari2 = 0.0;

	 //DSTR << "vari0 = " << vari0 << " vari1 = " << vari1 << " vari2 = " << vari2 << std::endl; 
		 
	 abschange = change.norm() / Tkc + vari0 + vari1 + vari2;

	 //DSTR << "abschange = " << abschange << std::endl;

	 aveRate = (timeleft + et) / T0;

	 if(1.0 - abschange > minCycleRate) tl = (1.0 - abschange) / aveRate * timeleft - TimeStep;
	 else tl = minCycleRate / aveRate * timeleft - TimeStep;

	 //DSTR << "pre tl = " << tl << std::endl;

	 if(tl > 0.0 && abs(CurrentFootPosition.y - FootSize/2.0)/MaxFootSpeedY > tl) tl = abs(CurrentFootPosition.y - FootSize/2.0)/MaxFootSpeedY;  //1.0部分後で要修正

	 //DSTR << "after tl = " << tl << std::endl;
	 //DSTR << "abschange = " << abschange << " LocalBodyVelocityZ = " << LocalBodyVelocityZ << std::endl;
	 //DSTR << "VelocityOfWholeBody = " << VelocityOfWholeBody << std::endl;
	 return tl;
}


double CRWCTimeLeft::CalcNextStepTimeLeft(double cx, double cz, double sx, double sz, double dsdx){

     double tl;
	 double vari0;
	 double vari1;
	 double vari2;
	 double param0 = -0.07;
	 double param1 = 1.1;
	 double LocalBodyX;
	 double LocalSX;
	 double LocalBodyVelocityX;
	 double LocalBodyVelocityZ;
	 double abschange;

	 LocalBodyX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, cx, cz, CurrentDirection);
	 LocalSX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, sx, sz, CurrentDirection);
	 LocalBodyVelocityX = CalcLocalVX(VelocityOfWholeBody.x, VelocityOfWholeBody.z, CurrentDirection);
     LocalBodyVelocityZ = CalcLocalVZ(VelocityOfWholeBody.x, VelocityOfWholeBody.z, CurrentDirection);

	 
	 if(CurrentFootLength < MaxFootLength && CurrentFootLength > MaxFootLength + param0) vari0 = 1/(MaxFootLength - CurrentFootLength)/Tkl;
	 else vari0 = 0.0;
	 if(LocalBodyVelocityX > VelocityX) vari1 = (LocalBodyVelocityX - VelocityX) / Tkvx;
	 else if(LocalBodyVelocityX < 0.0) vari1 = -LocalBodyVelocityX / Tkvx;
	 else vari1 = 0.0;
	 if(LocalBodyVelocityZ > param1*VelocityZ) vari2 = (LocalBodyVelocityZ - VelocityZ) / Tkvz;
	 else if(LocalBodyVelocityZ < -param1*VelocityZ) vari2 = (-VelocityZ - LocalBodyVelocityZ) / Tkvz;
	 else vari2 = 0.0;
		 
	 abschange = change.norm() / Tkc + vari0 + vari1 + vari2;

	 if(LocalBodyX > LocalSX + dsdx/2){
		 if(1.0 - abschange > minCycleRate) tl = (1.0 - abschange) * T0;
		 else tl = minCycleRate * T0;
	 } else tl = 0.0;

	 if(tl > 0.0 && abs(CurrentFootPosition.y - FootSize/2.0)/MaxFootSpeedY > tl) tl = abs(CurrentFootPosition.y - FootSize/2.0)/MaxFootSpeedY;  //1.0部分後で要修正

	 return tl;

}

/*
//両脚支持期間の予想残り時間
double TimeLeft::CalcDoubleSupportTimeLeft(double cx, double sx, double cvx, double dsdx){
  
    double param0 = 1.0;
	double param1 = 0.2;
    double dstl;

	dstl = param0 * (sx+dsdx - cx) / cvx;

	if(dstl < 0.0) dstl = 0.0;
	if(dstl > param1*T0) dstl = param1*T0;

	return dstl;
}*/

//両脚支持期間を半分に分けた時の第一のパートにかかる予想残り時間
double CRWCTimeLeft::CalcFirstHalfDoubleSupportTimeDouble(double cx, double sx, double cvx, double dsdx, double cz, double sz, double cvz){

	double param0 = 1.0;
	double param1 = 0.2;
	double LocalBodyX;
	double LocalBodyVelocityX;
	double LocalSX;
	double MidPointX; 
	double CurrentStrideX;
	double fhdst;

	LocalBodyX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, cx, cz, CurrentDirection);
	LocalBodyVelocityX = CalcLocalVX(cvx, cvz, CurrentDirection);
	LocalSX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, sx, sz, CurrentDirection);
	MidPointX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, NextLandingSite.x, NextLandingSite.z, CurrentDirection) / 2.0;
	CurrentStrideX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, NextLandingSite.x, NextLandingSite.z, CurrentDirection);


	if(abs(change.z) > LimitChange && LocalBodyVelocityX > 0.0 && dsdx > 0.0 && dsdx+LocalSX > MidPointX + DoubleSupportLimitRate*CurrentStrideX){
		if(LocalBodyX > MidPointX + DoubleSupportLimitRate*CurrentStrideX) fhdst = dsdx/LocalBodyVelocityX/2.0;
		else if(LocalSX + dsdx/2 < MidPointX + DoubleSupportLimitRate*CurrentStrideX){
			if(LocalBodyX < LocalSX + dsdx/2.0) fhdst = param0*(LocalSX+dsdx/2.0 - LocalBodyX) / LocalBodyVelocityX;
			else  fhdst = dsdx/LocalBodyVelocityX/2.0;
		} else fhdst = param0*(MidPointX+DoubleSupportLimitRate*CurrentStrideX - LocalBodyX) / LocalBodyVelocityX;
	} else if(abs(change.z) > LimitChange && LocalBodyVelocityX > 0.0 && dsdx < 0.0 && LocalSX > MidPointX - DoubleSupportLimitRate*CurrentStrideX){
        if(LocalBodyX > MidPointX - DoubleSupportLimitRate*CurrentStrideX) fhdst = -dsdx/LocalBodyVelocityX/2.0;
		else if(LocalSX+dsdx/2.0 < MidPointX - DoubleSupportLimitRate*CurrentStrideX){
			if(LocalBodyX < LocalSX+dsdx/2.0) fhdst = param0*(LocalSX+dsdx/2.0 - LocalBodyX) / LocalBodyVelocityX;
			else  fhdst = -dsdx/LocalBodyVelocityX/2.0;
		} else fhdst = param0*(MidPointX-DoubleSupportLimitRate*CurrentStrideX - LocalBodyX) / LocalBodyVelocityX;
	} else if(abs(change.z) > LimitChange && LocalBodyVelocityX < 0.0) fhdst = 0.0;
	else {
		if(LocalBodyVelocityX > 0.0){
			if(LocalBodyX < LocalSX + dsdx/2.0) fhdst = param0*(LocalSX+dsdx/2.0 - LocalBodyX) / LocalBodyVelocityX;  
			if(LocalBodyX > LocalSX + dsdx/2.0) fhdst = abs(dsdx/LocalBodyVelocityX/2.0);
		} else {
			fhdst = 0.0;
		}
	}
	
	if(fhdst > param1*T0) fhdst = param1*T0;
	if(CalcDoubleSupportTimeZ(cx,cvx,cz,cvz) < fhdst) fhdst = CalcDoubleSupportTimeZ(cx,cvx,cz,cvz);

	if(fhdst < 0.0) {
		DSTR << "Double fhdst:マイナス値を取っています" << std::endl;
		exit(0);
	}

	return fhdst;
}

//両脚支持期間を半分に分けた時の第一のパートにかかる予想残り時間
double CRWCTimeLeft::CalcFirstHalfDoubleSupportTimeSingle(double cx, double cvx, double dsdx, double cz, double cvz){

	double param0 = 1.0;
	double param1 = 0.2;
	double LocalBodyX;
	double LocalBodyVelocityX;
	double MidPointX; 
	double CurrentStrideX;
	double estimatedSx;
	double fhdst;

	LocalBodyX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, cx, cz, CurrentDirection);
	LocalBodyVelocityX = CalcLocalVX(cvx, cvz, CurrentDirection);
	MidPointX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, NextLandingSite.x, NextLandingSite.z, CurrentDirection) / 2.0;
	CurrentStrideX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, NextLandingSite.x, NextLandingSite.z, CurrentDirection);
    estimatedSx = LocalBodyVelocityX*timeleft + LocalBodyX;

	if(abs(change.z) > LimitChange && LocalBodyVelocityX > 0.0 && estimatedSx > MidPointX + DoubleSupportLimitRate*abs(CurrentStrideX)) fhdst = 0.0;
	else if(abs(change.z) > LimitChange && LocalBodyVelocityX > 0.0 && dsdx+estimatedSx > MidPointX + DoubleSupportLimitRate*CurrentStrideX){
		if(estimatedSx + dsdx/2 < MidPointX + DoubleSupportLimitRate*abs(CurrentStrideX)) {
			fhdst = param0*abs(dsdx)/2.0 / LocalBodyVelocityX;
		}
		else fhdst = param0*(MidPointX+DoubleSupportLimitRate*abs(CurrentStrideX) - estimatedSx) / LocalBodyVelocityX;
	} else if(abs(change.z) > LimitChange && LocalBodyVelocityX < 0.0) fhdst = 0.0;
	else {
		if(LocalBodyVelocityX > 0.0){
			fhdst = param0*abs(dsdx)/2.0 / LocalBodyVelocityX;  
		} else {
			/*
			if(cx < sx + dsdx/2) fhdst = 0.0;
			if(cx > sx + dsdx/2) fhdst = -param0*(cx - (sx+dsdx/2)) / cvx;
			*/
			fhdst = 0.0;
		}
	}

	if(fhdst > param1*T0) fhdst = param1*T0;
	if(CalcDoubleSupportTimeZ(cx,cvx,cz,cvz) < fhdst) fhdst = CalcDoubleSupportTimeZ(cx,cvx,cz,cvz);

	if(fhdst < 0.0) {
		DSTR << "Single fhdst:マイナス値を取っています" << std::endl;
		exit(0);
	}

	return fhdst;
}

//両脚支持期間を半分に分けた時の第二のパートにかかる予想残り時間
double CRWCTimeLeft::CalcSecondHalfDoubleSupportTime(double cx, double sx, double cvx, double dsdx, double cz, double sz, double cvz){

	double param0 = 1.0;
	double param1 = 0.2;
	double LocalBodyX;
	double LocalBodyVelocityX;
	double LocalSX;
	double MidPointX; 
	double CurrentStrideX;
	double shdst = 0.0;

	LocalBodyX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, cx, cz, CurrentDirection);
	LocalBodyVelocityX = CalcLocalVX(cvx, cvz, CurrentDirection);
	LocalSX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, sx, sz, CurrentDirection);
	MidPointX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, NextLandingSite.x, NextLandingSite.z, CurrentDirection) / 2.0;
	CurrentStrideX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, NextLandingSite.x, NextLandingSite.z, CurrentDirection);


	if(abs(change.z) > LimitChange && LocalBodyVelocityX > 0.0 && dsdx > 0.0 && dsdx+LocalSX > MidPointX + DoubleSupportLimitRate*CurrentStrideX){
	    if(LocalSX + dsdx/2 < MidPointX + DoubleSupportLimitRate*CurrentStrideX && LocalBodyX > LocalSX + dsdx/2 && MidPointX+DoubleSupportLimitRate*CurrentStrideX > LocalBodyX)
			shdst = param0*(MidPointX+DoubleSupportLimitRate*CurrentStrideX - LocalBodyX) / LocalBodyVelocityX;
		else  shdst = 0.0;
	} else if(abs(change.z) > LimitChange && LocalBodyVelocityX > 0.0 && dsdx < 0.0 && LocalSX > MidPointX - DoubleSupportLimitRate*CurrentStrideX){
        if(LocalSX+dsdx/2.0 < MidPointX - DoubleSupportLimitRate*CurrentStrideX && LocalBodyX > LocalSX+dsdx/2.0 && MidPointX-DoubleSupportLimitRate*CurrentStrideX > LocalBodyX)
			shdst = param0*(MidPointX-DoubleSupportLimitRate*CurrentStrideX - LocalBodyX) / LocalBodyVelocityX;
		else  shdst = 0.0;
	} /*else if(change.z < -LimitChange && cvx < 0.0 && dsdx < 0.0 && sx + dsdx < MidPointX + DoubleSupportLimitRate*CurrentStrideX){
        if(sx + dsdx/2 > MidPointX + DoubleSupportLimitRate*CurrentStrideX && cx < sx + dsdx/2.0 && cx > (MidPointX+DoubleSupportLimitRate*CurrentStrideX))
			shdst = -param0*(cx - (MidPointX+DoubleSupportLimitRate*CurrentStrideX)) / cvx;
		else shdst = 0.0;	
	} else if(change.z < -LimitChange && cvx < 0.0 && dsdx > 0.0 && sx < MidPointX - DoubleSupportLimitRate*CurrentStrideX){
        if(sx + dsdx/2 > MidPointX - DoubleSupportLimitRate*CurrentStrideX && cx < sx + dsdx/2.0 && cx > (MidPointX-DoubleSupportLimitRate*CurrentStrideX))
			shdst = -param0*(cx - (MidPointX - DoubleSupportLimitRate*CurrentStrideX)) / cvx;
		else shdst = 0.0;
		}*/
	else if(abs(change.z) > LimitChange && LocalBodyVelocityX < 0.0) shdst = 0.0;
	else {
		if(LocalBodyVelocityX > 0.0 && dsdx > 0.0){
			shdst = param0*(LocalSX+dsdx - LocalBodyX) / LocalBodyVelocityX;
			if(LocalBodyX < LocalSX+dsdx/2.0 || LocalBodyX > LocalSX+dsdx) shdst = 0.0;
		} else if(LocalBodyVelocityX > 0.0 && dsdx < 0.0){
			shdst = param0*(LocalSX - LocalBodyX) / LocalBodyVelocityX;
			if(LocalBodyX < LocalSX+dsdx/2.0 || LocalBodyX > LocalSX) shdst = 0.0;
		} /*else if(cvx < 0.0 && dsdx > 0.0){
			shdst = -param0*(cx - sx) / cvx;
			if(cx > sx+dsdx/2 || cx < sx) shdst = 0.0;
		} else {
			shdst = -param0*(cx - (sx+dsdx)) / cvx;
			if(cx > sx+dsdx/2 || cx < sx+dsdx) shdst = 0.0;
		}*/
		else shdst = 0.0;
	}

	if(shdst > param1*T0) shdst = param1*T0;
	if(CalcDoubleSupportTimeZ(cx,cvx,cz,cvz) < shdst) shdst = CalcDoubleSupportTimeZ(cx,cvx,cz,cvz);

    //DSTR << "shdst = " << shdst << std::endl;
	if(shdst < 0.0) {
		DSTR << "shdst:マイナス値を取っています" << std::endl;
		exit(0);
	}

	return shdst;
}

double CRWCTimeLeft::CalcDoubleSupportTimeZ(double cx, double cvx, double cz, double cvz){

	double LocalBodyZ;
	double LocalBodyVelocityZ;
	double MidPointZ;  
	double CurrentStrideZ;
	double dstz;

	LocalBodyZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, cx, cz, CurrentDirection);
	LocalBodyVelocityZ = CalcLocalVZ(cvx, cvz, CurrentDirection);
	MidPointZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, NextLandingSite.x, NextLandingSite.z, CurrentDirection) / 2.0;
	CurrentStrideZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, NextLandingSite.x, NextLandingSite.z, CurrentDirection);


	if(timeleft < TimeStep){
		if(abs(change.x) > LimitChange && LocalBodyVelocityZ > 0.0 && LocalBodyZ > MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ)) dstz = 0.0;
		else if(abs(change.x) > LimitChange && LocalBodyVelocityZ < 0.0 && LocalBodyZ < MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ)) dstz = 0.0;
		else if(abs(change.x) > LimitChange && LocalBodyVelocityZ > 0.0 && LocalBodyZ < MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ)) dstz = (MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ) - LocalBodyZ) / LocalBodyVelocityZ;
		else if(abs(change.x) > LimitChange && LocalBodyVelocityZ < 0.0 && LocalBodyZ > MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ)) dstz = (MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ) - LocalBodyZ) / LocalBodyVelocityZ;
		else dstz = 10000000000.0;
	} else {
        if(abs(change.x) > LimitChange && LF == true && LocalBodyVelocityZ < 0.0 && LocalBodyZ < MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ)) dstz = 0.0;
		else if(abs(change.x) > LimitChange && RF == true && LocalBodyVelocityZ > 0.0 && LocalBodyZ > MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ)) dstz = 0.0;
		else if(abs(change.x) > LimitChange && LF == true && LocalBodyVelocityZ < 0.0 && LocalBodyZ > MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ)) dstz = (MidPointZ - DoubleSupportLimitRate*abs(CurrentStrideZ) - LocalBodyZ) / LocalBodyVelocityZ;
		else if(abs(change.x) > LimitChange && RF == true && LocalBodyVelocityZ > 0.0 && LocalBodyZ < MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ)) dstz = (MidPointZ + DoubleSupportLimitRate*abs(CurrentStrideZ) - LocalBodyZ) / LocalBodyVelocityZ;
		else dstz = 10000000000.0;
	}

	return dstz;
}

double CRWCTimeLeft::CalcLocalX(double xb, double zb, double xt, double zt, double theta){

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

	if(abs(theta) < pi/2){
		if(vari0 < xt) relativeX = sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
		else relativeX = -sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
	} else if(abs(theta) > pi/2 && abs(theta) < 3*pi/2) {
		if(vari0 < xt) relativeX = -sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
		else relativeX = sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
	} else if(abs(theta) > 3/2*pi){
		if(vari0 < xt) relativeX = sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
		else relativeX = -sqrt((vari0-xt)*(vari0-xt) + (vari1-zt)*(vari1-zt));
    } else {
		if(theta > 0.0) relativeX = zt - zb;
		else relativeX = zb - zt;
	}

	return relativeX;
}

double CRWCTimeLeft::CalcLocalZ(double xb, double zb, double xt, double zt, double theta){

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

	if(abs(theta) < pi/2){
		if(vari1 > zb) relativeZ = sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
		else relativeZ = -sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
	} else if(abs(theta) > pi/2 && abs(theta) < 3*pi/2) {
		if(vari1 > zb) relativeZ = -sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
		else relativeZ = sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
	} else if(abs(theta) > 3/2*pi){
		if(vari1 > zb) relativeZ = sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
		else relativeZ = -sqrt((vari0-xb)*(vari0-xb) + (vari1-zb)*(vari1-zb));
	} else {
		if(theta > 0.0) relativeZ = xt - xb;
		else relativeZ = xb - xt;
	}

	return relativeZ;
}

double CRWCTimeLeft::CalcLocalVX(double vx, double vz, double theta){

	double LocalVX;

	LocalVX = cos(theta)*vx - sin(theta)*vz;

	return LocalVX;
}

double CRWCTimeLeft::CalcLocalVZ(double vx, double vz, double theta){

	double LocalVZ;

	LocalVZ = cos(theta)*vz + sin(theta)*vx;

	return LocalVZ;
}
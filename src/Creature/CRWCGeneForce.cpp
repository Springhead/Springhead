#include ".\crwcgeneforce.h"
#include <Base/TMatrix.h>
#include <Base/TVector.h>

using namespace PTM;

CRWCGeneForce::CRWCGeneForce(Vec3d agf, double ts)
{
	AntiGravityForce = agf;
	TimeStep = ts;
}

CRWCGeneForce::~CRWCGeneForce(void)
{
}

void CRWCGeneForce::Init(){
	presheta = 0.0;
	pitchdiff = 0.0;
	currentAmp = 0.0;
	pi= 3.1415926535;
    kpA = 5.0;
	kvA = 3.0; 
    miu = 0.8;
	DoubleFootForcePre.resize(6);
}

void CRWCGeneForce::UpdateState(Vec3d pw, Vec3d vw, Vec3d ph, double thc, Vec3d ch, Vec3d cls, Vec3d nls ,double pr, bool dsflag, double cd){
 
	PositionOfWholeBody = pw;
	VelocityOfWholeBody = vw;
	PositionOfHip = ph;
    timehalfcycle = thc;
	change = ch;
    CurrentLandingSite = cls;
	NextLandingSite = nls;
	PositionRate = pr;
	DoubleSupportTermFlag = dsflag;
	CurrentDirection = cd;

	CurrentFootLength = (CurrentLandingSite - PositionOfHip).norm();
}

/*
double CRWCGeneForce::CalcPitchDelay(void){
    double vari;
	double fv;

	if(timeleft == 0){
		 currentAmp = (maxY - minY) / 2; 
		 fv = VelocityOfWholeBody.y;

		 if(abs(fv*T0 / (2*pi*currentAmp)) > 1.0) vari = pitchdelay;
		 else vari = (-1) * T0/(2*pi) * asin(fv*T0 / (2*pi*currentAmp));

		 maxY = PositionOfWholeBody.y;
		 minY = PositionOfWholeBody.y;
	 } else {
         vari = pitchdelay;

		 if(PositionOfWholeBody.y > maxY) maxY = PositionOfWholeBody.y;
		 if(PositionOfWholeBody.y < minY) minY = PositionOfWholeBody.y;
	}
 
    
	 return vari;
}*/

double CRWCGeneForce::CalcPitchDelay(void){
    double vari;
	double fv;

	if(timehalfcycle == 0){
		 currentAmp = (maxY - minY) / 2; 
		 fv = VelocityOfWholeBody.y;

		 if(currentAmp == 0.0) vari = pitchdelay;
		 else if(abs(fv*(T0+T0ds) / (2*pi*currentAmp)) > 1.0) vari = pitchdelay;
		 else vari = (-1) * (T0+T0ds)/(2*pi) * asin(fv*(T0+T0ds) / (2*pi*currentAmp));

		 maxY = PositionOfWholeBody.y;
		 minY = PositionOfWholeBody.y;
	 } else {
         vari = pitchdelay;

		 if(PositionOfWholeBody.y > maxY) maxY = PositionOfWholeBody.y;
		 if(PositionOfWholeBody.y < minY) minY = PositionOfWholeBody.y;
	}
 
    
	 return vari;
}

//床反力のX方向の値
double CRWCGeneForce::ForwardForce(double changeZ, double Fy){

	 double dx = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfWholeBody.x, PositionOfWholeBody.z, CurrentDirection);
	 double dy = PositionOfWholeBody.y - CurrentLandingSite.y;
	 double Fx;

	 if(dy > 0.0) Fx = (Fy * dx + changeZ) / dy;
	 else Fx = 0.0;

	 return Fx;
}

//床反力のZ方向
double CRWCGeneForce::SideForce(double changeX, double Fy){

	double dz = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfWholeBody.x, PositionOfWholeBody.z, CurrentDirection);
	double dy = PositionOfWholeBody.y - CurrentLandingSite.y;
	double Fz;

	if(dy > 0.0) Fz = (Fy * dz - changeX) / dy;
	else Fz = 0.0;

	return Fz;
}


void CRWCGeneForce::GeneCenterForce(void){

	 double Fx;
	 double Fz;
     double Fy;
	 double Fyd;
	 double pd;
	 double vd;
	 double T1;
	 double delta;
	 double vari;
	 double vari0;
	 double vari1;
	 double param = 0.9;
	 double paramFootMargin = 0.12;
	 double LocalBodyX = CalcLocalX(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfWholeBody.x, PositionOfWholeBody.z, CurrentDirection);
	 double LocalBodyZ = CalcLocalZ(CurrentLandingSite.x, CurrentLandingSite.z, PositionOfWholeBody.x, PositionOfWholeBody.z, CurrentDirection);



     pitchdelay = CalcPitchDelay();

	 
	 if(timehalfcycle < 0.005 && timehalfcycle > 0.0) delta = presheta * preT1 / (2.0*pi); ///////////////////////////////////////////////////////////////TimeStepとの兼ね合いでパラメータ調整し直す
	 else delta = timehalfcycle*presheta / (2*pi + 2*pi*pitchdiff/(T0+T0ds) - presheta); // 2pi*delta/T1 = presheta -> delta = presheta*T1/2pi
	 if(timehalfcycle < 0.005 && timehalfcycle > 0.0) T1 = preT1;/////////////////////////////////////////////////////////////////////////////////////////TimeStepとの兼ね合いでパラメータ調整し直す
	 else  T1 = 2*pi * timehalfcycle / (2*pi + 2*pi*pitchdiff/(T0+T0ds) - presheta);;  // 2pi/T1*timeleft + presheta = 2pi + 2pi*pitchdiff/T0

	 pd = height;
	 vd = 0.0;

	 /*
	 if(T1 == 0.0){
		 pd = (-amplitude) * cos(0.0) + height;
		 vd = 0.0;
	 }
	 else{
		 pd = (-amplitude) * cos(2*pi * (delta + TimeStep) / T1) + height;
	     vd = 2*pi / T1 * amplitude * sin(2*pi * (delta + TimeStep) / T1);
	 }*/

	 Fyd = kpA * (pd - PositionOfWholeBody.y) + kvA * (vd - VelocityOfWholeBody.y) + AntiGravityForce.y;
     Fx = ForwardForce(change.z, Fyd) + AntiGravityForce.x;
	 Fz = SideForce(change.x, Fyd) + AntiGravityForce.z;
	 vari = max(Fyd, abs(Fx)/miu);
	 Fy = max(vari, abs(Fz)/miu);

	 //DSTR << "Fyd = " << Fyd << " Fy = " << Fy << std::endl;
	 //DSTR << "change.z " << change.z << std::endl;

	 if(Fy > 1.5*AntiGravityForce.y) Fy = 1.5*AntiGravityForce.y;
	 
	 /*
	 if(LocalBodyX > 0.0){
		 if(Fx < 0.0) Fx = 0.0;
	 } else if(LocalBodyX < 0.0){
		 if(Fx > 0.0) Fx = 0.0;
	 }

	 if(LocalBodyZ > 0.0){
		 if(Fz < 0.0) Fz = 0.0;
	 } else if(LocalBodyZ < 0.0){
		 if(Fz > 0.0) Fz = 0.0;
	 }*/

	 //DSTR << "Pre" << " Fx = " << Fx << " Fy = " << Fy << " Fz = " << Fz << std::endl;

	 ////力の身体的制約//////
	 if(CurrentFootLength > MaxFootLength-paramFootMargin) {
		 Fx = Fx*(MaxFootLength-CurrentFootLength)/paramFootMargin;
		 Fy = Fy*(MaxFootLength-CurrentFootLength)/paramFootMargin;
		 Fz = Fz*(MaxFootLength-CurrentFootLength)/paramFootMargin;
	 }
	 //DSTR << "CurrentFootLength = " << CurrentFootLength << std::endl;
	 //DSTR << "force = " << Vec3d(Fx,Fy,Fz) << std::endl;

	 //DSTR << "after" << " Fx = " << Fx << " Fy = " << Fy << " Fz = " << Fz << std::endl;

	 vari0 = cos(CurrentDirection)*Fx + sin(CurrentDirection)*Fz;
	 vari1 = cos(CurrentDirection)*Fz - sin(CurrentDirection)*Fx;
	 Fx = vari0;
	 Fz = vari1;

	 //DSTR << "Fx = " << Fx << " Fy = " << Fy << " Fz = " << Fz << std::endl;
	 if(DoubleSupportTermFlag) {
		 CenterObject-> AddForce(Vec3d(0.0, Fy/2, 0.0), CurrentLandingSite);
         CenterObject-> AddForce(Vec3d(0.0, Fy/2, 0.0), NextLandingSite);
		 //CenterObject->AddTorque(Vec3d(100*change.x, 0.0, 100*change.z));
		 //DoubleFootForcePre = CalcBothLegsFloorForce(Fy);
		 //DSTR << "Double Force = " << DoubleFootForcePre << std::endl;
	 }
	 else {
		 CenterObject->AddForce(Vec3d(Fx, Fy, Fz), CurrentLandingSite);
		 DoubleFootForcePre.clear();
	 }

	 if(T1 == 0.0) {
		 pitchdiff = pitchdelay + pitchdiff;
		 presheta = 2*pi*pitchdiff / (T0+T0ds);     //presheta = 0.0;
	 }
	 else presheta = 2*pi * (delta + TimeStep) / T1;

	 preT1 = T1;

	 //DSTR << "presheta = " << presheta << " pitchdiff = " << pitchdiff << " pitchdelay = " << pitchdelay  << " T1 = " << T1 << " timehalfcycle = " << timehalfcycle << std::endl;
}

void CRWCGeneForce::InitialGeneForce(void){

	 double Fx;
	 double Fz;
     double Fy;
	 double Fyd;
	 double vari;
	 double param = 10.0;

	 Fyd = AntiGravityForce.y;
	 Fx = ForwardForce(change.z + param, Fyd) + AntiGravityForce.x;
	 Fz = SideForce(change.x, Fyd) + AntiGravityForce.z;
	 vari = max(Fyd, abs(Fx)/miu);
	 Fy = max(vari, abs(Fz)/miu);

	 CenterObject->AddForce(Vec3d(Fx, Fy, Fz), CurrentLandingSite);
	 
	 DSTR << "force = " << Vec3d(Fx,Fy,Fz) << std::endl;
}
double CRWCGeneForce::CalcLocalX(double xb, double zb, double xt, double zt, double theta){

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

double CRWCGeneForce::CalcLocalZ(double xb, double zb, double xt, double zt, double theta){

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

double CRWCGeneForce::CalcLocalVX(double vx, double vz, double theta){

	double LocalVX;

	LocalVX = cos(theta)*vx - sin(theta)*vz;

	return LocalVX;
}

double CRWCGeneForce::CalcLocalVZ(double vx, double vz, double theta){

	double LocalVZ;

	LocalVZ = cos(theta)*vz + sin(theta)*vx;

	return LocalVZ;
}

VVector<double> CRWCGeneForce::CalcBothLegsFloorForce(double ForceY){

	double a1 = 0.3;
	double a2 = 0.3;
	double b1 = 1.0;
	double b2 = 1.0;
	double c1 = 0.05;
	double c2 = 0.05;
	double d1 = 0.3;
	double e1 = 0.001;
	double e2 = 0.001;
	double e3 = 0.001;
	double e4 = 0.001;
    double xf = PositionOfWholeBody.x - NextLandingSite.x;
	double xb = PositionOfWholeBody.x - CurrentLandingSite.x;
	double yf = (2.0*PositionOfWholeBody.y - CurrentLandingSite.y - NextLandingSite.y) / 2.0 + 0.001;
	double yb = (2.0*PositionOfWholeBody.y - CurrentLandingSite.y - NextLandingSite.y) / 2.0 - 0.001;
	double zf = PositionOfWholeBody.z - NextLandingSite.z;
	double zb = PositionOfWholeBody.z - CurrentLandingSite.z;
	double kvx = 10.0;
	double kvz = 10.0;
	double Fxo = kvx * (paramVX - VelocityOfWholeBody.x);
	double Fyo = ForceY;
	double Fzo;
	double tauXo = change.x;
	double tauZo = change.z;
	double Pxf = NextLandingSite.x;
	double Pxb = CurrentLandingSite.x;
	double Pzf = NextLandingSite.z;
	double Pzb = CurrentLandingSite.z;
	double ZMPxo = PositionRate * (Pxf - Pxb) + Pxb;
	double ZMPzo = PositionRate * (Pzf - Pzb) + Pzb;
	double miu = 0.7;
    VMatrixRow<double> Q;
	VMatrixRow<double> A;
	VMatrixRow<double> B;
	VVector<double> c;
	VVector<double> L;
	VVector<double> R;

	if(NextLandingSite.z > CurrentLandingSite.z) Fzo = kvz * (paramVZ - VelocityOfWholeBody.z);
	else Fzo = kvz * (-paramVZ - VelocityOfWholeBody.z);

	Q.resize(6,6);


	Q[0][0] = 2*(a1 + b1*yf*yf + e1*yf*yf);
    Q[0][1] = 2*a1 + 2*b1*yf*yb;
	Q[1][0] = 2*a1 + 2*b1*yf*yb;
	Q[0][2] = -2*b1*xf*yf -2*e1*xf*yf;
	Q[2][0] = -2*b1*xf*yf -2*e1*xf*yf;
    Q[1][1] = 2*(a1 + b1*yb*yb + e3*yb*yb);
	Q[0][3] = -2*b1*yf*xb;
	Q[3][0] = -2*b1*yf*xb;
	Q[1][2] = -2*b1*xf*yb; 
	Q[2][1] = -2*b1*xf*yb;
	Q[0][4] = 0.0;
    Q[4][0] = 0.0;
	Q[3][1] = -2*b1*xb*yb -2*e3*xb*yb;
	Q[1][3] = -2*b1*xb*yb -2*e3*xb*yb;
	Q[2][2] = 2*(b1*xf*xf + b2*zf*zf + c1*Pxf*Pxf + c1*ZMPxo*ZMPxo -2*c1*Pxf*ZMPxo + c2*Pzf*Pzf + c2*ZMPzo*ZMPzo -2*c2*Pzf*ZMPzo + d1 + e1*xf*xf + e2*zf*zf);
	Q[0][5] = 0.0;
	Q[5][0] = 0.0;
	Q[1][4] = 0.0;
	Q[4][1] = 0.0;
	Q[2][3] = 2*b1*xf*xb + 2*b2*zf*zb + 2*c1*ZMPxo*ZMPxo -2*c1*Pxf*ZMPxo + 2*c1*Pxf*Pxb -2*c1*Pxb*ZMPxo + 2*c2*ZMPzo*ZMPzo -2*c2*Pzf*ZMPzo + 2*c2*Pzf*Pzb -2*c2*Pzb*ZMPzo + 2*d1;
    Q[3][2] = 2*b1*xf*xb + 2*b2*zf*zb + 2*c1*ZMPxo*ZMPxo -2*c1*Pxf*ZMPxo + 2*c1*Pxf*Pxb -2*c1*Pxb*ZMPxo + 2*c2*ZMPzo*ZMPzo -2*c2*Pzf*ZMPzo + 2*c2*Pzf*Pzb -2*c2*Pzb*ZMPzo + 2*d1;
	Q[1][5] = 0.0;
	Q[5][1] = 0.0;
	Q[2][4] = -2*b2*yf*zf -2*e2*yf*zf;
	Q[4][2] = -2*b2*yf*zf -2*e2*yf*zf; 
	Q[3][3] = 2*(b1*xb*xb + b2*zb*zb + c1*Pxb*Pxb + c1*ZMPxo*ZMPxo -2*c1*Pxb*ZMPxo + c2*Pzb*Pzb + c2*ZMPzo*ZMPzo -2*c2*Pzb*ZMPzo + d1 + e3*xb*xb + e4*zb*zb); 
	Q[2][5] = -2*b2*zf*yb;
	Q[5][2] = -2*b2*zf*yb;
	Q[3][4] = -2*b2*yf*zb;
	Q[4][3] = -2*b2*yf*zb;
	Q[3][5] = -2*b2*yb*zb -2*e4*yb*zb;
    Q[5][3] = -2*b2*yb*zb -2*e4*yb*zb;
	Q[4][4] = 2*(a2 + b2*yf*yf + e2*yf*yf);
	Q[4][5] = 2*a2 + 2*b2*yf*yb;
	Q[5][4] = 2*a2 + 2*b2*yf*yb;
	Q[5][5] = 2*(a2 + b2*yb*yb + e4*yb*yb);


	c.resize(6);
 
	c[0] = -2*a1*Fxo + 2*b1*yf*tauZo;
	c[1] = -2*a1*Fxo + 2*b1*yb*tauZo;
	c[2] = -2*b1*xf*tauZo + 2*b2*zf*tauXo -2*d1*Fyo;
	c[3] = -2*b1*xb*tauZo + 2*b2*zb*tauXo -2*d1*Fyo;
	c[4] = -2*a2*Fzo -2*b2*yf*tauXo;
	c[5] = -2*a2*Fzo -2*b2*yb*tauXo;

    
	B.resize(8,6);

	if(CurrentLandingSite.x < NextLandingSite.x){

		B[0][0] = 1.0;
	    B[0][1] = 0.0;
	    B[0][2] = 0.0;
	    B[0][3] = 0.0;
	    B[0][4] = 0.0;
	    B[0][5] = 0.0;

	    B[1][0] = -1.0;
	    B[1][1] = 0.0;
        B[1][2] = -miu;
	    B[1][3] = 0.0;
	    B[1][4] = 0.0;
	    B[1][5] = 0.0;

	    B[2][0] = 0.0;
	    B[2][1] = -1.0;
        B[2][2] = 0.0;
	    B[2][3] = 0.0;
	    B[2][4] = 0.0;
	    B[2][5] = 0.0;

	    B[3][0] = 0.0;
	    B[3][1] = 1.0;
        B[3][2] = 0.0;
	    B[3][3] = -miu;
	    B[3][4] = 0.0;
	    B[3][5] = 0.0;

	} else {

		B[0][0] = -1.0;
	    B[0][1] = 0.0;
	    B[0][2] = 0.0;
	    B[0][3] = 0.0;
	    B[0][4] = 0.0;
	    B[0][5] = 0.0;

	    B[1][0] = 1.0;
	    B[1][1] = 0.0;
        B[1][2] = -miu;
	    B[1][3] = 0.0;
	    B[1][4] = 0.0;
	    B[1][5] = 0.0;

	    B[2][0] = 0.0;
	    B[2][1] = 1.0;
        B[2][2] = 0.0;
	    B[2][3] = 0.0;
	    B[2][4] = 0.0;
	    B[2][5] = 0.0;

	    B[3][0] = 0.0;
	    B[3][1] = -1.0;
        B[3][2] = 0.0;
	    B[3][3] = -miu;
	    B[3][4] = 0.0;
	    B[3][5] = 0.0;

	}

	if(CurrentLandingSite.z < NextLandingSite.z){

		B[4][0] = 0.0;
	    B[4][1] = 0.0;
        B[4][2] = 0.0;
	    B[4][3] = 0.0;
	    B[4][4] = 1.0;
	    B[4][5] = 0.0;

	    B[5][0] = 0.0;
	    B[5][1] = 0.0;
        B[5][2] = 0.0;
	    B[5][3] = 0.0;
	    B[5][4] = 0.0;
	    B[5][5] = -1.0;

	    B[6][0] = 0.0;
	    B[6][1] = 0.0;
        B[6][2] = 0.0;
	    B[6][3] = -miu;
	    B[6][4] = 0.0;
	    B[6][5] = 1.0;

	    B[7][0] = 0.0;
	    B[7][1] = 0.0;
        B[7][2] = -miu;
	    B[7][3] = 0.0;
	    B[7][4] = -1.0;
	    B[7][5] = 0.0;

	} else {

	    B[4][0] = 0.0;
	    B[4][1] = 0.0;
        B[4][2] = 0.0;
	    B[4][3] = 0.0;
	    B[4][4] = -1.0;
	    B[4][5] = 0.0;

	    B[5][0] = 0.0;
	    B[5][1] = 0.0;
        B[5][2] = 0.0;
	    B[5][3] = 0.0;
	    B[5][4] = 0.0;
	    B[5][5] = 1.0;

	    B[6][0] = 0.0;
	    B[6][1] = 0.0;
        B[6][2] = 0.0;
	    B[6][3] = -miu;
	    B[6][4] = 0.0;
	    B[6][5] = -1.0;

	    B[7][0] = 0.0;
	    B[7][1] = 0.0;
        B[7][2] = -miu;
	    B[7][3] = 0.0;
	    B[7][4] = 1.0;
	    B[7][5] = 0.0;

	}

	R.resize(8);

	R[0] = 0.0;
	R[1] = 0.0;
	R[2] = 0.0;
	R[3] = 0.0;
	R[4] = 0.0;
	R[5] = 0.0;
	R[6] = 0.0;
	R[7] = 0.0;

	A.resize(0,0);
	L.resize(0);
/*
	DoubleFootForcePre[0] = -0.2;
    DoubleFootForcePre[1] = 0.2;
	DoubleFootForcePre[2] = 0.8;
    DoubleFootForcePre[3] = 0.2;
	DoubleFootForcePre[4] = -0.2;
    DoubleFootForcePre[5] = 0.2;
*/
	return QuadraticProgramming(DoubleFootForcePre, Q, c, A, B, L, R);
}

//二次計画法(有効制約法)を行う
//目的関数がc^(T)y + 1/2y^(T)Qyで与えられ、
//等式制約がAy=L、不等式制約がBy<=Rで与えられたとする
//凸二次計画問題のみに対応
VVector<double> CRWCGeneForce::QuadraticProgramming(VVector<double> yPre, VMatrixRow<double> Q, VVector<double> c, VMatrixRow<double> A, VMatrixRow<double> B, VVector<double> L, VVector<double> R){

	VVector<double> y = yPre;                    //解ベクトル
	VVector<double> d;                           //減少方向ベクトル
	VVector<double> b = L;                       //working setの右辺ベクトル
	VVector<double> e = R;                       //working setに入っていない制約の右辺ベクトル
	VVector<double> miu;                         //ラグランジュ乗数
    VVector<double> variV;                      //変数として使うベクトル
	VMatrixRow<double>*Aq = new VMatrixRow<double>;   //working setの行列
	VMatrixRow<double>*Ar = new VMatrixRow<double>;   //working setに入っていない制約行列
    VMatrixRow<double> P;                        //射影行列
    VMatrixRow<double> I;                        //単位行列
	VMatrixRow<double> *Matrix0;                 //変数として用いる行列
	VMatrixRow<double> *Matrix1;                 //変数として用いる行列  
	double a1 = 10000000000.0;                    //実行可能領域をはみださないステップ幅
	double a2;                                   //最適解に到達するステップ幅
	double variD = 0.0;                          //変数    
	double threshold = 0.001;                          //閾値
	double preH = 10000000000.0;                 //1ステップ前の評価関数の値
	bool endflag = true;                         //終了フラグ
	bool Recalflag = true;                       //行列の再計算フラグ
	int changingNum;                             //入れ替える行列の行                           
	int LoopNum = 0;                             //現在のループ回数
	int MaxLoopNum = 30;                         //ループ回数の最大値

	d.resize(Q.width());                         //サイズ指定
	P.resize(Q.height(), Q.width());             //サイズ指定
	I.resize(Q.height(), Q.width());             //サイズ指定

	*Aq = A;                                     //行列の代入
	*Ar = B;                                     //行列の代入 
	Matrix0 = Aq;                                //アドレスの代入
	Matrix1 = Ar;                                //アドレスの代入

	I.clear(0.0);                                //単位行列を作成
	for(int i=0 ; i<(signed)I.width() ; i++){
		I[i][i] = 1.0;
	}

	while(true){
		if(Aq->width() == 0 || Aq->height() == 0){    //射影行列の計算
			P = I;
		} else {
			if(Recalflag){
				VMatrixRow<double> *variM0 = new VMatrixRow<double>; 
				*variM0 = ((*Aq)*(Aq->trans()));

				P = I - (Aq->trans())*(variM0->inv())*(*Aq);
                delete variM0;
			}
		}

		if(LoopNum < 2) d = (P)*(-Q.inv()*c - y);               //減少方向ベクトルの計算
		else d = (-P)*(c + Q*y);                                //最初の二回までニュートン法で減少方向ベクトルを決定
		                                                        //それ以降は勾配傾斜法を使用

		if(d.norm() > threshold){
			for(int i=0; i<(signed)e.size(); i++){          //実行可能領域をはみださないステップ幅の計算
				if(Ar->row(i)*d > 0){
					if((e[i] - Ar->row(i)*y) / (Ar->row(i)*d) < a1 && (e[i] - Ar->row(i)*y) / (Ar->row(i)*d) > 0.0) {
						a1 = (e[i] - Ar->row(i)*y) / (Ar->row(i)*d); 
						changingNum = i;
					}
				}
			}


			if(d.dot(Q*d) != 0.0){                  //最適解に到達するステップ幅の計算
				a2 = -(y.dot(Q*d) + c.dot(d))/d.dot(Q*d);
			}

			                                        //a1とa2を比較して小さい方をステップ幅とする
			if(a1 > a2) {
				y = y + a2*d;
				Recalflag = false;
			}
			else {
				Recalflag = true;

				y = y + a1*d;

				variV = b;                              //Ar*y<=e内の条件をworking Setに一つだけ追加する
				b.resize(b.size()+1);                   //実行可能領域をはみ出さない最小のステップ幅によって
				                                        //等式制約となるAr*y<=e内の条件をworking setに加える

			    VMatrixRow<double> *variM0 = new VMatrixRow<double>;
				*variM0 = *Aq;
	            delete Matrix0;
                Matrix0 = new VMatrixRow<double>;
				Matrix0->resize(variM0->height()+1 , y.size());
				Aq = Matrix0;
				for(int i=0; i<(signed)Aq->height(); i++){
					if(i < (signed)variM0->height()){
						Aq->row(i) = variM0->row(i);
						b[i] = variV[i];
					}
					else {
						Aq->row(i) = Ar->row(changingNum);
						b[i] = e[changingNum];
					}
				}
                delete variM0;

				variV = e;
				e.resize(e.size()-1);

				VMatrixRow<double> *variM1 = new VMatrixRow<double>;
				*variM1 = *Ar;
				delete Matrix1;
                Matrix1 = new VMatrixRow<double>;
				Matrix1->resize(variM1->height()-1, y.size());
				Ar = Matrix1;
				for(int i=0; i<(signed)Ar->height(); i++){
					if(i>=changingNum) {
						Ar->row(i) = variM1->row(i+1);
						e[i] = variV[i+1];
					}
					else {
						Ar->row(i) = variM1->row(i);
						e[i] = variV[i];
					}
				}
				delete variM1;
			}

			if(abs(c.dot(y) + y.dot(Q*y)/2 - preH) > threshold) endflag = false;         //前回と今回の評価関数の値の差が閾値以下ならば終了
			if(a1 > a2 && LoopNum == 0) endflag = true;                                  //ニュートン法で一回で解が求まれば終了

			if(endflag) {
				return y;
			}
			preH = c.dot(y) + y.dot(Q*y)/2;
			a1 = 10000000000.0;

		}
		else{
			VMatrixRow<double> *variM0 = new VMatrixRow<double>; 
			*variM0 = ((*Aq)*(Aq->trans()));
			miu = -(variM0->inv())*(*Aq)*(c + Q*y);
			delete variM0;
			for(int i=(signed)A.height(); i<(signed)miu.size(); i++){               //不等式制約がBy<=Rの中からworking setに				                                                    
			   if(miu[i] < variD) {                                 //加えられたworking setのmiuの値が正ならば終了
				   variD = miu[i];
				   changingNum = i;
				   endflag = false;
			   }
		   }
			if(endflag) {
				return y;
			}
		   else {
			   Recalflag = true;

			   variV = e;
			   e.resize(b.size()+1); 

			   VMatrixRow<double> *variM0 = new VMatrixRow<double>;
			   *variM0 = *Ar;
			   delete Matrix1;
			   Matrix1 = new VMatrixRow<double>;
			   Matrix1->resize(variM0->height()+1 , y.size());
			   Ar = Matrix1;
			   for(int i=0; i<(signed)Ar->height(); i++){
					if(i < (signed)variM0->height()){
						Ar->row(i) = variM0->row(i);
						e[i] = variV[i];
					}
					else {
						Ar->row(i) = Aq->row(changingNum);
						e[i] = b[changingNum];
					}
				}
                delete variM0;

				variV = b;
				b.resize(e.size()-1);

				VMatrixRow<double> *variM1 = new VMatrixRow<double>;
				*variM1 = *Aq;
				delete Matrix0;
                Matrix0 = new VMatrixRow<double>;
				Matrix0->resize(variM1->height()-1, y.size());
				Aq = Matrix0;

				for(int i=0; i<(signed)Aq->height(); i++){
					if(i>=changingNum) {
						Aq->row(i) = variM1->row(i+1);
						b[i] = variV[i+1];
					}
					else {
						Aq->row(i) = variM1->row(i);
						b[i] = variV[i];
					}
				}
				delete variM1;

		   }
		   variD = 0.0;
		}
		LoopNum = LoopNum + 1;                //MaxLoopNum回の反復計算を行ったらその時点の解を出力して終了
		if(LoopNum > MaxLoopNum){
			return y;
		}
		endflag = true;
	}
}

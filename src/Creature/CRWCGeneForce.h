/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#pragma once
#include <Springhead.h>		//	Springheadのインタフェース
#include <math.h>
#include <string.h>
#include <Base/TMatrix.h>
#include <Base/TVector.h>
#include <string.h>

using namespace Spr;
using namespace PTM;

class CRWCGeneForce
{
	Vec3d PositionOfWholeBody;
	Vec3d VelocityOfWholeBody;
	Vec3d PositionOfHip;
	PHSolidIf* CenterObject;

	Vec3d AntiGravityForce;
	double TimeStep;
	double PositionRate;
	double CurrentDirection;

	double amplitude;
	double height;
	double paramVX;
	double paramVZ;
	double MaxFootLength;

	double kpA;
	double kvA;
	double miu;

	double timehalfcycle;
	Vec3d CurrentLandingSite;
	Vec3d NextLandingSite;
	Vec3d change;
	double CurrentFootLength;
	double T0;
	double T0ds;

	double presheta;
	double preT1;
	double pitchdiff;
	double currentAmp;
	double pitchdelay;
	Vec3d force;
	Vec3d torque;
	VVector<double> DoubleFootForcePre;
	double pi;          // パイ
    double maxY;
	double minY;
	bool DoubleSupportTermFlag;

	double CalcPitchDelay(void);
	double ForwardForce(double changeZ, double Fy);
    double SideForce(double changeX, double Fy);
	double CalcLocalX(double xb, double zb, double xt, double zt, double theta);
	double CalcLocalZ(double xb, double zb, double xt, double zt, double theta);
	double CalcLocalVX(double vx, double vz, double theta);
	double CalcLocalVZ(double vx, double vz, double theta);

public:
	CRWCGeneForce(Vec3d agf, double ts);
	~CRWCGeneForce(void);
	void Init();
	void GeneCenterForce(void);
	void InitialGeneForce(void);
	Vec3d GetCenterForce() {return force;}
	VVector<double> QuadraticProgramming(VVector<double> yPre, VMatrixRow<double> Q, VVector<double> c, VMatrixRow<double> A, VMatrixRow<double> B, VVector<double> L, VVector<double> R);
    VVector<double> CalcBothLegsFloorForce(double ForceY);
	void UpdateState(Vec3d pw, Vec3d vw, Vec3d ph, double thc, Vec3d ch, Vec3d cls, Vec3d nls, double pr, bool dsflag, double cd);
	void SetCenterObject(PHSolidIf* co) {CenterObject = co;}
	void SetParamAmplitude(double am) {amplitude = am;}
	void SetParamheight(double h) {height = h;}
	void SetParamT0(double t) {T0 = t;}
	void SetParamT0ds(double t0ds) {T0ds = t0ds;}
	void SetParamVX(double vx) {paramVX = vx;}
	void SetParamVZ(double vz) {paramVZ = vz;}
	void SetParamMaxFootLength(double mfl) {MaxFootLength = mfl;}
	void SetParams(double KP, double KV, double MIU){
		kpA = KP;
		kvA = KV;
		miu = MIU;
	}
};

#pragma once
#include <Springhead.h>		//	Springheadのインタフェース
#include <Physics/PHScene.h>

using namespace Spr;


class CRWCTimeLeft
{
	Vec3d VelocityOfWholeBody;
	Vec3d CurrentLandingSite;
	Vec3d NextLandingSite;
	Vec3d CurrentFootPosition;

	double T0;
	double timeleft;
	double et;
	double maxDSRate;
	double minCycleRate;
	double TimeStep;
	double CurrentFootLength;
	double CurrentDirection;
	Vec3d change;
    bool LF;
	bool RF;

	double Tkc;
	double Tkvx;
	double Tkvz;
	double Tkl;
	double DoubleSupportLimitRate;
	double LimitChange;
	double MaxFootLength;
	double MaxFootSpeedY;
	double VelocityX;
    double VelocityZ;
	double FootSize;

	double pi;

	double CalcLocalX(double xb, double zb, double xt, double zt, double theta);
	double CalcLocalZ(double xb, double zb, double xt, double zt, double theta);
	double CalcLocalVX(double vx, double vz, double theta);
	double CalcLocalVZ(double vx, double vz, double theta);

public:
	CRWCTimeLeft(double t);
	~CRWCTimeLeft(void);
	double CalcNextStepTimeLeft(void);
	double CalcNextStepTimeLeft(double cx, double cz, double sx, double sz, double dsdx);
	double CalcDoubleSupportTimeLeft(double lvx, double dsdx);
	double CalcDoubleSupportTimeLeft(double cx, double sx, double cvx, double dsdx);
	double CalcFirstHalfDoubleSupportTimeSingle(double cx, double cvx, double dsdx, double cz, double cvz);
	double CalcFirstHalfDoubleSupportTimeDouble(double cx, double sx, double cvx, double dsdx, double cz, double sz, double cvz);
	double CalcSecondHalfDoubleSupportTime(double cx, double sx, double cvx, double dsdx, double cz, double sz, double cvz);
	double CalcDoubleSupportTimeZ(double cx, double cvx, double cz, double cvz);
	void Init(void);
	void UpdateState(double tl, double elat, double cfl ,Vec3d ch, Vec3d v, Vec3d cls, Vec3d nls, Vec3d cfp, bool lf, double cd);
	void SetParamT0(double t) {T0 = t;}
	void SetParamCycleRate(double c, double dsr) {minCycleRate = c; maxDSRate = dsr;}
	void SetParamTkc(double t) {Tkc = t;}
	void SetParamTkvx(double t) {Tkvx = t;}
	void SetParamTkvz(double t) {Tkvz = t;}
	void SetParamVelocityX(double vx) {VelocityX = vx;}
	void SetParamVelocityZ(double vz) {VelocityZ = vz;}
	void SetParamLimitChange(double lc) {LimitChange = lc;}
	void SetParamDoubleSupportLimitRate(double dslr) {DoubleSupportLimitRate = dslr;}
	void SetParamMaxFootLength(double mfl) {MaxFootLength = mfl;}
	void SetParamMaxFootSpeedY(double mfsy) {MaxFootSpeedY = mfsy;}
	void SetParamFootSize(double fs) {FootSize = fs;}
};

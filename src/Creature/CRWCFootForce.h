#pragma once
#include <Springhead.h>		//	Springheadのインタフェース
#include <math.h>
#include <Base/TMatrix.h>
#include <Base/TVector.h>
#include <string.h>

using namespace Spr;
using namespace PTM;

class CRWCFootForce
{
	Vec3d CurrentLandingSite;
	Vec3d NextLandingSite;
	Vec3d PositionOfHip;
	Vec3d VelocityOfHip;
	PHSolidIf* FootLeft;
	PHSolidIf* FootRight;
	Vec3d InitialRotLeft;
	Vec3d InitialRotRight;

	double timeleft;
	double TimeStep;
	double T0;
	Vec3d Gravity;

	bool LF;
	bool RF;
	bool PreLF;
	bool PreRF;
	bool LandAble;
	bool EarthConnection;
	bool footMoveTermFlag;

	double footheight;
	double MaxFootLength;
	double MinFootLength;
	double FootSize;
	double TargetDirection;

	void FootMoveLeft(void);
	void FootMoveRight(void);
	Vec3d CalcSwingFootForce(void);
	Vec3d CalcSwingFootTorque(void);
	Vec3d CalcSupportFootForce(void);
	Vec3d CalcDoubleSupportPreFootForce(void);
	Vec3d CalcDoubleSupportNextFootForce(void);

public:
	CRWCFootForce(double ts, Vec3d g, bool lf);
	~CRWCFootForce(void);
	void Init(void);
	void FootMove(void);
	void FootMove2(void);
	void FootDoubleSupport(void);
	void UpdateState(Vec3d cls, Vec3d nls, Vec3d ph, Vec3d vh, double tl, bool la, bool ec, double td);
	void ChangeSupportLegs(bool lf);
	void SetFoots(PHSolidIf* fl, PHSolidIf* fr){
		FootLeft = fl;
		FootRight = fr;
	}
	void SetParamT0(double t) {T0 = t;}
	void SetParamFootHeight(double fh) {footheight = fh;}
	void SetParamMaxFootLength(double maxfl) {MaxFootLength = maxfl;}
	void SetParamMinFootLength(double minfl) {MinFootLength = minfl;} 
	void SetParamFootSize(double fs) {FootSize = fs;}
};

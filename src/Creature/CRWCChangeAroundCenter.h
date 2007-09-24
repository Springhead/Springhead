#pragma once
#include <Springhead.h>		//	Springheadのインタフェース
#include <Physics/PHScene.h>

using namespace Spr;

class CRWCChangeAroundCenter
{
	
	std::vector<PHSolidIf*> WholeBody;   //体全体

	Vec3d PositionOfWholeBody;
	Vec3d VelocityOfWholeBody;
	Vec3d PositionOfUpperBody;

    Vec3d change;
	Vec3d AMAC;
	Vec3d RoDiff;

	Vec3d dvec;
	Vec3d cvec;

	double CurrentDirection;

	double kpx;
	double kvx;
	double kpz;
	double kvz;
	double Maxkpx;
	double Maxkpz;
	double Maxkvx;
	double Maxkvz;

	void CalcAngularMomentum(void);
    void CalcRotationalDifference(void);
	
public:
	CRWCChangeAroundCenter(std::vector<PHSolidIf*> wb);
	~CRWCChangeAroundCenter(void);

	Vec3d CalcChangeAroundCenter(void);
	void Init(void);
	void UpdateState(Vec3d pw, Vec3d vw, Vec3d pu, double cd){
		PositionOfWholeBody = pw;
		VelocityOfWholeBody = vw;
        PositionOfUpperBody = pu;
		CurrentDirection = cd;
	} 
	void SetWholeBody(std::vector<PHSolidIf*> wb) {WholeBody = wb;}
	void SetDvec(Vec3d ve) {dvec = ve.unit();}
	Vec3d GetCvec(void) {return cvec;}
	void SetParams(double px, double vx, double pz, double vz){
		kpx = px;
		kvx = vx;
		kpz = pz;
		kvz = vz;
	}

};
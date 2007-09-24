#include ".\crwcchangearoundcenter.h"

CRWCChangeAroundCenter::CRWCChangeAroundCenter(std::vector<PHSolidIf*> wb)
{
	WholeBody = wb;
}

CRWCChangeAroundCenter::~CRWCChangeAroundCenter(void)
{
}

void CRWCChangeAroundCenter::Init(){
	dvec = Vec3d(0.00,1.0,0.0);

	kpx = 30.0;
	kvx = 400.0;
	kpz = 30.0;  //kpz = 40.0;
	kvz = 400.0;   //kvz = 2.4;
	Maxkpx = 40.0;
	Maxkpz = 60.0;
	Maxkvx = 40.0;
	Maxkvz = 60.0;
}

//重心周りの、上体の変化量と角運動量を考慮したバランスに関する指標
Vec3d CRWCChangeAroundCenter::CalcChangeAroundCenter(void){

    CalcAngularMomentum();
    CalcRotationalDifference();

	double LocalChangeX;
	double LocalChangeZ;
	double lkpx, lkpz;
	double lkvx, lkvz;

	//DSTR << "RoDiff = " << RoDiff << " AMAC = " << AMAC << std::endl;
	
	lkpx = abs(RoDiff.x) * kpx;
	lkpz = abs(RoDiff.z) * kpz;
	lkvx = abs(AMAC.x) * kvx;
	lkvz = abs(AMAC.z) * kvz;

	if(lkpx > Maxkpx) lkpx = Maxkpx;
	if(lkpz > Maxkpz) lkpz = Maxkpz;
	if(lkvx > Maxkvx) lkvx = Maxkvx;
	if(lkvz > Maxkvz) lkvz = Maxkvz;
	lkpx = kpx;
	lkpz = kpz;
	lkvx = kvx;
	lkvz = kvz;

	//DSTR << "lkpx = " << lkpx << " lkpz = " << lkpz << " lkvx = " << lkvx << " lkvz = " << lkvz << std::endl;

	//RoDiff = Vec3d(0,0,0);
	//CurrentDirection = 0.0;////////////////////////////////////
	LocalChangeX = cos(CurrentDirection)*((lkpx * RoDiff - lkvx * AMAC).x) - sin(CurrentDirection)*((lkpz * RoDiff - lkvz * AMAC).z);
	LocalChangeZ = cos(CurrentDirection)*((lkpz * RoDiff - lkvz * AMAC).z) + sin(CurrentDirection)*((lkpx * RoDiff - lkvx * AMAC).x);

	//DSTR << "RoDiff = " << cos(CurrentDirection)*lkpz * RoDiff.z + cos(CurrentDirection)*lkpz * RoDiff.z << std::endl;
	DSTR << "AMAC = " << cos(CurrentDirection)*lkvz * AMAC.z + sin(CurrentDirection)*lkvx * AMAC.x << std::endl;
	DSTR << "LocalChangeZ = " << LocalChangeZ << std::endl;
	change = Vec3d(LocalChangeX, 0.0 , LocalChangeZ);

	return change;
}


//centerpoint周りのobjsに含まれる剛体の角運動量の和
void CRWCChangeAroundCenter::CalcAngularMomentum(void){

    Vec3d AM = Vec3d(0.0, 0.0, 0.0);

	for(int i = 0;i < (int) WholeBody.size();i++){
		AM = AM + cross((WholeBody[i]->GetCenterPosition() - PositionOfWholeBody), WholeBody[i]->GetMass() * (WholeBody[i]->GetVelocity() - VelocityOfWholeBody)); 
	}

	//AM = cross(WholeBody[4]->GetCenterPosition() - PositionOfWholeBody, (WholeBody[4]->GetVelocity() - VelocityOfWholeBody));

	AM = Vec3d(0,0,0); 
	AMAC = AM; 
	//DSTR << "AMAC = " << AMAC << std::endl;
	
	//DSTR << "re V " << WholeBody[4]->GetVelocity() - VelocityOfWholeBody << std::endl;
	//DSTR << "VelocityOfWholeBody = " << VelocityOfWholeBody << " PositionOfWholeBody = " << PositionOfWholeBody << std::endl;
};

//dvecとcvecの姿勢の差の回転ベクトル
void CRWCChangeAroundCenter::CalcRotationalDifference(void){

	cvec = (PositionOfUpperBody - PositionOfWholeBody).unit();

    Quaterniond qua;
    qua.RotationArc(cvec, dvec);

	RoDiff = qua.Rotation();
}
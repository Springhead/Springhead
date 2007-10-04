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

	kpx = 300.0;
	kvx = 30000.0;
	kpz = 300.0;  //kpz = 40.0;
	kvz = 30000.0;   //kvz = 2.4;
	Maxkpx = 50.0;
	Maxkpz = 80.0;
	Maxkvx = 50.0;
	Maxkvz = 80.0;

}

//重心周りの、上体の変化量と角運動量を考慮したバランスに関する指標
Vec3d CRWCChangeAroundCenter::CalcChangeAroundCenter(void){

    CalcAngularMomentum();
    CalcRotationalDifference();

	double LocalChangeX;
	double LocalChangeZ;
	double lkpx, lkpz;
	double lkvx, lkvz;


	lkpx = abs(RoDiff.x) * kpx;
	lkpz = abs(RoDiff.z) * kpz;
	lkvx = abs(AMAC.x) * kvx;
	lkvz = abs(AMAC.z) * kvz;

	//DSTR << "AMAC.z = " << AMAC.z << " RoDiff.z = " << RoDiff.z << std::endl;
	if(lkpx > Maxkpx) lkpx = Maxkpx;
	if(lkpz > Maxkpz) lkpz = Maxkpz;
	if(lkvx > Maxkvx) lkvx = Maxkvx;
	if(lkvz > Maxkvz) lkvz = Maxkvz;

	
	lkpx = 50.0;
	lkpz = 50.0;
	lkvx = 130.0;
	lkvz = 130.0;

	//DSTR << "lkpx = " << lkpx << " lkpz = " << lkpz << " lkvx = " << lkvx << " lkvz = " << lkvz << std::endl;

	LocalChangeX = cos(CurrentDirection)*((lkpx * RoDiff - lkvx * AMAC).x) - sin(CurrentDirection)*((lkpz * RoDiff - lkvz * AMAC).z);
	LocalChangeZ = cos(CurrentDirection)*((lkpz * RoDiff - lkvz * AMAC).z) + sin(CurrentDirection)*((lkpx * RoDiff - lkvx * AMAC).x);

	change = Vec3d(LocalChangeX, 0.0 , LocalChangeZ);

	return change;
}


//centerpoint周りのobjsに含まれる剛体の角運動量の和
void CRWCChangeAroundCenter::CalcAngularMomentum(void){

    Vec3d AM = Vec3d(0.0, 0.0, 0.0);

	for(int i = 0;i < (int) WholeBody.size();i++){
		AM = AM + cross((WholeBody[i]->GetCenterPosition() - PositionOfWholeBody), WholeBody[i]->GetMass() * (WholeBody[i]->GetVelocity() - VelocityOfWholeBody)); 
	}

	AMAC = AM; 
};

//dvecとcvecの姿勢の差の回転ベクトル
void CRWCChangeAroundCenter::CalcRotationalDifference(void){

	cvec = (PositionOfUpperBody - PositionOfWholeBody).unit();

    Quaterniond qua;
    qua.RotationArc(cvec, dvec);

	RoDiff = qua.Rotation();
}
#include "HumanInterface.h"
#pragma hdrstop
#include "HISpidar4D.h"

namespace Spr {
/*
//	モータの取り付け位置
//	モータが直方体に取り付けられている場合
#define PX	0.265f	//	x方向の辺の長さ/2
#define PY	0.265f	//	y方向の辺の長さ/2
#define PZ	0.265f	//	z方向の辺の長さ/2
Vec3f HISpidar4::motorPosDef[][4] =	//	モータの取り付け位置(中心を原点とする)
	{
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
*/
//----------------------------------------------------------------------------

HISpidar4D::HISpidar4D(){
    for (int i=0;i<4;i++){
        tension[i] = 0;
    }
}
HISpidar4D::~HISpidar4D(){SetMinForce();}

bool HISpidar4D::Init(const void* pDesc){
	HISdkIf* sdk = GetSdk();
	HISpidar4Desc& desc = *(HISpidar4Desc*)pDesc;
	if (desc.motors.size() != 4) return false;

	for(unsigned i=0; i<desc.motors.size(); ++i){
		motor[i].SetDesc(&desc.motors[i]);
	}
	////	ドライバの取得
	size_t i;
	char* name=NULL;
	Vec4i port=desc.port+Vec4i(-1,-1,-1,-1);

	for(i=0; i<desc.motors.size(); ++i){
		if(desc.port==Vec4i(0,0,0,0)){
			motor[i].da = sdk->RentVirtualDevice(DVDaBase::TypeS())->Cast();
		}else {
			motor[i].da = sdk->RentVirtualDeviceNo(DVDaBase::TypeS(),port[i])->Cast();
		}
		if (!motor[i].da) break;
		AddDeviceDependency(motor[i].da->RealDevice()->Cast());
	}

	for(i=0; i<desc.motors.size(); ++i){
		if(desc.port==Vec4i(0,0,0,0)){
			motor[i].counter = sdk->RentVirtualDevice(DVCounterBase::TypeS())->Cast();
		}else {
			motor[i].counter = sdk->RentVirtualDeviceNo(DVCounterBase::TypeS(),port[i])->Cast();
		}
		if (!motor[i].counter) break;
		AddDeviceDependency(motor[i].counter->RealDevice()->Cast());
	}

	SetMinForce();
	BeforeCalibration();
	Calibration();
	AfterCalibration();
	return true;
}

bool HISpidar4D::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	// calibration sets the center of the reference frame at the current
	// position of the spidar (motor[i].pos.norm())
	Update(0.001f);
	for(int i=0; i<4; i++) motor[i].SetLength(motor[i].pos.norm());
	return true;
}
void HISpidar4D::Update(float dt){
	HIForceInterface3D::Update(dt);	
	float len[4];
	for(int i=0; i<4; i++) len[i]=motor[i].GetLength();
//	TRACE("len = %1.3f, %1.3f, %1.3f, %1.3f\n", len[0], len[1], len[2], len[3]);
	// test debug
	/*
	for(int i=0;i<4;i++){
		printf("%d",len[i]);
	}
	*/
	//printf("\n");
	// test debug

	pos = matPos * (
		Vec3f(	Square(len[0])-Square(len[1]),
		Square(len[1])-Square(len[2]),
		Square(len[2])-Square(len[3])	) + posSqrConst);
}

/*	2次計画法による張力計算	*/
void HISpidar4D::SetForce(const Vec3f& v3force, float eff, float cont){
	//	糸の方向ベクトルを求める。
    for (int i=0; i<4; i++){
        phi[i] = (motor[i].pos-pos).unit();		//	張力の単位ベクトル		tension direction
	}
	/*	目的関数
		  |f-φt|^2 + cont*|t-t'|^2 + eff*t^2
		= (φ^2+eff+cont)*t^2 + (-2*φ*f -2*cont*t')*t + f^2+cont*t'^2
		を最小にする張力tを求める．	*/
    TQuadProgram<float, 4> qp;
    //	目的関数の２次係数行列
    for(int i=0;i<4;i++) qp.matQ[i][i]=phi[i]*phi[i]+eff+cont;
    qp.matQ[0][1]=qp.matQ[1][0]=phi[0]*phi[1];
    qp.matQ[0][2]=qp.matQ[2][0]=phi[0]*phi[2];
    qp.matQ[0][3]=qp.matQ[3][0]=phi[0]*phi[3];
    qp.matQ[1][2]=qp.matQ[2][1]=phi[1]*phi[2];
    qp.matQ[1][3]=qp.matQ[3][1]=phi[1]*phi[3];
    qp.matQ[2][3]=qp.matQ[3][2]=phi[2]*phi[3];

    //	目的関数の１次係数ベクトル
    for(int i=0;i<4;i++) qp.vecC[i]= phi[i]*v3force + cont*tension[i];
	//	最小張力・最大張力の設定
	Vec4f minF, maxF;
	minF.clear(1); maxF.clear(20);
	qp.Init(minF, maxF);
	qp.Solve();
    for(int i=0;i<4;i++) {
        tension[i]=qp.vecX[i];							//張力＝x[]
    }
    for (int i=0;i<4;i++){
        motor[i].SetForce(tension[i]);
    }
}
Vec3f HISpidar4D::GetForce(){
    int i;
	Vec3f f;
    for (i=0;i<4;i++) f=f+tension[i]*phi[i];
    return f;
}

void HISpidar4D::SetMinForce(){
	for(int i=0; i<4; i++) motor[i].SetForce(motor[i].minForce);
}
void HISpidar4D::InitMat(){
	matPos = Matrix3f(
		motor[1].pos-motor[0].pos,
		motor[2].pos-motor[1].pos,
		motor[3].pos-motor[2].pos).trans() * 2;
	matPos = matPos.inv();

	posSqrConst = Vec3f(motor[1].pos.square()-motor[0].pos.square(),
		motor[2].pos.square()-motor[1].pos.square(),
		motor[3].pos.square()-motor[2].pos.square());
}

}	//	namespace Spr

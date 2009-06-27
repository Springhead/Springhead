#include "HumanInterface.h"
#ifdef _WIN32
 #pragma hdrstop
#endif
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

//HIOBJECTIMP(HISpidar4D, HIForceDevice3D);
HISpidar4DDesc::HISpidar4DDesc(){


}

void HISpidar4DDesc::Init(int nMotor, Vec3f* motorPos, Vec3f* knotPos, float vpn, float lpp, float minF, float maxF){
	motors.resize(nMotor);
	for(int i=0; i<nMotor; ++i){
		motors[i].pos = motorPos[i];
		motors[i].jointPos = knotPos[i];
		motors[i].lengthPerPulse = lpp;
		motors[i].voltPerNewton = vpn;		//	volt per newton
		motors[i].currentPerVolt = 1;		//	抵抗値の逆数
		motors[i].minForce = minF;
		motors[i].maxForce = maxF;
	}
	VVector<float> minForce, maxForce;
	minForce.resize(nMotor);
	maxForce.resize(nMotor);
	for(int i=0; i<nMotor; ++i){
		minForce[i] = minF;
		maxForce[i] = maxF;
	}
}
void HISpidar4DDesc::Init(char* type){
	if (stricmp(type, "SpidarG6X3L")==0 || stricmp(type, "SpidarG6X3R")==0){
		bool bLeft = stricmp(type, "SpidarG6X3L")==0;
		//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
		const float PX = 0.12f/2;		//	x方向の辺の長さ/2
		const float PY = 0.14f/2;		//	y方向の辺の長さ/2
		const float PZ = 0.12f/2;		//	z方向の辺の長さ/2
		//	糸のグリップへの取り付け位置．グリップはピンポン玉を採用しています．
		const float GX = 0.045f/2;		//	x方向の辺の長さ/2
		const float GY = 0.045f/2;		//	y方向の辺の長さ/2
		Matrix3f rotR = Matrix3f::Rot((float)Rad(-45), 'y');
		Matrix3f rotL = Matrix3f::Rot((float)Rad(-45), 'y');

		Vec3f motorPos[2][8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
			{
				{rotR*Vec3f(-PX,-PY, PZ), rotR*Vec3f( -GX, 0.0f, 0.0f)},
				{rotR*Vec3f( PX,-PY, PZ), rotR*Vec3f(  GX, 0.0f, 0.0f)},
				{rotR*Vec3f( PX,-PY,-PZ), rotR*Vec3f(0.0f,  -GY, 0.0f)},
				{rotR*Vec3f(-PX,-PY,-PZ), rotR*Vec3f(0.0f,  -GY, 0.0f)},
				{rotR*Vec3f(-PX, PY, PZ), rotR*Vec3f( -GX, 0.0f, 0.0f)},
				{rotR*Vec3f( PX, PY, PZ), rotR*Vec3f(  GX, 0.0f, 0.0f)},
				{rotR*Vec3f( PX, PY,-PZ), rotR*Vec3f(0.0f,   GY, 0.0f)},
				{rotR*Vec3f(-PX, PY,-PZ), rotR*Vec3f(0.0f,   GY, 0.0f)}
			},
			{
				{rotL*Vec3f( PX, PY,-PZ), rotL*Vec3f(0, 0.0f,  -GX)},
				{rotL*Vec3f( PX, PY, PZ), rotL*Vec3f(0, 0.0f,   GX)},
				{rotL*Vec3f(-PX, PY, PZ), rotL*Vec3f(0,   GY, 0.0f)},
				{rotL*Vec3f(-PX, PY,-PZ), rotL*Vec3f(0,   GY, 0.0f)},
				{rotL*Vec3f( PX,-PY,-PZ), rotL*Vec3f(0, 0.0f,  -GX)},
				{rotL*Vec3f( PX,-PY, PZ), rotL*Vec3f(0, 0.0f,   GX)},
				{rotL*Vec3f(-PX,-PY, PZ), rotL*Vec3f(0,  -GY, 0.0f)},
				{rotL*Vec3f(-PX,-PY,-PZ), rotL*Vec3f(0,  -GY, 0.0f)},
			}
		};
		Vec3f mp[8];
		Vec3f kp[8];
		if (bLeft){
			for(int i=0; i<8; ++i){
				mp[i] = motorPos[1][i][0];
				kp[i] = motorPos[1][i][1];
			}
		}else{
			for(int i=0; i<8; ++i){
				mp[i] = motorPos[0][i][0];
				kp[i] = motorPos[0][i][1];
			}
		}
		Init(8, mp, kp, 0.365296803653f, 1.66555e-5f, 0.3f, 20.0f);
		if (bLeft){
			motors[2].lengthPerPulse *= -1;
			motors[3].lengthPerPulse *= -1;
			motors[4].lengthPerPulse *= -1;
			motors[5].lengthPerPulse *= -1;

		}else{
			motors[2].lengthPerPulse *= -1;
			motors[3].lengthPerPulse *= -1;
			motors[4].lengthPerPulse *= -1;
			motors[5].lengthPerPulse *= -1;
		}
	}
}

//---------------------------------------------------------------------------------------------------
HISpidar4D::HISpidar4D(){
    for (int i=0;i<4;i++){
        tension[i] = 0;
    }
}
HISpidar4D::~HISpidar4D(){SetMinForce();}

// links the HD to VD and RD
bool HISpidar4D::Init(const void* pDesc){
	HISdkIf* sdk = GetSdk();
	HISpidar4DDesc& desc = *(HISpidar4DDesc*)pDesc;
	//	計算のための定数の設定
	nRepeat = 2;
	sigma = 0.001f; //sigma=sigma*sigma
	int nMotor = desc.motors.size();
	motors.resize(nMotor);
	VVector<float> minForce, maxForce;
	minForce.resize(desc.motors.size());
	maxForce.resize(desc.motors.size());
	for(unsigned i=0; i<desc.motors.size(); ++i){
		motors[i].SetDesc(&desc.motors[i]);
		minForce[i] = desc.motors[i].minForce;
		maxForce[i] = desc.motors[i].maxForce;
	}
	HISpidarCalc3Dof::Init(3, minForce, maxForce);
	////	ドライバの取得
	int i;
	for(i=0; i<4; ++i){
		motors[i].da = sdk->RentVirtualDevice(DVDaBase::TypeS())->Cast();
		if (!motors[i].da) break;
		AddDeviceDependency(motors[i].da->RealDevice()->Cast());
	}
	if (i<4) return false;
	for(i=0; i<4; ++i){
		motors[i].counter = sdk->RentVirtualDevice(DVCounterBase::TypeS())->Cast();
		if (!motors[i].counter) break;
		AddDeviceDependency(motors[i].counter->RealDevice()->Cast());
	}
	if (i<4) return false;
	InitMat();
	BeforeCalibration();
	Calibration();
	AfterCalibration();
	bGood = true;
	return true;
}

bool HISpidar4D::Calibration(){
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

void HISpidar4D::MakeWireVec(){
	for(unsigned int i=0; i<motors.size(); ++i){
		//wireDirection[i] = motors[i].pos - (ori*motors[i].jointPos + pos);
		calculatedLength[i] = wireDirection[i].norm();
		wireDirection[i] /= calculatedLength[i];
	}
}
void HISpidar4D::UpdatePos(){
}
void HISpidar4D::MeasureWire(){
	for(unsigned int i=0; i<motors.size(); ++i){
		measuredLength[i] = motors[i].GetLength();
	}	
}

}	//	namespace Spr

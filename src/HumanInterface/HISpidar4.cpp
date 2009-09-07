#include "HumanInterface.h"
#ifdef _WIN32
 #pragma hdrstop
#endif
#include "HISpidar4.h"

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


//HISpidar4Desc--------------------------------------------------------------
HISpidar4Desc::HISpidar4Desc(){


}
void HISpidar4Desc::Init(int nMotor, Vec3f* motorPos, Vec3f* knotPos, float vpn, float lpp, float minF, float maxF){
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

void HISpidar4Desc::InitSpidarG(char* type){
		if (stricmp(type, "SpidarG6X3L")==0 || stricmp(type, "SpidarG6X3R")==0){

		bool bLeft = stricmp(type, "SpidarG6X3L")==0;
		//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
		const float PX = 0.12f/2;		//	x方向の辺の長さ/2
		const float PY = 0.14f/2;		//	y方向の辺の長さ/2
		const float PZ = 0.12f/2;		//	z方向の辺の長さ/2
		//	糸のグリップへの取り付け位置．グリップはピンポン玉を採用しています．
		const float GX = 0.02f/2;		//	x方向の辺の長さ/2
		const float GY = 0.02f/2;		//	y方向の辺の長さ/2
		Matrix3f rotR = Matrix3f::Rot((float)Rad(-90), 'y');
		Matrix3f rotL = Matrix3f::Rot((float)Rad(0), 'y');
		Matrix3f rotZ = Matrix3f::Rot((float)Rad(-45), 'z');
		Matrix3f tMat = Matrix3f(); tMat.xx*=-1; tMat.yy*=-1; //左手系を右手系に変換

	Vec3f motorPos[2][4][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{
			{rotR*Vec3f(-PX,-PY, PZ), rotR*rotZ*Vec3f(0.0f, -GY, 0.0f)},//1下左
			{rotR*Vec3f( PX,-PY,-PZ), rotR*rotZ*Vec3f( GX, 0.0f, 0.0f)},//3下右
			{rotR*Vec3f( PX, PY, PZ), rotR*rotZ*Vec3f(0.0f,  GY, 0.0f)},//6上奥
			{rotR*Vec3f(-PX, PY,-PZ), rotR*rotZ*Vec3f(-GX, 0.0f, 0.0f)}//8上前
		},
		{
			{tMat*rotL*Vec3f( PX, PY, PZ), tMat*rotL*rotZ*Vec3f( GX, 0.0f, 0.0f)},//2下奥
			{tMat*rotL*Vec3f(-PX, PY,-PZ), tMat*rotL*rotZ*Vec3f(0.0f, -GY, 0.0f)},//4下前
			{tMat*rotL*Vec3f( PX,-PY,-PZ), tMat*rotL*rotZ*Vec3f(-GX, 0.0f, 0.0f)},//5上左
			{tMat*rotL*Vec3f(-PX,-PY, PZ), tMat*rotL*rotZ*Vec3f(0.0f,  GY, 0.0f)},//7上右

		}
	};


	Vec3f mp[4];
	Vec3f kp[4];
	if (bLeft){
		for(int i=0; i<4; ++i){
			mp[i] = motorPos[1][i][0];
			kp[i] = motorPos[1][i][1];
		}
	}else{
		for(int i=0; i<4; ++i){
			mp[i] = motorPos[0][i][0];
			kp[i] = motorPos[0][i][1];
		}
	}
	Init(4, mp, kp, 0.365296803653f, 1.66555e-5f, 0.3f, 20.0f);
	if (bLeft){
		motors[1].lengthPerPulse *= -1;
		motors[2].lengthPerPulse *= -1;

	}else{
		motors[1].lengthPerPulse *= -1;
		motors[2].lengthPerPulse *= -1;
	}
	}
}

void HISpidar4Desc::InitSpidarBig(char* type){
	if (stricmp(type, "SpidarL")==0 || stricmp(type, "SpidarR")==0){
		bool bLeft = stricmp(type, "SpidarL")==0;
		//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
		const float PX = 0.625f/2;		//	x方向の辺の長さ/2
		const float PY = 0.54f/2;		//	y方向の辺の長さ/2
		const float PZ = 0.55f/2;		//	z方向の辺の長さ/2
		//	糸のグリップへの取り付け位置．グリップはピンポン玉を採用しています．
		const float GX = 0.02f/2;		//	x方向の辺の長さ/2
		const float GY = 0.02f/2;		//	y方向の辺の長さ/2

		Matrix3f rotZ = Matrix3f::Rot((float)Rad(45), 'z');
		Matrix3f tMatR = Matrix3f(); tMatR.zz*=-1; //座標調整
		Matrix3f tMatL = Matrix3f(); tMatL.xx*=-1; //座標調整

	Vec3f motorPos[2][4][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{
			{tMatR*Vec3f( PX,  PY,-PZ), rotZ*tMatR*Vec3f(0.0f,  GY, 0.0f)},//0上右前
			{tMatR*Vec3f(-PX,  PY, PZ), rotZ*tMatR*Vec3f(-GX,  0.0f, 0.0f)},//1上左奥
			{tMatR*Vec3f( PX, -PY, PZ), rotZ*tMatR*Vec3f( GX, 0.0f, 0.0f)},//2下右奥
			{tMatR*Vec3f(-PX, -PY,-PZ), rotZ*tMatR*Vec3f(0.0f, -GY, 0.0f)},//3下左前
		},
		{
			{tMatL*Vec3f( PX, PY, PZ),  rotZ*tMatL*Vec3f( -GX, 0.0f, 0.0f)},//4上左前
			{tMatL*Vec3f(-PX, PY,-PZ),  rotZ*tMatL*Vec3f( 0.0f, GY, 0.0f)},//5上右奥
			{tMatL*Vec3f( PX,-PY,-PZ),  rotZ*tMatL*Vec3f( 0.0f, -GY, 0.0f)},//6下右前
			{tMatL*Vec3f(-PX,-PY, PZ),  rotZ*tMatL*Vec3f( GX, 0.0f, 0.0f)},//7下左奥
		}
	};


	Vec3f mp[4];
	Vec3f kp[4];
	if (bLeft){
		for(int i=0; i<4; ++i){
			mp[i] = motorPos[1][i][0];
			kp[i] = motorPos[1][i][1];
		}
	}else{
		for(int i=0; i<4; ++i){
			mp[i] = motorPos[0][i][0];
			kp[i] = motorPos[0][i][1];
		}
	}
	Init(4, mp, kp, 0.365296803653f, 1.66555e-5f*1.667, 0.3f, 15.0f);

	}
	
}
void HISpidar4Desc::Init(char* type){
	InitSpidarG(type);
	InitSpidarBig(type);
}


//HISpidar4D--------------------------------------------------------------
HISpidar4::HISpidar4(){
    for (int i=0;i<4;i++){
        tension[i] = 0;
    }
}
HISpidar4::~HISpidar4(){SetMinForce();}


bool HISpidar4::Init(const void* pDesc){
	HISdkIf* sdk = GetSdk();
	HISpidar4Desc& desc = *(HISpidar4Desc*)pDesc;
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
	size_t i;
	char* name=NULL;
	Vec4i port=desc.port+Vec4i(-1,-1,-1,-1);

	for(i=0; i<motors.size(); ++i){
		if(desc.port==Vec4i(0,0,0,0)){
			motors[i].da = sdk->RentVirtualDevice(DVDaBase::TypeS())->Cast();
		}else {
			motors[i].da = sdk->RentVirtualDeviceNo(DVDaBase::TypeS(),port[i])->Cast();
		}
		if (!motors[i].da) break;
		AddDeviceDependency(motors[i].da->RealDevice()->Cast());
	}
	if (i<motors.size()) return false;

	for(i=0; i<motors.size(); ++i){
		if(desc.port==Vec4i(0,0,0,0)){
			motors[i].counter = sdk->RentVirtualDevice(DVCounterBase::TypeS())->Cast();
		}else {
			motors[i].counter = sdk->RentVirtualDeviceNo(DVCounterBase::TypeS(),port[i])->Cast();
		}
		if (!motors[i].counter) break;
		AddDeviceDependency(motors[i].counter->RealDevice()->Cast());
	}
	if (i<motors.size()) return false;

	SetMinForce();
	BeforeCalibration();
	Calibration();
	AfterCalibration();
	bGood = true;
	return true;
}

bool HISpidar4::Calibration(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	for(unsigned i=0; i<motors.size(); i++) motors[i].SetLength( (motors[i].pos - motors[i].jointPos).norm() );
	lengthDiffAve.clear();
	for(size_t i=0; i<motors.size(); ++i) HISpidarCalc3Dof::Update();	//	姿勢を更新
	return true;
}
void HISpidar4::Update(float dt){
	HIForceInterface3D::Update(dt);
	HISpidarCalc3Dof::Update();
	for(unsigned int i=0; i<motors.size(); ++i){
		motors[i].SetForce(Tension()[i]);
	}
}

Vec3f HISpidar4::GetForce(){
    size_t i;
	Vec3f f;
    for (i=0;i<motors.size();i++) f=f+tension[i]*phi[i];
    return f;
}

void HISpidar4::SetMinForce(){
	for(int i=0; i< (int)motors.size(); i++) motor[i].SetForce(motor[i].minForce);
}

void HISpidar4::MakeWireVec(){
	for(unsigned int i=0; i<motors.size(); ++i){
		wireDirection[i] = motors[i].pos - pos;
		calculatedLength[i] = wireDirection[i].norm();
		wireDirection[i] /= calculatedLength[i];
	}
}
void HISpidar4::UpdatePos(){
}
void HISpidar4::MeasureWire(){
	for(unsigned int i=0; i<motors.size(); ++i){
		measuredLength[i] = motors[i].GetLength();
	}	
}

}	//	namespace Spr

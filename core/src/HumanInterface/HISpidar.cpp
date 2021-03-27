/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HISpidar.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

#ifdef _WIN32
#else
# define stricmp strcasecmp
#endif

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

namespace Spr {;

bool HISpidar::Init(const void* pDesc){
	HISdkIf* sdk = GetSdk();
	const HISpidarDesc& desc = *(const HISpidarDesc*)pDesc;
	size_t nMotor = desc.motors.size();
	motors.resize(nMotor);
	
	size_t i;
	for(i = 0; i < motors.size(); ++i){
		motors[i].SetDesc(&desc.motors[i]);
		motors[i].da	  = DCAST( DVDaIf,		sdk->RentVirtualDevice(DVDaIf::GetIfInfoStatic(),	   "", motors[i].ch) );
		motors[i].counter = DCAST( DVCounterIf, sdk->RentVirtualDevice(DVCounterIf::GetIfInfoStatic(), "", motors[i].ch) );
		if (!motors[i].da || !motors[i].counter)
			break;
		AddDeviceDependency(motors[i].da->GetRealDevice()->Cast());
		AddDeviceDependency(motors[i].counter->GetRealDevice()->Cast());
	}
	if(i < motors.size())
		return false;
	return true;
}
	
void HISpidar::SetMinForce(){
	for(int i = 0; i < (int)motors.size(); i++)
		motors[i].SetForce(motors[i].minForce);
	Update(0.001f);
}

void HISpidar::SetLimitMinForce(float f){
	for(int i = 0; i < (int)motors.size(); i++)
		motors[i].SetLimitMinForce(f);
}

void HISpidar::SetLimitMaxForce(float f){
	for(int i = 0; i < (int)motors.size(); i++)
		motors[i].SetLimitMaxForce(f);
}
//----------------------------------------------------------------------------
// HISpidar4Desc

HISpidar4Desc::HISpidar4Desc(){
	InitSpidarG("SpidarG6X3L");
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
		Init(4, mp, kp, 0.365296803653f, 1.66555e-5f*1.667f, 0.25f, 10.0f);
	}
	
}

void HISpidar4Desc::Init(char* type){
	InitSpidarG(type);
	InitSpidarBig(type);
}

//----------------------------------------------------------------------------
// HISpidar4D

HISpidar4::HISpidar4(const HISpidar4Desc& desc){}	//	desc is null pointer here
HISpidar4::~HISpidar4(){}

bool HISpidar4::Init(const void* pDesc){
	if(!HISpidar::Init(pDesc))
		return false;

	const HISpidar4Desc& desc = *(const HISpidar4Desc*)pDesc;
	
	// HISpidarCalc3Dofの初期化
	nRepeat = 2;		
	sigma	= 0.001f;		//sigma=sigma*sigma
	VVector<float> minForce, maxForce;
	minForce.resize(desc.motors.size());
	maxForce.resize(desc.motors.size());
	for(unsigned i = 0; i < desc.motors.size(); ++i){
		minForce[i] = desc.motors[i].minForce;
		maxForce[i] = desc.motors[i].maxForce;
	}
	HISpidarCalc3Dof::Init(3, minForce, maxForce);
	
	SetMinForce();
	BeforeCalibration();
	Calibration();
	AfterCalibration();
	bGood = true;
	return true;
}

bool HISpidar4::Calibration(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	for(unsigned i=0; i<motors.size(); i++)
		motors[i].SetLength( (motors[i].pos - motors[i].jointPos).norm() );

	lengthDiffAve.clear();

	// 姿勢を更新
	for(size_t i=0; i<motors.size(); ++i)
		HISpidarCalc3Dof::Update();	

	return true;
}

void HISpidar4::Update(float dt){
	HIHaptic::Update(dt);
	HISpidarCalc3Dof::Update();
	for(unsigned int i = 0; i < motors.size(); ++i){
		motors[i].SetForce(Tension()[i]);
	}
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

//----------------------------------------------------------------------------
// HISpidar4D

HISpidar4D::HISpidar4D(const HISpidar4DDesc& desc){
    for (int i=0;i<4;i++){
        tension[i] = 0;
    }
}

bool HISpidar4D::Calibration(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	// calibration sets the center of the reference frame at the current
	// position of the spidar (motor[i].pos.norm())
	Update(0.001f);
	for(int i=0; i<4; i++)
		motors[i].SetLength(motors[i].pos.norm());
	return true;
}

void HISpidar4D::InitMat(){
	matPos = Matrix3f(
		motors[1].pos - motors[0].pos,
		motors[2].pos - motors[1].pos,
		motors[3].pos - motors[2].pos).trans() * 2;
	matPos = matPos.inv();

	posSqrConst = Vec3f(
		motors[1].pos.square() - motors[0].pos.square(),
		motors[2].pos.square() - motors[1].pos.square(),
		motors[3].pos.square() - motors[2].pos.square());
}

Vec3f HISpidar4D::GetForce(){
	Vec3f f;
	for (size_t i = 0; i < motors.size(); i++)
		f += tension[i] * phi[i];
	return f;
}

void HISpidar4D::SetForce(const Vec3f& v3force){
	/*	2次計画法による張力計算	*/
	//	糸の方向ベクトルを求める。
	for (int i = 0; i < 4; i++){
		phi[i] = (motors[i].pos - pos).unit();		//	張力の単位ベクトル		tension direction
	}

	/*	目的関数
			|f-φt|^2 + cont*|t-t'|^2 + eff*t^2
		= (φ^2+eff+cont)*t^2 + (-2*φ*f -2*cont*t')*t + f^2+cont*t'^2
		を最小にする張力tを求める．	*/
	TQuadProgram<float, 4> qp;
	//	目的関数の２次係数行列
	for(int i = 0; i < 4; i++)
		qp.matQ[i][i] = phi[i]*phi[i] + eff + cont;
	qp.matQ[0][1] = qp.matQ[1][0] = phi[0]*phi[1];
	qp.matQ[0][2] = qp.matQ[2][0] = phi[0]*phi[2];
	qp.matQ[0][3] = qp.matQ[3][0] = phi[0]*phi[3];
	qp.matQ[1][2] = qp.matQ[2][1] = phi[1]*phi[2];
	qp.matQ[1][3] = qp.matQ[3][1] = phi[1]*phi[3];
	qp.matQ[2][3] = qp.matQ[3][2] = phi[2]*phi[3];

	//	目的関数の１次係数ベクトル
	for(int i = 0; i < 4; i++)
		qp.vecC[i] = phi[i]*v3force + cont*tension[i];
	//	最小張力・最大張力の設定
	Vec4f minF, maxF;
	minF.clear(1); maxF.clear(20);
	qp.Init(minF, maxF);
	qp.Solve();
	for(int i=0;i<4;i++) {
		tension[i]=qp.vecX[i];							//張力＝x[]
	}
}

void HISpidar4D::Update(float dt){
	HIHaptic::Update(dt);

	float len[4];
	for(int i = 0; i < 4; i++)
		len[i] = motors[i].GetLength();

	pos = matPos * (
		Vec3f(	Square(len[0]) - Square(len[1]),
				Square(len[1]) - Square(len[2]),
				Square(len[2]) - Square(len[3])	) + posSqrConst);

	for (int i=0;i<4;i++){
		motors[i].SetForce(tension[i]);
	}
}

//----------------------------------------------------------------------------
// HISpidarG

HISpidarGDesc::HISpidarGDesc(){
}

void HISpidarGDesc::Init(int nMotor, Vec3f* motorPos, Vec3f* knotPos, float vpn, float lpp, float minF, float maxF){
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

void HISpidarGDesc::Init(const char* type) {
	if (stricmp(type, "SpidarG6T1") == 0) {
		const double XINLEN = 155 / 1000.0;
		const double XOUTLEN = (XINLEN + 37 * 2) / 1000.0;
		const double ZINLEN = 102 / 1000.0;
		const double ZOUTLEN = (ZINLEN + 39 * 2) / 1000.0;

		nButton = 0;
		Vec3f mp[8] = {
			Vec3f(-XOUTLEN / 2,	-0.090,		0),
			Vec3f(-XINLEN / 2,	-0.090,		0),
			Vec3f(XINLEN / 2,		-0.090,		0),
			Vec3f(XOUTLEN / 2,	-0.090,		0),
			Vec3f(0,			0.07359,	-0.16209),
			Vec3f(0,			0.08290,	-0.13535),
			Vec3f(0,			0.10897,	-0.03943),
			Vec3f(0,			0.11912,	 0.00323),
		};
		Vec3f gp[4] = {
			Vec3f(0,		-0.0192,	0.023),
			Vec3f(0,		-0.0192,	-0.023),
			Vec3f(0.023,	0.0192,		0),
			Vec3f(-0.023,	0.0192,		0),
		};
		Vec3f gps[8] = { gp[0], gp[1], gp[0], gp[1],
			gp[2], gp[3], gp[2], gp[3] };
		Init(8, mp, gps, 0.04f, (float)(0.051645 / 1000.0), 0.3f, 20.0f);
		motors[2].lengthPerPulse *= -1;
		motors[3].lengthPerPulse *= -1;
		motors[4].lengthPerPulse *= -1;
		motors[5].lengthPerPulse *= -1;

		motors[0].voltPerNewton *= -1;
		motors[1].voltPerNewton *= -1;
		motors[6].voltPerNewton *= -1;
		motors[7].voltPerNewton *= -1;
	}
	else if (stricmp(type, "SpidarG6X4L") == 0 || stricmp(type, "SpidarG6X4R") == 0) {
		bool bLeft = stricmp(type, "SpidarG6X4L") == 0;
		//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
		float PX = 0.138f / 2;		//	x方向の辺の長さ/2
		float PY = 0.190f / 2;		//	y方向の辺の長さ/2
		float PZ = 0.138f / 2;		//	z方向の辺の長さ/2
		//	糸のグリップへの取り付け位置．グリップはピンポン玉を採用しています．
		float GX = 0.031f / 2;		//	
		float GY = 0.035f / 2;		//	
		float GZ = 0.025f / 2;      //	
		Matrix3f rot = Matrix3f();	//	Rot((float)Rad(bLeft ? -45 : 45), 'y');
		Vec3f motorPos[2][8] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
			{
				rot * Vec3f( PX, -PY, -PZ),
				rot * Vec3f( PX, -PY,  PZ),
				rot * Vec3f(-PX, -PY,  PZ),
				rot * Vec3f(-PX, -PY, -PZ),
				rot * Vec3f( PX,  PY, -PZ),
				rot * Vec3f( PX,  PY,  PZ),
				rot * Vec3f(-PX,  PY,  PZ),
				rot * Vec3f(-PX,  PY, -PZ),
			},
			{
				rot* Vec3f(0.0f,  -GY,  -GZ),
				rot* Vec3f(GX,   0.0f,   GZ),
				rot* Vec3f(-GX,  0.0f,   GZ),
				rot* Vec3f(0.0f,  -GY,  -GZ),
				rot* Vec3f(0.0f,   GY,  -GZ),
				rot* Vec3f(GX,   0.0f,   GZ),
				rot* Vec3f(-GX,  0.0f,   GZ),
				rot* Vec3f(0.0f,   GY,  -GZ),
			}
		};
		Init(8, motorPos[0], motorPos[1], 0.08f, (float)(0.03485 / 1000.0), 0.3f, 20.0f);
		motors[0].lengthPerPulse *= -1;
		motors[1].lengthPerPulse *= -1;

		motors[2].voltPerNewton *= -1;
		motors[3].voltPerNewton *= -1;

		motors[4].lengthPerPulse *= -1;
		motors[5].lengthPerPulse *= -1;

		motors[6].voltPerNewton *= -1;
		motors[7].voltPerNewton *= -1;

	} else if (stricmp(type, "SpidarG6X3L")==0 || stricmp(type, "SpidarG6X3R")==0 || stricmp(type, "SpidarG6X3F")==0 ){
		bool bLeft = stricmp(type, "SpidarG6X3L")==0;
		bool bFinger = stricmp(type, "SpidarG6X3F")==0;

		//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
		float PX = 0.12f/2;		//	x方向の辺の長さ/2
		float PY = 0.152f/2;//0.14f/2;		//	y方向の辺の長さ/2
		float PZ = 0.12f/2;		//	z方向の辺の長さ/2
		//	糸のグリップへの取り付け位置．グリップはピンポン玉を採用しています．
		float GX = 0.045f/2;		//	x方向の辺の長さ/2
		float GY = 0.045f/2;		//	y方向の辺の長さ/2
		float GZ = 0.0;              //Used only for Spidar Finger grip configuration

		Matrix3f rotR = Matrix3f::Rot((float)Rad(-45), 'y');
		Matrix3f rotL = Matrix3f::Rot((float)Rad(-45), 'y');

		if (bFinger) {  //configuration for Spider Finger Grip
			PX = 0.12f/2;		
			PY = 0.146f/2;		
			PZ = 0.12f/2;

			GX = 0.032f/2;
			GZ = 0.032f/2;

			rotR = Matrix3f::Rot((float)Rad(0), 'y');
			rotL = Matrix3f::Rot((float)Rad(45), 'y');
		}

		Vec3f motorPos[3][8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
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
				{rotL*Vec3f(-PX,-PY,-PZ), rotL*Vec3f(0,  -GY, 0.0f)}
			},
			{    //Initialization for Spidar Finger Grip
			    {rotR*Vec3f(-PX,-PY, PZ), rotR*Vec3f(-GX, 0.0f,  0.0f)}, //1
				{rotR*Vec3f( PX,-PY, PZ), rotR*Vec3f(0.0f, 0.0f,   GZ)}, //2
				{rotR*Vec3f( PX,-PY,-PZ), rotR*Vec3f(GX,   0.0f, 0.0f)}, //3
				{rotR*Vec3f(-PX,-PY,-PZ), rotR*Vec3f(0.0f,  0.0f, -GZ)}, //4
				{rotR*Vec3f(-PX, PY, PZ), rotR*Vec3f(-GX, 0.0f,  0.0f)}, //5
				{rotR*Vec3f( PX, PY, PZ), rotR*Vec3f(0.0f, 0.0f,   GZ)}, //6
				{rotR*Vec3f( PX, PY,-PZ), rotR*Vec3f( GX,  0.0f, 0.0f)}, //7
				{rotR*Vec3f(-PX, PY,-PZ), rotR*Vec3f(0.0f,  0.0f, -GZ)}  //8
			}
		};
		Vec3f mp[8];
		Vec3f kp[8];
		if (bLeft){
			for(int i=0; i<8; ++i){
				mp[i] = motorPos[1][i][0];
				kp[i] = motorPos[1][i][1];
			}
		}else if (bFinger){
			for(int i=0; i<8; ++i){
				mp[i] = motorPos[2][i][0];
				kp[i] = motorPos[2][i][1];
			}
		}else{
			for(int i=0; i<8; ++i){
				mp[i] = motorPos[0][i][0];
				kp[i] = motorPos[0][i][1];
			}
		}
		if (bFinger){
			Init(8, mp, kp, 0.365296803653f, 1.66555e-5f, 0.3f, 20.0f);
		}else{
			Init(8, mp, kp, 0.365296803653f, 1.66555e-5f, 0.3f, 20.0f);
		}
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

//----------------------------------------------------------------------------
HISpidarG::HISpidarG(const HISpidarGDesc& desc){	// desc is null pointer here
	SetWeight();
}

HISpidarG::~HISpidarG(){
}

bool HISpidarG::Init(const void* pDesc){
	if(!HISpidar::Init(pDesc))
		return false;

	HISdkIf* sdk = GetSdk();
	const HISpidarGDesc& desc = *(const HISpidarGDesc*)pDesc;

	// HISpidarCalc6Dofの初期化
	nRepeat = 2;
	sigma = 0.001f; //sigma=sigma*sigma
	VVector<float> minForce, maxForce;
	minForce.resize(desc.motors.size());
	maxForce.resize(desc.motors.size());
	for(unsigned i=0; i<desc.motors.size(); ++i){
		minForce[i] = desc.motors[i].minForce;
		maxForce[i] = desc.motors[i].maxForce;
	}
	HISpidarCalc6Dof::Init(6, minForce, maxForce);

	// ボタンデバイスの初期化
	buttons.resize(8);
	size_t i;
	for(i=0; i<buttons.size(); ++i){
		buttons[i] = sdk->RentVirtualDevice(DVPioIf::GetIfInfoStatic())->Cast();
		if (!buttons[i]) break;
		AddDeviceDependency(buttons[i]->GetRealDevice()->Cast());
	}
	//	ボタンの無いSPIDARも許容したい
	if (i < buttons.size()) {
		buttons.resize(i);
	}
	bGood = true;
	SetMinForce();
	BeforeCalibration();
	Calibration();
	AfterCalibration();
	return true;
}

bool HISpidarG::Calibration(){
	if (!bGood) return false;
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	for(unsigned i = 0; i < motors.size(); i++)
		motors[i].SetLength( (motors[i].pos - motors[i].jointPos).norm() );
	lengthDiffAve.clear();
	pos = Vec3f();
	ori = Quaterniond();
	HISpidarCalc6Dof::Update();	//	姿勢を更新
	return true;
}

void HISpidarG::Update(float dt){
	if (!bGood) return;
	HIHaptic::Update(dt);
	HISpidarCalc6Dof::Update();
	for(unsigned int i=0; i<motors.size(); ++i){
		motors[i].SetForce(Tension()[i]);
	}
}

void HISpidarG::MakeWireVec(){
	if (!bGood) return;
	for(unsigned int i=0; i<motors.size(); ++i){
		wireDirection[i] = motors[i].pos - (ori*motors[i].jointPos + pos);
		calculatedLength[i] = wireDirection[i].norm();
		wireDirection[i] /= calculatedLength[i];
		wireMoment[i] = (ori * motors[i].jointPos) ^ wireDirection[i];
	}
}

void HISpidarG::UpdatePos(){
}

void HISpidarG::MeasureWire(){
	if (!bGood) return;
	for(unsigned int i=0; i<motors.size(); ++i){
		measuredLength[i] = motors[i].GetLength();
	}	
}

}	//	namespace Spr

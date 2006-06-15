#include "HIS.h"
#pragma hdrstop
//#include "HISpidar3.h"

namespace Spr {
extern "C" int quadpr_(float*,int*,float*,float*,float*,int*,float*,char*,
                        char*,float*,float*,float*,float*,float*,int*,float*
                        ,int,int);
//----------------------------------------------------------------------------

HISpidar3::HISpidar3(){}
HISpidar3::~HISpidar3(){SetMinForce();}
bool HISpidar3::Init(DVDeviceManager& dev){
	//	ドライバの取得
	int i;
	for(i=0; i<3; ++i){
        motor[i].da = NULL;
		dev.VPool().Rent(motor[i].da);
		if (!motor[i].da) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<3) return false;
	for(i=0; i<3; ++i){
		motor[i].counter = NULL;
		dev.VPool().Rent(motor[i].counter);
		if (!motor[i].counter) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	if (i<3) return false;
	BeforeCalib();
	Calib();
	AfterCalib();
	return true;
}
bool HISpidar3::Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF, bool bl){
	for(int i=0; i<3; i++){
		motor[i].pos = motorPos[i];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
		motor[i].minForce = minF;
		motor[i].maxForce = maxF;
	}
	bLeft = bl;
	return Init(dev);
}
bool HISpidar3::EasyCalib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	Vec3f g;
	for(int i=0; i<3; i++) g += motor[i].pos / 3;
	for(int i=0; i<3; i++) motor[i].SetLength( (g-motor[i].pos).norm() );
	return true;
}
bool HISpidar3::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	for(int i=0; i<3; i++) motor[i].SetLength( motor[i].pos.norm() );
	return true;
}
void HISpidar3::Update(float dt){
	HIForceDevice3D::Update(dt);

	float x0,y0,z0, x1,y1,z1, x2,y2,z2;
	x0 = motor[0].pos.X();
	y0 = motor[0].pos.Y();
	z0 = motor[0].pos.Z();
	
	x1 = motor[1].pos.X();
	y1 = motor[1].pos.Y();
	z1 = motor[1].pos.Z();
		
	x2 = motor[2].pos.X();
	y2 = motor[2].pos.Y();
	z2 = motor[2].pos.Z();

	Vec3f plnCenter = (motor[0].pos + motor[1].pos + motor[2].pos) / 3.0f;
		
	float A0, A1, A2;
	A0 = Square(x0) + Square(y0) + Square(z0);
	A1 = Square(x1) + Square(y1) + Square(z1);
	A2 = Square(x2) + Square(y2) + Square(z2);
				
	// Length of strings
	float L0, L1, L2, L0Sqr, L1Sqr, L2Sqr;
	L0 = motor[0].GetLength();
	L1 = motor[1].GetLength();
	L2 = motor[2].GetLength();
		
	L0Sqr = Square(L0);
	L1Sqr = Square(L1);
	L2Sqr = Square(L2);
	
	float B0, B1;
	B0 = (L0Sqr - L1Sqr - A0 + A1) / 2.0f;
	B1 = (L0Sqr - L2Sqr - A0 + A2) / 2.0f;
				
	float denom1, C0, C1;
	denom1 = ((z2 - z0) * (y1 - y0)) - ((z1 - z0) * (y2-y0));
	C0 = ((B0 * (z2 - z0)) - (B1 * (z1 - z0))) / denom1;
	C1 = (((x2 - x0) * (z1 - z0)) - ((x1 - x0) * (z2 - z0))) / denom1;
	
	float denom2, D0, D1;
	denom2 = ((y2 - y0) * (z1 - z0)) - ((y1 - y0) * (z2 - z0));
	D0 = ((B0 * (y2 - y0)) - (B1 * (y1 - y0))) / denom2;
	D1 = (((y1 - y0) * (x2 - x0)) - ((y2 - y0) * (x1 - x0))) / denom2;
					
	float a, b, c;
	a = 1.0f + Square(C1) + Square(D1);
	b = 2.0f * ((C0*C1) - (C1*y0) + (D0*D1) - (D1*z0) - x0);
	c = A0 - L0Sqr + Square(C0) + Square(D0) - (2.0f*C0*y0) - (2.0f*D0*z0);
	
	float criticalTerm = Square(b) - (4.0f*a*c);
	if (criticalTerm <= 0){
		//	TRACE("center= %f,%f,%f \n", plnCenter.x, plnCenter.y, plnCenter.z);
		pos = plnCenter;
	}
	
	// Solve polynomial eq. ax^2 + bx^1 + c = 0
	// solution x = (-b+-sqrt(b^2-4ac))/2a
	//
	// for finger0 to finger3 (Left hand)  use	x = (-b+sqrt(b^2-4ac))/2a
	//     ^^^^^^^^^^^^^^^^^^	                      ^^^	
	// for finger4 to finger7 (Right hand) use	x = (-b-sqrt(b^2-4ac))/2a
	//     ^^^^^^^^^^^^^^^^^^		                  ^^^	
	float x, y, z;
	if (bLeft) x = (-b + sqrt(Square(b) - (4.0f*a*c))) / (2.0f*a);
	else x = (-b - sqrt(Square(b) - (4.0f*a*c))) / (2.0f*a);
	y = C0 + (C1 * x);
	z = D0 + (D1 * x);
	//	TRACE("P= %f,%f,%f \n", x, y, z);
	pos = Vec3f(x,y,z);
}

void HISpidar3::SetForce(const Vec3f& f,float gamma){
    Vec3f v3force = f;           //目的の力
    int i;
    float tension[3];
	Vec3f phi[3];

    Vec3f v3fmin=Vec3f(0,0,0);
    for (i=0; i<3; i++){
        phi[i] = (motor[i].pos-pos).unit();			//	張力の単位ベクトル
        v3fmin=v3fmin+motor[i].minForce*phi[i];		//	最低力を算出
    }
    v3force = v3force-v3fmin;						//	入力から最低力を引く

    Matrix3f quad;									//	目的関数の２次係数行列
    for(int x=0; x<3; x++){
		for(int y=0; y<3; y++){
			quad[x][y] = phi[x]*phi[y];
		}
	}
	for(i=0;i<3;i++) quad[i][i] += gamma;

    Vec3f cost;										//	目的関数の１次係数ベクトル
    for(i=0;i<3;i++) cost[i] = -2*phi[i]*v3force;

    Matrix3f a;										//	条件式の左辺、単位行列

    Vec3f rhs;										//	条件式の右辺、ベクトル
    for(i=0;i<3;i++) rhs[i] = motor[i].maxForce;

    int kt[3]={1,1,1};								//	条件式の不等号、<=なら１
    int input[14]={3,3,3,3,0,240,1000,1,0,0,0,0,0,80};    
	//各設定変数
    float tol[2]={0,0};							   //	許容誤差

    char title[]="quadratic ploblem";
    char pfile[]="savefile";
    float obj[1],x[3],rc[3],dual[3],slk[3];        //	output＆auxiliary storage
    int iout[2];                                   //	output
    float ws[240];                                 //	work space
    int title_len=0, pfile_len=0;

    quadpr_(a, kt, rhs, cost, quad, input, tol, title, pfile,
		obj, x, rc, dual, slk, iout, ws,title_len,pfile_len);

    for(i=0;i<3;i++) {
        tension[i]=x[i];                              //張力＝x[]
    }

    for (i=0;i<3;i++){
        motor[i].SetForce(tension[i]+motor[i].minForce);
    }

}

void HISpidar3::SetMinForce(){
	for(int i=0; i<3; i++) motor[i].SetForce(motor[i].minForce);
}

}	//	namespace Spr

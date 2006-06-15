#include "HIS.h"
#pragma hdrstop
//#include "HISpidar4Angle.h"

namespace Spr {
extern "C" int quadpr_(float*,int*,float*,float*,float*,int*,float*,char*,
                        char*,float*,float*,float*,float*,float*,int*,float*
                        ,int,int);
/*
//	モータの取り付け位置
//	モータが直方体に取り付けられている場合
#define PX	0.265f	//	x方向の辺の長さ/2
#define PY	0.265f	//	y方向の辺の長さ/2
#define PZ	0.265f	//	z方向の辺の長さ/2
Vec3f HISpidar4Angle::motorPosDef[][4] =	//	モータの取り付け位置(中心を原点とする)
	{
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
*/
//----------------------------------------------------------------------------

HISpidar4Angle::HISpidar4Angle(){
    for (int i=0;i<4;i++){
        tension[i] = 0;
        realtens[i] = 0;
//追加
		flag = true;
    }
}
HISpidar4Angle::~HISpidar4Angle(){SetMinForce();}

// links the HD to VD and RD
bool HISpidar4Angle::Init(DVDeviceManager& dev){
	//	ドライバの取得
	int i;
	for(i=0; i<4; ++i){
		motor[i].da = (DVDaBase*)dev.VPool().Rent("D/A");
		if (!motor[i].da) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<4) return false;
	for(i=0; i<4; ++i){
		motor[i].counter = (DVCounterBase*)dev.VPool().Rent("Counter");
		if (!motor[i].counter) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	for (i=0; i<4; ++i){
		motor[i].counter_angle = (DVCounterBase*)dev.VPool().Rent("Counter");
		if (!motor[i].counter_angle) break;
		AddRealDeviceDependency(motor[i].counter_angle->RealDevice());
	}
	if (i<4) return false;
	InitMat();
	BeforeCalib();
	Calib();
	AfterCalib();
	return true;
}

bool HISpidar4Angle::Init(DVDeviceManager& dev, Vec3f* motorPos, Vec3f* iPos, float vpn, float lpp, float app, float minF, float maxF){
	for(int i=0; i<4; i++){
		motor[i].pos = motorPos[i];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
		motor[i].anglePerPulse = app;
		motor[i].minForce = minF;
		motor[i].maxForce = maxF;
		}
	for(int i=0; i<5; i++){
		/// posの初期化
		initPos[i] = iPos[i];
		pos[i] = initPos[i];
	}
	
	return Init(dev);
}

bool HISpidar4Angle::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	
	Update(0.001f);
	for(int i=0; i<4; i++) motor[i].SetLength((motor[i].pos-initPos[i]).norm());
	for(int i=0; i<4; i++) motor[i].Angle(atan((initPos[i].Z()-motor[i].pos.Z())/(initPos[i].X()-motor[i].pos.X())));


	return true;
}

void HISpidar4Angle::Update(float dt){
	HIHapticDevice::Update(dt);
	Pos_6dof();
}

//**********************コレスキー法？***************************//
void HISpidar4Angle::CholeskyFacToGetSolution(TMatrixRow<9,9,float> Q ,TVector<9,float> s)
{
	int i,j,k;
	int num=9;
	
	//reduction  foreward
	Q[0][0]=sqrt(Q[0][0]);
	s[0]/=Q[0][0];
	for(i=1;i<num;i++) {
		Q[0][i]/=Q[0][0];
	}	
	
	for(i=1;i<num;i++){
		for(k=0;k<i;k++){
			Q[i][i] -= Q[k][i]*Q[k][i];
			s[i]-=Q[k][i]*s[k];
		}
		Q[i][i]=sqrt(Q[i][i]);
		for(j=i+1;j<num;j++){
			for (k=0;k<i;k++){
				Q[i][j]-=Q[k][i]*Q[k][j];
			}
			Q[i][j]/=Q[i][i];
		}
		s[i]/=Q[i][i];
	}

	// backwark substitution 
    for(i=num-1;i>=0;i--){
		for(j=i+1;j<num;j++){			
			s[i] -= Q[i][j] * s[j];
        }
		s[i] /= Q[i][i];
    }
    
	deltaX=s[0]  , deltaY=s[1]  , deltaZ=s[2];
	deltaX1=s[3] , deltaY1=s[4] , deltaZ1=s[5];
	deltaX2=s[6] , deltaY2=s[7] , deltaZ2=s[8];

}


//******************* 逐次計算を行う関数 *********************//
void HISpidar4Angle::SetMat( float x,float y,float  z,float x1,float y1,float z1,float x2,float y2,float z2)
{
    int     i, j, k;
	float	denom1, denom2, denom3, denom4;		//　角度の項の分母

	denom1 = (x+x1-motor[0].pos.X())*(x+x1-motor[0].pos.X()) + (z+z1-motor[0].pos.Z())*(z+z1-motor[0].pos.Z());
	denom2 = (x-x1-motor[1].pos.X())*(x-x1-motor[1].pos.X()) + (z-z1-motor[1].pos.Z())*(z-z1-motor[1].pos.Z());
	denom3 = (x+x2-motor[2].pos.X())*(x+x2-motor[2].pos.X()) + (z+z2-motor[2].pos.Z())*(z+z2-motor[2].pos.Z());
	denom4 = (x-x2-motor[3].pos.X())*(x-x2-motor[3].pos.X()) + (z-z2-motor[3].pos.Z())*(z-z2-motor[3].pos.Z());
    
	matA[0][0]=x+x1-motor[0].pos.X();		matA[0][1]=y+y1-motor[0].pos.Y();		matA[0][2]=z+z1-motor[0].pos.Z();
    matA[0][3]=x+x1-motor[0].pos.X();		matA[0][4]=y+y1-motor[0].pos.Y();		matA[0][5]=z+z1-motor[0].pos.Z();
	matA[0][6]=0;			matA[0][7]=0;			matA[0][8]=0;
   
    matA[1][0]=x-x1-motor[1].pos.X();		matA[1][1]=y-y1-motor[1].pos.Y();		matA[1][2]=z-z1-motor[1].pos.Z();
    matA[1][3]=-(x-x1-motor[1].pos.X());	matA[1][4]=-(y-y1-motor[1].pos.Y());	matA[1][5]=-(z-z1-motor[1].pos.Z());
	matA[1][6]=0;			matA[1][7]=0;			matA[1][8]=0;
    
    matA[2][0]=x+x2-motor[2].pos.X();		matA[2][1]=y+y2-motor[2].pos.Y();		matA[2][2]=z+z2-motor[2].pos.Z();
    matA[2][3]=0;			matA[2][4]=0;			matA[2][5]=0;
	matA[2][6]=x+x2-motor[2].pos.X();		matA[2][7]=y+y2-motor[2].pos.Y();		matA[2][8]=z+z2-motor[2].pos.Z();
    
	matA[3][0]=x-x2-motor[3].pos.X();		matA[3][1]=y-y2-motor[3].pos.Y();		matA[3][2]=z-z2-motor[3].pos.Z();
    matA[3][3]=0;			matA[3][4]=0;			matA[3][5]=0;
	matA[3][6]=-(x-x2-motor[3].pos.X());	matA[3][7]=-(y-y2-motor[3].pos.Y());	matA[3][8]=-(z-z2-motor[3].pos.Z());

    matA[4][0]=-(z+z1-motor[0].pos.Z())/denom1;		matA[4][1]=0;	matA[4][2]=(x+x1-motor[0].pos.X())/denom1;
    matA[4][3]=-(z+z1-motor[0].pos.Z())/denom1;		matA[4][4]=0;	matA[4][5]=(x+x1-motor[0].pos.X())/denom1;
	matA[4][6]=0;			matA[4][7]=0;			matA[4][8]=0;

    matA[5][0]=-(z-z1-motor[1].pos.Z())/denom2;		matA[5][1]=0;	matA[5][2]=(x-x1-motor[1].pos.X())/denom2;
    matA[5][3]=(z-z1-motor[1].pos.Z())/denom2;		matA[5][4]=0;	matA[5][5]=-(x-x1-motor[1].pos.X())/denom2;
	matA[5][6]=0;			matA[5][7]=0;			matA[5][8]=0;
  
    matA[6][0]=-(z+z2-motor[2].pos.Z())/denom3;		matA[6][1]=0;	matA[6][2]=(x+x2-motor[2].pos.X())/denom3;
    matA[6][3]=0;			matA[6][4]=0;			matA[6][5]=0;
	matA[6][6]=-(z+z2-motor[2].pos.Z())/denom3;		matA[6][7]=0;	matA[6][8]=(x+x2-motor[2].pos.X())/denom3;
    
	matA[7][0]=-(z-z2-motor[3].pos.Z())/denom4;		matA[7][1]=0;	matA[7][2]=(x-x2-motor[3].pos.X())/denom4;
    matA[7][3]=0;			matA[7][4]=0;			matA[7][5]=0;
	matA[7][6]=(z-z2-motor[3].pos.Z())/denom4;		matA[7][7]=0;	matA[7][8]=-(x-x2-motor[3].pos.X())/denom4;

    matA[8][0]=0;			matA[8][1]=0;			matA[8][2]=0;
    matA[8][3]=x1;			matA[8][4]=y1;			matA[8][5]=z1;
	matA[8][6]=0;			matA[8][7]=0;			matA[8][8]=0;

    matA[9][0]=0;			matA[9][1]=0;			matA[9][2]=0;
    matA[9][3]=0;			matA[9][4]=0;			matA[9][5]=0;
	matA[9][6]=x2;			matA[9][7]=y2;			matA[9][8]=z2;

    matA[10][0]=0;			matA[10][1]=0;			matA[10][2]=0;
    matA[10][3]=-(x1-x2);	matA[10][4]=-(y1-y2);	matA[10][5]=-(z1-z2);
	matA[10][6]=x1-x2;		matA[10][7]=y1-y2;		matA[10][8]=z1-z2;
   	
	for( i=0;i<9;i++)
	{
	   for( j=0;j<9;j++)
	   {
		ATA[i][j]=0;
		for( k=0;k<11;k++)
		    ATA[i][j]+=matA[k][i]*matA[k][j];
       }
    }
	for( i=0;i<9;i++) ATA[i][i]+=sigma;
	
	//  lengthDiff[i] is the measured SetLength of this time minus the computed legth of last time  
    lengthDiff[0]=0.5f*(l1-l1old);
    lengthDiff[1]=0.5f*(l2-l2old);
	lengthDiff[2]=0.5f*(l3-l3old);
	lengthDiff[3]=0.5f*(l4-l4old);
	lengthDiff[4]=(a1-a1old);
	lengthDiff[5]=(a2-a2old);
	lengthDiff[6]=(a3-a3old);
	lengthDiff[7]=(a4-a4old);
    lengthDiff[8]=0;
	lengthDiff[9]=0;
	lengthDiff[10]=0;
    
	for(i=0;i<9;i++){
		postureDiff[i]=0;
		for(k=0;k<11;k++)
			postureDiff[i]+=matA[k][i]*lengthDiff[k];
	}

}

//*********************位置を呼ぶ関数(誤差を収束させる)************************//
bool HISpidar4Angle::Pos_6dof()
{
float   val_xyz1, val_xyz2, val_xyz3;
sigma = 0.0025f;
int step = 0;
if(flag == true){
	X1 = initPos[0].X();
	Y1 = initPos[0].Y();
	Z1 = initPos[0].Z();
	X2 = initPos[2].X();
	Y2 = initPos[2].Y();
	Z2 = initPos[2].Z();
	X  = initPos[4].X();
	Y  = initPos[4].Y();
	Z  = initPos[4].Z();

	//calibration時の糸の長さと角度を初期値として与える
	l1old = (motor[0].pos-initPos[0]).norm();
	l2old = (motor[1].pos-initPos[1]).norm();
	l3old = (motor[2].pos-initPos[2]).norm();
	l4old = (motor[3].pos-initPos[3]).norm();
	a1old = atan((initPos[0].Z()-motor[0].pos.Z())/(initPos[0].X()-motor[0].pos.X()));
	a2old = atan((initPos[1].Z()-motor[1].pos.Z())/(initPos[1].X()-motor[1].pos.X()));
	a3old = atan((initPos[2].Z()-motor[2].pos.Z())/(initPos[2].X()-motor[2].pos.X()));
	a4old = atan((initPos[3].Z()-motor[3].pos.Z())/(initPos[3].X()-motor[3].pos.X()));

	//l1old,a1old,などはすべて二乗で扱っている
	l1old = l1old*l1old;
	l2old = l2old*l2old;
	l3old = l3old*l3old;
	l4old = l4old*l4old;
	a1old = a1old*a1old;
	a2old = a2old*a2old;
	a3old = a3old*a3old;
	a4old = a4old*a4old;
	
	flag = false;
}
/******エンコーダの値から長さ、角度を計算******/	
		l1 = motor[0].GetLength();
		l2 = motor[1].GetLength();
		l3 = motor[2].GetLength();
		l4 = motor[3].GetLength();
		a1 = motor[0].Angle();
		a2 = motor[1].Angle();
		a3 = motor[2].Angle();
		a4 = motor[3].Angle();

		l1 = l1*l1;
		l2 = l2*l2;
		l3 = l3*l3;
		l4 = l4*l4;

		do{
			step++;

			SetMat(X,Y,Z,X1,Y1,Z1,X2,Y2,Z2); 
			CholeskyFacToGetSolution(ATA,postureDiff);

			Xnew=X+deltaX ; Ynew=Y+deltaY ; Znew=Z+deltaZ ;
			//float size1,size2;
			//size1=sqrt((X1+deltaX1)*(X1+deltaX1)+(Y1+deltaY1)*(Y1+deltaY1)+(Z1+deltaZ1)*(Z1+deltaZ1));
			//size2=sqrt((X2+deltaX2)*(X2+deltaX2)+(Y2+deltaY2)*(Y2+deltaY2)+(Z2+deltaZ2)*(Z2+deltaZ2));			
		
//ここは単位ベクトル化に直しているの？////////////////

///0.06020797という定数は手元位置の中心から手元位置の周辺位置の距離
			X1new=X1+deltaX1; Y1new=Y1+deltaY1; Z1new=Z1+deltaZ1;
			X2new=X2+deltaX2; Y2new=Y2+deltaY2; Z2new=Z2+deltaZ2;
			float size1,size2;
			size1=sqrt(X1new*X1new+Y1new*Y1new+Z1new*Z1new);
			size2=sqrt(X2new*X2new+Y2new*Y2new+Z2new*Z2new);
			X1new=X1new*0.0602079728f/size1;Y1new=Y1new*0.0602079728f/size1;Z1new=Z1new*0.0602079728f/size1;
			X2new=X2new*0.0602079728f/size2;Y2new=Y2new*0.0602079728f/size2;Z2new=Z2new*0.0602079728f/size2;
///////////////////////////////////////////////////////
			//次のループのためにそれぞれを保存
			X=Xnew; Y=Ynew; Z=Znew;
			X1=X1new; Y1=Y1new; Z1=Z1new;
			X2=X2new; Y2=Y2new; Z2=Z2new;
			

			//Saving the new legth for next iterative step
			l1new = (Xnew+X1new-motor[0].pos.X())*(Xnew+X1new-motor[0].pos.X())+(Ynew+Y1new-motor[0].pos.Y())*(Ynew+Y1new-motor[0].pos.Y())+(Znew+Z1new-motor[0].pos.Z())*(Znew+Z1new-motor[0].pos.Z());
			l2new = (Xnew-X1new-motor[1].pos.X())*(Xnew-X1new-motor[1].pos.X())+(Ynew-Y1new-motor[1].pos.Y())*(Ynew-Y1new-motor[1].pos.Y())+(Znew-Z1new-motor[1].pos.Z())*(Znew-Z1new-motor[1].pos.Z());
			l3new = (Xnew+X2new-motor[2].pos.X())*(Xnew+X2new-motor[2].pos.X())+(Ynew+Y2new-motor[2].pos.Y())*(Ynew+Y2new-motor[2].pos.Y())+(Znew+Z2new-motor[2].pos.Z())*(Znew+Z2new-motor[2].pos.Z());
			l4new = (Xnew-X2new-motor[3].pos.X())*(Xnew-X2new-motor[3].pos.X())+(Ynew-Y2new-motor[3].pos.Y())*(Ynew-Y2new-motor[3].pos.Y())+(Znew-Z2new-motor[3].pos.Z())*(Znew-Z2new-motor[3].pos.Z());
			a1new = atan((Znew+Z1new-motor[0].pos.Z())/(Xnew+X1new-motor[0].pos.X()));
			a2new = atan((Znew-Z1new-motor[1].pos.Z())/(Xnew-X1new-motor[1].pos.X()));
			a3new = atan((Znew+Z2new-motor[2].pos.Z())/(Xnew+X2new-motor[2].pos.X()));
			a4new = atan((Znew-Z2new-motor[3].pos.Z())/(Xnew-X2new-motor[3].pos.X()));


			l1old = l1new;
			l2old = l2new;
			l3old = l3new;
			l4old = l4new;
			a1old = a1new;
			a2old = a2new;
			a3old = a3new;
			a4old = a4new;

			//  Precision control by the error for abs of measured SetLength  minus computed SetLength       
			//errorLen=(abs(l1-l1old)+abs(l2-l2old)+abs(l3-l3old)+abs(l4-l4old)+abs(l5-l5old)+abs(l6-l6old)+abs(l7-l7old)+abs(l8-l8old));
			//   time  period must be computed  by getting the elapsed system time ,using function timeGetTime()
		}
		while(step < 5);
		
		pos[0] = Vec3f(Xnew+X1new, Ynew+Y1new, Znew+Z1new);
		pos[1] = Vec3f(Xnew-X1new, Ynew-Y1new, Znew-Z1new);
		pos[2] = Vec3f(Xnew+X2new, Ynew+Y2new, Znew+Z2new);
		pos[3] = Vec3f(Xnew-X2new, Ynew-Y2new, Znew-Z2new);
		pos[4] = Vec3f(Xnew, Ynew, Znew);

//回転のためのローカル座標系の軸を算出

		vec2[0] = X1new;
		vec2[1] = Y1new;
		vec2[2] = Z1new;

		vec1[0] = X2new;
		vec1[1] = Y2new;
		vec1[2] = Z2new;

		val_xyz1 = sqrt(vec1[0]*vec1[0] +vec1[1]*vec1[1] + vec1[2]*vec1[2]);
		val_xyz2 = sqrt(vec2[0]*vec2[0] +vec2[1]*vec2[1] + vec2[2]*vec2[2]);

	    vec2[0]/=val_xyz2;
		vec2[1]/=val_xyz2;
		vec2[2]/=val_xyz2;
    
		vec1[0]/=val_xyz1;
		vec1[1]/=val_xyz1;
		vec1[2]/=val_xyz1;
   

		vec3[0]=vec1[1]*vec2[2]-vec2[1]*vec1[2]; 
		vec3[1]=vec1[2]*vec2[0]-vec2[2]*vec1[0];
		vec3[2]=vec1[0]*vec2[1]-vec1[1]*vec2[0];

		val_xyz3=sqrt(vec3[0]*vec3[0] +vec3[1]*vec3[1] + vec3[2]*vec3[2]);
		vec3[0]/=val_xyz3;
		vec3[1]/=val_xyz3;
		vec3[2]/=val_xyz3;


	return true;
}

void HISpidar4Angle::SetForce(const Vec3f& f, float eff, float cont){
	/*	2次計画法による張力計算
		条件式を簡単にするため，最小張力 < 張力 < 最大張力 ではなく
		0 < t=張力-最小張力 < 最大張力-最小張力
		として，tを求めてから，実際に出力する張力を求める．	*/
	Vec3f v3force = f;							//	目的の力
    Vec3f v3fmin=Vec3f(0,0,0);
    for (int i=0; i<4; i++){
        phi[i] = (motor[i].pos-pos[5]).unit();		//	張力の単位ベクトル		tension direction
        v3fmin=v3fmin+motor[i].minForce*phi[i];	//	最小張力が作り出す力	"least power"
    }
    v3force = v3force-v3fmin;					//	入力から最小張力による力を引く

	/*	目的関数
		  |f-φt|^2 + cont*|t-t'|^2 + eff*t^2
		= (φ^2+eff+cont)*t^2 + (-2*φ*f -2*cont*t')*t + f^2+cont*t'^2
		を最小にする張力tを求める．	*/
    float quad[16];								//	目的関数の２次係数行列
    for(int i=0;i<4;i++) quad[4*i+i]=phi[i]*phi[i]+eff+cont;
    quad[1]=quad[4]=phi[0]*phi[1];
    quad[2]=quad[8]=phi[0]*phi[2];
    quad[3]=quad[12]=phi[0]*phi[3];
    quad[6]=quad[9]=phi[1]*phi[2];
    quad[7]=quad[13]=phi[1]*phi[3];
    quad[11]=quad[14]=phi[2]*phi[3];

    float cost[4];								//	目的関数の１次係数ベクトル
    for(int i=0;i<4;i++) cost[i]=-2*phi[i]*v3force -2*cont*tension[i];

    float a[16];								//	条件式の左辺、単位行列
    a[0]=a[5]=a[10]=a[15]=1;
    a[1]=a[2]=a[3]=a[4]=a[6]=a[7]=a[8]=a[9]=a[11]=a[12]=a[13]=a[14]=0;

    float rhs[4];								//	条件式の右辺、ベクトル
    for(int i=0;i<4;i++) rhs[i] = motor[i].maxForce - motor[i].minForce;

    int kt[4]={1,1,1,1};						//	条件式の不等号、<=なら１
    int input[14]={4,4,4,4,0,240,1000,1,0,0,0,0,0,80};	//	各設定変数
    float tol[2]={0,0};							//	許容誤差

    char title[]="quadratic ploblem";
    char pfile[]="savefile";

    float obj[1],x[4],rc[4],dual[4],slk[4];        //output＆auxiliary storage
    int iout[2];                                   //output
    float ws[240];                                 //work space
    int title_len=0, pfile_len=0;

    quadpr_(a, kt, rhs, cost, quad, input, tol, title, pfile,
	 obj, x, rc, dual, slk, iout, ws,title_len,pfile_len);

    for(int i=0;i<4;i++) {
        tension[i]=x[i];							//張力＝x[]
    }
    for (int i=0;i<4;i++){
		realtens[i] = tension[i] + motor[i].minForce;
        motor[i].SetForce(realtens[i]);
    }
}
Vec3f HISpidar4Angle::GetForce(){
    int i;
	Vec3f f;
    for (i=0;i<4;i++) f=f+tension[i]*phi[i];
    return f;
}

void HISpidar4Angle::SetMinForce(){
	for(int i=0; i<4; i++) motor[i].SetForce(motor[i].minForce);
}

void HISpidar4Angle::InitMat()
{
	matPos = 2*Matrix3f(
		motor[1].pos-motor[0].pos,
		motor[2].pos-motor[1].pos,
		motor[3].pos-motor[2].pos).trans();
	matPos = matPos.inv();

	posSqrConst = Vec3f(motor[1].pos.square()-motor[0].pos.square(),
		motor[2].pos.square()-motor[1].pos.square(),
		motor[3].pos.square()-motor[2].pos.square());
}
}	//	namespace Spr

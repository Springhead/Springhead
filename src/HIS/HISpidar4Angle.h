#ifndef HDSPIDAR_H
#define HDSPIDAR_H

#include "HIHapticDevice.h"
#include "HISpidarMotorAngle.h"
#include "Device/DVDeviceManager.h"
#include <vector>
#include <Base/TMatrix.h>

using namespace  PTM;
namespace Spr {

class SPR_DLL HISpidar4Angle: public HIForceDevice6D{
public:

	typedef HIForceDevice6D superclass;
protected:
	Vec3f initPos[5];
	Vec3f pos[5];
	///	位置を計算するための定数
	Vec3f posSqrConst;
	///	位置を計算するための行列
	Matrix3f matPos;
	Vec3f phi[4];
	float realtens[4];	// computed tension + min tension
	float tension[4];	// computed tension
	
protected:
	///	モータ
	HISpidarMotorAngle motor[4];

	/**	出力可能な力を計算し、その出力の得点を計算する.
		@param disable	使用しない糸の番号
		@param f		出力したい力. 出力可能な力に変更される.
		@param v3Str	4本の糸の向き
		@return 得点(力の正確さとモータ出力の大きさで決まる。
				この糸を選択することの正しさ)	*/
	float AvailableForce(int disable, Vec3f& f, Vec3f* v3Str);
	///	位置を計算するための行列の初期化
	void InitMat();

public:
	///
	HISpidar4Angle();
	///
	virtual ~HISpidar4Angle();
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev);
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev, Vec3f* motorPos, Vec3f* iPos, float vpn, float lpp, float app, float minF, float maxF);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	キャリブレーション
	bool Calib();
	///	最低張力を出力
	void SetMinForce();
	///	力の出力
	virtual void SetForce(const Vec3f& f){ SetForce(f, 0.1f); }
	virtual void SetForce(const Vec3f& f,float eff){ SetForce(f, eff, 0.0f); }
	virtual void SetForce(const Vec3f& f, float eff, float cont);
	///	デバイスの状態を更新する.
	virtual void Update(float dt);
	virtual Vec3f GetForce();
	virtual Vec3f GetPos(){return Vec3f();}
//追加	
	virtual Vec3f GetPos(int i){return pos[i];}

	HISpidarMotor* Motor(){
		return motor;
	}

//**************************追加分****************************//
	float l1, l2, l3, l4, a1, a2, a3, a4;
	float Len[4],Ang[4];
	float l1old, l2old, l3old, l4old, a1old, a2old, a3old, a4old;
	float l1new, l2new, l3new, l4new, a1new, a2new, a3new, a4new;

	float X, Y, Z, X1, Y1, Z1, X2, Y2, Z2;
	float Xnew, Ynew, Znew, X1new, Y1new, Z1new, X2new, Y2new, Z2new;
	float deltaX, deltaY, deltaZ, deltaX1, deltaY1, deltaZ1, deltaX2, deltaY2, deltaZ2;

	float	vec1[2], vec2[3],vec3[3];

	TMatrixRow<11,9,float>		matA;
	TMatrixRow<9,9,float>		ATA;
	TVector<9,float>		deltaL;
	TVector<9,float>		LdeltaL;
	TVector<11,float>		lengthDiff;
	TVector<9,float>		postureDiff;
	float sigma;
	float errorLen;
	float errorAng;

	Vec3f rot;
	
	void CholeskyFacToGetSolution(TMatrixRow<9,9,float> Q, TVector<9,float> s);
	void SetMat(float x,float y, float z, float x1,float y1, float z1, float x2,float y2, float z2);
	bool Pos_6dof();
	Vec3f GetRot(){return rot;}
	void CalcRot();
	bool flag;
//************************追加分終了***************************//

};


}	//	namespace Spr
#endif


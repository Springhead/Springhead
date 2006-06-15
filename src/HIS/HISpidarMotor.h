#ifndef VR_HAPTIC_HDSPIDARMOTOR_H
#define VR_HAPTIC_HDSPIDARMOTOR_H

#include "Base/Affine.h"
#include "Device/DVDaBase.h"
#include "Device/DVAdBase.h"
#include "Device/DVCounterBase.h"

namespace Spr {

class SPR_DLL HISpidarMotor{
public:
	///	@name	モータについての定数
	//@{
	///	最大出力(N)
	float maxForce;
	///	最小出力(N)
	float minForce;
	///	D/Aの出力電圧 / モータの出力  (V/N)
	float voltPerNewton;
	///	モータに流れる電流 / D/Aの出力電圧
	float currentPerVolt;
	///	糸の長さ / パルス数
	float lengthPerPulse;
	///	モータが取り付けられている位置(糸の出口)の座標
	Vec3f pos;
	///	糸がグリップに取り付けられている位置
	Vec3f jointPos;
	//
	//@}

	//@{	デバイスアクセス
	///	使用するD/A.
	DVDaBase* da;
	///	使用するエンコーダのカウンタ.
	DVCounterBase* counter;
	///	使用するA/D.
	DVAdBase* ad;
	//@}
	///	出力した力
	float force;

	//@{	モータ定数(内部抵抗・逆起電力・コイル定数)のキャリブレーション
	float voltageInt;	///<	モータ端子間電圧の積分
	float currentInt;	///<	モータ電流の積分
	float timeInt;		///<	積分区間の時間
	float currentLast;	///<	計測開始時のモータ電流
	float lengthLast;	///<	計測開始時の糸の長さ
	float vdd;			///<	電源電圧
	PTM::VMatrixCol<float> calibMat;
						///<	キャリブレーション用データ
	PTM::VVector<float> calibVec;
						///<	キャリブレーション用データ
	Vec4f calibResult;	///<	キャリブレーション結果 (R, L, E)
	//@}
public:
	///	
	HISpidarMotor();
	///	
	virtual ~HISpidarMotor(){}
	///	出力の設定
	void SetForce(float f){
		if(f < minForce) f = minForce;
		if(f > maxForce) f = maxForce;
		if(da) da->Voltage(f * voltPerNewton);
		force = f;
	}
	///	実際に出力した力の取得
	float GetForce(){
		return force;
	}
	///	電流値の取得
	float GetCurrent(){
		return force * voltPerNewton * currentPerVolt;
	}
	///	現在の糸の長さの設定
	void SetLength(float l){ if(counter) counter->Count(long(l / lengthPerPulse)); }
	///	現在の糸の長さ
	float GetLength(){
		if(counter) return counter->Count() * lengthPerPulse;
		else return 0;
	}
	///	速度の計測
	float GetVelocity();
	///	モータ定数のキャリブレーションのための計測 (100回くらいこれを呼んでから)
	void GetCalibrationData(float dt);
	///	モータ定数のキャリブレーション (こっちを呼んでキャリブレーション)
	void Calibrate(bool bUpdate);
	///	電源電圧の計測
	void GetVdd();
};

}	//	namespace Spr
#endif

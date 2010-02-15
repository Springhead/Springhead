#ifndef HISPIDAR4D_H
#define HISPIDAR4D_H

#include "HIHaptic.h"
#include "HISpidarMotor.h"
#include "Device/DVDeviceManager.h"
#include <vector>

namespace Spr {

/**	4本糸のSPIDAR.
	糸の長さから座標計算を行ったり、出力する力覚から各モータの
	出力を計算したりする。
@verbatim
SPIDARの定数の設定例
Init(dev, motPos, 0.5776, 2.924062107079e-5f, 0.5f, 20.0f);

//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
#define PX	0.265f	//	x方向の辺の長さ/2
#define PY	0.265f	//	y方向の辺の長さ/2
#define PZ	0.265f	//	z方向の辺の長さ/2
Vec3f motorPos[][4] =	//	モータの取り付け位置(中心を原点とする){
	{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
	{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
};

//	１ニュートン出すためのDAの出力電圧[V/N]
//	maxon:		0.008[m] / 0.0438[Nm/A] / 0.5[A/V] =  0.365296803653
//	千葉精機:	0.008[m] / 0.0277[Nm/A] / 0.5[A/V] =  0.5776
sp->VoltPerNewton(0.5776f);

//	１カウントの長さ [m/plus]	実測値: 2.924062107079e-5f
sp->LenPerPlus(2.924062107079e-5f);

//	最小張力 [N]
sp->MinForce(0.5f);

//	最大張力 [N]
sp->MaxForce(20.0f);
@endverbatim
*/

class SPR_DLL HISpidar4D: public HIForceInterface3D{
public:
	///	デバイスのタイプ
	SPR_OBJECTDEF(HISpidar4D);
protected:
	Vec3f pos;
	///	位置を計算するための定数
	Vec3f posSqrConst;
	///	位置を計算するための行列
	Matrix3f matPos;
	Vec3f phi[4];
	float tension[4];	// computed tension
	
protected:
	///	モータ
	HISpidarMotor motor[4];

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
	HISpidar4D();
	///
	virtual ~HISpidar4D();
	virtual bool Init(const void* pDesc);
/*
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev);
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF);
*/
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	キャリブレーション
	virtual bool Calib();
	///	最低張力を出力
	void SetMinForce();
	///	力の出力
	virtual void SetForce(const Vec3f& f){ SetForce(f, 0.1f); }
	virtual void SetForce(const Vec3f& f,float eff){ SetForce(f, eff, 0.0f); }
	virtual void SetForce(const Vec3f& f, float eff, float cont);
	///	デバイスの状態を更新する.
	virtual void Update(float dt);
	virtual Vec3f GetForce();
	virtual Vec3f GetPosition(){ return pos; }

	HISpidarMotor* Motor(){
		return motor;
	}

};


}	//	namespace Spr
#endif


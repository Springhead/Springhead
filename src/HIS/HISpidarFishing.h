#ifndef HDSPIDARFISHING_H
#define HDSPIDARFISHING_H

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVPioBase.h"
#include "Device/DVDeviceManager.h"
#include "HISpidarCalc3Dof.h"

namespace Spr {;

/**	@JA	エンドエフェクタ(グリップ)が剛体で6自由度なSPIDAR．糸の本数は可変．
	@EN	A SPIDAR with a 6DOF solid grip. The number of the strings is variable.@@*/
class SPR_DLL HISpidarFishing: public HIForceDevice3D,public HISpidarCalc3Dof{
public:
	std::vector<HISpidarMotor> motor;	///<	モータ
	std::vector<DVPioBase*> button;	///<	ボタン

		///	デバイスのタイプ
	HIOBJECTDEF(HISpidarFishing);

	HISpidarFishing();
	bool Init(DVDeviceManager& dev);
	virtual double  GetLength(int);

	///	デバイスの初期化(使用前に呼ぶ)
//	bool Init(DVDeviceManager& dev, int nMotor, const Vec3f(* motorPos)[2], float vpn, float lpp, float minF, float maxF);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	キャリブレーション
	virtual bool Calib();
	///	最低張力を出力
	void SetMinForce();
	
	///	位置の取得
	Vec3f GetPos(){ return pos; }
	///	角度の取得
//	Quaternionf GetOri(){ return ori; }
	
	///	力の設定
	void SetForce(const Vec3f& f){ HISpidarCalc3Dof::SetForce(f); }
	///	トルクの設定
//	void SetForce(const Vec3f& f, const Vec3f& t){ HISpidarCalc6Dof::SetForce(f, t); }
	///	重み付けの設定
	void SetWeight(float s=0.3f, float t=1.0f){ smooth=s; lambda_t=t; }

	///	力の取得
	Vec3f GetForce(){ return trnForce; }
	///	トルクの取得
//	Vec3f GetTorque(){ return rotForce; }

	/// ボタン情報の取得
	int HISpidarFishing::GetButton(int ch){
		if(button[ch]) return button[ch]->Get();
		else return 0;
	}

	///	デバイスの状態を更新する.
	virtual void Update(float dt);
	virtual void Update(){ HISpidarCalc3Dof::Update(); }

protected:
	virtual void MakeWireVec();					///<	ワイヤ方向ベクトルの計算
	virtual void UpdatePos();					///<	ワイヤ設置位置座標の更新
	virtual void MeasureWire();					///<	ワイヤ長の計測
};

}	//	namespace Spr
#endif



/*#ifndef HDSPIDARFISHING_H
#define HDSPIDARFISHING_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	フレームのサイズやモータの符号を含む	*/

/*
class SPR_DLL HISpidarFishing: public HISpidarG6{
public:
	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarFishing);

	HISpidarFishing();
	bool Init(DVDeviceManager& dev);
	virtual double  GetLength(int);

};

}	//	namespace Spr
#endif

*/
#ifndef HDSPIDARG6_H
#define HDSPIDARG6_H

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVPioBase.h"
#include "Device/DVDeviceManager.h"
#include "HISpidarCalc6Dof.h"

namespace Spr {;

/**	@JA	エンドエフェクタ(グリップ)が剛体で6自由度なSPIDAR．糸の本数は可変．
	@EN	A SPIDAR with a 6DOF solid grip. The number of the strings is variable.@@*/
class SPR_DLL HISpidarG6: public HIForceDevice6D,public HISpidarCalc6Dof{
public:
	std::vector<HISpidarMotor> motor;	///<	モータ
	std::vector<DVPioBase*> button;	///<	ボタン

	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarG6);

	HISpidarG6();
	virtual ~HISpidarG6();
	///	デバイスの初期化(使用前に呼ぶ)
	bool Init(DVDeviceManager& dev, int nMotor, const Vec3f(* motorPos)[2], float vpn, float lpp, float minF, float maxF);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	キャリブレーション
	virtual bool Calib();
	///	最低張力を出力
	void SetMinForce();
	
	///	位置の取得
	Vec3f GetPos(){ return pos; }
	///	角度の取得
	Quaternionf GetOri(){ return ori; }
	
	///	力の設定
	void SetForce(const Vec3f& f){ SetForce(f, Vec3f()); }
	///	トルクの設定
	void SetForce(const Vec3f& f, const Vec3f& t){ HISpidarCalc6Dof::SetForce(f, t); }
	///	重み付けの設定
	void SetWeight(float s=0.3f, float t=1.0f, float r=6.0f){ smooth=s; lambda_t=t; lambda_r=r; }

	///	力の取得
	Vec3f GetForce(){ return trnForce; }
	///	トルクの取得
	Vec3f GetTorque(){ return rotForce; }

	/// ボタン情報の取得
	int HISpidarG6::GetButton(int ch){
		if(button[ch]) return button[ch]->Get();
		else return 0;
	}

	///	デバイスの状態を更新する.
	virtual void Update(float dt);
	virtual void Update(){ HISpidarCalc6Dof::Update(); }

protected:
	virtual void MakeWireVec();					///<	ワイヤ方向ベクトルの計算
	virtual void UpdatePos();					///<	ワイヤ設置位置座標の更新
	virtual void MeasureWire();					///<	ワイヤ長の計測
};

}	//	namespace Spr
#endif

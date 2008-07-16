#ifndef HDSPIDARG6_H
#define HDSPIDARG6_H

#include "HIHaptic.h"
#include "HISpidarMotor.h"
#include "HISpidarCalc6Dof.h"
#include "DVPioBase.h"

namespace Spr {;

/**	@JA	エンドエフェクタ(グリップ)が剛体で6自由度なSPIDAR．糸の本数は可変．
	@EN	A SPIDAR with a 6DOF solid grip. The number of the strings is variable.@@*/
class SPR_DLL HISpidarG: public HIForceInterface6D, public HISpidarCalc6Dof{
protected:
	bool bGood;
public:
	std::vector<HISpidarMotor> motors;	///<	モータ
	std::vector<DVPioBaseIf*> buttons;	///<	ボタン

	///	デバイスのタイプ
	SPR_OBJECTDEF(HISpidarG);

	HISpidarG();
	virtual ~HISpidarG();
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(const void* desc);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	キャリブレーション
	virtual bool Calib();
	///	最低張力を出力
	void SetMinForce();
	
	///	モータ
	HISpidarMotorIf* GetMotor(size_t i);
	///	モータの数
	size_t NMotor() const;
	
	///	位置の取得
	Vec3f GetPosition(){ return pos; }
	///	角度の取得
	Quaternionf GetOrientation(){ return ori; }
	
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
	int HISpidarG::GetButton(int ch){
		if(buttons[ch]) return buttons[ch]->Get();
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

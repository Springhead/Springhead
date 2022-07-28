#ifndef HI_SPIDAR_H
#define HI_SPIDAR_H

#include <HumanInterface/HISpidarMotor.h>
#include <HumanInterface/HISpidarCalc.h>
#include <vector>

/**	4本糸のSPIDAR.
	糸の長さから座標計算を行ったり、出力する力覚から各モータの
	出力を計算したりする。出力の計算に2次計画法を用いているため，
	HIS/Fortran の libF77.lib libI77.lib QuadPR.lib が必要．
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

namespace Spr {;

/**
	Spidarの基本クラス
 **/
class HISpidar: public HIHaptic{
protected:
	std::string type;						///<	type of SPIDAR
	std::vector<HISpidarMotor>	motors;		///<	モータ

public:
	SPR_OBJECTDEF_ABST(HISpidar);

	const char* GetSpidarType() { return type.c_str(); }

	///	モータを取得
	HISpidarMotorIf* GetMotor(size_t i){ return motors[i].Cast(); }
	///	モータの数
	size_t NMotor() const { return motors.size(); }

	///	最低張力を出力
	void SetMinForce();
	/// 全てのモータに対して最小出力を設定
	void SetLimitMinForce(float f);
	/// 全てのモータに対して最大出力を設定
	void SetLimitMaxForce(float f);
	
	virtual bool Init(const void* desc);
	virtual bool BeforeCalibration(){
		SetMinForce();
		return true;
	}
	
	HISpidar(){ isGood = false; }
	HISpidar(const HISpidarDesc& desc) {}
	virtual ~HISpidar(){ SetMinForce(); }
};

/**
	4-wire Spidar
	内部計算にHISpidarCalc3Dofを使用
 */
class SPR_DLL HISpidar4: public HISpidar, public HISpidarCalc3Dof{
public:
	SPR_OBJECTDEF(HISpidar4);

	HISpidar4(const HISpidar4Desc& desc = HISpidar4Desc());
	virtual ~HISpidar4();
	Vec3f	GetPosition(){ return HISpidarCalc3Dof::GetPosition(); }

protected:
	/**	出力可能な力を計算し、その出力の得点を計算する.
		@param disable	使用しない糸の番号
		@param f		出力したい力. 出力可能な力に変更される.
		@param v3Str	4本の糸の向き
		@return 得点(力の正確さとモータ出力の大きさで決まる。
				この糸を選択することの正しさ)	*/
	float AvailableForce(int disable, Vec3f& f, Vec3f* v3Str);

	virtual bool Init(const void* pDesc);
	virtual bool Calibration();
	virtual void Update(float dt);

	/// HISpidarCalc3Dofの仮想関数
	virtual void MakeWireVec();					///<	ワイヤ方向ベクトルの計算
	virtual void UpdatePos();					///<	ワイヤ設置位置座標の更新
	virtual void MeasureWire();					///<	ワイヤ長の計測
};

/**
	4-wire Spidar double grip
	内部計算は独自実装（直接位置計算を行う方式）
 */
class HISpidar4D: public HISpidar4{
public:
	SPR_OBJECTDEF(HISpidar4D);

protected:
	Vec3f pos;
	///	位置を計算するための定数
	Vec3f posSqrConst;
	///	位置を計算するための行列
	Matrix3f matPos;
	Vec3f	phi[4];
	float	eff;
	float	cont;
	float	tension[4];
	///	位置を計算するための行列の初期化
	void InitMat();

public:
	virtual bool Calibration();
	virtual void Update(float dt);

	void SetForce(const Vec3f& f, const Vec3f& t = Vec3f());
	Vec3f	GetForce();
	Vec3f	GetPosition(){ return pos; }

	HISpidar4D(const HISpidar4DDesc& desc = HISpidar4DDesc());
};

/**	@JA	エンドエフェクタ(グリップ)が剛体で6自由度なSPIDAR．糸の本数は可変．
	@EN	A SPIDAR with a 6DOF solid grip. The number of the strings is variable.@@*/
class SPR_DLL HISpidarG: public HISpidar, protected HISpidarCalc6Dof{
public:
	SPR_OBJECTDEF(HISpidarG);

	std::vector< DVPioIf* >		buttons;	///<	ボタン

public:
	HISpidarG(const HISpidarGDesc& desc = HISpidarGDesc());
	virtual ~HISpidarG();

	/// ボタンの状態を取得
	int GetButton(size_t i){
		if(buttons[i]) return buttons[i]->Get();
		else return 0;
	}
	/// ボタンの数
	size_t NButton() const { return buttons.size(); }

	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(const void* desc);
	virtual bool Calibration();
	Vec3f	GetPosition(){ return HISpidarCalc6Dof::GetPosition(); }
	Quaternionf GetOrientation(){ return HISpidarCalc6Dof::GetOrientation(); }
	virtual Vec3f GetTorque(){ return HISpidarCalc6Dof::GetTorque(); }
	virtual Vec3f GetForce(){ return HISpidarCalc6Dof::GetForce(); }
	virtual void SetForce(const Vec3f& f, const Vec3f& t = Vec3f()){ HISpidarCalc6Dof::SetForce(f, t); }		

	///	重み付けの設定
	void SetWeight(float s=0.3f, float t=1.0f, float r=6.0f){ smooth=s; lambda_t=t; lambda_r=r; }

	///	デバイスの状態を更新する.
	virtual void Update(float dt);

protected:
	virtual void MakeWireVec();					///<	ワイヤ方向ベクトルの計算
	virtual void UpdatePos();					///<	ワイヤ設置位置座標の更新
	virtual void MeasureWire();					///<	ワイヤ長の計測
};

}	//	namespace Spr
#endif


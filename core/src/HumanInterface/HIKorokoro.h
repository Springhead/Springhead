#ifndef HI_KOROKORO_H
#define HI_KOROKORO_H


#include <HumanInterface/HIKorokoroMotor.h>
#include <vector>
#include <Base/TMatrixUtility.h>
#include <Base/TMatrix.h>
#include<fstream>
#include<iostream>



namespace Spr {;


class HIKorokoro: public HIHaptic{
protected:
	std::string type;						///<	type of SPIDAR
	std::vector<HIKorokoroMotor>	motors;		///<	モータ
	Vec3d handTransform;
	

public:
	SPR_OBJECTDEF_ABST(HIKorokoro);

	HIKorokoro(const HIKorokoroDesc& desc = HIKorokoroDesc());
	virtual ~HIKorokoro();

	//const char* GetSpidarType() { return type.c_str(); }

	///	モータを取得
	HIKorokoroMotorIf* GetMotor(size_t i){ return motors[i].Cast(); }
	///	モータの数
	size_t NMotor() const { return motors.size(); }


	/// 全てのモータに対して最小出力を設定
	void SetLimitMinForce(float f);
	/// 全てのモータに対して最大出力を設定
	void SetLimitMaxForce(float f);
	///エンコーダの読み出し
	int counterR;
	int counterL;
	int counterT;
	double beforeR;
	double beforeL;
	double beforeT;
	double difR;
	double difL;
	double difT;
	Quaterniond difOri;

	float korokoroTimer;

	
	virtual bool Init(const void* desc);
	
    void HIKorokoro::SetForce(const Vec3f& Force, const Vec3f&);
	
	//void HIKorokoro::SetForce(Vec3d Force, Vec3d Pos);



	virtual void Update(float dt);
	//モーターを取得する値
	//Vec3f	GetForce();

	//テスト用バーチャルカップリングの力
	Vec3d Force = Vec3d(4,5,1);
	//手のtransform
	Vec3d handPos = Vec3d(0.1, 0.1, 0);
	//Quaterniond handRotation;
	Vec3d forward = Vec3d(0, 0, 1);
	Vec3d right = Vec3d(1, 0, 0);
	//棒の長さ
	float l1 = 1.36f;
	float l2 = 0.07f;
	float lh = 0.1f;
	float l3 = 0.63f;
	//モーターにかかる値
	float f1;
	float f2;
	float T;
	float vertical;
	//送る値
	Vec3d SendForce;
	//持ち手の重みに反比例する
	float kForMass = 10;

	float maxCommand = 512;

    float maxForwardForce = 1.7f;
	float maxPitchTorque = 0.357f;
	float pitchOffset = -0.119f;

	//バーチャルカップリングをコロコロ用に直した力
	Vec3f korokoroForce;
	Vec3d springForce;
	//コロコロSetPose用
	Vec3f		setPosition;
	Vec3f setPositionInit=Vec3d(0,0,0);
	Vec3f beforeSetPosition;
    Quaterniond setOrientation;
	void SetPose(Posed pose);
	//コロコロ	GetPose用
	virtual Vec3f		GetPosition() { return setPosition; }
	virtual Quaternionf GetOrientation() { return setOrientation; }
	//カルマンフィルタの配列
	//エンコーダの重み
	float kwheel = 1;
	float ktorque = 1;
	//PTM::TMatrixRow<3, 4, float> matH1;
	//typedef PTM::TMatrixRow<4, 4, float> Matrix4f;
	//Matrix4f I;
    //Matrix4f Q;
	Vec3f mixPos;
	Vec3f sensorPos;
	Vec3f counterPos;
	Vec3f temp;
	Quaterniond tempOri;
	Quaterniond beforeOri;
	Quaterniond mixOri;
	Quaterniond sensorOri;
	Quaterniond counterOri;

	
	
		
	



	 
 };



}	//	namespace Spr
#endif


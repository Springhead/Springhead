#ifndef HI_KOROKORO_H
#define HI_KOROKORO_H


#include <HumanInterface/HIKorokoroMotor.h>
#include <vector>
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
	float l2 = 0.17f;
	float l3 = 0.63f;
	//モーターにかかる値
	float f1;
	float f2;
	float T;
	//送る値
	Vec3d SendForce;
	//持ち手の重みに反比例する
	float kForMass = 0.001f;

	float maxCommand = 512;

    float maxForwardForce = 1.7f;
	float maxPitchTorque = 0.357f;
	float pitchOffset = -0.119f;

	//バーチャルカップリングをコロコロ用に直した力
	Vec3d korokoroForce;
	//コロコロSetPose用
	Vec3d		setPosition;
    Quaterniond setOrientation;
	void SetPose(Posed pose);
	//コロコロ	GetPose用
	Vec3f		GetPosition() { return setPosition; }
	Quaternionf GetOrientation() { return setOrientation; }



	 
};



}	//	namespace Spr
#endif


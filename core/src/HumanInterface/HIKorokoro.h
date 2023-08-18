#ifndef HI_KOROKORO_H
#define HI_KOROKORO_H


#include <HumanInterface/HIKorokoroMotor.h>
#include <vector>



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
	
	virtual bool Init(const void* desc);
	
	Vec3d CalculateHandForce(Vec3d f,Vec3d Pos);
	
	//HIKorokoro(const HIKorokoroDesc& desc) {}
	//virtual ~HIKorokoro(){ SetMinForce(); }

	virtual void Update(float dt);
	//モーターの送る値?
	//void SetForce();
	//モーターを取得する値
	//Vec3f	GetForce();

	//バーチャルカップリングの力
	Vec3d handForce = Vec3d(4,5,1);
	//手のtransform
	Vec3d handPos = Vec3d(1, 1, 1);
	Quaterniond handRotation;
	Vec3d forward = Vec3d(0, 0, 1);
	//棒の長さ
	float l1 = 1.36f;
	float l2 = 0.17f;
	float l3 = 0.63f;
	//送る値
	Vec3d SumForce;


	 
};



}	//	namespace Spr
#endif


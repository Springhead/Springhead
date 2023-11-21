#ifndef VR_HAPTIC_HDKOROKOROMOTOR_H
#define VR_HAPTIC_HDKOROKOROMOTOR_H

#include <Base/Affine.h>
#include <HumanInterface/HIDevice.h>
#include <HumanInterface/HIBase.h>
#include <HumanInterface/SprHIKorokoro.h>

namespace Spr {

class SPR_DLL HIKorokoroMotor:public HIBase, public HIKorokoroMotorDesc{
public:
	SPR_OBJECTDEF(HIKorokoroMotor);
	ACCESS_DESC(HIKorokoroMotor);

	//@{	デバイスアクセス
	///	使用するD/A.
	DVDaIf* da;
	///	使用するエンコーダのカウンタ.
	DVCounterIf* counter;
	///	出力した力
	float force;

	//@{	モータ定数(内部抵抗・逆起電力・コイル定数)のキャリブレーション
	float voltageInt;	///<	モータ端子間電圧の積分
	float currentInt;	///<	モータ電流の積分
	
public:
	///	
	HIKorokoroMotor();
	///	
	virtual ~HIKorokoroMotor(){}
	///	出力の設定
	void SetForce(float f){
		if (f < minForce) f = minForce;
		if (f > maxForce) f = maxForce;
		if(da) da->Voltage(f * voltPerNewton);
		force = f;
		//モーター側に送る値のLog
		std::fstream log("log_motorForce.txt", std::ios::app);
		log << "motorForce" << f * voltPerNewton << " called." << std::endl;
	}
	///	実際に出力した力の取得
	/*float GetForce(){
		return force;
	}*/
	/// 最小出力の設定
	void SetLimitMinForce(float f){
		minForce = f;
	}
	/// 最大出力の設定
	void SetLimitMaxForce(float f){
		maxForce = f;
	}
	/// 最小最大出力の取得
	Vec2f GetLimitForce(){
		Vec2f f;
		f.x = minForce; f.y = maxForce;
		return f;
	}
	///	電流値の取得
	float GetCurrent(){
		return force * voltPerNewton * currentPerVolt;
	}

	/// エンコーダのカウンタ値の読みだし
	int GetCount() {
		if (counter) return counter->Count();
		else return 0;
	}


};

}	//	namespace Spr
#endif

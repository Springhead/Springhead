
#ifndef SPR_HIKOROKORO_H
#define SPR_HIKOROKORO_H

#include <HumanInterface/SprHIBase.h>

namespace Spr{;


struct HIKorokoroMotorIf: public HIBaseIf{
	SPR_IFDEF(HIKorokoroMotor);

	/// 出力の設定
	void SetForce(float f);

	/// 最小出力の設定
	void SetLimitMinForce(float f);
	/// 最小出力の設定
	void SetLimitMaxForce(float f);
	///	電流値の取得
	float GetCurrent();
	
};
///	@name	モータについての定数
struct HIKorokoroMotorDesc{
	//@{
	/// ポート番号
	int	ch;
	///	最大出力(N)
	float maxForce;
	///	最小出力(N)
	float minForce;
	///	D/Aの出力電圧 / モータの出力  (V/N)
	float voltPerNewton;
	///	モータに流れる電流 / D/Aの出力電圧
	float currentPerVolt;
	///	糸の長さ / パルス数
	//float lengthPerPulse;
	///	モータが取り付けられている位置(糸の出口)の座標
	//Vec3f pos;
	///	糸がグリップに取り付けられている位置
	//Vec3f jointPos;

	HIKorokoroMotorDesc(){
		ch				= -1;
		maxForce		= 512.0f;
		minForce		= -512.0f;
		voltPerNewton	= 300.0f;
		currentPerVolt	= 1.0f;
		//lengthPerPulse	= 2.924062107079e-5f;
	}
};
struct HIKorokoroIf : public HIHapticIf{
	SPR_VIFDEF(HIKorokoro);
	///	Spidar's type (string to identify type of the SPIDAR
	//const char* GetKorokoroType();
	/// 最小出力の設定
	void SetLimitMinForce(float f);
	/// 最大出力の設定
	void SetLimitMaxForce(float f);
	///	モータ
	HIKorokoroMotorIf* GetMotor(size_t i);
	///	モータの数
	size_t NMotor() const;
	//コロコロに送る力
	Vec3d handForce = Vec3d(1, 1, 1);

};
struct HIKorokoroDesc{
	SPR_DESCDEF(HIKorokoro);
	std::string type;
	std::vector<HIKorokoroMotorDesc> motors;
	HIKorokoroDesc() {
		Init();
	}	
	void Init();
};


}

#endif

#ifndef HISPIDARCALC3DOF_H
#define HISPIDARCALC3DOF_H

#include "HISpidarCalcBase.h"

namespace Spr{;

/**	エンドエフェクタ(グリップ)が並進3自由度だけを持つSPIDAR用の計算クラス．
	糸の数は可変．	*/
class HISpidarCalc3Dof: public HISpidarCalcBase{
protected:
	Vec3f pos;						///<	グリップの位置
	int nRepeat;							///<	繰り返し回数
	std::vector<Vec3f> wireDirection;		///<	ワイヤ方向単位ベクトル（並進）

	float sigma;							///<	係数行列の対角成分に加える
public:
	HISpidarCalc3Dof();
	virtual ~HISpidarCalc3Dof();
	void Init(int dof, const VVector<float>& minF, const VVector<float>& maxF);

	virtual void Update();								///<	姿勢情報の更新
	virtual void SetForce(const Vec3f&);				///<	提示力の設定（並進力）
	virtual void SetForce(const Vec3f&, const Vec3f&);	///<	提示力の設定（並進．回転力は提示しない）
	Vec3f trnForce;		///	並進力
	float smooth;				///	二次形式　張力の２乗項の係数
	float lambda_t;				///	二次形式　並進項の係数
};

}	//	namespace Spr

#endif

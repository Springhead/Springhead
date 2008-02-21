#ifndef HISPIDARCALC6DOF_H
#define HISPIDARCALC6DOF_H

#include "HISpidarCalc3Dof.h"

namespace Spr{;

/**	6自由度のSPIDAR用の計算クラス．糸の本数は可変	*/
class HISpidarCalc6Dof : public HISpidarCalc3Dof {

protected:
	Quaternionf ori;
	std::vector<Vec3f> wireMoment;				///<	ワイヤ方向単位ベクトル（回転モーメント）

public:
	HISpidarCalc6Dof();
	virtual ~HISpidarCalc6Dof();
	///	初期化
	void Init(int dof, const VVector<float>& minF, const VVector<float>& maxF);
	void Update();								///<	姿勢情報の更新
	void SetForce(const Vec3f&);				///<	提示力の設定（並進のみ）
	void SetForce(const Vec3f&,const Vec3f&);	///<	提示力の設定（並進・回転力）
	Vec3f rotForce;								///<	回転力
	float lambda_r;								///<	二次形式　回転項の係数
	float* InputForTest(float,float,float,float*,float); //テスト用入力関数
};

}	//	namespace Spr
#endif

#ifndef PH_JOINT1D_H
#define PH_JOINT1D_H

#include "PHJoint.h"

namespace Spr{;

struct XJoint1D;

///	1自由度の関節
class PHJoint1D:public PHJointBase{
public:
	SGOBJECTDEFABST(PHJoint1D);
	double minPosition;						///<	可動範囲(最大値)
	double maxPosition;						///<	可動範囲(最小値)
	double torque;							///<	トルク
	double accel;							///<	関節加速度
	double position;						///<	変位
	double velocity;						///<	速度
	void AddTorque(double t){ torque+=t; }	///<	トルクを追加
	void SetTorque(double t){ torque=t; }	///<	トルクを設定
	double GetTorque(){ return torque; }	///<	トルクを取得
	double GetPosition(){ return position; }///<	関節角度を取得
	double GetVelocity(){ return velocity; }///<	関節速度を取得
	virtual void Integrate(SGScene* scene);	///<	積分
	void CompArticulatedInertia(double dt);	///<	articulated inertia & ZA-force
	void CalcAccel(double dt);				///<	このジョイントの加速度計算．詳細は基本クラスのコメントを参照．

	///	関節の自由度
	virtual int GetJointDof(){ return 1; }
	///	関節位置の取得
	virtual double GetJointPosition(int i){ return position; }
	///	関節速度の取得
	virtual double GetJointVelocity(int i){ return velocity; }
	///	関節加速度の取得
	virtual double GetJointAccel(int i){ return accel; }
	///	関節トルクの取得
	virtual double GetJointTorque(int i){ return torque; }
	///	関節トルクの設定
	virtual void SetJointTorque(double v, int i){ torque = v; }
	///	関節トルクを加える
	virtual void AddJointTorque(double v, int i){ torque += v; }

	void SaveX(XJoint1D&) const;
	void LoadX(const XJoint1D&);
protected:
	SpVec6d			s;					///<	spatial joint axis in Fc coord.
	//	sを含むキャッシュ変数
	SpVec6d			Ia_s;
	double			dot_s_Ia_s, dot_s_Z_plus_Ia_c;

	virtual void Reset();
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
	
	virtual void ClearTorqueRecursive();
	virtual void LimitAngle(double& d){}
};

///	蝶番関節
class PHJointHinge:public PHJoint1D{
protected:
	double massFactor;
public:
	SGOBJECTDEF(PHJointHinge);
	virtual void CompJointAxis();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompCoriolisAccel();
protected:
	virtual void Loaded(SGScene* s);
	virtual double MassFactor();
	virtual void LimitAngle(double& t){
		if (!_finite(t) || t > 1e6 || t < -1e6){
			if (t>0) t = (double)M_PI;
			else t= - (double)M_PI;
			return;
		}
		int times;
		if (t > M_PI){
			times = int((t+M_PI) / (2*M_PI));
		}else if (t < -M_PI){
			times = int((t-M_PI) / (2*M_PI));
		}else{
			return;
		}
		t -= (double)(times*(2*M_PI));
		if (t < -M_PI || M_PI <= t){
			DSTR << t << std::endl;
			assert(0);
		}
	}
};
///	スライド関節
class PHJointSlider:public PHJoint1D{
protected:
	double massFactor;
public:
	SGOBJECTDEF(PHJointSlider);
	virtual void Loaded(SGScene* s);
	virtual double MassFactor();
	virtual void CompJointAxis();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompCoriolisAccel();
};

}

#endif
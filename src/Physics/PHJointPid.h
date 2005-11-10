#ifndef PH_JOINTPID_H
#define PH_JOINTPID_H

#include <Physics/PHJoint1D.h>
#include <Physics/PHJointMulti.h>

namespace Spr{;

class PHJointPid:public SGBehaviorEngine{
protected:
	///	前回の目標値
	float lastGoal;
	///	積分
	float integratedError;
public:
	SGOBJECTDEF(PHJointPid);
	///	制御対象関節
	UTRef<PHJointBase> joint;
	///	対象の軸
	int axis;
	///	目標値		Step()を呼ぶ前に設定する．
	float goal;
	///	目標の速度	Step()を呼ぶ前に設定する．
	float dGoal;
	///	PIDの係数
	float proportional, differential, integral;
	///	目標のタイプ 0:位置制御	1：速度制御
	int type;
	/// proportional,differential,integralによるトルク(PID制御)
	float pTorque, dTorque, iTorque;

	/// p_torque, d_torque,i_torqueの取得
	float GetProportionalTorque(){ return pTorque; }
	float GetDifferentialTorque(){ return dTorque; }
	float GetIntegralTorque(){ return iTorque; }

	///	目標値,目標の速度	Step()を呼ぶ前に設定する
	void SetPGoal(float p_goal){ goal = p_goal;}
	void SetDGoal(float d_goal){ dGoal = d_goal;}
	
	///	
	PHJointPid():type(0), proportional(0), differential(0), integral(0), goal(0), dGoal(FLT_MAX), lastGoal(0), integratedError(0), axis(0){}
	///
	void Print(std::ostream& os) const;
	///
	static PHJointPid* Find(PHJoint1D* j, SGScene* scene);
	///	プライオリティ
	virtual int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	シーンをクリアするときの処理
	virtual void Clear(SGScene* s){ joint=NULL; }

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
	///	PID制御をかける．
	void Step(SGScene* s);	

	///
	double GetJointTorque(){ return joint->GetJointTorque(axis); }

	size_t NReferenceObjects();
	SGObject* ReferenceObject(size_t i);
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
protected:
};



/*	可動域について
	ニュートラルをまっすぐとして，
	- θ・φのグラフで可動域を表す．折れ線近似．
	- 最後の軸は,定数 or 中心と θ・φの設定点について設定．
	- θ・φは違反したら，最寄のθ・φに向かってPID制御．
コーンで良い説．
コーンの中心の向きVec3f と角度θだけでよい．
*/
class PHJointBallPid:public SGBehaviorEngine{
protected:
	///	前回のゴール
	Quaterniond lastGoal;
	///	積分
	Vec3f integratedError;
public:
	SGOBJECTDEF(PHJointBallPid);
	///	制御対象関節
	UTRef<PHJointBall> joint;
	///	目標値			Step()を呼ぶ前に設定する．
	Quaternionf goal;
	///	目標値の微分	Step()を呼ぶ前に設定する．設定しなくても良い．
	Vec3f dGoal;
	///	PIDの係数
	float proportional, differential, integral;
	/// proportional,differential,integralによるトルク(PID制御)
	Vec3f pTorque, dTorque, iTorque;

	/// p_torque, d_torque,i_torqueの取得
	Vec3f GetProportionalTorque(){ return pTorque; }
	Vec3f GetDifferentialTorque(){ return dTorque; }
	Vec3f GetIntegralTorque(){ return iTorque; }
	
	///	
	PHJointBallPid():proportional(0), differential(0), integral(0), dGoal(FLT_MAX,0,0){}
	///
	static PHJointBallPid* Find(PHJointBall* j, SGScene* scene);
	///	プライオリティ
	virtual int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	シーンをクリアするときの処理
	virtual void Clear(SGScene* s){ joint=NULL; }

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
	///	PID制御をかける．
	void Step(SGScene* s);

	///
	Vec3f GetJointTorque(){ return joint->GetTorque(); }

	size_t NReferenceObjects();
	SGObject* ReferenceObject(size_t i);
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);	

	virtual void Print(std::ostream& os) const;

};


}

#endif
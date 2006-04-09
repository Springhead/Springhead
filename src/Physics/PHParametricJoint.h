#ifndef PHPARAMETRICJOINT_H
#define PHPARAMETRICJOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

namespace Spr{;

/*	2つの剛体間の相対位置・姿勢を返す関数
		[p; q] = f(s)
	があるとする．ここでpは相対位置, qは相対quaternion，sは一般化座標を表わすスカラー．
	微分して
		[p'; q'] = (df/ds)(s)s'．
	quaternionの時間微分から角速度を得る行列をE(q)とすると
		[v; w] = [I O; O E(q(s))](df/ds)(s)s' =: J(s)s'
	上の式よりsとs'が決まれば相対速度[v; w]が決まる．
	これを拘束条件の形に変える．6次元ベクトルJ(s)は相対速度の向きを表わしているので，
	これに直交する行列をJ~(s)とすると，拘束条件は
		J~(s)[v; w] = 0

	Correctionは，
	velocity update後の相対位置に最も近い軌道im(f)上の点を取得，その点に合わせる．
	本当の拘束条件は[p; q] in im(f)だが非線形なので扱えない．
 */

/// 関節の軌道のキーフレームを持ち，補完して返したりヤコビアンを計算したりするクラス
struct PHJointTrajectoryPoint{
	double		q;
	Posed		pose;
	Matrix6d	J;
};
class PHJointTrajectory : public std::vector<PHJointTrajectoryPoint>, public Object{
	iterator Find(double q);
	bool bRevolutive;	//[-pi, pi]の無限回転関節
public:
	void SetRevolutive(bool OnOff = true){bRevolutive = OnOff;}
	bool IsRevolutive(){return bRevolutive;}
	void AddPoint(const Posed& pose, double q);
	void CreateDB();
	void GetPose(Posed& pose, double q);
	void GetJacobian(Matrix6d& J, double q);
};

class PHParametricJoint : public InheritJoint1D<PHParametricJointIf, PHJoint1D>{
	UTRef<PHJointTrajectory> trajectory;
public:
	double	q, qd;				// 関節変位と関節角度
	//Matrix53d	Jcvrel, Jcwrel;	// 相対速度から拘束速度，相対角速度から拘束速度へのヤコビ行列

	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::PARAMETRICJOINT;}
	virtual double GetPosition();
	virtual double GetVelocity();
	virtual void CompConstraintJacobian();
	virtual void CompBias(double dt);
	virtual void CompError(double dt);
	virtual void ProjectionDynamics(double& f, int k);
	virtual void ProjectionCorrection(double& F, int k);
	PHParametricJoint();
};

}

#endif
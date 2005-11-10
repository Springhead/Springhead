#ifndef PHINTERSECTION_H
#define PHINTERSECTION_H
#include <Collision/CDFramePair.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/PHSolid.h>

namespace Spr{;

class PHConvexPair:public CDConvexPairWithNormal, public CDContactAnalysis{
public:
	//	抗力の計算
	float area;							///<	交差部の面積
	Vec3f reflexSpringForce;			///<	ばねによる抗力
	Vec3f reflexDamperForce;			///<	ダンパーによる抗力
	Vec3f reflexSpringTorque;			///<	ばねによる抗トルク
	Vec3f reflexDamperTorque;			///<	ダンパーによる抗トルク
	Vec3f reflexForcePoint;				///<	抗力の作用点
	
	//	摩擦の計算
	Vec3f transFrictionBase[2];			///<	並進静止摩擦用バネの端点
	Vec3f rotFricSpring;				///<	回転静止摩擦用バネの伸び
	Quaternionf lastOri01;
	Vec3f dynaFric;						///<	動摩擦力
	Vec3f dynaFricMom;					///<	動摩擦力のモーメント(原点回り)
	Vec3f frictionForce;				///<	摩擦力
	Vec3f frictionTorque;				///<	摩擦トルク(摩擦力の作用点が原点のときの)

	//	係数
	float rk, rb, fk, fb;

	/**	交差部分が発生する抗力を求める．
		ペナルティとそのモーメント，相対速度とそのモーメントを求める．	*/
	void CalcReflexForce(Vec3f* cog, Vec3f* vel, Vec3f* angVel, float k, float b);
	///	三角形1つが発生するペナルティとそのモーメント，相対速度とそのモーメントを求める．
	void CalcTriangleReflexForce(Vec3f p, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel, float k, float b);
	///	摩擦力を求める．	今のところ動摩擦の最大値がいいかげんでコマが回せない．
	void CalcFriction(Quaternionf* ori, Vec3f* pos, Vec3f* cog, Vec3f* vel, Vec3f* angVel, float k, float b);

	/**	交差部分が発生する抗力を求める．
		ペナルティとそのモーメント，相対速度とそのモーメントを求める．
		反力と摩擦力のばね・ダンパ係数を別々に分けた．　*/
	void CalcReflexForce(Vec3f* cog, Vec3f* vel, Vec3f* angVel, float rk, float rb, float fk, float fb);
	/**	三角形1つが発生するペナルティとそのモーメント，相対速度とそのモーメントを求める．
		反力と摩擦力のばね・ダンパ係数を別々に分けた．	*/
	void CalcTriangleReflexForce(Vec3f p, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel, float rk, float rb, float fk, float fb);
};
class PHGeometryPair:public CDGeometryPair{
};

};
#endif

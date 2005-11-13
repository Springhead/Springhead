#ifndef SPR_PHSolidIf_H
#define SPR_PHSolidIf_H
#include <Foundation/SprObject.h>

namespace Spr{;

struct PHSolidDesc{
	double		mass;			///<	質量
	Matrix3d	inertia;		///<	慣性テンソル
	Vec3d		velocity;		///<	速度			(World)
	Vec3d		angVelocity;	///<	角速度			(World)
	Vec3d		center;			///<	質量中心の位置	(Local..frameのposture系)
	Vec3d		position;		///<	位置			(World)
	Quaterniond orientation;	///<	向き			(World)

	PHSolidDesc(){ Init(); }
	void Init(){
		mass = 1.0f;
		inertia=Matrix3d::Unit();
	}
};

///	剛体
struct PHSolidIf : public ObjectIf{
	virtual void AddForce(Vec3d f)=0;					///< 力を質量中心に加える
	virtual void AddTorque(Vec3d t)=0;					///< トルクを加える
	virtual void AddForce(Vec3d f, Vec3d r)=0;			///< 力を 位置r(World系) に加える

	virtual double		GetMass()=0;							///< 質量
	virtual double		GetMassInv()=0;						///< 質量の逆数
	virtual void		SetMass(double m)=0;						///< 質量の設定
	virtual Matrix3d	GetInertia()=0;					///< 慣性テンソル

	virtual Vec3d		GetFramePosition() const =0;
	virtual void		SetFramePosition(const Vec3d& p) =0;
	virtual Vec3d		GetCenterPosition() const  =0;
	virtual void		SetCenterPosition(const Vec3d& p) =0;

	///	向きの取得
	virtual Quaterniond GetOrientation() const =0;
	///	向きの設定
	virtual void		SetOrientation(const Quaterniond& q) =0;

	///	質量中心の速度の取得
	virtual Vec3d		GetVelocity() const  =0;
	///	質量中心の速度の設定
	virtual void		SetVelocity(const Vec3d& v) =0;

	///	角速度の取得
	virtual Vec3d		GetAngularVelocity() const =0;
	///	角速度の設定
	virtual void		SetAngularVelocity(const Vec3d& av)=0;
};

}	//	namespace Spr
#endif

#ifndef SPR_PHSolidIf_H
#define SPR_PHSolidIf_H
#include <Foundation/SprObject.h>

/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;
///	剛体のステート
struct PHSolidState{
	Vec3d		velocity;		///<	速度			(World系)
	Vec3d		angVelocity;	///<	角速度			(World系)
	Vec3d		center;			///<	質量中心の位置	(Local系)
	Posed		pose;			///<	位置と向き		(World系)

	Vec3d		force;			///<	前の積分でこの剛体に加わった力(World系)
	Vec3d		torque;			///<	前の積分でこの剛体に加わったトルク(World系)
	Vec3d		nextForce;		///<	次の積分でこの剛体に加わる力(World系)
	Vec3d		nextTorque;		///<	次の積分でこの剛体に加わるトルク(World系)
};
///	剛体のディスクリプタ
struct PHSolidDesc: public PHSolidState{
	double		mass;			///<	質量
	Matrix3d	inertia;		///<	慣性テンソル	(Local系)
	bool		gravity;		///<	重力の影響を受けるか

	PHSolidDesc(){ Init(); }
	void Init(){
		mass = 1.0f;
		inertia=Matrix3d::Unit();
		gravity=true;
	}
};

struct CDShapeIf; 
///	剛体
struct PHSolidIf : public SceneObjectIf{
	IF_DEF(PHSolid);
	virtual void AddForce(Vec3d f)=0;					///< 力を質量中心に加える
	virtual void AddTorque(Vec3d t)=0;					///< トルクを加える
	virtual void AddForce(Vec3d f, Vec3d r)=0;			///< 力を 位置r(World系) に加える

	virtual double		GetMass()=0;					///< 質量
	virtual double		GetMassInv()=0;					///< 質量の逆数
	virtual void		SetMass(double m)=0;			///< 質量の設定
	virtual Matrix3d	GetInertia()=0;					///< 慣性テンソル
	virtual void		SetInertia(const Matrix3d& I)=0;

	virtual Vec3d		GetFramePosition() const =0;
	virtual void		SetFramePosition(const Vec3d& p) =0;
	virtual Vec3d		GetCenterPosition() const  =0;
	virtual void		SetCenterPosition(const Vec3d& p) =0;

	///	向きの取得
	virtual Quaterniond GetOrientation() const =0;
	///	向きの設定
	virtual void		SetOrientation(const Quaterniond& q) =0;

	/// 姿勢の取得
	virtual Posed		GetPose() const = 0;
	/// 姿勢の設定
	virtual void		SetPose(const Posed& p) = 0;

	///	質量中心の速度の取得
	virtual Vec3d		GetVelocity() const  =0;
	///	質量中心の速度の設定
	virtual void		SetVelocity(const Vec3d& v) =0;

	///	角速度の取得
	virtual Vec3d		GetAngularVelocity() const =0;
	///	角速度の設定
	virtual void		SetAngularVelocity(const Vec3d& av)=0;

	///	形状の追加
	virtual void		AddShape(CDShapeIf* shape)=0;

	/// 形状の位置の取得
	virtual Posed		GetShapePose(int pos)=0;

	/// 形状の位置の設定
	virtual void		SetShapePose(int pos, const Posed& pose)=0;

	///	重力を有効/無効化する	ここでよいか疑問
	virtual void		SetGravity(bool bOn)=0;
	/// 動力学法則に従うかどうかを有効/無効化する
	virtual void		SetDynamical(bool bOn)=0;
	virtual bool		IsDynamical()=0;

	/// 登録されている形状の個数を取得
	virtual int			NShape()=0;

	///	形状を取得
	virtual CDShapeIf*	GetShape(int i)=0;

};

//@}

}	//	namespace Spr
#endif

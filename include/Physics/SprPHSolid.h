#ifndef SPR_PHSolidIf_H
#define SPR_PHSolidIf_H
#include <Foundation/SprObject.h>

namespace Spr{;
struct PHSolidState{
	Vec3d		velocity;		///<	‘¬“x			(WorldŒn)
	Vec3d		angVelocity;	///<	Šp‘¬“x			(WorldŒn)
	Vec3d		center;			///<	Ž¿—Ê’†S‚ÌˆÊ’u	(LocalŒn)
	Posed		pose;			///<	ˆÊ’u‚ÆŒü‚«		(WorldŒn)

	Vec3d		force;			///<	‘O‚ÌÏ•ª‚Å‚±‚Ì„‘Ì‚É‰Á‚í‚Á‚½—Í(WorldŒn)
	Vec3d		torque;			///<	‘O‚ÌÏ•ª‚Å‚±‚Ì„‘Ì‚É‰Á‚í‚Á‚½ƒgƒ‹ƒN(WorldŒn)
	Vec3d		nextForce;		///<	ŽŸ‚ÌÏ•ª‚Å‚±‚Ì„‘Ì‚É‰Á‚í‚é—Í(WorldŒn)
	Vec3d		nextTorque;		///<	ŽŸ‚ÌÏ•ª‚Å‚±‚Ì„‘Ì‚É‰Á‚í‚éƒgƒ‹ƒN(WorldŒn)
};
struct PHSolidDesc: public PHSolidState{
	double		mass;			///<	Ž¿—Ê
	Matrix3d	inertia;		///<	Šµ«ƒeƒ“ƒ\ƒ‹	(LocalŒn)

	PHSolidDesc(){ Init(); }
	void Init(){
		mass = 1.0f;
		inertia=Matrix3d::Unit();
	}
};

struct CDShapeIf; 
///	„‘Ì
struct PHSolidIf : public SceneObjectIf{
	IF_DEF(PHSolid);
	virtual void AddForce(Vec3d f)=0;					///< —Í‚ðŽ¿—Ê’†S‚É‰Á‚¦‚é
	virtual void AddTorque(Vec3d t)=0;					///< ƒgƒ‹ƒN‚ð‰Á‚¦‚é
	virtual void AddForce(Vec3d f, Vec3d r)=0;			///< —Í‚ð ˆÊ’ur(WorldŒn) ‚É‰Á‚¦‚é

	virtual double		GetMass()=0;							///< Ž¿—Ê
	virtual double		GetMassInv()=0;						///< Ž¿—Ê‚Ì‹t”
	virtual void		SetMass(double m)=0;						///< Ž¿—Ê‚ÌÝ’è
	virtual Matrix3d	GetInertia()=0;					///< Šµ«ƒeƒ“ƒ\ƒ‹

	virtual Vec3d		GetFramePosition() const =0;
	virtual void		SetFramePosition(const Vec3d& p) =0;
	virtual Vec3d		GetCenterPosition() const  =0;
	virtual void		SetCenterPosition(const Vec3d& p) =0;

	///	Œü‚«‚ÌŽæ“¾
	virtual Quaterniond GetOrientation() const =0;
	///	Œü‚«‚ÌÝ’è
	virtual void		SetOrientation(const Quaterniond& q) =0;

	/// Žp¨‚ÌŽæ“¾
	virtual Posed		GetPose() const = 0;
	/// Žp¨‚ÌÝ’è
	virtual void		SetPose(const Posed& p) = 0;

	///	Ž¿—Ê’†S‚Ì‘¬“x‚ÌŽæ“¾
	virtual Vec3d		GetVelocity() const  =0;
	///	Ž¿—Ê’†S‚Ì‘¬“x‚ÌÝ’è
	virtual void		SetVelocity(const Vec3d& v) =0;

	///	Šp‘¬“x‚ÌŽæ“¾
	virtual Vec3d		GetAngularVelocity() const =0;
	///	Šp‘¬“x‚ÌÝ’è
	virtual void		SetAngularVelocity(const Vec3d& av)=0;

	///	Œ`ó‚Ì’Ç‰Á
	virtual void		AddShape(CDShapeIf* shape)=0;

	///	d—Í‚ð—LŒø/–³Œø‰»‚·‚é	‚±‚±‚Å‚æ‚¢‚©‹^–â
	virtual void		SetGravity(bool bOn)=0;

	virtual int			GetNShapes()=0;
	///	Œ`ó‚ðŽæ“¾
	virtual CDShapeIf**	GetShapes()=0;
};

}	//	namespace Spr
#endif

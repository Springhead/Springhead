/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRBODY_H
#define CRBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>

#include <Physics/PHIKActuator.h>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディの構成要素
*/
class CRBodyPart : public SceneObject, public CRBodyPartDesc {
protected:
public:
	SPR_OBJECTDEF(CRBodyPart);
	// ACCESS_DESC(CRBodyPart);

	CRBodyPart(){}
	CRBodyPart(const CRBodyPartDesc& desc) 
		: CRBodyPartDesc(desc)
	{
	}

	/** @brief ラベル（役割を示す文字列：Handなど）の取得
	 */
	const char* GetLabel() const { return label.c_str(); }

	/** @brief ラベル（役割を示す文字列：Handなど）の設定
	 */
	void SetLabel(const char* str) { label = std::string(str); }
};

class CRSolid : public CRBodyPart, public CRSolidDesc {
protected:
	PHSolidIf* solid;

public:
	SPR_OBJECTDEF(CRSolid);
	ACCESS_DESC(CRSolid);

	CRSolid()
		: solid(NULL)
	{
	}
	CRSolid(const CRSolidDesc& desc) 
		: solid(NULL), CRSolidDesc(desc), CRBodyPart(desc)
	{
	}

	/** @brief PHSolidを取得
	 */
	PHSolidIf* GetPHSolid() { return solid; }

	/** @brief PHSolidを設定
	 */
	void SetPHSolid(PHSolidIf* so) { solid = so; }

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { if (solid) { return 1; } else { return 0; } }
	virtual ObjectIf* GetChildObject(size_t i) { if (i==0) { return solid; } else { return NULL; } }
	virtual bool AddChildObject(ObjectIf* o) { solid = o->Cast(); return(solid!=NULL); }
	virtual bool DelChildObject(ObjectIf* o) { if (o==solid) { solid = NULL; return true; } return false; }
};

class CRIKSolid : public CRSolid, public CRIKSolidDesc {
protected:
	PHIKEndEffectorIf *ikEndEffector;

public:
	SPR_OBJECTDEF(CRIKSolid);
	ACCESS_DESC(CRIKSolid);

	CRIKSolid()
		: ikEndEffector(NULL)
	{
	}
	CRIKSolid(const CRIKSolidDesc& desc) 
		: ikEndEffector(NULL), CRIKSolidDesc(desc), CRSolid(desc)
	{
	}

	/** @brief IKエンドエフェクタを取得
	 */
	PHIKEndEffectorIf* GetIKEndEffector() {
		return ikEndEffector;
	}

	/** @brief IKエンドエフェクタを設定
	 */
	void SetIKEndEffector(PHIKEndEffectorIf* ikEE) {
		ikEndEffector = ikEE;
	}

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const {
		return( (solid ? 1 : 0) + (ikEndEffector ? 1 : 0) );
	}

	virtual ObjectIf* GetChildObject(size_t i) {
		if (solid) {
			if (i==0) {
				return solid;
			} else if (i==1) {
				return ikEndEffector;
			}
		} else {
			if (i==0) {
				return ikEndEffector;
			}
		}
		return NULL;
	}

	virtual bool AddChildObject(ObjectIf* o) {
		PHSolidIf* so = o->Cast();
		if (so) { solid = so; return true; }

		PHIKEndEffectorIf* ikef = o->Cast();
		if (ikef) { ikEndEffector = ikef; return true; }
		
		return false;
	}

	virtual bool DelChildObject(ObjectIf* o) {
		PHSolidIf* so = o->Cast();
		if (so && so==solid) { solid = NULL; return true; }

		PHIKEndEffectorIf* eef = o->Cast();
		if (eef && eef==ikEndEffector) { ikEndEffector = NULL; return true; }

		return false;
	}
};

class CRJoint : public CRBodyPart, public CRJointDesc {
protected:
	PHJointIf* joint;
	double spring, damper;

public:
	SPR_OBJECTDEF(CRJoint);
	ACCESS_DESC(CRJoint);

	CRJoint()
		: joint(NULL), spring(-1), damper(-1)
	{
	}
	CRJoint(const CRJointDesc& desc) 
		: joint(NULL), CRJointDesc(desc), CRBodyPart(desc), spring(-1), damper(-1)
	{
	}

	/** @brief PHJointを取得
	 */
	PHJointIf* GetPHJoint() { return joint; }

	/** @brief PHJointを設定
	 */
	void SetPHJoint(PHJointIf* jo) { joint = jo; }

	/** @brief バネダンパ係数の倍数を設定
	 */
	virtual void SetSpringRatio(double springRatio, double damperRatio) {
		if (PHBallJointIf* bj = joint->Cast()) {
			if (spring < 0) {
				spring = bj->GetSpring();
			}
			if (damper < 0) {
				damper = bj->GetDamper();
			}
			bj->SetSpring(spring*springRatio); bj->SetDamper(damper*damperRatio);
			std::cout << "bj_ssr : " << spring * springRatio << std::endl;
		}
		if (PHHingeJointIf* hj = joint->Cast()) {
			if (spring < 0) {
				spring = hj->GetSpring();
			}
			if (damper < 0) {
				damper = hj->GetDamper();
			}
			hj->SetSpring(spring*springRatio); hj->SetDamper(damper*damperRatio);
			std::cout << "hj_ssr : " << spring * springRatio << std::endl;
		}
	}

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { if (joint) { return 1; } else { return 0; } }
	virtual ObjectIf* GetChildObject(size_t i) { if (i==0) { return joint; } else { return NULL; } }
	virtual bool AddChildObject(ObjectIf* o) { joint = o->Cast(); return(joint!=NULL); }
	virtual bool DelChildObject(ObjectIf* o) { if (o==joint) { joint = NULL; return true; } return false; }
};

class CRIKJoint : public CRJoint, public CRIKJointDesc {
protected:
	PHIKActuatorIf* ikActuator;
	double ikSpring, ikDamper;

public:
	SPR_OBJECTDEF(CRIKJoint);
	ACCESS_DESC(CRIKJoint);

	CRIKJoint()
		: ikActuator(NULL), ikSpring(-1), ikDamper(-1)
	{
	}
	CRIKJoint(const CRIKJointDesc& desc) 
		: ikActuator(NULL), CRIKJointDesc(desc), CRJoint(desc), ikSpring(-1), ikDamper(-1)
	{
	}

	/** @brief IKアクチュエータを取得
	 */
	PHIKActuatorIf* GetIKActuator() { return ikActuator; }

	/** @brief IKアクチュエータを設定
	 */
	void SetIKActuator(PHIKActuatorIf* ikAct) { ikActuator = ikAct; ikSpring = ikAct->GetSpring(); ikDamper = ikAct->GetDamper(); }

	/** @brief バネダンパ係数の倍数を設定
	 */
	virtual void SetSpringRatio(double springRatio, double damperRatio) {
		if (PHBallJointIf* bj = joint->Cast()) {
			if (spring < 0) {
				spring = bj->GetSpring();
			}
			if (damper < 0) {
				damper = bj->GetDamper();
			}
			bj->SetSpring(spring*springRatio); bj->SetDamper(damper*damperRatio);
			DCAST(PHIKBallActuator,ikActuator)->jSpring = spring*springRatio;
			DCAST(PHIKBallActuator,ikActuator)->jDamper = damper*damperRatio;
			std::cout << "bj_ssr : " << spring * springRatio << std::endl;
		}
		if (PHHingeJointIf* hj = joint->Cast()) {
			if (spring < 0) {
				spring = hj->GetSpring();
			}
			if (damper < 0) {
				damper = hj->GetDamper();
			}
			hj->SetSpring(spring*springRatio); hj->SetDamper(damper*damperRatio);
			DCAST(PHIKHingeActuator,ikActuator)->jSpring = spring*springRatio;
			DCAST(PHIKHingeActuator,ikActuator)->jDamper = damper*damperRatio;
			std::cout << "hj_ssr : " << spring * springRatio << std::endl;
		}
	}

	/** @brief バネダンパ係数の倍数を設定
	 */
	void SetIKSpringRatio(double springRatio, double damperRatio) {
		if (ikSpring < 0) {
			ikSpring = ikActuator->GetSpring();
		}
		if (ikDamper < 0) {
			ikDamper = ikActuator->GetDamper();
		}
		ikActuator->SetSpring(ikSpring*springRatio); ikActuator->SetDamper(ikDamper*damperRatio);
	}

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const {
		return( (joint ? 1 : 0) + (ikActuator ? 1 : 0) );
	}

	virtual ObjectIf* GetChildObject(size_t i) {
		if (joint) {
			if (i==0) {
				return joint;
			} else if (i==1) {
				return ikActuator;
			}
		} else {
			if (i==0) {
				return ikActuator;
			}
		}
		return NULL;
	}

	virtual bool AddChildObject(ObjectIf* o) {
		PHJointIf* jo = o->Cast();
		if (jo) { joint = jo; return true; }

		PHIKActuatorIf* ikact = o->Cast();
		if (ikact) { ikActuator = ikact; return true; }
		
		return false;
	}

	virtual bool DelChildObject(ObjectIf* o) {
		PHJointIf* jo = o->Cast();
		if (jo && jo==joint) { joint = NULL; return true; }

		PHIKActuatorIf* act = o->Cast();
		if (act && act==ikActuator) { ikActuator = NULL; return true; }

		return false;
	}
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ
*/
class CRBody : public SceneObject, public CRBodyDesc {
protected:
	/// ボディを構成する剛体
	typedef std::vector< UTRef<CRSolidIf> > CRSolids;
	CRSolids solids;	
	
	/// ボディを構成する関節
	typedef std::vector< UTRef<CRJointIf> > CRJoints;
	CRJoints joints;

public:
	SPR_OBJECTDEF(CRBody);
	ACCESS_DESC(CRBody);

	CRBody(){}
	CRBody(const CRBodyDesc& desc) 
		: CRBodyDesc(desc)
	{
	}

	/** @brief 剛体の数を得る
	*/
	virtual int	NSolids() { return solids.size(); }

	/** @brief i番目の剛体を得る
	*/
	virtual CRSolidIf* GetSolid(int i) { return solids[i]; }

	/** @brief 関節の数を得る
	*/
	virtual int NJoints() { return joints.size(); }

	/** @brief i番目の関節を得る
	*/
	virtual CRJointIf* GetJoint(int i) { return joints[i]; }

	/** @brief ボディの重心座標を得る
	*/
	virtual Vec3d GetCenterOfMass();
	
	/** @brief ボディの質量を得る
	*/
	virtual double GetSumOfMass();

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { return solids.size()+joints.size(); }
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);

	virtual Matrix3d CalcBoxInertia(Vec3d boxsize, double mass);
};
}
//@}

#endif//CRBODY_H

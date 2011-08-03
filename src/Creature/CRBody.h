/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRBODY_H
#define CRBODY_H

#include <Foundation/Object.h>
#include <Creature/SprCRBody.h>

#include <map>

//@{
namespace Spr{;

struct PHIKActuatorIf;

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
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
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
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
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
	virtual void SetSpringRatio(double springRatio, double damperRatio);
	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
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
	void SetIKActuator(PHIKActuatorIf* ikAct);

	/** @brief バネダンパ係数の倍数を設定
	 */
	virtual void SetSpringRatio(double springRatio, double damperRatio);

	/** @brief バネダンパ係数の倍数を設定
	 */
	void SetIKSpringRatio(double springRatio, double damperRatio);

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
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

	/// ラベルから構成要素へのマップ
	typedef std::map<UTString, CRBodyPartIf*> LabelMap;
	LabelMap labelMap;

public:
	SPR_OBJECTDEF(CRBody);
	ACCESS_DESC(CRBody);

	CRBody(){}
	CRBody(const CRBodyDesc& desc) 
		: CRBodyDesc(desc)
	{
	}

	/** @brief ラベルから構成要素を探す
	*/
	virtual CRBodyPartIf* FindByLabel(UTString label);

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

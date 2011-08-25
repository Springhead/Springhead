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
#include <Creature/SprCRBodyPart.h>

#include <map>

//@{
namespace Spr{;

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

	/** @brief ボディに関するステップ処理を行う
	*/
	virtual void Step();

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

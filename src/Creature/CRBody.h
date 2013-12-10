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
#include <Creature/SprCRBone.h>

#include <map>

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ
*/
class CRBody : public SceneObject, public CRBodyDesc {
protected:
	/// ボディを構成するボーン
	typedef std::vector< UTRef<CRBoneIf> > CRBones;
	CRBones bones;	
	
	/// ラベルから構成要素へのマップ
	typedef std::map<UTString, CRBoneIf*> LabelMap;
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
	virtual CRBoneIf* FindByLabel(UTString label);

	/** @brief あるIKActuatorを持つ構成要素を探す
	*/
	virtual CRBoneIf* FindByIKActuator(PHIKActuatorIf* actuator) {
		for (size_t i=0; i<bones.size(); ++i) {
			if (bones[i]->GetIKActuator()!=NULL && bones[i]->GetIKActuator()==actuator) {
				return bones[i];
			}
		}
		return NULL;
	}

	/** @brief ボーンの数を得る
	*/
	virtual int	NBones() { return (int)bones.size(); }

	/** @brief i番目のボーンを得る
	*/
	virtual CRBoneIf* GetBone(int i) {
		if (0<=i && i<(int)bones.size()) {
			return bones[i];
		} else {
			return NULL;
		}
	}

	/** @brief ボディの重心座標を得る
	*/
	virtual Vec3d GetCenterOfMass();
	
	/** @brief ボディの質量を得る
	*/
	virtual double GetSumOfMass();

	/** @brief １ステップ
	*/
	void Step() {}

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { return bones.size(); }
	virtual ObjectIf* GetChildObject(size_t i) { return GetBone((int)i); }
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);

	virtual Matrix3d CalcBoxInertia(Vec3d boxsize, double mass);
};
}
//@}

#endif//CRBODY_H

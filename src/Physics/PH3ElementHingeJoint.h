/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 *  PHHingeJointはswingによる倒しこみ角度の制限と、twistによるねじりの制限ができます。
 *
 */
#ifndef PH3ELEMENTHINGEJOINT_H
#define PH3ELEMENTHINGEJOINT_H

#include <SprPhysics.h>
#include <Physics/PHHingeJoint.h>

namespace Spr{

class PH3ElementHingeJoint : public PHHingeJoint {
public:
	SPR_OBJECTDEF1(PH3ElementHingeJoint, PHJoint);
	SPR_DECLMEMBEROF_PH3ElementHingeJointDesc;
protected:
public:
		
	/// インタフェースの実装
	double  GetSecondDamper()				{return secondDamper;}
	void	SetSecondDamper(double input)	{secondDamper = input;}

	// このクラス内で再定義する（オーバーライドする）関数
	void	CompBias();		///< 侵入量の判定(オーバーライド)

	// コンストラクタ（引数付き）
	PH3ElementHingeJoint(const PH3ElementHingeJointDesc& desc = PH3ElementHingeJointDesc());				/// - コンストラクタ
};

}

#endif

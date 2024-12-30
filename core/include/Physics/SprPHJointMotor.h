/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHJointLimit.h
 *	@brief 関節可動域制限
*/
#ifndef SPR_PHJOINTMOTORIf_H
#define SPR_PHJOINTMOTORIf_H

#include <Foundation/SprObject.h>
#include <functional>
#include <Physics/SprPHConstraintBase.h>
#include <Physics/SprPHJointLimit.h>
namespace Spr{;

/** \addtogroup gpJoint */
//@{

/// 1自由度関節モータのインタフェース
struct PH1DJointMotorIf : PHConstraintBaseIf{
	SPR_IFDEF(PH1DJointMotor);
};


///	1自由度関節モータのデスクリプタ
struct PH1DJointMotorDesc{
	SPR_DESCDEF(PH1DJointMotor);
	PH1DJointMotorDesc() {}
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

///	1自由度関節非線形モータのデスクリプタ
struct PH1DJointNonLinearMotorDesc : public PH1DJointMotorDesc{
	//SPR_DESCDEF(PH1DJointNonLinearMotor);
	enum FunctionMode {
		LINEAR,
		HUMANJOINTRESITANCE,
	};
	PH1DJointNonLinearMotorDesc() {}
};

/// 1自由度関節非線形モータのインタフェース
struct PH1DJointNonLinearMotorIf : public PH1DJointMotorIf{
	SPR_IFDEF(PH1DJointNonLinearMotor);

	void SetSpring(PH1DJointNonLinearMotorDesc::FunctionMode m, void* param);
	void SetDamper(PH1DJointNonLinearMotorDesc::FunctionMode m, void* param);
	void SetSpringDamper(PH1DJointNonLinearMotorDesc::FunctionMode smode, PH1DJointNonLinearMotorDesc::FunctionMode dmode, void* sparam, void* dparam);
};
// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

/// 1自由度人体関節抵抗のインタフェース
struct PHHuman1DJointResistanceIf : public PH1DJointNonLinearMotorIf {
	SPR_IFDEF(PHHuman1DJointResistance);

	double GetCurrentResistance();
};
/// 1自由度人体関節特性抵抗のデスクリプタ
struct PHHuman1DJointResistanceDesc : public PH1DJointNonLinearMotorDesc {
	SPR_DESCDEF(PHHuman1DJointResistance);
	Vec4d coefficient;
	PHHuman1DJointResistanceDesc() {
		coefficient = Vec4d();
	}
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

/// ボールジョイントモータのインタフェース
struct PHBallJointMotorIf : public SceneObjectIf {
	SPR_IFDEF(PHBallJointMotor);

};
	
/// ボールジョイントモータのデスクリプタ
struct PHBallJointMotorDesc{
	SPR_DESCDEF(PHBallJointMotor);

	PHBallJointMotorDesc() {
	}
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

/// ボールジョイント非線形モータのインタフェース
struct PHBallJointNonLinearMotorIf : public PHBallJointMotorIf {
	SPR_IFDEF(PHBallJointNonLinearMotor);

	void SetFuncFromDatabaseN(int n, int i, int j, void* sparam, void* dparam);
};

/// ボールジョイント非線形モータのデスクリプタ
struct PHBallJointNonLinearMotorDesc : public PHBallJointMotorDesc{
	SPR_DESCDEF(PHBallJointNonLinearMotor);

	PHBallJointNonLinearMotorDesc() {
	}
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

/// 3自由度人体関節抵抗のインタフェース
struct PHHumanBallJointResistanceIf : public PHBallJointNonLinearMotorIf {
	SPR_IFDEF(PHHumanBallJointResistance);

	Vec3d GetCurrentResistance();
};
/// 3自由度人体関節特性抵抗のデスクリプタ
struct PHHumanBallJointResistanceDesc : public PHBallJointNonLinearMotorDesc {
	SPR_DESCDEF(PHHumanBallJointResistance);
	Vec4d xCoefficient;
	Vec4d yCoefficient;
	Vec4d zCoefficient;
	PHHumanBallJointResistanceDesc() {
		xCoefficient = Vec4d();
		yCoefficient = Vec4d();
		zCoefficient = Vec4d();
	}
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

/// バネのモータのインタフェース
struct PHSpringMotorIf : public SceneObjectIf {
	SPR_IFDEF(PHSpringMotor);

};
	
/// バネのモータのデスクリプタ
struct PHSpringMotorDesc{
	SPR_DESCDEF(PHSpringMotor);

	PHSpringMotorDesc() {
	}
};


}

#endif//SPR_PHJOINTMOTORIf_H

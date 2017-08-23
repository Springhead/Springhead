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

namespace Spr{;

/** \addtogroup gpJoint */
//@{

/// 1自由度関節モータのインタフェース
struct PH1DJointMotorIf : public SceneObjectIf{
	SPR_IFDEF(PH1DJointMotor);
};
///	1自由度関節モータのデスクリプタ
struct PH1DJointMotorDesc{
	SPR_DESCDEF(PH1DJointMotor);
	PH1DJointMotorDesc() {}
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

/// 1自由度関節非線形モータのインタフェース
struct PH1DJointNonLinearMotorIf : public PH1DJointMotorIf{
	SPR_IFDEF(PH1DJointNonLinearMotor);

	//void SetFunc(std::function<double(double)> func);
	void SetFuncFromDatabase(int i, void* param);
	void SetFuncFromDatabase(int i, int j, void* sparam, void* dparam);
};
///	1自由度関節非線形モータのデスクリプタ
struct PH1DJointNonLinearMotorDesc : public PH1DJointMotorDesc{
	SPR_DESCDEF(PH1DJointNonLinearMotor);
	PH1DJointNonLinearMotorDesc() {}
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

/// ボールジョイントモータのインタフェース
struct PHSpringMotorIf : public SceneObjectIf {
	SPR_IFDEF(PHSpringMotor);

};
	
/// ボールジョイントモータのデスクリプタ
struct PHSpringMotorDesc{
	SPR_DESCDEF(PHSpringMotor);

	PHSpringMotorDesc() {
	}
};


}

#endif//SPR_PHJOINTMOTORIf_H

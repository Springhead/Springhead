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

namespace Spr{;

/** \addtogroup gpJoint */
//@{

/// 1自由度関節モータのインタフェース
struct PH1DJointMotorIf : public SceneObjectIf{
	SPR_IFDEF(PH1DJointMotor);

};
///	1自由度関節モータのデスクリプタ
struct PH1DJointMotorDesc{
	PH1DJointMotorDesc() {}
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

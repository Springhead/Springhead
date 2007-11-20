/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRBodyIK.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// êßå‰ì_
IF_OBJECT_IMP(CRIKControl, SceneObject);

IF_OBJECT_IMP(CRIKControlPos, CRIKControl);
IF_OBJECT_IMP(CRIKControlOri, CRIKControl);
IF_OBJECT_IMP(CRIKControlForce, CRIKControl);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// êßå‰ëŒè€
IF_OBJECT_IMP(CRIKMovable, SceneObject);

IF_OBJECT_IMP(CRIKMovableSolidPos, CRIKMovable);
IF_OBJECT_IMP(CRIKMovableSolidOri, CRIKMovable);
IF_OBJECT_IMP(CRIKMovableBallJointOri, CRIKMovable);
IF_OBJECT_IMP(CRIKMovableBallJointTorque, CRIKMovable);
IF_OBJECT_IMP(CRIKMovable3HingeJointOri, CRIKMovable);
IF_OBJECT_IMP(CRIKMovable3HingeJointTorque, CRIKMovable);

}

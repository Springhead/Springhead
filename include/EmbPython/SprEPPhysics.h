#ifndef SprEPPhysics_H
#define SprEPPhysics_H
#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Python\Add_decl.h"
#include "..\..\include\Python\pyport.h"
#include "..\..\include\Python\patchlevel.h"
#include "..\..\include\Python\Python.h"
#include "..\..\include\Python\object.h"
#include "..\..\include\EmbPython\SprEPUtility.h"
#include "..\..\include\EmbPython\SprEPFoundation.h"
#include "..\..\include\EmbPython\SprEPCollision.h"
#include "..\..\include\Physics\SprPHDef.h"
#include "..\..\include\Physics\SprPHEngine.h"
#include "..\..\include\Physics\SprPHFemMesh.h"
#include "..\..\include\Physics\SprPHIK.h"
#include "..\..\include\Physics\SprPHJoint.h"
#include "..\..\include\Physics\SprPHNodeHandlers.h"
#include "..\..\include\Physics\SprPHScene.h"
#include "..\..\include\Physics\SprPHSdk.h"
#include "..\..\include\Physics\SprPHSolid.h"
typedef struct
{
	PyObject_HEAD
	UTRef<PHEngineIf> ptr;
} EPPHEngineIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHConstraintEngineIf> ptr;
} EPPHConstraintEngineIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHGravityEngineIf> ptr;
} EPPHGravityEngineIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHPenaltyEngineIf> ptr;
} EPPHPenaltyEngineIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHIKEngineIf> ptr;
} EPPHIKEngineIf;
typedef struct
{
	PyObject_HEAD
	struct PHFemMeshState* ptr;
} EPPHFemMeshState;
typedef struct
{
	PyObject_HEAD
	struct PHFemMeshDesc* ptr;
} EPPHFemMeshDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHFemMeshIf> ptr;
} EPPHFemMeshIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHIKEndEffectorIf> ptr;
} EPPHIKEndEffectorIf;
typedef struct
{
	PyObject_HEAD
	struct PHIKEndEffectorDesc* ptr;
} EPPHIKEndEffectorDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHIKActuatorIf> ptr;
} EPPHIKActuatorIf;
typedef struct
{
	PyObject_HEAD
	struct PHIKActuatorDesc* ptr;
} EPPHIKActuatorDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHIKBallActuatorIf> ptr;
} EPPHIKBallActuatorIf;
typedef struct
{
	PyObject_HEAD
	struct PHIKBallActuatorDesc* ptr;
} EPPHIKBallActuatorDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHIKHingeActuatorIf> ptr;
} EPPHIKHingeActuatorIf;
typedef struct
{
	PyObject_HEAD
	struct PHIKHingeActuatorDesc* ptr;
} EPPHIKHingeActuatorDesc;
typedef struct
{
	PyObject_HEAD
	struct PHConstraintEngineDesc* ptr;
} EPPHConstraintEngineDesc;
typedef struct
{
	PyObject_HEAD
	struct PHConstraintDesc* ptr;
} EPPHConstraintDesc;
typedef struct
{
	PyObject_HEAD
	struct PHJointDesc* ptr;
} EPPHJointDesc;
typedef struct
{
	PyObject_HEAD
	struct PHJoint1DDesc* ptr;
} EPPHJoint1DDesc;
typedef struct
{
	PyObject_HEAD
	struct PHTreeNodeDesc* ptr;
} EPPHTreeNodeDesc;
typedef struct
{
	PyObject_HEAD
	struct PHRootNodeDesc* ptr;
} EPPHRootNodeDesc;
typedef struct
{
	PyObject_HEAD
	struct PHTreeNode1DDesc* ptr;
} EPPHTreeNode1DDesc;
typedef struct
{
	PyObject_HEAD
	struct PHHingeJointNodeDesc* ptr;
} EPPHHingeJointNodeDesc;
typedef struct
{
	PyObject_HEAD
	struct PHSliderJointNodeDesc* ptr;
} EPPHSliderJointNodeDesc;
typedef struct
{
	PyObject_HEAD
	struct PHPathJointNodeDesc* ptr;
} EPPHPathJointNodeDesc;
typedef struct
{
	PyObject_HEAD
	struct PHBallJointNodeDesc* ptr;
} EPPHBallJointNodeDesc;
typedef struct
{
	PyObject_HEAD
	struct PHGearDesc* ptr;
} EPPHGearDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHConstraintIf> ptr;
} EPPHConstraintIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHConstraintsIf> ptr;
} EPPHConstraintsIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHContactPointIf> ptr;
} EPPHContactPointIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHJointIf> ptr;
} EPPHJointIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHJoint1DIf> ptr;
} EPPHJoint1DIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHHingeJointIf> ptr;
} EPPHHingeJointIf;
typedef struct
{
	PyObject_HEAD
	struct PHHingeJointDesc* ptr;
} EPPHHingeJointDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHSliderJointIf> ptr;
} EPPHSliderJointIf;
typedef struct
{
	PyObject_HEAD
	struct PHSliderJointDesc* ptr;
} EPPHSliderJointDesc;
typedef struct
{
	PyObject_HEAD
	struct PHPathPoint* ptr;
} EPPHPathPoint;
typedef struct
{
	PyObject_HEAD
	struct PHPathDesc* ptr;
} EPPHPathDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHPathIf> ptr;
} EPPHPathIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHPathJointIf> ptr;
} EPPHPathJointIf;
typedef struct
{
	PyObject_HEAD
	struct PHPathJointDesc* ptr;
} EPPHPathJointDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHBallJointIf> ptr;
} EPPHBallJointIf;
typedef struct
{
	PyObject_HEAD
	struct PHBallJointDesc* ptr;
} EPPHBallJointDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHSpringIf> ptr;
} EPPHSpringIf;
typedef struct
{
	PyObject_HEAD
	struct PHSpringDesc* ptr;
} EPPHSpringDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHTreeNodeIf> ptr;
} EPPHTreeNodeIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHRootNodeIf> ptr;
} EPPHRootNodeIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHTreeNode1DIf> ptr;
} EPPHTreeNode1DIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHHingeJointNodeIf> ptr;
} EPPHHingeJointNodeIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHSliderJointNodeIf> ptr;
} EPPHSliderJointNodeIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHPathJointNodeIf> ptr;
} EPPHPathJointNodeIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHBallJointNodeIf> ptr;
} EPPHBallJointNodeIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHGearIf> ptr;
} EPPHGearIf;
typedef struct
{
	PyObject_HEAD
	struct PHInactiveSolids* ptr;
} EPPHInactiveSolids;
typedef struct
{
	PyObject_HEAD
	struct PHRaycastHit* ptr;
} EPPHRaycastHit;
typedef struct
{
	PyObject_HEAD
	struct PHRayDesc* ptr;
} EPPHRayDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHRayIf> ptr;
} EPPHRayIf;
typedef struct
{
	PyObject_HEAD
	struct PHSceneState* ptr;
} EPPHSceneState;
typedef struct
{
	PyObject_HEAD
	struct PHSceneDesc* ptr;
} EPPHSceneDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHSceneIf> ptr;
} EPPHSceneIf;
typedef struct
{
	PyObject_HEAD
	struct PHSdkDesc* ptr;
} EPPHSdkDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHSdkIf> ptr;
} EPPHSdkIf;
typedef struct
{
	PyObject_HEAD
	struct PHFrameDesc* ptr;
} EPPHFrameDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHFrameIf> ptr;
} EPPHFrameIf;
typedef struct
{
	PyObject_HEAD
	struct PHSolidState* ptr;
} EPPHSolidState;
typedef struct
{
	PyObject_HEAD
	struct PHSolidDesc* ptr;
} EPPHSolidDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<PHSolidIf> ptr;
} EPPHSolidIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHShapePairForLCPIf> ptr;
} EPPHShapePairForLCPIf;
typedef struct
{
	PyObject_HEAD
	UTRef<PHSolidPairForLCPIf> ptr;
} EPPHSolidPairForLCPIf;

//{*********EPPHEngineIf*******
#define EPPHEngineIf_Check(ob) PyObject_TypeCheck(ob, &EPPHEngineIfType)
extern PyTypeObject EPPHEngineIfType;
PyObject* __EPDECL newEPPHEngineIf(struct PHEngineIf);
PyObject* __EPDECL newEPPHEngineIf(struct PHEngineIf*);

//}EPPHEngineIf

//{*********EPPHConstraintEngineIf*******
#define EPPHConstraintEngineIf_Check(ob) PyObject_TypeCheck(ob, &EPPHConstraintEngineIfType)
extern PyTypeObject EPPHConstraintEngineIfType;
PyObject* __EPDECL newEPPHConstraintEngineIf(struct PHConstraintEngineIf);
PyObject* __EPDECL newEPPHConstraintEngineIf(struct PHConstraintEngineIf*);
PyObject* EPPHConstraintEngineIf_GetContactCorrectionRate( EPPHConstraintEngineIf* self );
PyObject* EPPHConstraintEngineIf_GetContactPoints( EPPHConstraintEngineIf* self );
PyObject* EPPHConstraintEngineIf_GetPosCorrectionRate( EPPHConstraintEngineIf* self );
PyObject* EPPHConstraintEngineIf_GetShrinkRate( EPPHConstraintEngineIf* self );
PyObject* EPPHConstraintEngineIf_GetShrinkRateCorrection( EPPHConstraintEngineIf* self );
PyObject* EPPHConstraintEngineIf_GetVelCorrectionRate( EPPHConstraintEngineIf* self );
PyObject* EPPHConstraintEngineIf_SetBSaveConstraints( EPPHConstraintEngineIf* self,PyObject* var1 );
PyObject* EPPHConstraintEngineIf_SetContactCorrectionRate( EPPHConstraintEngineIf* self,PyFloatObject* var1 );
PyObject* EPPHConstraintEngineIf_SetPosCorrectionRate( EPPHConstraintEngineIf* self,PyFloatObject* var1 );
PyObject* EPPHConstraintEngineIf_SetShrinkRate( EPPHConstraintEngineIf* self,PyFloatObject* var1 );
PyObject* EPPHConstraintEngineIf_SetShrinkRateCorrection( EPPHConstraintEngineIf* self,PyFloatObject* var1 );
PyObject* EPPHConstraintEngineIf_SetUpdateAllSolidState( EPPHConstraintEngineIf* self,PyObject* var1 );
PyObject* EPPHConstraintEngineIf_SetUseContactSurface( EPPHConstraintEngineIf* self,PyObject* var1 );
PyObject* EPPHConstraintEngineIf_SetVelCorrectionRate( EPPHConstraintEngineIf* self,PyFloatObject* var1 );

//}EPPHConstraintEngineIf

//{*********EPPHGravityEngineIf*******
#define EPPHGravityEngineIf_Check(ob) PyObject_TypeCheck(ob, &EPPHGravityEngineIfType)
extern PyTypeObject EPPHGravityEngineIfType;
PyObject* __EPDECL newEPPHGravityEngineIf(struct PHGravityEngineIf);
PyObject* __EPDECL newEPPHGravityEngineIf(struct PHGravityEngineIf*);

//}EPPHGravityEngineIf

//{*********EPPHPenaltyEngineIf*******
#define EPPHPenaltyEngineIf_Check(ob) PyObject_TypeCheck(ob, &EPPHPenaltyEngineIfType)
extern PyTypeObject EPPHPenaltyEngineIfType;
PyObject* __EPDECL newEPPHPenaltyEngineIf(struct PHPenaltyEngineIf);
PyObject* __EPDECL newEPPHPenaltyEngineIf(struct PHPenaltyEngineIf*);

//}EPPHPenaltyEngineIf

//{*********EPPHIKEngineIf*******
#define EPPHIKEngineIf_Check(ob) PyObject_TypeCheck(ob, &EPPHIKEngineIfType)
extern PyTypeObject EPPHIKEngineIfType;
PyObject* __EPDECL newEPPHIKEngineIf(struct PHIKEngineIf);
PyObject* __EPDECL newEPPHIKEngineIf(struct PHIKEngineIf*);
PyObject* EPPHIKEngineIf_Enable( EPPHIKEngineIf* self,PyObject* var1 );
PyObject* EPPHIKEngineIf_IsEnabled( EPPHIKEngineIf* self );
PyObject* EPPHIKEngineIf_SetNumIter( EPPHIKEngineIf* self,PyLongObject* var1 );

//}EPPHIKEngineIf

//{*********EPPHFemMeshState*******
#define EPPHFemMeshState_Check(ob) PyObject_TypeCheck(ob, &EPPHFemMeshStateType)
extern PyTypeObject EPPHFemMeshStateType;
PyObject* __EPDECL newEPPHFemMeshState(struct PHFemMeshState);
PyObject* __EPDECL newEPPHFemMeshState(struct PHFemMeshState*);

//}EPPHFemMeshState

//{*********EPPHFemMeshDesc*******
#define EPPHFemMeshDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHFemMeshDescType)
extern PyTypeObject EPPHFemMeshDescType;
PyObject* __EPDECL newEPPHFemMeshDesc(struct PHFemMeshDesc);
PyObject* __EPDECL newEPPHFemMeshDesc(struct PHFemMeshDesc*);
PyObject* EPPHFemMeshDesc_Init( EPPHFemMeshDesc* self );

//}EPPHFemMeshDesc

//{*********EPPHFemMeshIf*******
#define EPPHFemMeshIf_Check(ob) PyObject_TypeCheck(ob, &EPPHFemMeshIfType)
extern PyTypeObject EPPHFemMeshIfType;
PyObject* __EPDECL newEPPHFemMeshIf(struct PHFemMeshIf);
PyObject* __EPDECL newEPPHFemMeshIf(struct PHFemMeshIf*);

//}EPPHFemMeshIf

//{*********EPPHIKEndEffectorIf*******
#define EPPHIKEndEffectorIf_Check(ob) PyObject_TypeCheck(ob, &EPPHIKEndEffectorIfType)
extern PyTypeObject EPPHIKEndEffectorIfType;
PyObject* __EPDECL newEPPHIKEndEffectorIf(struct PHIKEndEffectorIf);
PyObject* __EPDECL newEPPHIKEndEffectorIf(struct PHIKEndEffectorIf*);
PyObject* EPPHIKEndEffectorIf_Enable( EPPHIKEndEffectorIf* self,PyObject* var1 );
PyObject* EPPHIKEndEffectorIf_EnableForceControl( EPPHIKEndEffectorIf* self,PyObject* var1 );
PyObject* EPPHIKEndEffectorIf_EnableOrientationControl( EPPHIKEndEffectorIf* self,PyObject* var1 );
PyObject* EPPHIKEndEffectorIf_EnablePositionControl( EPPHIKEndEffectorIf* self,PyObject* var1 );
PyObject* EPPHIKEndEffectorIf_EnableTorqueControl( EPPHIKEndEffectorIf* self,PyObject* var1 );
PyObject* EPPHIKEndEffectorIf_GetSolid( EPPHIKEndEffectorIf* self );
PyObject* EPPHIKEndEffectorIf_GetTargetForce( EPPHIKEndEffectorIf* self );
PyObject* EPPHIKEndEffectorIf_GetTargetForceWorkingPoint( EPPHIKEndEffectorIf* self );
PyObject* EPPHIKEndEffectorIf_GetTargetLocalPosition( EPPHIKEndEffectorIf* self );
PyObject* EPPHIKEndEffectorIf_GetTargetOrientation( EPPHIKEndEffectorIf* self );
PyObject* EPPHIKEndEffectorIf_GetTargetPosition( EPPHIKEndEffectorIf* self );
PyObject* EPPHIKEndEffectorIf_GetTargetTorque( EPPHIKEndEffectorIf* self );
PyObject* EPPHIKEndEffectorIf_IsEnabled( EPPHIKEndEffectorIf* self );
PyObject* EPPHIKEndEffectorIf_SetOrientationPriority( EPPHIKEndEffectorIf* self,PyFloatObject* var1 );
PyObject* EPPHIKEndEffectorIf_SetPositionPriority( EPPHIKEndEffectorIf* self,PyFloatObject* var1 );
PyObject* EPPHIKEndEffectorIf_SetSolid( EPPHIKEndEffectorIf* self,EPPHSolidIf* var1 );
PyObject* EPPHIKEndEffectorIf_SetTargetForce( EPPHIKEndEffectorIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHIKEndEffectorIf_SetTargetOrientation( EPPHIKEndEffectorIf* self,EPQuaterniond* var1 );
PyObject* EPPHIKEndEffectorIf_SetTargetPosition( EPPHIKEndEffectorIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHIKEndEffectorIf_SetTargetTorque( EPPHIKEndEffectorIf* self,EPVec3d* var1 );

//}EPPHIKEndEffectorIf

//{*********EPPHIKEndEffectorDesc*******
#define EPPHIKEndEffectorDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHIKEndEffectorDescType)
extern PyTypeObject EPPHIKEndEffectorDescType;
PyObject* __EPDECL newEPPHIKEndEffectorDesc(struct PHIKEndEffectorDesc);
PyObject* __EPDECL newEPPHIKEndEffectorDesc(struct PHIKEndEffectorDesc*);

//}EPPHIKEndEffectorDesc

//{*********EPPHIKActuatorIf*******
#define EPPHIKActuatorIf_Check(ob) PyObject_TypeCheck(ob, &EPPHIKActuatorIfType)
extern PyTypeObject EPPHIKActuatorIfType;
PyObject* __EPDECL newEPPHIKActuatorIf(struct PHIKActuatorIf);
PyObject* __EPDECL newEPPHIKActuatorIf(struct PHIKActuatorIf*);
PyObject* EPPHIKActuatorIf_Enable( EPPHIKActuatorIf* self,PyObject* var1 );
PyObject* EPPHIKActuatorIf_GetBias( EPPHIKActuatorIf* self );
PyObject* EPPHIKActuatorIf_GetDamper( EPPHIKActuatorIf* self );
PyObject* EPPHIKActuatorIf_GetSpring( EPPHIKActuatorIf* self );
PyObject* EPPHIKActuatorIf_IsEnabled( EPPHIKActuatorIf* self );
PyObject* EPPHIKActuatorIf_Move( EPPHIKActuatorIf* self );
PyObject* EPPHIKActuatorIf_MoveToNaturalPosition( EPPHIKActuatorIf* self );
PyObject* EPPHIKActuatorIf_PrepareSolve( EPPHIKActuatorIf* self );
PyObject* EPPHIKActuatorIf_ProceedSolve( EPPHIKActuatorIf* self );
PyObject* EPPHIKActuatorIf_RegisterEndEffector( EPPHIKActuatorIf* self,EPPHIKEndEffectorIf* var1 );
PyObject* EPPHIKActuatorIf_SetBias( EPPHIKActuatorIf* self,PyFloatObject* var1 );
PyObject* EPPHIKActuatorIf_SetDamper( EPPHIKActuatorIf* self,PyFloatObject* var1 );
PyObject* EPPHIKActuatorIf_SetSpring( EPPHIKActuatorIf* self,PyFloatObject* var1 );

//}EPPHIKActuatorIf

//{*********EPPHIKActuatorDesc*******
#define EPPHIKActuatorDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHIKActuatorDescType)
extern PyTypeObject EPPHIKActuatorDescType;
PyObject* __EPDECL newEPPHIKActuatorDesc(struct PHIKActuatorDesc);
PyObject* __EPDECL newEPPHIKActuatorDesc(struct PHIKActuatorDesc*);

//}EPPHIKActuatorDesc

//{*********EPPHIKBallActuatorIf*******
#define EPPHIKBallActuatorIf_Check(ob) PyObject_TypeCheck(ob, &EPPHIKBallActuatorIfType)
extern PyTypeObject EPPHIKBallActuatorIfType;
PyObject* __EPDECL newEPPHIKBallActuatorIf(struct PHIKBallActuatorIf);
PyObject* __EPDECL newEPPHIKBallActuatorIf(struct PHIKBallActuatorIf*);
PyObject* EPPHIKBallActuatorIf_GetJoint( EPPHIKBallActuatorIf* self );
PyObject* EPPHIKBallActuatorIf_SetJoint( EPPHIKBallActuatorIf* self,EPPHBallJointIf* var1 );

//}EPPHIKBallActuatorIf

//{*********EPPHIKBallActuatorDesc*******
#define EPPHIKBallActuatorDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHIKBallActuatorDescType)
extern PyTypeObject EPPHIKBallActuatorDescType;
PyObject* __EPDECL newEPPHIKBallActuatorDesc(struct PHIKBallActuatorDesc);
PyObject* __EPDECL newEPPHIKBallActuatorDesc(struct PHIKBallActuatorDesc*);

//}EPPHIKBallActuatorDesc

//{*********EPPHIKHingeActuatorIf*******
#define EPPHIKHingeActuatorIf_Check(ob) PyObject_TypeCheck(ob, &EPPHIKHingeActuatorIfType)
extern PyTypeObject EPPHIKHingeActuatorIfType;
PyObject* __EPDECL newEPPHIKHingeActuatorIf(struct PHIKHingeActuatorIf);
PyObject* __EPDECL newEPPHIKHingeActuatorIf(struct PHIKHingeActuatorIf*);
PyObject* EPPHIKHingeActuatorIf_GetJoint( EPPHIKHingeActuatorIf* self );
PyObject* EPPHIKHingeActuatorIf_SetJoint( EPPHIKHingeActuatorIf* self,EPPHHingeJointIf* var1 );

//}EPPHIKHingeActuatorIf

//{*********EPPHIKHingeActuatorDesc*******
#define EPPHIKHingeActuatorDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHIKHingeActuatorDescType)
extern PyTypeObject EPPHIKHingeActuatorDescType;
PyObject* __EPDECL newEPPHIKHingeActuatorDesc(struct PHIKHingeActuatorDesc);
PyObject* __EPDECL newEPPHIKHingeActuatorDesc(struct PHIKHingeActuatorDesc*);

//}EPPHIKHingeActuatorDesc

//{*********EPPHConstraintEngineDesc*******
#define EPPHConstraintEngineDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHConstraintEngineDescType)
extern PyTypeObject EPPHConstraintEngineDescType;
PyObject* __EPDECL newEPPHConstraintEngineDesc(struct PHConstraintEngineDesc);
PyObject* __EPDECL newEPPHConstraintEngineDesc(struct PHConstraintEngineDesc*);

//}EPPHConstraintEngineDesc

//{*********EPPHConstraintDesc*******
#define EPPHConstraintDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHConstraintDescType)
extern PyTypeObject EPPHConstraintDescType;
PyObject* __EPDECL newEPPHConstraintDesc(struct PHConstraintDesc);
PyObject* __EPDECL newEPPHConstraintDesc(struct PHConstraintDesc*);

//}EPPHConstraintDesc

//{*********EPPHJointDesc*******
#define EPPHJointDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHJointDescType)
extern PyTypeObject EPPHJointDescType;
PyObject* __EPDECL newEPPHJointDesc(struct PHJointDesc);
PyObject* __EPDECL newEPPHJointDesc(struct PHJointDesc*);

//}EPPHJointDesc

//{*********EPPHJoint1DDesc*******
#define EPPHJoint1DDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHJoint1DDescType)
extern PyTypeObject EPPHJoint1DDescType;
PyObject* __EPDECL newEPPHJoint1DDesc(struct PHJoint1DDesc);
PyObject* __EPDECL newEPPHJoint1DDesc(struct PHJoint1DDesc*);

//}EPPHJoint1DDesc

//{*********EPPHTreeNodeDesc*******
#define EPPHTreeNodeDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHTreeNodeDescType)
extern PyTypeObject EPPHTreeNodeDescType;
PyObject* __EPDECL newEPPHTreeNodeDesc(struct PHTreeNodeDesc);
PyObject* __EPDECL newEPPHTreeNodeDesc(struct PHTreeNodeDesc*);

//}EPPHTreeNodeDesc

//{*********EPPHRootNodeDesc*******
#define EPPHRootNodeDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHRootNodeDescType)
extern PyTypeObject EPPHRootNodeDescType;
PyObject* __EPDECL newEPPHRootNodeDesc(struct PHRootNodeDesc);
PyObject* __EPDECL newEPPHRootNodeDesc(struct PHRootNodeDesc*);

//}EPPHRootNodeDesc

//{*********EPPHTreeNode1DDesc*******
#define EPPHTreeNode1DDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHTreeNode1DDescType)
extern PyTypeObject EPPHTreeNode1DDescType;
PyObject* __EPDECL newEPPHTreeNode1DDesc(struct PHTreeNode1DDesc);
PyObject* __EPDECL newEPPHTreeNode1DDesc(struct PHTreeNode1DDesc*);

//}EPPHTreeNode1DDesc

//{*********EPPHHingeJointNodeDesc*******
#define EPPHHingeJointNodeDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHHingeJointNodeDescType)
extern PyTypeObject EPPHHingeJointNodeDescType;
PyObject* __EPDECL newEPPHHingeJointNodeDesc(struct PHHingeJointNodeDesc);
PyObject* __EPDECL newEPPHHingeJointNodeDesc(struct PHHingeJointNodeDesc*);

//}EPPHHingeJointNodeDesc

//{*********EPPHSliderJointNodeDesc*******
#define EPPHSliderJointNodeDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHSliderJointNodeDescType)
extern PyTypeObject EPPHSliderJointNodeDescType;
PyObject* __EPDECL newEPPHSliderJointNodeDesc(struct PHSliderJointNodeDesc);
PyObject* __EPDECL newEPPHSliderJointNodeDesc(struct PHSliderJointNodeDesc*);

//}EPPHSliderJointNodeDesc

//{*********EPPHPathJointNodeDesc*******
#define EPPHPathJointNodeDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHPathJointNodeDescType)
extern PyTypeObject EPPHPathJointNodeDescType;
PyObject* __EPDECL newEPPHPathJointNodeDesc(struct PHPathJointNodeDesc);
PyObject* __EPDECL newEPPHPathJointNodeDesc(struct PHPathJointNodeDesc*);

//}EPPHPathJointNodeDesc

//{*********EPPHBallJointNodeDesc*******
#define EPPHBallJointNodeDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHBallJointNodeDescType)
extern PyTypeObject EPPHBallJointNodeDescType;
PyObject* __EPDECL newEPPHBallJointNodeDesc(struct PHBallJointNodeDesc);
PyObject* __EPDECL newEPPHBallJointNodeDesc(struct PHBallJointNodeDesc*);

//}EPPHBallJointNodeDesc

//{*********EPPHGearDesc*******
#define EPPHGearDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHGearDescType)
extern PyTypeObject EPPHGearDescType;
PyObject* __EPDECL newEPPHGearDesc(struct PHGearDesc);
PyObject* __EPDECL newEPPHGearDesc(struct PHGearDesc*);

//}EPPHGearDesc

//{*********EPPHConstraintIf*******
#define EPPHConstraintIf_Check(ob) PyObject_TypeCheck(ob, &EPPHConstraintIfType)
extern PyTypeObject EPPHConstraintIfType;
PyObject* __EPDECL newEPPHConstraintIf(struct PHConstraintIf);
PyObject* __EPDECL newEPPHConstraintIf(struct PHConstraintIf*);
PyObject* EPPHConstraintIf_Enable( EPPHConstraintIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHConstraintIf_GetAbsolutePoseQ( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_GetConstraintForce( EPPHConstraintIf* self,PyObject* tuple );
PyObject* EPPHConstraintIf_GetLimitf( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_GetMotorf( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_GetPlugPose( EPPHConstraintIf* self,EPPosed* var1 );
PyObject* EPPHConstraintIf_GetPlugSolid( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_GetRelativePose( EPPHConstraintIf* self,EPPosed* var1 );
PyObject* EPPHConstraintIf_GetRelativePoseQ( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_GetRelativePoseR( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_GetRelativeVelocity( EPPHConstraintIf* self,PyObject* tuple );
PyObject* EPPHConstraintIf_GetScene( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_GetSocketPose( EPPHConstraintIf* self,EPPosed* var1 );
PyObject* EPPHConstraintIf_GetSocketSolid( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_IsEnabled( EPPHConstraintIf* self );
PyObject* EPPHConstraintIf_SetPlugPose( EPPHConstraintIf* self,EPPosed* var1 );
PyObject* EPPHConstraintIf_SetSocketPose( EPPHConstraintIf* self,EPPosed* var1 );

//}EPPHConstraintIf

//{*********EPPHConstraintsIf*******
#define EPPHConstraintsIf_Check(ob) PyObject_TypeCheck(ob, &EPPHConstraintsIfType)
extern PyTypeObject EPPHConstraintsIfType;
PyObject* __EPDECL newEPPHConstraintsIf(struct PHConstraintsIf);
PyObject* __EPDECL newEPPHConstraintsIf(struct PHConstraintsIf*);
PyObject* EPPHConstraintsIf_FindBySolidPair( EPPHConstraintsIf* self,PyObject* tuple );
PyObject* EPPHConstraintsIf_GetTotalForce( EPPHConstraintsIf* self,PyObject* tuple );

//}EPPHConstraintsIf

//{*********EPPHContactPointIf*******
#define EPPHContactPointIf_Check(ob) PyObject_TypeCheck(ob, &EPPHContactPointIfType)
extern PyTypeObject EPPHContactPointIfType;
PyObject* __EPDECL newEPPHContactPointIf(struct PHContactPointIf);
PyObject* __EPDECL newEPPHContactPointIf(struct PHContactPointIf*);

//}EPPHContactPointIf

//{*********EPPHJointIf*******
#define EPPHJointIf_Check(ob) PyObject_TypeCheck(ob, &EPPHJointIfType)
extern PyTypeObject EPPHJointIfType;
PyObject* __EPDECL newEPPHJointIf(struct PHJointIf);
PyObject* __EPDECL newEPPHJointIf(struct PHJointIf*);
PyObject* EPPHJointIf_GetDefomationType( EPPHJointIf* self );
PyObject* EPPHJointIf_SetDefomationType( EPPHJointIf* self,PyLongObject* var1 );

//}EPPHJointIf

//{*********EPPHJoint1DIf*******
#define EPPHJoint1DIf_Check(ob) PyObject_TypeCheck(ob, &EPPHJoint1DIfType)
extern PyTypeObject EPPHJoint1DIfType;
PyObject* __EPDECL newEPPHJoint1DIf(struct PHJoint1DIf);
PyObject* __EPDECL newEPPHJoint1DIf(struct PHJoint1DIf*);
PyObject* EPPHJoint1DIf_GetDamper( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetDeformationMode( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetHardnessRate( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetMotorTorque( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetOffsetForce( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetPosition( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetRange( EPPHJoint1DIf* self,PyObject* tuple );
PyObject* EPPHJoint1DIf_GetRangeDamper( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetRangeSpring( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetSecondDamper( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetSpring( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetTargetPosition( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetTargetVelocity( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetTorqueMax( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetTrajectoryVelocity( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetVelocity( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_GetYieldStress( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_IsLimit( EPPHJoint1DIf* self );
PyObject* EPPHJoint1DIf_SetDamper( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetHardnessRate( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetMotorTorque( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetOffsetForce( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetRange( EPPHJoint1DIf* self,PyObject* tuple );
PyObject* EPPHJoint1DIf_SetRangeDamper( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetRangeSpring( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetSecondDamper( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetSpring( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetTargetPosition( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetTargetVelocity( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetTorqueMax( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetTrajectoryVelocity( EPPHJoint1DIf* self,PyFloatObject* var1 );
PyObject* EPPHJoint1DIf_SetYieldStress( EPPHJoint1DIf* self,PyFloatObject* var1 );

//}EPPHJoint1DIf

//{*********EPPHHingeJointIf*******
#define EPPHHingeJointIf_Check(ob) PyObject_TypeCheck(ob, &EPPHHingeJointIfType)
extern PyTypeObject EPPHHingeJointIfType;
PyObject* __EPDECL newEPPHHingeJointIf(struct PHHingeJointIf);
PyObject* __EPDECL newEPPHHingeJointIf(struct PHHingeJointIf*);

//}EPPHHingeJointIf

//{*********EPPHHingeJointDesc*******
#define EPPHHingeJointDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHHingeJointDescType)
extern PyTypeObject EPPHHingeJointDescType;
PyObject* __EPDECL newEPPHHingeJointDesc(struct PHHingeJointDesc);
PyObject* __EPDECL newEPPHHingeJointDesc(struct PHHingeJointDesc*);

//}EPPHHingeJointDesc

//{*********EPPHSliderJointIf*******
#define EPPHSliderJointIf_Check(ob) PyObject_TypeCheck(ob, &EPPHSliderJointIfType)
extern PyTypeObject EPPHSliderJointIfType;
PyObject* __EPDECL newEPPHSliderJointIf(struct PHSliderJointIf);
PyObject* __EPDECL newEPPHSliderJointIf(struct PHSliderJointIf*);

//}EPPHSliderJointIf

//{*********EPPHSliderJointDesc*******
#define EPPHSliderJointDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHSliderJointDescType)
extern PyTypeObject EPPHSliderJointDescType;
PyObject* __EPDECL newEPPHSliderJointDesc(struct PHSliderJointDesc);
PyObject* __EPDECL newEPPHSliderJointDesc(struct PHSliderJointDesc*);

//}EPPHSliderJointDesc

//{*********EPPHPathPoint*******
#define EPPHPathPoint_Check(ob) PyObject_TypeCheck(ob, &EPPHPathPointType)
extern PyTypeObject EPPHPathPointType;
PyObject* __EPDECL newEPPHPathPoint(struct PHPathPoint);
PyObject* __EPDECL newEPPHPathPoint(struct PHPathPoint*);

//}EPPHPathPoint

//{*********EPPHPathDesc*******
#define EPPHPathDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHPathDescType)
extern PyTypeObject EPPHPathDescType;
PyObject* __EPDECL newEPPHPathDesc(struct PHPathDesc);
PyObject* __EPDECL newEPPHPathDesc(struct PHPathDesc*);

//}EPPHPathDesc

//{*********EPPHPathIf*******
#define EPPHPathIf_Check(ob) PyObject_TypeCheck(ob, &EPPHPathIfType)
extern PyTypeObject EPPHPathIfType;
PyObject* __EPDECL newEPPHPathIf(struct PHPathIf);
PyObject* __EPDECL newEPPHPathIf(struct PHPathIf*);
PyObject* EPPHPathIf_AddPoint( EPPHPathIf* self,PyObject* tuple );
PyObject* EPPHPathIf_IsLoop( EPPHPathIf* self );
PyObject* EPPHPathIf_SetLoop( EPPHPathIf* self,PyObject* tuple );//Overloaded

//}EPPHPathIf

//{*********EPPHPathJointIf*******
#define EPPHPathJointIf_Check(ob) PyObject_TypeCheck(ob, &EPPHPathJointIfType)
extern PyTypeObject EPPHPathJointIfType;
PyObject* __EPDECL newEPPHPathJointIf(struct PHPathJointIf);
PyObject* __EPDECL newEPPHPathJointIf(struct PHPathJointIf*);
PyObject* EPPHPathJointIf_SetPosition( EPPHPathJointIf* self,PyFloatObject* var1 );

//}EPPHPathJointIf

//{*********EPPHPathJointDesc*******
#define EPPHPathJointDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHPathJointDescType)
extern PyTypeObject EPPHPathJointDescType;
PyObject* __EPDECL newEPPHPathJointDesc(struct PHPathJointDesc);
PyObject* __EPDECL newEPPHPathJointDesc(struct PHPathJointDesc*);

//}EPPHPathJointDesc

//{*********EPPHBallJointIf*******
#define EPPHBallJointIf_Check(ob) PyObject_TypeCheck(ob, &EPPHBallJointIfType)
extern PyTypeObject EPPHBallJointIfType;
PyObject* __EPDECL newEPPHBallJointIf(struct PHBallJointIf);
PyObject* __EPDECL newEPPHBallJointIf(struct PHBallJointIf*);
PyObject* EPPHBallJointIf_GetAngle( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetConstLine( EPPHBallJointIf* self,PyObject* tuple );
PyObject* EPPHBallJointIf_GetDamper( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetDeformationMode( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetHardnessRate( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetInertia( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetMotorTorque( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetOffsetForce( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetPosition( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetSecondDamper( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetSpring( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetSwingRange( EPPHBallJointIf* self,EPVec2d* var1 );
PyObject* EPPHBallJointIf_GetTargetPosition( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetTargetVelocity( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetTorqueMax( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetTrajectoryVelocity( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetTwistRange( EPPHBallJointIf* self,EPVec2d* var1 );
PyObject* EPPHBallJointIf_GetVelocity( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetYieldStress( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_GetmotorfNorm( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_IsLimit( EPPHBallJointIf* self );
PyObject* EPPHBallJointIf_SetConstLine( EPPHBallJointIf* self,PyObject* tuple );
PyObject* EPPHBallJointIf_SetConstPoint( EPPHBallJointIf* self,PyObject* tuple );
PyObject* EPPHBallJointIf_SetConstraintMode( EPPHBallJointIf* self,PyLongObject* var1 );
PyObject* EPPHBallJointIf_SetDamper( EPPHBallJointIf* self,PyFloatObject* var1 );
PyObject* EPPHBallJointIf_SetHardnessRate( EPPHBallJointIf* self,PyFloatObject* var1 );
PyObject* EPPHBallJointIf_SetInertia( EPPHBallJointIf* self,EPVec3d* var1 );
PyObject* EPPHBallJointIf_SetMotorTorque( EPPHBallJointIf* self,EPVec3d* var1 );
PyObject* EPPHBallJointIf_SetOffsetForce( EPPHBallJointIf* self,EPVec3d* var1 );
PyObject* EPPHBallJointIf_SetSecondDamper( EPPHBallJointIf* self,PyFloatObject* var1 );
PyObject* EPPHBallJointIf_SetSpring( EPPHBallJointIf* self,PyFloatObject* var1 );
PyObject* EPPHBallJointIf_SetSwingRange( EPPHBallJointIf* self,EPVec2d* var1 );
PyObject* EPPHBallJointIf_SetTargetPosition( EPPHBallJointIf* self,EPQuaterniond* var1 );
PyObject* EPPHBallJointIf_SetTargetVelocity( EPPHBallJointIf* self,EPVec3d* var1 );
PyObject* EPPHBallJointIf_SetTorqueMax( EPPHBallJointIf* self,PyFloatObject* var1 );
PyObject* EPPHBallJointIf_SetTrajectoryVelocity( EPPHBallJointIf* self,EPVec3d* var1 );
PyObject* EPPHBallJointIf_SetTwistPole( EPPHBallJointIf* self,EPVec2d* var1 );
PyObject* EPPHBallJointIf_SetTwistRange( EPPHBallJointIf* self,EPVec2d* var1 );
PyObject* EPPHBallJointIf_SetYieldStress( EPPHBallJointIf* self,PyFloatObject* var1 );

//}EPPHBallJointIf

//{*********EPPHBallJointDesc*******
#define EPPHBallJointDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHBallJointDescType)
extern PyTypeObject EPPHBallJointDescType;
PyObject* __EPDECL newEPPHBallJointDesc(struct PHBallJointDesc);
PyObject* __EPDECL newEPPHBallJointDesc(struct PHBallJointDesc*);

//}EPPHBallJointDesc

//{*********EPPHSpringIf*******
#define EPPHSpringIf_Check(ob) PyObject_TypeCheck(ob, &EPPHSpringIfType)
extern PyTypeObject EPPHSpringIfType;
PyObject* __EPDECL newEPPHSpringIf(struct PHSpringIf);
PyObject* __EPDECL newEPPHSpringIf(struct PHSpringIf*);
PyObject* EPPHSpringIf_GetDamper( EPPHSpringIf* self );
PyObject* EPPHSpringIf_GetDamperOri( EPPHSpringIf* self );
PyObject* EPPHSpringIf_GetSpring( EPPHSpringIf* self );
PyObject* EPPHSpringIf_GetSpringOri( EPPHSpringIf* self );
PyObject* EPPHSpringIf_SetDamper( EPPHSpringIf* self,EPVec3d* var1 );
PyObject* EPPHSpringIf_SetDamperOri( EPPHSpringIf* self,PyFloatObject* var1 );
PyObject* EPPHSpringIf_SetSpring( EPPHSpringIf* self,EPVec3d* var1 );
PyObject* EPPHSpringIf_SetSpringOri( EPPHSpringIf* self,PyFloatObject* var1 );

//}EPPHSpringIf

//{*********EPPHSpringDesc*******
#define EPPHSpringDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHSpringDescType)
extern PyTypeObject EPPHSpringDescType;
PyObject* __EPDECL newEPPHSpringDesc(struct PHSpringDesc);
PyObject* __EPDECL newEPPHSpringDesc(struct PHSpringDesc*);

//}EPPHSpringDesc

//{*********EPPHTreeNodeIf*******
#define EPPHTreeNodeIf_Check(ob) PyObject_TypeCheck(ob, &EPPHTreeNodeIfType)
extern PyTypeObject EPPHTreeNodeIfType;
PyObject* __EPDECL newEPPHTreeNodeIf(struct PHTreeNodeIf);
PyObject* __EPDECL newEPPHTreeNodeIf(struct PHTreeNodeIf*);
PyObject* EPPHTreeNodeIf_Enable( EPPHTreeNodeIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHTreeNodeIf_GetChild( EPPHTreeNodeIf* self,PyLongObject* var1 );
PyObject* EPPHTreeNodeIf_IsEnabled( EPPHTreeNodeIf* self );
PyObject* EPPHTreeNodeIf_NChildren( EPPHTreeNodeIf* self );

//}EPPHTreeNodeIf

//{*********EPPHRootNodeIf*******
#define EPPHRootNodeIf_Check(ob) PyObject_TypeCheck(ob, &EPPHRootNodeIfType)
extern PyTypeObject EPPHRootNodeIfType;
PyObject* __EPDECL newEPPHRootNodeIf(struct PHRootNodeIf);
PyObject* __EPDECL newEPPHRootNodeIf(struct PHRootNodeIf*);

//}EPPHRootNodeIf

//{*********EPPHTreeNode1DIf*******
#define EPPHTreeNode1DIf_Check(ob) PyObject_TypeCheck(ob, &EPPHTreeNode1DIfType)
extern PyTypeObject EPPHTreeNode1DIfType;
PyObject* __EPDECL newEPPHTreeNode1DIf(struct PHTreeNode1DIf);
PyObject* __EPDECL newEPPHTreeNode1DIf(struct PHTreeNode1DIf*);

//}EPPHTreeNode1DIf

//{*********EPPHHingeJointNodeIf*******
#define EPPHHingeJointNodeIf_Check(ob) PyObject_TypeCheck(ob, &EPPHHingeJointNodeIfType)
extern PyTypeObject EPPHHingeJointNodeIfType;
PyObject* __EPDECL newEPPHHingeJointNodeIf(struct PHHingeJointNodeIf);
PyObject* __EPDECL newEPPHHingeJointNodeIf(struct PHHingeJointNodeIf*);

//}EPPHHingeJointNodeIf

//{*********EPPHSliderJointNodeIf*******
#define EPPHSliderJointNodeIf_Check(ob) PyObject_TypeCheck(ob, &EPPHSliderJointNodeIfType)
extern PyTypeObject EPPHSliderJointNodeIfType;
PyObject* __EPDECL newEPPHSliderJointNodeIf(struct PHSliderJointNodeIf);
PyObject* __EPDECL newEPPHSliderJointNodeIf(struct PHSliderJointNodeIf*);

//}EPPHSliderJointNodeIf

//{*********EPPHPathJointNodeIf*******
#define EPPHPathJointNodeIf_Check(ob) PyObject_TypeCheck(ob, &EPPHPathJointNodeIfType)
extern PyTypeObject EPPHPathJointNodeIfType;
PyObject* __EPDECL newEPPHPathJointNodeIf(struct PHPathJointNodeIf);
PyObject* __EPDECL newEPPHPathJointNodeIf(struct PHPathJointNodeIf*);

//}EPPHPathJointNodeIf

//{*********EPPHBallJointNodeIf*******
#define EPPHBallJointNodeIf_Check(ob) PyObject_TypeCheck(ob, &EPPHBallJointNodeIfType)
extern PyTypeObject EPPHBallJointNodeIfType;
PyObject* __EPDECL newEPPHBallJointNodeIf(struct PHBallJointNodeIf);
PyObject* __EPDECL newEPPHBallJointNodeIf(struct PHBallJointNodeIf*);

//}EPPHBallJointNodeIf

//{*********EPPHGearIf*******
#define EPPHGearIf_Check(ob) PyObject_TypeCheck(ob, &EPPHGearIfType)
extern PyTypeObject EPPHGearIfType;
PyObject* __EPDECL newEPPHGearIf(struct PHGearIf);
PyObject* __EPDECL newEPPHGearIf(struct PHGearIf*);

//}EPPHGearIf

//{*********EPPHInactiveSolids*******
#define EPPHInactiveSolids_Check(ob) PyObject_TypeCheck(ob, &EPPHInactiveSolidsType)
extern PyTypeObject EPPHInactiveSolidsType;
PyObject* __EPDECL newEPPHInactiveSolids(struct PHInactiveSolids);
PyObject* __EPDECL newEPPHInactiveSolids(struct PHInactiveSolids*);

//}EPPHInactiveSolids

//{*********EPPHRaycastHit*******
#define EPPHRaycastHit_Check(ob) PyObject_TypeCheck(ob, &EPPHRaycastHitType)
extern PyTypeObject EPPHRaycastHitType;
PyObject* __EPDECL newEPPHRaycastHit(struct PHRaycastHit);
PyObject* __EPDECL newEPPHRaycastHit(struct PHRaycastHit*);

//}EPPHRaycastHit

//{*********EPPHRayDesc*******
#define EPPHRayDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHRayDescType)
extern PyTypeObject EPPHRayDescType;
PyObject* __EPDECL newEPPHRayDesc(struct PHRayDesc);
PyObject* __EPDECL newEPPHRayDesc(struct PHRayDesc*);

//}EPPHRayDesc

//{*********EPPHRayIf*******
#define EPPHRayIf_Check(ob) PyObject_TypeCheck(ob, &EPPHRayIfType)
extern PyTypeObject EPPHRayIfType;
PyObject* __EPDECL newEPPHRayIf(struct PHRayIf);
PyObject* __EPDECL newEPPHRayIf(struct PHRayIf*);
PyObject* EPPHRayIf_Apply( EPPHRayIf* self );
PyObject* EPPHRayIf_GetDirection( EPPHRayIf* self );
PyObject* EPPHRayIf_GetDynamicalNearest( EPPHRayIf* self );
PyObject* EPPHRayIf_GetHits( EPPHRayIf* self );
PyObject* EPPHRayIf_GetNearest( EPPHRayIf* self );
PyObject* EPPHRayIf_GetOrigin( EPPHRayIf* self );
PyObject* EPPHRayIf_NHits( EPPHRayIf* self );
PyObject* EPPHRayIf_SetDirection( EPPHRayIf* self,EPVec3d* var1 );
PyObject* EPPHRayIf_SetOrigin( EPPHRayIf* self,EPVec3d* var1 );

//}EPPHRayIf

//{*********EPPHSceneState*******
#define EPPHSceneState_Check(ob) PyObject_TypeCheck(ob, &EPPHSceneStateType)
extern PyTypeObject EPPHSceneStateType;
PyObject* __EPDECL newEPPHSceneState(struct PHSceneState);
PyObject* __EPDECL newEPPHSceneState(struct PHSceneState*);
PyObject* EPPHSceneState_Init( EPPHSceneState* self );

//}EPPHSceneState

//{*********EPPHSceneDesc*******
#define EPPHSceneDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHSceneDescType)
extern PyTypeObject EPPHSceneDescType;
PyObject* __EPDECL newEPPHSceneDesc(struct PHSceneDesc);
PyObject* __EPDECL newEPPHSceneDesc(struct PHSceneDesc*);
PyObject* EPPHSceneDesc_Init( EPPHSceneDesc* self );

//}EPPHSceneDesc

//{*********EPPHSceneIf*******
#define EPPHSceneIf_Check(ob) PyObject_TypeCheck(ob, &EPPHSceneIfType)
extern PyTypeObject EPPHSceneIfType;
PyObject* __EPDECL newEPPHSceneIf(struct PHSceneIf);
PyObject* __EPDECL newEPPHSceneIf(struct PHSceneIf*);
PyObject* EPPHSceneIf_Clear( EPPHSceneIf* self );
PyObject* EPPHSceneIf_ClearForce( EPPHSceneIf* self );
PyObject* EPPHSceneIf_CreateGear( EPPHSceneIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSceneIf_CreateJoint( EPPHSceneIf* self,PyObject* tuple );
PyObject* EPPHSceneIf_CreatePath( EPPHSceneIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSceneIf_CreateRay( EPPHSceneIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSceneIf_CreateRootNode( EPPHSceneIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSceneIf_CreateSolid( EPPHSceneIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSceneIf_CreateTreeNode( EPPHSceneIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSceneIf_GenerateForce( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetAirResistanceRate( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetConstraintEngine( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetContact( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetCount( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetEngine( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetGear( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetGravity( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetGravityEngine( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetIKActuator( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetIKEndEffector( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetIKEngine( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetJoint( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetNumIteration( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetPath( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetPenaltyEngine( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetRay( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetRootNode( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_GetSdk( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetSolidPair( EPPHSceneIf* self,PyObject* tuple );
PyObject* EPPHSceneIf_GetTimeStep( EPPHSceneIf* self );
PyObject* EPPHSceneIf_GetTimeStepInv( EPPHSceneIf* self );
PyObject* EPPHSceneIf_Integrate( EPPHSceneIf* self );
PyObject* EPPHSceneIf_IntegratePart1( EPPHSceneIf* self );
PyObject* EPPHSceneIf_IntegratePart2( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NContacts( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NEngines( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NGears( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NIKActuators( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NIKEndEffectors( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NJoints( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NPaths( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NRays( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NRootNodes( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NSolidPairs( EPPHSceneIf* self );
PyObject* EPPHSceneIf_NSolids( EPPHSceneIf* self );
PyObject* EPPHSceneIf_SetAirResistanceRate( EPPHSceneIf* self,PyFloatObject* var1 );
PyObject* EPPHSceneIf_SetCount( EPPHSceneIf* self,EPunsignedint* var1 );
PyObject* EPPHSceneIf_SetGravity( EPPHSceneIf* self,EPVec3d* var1 );
PyObject* EPPHSceneIf_SetNumIteration( EPPHSceneIf* self,PyLongObject* var1 );
PyObject* EPPHSceneIf_SetStateMode( EPPHSceneIf* self,PyObject* var1 );
PyObject* EPPHSceneIf_SetTimeStep( EPPHSceneIf* self,PyFloatObject* var1 );
PyObject* EPPHSceneIf_Step( EPPHSceneIf* self );

//}EPPHSceneIf

//{*********EPPHSdkDesc*******
#define EPPHSdkDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHSdkDescType)
extern PyTypeObject EPPHSdkDescType;
PyObject* __EPDECL newEPPHSdkDesc(struct PHSdkDesc);
PyObject* __EPDECL newEPPHSdkDesc(struct PHSdkDesc*);

//}EPPHSdkDesc

//{*********EPPHSdkIf*******
#define EPPHSdkIf_Check(ob) PyObject_TypeCheck(ob, &EPPHSdkIfType)
extern PyTypeObject EPPHSdkIfType;
PyObject* __EPDECL newEPPHSdkIf(struct PHSdkIf);
PyObject* __EPDECL newEPPHSdkIf(struct PHSdkIf*);
PyObject* EPPHSdkIf_CreateScene( EPPHSdkIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSdkIf_CreateSdk( EPPHSdkIf* self );
PyObject* EPPHSdkIf_CreateShape( EPPHSdkIf* self,PyObject* tuple );
PyObject* EPPHSdkIf_GetScene( EPPHSdkIf* self,PyLongObject* var1 );
PyObject* EPPHSdkIf_GetShape( EPPHSdkIf* self,PyLongObject* var1 );
PyObject* EPPHSdkIf_MergeScene( EPPHSdkIf* self,PyObject* tuple );
PyObject* EPPHSdkIf_NScene( EPPHSdkIf* self );
PyObject* EPPHSdkIf_NShape( EPPHSdkIf* self );
PyObject* EPPHSdkIf_RegisterSdk( EPPHSdkIf* self );

//}EPPHSdkIf

//{*********EPPHFrameDesc*******
#define EPPHFrameDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHFrameDescType)
extern PyTypeObject EPPHFrameDescType;
PyObject* __EPDECL newEPPHFrameDesc(struct PHFrameDesc);
PyObject* __EPDECL newEPPHFrameDesc(struct PHFrameDesc*);

//}EPPHFrameDesc

//{*********EPPHFrameIf*******
#define EPPHFrameIf_Check(ob) PyObject_TypeCheck(ob, &EPPHFrameIfType)
extern PyTypeObject EPPHFrameIfType;
PyObject* __EPDECL newEPPHFrameIf(struct PHFrameIf);
PyObject* __EPDECL newEPPHFrameIf(struct PHFrameIf*);
PyObject* EPPHFrameIf_GetPose( EPPHFrameIf* self );
PyObject* EPPHFrameIf_SetPose( EPPHFrameIf* self,EPPosed* var1 );

//}EPPHFrameIf

//{*********EPPHSolidState*******
#define EPPHSolidState_Check(ob) PyObject_TypeCheck(ob, &EPPHSolidStateType)
extern PyTypeObject EPPHSolidStateType;
PyObject* __EPDECL newEPPHSolidState(struct PHSolidState);
PyObject* __EPDECL newEPPHSolidState(struct PHSolidState*);

//}EPPHSolidState

//{*********EPPHSolidDesc*******
#define EPPHSolidDesc_Check(ob) PyObject_TypeCheck(ob, &EPPHSolidDescType)
extern PyTypeObject EPPHSolidDescType;
PyObject* __EPDECL newEPPHSolidDesc(struct PHSolidDesc);
PyObject* __EPDECL newEPPHSolidDesc(struct PHSolidDesc*);
PyObject* EPPHSolidDesc_Init( EPPHSolidDesc* self );

//}EPPHSolidDesc

//{*********EPPHSolidIf*******
#define EPPHSolidIf_Check(ob) PyObject_TypeCheck(ob, &EPPHSolidIfType)
extern PyTypeObject EPPHSolidIfType;
PyObject* __EPDECL newEPPHSolidIf(struct PHSolidIf);
PyObject* __EPDECL newEPPHSolidIf(struct PHSolidIf*);
PyObject* EPPHSolidIf_AddForce( EPPHSolidIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSolidIf_AddShape( EPPHSolidIf* self,EPCDShapeIf* var1 );
PyObject* EPPHSolidIf_AddTorque( EPPHSolidIf* self,EPVec3d* var1 );
PyObject* EPPHSolidIf_ClearShape( EPPHSolidIf* self );
PyObject* EPPHSolidIf_CompInertia( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetAngularVelocity( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetCenterOfMass( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetCenterPosition( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetDeltaPosition( EPPHSolidIf* self,PyObject* tuple );//Overloaded
PyObject* EPPHSolidIf_GetForce( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetFramePosition( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetInertia( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetInertiaInv( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetMass( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetMassInv( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetOrientation( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetPose( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetShape( EPPHSolidIf* self,PyLongObject* var1 );
PyObject* EPPHSolidIf_GetShapePose( EPPHSolidIf* self,PyLongObject* var1 );
PyObject* EPPHSolidIf_GetTorque( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetTreeNode( EPPHSolidIf* self );
PyObject* EPPHSolidIf_GetVelocity( EPPHSolidIf* self );
PyObject* EPPHSolidIf_IsDynamical( EPPHSolidIf* self );
PyObject* EPPHSolidIf_IsIntegrate( EPPHSolidIf* self );
PyObject* EPPHSolidIf_NShape( EPPHSolidIf* self );
PyObject* EPPHSolidIf_SetAngularVelocity( EPPHSolidIf* self,EPVec3d* var1 );
PyObject* EPPHSolidIf_SetCenterOfMass( EPPHSolidIf* self,EPVec3d* var1 );
PyObject* EPPHSolidIf_SetCenterPosition( EPPHSolidIf* self,EPVec3d* var1 );
PyObject* EPPHSolidIf_SetDynamical( EPPHSolidIf* self,PyObject* var1 );
PyObject* EPPHSolidIf_SetFramePosition( EPPHSolidIf* self,EPVec3d* var1 );
PyObject* EPPHSolidIf_SetGravity( EPPHSolidIf* self,PyObject* var1 );
PyObject* EPPHSolidIf_SetInertia( EPPHSolidIf* self,EPMatrix3d* var1 );
PyObject* EPPHSolidIf_SetIntegrate( EPPHSolidIf* self,PyObject* var1 );
PyObject* EPPHSolidIf_SetMass( EPPHSolidIf* self,PyFloatObject* var1 );
PyObject* EPPHSolidIf_SetOrientation( EPPHSolidIf* self,EPQuaterniond* var1 );
PyObject* EPPHSolidIf_SetPose( EPPHSolidIf* self,EPPosed* var1 );
PyObject* EPPHSolidIf_SetShapePose( EPPHSolidIf* self,PyObject* tuple );
PyObject* EPPHSolidIf_SetVelocity( EPPHSolidIf* self,EPVec3d* var1 );

//}EPPHSolidIf

//{*********EPPHShapePairForLCPIf*******
#define EPPHShapePairForLCPIf_Check(ob) PyObject_TypeCheck(ob, &EPPHShapePairForLCPIfType)
extern PyTypeObject EPPHShapePairForLCPIfType;
PyObject* __EPDECL newEPPHShapePairForLCPIf(struct PHShapePairForLCPIf);
PyObject* __EPDECL newEPPHShapePairForLCPIf(struct PHShapePairForLCPIf*);
PyObject* EPPHShapePairForLCPIf_GetContactDimension( EPPHShapePairForLCPIf* self );
PyObject* EPPHShapePairForLCPIf_GetNormalVector( EPPHShapePairForLCPIf* self );
PyObject* EPPHShapePairForLCPIf_GetSectionVertex( EPPHShapePairForLCPIf* self,PyLongObject* var1 );
PyObject* EPPHShapePairForLCPIf_GetShape( EPPHShapePairForLCPIf* self,PyLongObject* var1 );
PyObject* EPPHShapePairForLCPIf_NSectionVertexes( EPPHShapePairForLCPIf* self );

//}EPPHShapePairForLCPIf

//{*********EPPHSolidPairForLCPIf*******
#define EPPHSolidPairForLCPIf_Check(ob) PyObject_TypeCheck(ob, &EPPHSolidPairForLCPIfType)
extern PyTypeObject EPPHSolidPairForLCPIfType;
PyObject* __EPDECL newEPPHSolidPairForLCPIf(struct PHSolidPairForLCPIf);
PyObject* __EPDECL newEPPHSolidPairForLCPIf(struct PHSolidPairForLCPIf*);
PyObject* EPPHSolidPairForLCPIf_GetCommonPoint( EPPHSolidPairForLCPIf* self,PyObject* tuple );
PyObject* EPPHSolidPairForLCPIf_GetContactDepth( EPPHSolidPairForLCPIf* self,PyObject* tuple );
PyObject* EPPHSolidPairForLCPIf_GetContactState( EPPHSolidPairForLCPIf* self,PyObject* tuple );
PyObject* EPPHSolidPairForLCPIf_GetLastContactCount( EPPHSolidPairForLCPIf* self,PyObject* tuple );
PyObject* EPPHSolidPairForLCPIf_GetShapePair( EPPHSolidPairForLCPIf* self,PyObject* tuple );
PyObject* EPPHSolidPairForLCPIf_GetSolid( EPPHSolidPairForLCPIf* self,PyLongObject* var1 );

//}EPPHSolidPairForLCPIf
void __EPDECL initPhysics(void) ;
#endif

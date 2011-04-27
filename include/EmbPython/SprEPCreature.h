#ifndef SprEPCreature_H
#define SprEPCreature_H
#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Python\Add_decl.h"
#include "..\..\include\Python\pyport.h"
#include "..\..\include\Python\patchlevel.h"
#include "..\..\include\Python\Python.h"
#include "..\..\include\Python\object.h"
#include "..\..\include\EmbPython\SprEPUtility.h"
#include "..\..\include\EmbPython\SprEPFoundation.h"
#include "..\..\include\EmbPython\SprEPPhysics.h"
#include "..\..\include\EmbPython\SprEPGraphics.h"
#include "..\..\include\Creature\SprCRBody.h"
#include "..\..\include\Creature\SprCRCreature.h"
#include "..\..\include\Creature\SprCREngine.h"
#include "..\..\include\Creature\SprCRGazeController.h"
#include "..\..\include\Creature\SprCRReachController.h"
#include "..\..\include\Creature\SprCRScene.h"
#include "..\..\include\Creature\SprCRSdk.h"
#include "..\..\include\Creature\SprCRTouchSensor.h"
#include "..\..\include\Creature\SprCRTrajectoryController.h"
#include "..\..\include\Creature\SprCRVisualSensor.h"
typedef struct
{
	PyObject_HEAD
	UTRef<CRBodyPartIf> ptr;
} EPCRBodyPartIf;
typedef struct
{
	PyObject_HEAD
	struct CRBodyPartDesc* ptr;
} EPCRBodyPartDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRSolidIf> ptr;
} EPCRSolidIf;
typedef struct
{
	PyObject_HEAD
	struct CRSolidDesc* ptr;
} EPCRSolidDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRIKSolidIf> ptr;
} EPCRIKSolidIf;
typedef struct
{
	PyObject_HEAD
	struct CRIKSolidDesc* ptr;
} EPCRIKSolidDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRJointIf> ptr;
} EPCRJointIf;
typedef struct
{
	PyObject_HEAD
	struct CRJointDesc* ptr;
} EPCRJointDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRIKJointIf> ptr;
} EPCRIKJointIf;
typedef struct
{
	PyObject_HEAD
	struct CRIKJointDesc* ptr;
} EPCRIKJointDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRBodyIf> ptr;
} EPCRBodyIf;
typedef struct
{
	PyObject_HEAD
	struct CRBodyDesc* ptr;
} EPCRBodyDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRCreatureIf> ptr;
} EPCRCreatureIf;
typedef struct
{
	PyObject_HEAD
	struct CRCreatureDesc* ptr;
} EPCRCreatureDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CREngineIf> ptr;
} EPCREngineIf;
typedef struct
{
	PyObject_HEAD
	struct CREngineDesc* ptr;
} EPCREngineDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CREyeControllerIf> ptr;
} EPCREyeControllerIf;
typedef struct
{
	PyObject_HEAD
	struct CREyeControllerState* ptr;
} EPCREyeControllerState;
typedef struct
{
	PyObject_HEAD
	struct CREyeControllerDesc* ptr;
} EPCREyeControllerDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRNeckControllerIf> ptr;
} EPCRNeckControllerIf;
typedef struct
{
	PyObject_HEAD
	struct CRNeckControllerState* ptr;
} EPCRNeckControllerState;
typedef struct
{
	PyObject_HEAD
	struct CRNeckControllerDesc* ptr;
} EPCRNeckControllerDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRGazeControllerIf> ptr;
} EPCRGazeControllerIf;
typedef struct
{
	PyObject_HEAD
	struct CRGazeControllerState* ptr;
} EPCRGazeControllerState;
typedef struct
{
	PyObject_HEAD
	struct CRGazeControllerDesc* ptr;
} EPCRGazeControllerDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRAttentionControllerIf> ptr;
} EPCRAttentionControllerIf;
typedef struct
{
	PyObject_HEAD
	struct CRAttentionControllerDesc* ptr;
} EPCRAttentionControllerDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRReachingControllerIf> ptr;
} EPCRReachingControllerIf;
typedef struct
{
	PyObject_HEAD
	struct CRReachingControllerDesc* ptr;
} EPCRReachingControllerDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRReachingControllersIf> ptr;
} EPCRReachingControllersIf;
typedef struct
{
	PyObject_HEAD
	struct CRReachingControllersDesc* ptr;
} EPCRReachingControllersDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRGrabControllerIf> ptr;
} EPCRGrabControllerIf;
typedef struct
{
	PyObject_HEAD
	struct CRGrabControllerDesc* ptr;
} EPCRGrabControllerDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRSceneIf> ptr;
} EPCRSceneIf;
typedef struct
{
	PyObject_HEAD
	struct CRSceneDesc* ptr;
} EPCRSceneDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRSdkIf> ptr;
} EPCRSdkIf;
typedef struct
{
	PyObject_HEAD
	struct CRSdkDesc* ptr;
} EPCRSdkDesc;
typedef struct
{
	PyObject_HEAD
	struct CRContactInfo* ptr;
} EPCRContactInfo;
typedef struct
{
	PyObject_HEAD
	UTRef<CRTouchSensorIf> ptr;
} EPCRTouchSensorIf;
typedef struct
{
	PyObject_HEAD
	struct CRTouchSensorDesc* ptr;
} EPCRTouchSensorDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRTrajectoryIf> ptr;
} EPCRTrajectoryIf;
typedef struct
{
	PyObject_HEAD
	struct CRTrajectoryState* ptr;
} EPCRTrajectoryState;
typedef struct
{
	PyObject_HEAD
	struct CRTrajectoryDesc* ptr;
} EPCRTrajectoryDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRTrajectoryControllerIf> ptr;
} EPCRTrajectoryControllerIf;
typedef struct
{
	PyObject_HEAD
	struct CRTrajectoryControllerDesc* ptr;
} EPCRTrajectoryControllerDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CRVisualSensorIf> ptr;
} EPCRVisualSensorIf;
typedef struct
{
	PyObject_HEAD
	struct CRVisualSensorDesc* ptr;
} EPCRVisualSensorDesc;

//{*********EPCRBodyPartIf*******
#define EPCRBodyPartIf_Check(ob) PyObject_TypeCheck(ob, &EPCRBodyPartIfType)
extern PyTypeObject EPCRBodyPartIfType;
PyObject* __EPDECL newEPCRBodyPartIf(struct CRBodyPartIf);
PyObject* __EPDECL newEPCRBodyPartIf(struct CRBodyPartIf*);
PyObject* EPCRBodyPartIf_GetLabel( EPCRBodyPartIf* self );
PyObject* EPCRBodyPartIf_SetLabel( EPCRBodyPartIf* self,EPchar* var1 );

//}EPCRBodyPartIf

//{*********EPCRBodyPartDesc*******
#define EPCRBodyPartDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRBodyPartDescType)
extern PyTypeObject EPCRBodyPartDescType;
PyObject* __EPDECL newEPCRBodyPartDesc(struct CRBodyPartDesc);
PyObject* __EPDECL newEPCRBodyPartDesc(struct CRBodyPartDesc*);

//}EPCRBodyPartDesc

//{*********EPCRSolidIf*******
#define EPCRSolidIf_Check(ob) PyObject_TypeCheck(ob, &EPCRSolidIfType)
extern PyTypeObject EPCRSolidIfType;
PyObject* __EPDECL newEPCRSolidIf(struct CRSolidIf);
PyObject* __EPDECL newEPCRSolidIf(struct CRSolidIf*);
PyObject* EPCRSolidIf_GetPHSolid( EPCRSolidIf* self );
PyObject* EPCRSolidIf_SetPHSolid( EPCRSolidIf* self,EPPHSolidIf* var1 );

//}EPCRSolidIf

//{*********EPCRSolidDesc*******
#define EPCRSolidDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRSolidDescType)
extern PyTypeObject EPCRSolidDescType;
PyObject* __EPDECL newEPCRSolidDesc(struct CRSolidDesc);
PyObject* __EPDECL newEPCRSolidDesc(struct CRSolidDesc*);

//}EPCRSolidDesc

//{*********EPCRIKSolidIf*******
#define EPCRIKSolidIf_Check(ob) PyObject_TypeCheck(ob, &EPCRIKSolidIfType)
extern PyTypeObject EPCRIKSolidIfType;
PyObject* __EPDECL newEPCRIKSolidIf(struct CRIKSolidIf);
PyObject* __EPDECL newEPCRIKSolidIf(struct CRIKSolidIf*);
PyObject* EPCRIKSolidIf_GetIKEndEffector( EPCRIKSolidIf* self );
PyObject* EPCRIKSolidIf_SetIKEndEffector( EPCRIKSolidIf* self,EPPHIKEndEffectorIf* var1 );

//}EPCRIKSolidIf

//{*********EPCRIKSolidDesc*******
#define EPCRIKSolidDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRIKSolidDescType)
extern PyTypeObject EPCRIKSolidDescType;
PyObject* __EPDECL newEPCRIKSolidDesc(struct CRIKSolidDesc);
PyObject* __EPDECL newEPCRIKSolidDesc(struct CRIKSolidDesc*);

//}EPCRIKSolidDesc

//{*********EPCRJointIf*******
#define EPCRJointIf_Check(ob) PyObject_TypeCheck(ob, &EPCRJointIfType)
extern PyTypeObject EPCRJointIfType;
PyObject* __EPDECL newEPCRJointIf(struct CRJointIf);
PyObject* __EPDECL newEPCRJointIf(struct CRJointIf*);
PyObject* EPCRJointIf_GetPHJoint( EPCRJointIf* self );
PyObject* EPCRJointIf_SetPHJoint( EPCRJointIf* self,EPPHJointIf* var1 );
PyObject* EPCRJointIf_SetSpringRatio( EPCRJointIf* self,PyObject* tuple );

//}EPCRJointIf

//{*********EPCRJointDesc*******
#define EPCRJointDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRJointDescType)
extern PyTypeObject EPCRJointDescType;
PyObject* __EPDECL newEPCRJointDesc(struct CRJointDesc);
PyObject* __EPDECL newEPCRJointDesc(struct CRJointDesc*);

//}EPCRJointDesc

//{*********EPCRIKJointIf*******
#define EPCRIKJointIf_Check(ob) PyObject_TypeCheck(ob, &EPCRIKJointIfType)
extern PyTypeObject EPCRIKJointIfType;
PyObject* __EPDECL newEPCRIKJointIf(struct CRIKJointIf);
PyObject* __EPDECL newEPCRIKJointIf(struct CRIKJointIf*);
PyObject* EPCRIKJointIf_GetIKActuator( EPCRIKJointIf* self );
PyObject* EPCRIKJointIf_SetIKActuator( EPCRIKJointIf* self,EPPHIKActuatorIf* var1 );
PyObject* EPCRIKJointIf_SetIKSpringRatio( EPCRIKJointIf* self,PyObject* tuple );

//}EPCRIKJointIf

//{*********EPCRIKJointDesc*******
#define EPCRIKJointDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRIKJointDescType)
extern PyTypeObject EPCRIKJointDescType;
PyObject* __EPDECL newEPCRIKJointDesc(struct CRIKJointDesc);
PyObject* __EPDECL newEPCRIKJointDesc(struct CRIKJointDesc*);

//}EPCRIKJointDesc

//{*********EPCRBodyIf*******
#define EPCRBodyIf_Check(ob) PyObject_TypeCheck(ob, &EPCRBodyIfType)
extern PyTypeObject EPCRBodyIfType;
PyObject* __EPDECL newEPCRBodyIf(struct CRBodyIf);
PyObject* __EPDECL newEPCRBodyIf(struct CRBodyIf*);
PyObject* EPCRBodyIf_GetCenterOfMass( EPCRBodyIf* self );
PyObject* EPCRBodyIf_GetJoint( EPCRBodyIf* self,PyLongObject* var1 );
PyObject* EPCRBodyIf_GetSolid( EPCRBodyIf* self,PyLongObject* var1 );
PyObject* EPCRBodyIf_GetSumOfMass( EPCRBodyIf* self );
PyObject* EPCRBodyIf_NJoints( EPCRBodyIf* self );
PyObject* EPCRBodyIf_NSolids( EPCRBodyIf* self );

//}EPCRBodyIf

//{*********EPCRBodyDesc*******
#define EPCRBodyDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRBodyDescType)
extern PyTypeObject EPCRBodyDescType;
PyObject* __EPDECL newEPCRBodyDesc(struct CRBodyDesc);
PyObject* __EPDECL newEPCRBodyDesc(struct CRBodyDesc*);

//}EPCRBodyDesc

//{*********EPCRCreatureIf*******
#define EPCRCreatureIf_Check(ob) PyObject_TypeCheck(ob, &EPCRCreatureIfType)
extern PyTypeObject EPCRCreatureIfType;
PyObject* __EPDECL newEPCRCreatureIf(struct CRCreatureIf);
PyObject* __EPDECL newEPCRCreatureIf(struct CRCreatureIf*);
PyObject* EPCRCreatureIf_CreateBody( EPCRCreatureIf* self,PyObject* tuple );
PyObject* EPCRCreatureIf_CreateEngine( EPCRCreatureIf* self,PyObject* tuple );
PyObject* EPCRCreatureIf_CreateScene( EPCRCreatureIf* self,PyObject* tuple );
PyObject* EPCRCreatureIf_GetBody( EPCRCreatureIf* self,PyLongObject* var1 );
PyObject* EPCRCreatureIf_GetEngine( EPCRCreatureIf* self,PyLongObject* var1 );
PyObject* EPCRCreatureIf_GetPHScene( EPCRCreatureIf* self );
PyObject* EPCRCreatureIf_GetScene( EPCRCreatureIf* self,PyLongObject* var1 );
PyObject* EPCRCreatureIf_NBodies( EPCRCreatureIf* self );
PyObject* EPCRCreatureIf_NEngines( EPCRCreatureIf* self );
PyObject* EPCRCreatureIf_NScenes( EPCRCreatureIf* self );
PyObject* EPCRCreatureIf_Step( EPCRCreatureIf* self );

//}EPCRCreatureIf

//{*********EPCRCreatureDesc*******
#define EPCRCreatureDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRCreatureDescType)
extern PyTypeObject EPCRCreatureDescType;
PyObject* __EPDECL newEPCRCreatureDesc(struct CRCreatureDesc);
PyObject* __EPDECL newEPCRCreatureDesc(struct CRCreatureDesc*);

//}EPCRCreatureDesc

//{*********EPCREngineIf*******
#define EPCREngineIf_Check(ob) PyObject_TypeCheck(ob, &EPCREngineIfType)
extern PyTypeObject EPCREngineIfType;
PyObject* __EPDECL newEPCREngineIf(struct CREngineIf);
PyObject* __EPDECL newEPCREngineIf(struct CREngineIf*);
PyObject* EPCREngineIf_GetPriority( EPCREngineIf* self );
PyObject* EPCREngineIf_IsEnabled( EPCREngineIf* self );
PyObject* EPCREngineIf_Render( EPCREngineIf* self,EPGRRenderIf* var1 );
PyObject* EPCREngineIf_SetEnable( EPCREngineIf* self,PyObject* var1 );
PyObject* EPCREngineIf_Step( EPCREngineIf* self );

//}EPCREngineIf

//{*********EPCREngineDesc*******
#define EPCREngineDesc_Check(ob) PyObject_TypeCheck(ob, &EPCREngineDescType)
extern PyTypeObject EPCREngineDescType;
PyObject* __EPDECL newEPCREngineDesc(struct CREngineDesc);
PyObject* __EPDECL newEPCREngineDesc(struct CREngineDesc*);

//}EPCREngineDesc

//{*********EPCREyeControllerIf*******
#define EPCREyeControllerIf_Check(ob) PyObject_TypeCheck(ob, &EPCREyeControllerIfType)
extern PyTypeObject EPCREyeControllerIfType;
PyObject* __EPDECL newEPCREyeControllerIf(struct CREyeControllerIf);
PyObject* __EPDECL newEPCREyeControllerIf(struct CREyeControllerIf*);
PyObject* EPCREyeControllerIf_IsSaccading( EPCREyeControllerIf* self );
PyObject* EPCREyeControllerIf_LookAt( EPCREyeControllerIf* self,PyObject* tuple );

//}EPCREyeControllerIf

//{*********EPCREyeControllerState*******
#define EPCREyeControllerState_Check(ob) PyObject_TypeCheck(ob, &EPCREyeControllerStateType)
extern PyTypeObject EPCREyeControllerStateType;
PyObject* __EPDECL newEPCREyeControllerState(struct CREyeControllerState);
PyObject* __EPDECL newEPCREyeControllerState(struct CREyeControllerState*);

//}EPCREyeControllerState

//{*********EPCREyeControllerDesc*******
#define EPCREyeControllerDesc_Check(ob) PyObject_TypeCheck(ob, &EPCREyeControllerDescType)
extern PyTypeObject EPCREyeControllerDescType;
PyObject* __EPDECL newEPCREyeControllerDesc(struct CREyeControllerDesc);
PyObject* __EPDECL newEPCREyeControllerDesc(struct CREyeControllerDesc*);

//}EPCREyeControllerDesc

//{*********EPCRNeckControllerIf*******
#define EPCRNeckControllerIf_Check(ob) PyObject_TypeCheck(ob, &EPCRNeckControllerIfType)
extern PyTypeObject EPCRNeckControllerIfType;
PyObject* __EPDECL newEPCRNeckControllerIf(struct CRNeckControllerIf);
PyObject* __EPDECL newEPCRNeckControllerIf(struct CRNeckControllerIf*);
PyObject* EPCRNeckControllerIf_LookAt( EPCRNeckControllerIf* self,PyObject* tuple );

//}EPCRNeckControllerIf

//{*********EPCRNeckControllerState*******
#define EPCRNeckControllerState_Check(ob) PyObject_TypeCheck(ob, &EPCRNeckControllerStateType)
extern PyTypeObject EPCRNeckControllerStateType;
PyObject* __EPDECL newEPCRNeckControllerState(struct CRNeckControllerState);
PyObject* __EPDECL newEPCRNeckControllerState(struct CRNeckControllerState*);

//}EPCRNeckControllerState

//{*********EPCRNeckControllerDesc*******
#define EPCRNeckControllerDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRNeckControllerDescType)
extern PyTypeObject EPCRNeckControllerDescType;
PyObject* __EPDECL newEPCRNeckControllerDesc(struct CRNeckControllerDesc);
PyObject* __EPDECL newEPCRNeckControllerDesc(struct CRNeckControllerDesc*);

//}EPCRNeckControllerDesc

//{*********EPCRGazeControllerIf*******
#define EPCRGazeControllerIf_Check(ob) PyObject_TypeCheck(ob, &EPCRGazeControllerIfType)
extern PyTypeObject EPCRGazeControllerIfType;
PyObject* __EPDECL newEPCRGazeControllerIf(struct CRGazeControllerIf);
PyObject* __EPDECL newEPCRGazeControllerIf(struct CRGazeControllerIf*);
PyObject* EPCRGazeControllerIf_LookAt( EPCRGazeControllerIf* self,PyObject* tuple );

//}EPCRGazeControllerIf

//{*********EPCRGazeControllerState*******
#define EPCRGazeControllerState_Check(ob) PyObject_TypeCheck(ob, &EPCRGazeControllerStateType)
extern PyTypeObject EPCRGazeControllerStateType;
PyObject* __EPDECL newEPCRGazeControllerState(struct CRGazeControllerState);
PyObject* __EPDECL newEPCRGazeControllerState(struct CRGazeControllerState*);

//}EPCRGazeControllerState

//{*********EPCRGazeControllerDesc*******
#define EPCRGazeControllerDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRGazeControllerDescType)
extern PyTypeObject EPCRGazeControllerDescType;
PyObject* __EPDECL newEPCRGazeControllerDesc(struct CRGazeControllerDesc);
PyObject* __EPDECL newEPCRGazeControllerDesc(struct CRGazeControllerDesc*);

//}EPCRGazeControllerDesc

//{*********EPCRAttentionControllerIf*******
#define EPCRAttentionControllerIf_Check(ob) PyObject_TypeCheck(ob, &EPCRAttentionControllerIfType)
extern PyTypeObject EPCRAttentionControllerIfType;
PyObject* __EPDECL newEPCRAttentionControllerIf(struct CRAttentionControllerIf);
PyObject* __EPDECL newEPCRAttentionControllerIf(struct CRAttentionControllerIf*);

//}EPCRAttentionControllerIf

//{*********EPCRAttentionControllerDesc*******
#define EPCRAttentionControllerDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRAttentionControllerDescType)
extern PyTypeObject EPCRAttentionControllerDescType;
PyObject* __EPDECL newEPCRAttentionControllerDesc(struct CRAttentionControllerDesc);
PyObject* __EPDECL newEPCRAttentionControllerDesc(struct CRAttentionControllerDesc*);

//}EPCRAttentionControllerDesc

//{*********EPCRReachingControllerIf*******
#define EPCRReachingControllerIf_Check(ob) PyObject_TypeCheck(ob, &EPCRReachingControllerIfType)
extern PyTypeObject EPCRReachingControllerIfType;
PyObject* __EPDECL newEPCRReachingControllerIf(struct CRReachingControllerIf);
PyObject* __EPDECL newEPCRReachingControllerIf(struct CRReachingControllerIf*);
PyObject* EPCRReachingControllerIf_GetCRSolid( EPCRReachingControllerIf* self );
PyObject* EPCRReachingControllerIf_SetCRSolid( EPCRReachingControllerIf* self,EPCRIKSolidIf* var1 );
PyObject* EPCRReachingControllerIf_SetPos( EPCRReachingControllerIf* self,EPVec3f* var1 );
PyObject* EPCRReachingControllerIf_Start( EPCRReachingControllerIf* self,PyObject* tuple );
PyObject* EPCRReachingControllerIf_Stop( EPCRReachingControllerIf* self );

//}EPCRReachingControllerIf

//{*********EPCRReachingControllerDesc*******
#define EPCRReachingControllerDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRReachingControllerDescType)
extern PyTypeObject EPCRReachingControllerDescType;
PyObject* __EPDECL newEPCRReachingControllerDesc(struct CRReachingControllerDesc);
PyObject* __EPDECL newEPCRReachingControllerDesc(struct CRReachingControllerDesc*);

//}EPCRReachingControllerDesc

//{*********EPCRReachingControllersIf*******
#define EPCRReachingControllersIf_Check(ob) PyObject_TypeCheck(ob, &EPCRReachingControllersIfType)
extern PyTypeObject EPCRReachingControllersIfType;
PyObject* __EPDECL newEPCRReachingControllersIf(struct CRReachingControllersIf);
PyObject* __EPDECL newEPCRReachingControllersIf(struct CRReachingControllersIf*);
PyObject* EPCRReachingControllersIf_GetReachingController( EPCRReachingControllersIf* self,EPPHSolidIf* var1 );

//}EPCRReachingControllersIf

//{*********EPCRReachingControllersDesc*******
#define EPCRReachingControllersDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRReachingControllersDescType)
extern PyTypeObject EPCRReachingControllersDescType;
PyObject* __EPDECL newEPCRReachingControllersDesc(struct CRReachingControllersDesc);
PyObject* __EPDECL newEPCRReachingControllersDesc(struct CRReachingControllersDesc*);

//}EPCRReachingControllersDesc

//{*********EPCRGrabControllerIf*******
#define EPCRGrabControllerIf_Check(ob) PyObject_TypeCheck(ob, &EPCRGrabControllerIfType)
extern PyTypeObject EPCRGrabControllerIfType;
PyObject* __EPDECL newEPCRGrabControllerIf(struct CRGrabControllerIf);
PyObject* __EPDECL newEPCRGrabControllerIf(struct CRGrabControllerIf*);
PyObject* EPCRGrabControllerIf_Abort( EPCRGrabControllerIf* self );
PyObject* EPCRGrabControllerIf_AbortAll( EPCRGrabControllerIf* self );
PyObject* EPCRGrabControllerIf_GetControlState( EPCRGrabControllerIf* self );
PyObject* EPCRGrabControllerIf_IsPlaceComplete( EPCRGrabControllerIf* self );
PyObject* EPCRGrabControllerIf_IsPlaceable( EPCRGrabControllerIf* self,PyObject* tuple );//Overloaded
PyObject* EPCRGrabControllerIf_IsReachComplete( EPCRGrabControllerIf* self );
PyObject* EPCRGrabControllerIf_IsReachable( EPCRGrabControllerIf* self,PyObject* tuple );//Overloaded
PyObject* EPCRGrabControllerIf_IsUpholdComplete( EPCRGrabControllerIf* self );
PyObject* EPCRGrabControllerIf_IsUpholdable( EPCRGrabControllerIf* self );
PyObject* EPCRGrabControllerIf_Place( EPCRGrabControllerIf* self,EPVec3d* var1 );
PyObject* EPCRGrabControllerIf_Reach( EPCRGrabControllerIf* self,PyObject* tuple );
PyObject* EPCRGrabControllerIf_Uphold( EPCRGrabControllerIf* self );

//}EPCRGrabControllerIf

//{*********EPCRGrabControllerDesc*******
#define EPCRGrabControllerDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRGrabControllerDescType)
extern PyTypeObject EPCRGrabControllerDescType;
PyObject* __EPDECL newEPCRGrabControllerDesc(struct CRGrabControllerDesc);
PyObject* __EPDECL newEPCRGrabControllerDesc(struct CRGrabControllerDesc*);

//}EPCRGrabControllerDesc

//{*********EPCRSceneIf*******
#define EPCRSceneIf_Check(ob) PyObject_TypeCheck(ob, &EPCRSceneIfType)
extern PyTypeObject EPCRSceneIfType;
PyObject* __EPDECL newEPCRSceneIf(struct CRSceneIf);
PyObject* __EPDECL newEPCRSceneIf(struct CRSceneIf*);

//}EPCRSceneIf

//{*********EPCRSceneDesc*******
#define EPCRSceneDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRSceneDescType)
extern PyTypeObject EPCRSceneDescType;
PyObject* __EPDECL newEPCRSceneDesc(struct CRSceneDesc);
PyObject* __EPDECL newEPCRSceneDesc(struct CRSceneDesc*);

//}EPCRSceneDesc

//{*********EPCRSdkIf*******
#define EPCRSdkIf_Check(ob) PyObject_TypeCheck(ob, &EPCRSdkIfType)
extern PyTypeObject EPCRSdkIfType;
PyObject* __EPDECL newEPCRSdkIf(struct CRSdkIf);
PyObject* __EPDECL newEPCRSdkIf(struct CRSdkIf*);
PyObject* EPCRSdkIf_CreateCreature( EPCRSdkIf* self,PyObject* tuple );
PyObject* EPCRSdkIf_CreateSdk( EPCRSdkIf* self );
PyObject* EPCRSdkIf_GetCreature( EPCRSdkIf* self,PyLongObject* var1 );
PyObject* EPCRSdkIf_NCreatures( EPCRSdkIf* self );
PyObject* EPCRSdkIf_RegisterSdk( EPCRSdkIf* self );
PyObject* EPCRSdkIf_Step( EPCRSdkIf* self );

//}EPCRSdkIf

//{*********EPCRSdkDesc*******
#define EPCRSdkDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRSdkDescType)
extern PyTypeObject EPCRSdkDescType;
PyObject* __EPDECL newEPCRSdkDesc(struct CRSdkDesc);
PyObject* __EPDECL newEPCRSdkDesc(struct CRSdkDesc*);

//}EPCRSdkDesc

//{*********EPCRContactInfo*******
#define EPCRContactInfo_Check(ob) PyObject_TypeCheck(ob, &EPCRContactInfoType)
extern PyTypeObject EPCRContactInfoType;
PyObject* __EPDECL newEPCRContactInfo(struct CRContactInfo);
PyObject* __EPDECL newEPCRContactInfo(struct CRContactInfo*);

//}EPCRContactInfo

//{*********EPCRTouchSensorIf*******
#define EPCRTouchSensorIf_Check(ob) PyObject_TypeCheck(ob, &EPCRTouchSensorIfType)
extern PyTypeObject EPCRTouchSensorIfType;
PyObject* __EPDECL newEPCRTouchSensorIf(struct CRTouchSensorIf);
PyObject* __EPDECL newEPCRTouchSensorIf(struct CRTouchSensorIf*);
PyObject* EPCRTouchSensorIf_GetContact( EPCRTouchSensorIf* self,PyLongObject* var1 );
PyObject* EPCRTouchSensorIf_NContacts( EPCRTouchSensorIf* self );

//}EPCRTouchSensorIf

//{*********EPCRTouchSensorDesc*******
#define EPCRTouchSensorDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRTouchSensorDescType)
extern PyTypeObject EPCRTouchSensorDescType;
PyObject* __EPDECL newEPCRTouchSensorDesc(struct CRTouchSensorDesc);
PyObject* __EPDECL newEPCRTouchSensorDesc(struct CRTouchSensorDesc*);

//}EPCRTouchSensorDesc

//{*********EPCRTrajectoryIf*******
#define EPCRTrajectoryIf_Check(ob) PyObject_TypeCheck(ob, &EPCRTrajectoryIfType)
extern PyTypeObject EPCRTrajectoryIfType;
PyObject* __EPDECL newEPCRTrajectoryIf(struct CRTrajectoryIf);
PyObject* __EPDECL newEPCRTrajectoryIf(struct CRTrajectoryIf*);
PyObject* EPCRTrajectoryIf_Enable( EPCRTrajectoryIf* self,PyObject* var1 );
PyObject* EPCRTrajectoryIf_EnableAngularVelocityControl( EPCRTrajectoryIf* self,PyObject* var1 );
PyObject* EPCRTrajectoryIf_EnableOrientationControl( EPCRTrajectoryIf* self,PyObject* var1 );
PyObject* EPCRTrajectoryIf_EnablePositionControl( EPCRTrajectoryIf* self,PyObject* var1 );
PyObject* EPCRTrajectoryIf_EnableVelocityControl( EPCRTrajectoryIf* self,PyObject* var1 );
PyObject* EPCRTrajectoryIf_GetAngularVelocity( EPCRTrajectoryIf* self );
PyObject* EPCRTrajectoryIf_GetOrientation( EPCRTrajectoryIf* self );
PyObject* EPCRTrajectoryIf_GetPosition( EPCRTrajectoryIf* self );
PyObject* EPCRTrajectoryIf_GetVelocity( EPCRTrajectoryIf* self );
PyObject* EPCRTrajectoryIf_SetTargetAngularVelocity( EPCRTrajectoryIf* self,EPVec3f* var1 );
PyObject* EPCRTrajectoryIf_SetTargetOrientation( EPCRTrajectoryIf* self,EPVec3f* var1 );
PyObject* EPCRTrajectoryIf_SetTargetPosition( EPCRTrajectoryIf* self,EPVec3f* var1 );
PyObject* EPCRTrajectoryIf_SetTargetPositionInSolid( EPCRTrajectoryIf* self,EPVec3f* var1 );
PyObject* EPCRTrajectoryIf_SetTargetVelocity( EPCRTrajectoryIf* self,EPVec3f* var1 );
PyObject* EPCRTrajectoryIf_SetTimeLimit( EPCRTrajectoryIf* self,PyFloatObject* var1 );
PyObject* EPCRTrajectoryIf_Start( EPCRTrajectoryIf* self );
PyObject* EPCRTrajectoryIf_Step( EPCRTrajectoryIf* self );

//}EPCRTrajectoryIf

//{*********EPCRTrajectoryState*******
#define EPCRTrajectoryState_Check(ob) PyObject_TypeCheck(ob, &EPCRTrajectoryStateType)
extern PyTypeObject EPCRTrajectoryStateType;
PyObject* __EPDECL newEPCRTrajectoryState(struct CRTrajectoryState);
PyObject* __EPDECL newEPCRTrajectoryState(struct CRTrajectoryState*);

//}EPCRTrajectoryState

//{*********EPCRTrajectoryDesc*******
#define EPCRTrajectoryDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRTrajectoryDescType)
extern PyTypeObject EPCRTrajectoryDescType;
PyObject* __EPDECL newEPCRTrajectoryDesc(struct CRTrajectoryDesc);
PyObject* __EPDECL newEPCRTrajectoryDesc(struct CRTrajectoryDesc*);

//}EPCRTrajectoryDesc

//{*********EPCRTrajectoryControllerIf*******
#define EPCRTrajectoryControllerIf_Check(ob) PyObject_TypeCheck(ob, &EPCRTrajectoryControllerIfType)
extern PyTypeObject EPCRTrajectoryControllerIfType;
PyObject* __EPDECL newEPCRTrajectoryControllerIf(struct CRTrajectoryControllerIf);
PyObject* __EPDECL newEPCRTrajectoryControllerIf(struct CRTrajectoryControllerIf*);

//}EPCRTrajectoryControllerIf

//{*********EPCRTrajectoryControllerDesc*******
#define EPCRTrajectoryControllerDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRTrajectoryControllerDescType)
extern PyTypeObject EPCRTrajectoryControllerDescType;
PyObject* __EPDECL newEPCRTrajectoryControllerDesc(struct CRTrajectoryControllerDesc);
PyObject* __EPDECL newEPCRTrajectoryControllerDesc(struct CRTrajectoryControllerDesc*);

//}EPCRTrajectoryControllerDesc

//{*********EPCRVisualSensorIf*******
#define EPCRVisualSensorIf_Check(ob) PyObject_TypeCheck(ob, &EPCRVisualSensorIfType)
extern PyTypeObject EPCRVisualSensorIfType;
PyObject* __EPDECL newEPCRVisualSensorIf(struct CRVisualSensorIf);
PyObject* __EPDECL newEPCRVisualSensorIf(struct CRVisualSensorIf*);
PyObject* EPCRVisualSensorIf_IsInCenter( EPCRVisualSensorIf* self,PyObject* tuple );//Overloaded
PyObject* EPCRVisualSensorIf_IsSelfSolid( EPCRVisualSensorIf* self,EPPHSolidIf* var1 );
PyObject* EPCRVisualSensorIf_IsVisible( EPCRVisualSensorIf* self,PyObject* tuple );//Overloaded

//}EPCRVisualSensorIf

//{*********EPCRVisualSensorDesc*******
#define EPCRVisualSensorDesc_Check(ob) PyObject_TypeCheck(ob, &EPCRVisualSensorDescType)
extern PyTypeObject EPCRVisualSensorDescType;
PyObject* __EPDECL newEPCRVisualSensorDesc(struct CRVisualSensorDesc);
PyObject* __EPDECL newEPCRVisualSensorDesc(struct CRVisualSensorDesc*);

//}EPCRVisualSensorDesc
void __EPDECL initCreature(void) ;
#endif

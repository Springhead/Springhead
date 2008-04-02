#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Collision\SprCDShape.h"
#include "..\..\include\Physics\SprPHDef.h"
#include "..\..\include\Physics\SprPHEngine.h"
#include "..\..\include\Physics\SprPHJoint.h"
#include "..\..\include\Physics\SprPHNodeHandlers.h"
#include "..\..\include\Physics\SprPHScene.h"
#include "..\..\include\Physics\SprPHSdk.h"
#include "..\..\include\Physics\SprPHSolid.h"
#include "..\..\src\Base\BaseDebug.h"
#include "..\..\src\Foundation\UTTypeDesc.h"
#include "..\Foundation\Foundation.h"
#include "..\Foundation\Object.h"
#include "..\Foundation\Scene.h"
#include "..\Foundation\UTBaseType.h"
#include "..\Foundation\UTDllLoader.h"
#include "..\Foundation\UTDllLoaderImpl.h"
#include "..\Foundation\UTLoadContext.h"
#include "..\Foundation\UTLoadHandler.h"
#include "..\Foundation\UTMMTimer.h"
#include "..\Foundation\UTPath.h"
#include "..\Foundation\UTPreciseTimer.h"
#include "..\Foundation\UTTypeDesc.h"
#include "..\Collision\CDBox.h"
#include "..\Collision\CDCapsule.h"
#include "..\Collision\CDConvex.h"
#include "..\Collision\CDConvexMesh.h"
#include "..\Collision\CDCutRing.h"
#include "..\Collision\CDDetectorImp.h"
#include "..\Collision\CDQuickHull2D.h"
#include "..\Collision\CDQuickHull2DImp.h"
#include "..\Collision\CDQuickHull3D.h"
#include "..\Collision\CDQuickHull3DImp.h"
#include "..\Collision\CDShape.h"
#include "..\Collision\CDSphere.h"
#include "..\Collision\Collision.h"
#include "..\Physics\PHBallJoint.h"
#include "..\Physics\PHConstraint.h"
#include "..\Physics\PHConstraintEngine.h"
#include "..\Physics\PHContactDetector.h"
#include "..\Physics\PHContactPoint.h"
#include "..\Physics\PHEngine.h"
#include "..\Physics\PHForceField.h"
#include "..\Physics\PHGear.h"
#include "..\Physics\PHHingeJoint.h"
#include "..\Physics\PHJoint.h"
#include "..\Physics\PHPathJoint.h"
#include "..\Physics\PHPenaltyEngine.h"
#include "..\Physics\PHPoseSpring.h"
#include "..\Physics\PHScene.h"
#include "..\Physics\PHSdk.h"
#include "..\Physics\PHSliderJoint.h"
#include "..\Physics\PHSolid.h"
#include "..\Physics\PHSpatial.h"
#include "..\Physics\PHSpring.h"
#include "..\Physics\PHTreeNode.h"
#include "..\Physics\Physics.h"
namespace Spr{

void RegisterTypeDescPhysics(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	Spr::PHConstraintDesc* pPHConstraintDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHConstraintDesc");
	desc->size = sizeof(Spr::PHConstraintDesc);
	desc->ifInfo = PHConstraintIf::GetIfInfoStatic();
	((IfInfo*)PHConstraintIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHConstraintDesc>;
	field = desc->AddField("", "bool", "bEnabled", "");
	field->offset = int((char*)&(pPHConstraintDesc->bEnabled) - (char*)pPHConstraintDesc);
	field = desc->AddField("", "Posed", "poseSocket", "");
	field->offset = int((char*)&(pPHConstraintDesc->poseSocket) - (char*)pPHConstraintDesc);
	field = desc->AddField("", "Posed", "posePlug", "");
	field->offset = int((char*)&(pPHConstraintDesc->posePlug) - (char*)pPHConstraintDesc);
	db->RegisterDesc(desc);
	Spr::PHJoint1DDesc* pPHJoint1DDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHJoint1DDesc");
	desc->size = sizeof(Spr::PHJoint1DDesc);
	desc->ifInfo = PHJoint1DIf::GetIfInfoStatic();
	((IfInfo*)PHJoint1DIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHJoint1DDesc>;
	field = desc->AddBase("PHJointDesc");
	field->offset = int((char*)(PHJointDesc*)pPHJoint1DDesc - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "lower", "");
	field->offset = int((char*)&(pPHJoint1DDesc->lower) - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "upper", "");
	field->offset = int((char*)&(pPHJoint1DDesc->upper) - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "spring", "");
	field->offset = int((char*)&(pPHJoint1DDesc->spring) - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "origin", "");
	field->offset = int((char*)&(pPHJoint1DDesc->origin) - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "damper", "");
	field->offset = int((char*)&(pPHJoint1DDesc->damper) - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "desiredVelocity", "");
	field->offset = int((char*)&(pPHJoint1DDesc->desiredVelocity) - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "torque", "");
	field->offset = int((char*)&(pPHJoint1DDesc->torque) - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "fMax", "");
	field->offset = int((char*)&(pPHJoint1DDesc->fMax) - (char*)pPHJoint1DDesc);
	field = desc->AddField("", "double", "fMin", "");
	field->offset = int((char*)&(pPHJoint1DDesc->fMin) - (char*)pPHJoint1DDesc);
	db->RegisterDesc(desc);
	Spr::PHTreeNodeDesc* pPHTreeNodeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHTreeNodeDesc");
	desc->size = sizeof(Spr::PHTreeNodeDesc);
	desc->ifInfo = PHTreeNodeIf::GetIfInfoStatic();
	((IfInfo*)PHTreeNodeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHTreeNodeDesc>;
	field = desc->AddField("", "bool", "bEnabled", "");
	field->offset = int((char*)&(pPHTreeNodeDesc->bEnabled) - (char*)pPHTreeNodeDesc);
	db->RegisterDesc(desc);
	Spr::PHRootNodeDesc* pPHRootNodeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHRootNodeDesc");
	desc->size = sizeof(Spr::PHRootNodeDesc);
	desc->ifInfo = PHRootNodeIf::GetIfInfoStatic();
	((IfInfo*)PHRootNodeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHRootNodeDesc>;
	field = desc->AddBase("PHTreeNodeDesc");
	field->offset = int((char*)(PHTreeNodeDesc*)pPHRootNodeDesc - (char*)pPHRootNodeDesc);
	db->RegisterDesc(desc);
	Spr::PHTreeNode1DDesc* pPHTreeNode1DDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHTreeNode1DDesc");
	desc->size = sizeof(Spr::PHTreeNode1DDesc);
	desc->ifInfo = PHTreeNode1DIf::GetIfInfoStatic();
	((IfInfo*)PHTreeNode1DIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHTreeNode1DDesc>;
	field = desc->AddBase("PHTreeNodeDesc");
	field->offset = int((char*)(PHTreeNodeDesc*)pPHTreeNode1DDesc - (char*)pPHTreeNode1DDesc);
	db->RegisterDesc(desc);
	Spr::PHHingeJointNodeDesc* pPHHingeJointNodeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHHingeJointNodeDesc");
	desc->size = sizeof(Spr::PHHingeJointNodeDesc);
	desc->ifInfo = PHHingeJointNodeIf::GetIfInfoStatic();
	((IfInfo*)PHHingeJointNodeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHHingeJointNodeDesc>;
	field = desc->AddBase("PHTreeNode1DDesc");
	field->offset = int((char*)(PHTreeNode1DDesc*)pPHHingeJointNodeDesc - (char*)pPHHingeJointNodeDesc);
	db->RegisterDesc(desc);
	Spr::PHSliderJointNodeDesc* pPHSliderJointNodeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHSliderJointNodeDesc");
	desc->size = sizeof(Spr::PHSliderJointNodeDesc);
	desc->ifInfo = PHSliderJointNodeIf::GetIfInfoStatic();
	((IfInfo*)PHSliderJointNodeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHSliderJointNodeDesc>;
	field = desc->AddBase("PHTreeNode1DDesc");
	field->offset = int((char*)(PHTreeNode1DDesc*)pPHSliderJointNodeDesc - (char*)pPHSliderJointNodeDesc);
	db->RegisterDesc(desc);
	Spr::PHPathJointNodeDesc* pPHPathJointNodeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHPathJointNodeDesc");
	desc->size = sizeof(Spr::PHPathJointNodeDesc);
	desc->ifInfo = PHPathJointNodeIf::GetIfInfoStatic();
	((IfInfo*)PHPathJointNodeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHPathJointNodeDesc>;
	field = desc->AddBase("PHTreeNode1DDesc");
	field->offset = int((char*)(PHTreeNode1DDesc*)pPHPathJointNodeDesc - (char*)pPHPathJointNodeDesc);
	db->RegisterDesc(desc);
	Spr::PHBallJointNodeDesc* pPHBallJointNodeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHBallJointNodeDesc");
	desc->size = sizeof(Spr::PHBallJointNodeDesc);
	desc->ifInfo = PHBallJointNodeIf::GetIfInfoStatic();
	((IfInfo*)PHBallJointNodeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHBallJointNodeDesc>;
	field = desc->AddBase("PHTreeNodeDesc");
	field->offset = int((char*)(PHTreeNodeDesc*)pPHBallJointNodeDesc - (char*)pPHBallJointNodeDesc);
	db->RegisterDesc(desc);
	Spr::PHGearDesc* pPHGearDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHGearDesc");
	desc->size = sizeof(Spr::PHGearDesc);
	desc->ifInfo = PHGearIf::GetIfInfoStatic();
	((IfInfo*)PHGearIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHGearDesc>;
	field = desc->AddField("", "double", "ratio", "");
	field->offset = int((char*)&(pPHGearDesc->ratio) - (char*)pPHGearDesc);
	db->RegisterDesc(desc);
	Spr::PHHingeJointDesc* pPHHingeJointDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHHingeJointDesc");
	desc->size = sizeof(Spr::PHHingeJointDesc);
	desc->ifInfo = PHHingeJointIf::GetIfInfoStatic();
	((IfInfo*)PHHingeJointIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHHingeJointDesc>;
	field = desc->AddBase("PHJoint1DDesc");
	field->offset = int((char*)(PHJoint1DDesc*)pPHHingeJointDesc - (char*)pPHHingeJointDesc);
	db->RegisterDesc(desc);
	Spr::PHSliderJointDesc* pPHSliderJointDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHSliderJointDesc");
	desc->size = sizeof(Spr::PHSliderJointDesc);
	desc->ifInfo = PHSliderJointIf::GetIfInfoStatic();
	((IfInfo*)PHSliderJointIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHSliderJointDesc>;
	field = desc->AddBase("PHJoint1DDesc");
	field->offset = int((char*)(PHJoint1DDesc*)pPHSliderJointDesc - (char*)pPHSliderJointDesc);
	db->RegisterDesc(desc);
	Spr::PHPathPoint* pPHPathPoint = NULL;
	desc = DBG_NEW UTTypeDesc("PHPathPoint");
	desc->size = sizeof(Spr::PHPathPoint);
	desc->access = DBG_NEW UTAccess<Spr::PHPathPoint>;
	field = desc->AddField("", "double", "s", "");
	field->offset = int((char*)&(pPHPathPoint->s) - (char*)pPHPathPoint);
	field = desc->AddField("", "Posed", "pose", "");
	field->offset = int((char*)&(pPHPathPoint->pose) - (char*)pPHPathPoint);
	db->RegisterDesc(desc);
	Spr::PHPathDesc* pPHPathDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHPathDesc");
	desc->size = sizeof(Spr::PHPathDesc);
	desc->ifInfo = PHPathIf::GetIfInfoStatic();
	((IfInfo*)PHPathIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHPathDesc>;
	field = desc->AddField("vector", "PHPathPoint", "points", "");
	field->offset = int((char*)&(pPHPathDesc->points) - (char*)pPHPathDesc);
	field = desc->AddField("", "bool", "bLoop", "");
	field->offset = int((char*)&(pPHPathDesc->bLoop) - (char*)pPHPathDesc);
	db->RegisterDesc(desc);
	Spr::PHPathJointDesc* pPHPathJointDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHPathJointDesc");
	desc->size = sizeof(Spr::PHPathJointDesc);
	desc->ifInfo = PHPathJointIf::GetIfInfoStatic();
	((IfInfo*)PHPathJointIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHPathJointDesc>;
	field = desc->AddBase("PHJoint1DDesc");
	field->offset = int((char*)(PHJoint1DDesc*)pPHPathJointDesc - (char*)pPHPathJointDesc);
	db->RegisterDesc(desc);
	Spr::PHBallJointDesc* pPHBallJointDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHBallJointDesc");
	desc->size = sizeof(Spr::PHBallJointDesc);
	desc->ifInfo = PHBallJointIf::GetIfInfoStatic();
	((IfInfo*)PHBallJointIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHBallJointDesc>;
	field = desc->AddBase("PHJointDesc");
	field->offset = int((char*)(PHJointDesc*)pPHBallJointDesc - (char*)pPHBallJointDesc);
	field = desc->AddField("", "double", "spring", "");
	field->offset = int((char*)&(pPHBallJointDesc->spring) - (char*)pPHBallJointDesc);
	field = desc->AddField("", "double", "damper", "");
	field->offset = int((char*)&(pPHBallJointDesc->damper) - (char*)pPHBallJointDesc);
	field = desc->AddField("", "Vec2d", "limitSwing", "");
	field->offset = int((char*)&(pPHBallJointDesc->limitSwing) - (char*)pPHBallJointDesc);
	field = desc->AddField("", "Vec2d", "limitTwist", "");
	field->offset = int((char*)&(pPHBallJointDesc->limitTwist) - (char*)pPHBallJointDesc);
	field = desc->AddField("", "Vec3d", "limitDir", "");
	field->offset = int((char*)&(pPHBallJointDesc->limitDir) - (char*)pPHBallJointDesc);
	field = desc->AddField("", "Quaterniond", "goal", "");
	field->offset = int((char*)&(pPHBallJointDesc->goal) - (char*)pPHBallJointDesc);
	field = desc->AddField("", "Vec3d", "torque", "");
	field->offset = int((char*)&(pPHBallJointDesc->torque) - (char*)pPHBallJointDesc);
	field = desc->AddField("", "double", "fMax", "");
	field->offset = int((char*)&(pPHBallJointDesc->fMax) - (char*)pPHBallJointDesc);
	field = desc->AddField("", "double", "fMin", "");
	field->offset = int((char*)&(pPHBallJointDesc->fMin) - (char*)pPHBallJointDesc);
	db->RegisterDesc(desc);
	Spr::PHSpringDesc* pPHSpringDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHSpringDesc");
	desc->size = sizeof(Spr::PHSpringDesc);
	desc->ifInfo = PHSpringIf::GetIfInfoStatic();
	((IfInfo*)PHSpringIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHSpringDesc>;
	field = desc->AddBase("PHConstraintDesc");
	field->offset = int((char*)(PHConstraintDesc*)pPHSpringDesc - (char*)pPHSpringDesc);
	field = desc->AddField("", "Vec3d", "spring", "");
	field->offset = int((char*)&(pPHSpringDesc->spring) - (char*)pPHSpringDesc);
	field = desc->AddField("", "Vec3d", "damper", "");
	field->offset = int((char*)&(pPHSpringDesc->damper) - (char*)pPHSpringDesc);
	field = desc->AddField("", "double", "springOri", "");
	field->offset = int((char*)&(pPHSpringDesc->springOri) - (char*)pPHSpringDesc);
	field = desc->AddField("", "double", "damperOri", "");
	field->offset = int((char*)&(pPHSpringDesc->damperOri) - (char*)pPHSpringDesc);
	db->RegisterDesc(desc);
	Spr::PHInactiveSolids* pPHInactiveSolids = NULL;
	desc = DBG_NEW UTTypeDesc("PHInactiveSolids");
	desc->size = sizeof(Spr::PHInactiveSolids);
	desc->access = DBG_NEW UTAccess<Spr::PHInactiveSolids>;
	field = desc->AddField("vector", "string", "solids", "");
	field->offset = int((char*)&(pPHInactiveSolids->solids) - (char*)pPHInactiveSolids);
	db->RegisterDesc(desc);
	Spr::PHSceneState* pPHSceneState = NULL;
	desc = DBG_NEW UTTypeDesc("PHSceneState");
	desc->size = sizeof(Spr::PHSceneState);
	desc->ifInfo = PHSceneIf::GetIfInfoStatic();
	((IfInfo*)PHSceneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHSceneState>;
	field = desc->AddField("", "double", "timeStep", "");
	field->offset = int((char*)&(pPHSceneState->timeStep) - (char*)pPHSceneState);
	field = desc->AddField("", "unsigned", "count", "");
	field->offset = int((char*)&(pPHSceneState->count) - (char*)pPHSceneState);
	db->RegisterDesc(desc);
	Spr::PHSceneDesc* pPHSceneDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHSceneDesc");
	desc->size = sizeof(Spr::PHSceneDesc);
	desc->ifInfo = PHSceneIf::GetIfInfoStatic();
	((IfInfo*)PHSceneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHSceneDesc>;
	field = desc->AddBase("PHSceneState");
	field->offset = int((char*)(PHSceneState*)pPHSceneDesc - (char*)pPHSceneDesc);
	field = desc->AddField("", "Vec3f", "gravity", "");
	field->offset = int((char*)&(pPHSceneDesc->gravity) - (char*)pPHSceneDesc);
	field = desc->AddField("", "int", "numIteration", "");
	field->offset = int((char*)&(pPHSceneDesc->numIteration) - (char*)pPHSceneDesc);
	db->RegisterDesc(desc);
	Spr::PHSdkDesc* pPHSdkDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHSdkDesc");
	desc->size = sizeof(Spr::PHSdkDesc);
	desc->ifInfo = PHSdkIf::GetIfInfoStatic();
	((IfInfo*)PHSdkIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHSdkDesc>;
	db->RegisterDesc(desc);
	Spr::PHFrameDesc* pPHFrameDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHFrameDesc");
	desc->size = sizeof(Spr::PHFrameDesc);
	desc->ifInfo = PHFrameIf::GetIfInfoStatic();
	((IfInfo*)PHFrameIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHFrameDesc>;
	field = desc->AddField("", "Posed", "pose", "");
	field->offset = int((char*)&(pPHFrameDesc->pose) - (char*)pPHFrameDesc);
	db->RegisterDesc(desc);
	Spr::PHSolidState* pPHSolidState = NULL;
	desc = DBG_NEW UTTypeDesc("PHSolidState");
	desc->size = sizeof(Spr::PHSolidState);
	desc->ifInfo = PHSolidIf::GetIfInfoStatic();
	((IfInfo*)PHSolidIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHSolidState>;
	field = desc->AddField("", "Vec3d", "velocity", "");
	field->offset = int((char*)&(pPHSolidState->velocity) - (char*)pPHSolidState);
	field = desc->AddField("", "Vec3d", "angVelocity", "");
	field->offset = int((char*)&(pPHSolidState->angVelocity) - (char*)pPHSolidState);
	field = desc->AddField("", "Posed", "pose", "");
	field->offset = int((char*)&(pPHSolidState->pose) - (char*)pPHSolidState);
	field = desc->AddField("", "Vec3d", "force", "");
	field->offset = int((char*)&(pPHSolidState->force) - (char*)pPHSolidState);
	field = desc->AddField("", "Vec3d", "torque", "");
	field->offset = int((char*)&(pPHSolidState->torque) - (char*)pPHSolidState);
	field = desc->AddField("", "Vec3d", "nextForce", "");
	field->offset = int((char*)&(pPHSolidState->nextForce) - (char*)pPHSolidState);
	field = desc->AddField("", "Vec3d", "nextTorque", "");
	field->offset = int((char*)&(pPHSolidState->nextTorque) - (char*)pPHSolidState);
	db->RegisterDesc(desc);
	Spr::PHSolidDesc* pPHSolidDesc = NULL;
	desc = DBG_NEW UTTypeDesc("PHSolidDesc");
	desc->size = sizeof(Spr::PHSolidDesc);
	desc->ifInfo = PHSolidIf::GetIfInfoStatic();
	((IfInfo*)PHSolidIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::PHSolidDesc>;
	field = desc->AddBase("PHSolidState");
	field->offset = int((char*)(PHSolidState*)pPHSolidDesc - (char*)pPHSolidDesc);
	field = desc->AddField("", "double", "mass", "");
	field->offset = int((char*)&(pPHSolidDesc->mass) - (char*)pPHSolidDesc);
	field = desc->AddField("", "Matrix3d", "inertia", "");
	field->offset = int((char*)&(pPHSolidDesc->inertia) - (char*)pPHSolidDesc);
	field = desc->AddField("", "Vec3d", "center", "");
	field->offset = int((char*)&(pPHSolidDesc->center) - (char*)pPHSolidDesc);
	field = desc->AddField("", "bool", "dynamical", "");
	field->offset = int((char*)&(pPHSolidDesc->dynamical) - (char*)pPHSolidDesc);
	db->RegisterDesc(desc);
	Spr::OnLimit* pOnLimit = NULL;
	desc = DBG_NEW UTTypeDesc("OnLimit");
	desc->size = sizeof(Spr::OnLimit);
	desc->access = DBG_NEW UTAccess<Spr::OnLimit>;
	field = desc->AddField("", "bool", "onLower", "");
	field->offset = int((char*)&(pOnLimit->onLower) - (char*)pOnLimit);
	field = desc->AddField("", "bool", "onUpper", "");
	field->offset = int((char*)&(pOnLimit->onUpper) - (char*)pOnLimit);
	db->RegisterDesc(desc);
	Spr::PHSolidPairSt* pPHSolidPairSt = NULL;
	desc = DBG_NEW UTTypeDesc("PHSolidPairSt");
	desc->size = sizeof(Spr::PHSolidPairSt);
	desc->access = DBG_NEW UTAccess<Spr::PHSolidPairSt>;
	field = desc->AddField("", "bool", "bEnabled", "");
	field->offset = int((char*)&(pPHSolidPairSt->bEnabled) - (char*)pPHSolidPairSt);
	db->RegisterDesc(desc);
	Spr::PHContactDetectorSt* pPHContactDetectorSt = NULL;
	desc = DBG_NEW UTTypeDesc("PHContactDetectorSt");
	desc->size = sizeof(Spr::PHContactDetectorSt);
	desc->access = DBG_NEW UTAccess<Spr::PHContactDetectorSt>;
	field = desc->AddField("", "size_t", "nSolidPair", "");
	field->offset = int((char*)&(pPHContactDetectorSt->nSolidPair) - (char*)pPHContactDetectorSt);
	field = desc->AddField("", "size_t", "nShapePair", "");
	field->offset = int((char*)&(pPHContactDetectorSt->nShapePair) - (char*)pPHContactDetectorSt);
	db->RegisterDesc(desc);
	Spr::PHPathPointWithJacobian* pPHPathPointWithJacobian = NULL;
	desc = DBG_NEW UTTypeDesc("PHPathPointWithJacobian");
	desc->size = sizeof(Spr::PHPathPointWithJacobian);
	desc->access = DBG_NEW UTAccess<Spr::PHPathPointWithJacobian>;
	field = desc->AddBase("PHPathPoint");
	field->offset = int((char*)(PHPathPoint*)pPHPathPointWithJacobian - (char*)pPHPathPointWithJacobian);
	field = desc->AddField("", "Matrix6d", "J", "");
	field->offset = int((char*)&(pPHPathPointWithJacobian->J) - (char*)pPHPathPointWithJacobian);
	db->RegisterDesc(desc);
	Spr::SpatialTransform* pSpatialTransform = NULL;
	desc = DBG_NEW UTTypeDesc("SpatialTransform");
	desc->size = sizeof(Spr::SpatialTransform);
	desc->access = DBG_NEW UTAccess<Spr::SpatialTransform>;
	field = desc->AddField("", "Vec3d", "r", "");
	field->offset = int((char*)&(pSpatialTransform->r) - (char*)pSpatialTransform);
	field = desc->AddField("", "Quaterniond", "q", "");
	field->offset = int((char*)&(pSpatialTransform->q) - (char*)pSpatialTransform);
	db->RegisterDesc(desc);
	Spr::SpatialTransformTranspose* pSpatialTransformTranspose = NULL;
	desc = DBG_NEW UTTypeDesc("SpatialTransformTranspose");
	desc->size = sizeof(Spr::SpatialTransformTranspose);
	desc->access = DBG_NEW UTAccess<Spr::SpatialTransformTranspose>;
	field = desc->AddField("", "Vec3d", "r", "");
	field->offset = int((char*)&(pSpatialTransformTranspose->r) - (char*)pSpatialTransformTranspose);
	field = desc->AddField("", "Quaterniond", "q", "");
	field->offset = int((char*)&(pSpatialTransformTranspose->q) - (char*)pSpatialTransformTranspose);
	db->RegisterDesc(desc);
	Spr::SpatialVector* pSpatialVector = NULL;
	desc = DBG_NEW UTTypeDesc("SpatialVector");
	desc->size = sizeof(Spr::SpatialVector);
	desc->access = DBG_NEW UTAccess<Spr::SpatialVector>;
	field = desc->AddBase("Vec6d");
	field->offset = int((char*)(Vec6d*)pSpatialVector - (char*)pSpatialVector);
	db->RegisterDesc(desc);
	Spr::SpatialMatrix* pSpatialMatrix = NULL;
	desc = DBG_NEW UTTypeDesc("SpatialMatrix");
	desc->size = sizeof(Spr::SpatialMatrix);
	desc->access = DBG_NEW UTAccess<Spr::SpatialMatrix>;
	field = desc->AddBase("Matrix6d");
	field->offset = int((char*)(Matrix6d*)pSpatialMatrix - (char*)pSpatialMatrix);
	db->RegisterDesc(desc);
}

SPR_IFIMP1(PHEngine, SceneObject);
SPR_IFIMP1(PHConstraintEngine, PHEngine);
PHConstraintsIf* Spr::PHConstraintEngineIf::GetContactPoints(){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->GetContactPoints();
}
void Spr::PHConstraintEngineIf::SetVelCorrectionRate(double value){
	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetVelCorrectionRate(value);
}
void Spr::PHConstraintEngineIf::SetPosCorrectionRate(double value){
	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetPosCorrectionRate(value);
}
void Spr::PHConstraintEngineIf::SetContactCorrectionRate(double value){
	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetContactCorrectionRate(value);
}
SPR_IFIMP1(PHGravityEngine, PHEngine);
SPR_IFIMP1(PHPenaltyEngine, PHEngine);
SPR_IFIMP1(PHConstraint, SceneObject);
void Spr::PHConstraintIf::Enable(bool bEnable){
	((PHConstraint*)(Object*)(ObjectIf*)this)->Enable(bEnable);
}
bool Spr::PHConstraintIf::IsEnabled(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->IsEnabled();
}
void Spr::PHConstraintIf::GetSocketPose(Posed& pose){
	((PHConstraint*)(Object*)(ObjectIf*)this)->GetSocketPose(pose);
}
void Spr::PHConstraintIf::SetSocketPose(const Posed& pose){
	((PHConstraint*)(Object*)(ObjectIf*)this)->SetSocketPose(pose);
}
void Spr::PHConstraintIf::GetPlugPose(Posed& pose){
	((PHConstraint*)(Object*)(ObjectIf*)this)->GetPlugPose(pose);
}
void Spr::PHConstraintIf::SetPlugPose(const Posed& pose){
	((PHConstraint*)(Object*)(ObjectIf*)this)->SetPlugPose(pose);
}
void Spr::PHConstraintIf::GetRelativePose(Posed& p){
	((PHConstraint*)(Object*)(ObjectIf*)this)->GetRelativePose(p);
}
Vec3d Spr::PHConstraintIf::GetRelativePoseR(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetRelativePoseR();
}
Quaterniond Spr::PHConstraintIf::GetRelativePoseQ(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetRelativePoseQ();
}
void Spr::PHConstraintIf::GetRelativeVelocity(Vec3d& v, Vec3d& w){
	((PHConstraint*)(Object*)(ObjectIf*)this)->GetRelativeVelocity(v, w);
}
void Spr::PHConstraintIf::GetConstraintForce(Vec3d& f, Vec3d& t){
	((PHConstraint*)(Object*)(ObjectIf*)this)->GetConstraintForce(f, t);
}
SPR_IFIMP1(PHConstraints, SceneObject);
Spr::PHConstraintIf* Spr::PHConstraintsIf::FindBySolidPair(PHSolidIf* lhs, PHSolidIf* rhs){
	return	((PHConstraints*)(Object*)(ObjectIf*)this)->FindBySolidPair(lhs, rhs);
}
SPR_IFIMP1(PHContactPoint, PHConstraint);
SPR_IFIMP1(PHJoint, PHConstraint);
SPR_IFIMP1(PHJoint1D, PHJoint);
void Spr::PHJoint1DIf::SetRange(double lower, double upper){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->SetRange(lower, upper);
}
void Spr::PHJoint1DIf::GetRange(double& lower, double& upper){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetRange(lower, upper);
}
void Spr::PHJoint1DIf::SetMotorTorque(double t){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->SetMotorTorque(t);
}
double Spr::PHJoint1DIf::GetMotorTorque(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetMotorTorque();
}
void Spr::PHJoint1DIf::SetDesiredVelocity(double v){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->SetDesiredVelocity(v);
}
double Spr::PHJoint1DIf::GetDesiredVelocity(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetDesiredVelocity();
}
void Spr::PHJoint1DIf::SetSpring(double spring){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->SetSpring(spring);
}
double Spr::PHJoint1DIf::GetSpring(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetSpring();
}
void Spr::PHJoint1DIf::SetSpringOrigin(double origin){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->SetSpringOrigin(origin);
}
double Spr::PHJoint1DIf::GetSpringOrigin(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetSpringOrigin();
}
void Spr::PHJoint1DIf::SetDamper(double damper){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->SetDamper(damper);
}
double Spr::PHJoint1DIf::GetDamper(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetDamper();
}
double Spr::PHJoint1DIf::GetPosition(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetPosition();
}
double Spr::PHJoint1DIf::GetVelocity(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetVelocity();
}
void Spr::PHJoint1DIf::SetTorqueMax(double max){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->SetTorqueMax(max);
}
double Spr::PHJoint1DIf::GetTorqueMax(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetTorqueMax();
}
void Spr::PHJoint1DIf::SetTorqueMin(double min){
	((PHJoint1D*)(Object*)(ObjectIf*)this)->SetTorqueMin(min);
}
double Spr::PHJoint1DIf::GetTorqueMin(){
	return	((PHJoint1D*)(Object*)(ObjectIf*)this)->GetTorqueMin();
}
SPR_IFIMP1(PHHingeJoint, PHJoint1D);
SPR_IFIMP1(PHSliderJoint, PHJoint1D);
SPR_IFIMP1(PHPath, SceneObject);
void Spr::PHPathIf::AddPoint(double s, const Posed& pose){
	((PHPath*)(Object*)(ObjectIf*)this)->AddPoint(s, pose);
}
void Spr::PHPathIf::SetLoop(bool bOnOff){
	((PHPath*)(Object*)(ObjectIf*)this)->SetLoop(bOnOff);
}
bool Spr::PHPathIf::IsLoop(){
	return	((PHPath*)(Object*)(ObjectIf*)this)->IsLoop();
}
SPR_IFIMP1(PHPathJoint, PHJoint1D);
void Spr::PHPathJointIf::SetPosition(double q){
	((PHPathJoint*)(Object*)(ObjectIf*)this)->SetPosition(q);
}
SPR_IFIMP1(PHBallJoint, PHConstraint);
void Spr::PHBallJointIf::SetSwingRange(Vec2d range){
	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetSwingRange(range);
}
void Spr::PHBallJointIf::GetSwingRange(Vec2d& range){
	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetSwingRange(range);
}
void Spr::PHBallJointIf::SetTwistRange(Vec2d range){
	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetTwistRange(range);
}
void Spr::PHBallJointIf::GetTwistRange(Vec2d& range){
	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetTwistRange(range);
}
void Spr::PHBallJointIf::SetMotorTorque(const Vec3d& torque){
	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetMotorTorque(torque);
}
Vec3d Spr::PHBallJointIf::GetMotorTorque(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetMotorTorque();
}
Vec3d Spr::PHBallJointIf::GetAngle(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetAngle();
}
Vec3d Spr::PHBallJointIf::GetVelocity(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetVelocity();
}
void Spr::PHBallJointIf::SetTorqueMax(double max){
	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetTorqueMax(max);
}
double Spr::PHBallJointIf::GetTorqueMax(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetTorqueMax();
}
void Spr::PHBallJointIf::SetTorqueMin(double min){
	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetTorqueMin(min);
}
double Spr::PHBallJointIf::GetTorqueMin(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetTorqueMin();
}
SPR_IFIMP1(PHSpring, PHConstraint);
void Spr::PHSpringIf::SetSpring(const Vec3d& spring){
	((PHSpring*)(Object*)(ObjectIf*)this)->SetSpring(spring);
}
Vec3d Spr::PHSpringIf::GetSpring(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetSpring();
}
void Spr::PHSpringIf::SetDamper(const Vec3d& damper){
	((PHSpring*)(Object*)(ObjectIf*)this)->SetDamper(damper);
}
Vec3d Spr::PHSpringIf::GetDamper(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetDamper();
}
void Spr::PHSpringIf::SetSpringOri(const double spring){
	((PHSpring*)(Object*)(ObjectIf*)this)->SetSpringOri(spring);
}
double Spr::PHSpringIf::GetSpringOri(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetSpringOri();
}
void Spr::PHSpringIf::SetDamperOri(const double damper){
	((PHSpring*)(Object*)(ObjectIf*)this)->SetDamperOri(damper);
}
double Spr::PHSpringIf::GetDamperOri(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetDamperOri();
}
SPR_IFIMP1(PHTreeNode, SceneObject);
void Spr::PHTreeNodeIf::Enable(bool bEnable){
	((PHTreeNode*)(Object*)(ObjectIf*)this)->Enable(bEnable);
}
SPR_IFIMP1(PHRootNode, PHTreeNode);
SPR_IFIMP1(PHTreeNode1D, PHTreeNode);
SPR_IFIMP1(PHHingeJointNode, PHTreeNode1D);
SPR_IFIMP1(PHSliderJointNode, PHTreeNode1D);
SPR_IFIMP1(PHPathJointNode, PHTreeNode1D);
SPR_IFIMP1(PHBallJointNode, PHTreeNode);
SPR_IFIMP1(PHGear, SceneObject);
SPR_IFIMP1(PHScene, Scene);
Spr::PHSdkIf* Spr::PHSceneIf::GetSdk(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSdk();
}
PHSolidIf* Spr::PHSceneIf::CreateSolid(const PHSolidDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateSolid(desc);
}
int Spr::PHSceneIf::NSolids()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NSolids();
}
PHSolidIf** Spr::PHSceneIf::GetSolids(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSolids();
}
void Spr::PHSceneIf::SetContactMode(PHSolidIf* lhs, PHSolidIf* rhs, Spr::PHSceneDesc::ContactMode mode){
	((PHScene*)(Object*)(ObjectIf*)this)->SetContactMode(lhs, rhs, mode);
}
void Spr::PHSceneIf::SetContactMode(PHSolidIf** group, size_t length, Spr::PHSceneDesc::ContactMode mode){
	((PHScene*)(Object*)(ObjectIf*)this)->SetContactMode(group, length, mode);
}
void Spr::PHSceneIf::SetContactMode(PHSolidIf* solid, Spr::PHSceneDesc::ContactMode mode){
	((PHScene*)(Object*)(ObjectIf*)this)->SetContactMode(solid, mode);
}
void Spr::PHSceneIf::SetContactMode(Spr::PHSceneDesc::ContactMode mode){
	((PHScene*)(Object*)(ObjectIf*)this)->SetContactMode(mode);
}
int Spr::PHSceneIf::GetNumIteration(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetNumIteration();
}
void Spr::PHSceneIf::SetNumIteration(int n){
	((PHScene*)(Object*)(ObjectIf*)this)->SetNumIteration(n);
}
Spr::PHJointIf* Spr::PHSceneIf::CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const IfInfo* ii, const Spr::PHJointDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateJoint(lhs, rhs, ii, desc);
}
int Spr::PHSceneIf::NJoints()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NJoints();
}
Spr::PHJointIf* Spr::PHSceneIf::GetJoint(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetJoint(i);
}
int Spr::PHSceneIf::NContacts()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NContacts();
}
Spr::PHContactPointIf* Spr::PHSceneIf::GetContact(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetContact(i);
}
Spr::PHRootNodeIf* Spr::PHSceneIf::CreateRootNode(PHSolidIf* root, const Spr::PHRootNodeDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateRootNode(root, desc);
}
int Spr::PHSceneIf::NRootNodes()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NRootNodes();
}
Spr::PHRootNodeIf* Spr::PHSceneIf::GetRootNode(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetRootNode(i);
}
Spr::PHTreeNodeIf* Spr::PHSceneIf::CreateTreeNode(Spr::PHTreeNodeIf* parent, PHSolidIf* child, const Spr::PHTreeNodeDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateTreeNode(parent, child, desc);
}
Spr::PHGearIf* Spr::PHSceneIf::CreateGear(Spr::PHJoint1DIf* lhs, Spr::PHJoint1DIf* rhs, const Spr::PHGearDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateGear(lhs, rhs, desc);
}
int Spr::PHSceneIf::NGears()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NGears();
}
Spr::PHGearIf* Spr::PHSceneIf::GetGear(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetGear(i);
}
Spr::PHPathIf* Spr::PHSceneIf::CreatePath(const Spr::PHPathDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreatePath(desc);
}
int Spr::PHSceneIf::NPaths(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->NPaths();
}
Spr::PHPathIf* Spr::PHSceneIf::GetPath(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetPath(i);
}
double Spr::PHSceneIf::GetTimeStep()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetTimeStep();
}
void Spr::PHSceneIf::SetTimeStep(double dt){
	((PHScene*)(Object*)(ObjectIf*)this)->SetTimeStep(dt);
}
unsigned int Spr::PHSceneIf::GetCount()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetCount();
}
void Spr::PHSceneIf::SetCount(unsigned int count){
	((PHScene*)(Object*)(ObjectIf*)this)->SetCount(count);
}
void Spr::PHSceneIf::Step(){
	((PHScene*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::PHSceneIf::ClearForce(){
	((PHScene*)(Object*)(ObjectIf*)this)->ClearForce();
}
void Spr::PHSceneIf::GenerateForce(){
	((PHScene*)(Object*)(ObjectIf*)this)->GenerateForce();
}
void Spr::PHSceneIf::Integrate(){
	((PHScene*)(Object*)(ObjectIf*)this)->Integrate();
}
void Spr::PHSceneIf::Clear(){
	((PHScene*)(Object*)(ObjectIf*)this)->Clear();
}
void Spr::PHSceneIf::SetGravity(const Vec3d& accel){
	((PHScene*)(Object*)(ObjectIf*)this)->SetGravity(accel);
}
Vec3d Spr::PHSceneIf::GetGravity(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetGravity();
}
int Spr::PHSceneIf::NEngines(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->NEngines();
}
Spr::PHEngineIf* Spr::PHSceneIf::GetEngine(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetEngine(i);
}
Spr::PHConstraintEngineIf* Spr::PHSceneIf::GetConstraintEngine(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetConstraintEngine();
}
Spr::PHGravityEngineIf* Spr::PHSceneIf::GetGravityEngine(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetGravityEngine();
}
Spr::PHPenaltyEngineIf* Spr::PHSceneIf::GetPenaltyEngine(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetPenaltyEngine();
}
SPR_IFIMP1(PHSdk, Sdk);
Spr::PHSceneIf* Spr::PHSdkIf::CreateScene(){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->CreateScene();
}
Spr::PHSceneIf* Spr::PHSdkIf::CreateScene(const Spr::PHSceneDesc& desc){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->CreateScene(desc);
}
int Spr::PHSdkIf::NScene(){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->NScene();
}
Spr::PHSceneIf* Spr::PHSdkIf::GetScene(size_t i){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->GetScene(i);
}
void Spr::PHSdkIf::MergeScene(Spr::PHSceneIf* scene0, Spr::PHSceneIf* scene1){
	((PHSdk*)(Object*)(ObjectIf*)this)->MergeScene(scene0, scene1);
}
Spr::CDShapeIf* Spr::PHSdkIf::CreateShape(const IfInfo* ii, const Spr::CDShapeDesc& desc){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->CreateShape(ii, desc);
}
int Spr::PHSdkIf::NShape(){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->NShape();
}
Spr::CDShapeIf* Spr::PHSdkIf::GetShape(int i){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->GetShape(i);
}
SPR_IFIMP1(PHFrame, SceneObject);
Posed Spr::PHFrameIf::GetPose(){
	return	((PHFrame*)(Object*)(ObjectIf*)this)->GetPose();
}
void Spr::PHFrameIf::SetPose(Posed p){
	((PHFrame*)(Object*)(ObjectIf*)this)->SetPose(p);
}
SPR_IFIMP1(PHSolid, SceneObject);
void Spr::PHSolidIf::AddForce(Vec3d f){
	((PHSolid*)(Object*)(ObjectIf*)this)->AddForce(f);
}
void Spr::PHSolidIf::AddTorque(Vec3d t){
	((PHSolid*)(Object*)(ObjectIf*)this)->AddTorque(t);
}
void Spr::PHSolidIf::AddForce(Vec3d f, Vec3d r){
	((PHSolid*)(Object*)(ObjectIf*)this)->AddForce(f, r);
}
double Spr::PHSolidIf::GetMass(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetMass();
}
double Spr::PHSolidIf::GetMassInv(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetMassInv();
}
void Spr::PHSolidIf::SetMass(double m){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetMass(m);
}
Vec3d Spr::PHSolidIf::GetCenterOfMass(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetCenterOfMass();
}
void Spr::PHSolidIf::SetCenterOfMass(const Vec3d& center){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetCenterOfMass(center);
}
Matrix3d Spr::PHSolidIf::GetInertia(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetInertia();
}
Matrix3d Spr::PHSolidIf::GetInertiaInv(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetInertiaInv();
}
void Spr::PHSolidIf::SetInertia(const Matrix3d& I){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetInertia(I);
}
Vec3d Spr::PHSolidIf::GetFramePosition()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetFramePosition();
}
void Spr::PHSolidIf::SetFramePosition(const Vec3d& p){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetFramePosition(p);
}
Vec3d Spr::PHSolidIf::GetCenterPosition()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetCenterPosition();
}
void Spr::PHSolidIf::SetCenterPosition(const Vec3d& p){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetCenterPosition(p);
}
Vec3d Spr::PHSolidIf::GetDeltaPosition(const Vec3d& p)const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetDeltaPosition(p);
}
Vec3d Spr::PHSolidIf::GetDeltaPosition()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetDeltaPosition();
}
Quaterniond Spr::PHSolidIf::GetOrientation()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetOrientation();
}
void Spr::PHSolidIf::SetOrientation(const Quaterniond& q){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetOrientation(q);
}
Posed Spr::PHSolidIf::GetPose()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetPose();
}
Vec3d Spr::PHSolidIf::GetPoseR()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetPoseR();
}
Quaterniond Spr::PHSolidIf::GetPoseQ()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetPoseQ();
}
void Spr::PHSolidIf::SetPose(const Posed& p){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetPose(p);
}
Vec3d Spr::PHSolidIf::GetVelocity()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetVelocity();
}
void Spr::PHSolidIf::SetVelocity(const Vec3d& v){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetVelocity(v);
}
Vec3d Spr::PHSolidIf::GetAngularVelocity()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetAngularVelocity();
}
void Spr::PHSolidIf::SetAngularVelocity(const Vec3d& av){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetAngularVelocity(av);
}
Vec3d Spr::PHSolidIf::GetForce()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetForce();
}
Vec3d Spr::PHSolidIf::GetTorque()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetTorque();
}
void Spr::PHSolidIf::AddShape(Spr::CDShapeIf* shape){
	((PHSolid*)(Object*)(ObjectIf*)this)->AddShape(shape);
}
int Spr::PHSolidIf::NShape(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->NShape();
}
Spr::CDShapeIf* Spr::PHSolidIf::GetShape(int index){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetShape(index);
}
Posed Spr::PHSolidIf::GetShapePose(int index){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetShapePose(index);
}
void Spr::PHSolidIf::SetShapePose(int index, const Posed& pose){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetShapePose(index, pose);
}
void Spr::PHSolidIf::SetGravity(bool bOn){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetGravity(bOn);
}
void Spr::PHSolidIf::SetDynamical(bool bOn){
	((PHSolid*)(Object*)(ObjectIf*)this)->SetDynamical(bOn);
}
bool Spr::PHSolidIf::IsDynamical(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->IsDynamical();
}
SPR_OBJECTIMP0(PHBallJointNode);
SPR_OBJECTIMP0(PHBallJoint);
SPR_OBJECTIMP_ABST1(PHConstraint, SceneObject);
SPR_OBJECTIMP1(PHConstraints, SceneObject);
SPR_OBJECTIMP0(PHConstraintEngine);
SPR_OBJECTIMP1(PHContactPoint, PHConstraint);
SPR_OBJECTIMP1(PHEngine, SceneObject);
SPR_OBJECTIMP1(PHGravityEngine, PHEngine);
SPR_OBJECTIMP1(PHGear, SceneObject);
SPR_OBJECTIMP1(PHHingeJointNode, PHTreeNode1D);
SPR_OBJECTIMP1(PHHingeJoint, PHJoint1D);
SPR_OBJECTIMP_ABST1(PHJoint, PHConstraint);
SPR_OBJECTIMP_ABST0(PHJoint1D);
SPR_OBJECTIMP_ABST1(PHPath, SceneObject);
SPR_OBJECTIMP1(PHPathJointNode, PHTreeNode1D);
SPR_OBJECTIMP1(PHPathJoint, PHJoint1D);
SPR_OBJECTIMP0(PHPenaltyEngine);
SPR_OBJECTIMP1(PHScene, Scene);
SPR_OBJECTIMP1(PHSdk, Sdk);
SPR_OBJECTIMP1(PHSliderJointNode, PHTreeNode1D);
SPR_OBJECTIMP1(PHSliderJoint, PHJoint1D);
SPR_OBJECTIMP1(PHFrame, SceneObject);
SPR_OBJECTIMP1(PHSolid, SceneObject);
SPR_OBJECTIMP1(PHSolidContainer, PHEngine);
SPR_OBJECTIMP1(PHSolidInitializer, PHEngine);
SPR_OBJECTIMP1(PHSpring, PHJoint);
SPR_OBJECTIMP_ABST1(PHTreeNode, SceneObject);
SPR_OBJECTIMP1(PHRootNode, PHTreeNode);
SPR_OBJECTIMP_ABST0(PHTreeNode1D);
}

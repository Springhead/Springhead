#include "..\..\include\Springhead.h"
#include "..\..\include\Base\Env.h"
#include "..\..\include\Base\BaseDebug.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Foundation\SprUTCriticalSection.h"
#include "..\..\include\Foundation\SprUTOptimizer.h"
#include "..\..\include\Foundation\SprUTQPTimer.h"
#include "..\..\include\Foundation\SprUTTimer.h"
#include "..\..\include\Collision\SprCDDetector.h"
#include "..\..\include\Collision\SprCDShape.h"
#include "..\..\include\Collision\SprCDSpHash.h"
#include "..\..\include\Physics\SprPHDef.h"
#include "..\..\include\Physics\SprPHEngine.h"
#include "..\..\include\Physics\SprPHFemMesh.h"
#include "..\..\include\Physics\SprPHFemMeshNew.h"
#include "..\..\include\Physics\SprPHHaptic.h"
#include "..\..\include\Physics\SprPHIK.h"
#include "..\..\include\Physics\SprPHJoint.h"
#include "..\..\include\Physics\SprPHJointLimit.h"
#include "..\..\include\Physics\SprPHJointMotor.h"
#include "..\..\include\Physics\SprPHLuGreParametersData.h"
#include "..\..\include\Physics\SprPHNodeHandlers.h"
#include "..\..\include\Physics\SprPHOpObj.h"
#include "..\..\include\Physics\SprPHOpSpHashAgent.h"
#include "..\..\include\Physics\SprPHScene.h"
#include "..\..\include\Physics\SprPHSdk.h"
#include "..\..\include\Physics\SprPHSkeleton.h"
#include "..\..\include\Physics\SprPHSolid.h"
#include "..\Foundation\ClapackLibsDef.h"
#include "..\Foundation\Foundation.h"
#include "..\Foundation\Object.h"
#include "..\Foundation\Scene.h"
#include "..\Foundation\UTBaseType.h"
#include "..\Foundation\UTClapack.h"
#include "..\Foundation\UTDllLoader.h"
#include "..\Foundation\UTDllLoaderImpl.h"
#include "..\Foundation\UTLoadContext.h"
#include "..\Foundation\UTLoadHandler.h"
#include "..\Foundation\UTOptimizer.h"
#include "..\Foundation\UTPath.h"
#include "..\Foundation\UTQPTimer.h"
#include "..\Foundation\UTSocket.h"
#include "..\Foundation\UTTimer.h"
#include "..\Foundation\UTTypeDesc.h"
#include "..\Collision\CDBox.h"
#include "..\Collision\CDCapsule.h"
#include "..\Collision\CDCollisionFunctions.h"
#include "..\Collision\CDConvex.h"
#include "..\Collision\CDConvexMesh.h"
#include "..\Collision\CDConvexMeshInterpolate.h"
#include "..\Collision\CDCutRing.h"
#include "..\Collision\CDDetectorImp.h"
#include "..\Collision\CDEllipsoid.h"
#include "..\Collision\CDQuickHull2D.h"
#include "..\Collision\CDQuickHull2DImp.h"
#include "..\Collision\CDQuickHull3D.h"
#include "..\Collision\CDQuickHull3DImp.h"
#include "..\Collision\CDRoundCone.h"
#include "..\Collision\CDShape.h"
#include "..\Collision\CDSpatialHashTable.h"
#include "..\Collision\CDSphere.h"
#include "..\Collision\Collision.h"
#include "..\Physics\PHAxisIndex.h"
#include "..\Physics\PHBallJoint.h"
#include "..\Physics\PHBody.h"
#include "..\Physics\PHConstraint.h"
#include "..\Physics\PHConstraintEngine.h"
#include "..\Physics\PHContactDetector.h"
#include "..\Physics\PHContactPoint.h"
#include "..\Physics\PHContactSurface.h"
#include "..\Physics\PHEngine.h"
#include "..\Physics\PHFemBase.h"
#include "..\Physics\PHFemCollision.h"
#include "..\Physics\PHFemEngine.h"
#include "..\Physics\PHFemMesh.h"
#include "..\Physics\PHFemMeshNew.h"
#include "..\Physics\PHFemMeshThermo.h"
#include "..\Physics\PHFemPorousWOMove.h"
#include "..\Physics\PHFemThermo.h"
#include "..\Physics\PHFemVibration.h"
#include "..\Physics\PHFixJoint.h"
#include "..\Physics\PHForceField.h"
#include "..\Physics\PHGear.h"
#include "..\Physics\PHGenericJoint.h"
#include "..\Physics\PHHapticEngine.h"
#include "..\Physics\PHHapticPointer.h"
#include "..\Physics\PHHapticStepBase.h"
#include "..\Physics\PHHapticStepImpulse.h"
#include "..\Physics\PHHapticStepLocalDynamics.h"
#include "..\Physics\PHHapticStepLocalDynamicsDev.h"
#include "..\Physics\PHHapticStepMulti.h"
#include "..\Physics\PHHapticStepSingle.h"
#include "..\Physics\PHHingeJoint.h"
#include "..\Physics\PHIKActuator.h"
#include "..\Physics\PHIKEndEffector.h"
#include "..\Physics\PHIKEngine.h"
#include "..\Physics\PHJoint.h"
#include "..\Physics\PHJointLimit.h"
#include "..\Physics\PHJointMotor.h"
#include "..\Physics\PHOpAnimation.h"
#include "..\Physics\PHOpDecompositionMethods.h"
#include "..\Physics\PHOpEngine.h"
#include "..\Physics\PHOpGroup.h"
#include "..\Physics\PHOpHapticController.h"
#include "..\Physics\PHOpHapticRenderer.h"
#include "..\Physics\PHOpObj.h"
#include "..\Physics\PHOpParticle.h"
#include "..\Physics\PHOpSpHashColliAgent.h"
#include "..\Physics\PHPathJoint.h"
#include "..\Physics\PHPenaltyEngine.h"
#include "..\Physics\PHScene.h"
#include "..\Physics\PHSdk.h"
#include "..\Physics\PHSkeleton.h"
#include "..\Physics\PHSliderJoint.h"
#include "..\Physics\PHSolid.h"
#include "..\Physics\PHSpring.h"
#include "..\Physics\PHTrackingEngine.h"
#include "..\Physics\PHTreeNode.h"
#include "..\Physics\Physics.h"
namespace Spr{

void RegisterTypeDescPhysics(UTTypeDescDbIf* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDescIf* desc;
	int field;
	Spr::PHConstraintEngineDesc* pPHConstraintEngineDesc = NULL;
	desc = UTTypeDescIf::Create("PHConstraintEngineDesc");
	desc->SetSize(sizeof(Spr::PHConstraintEngineDesc));
	desc->SetIfInfo(PHConstraintEngineIf::GetIfInfoStatic());
	((IfInfo*)PHConstraintEngineIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHConstraintEngineDesc>);
	field = desc->AddField("", "int", "method", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->method) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "int", "numIter", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->numIter) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "int", "numIterCorrection", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->numIterCorrection) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "int", "numIterContactCorrection", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->numIterContactCorrection) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "velCorrectionRate", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->velCorrectionRate) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "posCorrectionRate", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->posCorrectionRate) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "contactCorrectionRate", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->contactCorrectionRate) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "shrinkRate", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->shrinkRate) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "shrinkRateCorrection", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->shrinkRateCorrection) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "freezeThreshold", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->freezeThreshold) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "accelSOR", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->accelSOR) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "dfEps", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->dfEps) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "double", "regularization", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->regularization) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "bool", "bSaveConstraints", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->bSaveConstraints) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "bool", "bUpdateAllState", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->bUpdateAllState) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "bool", "bUseContactSurface", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->bUseContactSurface) - (char*)pPHConstraintEngineDesc));
	field = desc->AddField("", "bool", "bReport", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintEngineDesc->bReport) - (char*)pPHConstraintEngineDesc));
	db->RegisterDesc(desc);
	Spr::PHIKEngineDesc* pPHIKEngineDesc = NULL;
	desc = UTTypeDescIf::Create("PHIKEngineDesc");
	desc->SetSize(sizeof(Spr::PHIKEngineDesc));
	desc->SetIfInfo(PHIKEngineIf::GetIfInfoStatic());
	((IfInfo*)PHIKEngineIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIKEngineDesc>);
	field = desc->AddField("", "size_t", "numIter", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->numIter) - (char*)pPHIKEngineDesc));
	field = desc->AddField("", "double", "maxVel", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->maxVel) - (char*)pPHIKEngineDesc));
	field = desc->AddField("", "double", "maxAngVel", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->maxAngVel) - (char*)pPHIKEngineDesc));
	field = desc->AddField("", "double", "maxActVel", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->maxActVel) - (char*)pPHIKEngineDesc));
	field = desc->AddField("", "double", "regularizeParam", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->regularizeParam) - (char*)pPHIKEngineDesc));
	field = desc->AddField("", "double", "regularizeParam2", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->regularizeParam2) - (char*)pPHIKEngineDesc));
	field = desc->AddField("", "int", "regularizeMode", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->regularizeMode) - (char*)pPHIKEngineDesc));
	field = desc->AddField("", "int", "iterGaussSeidel", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->iterGaussSeidel) - (char*)pPHIKEngineDesc));
	field = desc->AddField("", "int", "solverMode", "");
	desc->SetOffset(field, int((char*)&(pPHIKEngineDesc->solverMode) - (char*)pPHIKEngineDesc));
	db->RegisterDesc(desc);
	Spr::PHFemMeshState* pPHFemMeshState = NULL;
	desc = UTTypeDescIf::Create("PHFemMeshState");
	desc->SetSize(sizeof(Spr::PHFemMeshState));
	desc->SetIfInfo(PHFemMeshIf::GetIfInfoStatic());
	((IfInfo*)PHFemMeshIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemMeshState>);
	db->RegisterDesc(desc);
	Spr::PHFemMeshDesc* pPHFemMeshDesc = NULL;
	desc = UTTypeDescIf::Create("PHFemMeshDesc");
	desc->SetSize(sizeof(Spr::PHFemMeshDesc));
	desc->SetIfInfo(PHFemMeshIf::GetIfInfoStatic());
	((IfInfo*)PHFemMeshIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemMeshDesc>);
	field = desc->AddBase("PHFemMeshState");
	desc->SetOffset(field, int((char*)(PHFemMeshState*)pPHFemMeshDesc - (char*)pPHFemMeshDesc));
	field = desc->AddField("vector", "Vec3d", "vertices", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshDesc->vertices) - (char*)pPHFemMeshDesc));
	field = desc->AddField("vector", "int", "tets", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshDesc->tets) - (char*)pPHFemMeshDesc));
	field = desc->AddField("vector", "int", "faces", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshDesc->faces) - (char*)pPHFemMeshDesc));
	db->RegisterDesc(desc);
	Spr::PHFemMeshThermoDesc* pPHFemMeshThermoDesc = NULL;
	desc = UTTypeDescIf::Create("PHFemMeshThermoDesc");
	desc->SetSize(sizeof(Spr::PHFemMeshThermoDesc));
	desc->SetIfInfo(PHFemMeshThermoIf::GetIfInfoStatic());
	((IfInfo*)PHFemMeshThermoIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemMeshThermoDesc>);
	field = desc->AddBase("PHFemMeshDesc");
	desc->SetOffset(field, int((char*)(PHFemMeshDesc*)pPHFemMeshThermoDesc - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "rho", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->rho) - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "thConduct", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->thConduct) - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "thConduct_x", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->thConduct_x) - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "thConduct_y", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->thConduct_y) - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "thConduct_z", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->thConduct_z) - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "heatTrans", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->heatTrans) - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "specificHeat", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->specificHeat) - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "radiantHeat", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->radiantHeat) - (char*)pPHFemMeshThermoDesc));
	field = desc->AddField("", "double", "initial_temp", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshThermoDesc->initial_temp) - (char*)pPHFemMeshThermoDesc));
	db->RegisterDesc(desc);
	Spr::PHFemMeshNewState* pPHFemMeshNewState = NULL;
	desc = UTTypeDescIf::Create("PHFemMeshNewState");
	desc->SetSize(sizeof(Spr::PHFemMeshNewState));
	desc->SetIfInfo(PHFemMeshNewIf::GetIfInfoStatic());
	((IfInfo*)PHFemMeshNewIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemMeshNewState>);
	db->RegisterDesc(desc);
	Spr::PHFemMeshNewDesc* pPHFemMeshNewDesc = NULL;
	desc = UTTypeDescIf::Create("PHFemMeshNewDesc");
	desc->SetSize(sizeof(Spr::PHFemMeshNewDesc));
	desc->SetIfInfo(PHFemMeshNewIf::GetIfInfoStatic());
	((IfInfo*)PHFemMeshNewIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemMeshNewDesc>);
	field = desc->AddBase("PHFemMeshNewState");
	desc->SetOffset(field, int((char*)(PHFemMeshNewState*)pPHFemMeshNewDesc - (char*)pPHFemMeshNewDesc));
	field = desc->AddField("vector", "Vec3d", "vertices", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshNewDesc->vertices) - (char*)pPHFemMeshNewDesc));
	field = desc->AddField("vector", "int", "tets", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshNewDesc->tets) - (char*)pPHFemMeshNewDesc));
	field = desc->AddField("vector", "int", "faces", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshNewDesc->faces) - (char*)pPHFemMeshNewDesc));
	field = desc->AddField("", "bool", "spheric", "");
	desc->SetOffset(field, int((char*)&(pPHFemMeshNewDesc->spheric) - (char*)pPHFemMeshNewDesc));
	db->RegisterDesc(desc);
	Spr::PHFemBaseDesc* pPHFemBaseDesc = NULL;
	desc = UTTypeDescIf::Create("PHFemBaseDesc");
	desc->SetSize(sizeof(Spr::PHFemBaseDesc));
	desc->SetIfInfo(PHFemBaseIf::GetIfInfoStatic());
	((IfInfo*)PHFemBaseIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemBaseDesc>);
	db->RegisterDesc(desc);
	Spr::PHFemVibrationDesc* pPHFemVibrationDesc = NULL;
	desc = UTTypeDescIf::Create("PHFemVibrationDesc");
	desc->SetSize(sizeof(Spr::PHFemVibrationDesc));
	desc->SetIfInfo(PHFemVibrationIf::GetIfInfoStatic());
	((IfInfo*)PHFemVibrationIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemVibrationDesc>);
	field = desc->AddBase("PHFemBaseDesc");
	desc->SetOffset(field, int((char*)(PHFemBaseDesc*)pPHFemVibrationDesc - (char*)pPHFemVibrationDesc));
	field = desc->AddField("", "double", "young", "");
	desc->SetOffset(field, int((char*)&(pPHFemVibrationDesc->young) - (char*)pPHFemVibrationDesc));
	field = desc->AddField("", "double", "poisson", "");
	desc->SetOffset(field, int((char*)&(pPHFemVibrationDesc->poisson) - (char*)pPHFemVibrationDesc));
	field = desc->AddField("", "double", "density", "");
	desc->SetOffset(field, int((char*)&(pPHFemVibrationDesc->density) - (char*)pPHFemVibrationDesc));
	field = desc->AddField("", "double", "alpha", "");
	desc->SetOffset(field, int((char*)&(pPHFemVibrationDesc->alpha) - (char*)pPHFemVibrationDesc));
	field = desc->AddField("", "double", "beta", "");
	desc->SetOffset(field, int((char*)&(pPHFemVibrationDesc->beta) - (char*)pPHFemVibrationDesc));
	field = desc->AddField("vector", "int", "fixedVertices", "");
	desc->SetOffset(field, int((char*)&(pPHFemVibrationDesc->fixedVertices) - (char*)pPHFemVibrationDesc));
	db->RegisterDesc(desc);
	Spr::PHFemThermoDesc* pPHFemThermoDesc = NULL;
	desc = UTTypeDescIf::Create("PHFemThermoDesc");
	desc->SetSize(sizeof(Spr::PHFemThermoDesc));
	desc->SetIfInfo(PHFemThermoIf::GetIfInfoStatic());
	((IfInfo*)PHFemThermoIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemThermoDesc>);
	field = desc->AddBase("PHFemBaseDesc");
	desc->SetOffset(field, int((char*)(PHFemBaseDesc*)pPHFemThermoDesc - (char*)pPHFemThermoDesc));
	field = desc->AddField("", "double", "rho", "");
	desc->SetOffset(field, int((char*)&(pPHFemThermoDesc->rho) - (char*)pPHFemThermoDesc));
	field = desc->AddField("", "double", "heatTrans", "");
	desc->SetOffset(field, int((char*)&(pPHFemThermoDesc->heatTrans) - (char*)pPHFemThermoDesc));
	field = desc->AddField("", "double", "specificHeat", "");
	desc->SetOffset(field, int((char*)&(pPHFemThermoDesc->specificHeat) - (char*)pPHFemThermoDesc));
	field = desc->AddField("", "double", "radiantHeat", "");
	desc->SetOffset(field, int((char*)&(pPHFemThermoDesc->radiantHeat) - (char*)pPHFemThermoDesc));
	field = desc->AddField("", "double", "initial_temp", "");
	desc->SetOffset(field, int((char*)&(pPHFemThermoDesc->initial_temp) - (char*)pPHFemThermoDesc));
	field = desc->AddField("", "double", "weekPow_full", "");
	desc->SetOffset(field, int((char*)&(pPHFemThermoDesc->weekPow_full) - (char*)pPHFemThermoDesc));
	db->RegisterDesc(desc);
	Spr::PHFemPorousWOMoveDesc* pPHFemPorousWOMoveDesc = NULL;
	desc = UTTypeDescIf::Create("PHFemPorousWOMoveDesc");
	desc->SetSize(sizeof(Spr::PHFemPorousWOMoveDesc));
	desc->SetIfInfo(PHFemPorousWOMoveIf::GetIfInfoStatic());
	((IfInfo*)PHFemPorousWOMoveIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFemPorousWOMoveDesc>);
	field = desc->AddBase("PHFemBaseDesc");
	desc->SetOffset(field, int((char*)(PHFemBaseDesc*)pPHFemPorousWOMoveDesc - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "wDiffAir", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->wDiffAir) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "K", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->K) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "kc", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->kc) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "kp", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->kp) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "alpha", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->alpha) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "gamma", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->gamma) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "rhoWater", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->rhoWater) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "rhoOil", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->rhoOil) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "rhowInit", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->rhowInit) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "rhooInit", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->rhooInit) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "evapoRate", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->evapoRate) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "denatTemp", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->denatTemp) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "boundWaterRatio", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->boundWaterRatio) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "equilWaterCont", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->equilWaterCont) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "limitWaterCont", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->limitWaterCont) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "boundaryThick", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->boundaryThick) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "initMassAll", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->initMassAll) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "initWaterRatio", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->initWaterRatio) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "initOilRatio", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->initOilRatio) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "double", "shrinkageRatio", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->shrinkageRatio) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "Vec3d", "top", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->top) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "Vec3d", "center", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->center) - (char*)pPHFemPorousWOMoveDesc));
	field = desc->AddField("", "Vec3d", "bottom", "");
	desc->SetOffset(field, int((char*)&(pPHFemPorousWOMoveDesc->bottom) - (char*)pPHFemPorousWOMoveDesc));
	db->RegisterDesc(desc);
	Spr::PHHapticPointerDesc* pPHHapticPointerDesc = NULL;
	desc = UTTypeDescIf::Create("PHHapticPointerDesc");
	desc->SetSize(sizeof(Spr::PHHapticPointerDesc));
	desc->SetIfInfo(PHHapticPointerIf::GetIfInfoStatic());
	((IfInfo*)PHHapticPointerIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHHapticPointerDesc>);
	field = desc->AddField("", "HapticRenderMode", "renderMode", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerDesc->renderMode) - (char*)pPHHapticPointerDesc));
	db->RegisterDesc(desc);
	Spr::PHHapticEngineDesc* pPHHapticEngineDesc = NULL;
	desc = UTTypeDescIf::Create("PHHapticEngineDesc");
	desc->SetSize(sizeof(Spr::PHHapticEngineDesc));
	desc->SetIfInfo(PHHapticEngineIf::GetIfInfoStatic());
	((IfInfo*)PHHapticEngineIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHHapticEngineDesc>);
	db->RegisterDesc(desc);
	Spr::PHIKEndEffectorState* pPHIKEndEffectorState = NULL;
	desc = UTTypeDescIf::Create("PHIKEndEffectorState");
	desc->SetSize(sizeof(Spr::PHIKEndEffectorState));
	desc->SetIfInfo(PHIKEndEffectorIf::GetIfInfoStatic());
	((IfInfo*)PHIKEndEffectorIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIKEndEffectorState>);
	field = desc->AddField("", "Posed", "solidTempPose", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorState->solidTempPose) - (char*)pPHIKEndEffectorState));
	db->RegisterDesc(desc);
	Spr::PHIKEndEffectorDesc* pPHIKEndEffectorDesc = NULL;
	desc = UTTypeDescIf::Create("PHIKEndEffectorDesc");
	desc->SetSize(sizeof(Spr::PHIKEndEffectorDesc));
	desc->SetIfInfo(PHIKEndEffectorIf::GetIfInfoStatic());
	((IfInfo*)PHIKEndEffectorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIKEndEffectorDesc>);
	field = desc->AddBase("PHIKEndEffectorState");
	desc->SetOffset(field, int((char*)(PHIKEndEffectorState*)pPHIKEndEffectorDesc - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "bool", "bEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->bEnabled) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "bool", "bPosition", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->bPosition) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "bool", "bOrientation", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->bOrientation) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "int", "oriCtlMode", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->oriCtlMode) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "bool", "bForce", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->bForce) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "bool", "bTorque", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->bTorque) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "double", "positionPriority", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->positionPriority) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "double", "orientationPriority", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->orientationPriority) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "double", "forcePriority", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->forcePriority) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "double", "torquePriority", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->torquePriority) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetPosition", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetPosition) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetVelocity) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetLocalPosition", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetLocalPosition) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetLocalDirection", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetLocalDirection) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Quaterniond", "targetOrientation", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetOrientation) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetDirection", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetDirection) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetLookat", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetLookat) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetAngVel", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetAngVel) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetForce", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetForce) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetForceWorkingPoint", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetForceWorkingPoint) - (char*)pPHIKEndEffectorDesc));
	field = desc->AddField("", "Vec3d", "targetTorque", "");
	desc->SetOffset(field, int((char*)&(pPHIKEndEffectorDesc->targetTorque) - (char*)pPHIKEndEffectorDesc));
	db->RegisterDesc(desc);
	Spr::PHIKActuatorState* pPHIKActuatorState = NULL;
	desc = UTTypeDescIf::Create("PHIKActuatorState");
	desc->SetSize(sizeof(Spr::PHIKActuatorState));
	desc->SetIfInfo(PHIKActuatorIf::GetIfInfoStatic());
	((IfInfo*)PHIKActuatorIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIKActuatorState>);
	field = desc->AddField("", "Posed", "solidTempPose", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorState->solidTempPose) - (char*)pPHIKActuatorState));
	field = desc->AddField("", "Quaterniond", "jointTempOri", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorState->jointTempOri) - (char*)pPHIKActuatorState));
	field = desc->AddField("", "Vec3d", "jointTempOriIntp", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorState->jointTempOriIntp) - (char*)pPHIKActuatorState));
	field = desc->AddField("", "double", "jointTempAngle", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorState->jointTempAngle) - (char*)pPHIKActuatorState));
	field = desc->AddField("", "double", "jointTempAngleIntp", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorState->jointTempAngleIntp) - (char*)pPHIKActuatorState));
	field = desc->AddField("", "Posed", "solidPullbackPose", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorState->solidPullbackPose) - (char*)pPHIKActuatorState));
	field = desc->AddField("", "Posed", "jointTempPose", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorState->jointTempPose) - (char*)pPHIKActuatorState));
	db->RegisterDesc(desc);
	Spr::PHIKActuatorDesc* pPHIKActuatorDesc = NULL;
	desc = UTTypeDescIf::Create("PHIKActuatorDesc");
	desc->SetSize(sizeof(Spr::PHIKActuatorDesc));
	desc->SetIfInfo(PHIKActuatorIf::GetIfInfoStatic());
	((IfInfo*)PHIKActuatorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIKActuatorDesc>);
	field = desc->AddBase("PHIKActuatorState");
	desc->SetOffset(field, int((char*)(PHIKActuatorState*)pPHIKActuatorDesc - (char*)pPHIKActuatorDesc));
	field = desc->AddField("", "bool", "bEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorDesc->bEnabled) - (char*)pPHIKActuatorDesc));
	field = desc->AddField("", "float", "bias", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorDesc->bias) - (char*)pPHIKActuatorDesc));
	field = desc->AddField("", "double", "pullbackRate", "");
	desc->SetOffset(field, int((char*)&(pPHIKActuatorDesc->pullbackRate) - (char*)pPHIKActuatorDesc));
	db->RegisterDesc(desc);
	Spr::PHIKBallActuatorDesc* pPHIKBallActuatorDesc = NULL;
	desc = UTTypeDescIf::Create("PHIKBallActuatorDesc");
	desc->SetSize(sizeof(Spr::PHIKBallActuatorDesc));
	desc->SetIfInfo(PHIKBallActuatorIf::GetIfInfoStatic());
	((IfInfo*)PHIKBallActuatorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIKBallActuatorDesc>);
	field = desc->AddBase("PHIKActuatorDesc");
	desc->SetOffset(field, int((char*)(PHIKActuatorDesc*)pPHIKBallActuatorDesc - (char*)pPHIKBallActuatorDesc));
	field = desc->AddField("", "Quaterniond", "pullbackTarget", "");
	desc->SetOffset(field, int((char*)&(pPHIKBallActuatorDesc->pullbackTarget) - (char*)pPHIKBallActuatorDesc));
	db->RegisterDesc(desc);
	Spr::PHIKHingeActuatorDesc* pPHIKHingeActuatorDesc = NULL;
	desc = UTTypeDescIf::Create("PHIKHingeActuatorDesc");
	desc->SetSize(sizeof(Spr::PHIKHingeActuatorDesc));
	desc->SetIfInfo(PHIKHingeActuatorIf::GetIfInfoStatic());
	((IfInfo*)PHIKHingeActuatorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIKHingeActuatorDesc>);
	field = desc->AddBase("PHIKActuatorDesc");
	desc->SetOffset(field, int((char*)(PHIKActuatorDesc*)pPHIKHingeActuatorDesc - (char*)pPHIKHingeActuatorDesc));
	field = desc->AddField("", "double", "pullbackTarget", "");
	desc->SetOffset(field, int((char*)&(pPHIKHingeActuatorDesc->pullbackTarget) - (char*)pPHIKHingeActuatorDesc));
	db->RegisterDesc(desc);
	Spr::PHIKSpringActuatorDesc* pPHIKSpringActuatorDesc = NULL;
	desc = UTTypeDescIf::Create("PHIKSpringActuatorDesc");
	desc->SetSize(sizeof(Spr::PHIKSpringActuatorDesc));
	desc->SetIfInfo(PHIKSpringActuatorIf::GetIfInfoStatic());
	((IfInfo*)PHIKSpringActuatorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIKSpringActuatorDesc>);
	field = desc->AddBase("PHIKActuatorDesc");
	desc->SetOffset(field, int((char*)(PHIKActuatorDesc*)pPHIKSpringActuatorDesc - (char*)pPHIKSpringActuatorDesc));
	field = desc->AddField("", "Posed", "pullbackTarget", "");
	desc->SetOffset(field, int((char*)&(pPHIKSpringActuatorDesc->pullbackTarget) - (char*)pPHIKSpringActuatorDesc));
	db->RegisterDesc(desc);
	Spr::PHConstraintDesc* pPHConstraintDesc = NULL;
	desc = UTTypeDescIf::Create("PHConstraintDesc");
	desc->SetSize(sizeof(Spr::PHConstraintDesc));
	desc->SetIfInfo(PHConstraintIf::GetIfInfoStatic());
	((IfInfo*)PHConstraintIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHConstraintDesc>);
	field = desc->AddField("", "bool", "bEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintDesc->bEnabled) - (char*)pPHConstraintDesc));
	field = desc->AddField("", "Posed", "poseSocket", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintDesc->poseSocket) - (char*)pPHConstraintDesc));
	field = desc->AddField("", "Posed", "posePlug", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintDesc->posePlug) - (char*)pPHConstraintDesc));
	db->RegisterDesc(desc);
	Spr::PHJointDesc* pPHJointDesc = NULL;
	desc = UTTypeDescIf::Create("PHJointDesc");
	desc->SetSize(sizeof(Spr::PHJointDesc));
	desc->SetIfInfo(PHJointIf::GetIfInfoStatic());
	((IfInfo*)PHJointIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHJointDesc>);
	field = desc->AddBase("PHConstraintDesc");
	desc->SetOffset(field, int((char*)(PHConstraintDesc*)pPHJointDesc - (char*)pPHJointDesc));
	field = desc->AddField("", "double", "fMax", "");
	desc->SetOffset(field, int((char*)&(pPHJointDesc->fMax) - (char*)pPHJointDesc));
	db->RegisterDesc(desc);
	Spr::PH1DJointDesc* pPH1DJointDesc = NULL;
	desc = UTTypeDescIf::Create("PH1DJointDesc");
	desc->SetSize(sizeof(Spr::PH1DJointDesc));
	desc->SetIfInfo(PH1DJointIf::GetIfInfoStatic());
	((IfInfo*)PH1DJointIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PH1DJointDesc>);
	field = desc->AddBase("PHJointDesc");
	desc->SetOffset(field, int((char*)(PHJointDesc*)pPH1DJointDesc - (char*)pPH1DJointDesc));
	field = desc->AddField("", "bool", "cyclic", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->cyclic) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "spring", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->spring) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "damper", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->damper) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "secondDamper", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->secondDamper) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "targetPosition", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->targetPosition) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "targetVelocity", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->targetVelocity) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "offsetForce", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->offsetForce) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "yieldStress", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->yieldStress) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "hardnessRate", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->hardnessRate) - (char*)pPH1DJointDesc));
	field = desc->AddField("", "double", "secondMoment", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointDesc->secondMoment) - (char*)pPH1DJointDesc));
	db->RegisterDesc(desc);
	Spr::PHHingeJointDesc* pPHHingeJointDesc = NULL;
	desc = UTTypeDescIf::Create("PHHingeJointDesc");
	desc->SetSize(sizeof(Spr::PHHingeJointDesc));
	desc->SetIfInfo(PHHingeJointIf::GetIfInfoStatic());
	((IfInfo*)PHHingeJointIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHHingeJointDesc>);
	field = desc->AddBase("PH1DJointDesc");
	desc->SetOffset(field, int((char*)(PH1DJointDesc*)pPHHingeJointDesc - (char*)pPHHingeJointDesc));
	db->RegisterDesc(desc);
	Spr::PHSliderJointDesc* pPHSliderJointDesc = NULL;
	desc = UTTypeDescIf::Create("PHSliderJointDesc");
	desc->SetSize(sizeof(Spr::PHSliderJointDesc));
	desc->SetIfInfo(PHSliderJointIf::GetIfInfoStatic());
	((IfInfo*)PHSliderJointIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSliderJointDesc>);
	field = desc->AddBase("PH1DJointDesc");
	desc->SetOffset(field, int((char*)(PH1DJointDesc*)pPHSliderJointDesc - (char*)pPHSliderJointDesc));
	field = desc->AddField("", "bool", "bConstraintY", "");
	desc->SetOffset(field, int((char*)&(pPHSliderJointDesc->bConstraintY) - (char*)pPHSliderJointDesc));
	field = desc->AddField("", "bool", "bConstraintRollX", "");
	desc->SetOffset(field, int((char*)&(pPHSliderJointDesc->bConstraintRollX) - (char*)pPHSliderJointDesc));
	field = desc->AddField("", "bool", "bConstraintRollZ", "");
	desc->SetOffset(field, int((char*)&(pPHSliderJointDesc->bConstraintRollZ) - (char*)pPHSliderJointDesc));
	db->RegisterDesc(desc);
	Spr::PHPathPoint* pPHPathPoint = NULL;
	desc = UTTypeDescIf::Create("PHPathPoint");
	desc->SetSize(sizeof(Spr::PHPathPoint));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHPathPoint>);
	field = desc->AddField("", "double", "s", "");
	desc->SetOffset(field, int((char*)&(pPHPathPoint->s) - (char*)pPHPathPoint));
	field = desc->AddField("", "Posed", "pose", "");
	desc->SetOffset(field, int((char*)&(pPHPathPoint->pose) - (char*)pPHPathPoint));
	db->RegisterDesc(desc);
	Spr::PHPathDesc* pPHPathDesc = NULL;
	desc = UTTypeDescIf::Create("PHPathDesc");
	desc->SetSize(sizeof(Spr::PHPathDesc));
	desc->SetIfInfo(PHPathIf::GetIfInfoStatic());
	((IfInfo*)PHPathIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHPathDesc>);
	field = desc->AddField("vector", "PHPathPoint", "points", "");
	desc->SetOffset(field, int((char*)&(pPHPathDesc->points) - (char*)pPHPathDesc));
	field = desc->AddField("", "bool", "bLoop", "");
	desc->SetOffset(field, int((char*)&(pPHPathDesc->bLoop) - (char*)pPHPathDesc));
	db->RegisterDesc(desc);
	Spr::PHPathJointDesc* pPHPathJointDesc = NULL;
	desc = UTTypeDescIf::Create("PHPathJointDesc");
	desc->SetSize(sizeof(Spr::PHPathJointDesc));
	desc->SetIfInfo(PHPathJointIf::GetIfInfoStatic());
	((IfInfo*)PHPathJointIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHPathJointDesc>);
	field = desc->AddBase("PH1DJointDesc");
	desc->SetOffset(field, int((char*)(PH1DJointDesc*)pPHPathJointDesc - (char*)pPHPathJointDesc));
	db->RegisterDesc(desc);
	Spr::PHGenericJointDesc* pPHGenericJointDesc = NULL;
	desc = UTTypeDescIf::Create("PHGenericJointDesc");
	desc->SetSize(sizeof(Spr::PHGenericJointDesc));
	desc->SetIfInfo(PHGenericJointIf::GetIfInfoStatic());
	((IfInfo*)PHGenericJointIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHGenericJointDesc>);
	field = desc->AddBase("PH1DJointDesc");
	desc->SetOffset(field, int((char*)(PH1DJointDesc*)pPHGenericJointDesc - (char*)pPHGenericJointDesc));
	db->RegisterDesc(desc);
	Spr::PHBallJointDesc* pPHBallJointDesc = NULL;
	desc = UTTypeDescIf::Create("PHBallJointDesc");
	desc->SetSize(sizeof(Spr::PHBallJointDesc));
	desc->SetIfInfo(PHBallJointIf::GetIfInfoStatic());
	((IfInfo*)PHBallJointIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBallJointDesc>);
	field = desc->AddBase("PHJointDesc");
	desc->SetOffset(field, int((char*)(PHJointDesc*)pPHBallJointDesc - (char*)pPHBallJointDesc));
	field = desc->AddField("", "double", "spring", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->spring) - (char*)pPHBallJointDesc));
	field = desc->AddField("", "double", "damper", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->damper) - (char*)pPHBallJointDesc));
	field = desc->AddField("", "Vec3d", "secondDamper", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->secondDamper) - (char*)pPHBallJointDesc));
	field = desc->AddField("", "Quaterniond", "targetPosition", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->targetPosition) - (char*)pPHBallJointDesc));
	field = desc->AddField("", "Vec3d", "targetVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->targetVelocity) - (char*)pPHBallJointDesc));
	field = desc->AddField("", "Vec3d", "offsetForce", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->offsetForce) - (char*)pPHBallJointDesc));
	field = desc->AddField("", "double", "yieldStress", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->yieldStress) - (char*)pPHBallJointDesc));
	field = desc->AddField("", "double", "hardnessRate", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->hardnessRate) - (char*)pPHBallJointDesc));
	field = desc->AddField("", "Vec3d", "secondMoment", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointDesc->secondMoment) - (char*)pPHBallJointDesc));
	db->RegisterDesc(desc);
	Spr::PHFixJointDesc* pPHFixJointDesc = NULL;
	desc = UTTypeDescIf::Create("PHFixJointDesc");
	desc->SetSize(sizeof(Spr::PHFixJointDesc));
	desc->SetIfInfo(PHFixJointIf::GetIfInfoStatic());
	((IfInfo*)PHFixJointIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFixJointDesc>);
	field = desc->AddBase("PHJointDesc");
	desc->SetOffset(field, int((char*)(PHJointDesc*)pPHFixJointDesc - (char*)pPHFixJointDesc));
	db->RegisterDesc(desc);
	Spr::PHSpringDesc* pPHSpringDesc = NULL;
	desc = UTTypeDescIf::Create("PHSpringDesc");
	desc->SetSize(sizeof(Spr::PHSpringDesc));
	desc->SetIfInfo(PHSpringIf::GetIfInfoStatic());
	((IfInfo*)PHSpringIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSpringDesc>);
	field = desc->AddBase("PHJointDesc");
	desc->SetOffset(field, int((char*)(PHJointDesc*)pPHSpringDesc - (char*)pPHSpringDesc));
	field = desc->AddField("", "Vec3d", "targetPosition", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->targetPosition) - (char*)pPHSpringDesc));
	field = desc->AddField("", "Quaterniond", "targetOrientation", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->targetOrientation) - (char*)pPHSpringDesc));
	field = desc->AddField("", "Vec3d", "spring", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->spring) - (char*)pPHSpringDesc));
	field = desc->AddField("", "Vec3d", "damper", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->damper) - (char*)pPHSpringDesc));
	field = desc->AddField("", "Vec3d", "secondDamper", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->secondDamper) - (char*)pPHSpringDesc));
	field = desc->AddField("", "double", "springOri", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->springOri) - (char*)pPHSpringDesc));
	field = desc->AddField("", "double", "damperOri", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->damperOri) - (char*)pPHSpringDesc));
	field = desc->AddField("", "double", "secondDamperOri", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->secondDamperOri) - (char*)pPHSpringDesc));
	field = desc->AddField("", "double", "yieldStress", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->yieldStress) - (char*)pPHSpringDesc));
	field = desc->AddField("", "double", "hardnessRate", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->hardnessRate) - (char*)pPHSpringDesc));
	field = desc->AddField("", "Vec3d", "secondMoment", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->secondMoment) - (char*)pPHSpringDesc));
	field = desc->AddField("", "Vec6d", "targetVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->targetVelocity) - (char*)pPHSpringDesc));
	field = desc->AddField("", "Vec6d", "offsetForce", "");
	desc->SetOffset(field, int((char*)&(pPHSpringDesc->offsetForce) - (char*)pPHSpringDesc));
	db->RegisterDesc(desc);
	Spr::PHMateDesc* pPHMateDesc = NULL;
	desc = UTTypeDescIf::Create("PHMateDesc");
	desc->SetSize(sizeof(Spr::PHMateDesc));
	desc->SetIfInfo(PHMateIf::GetIfInfoStatic());
	((IfInfo*)PHMateIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHMateDesc>);
	field = desc->AddBase("PHJointDesc");
	desc->SetOffset(field, int((char*)(PHJointDesc*)pPHMateDesc - (char*)pPHMateDesc));
	db->RegisterDesc(desc);
	Spr::PHPointToPointMateDesc* pPHPointToPointMateDesc = NULL;
	desc = UTTypeDescIf::Create("PHPointToPointMateDesc");
	desc->SetSize(sizeof(Spr::PHPointToPointMateDesc));
	desc->SetIfInfo(PHPointToPointMateIf::GetIfInfoStatic());
	((IfInfo*)PHPointToPointMateIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHPointToPointMateDesc>);
	field = desc->AddBase("PHMateDesc");
	desc->SetOffset(field, int((char*)(PHMateDesc*)pPHPointToPointMateDesc - (char*)pPHPointToPointMateDesc));
	db->RegisterDesc(desc);
	Spr::PHPointToLineMateDesc* pPHPointToLineMateDesc = NULL;
	desc = UTTypeDescIf::Create("PHPointToLineMateDesc");
	desc->SetSize(sizeof(Spr::PHPointToLineMateDesc));
	desc->SetIfInfo(PHPointToLineMateIf::GetIfInfoStatic());
	((IfInfo*)PHPointToLineMateIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHPointToLineMateDesc>);
	field = desc->AddBase("PHMateDesc");
	desc->SetOffset(field, int((char*)(PHMateDesc*)pPHPointToLineMateDesc - (char*)pPHPointToLineMateDesc));
	db->RegisterDesc(desc);
	Spr::PHPointToPlaneMateDesc* pPHPointToPlaneMateDesc = NULL;
	desc = UTTypeDescIf::Create("PHPointToPlaneMateDesc");
	desc->SetSize(sizeof(Spr::PHPointToPlaneMateDesc));
	desc->SetIfInfo(PHPointToPlaneMateIf::GetIfInfoStatic());
	((IfInfo*)PHPointToPlaneMateIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHPointToPlaneMateDesc>);
	field = desc->AddBase("PHMateDesc");
	desc->SetOffset(field, int((char*)(PHMateDesc*)pPHPointToPlaneMateDesc - (char*)pPHPointToPlaneMateDesc));
	field = desc->AddField("", "Vec2d", "range", "");
	desc->SetOffset(field, int((char*)&(pPHPointToPlaneMateDesc->range) - (char*)pPHPointToPlaneMateDesc));
	db->RegisterDesc(desc);
	Spr::PHLineToLineMateDesc* pPHLineToLineMateDesc = NULL;
	desc = UTTypeDescIf::Create("PHLineToLineMateDesc");
	desc->SetSize(sizeof(Spr::PHLineToLineMateDesc));
	desc->SetIfInfo(PHLineToLineMateIf::GetIfInfoStatic());
	((IfInfo*)PHLineToLineMateIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHLineToLineMateDesc>);
	field = desc->AddBase("PHMateDesc");
	desc->SetOffset(field, int((char*)(PHMateDesc*)pPHLineToLineMateDesc - (char*)pPHLineToLineMateDesc));
	db->RegisterDesc(desc);
	Spr::PHPlaneToPlaneMateDesc* pPHPlaneToPlaneMateDesc = NULL;
	desc = UTTypeDescIf::Create("PHPlaneToPlaneMateDesc");
	desc->SetSize(sizeof(Spr::PHPlaneToPlaneMateDesc));
	desc->SetIfInfo(PHPlaneToPlaneMateIf::GetIfInfoStatic());
	((IfInfo*)PHPlaneToPlaneMateIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHPlaneToPlaneMateDesc>);
	field = desc->AddBase("PHMateDesc");
	desc->SetOffset(field, int((char*)(PHMateDesc*)pPHPlaneToPlaneMateDesc - (char*)pPHPlaneToPlaneMateDesc));
	db->RegisterDesc(desc);
	Spr::PHTreeNodeDesc* pPHTreeNodeDesc = NULL;
	desc = UTTypeDescIf::Create("PHTreeNodeDesc");
	desc->SetSize(sizeof(Spr::PHTreeNodeDesc));
	desc->SetIfInfo(PHTreeNodeIf::GetIfInfoStatic());
	((IfInfo*)PHTreeNodeIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHTreeNodeDesc>);
	field = desc->AddField("", "bool", "bEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHTreeNodeDesc->bEnabled) - (char*)pPHTreeNodeDesc));
	db->RegisterDesc(desc);
	Spr::PHRootNodeState* pPHRootNodeState = NULL;
	desc = UTTypeDescIf::Create("PHRootNodeState");
	desc->SetSize(sizeof(Spr::PHRootNodeState));
	desc->SetIfInfo(PHRootNodeIf::GetIfInfoStatic());
	((IfInfo*)PHRootNodeIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHRootNodeState>);
	field = desc->AddField("", "Posed", "nextPose", "");
	desc->SetOffset(field, int((char*)&(pPHRootNodeState->nextPose) - (char*)pPHRootNodeState));
	field = desc->AddField("", "bool", "useNextPose", "");
	desc->SetOffset(field, int((char*)&(pPHRootNodeState->useNextPose) - (char*)pPHRootNodeState));
	db->RegisterDesc(desc);
	Spr::PHRootNodeDesc* pPHRootNodeDesc = NULL;
	desc = UTTypeDescIf::Create("PHRootNodeDesc");
	desc->SetSize(sizeof(Spr::PHRootNodeDesc));
	desc->SetIfInfo(PHRootNodeIf::GetIfInfoStatic());
	((IfInfo*)PHRootNodeIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHRootNodeDesc>);
	field = desc->AddBase("PHTreeNodeDesc");
	desc->SetOffset(field, int((char*)(PHTreeNodeDesc*)pPHRootNodeDesc - (char*)pPHRootNodeDesc));
	field = desc->AddBase("PHRootNodeState");
	desc->SetOffset(field, int((char*)(PHRootNodeState*)pPHRootNodeDesc - (char*)pPHRootNodeDesc));
	db->RegisterDesc(desc);
	Spr::PHTreeNode1DDesc* pPHTreeNode1DDesc = NULL;
	desc = UTTypeDescIf::Create("PHTreeNode1DDesc");
	desc->SetSize(sizeof(Spr::PHTreeNode1DDesc));
	desc->SetIfInfo(PHTreeNode1DIf::GetIfInfoStatic());
	((IfInfo*)PHTreeNode1DIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHTreeNode1DDesc>);
	field = desc->AddBase("PHTreeNodeDesc");
	desc->SetOffset(field, int((char*)(PHTreeNodeDesc*)pPHTreeNode1DDesc - (char*)pPHTreeNode1DDesc));
	db->RegisterDesc(desc);
	Spr::PHHingeJointNodeDesc* pPHHingeJointNodeDesc = NULL;
	desc = UTTypeDescIf::Create("PHHingeJointNodeDesc");
	desc->SetSize(sizeof(Spr::PHHingeJointNodeDesc));
	desc->SetIfInfo(PHHingeJointNodeIf::GetIfInfoStatic());
	((IfInfo*)PHHingeJointNodeIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHHingeJointNodeDesc>);
	field = desc->AddBase("PHTreeNode1DDesc");
	desc->SetOffset(field, int((char*)(PHTreeNode1DDesc*)pPHHingeJointNodeDesc - (char*)pPHHingeJointNodeDesc));
	db->RegisterDesc(desc);
	Spr::PHSliderJointNodeDesc* pPHSliderJointNodeDesc = NULL;
	desc = UTTypeDescIf::Create("PHSliderJointNodeDesc");
	desc->SetSize(sizeof(Spr::PHSliderJointNodeDesc));
	desc->SetIfInfo(PHSliderJointNodeIf::GetIfInfoStatic());
	((IfInfo*)PHSliderJointNodeIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSliderJointNodeDesc>);
	field = desc->AddBase("PHTreeNode1DDesc");
	desc->SetOffset(field, int((char*)(PHTreeNode1DDesc*)pPHSliderJointNodeDesc - (char*)pPHSliderJointNodeDesc));
	db->RegisterDesc(desc);
	Spr::PHPathJointNodeDesc* pPHPathJointNodeDesc = NULL;
	desc = UTTypeDescIf::Create("PHPathJointNodeDesc");
	desc->SetSize(sizeof(Spr::PHPathJointNodeDesc));
	desc->SetIfInfo(PHPathJointNodeIf::GetIfInfoStatic());
	((IfInfo*)PHPathJointNodeIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHPathJointNodeDesc>);
	field = desc->AddBase("PHTreeNode1DDesc");
	desc->SetOffset(field, int((char*)(PHTreeNode1DDesc*)pPHPathJointNodeDesc - (char*)pPHPathJointNodeDesc));
	db->RegisterDesc(desc);
	Spr::PHGenericJointNodeDesc* pPHGenericJointNodeDesc = NULL;
	desc = UTTypeDescIf::Create("PHGenericJointNodeDesc");
	desc->SetSize(sizeof(Spr::PHGenericJointNodeDesc));
	desc->SetIfInfo(PHGenericJointNodeIf::GetIfInfoStatic());
	((IfInfo*)PHGenericJointNodeIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHGenericJointNodeDesc>);
	field = desc->AddBase("PHTreeNode1DDesc");
	desc->SetOffset(field, int((char*)(PHTreeNode1DDesc*)pPHGenericJointNodeDesc - (char*)pPHGenericJointNodeDesc));
	db->RegisterDesc(desc);
	Spr::PHBallJointNodeDesc* pPHBallJointNodeDesc = NULL;
	desc = UTTypeDescIf::Create("PHBallJointNodeDesc");
	desc->SetSize(sizeof(Spr::PHBallJointNodeDesc));
	desc->SetIfInfo(PHBallJointNodeIf::GetIfInfoStatic());
	((IfInfo*)PHBallJointNodeIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBallJointNodeDesc>);
	field = desc->AddBase("PHTreeNodeDesc");
	desc->SetOffset(field, int((char*)(PHTreeNodeDesc*)pPHBallJointNodeDesc - (char*)pPHBallJointNodeDesc));
	db->RegisterDesc(desc);
	Spr::PHFixJointNodeDesc* pPHFixJointNodeDesc = NULL;
	desc = UTTypeDescIf::Create("PHFixJointNodeDesc");
	desc->SetSize(sizeof(Spr::PHFixJointNodeDesc));
	desc->SetIfInfo(PHFixJointNodeIf::GetIfInfoStatic());
	((IfInfo*)PHFixJointNodeIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFixJointNodeDesc>);
	field = desc->AddBase("PHTreeNodeDesc");
	desc->SetOffset(field, int((char*)(PHTreeNodeDesc*)pPHFixJointNodeDesc - (char*)pPHFixJointNodeDesc));
	db->RegisterDesc(desc);
	Spr::PHGearDesc* pPHGearDesc = NULL;
	desc = UTTypeDescIf::Create("PHGearDesc");
	desc->SetSize(sizeof(Spr::PHGearDesc));
	desc->SetIfInfo(PHGearIf::GetIfInfoStatic());
	((IfInfo*)PHGearIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHGearDesc>);
	field = desc->AddField("", "bool", "bEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHGearDesc->bEnabled) - (char*)pPHGearDesc));
	field = desc->AddField("", "double", "ratio", "");
	desc->SetOffset(field, int((char*)&(pPHGearDesc->ratio) - (char*)pPHGearDesc));
	field = desc->AddField("", "double", "offset", "");
	desc->SetOffset(field, int((char*)&(pPHGearDesc->offset) - (char*)pPHGearDesc));
	field = desc->AddField("", "int", "mode", "");
	desc->SetOffset(field, int((char*)&(pPHGearDesc->mode) - (char*)pPHGearDesc));
	db->RegisterDesc(desc);
	Spr::PH1DJointLimitDesc* pPH1DJointLimitDesc = NULL;
	desc = UTTypeDescIf::Create("PH1DJointLimitDesc");
	desc->SetSize(sizeof(Spr::PH1DJointLimitDesc));
	desc->SetIfInfo(PH1DJointLimitIf::GetIfInfoStatic());
	((IfInfo*)PH1DJointLimitIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PH1DJointLimitDesc>);
	field = desc->AddField("", "bool", "bEnabled", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointLimitDesc->bEnabled) - (char*)pPH1DJointLimitDesc));
	field = desc->AddField("", "double", "spring", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointLimitDesc->spring) - (char*)pPH1DJointLimitDesc));
	field = desc->AddField("", "double", "damper", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointLimitDesc->damper) - (char*)pPH1DJointLimitDesc));
	field = desc->AddField("", "Vec2d", "range", "");
	desc->SetOffset(field, int((char*)&(pPH1DJointLimitDesc->range) - (char*)pPH1DJointLimitDesc));
	db->RegisterDesc(desc);
	Spr::PHBallJointLimitDesc* pPHBallJointLimitDesc = NULL;
	desc = UTTypeDescIf::Create("PHBallJointLimitDesc");
	desc->SetSize(sizeof(Spr::PHBallJointLimitDesc));
	desc->SetIfInfo(PHBallJointLimitIf::GetIfInfoStatic());
	((IfInfo*)PHBallJointLimitIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBallJointLimitDesc>);
	field = desc->AddField("", "bool", "bEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointLimitDesc->bEnabled) - (char*)pPHBallJointLimitDesc));
	field = desc->AddField("", "double", "spring", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointLimitDesc->spring) - (char*)pPHBallJointLimitDesc));
	field = desc->AddField("", "double", "damper", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointLimitDesc->damper) - (char*)pPHBallJointLimitDesc));
	field = desc->AddField("", "Vec3d", "limitDir", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointLimitDesc->limitDir) - (char*)pPHBallJointLimitDesc));
	db->RegisterDesc(desc);
	Spr::PHBallJointConeLimitDesc* pPHBallJointConeLimitDesc = NULL;
	desc = UTTypeDescIf::Create("PHBallJointConeLimitDesc");
	desc->SetSize(sizeof(Spr::PHBallJointConeLimitDesc));
	desc->SetIfInfo(PHBallJointConeLimitIf::GetIfInfoStatic());
	((IfInfo*)PHBallJointConeLimitIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBallJointConeLimitDesc>);
	field = desc->AddBase("PHBallJointLimitDesc");
	desc->SetOffset(field, int((char*)(PHBallJointLimitDesc*)pPHBallJointConeLimitDesc - (char*)pPHBallJointConeLimitDesc));
	field = desc->AddField("", "Vec2d", "limitSwing", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointConeLimitDesc->limitSwing) - (char*)pPHBallJointConeLimitDesc));
	field = desc->AddField("", "Vec2d", "limitSwingDir", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointConeLimitDesc->limitSwingDir) - (char*)pPHBallJointConeLimitDesc));
	field = desc->AddField("", "Vec2d", "limitTwist", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointConeLimitDesc->limitTwist) - (char*)pPHBallJointConeLimitDesc));
	db->RegisterDesc(desc);
	Spr::PHSplineLimitNode* pPHSplineLimitNode = NULL;
	desc = UTTypeDescIf::Create("PHSplineLimitNode");
	desc->SetSize(sizeof(Spr::PHSplineLimitNode));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSplineLimitNode>);
	field = desc->AddField("", "double", "swing", "");
	desc->SetOffset(field, int((char*)&(pPHSplineLimitNode->swing) - (char*)pPHSplineLimitNode));
	field = desc->AddField("", "double", "swingDir", "");
	desc->SetOffset(field, int((char*)&(pPHSplineLimitNode->swingDir) - (char*)pPHSplineLimitNode));
	field = desc->AddField("", "double", "dSwing", "");
	desc->SetOffset(field, int((char*)&(pPHSplineLimitNode->dSwing) - (char*)pPHSplineLimitNode));
	field = desc->AddField("", "double", "dSwingDir", "");
	desc->SetOffset(field, int((char*)&(pPHSplineLimitNode->dSwingDir) - (char*)pPHSplineLimitNode));
	field = desc->AddField("", "double", "twistMin", "");
	desc->SetOffset(field, int((char*)&(pPHSplineLimitNode->twistMin) - (char*)pPHSplineLimitNode));
	field = desc->AddField("", "double", "twistMax", "");
	desc->SetOffset(field, int((char*)&(pPHSplineLimitNode->twistMax) - (char*)pPHSplineLimitNode));
	db->RegisterDesc(desc);
	Spr::PHBallJointSplineLimitDesc* pPHBallJointSplineLimitDesc = NULL;
	desc = UTTypeDescIf::Create("PHBallJointSplineLimitDesc");
	desc->SetSize(sizeof(Spr::PHBallJointSplineLimitDesc));
	desc->SetIfInfo(PHBallJointSplineLimitIf::GetIfInfoStatic());
	((IfInfo*)PHBallJointSplineLimitIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBallJointSplineLimitDesc>);
	field = desc->AddBase("PHBallJointLimitDesc");
	desc->SetOffset(field, int((char*)(PHBallJointLimitDesc*)pPHBallJointSplineLimitDesc - (char*)pPHBallJointSplineLimitDesc));
	field = desc->AddField("vector", "PHSplineLimitNode", "nodes", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointSplineLimitDesc->nodes) - (char*)pPHBallJointSplineLimitDesc));
	field = desc->AddField("", "Vec2d", "poleTwist", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointSplineLimitDesc->poleTwist) - (char*)pPHBallJointSplineLimitDesc));
	db->RegisterDesc(desc);
	Spr::PHBallJointIndependentLimitDesc* pPHBallJointIndependentLimitDesc = NULL;
	desc = UTTypeDescIf::Create("PHBallJointIndependentLimitDesc");
	desc->SetSize(sizeof(Spr::PHBallJointIndependentLimitDesc));
	desc->SetIfInfo(PHBallJointIndependentLimitIf::GetIfInfoStatic());
	((IfInfo*)PHBallJointIndependentLimitIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBallJointIndependentLimitDesc>);
	field = desc->AddBase("PHBallJointLimitDesc");
	desc->SetOffset(field, int((char*)(PHBallJointLimitDesc*)pPHBallJointIndependentLimitDesc - (char*)pPHBallJointIndependentLimitDesc));
	field = desc->AddField("", "Vec2d", "limitX", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointIndependentLimitDesc->limitX) - (char*)pPHBallJointIndependentLimitDesc));
	field = desc->AddField("", "Vec2d", "limitY", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointIndependentLimitDesc->limitY) - (char*)pPHBallJointIndependentLimitDesc));
	field = desc->AddField("", "Vec2d", "limitZ", "");
	desc->SetOffset(field, int((char*)&(pPHBallJointIndependentLimitDesc->limitZ) - (char*)pPHBallJointIndependentLimitDesc));
	db->RegisterDesc(desc);
	Spr::PH1DJointMotorDesc* pPH1DJointMotorDesc = NULL;
	desc = UTTypeDescIf::Create("PH1DJointMotorDesc");
	desc->SetSize(sizeof(Spr::PH1DJointMotorDesc));
	desc->SetIfInfo(PH1DJointMotorIf::GetIfInfoStatic());
	((IfInfo*)PH1DJointMotorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PH1DJointMotorDesc>);
	db->RegisterDesc(desc);
	Spr::PH1DJointNonLinearMotorDesc* pPH1DJointNonLinearMotorDesc = NULL;
	desc = UTTypeDescIf::Create("PH1DJointNonLinearMotorDesc");
	desc->SetSize(sizeof(Spr::PH1DJointNonLinearMotorDesc));
	desc->SetIfInfo(PH1DJointNonLinearMotorIf::GetIfInfoStatic());
	((IfInfo*)PH1DJointNonLinearMotorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PH1DJointNonLinearMotorDesc>);
	field = desc->AddBase("PH1DJointMotorDesc");
	desc->SetOffset(field, int((char*)(PH1DJointMotorDesc*)pPH1DJointNonLinearMotorDesc - (char*)pPH1DJointNonLinearMotorDesc));
	db->RegisterDesc(desc);
	Spr::PHHuman1DJointResistanceDesc* pPHHuman1DJointResistanceDesc = NULL;
	desc = UTTypeDescIf::Create("PHHuman1DJointResistanceDesc");
	desc->SetSize(sizeof(Spr::PHHuman1DJointResistanceDesc));
	desc->SetIfInfo(PHHuman1DJointResistanceIf::GetIfInfoStatic());
	((IfInfo*)PHHuman1DJointResistanceIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHHuman1DJointResistanceDesc>);
	field = desc->AddBase("PH1DJointNonLinearMotorDesc");
	desc->SetOffset(field, int((char*)(PH1DJointNonLinearMotorDesc*)pPHHuman1DJointResistanceDesc - (char*)pPHHuman1DJointResistanceDesc));
	field = desc->AddField("", "Vec4d", "coefficient", "");
	desc->SetOffset(field, int((char*)&(pPHHuman1DJointResistanceDesc->coefficient) - (char*)pPHHuman1DJointResistanceDesc));
	db->RegisterDesc(desc);
	Spr::PHBallJointMotorDesc* pPHBallJointMotorDesc = NULL;
	desc = UTTypeDescIf::Create("PHBallJointMotorDesc");
	desc->SetSize(sizeof(Spr::PHBallJointMotorDesc));
	desc->SetIfInfo(PHBallJointMotorIf::GetIfInfoStatic());
	((IfInfo*)PHBallJointMotorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBallJointMotorDesc>);
	db->RegisterDesc(desc);
	Spr::PHBallJointNonLinearMotorDesc* pPHBallJointNonLinearMotorDesc = NULL;
	desc = UTTypeDescIf::Create("PHBallJointNonLinearMotorDesc");
	desc->SetSize(sizeof(Spr::PHBallJointNonLinearMotorDesc));
	desc->SetIfInfo(PHBallJointNonLinearMotorIf::GetIfInfoStatic());
	((IfInfo*)PHBallJointNonLinearMotorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBallJointNonLinearMotorDesc>);
	field = desc->AddBase("PHBallJointMotorDesc");
	desc->SetOffset(field, int((char*)(PHBallJointMotorDesc*)pPHBallJointNonLinearMotorDesc - (char*)pPHBallJointNonLinearMotorDesc));
	db->RegisterDesc(desc);
	Spr::PHHumanBallJointResistanceDesc* pPHHumanBallJointResistanceDesc = NULL;
	desc = UTTypeDescIf::Create("PHHumanBallJointResistanceDesc");
	desc->SetSize(sizeof(Spr::PHHumanBallJointResistanceDesc));
	desc->SetIfInfo(PHHumanBallJointResistanceIf::GetIfInfoStatic());
	((IfInfo*)PHHumanBallJointResistanceIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHHumanBallJointResistanceDesc>);
	field = desc->AddBase("PHBallJointNonLinearMotorDesc");
	desc->SetOffset(field, int((char*)(PHBallJointNonLinearMotorDesc*)pPHHumanBallJointResistanceDesc - (char*)pPHHumanBallJointResistanceDesc));
	field = desc->AddField("", "Vec4d", "xCoefficient", "");
	desc->SetOffset(field, int((char*)&(pPHHumanBallJointResistanceDesc->xCoefficient) - (char*)pPHHumanBallJointResistanceDesc));
	field = desc->AddField("", "Vec4d", "yCoefficient", "");
	desc->SetOffset(field, int((char*)&(pPHHumanBallJointResistanceDesc->yCoefficient) - (char*)pPHHumanBallJointResistanceDesc));
	field = desc->AddField("", "Vec4d", "zCoefficient", "");
	desc->SetOffset(field, int((char*)&(pPHHumanBallJointResistanceDesc->zCoefficient) - (char*)pPHHumanBallJointResistanceDesc));
	db->RegisterDesc(desc);
	Spr::PHSpringMotorDesc* pPHSpringMotorDesc = NULL;
	desc = UTTypeDescIf::Create("PHSpringMotorDesc");
	desc->SetSize(sizeof(Spr::PHSpringMotorDesc));
	desc->SetIfInfo(PHSpringMotorIf::GetIfInfoStatic());
	((IfInfo*)PHSpringMotorIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSpringMotorDesc>);
	db->RegisterDesc(desc);
	Spr::PHLuGreParameters* pPHLuGreParameters = NULL;
	desc = UTTypeDescIf::Create("PHLuGreParameters");
	desc->SetSize(sizeof(Spr::PHLuGreParameters));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHLuGreParameters>);
	field = desc->AddField("", "string", "material", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->material) - (char*)pPHLuGreParameters));
	field = desc->AddField("", "double", "A", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->A) - (char*)pPHLuGreParameters));
	field = desc->AddField("", "double", "B", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->B) - (char*)pPHLuGreParameters));
	field = desc->AddField("", "double", "C", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->C) - (char*)pPHLuGreParameters));
	field = desc->AddField("", "double", "bristlesSpringK", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->bristlesSpringK) - (char*)pPHLuGreParameters));
	field = desc->AddField("", "double", "bristlesDamperD", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->bristlesDamperD) - (char*)pPHLuGreParameters));
	field = desc->AddField("", "double", "bristlesViscosityV", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->bristlesViscosityV) - (char*)pPHLuGreParameters));
	field = desc->AddField("", "double", "frictionSpring", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->frictionSpring) - (char*)pPHLuGreParameters));
	field = desc->AddField("", "double", "frictionDamper", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreParameters->frictionDamper) - (char*)pPHLuGreParameters));
	db->RegisterDesc(desc);
	Spr::PHInactiveSolids* pPHInactiveSolids = NULL;
	desc = UTTypeDescIf::Create("PHInactiveSolids");
	desc->SetSize(sizeof(Spr::PHInactiveSolids));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHInactiveSolids>);
	field = desc->AddField("vector", "string", "solids", "");
	desc->SetOffset(field, int((char*)&(pPHInactiveSolids->solids) - (char*)pPHInactiveSolids));
	db->RegisterDesc(desc);
	Spr::PHOpObjState* pPHOpObjState = NULL;
	desc = UTTypeDescIf::Create("PHOpObjState");
	desc->SetSize(sizeof(Spr::PHOpObjState));
	desc->SetIfInfo(PHOpObjIf::GetIfInfoStatic());
	((IfInfo*)PHOpObjIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHOpObjState>);
	field = desc->AddField("", "Vec3d", "velocity", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjState->velocity) - (char*)pPHOpObjState));
	field = desc->AddField("", "Vec3d", "angVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjState->angVelocity) - (char*)pPHOpObjState));
	field = desc->AddField("", "Posed", "pose", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjState->pose) - (char*)pPHOpObjState));
	db->RegisterDesc(desc);
	Spr::PHOpHapticRendererDesc* pPHOpHapticRendererDesc = NULL;
	desc = UTTypeDescIf::Create("PHOpHapticRendererDesc");
	desc->SetSize(sizeof(Spr::PHOpHapticRendererDesc));
	desc->SetIfInfo(PHOpHapticRendererIf::GetIfInfoStatic());
	((IfInfo*)PHOpHapticRendererIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHOpHapticRendererDesc>);
	field = desc->AddField("", "float", "extForceSpring", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->extForceSpring) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "float", "outForceSpring", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->outForceSpring) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "float", "constraintSpring", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->constraintSpring) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "float", "timeStep", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->timeStep) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "float", "forceOnRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->forceOnRadius) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "bool", "useConstrainForce", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->useConstrainForce) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "bool", "useProxyfixSubitr", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->useProxyfixSubitr) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "bool", "useIncreaseMeshMove", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->useIncreaseMeshMove) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "bool", "sqrtAlphaForce", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->sqrtAlphaForce) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "bool", "hitWall", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->hitWall) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "bool", "useDualTranFix", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->useDualTranFix) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "bool", "rigid", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->rigid) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "int", "proxyItrtNum", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->proxyItrtNum) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "int", "proxyItrtMaxNum", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->proxyItrtMaxNum) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "int", "currDtType", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->currDtType) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "int", "objNum", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->objNum) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "int", "noCtcItrNum", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->noCtcItrNum) - (char*)pPHOpHapticRendererDesc));
	field = desc->AddField("", "Vec3f", "proxyFixPos", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticRendererDesc->proxyFixPos) - (char*)pPHOpHapticRendererDesc));
	db->RegisterDesc(desc);
	Spr::PHOpObjDesc* pPHOpObjDesc = NULL;
	desc = UTTypeDescIf::Create("PHOpObjDesc");
	desc->SetSize(sizeof(Spr::PHOpObjDesc));
	desc->SetIfInfo(PHOpObjIf::GetIfInfoStatic());
	((IfInfo*)PHOpObjIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHOpObjDesc>);
	field = desc->AddBase("PHOpObjState");
	desc->SetOffset(field, int((char*)(PHOpObjState*)pPHOpObjDesc - (char*)pPHOpObjDesc));
	field = desc->AddField("", "double", "mass", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->mass) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "Matrix3d", "inertia", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->inertia) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "Vec3d", "center", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->center) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "bool", "dynamical", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->dynamical) - (char*)pPHOpObjDesc));
	field = desc->AddField("pointer", "Vec3f", "objTargetVts", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objTargetVts) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "float", "objTargetVtsArr", "10000");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objTargetVtsArr) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "int", "objTargetVtsNum", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objTargetVtsNum) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "int", "assPsNum", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->assPsNum) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "int", "assGrpNum", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->assGrpNum) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "int", "objGrouplinkCount", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objGrouplinkCount) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "int", "objId", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objId) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "bool", "objUseDistCstr", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objUseDistCstr) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "bool", "objNoMeshObj", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objNoMeshObj) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "bool", "isRigid", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->isRigid) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "int", "objType", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objType) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "float", "objAverRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objAverRadius) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "float", "objMaxRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objMaxRadius) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "int", "objitrTime", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->objitrTime) - (char*)pPHOpObjDesc));
	field = desc->AddField("", "bool", "updateNormals", "");
	desc->SetOffset(field, int((char*)&(pPHOpObjDesc->updateNormals) - (char*)pPHOpObjDesc));
	db->RegisterDesc(desc);
	Spr::PHOpParticleDesc* pPHOpParticleDesc = NULL;
	desc = UTTypeDescIf::Create("PHOpParticleDesc");
	desc->SetSize(sizeof(Spr::PHOpParticleDesc));
	desc->SetIfInfo(PHOpParticleIf::GetIfInfoStatic());
	((IfInfo*)PHOpParticleIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHOpParticleDesc>);
	field = desc->AddField("", "int", "pNvertex", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pNvertex) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "int", "pMyGroupInd", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pMyGroupInd) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "int", "pPId", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pPId) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "int", "pObjId", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pObjId) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "float", "pParaAlpha", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pParaAlpha) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "bool", "hitedByMouse", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->hitedByMouse) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "float", "pSingleVMass", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pSingleVMass) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "float", "pTotalMass", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pTotalMass) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "float", "pRadii", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pRadii) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "float", "pDynamicRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pDynamicRadius) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pMainRadiusVec", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pMainRadiusVec) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pSecRadiusVec", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pSecRadiusVec) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pThrRadiusVec", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pThrRadiusVec) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "float", "pMainRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pMainRadius) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "float", "pSecRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pSecRadius) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "float", "pThrRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pThrRadius) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pCurrCtr", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pCurrCtr) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pOrigCtr", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pOrigCtr) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pNewCtr", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pNewCtr) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pVelocity) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "bool", "isColliedbySphash", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->isColliedbySphash) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "bool", "isColliedbyColliCube", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->isColliedbyColliCube) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "bool", "isFixedbyMouse", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->isFixedbyMouse) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "bool", "isFixed", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->isFixed) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "bool", "isColliedSphashSolved", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->isColliedSphashSolved) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "bool", "isColliedSphashSolvedReady", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->isColliedSphashSolvedReady) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pExternalForce", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pExternalForce) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pExternalTorque", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pExternalTorque) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pExternalDisplacement", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pExternalDisplacement) - (char*)pPHOpParticleDesc));
	field = desc->AddField("vector", "int", "pFaceInd", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pFaceInd) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Matrix3f", "pMomentR", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pMomentR) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Matrix3f", "pSmR", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pSmR) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Matrix3f", "pMomentInertia", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pMomentInertia) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Matrix3f", "pInverseOfMomentInertia", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pInverseOfMomentInertia) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Matrix3f", "ellipRotMatrix", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->ellipRotMatrix) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Quaternionf", "ellipOrigOrint", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->ellipOrigOrint) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Vec3f", "pWvel", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pWvel) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Matrix3f", "pPreEllipA", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pPreEllipA) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Quaternionf", "pCurrOrint", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pCurrOrint) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Quaternionf", "pOrigOrint", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pOrigOrint) - (char*)pPHOpParticleDesc));
	field = desc->AddField("", "Quaternionf", "pNewOrint", "");
	desc->SetOffset(field, int((char*)&(pPHOpParticleDesc->pNewOrint) - (char*)pPHOpParticleDesc));
	db->RegisterDesc(desc);
	Spr::PHOpGroupDesc* pPHOpGroupDesc = NULL;
	desc = UTTypeDescIf::Create("PHOpGroupDesc");
	desc->SetSize(sizeof(Spr::PHOpGroupDesc));
	desc->SetIfInfo(PHOpGroupIf::GetIfInfoStatic());
	((IfInfo*)PHOpGroupIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHOpGroupDesc>);
	field = desc->AddField("", "int", "gMyIndex", "");
	desc->SetOffset(field, int((char*)&(pPHOpGroupDesc->gMyIndex) - (char*)pPHOpGroupDesc));
	field = desc->AddField("", "Matrix3f", "gAgroup", "");
	desc->SetOffset(field, int((char*)&(pPHOpGroupDesc->gAgroup) - (char*)pPHOpGroupDesc));
	field = desc->AddField("", "Matrix3f", "gEigen", "");
	desc->SetOffset(field, int((char*)&(pPHOpGroupDesc->gEigen) - (char*)pPHOpGroupDesc));
	field = desc->AddField("", "float", "gtotalMass", "");
	desc->SetOffset(field, int((char*)&(pPHOpGroupDesc->gtotalMass) - (char*)pPHOpGroupDesc));
	field = desc->AddField("", "Vec3f", "gOrigCenter", "");
	desc->SetOffset(field, int((char*)&(pPHOpGroupDesc->gOrigCenter) - (char*)pPHOpGroupDesc));
	field = desc->AddField("", "Vec3f", "gCurrCenter", "");
	desc->SetOffset(field, int((char*)&(pPHOpGroupDesc->gCurrCenter) - (char*)pPHOpGroupDesc));
	field = desc->AddField("", "int", "gNptcl", "");
	desc->SetOffset(field, int((char*)&(pPHOpGroupDesc->gNptcl) - (char*)pPHOpGroupDesc));
	db->RegisterDesc(desc);
	Spr::PHOpHapticControllerDesc* pPHOpHapticControllerDesc = NULL;
	desc = UTTypeDescIf::Create("PHOpHapticControllerDesc");
	desc->SetSize(sizeof(Spr::PHOpHapticControllerDesc));
	desc->SetIfInfo(PHOpHapticControllerIf::GetIfInfoStatic());
	((IfInfo*)PHOpHapticControllerIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHOpHapticControllerDesc>);
	field = desc->AddField("", "float", "posScale", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->posScale) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "float", "rotScale", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->rotScale) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "float", "forceScale", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->forceScale) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "Vec3f", "userPos", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->userPos) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "Posef", "userPose", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->userPose) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "Vec3f", "hcCurrPPos", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcCurrPPos) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "Vec3f", "hcCurrUPos", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcCurrUPos) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "Vec3f", "hcLastUPos", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcLastUPos) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "Vec3f", "hcFixsubGoal", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcFixsubGoal) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "float", "c_obstRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->c_obstRadius) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "int", "hpObjIndex", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hpObjIndex) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "HapticDOFType", "hcType", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcType) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "bool", "logForce", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->logForce) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "bool", "hcReady", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcReady) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "bool", "IsSetForceReady", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->IsSetForceReady) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "int", "fileindex", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->fileindex) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "Vec3f", "lastHpRateF", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->lastHpRateF) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "bool", "hcCollied", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcCollied) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "int", "constrainCount", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->constrainCount) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "int", "constrainCountHc", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->constrainCountHc) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "Vec3f", "couterPlaneN", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->couterPlaneN) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "float", "hcElmDtcRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcElmDtcRadius) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "int", "collectCount", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->collectCount) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "int", "collectItrtor", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->collectItrtor) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "bool", "hcProxyOn", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->hcProxyOn) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "int", "suspObjid", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->suspObjid) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "float", "proxyRadius", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->proxyRadius) - (char*)pPHOpHapticControllerDesc));
	field = desc->AddField("", "int", "surrCnstrs", "");
	desc->SetOffset(field, int((char*)&(pPHOpHapticControllerDesc->surrCnstrs) - (char*)pPHOpHapticControllerDesc));
	db->RegisterDesc(desc);
	Spr::PHOpAnimationDesc* pPHOpAnimationDesc = NULL;
	desc = UTTypeDescIf::Create("PHOpAnimationDesc");
	desc->SetSize(sizeof(Spr::PHOpAnimationDesc));
	desc->SetIfInfo(PHOpAnimationIf::GetIfInfoStatic());
	((IfInfo*)PHOpAnimationIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHOpAnimationDesc>);
	field = desc->AddField("", "float", "timestep", "");
	desc->SetOffset(field, int((char*)&(pPHOpAnimationDesc->timestep) - (char*)pPHOpAnimationDesc));
	db->RegisterDesc(desc);
	Spr::PHOpSpHashColliAgentDesc* pPHOpSpHashColliAgentDesc = NULL;
	desc = UTTypeDescIf::Create("PHOpSpHashColliAgentDesc");
	desc->SetSize(sizeof(Spr::PHOpSpHashColliAgentDesc));
	desc->SetIfInfo(PHOpSpHashColliAgentIf::GetIfInfoStatic());
	((IfInfo*)PHOpSpHashColliAgentIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHOpSpHashColliAgentDesc>);
	field = desc->AddField("", "bool", "useDirColli", "");
	desc->SetOffset(field, int((char*)&(pPHOpSpHashColliAgentDesc->useDirColli) - (char*)pPHOpSpHashColliAgentDesc));
	field = desc->AddField("", "float", "collisionCstrStiffness", "");
	desc->SetOffset(field, int((char*)&(pPHOpSpHashColliAgentDesc->collisionCstrStiffness) - (char*)pPHOpSpHashColliAgentDesc));
	db->RegisterDesc(desc);
	Spr::PHRaycastHit* pPHRaycastHit = NULL;
	desc = UTTypeDescIf::Create("PHRaycastHit");
	desc->SetSize(sizeof(Spr::PHRaycastHit));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHRaycastHit>);
	field = desc->AddField("pointer", "PHSolidIf", "solid", "");
	desc->SetOffset(field, int((char*)&(pPHRaycastHit->solid) - (char*)pPHRaycastHit));
	field = desc->AddField("pointer", "CDShapeIf", "shape", "");
	desc->SetOffset(field, int((char*)&(pPHRaycastHit->shape) - (char*)pPHRaycastHit));
	field = desc->AddField("", "Vec3f", "point", "");
	desc->SetOffset(field, int((char*)&(pPHRaycastHit->point) - (char*)pPHRaycastHit));
	field = desc->AddField("", "float", "distance", "");
	desc->SetOffset(field, int((char*)&(pPHRaycastHit->distance) - (char*)pPHRaycastHit));
	db->RegisterDesc(desc);
	Spr::PHRayDesc* pPHRayDesc = NULL;
	desc = UTTypeDescIf::Create("PHRayDesc");
	desc->SetSize(sizeof(Spr::PHRayDesc));
	desc->SetIfInfo(PHRayIf::GetIfInfoStatic());
	((IfInfo*)PHRayIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHRayDesc>);
	field = desc->AddField("", "Vec3d", "origin", "");
	desc->SetOffset(field, int((char*)&(pPHRayDesc->origin) - (char*)pPHRayDesc));
	field = desc->AddField("", "Vec3d", "direction", "");
	desc->SetOffset(field, int((char*)&(pPHRayDesc->direction) - (char*)pPHRayDesc));
	db->RegisterDesc(desc);
	Spr::PHSceneState* pPHSceneState = NULL;
	desc = UTTypeDescIf::Create("PHSceneState");
	desc->SetSize(sizeof(Spr::PHSceneState));
	desc->SetIfInfo(PHSceneIf::GetIfInfoStatic());
	((IfInfo*)PHSceneIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSceneState>);
	field = desc->AddField("", "double", "timeStep", "");
	desc->SetOffset(field, int((char*)&(pPHSceneState->timeStep) - (char*)pPHSceneState));
	field = desc->AddField("", "double", "haptictimeStep", "");
	desc->SetOffset(field, int((char*)&(pPHSceneState->haptictimeStep) - (char*)pPHSceneState));
	field = desc->AddField("", "unsigned", "count", "");
	desc->SetOffset(field, int((char*)&(pPHSceneState->count) - (char*)pPHSceneState));
	db->RegisterDesc(desc);
	Spr::PHSceneDesc* pPHSceneDesc = NULL;
	desc = UTTypeDescIf::Create("PHSceneDesc");
	desc->SetSize(sizeof(Spr::PHSceneDesc));
	desc->SetIfInfo(PHSceneIf::GetIfInfoStatic());
	((IfInfo*)PHSceneIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSceneDesc>);
	field = desc->AddBase("PHSceneState");
	desc->SetOffset(field, int((char*)(PHSceneState*)pPHSceneDesc - (char*)pPHSceneDesc));
	field = desc->AddField("", "Vec3d", "gravity", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->gravity) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "airResistanceRateForVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->airResistanceRateForVelocity) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "airResistanceRateForAngularVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->airResistanceRateForAngularVelocity) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "contactTolerance", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->contactTolerance) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "impactThreshold", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->impactThreshold) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "frictionThreshold", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->frictionThreshold) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "maxDeltaPosition", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->maxDeltaPosition) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "maxDeltaOrientation", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->maxDeltaOrientation) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "maxVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->maxVelocity) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "maxAngularVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->maxAngularVelocity) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "maxForce", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->maxForce) - (char*)pPHSceneDesc));
	field = desc->AddField("", "double", "maxMoment", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->maxMoment) - (char*)pPHSceneDesc));
	field = desc->AddField("", "int", "numIteration", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->numIteration) - (char*)pPHSceneDesc));
	field = desc->AddField("", "int", "method", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->method) - (char*)pPHSceneDesc));
	field = desc->AddField("", "bool", "bContactDetectionEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->bContactDetectionEnabled) - (char*)pPHSceneDesc));
	field = desc->AddField("", "bool", "bCCDEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->bCCDEnabled) - (char*)pPHSceneDesc));
	field = desc->AddField("", "int", "broadPhaseMode", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->broadPhaseMode) - (char*)pPHSceneDesc));
	field = desc->AddField("", "int", "blendMode", "");
	desc->SetOffset(field, int((char*)&(pPHSceneDesc->blendMode) - (char*)pPHSceneDesc));
	db->RegisterDesc(desc);
	Spr::PHSdkDesc* pPHSdkDesc = NULL;
	desc = UTTypeDescIf::Create("PHSdkDesc");
	desc->SetSize(sizeof(Spr::PHSdkDesc));
	desc->SetIfInfo(PHSdkIf::GetIfInfoStatic());
	((IfInfo*)PHSdkIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSdkDesc>);
	db->RegisterDesc(desc);
	Spr::PHBoneDesc* pPHBoneDesc = NULL;
	desc = UTTypeDescIf::Create("PHBoneDesc");
	desc->SetSize(sizeof(Spr::PHBoneDesc));
	desc->SetIfInfo(PHBoneIf::GetIfInfoStatic());
	((IfInfo*)PHBoneIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHBoneDesc>);
	db->RegisterDesc(desc);
	Spr::PHSkeletonDesc* pPHSkeletonDesc = NULL;
	desc = UTTypeDescIf::Create("PHSkeletonDesc");
	desc->SetSize(sizeof(Spr::PHSkeletonDesc));
	desc->SetIfInfo(PHSkeletonIf::GetIfInfoStatic());
	((IfInfo*)PHSkeletonIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSkeletonDesc>);
	db->RegisterDesc(desc);
	Spr::PHFrameDesc* pPHFrameDesc = NULL;
	desc = UTTypeDescIf::Create("PHFrameDesc");
	desc->SetSize(sizeof(Spr::PHFrameDesc));
	desc->SetIfInfo(PHFrameIf::GetIfInfoStatic());
	((IfInfo*)PHFrameIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHFrameDesc>);
	field = desc->AddField("", "Posed", "pose", "");
	desc->SetOffset(field, int((char*)&(pPHFrameDesc->pose) - (char*)pPHFrameDesc));
	db->RegisterDesc(desc);
	Spr::PHSolidState* pPHSolidState = NULL;
	desc = UTTypeDescIf::Create("PHSolidState");
	desc->SetSize(sizeof(Spr::PHSolidState));
	desc->SetIfInfo(PHSolidIf::GetIfInfoStatic());
	((IfInfo*)PHSolidIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSolidState>);
	field = desc->AddField("", "Vec3d", "velocity", "");
	desc->SetOffset(field, int((char*)&(pPHSolidState->velocity) - (char*)pPHSolidState));
	field = desc->AddField("", "Vec3d", "angVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHSolidState->angVelocity) - (char*)pPHSolidState));
	field = desc->AddField("", "Posed", "pose", "");
	desc->SetOffset(field, int((char*)&(pPHSolidState->pose) - (char*)pPHSolidState));
	db->RegisterDesc(desc);
	Spr::PHSolidDesc* pPHSolidDesc = NULL;
	desc = UTTypeDescIf::Create("PHSolidDesc");
	desc->SetSize(sizeof(Spr::PHSolidDesc));
	desc->SetIfInfo(PHSolidIf::GetIfInfoStatic());
	((IfInfo*)PHSolidIf::GetIfInfoStatic())->SetDesc(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSolidDesc>);
	field = desc->AddBase("PHSolidState");
	desc->SetOffset(field, int((char*)(PHSolidState*)pPHSolidDesc - (char*)pPHSolidDesc));
	field = desc->AddField("", "double", "mass", "");
	desc->SetOffset(field, int((char*)&(pPHSolidDesc->mass) - (char*)pPHSolidDesc));
	field = desc->AddField("", "Matrix3d", "inertia", "");
	desc->SetOffset(field, int((char*)&(pPHSolidDesc->inertia) - (char*)pPHSolidDesc));
	field = desc->AddField("", "Vec3d", "center", "");
	desc->SetOffset(field, int((char*)&(pPHSolidDesc->center) - (char*)pPHSolidDesc));
	field = desc->AddField("", "bool", "dynamical", "");
	desc->SetOffset(field, int((char*)&(pPHSolidDesc->dynamical) - (char*)pPHSolidDesc));
	field = desc->AddField("", "bool", "stationary", "");
	desc->SetOffset(field, int((char*)&(pPHSolidDesc->stationary) - (char*)pPHSolidDesc));
	db->RegisterDesc(desc);
	Spr::PHConstraintState* pPHConstraintState = NULL;
	desc = UTTypeDescIf::Create("PHConstraintState");
	desc->SetSize(sizeof(Spr::PHConstraintState));
	desc->SetIfInfo(PHConstraintIf::GetIfInfoStatic());
	((IfInfo*)PHConstraintIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHConstraintState>);
	field = desc->AddField("", "Vec6d", "f", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintState->f) - (char*)pPHConstraintState));
	field = desc->AddField("", "Vec6d", "F", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintState->F) - (char*)pPHConstraintState));
	field = desc->AddField("", "Vec6d", "xs", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintState->xs) - (char*)pPHConstraintState));
	field = desc->AddField("", "Vec6d", "fAvg", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintState->fAvg) - (char*)pPHConstraintState));
	field = desc->AddField("", "bool", "bYielded", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintState->bYielded) - (char*)pPHConstraintState));
	db->RegisterDesc(desc);
	Spr::PHLuGreState* pPHLuGreState = NULL;
	desc = UTTypeDescIf::Create("PHLuGreState");
	desc->SetSize(sizeof(Spr::PHLuGreState));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHLuGreState>);
	field = desc->AddField("", "Vec2d", "z", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreState->z) - (char*)pPHLuGreState));
	field = desc->AddField("", "double", "T", "");
	desc->SetOffset(field, int((char*)&(pPHLuGreState->T) - (char*)pPHLuGreState));
	db->RegisterDesc(desc);
	Spr::PHConstraintsSt* pPHConstraintsSt = NULL;
	desc = UTTypeDescIf::Create("PHConstraintsSt");
	desc->SetSize(sizeof(Spr::PHConstraintsSt));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHConstraintsSt>);
	field = desc->AddField("vector", "PHConstraintState", "joints", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintsSt->joints) - (char*)pPHConstraintsSt));
	field = desc->AddField("vector", "PHConstraintState", "gears", "");
	desc->SetOffset(field, int((char*)&(pPHConstraintsSt->gears) - (char*)pPHConstraintsSt));
	db->RegisterDesc(desc);
	Spr::PHSolidPairSt* pPHSolidPairSt = NULL;
	desc = UTTypeDescIf::Create("PHSolidPairSt");
	desc->SetSize(sizeof(Spr::PHSolidPairSt));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSolidPairSt>);
	field = desc->AddField("", "bool", "bEnabled", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairSt->bEnabled) - (char*)pPHSolidPairSt));
	db->RegisterDesc(desc);
	Spr::PHSolidPairVars* pPHSolidPairVars = NULL;
	desc = UTTypeDescIf::Create("PHSolidPairVars");
	desc->SetSize(sizeof(Spr::PHSolidPairVars));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSolidPairVars>);
	field = desc->AddBase("PHSolidPairSt");
	desc->SetOffset(field, int((char*)(PHSolidPairSt*)pPHSolidPairVars - (char*)pPHSolidPairVars));
	field = desc->AddField("pointer", "PHContactDetector", "detector", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairVars->detector) - (char*)pPHSolidPairVars));
	field = desc->AddField("", "PHBody", "body", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairVars->body) - (char*)pPHSolidPairVars));
	db->RegisterDesc(desc);
	Spr::PHContactDetectorSt* pPHContactDetectorSt = NULL;
	desc = UTTypeDescIf::Create("PHContactDetectorSt");
	desc->SetSize(sizeof(Spr::PHContactDetectorSt));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHContactDetectorSt>);
	field = desc->AddField("", "size_t", "nSolidPair", "");
	desc->SetOffset(field, int((char*)&(pPHContactDetectorSt->nSolidPair) - (char*)pPHContactDetectorSt));
	field = desc->AddField("", "size_t", "nShapePair", "");
	desc->SetOffset(field, int((char*)&(pPHContactDetectorSt->nShapePair) - (char*)pPHContactDetectorSt));
	db->RegisterDesc(desc);
	Spr::PHContactPointState* pPHContactPointState = NULL;
	desc = UTTypeDescIf::Create("PHContactPointState");
	desc->SetSize(sizeof(Spr::PHContactPointState));
	desc->SetIfInfo(PHContactPointIf::GetIfInfoStatic());
	((IfInfo*)PHContactPointIf::GetIfInfoStatic())->SetState(desc);
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHContactPointState>);
	field = desc->AddBase("PHConstraintState");
	desc->SetOffset(field, int((char*)(PHConstraintState*)pPHContactPointState - (char*)pPHContactPointState));
	field = desc->AddField("", "int", "shapePair", "");
	desc->SetOffset(field, int((char*)&(pPHContactPointState->shapePair) - (char*)pPHContactPointState));
	field = desc->AddField("", "Vec3d", "pos", "");
	desc->SetOffset(field, int((char*)&(pPHContactPointState->pos) - (char*)pPHContactPointState));
	db->RegisterDesc(desc);
	Spr::FemEdge* pFemEdge = NULL;
	desc = UTTypeDescIf::Create("FemEdge");
	desc->SetSize(sizeof(Spr::FemEdge));
	desc->SetAccess(DBG_NEW UTAccess<Spr::FemEdge>);
	field = desc->AddField("", "int", "vertexIDs", "2");
	desc->SetOffset(field, int((char*)&(pFemEdge->vertexIDs) - (char*)pFemEdge));
	db->RegisterDesc(desc);
	Spr::data* pdata = NULL;
	desc = UTTypeDescIf::Create("data");
	desc->SetSize(sizeof(Spr::data));
	desc->SetAccess(DBG_NEW UTAccess<Spr::data>);
	field = desc->AddField("", "int", "faceId", "");
	desc->SetOffset(field, int((char*)&(pdata->faceId) - (char*)pdata));
	field = desc->AddField("", "Vec3d", "cpoint", "");
	desc->SetOffset(field, int((char*)&(pdata->cpoint) - (char*)pdata));
	field = desc->AddField("", "Vec3d", "median", "");
	desc->SetOffset(field, int((char*)&(pdata->median) - (char*)pdata));
	field = desc->AddField("", "Vec3d", "fnormal", "");
	desc->SetOffset(field, int((char*)&(pdata->fnormal) - (char*)pdata));
	field = desc->AddField("", "int", "vtxIds", "3");
	desc->SetOffset(field, int((char*)&(pdata->vtxIds) - (char*)pdata));
	field = desc->AddField("", "int", "tetraId", "");
	desc->SetOffset(field, int((char*)&(pdata->tetraId) - (char*)pdata));
	db->RegisterDesc(desc);
	Spr::FemFVPair* pFemFVPair = NULL;
	desc = UTTypeDescIf::Create("FemFVPair");
	desc->SetSize(sizeof(Spr::FemFVPair));
	desc->SetAccess(DBG_NEW UTAccess<Spr::FemFVPair>);
	field = desc->AddField("", "int", "vertexId", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->vertexId) - (char*)pFemFVPair));
	field = desc->AddField("", "int", "faceId", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->faceId) - (char*)pFemFVPair));
	field = desc->AddField("", "int", "tetraId", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->tetraId) - (char*)pFemFVPair));
	field = desc->AddField("", "int", "femIndex", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->femIndex) - (char*)pFemFVPair));
	field = desc->AddField("", "Vec3d", "projection", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->projection) - (char*)pFemFVPair));
	field = desc->AddField("", "double", "dist", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->dist) - (char*)pFemFVPair));
	field = desc->AddField("", "Vec3d", "initialPos", "2");
	desc->SetOffset(field, int((char*)&(pFemFVPair->initialPos) - (char*)pFemFVPair));
	field = desc->AddField("", "Vec3d", "debugVertex", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->debugVertex) - (char*)pFemFVPair));
	field = desc->AddField("", "int", "debugMaster", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->debugMaster) - (char*)pFemFVPair));
	field = desc->AddField("", "int", "debugSlave", "");
	desc->SetOffset(field, int((char*)&(pFemFVPair->debugSlave) - (char*)pFemFVPair));
	db->RegisterDesc(desc);
	Spr::PHHapticPointerHapticSt* pPHHapticPointerHapticSt = NULL;
	desc = UTTypeDescIf::Create("PHHapticPointerHapticSt");
	desc->SetSize(sizeof(Spr::PHHapticPointerHapticSt));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHHapticPointerHapticSt>);
	field = desc->AddField("", "Vec3d", "last_dr", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerHapticSt->last_dr) - (char*)pPHHapticPointerHapticSt));
	field = desc->AddField("", "Vec3d", "last_dtheta", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerHapticSt->last_dtheta) - (char*)pPHHapticPointerHapticSt));
	field = desc->AddField("", "Posed", "proxyPose", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerHapticSt->proxyPose) - (char*)pPHHapticPointerHapticSt));
	field = desc->AddField("", "Posed", "lastProxyPose", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerHapticSt->lastProxyPose) - (char*)pPHHapticPointerHapticSt));
	field = desc->AddField("", "SpatialVector", "lastProxyVelocity", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerHapticSt->lastProxyVelocity) - (char*)pPHHapticPointerHapticSt));
	field = desc->AddField("", "SpatialVector", "hapticForce", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerHapticSt->hapticForce) - (char*)pPHHapticPointerHapticSt));
	db->RegisterDesc(desc);
	Spr::PHHapticPointerPhysicsSt* pPHHapticPointerPhysicsSt = NULL;
	desc = UTTypeDescIf::Create("PHHapticPointerPhysicsSt");
	desc->SetSize(sizeof(Spr::PHHapticPointerPhysicsSt));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHHapticPointerPhysicsSt>);
	field = desc->AddField("", "bool", "bRotation", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->bRotation) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "bool", "bForce", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->bForce) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "bool", "bFriction", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->bFriction) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "bool", "bVibration", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->bVibration) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "bool", "bMultiPoints", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->bMultiPoints) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "double", "posScale", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->posScale) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "float", "localRange", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->localRange) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "float", "frictionSpring", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->frictionSpring) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "float", "frictionDamper", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->frictionDamper) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "float", "reflexSpring", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->reflexSpring) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "float", "reflexDamper", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->reflexDamper) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "float", "rotationReflexSpring", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->rotationReflexSpring) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "float", "rotationReflexDamper", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->rotationReflexDamper) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "Posed", "defaultPose", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->defaultPose) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "double", "rotaionalWeight", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->rotaionalWeight) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "PHFrictionType", "frictionType", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->frictionType) - (char*)pPHHapticPointerPhysicsSt));
	field = desc->AddField("", "bTimeVaryFrictionProxy", "bTimeVaryFriction", "");
	desc->SetOffset(field, int((char*)&(pPHHapticPointerPhysicsSt->bTimeVaryFriction) - (char*)pPHHapticPointerPhysicsSt));
	db->RegisterDesc(desc);
	Spr::PHIntermediateRepresentation* pPHIntermediateRepresentation = NULL;
	desc = UTTypeDescIf::Create("PHIntermediateRepresentation");
	desc->SetSize(sizeof(Spr::PHIntermediateRepresentation));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHIntermediateRepresentation>);
	field = desc->AddBase("UTRefCount");
	desc->SetOffset(field, int((char*)(UTRefCount*)pPHIntermediateRepresentation - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "int", "solidID", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->solidID) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("pointer", "PHSolidPairForHaptic", "solidPair", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->solidPair) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("pointer", "PHShapePairForHaptic", "shapePair", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->shapePair) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "double", "depth", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->depth) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "Vec3d", "normal", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->normal) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "Vec3d", "r", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->r) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "Vec3d", "pointerPointW", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->pointerPointW) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "Vec3d", "contactPointW", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->contactPointW) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "Vec3d", "pointerPointVel", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->pointerPointVel) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "Vec3d", "contactPointVel", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->contactPointVel) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "Vec3d", "force", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->force) - (char*)pPHIntermediateRepresentation));
	field = desc->AddField("", "Posed", "interpolation_pose", "");
	desc->SetOffset(field, int((char*)&(pPHIntermediateRepresentation->interpolation_pose) - (char*)pPHIntermediateRepresentation));
	db->RegisterDesc(desc);
	Spr::PHSolidPairForHapticVarsBase* pPHSolidPairForHapticVarsBase = NULL;
	desc = UTTypeDescIf::Create("PHSolidPairForHapticVarsBase");
	desc->SetSize(sizeof(Spr::PHSolidPairForHapticVarsBase));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSolidPairForHapticVarsBase>);
	field = desc->AddField("", "Vec3d", "force", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->force) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "Vec3d", "torque", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->torque) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "Posed", "interpolationPose", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->interpolationPose) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "Posed", "lastInterpolationPose", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->lastInterpolationPose) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "Posed", "initialRelativePose", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->initialRelativePose) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "Posed", "relativePose", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->relativePose) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "unsigned", "contactCount", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->contactCount) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "unsigned", "fricCount", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->fricCount) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "Vec3d", "contactVibrationVel", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->contactVibrationVel) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "Vec3d", "lastStaticFrictionForce", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->lastStaticFrictionForce) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "FrictionState", "frictionState", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->frictionState) - (char*)pPHSolidPairForHapticVarsBase));
	field = desc->AddField("", "int", "solidID", "2");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsBase->solidID) - (char*)pPHSolidPairForHapticVarsBase));
	db->RegisterDesc(desc);
	Spr::PHSolidPairForHapticVarsLocalDynamics* pPHSolidPairForHapticVarsLocalDynamics = NULL;
	desc = UTTypeDescIf::Create("PHSolidPairForHapticVarsLocalDynamics");
	desc->SetSize(sizeof(Spr::PHSolidPairForHapticVarsLocalDynamics));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSolidPairForHapticVarsLocalDynamics>);
	field = desc->AddField("", "int", "inLocal", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsLocalDynamics->inLocal) - (char*)pPHSolidPairForHapticVarsLocalDynamics));
	field = desc->AddField("", "TMatrixRow<(6,3,double)>", "A", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsLocalDynamics->A) - (char*)pPHSolidPairForHapticVarsLocalDynamics));
	field = desc->AddField("", "SpatialMatrix", "A6D", "");
	desc->SetOffset(field, int((char*)&(pPHSolidPairForHapticVarsLocalDynamics->A6D) - (char*)pPHSolidPairForHapticVarsLocalDynamics));
	db->RegisterDesc(desc);
	Spr::PHSolidPairForHapticVars* pPHSolidPairForHapticVars = NULL;
	desc = UTTypeDescIf::Create("PHSolidPairForHapticVars");
	desc->SetSize(sizeof(Spr::PHSolidPairForHapticVars));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSolidPairForHapticVars>);
	field = desc->AddBase("PHSolidPairForHapticVarsBase");
	desc->SetOffset(field, int((char*)(PHSolidPairForHapticVarsBase*)pPHSolidPairForHapticVars - (char*)pPHSolidPairForHapticVars));
	field = desc->AddBase("PHSolidPairForHapticVarsLocalDynamics");
	desc->SetOffset(field, int((char*)(PHSolidPairForHapticVarsLocalDynamics*)pPHSolidPairForHapticVars - (char*)pPHSolidPairForHapticVars));
	db->RegisterDesc(desc);
	Spr::SplinePoint* pSplinePoint = NULL;
	desc = UTTypeDescIf::Create("SplinePoint");
	desc->SetSize(sizeof(Spr::SplinePoint));
	desc->SetAccess(DBG_NEW UTAccess<Spr::SplinePoint>);
	field = desc->AddField("pointer", "ClosedSplineCurve", "spline", "");
	desc->SetOffset(field, int((char*)&(pSplinePoint->spline) - (char*)pSplinePoint));
	field = desc->AddField("", "Vec2d", "pos", "");
	desc->SetOffset(field, int((char*)&(pSplinePoint->pos) - (char*)pSplinePoint));
	field = desc->AddField("", "int", "edge", "");
	desc->SetOffset(field, int((char*)&(pSplinePoint->edge) - (char*)pSplinePoint));
	field = desc->AddField("", "double", "t", "");
	desc->SetOffset(field, int((char*)&(pSplinePoint->t) - (char*)pSplinePoint));
	db->RegisterDesc(desc);
	Spr::JointFunctions* pJointFunctions = NULL;
	desc = UTTypeDescIf::Create("JointFunctions");
	desc->SetSize(sizeof(Spr::JointFunctions));
	desc->SetAccess(DBG_NEW UTAccess<Spr::JointFunctions>);
	db->RegisterDesc(desc);
	Spr::DisCmpPoint* pDisCmpPoint = NULL;
	desc = UTTypeDescIf::Create("DisCmpPoint");
	desc->SetSize(sizeof(Spr::DisCmpPoint));
	desc->SetAccess(DBG_NEW UTAccess<Spr::DisCmpPoint>);
	field = desc->AddField("", "int", "pIndex", "");
	desc->SetOffset(field, int((char*)&(pDisCmpPoint->pIndex) - (char*)pDisCmpPoint));
	field = desc->AddField("", "float", "distance", "");
	desc->SetOffset(field, int((char*)&(pDisCmpPoint->distance) - (char*)pDisCmpPoint));
	db->RegisterDesc(desc);
	Spr::solvedColliPair* psolvedColliPair = NULL;
	desc = UTTypeDescIf::Create("solvedColliPair");
	desc->SetSize(sizeof(Spr::solvedColliPair));
	desc->SetAccess(DBG_NEW UTAccess<Spr::solvedColliPair>);
	field = desc->AddField("", "int", "cOInd1", "");
	desc->SetOffset(field, int((char*)&(psolvedColliPair->cOInd1) - (char*)psolvedColliPair));
	field = desc->AddField("", "int", "cOInd2", "");
	desc->SetOffset(field, int((char*)&(psolvedColliPair->cOInd2) - (char*)psolvedColliPair));
	field = desc->AddField("", "int", "cPInd1", "");
	desc->SetOffset(field, int((char*)&(psolvedColliPair->cPInd1) - (char*)psolvedColliPair));
	field = desc->AddField("", "int", "cPInd2", "");
	desc->SetOffset(field, int((char*)&(psolvedColliPair->cPInd2) - (char*)psolvedColliPair));
	db->RegisterDesc(desc);
	Spr::PHSolidStatePrivate* pPHSolidStatePrivate = NULL;
	desc = UTTypeDescIf::Create("PHSolidStatePrivate");
	desc->SetSize(sizeof(Spr::PHSolidStatePrivate));
	desc->SetAccess(DBG_NEW UTAccess<Spr::PHSolidStatePrivate>);
	field = desc->AddField("", "bool", "bFrozen", "");
	desc->SetOffset(field, int((char*)&(pPHSolidStatePrivate->bFrozen) - (char*)pPHSolidStatePrivate));
	field = desc->AddField("", "bool", "bUpdated", "");
	desc->SetOffset(field, int((char*)&(pPHSolidStatePrivate->bUpdated) - (char*)pPHSolidStatePrivate));
	field = desc->AddField("", "Vec3d", "nextForce", "");
	desc->SetOffset(field, int((char*)&(pPHSolidStatePrivate->nextForce) - (char*)pPHSolidStatePrivate));
	field = desc->AddField("", "Vec3d", "nextTorque", "");
	desc->SetOffset(field, int((char*)&(pPHSolidStatePrivate->nextTorque) - (char*)pPHSolidStatePrivate));
	field = desc->AddField("", "Vec3d", "force", "");
	desc->SetOffset(field, int((char*)&(pPHSolidStatePrivate->force) - (char*)pPHSolidStatePrivate));
	field = desc->AddField("", "Vec3d", "torque", "");
	desc->SetOffset(field, int((char*)&(pPHSolidStatePrivate->torque) - (char*)pPHSolidStatePrivate));
	field = desc->AddField("", "Vec3d", "accel", "");
	desc->SetOffset(field, int((char*)&(pPHSolidStatePrivate->accel) - (char*)pPHSolidStatePrivate));
	field = desc->AddField("", "Vec3d", "angAccel", "");
	desc->SetOffset(field, int((char*)&(pPHSolidStatePrivate->angAccel) - (char*)pPHSolidStatePrivate));
	db->RegisterDesc(desc);
}

SPR_IFIMP1(PHEngine, SceneObject);
int Spr::PHEngineIf::GetPriority()const{
	return	((PHEngine*)(Object*)(ObjectIf*)this)->GetPriority();
}
void Spr::PHEngineIf::Step(){
	((PHEngine*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::PHEngineIf::Enable(bool on){
	return	((PHEngine*)(Object*)(ObjectIf*)this)->Enable(on);
}
bool Spr::PHEngineIf::IsEnabled(){
	return	((PHEngine*)(Object*)(ObjectIf*)this)->IsEnabled();
}
Spr::PHSceneIf* Spr::PHEngineIf::GetScene(){
	return	((PHEngine*)(Object*)(ObjectIf*)this)->GetScene();
}
SPR_IFIMP1(PHConstraintEngine, PHEngine);
Spr::PHConstraintsIf* Spr::PHConstraintEngineIf::GetContactPoints(){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->GetContactPoints();
}
void Spr::PHConstraintEngineIf::SetVelCorrectionRate(double value){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetVelCorrectionRate(value);
}
double Spr::PHConstraintEngineIf::GetVelCorrectionRate(){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->GetVelCorrectionRate();
}
void Spr::PHConstraintEngineIf::SetPosCorrectionRate(double value){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetPosCorrectionRate(value);
}
double Spr::PHConstraintEngineIf::GetPosCorrectionRate(){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->GetPosCorrectionRate();
}
void Spr::PHConstraintEngineIf::SetContactCorrectionRate(double value){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetContactCorrectionRate(value);
}
double Spr::PHConstraintEngineIf::GetContactCorrectionRate(){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->GetContactCorrectionRate();
}
void Spr::PHConstraintEngineIf::SetBSaveConstraints(bool value){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetBSaveConstraints(value);
}
void Spr::PHConstraintEngineIf::SetUpdateAllSolidState(bool flag){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetUpdateAllSolidState(flag);
}
void Spr::PHConstraintEngineIf::SetUseContactSurface(bool flag){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetUseContactSurface(flag);
}
void Spr::PHConstraintEngineIf::SetShrinkRate(double data){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetShrinkRate(data);
}
double Spr::PHConstraintEngineIf::GetShrinkRate(){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->GetShrinkRate();
}
void Spr::PHConstraintEngineIf::SetShrinkRateCorrection(double data){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetShrinkRateCorrection(data);
}
double Spr::PHConstraintEngineIf::GetShrinkRateCorrection(){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->GetShrinkRateCorrection();
}
void Spr::PHConstraintEngineIf::SetRegularization(double reg){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->SetRegularization(reg);
}
double Spr::PHConstraintEngineIf::GetRegularization(){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->GetRegularization();
}
void Spr::PHConstraintEngineIf::EnableRenderContact(bool enable){
	return	((PHConstraintEngine*)(Object*)(ObjectIf*)this)->EnableRenderContact(enable);
}
SPR_IFIMP1(PHGravityEngine, PHEngine);
SPR_IFIMP1(PHPenaltyEngine, PHEngine);
SPR_IFIMP1(PHIKEngine, PHEngine);
void Spr::PHIKEngineIf::SetMaxVelocity(double maxVel){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->SetMaxVelocity(maxVel);
}
double Spr::PHIKEngineIf::GetMaxVelocity(){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->GetMaxVelocity();
}
void Spr::PHIKEngineIf::SetMaxAngularVelocity(double maxAV){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->SetMaxAngularVelocity(maxAV);
}
double Spr::PHIKEngineIf::GetMaxAngularVelocity(){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->GetMaxAngularVelocity();
}
void Spr::PHIKEngineIf::SetMaxActuatorVelocity(double maxAV){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->SetMaxActuatorVelocity(maxAV);
}
double Spr::PHIKEngineIf::GetMaxActuatorVelocity(){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->GetMaxActuatorVelocity();
}
void Spr::PHIKEngineIf::SetRegularizeParam(double epsilon){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->SetRegularizeParam(epsilon);
}
double Spr::PHIKEngineIf::GetRegularizeParam(){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->GetRegularizeParam();
}
void Spr::PHIKEngineIf::SetIterCutOffAngVel(double epsilon){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->SetIterCutOffAngVel(epsilon);
}
double Spr::PHIKEngineIf::GetIterCutOffAngVel(){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->GetIterCutOffAngVel();
}
void Spr::PHIKEngineIf::SetIntpRate(){
	((PHIKEngine*)(Object*)(ObjectIf*)this)->SetIntpRate();
}
int Spr::PHIKEngineIf::GetIntpRate(){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->GetIntpRate();
}
void Spr::PHIKEngineIf::ApplyExactState(bool reverse){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->ApplyExactState(reverse);
}
void Spr::PHIKEngineIf::SetNumIter(int numIter){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->SetNumIter(numIter);
}
int Spr::PHIKEngineIf::GetNumIter(){
	return	((PHIKEngine*)(Object*)(ObjectIf*)this)->GetNumIter();
}
void Spr::PHIKEngineIf::FK(){
	((PHIKEngine*)(Object*)(ObjectIf*)this)->FK();
}
SPR_IFIMP1(PHTrackingEngine, PHEngine);
void Spr::PHTrackingEngineIf::SetTrackingInputs(Spr::PHRootNodeIf* root, Vec3d rootPosition, std::vector< Quaterniond > inputs){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->SetTrackingInputs(root, rootPosition, inputs);
}
void Spr::PHTrackingEngineIf::SetTrackingRootPosition(Spr::PHRootNodeIf* root, Vec3d input){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->SetTrackingRootPosition(root, input);
}
void Spr::PHTrackingEngineIf::AddTrackingNode(Spr::PHRootNodeIf* root, Spr::PHBallJointIf* reactJoint, Spr::PHBallJointIf* calcJoint, Spr::PHSolidIf* reactRootSolid, Spr::PHSolidIf* calcRootSolid, bool isRoot){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->AddTrackingNode(root, reactJoint, calcJoint, reactRootSolid, calcRootSolid, isRoot);
}
void Spr::PHTrackingEngineIf::DeleteTrackingNodes(Spr::PHRootNodeIf* root){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->DeleteTrackingNodes(root);
}
Spr::SpatialVector Spr::PHTrackingEngineIf::GetTipAcceleration(Spr::PHRootNodeIf* root, int i){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->GetTipAcceleration(root, i);
}
void Spr::PHTrackingEngineIf::TrackWithForce(Spr::PHRootNodeIf* root){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->TrackWithForce(root);
}
void Spr::PHTrackingEngineIf::AddTrackingForce(Spr::PHBallJointNodeIf* calcNode, Spr::PHBallJointNodeIf* reactNode, double timeStep, Vec3d targetAngularAcceleration, Spr::SpatialVector parentTargetAcceleration, Spr::SpatialVector& targetAcceleration, Vec3d& force, Vec3d& torque){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->AddTrackingForce(calcNode, reactNode, timeStep, targetAngularAcceleration, parentTargetAcceleration, targetAcceleration, force, torque);
}
void Spr::PHTrackingEngineIf::SetMaxRootVelocity(double max){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->SetMaxRootVelocity(max);
}
void Spr::PHTrackingEngineIf::SetMaxAngularVelocity(double max){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->SetMaxAngularVelocity(max);
}
Vec3d Spr::PHTrackingEngineIf::CalcEigenvalue(Matrix3d a){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->CalcEigenvalue(a);
}
Matrix3d Spr::PHTrackingEngineIf::CalcLeavesInertia(Spr::PHSolidIf* solid, Spr::PHJointIf* joint){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->CalcLeavesInertia(solid, joint);
}
Matrix3d Spr::PHTrackingEngineIf::CalcInertiaFromJoint(Spr::PHSolidIf* solid, Spr::PHJointIf* joint){
	return	((PHTrackingEngine*)(Object*)(ObjectIf*)this)->CalcInertiaFromJoint(solid, joint);
}
SPR_IFIMP1(PHFemEngine, PHEngine);
void Spr::PHFemEngineIf::SetTimeStep(double dt){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->SetTimeStep(dt);
}
double Spr::PHFemEngineIf::GetTimeStep(){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->GetTimeStep();
}
void Spr::PHFemEngineIf::SetVibrationTransfer(bool bEnable){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->SetVibrationTransfer(bEnable);
}
void Spr::PHFemEngineIf::SetThermalTransfer(bool bEnable){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->SetThermalTransfer(bEnable);
}
int Spr::PHFemEngineIf::NMeshNew(){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->NMeshNew();
}
Spr::PHFemMeshNewIf* Spr::PHFemEngineIf::GetMeshNew(int i){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->GetMeshNew(i);
}
bool Spr::PHFemEngineIf::AddMeshPair(Spr::PHFemMeshNewIf* m0, Spr::PHFemMeshNewIf* m1){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->AddMeshPair(m0, m1);
}
bool Spr::PHFemEngineIf::RemoveMeshPair(Spr::PHFemMeshNewIf* m0, Spr::PHFemMeshNewIf* m1){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->RemoveMeshPair(m0, m1);
}
void Spr::PHFemEngineIf::ThermalTransfer(){
	((PHFemEngine*)(Object*)(ObjectIf*)this)->ThermalTransfer();
}
void Spr::PHFemEngineIf::setheatTransferRatio(double setheatTransferRatio){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->setheatTransferRatio(setheatTransferRatio);
}
void Spr::PHFemEngineIf::FEMSolidMatchRefresh(){
	((PHFemEngine*)(Object*)(ObjectIf*)this)->FEMSolidMatchRefresh();
}
void Spr::PHFemEngineIf::InitContacts(){
	((PHFemEngine*)(Object*)(ObjectIf*)this)->InitContacts();
}
void Spr::PHFemEngineIf::ClearContactVectors(){
	((PHFemEngine*)(Object*)(ObjectIf*)this)->ClearContactVectors();
}
int Spr::PHFemEngineIf::NMesh(){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->NMesh();
}
Spr::PHFemMeshIf* Spr::PHFemEngineIf::GetMesh(int i){
	return	((PHFemEngine*)(Object*)(ObjectIf*)this)->GetMesh(i);
}
SPR_IFIMP1(PHOpEngine, PHEngine);
void Spr::PHOpEngineIf::SetTimeStep(double dt){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetTimeStep(dt);
}
double Spr::PHOpEngineIf::GetTimeStep(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetTimeStep();
}
void Spr::PHOpEngineIf::SetGravity(bool gflag){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetGravity(gflag);
}
void Spr::PHOpEngineIf::InitialHapticRenderer(int objId){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->InitialHapticRenderer(objId);
}
PHOpObjDesc* Spr::PHOpEngineIf::GetOpObj(int i){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetOpObj(i);
}
void Spr::PHOpEngineIf::StepWithBlend(){
	((PHOpEngine*)(Object*)(ObjectIf*)this)->StepWithBlend();
}
int Spr::PHOpEngineIf::AddOpObj(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->AddOpObj();
}
void Spr::PHOpEngineIf::SetHapticSolveEnable(bool enable){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetHapticSolveEnable(enable);
}
bool Spr::PHOpEngineIf::IsHapticSolve(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->IsHapticSolve();
}
void Spr::PHOpEngineIf::SetProxyCorrectionEnable(bool enable){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetProxyCorrectionEnable(enable);
}
bool Spr::PHOpEngineIf::IsProxyCorrection(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->IsProxyCorrection();
}
void Spr::PHOpEngineIf::SetUpdateNormal(bool flag){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetUpdateNormal(flag);
}
bool Spr::PHOpEngineIf::IsUpdateNormal(int obji){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->IsUpdateNormal(obji);
}
void Spr::PHOpEngineIf::SetUseHaptic(bool hapticUsage){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetUseHaptic(hapticUsage);
}
bool Spr::PHOpEngineIf::GetUseHaptic(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetUseHaptic();
}
PHOpObjIf* Spr::PHOpEngineIf::GetOpObjIf(int obji){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetOpObjIf(obji);
}
int Spr::PHOpEngineIf::GetOpObjNum(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetOpObjNum();
}
Spr::ObjectIf* Spr::PHOpEngineIf::GetOpAnimator(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetOpAnimator();
}
Spr::ObjectIf* Spr::PHOpEngineIf::GetOpHapticController(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetOpHapticController();
}
Spr::ObjectIf* Spr::PHOpEngineIf::GetOpHapticRenderer(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetOpHapticRenderer();
}
void Spr::PHOpEngineIf::SetIterationCount(int count){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetIterationCount(count);
}
int Spr::PHOpEngineIf::GetIterationCount(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetIterationCount();
}
void Spr::PHOpEngineIf::SetAnimationFlag(bool flag){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetAnimationFlag(flag);
}
bool Spr::PHOpEngineIf::GetAnimationFlag(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetAnimationFlag();
}
void Spr::PHOpEngineIf::SetDrawPtclR(float r){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->SetDrawPtclR(r);
}
float Spr::PHOpEngineIf::GetDrawPtclR(){
	return	((PHOpEngine*)(Object*)(ObjectIf*)this)->GetDrawPtclR();
}
void Spr::PHOpEngineIf::InitialNoMeshHapticRenderer(){
	((PHOpEngine*)(Object*)(ObjectIf*)this)->InitialNoMeshHapticRenderer();
}
SPR_IFIMP1(PHFemMesh, SceneObject);
SPR_IFIMP1(PHFemMeshThermo, PHFemMesh);
int Spr::PHFemMeshThermoIf::GetSurfaceVertex(int id){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetSurfaceVertex(id);
}
int Spr::PHFemMeshThermoIf::NSurfaceVertices(){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->NSurfaceVertices();
}
void Spr::PHFemMeshThermoIf::SetVertexTc(int id, double temp){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetVertexTc(id, temp);
}
void Spr::PHFemMeshThermoIf::SetVertexTc(int id, double temp, double heatTrans){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetVertexTc(id, temp, heatTrans);
}
Vec3d Spr::PHFemMeshThermoIf::GetPose(int id){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetPose(id);
}
Vec3d Spr::PHFemMeshThermoIf::GetSufVtxPose(unsigned int id){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetSufVtxPose(id);
}
unsigned long Spr::PHFemMeshThermoIf::GetStepCount(){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetStepCount();
}
unsigned long Spr::PHFemMeshThermoIf::GetStepCountCyc(){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetStepCountCyc();
}
double Spr::PHFemMeshThermoIf::GetVertexTemp(unsigned int id){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetVertexTemp(id);
}
double Spr::PHFemMeshThermoIf::GetSufVertexTemp(unsigned int id){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetSufVertexTemp(id);
}
void Spr::PHFemMeshThermoIf::SetVertexTemp(unsigned int id, double temp){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetVertexTemp(id, temp);
}
void Spr::PHFemMeshThermoIf::SetVerticesTempAll(double temp){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetVerticesTempAll(temp);
}
void Spr::PHFemMeshThermoIf::AddvecFAll(unsigned int id, double dqdt){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->AddvecFAll(id, dqdt);
}
void Spr::PHFemMeshThermoIf::SetvecFAll(unsigned int id, double dqdt){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetvecFAll(id, dqdt);
}
void Spr::PHFemMeshThermoIf::SetRhoSpheat(double rho, double Cp){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetRhoSpheat(rho, Cp);
}
unsigned int Spr::PHFemMeshThermoIf::GetNFace(){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetNFace();
}
std::vector< Vec3d > Spr::PHFemMeshThermoIf::GetFaceEdgeVtx(unsigned int id){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetFaceEdgeVtx(id);
}
Vec3d Spr::PHFemMeshThermoIf::GetFaceEdgeVtx(unsigned int id, unsigned int vtx){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetFaceEdgeVtx(id, vtx);
}
Vec2d Spr::PHFemMeshThermoIf::GetIHbandDrawVtx(){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetIHbandDrawVtx();
}
void Spr::PHFemMeshThermoIf::CalcIHdqdt_atleast(double r, double R, double dqdtAll, unsigned int num){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->CalcIHdqdt_atleast(r, R, dqdtAll, num);
}
void Spr::PHFemMeshThermoIf::UpdateIHheatband(double xS, double xE, unsigned int heatingMODE){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->UpdateIHheatband(xS, xE, heatingMODE);
}
void Spr::PHFemMeshThermoIf::UpdateIHheat(unsigned int heating){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->UpdateIHheat(heating);
}
void Spr::PHFemMeshThermoIf::UpdateVecF(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->UpdateVecF();
}
void Spr::PHFemMeshThermoIf::UpdateVecF_frypan(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->UpdateVecF_frypan();
}
void Spr::PHFemMeshThermoIf::DecrMoist(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->DecrMoist();
}
void Spr::PHFemMeshThermoIf::DecrMoist_velo(double vel){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->DecrMoist_velo(vel);
}
void Spr::PHFemMeshThermoIf::DecrMoist_vel(double dt){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->DecrMoist_vel(dt);
}
void Spr::PHFemMeshThermoIf::InitAllVertexTemp(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->InitAllVertexTemp();
}
void Spr::PHFemMeshThermoIf::SetInitThermoConductionParam(double thConduct, double rho, double specificHeat, double heatTrans){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetInitThermoConductionParam(thConduct, rho, specificHeat, heatTrans);
}
void Spr::PHFemMeshThermoIf::SetParamAndReCreateMatrix(double thConduct0, double roh0, double specificHeat0){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetParamAndReCreateMatrix(thConduct0, roh0, specificHeat0);
}
double Spr::PHFemMeshThermoIf::GetArbitraryPointTemp(Vec3d temppos){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetArbitraryPointTemp(temppos);
}
double Spr::PHFemMeshThermoIf::GetVtxTempInTets(Vec3d temppos){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetVtxTempInTets(temppos);
}
void Spr::PHFemMeshThermoIf::InitVecFAlls(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->InitVecFAlls();
}
double Spr::PHFemMeshThermoIf::Get_thConduct(){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->Get_thConduct();
}
bool Spr::PHFemMeshThermoIf::SetConcentricHeatMap(std::vector< double > r, std::vector< double > temp, Vec2d origin){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetConcentricHeatMap(r, temp, origin);
}
void Spr::PHFemMeshThermoIf::SetThermalEmissivityToVerticesAll(double thermalEmissivity, double thermalEmissivity_const){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetThermalEmissivityToVerticesAll(thermalEmissivity, thermalEmissivity_const);
}
void Spr::PHFemMeshThermoIf::SetOuterTemp(double temp){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetOuterTemp(temp);
}
void Spr::PHFemMeshThermoIf::SetThermalRadiation(double ems, double ems_const){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetThermalRadiation(ems, ems_const);
}
void Spr::PHFemMeshThermoIf::SetGaussCalcParam(unsigned int cyc, double epsilon){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetGaussCalcParam(cyc, epsilon);
}
void Spr::PHFemMeshThermoIf::InitTcAll(double temp){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->InitTcAll(temp);
}
void Spr::PHFemMeshThermoIf::InitToutAll(double temp){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->InitToutAll(temp);
}
void Spr::PHFemMeshThermoIf::SetWeekPow(double weekPow_){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetWeekPow(weekPow_);
}
void Spr::PHFemMeshThermoIf::SetIHParamWEEK(double inr_, double outR_, double weekPow_){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetIHParamWEEK(inr_, outR_, weekPow_);
}
void Spr::PHFemMeshThermoIf::SetHeatTransRatioToAllVertex(double heatTransR_){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetHeatTransRatioToAllVertex(heatTransR_);
}
void Spr::PHFemMeshThermoIf::AfterSetDesc(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->AfterSetDesc();
}
void Spr::PHFemMeshThermoIf::SetStopTimespan(double timespan){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->SetStopTimespan(timespan);
}
void Spr::PHFemMeshThermoIf::UpdateMatk_RadiantHeatToAir(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->UpdateMatk_RadiantHeatToAir();
}
void Spr::PHFemMeshThermoIf::ActivateVtxbeRadiantHeat(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->ActivateVtxbeRadiantHeat();
}
PTM::TMatrixRow< 4,4,double > Spr::PHFemMeshThermoIf::GetKMatInTet(unsigned int id){
	return	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->GetKMatInTet(id);
}
void Spr::PHFemMeshThermoIf::OutputMatKall(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->OutputMatKall();
}
void Spr::PHFemMeshThermoIf::IfRadiantHeatTrans(){
	((PHFemMeshThermo*)(Object*)(ObjectIf*)this)->IfRadiantHeatTrans();
}
SPR_IFIMP1(PHFemMeshNew, SceneObject);
void Spr::PHFemMeshNewIf::SetPHSolid(Spr::PHSolidIf* s){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->SetPHSolid(s);
}
Spr::PHSolidIf* Spr::PHFemMeshNewIf::GetPHSolid(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetPHSolid();
}
Spr::PHFemVibrationIf* Spr::PHFemMeshNewIf::GetPHFemVibration(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetPHFemVibration();
}
Spr::PHFemThermoIf* Spr::PHFemMeshNewIf::GetPHFemThermo(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetPHFemThermo();
}
Spr::PHFemPorousWOMoveIf* Spr::PHFemMeshNewIf::GetPHFemPorousWOMove(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetPHFemPorousWOMove();
}
int Spr::PHFemMeshNewIf::NVertices(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->NVertices();
}
int Spr::PHFemMeshNewIf::NFaces(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->NFaces();
}
int Spr::PHFemMeshNewIf::NTets(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->NTets();
}
void Spr::PHFemMeshNewIf::SetVertexUpdateFlags(bool flg){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->SetVertexUpdateFlags(flg);
}
void Spr::PHFemMeshNewIf::SetVertexUpateFlag(int vid, bool flg){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->SetVertexUpateFlag(vid, flg);
}
double Spr::PHFemMeshNewIf::CompTetVolume(int tetID, bool bDeform){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->CompTetVolume(tetID, bDeform);
}
bool Spr::PHFemMeshNewIf::AddVertexDisplacementW(int vtxId, Vec3d disW){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->AddVertexDisplacementW(vtxId, disW);
}
bool Spr::PHFemMeshNewIf::AddVertexDisplacementL(int vtxId, Vec3d disL){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->AddVertexDisplacementL(vtxId, disL);
}
bool Spr::PHFemMeshNewIf::SetVertexPositionW(int vtxId, Vec3d posW){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->SetVertexPositionW(vtxId, posW);
}
bool Spr::PHFemMeshNewIf::SetVertexPositionL(int vtxId, Vec3d posL){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->SetVertexPositionL(vtxId, posL);
}
bool Spr::PHFemMeshNewIf::SetVertexVelocityL(int vtxId, Vec3d posL){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->SetVertexVelocityL(vtxId, posL);
}
Vec3d Spr::PHFemMeshNewIf::GetVertexVelocityL(int vtxId){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetVertexVelocityL(vtxId);
}
Vec3d Spr::PHFemMeshNewIf::GetVertexPositionL(int vtxId){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetVertexPositionL(vtxId);
}
Vec3d Spr::PHFemMeshNewIf::GetVertexDisplacementL(int vtxId){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetVertexDisplacementL(vtxId);
}
Vec3d Spr::PHFemMeshNewIf::GetVertexInitalPositionL(int vtxId){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetVertexInitalPositionL(vtxId);
}
void Spr::PHFemMeshNewIf::SetVelocity(Vec3d v){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->SetVelocity(v);
}
int* Spr::PHFemMeshNewIf::GetTetVertexIds(int t){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetTetVertexIds(t);
}
int* Spr::PHFemMeshNewIf::GetFaceVertexIds(int f){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetFaceVertexIds(f);
}
Vec3d Spr::PHFemMeshNewIf::GetFaceNormal(int f){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetFaceNormal(f);
}
int Spr::PHFemMeshNewIf::GetSurfaceVertex(int i){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->GetSurfaceVertex(i);
}
int Spr::PHFemMeshNewIf::NSurfaceVertices(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->NSurfaceVertices();
}
int Spr::PHFemMeshNewIf::NSurfaceFace(){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->NSurfaceFace();
}
bool Spr::PHFemMeshNewIf::CompTetShapeFunctionValue(const int& tetId, const Vec3d& posL, Vec4d& value, const bool& bDeform){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->CompTetShapeFunctionValue(tetId, posL, value, bDeform);
}
int Spr::PHFemMeshNewIf::FindTetFromFace(int faceId){
	return	((PHFemMeshNew*)(Object*)(ObjectIf*)this)->FindTetFromFace(faceId);
}
SPR_IFIMP1(PHFemBase, SceneObject);
Spr::PHFemMeshNewIf* Spr::PHFemBaseIf::GetPHFemMesh(){
	return	((PHFemBase*)(Object*)(ObjectIf*)this)->GetPHFemMesh();
}
SPR_IFIMP1(PHFemVibration, PHFemBase);
void Spr::PHFemVibrationIf::SetTimeStep(double dt){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetTimeStep(dt);
}
double Spr::PHFemVibrationIf::GetTimeStep(){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetTimeStep();
}
void Spr::PHFemVibrationIf::SetYoungModulus(double value){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetYoungModulus(value);
}
double Spr::PHFemVibrationIf::GetYoungModulus(){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetYoungModulus();
}
void Spr::PHFemVibrationIf::SetPoissonsRatio(double value){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetPoissonsRatio(value);
}
double Spr::PHFemVibrationIf::GetPoissonsRatio(){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetPoissonsRatio();
}
void Spr::PHFemVibrationIf::SetDensity(double value){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetDensity(value);
}
double Spr::PHFemVibrationIf::GetDensity(){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetDensity();
}
void Spr::PHFemVibrationIf::SetAlpha(double value){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetAlpha(value);
}
double Spr::PHFemVibrationIf::GetAlpha(){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetAlpha();
}
void Spr::PHFemVibrationIf::SetBeta(double value){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetBeta(value);
}
double Spr::PHFemVibrationIf::GetBeta(){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetBeta();
}
void Spr::PHFemVibrationIf::SetBoundary(int vtxIds){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetBoundary(vtxIds);
}
void Spr::PHFemVibrationIf::ClearBoundary(){
	((PHFemVibration*)(Object*)(ObjectIf*)this)->ClearBoundary();
}
std::vector< int > Spr::PHFemVibrationIf::GetBoundary(){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetBoundary();
}
void Spr::PHFemVibrationIf::SetAnalysisMode(Spr::PHFemVibrationDesc::ANALYSIS_MODE mode){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetAnalysisMode(mode);
}
void Spr::PHFemVibrationIf::SetIntegrationMode(Spr::PHFemVibrationDesc::INTEGRATION_MODE mode){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetIntegrationMode(mode);
}
bool Spr::PHFemVibrationIf::AddBoundaryCondition(int vtxId, Vec3i dof){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->AddBoundaryCondition(vtxId, dof);
}
void Spr::PHFemVibrationIf::DeleteBoundaryCondition(){
	((PHFemVibration*)(Object*)(ObjectIf*)this)->DeleteBoundaryCondition();
}
bool Spr::PHFemVibrationIf::FindNeighborTetrahedron(Vec3d posW, int& tetId, Vec3d& cpW, bool bDeform){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->FindNeighborTetrahedron(posW, tetId, cpW, bDeform);
}
bool Spr::PHFemVibrationIf::SetDamping(int tetId, Vec3d posW, double damp_ratio){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetDamping(tetId, posW, damp_ratio);
}
bool Spr::PHFemVibrationIf::AddForce(int tetId, Vec3d posW, Vec3d fW){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->AddForce(tetId, posW, fW);
}
bool Spr::PHFemVibrationIf::GetDisplacement(int tetId, Vec3d posW, Vec3d& disp, bool bDeform){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetDisplacement(tetId, posW, disp, bDeform);
}
bool Spr::PHFemVibrationIf::GetVelocity(int tetId, Vec3d posW, Vec3d& vel, bool bDeform){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetVelocity(tetId, posW, vel, bDeform);
}
bool Spr::PHFemVibrationIf::GetPosition(int tetId, Vec3d posW, Vec3d& pos, bool bDeform){
	return	((PHFemVibration*)(Object*)(ObjectIf*)this)->GetPosition(tetId, posW, pos, bDeform);
}
void Spr::PHFemVibrationIf::SetbRecomp(){
	((PHFemVibration*)(Object*)(ObjectIf*)this)->SetbRecomp();
}
void Spr::PHFemVibrationIf::Init(){
	((PHFemVibration*)(Object*)(ObjectIf*)this)->Init();
}
SPR_IFIMP1(PHFemThermo, PHFemBase);
int Spr::PHFemThermoIf::GetSurfaceVertex(int id){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetSurfaceVertex(id);
}
int Spr::PHFemThermoIf::NSurfaceVertices(){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->NSurfaceVertices();
}
void Spr::PHFemThermoIf::SetVertexTc(int id, double temp){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetVertexTc(id, temp);
}
void Spr::PHFemThermoIf::SetVertexTc(int id, double temp, double heatTrans){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetVertexTc(id, temp, heatTrans);
}
Vec3d Spr::PHFemThermoIf::GetPose(int id){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetPose(id);
}
Vec3d Spr::PHFemThermoIf::GetSufVtxPose(unsigned int id){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetSufVtxPose(id);
}
unsigned long Spr::PHFemThermoIf::GetStepCount(){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetStepCount();
}
unsigned long Spr::PHFemThermoIf::GetStepCountCyc(){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetStepCountCyc();
}
double Spr::PHFemThermoIf::GetVertexTemp(unsigned int id){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetVertexTemp(id);
}
double Spr::PHFemThermoIf::GetSufVertexTemp(unsigned int id){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetSufVertexTemp(id);
}
void Spr::PHFemThermoIf::SetVertexTemp(unsigned int id, double temp){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetVertexTemp(id, temp);
}
void Spr::PHFemThermoIf::SetVerticesTempAll(double temp){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetVerticesTempAll(temp);
}
void Spr::PHFemThermoIf::AddvecFAll(unsigned int id, double dqdt){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->AddvecFAll(id, dqdt);
}
void Spr::PHFemThermoIf::SetvecFAll(unsigned int id, double dqdt){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetvecFAll(id, dqdt);
}
void Spr::PHFemThermoIf::SetRhoSpheat(double rho, double Cp){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetRhoSpheat(rho, Cp);
}
Vec2d Spr::PHFemThermoIf::GetIHbandDrawVtx(){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetIHbandDrawVtx();
}
void Spr::PHFemThermoIf::CalcIHdqdt_atleast(double r, double R, double dqdtAll, unsigned int num){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->CalcIHdqdt_atleast(r, R, dqdtAll, num);
}
void Spr::PHFemThermoIf::UpdateIHheatband(double xS, double xE, unsigned int heatingMODE){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->UpdateIHheatband(xS, xE, heatingMODE);
}
void Spr::PHFemThermoIf::UpdateIHheat(unsigned int heating){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->UpdateIHheat(heating);
}
void Spr::PHFemThermoIf::UpdateVecF(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->UpdateVecF();
}
void Spr::PHFemThermoIf::UpdateVecF_frypan(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->UpdateVecF_frypan();
}
void Spr::PHFemThermoIf::DecrMoist(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->DecrMoist();
}
void Spr::PHFemThermoIf::DecrMoist_velo(double vel){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->DecrMoist_velo(vel);
}
void Spr::PHFemThermoIf::DecrMoist_vel(double dt){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->DecrMoist_vel(dt);
}
void Spr::PHFemThermoIf::InitAllVertexTemp(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->InitAllVertexTemp();
}
void Spr::PHFemThermoIf::SetInitThermoConductionParam(double thConduct, double rho, double specificHeat, double heatTrans){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetInitThermoConductionParam(thConduct, rho, specificHeat, heatTrans);
}
void Spr::PHFemThermoIf::SetParamAndReCreateMatrix(double thConduct0, double roh0, double specificHeat0){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetParamAndReCreateMatrix(thConduct0, roh0, specificHeat0);
}
double Spr::PHFemThermoIf::GetArbitraryPointTemp(Vec3d temppos){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetArbitraryPointTemp(temppos);
}
double Spr::PHFemThermoIf::GetVtxTempInTets(Vec3d temppos){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetVtxTempInTets(temppos);
}
void Spr::PHFemThermoIf::InitVecFAlls(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->InitVecFAlls();
}
double Spr::PHFemThermoIf::Get_thConduct(){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->Get_thConduct();
}
bool Spr::PHFemThermoIf::SetConcentricHeatMap(std::vector< double > r, std::vector< double > temp, Vec2d origin){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetConcentricHeatMap(r, temp, origin);
}
void Spr::PHFemThermoIf::SetThermalEmissivityToVerticesAll(double thermalEmissivity, double thermalEmissivity_const){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetThermalEmissivityToVerticesAll(thermalEmissivity, thermalEmissivity_const);
}
void Spr::PHFemThermoIf::SetOuterTemp(double temp){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetOuterTemp(temp);
}
void Spr::PHFemThermoIf::SetThermalRadiation(double ems, double ems_const){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetThermalRadiation(ems, ems_const);
}
void Spr::PHFemThermoIf::SetGaussCalcParam(unsigned int cyc, double epsilon){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetGaussCalcParam(cyc, epsilon);
}
void Spr::PHFemThermoIf::InitTcAll(double temp){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->InitTcAll(temp);
}
void Spr::PHFemThermoIf::InitToutAll(double temp){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->InitToutAll(temp);
}
void Spr::PHFemThermoIf::SetWeekPow(double weekPow_){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetWeekPow(weekPow_);
}
void Spr::PHFemThermoIf::SetIHParamWEEK(double inr_, double outR_, double weekPow_){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetIHParamWEEK(inr_, outR_, weekPow_);
}
void Spr::PHFemThermoIf::SetHeatTransRatioToAllVertex(double heatTransR_){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetHeatTransRatioToAllVertex(heatTransR_);
}
void Spr::PHFemThermoIf::AfterSetDesc(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->AfterSetDesc();
}
void Spr::PHFemThermoIf::SetStopTimespan(double timespan){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetStopTimespan(timespan);
}
void Spr::PHFemThermoIf::UpdateMatk_RadiantHeatToAir(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->UpdateMatk_RadiantHeatToAir();
}
void Spr::PHFemThermoIf::ReCreateMatrix(double thConduct0){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->ReCreateMatrix(thConduct0);
}
void Spr::PHFemThermoIf::ActivateVtxbeRadiantHeat(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->ActivateVtxbeRadiantHeat();
}
PTM::TMatrixRow< 4,4,double > Spr::PHFemThermoIf::GetKMatInTet(unsigned int id){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetKMatInTet(id);
}
void Spr::PHFemThermoIf::OutputMatKall(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->OutputMatKall();
}
void Spr::PHFemThermoIf::IfRadiantHeatTrans(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->IfRadiantHeatTrans();
}
void Spr::PHFemThermoIf::IfRadiantHeatTransSteak(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->IfRadiantHeatTransSteak();
}
float Spr::PHFemThermoIf::calcGvtx(std::string fwfood, int pv, unsigned int texture_mode){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->calcGvtx(fwfood, pv, texture_mode);
}
void Spr::PHFemThermoIf::SetTimeStep(double dt){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetTimeStep(dt);
}
Vec3d Spr::PHFemThermoIf::GetVertexNormal(unsigned int vtxid){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetVertexNormal(vtxid);
}
void Spr::PHFemThermoIf::SetVertexHeatTransRatio(unsigned int vtxid, double heattransRatio){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetVertexHeatTransRatio(vtxid, heattransRatio);
}
void Spr::PHFemThermoIf::SetVertexBeRadiantHeat(unsigned int vtxid, bool flag){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetVertexBeRadiantHeat(vtxid, flag);
}
double Spr::PHFemThermoIf::GetVertexArea(unsigned int vtxid){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetVertexArea(vtxid);
}
void Spr::PHFemThermoIf::SetVertexToofar(unsigned int vtxid, bool tooFar){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetVertexToofar(vtxid, tooFar);
}
bool Spr::PHFemThermoIf::GetVertexToofar(unsigned int vtxid){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetVertexToofar(vtxid);
}
void Spr::PHFemThermoIf::SetVertexBeCondVtxs(unsigned int vtxid, bool becondVtxs){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetVertexBeCondVtxs(vtxid, becondVtxs);
}
void Spr::PHFemThermoIf::CreateVecFAll(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->CreateVecFAll();
}
void Spr::PHFemThermoIf::CalcFaceNormalAll(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->CalcFaceNormalAll();
}
void Spr::PHFemThermoIf::CalcVertexNormalAll(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->CalcVertexNormalAll();
}
void Spr::PHFemThermoIf::InitFaceNormalAll(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->InitFaceNormalAll();
}
void Spr::PHFemThermoIf::InitVertexNormalAll(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->InitVertexNormalAll();
}
void Spr::PHFemThermoIf::RevVertexNormalAll(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->RevVertexNormalAll();
}
void Spr::PHFemThermoIf::SetWeekPowFULL(double weekPow_full){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetWeekPowFULL(weekPow_full);
}
void Spr::PHFemThermoIf::SetweekPow_FULL(double setweekPow_FULL){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetweekPow_FULL(setweekPow_FULL);
}
void Spr::PHFemThermoIf::setIhRatio(double a){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->setIhRatio(a);
}
void Spr::PHFemThermoIf::Setems(double setems){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->Setems(setems);
}
void Spr::PHFemThermoIf::Setems_steak(double setems_steak){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->Setems_steak(setems_steak);
}
void Spr::PHFemThermoIf::SetthConduct(double thConduct){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetthConduct(thConduct);
}
double Spr::PHFemThermoIf::GetWeekPowFULL(){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetWeekPowFULL();
}
Vec3d Spr::PHFemThermoIf::GetVertexPose(unsigned int vtxid){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetVertexPose(vtxid);
}
void Spr::PHFemThermoIf::OutTetVolumeAll(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->OutTetVolumeAll();
}
int Spr::PHFemThermoIf::GetTetsV(unsigned int tetid, unsigned int vtxid){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetTetsV(tetid, vtxid);
}
void Spr::PHFemThermoIf::VecFNegativeCheck(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->VecFNegativeCheck();
}
double Spr::PHFemThermoIf::GetVecFElem(unsigned int vtxid){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetVecFElem(vtxid);
}
int Spr::PHFemThermoIf::GetTetVNums(unsigned int id, unsigned int num){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetTetVNums(id, num);
}
double Spr::PHFemThermoIf::GetInitialTemp(){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->GetInitialTemp();
}
void Spr::PHFemThermoIf::UpdateVertexTempAll(){
	((PHFemThermo*)(Object*)(ObjectIf*)this)->UpdateVertexTempAll();
}
void Spr::PHFemThermoIf::SetThermoCameraScale(double minTemp, double maxTemp){
	return	((PHFemThermo*)(Object*)(ObjectIf*)this)->SetThermoCameraScale(minTemp, maxTemp);
}
SPR_IFIMP1(PHFemPorousWOMove, PHFemBase);
void Spr::PHFemPorousWOMoveIf::SetTimeStep(double dt){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->SetTimeStep(dt);
}
double Spr::PHFemPorousWOMoveIf::GetTimeStep(){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->GetTimeStep();
}
void Spr::PHFemPorousWOMoveIf::UpdateVertexRhoWAll(){
	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->UpdateVertexRhoWAll();
}
void Spr::PHFemPorousWOMoveIf::UpdateVertexRhoOAll(){
	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->UpdateVertexRhoOAll();
}
void Spr::PHFemPorousWOMoveIf::SetVertexMw(unsigned int vtxid, double mw){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->SetVertexMw(vtxid, mw);
}
void Spr::PHFemPorousWOMoveIf::SetVertexMo(unsigned int vtxid, double mo){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->SetVertexMo(vtxid, mo);
}
double Spr::PHFemPorousWOMoveIf::GetVertexMw(unsigned int vtxid){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->GetVertexMw(vtxid);
}
double Spr::PHFemPorousWOMoveIf::GetVertexMo(unsigned int vtxid){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->GetVertexMo(vtxid);
}
double Spr::PHFemPorousWOMoveIf::GetVtxWaterInTets(Vec3d temppos){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->GetVtxWaterInTets(temppos);
}
double Spr::PHFemPorousWOMoveIf::GetVtxOilInTets(Vec3d temppos){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->GetVtxOilInTets(temppos);
}
double Spr::PHFemPorousWOMoveIf::GetVertexRhoW(unsigned int vtxid){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->GetVertexRhoW(vtxid);
}
double Spr::PHFemPorousWOMoveIf::GetVertexRhoO(unsigned int vtxid){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->GetVertexRhoO(vtxid);
}
float Spr::PHFemPorousWOMoveIf::calcGvtx(std::string fwfood, int pv, unsigned int texture_mode){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->calcGvtx(fwfood, pv, texture_mode);
}
void Spr::PHFemPorousWOMoveIf::setGravity(Vec3d g){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->setGravity(g);
}
double Spr::PHFemPorousWOMoveIf::decideWetValue(unsigned int faceid){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->decideWetValue(faceid);
}
double Spr::PHFemPorousWOMoveIf::GetVtxSaturation(unsigned int vtxid){
	return	((PHFemPorousWOMove*)(Object*)(ObjectIf*)this)->GetVtxSaturation(vtxid);
}
SPR_IFIMP1(PHHapticPointer, PHSolid);
void Spr::PHHapticPointerIf::SetHapticRenderMode(Spr::PHHapticPointerDesc::HapticRenderMode m){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetHapticRenderMode(m);
}
Spr::PHHapticPointerDesc::HapticRenderMode Spr::PHHapticPointerIf::GetHapticRenderMode(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetHapticRenderMode();
}
void Spr::PHHapticPointerIf::EnableRotation(bool b){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->EnableRotation(b);
}
bool Spr::PHHapticPointerIf::IsRotation(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->IsRotation();
}
void Spr::PHHapticPointerIf::EnableForce(bool b){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->EnableForce(b);
}
bool Spr::PHHapticPointerIf::IsForce(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->IsForce();
}
void Spr::PHHapticPointerIf::EnableFriction(bool b){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->EnableFriction(b);
}
bool Spr::PHHapticPointerIf::IsFriction(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->IsFriction();
}
void Spr::PHHapticPointerIf::EnableTimeVaryFriction(bool b){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->EnableTimeVaryFriction(b);
}
bool Spr::PHHapticPointerIf::IsTimeVaryFriction(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->IsTimeVaryFriction();
}
void Spr::PHHapticPointerIf::EnableVibration(bool b){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->EnableVibration(b);
}
bool Spr::PHHapticPointerIf::IsVibration(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->IsVibration();
}
void Spr::PHHapticPointerIf::SetFrictionSpring(float s){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetFrictionSpring(s);
}
float Spr::PHHapticPointerIf::GetFrictionSpring(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetFrictionSpring();
}
void Spr::PHHapticPointerIf::SetFrictionDamper(float s){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetFrictionDamper(s);
}
float Spr::PHHapticPointerIf::GetFrictionDamper(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetFrictionDamper();
}
void Spr::PHHapticPointerIf::SetReflexSpring(float s){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetReflexSpring(s);
}
float Spr::PHHapticPointerIf::GetReflexSpring(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetReflexSpring();
}
void Spr::PHHapticPointerIf::SetReflexDamper(float d){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetReflexDamper(d);
}
float Spr::PHHapticPointerIf::GetReflexDamper(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetReflexDamper();
}
void Spr::PHHapticPointerIf::SetRotationReflexSpring(float s){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetRotationReflexSpring(s);
}
float Spr::PHHapticPointerIf::GetRotationReflexSpring(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetRotationReflexSpring();
}
void Spr::PHHapticPointerIf::SetRotationReflexDamper(float d){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetRotationReflexDamper(d);
}
float Spr::PHHapticPointerIf::GetRotationReflexDamper(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetRotationReflexDamper();
}
void Spr::PHHapticPointerIf::SetLocalRange(float r){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetLocalRange(r);
}
float Spr::PHHapticPointerIf::GetLocalRange(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetLocalRange();
}
void Spr::PHHapticPointerIf::SetPosScale(double scale){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetPosScale(scale);
}
double Spr::PHHapticPointerIf::GetPosScale(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetPosScale();
}
void Spr::PHHapticPointerIf::SetRotationalWeight(double w){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetRotationalWeight(w);
}
double Spr::PHHapticPointerIf::GetRotationalWeight(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetRotationalWeight();
}
void Spr::PHHapticPointerIf::SetDefaultPose(Posed p){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetDefaultPose(p);
}
Posed Spr::PHHapticPointerIf::GetDefaultPose(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetDefaultPose();
}
void Spr::PHHapticPointerIf::SetFrictionType(PHFrictionType type){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetFrictionType(type);
}
PHFrictionType Spr::PHHapticPointerIf::GetFrictionType(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetFrictionType();
}
int Spr::PHHapticPointerIf::NNeighborSolids(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->NNeighborSolids();
}
int Spr::PHHapticPointerIf::GetNeighborSolidId(int i){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetNeighborSolidId(i);
}
Spr::PHSolidIf* Spr::PHHapticPointerIf::GetNeighborSolid(int i){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetNeighborSolid(i);
}
float Spr::PHHapticPointerIf::GetContactForce(int i){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetContactForce(i);
}
Spr::SpatialVector Spr::PHHapticPointerIf::GetHapticForce(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetHapticForce();
}
Spr::SpatialVector Spr::PHHapticPointerIf::GetProxyVelocity(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetProxyVelocity();
}
Posed Spr::PHHapticPointerIf::GetProxyPose(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetProxyPose();
}
Posed Spr::PHHapticPointerIf::GetLastProxyPose(){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->GetLastProxyPose();
}
void Spr::PHHapticPointerIf::SetProxyVelocity(Spr::SpatialVector spv){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->SetProxyVelocity(spv);
}
void Spr::PHHapticPointerIf::AddHapticForce(const Spr::SpatialVector& f){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->AddHapticForce(f);
}
void Spr::PHHapticPointerIf::ClearHapticForce(){
	((PHHapticPointer*)(Object*)(ObjectIf*)this)->ClearHapticForce();
}
void Spr::PHHapticPointerIf::UpdateHumanInterface(const Posed& pose, const Spr::SpatialVector& vel){
	return	((PHHapticPointer*)(Object*)(ObjectIf*)this)->UpdateHumanInterface(pose, vel);
}
SPR_IFIMP1(PHSolidForHaptic, Object);
Spr::PHSolidIf* Spr::PHSolidForHapticIf::GetLocalSolid(){
	return	((PHSolidForHaptic*)(Object*)(ObjectIf*)this)->GetLocalSolid();
}
Spr::PHSolidIf* Spr::PHSolidForHapticIf::GetSceneSolid(){
	return	((PHSolidForHaptic*)(Object*)(ObjectIf*)this)->GetSceneSolid();
}
void Spr::PHSolidForHapticIf::AddForce(Vec3d f){
	return	((PHSolidForHaptic*)(Object*)(ObjectIf*)this)->AddForce(f);
}
void Spr::PHSolidForHapticIf::AddForce(Vec3d f, Vec3d r){
	return	((PHSolidForHaptic*)(Object*)(ObjectIf*)this)->AddForce(f, r);
}
SPR_IFIMP1(PHShapePairForHaptic, CDShapePair);
int Spr::PHShapePairForHapticIf::NIrs(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->NIrs();
}
int Spr::PHShapePairForHapticIf::NIrsNormal(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->NIrsNormal();
}
Vec3d Spr::PHShapePairForHapticIf::GetIrForce(int i){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetIrForce(i);
}
double Spr::PHShapePairForHapticIf::GetMu(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetMu();
}
PHFrameIf* Spr::PHShapePairForHapticIf::GetFrame(int i){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetFrame(i);
}
void Spr::PHShapePairForHapticIf::UpdateCache(){
	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->UpdateCache();
}
Vec3d Spr::PHShapePairForHapticIf::GetAvgBristlesDeflection(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetAvgBristlesDeflection();
}
Vec3d Spr::PHShapePairForHapticIf::GetPointerPos(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetPointerPos();
}
Vec3d Spr::PHShapePairForHapticIf::GetProxyPos(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetProxyPos();
}
Vec3d Spr::PHShapePairForHapticIf::GetRelativeVel(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetRelativeVel();
}
double Spr::PHShapePairForHapticIf::GetAvgStickingTime(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetAvgStickingTime();
}
Vec3d Spr::PHShapePairForHapticIf::GetFrictionForce(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetFrictionForce();
}
double Spr::PHShapePairForHapticIf::GetLuGreFunctionG(){
	return	((PHShapePairForHaptic*)(Object*)(ObjectIf*)this)->GetLuGreFunctionG();
}
SPR_IFIMP1(PHSolidPairForHaptic, PHSolidPair);
Spr::PHShapePairForHapticIf* Spr::PHSolidPairForHapticIf::GetShapePair(int i, int j){
	return	((PHSolidPairForHaptic*)(Object*)(ObjectIf*)this)->GetShapePair(i, j);
}
Spr::PHSolidPairForHapticIf::FrictionState Spr::PHSolidPairForHapticIf::GetFrictionState(){
	return	((PHSolidPairForHaptic*)(Object*)(ObjectIf*)this)->GetFrictionState();
}
unsigned int Spr::PHSolidPairForHapticIf::GetContactCount(){
	return	((PHSolidPairForHaptic*)(Object*)(ObjectIf*)this)->GetContactCount();
}
unsigned int Spr::PHSolidPairForHapticIf::GetFrictionCount(){
	return	((PHSolidPairForHaptic*)(Object*)(ObjectIf*)this)->GetFrictionCount();
}
Vec3d Spr::PHSolidPairForHapticIf::GetForce(){
	return	((PHSolidPairForHaptic*)(Object*)(ObjectIf*)this)->GetForce();
}
Vec3d Spr::PHSolidPairForHapticIf::GetTorque(){
	return	((PHSolidPairForHaptic*)(Object*)(ObjectIf*)this)->GetTorque();
}
SPR_IFIMP1(PHHapticStepBase, Object);
double Spr::PHHapticStepBaseIf::GetHapticTimeStep(){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->GetHapticTimeStep();
}
void Spr::PHHapticStepBaseIf::SetHapticTimeStep(double dt){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->SetHapticTimeStep(dt);
}
int Spr::PHHapticStepBaseIf::NHapticPointers(){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->NHapticPointers();
}
int Spr::PHHapticStepBaseIf::NHapticSolids(){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->NHapticSolids();
}
Spr::PHHapticPointerIf* Spr::PHHapticStepBaseIf::GetHapticPointer(int i){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->GetHapticPointer(i);
}
Spr::PHSolidForHapticIf* Spr::PHHapticStepBaseIf::GetHapticSolid(int i){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->GetHapticSolid(i);
}
int Spr::PHHapticStepBaseIf::NPointersInHaptic(){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->NPointersInHaptic();
}
int Spr::PHHapticStepBaseIf::NSolidsInHaptic(){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->NSolidsInHaptic();
}
Spr::PHHapticPointerIf* Spr::PHHapticStepBaseIf::GetPointerInHaptic(int i){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->GetPointerInHaptic(i);
}
Spr::PHSolidForHapticIf* Spr::PHHapticStepBaseIf::GetSolidInHaptic(int i){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->GetSolidInHaptic(i);
}
Spr::PHSolidPairForHapticIf* Spr::PHHapticStepBaseIf::GetSolidPairInHaptic(int i, int j){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->GetSolidPairInHaptic(i, j);
}
void Spr::PHHapticStepBaseIf::ReleaseState(Spr::PHSceneIf* scene){
	return	((PHHapticStepBase*)(Object*)(ObjectIf*)this)->ReleaseState(scene);
}
SPR_IFIMP1(PHHapticStepSingle, PHHapticStepBase);
SPR_IFIMP1(PHHapticStepMulti, PHHapticStepBase);
int Spr::PHHapticStepMultiIf::GetHapticCount(){
	return	((PHHapticStepMulti*)(Object*)(ObjectIf*)this)->GetHapticCount();
}
int Spr::PHHapticStepMultiIf::GetLoopCount(){
	return	((PHHapticStepMulti*)(Object*)(ObjectIf*)this)->GetLoopCount();
}
bool Spr::PHHapticStepMultiIf::GetSyncRequired(){
	return	((PHHapticStepMulti*)(Object*)(ObjectIf*)this)->GetSyncRequired();
}
bool Spr::PHHapticStepMultiIf::GetPhysicsRequired(){
	return	((PHHapticStepMulti*)(Object*)(ObjectIf*)this)->GetPhysicsRequired();
}
SPR_IFIMP1(PHHapticEngine, PHEngine);
void Spr::PHHapticEngineIf::SetHapticStepMode(Spr::PHHapticEngineDesc::HapticStepMode mode){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->SetHapticStepMode(mode);
}
Spr::PHHapticEngineDesc::HapticStepMode Spr::PHHapticEngineIf::GetHapticStepMode(){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->GetHapticStepMode();
}
Spr::PHHapticStepBaseIf* Spr::PHHapticEngineIf::GetHapticStep(){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->GetHapticStep();
}
int Spr::PHHapticEngineIf::NSolids(){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->NSolids();
}
int Spr::PHHapticEngineIf::NPointers(){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->NPointers();
}
Spr::PHHapticPointerIf* Spr::PHHapticEngineIf::GetPointer(int i){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->GetPointer(i);
}
Spr::PHSolidPairForHapticIf* Spr::PHHapticEngineIf::GetSolidPair(int i, int j){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->GetSolidPair(i, j);
}
int Spr::PHHapticEngineIf::NSolidsInHaptic(){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->NSolidsInHaptic();
}
int Spr::PHHapticEngineIf::NPointersInHaptic(){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->NPointersInHaptic();
}
Spr::PHHapticPointerIf* Spr::PHHapticEngineIf::GetPointerInHaptic(int i){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->GetPointerInHaptic(i);
}
Spr::PHSolidPairForHapticIf* Spr::PHHapticEngineIf::GetSolidPairInHaptic(int i, int j){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->GetSolidPairInHaptic(i, j);
}
void Spr::PHHapticEngineIf::StepPhysicsSimulation(){
	((PHHapticEngine*)(Object*)(ObjectIf*)this)->StepPhysicsSimulation();
}
void Spr::PHHapticEngineIf::ReleaseState(){
	((PHHapticEngine*)(Object*)(ObjectIf*)this)->ReleaseState();
}
bool Spr::PHHapticEngineIf::SetCallbackBeforeStep(Spr::PHHapticEngineIf::Callback f, void* arg){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->SetCallbackBeforeStep(f, arg);
}
bool Spr::PHHapticEngineIf::SetCallbackAfterStep(Spr::PHHapticEngineIf::Callback f, void* arg){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->SetCallbackAfterStep(f, arg);
}
void Spr::PHHapticEngineIf::HapticRendering(Spr::PHHapticStepBaseIf* hs){
	return	((PHHapticEngine*)(Object*)(ObjectIf*)this)->HapticRendering(hs);
}
SPR_IFIMP1(PHIKEndEffector, SceneObject);
void Spr::PHIKEndEffectorIf::SetSolid(Spr::PHSolidIf* solid){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetSolid(solid);
}
Spr::PHSolidIf* Spr::PHIKEndEffectorIf::GetSolid(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetSolid();
}
void Spr::PHIKEndEffectorIf::SetParentActuator(Spr::PHIKActuatorIf* ika){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetParentActuator(ika);
}
Spr::PHIKActuatorIf* Spr::PHIKEndEffectorIf::GetParentActuator(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetParentActuator();
}
void Spr::PHIKEndEffectorIf::Enable(bool enable){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->Enable(enable);
}
bool Spr::PHIKEndEffectorIf::IsEnabled(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->IsEnabled();
}
void Spr::PHIKEndEffectorIf::SetTargetLocalPosition(Vec3d localPosition){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetLocalPosition(localPosition);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetLocalPosition(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetLocalPosition();
}
void Spr::PHIKEndEffectorIf::SetTargetLocalDirection(Vec3d localDirection){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetLocalDirection(localDirection);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetLocalDirection(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetLocalDirection();
}
void Spr::PHIKEndEffectorIf::EnablePositionControl(bool enable){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->EnablePositionControl(enable);
}
bool Spr::PHIKEndEffectorIf::IsPositionControlEnabled(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->IsPositionControlEnabled();
}
void Spr::PHIKEndEffectorIf::SetPositionPriority(double priority){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetPositionPriority(priority);
}
double Spr::PHIKEndEffectorIf::GetPositionPriority(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetPositionPriority();
}
void Spr::PHIKEndEffectorIf::SetTargetPosition(Vec3d position){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetPosition(position);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetPosition(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetPosition();
}
void Spr::PHIKEndEffectorIf::EnableOrientationControl(bool enable){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->EnableOrientationControl(enable);
}
bool Spr::PHIKEndEffectorIf::IsOrientationControlEnabled(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->IsOrientationControlEnabled();
}
void Spr::PHIKEndEffectorIf::SetOrientationPriority(double priority){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetOrientationPriority(priority);
}
double Spr::PHIKEndEffectorIf::GetOrientationPriority(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetOrientationPriority();
}
void Spr::PHIKEndEffectorIf::SetOriCtlMode(Spr::PHIKEndEffectorDesc::OriCtlMode mode){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetOriCtlMode(mode);
}
Spr::PHIKEndEffectorDesc::OriCtlMode Spr::PHIKEndEffectorIf::GetOriCtlMode(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetOriCtlMode();
}
void Spr::PHIKEndEffectorIf::SetTargetOrientation(Quaterniond orientation){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetOrientation(orientation);
}
Quaterniond Spr::PHIKEndEffectorIf::GetTargetOrientation(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetOrientation();
}
void Spr::PHIKEndEffectorIf::SetTargetDirection(Vec3d direction){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetDirection(direction);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetDirection(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetDirection();
}
void Spr::PHIKEndEffectorIf::SetTargetLookat(Vec3d lookat){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetLookat(lookat);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetLookat(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetLookat();
}
void Spr::PHIKEndEffectorIf::SetTargetVelocity(Vec3d velocity){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetVelocity(velocity);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetVelocity(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetVelocity();
}
void Spr::PHIKEndEffectorIf::SetTargetAngularVelocity(Vec3d angVel){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetAngularVelocity(angVel);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetAngularVelocity(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetAngularVelocity();
}
void Spr::PHIKEndEffectorIf::EnableForceControl(bool enable){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->EnableForceControl(enable);
}
void Spr::PHIKEndEffectorIf::SetTargetForce(Vec3d force, Vec3d workingPoint){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetForce(force, workingPoint);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetForce(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetForce();
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetForceWorkingPoint(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetForceWorkingPoint();
}
void Spr::PHIKEndEffectorIf::EnableTorqueControl(bool enable){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->EnableTorqueControl(enable);
}
void Spr::PHIKEndEffectorIf::SetTargetTorque(Vec3d torque){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->SetTargetTorque(torque);
}
Vec3d Spr::PHIKEndEffectorIf::GetTargetTorque(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetTargetTorque();
}
Posed Spr::PHIKEndEffectorIf::GetSolidTempPose(){
	return	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->GetSolidTempPose();
}
void Spr::PHIKEndEffectorIf::ApplyExactState(){
	((PHIKEndEffector*)(Object*)(ObjectIf*)this)->ApplyExactState();
}
SPR_IFIMP1(PHIKActuator, SceneObject);
void Spr::PHIKActuatorIf::PrepareSolve(){
	((PHIKActuator*)(Object*)(ObjectIf*)this)->PrepareSolve();
}
void Spr::PHIKActuatorIf::ProceedSolve(){
	((PHIKActuator*)(Object*)(ObjectIf*)this)->ProceedSolve();
}
PTM::VVector< double > Spr::PHIKActuatorIf::GetRawSolution(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetRawSolution();
}
void Spr::PHIKActuatorIf::Move(){
	((PHIKActuator*)(Object*)(ObjectIf*)this)->Move();
}
void Spr::PHIKActuatorIf::ApplyExactState(bool reverse){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->ApplyExactState(reverse);
}
void Spr::PHIKActuatorIf::SetBias(float bias){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->SetBias(bias);
}
float Spr::PHIKActuatorIf::GetBias(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetBias();
}
void Spr::PHIKActuatorIf::SetPullbackRate(double pullbackRate){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->SetPullbackRate(pullbackRate);
}
double Spr::PHIKActuatorIf::GetPullbackRate(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetPullbackRate();
}
void Spr::PHIKActuatorIf::Enable(bool enable){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->Enable(enable);
}
bool Spr::PHIKActuatorIf::IsEnabled(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->IsEnabled();
}
int Spr::PHIKActuatorIf::NAncestors(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->NAncestors();
}
Spr::PHIKActuatorIf* Spr::PHIKActuatorIf::GetAncestor(int i){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetAncestor(i);
}
Spr::PHIKActuatorIf* Spr::PHIKActuatorIf::GetParent(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetParent();
}
int Spr::PHIKActuatorIf::NChildActuators(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->NChildActuators();
}
Spr::PHIKActuatorIf* Spr::PHIKActuatorIf::GetChildActuator(int i){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetChildActuator(i);
}
Spr::PHIKEndEffectorIf* Spr::PHIKActuatorIf::GetChildEndEffector(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetChildEndEffector();
}
Posed Spr::PHIKActuatorIf::GetSolidTempPose(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetSolidTempPose();
}
Posed Spr::PHIKActuatorIf::GetSolidPullbackPose(){
	return	((PHIKActuator*)(Object*)(ObjectIf*)this)->GetSolidPullbackPose();
}
SPR_IFIMP1(PHIKBallActuator, PHIKActuator);
void Spr::PHIKBallActuatorIf::SetJoint(Spr::PHBallJointIf* joint){
	return	((PHIKBallActuator*)(Object*)(ObjectIf*)this)->SetJoint(joint);
}
Spr::PHBallJointIf* Spr::PHIKBallActuatorIf::GetJoint(){
	return	((PHIKBallActuator*)(Object*)(ObjectIf*)this)->GetJoint();
}
void Spr::PHIKBallActuatorIf::SetJointTempOri(Quaterniond ori){
	return	((PHIKBallActuator*)(Object*)(ObjectIf*)this)->SetJointTempOri(ori);
}
void Spr::PHIKBallActuatorIf::SetPullbackTarget(Quaterniond ori){
	return	((PHIKBallActuator*)(Object*)(ObjectIf*)this)->SetPullbackTarget(ori);
}
Quaterniond Spr::PHIKBallActuatorIf::GetJointTempOri(){
	return	((PHIKBallActuator*)(Object*)(ObjectIf*)this)->GetJointTempOri();
}
Quaterniond Spr::PHIKBallActuatorIf::GetPullbackTarget(){
	return	((PHIKBallActuator*)(Object*)(ObjectIf*)this)->GetPullbackTarget();
}
SPR_IFIMP1(PHIKHingeActuator, PHIKActuator);
void Spr::PHIKHingeActuatorIf::SetJoint(Spr::PHHingeJointIf* joint){
	return	((PHIKHingeActuator*)(Object*)(ObjectIf*)this)->SetJoint(joint);
}
Spr::PHHingeJointIf* Spr::PHIKHingeActuatorIf::GetJoint(){
	return	((PHIKHingeActuator*)(Object*)(ObjectIf*)this)->GetJoint();
}
void Spr::PHIKHingeActuatorIf::SetJointTempAngle(double angle){
	return	((PHIKHingeActuator*)(Object*)(ObjectIf*)this)->SetJointTempAngle(angle);
}
void Spr::PHIKHingeActuatorIf::SetPullbackTarget(double angle){
	return	((PHIKHingeActuator*)(Object*)(ObjectIf*)this)->SetPullbackTarget(angle);
}
double Spr::PHIKHingeActuatorIf::GetJointTempAngle(){
	return	((PHIKHingeActuator*)(Object*)(ObjectIf*)this)->GetJointTempAngle();
}
double Spr::PHIKHingeActuatorIf::GetPullbackTarget(){
	return	((PHIKHingeActuator*)(Object*)(ObjectIf*)this)->GetPullbackTarget();
}
SPR_IFIMP1(PHIKSpringActuator, PHIKActuator);
void Spr::PHIKSpringActuatorIf::SetJoint(Spr::PHSpringIf* joint){
	return	((PHIKSpringActuator*)(Object*)(ObjectIf*)this)->SetJoint(joint);
}
Spr::PHSpringIf* Spr::PHIKSpringActuatorIf::GetJoint(){
	return	((PHIKSpringActuator*)(Object*)(ObjectIf*)this)->GetJoint();
}
void Spr::PHIKSpringActuatorIf::SetJointTempPose(Posed pose){
	return	((PHIKSpringActuator*)(Object*)(ObjectIf*)this)->SetJointTempPose(pose);
}
void Spr::PHIKSpringActuatorIf::SetPullbackTarget(Posed pose){
	return	((PHIKSpringActuator*)(Object*)(ObjectIf*)this)->SetPullbackTarget(pose);
}
Posed Spr::PHIKSpringActuatorIf::GetJointTempPose(){
	return	((PHIKSpringActuator*)(Object*)(ObjectIf*)this)->GetJointTempPose();
}
Posed Spr::PHIKSpringActuatorIf::GetPullbackTarget(){
	return	((PHIKSpringActuator*)(Object*)(ObjectIf*)this)->GetPullbackTarget();
}
SPR_IFIMP1(PHConstraint, SceneObject);
Spr::PHSolidIf* Spr::PHConstraintIf::GetSocketSolid(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetSocketSolid();
}
Spr::PHSolidIf* Spr::PHConstraintIf::GetPlugSolid(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetPlugSolid();
}
Spr::PHSceneIf* Spr::PHConstraintIf::GetScene()const{
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetScene();
}
void Spr::PHConstraintIf::Enable(bool bEnable){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->Enable(bEnable);
}
bool Spr::PHConstraintIf::IsEnabled(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->IsEnabled();
}
bool Spr::PHConstraintIf::IsArticulated(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->IsArticulated();
}
void Spr::PHConstraintIf::GetSocketPose(Posed& pose){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetSocketPose(pose);
}
void Spr::PHConstraintIf::SetSocketPose(const Posed& pose){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->SetSocketPose(pose);
}
void Spr::PHConstraintIf::GetPlugPose(Posed& pose){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetPlugPose(pose);
}
void Spr::PHConstraintIf::SetPlugPose(const Posed& pose){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->SetPlugPose(pose);
}
void Spr::PHConstraintIf::GetRelativePose(Posed& p){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetRelativePose(p);
}
Vec3d Spr::PHConstraintIf::GetRelativePoseR(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetRelativePoseR();
}
Quaterniond Spr::PHConstraintIf::GetRelativePoseQ(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetRelativePoseQ();
}
Quaternionf Spr::PHConstraintIf::GetAbsolutePoseQ(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetAbsolutePoseQ();
}
void Spr::PHConstraintIf::GetRelativeVelocity(Vec3d& v, Vec3d& w){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetRelativeVelocity(v, w);
}
void Spr::PHConstraintIf::GetConstraintForce(Vec3d& f, Vec3d& t){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->GetConstraintForce(f, t);
}
bool Spr::PHConstraintIf::IsYielded(){
	return	((PHConstraint*)(Object*)(ObjectIf*)this)->IsYielded();
}
void Spr::PHConstraintIf::UpdateState(){
	((PHConstraint*)(Object*)(ObjectIf*)this)->UpdateState();
}
SPR_IFIMP1(PHConstraints, SceneObject);
Spr::PHConstraintIf* Spr::PHConstraintsIf::FindBySolidPair(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs){
	return	((PHConstraints*)(Object*)(ObjectIf*)this)->FindBySolidPair(lhs, rhs);
}
Vec3d Spr::PHConstraintsIf::GetTotalForce(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs){
	return	((PHConstraints*)(Object*)(ObjectIf*)this)->GetTotalForce(lhs, rhs);
}
SPR_IFIMP1(PHContactPoint, PHConstraint);
Posed Spr::PHContactPointIf::GetPose(){
	return	((PHContactPoint*)(Object*)(ObjectIf*)this)->GetPose();
}
bool Spr::PHContactPointIf::IsStaticFriction(){
	return	((PHContactPoint*)(Object*)(ObjectIf*)this)->IsStaticFriction();
}
SPR_IFIMP1(PHJoint, PHConstraint);
void Spr::PHJointIf::SetMaxForce(double max){
	return	((PHJoint*)(Object*)(ObjectIf*)this)->SetMaxForce(max);
}
double Spr::PHJointIf::GetMaxForce(){
	return	((PHJoint*)(Object*)(ObjectIf*)this)->GetMaxForce();
}
SPR_IFIMP1(PH1DJoint, PHJoint);
PH1DJointLimitIf* Spr::PH1DJointIf::CreateLimit(const PH1DJointLimitDesc& desc){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->CreateLimit(desc);
}
PH1DJointMotorIf* Spr::PH1DJointIf::CreateMotor(const IfInfo* ii, const PH1DJointMotorDesc& desc){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->CreateMotor(ii, desc);
}
bool Spr::PH1DJointIf::AddMotor(PH1DJointMotorIf* m){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->AddMotor(m);
}
bool Spr::PH1DJointIf::RemoveMotor(int n){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->RemoveMotor(n);
}
bool Spr::PH1DJointIf::IsCyclic(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->IsCyclic();
}
void Spr::PH1DJointIf::SetCyclic(bool on){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetCyclic(on);
}
double Spr::PH1DJointIf::GetPosition(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetPosition();
}
double Spr::PH1DJointIf::GetVelocity(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetVelocity();
}
bool Spr::PH1DJointIf::HasLimit(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->HasLimit();
}
PH1DJointLimitIf* Spr::PH1DJointIf::GetLimit(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetLimit();
}
void Spr::PH1DJointIf::SetSpring(double spring){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetSpring(spring);
}
double Spr::PH1DJointIf::GetSpring(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetSpring();
}
void Spr::PH1DJointIf::SetDamper(double damper){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetDamper(damper);
}
double Spr::PH1DJointIf::GetDamper(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetDamper();
}
double Spr::PH1DJointIf::GetSecondDamper(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetSecondDamper();
}
void Spr::PH1DJointIf::SetSecondDamper(double input){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetSecondDamper(input);
}
void Spr::PH1DJointIf::SetTargetPosition(double targetPosition){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetTargetPosition(targetPosition);
}
double Spr::PH1DJointIf::GetTargetPosition(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetTargetPosition();
}
void Spr::PH1DJointIf::SetTargetVelocity(double v){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetTargetVelocity(v);
}
double Spr::PH1DJointIf::GetTargetVelocity(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetTargetVelocity();
}
void Spr::PH1DJointIf::SetOffsetForce(double dat){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetOffsetForce(dat);
}
double Spr::PH1DJointIf::GetOffsetForce(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetOffsetForce();
}
void Spr::PH1DJointIf::SetOffsetForceN(int n, double dat){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetOffsetForceN(n, dat);
}
double Spr::PH1DJointIf::GetOffsetForceN(int n){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetOffsetForceN(n);
}
double Spr::PH1DJointIf::GetYieldStress(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetYieldStress();
}
void Spr::PH1DJointIf::SetYieldStress(const double yS){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetYieldStress(yS);
}
double Spr::PH1DJointIf::GetHardnessRate(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetHardnessRate();
}
void Spr::PH1DJointIf::SetHardnessRate(const double hR){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetHardnessRate(hR);
}
void Spr::PH1DJointIf::SetSecondMoment(const double& sM){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->SetSecondMoment(sM);
}
double Spr::PH1DJointIf::GetSecondMoment(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetSecondMoment();
}
int Spr::PH1DJointIf::NMotors(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->NMotors();
}
PH1DJointMotorIf** Spr::PH1DJointIf::GetMotors(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetMotors();
}
double Spr::PH1DJointIf::GetMotorForce(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetMotorForce();
}
double Spr::PH1DJointIf::GetMotorForceN(int n){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetMotorForceN(n);
}
double Spr::PH1DJointIf::GetLimitForce(){
	return	((PH1DJoint*)(Object*)(ObjectIf*)this)->GetLimitForce();
}
SPR_IFIMP1(PHHingeJoint, PH1DJoint);
SPR_IFIMP1(PHSliderJoint, PH1DJoint);
SPR_IFIMP1(PHPath, SceneObject);
void Spr::PHPathIf::AddPoint(double s, const Posed& pose){
	return	((PHPath*)(Object*)(ObjectIf*)this)->AddPoint(s, pose);
}
void Spr::PHPathIf::SetLoop(bool bOnOff){
	return	((PHPath*)(Object*)(ObjectIf*)this)->SetLoop(bOnOff);
}
bool Spr::PHPathIf::IsLoop(){
	return	((PHPath*)(Object*)(ObjectIf*)this)->IsLoop();
}
SPR_IFIMP1(PHPathJoint, PH1DJoint);
void Spr::PHPathJointIf::SetPosition(double q){
	return	((PHPathJoint*)(Object*)(ObjectIf*)this)->SetPosition(q);
}
SPR_IFIMP1(PHGenericJoint, PH1DJoint);
void Spr::PHGenericJointIf::SetCallback(Spr::PHGenericJointCallback* cb){
	return	((PHGenericJoint*)(Object*)(ObjectIf*)this)->SetCallback(cb);
}
void Spr::PHGenericJointIf::SetParam(const std::string& name, double value){
	return	((PHGenericJoint*)(Object*)(ObjectIf*)this)->SetParam(name, value);
}
SPR_IFIMP1(PHBallJoint, PHJoint);
PHBallJointLimitIf* Spr::PHBallJointIf::CreateLimit(const IfInfo* ii, const PHBallJointLimitDesc& desc){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->CreateLimit(ii, desc);
}
Spr::PHBallJointMotorIf* Spr::PHBallJointIf::CreateMotor(const IfInfo* ii, const PHBallJointMotorDesc& desc){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->CreateMotor(ii, desc);
}
bool Spr::PHBallJointIf::AddMotor(Spr::PHBallJointMotorIf* m){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->AddMotor(m);
}
bool Spr::PHBallJointIf::RemoveMotor(int n){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->RemoveMotor(n);
}
Vec3d Spr::PHBallJointIf::GetAngle(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetAngle();
}
Quaterniond Spr::PHBallJointIf::GetPosition(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetPosition();
}
Vec3d Spr::PHBallJointIf::GetVelocity(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetVelocity();
}
bool Spr::PHBallJointIf::HasLimit(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->HasLimit();
}
PHBallJointLimitIf* Spr::PHBallJointIf::GetLimit(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetLimit();
}
void Spr::PHBallJointIf::SetSpring(double spring){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetSpring(spring);
}
double Spr::PHBallJointIf::GetSpring(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetSpring();
}
void Spr::PHBallJointIf::SetDamper(double damper){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetDamper(damper);
}
double Spr::PHBallJointIf::GetDamper(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetDamper();
}
Vec3d Spr::PHBallJointIf::GetSecondDamper(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetSecondDamper();
}
void Spr::PHBallJointIf::SetSecondDamper(Vec3d damper2){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetSecondDamper(damper2);
}
void Spr::PHBallJointIf::SetTargetPosition(Quaterniond p){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetTargetPosition(p);
}
Quaterniond Spr::PHBallJointIf::GetTargetPosition(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetTargetPosition();
}
void Spr::PHBallJointIf::SetTargetVelocity(Vec3d q){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetTargetVelocity(q);
}
Vec3d Spr::PHBallJointIf::GetTargetVelocity(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetTargetVelocity();
}
void Spr::PHBallJointIf::SetOffsetForce(Vec3d ofst){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetOffsetForce(ofst);
}
Vec3d Spr::PHBallJointIf::GetOffsetForce(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetOffsetForce();
}
void Spr::PHBallJointIf::SetOffsetForceN(int n, Vec3d ofst){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetOffsetForceN(n, ofst);
}
Vec3d Spr::PHBallJointIf::GetOffsetForceN(int n){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetOffsetForceN(n);
}
void Spr::PHBallJointIf::SetYieldStress(const double yS){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetYieldStress(yS);
}
double Spr::PHBallJointIf::GetYieldStress(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetYieldStress();
}
void Spr::PHBallJointIf::SetHardnessRate(const double hR){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetHardnessRate(hR);
}
double Spr::PHBallJointIf::GetHardnessRate(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetHardnessRate();
}
void Spr::PHBallJointIf::SetSecondMoment(const Vec3d m){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->SetSecondMoment(m);
}
Vec3d Spr::PHBallJointIf::GetSecondMoment(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetSecondMoment();
}
int Spr::PHBallJointIf::NMotors(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->NMotors();
}
Spr::PHBallJointMotorIf** Spr::PHBallJointIf::GetMotors(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetMotors();
}
Vec3d Spr::PHBallJointIf::GetMotorForce(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetMotorForce();
}
Vec3d Spr::PHBallJointIf::GetMotorForceN(int n){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetMotorForceN(n);
}
Vec3d Spr::PHBallJointIf::GetLimitForce(){
	return	((PHBallJoint*)(Object*)(ObjectIf*)this)->GetLimitForce();
}
SPR_IFIMP1(PHFixJoint, PHJoint);
SPR_IFIMP1(PHSpring, PHJoint);
void Spr::PHSpringIf::SetTargetPosition(const Vec3d& targetPosition){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetTargetPosition(targetPosition);
}
Vec3d Spr::PHSpringIf::GetTargetPosition(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetTargetPosition();
}
void Spr::PHSpringIf::SetTargetOrientation(const Quaterniond& targetOrientation){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetTargetOrientation(targetOrientation);
}
Quaterniond Spr::PHSpringIf::GetTargetOrientation(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetTargetOrientation();
}
void Spr::PHSpringIf::SetSpring(const Vec3d& spring){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetSpring(spring);
}
Vec3d Spr::PHSpringIf::GetSpring(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetSpring();
}
void Spr::PHSpringIf::SetDamper(const Vec3d& damper){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetDamper(damper);
}
Vec3d Spr::PHSpringIf::GetDamper(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetDamper();
}
void Spr::PHSpringIf::SetSecondDamper(const Vec3d& secondDamper){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetSecondDamper(secondDamper);
}
Vec3d Spr::PHSpringIf::GetSecondDamper(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetSecondDamper();
}
void Spr::PHSpringIf::SetSpringOri(const double spring){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetSpringOri(spring);
}
double Spr::PHSpringIf::GetSpringOri(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetSpringOri();
}
void Spr::PHSpringIf::SetDamperOri(const double damper){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetDamperOri(damper);
}
double Spr::PHSpringIf::GetDamperOri(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetDamperOri();
}
void Spr::PHSpringIf::SetSecondDamperOri(const double& secondDamperOri){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetSecondDamperOri(secondDamperOri);
}
double Spr::PHSpringIf::GetSecondDamperOri(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetSecondDamperOri();
}
void Spr::PHSpringIf::SetYieldStress(const double& yieldStress){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetYieldStress(yieldStress);
}
double Spr::PHSpringIf::GetYieldStress(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetYieldStress();
}
void Spr::PHSpringIf::SetHardnessRate(const double& hardnessRate){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetHardnessRate(hardnessRate);
}
double Spr::PHSpringIf::GetHardnessRate(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetHardnessRate();
}
void Spr::PHSpringIf::SetSecondMoment(const Vec3d& secondMoment){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetSecondMoment(secondMoment);
}
Vec3d Spr::PHSpringIf::GetSecondMoment(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetSecondMoment();
}
Vec6d Spr::PHSpringIf::GetMotorForce(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetMotorForce();
}
void Spr::PHSpringIf::SetOffsetForce(const Vec6d& offsetForce){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetOffsetForce(offsetForce);
}
void Spr::PHSpringIf::SetTargetVelocity(const Vec6d& targetVelocity){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->SetTargetVelocity(targetVelocity);
}
Vec6d Spr::PHSpringIf::GetTargetVelocity(){
	return	((PHSpring*)(Object*)(ObjectIf*)this)->GetTargetVelocity();
}
SPR_IFIMP1(PHMate, PHJoint);
SPR_IFIMP1(PHPointToPointMate, PHMate);
SPR_IFIMP1(PHPointToLineMate, PHMate);
SPR_IFIMP1(PHPointToPlaneMate, PHMate);
void Spr::PHPointToPlaneMateIf::SetRange(Vec2d range){
	return	((PHPointToPlaneMate*)(Object*)(ObjectIf*)this)->SetRange(range);
}
void Spr::PHPointToPlaneMateIf::GetRange(Vec2d& range){
	return	((PHPointToPlaneMate*)(Object*)(ObjectIf*)this)->GetRange(range);
}
SPR_IFIMP1(PHLineToLineMate, PHMate);
SPR_IFIMP1(PHPlaneToPlaneMate, PHMate);
SPR_IFIMP1(PHTreeNode, SceneObject);
void Spr::PHTreeNodeIf::Enable(bool bEnable){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->Enable(bEnable);
}
bool Spr::PHTreeNodeIf::IsEnabled(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->IsEnabled();
}
int Spr::PHTreeNodeIf::NChildren(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->NChildren();
}
Spr::PHTreeNodeIf* Spr::PHTreeNodeIf::GetParentNode(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetParentNode();
}
Spr::PHTreeNodeIf* Spr::PHTreeNodeIf::GetChildNode(int i){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetChildNode(i);
}
Spr::PHRootNodeIf* Spr::PHTreeNodeIf::GetRootNode(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetRootNode();
}
Spr::PHSolidIf* Spr::PHTreeNodeIf::GetSolid(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetSolid();
}
Spr::SpatialMatrix Spr::PHTreeNodeIf::GetI(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetI();
}
Spr::SpatialVector Spr::PHTreeNodeIf::GetIc(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetIc();
}
Spr::SpatialVector Spr::PHTreeNodeIf::GetC(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetC();
}
Spr::SpatialVector Spr::PHTreeNodeIf::GetZ(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetZ();
}
Spr::SpatialMatrix Spr::PHTreeNodeIf::GetXcp_mat(){
	return	((PHTreeNode*)(Object*)(ObjectIf*)this)->GetXcp_mat();
}
void Spr::PHTreeNodeIf::Setup(){
	((PHTreeNode*)(Object*)(ObjectIf*)this)->Setup();
}
void Spr::PHTreeNodeIf::InitArticulatedInertia(){
	((PHTreeNode*)(Object*)(ObjectIf*)this)->InitArticulatedInertia();
}
void Spr::PHTreeNodeIf::CompArticulatedInertia(){
	((PHTreeNode*)(Object*)(ObjectIf*)this)->CompArticulatedInertia();
}
SPR_IFIMP1(PHRootNode, PHTreeNode);
bool Spr::PHRootNodeIf::IsUseNextPose(){
	return	((PHRootNode*)(Object*)(ObjectIf*)this)->IsUseNextPose();
}
void Spr::PHRootNodeIf::SetUseNextPose(bool bOn){
	return	((PHRootNode*)(Object*)(ObjectIf*)this)->SetUseNextPose(bOn);
}
void Spr::PHRootNodeIf::SetNextPose(const Posed& p){
	return	((PHRootNode*)(Object*)(ObjectIf*)this)->SetNextPose(p);
}
SPR_IFIMP1(PHTreeNode1D, PHTreeNode);
SPR_IFIMP1(PHHingeJointNode, PHTreeNode1D);
SPR_IFIMP1(PHSliderJointNode, PHTreeNode1D);
SPR_IFIMP1(PHPathJointNode, PHTreeNode1D);
SPR_IFIMP1(PHGenericJointNode, PHTreeNode1D);
SPR_IFIMP1(PHBallJointNode, PHTreeNode);
SPR_IFIMP1(PHFixJointNode, PHTreeNode);
SPR_IFIMP1(PHGear, SceneObject);
void Spr::PHGearIf::Enable(bool bEnable){
	return	((PHGear*)(Object*)(ObjectIf*)this)->Enable(bEnable);
}
bool Spr::PHGearIf::IsEnabled(){
	return	((PHGear*)(Object*)(ObjectIf*)this)->IsEnabled();
}
void Spr::PHGearIf::SetRatio(double ratio){
	return	((PHGear*)(Object*)(ObjectIf*)this)->SetRatio(ratio);
}
double Spr::PHGearIf::GetRatio(){
	return	((PHGear*)(Object*)(ObjectIf*)this)->GetRatio();
}
void Spr::PHGearIf::SetOffset(double offset){
	return	((PHGear*)(Object*)(ObjectIf*)this)->SetOffset(offset);
}
double Spr::PHGearIf::GetOffset(){
	return	((PHGear*)(Object*)(ObjectIf*)this)->GetOffset();
}
void Spr::PHGearIf::SetMode(int mode){
	return	((PHGear*)(Object*)(ObjectIf*)this)->SetMode(mode);
}
int Spr::PHGearIf::GetMode(){
	return	((PHGear*)(Object*)(ObjectIf*)this)->GetMode();
}
SPR_IFIMP1(PH1DJointLimit, SceneObject);
void Spr::PH1DJointLimitIf::SetRange(Vec2d range){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->SetRange(range);
}
void Spr::PH1DJointLimitIf::GetRange(Vec2d& range){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->GetRange(range);
}
void Spr::PH1DJointLimitIf::SetSpring(double spring){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->SetSpring(spring);
}
double Spr::PH1DJointLimitIf::GetSpring(){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->GetSpring();
}
void Spr::PH1DJointLimitIf::SetDamper(double damper){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->SetDamper(damper);
}
double Spr::PH1DJointLimitIf::GetDamper(){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->GetDamper();
}
bool Spr::PH1DJointLimitIf::IsOnLimit(){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->IsOnLimit();
}
void Spr::PH1DJointLimitIf::Enable(bool b){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->Enable(b);
}
bool Spr::PH1DJointLimitIf::IsEnabled(){
	return	((PH1DJointLimit*)(Object*)(ObjectIf*)this)->IsEnabled();
}
SPR_IFIMP1(PHBallJointLimit, SceneObject);
void Spr::PHBallJointLimitIf::SetSpring(double rSpring){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->SetSpring(rSpring);
}
double Spr::PHBallJointLimitIf::GetSpring(){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->GetSpring();
}
void Spr::PHBallJointLimitIf::SetDamper(double rDamper){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->SetDamper(rDamper);
}
double Spr::PHBallJointLimitIf::GetDamper(){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->GetDamper();
}
void Spr::PHBallJointLimitIf::SetLimitDir(Vec3d limDir){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->SetLimitDir(limDir);
}
Vec3d Spr::PHBallJointLimitIf::GetLimitDir(){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->GetLimitDir();
}
bool Spr::PHBallJointLimitIf::IsOnLimit(){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->IsOnLimit();
}
void Spr::PHBallJointLimitIf::Enable(bool b){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->Enable(b);
}
bool Spr::PHBallJointLimitIf::IsEnabled(){
	return	((PHBallJointLimit*)(Object*)(ObjectIf*)this)->IsEnabled();
}
SPR_IFIMP1(PHBallJointConeLimit, PHBallJointLimit);
void Spr::PHBallJointConeLimitIf::SetSwingRange(Vec2d range){
	return	((PHBallJointConeLimit*)(Object*)(ObjectIf*)this)->SetSwingRange(range);
}
void Spr::PHBallJointConeLimitIf::GetSwingRange(Vec2d& range){
	return	((PHBallJointConeLimit*)(Object*)(ObjectIf*)this)->GetSwingRange(range);
}
void Spr::PHBallJointConeLimitIf::SetSwingDirRange(Vec2d range){
	return	((PHBallJointConeLimit*)(Object*)(ObjectIf*)this)->SetSwingDirRange(range);
}
void Spr::PHBallJointConeLimitIf::GetSwingDirRange(Vec2d& range){
	return	((PHBallJointConeLimit*)(Object*)(ObjectIf*)this)->GetSwingDirRange(range);
}
void Spr::PHBallJointConeLimitIf::SetTwistRange(Vec2d range){
	return	((PHBallJointConeLimit*)(Object*)(ObjectIf*)this)->SetTwistRange(range);
}
void Spr::PHBallJointConeLimitIf::GetTwistRange(Vec2d& range){
	return	((PHBallJointConeLimit*)(Object*)(ObjectIf*)this)->GetTwistRange(range);
}
SPR_IFIMP1(PHBallJointSplineLimit, PHBallJointLimit);
void Spr::PHBallJointSplineLimitIf::AddNode(Spr::PHSplineLimitNode node){
	return	((PHBallJointSplineLimit*)(Object*)(ObjectIf*)this)->AddNode(node);
}
void Spr::PHBallJointSplineLimitIf::AddNode(double S, double SD, double dS, double dSD, double tMin, double tMax){
	return	((PHBallJointSplineLimit*)(Object*)(ObjectIf*)this)->AddNode(S, SD, dS, dSD, tMin, tMax);
}
void Spr::PHBallJointSplineLimitIf::AddNode(Spr::PHSplineLimitNode node, int pos){
	return	((PHBallJointSplineLimit*)(Object*)(ObjectIf*)this)->AddNode(node, pos);
}
void Spr::PHBallJointSplineLimitIf::AddNode(double S, double SD, double dS, double dSD, double tMin, double tMax, int pos){
	return	((PHBallJointSplineLimit*)(Object*)(ObjectIf*)this)->AddNode(S, SD, dS, dSD, tMin, tMax, pos);
}
void Spr::PHBallJointSplineLimitIf::SetPoleTwistRange(Vec2d range){
	return	((PHBallJointSplineLimit*)(Object*)(ObjectIf*)this)->SetPoleTwistRange(range);
}
void Spr::PHBallJointSplineLimitIf::GetPoleTwistRange(Vec2d& range){
	return	((PHBallJointSplineLimit*)(Object*)(ObjectIf*)this)->GetPoleTwistRange(range);
}
SPR_IFIMP1(PHBallJointIndependentLimit, PHBallJointLimit);
bool Spr::PHBallJointIndependentLimitIf::IsOnLimit(){
	return	((PHBallJointIndependentLimit*)(Object*)(ObjectIf*)this)->IsOnLimit();
}
int Spr::PHBallJointIndependentLimitIf::AxesEnabled(){
	return	((PHBallJointIndependentLimit*)(Object*)(ObjectIf*)this)->AxesEnabled();
}
void Spr::PHBallJointIndependentLimitIf::SetLimitRangeN(int n, Vec2d range){
	return	((PHBallJointIndependentLimit*)(Object*)(ObjectIf*)this)->SetLimitRangeN(n, range);
}
void Spr::PHBallJointIndependentLimitIf::GetLimitRangeN(int n, Vec2d& range){
	return	((PHBallJointIndependentLimit*)(Object*)(ObjectIf*)this)->GetLimitRangeN(n, range);
}
SPR_IFIMP1(PH1DJointMotor, SceneObject);
SPR_IFIMP1(PH1DJointNonLinearMotor, PH1DJointMotor);
void Spr::PH1DJointNonLinearMotorIf::SetSpring(Spr::PH1DJointNonLinearMotorDesc::FunctionMode m, void* param){
	return	((PH1DJointNonLinearMotor*)(Object*)(ObjectIf*)this)->SetSpring(m, param);
}
void Spr::PH1DJointNonLinearMotorIf::SetDamper(Spr::PH1DJointNonLinearMotorDesc::FunctionMode m, void* param){
	return	((PH1DJointNonLinearMotor*)(Object*)(ObjectIf*)this)->SetDamper(m, param);
}
void Spr::PH1DJointNonLinearMotorIf::SetSpringDamper(Spr::PH1DJointNonLinearMotorDesc::FunctionMode smode, Spr::PH1DJointNonLinearMotorDesc::FunctionMode dmode, void* sparam, void* dparam){
	return	((PH1DJointNonLinearMotor*)(Object*)(ObjectIf*)this)->SetSpringDamper(smode, dmode, sparam, dparam);
}
SPR_IFIMP1(PHHuman1DJointResistance, PH1DJointNonLinearMotor);
double Spr::PHHuman1DJointResistanceIf::GetCurrentResistance(){
	return	((PHHuman1DJointResistance*)(Object*)(ObjectIf*)this)->GetCurrentResistance();
}
SPR_IFIMP1(PHBallJointMotor, SceneObject);
SPR_IFIMP1(PHBallJointNonLinearMotor, PHBallJointMotor);
void Spr::PHBallJointNonLinearMotorIf::SetFuncFromDatabaseN(int n, int i, int j, void* sparam, void* dparam){
	return	((PHBallJointNonLinearMotor*)(Object*)(ObjectIf*)this)->SetFuncFromDatabaseN(n, i, j, sparam, dparam);
}
SPR_IFIMP1(PHHumanBallJointResistance, PHBallJointNonLinearMotor);
Vec3d Spr::PHHumanBallJointResistanceIf::GetCurrentResistance(){
	return	((PHHumanBallJointResistance*)(Object*)(ObjectIf*)this)->GetCurrentResistance();
}
SPR_IFIMP1(PHSpringMotor, SceneObject);
SPR_IFIMP1(PHOpObj, SceneObject);
bool Spr::PHOpObjIf::initialPHOpObj(Vec3f* vts, int vtsNum, float pSize){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->initialPHOpObj(vts, vtsNum, pSize);
}
void Spr::PHOpObjIf::SetGravity(bool bOn){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetGravity(bOn);
}
void Spr::PHOpObjIf::SimpleSimulationStep(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->SimpleSimulationStep();
}
void Spr::PHOpObjIf::AddVertextoLocalBuffer(Vec3f v){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->AddVertextoLocalBuffer(v);
}
bool Spr::PHOpObjIf::InitialObjUsingLocalBuffer(float pSize){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->InitialObjUsingLocalBuffer(pSize);
}
void Spr::PHOpObjIf::positionPredict(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->positionPredict();
}
void Spr::PHOpObjIf::groupStep(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->groupStep();
}
void Spr::PHOpObjIf::integrationStep(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->integrationStep();
}
void Spr::PHOpObjIf::ReducedPositionProject(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->ReducedPositionProject();
}
void Spr::PHOpObjIf::positionProject(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->positionProject();
}
void Spr::PHOpObjIf::SetDefaultLinkNum(int linkNum){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetDefaultLinkNum(linkNum);
}
void Spr::PHOpObjIf::BuildBlendWeight(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->BuildBlendWeight();
}
void Spr::PHOpObjIf::buildGroupCenter(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->buildGroupCenter();
}
int Spr::PHOpObjIf::GetVertexNum(){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetVertexNum();
}
Vec3f Spr::PHOpObjIf::GetVertex(int vi){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetVertex(vi);
}
Spr::ObjectIf* Spr::PHOpObjIf::GetOpParticle(int pi){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetOpParticle(pi);
}
Spr::ObjectIf* Spr::PHOpObjIf::GetOpGroup(int gi){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetOpGroup(gi);
}
int Spr::PHOpObjIf::GetobjVtoPmap(int vi){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetobjVtoPmap(vi);
}
float Spr::PHOpObjIf::GetVtxBlendWeight(int Vtxi, int Grpi){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetVtxBlendWeight(Vtxi, Grpi);
}
void Spr::PHOpObjIf::SetVelocityDamping(float vd){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetVelocityDamping(vd);
}
float Spr::PHOpObjIf::GetVelocityDamping(){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetVelocityDamping();
}
float Spr::PHOpObjIf::GetBoundLength(){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetBoundLength();
}
void Spr::PHOpObjIf::SetBound(float b){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetBound(b);
}
void Spr::PHOpObjIf::SetTimeStep(float t){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetTimeStep(t);
}
float Spr::PHOpObjIf::GetTimeStep(){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetTimeStep();
}
float Spr::PHOpObjIf::GetObjBeta(){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetObjBeta();
}
void Spr::PHOpObjIf::SetObjBeta(float beta){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetObjBeta(beta);
}
void Spr::PHOpObjIf::SetObjAlpha(float alpha){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetObjAlpha(alpha);
}
float Spr::PHOpObjIf::GetObjAlpha(){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetObjAlpha();
}
bool Spr::PHOpObjIf::GetObjDstConstraint(){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetObjDstConstraint();
}
void Spr::PHOpObjIf::SetObjDstConstraint(bool d){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetObjDstConstraint(d);
}
void Spr::PHOpObjIf::SetObjItrTime(int itrT){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->SetObjItrTime(itrT);
}
int Spr::PHOpObjIf::GetObjItrTime(){
	return	((PHOpObj*)(Object*)(ObjectIf*)this)->GetObjItrTime();
}
void Spr::PHOpObjIf::StoreOrigPose(){
	((PHOpObj*)(Object*)(ObjectIf*)this)->StoreOrigPose();
}
SPR_IFIMP1(PHOpParticle, Object);
int Spr::PHOpParticleIf::GetOpPtclVtxId(int vi){
	return	((PHOpParticle*)(Object*)(ObjectIf*)this)->GetOpPtclVtxId(vi);
}
int Spr::PHOpParticleIf::GetinGrpListNum(){
	return	((PHOpParticle*)(Object*)(ObjectIf*)this)->GetinGrpListNum();
}
int Spr::PHOpParticleIf::GetinGrpList(int gi){
	return	((PHOpParticle*)(Object*)(ObjectIf*)this)->GetinGrpList(gi);
}
float Spr::PHOpParticleIf::GetVtxDisWeight(int vi){
	return	((PHOpParticle*)(Object*)(ObjectIf*)this)->GetVtxDisWeight(vi);
}
Spr::PHOpParticleDesc* Spr::PHOpParticleIf::GetParticleDesc(){
	return	((PHOpParticle*)(Object*)(ObjectIf*)this)->GetParticleDesc();
}
SPR_IFIMP1(PHOpGroup, Object);
int Spr::PHOpGroupIf::GetGrpInPtclNum(){
	return	((PHOpGroup*)(Object*)(ObjectIf*)this)->GetGrpInPtclNum();
}
int Spr::PHOpGroupIf::GetGrpInPtcl(int gi){
	return	((PHOpGroup*)(Object*)(ObjectIf*)this)->GetGrpInPtcl(gi);
}
Spr::PHOpGroupDesc* Spr::PHOpGroupIf::GetGroupDesc(){
	return	((PHOpGroup*)(Object*)(ObjectIf*)this)->GetGroupDesc();
}
SPR_IFIMP1(PHOpHapticController, SceneObject);
void Spr::PHOpHapticControllerIf::SetHCForceReady(bool flag){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->SetHCForceReady(flag);
}
bool Spr::PHOpHapticControllerIf::GetHCForceReady(){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->GetHCForceReady();
}
bool Spr::PHOpHapticControllerIf::CheckProxyState(){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->CheckProxyState();
}
void Spr::PHOpHapticControllerIf::AddColliedPtcl(int pIndex, int objindex, Vec3f ctcPos){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->AddColliedPtcl(pIndex, objindex, ctcPos);
}
bool Spr::PHOpHapticControllerIf::BeginLogForce(){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->BeginLogForce();
}
void Spr::PHOpHapticControllerIf::EndLogForce(){
	((PHOpHapticController*)(Object*)(ObjectIf*)this)->EndLogForce();
}
void Spr::PHOpHapticControllerIf::setC_ObstacleRadius(float r){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->setC_ObstacleRadius(r);
}
Vec3f Spr::PHOpHapticControllerIf::GetUserPos(){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->GetUserPos();
}
Vec3f Spr::PHOpHapticControllerIf::GetHCPosition(){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->GetHCPosition();
}
void Spr::PHOpHapticControllerIf::SetHCPosition(Vec3f pos){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->SetHCPosition(pos);
}
void Spr::PHOpHapticControllerIf::SetHCPose(Posef pose){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->SetHCPose(pose);
}
Posef Spr::PHOpHapticControllerIf::GetHCPose(){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->GetHCPose();
}
float Spr::PHOpHapticControllerIf::GetC_ObstacleRadius(){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->GetC_ObstacleRadius();
}
Vec3f Spr::PHOpHapticControllerIf::GetCurrentOutputForce(){
	return	((PHOpHapticController*)(Object*)(ObjectIf*)this)->GetCurrentOutputForce();
}
SPR_IFIMP1(PHOpHapticRenderer, SceneObject);
void Spr::PHOpHapticRendererIf::SetRigid(bool set){
	return	((PHOpHapticRenderer*)(Object*)(ObjectIf*)this)->SetRigid(set);
}
bool Spr::PHOpHapticRendererIf::IsRigid(){
	return	((PHOpHapticRenderer*)(Object*)(ObjectIf*)this)->IsRigid();
}
SPR_IFIMP1(PHOpAnimation, SceneObject);
void Spr::PHOpAnimationIf::AddAnimationP(int objIndex, int pIndex, Vec3f start, Vec3f end, float totalTime){
	return	((PHOpAnimation*)(Object*)(ObjectIf*)this)->AddAnimationP(objIndex, pIndex, start, end, totalTime);
}
void Spr::PHOpAnimationIf::AddAnimationP(int objIndex, int pIndex, Vec3f force, float totalTime){
	return	((PHOpAnimation*)(Object*)(ObjectIf*)this)->AddAnimationP(objIndex, pIndex, force, totalTime);
}
void Spr::PHOpAnimationIf::AnimationStep(void* opEngine){
	return	((PHOpAnimation*)(Object*)(ObjectIf*)this)->AnimationStep(opEngine);
}
void Spr::PHOpAnimationIf::AnimationIntergration(void* opEngine){
	return	((PHOpAnimation*)(Object*)(ObjectIf*)this)->AnimationIntergration(opEngine);
}
SPR_IFIMP1(PHOpSpHashColliAgent, Object);
void Spr::PHOpSpHashColliAgentIf::SetCollisionCstrStiffness(float alpha){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->SetCollisionCstrStiffness(alpha);
}
float Spr::PHOpSpHashColliAgentIf::GetCollisionCstrStiffness(){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->GetCollisionCstrStiffness();
}
void Spr::PHOpSpHashColliAgentIf::EnableCollisionDetection(bool able){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->EnableCollisionDetection(able);
}
void Spr::PHOpSpHashColliAgentIf::Initial(float cellSize, Spr::CDBounds bounds){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->Initial(cellSize, bounds);
}
void Spr::PHOpSpHashColliAgentIf::OpCollisionProcedure(int myTimeStamp){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->OpCollisionProcedure(myTimeStamp);
}
void Spr::PHOpSpHashColliAgentIf::OpCollisionProcedure(){
	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->OpCollisionProcedure();
}
void Spr::PHOpSpHashColliAgentIf::OpDirCollision(){
	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->OpDirCollision();
}
void Spr::PHOpSpHashColliAgentIf::AddContactPlane(Vec3f planeP, Vec3f planeN){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->AddContactPlane(planeP, planeN);
}
bool Spr::PHOpSpHashColliAgentIf::IsCollisionEnabled(){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->IsCollisionEnabled();
}
void Spr::PHOpSpHashColliAgentIf::SetIsDirCollision(bool flag){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->SetIsDirCollision(flag);
}
bool Spr::PHOpSpHashColliAgentIf::GetIsDirCollision(){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->GetIsDirCollision();
}
void Spr::PHOpSpHashColliAgentIf::SetDebugMode(bool flag){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->SetDebugMode(flag);
}
bool Spr::PHOpSpHashColliAgentIf::GetDebugMode(){
	return	((PHOpSpHashColliAgent*)(Object*)(ObjectIf*)this)->GetDebugMode();
}
SPR_IFIMP1(PHRay, SceneObject);
Vec3d Spr::PHRayIf::GetOrigin(){
	return	((PHRay*)(Object*)(ObjectIf*)this)->GetOrigin();
}
void Spr::PHRayIf::SetOrigin(const Vec3d& ori){
	return	((PHRay*)(Object*)(ObjectIf*)this)->SetOrigin(ori);
}
Vec3d Spr::PHRayIf::GetDirection(){
	return	((PHRay*)(Object*)(ObjectIf*)this)->GetDirection();
}
void Spr::PHRayIf::SetDirection(const Vec3d& dir){
	return	((PHRay*)(Object*)(ObjectIf*)this)->SetDirection(dir);
}
void Spr::PHRayIf::Apply(){
	((PHRay*)(Object*)(ObjectIf*)this)->Apply();
}
int Spr::PHRayIf::NHits(){
	return	((PHRay*)(Object*)(ObjectIf*)this)->NHits();
}
Spr::PHRaycastHit* Spr::PHRayIf::GetHits(){
	return	((PHRay*)(Object*)(ObjectIf*)this)->GetHits();
}
Spr::PHRaycastHit* Spr::PHRayIf::GetNearest(){
	return	((PHRay*)(Object*)(ObjectIf*)this)->GetNearest();
}
Spr::PHRaycastHit* Spr::PHRayIf::GetDynamicalNearest(){
	return	((PHRay*)(Object*)(ObjectIf*)this)->GetDynamicalNearest();
}
SPR_IFIMP1(PHScene, Scene);
Spr::PHSdkIf* Spr::PHSceneIf::GetSdk(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSdk();
}
Spr::PHSolidIf* Spr::PHSceneIf::CreateSolid(const PHSolidDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateSolid(desc);
}
int Spr::PHSceneIf::NSolids()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NSolids();
}
Spr::PHSolidIf* Spr::PHSceneIf::GetSolid(int idx){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSolid(idx);
}
int Spr::PHSceneIf::GetSolidIndex(Spr::PHSolidIf* s){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSolidIndex(s);
}
Spr::PHSolidIf** Spr::PHSceneIf::GetSolids(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSolids();
}
int Spr::PHSceneIf::NSolidPairs()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NSolidPairs();
}
PHSolidPairForLCPIf* Spr::PHSceneIf::GetSolidPair(int i, int j){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSolidPair(i, j);
}
PHSolidPairForLCPIf* Spr::PHSceneIf::GetSolidPair(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs, bool& bSwap){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSolidPair(lhs, rhs, bSwap);
}
void Spr::PHSceneIf::SetContactMode(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs, Spr::PHSceneDesc::ContactMode mode){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetContactMode(lhs, rhs, mode);
}
void Spr::PHSceneIf::SetContactMode(Spr::PHSolidIf** group, size_t length, Spr::PHSceneDesc::ContactMode mode){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetContactMode(group, length, mode);
}
void Spr::PHSceneIf::SetContactMode(Spr::PHSolidIf* solid, Spr::PHSceneDesc::ContactMode mode){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetContactMode(solid, mode);
}
void Spr::PHSceneIf::SetContactMode(Spr::PHSceneDesc::ContactMode mode){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetContactMode(mode);
}
Spr::PHJointIf* Spr::PHSceneIf::CreateJoint(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs, const IfInfo* ii, const Spr::PHJointDesc& desc){
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
int Spr::PHSceneIf::NFemMeshes()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NFemMeshes();
}
Spr::PHFemMeshIf* Spr::PHSceneIf::GetFemMesh(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetFemMesh(i);
}
int Spr::PHSceneIf::NFemMeshesNew()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NFemMeshesNew();
}
Spr::PHFemMeshNewIf* Spr::PHSceneIf::GetFemMeshNew(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetFemMeshNew(i);
}
Spr::PHRootNodeIf* Spr::PHSceneIf::CreateRootNode(Spr::PHSolidIf* root, const Spr::PHRootNodeDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateRootNode(root, desc);
}
int Spr::PHSceneIf::NRootNodes()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->NRootNodes();
}
Spr::PHRootNodeIf* Spr::PHSceneIf::GetRootNode(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetRootNode(i);
}
Spr::PHTreeNodeIf* Spr::PHSceneIf::CreateTreeNode(Spr::PHTreeNodeIf* parent, Spr::PHSolidIf* child, const Spr::PHTreeNodeDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateTreeNode(parent, child, desc);
}
Spr::PHRootNodeIf* Spr::PHSceneIf::CreateTreeNodes(Spr::PHSolidIf* solid){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateTreeNodes(solid);
}
Spr::PHGearIf* Spr::PHSceneIf::CreateGear(Spr::PH1DJointIf* lhs, Spr::PH1DJointIf* rhs, const Spr::PHGearDesc& desc){
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
Spr::PHRayIf* Spr::PHSceneIf::CreateRay(const Spr::PHRayDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateRay(desc);
}
int Spr::PHSceneIf::NRays(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->NRays();
}
Spr::PHRayIf* Spr::PHSceneIf::GetRay(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetRay(i);
}
bool Spr::PHSceneIf::SetPosesOfJointedSolids(const Spr::PHSolidIf* base){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetPosesOfJointedSolids(base);
}
Spr::PHIKActuatorIf* Spr::PHSceneIf::CreateIKActuator(const IfInfo* ii, const Spr::PHIKActuatorDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateIKActuator(ii, desc);
}
int Spr::PHSceneIf::NIKActuators(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->NIKActuators();
}
Spr::PHIKActuatorIf* Spr::PHSceneIf::GetIKActuator(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetIKActuator(i);
}
Spr::PHIKEndEffectorIf* Spr::PHSceneIf::CreateIKEndEffector(const Spr::PHIKEndEffectorDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateIKEndEffector(desc);
}
int Spr::PHSceneIf::NIKEndEffectors(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->NIKEndEffectors();
}
Spr::PHIKEndEffectorIf* Spr::PHSceneIf::GetIKEndEffector(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetIKEndEffector(i);
}
int Spr::PHSceneIf::NSkeletons(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->NSkeletons();
}
PHSkeletonIf* Spr::PHSceneIf::GetSkeleton(int i){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetSkeleton(i);
}
PHSkeletonIf* Spr::PHSceneIf::CreateSkeleton(const PHSkeletonDesc& desc){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateSkeleton(desc);
}
double Spr::PHSceneIf::GetTimeStep()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetTimeStep();
}
double Spr::PHSceneIf::GetTimeStepInv()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetTimeStepInv();
}
void Spr::PHSceneIf::SetTimeStep(double dt){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetTimeStep(dt);
}
double Spr::PHSceneIf::GetHapticTimeStep()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetHapticTimeStep();
}
void Spr::PHSceneIf::SetHapticTimeStep(double dt){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetHapticTimeStep(dt);
}
unsigned int Spr::PHSceneIf::GetCount()const{
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetCount();
}
void Spr::PHSceneIf::SetCount(unsigned int count){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetCount(count);
}
void Spr::PHSceneIf::SetGravity(const Vec3d& accel){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetGravity(accel);
}
Vec3d Spr::PHSceneIf::GetGravity(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetGravity();
}
void Spr::PHSceneIf::SetAirResistanceRateForVelocity(double rate){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetAirResistanceRateForVelocity(rate);
}
void Spr::PHSceneIf::SetAirResistanceRateForAngularVelocity(double rate){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetAirResistanceRateForAngularVelocity(rate);
}
double Spr::PHSceneIf::GetAirResistanceRateForVelocity(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetAirResistanceRateForVelocity();
}
double Spr::PHSceneIf::GetAirResistanceRateForAngularVelocity(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetAirResistanceRateForAngularVelocity();
}
void Spr::PHSceneIf::SetContactTolerance(double tol){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetContactTolerance(tol);
}
double Spr::PHSceneIf::GetContactTolerance(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetContactTolerance();
}
void Spr::PHSceneIf::SetImpactThreshold(double vth){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetImpactThreshold(vth);
}
double Spr::PHSceneIf::GetImpactThreshold(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetImpactThreshold();
}
void Spr::PHSceneIf::SetFrictionThreshold(double vth){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetFrictionThreshold(vth);
}
double Spr::PHSceneIf::GetFrictionThreshold(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetFrictionThreshold();
}
void Spr::PHSceneIf::SetMaxVelocity(double vmax){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetMaxVelocity(vmax);
}
double Spr::PHSceneIf::GetMaxVelocity(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetMaxVelocity();
}
void Spr::PHSceneIf::SetMaxAngularVelocity(double wmax){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetMaxAngularVelocity(wmax);
}
double Spr::PHSceneIf::GetMaxAngularVelocity(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetMaxAngularVelocity();
}
void Spr::PHSceneIf::SetMaxForce(double fmax){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetMaxForce(fmax);
}
double Spr::PHSceneIf::GetMaxForce(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetMaxForce();
}
void Spr::PHSceneIf::SetMaxMoment(double tmax){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetMaxMoment(tmax);
}
double Spr::PHSceneIf::GetMaxMoment(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetMaxMoment();
}
void Spr::PHSceneIf::SetMaxDeltaPosition(double dpmax){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetMaxDeltaPosition(dpmax);
}
double Spr::PHSceneIf::GetMaxDeltaPosition(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetMaxDeltaPosition();
}
void Spr::PHSceneIf::SetMaxDeltaOrientation(double dqmax){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetMaxDeltaOrientation(dqmax);
}
double Spr::PHSceneIf::GetMaxDeltaOrientation(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetMaxDeltaOrientation();
}
int Spr::PHSceneIf::GetLCPSolver(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetLCPSolver();
}
void Spr::PHSceneIf::SetLCPSolver(int method){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetLCPSolver(method);
}
int Spr::PHSceneIf::GetNumIteration(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetNumIteration();
}
void Spr::PHSceneIf::SetNumIteration(int n){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetNumIteration(n);
}
void Spr::PHSceneIf::SetStateMode(bool bConstraints){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetStateMode(bConstraints);
}
void Spr::PHSceneIf::EnableContactDetection(bool enable){
	return	((PHScene*)(Object*)(ObjectIf*)this)->EnableContactDetection(enable);
}
bool Spr::PHSceneIf::IsContactDetectionEnabled(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->IsContactDetectionEnabled();
}
void Spr::PHSceneIf::EnableCCD(bool enable){
	return	((PHScene*)(Object*)(ObjectIf*)this)->EnableCCD(enable);
}
bool Spr::PHSceneIf::IsCCDEnabled(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->IsCCDEnabled();
}
void Spr::PHSceneIf::SetBroadPhaseMode(int mode){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetBroadPhaseMode(mode);
}
int Spr::PHSceneIf::GetBroadPhaseMode(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetBroadPhaseMode();
}
void Spr::PHSceneIf::SetContactDetectionRange(Vec3f center, Vec3f extent, int nx, int ny, int nz){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetContactDetectionRange(center, extent, nx, ny, nz);
}
void Spr::PHSceneIf::SetMaterialBlending(int mode){
	return	((PHScene*)(Object*)(ObjectIf*)this)->SetMaterialBlending(mode);
}
void Spr::PHSceneIf::GetMaterialBlending(){
	((PHScene*)(Object*)(ObjectIf*)this)->GetMaterialBlending();
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
void Spr::PHSceneIf::IntegratePart1(){
	((PHScene*)(Object*)(ObjectIf*)this)->IntegratePart1();
}
void Spr::PHSceneIf::IntegratePart2(){
	((PHScene*)(Object*)(ObjectIf*)this)->IntegratePart2();
}
void Spr::PHSceneIf::Clear(){
	((PHScene*)(Object*)(ObjectIf*)this)->Clear();
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
Spr::PHIKEngineIf* Spr::PHSceneIf::GetIKEngine(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetIKEngine();
}
Spr::PHFemEngineIf* Spr::PHSceneIf::GetFemEngine(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetFemEngine();
}
Spr::PHHapticEngineIf* Spr::PHSceneIf::GetHapticEngine(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetHapticEngine();
}
Spr::PHOpEngineIf* Spr::PHSceneIf::GetOpEngine(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetOpEngine();
}
Spr::PHTrackingEngineIf* Spr::PHSceneIf::GetTrackingEngine(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetTrackingEngine();
}
Spr::PHOpSpHashColliAgentIf* Spr::PHSceneIf::GetOpColliAgent(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetOpColliAgent();
}
Spr::PHHapticPointerIf* Spr::PHSceneIf::CreateHapticPointer(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->CreateHapticPointer();
}
void Spr::PHSceneIf::StepHapticLoop(){
	((PHScene*)(Object*)(ObjectIf*)this)->StepHapticLoop();
}
void Spr::PHSceneIf::StepHapticSync(){
	((PHScene*)(Object*)(ObjectIf*)this)->StepHapticSync();
}
Spr::UTPerformanceMeasureIf* Spr::PHSceneIf::GetPerformanceMeasure(){
	return	((PHScene*)(Object*)(ObjectIf*)this)->GetPerformanceMeasure();
}
SPR_IFIMP1(PHSdk, Sdk);
Spr::PHSceneIf* Spr::PHSdkIf::CreateScene(const Spr::PHSceneDesc& desc){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->CreateScene(desc);
}
int Spr::PHSdkIf::NScene(){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->NScene();
}
Spr::PHSceneIf* Spr::PHSdkIf::GetScene(int i){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->GetScene(i);
}
void Spr::PHSdkIf::MergeScene(Spr::PHSceneIf* scene0, Spr::PHSceneIf* scene1){
	return	((PHSdk*)(Object*)(ObjectIf*)this)->MergeScene(scene0, scene1);
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
SPR_IFIMP1(PHBone, SceneObject);
void Spr::PHBoneIf::SetSolid(Spr::PHSolidIf* solid){
	return	((PHBone*)(Object*)(ObjectIf*)this)->SetSolid(solid);
}
Spr::PHSolidIf* Spr::PHBoneIf::GetSolid(){
	return	((PHBone*)(Object*)(ObjectIf*)this)->GetSolid();
}
void Spr::PHBoneIf::SetProxySolid(Spr::PHSolidIf* solid){
	return	((PHBone*)(Object*)(ObjectIf*)this)->SetProxySolid(solid);
}
Spr::PHSolidIf* Spr::PHBoneIf::GetProxySolid(){
	return	((PHBone*)(Object*)(ObjectIf*)this)->GetProxySolid();
}
void Spr::PHBoneIf::SetJoint(Spr::PHJointIf* joint){
	return	((PHBone*)(Object*)(ObjectIf*)this)->SetJoint(joint);
}
Spr::PHJointIf* Spr::PHBoneIf::GetJoint(){
	return	((PHBone*)(Object*)(ObjectIf*)this)->GetJoint();
}
void Spr::PHBoneIf::SetParent(Spr::PHBoneIf* parent){
	return	((PHBone*)(Object*)(ObjectIf*)this)->SetParent(parent);
}
Spr::PHBoneIf* Spr::PHBoneIf::GetParent(){
	return	((PHBone*)(Object*)(ObjectIf*)this)->GetParent();
}
void Spr::PHBoneIf::SetLength(double length){
	return	((PHBone*)(Object*)(ObjectIf*)this)->SetLength(length);
}
void Spr::PHBoneIf::SetDirection(Vec3d dir){
	return	((PHBone*)(Object*)(ObjectIf*)this)->SetDirection(dir);
}
void Spr::PHBoneIf::SetPosition(Vec3d pos){
	return	((PHBone*)(Object*)(ObjectIf*)this)->SetPosition(pos);
}
SPR_IFIMP1(PHSkeleton, SceneObject);
int Spr::PHSkeletonIf::NBones(){
	return	((PHSkeleton*)(Object*)(ObjectIf*)this)->NBones();
}
Spr::PHBoneIf* Spr::PHSkeletonIf::GetBone(int i){
	return	((PHSkeleton*)(Object*)(ObjectIf*)this)->GetBone(i);
}
Spr::PHBoneIf* Spr::PHSkeletonIf::CreateBone(Spr::PHBoneIf* parent, const Spr::PHBoneDesc& desc){
	return	((PHSkeleton*)(Object*)(ObjectIf*)this)->CreateBone(parent, desc);
}
float Spr::PHSkeletonIf::GetGrabStrength(){
	return	((PHSkeleton*)(Object*)(ObjectIf*)this)->GetGrabStrength();
}
SPR_IFIMP1(PHFrame, SceneObject);
Posed Spr::PHFrameIf::GetPose(){
	return	((PHFrame*)(Object*)(ObjectIf*)this)->GetPose();
}
void Spr::PHFrameIf::SetPose(Posed p){
	return	((PHFrame*)(Object*)(ObjectIf*)this)->SetPose(p);
}
Spr::CDShapeIf* Spr::PHFrameIf::GetShape(){
	return	((PHFrame*)(Object*)(ObjectIf*)this)->GetShape();
}
SPR_IFIMP1(PHBody, SceneObject);
Posed Spr::PHBodyIf::GetPose()const{
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetPose();
}
Vec3d Spr::PHBodyIf::GetVelocity()const{
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetVelocity();
}
Vec3d Spr::PHBodyIf::GetAngularVelocity()const{
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetAngularVelocity();
}
Vec3d Spr::PHBodyIf::GetCenterOfMass(){
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetCenterOfMass();
}
Vec3d Spr::PHBodyIf::GetFramePosition(){
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetFramePosition();
}
Vec3d Spr::PHBodyIf::GetCenterPosition(){
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetCenterPosition();
}
bool Spr::PHBodyIf::IsDynamical(){
	return	((PHBody*)(Object*)(ObjectIf*)this)->IsDynamical();
}
bool Spr::PHBodyIf::IsFrozen(){
	return	((PHBody*)(Object*)(ObjectIf*)this)->IsFrozen();
}
void Spr::PHBodyIf::AddShape(Spr::CDShapeIf* shape){
	return	((PHBody*)(Object*)(ObjectIf*)this)->AddShape(shape);
}
void Spr::PHBodyIf::AddShapes(Spr::CDShapeIf** shBegin, Spr::CDShapeIf** shEnd){
	return	((PHBody*)(Object*)(ObjectIf*)this)->AddShapes(shBegin, shEnd);
}
void Spr::PHBodyIf::RemoveShape(int index){
	return	((PHBody*)(Object*)(ObjectIf*)this)->RemoveShape(index);
}
void Spr::PHBodyIf::RemoveShapes(int idxBegin, int idxEnd){
	return	((PHBody*)(Object*)(ObjectIf*)this)->RemoveShapes(idxBegin, idxEnd);
}
void Spr::PHBodyIf::RemoveShape(Spr::CDShapeIf* shape){
	return	((PHBody*)(Object*)(ObjectIf*)this)->RemoveShape(shape);
}
int Spr::PHBodyIf::NShape(){
	return	((PHBody*)(Object*)(ObjectIf*)this)->NShape();
}
Spr::CDShapeIf* Spr::PHBodyIf::GetShape(int index){
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetShape(index);
}
Posed Spr::PHBodyIf::GetShapePose(int index){
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetShapePose(index);
}
void Spr::PHBodyIf::SetShapePose(int index, const Posed& pose){
	return	((PHBody*)(Object*)(ObjectIf*)this)->SetShapePose(index, pose);
}
void Spr::PHBodyIf::ClearShape(){
	((PHBody*)(Object*)(ObjectIf*)this)->ClearShape();
}
void Spr::PHBodyIf::GetBBox(Vec3d& bbmin, Vec3d& bbmax, bool world){
	return	((PHBody*)(Object*)(ObjectIf*)this)->GetBBox(bbmin, bbmax, world);
}
bool Spr::PHBodyIf::InvalidateBbox(){
	return	((PHBody*)(Object*)(ObjectIf*)this)->InvalidateBbox();
}
SPR_IFIMP1(PHSolid, PHBody);
void Spr::PHSolidIf::AddForce(Vec3d f){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->AddForce(f);
}
void Spr::PHSolidIf::AddTorque(Vec3d t){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->AddTorque(t);
}
void Spr::PHSolidIf::AddForce(Vec3d f, Vec3d r){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->AddForce(f, r);
}
double Spr::PHSolidIf::GetMass(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetMass();
}
double Spr::PHSolidIf::GetMassInv(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetMassInv();
}
void Spr::PHSolidIf::SetMass(double m){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetMass(m);
}
void Spr::PHSolidIf::SetCenterOfMass(const Vec3d& center){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetCenterOfMass(center);
}
Matrix3d Spr::PHSolidIf::GetInertia(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetInertia();
}
Matrix3d Spr::PHSolidIf::GetInertiaInv(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetInertiaInv();
}
Matrix3d Spr::PHSolidIf::GetGlobalInertia(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetGlobalInertia();
}
void Spr::PHSolidIf::SetInertia(const Matrix3d& I){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetInertia(I);
}
void Spr::PHSolidIf::CompInertia(){
	((PHSolid*)(Object*)(ObjectIf*)this)->CompInertia();
}
void Spr::PHSolidIf::SetFramePosition(const Vec3d& p){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetFramePosition(p);
}
void Spr::PHSolidIf::SetCenterPosition(const Vec3d& p){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetCenterPosition(p);
}
Vec3d Spr::PHSolidIf::GetDeltaPosition()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetDeltaPosition();
}
Vec3d Spr::PHSolidIf::GetDeltaPosition(const Vec3d& p)const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetDeltaPosition(p);
}
Quaterniond Spr::PHSolidIf::GetOrientation()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetOrientation();
}
void Spr::PHSolidIf::SetOrientation(const Quaterniond& q){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetOrientation(q);
}
void Spr::PHSolidIf::SetPose(const Posed& p){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetPose(p);
}
void Spr::PHSolidIf::SetVelocity(const Vec3d& v){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetVelocity(v);
}
void Spr::PHSolidIf::SetAngularVelocity(const Vec3d& av){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetAngularVelocity(av);
}
void Spr::PHSolidIf::SetDv(const Spr::SpatialVector& a){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetDv(a);
}
Vec3d Spr::PHSolidIf::GetForce()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetForce();
}
Vec3d Spr::PHSolidIf::GetTorque()const{
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetTorque();
}
void Spr::PHSolidIf::SetGravity(bool bOn){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetGravity(bOn);
}
void Spr::PHSolidIf::SetDynamical(bool bOn){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetDynamical(bOn);
}
bool Spr::PHSolidIf::IsDynamical(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->IsDynamical();
}
void Spr::PHSolidIf::SetStationary(bool bOn){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->SetStationary(bOn);
}
bool Spr::PHSolidIf::IsStationary(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->IsStationary();
}
Spr::PHTreeNodeIf* Spr::PHSolidIf::GetTreeNode(){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->GetTreeNode();
}
void Spr::PHSolidIf::UpdateCacheLCP(double dt){
	return	((PHSolid*)(Object*)(ObjectIf*)this)->UpdateCacheLCP(dt);
}
SPR_IFIMP1(PHShapePairForLCP, CDShapePair);
int Spr::PHShapePairForLCPIf::NSectionVertexes(){
	return	((PHShapePairForLCP*)(Object*)(ObjectIf*)this)->NSectionVertexes();
}
Vec3d Spr::PHShapePairForLCPIf::GetSectionVertex(int i){
	return	((PHShapePairForLCP*)(Object*)(ObjectIf*)this)->GetSectionVertex(i);
}
double Spr::PHShapePairForLCPIf::GetContactDimension(){
	return	((PHShapePairForLCP*)(Object*)(ObjectIf*)this)->GetContactDimension();
}
Vec3d Spr::PHShapePairForLCPIf::GetNormalVector(){
	return	((PHShapePairForLCP*)(Object*)(ObjectIf*)this)->GetNormalVector();
}
void Spr::PHShapePairForLCPIf::GetClosestPoints(Vec3d& pa, Vec3d& pb){
	return	((PHShapePairForLCP*)(Object*)(ObjectIf*)this)->GetClosestPoints(pa, pb);
}
Spr::CDShapeIf* Spr::PHShapePairForLCPIf::GetShape(int i){
	return	((PHShapePairForLCP*)(Object*)(ObjectIf*)this)->GetShape(i);
}
SPR_IFIMP1(PHSolidPair, Object);
Spr::PHSolidIf* Spr::PHSolidPairIf::GetSolid(int i){
	return	((PHSolidPair*)(Object*)(ObjectIf*)this)->GetSolid(i);
}
int Spr::PHSolidPairIf::NListener(){
	return	((PHSolidPair*)(Object*)(ObjectIf*)this)->NListener();
}
Spr::PHCollisionListener* Spr::PHSolidPairIf::GetListener(int i){
	return	((PHSolidPair*)(Object*)(ObjectIf*)this)->GetListener(i);
}
void Spr::PHSolidPairIf::RemoveListener(int i){
	return	((PHSolidPair*)(Object*)(ObjectIf*)this)->RemoveListener(i);
}
void Spr::PHSolidPairIf::AddListener(Spr::PHCollisionListener* l, int pos){
	return	((PHSolidPair*)(Object*)(ObjectIf*)this)->AddListener(l, pos);
}
SPR_IFIMP1(PHSolidPairForLCP, PHSolidPair);
bool Spr::PHSolidPairForLCPIf::IsContactEnabled(){
	return	((PHSolidPairForLCP*)(Object*)(ObjectIf*)this)->IsContactEnabled();
}
void Spr::PHSolidPairForLCPIf::EnableContact(bool enable){
	return	((PHSolidPairForLCP*)(Object*)(ObjectIf*)this)->EnableContact(enable);
}
int Spr::PHSolidPairForLCPIf::GetContactState(int i, int j){
	return	((PHSolidPairForLCP*)(Object*)(ObjectIf*)this)->GetContactState(i, j);
}
Vec3d Spr::PHSolidPairForLCPIf::GetCommonPoint(int i, int j){
	return	((PHSolidPairForLCP*)(Object*)(ObjectIf*)this)->GetCommonPoint(i, j);
}
unsigned int Spr::PHSolidPairForLCPIf::GetLastContactCount(int i, int j){
	return	((PHSolidPairForLCP*)(Object*)(ObjectIf*)this)->GetLastContactCount(i, j);
}
double Spr::PHSolidPairForLCPIf::GetContactDepth(int i, int j){
	return	((PHSolidPairForLCP*)(Object*)(ObjectIf*)this)->GetContactDepth(i, j);
}
Spr::PHShapePairForLCPIf* Spr::PHSolidPairForLCPIf::GetShapePair(int i, int j){
	return	((PHSolidPairForLCP*)(Object*)(ObjectIf*)this)->GetShapePair(i, j);
}
SPR_OBJECTIMP1(PHBallJointNode, PHTreeNode);
SPR_OBJECTIMP1(PHBallJoint, PHJoint);
SPR_OBJECTIMP1(PHFrame, SceneObject);
SPR_OBJECTIMP_ABST1(PHBody, SceneObject);
SPR_OBJECTIMP_ABST1(PHConstraint, SceneObject);
SPR_OBJECTIMP1(PHConstraints, SceneObject);
SPR_OBJECTIMP1(PHShapePairForLCP, PHShapePair);
SPR_OBJECTIMP1(PHSolidPairForLCP, PHSolidPair);
SPR_OBJECTIMP1(PHConstraintEngine, PHEngine);
SPR_OBJECTIMP_ABST1(PHShapePair, CDShapePair);
SPR_OBJECTIMP_ABST1(PHSolidPair, Object);
SPR_OBJECTIMP1(PHContactPoint, PHConstraint);
SPR_OBJECTIMP1(PHEngine, SceneObject);
SPR_OBJECTIMP_ABST1(PHFemBase, SceneObject);
SPR_OBJECTIMP1(PHFemEngine, PHEngine);
SPR_OBJECTIMP1(PHFemMesh, SceneObject);
SPR_OBJECTIMP1(PHFemMeshNew, SceneObject);
SPR_OBJECTIMP1(PHFemMeshThermo, PHFemMesh);
SPR_OBJECTIMP1(PHFemPorousWOMove, PHFemBase);
SPR_OBJECTIMP1(PHFemThermo, PHFemBase);
SPR_OBJECTIMP1(PHFemVibration, PHFemBase);
SPR_OBJECTIMP1(PHFixJointNode, PHTreeNode);
SPR_OBJECTIMP1(PHFixJoint, PHJoint);
SPR_OBJECTIMP1(PHGravityEngine, PHEngine);
SPR_OBJECTIMP1(PHGear, SceneObject);
SPR_OBJECTIMP1(PHGenericJointNode, PHTreeNode1D);
SPR_OBJECTIMP1(PHGenericJoint, PH1DJoint);
SPR_OBJECTIMP1(PHHapticEngine, PHEngine);
SPR_OBJECTIMP1(PHHapticPointer, PHSolid);
SPR_OBJECTIMP_ABST1(PHSolidForHaptic, Object);
SPR_OBJECTIMP1(PHShapePairForHaptic, PHShapePair);
SPR_OBJECTIMP1(PHSolidPairForHaptic, PHSolidPair);
SPR_OBJECTIMP_ABST1(PHHapticStepBase, SceneObject);
SPR_OBJECTIMP1(PHHapticStepImpulse, PHHapticStepMulti);
SPR_OBJECTIMP1(PHHapticStepLocalDynamics, PHHapticStepMulti);
SPR_OBJECTIMP1(PHHapticStepLocalDynamicsDev, PHHapticStepMulti);
SPR_OBJECTIMP_ABST1(PHHapticStepMulti, PHHapticStepBase);
SPR_OBJECTIMP1(PHHapticStepSingle, PHHapticStepBase);
SPR_OBJECTIMP1(PHHingeJointNode, PHTreeNode1D);
SPR_OBJECTIMP1(PHHingeJoint, PH1DJoint);
SPR_OBJECTIMP1(PHIKActuator, SceneObject);
SPR_OBJECTIMP1(PHIKBallActuator, PHIKActuator);
SPR_OBJECTIMP1(PHIKHingeActuator, PHIKActuator);
SPR_OBJECTIMP1(PHIKSpringActuator, PHIKActuator);
SPR_OBJECTIMP1(PHIKEndEffector, SceneObject);
SPR_OBJECTIMP1(PHIKEngine, PHEngine);
SPR_OBJECTIMP_ABST1(PHJoint, PHConstraint);
SPR_OBJECTIMP_ABST1(PH1DJoint, PHJoint);
SPR_OBJECTIMP_ABST1(PHMate, PHJoint);
SPR_OBJECTIMP1(PHPointToPointMate, PHMate);
SPR_OBJECTIMP1(PHPointToLineMate, PHMate);
SPR_OBJECTIMP1(PHPointToPlaneMate, PHMate);
SPR_OBJECTIMP1(PHLineToLineMate, PHMate);
SPR_OBJECTIMP1(PHPlaneToPlaneMate, PHMate);
SPR_OBJECTIMP1(PH1DJointLimit, SceneObject);
SPR_OBJECTIMP1(PHBallJointLimit, SceneObject);
SPR_OBJECTIMP1(PHBallJointConeLimit, PHBallJointLimit);
SPR_OBJECTIMP1(PHBallJointSplineLimit, PHBallJointLimit);
SPR_OBJECTIMP1(PHBallJointIndependentLimit, PHBallJointLimit);
SPR_OBJECTIMP1(PH1DJointMotor, SceneObject);
SPR_OBJECTIMP1(PH1DJointNonLinearMotor, PH1DJointMotor);
SPR_OBJECTIMP1(PHHuman1DJointResistance, PH1DJointNonLinearMotor);
SPR_OBJECTIMP1(PHBallJointMotor, SceneObject);
SPR_OBJECTIMP1(PHBallJointNonLinearMotor, PHBallJointMotor);
SPR_OBJECTIMP1(PHHumanBallJointResistance, PHBallJointNonLinearMotor);
SPR_OBJECTIMP1(PHSpringMotor, SceneObject);
SPR_OBJECTIMP1(PHOpAnimation, Object);
SPR_OBJECTIMP1(PHOpEngine, PHEngine);
SPR_OBJECTIMP1(PHOpGroup, SceneObject);
SPR_OBJECTIMP1(PHOpHapticController, SceneObject);
SPR_OBJECTIMP1(PHOpHapticRenderer, SceneObject);
SPR_OBJECTIMP1(PHOpObj, SceneObject);
SPR_OBJECTIMP1(PHOpParticle, SceneObject);
SPR_OBJECTIMP1(PHOpSpHashColliAgent, SceneObject);
SPR_OBJECTIMP_ABST1(PHPath, SceneObject);
SPR_OBJECTIMP1(PHPathJointNode, PHTreeNode1D);
SPR_OBJECTIMP1(PHPathJoint, PH1DJoint);
SPR_OBJECTIMP1(PHPenaltyEngine, PHEngine);
SPR_OBJECTIMP1(PHRay, SceneObject);
SPR_OBJECTIMP1(PHScene, Scene);
SPR_OBJECTIMP1(PHSdk, Sdk);
SPR_OBJECTIMP1(PHBone, SceneObject);
SPR_OBJECTIMP1(PHSkeleton, SceneObject);
SPR_OBJECTIMP1(PHSliderJointNode, PHTreeNode1D);
SPR_OBJECTIMP1(PHSliderJoint, PH1DJoint);
SPR_OBJECTIMP1(PHSolid, PHBody);
SPR_OBJECTIMP1(PHSolidContainer, PHEngine);
SPR_OBJECTIMP1(PHSolidInitializer, PHEngine);
SPR_OBJECTIMP1(PHSpring, PHJoint);
SPR_OBJECTIMP1(PHTrackingEngine, PHEngine);
SPR_OBJECTIMP_ABST1(PHTreeNode, SceneObject);
SPR_OBJECTIMP1(PHRootNode, PHTreeNode);
SPR_OBJECTIMP_ABST1(PHTreeNode1D, PHTreeNode);
}

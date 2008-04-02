#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Creature\SprCRBody.h"
#include "..\..\include\Creature\SprCRController.h"
#include "..\..\include\Creature\SprCRCreature.h"
#include "..\..\include\Creature\SprCRInternalScene.h"
#include "..\..\include\Creature\SprCRInverseKinematics.h"
#include "..\..\include\Creature\SprCRSensor.h"
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
#include "..\Creature\CRAttentionController.h"
#include "..\Creature\CRBody.h"
#include "..\Creature\CRController.h"
#include "..\Creature\CRCreature.h"
#include "..\Creature\CREseWalkingController.h"
#include "..\Creature\CREyeController.h"
#include "..\Creature\CRFLAnimalGene.h"
#include "..\Creature\CRFLAnimalGeneData.h"
#include "..\Creature\CRFLAnimalQL.h"
#include "..\Creature\CRFourLegsAnimalBody.h"
#include "..\Creature\CRGazeController.h"
#include "..\Creature\CRGrabController.h"
#include "..\Creature\CRHingeHumanBody.h"
#include "..\Creature\CRInternalScene.h"
#include "..\Creature\CRInverseKinematics.h"
#include "..\Creature\CRNeckController.h"
#include "..\Creature\CROpticalSensor.h"
#include "..\Creature\CRReachingController.h"
#include "..\Creature\CRSensor.h"
#include "..\Creature\CRTravelController.h"
#include "..\Creature\CRTrunkFootAnimalBody.h"
#include "..\Creature\CRTrunkFootHumanBody.h"
#include "..\Creature\CRTryStandingUpController.h"
#include "..\Creature\CRWalkingController.h"
#include "..\Creature\CRWCChangeAroundCenter.h"
#include "..\Creature\CRWCFootForce.h"
#include "..\Creature\CRWCGeneForce.h"
#include "..\Creature\CRWCLandingSite.h"
#include "..\Creature\CRWCTimeLeft.h"
namespace Spr{

void RegisterTypeDescCreature(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	Spr::CRBodyDesc* pCRBodyDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRBodyDesc");
	desc->size = sizeof(Spr::CRBodyDesc);
	desc->ifInfo = CRBodyIf::GetIfInfoStatic();
	((IfInfo*)CRBodyIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRBodyDesc>;
	field = desc->AddField("CRCreatureJointOrder", "enum", "jointOrder", "");
	field->AddEnumConst("SOCKET_PARENT", Spr::CRBodyDesc::SOCKET_PARENT);
	field->AddEnumConst("PLUG_PARENT", Spr::CRBodyDesc::PLUG_PARENT);
	field->offset = int((char*)&(pCRBodyDesc->jointOrder) - (char*)pCRBodyDesc);
	db->RegisterDesc(desc);
	Spr::CRHingeHumanBodyDesc* pCRHingeHumanBodyDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRHingeHumanBodyDesc");
	desc->size = sizeof(Spr::CRHingeHumanBodyDesc);
	desc->ifInfo = CRHingeHumanBodyIf::GetIfInfoStatic();
	((IfInfo*)CRHingeHumanBodyIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRHingeHumanBodyDesc>;
	field = desc->AddBase("CRBodyDesc");
	field->offset = int((char*)(CRBodyDesc*)pCRHingeHumanBodyDesc - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "bodyMass", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->bodyMass) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "waistHeight", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->waistHeight) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "waistBreadth", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->waistBreadth) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "waistThickness", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->waistThickness) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "abdomenHeight", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->abdomenHeight) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "abdomenBreadth", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->abdomenBreadth) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "abdomenThickness", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->abdomenThickness) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "chestHeight", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->chestHeight) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "chestBreadth", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->chestBreadth) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "chestThickness", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->chestThickness) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "neckLength", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->neckLength) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "neckDiameter", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->neckDiameter) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "headDiameter", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->headDiameter) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "upperArmLength", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->upperArmLength) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "upperArmDiameter", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->upperArmDiameter) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "lowerArmLength", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->lowerArmLength) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "lowerArmDiameter", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->lowerArmDiameter) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "handLength", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->handLength) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "handBreadth", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->handBreadth) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "handThickness", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->handThickness) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "upperLegLength", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->upperLegLength) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "upperLegDiameter", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->upperLegDiameter) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "interLegDistance", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->interLegDistance) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "lowerLegLength", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->lowerLegLength) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "lowerLegDiameter", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->lowerLegDiameter) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "footLength", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->footLength) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "footBreadth", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->footBreadth) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "footThickness", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->footThickness) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "ankleToeDistance", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->ankleToeDistance) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "vertexToEyeHeight", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->vertexToEyeHeight) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "occiputToEyeDistance", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->occiputToEyeDistance) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "eyeDiameter", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->eyeDiameter) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "interpupillaryBreadth", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->interpupillaryBreadth) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "spring", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->spring) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damper", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damper) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springWaistAbdomen", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springWaistAbdomen) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperWaistAbdomen", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperWaistAbdomen) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springAbdomenChest", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springAbdomenChest) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperAbdomenChest", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperAbdomenChest) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springChestNeckX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springChestNeckX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperChestNeckX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperChestNeckX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springChestNeckY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springChestNeckY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperChestNeckY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperChestNeckY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springChestNeckZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springChestNeckZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperChestNeckZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperChestNeckZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springNeckHeadX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springNeckHeadX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperNeckHeadX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperNeckHeadX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springNeckHeadZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springNeckHeadZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperNeckHeadZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperNeckHeadZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springShoulderZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springShoulderZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperShoulderZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperShoulderZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springShoulderX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springShoulderX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperShoulderX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperShoulderX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springShoulderY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springShoulderY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperShoulderY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperShoulderY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springElbow", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springElbow) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperElbow", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperElbow) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springWristY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springWristY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperWristY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperWristY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springWristX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springWristX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperWristX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperWristX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springWristZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springWristZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperWristZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperWristZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springWaistLegZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springWaistLegZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperWaistLegZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperWaistLegZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springWaistLegX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springWaistLegX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperWaistLegX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperWaistLegX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springWaistLegY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springWaistLegY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperWaistLegY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperWaistLegY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springKnee", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springKnee) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperKnee", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperKnee) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springAnkleY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springAnkleY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperAnkleY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperAnkleY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springAnkleX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springAnkleX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperAnkleX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperAnkleX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springAnkleZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springAnkleZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperAnkleZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperAnkleZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springEyeY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springEyeY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperEyeY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperEyeY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "springEyeX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->springEyeX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "double", "damperEyeX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->damperEyeX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec3d", "posRightUpperArm", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->posRightUpperArm) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Quaterniond", "oriRightUpperArm", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->oriRightUpperArm) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Quaterniond", "oriRightLowerArm", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->oriRightLowerArm) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Quaterniond", "oriRightHand", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->oriRightHand) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWaistAbdomen", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeWaistAbdomen) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeAbdomenChest", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeAbdomenChest) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeChestNeckX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeChestNeckX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeChestNeckY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeChestNeckY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeChestNeckZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeChestNeckZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeNeckHeadX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeNeckHeadX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeNeckHeadZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeNeckHeadZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeShoulderZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeShoulderZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeShoulderX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeShoulderX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeShoulderY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeShoulderY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeElbow", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeElbow) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWristY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeWristY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWristX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeWristX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWristZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeWristZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWaistLegZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeWaistLegZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWaistLegX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeWaistLegX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWaistLegY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeWaistLegY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeKnee", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeKnee) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeAnkleY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeAnkleY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeAnkleX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeAnkleX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeAnkleZ", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeAnkleZ) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeRightEyeY", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeRightEyeY) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeEyeX", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->rangeEyeX) - (char*)pCRHingeHumanBodyDesc);
	field = desc->AddField("", "bool", "noLegs", "");
	field->offset = int((char*)&(pCRHingeHumanBodyDesc->noLegs) - (char*)pCRHingeHumanBodyDesc);
	db->RegisterDesc(desc);
	Spr::CRFourLegsAnimalBodyDesc* pCRFourLegsAnimalBodyDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRFourLegsAnimalBodyDesc");
	desc->size = sizeof(Spr::CRFourLegsAnimalBodyDesc);
	desc->ifInfo = CRFourLegsAnimalBodyIf::GetIfInfoStatic();
	((IfInfo*)CRFourLegsAnimalBodyIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRFourLegsAnimalBodyDesc>;
	field = desc->AddBase("CRBodyDesc");
	field->offset = int((char*)(CRBodyDesc*)pCRFourLegsAnimalBodyDesc - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "int", "soNSolids", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->soNSolids) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "int", "joNBallJoints", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->joNBallJoints) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "int", "joNHingeJoints", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->joNHingeJoints) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "int", "joNJoints", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->joNJoints) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "waistBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->waistBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "waistHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->waistHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "waistThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->waistThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "chestBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->chestBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "chestHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->chestHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "chestThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->chestThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "tailBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->tailBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "tailHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->tailHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "tailThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->tailThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "neckBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->neckBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "neckHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->neckHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "neckThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->neckThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "headBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->headBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "headHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->headHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "headThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->headThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "breastboneBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->breastboneBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "breastboneHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->breastboneHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "breastboneThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->breastboneThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "radiusBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->radiusBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "radiusHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->radiusHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "radiusThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->radiusThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "frontCannonBoneBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->frontCannonBoneBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "frontCannonBoneHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->frontCannonBoneHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "frontCannonBoneThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->frontCannonBoneThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "frontToeBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->frontToeBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "frontToeHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->frontToeHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "frontToeThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->frontToeThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "femurBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->femurBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "femurHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->femurHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "femurThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->femurThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "tibiaBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->tibiaBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "tibiaHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->tibiaHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "tibiaThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->tibiaThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "rearCannonBoneBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rearCannonBoneBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "rearCannonBoneHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rearCannonBoneHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "rearCannonBoneThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rearCannonBoneThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "rearToeBreadth", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rearToeBreadth) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "rearToeHeight", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rearToeHeight) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "rearToeThickness", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rearToeThickness) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springWaistChest", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springWaistChest) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperWaistChest", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperWaistChest) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springWaistTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springWaistTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperWaistTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperWaistTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springChestNeck", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springChestNeck) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperChestNeck", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperChestNeck) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springNeckHead", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springNeckHead) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperNeckHead", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperNeckHead) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springShoulder", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springShoulder) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperShoulder", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperShoulder) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springFrontAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springFrontAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperFrontAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperFrontAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springHip", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springHip) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperHip", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperHip) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springRearAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springRearAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperRearAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperRearAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springElbow", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springElbow) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperElbow", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperElbow) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springFrontKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springFrontKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperFrontKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperFrontKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springStifle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springStifle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperStifle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperStifle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "springRearKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->springRearKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "damperRearKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->damperRearKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeElbow", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rangeElbow) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeFrontKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rangeFrontKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeStifle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rangeStifle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeRearKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->rangeRearKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalWaistChest", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalWaistChest) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalWaistTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalWaistTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalChestNeck", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalChestNeck) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalNeckHead", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalNeckHead) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalShoulder", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalShoulder) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalFrontAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalFrontAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalHip", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalHip) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Quaterniond", "goalRearAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->goalRearAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "originElbow", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->originElbow) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "originFrontKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->originFrontKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "originStifle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->originStifle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "originRearKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->originRearKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirWaistChest", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirWaistChest) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirWaistTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirWaistTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirChestNeck", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirChestNeck) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirNeckHead", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirNeckHead) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirShoulder", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirShoulder) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirFrontAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirFrontAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirHip", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirHip) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec3d", "limitDirRearAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitDirRearAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingWaistChest", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingWaistChest) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingWaistTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingWaistTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingChestNeck", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingChestNeck) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingNeckHead", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingNeckHead) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingShoulder", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingShoulder) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingFrontAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingFrontAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingHip", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingHip) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitSwingRearAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitSwingRearAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistWaistChest", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistWaistChest) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistWaistTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistWaistTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistChestNeck", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistChestNeck) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistNeckHead", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistNeckHead) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistShoulder", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistShoulder) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistFrontAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistFrontAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistHip", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistHip) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "limitTwistRearAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->limitTwistRearAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxWaistChest", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxWaistChest) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxChestNeck", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxChestNeck) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxNeckHead", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxNeckHead) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxWaistTail", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxWaistTail) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxTail12", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxTail12) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxTail23", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxTail23) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxLeftShoulder", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxLeftShoulder) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxLeftElbow", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxLeftElbow) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxLeftFrontKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxLeftFrontKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxLeftFrontAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxLeftFrontAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxLeftHip", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxLeftHip) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxLeftStifle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxLeftStifle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxLeftRearKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxLeftRearKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxLeftRearAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxLeftRearAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxRightShoulder", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxRightShoulder) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxRightElbow", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxRightElbow) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxRightFrontKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxRightFrontKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxRightFrontAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxRightFrontAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxRightHip", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxRightHip) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxRightStifle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxRightStifle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxRightRearKnee", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxRightRearKnee) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "fMaxRightRearAnkle", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->fMaxRightRearAnkle) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "float", "materialMu", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->materialMu) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "bool", "noLegs", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->noLegs) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "bool", "dynamicalMode", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->dynamicalMode) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "double", "totalMass", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->totalMass) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "bool", "flagFMax", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->flagFMax) - (char*)pCRFourLegsAnimalBodyDesc);
	field = desc->AddField("", "bool", "flagRange", "");
	field->offset = int((char*)&(pCRFourLegsAnimalBodyDesc->flagRange) - (char*)pCRFourLegsAnimalBodyDesc);
	db->RegisterDesc(desc);
	Spr::CRTrunkFootHumanBodyDesc* pCRTrunkFootHumanBodyDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRTrunkFootHumanBodyDesc");
	desc->size = sizeof(Spr::CRTrunkFootHumanBodyDesc);
	desc->ifInfo = CRTrunkFootHumanBodyIf::GetIfInfoStatic();
	((IfInfo*)CRTrunkFootHumanBodyIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRTrunkFootHumanBodyDesc>;
	field = desc->AddBase("CRBodyDesc");
	field->offset = int((char*)(CRBodyDesc*)pCRTrunkFootHumanBodyDesc - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "waistHeight", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->waistHeight) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "waistBreadth", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->waistBreadth) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "waistThickness", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->waistThickness) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "chestHeight", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->chestHeight) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "chestBreadth", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->chestBreadth) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "chestThickness", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->chestThickness) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "neckLength", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->neckLength) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "headDiameter", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->headDiameter) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "footLength", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->footLength) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "footBreadth", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->footBreadth) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "footThickness", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->footThickness) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "springWaistChest", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->springWaistChest) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "damperWaistChest", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->damperWaistChest) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "springChestHead", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->springChestHead) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "double", "damperChestHead", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->damperChestHead) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWaistChest", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->rangeWaistChest) - (char*)pCRTrunkFootHumanBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeChestHead", "");
	field->offset = int((char*)&(pCRTrunkFootHumanBodyDesc->rangeChestHead) - (char*)pCRTrunkFootHumanBodyDesc);
	db->RegisterDesc(desc);
	Spr::CRTrunkFootAnimalBodyDesc* pCRTrunkFootAnimalBodyDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRTrunkFootAnimalBodyDesc");
	desc->size = sizeof(Spr::CRTrunkFootAnimalBodyDesc);
	desc->ifInfo = CRTrunkFootAnimalBodyIf::GetIfInfoStatic();
	((IfInfo*)CRTrunkFootAnimalBodyIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRTrunkFootAnimalBodyDesc>;
	field = desc->AddBase("CRBodyDesc");
	field->offset = int((char*)(CRBodyDesc*)pCRTrunkFootAnimalBodyDesc - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "waistHeight", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->waistHeight) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "waistBreadth", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->waistBreadth) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "waistThickness", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->waistThickness) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "chestHeight", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->chestHeight) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "chestBreadth", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->chestBreadth) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "chestThickness", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->chestThickness) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "neckLength", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->neckLength) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "float", "headDiameter", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->headDiameter) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "footLength", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->footLength) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "footBreadth", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->footBreadth) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "footThickness", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->footThickness) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "springWaistChest", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->springWaistChest) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "damperWaistChest", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->damperWaistChest) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "springChestHead", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->springChestHead) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "double", "damperChestHead", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->damperChestHead) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeWaistChest", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->rangeWaistChest) - (char*)pCRTrunkFootAnimalBodyDesc);
	field = desc->AddField("", "Vec2d", "rangeChestHead", "");
	field->offset = int((char*)&(pCRTrunkFootAnimalBodyDesc->rangeChestHead) - (char*)pCRTrunkFootAnimalBodyDesc);
	db->RegisterDesc(desc);
	Spr::CRControllerDesc* pCRControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRControllerDesc");
	desc->size = sizeof(Spr::CRControllerDesc);
	desc->ifInfo = CRControllerIf::GetIfInfoStatic();
	((IfInfo*)CRControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRControllerDesc>;
	db->RegisterDesc(desc);
	Spr::CREyeControllerState* pCREyeControllerState = NULL;
	desc = DBG_NEW UTTypeDesc("CREyeControllerState");
	desc->size = sizeof(Spr::CREyeControllerState);
	desc->ifInfo = CREyeControllerIf::GetIfInfoStatic();
	((IfInfo*)CREyeControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CREyeControllerState>;
	field = desc->AddField("ControlState", "enum", "controlState", "");
	field->AddEnumConst("CS_SACCADE", Spr::CREyeControllerState::CS_SACCADE);
	field->AddEnumConst("CS_PURSUIT", Spr::CREyeControllerState::CS_PURSUIT);
	field->offset = int((char*)&(pCREyeControllerState->controlState) - (char*)pCREyeControllerState);
	db->RegisterDesc(desc);
	Spr::CREyeControllerDesc* pCREyeControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CREyeControllerDesc");
	desc->size = sizeof(Spr::CREyeControllerDesc);
	desc->ifInfo = CREyeControllerIf::GetIfInfoStatic();
	((IfInfo*)CREyeControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CREyeControllerDesc>;
	field = desc->AddBase("CREyeControllerState");
	field->offset = int((char*)(CREyeControllerState*)pCREyeControllerDesc - (char*)pCREyeControllerDesc);
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCREyeControllerDesc - (char*)pCREyeControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRNeckControllerState* pCRNeckControllerState = NULL;
	desc = DBG_NEW UTTypeDesc("CRNeckControllerState");
	desc->size = sizeof(Spr::CRNeckControllerState);
	desc->ifInfo = CRNeckControllerIf::GetIfInfoStatic();
	((IfInfo*)CRNeckControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRNeckControllerState>;
	db->RegisterDesc(desc);
	Spr::CRNeckControllerDesc* pCRNeckControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRNeckControllerDesc");
	desc->size = sizeof(Spr::CRNeckControllerDesc);
	desc->ifInfo = CRNeckControllerIf::GetIfInfoStatic();
	((IfInfo*)CRNeckControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRNeckControllerDesc>;
	field = desc->AddBase("CRNeckControllerState");
	field->offset = int((char*)(CRNeckControllerState*)pCRNeckControllerDesc - (char*)pCRNeckControllerDesc);
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCRNeckControllerDesc - (char*)pCRNeckControllerDesc);
	field = desc->AddField("", "float", "lowerAttractiveness", "");
	field->offset = int((char*)&(pCRNeckControllerDesc->lowerAttractiveness) - (char*)pCRNeckControllerDesc);
	field = desc->AddField("", "float", "upperAttractiveness", "");
	field->offset = int((char*)&(pCRNeckControllerDesc->upperAttractiveness) - (char*)pCRNeckControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRReachingControllerDesc* pCRReachingControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRReachingControllerDesc");
	desc->size = sizeof(Spr::CRReachingControllerDesc);
	desc->ifInfo = CRReachingControllerIf::GetIfInfoStatic();
	((IfInfo*)CRReachingControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRReachingControllerDesc>;
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCRReachingControllerDesc - (char*)pCRReachingControllerDesc);
	field = desc->AddField("pointer", "PHSolidIf", "solid", "");
	field->offset = int((char*)&(pCRReachingControllerDesc->solid) - (char*)pCRReachingControllerDesc);
	field = desc->AddField("", "Vec3f", "reachPos", "");
	field->offset = int((char*)&(pCRReachingControllerDesc->reachPos) - (char*)pCRReachingControllerDesc);
	field = desc->AddField("", "Quaterniond", "fixOri", "");
	field->offset = int((char*)&(pCRReachingControllerDesc->fixOri) - (char*)pCRReachingControllerDesc);
	field = desc->AddField("", "float", "limitForce", "");
	field->offset = int((char*)&(pCRReachingControllerDesc->limitForce) - (char*)pCRReachingControllerDesc);
	field = desc->AddField("", "float", "springPos", "");
	field->offset = int((char*)&(pCRReachingControllerDesc->springPos) - (char*)pCRReachingControllerDesc);
	field = desc->AddField("", "float", "damperPos", "");
	field->offset = int((char*)&(pCRReachingControllerDesc->damperPos) - (char*)pCRReachingControllerDesc);
	field = desc->AddField("", "float", "springOri", "");
	field->offset = int((char*)&(pCRReachingControllerDesc->springOri) - (char*)pCRReachingControllerDesc);
	field = desc->AddField("", "float", "damperOri", "");
	field->offset = int((char*)&(pCRReachingControllerDesc->damperOri) - (char*)pCRReachingControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRGazeControllerState* pCRGazeControllerState = NULL;
	desc = DBG_NEW UTTypeDesc("CRGazeControllerState");
	desc->size = sizeof(Spr::CRGazeControllerState);
	desc->ifInfo = CRGazeControllerIf::GetIfInfoStatic();
	((IfInfo*)CRGazeControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRGazeControllerState>;
	db->RegisterDesc(desc);
	Spr::CRGazeControllerDesc* pCRGazeControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRGazeControllerDesc");
	desc->size = sizeof(Spr::CRGazeControllerDesc);
	desc->ifInfo = CRGazeControllerIf::GetIfInfoStatic();
	((IfInfo*)CRGazeControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRGazeControllerDesc>;
	field = desc->AddBase("CRGazeControllerState");
	field->offset = int((char*)(CRGazeControllerState*)pCRGazeControllerDesc - (char*)pCRGazeControllerDesc);
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCRGazeControllerDesc - (char*)pCRGazeControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRAttentionControllerDesc* pCRAttentionControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRAttentionControllerDesc");
	desc->size = sizeof(Spr::CRAttentionControllerDesc);
	desc->ifInfo = CRAttentionControllerIf::GetIfInfoStatic();
	((IfInfo*)CRAttentionControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRAttentionControllerDesc>;
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCRAttentionControllerDesc - (char*)pCRAttentionControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRTryStandingUpControllerDesc* pCRTryStandingUpControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRTryStandingUpControllerDesc");
	desc->size = sizeof(Spr::CRTryStandingUpControllerDesc);
	desc->ifInfo = CRTryStandingUpControllerIf::GetIfInfoStatic();
	((IfInfo*)CRTryStandingUpControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRTryStandingUpControllerDesc>;
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCRTryStandingUpControllerDesc - (char*)pCRTryStandingUpControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRWalkingControllerDesc* pCRWalkingControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRWalkingControllerDesc");
	desc->size = sizeof(Spr::CRWalkingControllerDesc);
	desc->ifInfo = CRWalkingControllerIf::GetIfInfoStatic();
	((IfInfo*)CRWalkingControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRWalkingControllerDesc>;
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCRWalkingControllerDesc - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "paramLdx", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->paramLdx) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "paramLdz", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->paramLdz) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "maxHalfStride", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->maxHalfStride) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "height", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->height) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "minCycleRate", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->minCycleRate) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "maxDSRate", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->maxDSRate) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "FootLength", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->FootLength) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxFootLength", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxFootLength) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MinFootLength", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MinFootLength) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxRoGround", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxRoGround) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxRoLandingSiteFront", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxRoLandingSiteFront) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxRoLandingSiteSide", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxRoLandingSiteSide) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxRoLandingSiteBack", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxRoLandingSiteBack) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxRoConstraint", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxRoConstraint) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxFootSpeedFront", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxFootSpeedFront) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxFootSpeedSide", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxFootSpeedSide) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxFootSpeedBack", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxFootSpeedBack) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxFootAccelerationFront", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxFootAccelerationFront) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxFootAccelerationSide", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxFootAccelerationSide) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "MaxFootAccelerationBack", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->MaxFootAccelerationBack) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "DoubleSupportLimitRate", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->DoubleSupportLimitRate) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "LimitChange", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->LimitChange) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "pi", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->pi) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "footsize", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->footsize) - (char*)pCRWalkingControllerDesc);
	field = desc->AddField("", "double", "miu", "");
	field->offset = int((char*)&(pCRWalkingControllerDesc->miu) - (char*)pCRWalkingControllerDesc);
	db->RegisterDesc(desc);
	Spr::CREseWalkingControllerDesc* pCREseWalkingControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CREseWalkingControllerDesc");
	desc->size = sizeof(Spr::CREseWalkingControllerDesc);
	desc->ifInfo = CREseWalkingControllerIf::GetIfInfoStatic();
	((IfInfo*)CREseWalkingControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CREseWalkingControllerDesc>;
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCREseWalkingControllerDesc - (char*)pCREseWalkingControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRTravelControllerDesc* pCRTravelControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRTravelControllerDesc");
	desc->size = sizeof(Spr::CRTravelControllerDesc);
	desc->ifInfo = CRTravelControllerIf::GetIfInfoStatic();
	((IfInfo*)CRTravelControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRTravelControllerDesc>;
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCRTravelControllerDesc - (char*)pCRTravelControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRGrabControllerDesc* pCRGrabControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRGrabControllerDesc");
	desc->size = sizeof(Spr::CRGrabControllerDesc);
	desc->ifInfo = CRGrabControllerIf::GetIfInfoStatic();
	((IfInfo*)CRGrabControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRGrabControllerDesc>;
	field = desc->AddBase("CRControllerDesc");
	field->offset = int((char*)(CRControllerDesc*)pCRGrabControllerDesc - (char*)pCRGrabControllerDesc);
	field = desc->AddField("", "double", "rateSpringSoften", "");
	field->offset = int((char*)&(pCRGrabControllerDesc->rateSpringSoften) - (char*)pCRGrabControllerDesc);
	field = desc->AddField("", "double", "rateDamperSoften", "");
	field->offset = int((char*)&(pCRGrabControllerDesc->rateDamperSoften) - (char*)pCRGrabControllerDesc);
	field = desc->AddField("", "double", "rateSpringHarden", "");
	field->offset = int((char*)&(pCRGrabControllerDesc->rateSpringHarden) - (char*)pCRGrabControllerDesc);
	field = desc->AddField("", "double", "rateDamperHarden", "");
	field->offset = int((char*)&(pCRGrabControllerDesc->rateDamperHarden) - (char*)pCRGrabControllerDesc);
	db->RegisterDesc(desc);
	Spr::CRCreatureDesc* pCRCreatureDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRCreatureDesc");
	desc->size = sizeof(Spr::CRCreatureDesc);
	desc->ifInfo = CRCreatureIf::GetIfInfoStatic();
	((IfInfo*)CRCreatureIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRCreatureDesc>;
	db->RegisterDesc(desc);
	Spr::CRInternalSceneObjectDesc* pCRInternalSceneObjectDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRInternalSceneObjectDesc");
	desc->size = sizeof(Spr::CRInternalSceneObjectDesc);
	desc->ifInfo = CRInternalSceneObjectIf::GetIfInfoStatic();
	((IfInfo*)CRInternalSceneObjectIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRInternalSceneObjectDesc>;
	field = desc->AddField("pointer", "char", "type", "");
	field->offset = int((char*)&(pCRInternalSceneObjectDesc->type) - (char*)pCRInternalSceneObjectDesc);
	field = desc->AddField("pointer", "PHSolidIf", "solid", "");
	field->offset = int((char*)&(pCRInternalSceneObjectDesc->solid) - (char*)pCRInternalSceneObjectDesc);
	field = desc->AddField("", "Vec3f", "position", "");
	field->offset = int((char*)&(pCRInternalSceneObjectDesc->position) - (char*)pCRInternalSceneObjectDesc);
	db->RegisterDesc(desc);
	Spr::CRISAttractiveObjectDesc* pCRISAttractiveObjectDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRISAttractiveObjectDesc");
	desc->size = sizeof(Spr::CRISAttractiveObjectDesc);
	desc->ifInfo = CRISAttractiveObjectIf::GetIfInfoStatic();
	((IfInfo*)CRISAttractiveObjectIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRISAttractiveObjectDesc>;
	field = desc->AddBase("CRInternalSceneObjectDesc");
	field->offset = int((char*)(CRInternalSceneObjectDesc*)pCRISAttractiveObjectDesc - (char*)pCRISAttractiveObjectDesc);
	field = desc->AddField("", "float", "bottomupAttr", "");
	field->offset = int((char*)&(pCRISAttractiveObjectDesc->bottomupAttr) - (char*)pCRISAttractiveObjectDesc);
	field = desc->AddField("", "float", "topdownAttr", "");
	field->offset = int((char*)&(pCRISAttractiveObjectDesc->topdownAttr) - (char*)pCRISAttractiveObjectDesc);
	field = desc->AddField("", "float", "unknownAttr", "");
	field->offset = int((char*)&(pCRISAttractiveObjectDesc->unknownAttr) - (char*)pCRISAttractiveObjectDesc);
	field = desc->AddField("", "float", "uncertainty", "");
	field->offset = int((char*)&(pCRISAttractiveObjectDesc->uncertainty) - (char*)pCRISAttractiveObjectDesc);
	field = desc->AddField("", "float", "uncertaintyIncRate", "");
	field->offset = int((char*)&(pCRISAttractiveObjectDesc->uncertaintyIncRate) - (char*)pCRISAttractiveObjectDesc);
	field = desc->AddField("", "float", "uncertaintyDecRate", "");
	field->offset = int((char*)&(pCRISAttractiveObjectDesc->uncertaintyDecRate) - (char*)pCRISAttractiveObjectDesc);
	db->RegisterDesc(desc);
	Spr::CRISTravelPotentialObjectDesc* pCRISTravelPotentialObjectDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRISTravelPotentialObjectDesc");
	desc->size = sizeof(Spr::CRISTravelPotentialObjectDesc);
	desc->ifInfo = CRISTravelPotentialObjectIf::GetIfInfoStatic();
	((IfInfo*)CRISTravelPotentialObjectIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRISTravelPotentialObjectDesc>;
	field = desc->AddBase("CRInternalSceneObjectDesc");
	field->offset = int((char*)(CRInternalSceneObjectDesc*)pCRISTravelPotentialObjectDesc - (char*)pCRISTravelPotentialObjectDesc);
	field = desc->AddField("", "Vec2f", "strength", "");
	field->offset = int((char*)&(pCRISTravelPotentialObjectDesc->strength) - (char*)pCRISTravelPotentialObjectDesc);
	field = desc->AddField("", "Vec2f", "decay", "");
	field->offset = int((char*)&(pCRISTravelPotentialObjectDesc->decay) - (char*)pCRISTravelPotentialObjectDesc);
	db->RegisterDesc(desc);
	Spr::CRInternalSceneDesc* pCRInternalSceneDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRInternalSceneDesc");
	desc->size = sizeof(Spr::CRInternalSceneDesc);
	desc->ifInfo = CRInternalSceneIf::GetIfInfoStatic();
	((IfInfo*)CRInternalSceneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRInternalSceneDesc>;
	db->RegisterDesc(desc);
	Spr::CRIKControlDesc* pCRIKControlDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKControlDesc");
	desc->size = sizeof(Spr::CRIKControlDesc);
	desc->ifInfo = CRIKControlIf::GetIfInfoStatic();
	((IfInfo*)CRIKControlIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKControlDesc>;
	field = desc->AddField("pointer", "PHSolidIf", "solid", "");
	field->offset = int((char*)&(pCRIKControlDesc->solid) - (char*)pCRIKControlDesc);
	db->RegisterDesc(desc);
	Spr::CRIKControlPosDesc* pCRIKControlPosDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKControlPosDesc");
	desc->size = sizeof(Spr::CRIKControlPosDesc);
	desc->ifInfo = CRIKControlPosIf::GetIfInfoStatic();
	((IfInfo*)CRIKControlPosIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKControlPosDesc>;
	field = desc->AddBase("CRIKControlDesc");
	field->offset = int((char*)(CRIKControlDesc*)pCRIKControlPosDesc - (char*)pCRIKControlPosDesc);
	field = desc->AddField("", "Vec3d", "pos", "");
	field->offset = int((char*)&(pCRIKControlPosDesc->pos) - (char*)pCRIKControlPosDesc);
	db->RegisterDesc(desc);
	Spr::CRIKControlOriDesc* pCRIKControlOriDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKControlOriDesc");
	desc->size = sizeof(Spr::CRIKControlOriDesc);
	desc->ifInfo = CRIKControlOriIf::GetIfInfoStatic();
	((IfInfo*)CRIKControlOriIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKControlOriDesc>;
	field = desc->AddBase("CRIKControlDesc");
	field->offset = int((char*)(CRIKControlDesc*)pCRIKControlOriDesc - (char*)pCRIKControlOriDesc);
	db->RegisterDesc(desc);
	Spr::CRIKMovableDesc* pCRIKMovableDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKMovableDesc");
	desc->size = sizeof(Spr::CRIKMovableDesc);
	desc->ifInfo = CRIKMovableIf::GetIfInfoStatic();
	((IfInfo*)CRIKMovableIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKMovableDesc>;
	field = desc->AddField("", "float", "bias", "");
	field->offset = int((char*)&(pCRIKMovableDesc->bias) - (char*)pCRIKMovableDesc);
	db->RegisterDesc(desc);
	Spr::CRIKMovableSolidPosDesc* pCRIKMovableSolidPosDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKMovableSolidPosDesc");
	desc->size = sizeof(Spr::CRIKMovableSolidPosDesc);
	desc->ifInfo = CRIKMovableSolidPosIf::GetIfInfoStatic();
	((IfInfo*)CRIKMovableSolidPosIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKMovableSolidPosDesc>;
	field = desc->AddBase("CRIKMovableDesc");
	field->offset = int((char*)(CRIKMovableDesc*)pCRIKMovableSolidPosDesc - (char*)pCRIKMovableSolidPosDesc);
	field = desc->AddField("pointer", "PHSolidIf", "solid", "");
	field->offset = int((char*)&(pCRIKMovableSolidPosDesc->solid) - (char*)pCRIKMovableSolidPosDesc);
	db->RegisterDesc(desc);
	Spr::CRIKMovableSolidOriDesc* pCRIKMovableSolidOriDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKMovableSolidOriDesc");
	desc->size = sizeof(Spr::CRIKMovableSolidOriDesc);
	desc->ifInfo = CRIKMovableSolidOriIf::GetIfInfoStatic();
	((IfInfo*)CRIKMovableSolidOriIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKMovableSolidOriDesc>;
	field = desc->AddBase("CRIKMovableDesc");
	field->offset = int((char*)(CRIKMovableDesc*)pCRIKMovableSolidOriDesc - (char*)pCRIKMovableSolidOriDesc);
	field = desc->AddField("pointer", "PHSolidIf", "solid", "");
	field->offset = int((char*)&(pCRIKMovableSolidOriDesc->solid) - (char*)pCRIKMovableSolidOriDesc);
	db->RegisterDesc(desc);
	Spr::CRIKMovableBallJointOriDesc* pCRIKMovableBallJointOriDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKMovableBallJointOriDesc");
	desc->size = sizeof(Spr::CRIKMovableBallJointOriDesc);
	desc->ifInfo = CRIKMovableBallJointOriIf::GetIfInfoStatic();
	((IfInfo*)CRIKMovableBallJointOriIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKMovableBallJointOriDesc>;
	field = desc->AddBase("CRIKMovableDesc");
	field->offset = int((char*)(CRIKMovableDesc*)pCRIKMovableBallJointOriDesc - (char*)pCRIKMovableBallJointOriDesc);
	field = desc->AddField("pointer", "PHBallJointIf", "joint", "");
	field->offset = int((char*)&(pCRIKMovableBallJointOriDesc->joint) - (char*)pCRIKMovableBallJointOriDesc);
	db->RegisterDesc(desc);
	Spr::CRIKMovable3HingeJointOriDesc* pCRIKMovable3HingeJointOriDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKMovable3HingeJointOriDesc");
	desc->size = sizeof(Spr::CRIKMovable3HingeJointOriDesc);
	desc->ifInfo = CRIKMovable3HingeJointOriIf::GetIfInfoStatic();
	((IfInfo*)CRIKMovable3HingeJointOriIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKMovable3HingeJointOriDesc>;
	field = desc->AddBase("CRIKMovableDesc");
	field->offset = int((char*)(CRIKMovableDesc*)pCRIKMovable3HingeJointOriDesc - (char*)pCRIKMovable3HingeJointOriDesc);
	field = desc->AddField("pointer", "PHHingeJointIf", "joint1", "");
	field->offset = int((char*)&(pCRIKMovable3HingeJointOriDesc->joint1) - (char*)pCRIKMovable3HingeJointOriDesc);
	field = desc->AddField("pointer", "PHHingeJointIf", "joint2", "");
	field->offset = int((char*)&(pCRIKMovable3HingeJointOriDesc->joint2) - (char*)pCRIKMovable3HingeJointOriDesc);
	field = desc->AddField("pointer", "PHHingeJointIf", "joint3", "");
	field->offset = int((char*)&(pCRIKMovable3HingeJointOriDesc->joint3) - (char*)pCRIKMovable3HingeJointOriDesc);
	db->RegisterDesc(desc);
	Spr::CRIKMovableHingeJointOriDesc* pCRIKMovableHingeJointOriDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRIKMovableHingeJointOriDesc");
	desc->size = sizeof(Spr::CRIKMovableHingeJointOriDesc);
	desc->ifInfo = CRIKMovableHingeJointOriIf::GetIfInfoStatic();
	((IfInfo*)CRIKMovableHingeJointOriIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRIKMovableHingeJointOriDesc>;
	field = desc->AddBase("CRIKMovableDesc");
	field->offset = int((char*)(CRIKMovableDesc*)pCRIKMovableHingeJointOriDesc - (char*)pCRIKMovableHingeJointOriDesc);
	field = desc->AddField("pointer", "PHHingeJointIf", "joint", "");
	field->offset = int((char*)&(pCRIKMovableHingeJointOriDesc->joint) - (char*)pCRIKMovableHingeJointOriDesc);
	db->RegisterDesc(desc);
	Spr::CRSensorDesc* pCRSensorDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRSensorDesc");
	desc->size = sizeof(Spr::CRSensorDesc);
	desc->ifInfo = CRSensorIf::GetIfInfoStatic();
	((IfInfo*)CRSensorIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CRSensorDesc>;
	db->RegisterDesc(desc);
	Spr::CROpticalSensorDesc* pCROpticalSensorDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CROpticalSensorDesc");
	desc->size = sizeof(Spr::CROpticalSensorDesc);
	desc->ifInfo = CROpticalSensorIf::GetIfInfoStatic();
	((IfInfo*)CROpticalSensorIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CROpticalSensorDesc>;
	field = desc->AddBase("CRSensorDesc");
	field->offset = int((char*)(CRSensorDesc*)pCROpticalSensorDesc - (char*)pCROpticalSensorDesc);
	db->RegisterDesc(desc);
	CRFLAnimalGeneData* pCRFLAnimalGeneData = NULL;
	desc = DBG_NEW UTTypeDesc("CRFLAnimalGeneData");
	desc->size = sizeof(CRFLAnimalGeneData);
	desc->access = DBG_NEW UTAccess<CRFLAnimalGeneData>;
	field = desc->AddField("", "Quaterniond", "goalDir", "");
	field->offset = int((char*)&(pCRFLAnimalGeneData->goalDir) - (char*)pCRFLAnimalGeneData);
	field = desc->AddField("$unnamed1$", "enum", "geneType", "");
	field->offset = int((char*)&(pCRFLAnimalGeneData->geneType) - (char*)pCRFLAnimalGeneData);
	db->RegisterDesc(desc);
}

SPR_IFIMP1(CRBody, SceneObject);
void Spr::CRBodyIf::Init(){
	((CRBody*)(Object*)(ObjectIf*)this)->Init();
}
int Spr::CRBodyIf::NSolids(){
	return	((CRBody*)(Object*)(ObjectIf*)this)->NSolids();
}
Spr::PHSolidIf* Spr::CRBodyIf::GetSolid(int i){
	return	((CRBody*)(Object*)(ObjectIf*)this)->GetSolid(i);
}
int Spr::CRBodyIf::NJoints(){
	return	((CRBody*)(Object*)(ObjectIf*)this)->NJoints();
}
Spr::PHJointIf* Spr::CRBodyIf::GetJoint(int i){
	return	((CRBody*)(Object*)(ObjectIf*)this)->GetJoint(i);
}
Spr::CRIKControlIf* Spr::CRBodyIf::CreateIKControl(const IfInfo* ii, const Spr::CRIKControlDesc& desc){
	return	((CRBody*)(Object*)(ObjectIf*)this)->CreateIKControl(ii, desc);
}
Spr::CRIKMovableIf* Spr::CRBodyIf::CreateIKMovable(const IfInfo* ii, const Spr::CRIKMovableDesc& desc){
	return	((CRBody*)(Object*)(ObjectIf*)this)->CreateIKMovable(ii, desc);
}
void Spr::CRBodyIf::CalcIK(){
	((CRBody*)(Object*)(ObjectIf*)this)->CalcIK();
}
Vec3d Spr::CRBodyIf::GetCenterOfMass(){
	return	((CRBody*)(Object*)(ObjectIf*)this)->GetCenterOfMass();
}
double Spr::CRBodyIf::GetSumOfMass(){
	return	((CRBody*)(Object*)(ObjectIf*)this)->GetSumOfMass();
}
SPR_IFIMP1(CRHingeHumanBody, CRBody);
void Spr::CRHingeHumanBodyIf::SetUpperBodyStiffness(float stiffness){
	((CRHingeHumanBody*)(Object*)(ObjectIf*)this)->SetUpperBodyStiffness(stiffness);
}
void Spr::CRHingeHumanBodyIf::KeepUpperBodyPose(){
	((CRHingeHumanBody*)(Object*)(ObjectIf*)this)->KeepUpperBodyPose();
}
void Spr::CRHingeHumanBodyIf::ResetUpperBodyPose(){
	((CRHingeHumanBody*)(Object*)(ObjectIf*)this)->ResetUpperBodyPose();
}
SPR_IFIMP1(CRFourLegsAnimalBody, CRBody);
void Spr::CRFourLegsAnimalBodyIf::Init(){
	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->Init();
}
Vec3d Spr::CRFourLegsAnimalBodyIf::GetUpperCenterOfMass(){
	return	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->GetUpperCenterOfMass();
}
int Spr::CRFourLegsAnimalBodyIf::NSolids(){
	return	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->NSolids();
}
int Spr::CRFourLegsAnimalBodyIf::NJoints(){
	return	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->NJoints();
}
int Spr::CRFourLegsAnimalBodyIf::NBallJoints(){
	return	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->NBallJoints();
}
int Spr::CRFourLegsAnimalBodyIf::NHingeJoints(){
	return	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->NHingeJoints();
}
double Spr::CRFourLegsAnimalBodyIf::VSolid(int i){
	return	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->VSolid(i);
}
double Spr::CRFourLegsAnimalBodyIf::VSolids(){
	return	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->VSolids();
}
double Spr::CRFourLegsAnimalBodyIf::GetTotalMass(){
	return	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->GetTotalMass();
}
void Spr::CRFourLegsAnimalBodyIf::SetTotalMass(double value){
	((CRFourLegsAnimalBody*)(Object*)(ObjectIf*)this)->SetTotalMass(value);
}
SPR_IFIMP1(CRTrunkFootHumanBody, CRBody);
SPR_IFIMP1(CRTrunkFootAnimalBody, CRBody);
SPR_IFIMP1(CRController, SceneObject);
void Spr::CRControllerIf::Init(){
	((CRController*)(Object*)(ObjectIf*)this)->Init();
}
void Spr::CRControllerIf::Step(){
	((CRController*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::CRControllerIf::SetEnable(bool enable){
	((CRController*)(Object*)(ObjectIf*)this)->SetEnable(enable);
}
bool Spr::CRControllerIf::IsEnabled(){
	return	((CRController*)(Object*)(ObjectIf*)this)->IsEnabled();
}
SPR_IFIMP1(CREyeController, CRController);
void Spr::CREyeControllerIf::LookAt(Vec3f pos, Vec3f vel){
	((CREyeController*)(Object*)(ObjectIf*)this)->LookAt(pos, vel);
}
bool Spr::CREyeControllerIf::IsSaccading(){
	return	((CREyeController*)(Object*)(ObjectIf*)this)->IsSaccading();
}
SPR_IFIMP1(CRNeckController, CRController);
void Spr::CRNeckControllerIf::LookAt(Vec3f pos, Vec3f vel, float attractiveness){
	((CRNeckController*)(Object*)(ObjectIf*)this)->LookAt(pos, vel, attractiveness);
}
SPR_IFIMP1(CRReachingController, CRController);
Spr::PHSolidIf* Spr::CRReachingControllerIf::GetSolid(){
	return	((CRReachingController*)(Object*)(ObjectIf*)this)->GetSolid();
}
void Spr::CRReachingControllerIf::SetTargetPos(Vec3f p, Vec3f v){
	((CRReachingController*)(Object*)(ObjectIf*)this)->SetTargetPos(p, v);
}
void Spr::CRReachingControllerIf::SetTargetOri(Quaterniond q, Vec3f av){
	((CRReachingController*)(Object*)(ObjectIf*)this)->SetTargetOri(q, av);
}
void Spr::CRReachingControllerIf::SetTargetTime(float t){
	((CRReachingController*)(Object*)(ObjectIf*)this)->SetTargetTime(t);
}
void Spr::CRReachingControllerIf::Start(Spr::CRReachingControllerIf::ConstraintMode mode, float keeptime){
	((CRReachingController*)(Object*)(ObjectIf*)this)->Start(mode, keeptime);
}
float Spr::CRReachingControllerIf::GetRemainingTime(){
	return	((CRReachingController*)(Object*)(ObjectIf*)this)->GetRemainingTime();
}
Spr::CRReachingControllerIf::ReachState Spr::CRReachingControllerIf::GetReachState(){
	return	((CRReachingController*)(Object*)(ObjectIf*)this)->GetReachState();
}
void Spr::CRReachingControllerIf::Reset(){
	((CRReachingController*)(Object*)(ObjectIf*)this)->Reset();
}
SPR_IFIMP1(CRGazeController, CRController);
void Spr::CRGazeControllerIf::LookAt(Vec3f pos, Vec3f vel, float attractiveness){
	((CRGazeController*)(Object*)(ObjectIf*)this)->LookAt(pos, vel, attractiveness);
}
SPR_IFIMP1(CRAttentionController, CRController);
SPR_IFIMP1(CRTryStandingUpController, CRController);
void Spr::CRTryStandingUpControllerIf::Init(){
	((CRTryStandingUpController*)(Object*)(ObjectIf*)this)->Init();
}
void Spr::CRTryStandingUpControllerIf::Step(){
	((CRTryStandingUpController*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::CRTryStandingUpControllerIf::Sync(){
	((CRTryStandingUpController*)(Object*)(ObjectIf*)this)->Sync();
}
SPR_IFIMP1(CRWalkingController, CRController);
void Spr::CRWalkingControllerIf::SetSpeed(float speed){
	((CRWalkingController*)(Object*)(ObjectIf*)this)->SetSpeed(speed);
}
void Spr::CRWalkingControllerIf::SetRotationAngle(float rot){
	((CRWalkingController*)(Object*)(ObjectIf*)this)->SetRotationAngle(rot);
}
void Spr::CRWalkingControllerIf::SetRotationWorldCoordinate(double r){
	((CRWalkingController*)(Object*)(ObjectIf*)this)->SetRotationWorldCoordinate(r);
}
void Spr::CRWalkingControllerIf::Stop(){
	((CRWalkingController*)(Object*)(ObjectIf*)this)->Stop();
}
void Spr::CRWalkingControllerIf::Reverse(){
	((CRWalkingController*)(Object*)(ObjectIf*)this)->Reverse();
}
void Spr::CRWalkingControllerIf::SetPos(Vec3f pos){
	((CRWalkingController*)(Object*)(ObjectIf*)this)->SetPos(pos);
}
double Spr::CRWalkingControllerIf::GetBasicCycle(){
	return	((CRWalkingController*)(Object*)(ObjectIf*)this)->GetBasicCycle();
}
bool Spr::CRWalkingControllerIf::IsCompleteFall(){
	return	((CRWalkingController*)(Object*)(ObjectIf*)this)->IsCompleteFall();
}
SPR_IFIMP1(CREseWalkingController, CRController);
void Spr::CREseWalkingControllerIf::SetSpeed(float speed){
	((CREseWalkingController*)(Object*)(ObjectIf*)this)->SetSpeed(speed);
}
void Spr::CREseWalkingControllerIf::SetRotationAngle(float rot){
	((CREseWalkingController*)(Object*)(ObjectIf*)this)->SetRotationAngle(rot);
}
void Spr::CREseWalkingControllerIf::SetPos(Vec3f pos){
	((CREseWalkingController*)(Object*)(ObjectIf*)this)->SetPos(pos);
}
SPR_IFIMP1(CRTravelController, CRController);
void Spr::CRTravelControllerIf::SetGoal(Vec3f goal){
	((CRTravelController*)(Object*)(ObjectIf*)this)->SetGoal(goal);
}
SPR_IFIMP1(CRGrabController, CRController);
bool Spr::CRGrabControllerIf::Reach(Spr::PHSolidIf* solid, float radius){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->Reach(solid, radius);
}
bool Spr::CRGrabControllerIf::IsReachable(Spr::PHSolidIf* solid){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->IsReachable(solid);
}
bool Spr::CRGrabControllerIf::IsReachable(Spr::PHSolidIf* solid, float safety){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->IsReachable(solid, safety);
}
bool Spr::CRGrabControllerIf::IsReachComplete(){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->IsReachComplete();
}
bool Spr::CRGrabControllerIf::Uphold(){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->Uphold();
}
bool Spr::CRGrabControllerIf::IsUpholdable(){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->IsUpholdable();
}
bool Spr::CRGrabControllerIf::IsUpholdComplete(){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->IsUpholdComplete();
}
bool Spr::CRGrabControllerIf::Place(Vec3d pos){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->Place(pos);
}
bool Spr::CRGrabControllerIf::IsPlaceable(Vec3d pos){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->IsPlaceable(pos);
}
bool Spr::CRGrabControllerIf::IsPlaceable(Vec3d pos, float safety){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->IsPlaceable(pos, safety);
}
bool Spr::CRGrabControllerIf::IsPlaceComplete(){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->IsPlaceComplete();
}
void Spr::CRGrabControllerIf::Abort(){
	((CRGrabController*)(Object*)(ObjectIf*)this)->Abort();
}
void Spr::CRGrabControllerIf::AbortAll(){
	((CRGrabController*)(Object*)(ObjectIf*)this)->AbortAll();
}
Spr::CRGrabControllerIf::CRGCControlState Spr::CRGrabControllerIf::GetControlState(){
	return	((CRGrabController*)(Object*)(ObjectIf*)this)->GetControlState();
}
SPR_IFIMP1(CRCreature, SceneObject);
void Spr::CRCreatureIf::Init(){
	((CRCreature*)(Object*)(ObjectIf*)this)->Init();
}
void Spr::CRCreatureIf::Step(){
	((CRCreature*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::CRCreatureIf::ClearInternalScene(){
	((CRCreature*)(Object*)(ObjectIf*)this)->ClearInternalScene();
}
void Spr::CRCreatureIf::SensorStep(){
	((CRCreature*)(Object*)(ObjectIf*)this)->SensorStep();
}
void Spr::CRCreatureIf::InternalSceneStep(){
	((CRCreature*)(Object*)(ObjectIf*)this)->InternalSceneStep();
}
void Spr::CRCreatureIf::ControllerStep(){
	((CRCreature*)(Object*)(ObjectIf*)this)->ControllerStep();
}
Spr::CRBodyIf* Spr::CRCreatureIf::CreateBody(const IfInfo* ii, const Spr::CRBodyDesc& desc){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->CreateBody(ii, desc);
}
Spr::CRBodyIf* Spr::CRCreatureIf::GetBody(int i){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->GetBody(i);
}
int Spr::CRCreatureIf::NBodies(){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->NBodies();
}
Spr::CRSensorIf* Spr::CRCreatureIf::CreateSensor(const IfInfo* ii, const Spr::CRSensorDesc& desc){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->CreateSensor(ii, desc);
}
Spr::CRSensorIf* Spr::CRCreatureIf::GetSensor(int i){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->GetSensor(i);
}
int Spr::CRCreatureIf::NSensors(){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->NSensors();
}
Spr::CRControllerIf* Spr::CRCreatureIf::CreateController(const IfInfo* ii, const Spr::CRControllerDesc& desc){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->CreateController(ii, desc);
}
Spr::CRControllerIf* Spr::CRCreatureIf::GetController(int i){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->GetController(i);
}
int Spr::CRCreatureIf::NControllers(){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->NControllers();
}
Spr::CRInternalSceneIf* Spr::CRCreatureIf::CreateInternalScene(const Spr::CRInternalSceneDesc& desc){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->CreateInternalScene(desc);
}
Spr::CRInternalSceneIf* Spr::CRCreatureIf::GetInternalScene(){
	return	((CRCreature*)(Object*)(ObjectIf*)this)->GetInternalScene();
}
SPR_IFIMP1(CRInternalSceneObject, SceneObject);
bool Spr::CRInternalSceneObjectIf::IsA(const char* typestr){
	return	((CRInternalSceneObject*)(Object*)(ObjectIf*)this)->IsA(typestr);
}
const char* Spr::CRInternalSceneObjectIf::GetISObjType(){
	return	((CRInternalSceneObject*)(Object*)(ObjectIf*)this)->GetISObjType();
}
Spr::PHSolidIf* Spr::CRInternalSceneObjectIf::GetSolid(){
	return	((CRInternalSceneObject*)(Object*)(ObjectIf*)this)->GetSolid();
}
Vec3f Spr::CRInternalSceneObjectIf::GetPos(){
	return	((CRInternalSceneObject*)(Object*)(ObjectIf*)this)->GetPos();
}
void Spr::CRInternalSceneObjectIf::SetPos(Vec3d pos){
	((CRInternalSceneObject*)(Object*)(ObjectIf*)this)->SetPos(pos);
}
SPR_IFIMP1(CRISAttractiveObject, CRInternalSceneObject);
float Spr::CRISAttractiveObjectIf::GetTotalAttractiveness(){
	return	((CRISAttractiveObject*)(Object*)(ObjectIf*)this)->GetTotalAttractiveness();
}
void Spr::CRISAttractiveObjectIf::AddBottomupAttr(float attr){
	((CRISAttractiveObject*)(Object*)(ObjectIf*)this)->AddBottomupAttr(attr);
}
void Spr::CRISAttractiveObjectIf::ClearBottomupAttr(){
	((CRISAttractiveObject*)(Object*)(ObjectIf*)this)->ClearBottomupAttr();
}
void Spr::CRISAttractiveObjectIf::SetTopdownAttr(float attr){
	((CRISAttractiveObject*)(Object*)(ObjectIf*)this)->SetTopdownAttr(attr);
}
void Spr::CRISAttractiveObjectIf::IncUncertainty(){
	((CRISAttractiveObject*)(Object*)(ObjectIf*)this)->IncUncertainty();
}
void Spr::CRISAttractiveObjectIf::DecUncertainty(){
	((CRISAttractiveObject*)(Object*)(ObjectIf*)this)->DecUncertainty();
}
float Spr::CRISAttractiveObjectIf::GetUncertainty(){
	return	((CRISAttractiveObject*)(Object*)(ObjectIf*)this)->GetUncertainty();
}
SPR_IFIMP1(CRISTravelPotentialObject, CRInternalSceneObject);
Vec2f Spr::CRISTravelPotentialObjectIf::GetStrengthCoeff(){
	return	((CRISTravelPotentialObject*)(Object*)(ObjectIf*)this)->GetStrengthCoeff();
}
void Spr::CRISTravelPotentialObjectIf::SetStrengthCoeff(Vec2f strength){
	((CRISTravelPotentialObject*)(Object*)(ObjectIf*)this)->SetStrengthCoeff(strength);
}
Vec2f Spr::CRISTravelPotentialObjectIf::GetDecayCoeff(){
	return	((CRISTravelPotentialObject*)(Object*)(ObjectIf*)this)->GetDecayCoeff();
}
void Spr::CRISTravelPotentialObjectIf::SetDecayCoeff(Vec2f decay){
	((CRISTravelPotentialObject*)(Object*)(ObjectIf*)this)->SetDecayCoeff(decay);
}
Vec2f Spr::CRISTravelPotentialObjectIf::GetPotential(Vec2f currPos){
	return	((CRISTravelPotentialObject*)(Object*)(ObjectIf*)this)->GetPotential(currPos);
}
SPR_IFIMP1(CRInternalScene, SceneObject);
void Spr::CRInternalSceneIf::Step(){
	((CRInternalScene*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::CRInternalSceneIf::ClearAttractiveness(){
	((CRInternalScene*)(Object*)(ObjectIf*)this)->ClearAttractiveness();
}
void Spr::CRInternalSceneIf::SortByAttractiveness(){
	((CRInternalScene*)(Object*)(ObjectIf*)this)->SortByAttractiveness();
}
Spr::CRInternalSceneObjectIf* Spr::CRInternalSceneIf::FindObject(Spr::PHSolidIf* solid, Vec3f pos){
	return	((CRInternalScene*)(Object*)(ObjectIf*)this)->FindObject(solid, pos);
}
Spr::CRInternalSceneObjectIf* Spr::CRInternalSceneIf::GetISObject(int i){
	return	((CRInternalScene*)(Object*)(ObjectIf*)this)->GetISObject(i);
}
int Spr::CRInternalSceneIf::NObjects(){
	return	((CRInternalScene*)(Object*)(ObjectIf*)this)->NObjects();
}
Spr::CRInternalSceneObjectIf* Spr::CRInternalSceneIf::CreateInternalSceneObject(const IfInfo* ii, const Spr::CRInternalSceneObjectDesc& desc){
	return	((CRInternalScene*)(Object*)(ObjectIf*)this)->CreateInternalSceneObject(ii, desc);
}
SPR_IFIMP1(CRIKControl, SceneObject);
void Spr::CRIKControlIf::SetGoal(Vec3d goal){
	((CRIKControl*)(Object*)(ObjectIf*)this)->SetGoal(goal);
}
SPR_IFIMP1(CRIKControlPos, CRIKControl);
SPR_IFIMP1(CRIKControlOri, CRIKControl);
SPR_IFIMP1(CRIKMovable, SceneObject);
void Spr::CRIKMovableIf::PrepareSolve(){
	((CRIKMovable*)(Object*)(ObjectIf*)this)->PrepareSolve();
}
void Spr::CRIKMovableIf::ProceedSolve(){
	((CRIKMovable*)(Object*)(ObjectIf*)this)->ProceedSolve();
}
PTM::VVector< double > Spr::CRIKMovableIf::GetValue(){
	return	((CRIKMovable*)(Object*)(ObjectIf*)this)->GetValue();
}
void Spr::CRIKMovableIf::Move(){
	((CRIKMovable*)(Object*)(ObjectIf*)this)->Move();
}
void Spr::CRIKMovableIf::AddIKControl(Spr::CRIKControlIf* control){
	((CRIKMovable*)(Object*)(ObjectIf*)this)->AddIKControl(control);
}
SPR_IFIMP1(CRIKMovableSolidPos, CRIKMovable);
SPR_IFIMP1(CRIKMovableSolidOri, CRIKMovable);
SPR_IFIMP1(CRIKMovableBallJointOri, CRIKMovable);
SPR_IFIMP1(CRIKMovable3HingeJointOri, CRIKMovable);
SPR_IFIMP1(CRIKMovableHingeJointOri, CRIKMovable);
SPR_IFIMP1(CRSensor, SceneObject);
void Spr::CRSensorIf::Init(){
	((CRSensor*)(Object*)(ObjectIf*)this)->Init();
}
void Spr::CRSensorIf::Step(){
	((CRSensor*)(Object*)(ObjectIf*)this)->Step();
}
SPR_IFIMP1(CROpticalSensor, CRSensor);
bool Spr::CROpticalSensorIf::IsVisible(Spr::PHSolidIf* solid){
	return	((CROpticalSensor*)(Object*)(ObjectIf*)this)->IsVisible(solid);
}
bool Spr::CROpticalSensorIf::IsInCenter(Spr::PHSolidIf* solid){
	return	((CROpticalSensor*)(Object*)(ObjectIf*)this)->IsInCenter(solid);
}
bool Spr::CROpticalSensorIf::IsVisible(Vec3f pos){
	return	((CROpticalSensor*)(Object*)(ObjectIf*)this)->IsVisible(pos);
}
bool Spr::CROpticalSensorIf::IsInCenter(Vec3f pos){
	return	((CROpticalSensor*)(Object*)(ObjectIf*)this)->IsInCenter(pos);
}
bool Spr::CROpticalSensorIf::IsSelfSolid(Spr::PHSolidIf* solid){
	return	((CROpticalSensor*)(Object*)(ObjectIf*)this)->IsSelfSolid(solid);
}
SPR_OBJECTIMP1(CRAttentionController, CRController);
SPR_OBJECTIMP1(CRBody, SceneObject);
SPR_OBJECTIMP1(CRController, SceneObject);
SPR_OBJECTIMP1(CRCreature, SceneObject);
SPR_OBJECTIMP1(CREseWalkingController, CRController);
SPR_OBJECTIMP1(CREyeController, CRController);
SPR_OBJECTIMP1(CRFourLegsAnimalBody, CRBody);
SPR_OBJECTIMP1(CRGazeController, CRController);
SPR_OBJECTIMP1(CRGrabController, CRController);
SPR_OBJECTIMP1(CRHingeHumanBody, CRBody);
SPR_OBJECTIMP1(CRInternalSceneObject, SceneObject);
SPR_OBJECTIMP1(CRISAttractiveObject, CRInternalSceneObject);
SPR_OBJECTIMP1(CRISTravelPotentialObject, CRInternalSceneObject);
SPR_OBJECTIMP1(CRInternalScene, SceneObject);
SPR_OBJECTIMP1(CRIKControl, SceneObject);
SPR_OBJECTIMP1(CRIKControlPos, CRIKControl);
SPR_OBJECTIMP1(CRIKControlOri, CRIKControl);
SPR_OBJECTIMP1(CRIKMovable, SceneObject);
SPR_OBJECTIMP1(CRIKMovableSolidPos, CRIKMovable);
SPR_OBJECTIMP1(CRIKMovableSolidOri, CRIKMovable);
SPR_OBJECTIMP1(CRIKMovableBallJointOri, CRIKMovable);
SPR_OBJECTIMP1(CRIKMovable3HingeJointOri, CRIKMovable);
SPR_OBJECTIMP1(CRIKMovableHingeJointOri, CRIKMovable);
SPR_OBJECTIMP1(CRNeckController, CRController);
SPR_OBJECTIMP1(CROpticalSensor, CRSensor);
SPR_OBJECTIMP1(CRReachingController, CRController);
SPR_OBJECTIMP1(CRSensor, SceneObject);
SPR_OBJECTIMP1(CRTravelController, CRController);
SPR_OBJECTIMP1(CRTrunkFootAnimalBody, CRBody);
SPR_OBJECTIMP1(CRTrunkFootHumanBody, CRBody);
SPR_OBJECTIMP1(CRTryStandingUpController, CRController);
SPR_OBJECTIMP1(CRWalkingController, CRController);
}

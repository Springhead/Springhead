/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRCreature.h"

#include "CRSensor.h"
#include "CROpticalSensor.h"

#include "CRController.h"
#include "CRAttentionController.h"
#include "CREyeController.h"
#include "CRGazeController.h"
#include "CRNeckController.h"
#include "CRReachingController.h"
#include "CRWalkingController.h"

#include "CRBody.h"
#include "CRHingeHumanBody.h"

#include "CRInternalScene.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRCreature, SceneObject);

void CRCreature::Init(){
	body->Init();

	for (int i=0; i<sensors.size(); i++){
		sensors[i]->Init();
	}

	for (int i=0; i<controllers.size(); i++){
		controllers[i]->Init();
	}
}

void CRCreature::Step(){
	for (int i=0; i<sensors.size(); i++){
		sensors[i]->Step();
	}

	for (int i=0; i<controllers.size(); i++){
		controllers[i]->Step();
	}
}

CRBodyIf* CRCreature::CreateBody(const IfInfo* ii, const CRBodyDesc& desc){
	if (ii == CRHingeHumanBodyIf::GetIfInfoStatic()) {
		body = (DBG_NEW CRHingeHumanBody((const CRHingeHumanBodyDesc&)desc, this->Cast()))->Cast();

	} else {
		assert(0 && "‘z’è‚³‚ê‚Ä‚È‚¢Œ^");
		return NULL;

	}

	return body;
}

CRBodyIf* CRCreature::GetBody(){
	return body;
}

CRSensorIf* CRCreature::CreateSensor(const IfInfo* ii, const CRSensorDesc& desc){
	if (ii == CROpticalSensorIf::GetIfInfoStatic()) {
		sensors.push_back((DBG_NEW CROpticalSensor((const CROpticalSensorDesc&)desc, this->Cast()))->Cast());

	} else {
		assert(0 && "‘z’è‚³‚ê‚Ä‚È‚¢Œ^");
		return NULL;

	}

	return sensors.back();
}

CRSensorIf* CRCreature::GetSensor(int i){
	return sensors[i];
}

int CRCreature::NSensors(){
	return sensors.size();
}

CRControllerIf* CRCreature::CreateController(const IfInfo* ii, const CRControllerDesc& desc){
	if (ii == CRAttentionControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRAttentionController((const CRAttentionControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRGazeControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRGazeController((const CRGazeControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRNeckControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRNeckController((const CRNeckControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CREyeControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CREyeController((const CREyeControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRReachingControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRReachingController((const CRReachingControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRWalkingControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRWalkingController((const CRWalkingControllerDesc&)desc, this->Cast()))->Cast());

	} else {
		assert(0 && "‘z’è‚³‚ê‚Ä‚È‚¢Œ^");

	}

	return controllers.back();
}

CRControllerIf* CRCreature::GetController(int i){
	return controllers[i];
}

int CRCreature::NControllers(){
	return controllers.size();
}

CRInternalSceneIf* CRCreature::CreateInternalScene(const CRInternalSceneDesc& desc){
	internalScene = (DBG_NEW CRInternalScene((const CRInternalSceneDesc&)desc, this->Cast()))->Cast();
	return internalScene;
}

CRInternalSceneIf* CRCreature::GetInternalScene(){
	return internalScene;
}

}

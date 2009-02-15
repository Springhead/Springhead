/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRCreature.h"

#include "CRSensor.h"
#include "CROpticalSensor.h"
#include "CRTouchSensor.h"

#include "CRController.h"
#include "CRAttentionController.h"
#include "CREyeController.h"
#include "CRGazeController.h"
#include "CRNeckController.h"
#include "CRReachingController.h"
#include "CRWalkingController.h"
#include "CREseWalkingController.h"
#include "CRTravelController.h"
#include "CRGrabController.h"
#include "CRTryStandingUpController.h"

#include "CRBody.h"
#include "CRHingeHumanBody.h"
#include "CRBallHumanBody.h"
#include "CRFourLegsAnimalBody.h"
#include "CRTrunkFootHumanBody.h"
#include "CRFourLegsTinyAnimalBody.h"
#include "CRDebugLinkBody.h"

#include "CRInternalScene.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 


void CRCreature::Init(){
	// 将来的には依存するセンサ・コントローラを先にInitするようコードしたほうがよさそうだ．(07/09/11, mitake)

	for (unsigned int i=0; i<body.size(); ++i){
		body[i]->Init();
	}

	for (unsigned int i=0; i<sensors.size(); ++i){
		sensors[i]->Init();
	}

	for (unsigned int i=0; i<controllers.size(); ++i){
		controllers[i]->Init();
	}
}

void CRCreature::Step(){
	ClearInternalScene();
	SensorStep();
	InternalSceneStep();
	ControllerStep();
}

void CRCreature::ClearInternalScene(){
	if (internalScene) {
		internalScene->ClearAttractiveness();
	}
}

void CRCreature::SensorStep(){
	for (unsigned int i=0; i<sensors.size(); i++){
		sensors[i]->Step();
	}
}

void CRCreature::InternalSceneStep(){
	if (internalScene) {
		internalScene->Step();
	}
}

void CRCreature::ControllerStep(){
	for (unsigned int i=0; i<controllers.size(); i++){
		if (controllers[i]->IsEnabled()) {
			controllers[i]->Step();
		}
	}
}

CRBodyIf* CRCreature::CreateBody(const IfInfo* ii, const CRBodyDesc& desc){
	if (ii == CRHingeHumanBodyIf::GetIfInfoStatic()) {
		body.push_back((DBG_NEW CRHingeHumanBody((const CRHingeHumanBodyDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRBallHumanBodyIf::GetIfInfoStatic()) {
		body.push_back((DBG_NEW CRBallHumanBody((const CRBallHumanBodyDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRTrunkFootHumanBodyIf::GetIfInfoStatic()) {
		body.push_back((DBG_NEW CRTrunkFootHumanBody((const CRTrunkFootHumanBodyDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRFourLegsAnimalBodyIf::GetIfInfoStatic()) {
		body.push_back((DBG_NEW CRFourLegsAnimalBody((const CRFourLegsAnimalBodyDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRFourLegsTinyAnimalBodyIf::GetIfInfoStatic()) {
		body.push_back((DBG_NEW CRFourLegsTinyAnimalBody((const CRFourLegsTinyAnimalBodyDesc&)desc, this->Cast()))->Cast());

	} else if(ii == CRDebugLinkBodyIf::GetIfInfoStatic()){
		body.push_back((DBG_NEW CRDebugLinkBody((const CRDebugLinkBodyDesc&)desc, this->Cast()))->Cast());

	}else {
		assert(0 && "想定されてない型");
		return NULL;

	}

	return body.back();
}

CRBodyIf* CRCreature::GetBody(int i){
	return body[i];
}

int CRCreature::NBodies(){
	return body.size();
}

CRSensorIf* CRCreature::CreateSensor(const IfInfo* ii, const CRSensorDesc& desc){
	if (ii == CROpticalSensorIf::GetIfInfoStatic()) {
		sensors.push_back((DBG_NEW CROpticalSensor((const CROpticalSensorDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRTouchSensorIf::GetIfInfoStatic()) {
		sensors.push_back((DBG_NEW CRTouchSensor((const CRTouchSensorDesc&)desc, this->Cast()))->Cast());

	} else {
		assert(0 && "想定されてない型");
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

	} else if (ii == CRReachingControllersIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRReachingControllers((const CRReachingControllersDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRWalkingControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRWalkingController((const CRWalkingControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CREseWalkingControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CREseWalkingController((const CREseWalkingControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRTravelControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRTravelController((const CRTravelControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRGrabControllerIf::GetIfInfoStatic()) {
		controllers.push_back((DBG_NEW CRGrabController((const CRGrabControllerDesc&)desc, this->Cast()))->Cast());

	} else if (ii == CRTryStandingUpControllerIf::GetIfInfoStatic()){
		controllers.push_back((DBG_NEW CRTryStandingUpController((const CRTryStandingUpControllerDesc&)desc, this->Cast()))->Cast());
	} else {
		assert(0 && "想定されてない型");

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

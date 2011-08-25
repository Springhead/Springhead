/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRSdk.h>
#include <Creature/CRScene.h>
#include <Creature/CRCreature.h>
#include <Creature/CRBody.h>
#include <Creature/CRBodyPart.h>
// #include <Creature/CRBodyController.h>

#include <Creature/CRTouchSensor.h>
#include <Creature/CRVisualSensor.h>

#include <Creature/CREyeController.h>
#include <Creature/CRNeckController.h>
#include <Creature/CRGazeController.h>
// #include <Creature/CRAttentionController.h>
#include <Creature/CRReachingController.h>
#include <Creature/CRTrajectoryController.h>
#include <Creature/CRTrajectory.h>
#include <Creature/CRGrabController.h>

using namespace std;

namespace Spr{;

void SPR_CDECL CRRegisterTypeDescs();
void SPR_CDECL CRSdkIf::RegisterSdk(){
	static bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;
	CRRegisterTypeDescs();

	CRSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRCreature));

	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRBody));
	// CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRBodyController));

	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRTouchSensor));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRVisualSensor));

	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CREyeController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRNeckController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRGazeController));
	// CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRAttentionController));

	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRReachingController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRReachingControllers));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRGrabController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRTrajectoryController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRTrajectory));

	/*
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRWalkController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CREseWalkController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRTravelController));
	*/

	CRBodyIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRSolid));
	CRBodyIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRIKSolid));
	CRBodyIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRJoint));
	CRBodyIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRIKJoint));
}

CRSdkIf* SPR_CDECL CRSdkIf::CreateSdk() {
	CRSdk* rv = DBG_NEW CRSdk;
	CRSdkIf::RegisterSdk();
	return rv->Cast();
}

//----------------------------------------------------------------------------
//	CRSdk

CRSdk::CRSdk() {
	name = "crSdk";
}

CRSdk::~CRSdk() {
}

CRCreatureIf* CRSdk::CreateCreature(const IfInfo* ii, const CRCreatureDesc& desc){
	CRCreatureIf* creature = CreateObject(ii, &desc)->Cast();
	AddChildObject(creature);
	return creature;
}

void CRSdk::Step() {
	for (size_t i=0; i<creatures.size(); ++i) {
		creatures[i]->Step();
	}
}

void CRSdk::Clear(){
	Sdk::Clear();
	creatures.clear();
}

bool CRSdk::AddChildObject(ObjectIf* o){
	CRCreatureIf* c = DCAST(CRCreatureIf, o);
	if (c){
		if (std::find(creatures.begin(), creatures.end(), c) == creatures.end()){
			creatures.push_back(c);
			return true;
		}
	}
	return false;
}

bool CRSdk::DelChildObject(ObjectIf* o){
	CRCreatureIf* c = DCAST(CRCreatureIf, o);
	if(c){
		CRCreatures::iterator it = std::find(creatures.begin(), creatures.end(), c);
		if(it != creatures.end()){
			creatures.erase(it);
			return true;
		}
	}
	return false;
}

}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRSdk.h>
#include <Creature/CRCreature.h>
#include <Creature/CRBody.h>
#include <Creature/CRBone.h>

#include <Creature/CRTouchSensor.h>
#include <Creature/CRVisualSensor.h>

#include <Creature/CRGazeController.h>
#include <Creature/CRReachController.h>
#include <Creature/CRGrabController.h>

#include <Creature/CRTrajectoryPlanner.h>

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
	CRBodyIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRBone));

	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRTouchSensor));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRVisualSensor));

	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRReachController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRTrajectoryPlanner));

	// 使っているのだろうか？ <!!>
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRGazeController));
	CRCreatureIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(CRGrabController));
}

CRSdkIf* SPR_CDECL CRSdkIf::CreateSdk() {
	CRSdk* rv = DBG_NEW CRSdk;
	CRSdkIf::RegisterSdk();
	return rv->Cast();
}

CRSdkIf* CRSdk::crSdkInstance = NULL;
CRSdkIf* SPR_CDECL CRSdkIf::GetSdk() {
	if (CRSdk::crSdkInstance == NULL) {
		CRSdk::crSdkInstance = CreateSdk();
	}
	return CRSdk::crSdkInstance;
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
		//std::cout << i << " : " << creatures[i] << std::endl;
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

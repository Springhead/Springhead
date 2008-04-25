#include "CRManipulator.h"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Cosntructer, Destructer
CRManipulator::CRManipulator(){
	;
}

CRManipulator::CRManipulator(PHSceneIf* scene){
	phScene = scene;
	Init();
}

CRManipulator::~CRManipulator(){
	;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Functions In This Class
bool CRManipulator::Init(){
	
	if(CreateHand()){
	}
	else return false;

	return true;
}

bool CRManipulator::CreateHand(){

	if(CreateThumb()){
		
	}
	else return false;
	if(CreateIndexFinger()){
	
	}
	else return false;
	if(CreateLongFinger()){
	
	}
	else return false;
	if(CreateAnnularFinger()){
	
	}
	else return false;
	if(CreateDigitusFinger()){
	
	}
	else return false;

	return true;
}

bool CRManipulator::CreateThumb(){

	PHSolidDesc			sDesc;
	PHHingeJointDesc	hjDesc;
	PHBallJointDesc		bjDesc;

	phScene->CreateObject(PHSolidIf, sDesc);
	phScene->CreateObject(PHSolidIf, sDesc);

	return true;
}

bool CRManipulator::CreateIndexFinger(){
	return true;
}

bool CRManipulator::CreateLongFinger(){
	return true;
}

bool CRManipulator::CreateAnnularFinger(){
	return true;
}

bool CRManipulator::CreateDigitusFinger(){
	return true;
}

void CRManipulator::Grip(){
	;
}

void CRManipulator::Release(){
	;
}

void CRManipulator::Rock(){
	;
}

void CRManipulator::Paper(){
	;
}

void CRManipulator::Scissors(){
	;
}
#include <Springhead.h>
#include "CRManipulator.h"

using namespace Spr;
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Functions In This Class

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

	phScene->CreateSolid(sDesc);
	phScene->CreateSolid(sDesc);

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
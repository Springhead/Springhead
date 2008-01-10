/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef LOADDATAFROMGUI_H
#define LOADDATAFROMGUI_H

#include <Springhead.h>
#include <iostream>
#include <gl/glui.h>
#include "MyGlobalValue.h"

/// Load Data ----------------------------------------------------------------------------
void guiLoadSolid(int control){
	
	DSTR << "nowSolid: " << nowSolid << endl;
	
	dynamicalValue = (int) animalBody->GetSolid(CRFourLegsAnimalBodyDesc::SO_WAIST)->IsDynamical();

	defaultBoxsize = box[nowSolid]->GetBoxSize();
	boxsize = defaultBoxsize;

	defaultMass    = solids[nowSolid]->GetMass(); 
	mass = defaultMass;
	glui->sync_live();
}

void guiLoadBallJoint(int control){	
	DSTR << "nowBallJoint: " << nowBallJoint << endl;

	//ぐりぐりコントローラのEuler回転行列を編集する関節を変更するたびに初期化
	for(int i = 0; i < 16; i++){
		if(i%5 == 0) 
			rotationBallJoint[i] = 1.0;
		else 
			rotationBallJoint[i] = 0.0;
	}

	if(DCAST(PHBallJointIf, animalBody->GetJoint(nowBallJoint))){
		PHBallJointDesc ballDesc;
		DCAST(PHBallJointIf, animalBody->GetJoint(nowBallJoint))->GetDesc(&ballDesc);
		defaultBallJointData[0] = ballDesc.spring;
		ballJointData[0]		= defaultBallJointData[0];
		defaultBallJointData[1] = ballDesc.damper;
		ballJointData[1]		= defaultBallJointData[1];
		defaultBallJointData[2] = ballDesc.limitSwing.upper;
		ballJointData[2]		= defaultBallJointData[2];
		defaultBallJointData[3] = ballDesc.limitTwist.lower;
		ballJointData[3]		= defaultBallJointData[3];
		defaultBallJointData[4] = ballDesc.limitTwist.upper;
		ballJointData[4]		= defaultBallJointData[4];
		ballDesc.goal.ToMatrix(matMadeByQuaterinon.Rot());
	}
	else{
		DSTR << "GetJoint is not type of PHBallJointIf" << endl;
	}
	glui->sync_live();
}

void guiLoadHingeJoint(int control){
	
	double lower,upper;

	DSTR << "nowHingeJoint: " << nowHingeJoint << endl;
	if(DCAST(PHHingeJointIf, animalBody->GetJoint(nowHingeJoint))){
		
		defaultHingeJointData[0] =	DCAST(PHHingeJointIf, animalBody->GetJoint(nowHingeJoint))->GetSpring();
		hingeJointData[0]		 =  defaultHingeJointData[0];
		
		defaultHingeJointData[1] =  DCAST(PHHingeJointIf, animalBody->GetJoint(nowHingeJoint))->GetDamper();
		hingeJointData[1]		 =	defaultHingeJointData[1];
		
		DCAST(PHHingeJointIf, animalBody->GetJoint(nowHingeJoint))->GetRange(lower, upper);
		defaultHingeJointData[2] =	lower;
		hingeJointData[2]		 =	defaultHingeJointData[2];
		defaultHingeJointData[3] =	upper;
		hingeJointData[3]		 =	defaultHingeJointData[3];
		
		defaultHingeJointGoal	 =	DCAST(PHHingeJointIf, animalBody->GetJoint(nowHingeJoint))->GetSpringOrigin();
		hingeJointGoal			 =	defaultHingeJointGoal;
		
	}
	else{
		DSTR << "GetJoint is not type of PHHingeJointIf" << endl;
	}
	glui->sync_live();
}

void guiLoadScene(){
	// 初期データのロード
	// Solids
	dynamicalValue = animalBody->GetSolid(CRFourLegsAnimalBodyDesc::SO_WAIST)->IsDynamical();
	solids.resize(animalBody->NSolids());
	solidsSpace.resize(animalBody->NSolids());
	box.resize(animalBody->NSolids());
	for (int i=0; i<animalBody->NSolids(); i++){
		solids[i]		= DCAST(PHSolidIf, animalBody->GetSolid(i));
		solidsSpace[i]  = i;		
		if(animalBody->GetSolid(i))
			box[i] = DCAST(CDBoxIf,   animalBody->GetSolid(i)->GetShape(0));
	}

	// Joints
	ballJoints.resize(animalBody->NBallJoints());
	ballJointsSpace.resize(animalBody->NBallJoints());
	hingeJoints.resize(animalBody->NHingeJoints());
	hingeJointsSpace.resize(animalBody->NHingeJoints());
	int j = 0;
	int k = 0;
	for (int i=0; i<animalBody->NJoints(); i++){
		if(DCAST(PHBallJointIf, animalBody->GetJoint(i)) != NULL){
			ballJoints[j]		= DCAST(PHBallJointIf, animalBody->GetJoint(i));
			ballJointsSpace[j]	= i;
			j ++;
		}	
		else if(DCAST(PHHingeJointIf, animalBody->GetJoint(i)) != NULL){
			hingeJoints[k]		= DCAST(PHHingeJointIf, animalBody->GetJoint(i));
			hingeJointsSpace[k] = i;
			k ++;
		}
	}
}
void guiLoadInit(){
	guiLoadSolid(0);
	guiLoadBallJoint(0);
	guiLoadHingeJoint(0);
}

#endif
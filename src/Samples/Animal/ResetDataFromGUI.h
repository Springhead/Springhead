/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef RESETDATAFROMGUI_H
#define RESETDATAFROMGUI_H

#include <Springhead.h>
#include <iostream>
#include <gl/glui.h>

#include "MyGlobalValue.h"
#include "SetDataFromGUI.h"

/// Reset Data ---------------------------------------------------------------------------
void guiResetSolid(int control){
	
		DSTR << "Reset solid values" << endl;
		cout << "Reset solid values" << endl;
		boxsize = defaultBoxsize;
		guiSetBoxsize(control);
		mass = defaultMass;
		guiSetMass(control);

}


void guiResetBallJoint(int control){

		DSTR << "Reset ball joint values" << endl;
		cout << "Reset ball joint values" << endl;

		ballJointData[0] = defaultBallJointData[0];
		ballJointData[1] = defaultBallJointData[1];
		ballJointData[2] = defaultBallJointData[2];
		ballJointData[3] = defaultBallJointData[3];
		ballJointData[4] = defaultBallJointData[4];

		guiSetDataBallJoint(control);	
}


void guiResetHingeJoint(int control){
	
		glui->sync_live();
		DSTR << "Reset hinge joint values" << endl;
		cout << "Reset hinge joint values" << endl;

		hingeJointData[0] = defaultHingeJointData[0];
		hingeJointData[1] = defaultHingeJointData[1];
		hingeJointData[2] = defaultHingeJointData[2];
		hingeJointData[3] = defaultHingeJointData[3];
		hingeJointGoal	  = defaultHingeJointGoal;
		guiSetDataHingeJoint(control);

}

#endif
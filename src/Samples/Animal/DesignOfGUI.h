/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef DESIGNOFGUI_H
#define DESIGNOFGUI_H

#include <Springhead.h>
#include <gl/glui.h>
#include "MyGlobalValue.h"
#include "LoadDataFromGUI.h"
#include "SetDataFromGUI.h"
#include "ResetDataFromGUI.h"

/// Design Of My GUI ---------------------------------------------------------------------------
void DesignGUI(){
	
///　---- GLUIのデザイン from here↓	
	menu = glui->add_rollout("Menu", true);	

//　セッティング
	settingModel = glui->add_panel_to_panel(menu, "Setting", true);

//Solids Data
	{
	// チェックボックス
	glui->add_checkbox_to_panel(settingModel, "Dynamical Mode", &dynamicalValue, dynamicalValue, guiSetDynamical); 
	
	// ロールアウト
	rollSolid = glui->add_rollout_to_panel(settingModel, "Solid", false);
	glui->add_statictext_to_panel(rollSolid, "Select a solid & set datas of it.");

	// リストボックス
	listBoxSolid = glui->add_listbox_to_panel(rollSolid, "Solid:", &nowSolid, NULL, guiLoadSolid);
	
	for(int i=0; i<animalBody->NSolids(); i++){
		if(solids[i])
			listBoxSolid->add_item(solidsSpace[i], (char*)solids[i]->GetName());
	}

	// パネル
	solidDataArea = glui->add_panel_to_panel(rollSolid, "Datas", GLUI_PANEL_EMBOSSED);
	// パネルの中身
	{
		// スピナー
		spinnerBoxsize[0] = glui->add_spinner_to_panel(solidDataArea, "Breath(float):", GLUI_SPINNER_FLOAT, &boxsize[0], NULL, guiSetBoxsize);
		spinnerBoxsize[0]->set_int_limits(0.0, 10.0);
		spinnerBoxsize[1] = glui->add_spinner_to_panel(solidDataArea, "Height(float):", GLUI_SPINNER_FLOAT, &boxsize[1], NULL, guiSetBoxsize);
		spinnerBoxsize[1]->set_int_limits(0.0, 10.0);
		spinnerBoxsize[2] = glui->add_spinner_to_panel(solidDataArea, "Thickness(float):", GLUI_SPINNER_FLOAT, &boxsize[2], NULL, guiSetBoxsize);
		spinnerBoxsize[2]->set_int_limits(0.0, 10.0);
	
		spinnerMass = glui->add_spinner_to_panel(solidDataArea, "Mass(float):", GLUI_SPINNER_FLOAT, &mass, NULL, guiSetMass);
		spinnerMass->set_int_limits(0.01, 10.0);

		
		// セパレータ
		glui->add_separator_to_panel(solidDataArea);
		// リセットボタン
		glui->add_button_to_panel(solidDataArea, "Reset", 0, guiResetSolid);
	}
	}

// Ball Joint Data
	{
	
	// ロールアウト
	rollBallJoint = glui->add_rollout_to_panel(settingModel, "Ball Joint", false);
	glui->add_statictext_to_panel(rollBallJoint, "Select a ball joint & set datas of it.");

	// リストボックス
	listBoxBallJoint = glui->add_listbox_to_panel(rollBallJoint, "Ball Joint:", &nowBallJoint, NULL, guiLoadBallJoint);
	
	for(int i=0; i<animalBody->NBallJoints(); i++){
		if(ballJoints[i])
			listBoxBallJoint->add_item(ballJointsSpace[i], (char*)ballJoints[i]->GetName());
	}

	// パネル
	ballJointDataArea = glui->add_panel_to_panel(rollBallJoint, "Datas", GLUI_PANEL_EMBOSSED);
	// パネルの中身
	{
		// スピナー
		spinnerDataBallJoint[0] = glui->add_spinner_to_panel(ballJointDataArea, "Spring(float):", GLUI_SPINNER_FLOAT, &ballJointData[0], 0, guiSetDataBallJoint);
		spinnerDataBallJoint[0]->set_speed(10.0);
		spinnerDataBallJoint[0]->set_float_limits(0.0, 1000.0);
		spinnerDataBallJoint[1] = glui->add_spinner_to_panel(ballJointDataArea, "Damper(float):", GLUI_SPINNER_FLOAT, &ballJointData[1], 0, guiSetDataBallJoint);
		spinnerDataBallJoint[1]->set_speed(10.0);
		spinnerDataBallJoint[1]->set_float_limits(0.0, 1000.0);

		spinnerDataBallJoint[2] = glui->add_spinner_to_panel(ballJointDataArea, "Upper Swing Range(rad):", GLUI_SPINNER_FLOAT, &ballJointData[2], 0, guiSetDataBallJoint);
		spinnerDataBallJoint[2]->set_speed(1.0);
		spinnerDataBallJoint[2]->set_float_limits(0, M_PI/2.0);
		spinnerDataBallJoint[2]->disable();
		spinnerDataBallJoint[3] = glui->add_spinner_to_panel(ballJointDataArea, "Lower Twist Range(rad):", GLUI_SPINNER_FLOAT, &ballJointData[3], 0, guiSetDataBallJoint);
		spinnerDataBallJoint[3]->set_speed(1.0);
		spinnerDataBallJoint[3]->set_float_limits(-2*M_PI, 2*M_PI);
		spinnerDataBallJoint[3]->disable();
		spinnerDataBallJoint[4] = glui->add_spinner_to_panel(ballJointDataArea, "Upper Twist Range(rad):", GLUI_SPINNER_FLOAT, &ballJointData[4], 0, guiSetDataBallJoint);
		spinnerDataBallJoint[4]->set_speed(1.0);
		spinnerDataBallJoint[4]->set_float_limits(-2*M_PI, 2*M_PI);
		spinnerDataBallJoint[4]->disable();
		// セパレータ
		glui->add_separator_to_panel(ballJointDataArea);
		// ぐるぐるコントローラ
		glui->add_rotation_to_panel(ballJointDataArea, "Destination Of The Joint", rotationBallJoint, 1, guiSetDataBallJoint);
		// セパレータ
		glui->add_separator_to_panel(ballJointDataArea);
		// リセットボタン
		glui->add_button_to_panel(ballJointDataArea, "Reset", 0, guiResetBallJoint);
		

	}
	}

// Hinge Joint Data
	{
	

	// ロールアウト
	rollHingeJoint = glui->add_rollout_to_panel(settingModel, "Hinge Joint", false);
	glui->add_statictext_to_panel(rollHingeJoint, "Select a hinge joint & set datas of it.");
	// リストボックス
	listBoxHingeJoint = glui->add_listbox_to_panel(rollHingeJoint, "Hinge Joint:", &nowHingeJoint, NULL, guiLoadHingeJoint);
	for(int i=0; i<animalBody->NHingeJoints(); i++){
		if(hingeJoints[i])
			listBoxHingeJoint->add_item(hingeJointsSpace[i], (char*)hingeJoints[i]->GetName());
	}

	// パネル
	hingeJointDataArea = glui->add_panel_to_panel(rollHingeJoint, "Datas", GLUI_PANEL_EMBOSSED);
	// パネルの中身
	{
		// スピナー
		spinnerDataHingeJoint[0] = glui->add_spinner_to_panel(hingeJointDataArea, "Spring(float):", GLUI_SPINNER_FLOAT, &hingeJointData[0], NULL, guiSetDataHingeJoint);
		spinnerDataHingeJoint[0]->set_speed(10.0);
		spinnerDataHingeJoint[0]->set_float_limits(0.0, 1000.0);
		spinnerDataHingeJoint[1] = glui->add_spinner_to_panel(hingeJointDataArea, "Damper(float):", GLUI_SPINNER_FLOAT, &hingeJointData[1], NULL, guiSetDataHingeJoint);
		spinnerDataHingeJoint[1]->set_speed(10.0);
		spinnerDataHingeJoint[1]->set_float_limits(0.0, 1000.0);
		spinnerDataHingeJoint[2] = glui->add_spinner_to_panel(hingeJointDataArea, "Lower Range(rad):", GLUI_SPINNER_FLOAT, &hingeJointData[2], NULL, guiSetDataHingeJoint);
		spinnerDataHingeJoint[0]->set_speed(1.0);
		spinnerDataHingeJoint[2]->disable();
		spinnerDataHingeJoint[2]->set_float_limits(-2*M_PI, 2*M_PI);
		spinnerDataHingeJoint[3] = glui->add_spinner_to_panel(hingeJointDataArea, "Upper Range(rad):", GLUI_SPINNER_FLOAT, &hingeJointData[3], NULL, guiSetDataHingeJoint);
		spinnerDataHingeJoint[0]->set_speed(1.0);
		spinnerDataHingeJoint[3]->set_float_limits(-2*M_PI, 2*M_PI);
		spinnerDataHingeJoint[3]->disable();
		// セパレータ
		glui->add_separator_to_panel(hingeJointDataArea);
		// スピナー
		spinnerDataHingeJointGoal = glui->add_spinner_to_panel(hingeJointDataArea, "Destination Of The Joint(rad):", GLUI_SPINNER_FLOAT, &hingeJointGoal, NULL, guiSetDataHingeJoint);
		spinnerDataHingeJointGoal->set_speed(0.1);
		spinnerDataHingeJointGoal->set_float_limits(-2*M_PI, 2*M_PI);
		// セパレータ
		glui->add_separator_to_panel(hingeJointDataArea);
		// リセットボタン
		glui->add_button_to_panel(hingeJointDataArea, "Reset", 0, guiResetHingeJoint);
	}
    }

	glui->add_separator();
    glui->add_button( "Quit", 0 , PressQuit);

/// ---- to here↑
}

#endif
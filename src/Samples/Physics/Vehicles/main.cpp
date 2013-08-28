/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/Samples/Vehicles/main.cpp

【概要】Springhead2の総合的なデモ第１段．8足ロボットの歩行
 
【仕様】

*/

#include "../../SampleApp.h"
#include "robot.h"

class MyApp : public SampleApp{
public:
	enum {
		ID_FORWARD,
		ID_BACKWARD,
		ID_TURN_LEFT,
		ID_TURN_RIGHT,
		ID_STOP,
	};

	Robot robot;

public:
	MyApp(){
		AddMenu(MENU_SCENE, "control robot");
		AddAction(MENU_SCENE, ID_FORWARD, "move forward");
		AddHotKey(MENU_SCENE, ID_FORWARD, 'f');
		AddAction(MENU_SCENE, ID_BACKWARD, "move backward");
		AddHotKey(MENU_SCENE, ID_BACKWARD, 'b');
		AddAction(MENU_SCENE, ID_TURN_LEFT, "turn left");
		AddHotKey(MENU_SCENE, ID_TURN_LEFT, 'l');
		AddAction(MENU_SCENE, ID_TURN_RIGHT, "turn right");
		AddHotKey(MENU_SCENE, ID_TURN_RIGHT, 'r');
		AddAction(MENU_SCENE, ID_STOP, "stop");
		AddHotKey(MENU_SCENE, ID_STOP, 's');
	}

	virtual void BuildScene(){
		Posed pose;
		pose.Pos() = Vec3d(3.0, 2.0, 0.0);
		robot.Build(pose, GetFWScene()->GetPHScene(), GetSdk()->GetPHSdk());			//	ロボット
		pose.Pos() = Vec3d(0.0, 1.0, 1.0);

		CreateFloor();								//	床
		CDBoxDesc box;								//	三つ重なっている箱
		box.boxsize = Vec3f(1.0, 1.0, 2.0);
		CDBoxIf* boxBody = GetFWScene()->GetPHScene()->GetSdk()->CreateShape(box)->Cast();
		PHSolidDesc sd;
		sd.mass *= 0.7;
		sd.inertia *= 0.7;
		PHSolidIf* soBox;
		for(int i=0; i<3;++i){
			soBox = GetFWScene()->GetPHScene()->CreateSolid(sd);
			soBox->AddShape(boxBody);
			soBox->SetPose(pose);
			pose.PosY()+=1.0;
			pose.PosX()-=0.05;
		}

		GetFWScene()->GetPHScene()->SetGravity(Vec3f(0.0, -9.8, 0.0));	//	重力を設定

		GetFWScene()->EnableRenderAxis(false, false, false);

		ToggleAction(MENU_ALWAYS, ID_RUN);
	}
	virtual void OnAction(int menu, int id){
		if(menu == MENU_SCENE){
			if(id == ID_FORWARD)
				robot.Forward();
			if(id == ID_BACKWARD)
				robot.Backward();
			if(id == ID_TURN_LEFT)
				robot.TurnLeft();
			if(id == ID_TURN_RIGHT)
				robot.TurnRight();
			if(id == ID_STOP)
				robot.Stop();
		}
		SampleApp::OnAction(menu, id);
	}
} app;

int __cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

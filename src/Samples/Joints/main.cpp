/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/Samples/Joints/main.cpp

【概要】関節機能のデモ

【テストパターン】
  ・シーン0：ヒンジの鎖
  ・シーン1：チェビシェフリンク
　・シーン2：ボールジョイントの鎖
　・シーン3：スライダの鎖
  ・シーン4：パスジョイント
 
【仕様】
	共通の操作：
		・シーン番号0, 1, ...をタイプすると対応するシーンに切り替わる（デフォルト：シーン0)
		・'x'をタイプすると関節の有効/無効が切り替わる
		・'z'で、左方向からボックスを飛ばし、衝突させる		
　　シーン0：
		・'0'で、ヒンジシーンの設定を行う（シーン切換え）
		・スペースキーでヒンジ用のboxを生成
	シーン1：
		・'1'で、チェビシェフリンクを生成する（シーン切換え）
		・'a'で、モータートルクを1.0に設定する
		・'s'で、モータートルクを0.0に設定する
		・'d'で、モータートルクを-1.0に設定する
		・'f'で、目標速度を90.0に設定する
		・'g'で、目標速度を0.0に設定する
		・'h'で、目標速度を-90.0に設定する
		・'j'で、バネ原点(バネの力が0となる関節変位)を1.0に設定する
		・'k'で、バネ原点(バネの力が0となる関節変位)を0.0に設定する
		・'l'で、バネ原点(バネの力が0となる関節変位)を-1.0に設定する
	シーン2：
		・'2'で、ボールジョイントシーンの設定を行う（シーン切換え）
		・スペースキーでボールジョイント用のboxを生成
*/

#include <ctime>
#include <string>
#include <GL/glut.h>
#include <sstream>
#include <windows.h>

#include <Springhead.h>		//	Springheadのインタフェース

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC		27

PHSdkIf* phSdk;			// SDKインタフェース
GRSdkIf* grSdk;
PHSceneIf* scene;		// Sceneインタフェース
GRDebugRenderIf* render;
GRDeviceGLIf* device;

double simulationPeriod = 32.0;
Vec3d lookAt;
int sceneNo;			// シーン番号

PHSolidDesc descFloor;					//床剛体のディスクリプタ
PHSolidDesc descBox;					//箱剛体のディスクリプタ
CDShapeIf* shapeBox;
CDShapeIf* shapeSphere;

PHSolidIf* soFloor;						//床剛体のインタフェース
std::vector<PHSolidIf*> soBox;			//箱剛体のインタフェース
std::vector<PHJointIf*> jntLink;		//関節のインタフェース


void CreateFloor(bool s=true){
	CDBoxDesc desc;
	desc.boxsize = Vec3f(30.0f, 5.0f, 20.0f);
	soFloor = scene->CreateSolid(descFloor);
	if (s) soFloor->AddShape(phSdk->CreateShape(desc));
	soFloor->SetFramePosition(Vec3f(0,-2,0));
	soFloor->SetDynamical(false);			// 床は外力によって動かないようにする
}

// シーン0 : 鎖のデモ。space keyで箱が増える
void BuildScene0(){	
	CreateFloor();
	//鎖の根になる箱を作成
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shapeBox);
	//空中に固定する
	soBox.back()->SetFramePosition(Vec3f(0.0, 15.0, 0.0));
	soBox.back()->SetOrientation(Quaterniond::Rot(-1.57, Vec3d(0.0, 0.0, 1.0)));
	soBox.back()->SetDynamical(false);

	// 重力を設定
	scene->SetGravity(Vec3f(0, -9.8, 0));
}

// シーン1 : アクチュエータのデモ
void BuildScene1(){
	CreateFloor();
	CDBoxDesc bd;
	soBox.resize(3);
	bd.boxsize = Vec3f(1.0, 2.0, 1.0);
	soBox[0] = scene->CreateSolid(descBox);
	soBox[0]->AddShape(phSdk->CreateShape(bd));
	soBox[0]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	bd.boxsize = Vec3f(1.0, 5.0, 1.0);
	soBox[1] = scene->CreateSolid(descBox);
	soBox[1]->AddShape(phSdk->CreateShape(bd));
	soBox[1]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	bd.boxsize = Vec3f(1.0, 10.0, 1.0);
	soBox[2] = scene->CreateSolid(descBox);
	soBox[2]->AddShape(phSdk->CreateShape(bd));
	soBox[2]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	PHHingeJointDesc jd;
	jntLink.resize(4);
	jd.posePlug.Pos() = Vec3d(0.0, 10.0, 0.0);
	jd.poseSocket.Pos() = Vec3d(0.0, -1.0, 0.0);
	jntLink[0] = scene->CreateJoint(soFloor, soBox[0], jd);

	jd.posePlug.Pos() = Vec3d(4.0, 10.0, 0.0);
	jd.poseSocket.Pos() = Vec3d(0.0, -2.5, 0.0);
	jntLink[1] = scene->CreateJoint(soFloor, soBox[1], jd);

	jd.posePlug.Pos() = Vec3d(0.0, 1.0, 0.0);
	jd.poseSocket.Pos() = Vec3d(0.0, -5.0, 0.0);
	jntLink[2] = scene->CreateJoint(soBox[0], soBox[2], jd);

	jd.posePlug.Pos() = Vec3d(0.0, 2.5, 0.0);
	jd.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jntLink[3] = scene->CreateJoint(soBox[1], soBox[2], jd);

	scene->SetContactMode(&soBox[0], 3, PHSceneDesc::MODE_NONE);
	scene->SetGravity(Vec3f(0, 0.0, 0));
}

void BuildScene2(){
	CreateFloor();
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shapeBox);
	soBox.back()->SetFramePosition(Vec3f(0.0, 20.0, 0.0));
	soBox.back()->SetDynamical(false);
	scene->SetGravity(Vec3f(0, -9.8, 0));	
}

void BuildScene3(){
	CreateFloor();
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	descBox.mass=10.0;
	descBox.inertia = 10 * Matrix3d::Unit();
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shapeBox);
	soBox.back()->SetFramePosition(Vec3f(0.0, 20.0, 0.0));
	soBox.back()->SetDynamical(false);
	scene->SetGravity(Vec3f(0, -9.8, 0));	
}

void BuildScene4(){
	CreateFloor();
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(shapeBox);
	soBox.back()->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	PHPathDesc desc;
	PHPathIf* path = scene->CreatePath(desc);
	double s;
	double radius = 5.0;
	double pitch = 4.0;
	Posed pose;
	for(s = 0.0; s < 4 * (2 * M_PI); s += Rad(1.0)){
		double stmp = s;
		while(stmp > M_PI) stmp -= 2 * M_PI;
		pose.Pos() = Vec3d(radius * cos(stmp), 5.0 + pitch * s / (2 * M_PI), radius * sin(stmp));
		pose.Ori().FromMatrix(Matrix3d::Rot(-3*stmp, 'y'));
		path->AddPoint(s, pose);
	}
	PHPathJointDesc descJoint;
	jntLink.push_back(scene->CreateJoint(soFloor, soBox[0], descJoint));
	PHPathJointIf* joint = DCAST(PHPathJointIf, jntLink[0]);
	joint->AddChildObject(path);
	joint->SetPosition(2 * 2 * M_PI);
	
	scene->SetGravity(Vec3f(0, -9.8, 0));
}

void BuildScene5(){
	CreateFloor(false);
	
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 6.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	
	soBox.resize(6);

	soBox[0] = scene->CreateSolid(descBox);
	soBox[0]->AddShape(shapeBox);

	soBox[1] = scene->CreateSolid(descBox);
	soBox[1]->AddShape(shapeBox);

	soBox[2] = scene->CreateSolid(descBox);
	soBox[2]->AddShape(shapeBox);

	soBox[3] = scene->CreateSolid(descBox);
	soBox[3]->AddShape(shapeBox);

	soBox[4] = scene->CreateSolid(descBox);
	soBox[4]->AddShape(shapeBox);

	CDSphereDesc descSphere;
	descSphere.radius = 1.0;
	soBox[5] = scene->CreateSolid(descBox);
	soBox[5]->AddShape(scene->CreateShape(descSphere));

	jntLink.resize(6);
	PHHingeJointDesc descHinge;
	descHinge.poseSocket.Pos() = Vec3d(0.0, -3.0, 0.0);
	jntLink[0] = scene->CreateJoint(soFloor, soBox[0], descHinge);

	descHinge.posePlug.Pos() = Vec3d(0.0, 3.0, 0.0);
	descHinge.poseSocket.Pos() = Vec3d(0.0, -3.0, 0.0);
	jntLink[1] = scene->CreateJoint(soBox[0], soBox[1], descHinge);
	jntLink[2] = scene->CreateJoint(soBox[1], soBox[2], descHinge);
	jntLink[3] = scene->CreateJoint(soBox[2], soBox[3], descHinge);
	jntLink[4] = scene->CreateJoint(soBox[3], soBox[4], descHinge);

	double K = 2000, D = 100;
	//double K = 100000, D = 10000;	
	DCAST(PHHingeJointIf, jntLink[0])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[0])->SetDamper(D);
	DCAST(PHHingeJointIf, jntLink[1])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[1])->SetDamper(D);
	DCAST(PHHingeJointIf, jntLink[2])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[2])->SetDamper(D);
	DCAST(PHHingeJointIf, jntLink[3])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[3])->SetDamper(D);
	DCAST(PHHingeJointIf, jntLink[4])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[4])->SetDamper(D);

	soBox[5]->SetFramePosition(Vec3d(10.0, 5.0, 0.0));
	soBox[5]->SetDynamical(false);
	
	scene->SetContactMode(PHSceneDesc::MODE_NONE);	// 接触を切る
	scene->SetGravity(Vec3f(0, -9.8, 0));	
}

void BuildScene(){
	switch(sceneNo){
	case 0: BuildScene0(); break;
	case 1: BuildScene1(); break;
	case 2: BuildScene2(); break;
	case 3: BuildScene3(); break;
	case 4: BuildScene4(); break;
	case 5: BuildScene5(); break;
	}
}

void OnKey0(char key){
	switch(key){
	case ' ':{
		soBox.push_back(scene->CreateSolid(descBox));
		soBox.back()->AddShape(shapeBox);
		soBox.back()->SetFramePosition(Vec3f(10.0, 10.0, 0.0));
		PHHingeJointDesc jdesc;
		jdesc.posePlug.Pos() = Vec3d( 1.1,  1.1,  0);
		jdesc.poseSocket.Pos() = Vec3d(-1.1, -1.1,  0);
		jdesc.lower = Rad(-30.0);
		jdesc.upper = Rad( 30.0);

		size_t n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		}break;
	}
}
void display();
void OnKey1(char key){
	const double K = 30.0;
	const double B = 10.0;
	PHHingeJointIf* hinge = DCAST(PHHingeJointIf, jntLink[0]);
	PHPathJointIf* path = (jntLink.size() == 5 ? DCAST(PHPathJointIf, jntLink[4]) : NULL); 
	switch(key){
	case 'a': hinge->SetMotorTorque(1.0);	break;
	case 's': hinge->SetMotorTorque(0.0);	break;
	case 'd': hinge->SetMotorTorque(-1.0);	break;
	case 'f':
		hinge->SetDesiredVelocity(Rad(180));
		if(path)
			path->SetDesiredVelocity(Rad(90.0));
		break;
	case 'g':
		hinge->SetDesiredVelocity(Rad(0.0));
		if(path)
			path->SetDesiredVelocity(Rad(0.0));
		break;
	case 'h': hinge->SetDesiredVelocity(Rad(-90.0));	break;
	case 'j':
		hinge->SetSpring(K);
		hinge->SetSpringOrigin(1.0);
		hinge->SetDamper(B);
		break;
	case 'k':
		hinge->SetSpring(K);
		hinge->SetSpringOrigin(0.0);
		hinge->SetDamper(B);
		break;
	case 'l':
		hinge->SetSpring(K);
		hinge->SetSpringOrigin(-1.0);
		hinge->SetDamper(B);
		break;
	case 'c':{
		//チェビシェフリンク一周分の軌跡を記憶させてパスジョイントを作成
		PHPathDesc descPath;
		descPath.bLoop = true;
		PHPathIf* trajectory = scene->CreatePath(descPath);

		hinge->SetSpring(K);
		hinge->SetDamper(B);
		double theta = -Rad(180.0);
		hinge->SetSpringOrigin(theta);
		for(int i = 0; i < 50; i++)
			scene->Step();
		for(; theta < Rad(180.0); theta += Rad(1.0)){
			hinge->SetSpringOrigin(theta);
			for(int i = 0; i < 5; i++)
				scene->Step();
			Posed pose = soFloor->GetPose().Inv() * soBox[2]->GetPose();
			//pose.Ori() = Quaterniond();
			trajectory->AddPoint(theta, pose);
			display();
		}
	
		soBox.resize(4);
		soBox[3] = scene->CreateSolid(descBox);
		soBox[3]->AddShape(soBox[2]->GetShape(0));
		soBox[3]->SetFramePosition(Vec3f(10.0, 20.0, 0.0));

		PHPathJointDesc descJoint;
		descJoint.posePlug.Pos().x = 15.0;
		jntLink.resize(5);
		jntLink[4] = scene->CreateJoint(soFloor, soBox[3], descJoint);
		PHPathJointIf* joint = DCAST(PHPathJointIf, jntLink[4]);
		joint->AddChildObject(trajectory);
		joint->SetPosition(0);
	
		}break;
	}
}

void OnKey2(char key){
	switch(key){
	case ' ':{
		soBox.push_back(scene->CreateSolid(descBox));
		soBox.back()->AddShape(shapeBox);
		soBox.back()->SetFramePosition(Vec3f(10.0, 10.0, 0.0));
		PHBallJointDesc jdesc;
		jdesc.posePlug.Pos() = Vec3d(-1.01, -1.01, -1.01);
		jdesc.poseSocket.Pos() = Vec3d(1.01, 1.01, 1.01);
		size_t n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		}break;
	}
}

void OnKey3(char key){
	switch(key){
	case ' ':{
		soBox.push_back(scene->CreateSolid(descBox));
		soBox.back()->AddShape(shapeBox);
		soBox.back()->SetFramePosition(Vec3f(10.0, 10.0, 0.0));
		PHSliderJointDesc jdesc;
		jdesc.posePlug.Pos() = Vec3d(0, -1.1, 0);
		jdesc.posePlug.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
		jdesc.poseSocket.Pos() = Vec3d(0,  1.1, 0);
		jdesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
		size_t n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		PHSliderJointIf* slider = DCAST(PHSliderJointIf, jntLink.back());
		slider->SetRange(-0.3, 0.3);
		//slider->SetSpring(1000.0);
		//slider->SetDamper(300);
		//slider->SetSpringOrigin(0.0);
		}break;
	case 'a': scene->SetGravity(Vec3f(5.0, -5, 0.0)); break;
	case 'd': scene->SetGravity(Vec3f(-5.0, -5, 0.0)); break;
	}
}

void OnKey4(char key){
	switch(key){
	case 'a': scene->SetGravity(Vec3f(0.0, -9.8, 0.0)); break;
	case 'd': scene->SetGravity(Vec3f(0.0,  9.8, 0.0)); break;
	}
}

float origin = 0;
void OnKey5(char key){
	switch(key){
	case 'a': soBox[5]->SetFramePosition(Vec3d(-20.0, 30.0, 0.0)); break;
	case 's': soBox[5]->SetFramePosition(Vec3d(-10.0, 20.0, 0.0)); break;
	case 'd': soBox[5]->SetFramePosition(Vec3d( -5.0, 10.0, 0.0)); break;
	case 'f': soBox[5]->SetFramePosition(Vec3d(  0.0, 10.0, 0.0)); break;
	case 'g': soBox[5]->SetFramePosition(Vec3d(  5.0, 10.0, 0.0)); break;
	case 'h': soBox[5]->SetFramePosition(Vec3d( 10.0, 20.0, 0.0)); break;
	case 'j': soBox[5]->SetFramePosition(Vec3d( 20.0, 30.0, 0.0)); break;
	case '.': 
		simulationPeriod /= 2.0;
		if (simulationPeriod < 1) simulationPeriod = 1.0;
		break;
	case ',': 
		simulationPeriod *= 2.0;
		break;
	/*case ' ':{
		//	剛体追加
		soBox.push_back(scene->CreateSolid(descBox));
		soBox.back()->AddShape(shapeBox);
		soBox.back()->SetFramePosition(Vec3f(10.0, 10.0, 0.0));

		//	ジョイント作成
		PHHingeJointDesc descHinge;
		descHinge.posePlug.Pos() = Vec3d(0.0, 3.0, 0.0);
		descHinge.poseSocket.Pos() = Vec3d(0.0, -3.0, 0.0);
		size_t n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], descHinge));
		DCAST(PHHingeJointIf, jntLink.back())->SetSpring(K6);
		DCAST(PHHingeJointIf, jntLink.back())->SetDamper(D6);

		scene->SetContactMode(PHSceneDesc::MODE_NONE);	// 接触を切る
		}break;*/
	case 'n':
		origin += 0.01;
		for(unsigned i=0; i<jntLink.size(); ++i){
			PHHingeJointIf* j = DCAST(PHHingeJointIf, jntLink[i]);
			if (j) j->SetSpringOrigin(origin);
		}
		break;
	case 'm':
		origin -= 0.01;
		for(unsigned i=0; i<jntLink.size(); ++i){
			PHHingeJointIf* j = DCAST(PHHingeJointIf, jntLink[i]);
			if (j) j->SetSpringOrigin(origin);
		}
		break;
	}

}

void OnKey(char key){
	switch(sceneNo){
	case 0: OnKey0(key); break;
	case 1: OnKey1(key); break;
	case 2: OnKey2(key); break;
	case 3: OnKey3(key); break;
	case 4: OnKey4(key); break;
	case 5: OnKey5(key); break;
	}
}

void OnTimer0(){}
void OnTimer1(){}
void OnTimer2(){}
void OnTimer3(){}
void OnTimer4(){}

void OnTimer5(){
/*
	PHSpringDesc descSpring;
	descSpring.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
	descSpring.spring = Vec3d(1.0, 1.0, 1.0) * K;
	descSpring.damper = Vec3d(1.0, 1.0, 1.0) * D;
	jntLink[5] = scene->CreateJoint(soBox[5], soBox[4], descSpring);

	
	soBox[5]->SetFramePosition(Vec3d(10.0, 5.0, 0.0));
	soBox[5]->SetDynamical(false);
*/
	Vec3f dVel = Vec3f() - soBox[4]->GetVelocity();
	Vec3f dPos = soBox[5]->GetPose().Pos() - soBox[4]->GetPose().Pos();
//	float K = 200;
//	float B = 10;
	float K = 150;
	float B = 7;
	Vec3f force = K*dPos + B*dVel;
	soBox[4]->AddForce(force, soBox[4]->GetPose()*Vec3f(0,3,0));
}

void OnTimer(){
	switch(sceneNo){
	case 0: OnTimer0(); break;
	case 1: OnTimer1(); break;
	case 2: OnTimer2(); break;
	case 3: OnTimer3(); break;
	case 4: OnTimer4(); break;
	case 5: OnTimer5(); break;
	}
}	

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);
	render->EndScene();
}

/**
 brief     	光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 0.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 0.0);
	render->PushLight(light0);
	render->PushLight(light1);
}

/**
 brief     	初期化処理
 param	 	なし
 return 	なし
 */
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	lookAt.x = 30.0;
	lookAt.y = -20.0;
	lookAt.z = 100.0;
	lookAt.x = 3.0;
	lookAt.y = 10.0;
	lookAt.z = 30.0;
	gluLookAt(lookAt.x, lookAt.y, lookAt.z, 
		      0.0, lookAt.y, 0.0,
		 	  0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	setLight();
}

/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		なし
 */
void reshape(int w, int h){
	// Viewportと射影行列を設定
	render->Reshape(Vec2f(), Vec2f(w,h));
}

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void keyboard(unsigned char key, int x, int y){
	unsigned int i = 0;
	switch (key) {
		//終了
		case ESC:		
		case 'q':
			exit(0);
			break;
		//シーン切り替え
		case '0': case '1': case '2': case '3': case '4': case '5': case '6':
			scene->Clear();
			soFloor = NULL;
			soBox.clear();
			jntLink.clear();
			sceneNo = key - '0';
			BuildScene();
			break;
		case 'x':
			{
				static bool bEnable = true;
				bEnable = !bEnable;
				for(i = 0; i < jntLink.size(); i++)
					jntLink[i]->Enable(bEnable);
			}break;
		case 'z':{
			soBox.push_back(scene->CreateSolid(descBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3f(15.0, 15.0, 0.0));
			soBox.back()->SetVelocity(Vec3d(-10.0, 0.0, 0.0));
			soBox.back()->SetMass(2.0);
			}break;	
		case 'Z':{
			soBox.push_back(scene->CreateSolid(descBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3f(5.0, 13.0, 5.0));
			soBox.back()->SetVelocity(Vec3d(0.0, 0.0, -20.0));
			soBox.back()->SetMass(2.0);
			}break;	
		case 'C':{
			CDCapsuleDesc cd;
			cd.radius = 1.0;
			cd.length = 4.0;
			CDShapeIf* shape = phSdk->CreateShape(cd);

			soBox.push_back(scene->CreateSolid(descBox));
			soBox.back()->AddShape(shape);
			soBox.back()->SetOrientation(Quaterniond::Rot(Rad(90), 'y'));
			soBox.back()->SetFramePosition(Vec3f(15.0, 15.0, 0.0));
			soBox.back()->SetVelocity(Vec3d(-10.0, 0.0, 0.0));
			soBox.back()->SetMass(2.0);
			}break;	
		default:
			OnKey(key);
			break;
	}
}	

/**
 brief  	glutTimerFuncで指定したコールバック関数
 param	 	<in/--> id　　 タイマーの区別をするための情報
 return 	なし
 */
void timer(int id){
	glutTimerFunc(simulationPeriod, timer, 0);
	OnTimer();
	scene->ClearForce();
	scene->GenerateForce();
	scene->Integrate();
	glutPostRedisplay();
}
void idle(){
	scene->ClearForce();
	scene->GenerateForce();
	scene->Integrate();
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){

	// SDKの作成　
	phSdk = PHSdkIf::CreateSdk();
	grSdk = GRSdkIf::CreateSdk();
	// シーンオブジェクトの作成
	PHSceneDesc dscene;
	dscene.timeStep = 0.05;
	dscene.numIteration = 20;
	scene = phSdk->CreateScene(dscene);				// シーンの作成
	// シーンの構築
	sceneNo = 0;
	BuildScene();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	int window = glutCreateWindow("Joints");

	render = grSdk->CreateDebugRender();
	device = grSdk->CreateDeviceGL(window);

	// 初期設定
	device->Init();

	glutTimerFunc(20, timer, 0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(idle);
	
	render->SetDevice(device);	// デバイスの設定

	initialize();

	glutMainLoop();

}

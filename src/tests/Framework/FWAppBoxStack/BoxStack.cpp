#include "BoxStack.h"
//#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <GL/glut.h>

#define ESC 27

BoxStack bstack;

BoxStack::BoxStack(){
	dt = 0.02;//0.05;
	gravity =  Vec3d(0, -9.8f , 0);
	nIter = 15;
	bGravity = true;
	bDebug = false;
	bStep = true;
}

void BoxStack::Init(int argc, char* argv[]){
	FWApp::Init(argc, argv);

	GetSdk()->Clear();															// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	DesignObject();																// 剛体を作成

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(nIter);

	FWWinDesc windowDesc;
	windowDesc.title = "FWAppBoxStack";
	window = CreateWin(windowDesc);
	window->SetScene(GetSdk()->GetScene());

	InitCameraView();
}

void BoxStack::Reset(){
	GetSdk()->Clear();															// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	DesignObject();																// 剛体を作成

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(nIter);

	GetCurrentWin()->SetScene(GetSdk()->GetScene());
	InitCameraView();
}
void BoxStack::InitCameraView(){
/*	cameraInfo.view[0][0] = 0.9996;
	cameraInfo.view[0][1] = 0.0107463;
	cameraInfo.view[0][2] = -0.0261432;
	cameraInfo.view[0][3] = -0.389004;
	cameraInfo.view[1][0] = -6.55577e-010;
	cameraInfo.view[1][1] = 0.924909;
	cameraInfo.view[1][2] = 0.380188;
	cameraInfo.view[1][3] = 5.65711;
	cameraInfo.view[2][0] = 0.0282657;
	cameraInfo.view[2][1] = -0.380037;
	cameraInfo.view[2][2] = 0.92454;
	cameraInfo.view[2][3] = 13.7569;
	cameraInfo.view[3][0] = 0;
	cameraInfo.view[3][1] = 0;
	cameraInfo.view[3][2] = 0;
	cameraInfo.view[3][3] = 1;	*/
}

void BoxStack::DesignObject(){
	// soFloor用のdesc
	desc.mass = 1e20f;
	desc.inertia *= 1e30f;
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetDynamical(false);
	soFloor->SetGravity(false);
	
	// soBox用のdesc
	desc.mass = 0.05;
	desc.inertia = 0.033 * Matrix3d::Unit();
	{
		CDBoxDesc bd;
		bd.boxsize = Vec3f(2,2,2);
		meshBox = XCAST(GetSdk()->GetPHSdk()->CreateShape(bd));
		meshBox->SetName("meshBox");
		CDSphereDesc sd;
		sd.radius = 1.2;
		meshSphere = XCAST(GetSdk()->GetPHSdk()->CreateShape(sd));
		meshSphere->SetName("meshSphere");
		CDCapsuleDesc cd;
		cd.radius = 1;
		cd.length = 1;
		meshCapsule = XCAST(GetSdk()->GetPHSdk()->CreateShape(cd));
		meshCapsule->SetName("meshCapsule");
	}

	{
		// meshConvex(soBox)のメッシュ形状
		CDConvexMeshInterpolateDesc md;
		md.vertices.push_back(Vec3d(-1,-1,-1));
		md.vertices.push_back(Vec3d(-1,-1, 1));	
		md.vertices.push_back(Vec3d(-1, 1,-1));	
		md.vertices.push_back(Vec3d(-1, 1, 1));
		md.vertices.push_back(Vec3d( 1,-1,-1));	
		md.vertices.push_back(Vec3d( 1,-1, 1));
		md.vertices.push_back(Vec3d( 1, 1,-1));
		md.vertices.push_back(Vec3d( 1, 1, 1));
		meshConvex = DCAST(CDConvexMeshIf, GetSdk()->GetPHSdk()->CreateShape(md));
		meshConvex->SetName("meshConvex");

		// meshFloor(soFloor)のメッシュ形状
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 30;
			md.vertices[i].z *= 20;
			md.vertices[i].y *= 20;
		}
		meshFloor = DCAST(CDConvexMeshIf, GetSdk()->GetPHSdk()->CreateShape(md));
		meshFloor->SetName("meshFloor");
	}
	soFloor->AddShape(meshFloor);
	soFloor->SetFramePosition(Vec3d(0,-20 - 0.7,0));
	soFloor->GetShape(0)->SetVibration(-100, 150, 150);
	soFloor->SetName("solidFloor");
}

int exitCount = 0;
void BoxStack::Step(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	if(bStep) phscene->Step();
	else if (bOneStep){
		phscene->Step();
		bOneStep = false;
	}
	glutPostRedisplay();
	exitCount ++;
	if (exitCount > 60.0/dt) exit(0);
}

void BoxStack::Display(){
	// 描画の設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = window->render->Cast();

	// 描画モードの設定
	render->SetRenderMode(true, false);
//	render->EnableRenderAxis(bDebug);
	render->EnableRenderForce(bDebug);
	render->EnableRenderContact(bDebug);

	// カメラ座標の指定
	GRCameraIf* cam = window->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		window->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!GetCurrentWin()) return;
	GRRenderIf*curRender =  GetCurrentWin()->GetRender();
	FWSceneIf* curScene = GetCurrentWin()->GetScene();

	GetSdk()->SwitchScene(curScene);
	GetSdk()->SwitchRender(curRender);

	if(!curRender) return;
	curRender->ClearBuffer();
	curRender->BeginScene();

	if (curScene) curScene->Draw(curRender, GetSdk()->GetDebugMode());

	//	光源の追加
	GRLightDesc ld;
	ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
	ld.specular = Vec4f(1,1,1,1) * 0.8f;
	ld.ambient = Vec4f(1,1,1,1) * 0.4f;
	ld.position = Vec4f(1,1,1,0);
	render->PushLight(ld);
	render->PopLight();	//	光源の削除

	curRender->EndScene();
	glutSwapBuffers();
}

void BoxStack::Keyboard(int key, int x, int y){
	exitCount = 0;
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	switch (key) {
		case ESC:
		case  'q':
			exit(0);
			break;
		case 'r':
			Reset();
			break;
		case 'w':
			InitCameraView();
			DSTR << "InitCameraView" << endl;
			break;
		case 'd':
			if(bDebug){
				bDebug = false;
				DSTR << "Debug Mode OFF" << endl;
			}else{
				bDebug = true;
				DSTR << "Debug Mode ON" << endl;
			}
			break;
		case 'g':
			if(bGravity){
				bGravity = false;
				Vec3d zeroG = Vec3d(0.0, 0.0, 0.0);
				phscene->SetGravity(zeroG);
				DSTR << "Gravity OFF" << endl;
			}else{
				bGravity = true;
				phscene->SetGravity(gravity);
				DSTR << "Gravity ON" << endl;
			}
			break;
		case 's':
			bStep = false;
			bOneStep = true;
			break;
		case 'a':
			bStep = true;
			break;
		case ' ':
			{
				// ConvexBox
				desc.mass = 0.05;
				desc.inertia = 0.0333 * Matrix3d::Unit();
				//desc.dynamical = false;
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshBox);
				soBox.back()->SetFramePosition(Vec3d(-1, 5, 4));
				soBox.back()->GetShape(0)->SetVibration(-200,120,300);
				soBox.back()->GetShape(0)->SetStaticFriction(0.8);
				soBox.back()->GetShape(0)->SetDynamicFriction(0.6);
				//soBox.back()->SetOrientation(
				//	Quaternionf::Rot(Rad(30), 'y') * 
				//	Quaternionf::Rot(Rad(10), 'x'));  
				ostringstream os;
				os << "box" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			DSTR << "Create Box" << endl;
			DSTR << "NSolids		" <<  phscene->NSolids() << endl;
			break;
		case 'v':
			{
				// MeshCapsule
				desc.mass = 0.05;
				desc.inertia[0][0] = 0.0325;
				desc.inertia[0][1] = 0.0;
				desc.inertia[0][2] = 0.0;
				desc.inertia[1][0] = 0.0;
				desc.inertia[1][1] = 0.02;
				desc.inertia[1][2] = 0.0;
				desc.inertia[2][0] = 0.0;
				desc.inertia[2][1] = 0.0;
				desc.inertia[2][2] = 0.0325;

				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->SetAngularVelocity(Vec3f(0,0,0.2));
				soBox.back()->AddShape(meshCapsule);
				soBox.back()->SetFramePosition(Vec3f(0.5, 20,0));
				soBox.back()->GetShape(0)->SetVibration(-80,200,150);
				soBox.back()->GetShape(0)->SetElasticity(0.1);
//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				ostringstream os;
				os << "capsule" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			break;
		case 'b':
			{
				// MeshSphere
				desc.mass = 0.05;
				desc.inertia = 0.0288* Matrix3d::Unit();
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshSphere);
//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->GetShape(0)->SetElasticity(0.4);
				soBox.back()->SetFramePosition(Vec3f(0.5, 20,0));
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				ostringstream os;
				os << "sphere" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			break;
		case 'n':
			{
				// ConvexMesh
				desc.mass = 0.1;
				desc.inertia = 0.36 * Matrix3d::Unit();
				soBox.push_back(phscene->CreateSolid(desc));
				CDConvexMeshDesc md;
				int nv = rand() % 100 + 50;
				for(int i=0; i < nv; ++i){
					Vec3d v;
					for(int c=0; c<3; ++c){
						v[c] = (rand() % 100 / 100.0 - 0.5) * 5 * 1.3;
					}
					md.vertices.push_back(v);
				}
				CDShapeIf* s = GetSdk()->GetPHSdk()->CreateShape(md);
				soBox.back()->AddShape(s);
				soBox.back()->SetFramePosition(Vec3f(0.5, 20,0));
//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->GetShape(0)->SetVibration(-100,60,100);
				soBox.back()->GetShape(0)->SetStaticFriction(0.8);
				soBox.back()->GetShape(0)->SetDynamicFriction(0.6);

				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				ostringstream os;
				os << "sphere" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			break;
		case 'm':
			{
				// Lump of Box
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				Posed pose;
				pose.Pos() = Vec3d(3, 0, 0);
				soBox.back()->SetShapePose(1, pose);
				pose.Pos() = Vec3d(-3, 0, 0);
				soBox.back()->SetShapePose(2, pose);
				pose.Pos() = Vec3d(0, 3, 0);
				soBox.back()->SetShapePose(3, pose);
				pose.Pos() = Vec3d(0, -3, 0);
				soBox.back()->SetShapePose(4, pose);
				pose.Pos() = Vec3d(0, 0, 3);
				soBox.back()->SetShapePose(5, pose);
				pose.Pos() = Vec3d(0, 0, -3);
				soBox.back()->SetShapePose(6, pose);
				
				soBox.back()->SetFramePosition(Vec3f(0.5, 20,0));
	//			soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				ostringstream os;
				os << "box" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			    phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);				
			}
			break;	
		case 'x':
			{
				// Wall
				PHSolidDesc soliddesc;
				soliddesc.mass = 0.05;
				soliddesc.inertia = 0.0333 * Matrix3d::Unit();
				int wall_height = 4;
				int numbox = 5;
				for(int i = 0; i < wall_height; i++){
					for(int j = 0; j < numbox; j++){
						soBox.push_back(phscene->CreateSolid(soliddesc));
						soBox.back()->AddShape(meshBox);
						soBox.back()->SetFramePosition(Vec3d(-4.0 + (2.0 + 0.1) * j , (2.0 + 0.1) * (double)i, -2.0));  
						soBox.back()->GetShape(0)->SetVibration(-200,120,300);
						soBox.back()->GetShape(0)->SetStaticFriction(0.8);
						soBox.back()->GetShape(0)->SetDynamicFriction(0.6);

					}
				}
			}
			break;
		case 'z':
			{
				// Tower
				PHSolidDesc soliddesc;
				soliddesc.mass = 0.05;
				soliddesc.inertia = 0.0333 * Matrix3d::Unit();
				double tower_radius = 5;
				int tower_height = 5;
				int numbox = 5;
				double theta;
				for(int i = 0; i < tower_height; i++){
					for(int j = 0; j < numbox; j++){
						soBox.push_back(phscene->CreateSolid(soliddesc));
						soBox.back()->AddShape(meshBox);
						theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)numbox;
//						soBox.back()->SetFramePosition(Vec3f(0.5, 20, 0));
						soBox.back()->SetFramePosition(Vec3d(tower_radius * cos(theta), 2.0 * ((double)i), tower_radius * sin(theta)));
						soBox.back()->SetOrientation(Quaterniond::Rot(-theta, 'y'));  
						soBox.back()->GetShape(0)->SetVibration(-200,120,300);
						soBox.back()->GetShape(0)->SetStaticFriction(0.4);
						soBox.back()->GetShape(0)->SetDynamicFriction(0.3);
					}
				}
			}
			break;
		default:
			break;
	}
}
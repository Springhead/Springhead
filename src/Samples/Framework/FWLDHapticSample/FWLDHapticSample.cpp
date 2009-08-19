#include "FWLDHapticSample.h"
//#include "SampleModel.h"
#include <iostream>
#include <sstream>
#include <GL/glut.h>
#include "Foundation/UTMMTimer.h"
#include <Framework/FWInteractScene.h>

#define ESC 27

using namespace std;

FWLDHapticSample::FWLDHapticSample(){
	bDrawInfo = false;
}

void FWLDHapticSample::Init(int argc, char* argv[]){
	SetGRAdaptee(grAdapteeType::TypeGLUT);
	GetGRAdaptee()->Init(argc, argv);						// Sdkの作成
	CreateSdk();
	GetSdk()->Clear();										// SDKの初期化
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.02);

	FWWinDesc windowDesc;									// GLのウィンドウディスクリプタ
	windowDesc.title = "FWLDHapticSample";					// ウィンドウのタイトル
	CreateWin(windowDesc);									// ウィンドウの作成
	InitWindow();
	InitCameraView();										// カメラビューの初期化

	InitHumanInterface();
	FWInteractSceneDesc desc;
	desc.fwScene = GetSdk()->GetScene();
	desc.mode = LOCAL_DYNAMICS;
	desc.hdt = 0.001;
	CreateINScene(desc);

	BuildScene();											// 剛体を作成

	UTMMTimer* mtimer = CreateMMTimerFunc();
	mtimer->Resolution(1);
	mtimer->Interval(1);
	mtimer->Set(CallBackHapticLoop, NULL);
	mtimer->Create();
}

void FWLDHapticSample::InitCameraView(){
	//	Affinef 型が持つ、ストリームから行列を読み出す機能を利用して視点行列を初期化
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWLDHapticSample::InitHumanInterface(){
	CreateHISdk();
	DRUsb20SimpleDesc usbSimpleDesc;
	GetHISdk()->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i<10; ++i){
		usb20Sh4Desc.number = i;
		GetHISdk()->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	GetHISdk()->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());

	GetHISdk()->Init();
	GetHISdk()->Print(DSTR);
	UTRef<HISpidarGIf> spg = GetHISdk()->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidarGDesc("SpidarG6X3R"));
	AddHI(spg);
}

void FWLDHapticSample::IdleFunc(){
	CallBackPhysicsLoop();
}

void FWLDHapticSample::CallBackHapticLoop(void* arg){	
//	FWLDHapticSample::instance->GetInteractAdaptee()->CallBackHapticLoop();
	((FWLDHapticSample*)instance)->GetINScene()->CallBackHapticLoop();

}
void FWLDHapticSample::CallBackPhysicsLoop(){
	FWLDHapticSample::instance->GetINScene()->Step();
	glutPostRedisplay();
}

void FWLDHapticSample::Display(){
	// 描画モードの設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = GetCurrentWin()->render->Cast();
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDrawInfo);
	render->EnableRenderForce(bDrawInfo);
	render->EnableRenderContact(bDrawInfo);

	// カメラ座標の指定
	GRCameraIf* cam = GetCurrentWin()->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		GetCurrentWin()->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!GetCurrentWin()) return;
	GetSdk()->SwitchScene(GetCurrentWin()->GetScene());
	GetSdk()->SwitchRender(GetCurrentWin()->GetRender());
	GetSdk()->Draw();

	DisplayContactPlane();
	DisplayLineToNearestPoint();

	glutSwapBuffers();
}

void FWLDHapticSample::Reset(){
	UTMMTimer* mtimer = GetMMTimerFunc(0);
	mtimer->Release();
	GetINScene()->Clear();
	GetSdk()->GetScene()->GetPHScene()->Clear();
	BuildScene();
	mtimer->Create();
}

void FWLDHapticSample::BuildScene(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidDesc desc;
	CDBoxDesc bd;

	// 床(物理法則に従わない，運動が変化しない)
	{
		// 剛体(soFloor)の作成
		desc.mass = 1e20f;
		desc.inertia *= 1e30f;
		PHSolidIf* soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
		soFloor->SetDynamical(false);
		soFloor->SetGravity(false);
		// 形状(shapeFloor)の作成
		bd.boxsize = Vec3f(50, 10, 50);
		CDShapeIf* shapeFloor = GetSdk()->GetPHSdk()->CreateShape(bd);
		// 剛体に形状を付加する
		soFloor->AddShape(shapeFloor);
		soFloor->SetFramePosition(Vec3d(0, -10, 0));
	}

	// 箱(物理法則に従う，運動が変化)
	{
		// 剛体(soBox)の作成
		desc.mass = 0.05;
		desc.inertia *= 0.033;
		PHSolidIf* soBox = phscene->CreateSolid(desc);
		// 形状(shapeBox)の作成
		bd.boxsize = Vec3f(2,2,2);
		CDShapeIf* shapeBox = GetSdk()->GetPHSdk()->CreateShape(bd);
		// 剛体に形状を付加
		soBox->AddShape(shapeBox);
		soBox->SetFramePosition(Vec3d(-5, 10, 0));
	
		// ポインタ
		soBox = phscene->CreateSolid(desc);
		CDSphereDesc sd;
		sd.radius = 0.5;//1.0;
		CDSphereIf* shapePointer = DCAST(CDSphereIf,  GetSdk()->GetPHSdk()->CreateShape(sd));
		soBox->AddShape(shapePointer);
		soBox->SetDynamical(false);
		GetSdk()->GetScene()->GetPHScene()->SetContactMode(soBox, PHSceneDesc::MODE_NONE);
		FWInteractPointerDesc idesc;
		idesc.pointerSolid = soBox;;
		idesc.humanInterface = GetHI(0); 
		idesc.springK = 10;
		idesc.damperD = 0.0;
		idesc.posScale = 300;
		idesc.localRange = 0.7 * 10;
		GetINScene()->CreateINPointer(idesc);
	}
}

void FWLDHapticSample::DisplayContactPlane(){
	FWInteractScene* inScene = GetINScene()->Cast();
	int N = inScene->NINSolids();
	for(unsigned int i = 0; i <  N; i++){
		FWInteractSolid* inSolid = inScene->GetINSolid(i);
		if(!inSolid->bSim) continue;
		for(int j = 0; j < inScene->NINPointers(); j++){
			FWInteractPointer* inPointer = inScene->GetINPointer(j)->Cast();
			FWInteractInfo* inInfo = &inPointer->interactInfo[i];
			Vec3d pPoint = inPointer->pointerSolid->GetPose() * inInfo->neighborInfo.pointer_point;
			Vec3d cPoint = inSolid->sceneSolid->GetPose() * inInfo->neighborInfo.closest_point;
			Vec3d normal = inInfo->neighborInfo.face_normal;
			Vec3d v1(0,1,0);

			v1 +=  Vec3d(0, 0, 0.5) - Vec3d(0, 0, 0.5)*normal*normal;
			v1 -= v1*normal * normal;
			v1.unitize();
			Vec3d v2 = normal ^ v1;

			Vec4f moon(1.0, 1.0, 0.8, 0.3);
			GRDebugRenderIf* render = GetCurrentWin()->render->Cast();
			render->SetMaterial( GRMaterialDesc(moon) );
			render->PushModelMatrix();
			Vec3d offset = 0.02 * normal;
			render->SetLighting( false );
			render->SetAlphaTest(true);
			render->SetAlphaMode(render->BF_SRCALPHA, render->BF_ONE);
			cPoint += offset/2;
			glBegin(GL_QUADS);
				// 接触面底面	
				glVertex3d(cPoint[0] + v1[0] + v2[0], cPoint[1] + v1[1] + v2[1], cPoint[2] + v1[2] + v2[2]);
				glVertex3d(cPoint[0] - v1[0] + v2[0], cPoint[1] - v1[1] + v2[1], cPoint[2] - v1[2] + v2[2]);
				glVertex3d(cPoint[0] - v1[0] - v2[0], cPoint[1] - v1[1] - v2[1], cPoint[2] - v1[2] - v2[2]);
				glVertex3d(cPoint[0] + v1[0] - v2[0], cPoint[1] + v1[1] - v2[1], cPoint[2] + v1[2] - v2[2]);
				// 側面1
				glVertex3d(cPoint[0] + v1[0] + v2[0] + offset[0], 
								cPoint[1] + v1[1] + v2[1] + offset[1], 
								cPoint[2] + v1[2] + v2[2] + offset[2]);
				glVertex3d(cPoint[0] + v1[0] + v2[0], 
								cPoint[1] + v1[1] + v2[1], 
								cPoint[2] + v1[2] + v2[2]);
				glVertex3d(cPoint[0] - v1[0] + v2[0], 
								cPoint[1] - v1[1] + v2[1], 
								cPoint[2] - v1[2] + v2[2]);
				glVertex3d(cPoint[0] - v1[0] + v2[0] + offset[0], 
								cPoint[1] - v1[1] + v2[1] + offset[1], 
								cPoint[2] - v1[2] + v2[2] + offset[2]);
				// 側面2
				glVertex3d(cPoint[0] - v1[0] + v2[0] + offset[0], 
								cPoint[1] - v1[1] + v2[1] + offset[1], 
								cPoint[2] - v1[2] + v2[2] + offset[2]);
				glVertex3d(cPoint[0] - v1[0] + v2[0], 
								cPoint[1] - v1[1] + v2[1], 
								cPoint[2] - v1[2] + v2[2]);
				glVertex3d(cPoint[0] - v1[0] - v2[0], 
								cPoint[1] - v1[1] - v2[1], 
								cPoint[2] - v1[2] - v2[2]);
				glVertex3d(cPoint[0] - v1[0] - v2[0] + offset[0], 
								cPoint[1] - v1[1] - v2[1] + offset[1], 
								cPoint[2] - v1[2] - v2[2] + offset[2]);
				// 側面3
				glVertex3d(cPoint[0] - v1[0] - v2[0] + offset[0], 
								cPoint[1] - v1[1] - v2[1] + offset[1], 
								cPoint[2] - v1[2] - v2[2] + offset[2]);
				glVertex3d(cPoint[0] - v1[0] - v2[0], 
								cPoint[1] - v1[1] - v2[1], 
								cPoint[2] - v1[2] - v2[2]);
				glVertex3d(cPoint[0] + v1[0] - v2[0], 
								cPoint[1] + v1[1] - v2[1], 
								cPoint[2] + v1[2] - v2[2]);
				glVertex3d(cPoint[0] + v1[0] - v2[0] + offset[0], 
								cPoint[1] + v1[1] - v2[1] + offset[1], 
								cPoint[2] + v1[2] - v2[2] + offset[2]);
				// 側面4
				glVertex3d(cPoint[0] + v1[0] - v2[0] + offset[0], 
								cPoint[1] + v1[1] - v2[1] + offset[1], 
								cPoint[2] + v1[2] - v2[2] + offset[2]);
				glVertex3d(cPoint[0] + v1[0] - v2[0], 
								cPoint[1] + v1[1] - v2[1], 
								cPoint[2] + v1[2] - v2[2]);
				glVertex3d(cPoint[0] + v1[0] + v2[0], 
								cPoint[1] + v1[1] + v2[1],
								cPoint[2] + v1[2] + v2[2]);
				glVertex3d(cPoint[0] + v1[0] + v2[0] + offset[0], 
								cPoint[1] + v1[1] + v2[1] + offset[1], 
								cPoint[2] + v1[2] + v2[2] + offset[2]);
				// 接触面上面
				glVertex3d(cPoint[0] - v1[0] + v2[0] + offset[0], 
								cPoint[1] - v1[1] + v2[1] + offset[1], 
								cPoint[2] - v1[2] + v2[2] + offset[2]);
				glVertex3d(cPoint[0] + v1[0] + v2[0] + offset[0], 
								cPoint[1] + v1[1] + v2[1] + offset[1], 
								cPoint[2] + v1[2] + v2[2] + offset[2]);
				glVertex3d(cPoint[0] + v1[0] - v2[0] + offset[0], 
								cPoint[1] + v1[1] - v2[1] + offset[1], 
								cPoint[2] + v1[2] - v2[2] + offset[2]);
				glVertex3d(cPoint[0] - v1[0] - v2[0] + offset[0], 
								cPoint[1] - v1[1] - v2[1] + offset[1], 
								cPoint[2] - v1[2] - v2[2] + offset[2]);
			glEnd();
			render->SetLighting( true);
			render->SetAlphaTest(false);
			render->PopModelMatrix();
			glEnable(GL_DEPTH_TEST);
		}
	}
}

void FWLDHapticSample::DisplayLineToNearestPoint(){
	FWInteractScene* inScene = DCAST(FWInteractScene, GetINScene());
	int N = inScene->NINSolids();
	GLfloat moon[]={0.8,0.8,0.8};
	for(unsigned int i = 0; i <  N; i++){
		FWInteractSolid* inSolid = inScene->GetINSolid(i);
		if(!inSolid->bSim) continue;
		for(int j = 0; j < inScene->NINPointers(); j++){
			FWInteractPointer* inPointer = inScene->GetINPointer(j)->Cast();
			FWInteractInfo* inInfo = &inPointer->interactInfo[i];
			Vec3d pPoint = inPointer->pointerSolid->GetPose() * inInfo->neighborInfo.pointer_point;
			Vec3d cPoint = inSolid->sceneSolid->GetPose() * inInfo->neighborInfo.closest_point;
			Vec3d normal = inInfo->neighborInfo.face_normal;
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, moon);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, moon);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, moon);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, moon);
			glDisable(GL_DEPTH_TEST);
			glBegin(GL_LINES);
			glVertex3f(pPoint.X() + normal[0], pPoint.Y() + normal[1], pPoint.Z() + normal[2]);
			glVertex3f(cPoint.X(), cPoint.Y(), cPoint.Z());
			glEnd();
			glEnable(GL_DEPTH_TEST);
		}
	}
}

void FWLDHapticSample::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:
		case 'q':
			Exit();
			break;
		case 'r':
			Reset();
			break;
		case 'd':
			bDrawInfo = !bDrawInfo;
			break;
		//case '1':
		//	DSTR << "box" << std::endl;
		//	CreateBox(GetSdk());
		//	break;
		//case '2':
		//	DSTR << "sphere" << std::endl;
		//	CreateSphere(GetSdk());
		//	break;
		//case '3':
		//	DSTR << "capsule" << std::endl;
		//	CreateCapsule(GetSdk());
		//	break;
		//case '4':
		//	DSTR << "roundcone" << std::endl;
		//	CreateRoundCone(GetSdk());
		//	break;
		default:
			break;
	}
}
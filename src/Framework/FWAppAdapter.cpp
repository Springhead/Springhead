#include <windows.h>
#include <sstream>
#include <GL/glut.h>
#include "FWAppAdapter.h"

#define ESC 27
#define ENTER 13

/*MMTimeで呼ぶコールバック関数*/
/////////////////////////////////////////////////////////////////////////
FWAppImp* callbackimp;
void MMTimerCallBack(void* arg){
	static int dtimer = 1;
	if(dtimer > 50){
		glutPostRedisplay();
		dtimer = 1;
	}
	dtimer++;
	callbackimp->CallBack();
}

/*FWAppAdapterの実装*/
////////////////////////////////////////////////////////////////////////////
FWAppAdapter::FWAppAdapter(){
	imp = NULL;
	bDebug = false;
	bHaptic = false;
}

int FWAppAdapter::GetNImp(){return (int)imps.size();}
int FWAppAdapter::GetCurrentImpID(){ return impId; }

void FWAppAdapter::AddImp(FWAppImp* i){
	imps.push_back(i);
	imps.back()->SetAdapter(this);
}

void FWAppAdapter::SetCallBackImp(FWAppImp* i){ callbackimp = i; }

void FWAppAdapter::SwitchImp(int i){
	impId = i;
	imp = imps[i];
	SetCallBackImp(imp);
	DSTR << "Change to " << imp->GetSceneName() << endl;
}

void FWAppAdapter::InitHapticInterface(){
	DSTR << "Init Haptic Device" << endl;
	hisdk = HISdkIf::CreateSdk();
	DRUsb20SimpleDesc usbSimpleDesc;
	hisdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i<10; ++i){
		usb20Sh4Desc.number = i;
		hisdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	hisdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());

	hisdk->Init();
	hisdk->Print(DSTR);
	spidarG6 = hisdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	spidarG6->Init(&HISpidarGDesc("SpidarG6X3R"));

	for(int i = 0; i < (int)imps.size(); i++){
		imps[i]->SetHapticInterface(spidarG6);
	}
	DSTR << "Init Haptic Device Done." << endl;
}

void FWAppAdapter::Init(int argc, char* argv[]){
	InitHapticInterface();
	// 各シーンの初期化
	FWAppGLUT::Init(argc, argv);
	GetSdk()->Clear(); 

	for(int i = 0; i < (int)imps.size(); i++){
		SwitchImp(i);
		imp->Init();
	}
	imp->ResetScene();

	FWWinDesc windowDesc;
	windowDesc.top = 100;
	windowDesc.left = 250;
	windowDesc.width = 1200;
	windowDesc.height= 900;
	windowDesc.title = "FWApp";
	FWWin* window = CreateWin(windowDesc);
	window->scene = imp->GetScene();
	InitCameraView();
}

void FWAppAdapter::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	std::istringstream issView2(
		"((0.999985 0.000545773 -0.00543952 -0.0329072)"
		"(2.38872e-011 0.995004 0.0998334 0.603957)"
		"(0.00546684 -0.0998319 0.994989 6.01934)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
	issView2 >> cameraInfo.view;
}

// glutMainLoopで呼ぶのはIdle()のみ
void FWAppAdapter::Start(){
	instance = this;
	if (!NWin()){
		CreateWin();
		wins.back()->SetScene(imp->GetScene());
	}
//	glutTimerFunc(20, FWAppGLUT::GlutTimerFunc, 0);
	glutIdleFunc(FWAppGLUT::GlutIdleFunc);
	glutMainLoop();
}

void FWAppAdapter::Step(){
};

void FWAppAdapter::Idle(){
	imp->Idle();
}

void FWAppAdapter::Display(){
	FWWin* window = GetCurrentWin();
	window->SetScene(imp->GetScene());
	GRCameraIf* cam = window->scene->GetGRScene()->GetCamera();
	GRDebugRenderIf* render = window->render->Cast();

	// 描画モードの設定
	GetSdk()->SetDebugMode(true);	
	render->SetRenderMode(true, false);
//	render->EnableRenderAxis(bDebug);
	render->EnableRenderForce(bDebug);
	render->EnableRenderContact(false);			// 接触状態は割り込み処理で動的に変化するので常にfalse

	// カメラ座標の指定
	if(cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		window->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!window) return;
	GRRenderIf*curRender =  window->GetRender();
	FWSceneIf* curScene = window->GetScene();
	GetSdk()->SwitchRender(curRender);
	GetSdk()->SwitchScene(curScene);

	if(!curRender) return;
	curRender->ClearBuffer();
	curRender->BeginScene();

	if (curScene) curScene->Draw(curRender, GetSdk()->GetDebugMode());

	// 光源の追加
	GRLightDesc ld;
	ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
	ld.specular = Vec4f(1,1,1,1) * 0.8f;
	ld.ambient = Vec4f(1,1,1,1) * 0.4f;
	ld.position = Vec4f(1,1,1,0);
	render->PushLight(ld);
	//if(bDebug){
	//	imp->DebugMode(bDebug, render);
	//}
	//{
	//	Vec4f moon(1.0, 1.0, 0.8, 0.3);
	//	render->SetMaterial( GRMaterialDesc(moon) );
	//	render->PushModelMatrix();
	//	render->SetLighting( false );
	//	render->SetAlphaTest(true);
	//	render->SetAlphaMode(render->BF_SRCALPHA, render->BF_ONE);
	//	glTranslated(0,+ 0.5, 0); 
	//	glutSolidSphere(0.8, 10, 10);
	//	render->SetLighting( true);
	//	render->SetAlphaTest(false);
	//	render->PopModelMatrix();
	//	glEnable(GL_DEPTH_TEST);
	//}
	render->PopLight();	//	光源の削除

	curRender->EndScene();

	if(bDebug){
	}

	glutSwapBuffers(); 
}

void FWAppAdapter::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:		
		case 'q':
			timer.Release();
			DSTR << "Quit Program" << endl;
			exit(0);
			break;
		case ENTER:
			break;
		case ' ':
			break;
		case 'w':
			InitCameraView();
			break;
		case 'c':
			timer.Release();
			spidarG6->SetForce(Vec3d(), Vec3d());
			spidarG6->Calibration();
			DSTR << "Calibration" << endl;
			timer.Create();
			break;
		case 'f':
			timer.Release();
			if(bHaptic){
				bHaptic = false;
				for(int i = 0; i < (int)imps.size(); i++){
					imps[i]->Keyboard(key, x, y);
				}
				DSTR << "Force OFF" << endl;
			}else{
				bHaptic = true;
				for(int i = 0; i < (int)imps.size(); i++){
					imps[i]->Keyboard(key, x, y);
				}
				DSTR << "Force ON" << endl;
			}
			timer.Create();
			break;
		case 'd':
			if(bDebug) bDebug = false;
			else bDebug = true;
			break;
		default:
			timer.Release();
			imp->Keyboard(key, x, y);
			timer.Create();
			break;
	}
}
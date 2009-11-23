// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "FWAppGLtest.h"
#include "Windows.h"

void MyApp::Init(int argc, char* argv[]){
	#ifdef _DEBUG
	//	GetSdk()->SetDebugMode(true);
	#endif
	SetGRAdaptee(TypeGLUT);
	GRInit(argc, argv);										// Sdkの作成
	CreateSdk();
	GetSdk()->Clear();										// SDKの初期化
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(GetSdk()->GetPHSdk()->CreateShape(boxdesc));
	GetSdk()->LoadScene(FILE_NAME);
	TexInit();
	//GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.01);

	FWWinDesc windowDesc;									// GLのウィンドウディスクリプタ
	windowDesc.title = "Springhead2";						// ウィンドウのタイトル
	CreateWin(windowDesc);									// ウィンドウの作成
	InitWindow();

	int timerId = CreateTimer(FWTimer::GLUT);				// タイマーの生成
}
void MyApp::IdleFunc(){
	GRAnimationControllerIf* anim = GetSdk()->GetScene()->GetGRScene()->GetAnimationController();
	anim->ResetPose();
	static float time;
	anim->BlendPose(ANIMATION_SET_NAME, time, 1);
	time += 1;
	if (time > FRAME_NUMBER - 1) time = 0;
	Sleep(10);
	glutPostRedisplay();
}

void MyApp::Display(){
	static int timing = 0;
	FWWin* win = GetWin(0);
	win->render->ClearBuffer();
	win->render->BeginScene();

	GRCameraIf* cam = GetSdk()->GetScene() ? GetSdk()->GetScene()->GetGRScene()->GetCamera() : NULL;
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		win->render->SetViewMatrix(cameraInfo.view.inv());
	}

	GRMaterialDesc material;
	material.diffuse = Vec4f(1.0,0.8,0.4,1.0);
	win->render->SetMaterial(material);
		cam = GetSdk()->GetScene() ? GetSdk()->GetScene()->GetGRScene()->GetCamera() : NULL;
	if (cam && cam->GetFrame()){
		//Affinef cameraInfo.view = cam->GetFrame()->GetTransform();
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		win->render->SetViewMatrix(cameraInfo.view.inv());
	}

	DrawTexQuad();
	FWSceneIf* fwScene = GetSdk()->GetScene();
	if(fwScene) fwScene->Draw(win->render, false);

	win->render->EndScene();
	glutSwapBuffers();
}
void MyApp::TimerFunc(int id){
	GetSdk()->Step();
}
void MyApp::Keyboard(int key, int x, int y){
	if (key==0x1b){
		exit(0);
	}
	GRCameraIf* cam;
	switch(key){
	
		case('a'):	GetSdk()->GetScene()->FindObject(cam, "cam");
					cameraInfo.view = cam->GetFrame()->GetTransform();
					cameraInfo.view =  Affinef::Rot(Rad(5), 'y') * cameraInfo.view;
					cam->GetFrame()->SetTransform(cameraInfo.view);
					break;
	
		case('s'):  GetSdk()->GetScene()->FindObject(cam, "cam");
					cameraInfo.view = cam->GetFrame()->GetTransform();
					cameraInfo.view =  Affinef::Rot(Rad(-5), 'y') * cameraInfo.view;
					cam->GetFrame()->SetTransform(cameraInfo.view);
					break;
	
		case('w'):  GetSdk()->GetScene()->FindObject(cam, "cam");
					cameraInfo.view = cam->GetFrame()->GetTransform();
					cameraInfo.view =  Affinef::Rot(Rad(5), 'x') * cameraInfo.view;
					cam->GetFrame()->SetTransform(cameraInfo.view);
					break;

		case('z'):  GetSdk()->GetScene()->FindObject(cam, "cam");
					cameraInfo.view = cam->GetFrame()->GetTransform();
					cameraInfo.view =  Affinef::Rot(Rad(-5), 'x') * cameraInfo.view;
					cam->GetFrame()->SetTransform(cameraInfo.view);
					break;

		case('e'):  GetSdk()->GetScene()->FindObject(cam, "cam");
					cameraInfo.view = cam->GetFrame()->GetTransform();
					cameraInfo.view =  Affinef::Rot(Rad(5), 'z') * cameraInfo.view;
					cam->GetFrame()->SetTransform(cameraInfo.view);
					break;

		case('x'):  GetSdk()->GetScene()->FindObject(cam, "cam");
					cameraInfo.view = cam->GetFrame()->GetTransform();
					cameraInfo.view =  Affinef::Rot(Rad(-5), 'z') * cameraInfo.view;
					cam->GetFrame()->SetTransform(cameraInfo.view);
					break;

		case('d'):  GetSdk()->GetScene()->FindObject(cam, "cam");
					cameraInfo.view = cam->GetFrame()->GetTransform();
					cameraInfo.view = cameraInfo.view * Affinef::Trn(0,0,0.1);
					cam->GetFrame()->SetTransform(cameraInfo.view);
					break;

		case('f'):  GetSdk()->GetScene()->FindObject(cam, "cam");
					cameraInfo.view = cam->GetFrame()->GetTransform();
					cameraInfo.view = cameraInfo.view * Affinef::Trn(0,0,-0.1);
					cam->GetFrame()->SetTransform(cameraInfo.view);
					break;

	}
}
void MyApp::TexInit(){
	texSize = 256;
	for (int i = 0 ; i < texSize ; i++) {
		int r = (i * 0xFF) / texSize;
		for (int j = 0 ; j < texSize ; j++) {
			bits[i][j][0] = 0xFF;//(GLubyte)r;
			bits[i][j][1] = (GLubyte)(( j * 0xFF ) / texSize);
			bits[i][j][2] = 0xFF;//(GLubyte)~r;
		}
	}
	glGenTextures(1, &texName);
	
}
void MyApp::DrawTexQuad(){
	glEnable(GL_TEXTURE_2D);
	//GRMaterialDesc material;
	//material.ambient  = Vec4d(0.0, 0.0, 0.0, 0.0);
	//material.emissive = Vec4d(1.0, 1.0, 1.0, 1.0);
	//material.diffuse  = Vec4d(0.0, 0.0, 0.0, 0.0);
	//material.specular = Vec4d(0.0, 0.0, 0.0, 0.0);
	//material.power	  = 0.0;
	FWWin* win = GetWin(0);
	//win->render->SetMaterial(material);
	glBindTexture(GL_TEXTURE_2D , texName);
	glTexImage2D(
		GL_TEXTURE_2D , 0 , 3 , texSize , texSize,
		0 , GL_RGB , GL_UNSIGNED_BYTE , bits
	);
	glBegin(GL_QUADS);
		glTexCoord2f(0 , 0); glVertex2f(-1* win->width/2, -win->height/2);
		glTexCoord2f(0 , 1); glVertex2f(-1* win->width/2,  win->height/2);
		glTexCoord2f(1 , 1); glVertex2f(	win->width/2,  win->height/2);
		glTexCoord2f(1 , 0); glVertex2f(	win->width/2, -win->height/2);	
	glEnd();
}

/*
void idle(void){
	Affinef afV;
	afV.LookAt(Vec3f(10,0,0), Vec3f(0,1,0));
	GRAnimationControllerIf* anim = GetSdk()->GetScene()->GetGRScene()->GetAnimationController();
	anim->ResetPose();
	static float time;
	anim->BlendPose(ANIMATION_SET_NAME, time, 1);
	time += 1;
	if (time > FRAME_NUMBER - 1) time = 0;
	Sleep(10);
	glutPostRedisplay();
}
*/



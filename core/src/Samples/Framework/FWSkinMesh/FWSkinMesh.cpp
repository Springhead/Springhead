// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "FWSkinMesh.h"
#include "Windows.h"

void MyApp::Init(int argc, char* argv[]){
	CreateSdk();
	SetGRHandler(TypeGLUT);
	GRInit(argc, argv);
	
	// シーンをロード
	GetSdk()->LoadScene(FILE_NAME);
	//TexInit();
	// ウィンドウ
	CreateWin();
	GetCurrentWin()->SetDebugMode(false);

	// タイマ
	UTTimerIf* timer = CreateTimer();
	timer->SetInterval(10);
}

void MyApp::TimerFunc(int id){
	GRAnimationControllerIf* anim = GetCurrentWin()->GetScene()->GetGRScene()->GetAnimationController();
	anim->ResetPose();
	static float time;
	anim->BlendPose(ANIMATION_SET_NAME, time, 1);
	time += 1;
	if (time > FRAME_NUMBER - 1)
		time = 0;
	
	PostRedisplay();
}

void MyApp::Display(){
	FWApp::Display();
	/*static int timing = 0;
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

	DrawTexQuad();
	//FWSceneIf* fwScene = GetSdk()->GetScene();
	//if(fwScene) fwScene->Draw(win->render, false);
	GetSdk()->Draw();

	win->render->EndScene();
	glutSwapBuffers();*/
}

void MyApp::Keyboard(int key, int x, int y){
//	GRCameraIf* cam;
	switch(key){
	case DVKeyCode::ESC:
				exit(0);

/*	case('a'):	GetSdk()->GetScene()->FindObject(cam, "cam");
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
				break;*/
	}
}
/*void MyApp::TexInit(){
	texSize = 256;
	for (int i = 0 ; i < texSize ; i++) {
		int r = (i * 0xFF) / texSize;
		for (int j = 0 ; j < texSize ; j++) {
			bits[i][j][0] = 0xFF;//(GLubyte)r;
			bits[i][j][1] = (unsigned char)(( j * 0xFF ) / texSize);
			bits[i][j][2] = 0xFF;//(GLubyte)~r;
		}
	}
	glGenTextures(1, &texName);
	
}
void MyApp::DrawTexQuad(){
	glEnable(GL_TEXTURE_2D);
	FWWin* win = GetWin(0);
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
}*/



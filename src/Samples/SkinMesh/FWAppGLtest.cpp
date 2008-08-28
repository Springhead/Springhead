// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>
//#include <HumanInterface/HIBase.h>
#include <GL/glut.h>
#include <windows.h>
using namespace Spr;

class MyApp;
MyApp* app;

#define TEST_CASE 4

//人間モデルのロード
#if defined(TEST_CASE) && (TEST_CASE == 0)
#define FILE_NAME "test2.x"
#define ANIMATION_SET_NAME "Walk"
#define FRAME_NUMBER 72

#elif defined(TEST_CASE) && (TEST_CASE == 1)
#define FILE_NAME "test2.x"
#define ANIMATION_SET_NAME "Run"
#define FRAME_NUMBER 48

#elif defined(TEST_CASE) && (TEST_CASE == 2)
#define FILE_NAME "test2.x"
#define ANIMATION_SET_NAME "WaveOnesArm"
#define FRAME_NUMBER 48

#elif defined(TEST_CASE) && (TEST_CASE == 3)
#define FILE_NAME "test2.x"
#define ANIMATION_SET_NAME "RunGround"
#define FRAME_NUMBER 72

#elif defined(TEST_CASE) && (TEST_CASE == 4)
#define FILE_NAME "testkobito.x"
#define ANIMATION_SET_NAME "push"
#define FRAME_NUMBER 60

#endif


class MyApp: public FWAppGLUT{
public:
	int texSize;
	GLubyte bits[256][256][4];
	GLuint texName;
/*
	void Start(){
		instance = this;
		glutGameModeString("800x600:32@60");
		Sleep(500);
		windowID = glutEnterGameMode();
		CreateRender();
		glutDisplayFunc(FWAppGLUT::GlutDisplayFunc);
		glutReshapeFunc(FWAppGLUT::GlutReshapeFunc);
		glutKeyboardFunc(FWAppGLUT::GlutKeyboardFunc);
		glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, 0);
		glutMainLoop();
	}
*/
	void Idle(){
		GRAnimationControllerIf* anim = app->GetSdk()->GetScene()->GetGRScene()->GetAnimationController();
		anim->ResetPose();
		static float time;
		anim->BlendPose(ANIMATION_SET_NAME, time, 1);
		time += 1;
		if (time > FRAME_NUMBER - 1) time = 0;
		Sleep(10);
		glutPostRedisplay();
	}
	void DrawTexQuad(){
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
	void Display(){
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
		FWSceneIf* fwScene = fwSdk->GetScene();
		if(fwScene) fwScene->Draw(win->render, false);

		win->render->EndScene();
		glutSwapBuffers();
	}
	void Step(){
		FWAppGLUT::Step();
		PHSolidIf* s = DCAST(PHSolidIf,  GetSdk()->GetScene()->FindObject("soBlock1"));
	}
	void Keyboard(int key, int x, int y){
		if (key==0x1b){
			exit(0);
		}
		GRCameraIf* cam;
		switch(key){
		
			case('a'):	app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						cameraInfo.view = cam->GetFrame()->GetTransform();
						cameraInfo.view =  Affinef::Rot(Rad(5), 'y') * cameraInfo.view;
						cam->GetFrame()->SetTransform(cameraInfo.view);
						break;
		
			case('s'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						cameraInfo.view = cam->GetFrame()->GetTransform();
						cameraInfo.view =  Affinef::Rot(Rad(-5), 'y') * cameraInfo.view;
						cam->GetFrame()->SetTransform(cameraInfo.view);
						break;
		
			case('w'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						cameraInfo.view = cam->GetFrame()->GetTransform();
						cameraInfo.view =  Affinef::Rot(Rad(5), 'x') * cameraInfo.view;
						cam->GetFrame()->SetTransform(cameraInfo.view);
						break;

			case('z'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						cameraInfo.view = cam->GetFrame()->GetTransform();
						cameraInfo.view =  Affinef::Rot(Rad(-5), 'x') * cameraInfo.view;
						cam->GetFrame()->SetTransform(cameraInfo.view);
						break;

			case('e'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						cameraInfo.view = cam->GetFrame()->GetTransform();
						cameraInfo.view =  Affinef::Rot(Rad(5), 'z') * cameraInfo.view;
						cam->GetFrame()->SetTransform(cameraInfo.view);
						break;

			case('x'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						cameraInfo.view = cam->GetFrame()->GetTransform();
						cameraInfo.view =  Affinef::Rot(Rad(-5), 'z') * cameraInfo.view;
						cam->GetFrame()->SetTransform(cameraInfo.view);
						break;

			case('d'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						cameraInfo.view = cam->GetFrame()->GetTransform();
						cameraInfo.view = cameraInfo.view * Affinef::Trn(0,0,0.1);
						cam->GetFrame()->SetTransform(cameraInfo.view);
						break;

			case('f'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						cameraInfo.view = cam->GetFrame()->GetTransform();
						cameraInfo.view = cameraInfo.view * Affinef::Trn(0,0,-0.1);
						cam->GetFrame()->SetTransform(cameraInfo.view);
						break;

		}
	}
	void TexInit(){
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
};
MyApp app_;

/*
void idle(void){
	Affinef afV;
	afV.LookAt(Vec3f(10,0,0), Vec3f(0,1,0));
	GRAnimationControllerIf* anim = app->GetSdk()->GetScene()->GetGRScene()->GetAnimationController();
	anim->ResetPose();
	static float time;
	anim->BlendPose(ANIMATION_SET_NAME, time, 1);
	time += 1;
	if (time > FRAME_NUMBER - 1) time = 0;
	Sleep(10);
	glutPostRedisplay();
}
*/

int SPR_CDECL main(int argc, char* argv[]){
	app = &app_;
	app->Init(argc, argv);
#ifdef _DEBUG
//	app->GetSdk()->SetDebugMode(true);
#endif
	// glutIdleFunc(idle);
	app->GetSdk()->Clear();
	app->GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	PHSceneIf* phscene = app->GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));

	app->GetSdk()->LoadScene(FILE_NAME);
	
	app->TexInit();
//	app->GetFWScene()->AddHumanInterface(new HIMouse);

	app->Start();
	return 0;
}

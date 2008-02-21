// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>
#include <HumanInterface/HIBase.h>
#include <GL/glut.h>
#include <windows.h>
using namespace Spr;

class MyApp;
MyApp* app;

#define TEST_CASE 0

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

#endif


class MyApp: public FWAppGLUT{
public:
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
	void Display(){
		static int timing = 0;
		FWWin* win = GetWin(0);
		win->render->ClearBuffer();
		win->render->BeginScene();

		GRMaterialDesc material;
		material.diffuse = Vec4f(1.0,0.8,0.4,1.0);
		win->render->SetMaterial(material);
		FWSceneIf* fwScene = fwSdk->GetScene();
		if(fwScene) fwScene->Draw(win->render, false);

		win->render->EndScene();
		glutSwapBuffers();
	}
	void Step(){
		FWAppGLUT::Step();
		PHSolidIf* s = DCAST(PHSolidIf,  GetSdk()->GetScene()->FindObject("soBlock1"));
	}
	void Keyboard(unsigned char key, int x, int y){
		if (key==0x1b){
			exit(0);
		}
		Affinef af;
		GRCameraIf* cam;
		switch(key){
		
			case('a'):	app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						af = cam->GetFrame()->GetTransform();
						af =  Affinef::Rot(Rad(5), 'y') * af;
						cam->GetFrame()->SetTransform(af);
						break;
		
			case('s'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						af = cam->GetFrame()->GetTransform();
						af =  Affinef::Rot(Rad(-5), 'y') * af;
						cam->GetFrame()->SetTransform(af);
						break;
		
			case('w'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						af = cam->GetFrame()->GetTransform();
						af =  Affinef::Rot(Rad(5), 'x') * af;
						cam->GetFrame()->SetTransform(af);
						break;

			case('z'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						af = cam->GetFrame()->GetTransform();
						af =  Affinef::Rot(Rad(-5), 'x') * af;
						cam->GetFrame()->SetTransform(af);
						break;

			case('e'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						af = cam->GetFrame()->GetTransform();
						af =  Affinef::Rot(Rad(5), 'z') * af;
						cam->GetFrame()->SetTransform(af);
						break;

			case('x'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						af = cam->GetFrame()->GetTransform();
						af =  Affinef::Rot(Rad(-5), 'z') * af;
						cam->GetFrame()->SetTransform(af);
						break;

			case('d'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						af = cam->GetFrame()->GetTransform();
						af = af * Affinef::Trn(0,0,0.1);
						cam->GetFrame()->SetTransform(af);
						break;

			case('f'):  app->GetSdk()->GetScene()->FindObject(cam, "cam");
 						af = cam->GetFrame()->GetTransform();
						af = af * Affinef::Trn(0,0,-0.1);
						cam->GetFrame()->SetTransform(af);
						break;

		}
	}
};
MyApp app_;

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

int SPR_CDECL main(int argc, char* argv[]){
	app = &app_;
	app->Init(argc, argv);
#ifdef _DEBUG
//	app->GetSdk()->SetDebugMode(true);
#endif
	glutIdleFunc(idle);
	app->GetSdk()->Clear();
	app->GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	PHSceneIf* phscene = app->GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));

	app->GetSdk()->LoadScene(FILE_NAME);
//	app->GetFWScene()->AddHumanInterface(new HIMouse);

	app->Start();
	return 0;
}

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

class MyApp: public FWAppGLUT{
public:
	void Init(int argc, char* argv[]){
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		FWAppGL::Init(argc, argv);
		atexit(FWAppGLUT::AtExit);
	}
/*	void Start(){
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

int SPR_CDECL main(int argc, char* argv[]){
	app = &app_;
	app->Init(argc, argv);
#ifdef _DEBUG
//	app->GetSdk()->SetDebugMode(true);
#endif
	app->GetSdk()->Clear();
	app->GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	PHSceneIf* phscene = app->GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));

	app->GetSdk()->LoadScene("test2.x");
//	app->GetFWScene()->AddHumanInterface(new HIMouse);

	app->Start();
	return 0;
}

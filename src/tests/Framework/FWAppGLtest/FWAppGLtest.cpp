// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>
#include <HumanInterface/HIBase.h>
#include <GL/glut.h>

using namespace Spr;

class MyApp;
MyApp* app;

class MyApp: public FWAppGLUT{
public:
	void Step(){
		FWAppGLUT::Step();
		PHSolidIf* s = DCAST(PHSolidIf,  GetSdk()->GetScene()->FindObject("soBlock1"));
	}
	void Keyboard(unsigned char key, int x, int y){
		if (key==0x1b){
			exit(0);
		}
		GRCameraIf* cam;
		app->GetSdk()->GetScene()->FindObject(cam, "cam");
 		Affinef af = cam->GetFrame()->GetTransform();
		af =  Affinef::Rot(Rad(5), 'y') * af;
		cam->GetFrame()->SetTransform(af);
	}
};

void idle(void){
/*	Affinef afV;
	afV.LookAt(Vec3f(10,0,0), Vec3f(0,1,0));
*/
	GRFrameIf* frLeftFoot;
	app->GetSdk()->GetScene()->GetGRScene()->FindObject( frLeftFoot, "LeftFoot");
	static Affinef af;
	af = af * Affinef::Rot(Rad(5), 'X');
	frLeftFoot->SetTransform(af);
	glutPostRedisplay();

}

int SPR_CDECL main(int argc, char* argv[]){
	MyApp app_;
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

	app->GetSdk()->LoadScene("test2.x");
//	app->GetFWScene()->AddHumanInterface(new HIMouse);

	app->Start();
	return 0;
}

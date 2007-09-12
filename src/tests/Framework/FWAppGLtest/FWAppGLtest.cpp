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
#define FRAME_NAME "RightUpperArm"
#define ANIMATION_SET_NAME "WaveOnesArm"
#define FRAME_NUMBER 60

//X軸方向に拡大、縮小
#elif defined(TEST_CASE) && (TEST_CASE==1)
#define FILE_NAME "test3.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Amplification"
#define FRAME_NUMBER 60
//Y軸方向に拡大、縮小
#elif defined(TEST_CASE) && (TEST_CASE==2)
#define FILE_NAME "test4.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Amplification"
#define FRAME_NUMBER 60

//Z軸方向に拡大、縮小
#elif defined(TEST_CASE) && (TEST_CASE==3)
#define FILE_NAME "test5.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Amplification"
#define FRAME_NUMBER 60

//X軸方向に移動
#elif defined(TEST_CASE) && (TEST_CASE==4)
#define FILE_NAME "test6.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Movement"
#define FRAME_NUMBER 60

//Y軸方向に移動
#elif defined(TEST_CASE) && (TEST_CASE==5)
#define FILE_NAME "test7.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Movement"
#define FRAME_NUMBER 60

//Z軸方向に移動
#elif defined(TEST_CASE) && (TEST_CASE==6)
#define FILE_NAME "test8.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Movement"
#define FRAME_NUMBER 60

//X軸方向に回転
#elif defined(TEST_CASE) && (TEST_CASE==7)
#define FILE_NAME "test9.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Rotation"
#define FRAME_NUMBER 60

//Y軸方向に回転
#elif defined(TEST_CASE) && (TEST_CASE==8)
#define FILE_NAME "test10.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Rotation"
#define FRAME_NUMBER 60

//Z軸方向に回転
#elif defined(TEST_CASE) && (TEST_CASE==9)
#define FILE_NAME "test11.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Rotation"
#define FRAME_NUMBER 60

//X軸方向に回転⇒Y軸方向に回転
#elif defined(TEST_CASE) && (TEST_CASE==10)
#define FILE_NAME "test12.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "RotationXY"
#define FRAME_NUMBER 60

//X軸方向に回転⇒Y軸方向に回転
#elif defined(TEST_CASE) && (TEST_CASE==11)
#define FILE_NAME "test13.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "RotationXYZ"
#define FRAME_NUMBER 60

//X軸方向に回転⇒Y軸方向に回転
#elif defined(TEST_CASE) && (TEST_CASE==12)
#define FILE_NAME "test14.x"
#define FRAME_NAME "Hand"
#define ANIMATION_SET_NAME "Rotations"
#define FRAME_NUMBER 60

//X軸方向に回転⇒Y軸方向に回転
#elif defined(TEST_CASE) && (TEST_CASE==13)
#define FILE_NAME "test15.x"
#define FRAME_NAME "right"
#define ANIMATION_SET_NAME "kick"
#define FRAME_NUMBER 60

#endif


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
#if 0
	GRFrameIf* fr;
	app->GetSdk()->GetScene()->GetGRScene()->FindObject(fr, FRAME_NAME);
	if (fr){
		Affinef af = fr->GetTransform();
		af = af * Affinef::Rot(Rad(5), 'X');
		fr->SetTransform(af);
	}
#else
	GRAnimationControllerIf* anim = app->GetSdk()->GetScene()->GetGRScene()->GetAnimationController();
	anim->ResetPose();
	static float time;
	anim->BlendPose(ANIMATION_SET_NAME, time, 1);
	time += 1;
	if (time > FRAME_NUMBER - 1) time = 0;
	Sleep(10);
#endif
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

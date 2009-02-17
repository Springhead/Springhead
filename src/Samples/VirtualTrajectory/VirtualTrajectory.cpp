// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include <windows.h>
#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>
#include <gl/glut.h>

using namespace Spr;

class MyApp;
MyApp* app;
int quitCount = 0;

PHSceneIf* phScene;
PHBallJointIf* ballJoint;
PHHingeJointIf* hingeJoint;
PHSolidIf* bar;
PHSolidIf* flr;
PHSolidIf* ball;

UTRef<ObjectStatesIf> state = ObjectStatesIf::Create();

class MyApp: public FWAppGLUT{
protected:
	bool bRun;
	

public:
	bool isBall;

	MyApp():bRun(true), isBall(true){
	}
	void CalcBallJoint(){			//	トルクの計算
		state->SaveState(phScene);
		phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		ballJoint->SetMode(PHJointDesc::MODE_VELOCITY);
		Vec3d v(0,0,10);
		ballJoint->SetDesiredVelocity(v);
		phScene->Step();
		Vec3d f,t;
		ballJoint->GetConstraintForce(f, t);
		state->LoadState(phScene);

		//	実シミュレーション
		phScene->SetContactMode(PHSceneDesc::MODE_LCP);
		ballJoint->SetMode(PHJointDesc::MODE_POSITION);
		static Quaterniond q;
		q = Quaterniond::Rot(v * phScene->GetTimeStep()) * q;
		ballJoint->SetGoal(q);
		ballJoint->SetDesiredVelocity(v);
		ballJoint->SetOffsetForce(t);
	}
	void CalcHingeJoint(){			//	トルクの計算
		state->SaveState(phScene);
		phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		hingeJoint->SetMode(PHJointDesc::MODE_VELOCITY);
		double v = 10;
		hingeJoint->SetDesiredVelocity(v);
		phScene->Step();
		Vec3d f,t;
		hingeJoint->GetConstraintForce(f, t);
		Vec3d w;
		hingeJoint->GetRelativeVelocity(f, w);
		DSTR << hingeJoint->GetVelocity() << ", " << t << std::endl;
		state->LoadState(phScene);

		//	実シミュレーション
		phScene->SetContactMode(PHSceneDesc::MODE_LCP);
		hingeJoint->SetMode(PHJointDesc::MODE_POSITION);
		static double ori = 0;
		ori = v * phScene->GetTimeStep() + ori;
		hingeJoint->SetSpringOrigin(ori);
		hingeJoint->SetDesiredVelocity(v);
		hingeJoint->SetOffsetForce(t.Z());
	}
	void Step(){
		if (bRun){

			if(isBall){
				CalcBallJoint();
			}else{
				CalcHingeJoint();
			}
			phScene->Step();

			if(isBall){
				if(ballJoint->GetPosition().Rotation().norm() < 0.3){
					ball->SetFramePosition(Vec3d(0.2, 0.6 ,0));
					ball->SetVelocity(Vec3d());
				}
			}else{
				if(hingeJoint->GetPosition() < 0.3){
					ball->SetFramePosition(Vec3d(0.2, 0.6 ,0));
					ball->SetVelocity(Vec3d());
				}
			}
			SetCurrentWin(GetWin(0));
			glutPostRedisplay();
			if (GetWin(1)){
				SetCurrentWin(GetWin(1));
				glutPostRedisplay();
			}
		}
		quitCount ++;
		if (quitCount > 60*60*10) exit(0);
	}
	void Keyboard(int key, int x, int y){
		if (key==0x1b){
			delete app;
			exit(0);
		}
		static double stiff = 1.0;
		if (key == 's'){
			stiff /= 2;
			if(isBall){
				ballJoint->SetSpring(stiff*10);
				ballJoint->SetDamper(stiff);
			}else {
				hingeJoint->SetSpring(stiff*10);
				hingeJoint->SetDamper(stiff);
			}
			DSTR << "stiff:" << stiff << std::endl;
		}
		if (key == 'h'){
			stiff *= 2;
			if(isBall){
				ballJoint->SetSpring(stiff*10);
				ballJoint->SetDamper(stiff);
			}else {
				hingeJoint->SetSpring(stiff*10);
				hingeJoint->SetDamper(stiff);
			}
			DSTR << "stiff:" << stiff << std::endl;
		}
		if (key=='p'){
			if (!bRun){
				bRun = true;
				Step();
			}
			bRun = false;
		}
		if (key=='r'){
			bRun = true;
		}
	}
	void Display(){
		FWWin* wr = GetCurrentWin();
#ifdef _DEBUG
		GetSdk()->SetDebugMode(true);
		GRDebugRenderIf* r = wr->render->Cast();
		r->SetRenderMode(true, true);
//		r->EnableRenderAxis();
		r->EnableRenderForce();
		r->EnableRenderContact();
#endif
		GRCameraIf* cam = wr->scene->GetGRScene()->GetCamera();
		if (cam && cam->GetFrame()){
			//Affinef af = cam->GetFrame()->GetTransform();
			cam->GetFrame()->SetTransform(cameraInfo.view);
		}else{
			wr->render->SetViewMatrix(cameraInfo.view.inv());
		}
		FWAppGLUT::Display();
	}
};
void SetUpBallJoint(){
	PHBallJointDesc jdesc;
	//jdesc.limit[1].upper =  0.2;	// 最大スイング角
	//jdesc.limit[2].lower= -0.2;	// ツイスト角範囲
	//jdesc.limit[2].upper =  0.2;
	jdesc.poseSocket.Pos() = Vec3d(0, 0.2, 0);
	jdesc.posePlug.Pos() = Vec3d(0, -0.2, 0);
	ballJoint = phScene->CreateJoint(flr, bar, PHBallJointIf::GetIfInfoStatic(), jdesc)->Cast();
}

void SetUpHingeJoint(){
	PHHingeJointDesc jdesc;
	jdesc.spring = 0;
	jdesc.damper = 0;
	jdesc.poseSocket.Pos() = Vec3d(0, 0.2, 0);
	jdesc.posePlug.Pos() = Vec3d(0, -0.2, 0);
	hingeJoint = phScene->CreateJoint(flr, bar, jdesc)->Cast();
}

int SPR_CDECL main(int argc, char* argv[]){
	app = new MyApp;
	app->isBall = false;
	app->Init(argc, argv);
	app->GetSdk()->Clear();	//	SDK全体を初期化

	//	シーンの構築
	app->GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	phScene = app->GetSdk()->GetScene()->GetPHScene();
	flr = phScene->CreateSolid(PHSolidDesc());
	flr->SetDynamical(false);
	bar = phScene->CreateSolid(PHSolidDesc());
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.3, 0.1);
//	flr->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));
	bar->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));
	
	ball = phScene->CreateSolid(PHSolidDesc());
	ball->SetMass(100);
	CDSphereDesc sd;
	sd.radius = 0.05;
	ball->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(sd));

	if(app->isBall){
		SetUpBallJoint();
	}else{
		SetUpHingeJoint();
	}

	//	ウィンドウ1を作成
	FWAppGLUTDesc wd;
	wd.left = 0; wd.top = 0; wd.width = 500; wd.title = "original scene";
	app->CreateWin(wd);	//	作成と同時に，シーン0 がウィンドウ0 に割り当てられる．
	app->Start();
	return 0;
}



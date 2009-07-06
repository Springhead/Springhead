#include "VirtualHuman.h"

#include <iostream>
#include <sstream>
#include <ctime>
#include <string>

#include <GL/glut.h>

#include "Physics/PHConstraint.h"
#include "Physics/PHSpatial.h"
#include "Physics/PHScene.h"

#define ESC 27

VirtualHuman app;

VirtualHuman::VirtualHuman(){
	bGravity	= true;
	bDebug		= false;
	bStep		= true;
	bOneStep	= false;
	bIK			= false;

	gravity		= Vec3d(0.0, -9.8, 0.0);
	zP			= 1.0;
}

void VirtualHuman::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());

	BuildScene();

	FWWinDesc windowDesc;
	windowDesc.title = "Virtual Human Sample";
	window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();

	InitCameraView();
}

void VirtualHuman::Reset(int sceneNum){
	GetSdk()->Clear();
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());

	bGravity	= true;
	bDebug		= false;
	bStep		= true;
	bOneStep	= false;
	bIK			= false;

	BuildScene(sceneNum);

	GetCurrentWin()->SetScene(GetSdk()->GetScene());
	InitCameraView();
}

void VirtualHuman::InitCameraView(){
	istringstream iss(
		"((0.9996 0.00141267 -0.0282299 -0.0937266)"
		"(1.59067e-012 0.99875 0.0499792 0.165937)"
		"(0.0282652 -0.0499592 0.998351 3.31465)"
		"(	   0      0      0      1))"
		);
	iss >> cameraInfo.view;
}

bool VirtualHuman::LoadScene(UTString filename){
	//filename末端に改行コード( = 0x0a)が含まれているとロードされないので，あれば最初に削除する
	if(filename.at(filename.length()-1) == 0x0a){
		filename.erase(filename.length()-1);
	}
	//	デフォルトの先祖オブジェクトをを設定
	//	これらのCreateObjectが呼ばれてシーングラフが作られる。
	ObjectIfs objs;
	objs.Push(GetSdk()->GetGRSdk());	//	GRSdk
	objs.Push(GetSdk()->GetPHSdk());	//	PHSdk
	objs.Push(crSdk);					//	CRSdk
	//	FWSdk	FWScene は FWSdkの子になるので、FWSdkを最後にPushする必要がある。
	objs.Push(GetSdk()->Cast());
	int first = GetSdk()->NScene();	//	ロードされるFWSceneの位置を覚えておく

	//	ファイルローダーの作成
	UTRef<FIFileXIf> fiFileX = GetSdk()->GetFISdk()->CreateFileX();
	//	ファイルのロード
	if (! fiFileX->Load(objs, filename.data()) ) {
		DSTR << "Error: Cannot load file " << filename.c_str() << std::endl;
		//exit(EXIT_FAILURE);
		return false;
	}
	//	ロードしたシーンを取得
	DSTR << "Loaded " << GetSdk()->NScene() - first << " scenes." << std::endl;
	DSTR << "LoadFile Complete." << std::endl;
	for(int i=first; i<GetSdk()->NScene(); ++i){
		GetSdk()->SwitchScene(GetSdk()->GetScene(i));
		GetSdk()->GetScene(i)->Print(DSTR);
	}
	return true;
}

void VirtualHuman::BuildScene(int sceneNum){
	crSdk = CRSdkIf::CreateSdk();

	// ファイルからロード
	UTString filename = "test.x";
	LoadScene(filename);

	PHSdkIf*	phSdk	= GetSdk()->GetPHSdk();
	PHSceneIf*	phScene	= GetSdk()->GetScene()->GetPHScene();

	/*
	{
		PHSolidDesc descSolid;
		CDBoxDesc descBox;
		CDRoundConeDesc descRC;
		CDCapsuleDesc descCap;

		descSolid.dynamical = false;
		PHSolidIf* soFloor = phScene->CreateSolid(descSolid);
		soFloor->SetFramePosition(Vec3f(0,-2.5,0));
		descBox.boxsize = Vec3f(10,5,10);
		soFloor->AddShape(phSdk->CreateShape(descBox));

		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		soRC = phScene->CreateSolid(descSolid);
		soRC->SetFramePosition(Vec3f(0,5,0));
		soRC->SetOrientation(Quaterniond::Rot(Rad(45.0), 'x'));

		descRC.length = 1.0;
		descRC.radius = Vec2f(0.3, 0.5);
		soRC->AddShape(phSdk->CreateShape(descRC));
	}
	*/

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// ユーザの指
	PHSolidDesc descSolid;
	CDSphereDesc descSphere;
	descSolid.dynamical = false;
	soCursor = phScene->CreateSolid(descSolid);
	soCursor->SetFramePosition(Vec3f(0,5,0));
	descSphere.radius = 0.1;
	soCursor->AddShape(phSdk->CreateShape(descSphere));

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// ボディと到達運動コントローラ
	{
		crSdk->FindObject(creature, "creature1");
		if (creature) {
			// ボディ
			CRBallHumanBodyGenDesc bodyGenDesc;
			bodyGen = new CRBallHumanBodyGen(bodyGenDesc, phScene);

			PHSolidIf* waist = bodyGen->GetSolid(CRBallHumanBodyGenDesc::SO_WAIST);
			waist->SetDynamical(false);

			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_LEFT_ELBOW)->AddControlPoint(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_LEFT_HAND));
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_LEFT_SHOULDER)->AddControlPoint(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_LEFT_HAND));
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_ABDOMEN_CHEST)->AddControlPoint(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_LEFT_HAND));
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_WAIST_ABDOMEN)->AddControlPoint(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_LEFT_HAND));

			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_RIGHT_ELBOW)->AddControlPoint(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_RIGHT_HAND));
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_RIGHT_SHOULDER)->AddControlPoint(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_RIGHT_HAND));
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_ABDOMEN_CHEST)->AddControlPoint(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_RIGHT_HAND));
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_WAIST_ABDOMEN)->AddControlPoint(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_RIGHT_HAND));

			/*
			DCAST(PHBallJointIf,bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_WAIST_ABDOMEN))->SetSpring(2000.0);
			DCAST(PHBallJointIf,bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_WAIST_ABDOMEN))->SetDamper(  50.0);
			DCAST(PHBallJointIf,bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_ABDOMEN_CHEST))->SetSpring(1000.0);
			DCAST(PHBallJointIf,bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_ABDOMEN_CHEST))->SetDamper(  20.0);
			*/

			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_LEFT_SHOULDER)->SetBias(1.5);
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_LEFT_ELBOW)->SetBias(1);
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_ABDOMEN_CHEST)->SetBias(5);
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_WAIST_ABDOMEN)->SetBias(7);

			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_RIGHT_SHOULDER)->SetBias(1.5);
			bodyGen->GetIKNode(CRBallHumanBodyGenDesc::JO_RIGHT_ELBOW)->SetBias(1);

			CRBodyDesc bodyDesc;
			body = creature->CreateBody(bodyDesc);

			{
				CRIKSolidDesc iksd;
				iksd.label = "LeftHand";
				CRIKSolidIf* csoLeftHand = body->CreateObject(CRIKSolidIf::GetIfInfoStatic(), &iksd)->Cast();
				csoLeftHand->AddChildObject(bodyGen->GetSolid(CRBallHumanBodyGenDesc::SO_LEFT_HAND));
				csoLeftHand->AddChildObject(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_LEFT_HAND));
				body->AddChildObject(csoLeftHand);
			}
			{
				CRIKJointDesc ikjd;
				CRIKJointIf* cjo = body->CreateObject(CRIKJointIf::GetIfInfoStatic(), &ikjd)->Cast();
				cjo->AddChildObject(bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_LEFT_ELBOW));
				body->AddChildObject(cjo);
			}
			{
				CRIKJointDesc ikjd;
				CRIKJointIf* cjo = body->CreateObject(CRIKJointIf::GetIfInfoStatic(), &ikjd)->Cast();
				cjo->AddChildObject(bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_LEFT_SHOULDER));
				body->AddChildObject(cjo);
			}
			{
				CRIKJointDesc ikjd;
				CRIKJointIf* cjo = body->CreateObject(CRIKJointIf::GetIfInfoStatic(), &ikjd)->Cast();
				cjo->AddChildObject(bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_ABDOMEN_CHEST));
				body->AddChildObject(cjo);
			}
			{
				CRIKJointDesc ikjd;
				CRIKJointIf* cjo = body->CreateObject(CRIKJointIf::GetIfInfoStatic(), &ikjd)->Cast();
				cjo->AddChildObject(bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_WAIST_ABDOMEN));
				body->AddChildObject(cjo);
			}

			{
				CRIKSolidDesc iksd;
				iksd.label = "RightHand";
				CRIKSolidIf* csoRightHand = body->CreateObject(CRIKSolidIf::GetIfInfoStatic(), &iksd)->Cast();
				csoRightHand->AddChildObject(bodyGen->GetSolid(CRBallHumanBodyGenDesc::SO_RIGHT_HAND));
				csoRightHand->AddChildObject(bodyGen->GetControlPoint(2*CRBallHumanBodyGenDesc::SO_RIGHT_HAND));
				body->AddChildObject(csoRightHand);
			}
			{
				CRIKJointDesc ikjd;
				CRIKJointIf* cjo = body->CreateObject(CRIKJointIf::GetIfInfoStatic(), &ikjd)->Cast();
				cjo->AddChildObject(bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_RIGHT_ELBOW));
				body->AddChildObject(cjo);
			}
			{
				CRIKJointDesc ikjd;
				CRIKJointIf* cjo = body->CreateObject(CRIKJointIf::GetIfInfoStatic(), &ikjd)->Cast();
				cjo->AddChildObject(bodyGen->GetJoint(CRBallHumanBodyGenDesc::JO_RIGHT_SHOULDER));
				body->AddChildObject(cjo);
			}

			// 到達運動コントローラ
			// xFileからのロードに対応する実験としてあえてcsoLeftHandを直接代入せずにbodyから取得している
			creature->FindObject(reachLH, "reachLH");
			creature->FindObject(reachRH, "reachRH");
			for (size_t i=0; i<body->NChildObject(); ++i) {
				CRIKSolidIf* cso = body->GetChildObject(i)->Cast();
				if (cso) {
					if (std::string("LeftHand") == cso->GetLabel()) {
						reachLH->SetCRSolid(cso);
					}
					if (std::string("RightHand") == cso->GetLabel()) {
						reachRH->SetCRSolid(cso);
					}
				}
			}
		}
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// Sceneの設定
	/// IKの設定
	phScene->GetIKEngine()->SetNumIter(10);
	phScene->GetIKEngine()->Enable(bIK);
	/// その他の設定
	// phScene->SetGravity(gravity);
	// phScene->SetTimeStep(0.01);
	phScene->SetNumIteration(15);
	phScene->SetContactMode(PHSceneDesc::MODE_NONE);
}

void VirtualHuman::Step(){
	if(bStep) {
		OneStep();
	} else if (bOneStep) {
		OneStep();
		bOneStep = false;
	}

	glutPostRedisplay();
}

void VirtualHuman::OneStep(){
	PHSceneIf* phScene = GetSdk()->GetScene()->GetPHScene();

	phScene->ClearForce();

	crSdk->Step();

	phScene->GenerateForce();
	phScene->Integrate();
}

void VirtualHuman::Display(){
	// 描画の設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = window->render->Cast();

	// 描画モードの設定
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDebug);
	render->EnableRenderForce(bDebug);
	render->EnableRenderContact(bDebug);

	// カメラ座標の指定
	GRCameraIf* cam = window->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		window->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!GetCurrentWin()) return;
	GRRenderIf*curRender =  GetCurrentWin()->GetRender();
	FWSceneIf* curScene = GetCurrentWin()->GetScene();

	GetSdk()->SwitchScene(curScene);
	GetSdk()->SwitchRender(curRender);

	if(!curRender) return;
	curRender->ClearBuffer();
	curRender->BeginScene();

	if (curScene) curScene->Draw(curRender, GetSdk()->GetDebugMode());

	//	光源の追加
	GRLightDesc ld;
	ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
	ld.specular = Vec4f(1,1,1,1) * 0.8f;
	ld.ambient = Vec4f(1,1,1,1) * 0.4f;
	ld.position = Vec4f(1,1,1,0);
	render->PushLight(ld);

	/*
	CRReachingControllerIf* rLHand = reaches->GetReachingController(body->GetSolid(CRBallHumanBodyDesc::SO_LEFT_HAND));
	rLHand->Render(curRender);
	*/
	DCAST(GRDebugRenderIf,curRender)->SetMaterialSample(GRDebugRenderIf::RED);
	DCAST(CRReachingControllerIf,reachLH)->Render(curRender);

	render->PopLight();

	curRender->EndScene();
	glutSwapBuffers();
}

void VirtualHuman::Keyboard(int key, int x, int y){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	switch (key) {
		case ESC:

		case 'q':
			exit(0);
			break;

		case 'r':
			Reset();
			break;

		case 'w':
			InitCameraView();
			DSTR << "InitCameraView" << endl;
			break;

		case 'd':
			if(bDebug){
				bDebug = false;
				DSTR << "Debug Mode OFF" << endl;
			}else{
				bDebug = true;
				DSTR << "Debug Mode ON" << endl;
			}
			break;

		case 'g':
			if(bGravity){
				bGravity = false;
				phscene->SetGravity(Vec3d(0,0,0));
				DSTR << "Gravity OFF" << endl;
			}else{
				bGravity = true;
				phscene->SetGravity(gravity);
				DSTR << "Gravity ON" << endl;
			}
			break;

		case 's':
			bStep = false;
			bOneStep = true;
			break;

		case 'a':
			bStep = true;
			break;

		case 'f':
			{
				bIK = !bIK;
				DSTR << (bIK ? "IK ON" : "IK OFF") << endl;
				phscene->GetIKEngine()->Enable(bIK);
			}
			break;

		case '1':
			Reset(0);
			break;

			// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
			// 
			// 独自のキーファンクションはここで定義します．
			// 
			// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

		case 'o':
			{
				reachLH->Start(soCursor->GetPose().Pos(), Vec3d(), 0.5f);
				reachRH->Start(soCursor->GetPose().Pos(), Vec3d(), 0.5f);
			}
			break;

		case 'i':
			{
				// body->GetSolid(CRBallHumanBodyDesc::SO_LEFT_HAND)->AddForce(Vec3d(0, -10.0, 0));
				// soRC->AddTorque(Vec3d(100,0,0));
			}
			break;

		case 'z':
			{
				zP += 0.2;
				UpdateCursor(mouseInfo.lastPos[0], mouseInfo.lastPos[1]);
			}
			break;

		case 'x':
			{
				zP -= 0.2;
				UpdateCursor(mouseInfo.lastPos[0], mouseInfo.lastPos[1]);
			}
			break;

		default:
			break;
	}
}

void VirtualHuman::UpdateCursor(int x, int y){
	GRRenderIf* render = window->render->Cast();
	Affinef mat = cameraInfo.view;
	Vec2f  vs = render->GetViewportSize();
	Vec2f hVS = vs / 2.0;
	double  r = (vs[0] / vs[1]);
	Vec3f  vF = mat * Vec3f((x-hVS[0])/hVS[0]*zP, -(y-hVS[1])/hVS[1]*zP, -1*zP);
	soCursor->SetCenterPosition(vF);
}

void VirtualHuman::MouseButton(int button, int state, int x, int y){
	mouseInfo.lastPos.x = x, mouseInfo.lastPos.y = y;
	if(button == LEFT_BUTTON)
		mouseInfo.left = (state == BUTTON_DOWN);
	if(button == MIDDLE_BUTTON)
		mouseInfo.middle = (state == BUTTON_DOWN);
	if(button == RIGHT_BUTTON)
		mouseInfo.right = (state == BUTTON_DOWN);
	if(state == BUTTON_DOWN)
		mouseInfo.first = true;
	int mod = GetModifier();
	mouseInfo.shift = (mod & ACTIVE_SHIFT) != 0;
	mouseInfo.ctrl  = (mod & ACTIVE_CTRL) != 0;
	mouseInfo.alt   = (mod & ACTIVE_ALT) != 0;
}

void VirtualHuman::MouseMove(int x, int y){
	if (mouseInfo.middle) {
		UpdateCursor(x, y);
	} else {
		FWAppGLUT::MouseMove(x,y);
	}
}

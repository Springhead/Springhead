/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "PHOpDemo.h"
#include "Physics\PHOpSpHashColliAgent.h"
#include "Physics\PHOpEngine.h"


#define USE_SPRFILE
#define ESC 27
#define USE_AVG_RADIUS
#define COLLISION_DEMO

#ifndef COLLISION_DEMO
#define HAPTIC_DEMO
#endif

#ifdef HAPTIC_DEMO
//#define _3DOF

#ifndef _3DOF
#define _6DOF
#endif

#endif

using namespace std;

int hapticObjId;

PHOpDemo::PHOpDemo(){
#ifdef COLLISION_DEMO
	fileName = "./files/sceneSampleColli.spr";	// sprファイル
#else if  HAPTIC_DEMO
	fileName = "./files/sceneSampleHaptic.spr";
#endif
//	fileName = "./files/sceneSample.spr";

	gravity = false;
	drawVertex = false;
	drawPs = false;
	//fileOp = new FIOpStateHandlerIf();
	fileVersion = 2.1f;
}

void PHOpDemo::Init(int argc, char* argv[]){
	CreateSdk();			// SDKの作成
	UTRef<ImportIf> import = GetSdk()->GetFISdk()->CreateImport();
	GetSdk()->LoadScene(fileName, import);			// ファイルのロード
	//GetSdk()->SaveScene("save.spr", import);		// ファイルのセーブテスト
	GRInit(argc, argv);		// ウィンドウマネジャ初期化
	CreateWin();			// ウィンドウを作成
	CreateTimer();			// タイマを作成

	InitCameraView();		// カメラビューの初期化
	GetSdk()->SetDebugMode(false);						// デバックモードの無効化
	GetSdk()->GetScene()->EnableRenderAxis(true);		// 座標軸の表示
	GetSdk()->GetScene()->EnableRenderContact(true);	// 接触領域の表示

	HISdkIf* hisdk = GetSdk()->GetHISdk();
	int U = 0;

	PHOpEngineIf* opEngine = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
	for (int obji = 0; obji< (int)opEngine->GetOpObjNum(); obji++)
	{
		opEngine->GetOpObjIf(obji)->SetDefaultLinkNum(10);


	}

	//initial op objects
	FWOpObjIf *tmp = GetSdk()->GetScene()->FindObject("fwSLBunny")->Cast();
//#ifdef _3DOF
	tmp->CreateOpObj();
//#endif
	
	
#ifdef HAPTIC_DEMO
	//initial for haptic
	PHOpEngineIf* opEngine = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
	
	int objid;
	
	hapticObjId = opEngine->AddOpObj();
	PHOpObjIf *opObjIf = opEngine->GetOpObjIf(hapticObjId);
	Vec3f* vArr;
	vArr = new Vec3f[((GRMesh*)tmp->GetGRMesh())->vertices.size()];
	//vector<int> a;
	for (int vi = 0; vi<((GRMesh*)tmp->GetGRMesh())->vertices.size(); vi++) {
		Vec3f v;// = new Vec3f();
		v.x = ((GRMesh*)tmp->GetGRMesh())->vertices[vi].x;
		v.y = ((GRMesh*)tmp->GetGRMesh())->vertices[vi].y;
		v.z = ((GRMesh*)tmp->GetGRMesh())->vertices[vi].z;

		opObjIf->AddVertextoLocalBuffer(v);
	}
	opObjIf->InitialObjUsingLocalBuffer(0.5f);
#ifdef _6DOF
	opEngine->InitialHapticRenderer(1, hisdk);
#endif
#ifdef _3DOF
	opEngine->InitialNoMeshHapticRenderer();
#endif
	


	PHOpHapticControllerIf* opHc = (PHOpHapticControllerIf*)opEngine->GetOpHapticController();
	opHc->setC_ObstacleRadius(0.2f);
	PHOpHapticRendererIf* opHr = (PHOpHapticRendererIf*)opEngine->GetOpHapticRenderer();
	
	//for clip
	//opEngine->InitialHapticRenderer(1, hisdk);
	//PHOpHapticRendererIf* opHr = (PHOpHapticRendererIf*)opEngine->GetOpHapticRenderer();

	////for rigid Clip
	//opHr->SetRigid(true);

	//opAnimator = (PHOpAnimationIf*)opEngine->GetOpAnimator();
	//opAnimator->AddAnimationP(1, 38, Vec3f(0, -300, 0), 100);
	//for (int pi = 48; pi < 56; pi++)
	//{
	//	opAnimator->AddAnimationP(1, pi, Vec3f(0, -300, 0), 100);
	//}


	//opHrDesc = DCAST(PHOpHapticRenderer, opHr);
#endif

#ifdef COLLISION_DEMO

	FWOpObjIf *tmp2 = GetSdk()->GetScene()->FindObject("fwSLBunny2")->Cast();
	tmp2->CreateOpObj();

	//initial collision detection
	PHOpSpHashColliAgentIf* spIf;
	spIf = GetSdk()->GetScene()->GetPHScene() -> GetOpColliAgent();

	Bounds bounds;
	float boundcube = ((PHOpObjIf*)tmp->GetOpObj())->GetBoundLength();
	bounds.min.x = -boundcube;
	bounds.min.y = -boundcube;
	bounds.min.z = -boundcube;
	bounds.max.x = boundcube;
	bounds.max.y = boundcube;
	bounds.max.z = boundcube;

	spIf->Initial(0.5f, bounds);

	spIf->EnableCollisionDetection(false);
#endif
	
	PHOpEngine* opEnginedesc = DCAST(PHOpEngine, opEngine);
	

	PHOpObjDesc* dp1 = opEnginedesc->opObjs[0];
	PHOpObjDesc* dp2 = opEnginedesc->opObjs[1];
	cout << "obji = 0" << "pNum" << dp1->assPsNum << "gNum" << dp1-> assGrpNum<< endl;
	cout << "obji = 1" << "pNum" << dp2->assPsNum << "gNum" << dp2->assGrpNum << endl;

	DrawHelpInfo = true;
	checkPtclInfo = true;
	useMouseSelect = false;
	useMouseSelect = false;
	useMouseSlcObj = false;
	useAnime = false;
	pgroupEditionModel = false;
	fixPs = false;
	mouseX = 0.0f;
	mouseY = 0.0f;
	recX = 0.0f;
	recY = 0.0f;
	mouseHit = false;
	mouseLbtn = false;
	mouseVertmotion = false;
	pgroupEditionModel = false;
	runByStep = false;
	addGrpLink = true;
	ediFirP = true;
	render = GetCurrentWin()->GetRender();
	radiusCoe = 1.0f;
	
	//STEPタイマ作成
	UTTimerIf* SimuTimer = CreateTimer(UTTimerIf::FRAMEWORK);
	int intervaltime = 10;
	//FEMdt = intervaltime / 1000.0; 
	SimuTimer->SetInterval(intervaltime);
	SimuTimer->SetResolution(1);
	opSimuTimerId = SimuTimer->GetID();

	//カメラ位置調整
	Vec3d pos = Vec3d(0.0, 5.0, 10.0);		// カメラ初期位置
	GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定
	float rmin, rmax;
	GetCurrentWin()->GetTrackball()->GetLongitudeRange(rmin, rmax);
	rmin *= 2; rmax *= 2;
	GetCurrentWin()->GetTrackball()->SetLongitudeRange(rmin, rmax);

}
bool PHOpDemo::OnMouse(int button, int state, int x, int y)
{
	MouseButton(button, state, x, y);
	return true;
}
void PHOpDemo::InitCameraView(){
	Vec3d pos = Vec3d(-0.978414, 11.5185, 24.4473);		// カメラ初期位置
	GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定
}

void PHOpDemo::Reset(){
	GetSdk()->Clear();
	GetSdk()->LoadScene(fileName);
	GetCurrentWin()->SetScene(GetSdk()->GetScene());
}

void PHOpDemo::TimerFunc(int id)
{
	FWOpObjIf *tmp = GetSdk()->GetScene()->FindObject("fwSLBunny")->Cast();
	PHOpEngineIf* opEngine = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
	PHOpObjIf *opObjIf = opEngine->GetOpObjIf(hapticObjId);

	FWApp::TimerFunc(id);
	if (id == opSimuTimerId)
	{
		//OpのシミュレーションStepをコール
		PHOpEngineIf* opEngine = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
		//opEngine->Step();
		if (!runByStep)
		opEngine->StepWithBlend();
		PostRedisplay();
	}

	//blend
	GRMeshIf *grMesh = (GRMeshIf*)tmp->GetGRMesh();
	for (int vi = 0; vi < opObjIf->GetVertexNum(); vi++)
	{
		grMesh->GetVertices()[vi] = opObjIf->GetVertex(vi);
		
	}

	//save tst pos
	if (recordingPos)
		SaveTstPPos("TstP.dfmobj", 55, dpAdd->pCurrCtr);

	/*if (useAnime)
		opAnimator->AnimationStep(opEngine->GetDescAddress());*/
}

void PHOpDemo::SaveTstPPos(char *filename, int pi, Vec3f Pos)
{
	FILE *f;
	fopen_s(&f, filename, "a");
	if (!f)
	{
		std::cout << " file dir can not open" << endl;
		return;
	}
	fprintf(f, "Pos:");
	fprintf(f, "%d  ", pi);
	fprintf(f, "%f %f %f\n", Pos.x, Pos.y, Pos.z);

	fclose(f);
}

void PHOpDemo::Keyboard(int key, int x, int y){

	float alpha, beta;
	int itr = -1;
	float vd;
	PHOpSpHashColliAgentIf* spIf = GetSdk()->GetScene()->GetPHScene()->GetOpColliAgent();
	

	//PHOpObj *opObj = GetSdk()->GetScene()->FindObject("phObj")->Cast();
	PHOpEngineIf* opEngineif = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();

	PHOpHapticControllerIf* opHc = NULL;
#ifdef HAPTIC_DEMO
	opHc= (PHOpHapticControllerIf*)opEngineif->GetOpHapticController();
#endif
	//PHOpEngine* opEngine = DCAST(PHOpEngine, opEngineif);
	PHOpObjIf* objif = opEngineif->GetOpObjIf(0);
	PHOpObjIf* objif2 = NULL;
	if (opEngineif->GetOpObjNum()>1)
		objif2 = opEngineif->GetOpObjIf(1);
	PHOpParticleIf *dpif = (PHOpParticleIf*)objif->GetOpParticle(0);
	PHOpParticleDesc *dp = dpif->GetParticleDesc();


	PHOpObjIf *opObjIf = opEngineif->GetOpObjIf(1);
	void * dp1;
	


	switch (key) {
	case ESC:
	case 'q':
		// アプリケーションの終了
		exit(0);
		break;
	case '=':
		recordingPos = !recordingPos;
		cout << "recording" << endl;
		break;
	case '<':
		opEngineif->SetDrawPtclR(opEngineif->GetDrawPtclR() + 0.3f);
		
		cout << "Radius++ " << opEngineif->GetDrawPtclR() << endl;
		break;
	case '>':
		opEngineif->SetDrawPtclR(opEngineif->GetDrawPtclR() - 0.3f);
		cout << "Radius-- " << opEngineif->GetDrawPtclR() << endl;
		break;
	case 'C':
		drawCtcP = !drawCtcP;
		cout << "drawCtcP" << drawCtcP<< endl;
		break;
	case'i':
		opEngineif->SetProxyCorrectionEnable(!opEngineif->IsProxyCorrection());
		if (opEngineif->IsProxyCorrection())
			DSTR << "Enable ProxyCorrection" << std::endl;
		else DSTR << "Disable ProxyCorrection" << std::endl;
		break;
	case'o':
		opEngineif->SetHapticSolveEnable(!opEngineif->IsHapticSolve());
		if (opEngineif->IsHapticSolve())
			DSTR << "Enable HapticSolve" << std::endl;
		else DSTR << "Disable HapticSolve" << std::endl;
		break;
	case 'w':
		useAnime = !useAnime;
		opEngineif->SetAnimationFlag(useAnime);
		cout << "useAnime" << useAnime << endl;
		break;
	case't' :
		if (!opEngineif->IsHapticEnabled())
		{
			if (opEngineif->TrySetHapticEnable(true,GetSdk()->GetHISdk()))
			{

				DSTR << "Enable Haptic" << std::endl;

			}
			else
			{
				DSTR << "Haptic Initial Failed" << std::endl;
			}
		}
		else {
			if (opEngineif->TrySetHapticEnable(false, GetSdk()->GetHISdk()))
			{

				DSTR << "Disable Haptic" << std::endl;
			}
			else DSTR << "Haptic Initial Failed" << std::endl;
		}
		break;
	case 'b':
		dp->pCurrCtr.y += dp->pCurrCtr.y;
		break;
	case 'c':
		spIf->EnableCollisionDetection(!spIf->IsCollisionEnabled());
		if (spIf->IsCollisionEnabled())
		DSTR << "Enable Collision detection" << std::endl;
		else DSTR << "Disable Collision detection" << std::endl;
		break;
	case '3':
		//Adjust iteration count
		for (int obji = 0; obji< (int)opEngineif->GetOpObjNum(); obji++)
		{
			itr = ((PHOpObjIf*)opEngineif->GetOpObjIf(obji))->GetObjItrTime();
			++itr;
			opEngineif->GetOpObjIf(obji)->SetObjItrTime(itr);
		}
		DSTR << "increase Op iteration num =" << itr <<std::endl;
		break;
	case '4':
		//Adjust iteration count
		for (int obji = 0; obji < (int)opEngineif->GetOpObjNum(); obji++)
		{
			itr = ((PHOpObjIf*)opEngineif->GetOpObjIf(obji))->GetObjItrTime();
			--itr;
			opEngineif->GetOpObjIf(obji)->SetObjItrTime(itr);
		}
		DSTR << "decrease Op iteration num =" << itr << std::endl;
		break;
	case '+':
		//Adjust Alpha of op
		for (int obji = 0; obji< (int)opEngineif->GetOpObjNum(); obji++)
		{
			alpha = ((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->GetObjAlpha();
			
			if (alpha<0.9501f)
				alpha += 0.05f;
			((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->SetObjAlpha(alpha);
		}
		DSTR << "increase alpha =" << alpha << std::endl;
		break;
	case '-':
		//Adjust Alpha of op
		for (int obji = 0; obji< (int)opEngineif->GetOpObjNum(); obji++)
		{
			alpha = ((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->GetObjAlpha();
		
			if (alpha>0.0f)
				alpha -= 0.05f;
			((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->SetObjAlpha(alpha);
		}
		DSTR << "decrease alpha =" << alpha << std::endl;
		break;
	case '*':
		//Adjust Beta of op
		for (int obji = 0; obji< (int)opEngineif->GetOpObjNum(); obji++)
		{
			beta = ((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->GetObjBeta();

			if (beta<0.9501f)
				beta += 0.05f;
			((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->SetObjBeta(beta);
		}
		DSTR << "increase beta =" << beta << std::endl;
		break;
	case '/':
		//Adjust Beta of op
		for (int obji = 0; obji< (int)opEngineif->GetOpObjNum(); obji++)
		{
			beta = ((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->GetObjBeta();

			if (beta>0.0f)
				beta -= 0.05f;
			((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->SetObjBeta(beta);
		}
		DSTR << "decrease beta =" << beta << std::endl;
		break;
	case'h':
		//show help information
		DrawHelpInfo = !DrawHelpInfo;
		break;
	case'l':
		drawlink = !drawlink;
		cout << "drawlink" << drawlink << endl;
		break;
	case 'e':
		//edit particle link model
		pgroupEditionModel = !pgroupEditionModel;
		if (pgroupEditionModel)
			DSTR << "Enter pGroupEditingModel" << endl;
		else DSTR << "Exit pGroupEditingModel" << endl;
		break;
	case 'E':
		//change "add" or "delete link" in edit model
		addGrpLink = !addGrpLink;
		if (addGrpLink)
			DSTR << "Add Pgroup Function" << endl;
		else DSTR << "Del Pgroup Function" << endl;
		break;
	case 'f':
		//Use mouse to fix touched particle
		fixPs = !fixPs;
		if (fixPs)
			DSTR << "EnterFixParticleModel" << std::endl;
		else DSTR << "ExitFixParticleModel" << std::endl;
		break;
	case 'g':
		//Enable gravity
		gravity = !gravity;
		opEngineif->SetGravity(gravity);
		if (gravity)
			DSTR << "EnableGravity" << std::endl;
		else DSTR << "DisableGravity" << std::endl;
		break;
	case 'p':
		//Draw particle in physics scene
		drawPs = !drawPs;
		GetSdk()->GetScene()->EnableRenderOp(drawPs);
		if (gravity)
			DSTR << "DrawParticles" << std::endl;
		else DSTR << "ClearDrawParticles" << std::endl;
		break;
	case 'y':
		//Enable mouse function
		useMouseSelect = !useMouseSelect;
		DSTR << "useMouseSelect" << useMouseSelect << std::endl;;
		break;
	
	case 'u':
		//Enable globle mouse catch
		useMouseSlcObj = !useMouseSlcObj;
		DSTR << "useMouseSlcObj" << useMouseSlcObj << std::endl;;
		break;

	case 'D':
		//Enable Distance Constraint
		for (int obji = 0; obji< (int)opEngineif->GetOpObjNum(); obji++)
		{
			((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->SetObjDstConstraint(!((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->GetObjDstConstraint());
		}
		DSTR << "useDistCstr" << ((PHOpObjIf*)opEngineif->GetOpObjIf(0)->Cast())->GetObjDstConstraint() << std::endl;;
		break;
#ifdef HAPTIC_DEMO
	case ',':
		//adjust constraint spring of haptic
		opHrDesc->constraintSpring -= 0.2f;
		DSTR << "constraintSpring is " << opHrDesc->constraintSpring << std::endl;
		break;
	case '.':
		//adjust constraint spring of haptic
		opHrDesc->constraintSpring += 0.2f;
		DSTR << "constraintSpring is " << opHrDesc->constraintSpring << std::endl;
		break;
	case '8':
		//adjust forceSpring spring of haptic
		opHrDesc->forceSpring -= 0.2f;
		DSTR << "forceSpring is " << opHrDesc->forceSpring << std::endl;
		break;
	case '9':
		//adjust forceSpring spring of haptic
		opHrDesc->forceSpring += 0.2f;
		DSTR << "forceSpring is " << opHrDesc->forceSpring << std::endl;
		break;
#endif
	case '(':
		//Adjust velocity damping coefficient
		for (int obji = 0; obji< (int)opEngineif->GetOpObjNum(); obji++)
		{
			//opEngine->opObjs[obji]->params.veloDamping += 0.05f;
			vd = ((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->GetVelocityDamping();
			vd += 0.05f;
			((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->SetVelocityDamping(vd);
		}
		DSTR << "params.veloDamping +=0.05f; " << vd << std::endl;;
		break;
	case ')':
		//Adjust velocity damping coefficient
		for (int obji = 0; obji< (int)opEngineif->GetOpObjNum(); obji++)
		{
			vd = ((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->GetVelocityDamping();
			vd -= 0.05f;
			((PHOpObjIf*)opEngineif->GetOpObjIf(obji)->Cast())->SetVelocityDamping(vd);
		}
		DSTR << "params.veloDamping -=0.05f; " << vd<< std::endl;;
		break;
	case 257:// key "F1"
 		//mesh state Save1 
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Save FirObjState1" << endl;
			fileOp->saveToFile(objif, "opDeformObject1.dfOp", fileVersion);
		}
		break;
	case 258:// key "F2"
		//mesh state Save2
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Save FirObjState2" << endl;
			fileOp->saveToFile(objif, "opDeformObject2.dfOp", fileVersion);
		}
		break;
	case 259:// key "F3"
		//mesh state Save3
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Save FirObjState3" << endl;

			fileOp->saveToFile(objif, "opDeformObject3.dfOp", fileVersion);
		}
		break;
	case 260:// key "F4"
		//mesh state Save4 
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Save FirObjState4" << endl;
			fileOp->saveToFile(objif, "opDeformObject4.dfOp", fileVersion);
		}
		break;
	case 261:// key "F5"
		//mesh state Save5 
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Load FirObjState1" << endl;
			fileOp->loadFromFile(objif, "opDeformObject1.dfOp", false, fileVersion);
		}
		break;
	case 262:// key "F6"
		//mesh state Save6 
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "load FirObjState2" << endl;
			fileOp->loadFromFile(objif, "opDeformObject2.dfOp", false, fileVersion);
		}
		break;
	case 263:// key "F7"
		//mesh state Save7
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Load FirObjState3" << endl;
			fileOp->loadFromFile(objif, "opDeformObject3.dfOp", false, fileVersion);
		}
		break;
	case 264:// key "F8"
		//mesh state Save8
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "load FirObjState4" << endl;
			fileOp->loadFromFile(objif, "opDeformObject4.dfOp", false, fileVersion);
		}
		break;
	case 265:// key "F9"
		//mesh state Save
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Save SecObjState1" << endl;
			fileOp->saveToFile(objif2, "opDeformObject1_other.dfOp", fileVersion);
		}
		break;
	case 266:// key "F10"
		//mesh state Save 
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Save SecObjState2" << endl;
			fileOp->saveToFile(objif2, "opDeformObject2_other.dfOp", fileVersion);
		}
		break;
	case 267:// key "F11"
		//mesh state Load
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "Load SecObjState1" << endl;
			fileOp->loadFromFile(objif2, "opDeformObject1_other.dfOp", false, fileVersion);
		}
		break;
	case 268:// key "F12"
		//mesh state Load
		if (opEngineif->GetOpObjNum()>0)
		{
			std::cout << "load SecObjState2" << endl;
			fileOp->loadFromFile(objif2, "opDeformObject2_other.dfOp", false, fileVersion);
		}
		break;
	case 's':
		//step debugger run one step
		opEngineif->StepWithBlend();
		break;
	case 'v':
		drawVertex = !drawVertex;
		DSTR << "drawVertex = " << drawVertex << std::endl;
		break;
	case 'z':
		//step debugger
		runByStep = !runByStep;
		break;
	case 'r':
		// ファイルの再読み込み
		Reset();
		break;
	case ' ':
		//change radius collision test
		dp1 = (void *)((opObjIf->GetOpParticle(48)->GetDescAddress()));
		dpAdd = (PHOpParticleDesc*)dp1;
		dpAdd->pMainRadius /= 5;
		dpAdd->pSecRadius /= 5;
		dpAdd->pThrRadiusVec /= 5;
		dp1 = (void *)((opObjIf->GetOpParticle(38)->GetDescAddress()));
		dpAdd = (PHOpParticleDesc*)dp1;
		dpAdd->pMainRadius /= 5;
		dpAdd->pSecRadius /= 5;
		dpAdd->pThrRadiusVec /= 5;
		dp1 = (void *)((opObjIf->GetOpParticle(49)->GetDescAddress()));
		dpAdd = (PHOpParticleDesc*)dp1;
		dpAdd->pMainRadius /= 5;
		dpAdd->pSecRadius /= 5;
		dpAdd->pThrRadiusVec /= 5;
		dp1 = (void *)((opObjIf->GetOpParticle(50)->GetDescAddress()));
		dpAdd = (PHOpParticleDesc*)dp1;
		dpAdd->pMainRadius /= 5;
		dpAdd->pSecRadius /= 5;
		dpAdd->pThrRadiusVec /= 5;
		std::cout << "changed"<<endl;
		break;
	//case 'w':
	//	// カメラ位置の初期化
	//	InitCameraView();
	//	break;
	case 'd':
	{
		// デバック表示
		static bool bDebug = GetSdk()->GetDebugMode();
		if (bDebug)	bDebug = false;
		else		bDebug = true;
		GetSdk()->SetDebugMode(bDebug);
		DSTR << "Debug Mode " << bDebug << std::endl;
		//DSTR << "CameraPosition" << std::endl;
		//DSTR << GetCurrentWin()->GetTrackball()->GetPosition() << std::endl;
	}
		break;
	default:
		break;
	}
}


void PHOpDemo::Display()
{
	//GRRenderIf* render = GetCurrentWin()->GetRender();
	render->SetLighting(false);
#ifdef HAPTIC_DEMO
#ifdef _3DOF
	//draw proxy
	if (1)
	{
		PHOpEngineIf* opEngine = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
		PHOpHapticControllerIf* opHc = (PHOpHapticControllerIf*)opEngine->GetOpHapticController();

		Affinef affpos;
		//affpos.Pos() = HaptcRatePos
		affpos.Pos() = opHc->GetUserPos();

		render->SetMaterial(GRRenderIf::AZURE);
		render->PushModelMatrix();//相対座標で使う
		render->MultModelMatrix(affpos);
		//render->DrawSphere(myHc->proxyRadius*1.01f,8,8,false);
		//render->DrawSphere(myHc->hcElmDtcRadius*1.01f,8,8,false);
		render->DrawSphere(opHc->GetC_ObstacleRadius() * 2 * 1.01f, 8, 8, true);
		//render->DrawSphere(myHc->c_obstRadius * 0.02f * 1.01f, 8, 8, true);

		render->PopModelMatrix();

	}
#endif
#endif

#if 1
	if (DrawHelpInfo)
	{
		render->EnterScreenCoordinate();
		std::ostringstream sstr;
		int Ycor = 1;
		sstr.str("");

		sstr << "Example of OrientedParticle Simulation by Tei";
		render->DrawFont(Vec2f(0, 10), sstr.str());
		sstr.str("");
		sstr << "Hot keys:"  ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Help: 'h' "  ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Adjust alpha(stiffness): '+' and '-' " ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Adjust beta (not been used): '*' and '/' "  ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Velocity damping: '(' to grow and ')' to decay";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Distance Constraint: 'D'";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Op iteration count: '3' to grow '4' to decay";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		++Ycor;
		sstr << "Gravity: 'g' "  ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Switch scene: 'd'"  ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Show particles: 'p' (in physics scene)"  ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Show particles links: 'l' ";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		++Ycor;
		sstr << "Mouse control: 'y' "  ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Mouse Object grab model: 'u'"  ;
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Mouse Fix particle Model: 'f'";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Mouse edit particle link Model: 'e'";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "'Add' or 'Delete' link in edit Model: 'E'";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		++Ycor;
		sstr << "Model File Save: From 'F1' to 'F4";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Model File Load: From 'F5' to 'F8";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Enable Collision: 'c'";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Enable Haptic: 't'";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Please watch springhead output for more information";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		render->LeaveScreenCoordinate();

	}
#endif 
	PHOpEngineIf* opEngineif = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
	PHOpEngine* opEngine = DCAST(PHOpEngine, opEngineif);

	std::ostringstream sstr;
	if (drawVertex)
	for (int obji = 0; obji < (int)opEngineif->GetOpObjNum(); obji++)
	{
		
		PHOpObj& drawObj = *opEngine->opObjs[obji];
		{
			for (int i = 0; i < drawObj.assGrpNum; i++)
			{
				//Vec3f sttP = drawObj.objPArr[i].pOrigCtr;

				//Vec3f sttP = drawObj.objPArr[i].pOritPose.Pos();// / drawObj.floatErrKill ;//kill
				Vec3f &sttP = drawObj.objPArr[i].pNewCtr;

				//DrawPindex
				if (drawObj.objId == 0)
					sstr << "PA " << i;
				else if (drawObj.objId == 1)
					sstr << "PB " << i;
				else sstr << "PX " << i;
				//if(i==0||i==18||i==22)
				render->DrawFont(sttP, sstr.str());
				sstr.str("");
			}
		}
	}

	if (drawCtcP)
	{
		PHOpSpHashColliAgentIf* spIf = GetSdk()->GetScene()->GetPHScene()->GetOpColliAgent();
		PHOpSpHashColliAgent* opsh = DCAST(PHOpSpHashColliAgent, spIf);

		render->SetMaterial(GRRenderIf::CHOCOLATE);

		Affinef affpos;
		for (int j = 0; j<(int)opsh->pPCtcPs.size(); j++)
		{
			affpos.Pos() = opsh->pPCtcPs[j];
			render->SetMaterial(GRRenderIf::CHOCOLATE);
			render->PushModelMatrix();
			render->MultModelMatrix(affpos);
			render->DrawSphere(0.15f, 10, 10, true);

			render->PopModelMatrix();
		}
		render->SetMaterial(GRRenderIf::NAVAJOWHITE);


		for (int j = 0; j<(int)opsh->pPCtcPs2.size(); j++)
		{
			affpos.Pos() = opsh->pPCtcPs2[j];
			render->SetMaterial(GRRenderIf::NAVAJOWHITE);
			render->PushModelMatrix();
			render->MultModelMatrix(affpos);
			render->DrawSphere(0.15f, 10, 10, true);

			render->PopModelMatrix();
		}
	}
	
	//if (drawPs)
	//{
	//	for (int obji = 0; obji < (int)opEngineif->GetOpObjNum(); obji++)
	//	{
	//		PHOpObj& drawObj = *opEngine->opObjs[obji];
	//		{
	//			for (int j = 0; j < drawObj.assPsNum; j++)
	//			{
	//				PHOpParticle &dp = drawObj.objPArr[j];
	//				Vec3f &spCtr = dp.pCurrCtr;
	//				Affinef affpos; affpos.Pos() = spCtr;
	//				float radius = drawObj.objAverRadius;
	//				if (dp.hitedByMouse)
	//					render->SetMaterial(GRRenderIf::YELLOW);
	//				else if (dp.isColliedSphashSolved)
	//					render->SetMaterial(GRRenderIf::RED);
	//				else if (dp.isColliedSphashSolvedReady)
	//					render->SetMaterial(GRRenderIf::DEEPPINK);
	//				else if (dp.isColliedbySphash)
	//					render->SetMaterial(GRRenderIf::GREEN);
	//				else if (dp.isColliedbyColliCube)
	//					;
	//				else render->SetMaterial(GRRenderIf::CADETBLUE);

	//				render->PushModelMatrix();//相対座標で使う
	//				render->MultModelMatrix(affpos);
	//				//render->DrawSphere(radius, 10, 10, false);

	//				float ra = drawObj.objAverRadius, rb = ra, rc = ra / 2;
	//				Spr::TQuaternion<float> elliRotQ; elliRotQ.FromMatrix(dp.pCurrOrint.Inv() * dp.ellipRotMatrix);
	//				//Spr::TQuaternion<float> elliRotQ = dp.pCurrOrint.Inv();//with no ellip orint
	//				//TPose<float> ta; ta= TPose<float>(pos1,elliRotQ); 
	//				DrawEllipsoid drawEll;
	//				drawEll.drawOval(ra * radiusCoe, rb * radiusCoe, rc* radiusCoe, 8, elliRotQ);//dp.pCurrOrint.Inv());

	//				render->PopModelMatrix();
	//			}
	//		}
	//	}
	//}
	for (int obji = 0; obji < (int) opEngineif->GetOpObjNum(); obji++)
	{
		PHOpObj& drawObj = *opEngine->opObjs[obji];
		
		Vec3f cp, lp;
		if (drawlink){
			for (int j = 0; j<drawObj.assGrpNum; j++)
			{
				int count = drawObj.objGArr[j].gNptcl;
				int pInd = drawObj.objGArr[j].getParticleGlbIndex(0);
				cp = drawObj.objPArr[pInd].pCurrCtr;
				for (int k = 1; k<count; k++)
				{
					pInd = drawObj.objGArr[j].getParticleGlbIndex(k);
					lp = drawObj.objPArr[pInd].pCurrCtr;
					render->DrawLine(cp, lp);
				}
			}
		}

		{
			for (int i = 0; i < drawObj.assPsNum; i++)
			{

				PHOpParticle &dp = drawObj.objPArr[i];
				Vec3f pos1 = dp.pCurrCtr;
				dp.hitedByMouse = false;
				float detectSphereRadiu;
#ifdef USE_AVG_RADIUS
				detectSphereRadiu = drawObj.objAverRadius * opEngine->radiusCoe;
#endif
#ifdef USE_DYN_RADIUS
				detectSphereRadiu = dp.pDynamicRadius * radiusCoe;
#endif
				float distance = 100.0;
				Vec3f ab = infiniP - cameraPosition, ac = pos1 - cameraPosition, bc = pos1 - infiniP;
				float e = ac.dot(ab);

				if (e <= 0.0f) distance = ac.dot(ac);
				else {
					float f = ab.dot(ab);
					if (e >= f) distance = bc.dot(bc);
					else distance = (ac.dot(ac) - e*e / f);
				}

				if (mouseHit)
				{
					if ((mousehitP == i) && (mousehitObj == drawObj.objId))
						if (((distance) > ((detectSphereRadiu)*(detectSphereRadiu))))
							mouseHit = false;
				}



				if (useMouseSelect && ((distance) < ((detectSphereRadiu)*(detectSphereRadiu))))
				{
					std::ostringstream sstr;
					dp.hitedByMouse = true;
					mousehitP = i;
					mousehitObj = drawObj.objId;
					mouseHit = true;

					PHOpGroup &pg = drawObj.objGArr[i];
					if (checkPtclInfo)
					{
					int line = 0;

					render->EnterScreenCoordinate();
					sstr << "Pctl-Info.: -Pindex: " << i;
					render->DrawFont(Vec2f(0, 10 * line + 20), sstr.str());
					sstr.str("");
					line++;

					sstr << "-PgroupSize: " << pg.gNptcl;
					render->DrawFont(Vec2f(0, 10 * line + 20), sstr.str());
					sstr.str();
					line++;

					sstr << "-PgroupInd: ";
					for (int vi = 0; vi<pg.gNptcl; vi++)
					sstr << " " << pg.getParticleGlbIndex(vi);
					render->DrawFont(Vec2f(0, 10 * line + 20), sstr.str());
					sstr.str(""); line++;

					sstr << "-PInGroupInd: ";
					for (int vi = 0; vi< (int) dp.pInGrpList.size(); vi++)
					sstr << " " << dp.pInGrpList[vi];
					render->DrawFont(Vec2f(0, 10 * line + 20), sstr.str());
					sstr.str(""); line++;

					sstr << "-PtotalMass: ";
					sstr << " " << dp.pTotalMass;
					render->DrawFont(Vec2f(0, 10 * line + 20), sstr.str());
					sstr.str(""); line++;
					render->LeaveScreenCoordinate();

					}

					Vec3f &p1 = drawObj.objPArr[pg.getParticleGlbIndex(0)].pCurrCtr;
					for (int vi = 1; vi < pg.gNptcl; vi++)
					{
						Vec3f &p2 = drawObj.objPArr[pg.getParticleGlbIndex(vi)].pCurrCtr;
						render->DrawLine(p1, p2);
					}
				}
			}

		}
		
	}

	

	render->SetLighting(true);



	FWApp::OnDisplay();
}
void PHOpDemo::MouseButton(int button, int state, int x, int y)
{

	PHOpEngineIf* opEngineif = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
	PHOpEngine* opEngine = DCAST(PHOpEngine, opEngineif);
	
	if (pgroupEditionModel)
	{
		if (button == 1 && state == 0)
		{

			if (ediFirP)
			{
				if (opEngine->opObjs[ediObj]->objPArr[mousehitP].hitedByMouse)
				{
					ediObj = mousehitObj;
					ediPtcl = mousehitP;
					ediFirP = false;
					DSTR << "editing " << mousehitP << std::endl;
				}
			}
			else
			{
				if (ediObj != mousehitObj)
					return;
				if (opEngine->opObjs[ediObj]->objPArr[mousehitP].hitedByMouse)
				{
					if (addGrpLink)
					{
						//add in group from each other
						opEngine->opObjs[ediObj]->objGArr[ediPtcl].addPInds(mousehitP);
						opEngine->opObjs[ediObj]->objGArr[mousehitP].addPInds(ediPtcl);
						DSTR << "linked " << mousehitP << " to " << ediPtcl << std::endl;

					}
					else
					{
						//add in group from each other
						opEngine->opObjs[ediObj]->objGArr[ediPtcl].delPInds(mousehitP);
						std::vector<int> &tmp = opEngine->opObjs[ediObj]->objPArr[mousehitP].pInGrpList;
						tmp.erase(remove(tmp.begin(), tmp.end(), ediPtcl), tmp.end());
						opEngine->opObjs[ediObj]->objGArr[mousehitP].delPInds(ediPtcl);
						std::vector<int> &tmp1 = opEngine->opObjs[ediObj]->objPArr[ediPtcl].pInGrpList;
						tmp1.erase(remove(tmp1.begin(), tmp1.end(), mousehitP), tmp1.end());

						DSTR << "unlinked " << mousehitP << " to " << ediPtcl << std::endl;
					}
					//rebuild groupcenter
					opEngine->opObjs[ediObj]->buildGroupCenter();

					//reCompute blendWeight
					opEngine->opObjs[ediObj]->BuildBlendWeight();
					ediFirP = true;
				}
			}
		}
		else if (button == 2)
		{
			ediFirP = true;
			DSTR << "Edit cancalled " << mousehitObj << std::endl;
		}
		return;
	}

	if (fixPs)
	{
		if (button == 1 && state == 0)
		{
			if (!opEngine->opObjs[ediObj]->objPArr[mousehitP].hitedByMouse)
				return;

			bool &fix = opEngine->opObjs[mousehitObj]->objPArr[mousehitP].isFixed;
			fix = !fix;
			if (fix)
				DSTR << mousehitP << " is fixed" <<std::endl;
			else DSTR << mousehitP << " is unfixed" << std::endl;
		}
		return;
	}

	//std::cout<<"on state "<<state<<endl;
	if (button == 2)
	{//keep right click usefull
		GetCurrentWin()->GetTrackball()->Enable();
		return;
	}
	if (button == 4 && state == 0)
	{//midBtn is lock the camera on
		Vec3f pos = opEngine->opObjs[mouselockObj]->objPArr[mouselockP].pCurrCtr;
		//Affinef aff = GetCurrentWin()->GetTrackball()->GetAffine();
		GetCurrentWin()->GetTrackball()->SetTarget(pos);
		return;
	}

	if (state == 1)
	{
		GetCurrentWin()->GetTrackball()->Enable();
		mouseLbtn = false;
		opEngine->opObjs[mouselockObj]->objPArr[mouselockP].isFixedbyMouse = false;
		if (useMouseSlcObj)
		{
			for (int pci = 0; pci<opEngine->opObjs[mouselockObj]->assPsNum; pci++)
				opEngine->opObjs[mouselockObj]->objPArr[pci].isFixedbyMouse = false;
		}
		recX = 0;
		recY = 0;
		return;
	}

	if (!mouseHit)
	{
		GetCurrentWin()->GetTrackball()->Enable();
		return;
	}

	GetCurrentWin()->GetTrackball()->Enable(false);
	int test = 0; test = state&DVButtonMask::CONTROL;

	
	if (button == DVButtonMask::LBUTTON)
	{
		recX = (float) x;
		recY = (float) y;
		mouselockP = mousehitP;
		mouselockObj = mousehitObj;
		mouseLbtn = true;
		
	}

}
bool PHOpDemo::OnMouseMove(int button, int x, int y, int zdelta)
{
	GRRenderIf* render = GetCurrentWin()->GetRender();
	PHOpEngineIf* opEngineif = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
	PHOpEngine* opEngine = DCAST(PHOpEngine, opEngineif);
	//MouseMove(x, y);
	if (useMouseSelect)
	{
		int u = 0;
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		render->PushModelMatrix();

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		render->PopModelMatrix();

		winX = (float)x;
		winY = viewport[3] - (float)y;
		glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		cameraPose = GetCurrentWin()->GetTrackball()->GetPose();
		cameraPosition = GetCurrentWin()->GetTrackball()->GetPosition();
		cameraTarget = GetCurrentWin()->GetTrackball()->GetTarget();

		infiniP = Vec3f(posX, posY, posZ);
	
		if (mouseLbtn)
		{
			float dx = recX - x;
			float dy = y - recY;


			float dis = GetCurrentWin()->GetTrackball()->GetDistance();//>GetZoomGain();
			float latMin, latMax, lonMin, lonMax;
			GetCurrentWin()->GetTrackball()->GetLatitudeRange(latMin, latMax);
			GetCurrentWin()->GetTrackball()->GetLongitudeRange(lonMin, lonMax);
			Vec2f winsize = GetCurrentWin()->GetSize();
			float propX, propY;
		
			propX = 0.02f;
			propY = 0.02f;
			dx = dx * propX;
			dy = dy * propY;
		
			cameraPose.Pos() = Vec3f(0, 0, 0);//cameraTarget;

			Vec3f dmove;
			if (!mouseVertmotion)
				dmove = Vec3f(dx, dy, 0);
			else dmove = Vec3f(dx, 0, -dy);
			dmove = cameraPose * dmove;

			float dt1 = 1.0f / (opEngine->opObjs[mouselockObj]->params.timeStep);

		
			if (useMouseSlcObj)
			{
				for (int pci = 0; pci<opEngine->opObjs[mouselockObj]->assPsNum; pci++)
				{
					
					opEngine->opObjs[mouselockObj]->objPArr[pci].isFixedbyMouse = true;
					Vec3f tmpP = opEngine->opObjs[mouselockObj]->objPArr[pci].pCurrCtr;
					opEngine->opObjs[mouselockObj]->objPArr[pci].pCurrCtr -= dmove;
					
				}
			}
			else{
				opEngine->opObjs[mouselockObj]->objPArr[mouselockP].isFixedbyMouse = true;
				Vec3f tmpP = opEngine->opObjs[mouselockObj]->objPArr[mouselockP].pCurrCtr;
				opEngine->opObjs[mouselockObj]->objPArr[mouselockP].pCurrCtr -= dmove;
			
			}
			recX = (float) x;
			recY = (float) y;
			mouseHit = false;
		}



	}
	else{
		GetCurrentWin()->GetTrackball()->Enable();
	}
	return true;
}

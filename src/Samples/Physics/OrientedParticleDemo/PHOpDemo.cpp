/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "PHOpDemo.h"
#include "Physics\PHOpObj.h"
#include "Framework\FWOpObj.h"
#include "Physics\PHOpEngine.h"

#define USE_SPRFILE
#define ESC 27
#define USE_AVG_RADIUS

PHOpDemo::PHOpDemo(){
#ifdef USE_SPRFILE
	fileName = "./files/sceneSample.spr";	// sprファイル
#else
	fileName = "./files/sceneSample.x";		// xファイル
#endif

	gravity = false;
	drawPs = false;
}

void PHOpDemo::Init(int argc, char* argv[]){
	CreateSdk();			// SDKの作成
	UTRef<ImportIf> import = GetSdk()->GetFISdk()->CreateImport();
	GetSdk()->LoadScene(fileName, import);			// ファイルのロード
	GetSdk()->SaveScene("save.spr", import);		// ファイルのセーブテスト
	GRInit(argc, argv);		// ウィンドウマネジャ初期化
	CreateWin();			// ウィンドウを作成
	CreateTimer();			// タイマを作成

	InitCameraView();		// カメラビューの初期化
	GetSdk()->SetDebugMode(false);						// デバックモードの無効化
	GetSdk()->GetScene()->EnableRenderAxis(true);		// 座標軸の表示
	GetSdk()->GetScene()->EnableRenderContact(true);	// 接触領域の表示

	FWOpObj *tmp = GetSdk()->GetScene()->FindObject("fwOpObj")->Cast();
	tmp->CreateOpObj();

	DrawHelpInfo = true;
	checkPtclInfo = true;
	useMouseSelect = false;
	useMouseSelect = false;
	useMouseSlcObj = false;
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
	addGrpLink = true;
	ediFirP = true;
	
	//STEPタイマ作成
	UTTimerIf* SimuTimer = CreateTimer(UTTimerIf::FRAMEWORK);
	int intervaltime = 10;
	//FEMdt = intervaltime / 1000.0; 
	SimuTimer->SetInterval(intervaltime);
	SimuTimer->SetResolution(1);
	opSimuTimerId = SimuTimer->GetID();

	mymeshname = "demoMesh";
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
	FWApp::TimerFunc(id);
	if (id == opSimuTimerId)
	{
		//OpのシミュレーションStepをコール
		PHOpEngineIf* opEngine = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
		opEngine->Step();
		PostRedisplay();
	}
}
void PHOpDemo::Keyboard(int key, int x, int y){

	float alpha, beta;
	int itr = -1;

	//PHOpObj *opObj = GetSdk()->GetScene()->FindObject("phObj")->Cast();
	PHOpEngineIf* opEngineif = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
	PHOpEngine* opEngine = DCAST(PHOpEngine, opEngineif);

	switch (key) {
	case ESC:
	case 'q':
		// アプリケーションの終了
		exit(0);
		break;
	case '3':
		//Adjust iteration count
		for (int obji = 0; obji< (int)opEngine->opObjs.size(); obji++)
		{
			itr = ++opEngine->opObjs[obji]->objitrTime;
		}
		DSTR << "increase Op iteration num =" << itr <<std::endl;
		break;
	case '4':
		//Adjust iteration count
		for (int obji = 0; obji< (int)opEngine->opObjs.size(); obji++)
		{
			itr = --opEngine->opObjs[obji]->objitrTime;
		}
		DSTR << "decrease Op iteration num =" << itr << std::endl;
		break;
	case '+':
		//Adjust Alpha of op
		for (int obji = 0; obji< (int) opEngine->opObjs.size(); obji++)
		{
			alpha = opEngine->opObjs[obji]->params.alpha;
			
			if (alpha<0.9501f)
				alpha += 0.05f;
			opEngine->opObjs[obji]->params.alpha = alpha;
		}
		DSTR << "increase alpha =" << alpha << std::endl;
		break;
	case '-':
		//Adjust Alpha of op
		for (int obji = 0; obji< (int) opEngine->opObjs.size(); obji++)
		{
			alpha = opEngine->opObjs[obji]->params.alpha;
		
			if (alpha>0.0f)
				alpha -= 0.05f;
			opEngine->opObjs[obji]->params.alpha = alpha;
		}
		DSTR << "decrease alpha =" << alpha << std::endl;
		break;
	case '*':
		//Adjust Beta of op
		for (int obji = 0; obji< (int)opEngine->opObjs.size(); obji++)
		{
			beta = opEngine->opObjs[obji]->params.beta;

			if (beta<0.9501f)
				beta += 0.05f;
			opEngine->opObjs[obji]->params.beta = beta;
		}
		DSTR << "increase beta =" << beta << std::endl;
		break;
	case '/':
		//Adjust Beta of op
		for (int obji = 0; obji< (int)opEngine->opObjs.size(); obji++)
		{
			beta = opEngine->opObjs[obji]->params.beta;

			if (beta>0.0f)
				beta -= 0.05f;
			opEngine->opObjs[obji]->params.beta = beta;
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
		else DSTR << " ExitFixParticleModel" << std::endl;
		break;
	case 'g':
		//Enable gravity
		gravity = !gravity;
		opEngineif->SetGravity(gravity);
		break;
	case 'p':
		//Draw particle in physics scene
		drawPs = !drawPs;
		GetSdk()->GetScene()->EnableRenderOp(drawPs);
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
		for (int obji = 0; obji< (int)opEngine->opObjs.size(); obji++)
		{
			opEngine->opObjs[obji]->objUseDistCstr = !opEngine->opObjs[obji]->objUseDistCstr;
		}
		DSTR << "useDistCstr" << opEngine->opObjs[0]->objUseDistCstr << std::endl;;
		break;

	case '(':
		//Adjust velocity damping coefficient
		for (int obji = 0; obji< (int)opEngine->opObjs.size(); obji++)
		{
			opEngine->opObjs[obji]->params.veloDamping += 0.05f;
		}
		DSTR << "params.veloDamping +=0.05f; " << opEngine->opObjs[0]->params.veloDamping << std::endl;;
		break;
	case ')':
		//Adjust velocity damping coefficient
		for (int obji = 0; obji< (int)opEngine->opObjs.size(); obji++)
		{
			opEngine->opObjs[obji]->params.veloDamping -= 0.05f;
		}
		DSTR << "params.veloDamping -=0.05f; " << opEngine->opObjs[0]->params.veloDamping << std::endl;;
		break;
	case 257:// key "F1"
 		//mesh state Save1 
		if (opEngine->opObjs.size()>0)
		{
			std::cout << "Save FirObj1" << endl;
			fileOp.saveToFile(*opEngine->opObjs[0], "opDeformObject1.dfOp", mymeshname);
		}
		break;
	case 258:// key "F2"
		//mesh state Save2
		if (opEngine->opObjs.size()>0)
		{
			std::cout << "Save FirObj2" << endl;
			fileOp.saveToFile(*opEngine->opObjs[0], "opDeformObject2.dfOp", mymeshname);
		}
		break;
	case 259:// key "F3"
		//mesh state Save3
		if (opEngine->opObjs.size()>0)
		{
			std::cout << "Save FirObj3" << endl;
			fileOp.saveToFile(*opEngine->opObjs[0], "opDeformObject3.dfOp", mymeshname);
		}
		break;
	case 260:// key "F4"
		//mesh state Save4 
		if (opEngine->opObjs.size()>0)
		{
			std::cout << "Save FirObj4" << endl;
			fileOp.saveToFile(*opEngine->opObjs[0], "opDeformObject4.dfOp", mymeshname);
		}
		break;
	case 261:// key "F5"
		//mesh state Save5 
		if (opEngine->opObjs.size()>0)
		{
			std::cout << "Load FirObj1" << endl;
			fileOp.loadFromFile(*opEngine->opObjs[0], "opDeformObject1.dfOp", mymeshname);
		}
		break;
	case 262:// key "F6"
		//mesh state Save6 
		if (opEngine->opObjs.size()>0)
		{
			std::cout << "load FirObj2" << endl;
			fileOp.loadFromFile(*opEngine->opObjs[0], "opDeformObject2.dfOp", mymeshname);
		}
		break;
	case 263:// key "F7"
		//mesh state Save7
		if (opEngine->opObjs.size()>0)
		{
			std::cout << "Load FirObj3" << endl;
			fileOp.loadFromFile(*opEngine->opObjs[0], "opDeformObject3.dfOp", mymeshname);
		}
		break;
	case 264:// key "F8"
		//mesh state Save8
		if (opEngine->opObjs.size()>0)
		{
			std::cout << "load FirObj4" << endl;
			fileOp.loadFromFile(*opEngine->opObjs[0], "opDeformObject4.dfOp", mymeshname);
		}
		break;
	case 'r':
		// ファイルの再読み込み
		Reset();
		break;
	case 'w':
		// カメラ位置の初期化
		InitCameraView();
		break;
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
	GRRenderIf* render = GetCurrentWin()->GetRender();
	render->SetLighting(false);


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
		sstr << "Stage Save: From 'F1' to 'F4";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Stage Load: From 'F5' to 'F8";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		sstr << "Please watch springhead output for more information";
		render->DrawFont(Vec2f(0, ++Ycor * 10), sstr.str());
		sstr.str("");
		render->LeaveScreenCoordinate();

	}



	PHOpEngineIf* opEngineif = GetSdk()->GetScene()->GetPHScene()->GetOpEngine()->Cast();
	PHOpEngine* opEngine = DCAST(PHOpEngine, opEngineif);
	for (int obji = 0; obji < (int) opEngine->opObjs.size(); obji++)
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

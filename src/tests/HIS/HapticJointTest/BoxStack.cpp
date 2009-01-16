#include "BoxStack.h"
#include "HapticProcess.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <GL/glut.h>
#include <windows.h>
#include <Physics/PHConstraintEngine.h>
#include <Base/TMatrix.h>
#include <math.h>

BoxStack bstack;
#define FULLSCREAN 0;
void BoxStack::DesignObject(){
	CreateObject();
}
void BoxStack::CreateObject(){

//　形状のdesc ----------------------------------------------------------------------
	CDBoxDesc dBox;
	{
		dBox.boxsize = Vec3f(1.0,0.5,1.5);
		shapeBox = XCAST(GetSdk()->GetPHSdk()->CreateShape(dBox));
		shapeBox->SetName("shapeBox");
	}
	CDSphereDesc dSphere;
	{
		dSphere.radius = 1.2;
		shapeSphere = XCAST(GetSdk()->GetPHSdk()->CreateShape(dSphere));
		shapeSphere->SetName("shapeSphere");
	}
	CDBoxDesc dBoxFloor;
	{
		dBoxFloor.boxsize=Vec3f(100.0f, 1.0f, 100.0f);
		shapeFloor = DCAST(CDBoxIf, GetSdk()->GetPHSdk()->CreateShape(dBoxFloor));
		shapeFloor->SetName("shapeFloor");
	}
//　solidのdesc-----------------------------------------------------------------------------------
	// soFloor用のdesc
	PHSolidDesc floorDesc;
	floorDesc.mass = 1e20f;
	floorDesc.inertia *= 1e30f;
	soFloor = phscene->CreateSolid(floorDesc);		// 剛体をdescに基づいて作成
	{
		soFloor->SetDynamical(false);
		soFloor->SetGravity(false);
		soFloor->SetFramePosition(Vec3d(0.0, -5.0, 0.0));//床の位置を指定する
	}

	// soObject用のdesc
	desc.mass = 0.05;
	desc.inertia = 0.033 * Matrix3d::Unit();

	soBox2[0] = phscene->CreateSolid(desc);
	{
		soBox2[0]->SetDynamical(false);
		soBox2[0]->SetGravity(false);
		soBox2[0]->SetFramePosition(Vec3d(0.0, 0.0, 0.0));//床の位置を指定する
	}
	soBox2[1] = phscene->CreateSolid(desc);
	{
		soBox2[1]->SetDynamical(true);
		soBox2[1]->SetGravity(false);
		soBox2[1]->SetFramePosition(Vec3d(0.0, 0.0, 2.0));//床の位置を指定する
	}
	

//	形状登録-----------------------------------------------------------------------------------
	//　soFloorの形状登録
	soFloor->AddShape(shapeFloor);
	soFloor->SetName("solidFloor");

	soBox2[0]->AddShape(shapeBox);
	soBox2[0]->SetName("solidBox");

	soBox2[1]->AddShape(shapeBox);
	soBox2[1]->SetName("solidBox2");
// ジョイントの接続-----------------------------------------------------------------------------
	double K=3;
	double D1=0.001;
	double D2=0.01;
	double l=1.5;
	PH3ElementBallJointDesc d3Ball2;

	{
		d3Ball2.poseSocket.Pos()	= Vec3f(0.0f, 0.0f, l/2);
		d3Ball2.poseSocket.Ori()	= Quaternionf::Rot(Rad(-180.0f), 'x');
		d3Ball2.posePlug.Pos()	= Vec3f(0.0f, 0.0f, -l/2);
		d3Ball2.posePlug.Ori()	= Quaternionf::Rot(Rad(-180.0f), 'x');
		d3Ball2.spring			= K;
		d3Ball2.damper			= D1;
		d3Ball2.secondDamper		= D2;
		d3Ball2.yieldStress			=0.1;
		d3Ball2.hardnessRate		=1e3;
	}
	jointBox = phscene->CreateJoint( soBox2[0], soBox2[1], d3Ball2);
	phscene->SetContactMode(soBox2[0], soBox2[1], PHSceneDesc::MODE_NONE);
}


void BoxStack::GrabSpring(double dt){
	double K,D;
	double h=dt;
	Vec3d Vg,Xg,Vp,Xp,grabSpringForce;

	//ポインタと剛体間のバネダンパの設定
	K=2.5;
	D=0.1;
	
	if(grabObject.blocal && grabObjectFlag==true){
		phscene->SetContactMode(grabObject.phSolidIf, soPointer, PHSceneDesc::MODE_NONE);

		Xg = grabObject.phSolidIf->GetCenterPosition();//+grabObject.phSolid.GetPose()*grabObject.closestPoint;
		Vg = grabObject.phSolidIf->GetVelocity();

		Xp = hprocess.hpointer.GetCenterPosition();
		Vp = hprocess.hpointer.GetVelocity();

		//バネダンパの計算
		grabSpringForce=(K*(Xp-Xg)+(K*h+D)*(Vp-Vg))/(1+h);

		grabObject.phSolidIf->AddForce(grabSpringForce,Xg);	//対象物体に力を加える
		grabForce=grabSpringForce;							//ポインタに加える力
		
	}else if(grabObject.blocal && !grabObjectFlag==true){
		grabForce=Vec3d(0.0, 0.0, 0.0);
	}

}


BoxStack::BoxStack(){
	// xfileのパスの指定
	filePath			= "./xFiles/";
	fileName			= "sceneHapticJointTest.x";
	hPointerName = "soHapticPointer";

	// 変数の初期化
	bsync=false;
	calcPhys=true;
	dt = 0.02;
	gravity =  Vec3d(0, -9.8 , 0);
	nIter = 15;
	bGravity = true;
	phscene = NULL;
	render = NULL;
	range = 0.5;
	bDebug = true;
	neighborObjects.clear();
	bStep = true;
	countKey = 0;
	grabObjectFlag = false;
	grabForceRate = 1;
	
}
namespace Spr{
void FASTCALL ContFindCommonPointSaveParam(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist);
}

void BoxStack::Init(int argc, char* argv[]){
	//glutGameModeString("width=800 height=600 bpp~24 hertz>=100");
	//glutEnterGameMode();
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();															// SDKの作成
	// xfileのロード
	stringstream imputFileName;
	imputFileName << filePath << fileName;
	GetSdk()->LoadScene(imputFileName.str().c_str());				// シーンをxfileからロード
	phscene = GetSdk()->GetScene()->GetPHScene();
	phscene->SetStateMode(true);
	DesignObject();

	states = ObjectStatesIf::Create();
	states2 = ObjectStatesIf::Create();

	

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(nIter);

	SetHapticPointer();

	FWWinDesc windowDesc;
#if FULLSCREAN
	windowDesc.height=1024;
	windowDesc.width=768;
	windowDesc.fullscreen=true;
#endif
	windowDesc.title = "Test";
	window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();

	InitCameraView();
}

void BoxStack::SetHapticPointer(){
	soPointer = DCAST(PHSolidIf, phscene->FindObject(hPointerName));
	soPointer->SetDynamical(false);
	soPointer->SetFrozen(true);
	phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
}

void BoxStack::InitCameraView(){
	std::istringstream issView(
		"((0.999833 0.00643529 -0.0170968 -0.114308)"
		"(-7.16819e-010 0.935897 0.352274 2.35527)"
		"(0.0182679 -0.352215 0.935741 6.25627)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void BoxStack::Start(){
	instance = this;
	if (!NWin()){
		CreateWin();
		wins.back()->SetScene(GetSdk()->GetScene());
	}
	glutIdleFunc(FWAppGLUT::GlutIdleFunc);
	glutMainLoop();
}

void BoxStack::Idle(){
	PhysicsStep();
}

void BoxStack::PhysicsStep(){
	if (bsync) return;
	if (calcPhys){
		UpdateHapticPointer();
		for(unsigned i = 0; i < neighborObjects.size(); i++){
			if(!neighborObjects[i].blocal) continue;
			neighborObjects[i].lastvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
			neighborObjects[i].lastvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		}
		naga=true;
		if(bStep) phscene->Step();
		else if (bOneStep){
			phscene->Step();
			bOneStep = false;
		}

		for(unsigned i = 0; i < neighborObjects.size(); i++){
			if(!neighborObjects[i].blocal) continue;
			SpatialVector curvel;
			curvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
			curvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
			neighborObjects[i].curb = (curvel - neighborObjects[i].lastvel) / dt;
		}
		calcPhys = false;
		naga=false;
		FindNearestObject();	// 近傍物体の取得
		PredictSimulation();
		glutPostRedisplay();
		
	}
	if (hapticcount < dt/hprocess.dt) return;
	hapticcount -= dt/hprocess.dt;
	bsync = true;
	calcPhys = true;

	static DWORD last;
	//if (phscene->GetCount() % 10==0){
	//	DWORD t = timeGetTime();
	//	DSTR << phscene->GetCount() << " dt:" << t-last << std::endl;
	//	last = t;
	//}
}

void BoxStack::Display(){
	// 描画の設定
	GetSdk()->SetDebugMode(bDebug);
	render = window->render->Cast();
	DCAST(GRDebugRenderIf, render)->EnableRenderForce(bDebug, 0.7);

//	// 描画モードの設定
	if(GetSdk() && GetSdk()->GetScene())
		GetSdk()->GetScene()->Draw(render, bDebug);
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

	if (curScene) curScene->Draw(curRender, bDebug);

	//	光源の追加
	GRLightDesc ld;
	ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
	ld.specular = Vec4f(1,1,1,1) * 0.8f;
	ld.ambient = Vec4f(1,1,1,1) * 0.4f;
	ld.position = Vec4f(1,1,1,0);
	render->PushLight(ld);
	if(bDebug){
	DisplayContactPlane();
	DisplayLineToNearestPoint();			// 力覚ポインタと剛体の近傍点の間をつなぐ
	//	DrawHapticSolids();
	}
	render->PopLight();	//	光源の削除

	curRender->EndScene();
	glFinish();
	glutSwapBuffers();
}

void BoxStack::UpdateHapticPointer(){
	// cameraInfo.view.Rot()をかけて力覚ポインタの操作をカメラを回転にあわせる
	soPointer->SetFramePosition(phpointer.GetFramePosition());//cameraInfo.view.Rot() * phpointer.GetFramePosition());				
	soPointer->SetOrientation(phpointer.GetOrientation());					
	soPointer->SetVelocity(phpointer.GetVelocity());
	soPointer->SetAngularVelocity(phpointer.GetAngularVelocity());	
	soPointer->SetDynamical(false);
};

void BoxStack::FindNearestObject(){
	// GJKを使って近傍物体と近傍物体の最近点を取得
	// これをすべてのshapeをもつ剛体についてやる

	// シーンで新たに剛体が生成されたらローカルでシミュレーションしているかどうかの情報を加えsceneSolidsに格納する
	// sceneSolidsは力覚ポインタも持っている
	sceneSolids.clear();
	PHSolidIf** solids = phscene->GetSolids();
	for(int i = 0; i < phscene->NSolids(); i++){
		sceneSolids.resize(sceneSolids.size() + 1);
		sceneSolids.back().phSolidIf = solids[i];
		sceneSolids.back().bneighbor = false;
		sceneSolids.back().blocal = false;
	}

	// sceneSolidsで新しく増えた分をneighborObjectsに追加する
	//neighborObjectsをいちいちclearしてると，昔のneighborObjectsを保存する必要があるので 
	//今はneighborObjectsにsceneのすべてのsolidを格納している．
	//neighborObjectsは力覚ポインタを持つがblocalを常にfalseにしておく必要がある．
	for(unsigned i = (int)neighborObjects.size(); i < sceneSolids.size(); i++){
		neighborObjects.resize(i + 1);
		neighborObjects.back().phSolidIf = sceneSolids[i].phSolidIf;
		neighborObjects.back().bneighbor = false;
		neighborObjects.back().blocal = false;
	}

#if 1		
	// ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
	// SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外		
	//1. BBoxレベルの衝突判定
	size_t N = neighborObjects.size();
	Vec3f dir(0,0,1);
	Edges edges;
	edges.resize(2 * N);
	Edges::iterator eit = edges.begin();
	for(int i = 0; i < N; ++i){
		// ローカル判定をすべてfalseにする
		neighborObjects[i].bneighbor = false;
		DCAST(PHSolid, neighborObjects[i].phSolidIf)->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		Vec3d dPos = neighborObjects[i].phSolidIf->GetDeltaPosition();
		float dLen = (float) (dPos * dir);
		if (dLen < 0){
			eit[0].edge += dLen;
		}else{
			eit[1].edge += dLen;
		}
		eit[0].index = i; eit[0].bMin = true;
		eit[1].index = i; eit[1].bMin = false;
		eit += 2;
	}
	sort(edges.begin(), edges.end());
	//端から見ていって，接触の可能性があるノードの判定をする．
	typedef set<int> SolidSet;
	SolidSet cur;							//	現在のSolidのセット
	bool found = false;

	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	初端だったら，リスト内の物体と判定
			for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->index;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				// 近傍物体として決定
				if(neighborObjects[f1].phSolidIf == soPointer){
					neighborObjects[f2].bneighbor = true;
				}else if(neighborObjects[f2].phSolidIf == soPointer){
					neighborObjects[f1].bneighbor = true;
				}
			}
			cur.insert(it->index);
		}else{
			cur.erase(it->index);			//	終端なので削除．
		}
	}

	for(size_t i = 0; i < neighborObjects.size(); i++){
		if(neighborObjects[i].bneighbor) continue;
		neighborObjects[i].bfirstlocal = false;			//近傍物体でないのでfalseにする
		neighborObjects[i].blocal = false;
	}
#else
	// すべての物体についてGJKを行う
	for(int i = 0; i < neighborObjects.size(); i++){
		neighborObjects[i].bneighbor = true;
		if(neighborObjects[i].phSolidIf == soPointer) neighborObjects[i].bneighbor = false;
	}
#endif

	// GJKで近傍点を求め，力覚ポインタ最近傍の物体を決定する
	// 最近傍物体だったらblocalをtrueにし，phSolidにphSolidIfをコピーする
	// blocalがすでにtrueだったらコピー済みなので近傍点だけコピーする
	for(unsigned i = 0; i < (int)neighborObjects.size(); i++){
		if(!neighborObjects[i].bneighbor) continue;															// 近傍でなければ次へ

		CDConvex* a = DCAST(CDConvex, neighborObjects[i].phSolidIf->GetShape(0));		// 剛体が持つ凸形状
		CDConvex* b = DCAST(CDConvex, soPointer->GetShape(0));									// 力覚ポインタの凸形状
		Posed a2w, b2w;																								// 剛体のワールド座標
/*		if(neighborObjects[i].blocal) 	a2w = neighborObjects[i].phSolid.GetPose();				// blocalがtrueなら最新の情報でやる
		else */								a2w = neighborObjects[i].phSolidIf->GetPose();
		b2w = soPointer->GetPose();																			// 力覚ポインタのワールド座標
		Vec3d pa ,pb;																									// pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）
		pa = pb = Vec3d(0.0, 0.0, 0.0);
		FindClosestPoints(a, b, a2w, b2w, pa, pb);															// GJKで近傍点の算出
		Vec3d wa = a2w * pa;																						// 剛体近傍点のワールド座標
		Vec3d wb = b2w * pb;																						// 力覚ポインタ近傍点のワールド座標
		Vec3d a2b = wb - wa;																						// 剛体から力覚ポインタへのベクトル
		Vec3d normal = a2b.unit();
		if(a2b.norm() < range){																					// 近傍点までの長さから近傍物体を絞る
			if(a2b.norm() < 0.01){																				// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める		
				pa = pb = Vec3d(0.0, 0.0, 0.0);
				Vec3d dir = -1.0 * neighborObjects[i].face_normal;
				if(dir == Vec3d(0.0, 0.0, 0.0) ){
					dir = -(soPointer->GetCenterPosition() - wa);
				}
				double dist = 0.0;
				int cp = ContFindCommonPoint(a, b, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
				if(cp != 1){
					ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
					DSTR << "ContFindCommonPoint do not find contact point" << endl;
				}
			}
			
			grabObject.phSolid = *DCAST(PHSolid, neighborObjects[i].phSolidIf);	

			if(!neighborObjects[i].blocal){																			// 初めて最近傍物体になった時
				neighborObjects[i].bfirstlocal = true;
				neighborObjects[i].phSolid = *DCAST(PHSolid, neighborObjects[i].phSolidIf);		// シーンが持つ剛体の中身を力覚プロセスで使う剛体（実体）としてコピーする
#ifdef _DEBUG
				if (neighborObjects[i].face_normal * normal < 0.8){
					DSTR << "Too big change on normal" << normal << std::endl;
				}
#endif
				neighborObjects[i].face_normal = normal;														// 初めて最近傍物体になったので，前回の法線は使わない．										
			}
			neighborObjects[i].blocal = true;																		// 最近傍物体なのでblocalをtrueにする
			neighborObjects[i].closestPoint = pa;																	// 剛体近傍点のローカル座標
			neighborObjects[i].pointerPoint = pb;																// 力覚ポインタ近傍点のローカル座標
			neighborObjects[i].last_face_normal = neighborObjects[i].face_normal;					// 前回の法線（法線の補間に使う）
			neighborObjects[i].face_normal = normal;															// 剛体から力覚ポインタへの法線



			if(grabObjectFlag==false){
				grabObject.phSolidIf=neighborObjects[i].phSolidIf;	
				grabObject.blocal = true ;
				grabObject.closestPoint = pa;																	// 剛体近傍点のローカル座標
				grabObject.pointerPoint = pb;																// 力覚ポインタ近傍点のローカル座標
				grabObject.last_face_normal = neighborObjects[i].face_normal;					// 前回の法線（法線の補間に使う）
				grabObject.face_normal = normal;												// 剛体から力覚ポインタへの法線
			}
			
		}else{
			neighborObjects[i].blocal = false;																		// 最近傍物体ではないのでblocalをfalseにする
			neighborObjects[i].bfirstlocal = false;
		}
	}
}

#define DIVIDE_STEP

void BoxStack::PredictSimulation(){
	// neighborObjetsのblocalがtrueの物体に対して単位力を加え，接触しているすべての物体について，運動係数を計算する
#ifdef DIVIDE_STEP

	states2->SaveState(phscene);			// 予測シミュレーションのために現在の剛体の状態を保存する
	//	LCPの直前までシミュレーションしてその状態を保存
	phscene->ClearForce();
	phscene->GenerateForce();
	phscene->IntegratePart1();
#endif
	states->SaveState(phscene);			// 予測シミュレーションのために現在の剛体の状態を保存する
	
	for(unsigned i = 0; i < neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		
		// 現在の速度を保存
		SpatialVector currentvel, nextvel; 
		currentvel.v() = neighborObjects[i].phSolidIf->GetVelocity();											// 現在の速度
		currentvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();									// 現在の角速度									
		Vec3d cPoint = neighborObjects[i].phSolidIf->GetPose() * neighborObjects[i].closestPoint;	// 力を加える点
		const float minTestForce = 0.5;
		if(neighborObjects[i].test_force_norm < minTestForce){
			neighborObjects[i].test_force_norm = minTestForce;		// テスト力が0なら1にする 
		}

		// 拘束座標系を作るための準備
		Vec3d rpjabs, vpjabs;
		rpjabs = cPoint - soPointer->GetCenterPosition();																							//力覚ポインタの中心から接触点までのベクトル
		vpjabs = soPointer->GetVelocity() + soPointer->GetAngularVelocity() % rpjabs;													//接触点での速度
		Vec3d rjabs, vjabs;
		rjabs = cPoint - neighborObjects[i].phSolidIf->GetCenterPosition();																	//剛体の中心から接触点までのベクトル
		vjabs = neighborObjects[i].phSolidIf->GetVelocity() + neighborObjects[i].phSolidIf->GetAngularVelocity() % rjabs;	//接触点での速度

		//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
		Vec3d n, t[2], vjrel, vjrelproj;
		n = -neighborObjects[i].face_normal;
		vjrel = vjabs - vpjabs;										// 相対速度
		vjrelproj = vjrel - (n * vjrel) * n;						// 相対速度ベクトルを法線に直交する平面に射影したベクトル
		double vjrelproj_norm = vjrelproj.norm();
		if(vjrelproj_norm < 1.0e-10){							// 射影ベクトルのノルムが小さいとき
			t[0] = n % Vec3d(1.0, 0.0, 0.0);					// t[0]を法線とVec3d(1.0, 0.0, 0.0)の外積とする
			if(t[0].norm() < 1.0e-10)								// それでもノルムが小さかったら
				t[0] = n % Vec3d(0.0, 1.0, 0.0);				// t[0]を法線とVec3d(0.0, 1.0, 0.0)の外積とする
			t[0].unitize();												// t[0]を単位ベクトルにする
		}
		else{
			t[0] = vjrelproj / vjrelproj_norm;					// ノルムが小さくなかったら，射影ベクトルのまま
		}
		t[1] = n % t[0];												// t[1]は法線とt[0]の外積できまる

		// 何も力を加えないでシミュレーションを1ステップ進める
#ifdef DIVIDE_STEP
		phscene->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		neighborObjects[i].lastb = neighborObjects[i].b;
		neighborObjects[i].b = (nextvel - currentvel) / dt;

		TMatrixRow<6, 3, double> u;
		TMatrixRow<3, 3, double> force;
		// 法線方向に力を加える
		states->LoadState(phscene);
		force.col(0) = neighborObjects[i].test_force_norm * n;
		neighborObjects[i].phSolidIf->AddForce(force.col(0), cPoint);
#ifdef DIVIDE_STEP
		phscene->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		u.col(0) = (nextvel - currentvel) /dt - neighborObjects[i].b;

		// n + t[0]方向に力を加える
		states->LoadState(phscene);
		force.col(1) = neighborObjects[i].test_force_norm * (n + t[0]);
		neighborObjects[i].phSolidIf->AddForce(force.col(1), cPoint);
		phscene->IntegratePart2();
		nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		u.col(1) = (nextvel - currentvel) /dt - neighborObjects[i].b;

		// n+t[1]方向力を加える
		states->LoadState(phscene);
		force.col(2) = neighborObjects[i].test_force_norm * (n + t[1]);
		neighborObjects[i].phSolidIf->AddForce(force.col(2), cPoint);
#ifdef DIVIDE_STEP
		phscene->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		u.col(2) = (nextvel - currentvel) /dt - neighborObjects[i].b;
		
		neighborObjects[i].A = u  * force.inv();				// 運動係数Aの計算
		states->LoadState(phscene);								// 元のstateに戻しシミュレーションを進める
	}
#ifdef DIVIDE_STEP
		states2->LoadState(phscene);								// 元のstateに戻しシミュレーションを進める
#endif

}

void BoxStack::DisplayContactPlane(){
	for(unsigned int i = 0; i <  neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		Vec3d pPoint = soPointer->GetPose() * neighborObjects[i].pointerPoint;
		Vec3d cPoint = neighborObjects[i].phSolidIf->GetPose() * neighborObjects[i].closestPoint;
		Vec3d normal = neighborObjects[i].face_normal;
		Vec3d v1(0,1,0);
		v1 +=  Vec3d(0, 0, 0.5) - Vec3d(0, 0, 0.5)*normal*normal;
		v1 -= v1*normal * normal;
		v1.unitize();
		Vec3d v2 = normal ^ v1;
		Vec4f moon(1.0, 1.0, 0.8, 0.3);
		render->SetMaterial( GRMaterialDesc(moon) );
		render->PushModelMatrix();
		Vec3d offset = 0.02 * normal;
		render->SetLighting( false );
		render->SetAlphaTest(true);
		render->SetAlphaMode(render->BF_SRCALPHA, render->BF_ONE);
		cPoint += offset/2;
		glBegin(GL_QUADS);
			// 接触面底面	
			glVertex3d(cPoint[0] + v1[0] + v2[0], cPoint[1] + v1[1] + v2[1], cPoint[2] + v1[2] + v2[2]);
			glVertex3d(cPoint[0] - v1[0] + v2[0], cPoint[1] - v1[1] + v2[1], cPoint[2] - v1[2] + v2[2]);
			glVertex3d(cPoint[0] - v1[0] - v2[0], cPoint[1] - v1[1] - v2[1], cPoint[2] - v1[2] - v2[2]);
			glVertex3d(cPoint[0] + v1[0] - v2[0], cPoint[1] + v1[1] - v2[1], cPoint[2] + v1[2] - v2[2]);
			// 側面1
			glVertex3d(cPoint[0] + v1[0] + v2[0] + offset[0], 
							cPoint[1] + v1[1] + v2[1] + offset[1], 
							cPoint[2] + v1[2] + v2[2] + offset[2]);
			glVertex3d(cPoint[0] + v1[0] + v2[0], 
							cPoint[1] + v1[1] + v2[1], 
							cPoint[2] + v1[2] + v2[2]);
			glVertex3d(cPoint[0] - v1[0] + v2[0], 
							cPoint[1] - v1[1] + v2[1], 
							cPoint[2] - v1[2] + v2[2]);
			glVertex3d(cPoint[0] - v1[0] + v2[0] + offset[0], 
							cPoint[1] - v1[1] + v2[1] + offset[1], 
							cPoint[2] - v1[2] + v2[2] + offset[2]);
			// 側面2
			glVertex3d(cPoint[0] - v1[0] + v2[0] + offset[0], 
							cPoint[1] - v1[1] + v2[1] + offset[1], 
							cPoint[2] - v1[2] + v2[2] + offset[2]);
			glVertex3d(cPoint[0] - v1[0] + v2[0], 
							cPoint[1] - v1[1] + v2[1], 
							cPoint[2] - v1[2] + v2[2]);
			glVertex3d(cPoint[0] - v1[0] - v2[0], 
							cPoint[1] - v1[1] - v2[1], 
							cPoint[2] - v1[2] - v2[2]);
			glVertex3d(cPoint[0] - v1[0] - v2[0] + offset[0], 
							cPoint[1] - v1[1] - v2[1] + offset[1], 
							cPoint[2] - v1[2] - v2[2] + offset[2]);
			// 側面3
			glVertex3d(cPoint[0] - v1[0] - v2[0] + offset[0], 
							cPoint[1] - v1[1] - v2[1] + offset[1], 
							cPoint[2] - v1[2] - v2[2] + offset[2]);
			glVertex3d(cPoint[0] - v1[0] - v2[0], 
							cPoint[1] - v1[1] - v2[1], 
							cPoint[2] - v1[2] - v2[2]);
			glVertex3d(cPoint[0] + v1[0] - v2[0], 
							cPoint[1] + v1[1] - v2[1], 
							cPoint[2] + v1[2] - v2[2]);
			glVertex3d(cPoint[0] + v1[0] - v2[0] + offset[0], 
							cPoint[1] + v1[1] - v2[1] + offset[1], 
							cPoint[2] + v1[2] - v2[2] + offset[2]);
			// 側面4
			glVertex3d(cPoint[0] + v1[0] - v2[0] + offset[0], 
							cPoint[1] + v1[1] - v2[1] + offset[1], 
							cPoint[2] + v1[2] - v2[2] + offset[2]);
			glVertex3d(cPoint[0] + v1[0] - v2[0], 
							cPoint[1] + v1[1] - v2[1], 
							cPoint[2] + v1[2] - v2[2]);
			glVertex3d(cPoint[0] + v1[0] + v2[0], 
							cPoint[1] + v1[1] + v2[1],
							cPoint[2] + v1[2] + v2[2]);
			glVertex3d(cPoint[0] + v1[0] + v2[0] + offset[0], 
							cPoint[1] + v1[1] + v2[1] + offset[1], 
							cPoint[2] + v1[2] + v2[2] + offset[2]);
			// 接触面上面
			glVertex3d(cPoint[0] - v1[0] + v2[0] + offset[0], 
							cPoint[1] - v1[1] + v2[1] + offset[1], 
							cPoint[2] - v1[2] + v2[2] + offset[2]);
			glVertex3d(cPoint[0] + v1[0] + v2[0] + offset[0], 
							cPoint[1] + v1[1] + v2[1] + offset[1], 
							cPoint[2] + v1[2] + v2[2] + offset[2]);
			glVertex3d(cPoint[0] + v1[0] - v2[0] + offset[0], 
							cPoint[1] + v1[1] - v2[1] + offset[1], 
							cPoint[2] + v1[2] - v2[2] + offset[2]);
			glVertex3d(cPoint[0] - v1[0] - v2[0] + offset[0], 
							cPoint[1] - v1[1] - v2[1] + offset[1], 
							cPoint[2] - v1[2] - v2[2] + offset[2]);
		glEnd();
		render->SetLighting( true);
		render->SetAlphaTest(false);
		render->PopModelMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}

void BoxStack::DisplayLineToNearestPoint(){
	GLfloat moon[]={0.8,0.8,0.8};
	for(unsigned int i = 0; i <  neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		Vec3d pPoint = soPointer->GetPose() * neighborObjects[i].pointerPoint;
		Vec3d cPoint = neighborObjects[i].phSolidIf->GetPose() * neighborObjects[i].closestPoint;
		Vec3d normal = neighborObjects[i].face_normal;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, moon);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_LINES);
		glVertex3f(pPoint.X() + normal[0], pPoint.Y() + normal[1], pPoint.Z() + normal[2]);
		glVertex3f(cPoint.X(), cPoint.Y(), cPoint.Z());
		glEnd();
		glEnable(GL_DEPTH_TEST);
	}
}

void BoxStack::DrawHapticSolids(){
	GLfloat purple[] = {1.0, 0.0, 1.0, 0.0};
	GRDebugRenderIf* render = GetCurrentWin()->GetRender()->Cast();
	render->SetMaterialSample(GRDebugRenderIf::GRAY);
	for(unsigned int i = 0; i < hapticsolids.size(); i++){
		PHSolid* solid = &hapticsolids[i];		
		PHSolidIf* solidIf = solid->Cast(); 
		render->DrawSolid(solidIf);
	}
};


void BoxStack::Keyboard(unsigned char key){
	states->ReleaseState(phscene);
	states2->ReleaseState(phscene);
	switch (key) {
		case 'w':
			InitCameraView();
			DSTR << "InitCameraView" << endl;
			break;
		case 'd':
			{
				if(bDebug){
					bDebug = false;
//					DCAST(GRDebugRenderIf, render)->EnableRenderAxis(bDebug, 0.1);
					DCAST(GRDebugRenderIf, render)->EnableRenderForce(bDebug, 100);
					DCAST(GRDebugRenderIf, render)->EnableRenderContact(bDebug);
					DSTR << "Debug Mode OFF" << endl;
				}
				else if(!bDebug) {
					bDebug = true;
//					DCAST(GRDebugRenderIf, render)->EnableRenderAxis(bDebug, 0.1);
					DCAST(GRDebugRenderIf, render)->EnableRenderForce(bDebug, 0.7);
					DCAST(GRDebugRenderIf, render)->EnableRenderContact(bDebug);
					DSTR << "Debug Mode ON" << endl;
				}
			}
			break;
		case 'g':
			if(bGravity){
				bGravity = false;
				Vec3d zeroG = Vec3d(0.0, 0.0, 0.0);
				phscene->SetGravity(zeroG);
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

		case ' ':
			if(grabObjectFlag==true){
				grabObjectFlag=false;
			}else{
				grabObjectFlag=true;
			}
			break;
		case '/':
			grabForceRate+=0.01;
			cout<<"grabForceRate"<<grabForceRate<<endl;
			break;
		case '.':
			grabForceRate-=0.01;
			cout<<"grabForceRate"<<grabForceRate<<endl;
			break;


		default:
			break;
	}
}
#include "PhysicsProcess.h"
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

PhysicsProcess pprocess;

PhysicsProcess::PhysicsProcess(){
	bsync=false;
	calcPhys=true;
	dt = 0.02;//0.05;
	gravity =  Vec3d(0, -9.8f , 0);
	nIter = 15;
	bGravity = true;
	range = 0.7;
	bDebug = false;
	expandedObjects.clear();
	bStep = true;
}

namespace Spr{
void FASTCALL ContFindCommonPointSaveParam(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist);
}

void PhysicsProcess::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();															// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(nIter);

	states = ObjectStatesIf::Create();
	states2 = ObjectStatesIf::Create();

	DesignObject();																// 剛体を作成

	FWWinDesc windowDesc;
	windowDesc.title = "HapticConnect1.2";
	window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();

	InitCameraView();
}

void PhysicsProcess::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void PhysicsProcess::DesignObject(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	phscene->SetStateMode(true);

	PHSolidDesc desc;		// 剛体のディスクリプタ
	CDBoxDesc bd;			// 直方体形状のディスクリプタ
	CDSphereDesc sd;		// 球体のディスクリプタ

	// 床の作成
	{
		// soFloor用のdesc
		desc.mass = 1e20f;
		desc.inertia *= 1e30f;
		PHSolidIf* soFloor = phscene->CreateSolid(desc);		
		soFloor->SetDynamical(false);
		soFloor->SetGravity(false);

		bd.boxsize = Vec3f(60, 40, 40);
		CDBoxIf* shapeFloor = XCAST(GetSdk()->GetPHSdk()->CreateShape(bd));
		shapeFloor->SetName("shapeFloor");
		soFloor->AddShape(shapeFloor);
		soFloor->SetFramePosition(Vec3d(0, -20.7 ,0));
		soFloor->GetShape(0)->SetVibration(-100, 150, 150);
		soFloor->SetName("solidFloor");
	}
	// 力覚ポインタの作成
	{
		soPointer = phscene->CreateSolid(desc);
		sd.radius = 0.5;//1.0;
		CDSphereIf* shapePointer = DCAST(CDSphereIf,  GetSdk()->GetPHSdk()->CreateShape(sd));
		soPointer->AddShape(shapePointer);
		soPointer->SetFramePosition(Vec3d(0, 3.0, 0));  
		soPointer->SetDynamical(false);
		// 力覚ポインタをspringhead2の接触計算から外す
		// 剛体が増えるたびにContactModeをNONEにしなけらばいけない(増えた剛体と接触計算をしてしまうため)
		phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
	}
}

void PhysicsProcess::Idle(){
	PhysicsStep();
}

void PhysicsProcess::Start(){
	instance = this;
	if (!NWin()){
		CreateWin();
		wins.back()->SetScene(GetSdk()->GetScene());
	}
	glutIdleFunc(FWAppGLUT::GlutIdleFunc);
	glutMainLoop();
}

void PhysicsProcess::PhysicsStep(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	if (bsync) return;
	if (calcPhys){
		UpdateHapticPointer();
		vector<SpatialVector> lastvel;
		for(unsigned int i = 0; i < expandedObjects.size(); i++){
			if(!expandedObjects[i].flag.blocal) continue;
			lastvel.resize(i + 1);
			lastvel.back().v() = expandedObjects[i].phSolidIf->GetVelocity();
			lastvel.back().w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		}
		if(bStep) phscene->Step();
		else if (bOneStep){
			phscene->Step();
			bOneStep = false;
		}

		for(unsigned i = 0; i < expandedObjects.size(); i++){
			if(!expandedObjects[i].flag.blocal) continue;
			SpatialVector curvel;
			curvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
			curvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
			expandedObjects[i].syncInfo.motionCoeff.curb = (curvel - lastvel[i]) / dt;
		}
		ExpandSolidInfo();
		FindNearestObject();	// 近傍物体の取得
		PredictSimulation();
		glutPostRedisplay();
		calcPhys = false;
	}
	if (hapticcount < dt/hprocess.dt) return;
	hapticcount -= dt/hprocess.dt;
	bsync = true;
	calcPhys = true;

	//static DWORD last;
	//if (phscene->GetCount() % 10==0){
	//	DWORD t = timeGetTime();
	//	DSTR << phscene->GetCount() << " dt:" << t-last << std::endl;
	//	last = t;
	//}
}

void PhysicsProcess::Display(){
	// 描画の設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = window->render->Cast();

	// 描画モードの設定
	render->SetRenderMode(true, false);
//	render->EnableRenderAxis(bDebug);
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
	if(bDebug){
		DisplayContactPlane();
		DisplayLineToNearestPoint();			// 力覚ポインタと剛体の近傍点の間をつなぐ
	}
	render->PopLight();	//	光源の削除

	curRender->EndScene();
	glutSwapBuffers();
}

void PhysicsProcess::UpdateHapticPointer(){
	soPointer->SetFramePosition(phpointer.GetFramePosition());				
//	soPointer->SetFramePosition(cameraInfo.view.Rot() * phpointer.GetFramePosition());		// (未実装)cameraInfo.view.Rot()をかけて力覚ポインタの操作をカメラを回転にあわせる(*力覚も考えないといけん)			
	soPointer->SetOrientation(phpointer.GetOrientation());					
	soPointer->SetVelocity(phpointer.GetVelocity());
	soPointer->SetAngularVelocity(phpointer.GetAngularVelocity());	
	soPointer->SetDynamical(false);
}

void PhysicsProcess::ExpandSolidInfo(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene(); 
	PHSolidIf** solids = phscene->GetSolids();
	expandedObjects.clear();
	for(unsigned i = (int)expandedObjects.size(); i < phscene->NSolids(); i++){
		expandedObjects.resize(i + 1);
		expandedObjects.back().phSolidIf = solids[i];
	}
}

void PhysicsProcess::FindNearestObject(){	
	// GJKを使って近傍物体と近傍物体の最近点を取得
	// これをすべてのshapeをもつ剛体についてやる
	// AABBで力覚ポインタ近傍の物体を絞る
	// ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
	// SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外		
	//1. BBoxレベルの衝突判定
	size_t N = expandedObjects.size();
	Vec3f dir(0,0,1);
	Edges edges;
	edges.resize(2 * N);
	Edges::iterator eit = edges.begin();
	for(int i = 0; i < N; ++i){
		// ローカル判定をすべてfalseにする
		expandedObjects[i].flag.bneighbor = false;
		DCAST(PHSolid, expandedObjects[i].phSolidIf)->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		Vec3d dPos = expandedObjects[i].phSolidIf->GetDeltaPosition();
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
				if(expandedObjects[f1].phSolidIf == soPointer){
					expandedObjects[f2].flag.bneighbor = true;
				}else if(expandedObjects[f2].phSolidIf == soPointer){
					expandedObjects[f1].flag.bneighbor = true;
				}
			}
			cur.insert(it->index);
		}else{
			cur.erase(it->index);			//	終端なので削除．
		}
	}

	for(size_t i = 0; i < expandedObjects.size(); i++){
		if(expandedObjects[i].flag.bneighbor) continue;
		expandedObjects[i].flag.bfirstlocal = false;			//近傍物体でないのでfalseにする
		expandedObjects[i].flag.blocal = false;
	}

	// GJKで近傍点を求め，力覚ポインタ最近傍の物体を決定する
	// 最近傍物体だったらblocalをtrueにし，phSolidにphSolidIfをコピーする
	// blocalがすでにtrueだったらコピー済みなので近傍点だけコピーする
	for(unsigned i = 0; i < (int)expandedObjects.size(); i++){
		if(!expandedObjects[i].flag.bneighbor) continue;															// 近傍でなければ次へ
		CDConvex* a = DCAST(CDConvex, expandedObjects[i].phSolidIf->GetShape(0));		// 剛体が持つ凸形状
		CDConvex* b = DCAST(CDConvex, soPointer->GetShape(0));									// 力覚ポインタの凸形状
		Posed a2w, b2w;																								// 剛体のワールド座標
/*		if(expandedObjects[i].blocal) 	a2w = expandedObjects[i].phSolid.GetPose();				// blocalがtrueなら最新の情報でやる
		else */								a2w = expandedObjects[i].phSolidIf->GetPose();
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
				Vec3d dir = -1.0 * expandedObjects[i].syncInfo.neighborPoint.face_normal;
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
			if(!expandedObjects[i].flag.blocal){																			// 初めて最近傍物体になった時
				expandedObjects[i].flag.bfirstlocal = true;
				expandedObjects[i].phSolid = *DCAST(PHSolid, expandedObjects[i].phSolidIf);		// シーンが持つ剛体の中身を力覚プロセスで使う剛体（実体）としてコピーする
#ifdef _DEBUG
				if (expandedObjects[i].syncInfo.neighborPoint.face_normal * normal < 0.8){
					DSTR << "Too big change on normal" << normal << std::endl;
				}
#endif
				expandedObjects[i].syncInfo.neighborPoint.face_normal = normal;														// 初めて最近傍物体になったので，前回の法線は使わない．										
			}
			expandedObjects[i].flag.blocal = true;																		// 最近傍物体なのでblocalをtrueにする
			expandedObjects[i].syncInfo.neighborPoint.closestPoint = pa;																	// 剛体近傍点のローカル座標
			expandedObjects[i].syncInfo.neighborPoint.pointerPoint = pb;																// 力覚ポインタ近傍点のローカル座標
			expandedObjects[i].syncInfo.neighborPoint.last_face_normal = expandedObjects[i].syncInfo.neighborPoint.face_normal;					// 前回の法線（法線の補間に使う）
			expandedObjects[i].syncInfo.neighborPoint.face_normal = normal;															// 剛体から力覚ポインタへの法線
		}else{
			expandedObjects[i].flag.blocal = false;																		// 最近傍物体ではないのでblocalをfalseにする
			expandedObjects[i].flag.bfirstlocal = false;
		}
	}
}

#define DIVIDE_STEP

void PhysicsProcess::PredictSimulation(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	// neighborObjetsのblocalがtrueの物体に対して単位力を加え，接触しているすべての物体について，運動係数を計算する
#ifdef DIVIDE_STEP
	states2->SaveState(phscene);			// 予測シミュレーションのために現在の剛体の状態を保存する
	//	LCPの直前までシミュレーションしてその状態を保存
	phscene->ClearForce();
	phscene->GenerateForce();
	phscene->IntegratePart1();
#endif
	states->SaveState(phscene);			// 予測シミュレーションのために現在の剛体の状態を保存する

	for(unsigned i = 0; i < expandedObjects.size(); i++){
		if(!expandedObjects[i].flag.blocal) continue;
		
		// 現在の速度を保存
		SpatialVector currentvel, nextvel; 
		currentvel.v() = expandedObjects[i].phSolidIf->GetVelocity();											// 現在の速度
		currentvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();									// 現在の角速度									
		Vec3d cPoint = expandedObjects[i].phSolidIf->GetPose() * expandedObjects[i].syncInfo.neighborPoint.closestPoint;	// 力を加える点
		const float minTestForce = 0.5;
		if(expandedObjects[i].syncInfo.neighborPoint.test_force_norm < minTestForce){
			expandedObjects[i].syncInfo.neighborPoint.test_force_norm = minTestForce;		// テスト力が0なら1にする 
		}

		// 拘束座標系を作るための準備
		Vec3d rpjabs, vpjabs;
		rpjabs = cPoint - soPointer->GetCenterPosition();																							//力覚ポインタの中心から接触点までのベクトル
		vpjabs = soPointer->GetVelocity() + soPointer->GetAngularVelocity() % rpjabs;													//接触点での速度
		Vec3d rjabs, vjabs;
		rjabs = cPoint - expandedObjects[i].phSolidIf->GetCenterPosition();																	//剛体の中心から接触点までのベクトル
		vjabs = expandedObjects[i].phSolidIf->GetVelocity() + expandedObjects[i].phSolidIf->GetAngularVelocity() % rjabs;	//接触点での速度

		//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
		Vec3d n, t[2], vjrel, vjrelproj;
		n = -expandedObjects[i].syncInfo.neighborPoint.face_normal;
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
		nextvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		expandedObjects[i].syncInfo.motionCoeff.lastb = expandedObjects[i].syncInfo.motionCoeff.b;
		expandedObjects[i].syncInfo.motionCoeff.b = (nextvel - currentvel) / dt;

		TMatrixRow<6, 3, double> u;
		TMatrixRow<3, 3, double> force;
		// 法線方向に力を加える
		states->LoadState(phscene);
		force.col(0) = expandedObjects[i].syncInfo.neighborPoint.test_force_norm * n;
		expandedObjects[i].phSolidIf->AddForce(force.col(0), cPoint);
#ifdef DIVIDE_STEP
		phscene->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		u.col(0) = (nextvel - currentvel) /dt - expandedObjects[i].syncInfo.motionCoeff.b;

		// n + t[0]方向に力を加える
		states->LoadState(phscene);
		force.col(1) = expandedObjects[i].syncInfo.neighborPoint.test_force_norm * (n + t[0]);
		expandedObjects[i].phSolidIf->AddForce(force.col(1), cPoint);
		phscene->IntegratePart2();
		nextvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		u.col(1) = (nextvel - currentvel) /dt - expandedObjects[i].syncInfo.motionCoeff.b;

		// n+t[1]方向力を加える
		states->LoadState(phscene);
		force.col(2) = expandedObjects[i].syncInfo.neighborPoint.test_force_norm * (n + t[1]);
		expandedObjects[i].phSolidIf->AddForce(force.col(2), cPoint);
#ifdef DIVIDE_STEP
		phscene->IntegratePart2();
#else
		phscene->Step();
#endif
		nextvel.v() = expandedObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = expandedObjects[i].phSolidIf->GetAngularVelocity();
		u.col(2) = (nextvel - currentvel) /dt - expandedObjects[i].syncInfo.motionCoeff.b;
		
		expandedObjects[i].syncInfo.motionCoeff.A = u  * force.inv();				// 運動係数Aの計算
		states->LoadState(phscene);								// 元のstateに戻しシミュレーションを進める
	}
#ifdef DIVIDE_STEP
		states2->LoadState(phscene);								// 元のstateに戻しシミュレーションを進める
#endif
}

void PhysicsProcess::DisplayContactPlane(){
	for(unsigned int i = 0; i <  expandedObjects.size(); i++){
		if(!expandedObjects[i].flag.blocal) continue;
		Vec3d pPoint = soPointer->GetPose() * expandedObjects[i].syncInfo.neighborPoint.pointerPoint;
		Vec3d cPoint = expandedObjects[i].phSolidIf->GetPose() * expandedObjects[i].syncInfo.neighborPoint.closestPoint;
		Vec3d normal = expandedObjects[i].syncInfo.neighborPoint.face_normal;
		Vec3d v1(0,1,0);

		v1 +=  Vec3d(0, 0, 0.5) - Vec3d(0, 0, 0.5)*normal*normal;
		v1 -= v1*normal * normal;
		v1.unitize();
		Vec3d v2 = normal ^ v1;

		Vec4f moon(1.0, 1.0, 0.8, 0.3);
		GRDebugRenderIf* render = window->render->Cast();
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

void PhysicsProcess::DisplayLineToNearestPoint(){
	GLfloat moon[]={0.8,0.8,0.8};
	for(unsigned int i = 0; i <  expandedObjects.size(); i++){
		if(!expandedObjects[i].flag.blocal) continue;
		Vec3d pPoint = soPointer->GetPose() * expandedObjects[i].syncInfo.neighborPoint.pointerPoint;
		Vec3d cPoint = expandedObjects[i].phSolidIf->GetPose() * expandedObjects[i].syncInfo.neighborPoint.closestPoint;
		Vec3d normal = expandedObjects[i].syncInfo.neighborPoint.face_normal;
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

void PhysicsProcess::Keyboard(unsigned char key){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	states->ReleaseState(phscene);
	states2->ReleaseState(phscene);
	switch (key) {
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
			{
				// 立方体
				PHSolidDesc desc;
				CDBoxDesc bd;

				desc.mass = 0.05;
				desc.inertia = 0.0333 * Matrix3d::Unit();

				PHSolidIf* soBox = phscene->CreateSolid(desc);
				bd.boxsize = Vec3f(2, 2, 2);
				CDBoxIf* shapeBox = XCAST(GetSdk()->GetPHSdk()->CreateShape(bd));
				shapeBox->SetName("shapeBox");

				soBox->AddShape(shapeBox);
				soBox->SetFramePosition(Vec3d(-1, 5, 4));
				//soBox.back()->SetOrientation(
				//	Quaternionf::Rot(Rad(30), 'y') * 
				//	Quaternionf::Rot(Rad(10), 'x'));  
				soBox->GetShape(0)->SetVibration(-200,120,300);
				soBox->GetShape(0)->SetStaticFriction(1);//0.8);
				soBox->GetShape(0)->SetDynamicFriction(1);//0.6);
				phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
			}
			break;
		default:
			break;
	}
}
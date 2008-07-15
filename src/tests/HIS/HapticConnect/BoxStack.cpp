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

BoxStack bstack;

BoxStack::BoxStack(){
	bsync=false;
	calcPhys=true;
	dt = 0.02;//0.05;
	gravity =  Vec3d(0, -9.8f , 0);
	nIter = 15;
	bGravity = true;
	phscene = NULL;
	render = NULL;
	range = 1.5;
	bDebug = true;
	neighborObjects.clear();
}
namespace Spr{
void FASTCALL ContFindCommonPointSaveParam(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist);
}

void BoxStack::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();															// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	phscene = GetSdk()->GetScene()->GetPHScene();
	states = ObjectStatesIf::Create();
	states2 = ObjectStatesIf::Create();

	DesignObject();																// 剛体を作成

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(nIter);

	FWWinDesc windowDesc;
	windowDesc.title = "HapticConnect1.2";
	window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();

	InitCameraView();
}

void BoxStack::InitCameraView(){
	cameraInfo.view[0][0] = 0.9996;
	cameraInfo.view[0][1] = 0.0107463;
	cameraInfo.view[0][2] = -0.0261432;
	cameraInfo.view[0][3] = -0.389004;
	cameraInfo.view[1][0] = -6.55577e-010;
	cameraInfo.view[1][1] = 0.924909;
	cameraInfo.view[1][2] = 0.380188;
	cameraInfo.view[1][3] = 5.65711;
	cameraInfo.view[2][0] = 0.0282657;
	cameraInfo.view[2][1] = -0.380037;
	cameraInfo.view[2][2] = 0.92454;
	cameraInfo.view[2][3] = 13.7569;
	cameraInfo.view[3][0] = 0;
	cameraInfo.view[3][1] = 0;
	cameraInfo.view[3][2] = 0;
	cameraInfo.view[3][3] = 1;
}

void BoxStack::DesignObject(){
	// soFloor用のdesc
	phscene->SetStateMode(true);
	desc.mass = 1e20f;
	desc.inertia *= 1e30f;
	soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetDynamical(false);
	soFloor->SetGravity(false);
	
	// soBox用のdesc
	desc.mass = 0.05;
	desc.inertia = 0.033 * Matrix3d::Unit();
	{
		CDBoxDesc bd;
		bd.boxsize = Vec3f(2,2,2)*2.5  ;
		meshBox = XCAST(GetSdk()->GetPHSdk()->CreateShape(bd));
		meshBox->SetName("meshBox");
		CDSphereDesc sd;
		sd.radius = 1.2;
		meshSphere = XCAST(GetSdk()->GetPHSdk()->CreateShape(sd));
		meshSphere->SetName("meshSphere");
		CDCapsuleDesc cd;
		cd.radius = 1;
		cd.length = 1;
		meshCapsule = XCAST(GetSdk()->GetPHSdk()->CreateShape(cd));
		meshCapsule->SetName("meshCapsule");
	}

	{
		// meshConvex(soBox)のメッシュ形状
		CDConvexMeshDesc md;
		md.vertices.push_back(Vec3d(-1,-1,-1));
		md.vertices.push_back(Vec3d(-1,-1, 1));	
		md.vertices.push_back(Vec3d(-1, 1,-1));	
		md.vertices.push_back(Vec3d(-1, 1, 1));
		md.vertices.push_back(Vec3d( 1,-1,-1));	
		md.vertices.push_back(Vec3d( 1,-1, 1));
		md.vertices.push_back(Vec3d( 1, 1,-1));
		md.vertices.push_back(Vec3d( 1, 1, 1));
		meshConvex = DCAST(CDConvexMeshIf, GetSdk()->GetPHSdk()->CreateShape(md));
		meshConvex->SetName("meshConvex");

		// meshFloor(soFloor)のメッシュ形状
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 30;
			md.vertices[i].z *= 20;
			md.vertices[i].y *= 20;
		}
		meshFloor = DCAST(CDConvexMeshIf, GetSdk()->GetPHSdk()->CreateShape(md));
		meshFloor->SetName("meshFloor");
	}
	soFloor->AddShape(meshFloor);
	soFloor->SetFramePosition(Vec3d(0,-20 - 0.7,0));
	soFloor->SetName("solidFloor");

	// 力覚ポインタ
	CDSphereDesc sd;
	sd.radius = 0.5;//1.0;
	CDSphereIf* sphere = DCAST(CDSphereIf,  GetSdk()->GetPHSdk()->CreateShape(sd));
	soPointer = phscene->CreateSolid(desc);
	soPointer->AddShape(sphere);//meshConvex);
	soPointer->SetFramePosition(Vec3d(0, 3.0, 0));  
	soPointer->SetDynamical(false);
	// 力覚ポインタをspringhead2の接触計算から外す
	// 剛体が増えるたびにContactModeをNONEにしなけらばいけない(増えた剛体と接触計算をしてしまうため)
	phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
}

void BoxStack::Idle(){
	PhysicsStep();
}
void BoxStack::Step(){
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
		phscene->Step();

		for(unsigned i = 0; i < neighborObjects.size(); i++){
			if(!neighborObjects[i].blocal) continue;
			SpatialVector curvel;
			curvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
			curvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
			neighborObjects[i].curb = (curvel - neighborObjects[i].lastvel) / dt;
		}
		FindNearestObject();	// 近傍物体の取得
		PredictSimulation();
		glutPostRedisplay();
		calcPhys = false;
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
	GetSdk()->SetDebugMode(true);
	render = window->render->Cast();

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
	//	DrawHapticSolids();
	}
	render->PopLight();	//	光源の削除

	curRender->EndScene();
	glutSwapBuffers();
}

void BoxStack::UpdateHapticPointer(){
	// cameraInfo.view.Rot()をかけて力覚ポインタの操作をカメラを回転にあわせる
	soPointer->SetFramePosition(phpointer.GetFramePosition());//cameraInfo.view.Rot() * phpointer.GetFramePosition());				
	soPointer->SetOrientation(phpointer.GetOrientation());					
	//soPointer->SetVelocity(phpointer.GetVelocity());
	//soPointer->SetAngularVelocity(phpointer.GetAngularVelocity());	
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

#if 0		
	// AABBで力覚ポインタ近傍の物体を絞る（実装中）
	// ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
	// SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外		
	//1. BBoxレベルの衝突判定
	int N = neighborObjects.size();
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

	for(int i = 0; i < neighborObjects.size(); i++){
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
			if(!neighborObjects[i].blocal){																			// 初めて最近傍物体になった時
				neighborObjects[i].bfirstlocal = true;
				neighborObjects[i].phSolid = *DCAST(PHSolid, neighborObjects[i].phSolidIf);		// シーンが持つ剛体の中身を力覚プロセスで使う剛体（実体）としてコピーする
				neighborObjects[i].face_normal = normal;														// 初めて最近傍物体になったので，前回の法線は使わない．										
			}
			neighborObjects[i].blocal = true;																		// 最近傍物体なのでblocalをtrueにする
			neighborObjects[i].closestPoint = pa;																	// 剛体近傍点のローカル座標
			neighborObjects[i].pointerPoint = pb;																// 力覚ポインタ近傍点のローカル座標
			neighborObjects[i].last_face_normal = neighborObjects[i].face_normal;					// 前回の法線（法線の補間に使う）
			neighborObjects[i].face_normal = normal;															// 剛体から力覚ポインタへの法線
		}else{
			neighborObjects[i].blocal = false;																		// 最近傍物体ではないのでblocalをfalseにする
			neighborObjects[i].bfirstlocal = false;
		}
	}
}

//#define DIVIDE_STEP

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
		if(neighborObjects[i].test_force_norm == 0)	neighborObjects[i].test_force_norm = 1;		// テスト力が0なら1にする  

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
#ifdef DIVIDE_STEP
		states2->LoadState(phscene);								// 元のstateに戻しシミュレーションを進める
#else
		states->LoadState(phscene);								// 元のstateに戻しシミュレーションを進める
#endif
	}
}

void BoxStack::DisplayContactPlane(){
	GLfloat moon[] = {0.8,0.8,0.8};
	for(unsigned int i = 0; i <  neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		Vec3d pPoint = soPointer->GetPose() * neighborObjects[i].pointerPoint;
		Vec3d cPoint = neighborObjects[i].phSolidIf->GetPose() * neighborObjects[i].closestPoint;
		Vec3d normal = neighborObjects[i].face_normal;
		Vec3d v1;
		v1[0] = 1 / normal[0];
		v1[1] = -1 / normal[1];
		v1[2] = 1 / normal[2];
		Vec3d v2;
		// 法線ベクトルの向きによって外積の順番を考えないと面がうまく張れない．
		if(normal[2]>0) v2 = normal % v1;
		else v2 = v1 % normal;		
		v1 = v1.unit();
		v2 = v2.unit();
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, moon);
		render->PushModelMatrix();
		Vec3d offset = 0.02 * normal;
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
		case 'a':
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
		case ' ':
			{
				// ConvexBox
				desc.mass = 0.08;
				desc.inertia = 0.033 * Matrix3d::Unit() * 10;
				//desc.dynamical = false;
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshBox);
				soBox.back()->SetFramePosition(Vec3d(-1, 5, 4));
				//soBox.back()->SetOrientation(
				//	Quaternionf::Rot(Rad(30), 'y') * 
				//	Quaternionf::Rot(Rad(10), 'x'));  
				ostringstream os;
				os << "box" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
			DSTR << "Create Box" << endl;
			DSTR << "NSolids		" <<  phscene->NSolids() << endl;
			break;
		case 'v':
			{
				// MeshCapsule
				desc.mass = 0.05;
				desc.inertia = 0.001 * Matrix3d::Unit();
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->SetAngularVelocity(Vec3f(0,0,0.2));
				soBox.back()->AddShape(meshCapsule);
				soBox.back()->SetFramePosition(Vec3f(0.5, 20,0));
	//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				ostringstream os;
				os << "capsule" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
				phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
			}
			break;
		case 'b':
			{
				// MeshSphere
				desc.mass = 0.05;
				desc.inertia = 0.001 * Matrix3d::Unit();
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshSphere);
//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->SetFramePosition(Vec3f(0.5, 20,0));
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				ostringstream os;
				os << "sphere" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
				phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
			}
			break;
		case 'n':
			{
				// ConvexMesh
				desc.mass = 0.1;
				desc.inertia = 0.1 * Matrix3d::Unit();
				soBox.push_back(phscene->CreateSolid(desc));
				CDConvexMeshDesc md;
				int nv = rand() % 100 + 50;
				for(int i=0; i < nv; ++i){
					Vec3d v;
					for(int c=0; c<3; ++c){
						v[c] = (rand() % 100 / 100.0 - 0.5) * 5 * 1.3;
					}
					md.vertices.push_back(v);
				}
				CDShapeIf* s = GetSdk()->GetPHSdk()->CreateShape(md);
				soBox.back()->AddShape(s);
				soBox.back()->SetFramePosition(Vec3f(0.5, 20,0));
//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				ostringstream os;
				os << "sphere" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
				phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
			}
			break;
		case 'm':
			{
				// Lump of Box
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				Posed pose;
				pose.Pos() = Vec3d(3, 0, 0);
				soBox.back()->SetShapePose(1, pose);
				pose.Pos() = Vec3d(-3, 0, 0);
				soBox.back()->SetShapePose(2, pose);
				pose.Pos() = Vec3d(0, 3, 0);
				soBox.back()->SetShapePose(3, pose);
				pose.Pos() = Vec3d(0, -3, 0);
				soBox.back()->SetShapePose(4, pose);
				pose.Pos() = Vec3d(0, 0, 3);
				soBox.back()->SetShapePose(5, pose);
				pose.Pos() = Vec3d(0, 0, -3);
				soBox.back()->SetShapePose(6, pose);
				
				soBox.back()->SetFramePosition(Vec3f(0.5, 20,0));
	//			soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				ostringstream os;
				os << "box" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			    phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);				
			}
			break;
		case 'x':
			{
				// Wall
				PHSolidDesc soliddesc;
				soliddesc.mass = 0.05;
				soliddesc.inertia = 0.033 * Matrix3d::Unit();
				int wall_height = 4;
				int numbox = 5;
				for(int i = 0; i < wall_height; i++){
					for(int j = 0; j < numbox; j++){
						soBox.push_back(phscene->CreateSolid(soliddesc));
						soBox.back()->AddShape(meshBox);
						soBox.back()->SetFramePosition(Vec3d(-4.0 + (2.0 + 0.1) * j , (2.0 + 0.1) * (double)i, -2.0));  
					}
				}
			}
			break;
		case 'z':
			{
				// Tower
				PHSolidDesc soliddesc;
				double tower_radius = 5;
				int tower_height = 5;
				int numbox = 5;
				double theta;
				for(int i = 0; i < tower_height; i++){
					for(int j = 0; j < numbox; j++){
						soBox.push_back(phscene->CreateSolid(soliddesc));
						soBox.back()->AddShape(meshBox);
						theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)numbox;
//						soBox.back()->SetFramePosition(Vec3f(0.5, 20, 0));
						soBox.back()->SetFramePosition(Vec3d(tower_radius * cos(theta), 2.0 * ((double)i), tower_radius * sin(theta)));
						soBox.back()->SetOrientation(Quaterniond::Rot(-theta, 'y'));  
					}
				}
			}
			break;
		default:
			break;
	}
}
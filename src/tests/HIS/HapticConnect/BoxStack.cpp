#include "BoxStack.h"
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

BoxStack::BoxStack(){
	dt = 0.05;
	gravity =  Vec3d(0, -9.8f, 0);
	nIter = 15;
	bGravity = true;
	bStep = true;
	phscene = NULL;
	render = NULL;
	range = 1.5;
	neighborObjects.clear();
}

void BoxStack::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();															// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	phscene = GetSdk()->GetScene()->GetPHScene();
	states = ObjectStatesIf::Create();

	DesignObject();																// 剛体を作成

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(nIter);

	FWWinDesc windowDesc;
	windowDesc.title = "HapticConnect";
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
	desc.mass = 1e20f;
	desc.inertia *= 1e30f;
	soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetDynamical(false);
	soFloor->SetGravity(false);
	
	// soBox用のdesc
	desc.mass = 1.0;
	desc.inertia = 2.0 * Matrix3d::Unit();

	{
		// meshConvex(soBox)のメッシュ形状
		CDConvexMeshDesc md;
		md.vertices.push_back(Vec3d(-1,-1,-1) * 2);
		md.vertices.push_back(Vec3d(-1,-1, 1)* 2);	
		md.vertices.push_back(Vec3d(-1, 1,-1)* 2);	
		md.vertices.push_back(Vec3d(-1, 1, 1)* 2);
		md.vertices.push_back(Vec3d( 1,-1,-1)* 2);	
		md.vertices.push_back(Vec3d( 1,-1, 1)* 2);
		md.vertices.push_back(Vec3d( 1, 1,-1)* 2);
		md.vertices.push_back(Vec3d( 1, 1, 1)* 2);
		meshConvex = DCAST(CDConvexMeshIf, GetSdk()->GetPHSdk()->CreateShape(md));
		meshConvex->SetName("meshConvex");

		// meshFloor(soFloor)のメッシュ形状
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 30;
			md.vertices[i].z *= 20;
		}
		meshFloor = DCAST(CDConvexMeshIf, GetSdk()->GetPHSdk()->CreateShape(md));
		meshFloor->SetName("meshFloor");
	}
	soFloor->AddShape(meshFloor);
	soFloor->SetFramePosition(Vec3d(0,-2.7,0));
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

void BoxStack::Start(){
	instance = this;
	if (!NWin()){
		CreateWin();
		wins.back()->SetScene(GetSdk()->GetScene());
	}
};

void BoxStack::Step(){
	UpdateHapticPointer();
//	if(bStep)	FWAppGLUT::Step();
//	PHConstraintEngine* engine = phscene->GetConstraintEngine()->Cast();
	//if(engine->solidPairs.width() > 2){
	//	DSTR << engine->solidPairs.item(0, 2)->shapePairs.item(0,0)->state << ":::"
	//		<< engine->solidPairs.item(0, 2)->solid[1]->GetCenterPosition() << ":::"
	//		<< engine->solidPairs.item(0, 2)->solid[1]->GetVelocity() << ":::"
	//		<<engine->solidPairs.item(0, 2)->shapePairs.item(0,0)->normal << endl;
	//}
	FindNearestObject();	// 近傍物体の取得
	PredictSimulation();
	glutPostRedisplay();
 }

void BoxStack::Display(){
	// 物理シミュレーションを進める
	Step();			

	// 描画の設定
	GetSdk()->SetDebugMode(true);
	render = window->render->Cast();
	render->SetRenderMode(true, false);
//	render->EnableRenderAxis();
//	render->EnableRenderForce();
//	render->EnableRenderContact();

	// カメラ座標の指定
	GRCameraIf* cam = window->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		//Affinef af = cam->GetFrame()->GetTransform();
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
	DisplayLineToNearestPoint();			// 力覚ポインタと剛体の近傍点の間をつなぐ
//	DrawHapticSolids();
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
	sceneSolids.clear();
	PHSolidIf** solids = phscene->GetSolids();
	for(int i = 0; i < phscene->NSolids(); i++){
		if(solids[i] == soPointer) continue;
		sceneSolids.resize(sceneSolids.size() + 1);
		sceneSolids.back().phSolidIf = solids[i];
		sceneSolids.back().blocal = false;
	}
	// sceneSolidsで新しく増えた分をneighborObjectsに追加する
	for(unsigned i = (int)neighborObjects.size(); i < sceneSolids.size(); i++){
		neighborObjects.resize(i + 1);
		neighborObjects.back().phSolidIf = sceneSolids[i].phSolidIf;
		neighborObjects.back().blocal = false;
	}
	// GJKで近傍点を求め近傍物体を探す
	// 近傍物体だったらblocalをtrueにし，phSolidにphSolidIfをコピーする
	// blocalがすでにtrueだったらコピー済みなので近傍点だけコピーする
	for(unsigned i = 0; i < (int)neighborObjects.size(); i++){
		CDConvex* a = DCAST(CDConvex, neighborObjects[i].phSolidIf->GetShape(0));		// 剛体が持つ凸形状
		CDConvex* b = DCAST(CDConvex, soPointer->GetShape(0));									// 力覚ポインタの凸形状
		Posed a2w, b2w;																								// 剛体のワールド座標
/*		if(neighborObjects[i].blocal) 	a2w = neighborObjects[i].phSolid.GetPose();				// blocalがtrueなら最新の情報でやる
		else */								a2w = neighborObjects[i].phSolidIf->GetPose();
		b2w = soPointer->GetPose();												// 力覚ポインタのワールド座標
		Vec3d pa ,pb;																		// pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）
		pa = pb = Vec3d(0.0, 0.0, 0.0);
		FindClosestPoints(a, b, a2w, b2w, pa, pb);								// GJKで近傍点の算出
		Vec3d wa = a2w * pa;															// 剛体近傍点のワールド座標
		Vec3d wb = b2w * pb;															// 力覚ポインタ近傍点のワールド座標
		Vec3d a2b = wb - wa;															// 剛体から力覚ポインタへのベクトル
		Vec3d normal = a2b.unit();
		if(a2b.norm() < range){									// 近傍点までの長さから近傍物体を絞る
			if(a2b.norm() < 0.01){								// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める		
				pa = pb = Vec3d(0.0, 0.0, 0.0);
				Vec3d dir = -neighborObjects[i].direction;
				if(dir = Vec3f(0.0, 0.0, 0.0) ) dir = -(soPointer->GetCenterPosition() - wa);
				double dist = 0.0;
				int cp = ContFindCommonPoint(a, b, a2w, b2w, dir, -DBL_MAX, 0.0, normal, pa, pb, dist);
			}
			if(!neighborObjects[i].blocal){					// 初めて近傍物体になった時はシーンの剛体の中身を力覚プロセスで使う剛体としてコピーする
				neighborObjects[i].phSolid = *DCAST(PHSolid, neighborObjects[i].phSolidIf);
				neighborObjects[i].bfirstlocal = true;
			}
			neighborObjects[i].blocal = true;				// 近傍物体なのでblocalをtrueにする
			neighborObjects[i].closestPoint = pa;			// 剛体近傍点のローカル座標
			neighborObjects[i].pointerPoint = pb;		// 力覚ポインタ近傍点のローカル座標
			neighborObjects[i].direction = normal;		// 剛体から力覚ポインタへの法線
		}else{
			neighborObjects[i].blocal = false;				// 近傍物体ではないのでblocalをfalseにする
			neighborObjects[i].bfirstlocal = false;
		}
	}
}

void BoxStack::PredictSimulation(){
	// neighborObjetsのblocalがtrueの物体に対して単位力を加え，接触しているすべての物体について，運動係数を計算する
	states->ReleaseState(phscene);		// SaveStateする前に解放する
	states->SaveState(phscene);			// 予測シミュレーションのために現在の剛体の状態を保存する

	for(unsigned i = 0; i < neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		
		// 現在の速度を保存
		currentvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		currentvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		Vec3d cPoint = neighborObjects[i].phSolidIf->GetPose() * neighborObjects[i].closestPoint;	// 力を加える点

		Vec3d rpjabs, vpjabs;
		rpjabs = cPoint - soPointer->GetCenterPosition();									//力覚ポインタの中心から接触点までのベクトル
		vpjabs = soPointer->GetVelocity() + soPointer->GetAngularVelocity() % rpjabs;	//接触点での速度
		Vec3d rjabs, vjabs;
		rjabs = cPoint - neighborObjects[i].phSolidIf->GetCenterPosition();	//剛体の中心から接触点までのベクトル
		vjabs = neighborObjects[i].phSolidIf->GetVelocity() + neighborObjects[i].phSolidIf->GetAngularVelocity() % rjabs;	//接触点での速度
		
		//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
		Vec3d n, t[2], vjrel, vjrelproj;
		n = -neighborObjects[i].direction;
		vjrel = vjabs - vpjabs;
		vjrelproj = vjrel - (n * vjrel) * n;		//相対速度ベクトルを法線に直交する平面に射影したベクトル
		double vjrelproj_norm = vjrelproj.norm();
		if(vjrelproj_norm < 1.0e-10){
			t[0] = n % Vec3d(1.0, 0.0, 0.0);	
			if(t[0].norm() < 1.0e-10)
				t[0] = n % Vec3d(0.0, 1.0, 0.0);
			t[0].unitize();
		}
		else{
			t[0] = vjrelproj / vjrelproj_norm;
		}
		t[1] = n % t[0];

		//// 何も力を加えないでシミュレーションを1ステップ進める
		//FWAppGLUT::Step();
		//nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		//nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		//neighborObjects[i].b = (nextvel - currentvel) / dt;
		//DSTR << "----------"<< endl;
		//DSTR << "current" << currentvel.v() << endl;
		//DSTR << "nextvel" << nextvel.v() << endl;
		//DSTR << "b" << neighborObjects[i].b << endl;

		TMatrixRow<6, 3, double> u;
		TMatrixRow<3, 3, double> force;
		// 単位力(1.0, 0.0, 0.0)を加える
		states->LoadState(phscene);
		neighborObjects[i].phSolidIf->AddForce(n, cPoint);
		PHSolid* solid = neighborObjects[i].phSolidIf->Cast();
		FWAppGLUT::Step();
		nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		force.col(0) = n;
		u.col(0) = (nextvel - currentvel) /dt - neighborObjects[i].b;
		//		neighborObjects[i].A.col(0) = (nextvel - currentvel) / dt - neighborObjects[i].b;//(nextvel - currentvel) / dt - neighborObjects[i].b;
//		DSTR << "colum1" << neighborObjects[i].A.col(0) << endl;
		//DSTR << "normal" <<-neighborObjects[i].direction << endl;
		//DSTR << "current" << currentvel.v() << endl;
		//DSTR << "nextvel" << nextvel.v() << endl;
		//DSTR << "diff" << nextvel - currentvel << endl;

		// 単位力(0.0, 1.0, 0.0)を加える
		states->LoadState(phscene);
		neighborObjects[i].phSolidIf->AddForce(n + t[0] , cPoint);
		FWAppGLUT::Step();
		nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		force.col(1) = n + t[0];
		u.col(1) = (nextvel - currentvel) /dt - neighborObjects[i].b;
//		neighborObjects[i].A.col(1) = (nextvel - currentvel) / dt - neighborObjects[i].b;//(nextvel - currentvel) / dt - neighborObjects[i].b;
		//DSTR << "colum2" << neighborObjects[i].A.col(1) << endl;
		//DSTR << "current" << currentvel << endl;
		//DSTR << "next" << nextvel<< endl;
		//DSTR << "diff" << nextvel - currentvel << endl;

		// 単位力(0.0, 0.0 ,1.0)を加える
		states->LoadState(phscene);
		neighborObjects[i].phSolidIf->AddForce(n + t[1], cPoint);
		FWAppGLUT::Step();
		nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		force.col(2) = n + t[1];
		u.col(2) = (nextvel - currentvel) /dt - neighborObjects[i].b;
		//		neighborObjects[i].A.col(2) = (nextvel - currentvel) / dt - neighborObjects[i].b;//(nextvel - currentvel) / dt - neighborObjects[i].b;
		//DSTR << "colum3" << neighborObjects[i].A.col(2) << endl;
		//DSTR << "current" << currentvel << endl;
		//DSTR << "next" << nextvel<< endl;
		
		neighborObjects[i].A = u  * force.inv();
		states->LoadState(phscene);			// 元のstateに戻しシミュレーションを進める
		DSTR << "A" <<neighborObjects[i].A << endl;
		DSTR << "b" << neighborObjects[i].b << endl;
	}
	if(bStep)	FWAppGLUT::Step();
	for(unsigned i = 0; i < neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		nextvel.v() = neighborObjects[i].phSolidIf->GetVelocity();
		nextvel.w() = neighborObjects[i].phSolidIf->GetAngularVelocity();
		neighborObjects[i].b = (nextvel - currentvel) / dt;
	}
}

void BoxStack::DisplayLineToNearestPoint(){
	GLfloat moon[]={0.8,0.8,0.8};
	for(unsigned int i = 0; i <  neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		Vec3d pPoint = soPointer->GetPose() * neighborObjects[i].pointerPoint;
		Vec3d cPoint = neighborObjects[i].phSolidIf->GetPose() * neighborObjects[i].closestPoint;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, moon);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, moon);
		//glBegin(GL_LINES);
		//glVertex3f(pPoint.X(), pPoint.Y(), pPoint.Z());
		//glVertex3f(cPoint.X(), cPoint.Y(), cPoint.Z());
		//glEnd();

		Vec3d rpjabs, vpjabs;
		rpjabs = cPoint - soPointer->GetCenterPosition();									//力覚ポインタの中心から接触点までのベクトル
		vpjabs = soPointer->GetVelocity() + soPointer->GetAngularVelocity() % rpjabs;	//接触点での速度
		Vec3d rjabs, vjabs;
		rjabs = cPoint - neighborObjects[i].phSolidIf->GetCenterPosition();	//剛体の中心から接触点までのベクトル
		vjabs = neighborObjects[i].phSolidIf->GetVelocity() + neighborObjects[i].phSolidIf->GetAngularVelocity() % rjabs;	//接触点での速度

		//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
		Vec3d n, t[2], vjrel, vjrelproj;
		n = -neighborObjects[i].direction;
		vjrel = vjabs - vpjabs;
		vjrelproj = vjrel - (n * vjrel) * n;		//相対速度ベクトルを法線に直交する平面に射影したベクトル
		double vjrelproj_norm = vjrelproj.norm();
		if(vjrelproj_norm < 1.0e-10){
			t[0] = n % Vec3d(1.0, 0.0, 0.0);	
			if(t[0].norm() < 1.0e-10)
				t[0] = n % Vec3d(0.0, 1.0, 0.0);
			t[0].unitize();
		}
		else{
			t[0] = vjrelproj / vjrelproj_norm;
		}
		t[1] = n % t[0];
		glBegin(GL_LINES);
		glVertex3f(cPoint.X(), cPoint.Y(), cPoint.Z());
		glVertex3f(cPoint.X() + n[0], cPoint.Y() + n[1], cPoint.Z()+ n[2]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(cPoint.X(), cPoint.Y(), cPoint.Z());
		glVertex3f(cPoint.X() + t[0][0], cPoint.Y() + t[0][1], cPoint.Z() + t[0][2]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(cPoint.X(), cPoint.Y(), cPoint.Z());
		glVertex3f(cPoint.X() + t[1][0], cPoint.Y() + t[1][1], cPoint.Z() + t[1][2]);
		glEnd();
	}
}

void BoxStack::DrawHapticSolids(){
	GLfloat purple[] = {1.0, 0.0, 1.0, 0.0};
	GRRenderIf* render = GetCurrentWin()->GetRender();
	for(unsigned int i = 0; i < hapticsolids.size(); i++){
		render->PushModelMatrix();
		Affinef hsolid;
		Posed::Unit(hapticsolids[i].GetCenterPosition()).ToAffine(hsolid);
		render->MultModelMatrix(hsolid);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, purple);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, purple);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, purple);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, purple);
		glutSolidCube(1.0);
		render->PopModelMatrix();
	}
};

void BoxStack::Keyboard(unsigned char key){
	switch (key) {
		case 'r':
			if(bStep){
				bStep = false;
				DSTR << "Stop Simulation" << endl;
			}else{
				bStep = true;
				DSTR << "Run Simulation" << endl;
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
				states->ReleaseState(phscene);
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshConvex);
				soBox.back()->SetFramePosition(Vec3d(-1, 3, 4));
				soBox.back()->SetOrientation(
					Quaternionf::Rot(Rad(30), 'y') * 
					Quaternionf::Rot(Rad(10), 'x'));  
				ostringstream os;
				os << "box" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
			DSTR << "Create Box" << endl;
			DSTR << "NSolids		" <<  phscene->NSolids() << endl;
			break;
		default:
			break;
	}
}
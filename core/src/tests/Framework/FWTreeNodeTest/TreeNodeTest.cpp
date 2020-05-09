#include "TreeNodeTest.h"
//#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <GL/glut.h>

#define ESC 27

TreeNodeTest ttest;

TreeNodeTest::TreeNodeTest(){
	dt = 0.02;
	gravity =  Vec3d(0, -9.8f , 0);
	nIter = 5;
	bGravity = true;
	bDebug = false;
	bStep = true;
}

void TreeNodeTest::Init(int argc, char* argv[]){
	FWApp::Init(argc, argv);
	Vec3d pos = Vec3d(0, 6, 10.0);						// カメラ初期位置
	GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定

	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	BuildScene();		// 剛体を作成
	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration((int)nIter);
}

void TreeNodeTest::Reset(){
	GetSdk()->Clear();															// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	BuildScene();																// 剛体を作成

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration((int)nIter);

	GetCurrentWin()->SetScene(GetSdk()->GetScene());
}

void TreeNodeTest::BuildScene(){
	PHSdkIf* phsdk = GetSdk()->GetPHSdk();
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	// boxFloor
	CDBoxDesc bdFloor;
	bdFloor.boxsize = Vec3f(20, 2, 20);
	CDBoxIf* boxFloor = phsdk->CreateShape(bdFloor)->Cast();
	boxFloor->SetName("boxFloor");
	// soFloor
	PHSolidDesc sdFloor;
	sdFloor.dynamical = false;
	sdFloor.pose = Posed::Trn(0, -6, 0);
	soFloor = phscene->CreateSolid(sdFloor);		// 剛体をdescに基づいて作成
	soFloor->AddShape(boxFloor);
	soFloor->SetShapePose(0, Posed::Trn(0,-1,0));
	soFloor->SetName("solidFloor");

	//	リンクの作成
	const int nLink = 6;
	PHSolidDesc sd;
	CDBoxDesc bd;
	bd.boxsize = Vec3d(1, 0.2, 0.2);
	CDBoxDesc bdTop = bd;
	bdTop.boxsize.x = 0.2;
	const double rootDist = 8;
	for (int tid = 0; tid < 2; ++tid) {
		for (int i = 0; i < nLink; ++i) {
			links[tid].push_back(phscene->CreateSolid(sd));
			links[tid][i]->SetPose(Posed::Trn((-rootDist/2 + rootDist*tid) + i , 6, 0));
			CDBoxIf* box = phsdk->CreateShape(i ? bd : bdTop)->Cast();
			links[tid][i]->AddShape(box);
		}
		links[tid][0]->SetDynamical(false);
		for (int i = 1; i < links[tid].size(); ++i) {
			PHBallJointDesc djoint;
			djoint.poseSocket.Pos() = i == 1 ? Vec3d(0,0,0) : Vec3d(0.5, 0, 0);
			djoint.posePlug.Pos() = Vec3d(-0.5, 0, 0);
			if (tid == 0) {
				djoint.spring = 100;
				djoint.damper = 100;
			}
			joints[tid].push_back(phscene->CreateJoint(links[tid][i - 1], links[tid][i], djoint)->Cast());
		}
		phscene->CreateTreeNodes(links[tid][0]);
	}
	phscene->SetContactMode(PHSceneDesc::MODE_NONE);
}

int count = 0;
void TreeNodeTest::UserFunc() {
	count++;
	//	read joint torques
	DSTR << "count:" << count << std::endl;
	for (int tid = 0; tid < 2; ++tid) {
#if 0
		Vec3d cf, ct, mf;
		joints[tid][0]->GetConstraintForce(cf, ct);
		mf = joints[tid][0]->GetMotorForce();
		DSTR << tid << ": mf:" << mf << ", cf:" << cf << ct << std::endl;
#else
		DSTR << tid << ": ";
		for (int i = 0; i < joints->size(); ++i) {
			Vec3f mf = joints[tid][i]->GetMotorForce();
			DSTR << i << ":" << mf << "  ";
		}
		DSTR << std::endl;
#endif
	}
	for (int i = 0; i < joints->size(); ++i) {
		Vec3f mf = joints[0][i]->GetMotorForce();
		joints[1][i]->SetOffsetForce(mf);
	}
	return;

	//	Exit by timer
	if (count == 1 || count == 20) {
		Keyboard(' ', 0, 0);
	}
	else if (count == 40) {
		Keyboard('n', 0, 0);
	}else if (count > 80) {
		PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
		if (phscene->NSolids() == 1) goto next;
		for (int i = 0; i < phscene->NSolids(); ++i) {
			if (phscene->GetSolid(i)->GetVelocity().norm() > 0.1) {
				goto next;
			}
		}
		exit(0);
	}
	next:
	if (count > 180.0 / dt) exit(-1);
}
void TreeNodeTest::Keyboard(int key, int x, int y){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	switch (key) {
		case ESC:
		case  'q':
			exit(0);
			break;
		case 'r':
			Reset();
			break;
		case 'd':
			if(bDebug){
				bDebug = false;
				DSTR << "Debug Mode OFF" << std::endl;
			}else{
				bDebug = true;
				DSTR << "Debug Mode ON" << std::endl;
			}
			break;
		case 'g':
			if(bGravity){
				bGravity = false;
				Vec3d zeroG = Vec3d(0.0, 0.0, 0.0);
				phscene->SetGravity(zeroG);
				DSTR << "Gravity OFF" << std::endl;
			}else{
				bGravity = true;
				phscene->SetGravity(gravity);
				DSTR << "Gravity ON" << std::endl;
			}
			break;
		case 's':
			bStep = false;
			bOneStep = true;
			break;
		case 'a':
			bStep = true;
			break;
		default:
			break;
	}
}
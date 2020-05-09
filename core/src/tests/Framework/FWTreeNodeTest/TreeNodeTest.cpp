#include "TreeNodeTest.h"
//#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <GL/glut.h>

TreeNodeTest ttest;


const double dt = 0.02;
void TreeNodeTest::Init(int argc, char* argv[]){
	FWApp::Init(argc, argv);
	Vec3d pos = Vec3d(0, 6, 10.0);						// カメラ初期位置
	GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定

	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	BuildScene();		// 剛体を作成
	phscene->SetGravity(Vec3d(0, -9.8, 0));				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(5);
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
	PHSolidIf* soFloor;
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
	bdTop.boxsize.x = 0.2f;
	const double rootDist = 8;
	for (int tid = 0; tid < 2; ++tid) {
		for (int i = 0; i < nLink; ++i) {
			links[tid].push_back(phscene->CreateSolid(sd));
			links[tid][i]->SetPose(Posed::Trn((-rootDist/2 + rootDist*tid) + i , 6, 0));
			CDBoxIf* box = phsdk->CreateShape(i ? bd : bdTop)->Cast();
			links[tid][i]->AddShape(box);
			if (i > 0) {
				CDBoxIf* box = phsdk->CreateShape(bdTop)->Cast();
				links[tid][i]->AddShape(box);
				links[tid][i]->SetShapePose(1, Posed::Trn(0,0.2,0));
			}
		}
		links[tid][0]->SetDynamical(false);
		const double rot = Rad(90);
		for (int i = 1; i < links[tid].size(); ++i) {
			PHBallJointDesc djoint;
			djoint.poseSocket.Pos() = i == 1 ? Vec3d(0,0,0) : Vec3d(0.5, 0, 0);
			djoint.posePlug.Pos() = Vec3d(-0.5, 0, 0);
			djoint.posePlug.Ori() = Quaterniond::Rot(rot, 'x');
			Posed lp = links[tid][i]->GetPose();
			lp.Ori() = Quaterniond::Rot(-rot*i, 'x') * lp.Ori();
			links[tid][i]->SetPose(lp);
			if (tid == 0) {
				djoint.spring = 100;
				djoint.damper = 100;
			}
			else {
				djoint.spring = 1;
				djoint.damper = 1;
			}
			joints[tid].push_back(phscene->CreateJoint(links[tid][i - 1], links[tid][i], djoint)->Cast());
		}
	}
	//	次をコメントアウトすると、TreeNode=フェザーストーンを使わない、全自由度の関節になる。
	phscene->CreateTreeNodes(links[0][0]);	//	トルク計測用 PD強
	phscene->CreateTreeNodes(links[1][0]);	//	トルク適用用 PD弱
	phscene->SetContactMode(PHSceneDesc::MODE_NONE);
	//phscene->Print(DSTR);
}

int count = 0;
int stopCount = 0;
void TreeNodeTest::UserFunc() {
	count++;
	//	read joint torques
	DSTR << "count:" << count << std::endl;
	for (int tid = 0; tid < 2; ++tid) {
#if 0	//	拘束力を見る
		Vec3d cf, ct, mf;
		joints[tid][0]->GetConstraintForce(cf, ct);
		mf = joints[tid][0]->GetMotorForce();
		DSTR << tid << ": mf:" << mf << ", cf:" << cf << ct << std::endl;
#else	//	制御のトルクを見る
		DSTR << tid << ": ";
		for (int i = 0; i < joints->size(); ++i) {
			Vec3f mf = joints[tid][i]->GetMotorForce();
			DSTR << i << ":" << mf << "  ";
		}
		DSTR << std::endl;
#endif
	}
	//	joints[0]のmotor force をjoints[1]の offset forceに設定
	for (int i = 0; i < joints->size(); ++i) {
		Vec3f mf = joints[0][i]->GetMotorForce();
		joints[1][i]->SetOffsetForce(mf);
	}
	
	//	しばらくたってから、しばらく静止が続いたら、終了
	if (count > 1.0 / dt) {
		stopCount++;
		for (int i = 0; i < joints->size(); ++i) {
			if (joints[0][i]->GetVelocity().norm() > 0.001) {
				stopCount = 0;	//	wait until stop
				break;
			}
			Quaterniond qDiff = joints[1][i]->GetPosition() * joints[0][i]->GetPosition().Inv();
			Vec3d angDiff = qDiff.Rotation();
			if (angDiff.norm() > 0.5*M_PI) {
				stopCount = 0;	//	wait until angle difference is larger than threshold.
				break;
			}
		}
	}
	if (stopCount > 60 / dt) {	//	1分停止したら正常終了
		exit(0);
	}
	//	3分で異常終了
	if (count > 180 / dt) exit(-1);
}
void TreeNodeTest::Keyboard(int key, int x, int y){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	switch (key) {
	case DVKeyCode::ESC:
		case  'q':
			exit(0);
			break;
		default:
			break;
	}
}
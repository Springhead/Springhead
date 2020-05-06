#include "Handler.h"

class LinkHandler : public Handler{
public:
	enum{
		ID_TORQUE_PLUS,
		ID_TORQUE_ZERO,
		ID_TORQUE_MINUS,
		ID_VEL_PLUS,
		ID_VEL_ZERO,
		ID_VEL_MINUS,
		ID_POS_PLUS,
		ID_POS_ZERO,
		ID_POS_MINUS,
		ID_CREATE_PATH,
	};
	vector<PHSolidIf*>		soBox;

	double					spring;
	double					damper;

public:
	LinkHandler(SampleApp* a):Handler(a){
		spring = 30000.0;
		damper = 10000.0;
		
		app->AddAction(MENU_LINK, ID_TORQUE_PLUS, "positive torque");
		app->AddHotKey(MENU_LINK, ID_TORQUE_PLUS, 'a');
		app->AddAction(MENU_LINK, ID_TORQUE_ZERO, "zero torque");
		app->AddHotKey(MENU_LINK, ID_TORQUE_ZERO, 's');
		app->AddAction(MENU_LINK, ID_TORQUE_MINUS, "negative torque");
		app->AddHotKey(MENU_LINK, ID_TORQUE_MINUS, 'd');
		app->AddAction(MENU_LINK, ID_VEL_PLUS, "positive velocity");
		app->AddHotKey(MENU_LINK, ID_VEL_PLUS, 'f');
		app->AddAction(MENU_LINK, ID_VEL_ZERO, "zero velocity");
		app->AddHotKey(MENU_LINK, ID_VEL_ZERO, 'g');
		app->AddAction(MENU_LINK, ID_VEL_MINUS, "negative velocity");
		app->AddHotKey(MENU_LINK, ID_VEL_MINUS, 'y');
		app->AddAction(MENU_LINK, ID_POS_PLUS, "positive position");
		app->AddHotKey(MENU_LINK, ID_POS_PLUS, 'j');
		app->AddAction(MENU_LINK, ID_POS_ZERO, "zero position");
		app->AddHotKey(MENU_LINK, ID_POS_ZERO, 'k');
		app->AddAction(MENU_LINK, ID_POS_MINUS, "negative position");
		app->AddHotKey(MENU_LINK, ID_POS_MINUS, 'l');
		app->AddAction(MENU_LINK, ID_CREATE_PATH, "create path joint");
		app->AddHotKey(MENU_LINK, ID_CREATE_PATH, 'p');
	}
	virtual ~LinkHandler(){}

	virtual void BuildScene(){
		PHSdkIf*	phSdk	= GetPHSdk();
		PHSceneIf*	phScene = GetPHScene();

		soFloor = app->CreateFloor();
		
		CDBoxDesc bd;
		soBox.resize(3);
		bd.boxsize = Vec3d(1.0, 2.0, 1.0);
		soBox[0] = phScene->CreateSolid(sdBox);
		soBox[0]->AddShape(phSdk->CreateShape(bd));
		soBox[0]->SetFramePosition(Vec3d(0.0, 20.0, 0.0));
	
		bd.boxsize = Vec3d(1.0, 5.0, 1.0);
		soBox[1] = phScene->CreateSolid(sdBox);
		soBox[1]->AddShape(phSdk->CreateShape(bd));
		soBox[1]->SetFramePosition(Vec3d(0.0, 20.0, 0.0));

		bd.boxsize = Vec3d(1.0, 10.0, 1.0);
		soBox[2] = phScene->CreateSolid(sdBox);
		soBox[2]->AddShape(phSdk->CreateShape(bd));
		soBox[2]->SetFramePosition(Vec3d(0.0, 20.0, 0.0));

		PHHingeJointDesc jd;
		jntLink.resize(4);
		jd.poseSocket.Pos() = Vec3d(0.0, 10.0, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, -1.0, 0.0);
		jntLink[0] = phScene->CreateJoint(soFloor, soBox[0], jd);

		jd.poseSocket.Pos() = Vec3d(4.0, 10.0, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, -2.5, 0.0);
		jntLink[1] = phScene->CreateJoint(soFloor, soBox[1], jd);

		jd.poseSocket.Pos() = Vec3d(0.0, 1.0, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, -5.0, 0.0);
		jntLink[2] = phScene->CreateJoint(soBox[0], soBox[2], jd);

		jd.poseSocket.Pos() = Vec3d(0.0, 2.5, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, 0.0, 0.0);
		jntLink[3] = phScene->CreateJoint(soBox[1], soBox[2], jd);

		// 以下を有効化するとABAが機能し、閉リンクを構成するための1関節のみLCPで解かれる
		nodeTree.push_back(phScene->CreateRootNode(soFloor, PHRootNodeDesc()));
		nodeTree.push_back(phScene->CreateTreeNode(nodeTree[0], soBox[0], PHTreeNodeDesc()));
		nodeTree.push_back(phScene->CreateTreeNode(nodeTree[1], soBox[2], PHTreeNodeDesc()));
		nodeTree.push_back(phScene->CreateTreeNode(nodeTree[0], soBox[1], PHTreeNodeDesc()));

		phScene->SetContactMode(&soBox[0], 3, PHSceneDesc::MODE_NONE);
		//phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		
		//	最初はABAを無効化	
		app->OnAction(SampleApp::MENU_CONFIG, SampleApp::ID_TOGGLE_ABA);
	}

	virtual void OnAction(int id){
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		PHHingeJointIf* hinge = DCAST(PHHingeJointIf, jntLink[0]);
		//PHPathJointIf* path = (jntLink.size() == 5 ? DCAST(PHPathJointIf, jntLink[4]) : NULL); 

		if(id == ID_TORQUE_PLUS)
			hinge->SetOffsetForce(0.4);
		if(id == ID_TORQUE_ZERO)
			hinge->SetOffsetForce(0.0);
		if(id == ID_TORQUE_MINUS)
			hinge->SetOffsetForce(-0.4);	
		if(	id == ID_TORQUE_PLUS || id == ID_TORQUE_ZERO || id == ID_TORQUE_MINUS ){
			hinge->SetSpring(0.0);
			hinge->SetDamper(0.0);
		}

		if(id == ID_VEL_PLUS)
			hinge->SetTargetVelocity(Rad(180));
		if(id == ID_VEL_ZERO)
			hinge->SetTargetVelocity(Rad(0.0));
		if(id == ID_VEL_MINUS)
			hinge->SetTargetVelocity(Rad(-180.0));
		if(	id == ID_VEL_PLUS || id == ID_VEL_ZERO || id == ID_VEL_MINUS ){
			hinge->SetSpring(0.0);
			hinge->SetDamper(damper);
		}

		if(id == ID_POS_PLUS)
			hinge->SetTargetPosition(1.0);
		if(id == ID_POS_ZERO)
			hinge->SetTargetPosition(0.0);
		if(id == ID_POS_MINUS)
			hinge->SetTargetPosition(-1.0);
		if(id == ID_POS_PLUS || id == ID_POS_ZERO || id == ID_POS_MINUS){
			hinge->SetSpring(spring);
			hinge->SetDamper(damper);
		}

		if(id == ID_CREATE_PATH){
			//チェビシェフリンク一周分の軌跡を記憶させてパスジョイントを作成
			//	2010.12.12 コメントを外してみたところ落ちる。未完？	by hase
			PHPathDesc descPath;
			descPath.bLoop = true;
			PHPathIf* trajectory = phScene->CreatePath(descPath);

			hinge->SetSpring(spring);
			hinge->SetDamper(damper);
			double theta = -Rad(180.0);
			hinge->SetTargetPosition(theta);
			for(int i = 0; i < 50; i++)
				phScene->Step();
			for(; theta < Rad(180.0); theta += Rad(2.0)){
				hinge->SetTargetPosition(theta);
				for(int i = 0; i < 5; i++)
					phScene->Step();
				Posed pose = soFloor->GetPose().Inv() * soBox[2]->GetPose();
				//pose.Ori() = Quaterniond();
				trajectory->AddPoint(theta, pose);
			}
		
			soBox.resize(4);
			soBox[3] = phScene->CreateSolid(sdBox);
			soBox[3]->AddShape(soBox[2]->GetShape(0));
			soBox[3]->SetFramePosition(Vec3d(10.0, 20.0, 0.0));

			PHPathJointDesc descJoint;
			descJoint.posePlug.Pos().x = 15.0;
			jntLink.resize(5);
			jntLink[4] = phScene->CreateJoint(soFloor, soBox[3], descJoint);
			PHPathJointIf* joint = DCAST(PHPathJointIf, jntLink[4]);
			joint->AddChildObject(trajectory);
			joint->SetPosition(0);
		}
	}
};

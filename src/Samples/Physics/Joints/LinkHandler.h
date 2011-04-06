#include "Handler.h"

class LinkHandler : public Handler{
public:
	virtual void Build(){
		BuildCommon(true, false, true);
		
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

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

		Handler::Build();
	}
	virtual void OnKey(int key){
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		const double K = 30.0;
		const double B = 10.0;
		PHHingeJointIf* hinge = DCAST(PHHingeJointIf, jntLink[0]);
		//PHPathJointIf* path = (jntLink.size() == 5 ? DCAST(PHPathJointIf, jntLink[4]) : NULL); 
		switch(key){
		case 'a': hinge->SetMotorTorque(1.0);	break;
		case 's': hinge->SetMotorTorque(0.0);	break;
		case 'd': hinge->SetMotorTorque(-1.0);	break;
		case 'f':
			hinge->SetTargetVelocity(Rad(180));
			//if(path)
			//	path->SetTargetVelocity(Rad(90.0));
			break;
		case 'g':
			hinge->SetTargetVelocity(Rad(0.0));
			//if(path)
			//	path->SetTargetVelocity(Rad(0.0));
			break;
		case 'h': hinge->SetTargetVelocity(Rad(-90.0));	break;
		case 'j':
			hinge->SetSpring(K);
			hinge->SetTargetPosition(1.0);
			hinge->SetDamper(B);
			break;
		case 'k':
			hinge->SetSpring(K);
			hinge->SetTargetPosition(0.0);
			hinge->SetDamper(B);
			break;
		case 'l':
			hinge->SetSpring(K);
			hinge->SetTargetPosition(-1.0);
			hinge->SetDamper(B);
			break;
		case 'c':{
			//チェビシェフリンク一周分の軌跡を記憶させてパスジョイントを作成
			//	2010.12.12 コメントを外してみたところ落ちる。未完？	by hase
			PHPathDesc descPath;
			descPath.bLoop = true;
			PHPathIf* trajectory = phScene->CreatePath(descPath);

			hinge->SetSpring(K);
			hinge->SetDamper(B);
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
		
			}break;
		}
	}
};

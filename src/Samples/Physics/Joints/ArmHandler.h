#include "Handler.h"

class ArmHandler : public Handler{
public:
	/** true	-> 外部で手先をPD制御
		false	-> バネ拘束でPD制御
	 */
	bool bExplicit;

	virtual void Build(){
		BuildCommon(true, false, true);

		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();
		
		CDBoxDesc bd;
		bd.boxsize = Vec3d(2.0, 6.0, 2.0);
		shapeBox = phSdk->CreateShape(bd);
		
		soBox.resize(6);

		soBox[0] = phScene->CreateSolid(sdBox);
		soBox[0]->AddShape(shapeBox);

		soBox[1] = phScene->CreateSolid(sdBox);
		soBox[1]->AddShape(shapeBox);

		soBox[2] = phScene->CreateSolid(sdBox);
		soBox[2]->AddShape(shapeBox);

		soBox[3] = phScene->CreateSolid(sdBox);
		soBox[3]->AddShape(shapeBox);

		soBox[4] = phScene->CreateSolid(sdBox);
		soBox[4]->AddShape(shapeBox);

		CDSphereDesc descSphere;
		descSphere.radius = 1.0;
		soBox[5] = phScene->CreateSolid(sdBox);
		soBox[5]->AddShape(phSdk->CreateShape(descSphere));

		jntLink.resize(6);
		PHHingeJointDesc descHinge;
		descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
		jntLink[0] = phScene->CreateJoint(soFloor, soBox[0], descHinge);

		descHinge.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
		descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
		jntLink[1] = phScene->CreateJoint(soBox[0], soBox[1], descHinge);
		jntLink[2] = phScene->CreateJoint(soBox[1], soBox[2], descHinge);
		jntLink[3] = phScene->CreateJoint(soBox[2], soBox[3], descHinge);
		jntLink[4] = phScene->CreateJoint(soBox[3], soBox[4], descHinge);

		//以下を有効化すると鎖がABAで計算されて関節のドリフトが防がれる．
		/*
		PHTreeNodeIf* node = phScene->CreateRootNode(soFloor, PHRootNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[0], PHTreeNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[1], PHTreeNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[2], PHTreeNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[3], PHTreeNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[4], PHTreeNodeDesc());
		*/

		double K = 2000, D = 100;
		//double K = 100000, D = 10000;	
		DCAST(PHHingeJointIf, jntLink[0])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[0])->SetDamper(D);
		DCAST(PHHingeJointIf, jntLink[1])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[1])->SetDamper(D);
		DCAST(PHHingeJointIf, jntLink[2])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[2])->SetDamper(D);
		DCAST(PHHingeJointIf, jntLink[3])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[3])->SetDamper(D);
		DCAST(PHHingeJointIf, jntLink[4])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[4])->SetDamper(D);

		bExplicit = false;	
		if(!bExplicit){
			PHSpringDesc descSpring;
			descSpring.spring = 500 * Vec3d(1,1,1);
			descSpring.damper = 100 * Vec3d(1,1,1);
			jntLink[5] = phScene->CreateJoint(soBox[4], soBox[5], descSpring);
		}

		soBox[5]->SetFramePosition(Vec3d(10.0, 5.0, 1.0));
		soBox[5]->SetDynamical(false);
		
		phScene->SetContactMode(PHSceneDesc::MODE_NONE);	// 接触を切る

		Handler::Build();
	}

	virtual void OnKey(int key){
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		switch(key){
		/// 目標物体の位置変更
		case 'a': soBox[5]->SetFramePosition(Vec3d(-20.0, 30.0, 0.0)); break;
		case 's': soBox[5]->SetFramePosition(Vec3d(-10.0, 20.0, 0.0)); break;
		case 'd': soBox[5]->SetFramePosition(Vec3d( -5.0, 10.0, 0.0)); break;
		case 'f': soBox[5]->SetFramePosition(Vec3d(  0.0, 10.0, 0.0)); break;
		case 'g': soBox[5]->SetFramePosition(Vec3d(  5.0, 10.0, 0.0)); break;
		case 'h': soBox[5]->SetFramePosition(Vec3d( 10.0, 20.0, 0.0)); break;
		case 'j': soBox[5]->SetFramePosition(Vec3d( 20.0, 30.0, 0.0)); break;
		/*case ' ':{
			//	剛体追加
			soBox.push_back(phScene->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(10.0, 10.0, 0.0));

			//	ジョイント作成
			PHHingeJointDesc descHinge;
			descHinge.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
			descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
			size_t n = soBox.size();
			jntLink.push_back(phScene->CreateJoint(soBox[n-2], soBox[n-1], descHinge));
			DCAST(PHHingeJointIf, jntLink.back())->SetSpring(K6);
			DCAST(PHHingeJointIf, jntLink.back())->SetDamper(D6);

			phScene->SetContactMode(PHSceneDesc::MODE_NONE);	// 接触を切る
			}break;*/
		case 'n':
		//	goal += 0.01;
			for(unsigned i=0; i<jntLink.size(); ++i){
				PHHingeJointIf* j = DCAST(PHHingeJointIf, jntLink[i]);
				//if (j) j->SetTargetPosition(goal);
			}
			break;
		case 'm':
			//goal -= 0.01;
			for(unsigned i=0; i<jntLink.size(); ++i){
				PHHingeJointIf* j = DCAST(PHHingeJointIf, jntLink[i]);
				//if (j) j->SetTargetPosition(goal);
			}
			break;
		}
	}

	virtual void OnTimer(){
		if(bExplicit){
			Vec3d dVel = Vec3d() - soBox[4]->GetVelocity();
			Vec3d dPos = soBox[5]->GetPose().Pos() - soBox[4]->GetPose().Pos();
			static float K = 500;
			static float B = 15;
			Vec3d force = K*dPos + B*dVel;
			soBox[4]->AddForce(force, soBox[4]->GetPose()*Vec3d(0,3,0));
		}
	}
};

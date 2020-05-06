#include "Handler.h"

/// PHBallJointのデモ
class BallJointHandler : public Handler{
public:
	virtual void Build(){
		BuildCommon();

		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		CDBoxDesc bd;
		bd.boxsize = Vec3d(2.0, 6.0, 2.0);
		//bd.boxsize = Vec3f(6.0, 2.0, 2.0);
		shapeBox = phSdk->CreateShape(bd);

		soBox.resize(2);
		soBox[0] = phScene->CreateSolid(sdBox);
		soBox[0]->AddShape(shapeBox);
		soBox[0]->SetDynamical(false);
		soBox[1] = phScene->CreateSolid(sdBox);
		soBox[1]->AddShape(shapeBox);
		phScene->SetContactMode(soBox[0], soBox[1], PHSceneDesc::MODE_NONE);
		phScene->SetGravity(Vec3d());

		jntLink.resize(1);
		PHBallJointDesc desc;
		desc.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
		desc.posePlug.Pos()   = Vec3d(0.0, -3.0, 0.0);
		desc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
		/*
		desc.poseSocket.Pos() = Vec3d(3.0, 0.0, 0.0);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
		desc.posePlug.Pos()   = Vec3d(-3.0, 0.0, 0.0);
		desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
		*/

		desc.spring			  = 1000;
		desc.damper			  = 50;
		desc.limitSwing[0]	  = Rad(  0); // swing lower
		desc.limitSwing[1]	  = Rad( 20); // swing upper
		desc.limitTwist[0]	  = Rad(-20); // twist lower
		desc.limitTwist[1]	  = Rad( 20); // twist upper
		jntLink[0] = phScene->CreateJoint(soBox[0], soBox[1], desc);
		
		Handler::Build();
	}

	virtual void OnKey(int key){
		PHSceneIf* phScene = GetPHScene();
		PHBallJointDesc ballDesc;
		PHBallJointIf* joint = DCAST(PHBallJointIf, phScene->GetJoint(0));
		
		joint->GetDesc(&ballDesc);
		
/*		if(key == 'a')		ballDesc.goal = Quaterniond::Rot(Rad( 30), 'x');
		else if(key == 's') ballDesc.goal = Quaterniond::Rot(Rad(-30), 'x');
		else if(key == 'd') ballDesc.goal = Quaterniond::Rot(Rad( 30), 'z');
		else if(key == 'f') ballDesc.goal = Quaterniond::Rot(Rad( 30), 'z');
		else if(key == 'w') ballDesc.goal = Quaterniond::Rot(Rad( 30), 'y');
		else if(key == 'i') ballDesc.goal = Quaterniond::Rot(Rad(120), 'x');
		else if(key == 'o') ballDesc.goal = Quaterniond::Rot(Rad(120), 'x') * Quaterniond::Rot(Rad(20), 'y');
*/		
		if(key == 'a') soBox[1]->SetVelocity(Vec3d(2.0, 0.0, 0.0));
		else if(key == 's') soBox[1]->SetVelocity(Vec3d(-2.0, 0.0, 0.0));
		else if(key == 'd') soBox[1]->SetVelocity(Vec3d(0.0, 0.0, -2.0));
		else if(key == 'f') soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
		else if(key == 'g') soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
		else if(key == 'h') soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
		else if(key == 'w'){
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
		}
		else if(key == 'e'){
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
		}
		else if(key == 'r'){
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, -2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
		}
		else if(key == 't'){
			soBox[1]->SetVelocity(Vec3d(2.0, 0.0, 0.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
		}
		else if(key == 'y'){
			soBox[1]->SetVelocity(Vec3d(-2.0, 0.0, 0.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
		}
		else if(key == 'u')	soBox[1]->SetAngularVelocity(Vec3d(2.0, 0.0, 0.0));

		joint->SetDesc(&ballDesc);

	}
};

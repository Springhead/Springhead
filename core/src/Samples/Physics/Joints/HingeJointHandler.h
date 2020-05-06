#include "Handler.h"

/// PHHingeJointのデモ
class HingeJointHandler : public Handler{
public:
	double curAngle;

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

		jntLink.resize(1);
		PHHingeJointDesc desc;
		desc.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
		desc.posePlug.Pos()   = Vec3d(0.0, -3.0, 0.0);
		desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
		/*
		desc.poseSocket.Pos() = Vec3d(3.0, 0.0, 0.0);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
		desc.posePlug.Pos()   = Vec3d(-3.0, 0.0, 0.0);
		desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
		*/

		desc.spring			  = 1000;
		desc.damper			  = 10;
		desc.targetPosition	  = Rad(0);
		desc.lower			  = Rad(-60);
		desc.upper			  = Rad( 60);
		jntLink[0] = phScene->CreateJoint(soBox[0], soBox[1], desc);

		curAngle = 0.0;

		Handler::Build();
	}

	virtual void OnKey(int key){
		if(key == 'w')
			curAngle += 10.0;
		else if(key == 'i')
			curAngle -= 10.0;

		// バネ原点（目標角度）を更新
		PHHingeJointIf* hiJoint = DCAST(PHHingeJointIf, jntLink[0]);
		hiJoint->SetTargetPosition(Rad(curAngle));
		
		/*
		// デスクリプタで設定することも可能:
		PHHingeJointDesc hingeDesc;
		GetDesc(&hingeDesc);
		hingeDesc.origin = Rad(curAngle);
		hiJoint->SetDesc(&hingeDesc);
		*/
	}
};

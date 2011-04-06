#include "Handler.h"

class ChainHandler : public Handler{
public:
	virtual void Build(){	
		BuildCommon();
		Handler::Build();
	}
	virtual void OnKey(int key){
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();
		if(key == 'h'){
			soBox.push_back(phScene->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(10, 10, 0));
			PHHingeJointDesc jdesc;
			jdesc.poseSocket.Pos() = Vec3d( 1.1,  -1.1,  0);
			jdesc.posePlug.Pos() = Vec3d(-1.1, 1.1,  0);
			jdesc.lower = Rad(-30.0);
			jdesc.upper = Rad( 30.0);
			jdesc.damper = 2.0;
			size_t n = soBox.size();
			jntLink.push_back(phScene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		}
		else if(key == 'b'){
			soBox.push_back(phScene->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(10.0, 10.0, 0.0));
			PHBallJointDesc jdesc;
			//jdesc.limit[1].upper =  0.2;	// 最大スイング角
			//jdesc.limit[2].lower= -0.2;	// ツイスト角範囲
			//jdesc.limit[2].upper =  0.2;
			jdesc.poseSocket.Pos() = Vec3d(-1.01, -1.01, -1.01);
			jdesc.posePlug.Pos() = Vec3d(1.01, 1.01, 1.01);
			size_t n = soBox.size();
			jntLink.push_back(phScene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
			if(key == ' ')
				nodeTree.push_back(phScene->CreateTreeNode(nodeTree.back(), soBox[n-1], PHTreeNodeDesc()));
			//phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		}
		else if(key == 's'){
			soBox.push_back(phScene->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(10.0, 10.0, 0.0));
			PHSliderJointDesc jdesc;
			jdesc.poseSocket.Pos() = Vec3d(0, -1.1, 0);
			jdesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
			jdesc.posePlug.Pos() = Vec3d(0,  1.1, 0);
			jdesc.posePlug.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
			size_t n = soBox.size();
			jntLink.push_back(phScene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
			PHSliderJointIf* slider = DCAST(PHSliderJointIf, jntLink.back());
			slider->SetRange(-0.3, 0.3);
			//slider->SetSpring(1000.0);
			//slider->SetDamper(300);
			//slider->SetTargetPosition(0.0);
		}
		else if(key == 't'){
			// ツリーノードを作成し，ABAで計算するように指定
			size_t n = soBox.size();
			nodeTree.push_back(phScene->CreateTreeNode(nodeTree.back(), soBox[n-1], PHTreeNodeDesc()));
		}
		else if(key == 'g'){
			// 以下はギアの作成コード
			/*if(jntLink.size() >= 2){
				size_t m = jntLink.size();
				PHGearDesc gdesc;
				gdesc.ratio = 1.0;
				phScene->CreateGear(DCAST(PHJoint1DIf, jntLink[m-2]), DCAST(PHJoint1DIf, jntLink[m-1]), gdesc);
			}*/
		}
	}
};

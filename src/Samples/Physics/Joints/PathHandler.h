#include "Handler.h"

/*
	PathJointのデモだが、誤差補正が上手く行っていないように見える。
	床にぶつかると発散してしまう。	2010.12.12 by hase	*/
class PathHandler : public Handler{
public:
	virtual void Build(){
		BuildCommon();

		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		PHPathDesc desc;
		PHPathIf* path = phScene->CreatePath(desc);
		double s;
		double radius = 5.0;
		double pitch = 4.0;
		Posed pose;
		for(s = 0.0; s < 4 * (2 * M_PI); s += Rad(1.0)){
			double stmp = s;
			while(stmp > M_PI) stmp -= 2 * M_PI;
			pose.Pos() = Vec3d(radius * cos(stmp), 5.0 + pitch * s / (2 * M_PI), radius * sin(stmp));
			pose.Ori().FromMatrix(Matrix3d::Rot(-stmp, 'y'));
			path->AddPoint(s, pose);
		}
		soBox[0]->SetDynamical(true);
		PHPathJointDesc descJoint;
		jntLink.push_back(phScene->CreateJoint(soFloor, soBox[0], descJoint));
		PHPathJointIf* joint = DCAST(PHPathJointIf, jntLink[0]);
		joint->AddChildObject(path);
		joint->SetPosition(2 * 2 * M_PI);
		PHTreeNodeIf* node = phScene->CreateRootNode(soFloor, PHRootNodeDesc());
		phScene->CreateTreeNode(node, soBox[0], PHTreeNodeDesc());

		Handler::Build();
	}
};

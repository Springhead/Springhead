#include "Handler.h"

class ArmHandler : public Handler{
public:
	enum{
		ID_MOVE_BALL_0,
		ID_MOVE_BALL_1,
		ID_MOVE_BALL_2,
		ID_MOVE_BALL_3,
		ID_MOVE_BALL_4,
		ID_MOVE_BALL_5,
		ID_MOVE_BALL_6,
	};

	CDShapeIf*	shapeBox;
	CDShapeIf*	shapeSphere;

	vector<PHSolidIf*>			links;
	vector<PHHingeJointIf*>		joints;
	PHSolidIf*					ball;
	PHSpringIf*					spring;

	int							nLinks;			///< リンクの数
	double						linkSize;		///< リンクの長さ
	double						jointSpring;	///< 関節のバネ係数
	double						jointDamper;	///< 関節のダンパ係数
	double						handSpring;		///< 手先のバネ係数
	double						handDamper;		///< 手先のダンパ係数
	bool						useExternal;	///< 外部コントローラで手先制御を行うかどうか
	bool						useABA;			///< ABAを使うかどうか

public:
	ArmHandler(SampleApp* a):Handler(a){
		nLinks		= 5;
		linkSize	= 5.0;

		jointSpring = 500;
		jointDamper = 100;
		handSpring	= 100;
		handDamper  =  20;

		useExternal	= false;
		useABA		= true;

		app->AddAction(MENU_ARM, ID_MOVE_BALL_0, "move ball");
		app->AddHotKey(MENU_ARM, ID_MOVE_BALL_0, 'z');
		app->AddAction(MENU_ARM, ID_MOVE_BALL_1, "move ball");
		app->AddHotKey(MENU_ARM, ID_MOVE_BALL_1, 'x');
		app->AddAction(MENU_ARM, ID_MOVE_BALL_2, "move ball");
		app->AddHotKey(MENU_ARM, ID_MOVE_BALL_2, 'c');
		app->AddAction(MENU_ARM, ID_MOVE_BALL_3, "move ball");
		app->AddHotKey(MENU_ARM, ID_MOVE_BALL_3, 'v');
		app->AddAction(MENU_ARM, ID_MOVE_BALL_4, "move ball");
		app->AddHotKey(MENU_ARM, ID_MOVE_BALL_4, 'b');
		app->AddAction(MENU_ARM, ID_MOVE_BALL_5, "move ball");
		app->AddHotKey(MENU_ARM, ID_MOVE_BALL_5, 'n');
	}

	virtual void BuildScene(){
		soFloor = app->CreateFloor(true);
		soFloor->SetFramePosition(Vec3d(0,-30,0));
		
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();
		
		// 形状作成
		CDBoxDesc bd;
		bd.boxsize = Vec3d(2.0, 6.0, 2.0);
		shapeBox = phSdk->CreateShape(bd);

		CDSphereDesc descSphere;
		descSphere.radius = 1.0;
		shapeSphere = phSdk->CreateShape(descSphere);
		
		// ロボットアームを構成する剛体
		links.resize(nLinks);
		for(int i = 0; i < nLinks; i++){
			links[i] = phScene->CreateSolid();
			links[i]->AddShape(shapeBox);
		}
		
		// リンクを関節でつなぐ
		joints.resize(nLinks);
		PHHingeJointDesc descHinge;
		descHinge.poseSocket.Pos()	= Vec3d(0.0,  0.5 * linkSize, 0.0);
		descHinge.posePlug.Pos()	= Vec3d(0.0, -0.5 * linkSize, 0.0);
		joints[0] = DCAST(PHHingeJointIf, phScene->CreateJoint(soFloor, links[0], descHinge));

		for(int i = 1; i < nLinks; i++){
			joints[i] = DCAST(PHHingeJointIf, phScene->CreateJoint(links[i-1], links[i], descHinge));
			joints[i]->SetSpring(jointSpring);
			joints[i]->SetDamper(jointDamper);
		}
#if 0
		if(useABA){
			PHTreeNodeIf* node = phScene->CreateRootNode(links[0], PHRootNodeDesc());
			for(int i = 1; i < nLinks; i++)
				node = phScene->CreateTreeNode(node, links[i], PHTreeNodeDesc());
		}
#endif

		// ボール（目標物体)
		ball = phScene->CreateSolid();
		ball->AddShape(shapeSphere);
		ball->SetFramePosition(Vec3d(10.0, 5.0, 1.0));
		ball->SetDynamical(false);		
	
		// 手先とボールをつなぐバネ
		if(!useExternal){
			PHSpringDesc descSpring;
			descSpring.spring = handSpring * Vec3d(1,1,1);
			descSpring.damper = handDamper * Vec3d(1,1,1);
			spring = DCAST(PHSpringIf, phScene->CreateJoint(links.back(), ball, descSpring));
		}
		links.front()->SetDynamical(false);

		phScene->SetContactMode(PHSceneDesc::MODE_NONE);		// 接触を切る
		phScene->SetNumIteration(1);
	}

	virtual void OnAction(int id){
		if(id == ID_MOVE_BALL_0)
			ball->SetFramePosition(Vec3d(-20.0, 30.0, 0.0));
		if(id == ID_MOVE_BALL_1)
			ball->SetFramePosition(Vec3d(-10.0, 20.0, 0.0));
		if(id == ID_MOVE_BALL_2)
			ball->SetFramePosition(Vec3d( -5.0, 10.0, 0.0));
		if(id == ID_MOVE_BALL_3)
			ball->SetFramePosition(Vec3d(  0.0, 10.0, 0.0));
		if(id == ID_MOVE_BALL_4)
			ball->SetFramePosition(Vec3d(  5.0, 10.0, 0.0));
		if(id == ID_MOVE_BALL_5)
			ball->SetFramePosition(Vec3d( 10.0, 20.0, 0.0));
		if(id == ID_MOVE_BALL_6)
			ball->SetFramePosition(Vec3d( 20.0, 30.0, 0.0));
	}

	virtual void OnTimer(){
		// 手先コントローラ（PD制御）
		if(useExternal){
			// 相対速度
			Vec3d dVel = Vec3d() - links.back()->GetVelocity();
			// 位置偏差
			Vec3d dPos = ball->GetPose().Pos() - links.back()->GetPose().Pos();
			Vec3d force = handSpring*dPos + handDamper*dVel;
			links.back()->AddForce(force, soBox[4]->GetPose()*Vec3d(0,3,0));
		}
	}
};

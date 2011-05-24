#include <vector>

#include "../../SampleApp.h"

class MyApp : public SampleApp{
public:
	enum {
		ID_TOGGLE_IK,
		ID_MOVE_TARGET0,
		ID_MOVE_TARGET1,
		ID_MOVE_TARGET2,
		ID_MOVE_TARGET3,
		ID_MOVE_TARGET4,
		ID_MOVE_TARGET5,
	};

	struct BJOffset {
		PHBallJointIf* jo;
		Vec3d          offset;
		double         damper;
		Vec3d          vel;
		Quaterniond    goal;
	};

	CDShapeIf*						shapeBase;
	CDShapeIf*						shapeLink;
	CDShapeIf*						shapeTarget;

	vector<PHSolidIf*>				links;
	PHSolidIf*						target;
	vector<PHBallJointIf*>			joints;
	vector<PHIKBallActuatorIf*>		actuators;
	//vector<PHIKEndEffectorIf*>		endEffectors;
	PHIKEndEffectorIf*				endEffector;

	PHSolidDesc				descSolid;
	CDBoxDesc				descBox;
	PHBallJointDesc			descBallJoint;
	PHIKBallActuatorDesc	descIKBall;
	PHIKEndEffectorDesc		descIKEE;

	bool	bStep;
	bool	bOneStep;
	bool	bIK;
	int		numLinks;

public:
	MyApp(){
		bStep		= true;
		bOneStep	= false;
		bIK			= true;

		appName = "Virtual Trajectory Tracking";
		numLinks	= 2;
		numScenes   = 1;

		AddMenu(MENU_SCENE, "< scene settings >");

		AddAction(MENU_SCENE, ID_TOGGLE_IK, "enable/disable IK");
		AddHotKey(MENU_SCENE, ID_TOGGLE_IK, 'p');

		AddAction(MENU_SCENE, ID_MOVE_TARGET0, "move left");
		AddHotKey(MENU_SCENE, ID_MOVE_TARGET0, 'j');

		AddAction(MENU_SCENE, ID_MOVE_TARGET1, "move right");
		AddHotKey(MENU_SCENE, ID_MOVE_TARGET1, 'l');

		AddAction(MENU_SCENE, ID_MOVE_TARGET2, "move upward");
		AddHotKey(MENU_SCENE, ID_MOVE_TARGET2, 'i');

		AddAction(MENU_SCENE, ID_MOVE_TARGET3, "move downward");
		AddHotKey(MENU_SCENE, ID_MOVE_TARGET3, 'k');

		AddAction(MENU_SCENE, ID_MOVE_TARGET4, "move foreward");
		AddHotKey(MENU_SCENE, ID_MOVE_TARGET4, 'u');

		AddAction(MENU_SCENE, ID_MOVE_TARGET5, "move backward");
		AddHotKey(MENU_SCENE, ID_MOVE_TARGET5, 'o');
	}

	void CalcOffsetForce(){
		PHIKEngineIf* ikEngine = phScene->GetIKEngine();

		states->SaveState(phScene);
		phScene->GetIKEngine()->Enable(true);
		phScene->Step();
		
		typedef std::vector<BJOffset> BJOffsets;
		BJOffsets bjOffsets;
	
		for(size_t i = 0; i < phScene->NIKActuators(); ++i) {
			PHIKBallActuatorIf* ac = phScene->GetIKActuator(i)->Cast();
			if(!ac)
				continue;
			
			bjOffsets.resize(bjOffsets.size()+1);
			bjOffsets.back().jo = ac->GetJoint()->Cast();
			bjOffsets.back().vel = ac->GetJoint()->GetTargetVelocity();
			bjOffsets.back().goal = ac->GetJoint()->GetTargetPosition();

			if (bjOffsets.back().vel.norm() > Rad(360)) {
				bjOffsets.back().vel = bjOffsets.back().vel.unit() * Rad(360);
			}
		}

		states->LoadState(phScene);

		for (size_t i=0; i<bjOffsets.size(); ++i) {
			bjOffsets[i].damper = bjOffsets[i].jo->GetDamper();
			bjOffsets[i].jo->SetDamper(DBL_MAX);
			bjOffsets[i].jo->SetTargetVelocity(bjOffsets[i].vel);
		}

		phScene->GetIKEngine()->Enable(false);

		phScene->Step();

		for (size_t i=0; i<bjOffsets.size(); ++i) {
			bjOffsets[i].offset = bjOffsets[i].jo->GetOffsetForce();
		}

		states->LoadState(phScene);

		for (size_t i=0; i<bjOffsets.size(); ++i) {
			bjOffsets[i].jo->SetDamper(bjOffsets[i].damper);
			bjOffsets[i].jo->SetTargetPosition(bjOffsets[i].goal);
			bjOffsets[i].jo->SetTargetVelocity(bjOffsets[i].vel);
			bjOffsets[i].jo->SetOffsetForce(bjOffsets[i].offset);
		}
	}

	virtual void BuildScene(){
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();

		// ボールジョイント２つの直線状リンク
		descBallJoint.spring =   0.01;
		descBallJoint.damper = 400.0;
		descIKBall.spring = 0.1f;
		descIKBall.damper = 0.001f;
		
		descBallJoint.spring =   0.01f;
		descBallJoint.damper =   400.0f;
		descIKBall.spring = 0.1f;
		descIKBall.damper = 0.001f;
		
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		shapeBase = phSdk->CreateShape(descBox);
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		shapeLink = phSdk->CreateShape(descBox);
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		shapeTarget = phSdk->CreateShape(descBox);
		
		/// -- ルート剛体
		///// 剛体
		descSolid.dynamical = false;
		descSolid.mass		= 1.0;
		PHSolidIf* base = phScene->CreateSolid(descSolid);
		base->AddShape(shapeBase);
		links.push_back(base);

		descSolid.dynamical = true;
		descSolid.mass = 1.0;
			
		for(int i = 0; i < numLinks; i++){
			PHSolidIf* so = phScene->CreateSolid(descSolid);
			so->AddShape(shapeLink);
			
			descBallJoint.poseSocket.Pos() = Vec3d(0.0, (i == 0 ? 0.1 : 0.5), 0.0);
			descBallJoint.posePlug.Pos()   = Vec3d(0.0, -0.5, 0.0);
			descBallJoint.spring =   1.0f;
			descBallJoint.damper =   0.1f;
			PHBallJointIf* bj = phScene->CreateJoint(links.back(), so, descBallJoint)->Cast();

			///// IKノード
			descIKBall.spring =   100.0f;
			descIKBall.damper =     1.0f;
			descIKBall.bias   =     1.0f;
			descIKBall.bEnabled = true;

			PHIKBallActuatorIf* ac = phScene->CreateIKActuator(descIKBall)->Cast();
			ac->SetJoint(bj);

			links.push_back(so);
			joints.push_back(bj);
			actuators.push_back(ac);
		}

		/// -- IK制御点
		///// 制御点の作成
		descIKEE.targetPosition = Vec3d(0.0, 0.5, 0.0);
		descIKEE.bEnabled = true;
		descIKEE.bPosition = true;
		endEffector = phScene->CreateIKEndEffector(descIKEE);
		endEffector->SetSolid(links.back());
		///// ノードへの登録
		for(int i = 0; i < (int)actuators.size(); i++)
			actuators[i]->RegisterEndEffector(endEffector);
		
		///// 制御点を指し示す剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		target = phScene->CreateSolid(descSolid);
		target->AddShape(shapeTarget);

		// Sceneの設定
		/// IKの設定
		phScene->GetIKEngine()->SetNumIter(15);
		phScene->GetIKEngine()->Enable(bIK);
		
		/// その他の設定
		phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		phScene->SetGravity(Vec3d(0.0, -9.8, 0.0));
		phScene->SetTimeStep(0.01);
		phScene->SetNumIteration(15);

	}

	virtual void OnStep(){
		CalcOffsetForce();
		phScene->Step();

		target->SetFramePosition(endEffector->GetTargetPosition());
	}

	void MoveTarget(Vec3d rel){
		Vec3d goal = endEffector->GetTargetPosition();
		goal += rel;
		endEffector->SetTargetPosition(goal);
		target->SetFramePosition(goal);

		DSTR << "Move to " << goal << std::endl;
	}

	virtual void OnAction(int menu, int id){
		SampleApp::OnAction(menu, id);

		if(menu == MENU_SCENE){
			if(id == ID_TOGGLE_IK){
				bIK = !bIK;
				DSTR << (bIK ? "IK ON" : "IK OFF") << endl;
				phScene->GetIKEngine()->Enable(bIK);
			}
			if(id == ID_MOVE_TARGET0){
				MoveTarget(Vec3d(0.2, 0.0, 0.0));
			}
			if(id == ID_MOVE_TARGET1){
				MoveTarget(Vec3d(-0.2, 0.0, 0.0));
			}
			if(id == ID_MOVE_TARGET2){
				MoveTarget(Vec3d(0.0, 0.2, 0.0));
			}
			if(id == ID_MOVE_TARGET3){
				MoveTarget(Vec3d(0.0, -0.2, 0.0));
			}
			if(id == ID_MOVE_TARGET4){
				MoveTarget(Vec3d(0.0, 0.0, 0.2));
			}
			if(id == ID_MOVE_TARGET5){
				MoveTarget(Vec3d(0.0, 0.0, -0.2));
			}
			/*case 'L':
				{
					Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
					relGoal += Vec3f(+0.2f,  0.0f,  0.0f);
					ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
				}
				break;

			case 'J':
				{
					Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
					relGoal += Vec3f(-0.2f,  0.0f,  0.0f);
					ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
				}
				break;

			case 'I':
				{
					Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
					relGoal += Vec3f( 0.0f, +0.2f,  0.0f);
					ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
				}
				break;

			case 'K':
				{
					Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
					relGoal += Vec3f( 0.0f, -0.2f,  0.0f);
					ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
				}
				break;

			case 'U':
				{
					Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
					relGoal += Vec3f( 0.0f,  0.0f, +0.2f);
					ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
				}
				break;

			case 'O':
				{
					Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
					relGoal += Vec3f( 0.0f,  0.0f, -0.2f);
					ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
				}
				break;
				*/
		}
	}

} app; 

int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

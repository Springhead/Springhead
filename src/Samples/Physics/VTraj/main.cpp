#include <vector>

#include "../../SampleApp.h"

class MyApp : public SampleApp{
public:
	enum {
		ID_TOGGLE_IK,
		ID_TOGGLE_VT,
		ID_MOVE_TARGET0,
		ID_MOVE_TARGET1,
		ID_MOVE_TARGET2,
		ID_MOVE_TARGET3,
		ID_MOVE_TARGET4,
		ID_MOVE_TARGET5,
		ID_DROP_OBJECT,
	};

	struct BJOffset {
		PHBallJointIf* jo;
		Vec3d          offset;
		double         damper;
		Vec3d          vel;
		Quaterniond    goal;
	};

	vector<PHSolidIf*>				links;
	PHSolidIf*						target;
	vector<PHBallJointIf*>			joints;
	vector<PHIKBallActuatorIf*>		actuators;
	PHIKEndEffectorIf*				endEffector;

	bool	bStep;
	bool	bOneStep;
	bool	bIK;
	bool	bVT;
	int		numLinks;

public:
	MyApp(){
		bStep		= true;
		bOneStep	= false;
		bIK			= false;
		bVT			= true;

		appName = "IK and Virtual Trajectory Tracking Sample";
		numLinks	= 2;
		numScenes   = 1;

		AddMenu(MENU_SCENE, "< scene settings >");

		AddAction(MENU_SCENE, ID_TOGGLE_IK, "enable/disable IK");
		AddHotKey(MENU_SCENE, ID_TOGGLE_IK, 'p');

		AddAction(MENU_SCENE, ID_TOGGLE_VT, "enable/disable Trajectory Tracking");
		AddHotKey(MENU_SCENE, ID_TOGGLE_VT, 'v');

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

		AddAction(MENU_SCENE, ID_DROP_OBJECT,  "drop object");
		AddHotKey(MENU_SCENE, ID_DROP_OBJECT,  'n');
	}

	void CalcOffsetForce(){
		states->SaveState(phScene);
		for (int i=0; i<phScene->NJoints(); ++i) {
			PHBallJointIf* jo = phScene->GetJoint(i)->Cast();
			jo->SetDamper(DBL_MAX);
		}
		phScene->Step();
		states->LoadState(phScene);
		for (int i=0; i<phScene->NJoints(); ++i) {
			PHBallJointIf* jo = phScene->GetJoint(i)->Cast();
            // jo->SetTargetPosition(Quaterniond());
			// jo->SetTargetVelocity(Vec3d());
			// jo->SetSpring(0);
			jo->SetDamper(0);
		}

		return;





		std::vector<double> dampers;
		dampers.resize(phScene->NJoints());

		std::vector<Quaterniond> goals;
		goals.resize(phScene->NJoints());

		std::vector<Vec3d> vels;
		vels.resize(phScene->NJoints());

		states->SaveState(phScene);

		// ---

		// phScene->GetIKEngine()->Enable(true);
		// phScene->Step();

		// for (size_t i=0; i<dampers.size(); ++i) {
		// 	PHBallJointIf* jo = phScene->GetJoint(i)->Cast();
		// 	goals[i] = jo->GetTargetPosition();
		// }

		// states->LoadState(phScene);

		// ---

		for (size_t i=0; i<dampers.size(); ++i) {
			PHBallJointIf* jo = phScene->GetJoint(i)->Cast();
			dampers[i] = jo->GetDamper();
			// vels[i] = jo->GetTargetVelocity();
			jo->SetDamper(DBL_MAX);
		}

		// phScene->GetIKEngine()->Enable(false);

		phScene->Step();

		states->LoadState(phScene);

		// ---

		for (size_t i=0; i<dampers.size(); ++i) {
			PHBallJointIf* jo = phScene->GetJoint(i)->Cast();
			// jo->SetTargetPosition(goals[i]);
			// jo->SetTargetVelocity(vels[i]);
            jo->SetTargetPosition(Quaterniond());
			jo->SetTargetVelocity(Vec3d());
			jo->SetSpring(0);
			jo->SetDamper(dampers[i]);
		}

		return;




		///------------------------------------------------------------

		PHIKEngineIf* ikEngine = phScene->GetIKEngine();

		states->SaveState(phScene);
		phScene->GetIKEngine()->Enable(true);
		phScene->Step();
		
		typedef std::vector<BJOffset> BJOffsets;
		BJOffsets bjOffsets;
	
		for(int i = 0; i < phScene->NIKActuators(); ++i) {
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
			// bjOffsets[i].jo->SetTargetVelocity(bjOffsets[i].vel);
		}

		phScene->GetIKEngine()->Enable(false);

		phScene->Step();

		/*
		for (size_t i=0; i<bjOffsets.size(); ++i) {
			bjOffsets[i].offset = bjOffsets[i].jo->GetOffsetForce();
		}
		*/

		states->LoadState(phScene);

		for (size_t i=0; i<bjOffsets.size(); ++i) {
			bjOffsets[i].jo->SetDamper(bjOffsets[i].damper);
			bjOffsets[i].jo->SetTargetPosition(bjOffsets[i].goal);
			// bjOffsets[i].jo->SetTargetVelocity(bjOffsets[i].vel);
			// bjOffsets[i].jo->SetTargetVelocity(Vec3d(0,0,0));
			/*
			Vec3d offset = bjOffsets[i].offset;
			std::cout << offset << std::endl;
			if (offset.norm() > 3000.0) {
				offset = offset.unit() * 3000.0;
			}
			*/
			// bjOffsets[i].jo->SetOffsetForce(offset);
		}
	}

	virtual void BuildScene(){
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();

		///// 形状
		CDBoxDesc descBox;

		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		CDShapeIf* shapeBase = phSdk->CreateShape(descBox);

		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		CDShapeIf* shapeLink = phSdk->CreateShape(descBox);

		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		CDShapeIf* shapeTarget = phSdk->CreateShape(descBox);
		
		///// 剛体・関節
		PHSolidDesc descSolid;
		PHBallJointDesc			descBallJoint;
		PHIKBallActuatorDesc	descIKBall;

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

			///// 関節
			descBallJoint.poseSocket.Pos() = Vec3d(0.0, (i == 0 ? 0.1 : 0.5), 0.0);
			descBallJoint.posePlug.Pos()   = Vec3d(0.0, -0.5, 0.0);
			descBallJoint.spring = 10.0f;
			descBallJoint.damper = 5.0f;
			PHBallJointIf* bj = phScene->CreateJoint(links.back(), so, descBallJoint)->Cast();

			///// IK関節（アクチュエータ）
			descIKBall.bias   =     1.0f;

			PHIKBallActuatorIf* ac = phScene->CreateIKActuator(descIKBall)->Cast();
			ac->SetJoint(bj);

			links.push_back(so);
			joints.push_back(bj);
			actuators.push_back(ac);
		}

		///// IK制御点
		PHIKEndEffectorDesc		descIKEE;

		///// -- 制御点の作成
		descIKEE.targetPosition = Vec3d(0.0, 0.5, 0.0);
		endEffector = phScene->CreateIKEndEffector(descIKEE);
		endEffector->SetSolid(links.back());
		///// -- ノードへの登録
		for(int i = 0; i < (int)actuators.size(); i++) {
			actuators[i]->AddChildObject(endEffector);
		}
		
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
		for (unsigned i=0; i<links.size(); ++i) {
			for (unsigned j=0; j<links.size(); ++j) {
				if (i!=j) {
					phScene->SetContactMode(links[i], links[j], PHSceneDesc::MODE_NONE);
				}
			}
			phScene->SetContactMode(links[i], target, PHSceneDesc::MODE_NONE);
		}
		// phScene->SetGravity(Vec3d(0.0, -9.8, 0.0));
		phScene->SetGravity(Vec3d(0.0, 0.0, 0.0));
		phScene->SetTimeStep(0.01);
		phScene->SetNumIteration(15);
	}

	virtual void OnStep(){
		if (bVT) {
			CalcOffsetForce();
		} else {
			for (int i=0; i<phScene->NJoints(); ++i) {
				PHBallJointIf* jo = phScene->GetJoint(i)->Cast();
				jo->SetOffsetForce(Vec3d(0,0,0));
			}
		}
		phScene->Step();

		target->SetFramePosition(endEffector->GetTargetPosition());
	}

	void MoveTarget(Vec3d rel){
		Vec3d goal = endEffector->GetTargetPosition();
		goal += rel;
		endEffector->SetTargetPosition(goal);
		target->SetFramePosition(goal);
	}

	virtual void OnAction(int menu, int id){
		SampleApp::OnAction(menu, id);

		if(menu == MENU_SCENE){
			if(id == ID_TOGGLE_IK){
				bIK = !bIK;
				DSTR << (bIK ? "IK ON" : "IK OFF") << endl;
				phScene->GetIKEngine()->Enable(bIK);
			}
			if(id == ID_TOGGLE_VT){
				bVT = !bVT;
				DSTR << (bVT ? "VT ON" : "VT OFF") << endl;
			}
			if(id == ID_MOVE_TARGET0){
				MoveTarget(Vec3d(-0.2, 0.0, 0.0));
			}
			if(id == ID_MOVE_TARGET1){
				MoveTarget(Vec3d(0.2, 0.0, 0.0));
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
			if(id == ID_DROP_OBJECT){
				Drop(SHAPE_SPHERE, 0, Vec3d(), Vec3d(), Vec3d(8,0,0), Quaterniond());
				PHSolidIf* so = phScene->GetSolids()[phScene->NSolids() - 1];
				so->SetMass(5.0);
				so->SetVelocity(Vec3d(-8,2,0));
				phScene->SetContactMode(so, target, PHSceneDesc::MODE_NONE);
			}
		}
	}

} app; 

int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

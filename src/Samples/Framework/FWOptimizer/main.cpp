#include <vector>
#include "../../SampleApp.h"
#include <Framework/SprFWOptimizer.h>

#include "windows.h"

#pragma  hdrstop

using namespace Spr;
using namespace std;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 軌道クラス
class MinimumJerkTrajectory {
	double x, t0, dur;
public:
	MinimumJerkTrajectory() : x(0), t0(0), dur(0) {}
	MinimumJerkTrajectory(double x_, double t0_, double dur_) : x(x_), t0(t0_), dur(dur_) {}
	void Set(double x_, double t0_, double dur_) { x = x_; t0 = t0_; dur = dur_; }
	double At(double t) {
		if (dur < 1e-10) { return x; }
		double t1 = t0 + dur;
		if (t < t0) {
			return 0;
		} else if (t1 < t) {
			return x;
		} else {
			double s = (t - t0) / (t1 - t0);
			double s3 = s*s*s, s4 = s3*s, s5 = s4*s;
			return x * (10 * s3 - 15 * s4 + 6 * s5);
		}
	}
};

template <size_t NSUB>
class JointTrajectory {
public:
	MinimumJerkTrajectory submovements[NSUB];
	JointTrajectory() {}
	JointTrajectory(double const *params) {
		Set(params);
	}
	void Set(double const *params) {
		for (int i = 0; i < NSUB; ++i) {
			submovements[i].Set(params[0], params[1], params[2]);
			params += 3;
		}
	}
	double At(double t) {
		double x=0;
		for (int i = 0; i < NSUB; ++i) {
			x += submovements[i].At(t);
		}
		return x;
	}
	Vec3f ToScreen(Vec2f v, double offset = 0) {
		return Vec3f(v.x * 200 + 50, -v.y * 100 + 500 + offset, 0);
	}
	void Draw(GRRenderIf* render, double t0, double t1, double offset = 0) {
		int slice = 100;
		double last = 0;
		for (int i = 0; i < slice; ++i) {
			double dt = (t1 - t0) / slice;
			double t = t0 + dt * i;
			double x = At(t);
			render->DrawLine(ToScreen(Vec2f(t - dt, last), offset), ToScreen(Vec2f(t, x), offset));
			last = x;
		}
	}
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 最適化クラス
template <size_t NSUB>
class Optimizer : public FWOptimizer {
public:
	static const int nJoints = 2;

	JointTrajectory<NSUB> jt[nJoints];

	void Init() {
		FWOptimizer::Init(nJoints * NSUB * 5);
	}

	virtual double Objective(double const *x, int n) {
		PHHingeJointIf* jo[nJoints];
		for (int i = 0; i < nJoints; ++i) {
			jo[i] = fwScene->GetPHScene()->GetJoint(i)->Cast();
		}

		double obj = 0;

		// 1. Apply x to Scene
		for (int i = 0; i < nJoints; ++i) {
			jt[i].Set(&(x[i * NSUB * 5]));

			double spring = (x[i * NSUB * 5 + 3]);
			double damper = (x[i * NSUB * 5 + 4]);
			jo[i]->SetSpring(abs(spring) * 100 + 100);
			jo[i]->SetDamper(abs(damper) * 10 + 10);

			obj += 1e+3*abs(spring) + 1e+3*abs(damper);
		}

		double lastTorque[nJoints]; for (int i = 0; i < nJoints; ++i) { lastTorque[i] = 0; }
		for (int step = 0; step < 100; ++step) {
			// 2. Do Simulation Step
			double t = fwScene->GetPHScene()->GetCount() * fwScene->GetPHScene()->GetTimeStep();

			for (int i = 0; i < nJoints; ++i) {
				jo[i]->SetTargetPosition(jt[i].At(t));
			}

			fwScene->Step();

			// 3. Calc Criterion and Sum up
			for (int i = 0; i < nJoints; ++i) {
				double torque = jo[i]->GetMotorForce();
				double dtorque = torque - lastTorque[i];
				lastTorque[i] = torque;

				obj += (dtorque*dtorque) + (torque*torque);
			}
		}

		// 4. Calc Criterion for Final State
		for (int step = 100; step < 150; ++step) {
			fwScene->Step();

			PHIKEndEffectorIf* eef = fwScene->GetPHScene()->GetIKEndEffector(0);
			double error = ((eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition()) - eef->GetTargetPosition()).norm();

			obj += 1e+4 * abs(error);

			obj += 1e+3 * eef->GetSolid()->GetVelocity().norm();
			obj += 1e+3 * eef->GetSolid()->GetAngularVelocity().norm();
		}

		return obj;
	}
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// アプリケーションクラス
class FWOptimizerSample : public SampleApp{
public:
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_RESET,
	};

	int argc;
	char** argv;

	static const int nsub = 1;
	//Optimizer<nsub> optimizer;
	//FWOptimizer optimizer;
	FWStaticTorqueOptimizer optimizer;
	JointTrajectory<nsub> jt[2];

	ObjectStatesIf *states_;

	bool bStarted;

	FWOptimizerSample(){
		appName = "FW Optimizer Sample";
		numScenes = 1;
		bStarted = false;
	}
	~FWOptimizerSample(){}

	// 初期化
	virtual void Init(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;

		SampleApp::Init(argc, argv);
		ToggleAction(MENU_ALWAYS, ID_RUN);
		curScene = 0;

		GetFWScene()->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);

		AddAction(MENU_SCENE, ID_RESET, "reset", "reset done.");
		AddHotKey(MENU_SCENE, ID_RESET, 'r');
	}

	PHSolidIf         *soTarget;
	PHIKEndEffectorIf *ikeTarget;
	PHHingeJointIf    *jo1, *jo2, *jo3, *jo4;

	// シーン構築
	virtual void BuildScene() {
		/**/
		PHSdkIf* phSdk = GetFWScene()->GetPHScene()->GetSdk();

		PHSolidDesc descSolid;

		CDRoundConeDesc descCapsule;
		descCapsule.radius = Vec2f(0.3, 0.3);
		descCapsule.length = 4;

		CDSphereDesc descSphere;
		descSphere.radius  = 0.5;

		CDBoxDesc descBox;
		descBox.boxsize = Vec3d(1.0, 1.0, 1.0);

		Posed shapePose; shapePose.Ori() = Quaterniond::Rot(Rad(90), 'x');

		// Base Link
		PHSolidIf* so0 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so0->SetDynamical(false);
		so0->AddShape(phSdk->CreateShape(descCapsule));
		so0->SetShapePose(0, shapePose);

		// Link 1
		PHSolidIf* so1 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so1->SetFramePosition(Vec3d(0,-4,0));
		so1->AddShape(phSdk->CreateShape(descCapsule));
		so1->SetShapePose(0, shapePose);

		// Link 2
		PHSolidIf* so2 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so2->SetFramePosition(Vec3d(0,-8,0));
		so2->AddShape(phSdk->CreateShape(descCapsule));
		so2->SetShapePose(0, shapePose);

		// Link 3
		PHSolidIf* so3 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so3->SetFramePosition(Vec3d(0, -12, 0));
		so3->AddShape(phSdk->CreateShape(descCapsule));
		so3->SetShapePose(0, shapePose);

		// Link 4
		PHSolidIf* so4 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so4->SetFramePosition(Vec3d(0, -16, 0));
		so4->AddShape(phSdk->CreateShape(descCapsule));
		so4->SetShapePose(0, shapePose);

		// Pointer
		PHSolidIf* so5 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so5->SetFramePosition(Vec3d(6,0,0));
		so5->AddShape(phSdk->CreateShape(descSphere));
		so5->SetDynamical(false);

		soTarget = so5;

		// ----- ----- ----- ----- -----

		PHHingeJointDesc descJoint;
		descJoint.poseSocket = Posed(1,0,0,0, 0,-2,0);
		descJoint.posePlug   = Posed(1,0,0,0, 0, 2,0);
		descJoint.spring =   100.0;
		descJoint.damper =    10.0;
		descJoint.cyclic =     true;
		// descJoint.fMax   =   2000.0;

		PHIKHingeActuatorDesc descIKA;
		PHIKEndEffectorDesc   descIKE;

		// Base <-> Link 1
		jo1 = GetFWScene()->GetPHScene()->CreateJoint(so0, so1, descJoint)->Cast();
		PHIKHingeActuatorIf* ika1 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKA)->Cast();
		ika1->AddChildObject(jo1);

		// Link 1 <-> Link 2
		jo2 = GetFWScene()->GetPHScene()->CreateJoint(so1, so2, descJoint)->Cast();
		PHIKHingeActuatorIf* ika2 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKA)->Cast();
		ika2->AddChildObject(jo2);

		ika1->AddChildObject(ika2);

		// Link 2 <-> Link 3
		jo3 = GetFWScene()->GetPHScene()->CreateJoint(so2, so3, descJoint)->Cast();
		PHIKHingeActuatorIf* ika3 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKA)->Cast();
		ika3->AddChildObject(jo3);

		ika2->AddChildObject(ika3);

		// Link 3 <-> Link 4
		jo4 = GetFWScene()->GetPHScene()->CreateJoint(so3, so4, descJoint)->Cast();
		PHIKHingeActuatorIf* ika4 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKA)->Cast();
		ika4->AddChildObject(jo4);

		ika3->AddChildObject(ika4);

		// Link4 = End Effector
		descIKE.targetLocalPosition = Vec3d(0,-2,0);
		PHIKEndEffectorIf* ike1 = GetFWScene()->GetPHScene()->CreateIKEndEffector(descIKE);
		ike1->AddChildObject(so4);
		ika4->AddChildObject(ike1);
		ike1->SetTargetPosition(Vec3d(-5,0,0));

		ikeTarget = ike1;

		// ----- ----- ----- ----- -----

		GetFWScene()->GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);
		GetFWScene()->GetPHScene()->SetNumIteration(15);

		// ----- ----- ----- ----- -----

		ike1->SetTargetPosition(soTarget->GetPose().Pos());

		optimizer.CopyScene(GetFWScene()->GetPHScene());

		optimizer.Init();
		//optimizer.Start();
		//bStarted = true;

		optimizer.Optimize();

		double* params = optimizer.GetResults();
		if (params) {
			DSTR << "Result : " << std::endl;
			for (int i = 0; i < optimizer.NResults(); ++i) {
				DSTR << i << " : " << params[i] << std::endl;
			}
			DSTR << " --- " << std::endl;
		}
		optimizer.ApplyPop(GetFWScene()->GetPHScene(), optimizer.GetResults(), optimizer.NResults());

		/*
		double* params = optimizer.GetResults();
		if (params) {
			DSTR << "Result : " << std::endl;
			for (int i = 0; i < (nsub * 5 * 2); ++i) {
				DSTR << i << " : " << params[i] << std::endl;
			}
			DSTR << " --- " << std::endl;
			jt[0].Set(&(params[0 * 5]));
			jt[1].Set(&(params[nsub * 5]));
			jo1->SetSpring(abs(params[0 * nsub * 5 + 3]) * 100 + 100);
			jo1->SetDamper(abs(params[0 * nsub * 5 + 4]) * 10 + 10);
			jo2->SetSpring(abs(params[1 * nsub * 5 + 3]) * 100 + 100);
			jo2->SetDamper(abs(params[1 * nsub * 5 + 4]) * 10 + 10);
		}
		*/

		// ----- ----- ----- ----- -----

		/*
		ike1->SetTargetPosition(soTarget->GetPose().Pos());
		GetFWScene()->GetPHScene()->GetIKEngine()->Enable(true);
		GetFWScene()->GetPHScene()->GetIKEngine()->SetMaxVelocity(50);
		GetFWScene()->GetPHScene()->GetIKEngine()->SetMaxAngularVelocity(Rad(1000));

		optimizer.CopyScene(GetFWScene());

		optimizer.Init(4);
		optimizer.Start();
		bStarted = true;
		*/

		//optimizer.Start();
		//bStarted = true;

		// ----- ----- ----- ----- -----

		states_ = ObjectStatesIf::Create();
		states_->SaveState(GetFWScene());
	}

	virtual void OnAction(int menu, int id){
		if (menu == MENU_SCENE) {
			if (id == ID_RESET) {
				states_->LoadState(GetFWScene());
			}
		}
		SampleApp::OnAction(menu, id);
	}

	/// 1ステップのシミュレーション
	virtual void OnStep(){
		/*
		if (GetFWScene()->GetPHScene()->GetCount() < 100) {
			double t = GetFWScene()->GetPHScene()->GetCount() * GetFWScene()->GetPHScene()->GetTimeStep();
			jo1->SetTargetPosition(jt[0].At(t));
			jo2->SetTargetPosition(jt[1].At(t));
			GetFWScene()->Step();

		} else if (GetFWScene()->GetPHScene()->GetCount() < 150) {
			GetFWScene()->Step();

		} else {
			states_->LoadState(GetFWScene());

		}
		*/

		/*
		if (bStarted && !optimizer.IsRunning()) {
			double* params = optimizer.GetResults();
			if (params) {
				DSTR << "Result : " << std::endl;
				for (int i = 0; i < 30; ++i) {
					DSTR << i << " : " << params[i] << std::endl;
				}
				DSTR << " --- " << std::endl;
				jt[0].Set(&(params[0 * 3]));
				jt[1].Set(&(params[5 * 3]));
			}

			bStarted = false;
		}
		*/
	}

	/// 描画
	virtual void OnDraw(GRRenderIf* render){
		GetFWScene()->Draw(render);

		render->SetLighting(false);
		render->SetDepthTest(false);
		render->EnterScreenCoordinate();
		double dt = GetFWScene()->GetPHScene()->GetTimeStep();
		jt[0].Draw(render, 0, 100 * dt,   0);
		jt[1].Draw(render, 0, 100 * dt, 500);
		render->LeaveScreenCoordinate();
		render->SetLighting(true);
		render->SetDepthTest(true);
	}
} app;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
#if _MSC_VER <= 1500
#include <GL/glut.h>
#endif
int __cdecl main(int argc, char *argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

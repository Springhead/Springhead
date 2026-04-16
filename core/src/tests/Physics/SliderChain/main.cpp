#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <../src/Physics/PHJoint.h>

#include <array>
#include <iomanip>

#ifdef _WIN32
#include <direct.h>   // _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

using namespace Spr;

class MyApp : public FWApp {
public:
	void Init(int argc = 0, char* argv[] = 0) override {
		FWApp::Init(argc, argv);

		InitCamera();
		BuildScene();

		OpenCsv();
		WriteCsvRow(GetSdk()->GetScene()->GetPHScene());
	}

	void InitCamera() {
		Vec3d pos = Vec3d(5, 15, 0);
		GetCurrentWin()->GetTrackball()->SetPosition(pos);
		Vec3d target = Vec3d(5, 0.0, 0.0);
		GetCurrentWin()->GetTrackball()->SetTarget(target);
	}

private:
	bool bRun_ = false;

	const int N = 2;
	const int L = 10;
	const std::vector<int> steps = { 1 };

	void OpenCsv() {
		csv_.open(R"(C:\springhead_logs\SliderChain\Baum_one_1_2_4_8\solid_y.csv)");
		if (!csv_.is_open()) {
			std::cerr << "failed to open solid_y.csv\n";
			return;
		}

		csv_ << "step,time";
		for (int i = 0; i < N; ++i) {
			csv_ << ",solid" << i << "_y";
		}
		csv_ << "\n";

		csv_ << std::fixed << std::setprecision(10);
	}

	void WriteCsvRow(PHSceneIf* scene) {
		if (!csv_.is_open()) return;

		PHSolidIf** solids = scene->GetSolids();

		double time = simStep_ * scene->GetTimeStep();
		csv_ << simStep_ << "," << time;

		for (int i = 0; i < N; ++i) {
			Vec3d p = solids[i]->GetCenterPosition();
			csv_ << "," << p.y;   // もし環境で p.y が使えなければ p[1] に置き換え
		}
		csv_ << "\n";
	}

	std::ofstream csv_;
	int simStep_ = 0;

	void BuildScene() {
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

		// --- solver settings ---
		phscene->SetNumIteration(100);
		phscene->GetConstraintEngine()->SetVelCorrectionRate(0.3); // ERP (Baumgarte)
		// Position LCP: enable via PHConstraintEngine.cpp (numIterCorrection)
		phscene->SetTimeStep(0.001);

		struct SliderLevel {
			int step = 1;
			std::vector<PHSliderJointIf*> forward;   // i -> i + step
			std::vector<PHSliderJointIf*> backward;  // i + step -> i

			static PHSliderJointDesc MakeSliderDesc() {
				PHSliderJointDesc sjd;
				sjd.spring = 0.0;
				sjd.damper = 0.0;

				// joint軸を x 方向にそろえる
				sjd.poseSocket.Ori().RotationArc(Vec3d(0, 0, 1), Vec3d(1, 0, 0));
				sjd.posePlug.Ori().RotationArc(Vec3d(0, 0, 1), Vec3d(1, 0, 0));

				sjd.poseSocket.Pos() = Vec3d(0, 0, 0);
				sjd.posePlug.Pos() = Vec3d(0, 0, 0);

				return sjd;
			}

			SliderLevel() = default;

			SliderLevel(
				PHSceneIf* phscene,
				const std::vector<PHSolidIf*>& solids,
				int step_,
				double L,
				bool createBackward = true,
				bool createLimit = true
			) : step(step_) {
				const int N = static_cast<int>(solids.size());
				if (step <= 0 || step >= N) return;

				const int count = N - step;
				forward.resize(count, nullptr);

				if (createBackward) {
					backward.resize(count, nullptr);
				}

				for (int i = 0; i < count; ++i) {
					// forward: i -> i + step
					{
						PHSliderJointDesc sjd = MakeSliderDesc();
						forward[i] = phscene->CreateJoint(solids[i], solids[i + step], sjd)->Cast();

						if (createLimit && forward[i]) {
							double d = step * L;
							auto limit = forward[i]->CreateLimit();
							limit->SetRange(Vec2d(d,  d + L / 100.0));
							limit->SetSpring(1);
							limit->SetDamper(1000000);
						}
					}

					// backward: i + step -> i
					if(createBackward)  {
						PHSliderJointDesc sjd = MakeSliderDesc();
						backward[i] = phscene->CreateJoint(solids[i + step], solids[i], sjd)->Cast();

						if (createLimit && backward[i]) {
							double d = step * L;
							auto limit = backward[i]->CreateLimit();
							limit->SetRange(Vec2d(-1.25*d, -0.75*d));
							limit->SetSpring(1);
							limit->SetDamper(100);
						}
					}
				}
			}
		};

		struct ChainSystem {
			int N = 0;
			std::vector<PHSolidIf*> solids;
			std::vector<SliderLevel> sliderLevels;

			ChainSystem() = default;

			ChainSystem(
				PHSdkIf* phSdk,
				PHSceneIf* phscene,
				const CDBoxDesc& bd,
				int N_,
				double L,
				const std::vector<int>& steps,
				bool fixHead = true,
				bool createBackward = true,
				bool createLimit = true
			) : N(N_), solids(N_, nullptr) {
				// -----------------------------
				// 剛体生成
				// -----------------------------
				for (int i = 0; i < N; ++i) {
					PHSolidIf* so = phscene->CreateSolid();
					solids[i] = so;

					so->SetGravity(false);

					CDShapeIf* shape = phSdk->CreateShape(bd);
					shape->SetDensity(100000);
					so->AddShape(shape);
					so->CompInertia();
					// x方向に並べる
					so->SetFramePosition(Vec3d(i * L, 0, 0));
					so->SetDynamical(true);
				}

				// 先頭固定
				if (fixHead && !solids.empty()) {
					solids[0]->SetDynamical(false);
				}

				// -----------------------------
				// レベルごとの拘束生成
				// -----------------------------
				for (int step : steps) {
					if (step <= 0 || step >= N) continue;

					sliderLevels.emplace_back(
						phscene,
						solids,
						step,
						L,
						createBackward,
						createLimit
					);
				}
			}
		};

		const double BOX_SIZE = 0.25;
		CDBoxDesc bd; bd.boxsize = BOX_SIZE * Vec3d(1, 1, 1);

		ChainSystem chain(
			phSdk,
			phscene,
			bd,
			N,
			L,
			steps,
			true,           // fixHead
			false,			// backward を作るか
			true           // limit を作るか
		);

		chain.solids[N - 1]->SetCenterPosition(Vec3d((N - 1) * L, 0, 0));
		chain.solids[N - 1]->SetVelocity(Vec3d(10, 0, 0));

		GetSdk()->SetDebugMode(true);
		GetSdk()->GetScene()->EnableRenderAxis();
	}

public:
	void Keyboard(int key, int x, int y) override {
		switch (key) {
		case 27:
		case 'q':
			exit(0);
			break;
		case 'r':
			break;
		case ' ':
			if (bRun_) {
				bRun_ = false;
				std::cout << "Paused\n";
			}
			else {
				StepOnce();
				std::cout << "Step\n";
			}
			break;
		case '\r': {
			PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
			std::cout << "dt=" << phscene->GetTimeStep() << "\n";
			bRun_ = true;
			std::cout << "Run\n";
			break;
		}
		default:
			break;
		}
	}
	
	void StepOnce() {
		if (GetCurrentWin() && GetCurrentWin()->GetScene()) {
			PHSceneIf* scene = GetCurrentWin()->GetScene()->GetPHScene();
			scene->Step();
			            
			++simStep_;
			WriteCsvRow(scene);

			// if (simStep_ == 1000) exit(0);
			auto solids = scene->GetSolids();
			for (int i = 0; i < N; i++) {
				std::cout<<i<<':' << solids[i]->GetVelocity() << std::endl;
				std::cout << i << ':' << solids[i]->GetCenterPosition() << std::endl;
			}
		}

		PostRedisplay();
	}

	void TimerFunc(int id) override {
		if (bRun_) StepOnce();
	}

	~MyApp() override {
		if (csv_.is_open()) {
			csv_.close();
		}
	}
};

static MyApp app;

#if _MSC_VER <= 1600
#include <GL/glut.h>
#endif

int __cdecl main(int argc, char* argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

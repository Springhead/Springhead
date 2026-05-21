#include <Springhead.h>
#include <Framework/SprFWApp.h>

#include <../src/Physics/PHJoint.h>
#include <../src/Physics/PHSliderJoint.h>

#include <Foundation/UTQPTimer.h>

#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

using namespace Spr;

enum class SliderDirectionMode {
    Forward,
    Backward,
    Bidirectional
};

enum class FixedBodyMode {
    None,
    DynamicsOff,
    PositionFixedRotationFree
};

struct ExperimentConfig {
    int numBodies = 32;
    double spacing = 1.0;
    std::vector<int> skips = { 1 };

    double dt = 0.001;
    int velocityIterations = 100;
    double velocityCorrectionRate = 0.3;
    double positionCorrectionRate = 0.3;

    SliderDirectionMode direction = SliderDirectionMode::Bidirectional;
    FixedBodyMode fixedMode = FixedBodyMode::DynamicsOff;

    int fixedBodyIndex = 0;

    bool createAnchorShape = false;

    int forceBodyIndex = 31;
    Vec3d externalForce = Vec3d(0.0, 1000.0, 0.0);

    // ----------------------------
    // 形状・物性
    // ----------------------------
    double boxSize = 0.25;
    double density = 100000.0;

    // ----------------------------
    // Slider limit 設定
    // ----------------------------
    bool createLimit = true;

    // Balanced 用:
    //   forward  [ d - eps,  d + eps]
    //   backward [-d - eps, -d + eps]
    double balancedLimitWidthRatio = 1.0 / 100.0;
    double balancedLimitSpring = 1.0;
    double balancedLimitDamper = 1000000.0;

    // ----------------------------
    // 実行設定
    // ----------------------------
    int numSimSteps = 3000;
    bool printEveryStepState = false;
    bool printConfig = true;

    // ----------------------------
    // CSV 書き込み切り替え
    // ----------------------------
    bool enableCsvOutput = true;
    bool enableSolidStateLog = true;
    bool enableJointForceLog = true;
    bool enableStepTimeLog = true;

    std::string outputDir = "C:\\springhead_logs\\SliderChain";
    std::string caseName = "force_y_no_initial_error";
};

class MyApp : public FWApp {
public:
    void Init(int argc = 0, char* argv[] = 0) override {
        FWApp::Init(argc, argv);

        InitDefaultExperimentConfig();

        InitCamera();
        BuildScene();

        SetupLogging();

        if (cfg_.printConfig) {
            PrintConfig();
        }
    }

    void InitCamera() {
        double centerX = 0.5 * (cfg_.numBodies - 1) * cfg_.spacing;

        Vec3d pos = Vec3d(centerX, 15, 15);
        GetCurrentWin()->GetTrackball()->SetPosition(pos);

        Vec3d target = Vec3d(centerX, 0.0, 0.0);
        GetCurrentWin()->GetTrackball()->SetTarget(target);
    }

private:
    bool bRun_ = false;
    int simStep_ = 0;

    ExperimentConfig cfg_;

    struct JointRecord {
        PHSliderJointIf* jointIf = nullptr;
        PHSliderJoint* jointImpl = nullptr;

        int bodyA = -1;
        int bodyB = -1;
        int skip = 1;

        std::string directionLabel;
        std::string label;
    };

    std::vector<PHSolidIf*> solids_;
    std::vector<Vec3d> restPositions_;
    std::vector<JointRecord> joints_;

    // PositionFixedRotationFree 用。
    PHSolidIf* anchorSolid_ = nullptr;
    PHJointIf* anchorJoint_ = nullptr;

    std::ofstream solidCsv_;
    std::ofstream jointForceCsv_;
    std::ofstream stepTimeCsv_;

private:
    // ============================================================
    // ここを変える
    // ============================================================
    void InitDefaultExperimentConfig() {
        cfg_.numBodies = 7;
        cfg_.spacing = 10.0;
        cfg_.skips = { 1 };

        cfg_.dt = 0.001;
        cfg_.velocityIterations = 1000;
        cfg_.velocityCorrectionRate = 0.3;
        cfg_.positionCorrectionRate = 0.3;

        // Forward / Backward / Bidirectional
        cfg_.direction = SliderDirectionMode::Forward;

        // 固定は最大1つ。
        cfg_.fixedMode = FixedBodyMode::None;
        cfg_.fixedBodyIndex = 0;

        // 回転のみ許可の固定を試す場合:
        // cfg_.fixedMode = FixedBodyMode::PositionFixedRotationFree;
        // cfg_.fixedBodyIndex = 0;

        // X方向力は禁止。
        // forceBodyIndex の剛体に Y/Z 方向の力を加える。
        cfg_.forceBodyIndex = cfg_.numBodies / 2 ;
        cfg_.externalForce = Vec3d(0.0, 1000.0, 0.0);

        cfg_.boxSize = 0.25;
        cfg_.density = 1.0;

        cfg_.createLimit = false;

        cfg_.numSimSteps = 10000;

        cfg_.enableCsvOutput = true;
        cfg_.enableSolidStateLog = true;
        cfg_.enableJointForceLog = true;
        cfg_.enableStepTimeLog = true;

        cfg_.printEveryStepState = false;
        cfg_.printConfig = true;

        cfg_.outputDir = "C:\\springhead_logs\\SliderChain";
        cfg_.caseName = "balanced_bidir_force_y_tip";

        // ========================================================
        // 実験例
        // ========================================================
        //
        // 1. CSVを完全に止める:
        // cfg_.enableCsvOutput = false;
        //
        // 2. CPU時間だけ書く:
        // cfg_.enableCsvOutput = true;
        // cfg_.enableSolidStateLog = false;
        // cfg_.enableJointForceLog = false;
        // cfg_.enableStepTimeLog = true;
        //
        // 3. Forward only:
        // cfg_.direction = SliderDirectionMode::Forward;
        // cfg_.caseName = "forward_force_y_tip";
        //
        // 4. Backward only:
        // cfg_.direction = SliderDirectionMode::Backward;
        // cfg_.caseName = "backward_force_y_tip";
        //
        // 5. 旧コード互換 Bidirectional:
        // cfg_.direction = SliderDirectionMode::LegacyBidirectional;
        // cfg_.caseName = "legacy_bidir_force_y_tip";
        //
        // 6. 対称 Bidirectional:
        // cfg_.direction = SliderDirectionMode::BalancedBidirectional;
        // cfg_.caseName = "balanced_bidir_force_y_tip";
        //
        // 7. 長距離拘束:
        // cfg_.skips = { 1, 2, 4, 8 };
        // cfg_.caseName = "balanced_bidir_skip1_2_4_8_force_y_tip";
        //
        // 8. 根元付近に外力:
        // cfg_.forceBodyIndex = 1;
        // cfg_.caseName = "balanced_bidir_force_y_root_near";
        //
        // 9. 中央に外力:
        // cfg_.forceBodyIndex = cfg_.numBodies / 2;
        // cfg_.caseName = "balanced_bidir_force_y_middle";
        //
        // 10. Z方向に外力:
        // cfg_.externalForce = Vec3d(0.0, 0.0, 1000.0);
        // cfg_.caseName = "balanced_bidir_force_z_tip";
        // ========================================================
    }

    // ============================================================
    // Utility
    // ============================================================
    static void MakeDirectoryIfNeeded(const std::string& path) {
#ifdef _WIN32
        _mkdir(path.c_str());
#else
        mkdir(path.c_str(), 0777);
#endif
    }

    std::string PathJoin(const std::string& a, const std::string& b) const {
#ifdef _WIN32
        return a + "\\" + b;
#else
        return a + "/" + b;
#endif
    }

    std::string CsvBasePath() const {
        return PathJoin(cfg_.outputDir, cfg_.caseName);
    }

    Vec3d ForceVector() const {
        // X方向力は禁止。
        return Vec3d(0.0, cfg_.externalForce.y, cfg_.externalForce.z);
    }

    bool IsValidChainBodyIndex(int idx) const {
        return 0 <= idx && idx < cfg_.numBodies;
    }

    bool IsFixedBodyIndex(int idx) const {
        return cfg_.fixedMode != FixedBodyMode::None &&
            cfg_.fixedBodyIndex == idx;
    }

    std::string DirectionToString() const {
        switch (cfg_.direction) {
        case SliderDirectionMode::Forward:
            return "Forward";
        case SliderDirectionMode::Backward:
            return "Backward";
        case SliderDirectionMode::Bidirectional:
            return "Bidirectional";
        default:
            return "Unknown";
        }
    }

    std::string FixedModeToString() const {
        switch (cfg_.fixedMode) {
        case FixedBodyMode::None:
            return "None";
        case FixedBodyMode::DynamicsOff:
            return "DynamicsOff";
        case FixedBodyMode::PositionFixedRotationFree:
            return "PositionFixedRotationFree";
        default:
            return "Unknown";
        }
    }

    void PrintConfig() {
        Vec3d f = ForceVector();

        std::cout << "\n[SliderChain Config]\n";
        std::cout << "numBodies       = " << cfg_.numBodies << "\n";
        std::cout << "spacing         = " << cfg_.spacing << "\n";

        std::cout << "skips           = ";
        for (size_t i = 0; i < cfg_.skips.size(); ++i) {
            std::cout << cfg_.skips[i] << " ";
        }
        std::cout << "\n";

        std::cout << "dt              = " << cfg_.dt << "\n";
        std::cout << "iterations      = " << cfg_.velocityIterations << "\n";
        std::cout << "velCorrRate     = " << cfg_.velocityCorrectionRate << "\n";
        std::cout << "direction       = " << DirectionToString() << "\n";

        std::cout << "fixedMode       = " << FixedModeToString() << "\n";
        std::cout << "fixedBodyIndex  = " << cfg_.fixedBodyIndex << "\n";

        std::cout << "forceBodyIndex  = " << cfg_.forceBodyIndex << "\n";
        std::cout << "externalForce   = " << f << "  // x is forced to 0\n";

        std::cout << "chain solids    = " << solids_.size() << "\n";
        std::cout << "slider joints   = " << joints_.size() << "\n";
        std::cout << "csv output      = " << cfg_.enableCsvOutput << "\n";
        std::cout << "caseName        = " << cfg_.caseName << "\n";
        std::cout << "outputDir       = " << cfg_.outputDir << "\n";
        std::cout << std::endl;
    }

    // ============================================================
    // Solver
    // ============================================================
    void ApplySolverConfig(PHSceneIf* phscene) {
        phscene->SetTimeStep(cfg_.dt);
        phscene->SetNumIteration(cfg_.velocityIterations);
        phscene->GetConstraintEngine()->SetVelCorrectionRate(cfg_.velocityCorrectionRate);
        phscene->GetConstraintEngine()->SetPosCorrectionRate(cfg_.positionCorrectionRate);
    }

    // ============================================================
    // Joint desc
    // ============================================================
    static PHSliderJointDesc MakeSliderDesc() {
        PHSliderJointDesc sjd;

        sjd.spring = 0.0;
        sjd.damper = 0.0;

        // PHSliderJoint のスライド軸は local z。
        // それを world x 方向に合わせる。
        sjd.poseSocket.Ori().RotationArc(Vec3d(0, 0, 1), Vec3d(1, 0, 0));
        sjd.posePlug.Ori().RotationArc(Vec3d(0, 0, 1), Vec3d(1, 0, 0));

        sjd.poseSocket.Pos() = Vec3d(0, 0, 0);
        sjd.posePlug.Pos() = Vec3d(0, 0, 0);

        return sjd;
    }

    static PHBallJointDesc MakeAnchorBallJointDesc() {
        PHBallJointDesc bjd;

        bjd.spring = 0.0;
        bjd.damper = 0.0;

        // anchor と対象bodyの原点を一致させる。
        // これで位置は固定されるが、ball joint なので回転は自由。
        bjd.poseSocket.Pos() = Vec3d(0, 0, 0);
        bjd.posePlug.Pos() = Vec3d(0, 0, 0);

        return bjd;
    }

    // ============================================================
    // Scene construction
    // ============================================================
    void CreateBodies(PHSdkIf* phSdk, PHSceneIf* phscene) {
        solids_.clear();
        restPositions_.clear();
        anchorSolid_ = nullptr;
        anchorJoint_ = nullptr;

        CDBoxDesc bd;
        bd.boxsize = cfg_.boxSize * Vec3d(1, 1, 1);

        for (int i = 0; i < cfg_.numBodies; ++i) {
            PHSolidIf* so = phscene->CreateSolid();
            solids_.push_back(so);

            so->SetGravity(false);

            CDShapeIf* shape = phSdk->CreateShape(bd);
            shape->SetDensity(cfg_.density);
            so->AddShape(shape);
            so->CompInertia();

            Vec3d p(i * cfg_.spacing, 0, 0);

            // 初期誤差なし。
            so->SetFramePosition(p);
            so->SetCenterPosition(p);
            so->SetVelocity(Vec3d(0, 0, 0));
            so->SetAngularVelocity(Vec3d(0, 0, 0));
            so->SetDynamical(true);

            restPositions_.push_back(p);
        }

        ApplySingleFixedBody(phSdk, phscene);
        ApplyExternalForceToConfiguredBody();
        
    }

    void ApplySingleFixedBody(PHSdkIf* phSdk, PHSceneIf* phscene) {
        if (cfg_.fixedMode == FixedBodyMode::None) {
            return;
        }

        if (!IsValidChainBodyIndex(cfg_.fixedBodyIndex)) {
            std::cerr << "[warning] invalid fixedBodyIndex: "
                << cfg_.fixedBodyIndex << "\n";
            return;
        }

        PHSolidIf* target = solids_[cfg_.fixedBodyIndex];

        if (cfg_.fixedMode == FixedBodyMode::DynamicsOff) {
            target->SetDynamical(false);
            return;
        }

        if (cfg_.fixedMode == FixedBodyMode::PositionFixedRotationFree) {
            Vec3d p = restPositions_[cfg_.fixedBodyIndex];

            anchorSolid_ = phscene->CreateSolid();
            anchorSolid_->SetGravity(false);
            anchorSolid_->SetFramePosition(p);
            anchorSolid_->SetCenterPosition(p);
            anchorSolid_->SetVelocity(Vec3d(0, 0, 0));
            anchorSolid_->SetAngularVelocity(Vec3d(0, 0, 0));
            anchorSolid_->SetDynamical(false);

            if (cfg_.createAnchorShape) {
                CDBoxDesc abd;
                abd.boxsize = cfg_.boxSize * Vec3d(1, 1, 1);

                CDShapeIf* ashape = phSdk->CreateShape(abd);
                ashape->SetDensity(cfg_.density);
                anchorSolid_->AddShape(ashape);
                anchorSolid_->CompInertia();
            }

            PHBallJointDesc bjd = MakeAnchorBallJointDesc();

            anchorJoint_ = phscene->CreateJoint(
                anchorSolid_,
                target,
                bjd
            );

            return;
        }
    }

    void ApplyExternalForceToConfiguredBody() {
        if (!IsValidChainBodyIndex(cfg_.forceBodyIndex)) {
            return;
        }

        Vec3d f = ForceVector();

        if (f.norm() == 0.0) {
            return;
        }

        PHSolidIf* so = solids_[cfg_.forceBodyIndex];

        if (IsFixedBodyIndex(cfg_.forceBodyIndex) &&
            cfg_.fixedMode == FixedBodyMode::DynamicsOff) {
            std::cerr << "[warning] forceBodyIndex is DynamicsOff fixed body. "
                << "External force will not move it.\n";
        }

        so->AddForce(f);
    }

    void SetForwardLimit(PHSliderJointIf* joint, int skip) {
        if (!cfg_.createLimit || !joint) {
            return;
        }

        double d = skip * cfg_.spacing;
        PH1DJointLimitIf* limit = joint->CreateLimit();

        if (cfg_.direction == SliderDirectionMode::Bidirectional) {
            double eps = cfg_.spacing * cfg_.balancedLimitWidthRatio;

            limit->SetRange(Vec2d(d - eps, d + eps));
            limit->SetSpring(cfg_.balancedLimitSpring);
            limit->SetDamper(cfg_.balancedLimitDamper);
        }
    }

    void SetBackwardLimit(PHSliderJointIf* joint, int skip) {
        if (!cfg_.createLimit || !joint) {
            return;
        }

        double d = skip * cfg_.spacing;
        PH1DJointLimitIf* limit = joint->CreateLimit();

        if (cfg_.direction == SliderDirectionMode::Bidirectional) {
            double eps = cfg_.spacing * cfg_.balancedLimitWidthRatio;

            limit->SetRange(Vec2d(-d - eps, -d + eps));
            limit->SetSpring(cfg_.balancedLimitSpring);
            limit->SetDamper(cfg_.balancedLimitDamper);
        }
    }

    void AddSliderJoint(
        PHSceneIf* phscene,
        int bodyA,
        int bodyB,
        int skip,
        const std::string& directionLabel
    ) {
        PHSliderJointDesc sjd = MakeSliderDesc();

        PHSliderJointIf* jif = phscene->CreateJoint(
            solids_[bodyA],
            solids_[bodyB],
            sjd
        )->Cast();

        if (directionLabel == "forward") {
            SetForwardLimit(jif, skip);
        }
        else {
            SetBackwardLimit(jif, skip);
        }

        JointRecord rec;
        rec.jointIf = jif;
        rec.jointImpl = DCAST(PHSliderJoint, jif);
        rec.bodyA = bodyA;
        rec.bodyB = bodyB;
        rec.skip = skip;
        rec.directionLabel = directionLabel;

        std::ostringstream oss;
        oss << directionLabel << "_i" << bodyA << "_j" << bodyB << "_s" << skip;
        rec.label = oss.str();

        joints_.push_back(rec);
    }

    void CreateSliderConstraints(PHSceneIf* phscene) {
        joints_.clear();

        for (size_t si = 0; si < cfg_.skips.size(); ++si) {
            int skip = cfg_.skips[si];

            if (skip <= 0 || skip >= cfg_.numBodies) {
                continue;
            }

            int count = cfg_.numBodies - skip;

            for (int i = 0; i < count; ++i) {
                int j = i + skip;

                if (cfg_.direction == SliderDirectionMode::Forward) {
                    AddSliderJoint(phscene, i, j, skip, "forward");
                }
                else if (cfg_.direction == SliderDirectionMode::Backward) {
                    AddSliderJoint(phscene, j, i, skip, "backward");
                }
                else {
                    AddSliderJoint(phscene, i, j, skip, "forward");
                    AddSliderJoint(phscene, j, i, skip, "backward");
                }
            }
        }
    }

    void BuildScene() {
        PHSdkIf* phSdk = GetSdk()->GetPHSdk();
        PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

        phscene->SetGravity(Vec3d(0, 0, 0));

        ApplySolverConfig(phscene);

        CreateBodies(phSdk, phscene);
        CreateSliderConstraints(phscene);

        GetSdk()->SetDebugMode(true);
        GetSdk()->GetScene()->EnableRenderAxis();
    }

    // ============================================================
    // CSV logging
    // ============================================================
    void SetupLogging() {
        if (!cfg_.enableCsvOutput) {
            return;
        }

        OpenCsvFiles();

        WriteSolidState(0.0);
        WriteJointForces(0.0);
        WriteStepTime(0.0, 0);
    }

    void OpenCsvFiles() {
        if (!cfg_.enableCsvOutput) {
            return;
        }

        MakeDirectoryIfNeeded("C:\\springhead_logs");
        MakeDirectoryIfNeeded("C:\\springhead_logs\\SliderChain");
        MakeDirectoryIfNeeded(cfg_.outputDir);

        std::string base = CsvBasePath();

        if (cfg_.enableSolidStateLog) {
            solidCsv_.open(base + "_solid_state.csv");

            if (!solidCsv_.is_open()) {
                std::cerr << "failed to open solid_state csv\n";
            }
            else {
                solidCsv_ << std::fixed << std::setprecision(10);

                solidCsv_
                    << "step,time,"
                    << "max_abs_y_error,"
                    << "max_abs_z_error,"
                    << "max_transverse_error,"
                    << "applied_fx,applied_fy,applied_fz";

                for (int i = 0; i < cfg_.numBodies; ++i) {
                    solidCsv_
                        << ",solid" << i << "_x"
                        << ",solid" << i << "_y"
                        << ",solid" << i << "_z"
                        << ",solid" << i << "_err_y"
                        << ",solid" << i << "_err_z"
                        << ",solid" << i << "_vx"
                        << ",solid" << i << "_vy"
                        << ",solid" << i << "_vz";
                }

                solidCsv_ << "\n";
            }
        }

        if (cfg_.enableJointForceLog) {
            jointForceCsv_.open(base + "_joint_force.csv");

            if (!jointForceCsv_.is_open()) {
                std::cerr << "failed to open joint_force csv\n";
            }
            else {
                jointForceCsv_ << std::fixed << std::setprecision(10);

                jointForceCsv_
                    << "step,time,joint_id,label,bodyA,bodyB,skip,direction,"
                    << "fx,fy,fz,tx,ty,tz,force_norm,torque_norm\n";
            }
        }

        if (cfg_.enableStepTimeLog) {
            stepTimeCsv_.open(base + "_step_time.csv");

            if (!stepTimeCsv_.is_open()) {
                std::cerr << "failed to open step_time csv\n";
            }
            else {
                stepTimeCsv_ << std::fixed << std::setprecision(10);
                stepTimeCsv_ << "step,time,cpu_usec\n";
            }
        }
    }

    void CloseCsvFiles() {
        if (solidCsv_.is_open()) {
            solidCsv_.close();
        }

        if (jointForceCsv_.is_open()) {
            jointForceCsv_.close();
        }

        if (stepTimeCsv_.is_open()) {
            stepTimeCsv_.close();
        }
    }

    void WriteSolidState(double time) {
        if (!cfg_.enableCsvOutput || !cfg_.enableSolidStateLog) {
            return;
        }

        if (!solidCsv_.is_open()) {
            return;
        }

        double maxAbsY = 0.0;
        double maxAbsZ = 0.0;
        double maxTransverse = 0.0;

        for (int i = 0; i < cfg_.numBodies; ++i) {
            Vec3d p = solids_[i]->GetCenterPosition();

            double errY = p.y - restPositions_[i].y;
            double errZ = p.z - restPositions_[i].z;
            double transverse = sqrt(errY * errY + errZ * errZ);

            maxAbsY = std::max(maxAbsY, std::abs(errY));
            maxAbsZ = std::max(maxAbsZ, std::abs(errZ));
            maxTransverse = std::max(maxTransverse, transverse);
        }

        Vec3d f = ForceVector();

        solidCsv_
            << simStep_ << ","
            << time << ","
            << maxAbsY << ","
            << maxAbsZ << ","
            << maxTransverse << ","
            << f.x << ","
            << f.y << ","
            << f.z;

        for (int i = 0; i < cfg_.numBodies; ++i) {
            Vec3d p = solids_[i]->GetCenterPosition();
            Vec3d v = solids_[i]->GetVelocity();

            double errY = p.y - restPositions_[i].y;
            double errZ = p.z - restPositions_[i].z;

            solidCsv_
                << ","
                << p.x << ","
                << p.y << ","
                << p.z << ","
                << errY << ","
                << errZ << ","
                << v.x << ","
                << v.y << ","
                << v.z;
        }

        solidCsv_ << "\n";
    }

    void WriteJointForces(double time) {
        if (!cfg_.enableCsvOutput || !cfg_.enableJointForceLog) {
            return;
        }

        if (!jointForceCsv_.is_open()) {
            return;
        }

        for (size_t k = 0; k < joints_.size(); ++k) {
            if (!joints_[k].jointImpl) {
                continue;
            }

            Vec3d f, t;
            joints_[k].jointImpl->GetConstraintForce(f, t);

            double fn = f.norm();
            double tn = t.norm();

            jointForceCsv_
                << simStep_ << ","
                << time << ","
                << k << ","
                << joints_[k].label << ","
                << joints_[k].bodyA << ","
                << joints_[k].bodyB << ","
                << joints_[k].skip << ","
                << joints_[k].directionLabel << ","
                << f.x << ","
                << f.y << ","
                << f.z << ","
                << t.x << ","
                << t.y << ","
                << t.z << ","
                << fn << ","
                << tn << "\n";
        }
    }

    void WriteStepTime(double time, long long cpuUSec) {
        if (!cfg_.enableCsvOutput || !cfg_.enableStepTimeLog) {
            return;
        }

        if (!stepTimeCsv_.is_open()) {
            return;
        }

        stepTimeCsv_
            << simStep_ << ","
            << time << ","
            << cpuUSec << "\n";
    }

    // ============================================================
    // Runtime
    // ============================================================
public:
    void Keyboard(int key, int x, int y) override {
        switch (key) {
        case 27:
        case 'q':
            exit(0);
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

        case '\r':
        {
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
        PHSceneIf* scene = GetSdk()->GetScene()->GetPHScene();

        long long cpuUSec = 0;

        if (cfg_.enableCsvOutput && cfg_.enableStepTimeLog) {
            UTQPTimer timer;
            timer.CountUS();

            scene->Step();

            cpuUSec = timer.CountUS();
        }
        else {
            scene->Step();
        }

        ++simStep_;

        double time = simStep_ * scene->GetTimeStep();

        WriteSolidState(time);
        WriteJointForces(time);
        WriteStepTime(time, cpuUSec);

        if (cfg_.printEveryStepState) {
            for (int i = 0; i < cfg_.numBodies; ++i) {
                std::cout << i << " velocity: "
                    << solids_[i]->GetVelocity() << std::endl;
                std::cout << i << " position: "
                    << solids_[i]->GetCenterPosition() << std::endl;
            }
        }

        if (simStep_ >= cfg_.numSimSteps) {
            bRun_ = false;
            std::cout << "Reached numSimSteps = "
                << cfg_.numSimSteps << "\n";
        }

        PostRedisplay();
    }

    void TimerFunc(int id) override {
        if (bRun_) {
            StepOnce();
        }
    }

    ~MyApp() override {
        CloseCsvFiles();
    }
};

static MyApp app;

#if _MSC_VER <= 1600
#include <conio.h>
#endif

int __cdecl main(int argc, char* argv[]) {
    app.Init(argc, argv);
    app.StartMainLoop();
    return 0;
}
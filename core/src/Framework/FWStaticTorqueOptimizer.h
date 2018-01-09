#ifndef FWSTATICTORQUEOPTIMIZER_H
#define FWSTATICTORQUEOPTIMIZER_H

#include <Framework/SprFWOptimizer.h>
#include <Framework/FWOptimizer.h>
#include <Framework/FrameworkDecl.hpp>

#include<../src/Foundation/UTClapack.h>

namespace Spr {
	;

	class GrahamConvexHull : public UTRefCount {
		struct Sort {
			static void quicksort(std::vector<Vec3f>& ver, int i, int j, Vec3d normal);
			static void simplesort(std::vector<Vec3f>& ver, Vec3d normal);
		};
	public:
		//凸集合形成の点集合
		std::vector<Vec3f> hull;
		//多角形の仮中心
		Vec3f center;
		//多角形法線
		Vec3d normal;
		//面の原点からの距離
		double dist;
	public:
		GrahamConvexHull();
		~GrahamConvexHull();
		//与えられた点集合からグラハム凸多角形を再計算
		void Recalc(std::vector<Vec3f> vertices);
		//グラハム凸多角形内に与えた点が入るか
		bool InsideGrahamConvexHull(Vec3f v);
		//多角形中心からの距離に応じた評価値（遠->大）
		double Eval(Vec3f v);
		//多角形をなす点集合を返す
		Vec3f* GetVertices();
	};

	template<class TVtx>
	class FWQuickHull2D {
	public:
		std::vector<TVtx> points;
		Vec3d** begin;
		Vec3d** end;
	};

	class FWSupportPolygon {
	public:
		Vec3d normal;
		Vec3d dist;
	};
	
	class FWStaticTorqueOptimizer : public FWOptimizer {
		std::vector<JointPos> initialPos;
		Vec3d initialRootPos;

		double errorWeight, stabilityWeight, torqueWeight, resistWeight, constWeight, gravcenterWeight, differentialWeight;    //各評価値Weight

		std::vector<FWGroundConstraint*> groundConst;               //接地拘束集合、とりあえず書きやすかったのでvector
		std::vector<FWUngroundedConstraint*> ungroundedConst;       //非接地拘束

		std::vector<int> bodyIndex;            //身体構成剛体のindex

											   //データ送信用
		FWObjectiveValues val;                 //評価値内訳保存構造体
		Vec3d cog;                             //重心座標
		double mass;
		std::vector<Vec3f> supportPolygon;     //支持多角形形成頂点集合

	public:
		SPR_OBJECTDEF(FWStaticTorqueOptimizer);
		SPR_DECLMEMBEROF_FWStaticTorqueOptimizerDesc;

		FWStaticTorqueOptimizer(const FWStaticTorqueOptimizerDesc& desc = FWStaticTorqueOptimizerDesc()) {
			SetDesc(&desc);
		}

		void Init();

		void Iterate();

		void ApplyResult(PHSceneIf* phScene);

		double ApplyPop(PHSceneIf* phScene, double const *x, int n);

		double Objective(double const *x, int n);

		void SetScene(PHSceneIf* phSceneInput);

		void Optimize();

		bool TestForTermination();

		void TakeFinalValue();


		double CalcErrorCriterion();
		double CalcGroundedCriterion();
		double CalcPositionCriterion();
		double CalcCOGCriterion();
		double CalcDifferenceCriterion();
		double CalcTorqueCriterion();
		double CalcStabilityCriterion();

		double CenterOfGravity(PHIKActuatorIf* root, Vec3d& point);
		double CalcTorqueInChildren(PHIKActuatorIf* root, const Vec3d& point, const Vec3d& forceInChildren);

		void SetErrorWeight(double v);
		double GetErrorWeight();

		void SetStabilityWeight(double v);
		double GetStabilityWeight();

		void SetTorqueWeight(double v);
		double GetTorqueWeight();

		void SetResistWeight(double v);
		double GetResistWeight();

		void SetConstWeight(double v);
		double GetConstWeight();

		void SetGravcenterWeight(double v);
		double GetGravcenterWeight();

		void SetDifferentialWeight(double v);
		double GetDifferentialWeight();

		//構造体の配列を外部から取れないので１要素ずつpush
		void AddPositionConst(FWGroundConstraint* f);
		FWGroundConstraint GetGroundConst(int n);
		void ClearGroundConst();
		void AddPositionConst(FWUngroundedConstraint* f);
		FWUngroundedConstraint GetUngroundConst(int n);
		void ClearUngroundedConst();

		void SetESParameters(double xs, double st, double tf, double la, double mi);

		FWObjectiveValues GetObjectiveValues();

		Vec3f GetCenterOfGravity();

		int NSupportPolygonVertices();

		Vec3f GetSupportPolygonVerticesN(int n);
	};
	
}

#endif

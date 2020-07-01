#include <vector>
#include <Foundation/UTOptimizer.h>

#pragma  hdrstop

using namespace Spr;
using namespace std;

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
class UTOptimizerSample {
public:
	UTCMAESOptimizer* optimizer;

	UTOptimizerSample() {}
	~UTOptimizerSample(){}

	// ----- ----- -----

	void Run() {
		UTCMAESOptimizerDesc desc;
		// --
		// CMAESのパラメータを変更したい場合はここでdescにセットする
		// --
        desc.lambda = 20;

		// 最適化クラスの作成
		optimizer = DBG_NEW UTCMAESOptimizer(desc);

		// 次元のセット
		optimizer->SetDimension(3);

		// 初期解のセット
		double initialValue[] = { 0, 0, 0 };
		optimizer->SetInitialValue(initialValue);

		// 初期標準偏差のセット
		double initialStdDev[] = { 10, 10, 10 };
		optimizer->SetInitialStdDev(initialStdDev);

		// 初期化を実行
		optimizer->Initialize();

		// 最適化ループ：最適化が完了するまで続ける
		while (!optimizer->IsFinished()) {
			// 次に目的関数を計算すべきパラメータを取得
			double* population = optimizer->GetPopulation();

			// 目的関数を計算して結果をセット
			double objectiveFunctionValue = ObjectiveFunction(population);
			optimizer->SetObjectiveFunctionValue(objectiveFunctionValue);

			// 次へ
			optimizer->Next();
			DSTR << optimizer->GetCurrentPopulation() << std::endl;
			if (optimizer->GetCurrentPopulation() == 0) {
				// １世代ごとに途中経過表示
				DSTR << "Generation: " << optimizer->GetCurrentGeneration() << std::endl;
				DSTR << "Fitness: " << optimizer->GetFitness() << std::endl;
				DSTR << " -- " << std::endl;
			}
		}

        DSTR << "Lambda : " << optimizer->GetLambda() << std::endl;

		// 最適化が完了：結果を取得
		double* result = optimizer->GetResult();

		// 結果表示
		DSTR << "Result : " << std::endl;
		for (int i = 0; i < optimizer->GetDimension(); ++i) {
			DSTR << i << " : " << result[i] << std::endl;
		}
		DSTR << " --- " << std::endl;
		delete optimizer;
	}

	// 目的関数（例）
	double ObjectiveFunction(double* population) {
		double x = population[0], y = population[1], z = population[2];

		return (x - 10) * (x - 10) + (y - 20) * (y - 20) + (z - 30) * (z - 30) + 10;
	}

} app;

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
int __cdecl main(int argc, char *argv[]) {
	app.Run();
	return 0;
}

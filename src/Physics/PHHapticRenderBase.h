#ifndef PH_HAPTICRENDERBASE_H
#define PH_HAPTICRENDERBASE_H

#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticPointer.h>

namespace Spr{;

class PHHapticRenderBase{
public:
	struct PHHapticRenderInfo{
		PHHapticPointers*		pointers;
		PHSolidsForHaptic*		hsolids;
		PHSolidPairsForHaptic*	sps;
		int loopCount;
		double pdt;
		double hdt;
		bool bInterpolatePose;
		bool bMultiPoints;
	};

	virtual PHIrs CompIntermediateRepresentation(PHHapticPointer* pointer, PHHapticRenderInfo hri);
	virtual void PenaltyBasedRendering(PHHapticRenderInfo hri);
	virtual void ConstraintBasedRendering(PHHapticRenderInfo hri);
	virtual void ReflectForce2Solid(PHSolidsForHaptic* hsolids, double hdt, double pdt);
	virtual void VirtualCoupling(PHHapticPointer* pointer);
	// ガウスザイデル法を使いAx+b>0を解く
	template <class AD, class XD, class BD>
	void GaussSeidel(MatrixImp<AD>& a, VectorImp<XD>& x, const VectorImp<BD>& b){
		int nIter = 15;					// 反復回数の上限
		double error = 0.0;
		double errorRange = 10e-8;		// 許容誤差
		int n = (int)a.height();		// 連立方程式の数(行列aの行数)
		std::vector< double > lastx;
		for(int i = 0; i < n; i++){
			lastx.push_back(x[i]);
			x[i] = 0;
		}

		for(int k = 0; k < nIter; k++){		
			for(int i = 0; i < n; i++){
				double term1 = 0.0;
				double term2 = 0.0;
				for(int j = 0; j < i; j++){
					term1 += a[i][j] * x[j];
				}
				for(int j = i+1; j < n; j++){
					term2 += a[i][j] * lastx[j];
				}
				// xの更新(繰り返し計算の式を使用)
				x[i] =  (-b[i] - term1 - term2) / a[i][i];
				if(x[i] < 0) x[i] = 0.0;
			}

			// (lastx - x)の2乗の総和と誤差範囲を比較
			error = 0.0;
			for(int i = 0; i < n; i++){
				error += pow(x[i] - lastx[i], 2);
				//DSTR << "iterete" << i << "," << x[i] << std::endl;
			}
			if(error < errorRange){
				//DSTR << "Finish the iteration in admissible error. " << std::endl;
				//DSTR << k << std::endl;
				return;
			}

			// 繰り返し計算のために更新後のxをlastxに保存
			for(int i = 0; i < n; i++) lastx[i] = x[i];
		}
		//nIterで計算が終わらなかったので打ち切り
		//static int iterError = 0;
		//iterError += 1;
		//DSTR << iterError << "Could not converge in iteration steps. Error = " << error << std::endl;
		//CSVOUT << error << std::endl;
	}
};

}

#endif
#ifndef PH_HAPTICRENDERBASE_H
#define PH_HAPTICRENDERBASE_H

#include <Physics/PHHapticPointer.h>

using namespace PTM;
namespace Spr{;

class PHSolidsForHaptic;
class PHSolidPairForHaptic;
class PHSolidPairsForHaptic;
class PHShapePairForHaptic;

struct PHIntermediateRepresentation :public UTRefCount{
	int solidID;				///< 剛体のID(PHSolidsForHapitcとPHSolidPairForHaptic)と侵入しているか
	PHSolidPairForHaptic* solidPair;	///< PHSolidPairForHapticへのポインタ
	PHShapePairForHaptic* shapePair;	///< PHSolidPairForHapticへのポインタ
	double depth;				///< 侵入量
	Vec3d normal;				///< 面法線
	Vec3d r;					///< 力覚ポインタの重心からの接触点までのベクトル
	Vec3d pointerPointW;		///< 力覚ポンタの侵入点(world)
	Vec3d contactPointW;		///< 中間表現面にvertexを投影した位置(world)
	Vec3d pointerPointVel;		///< 力覚ポインタ側の接触点の速度(world)
	Vec3d contactPointVel;		///< 剛体側の接触点の速度(world)
	Vec3d force;				///< 力覚ポインタからの力
	Posed interpolation_pose;	///< 形状の補間姿勢

	PHIntermediateRepresentation(){
		solidID = -1;
		solidPair = NULL;
		shapePair = NULL;
	}
	PHIntermediateRepresentation(PHSolidPairForHaptic* so, int soId, PHShapePairForHaptic* sh)
		:solidPair(so), solidID(soId), shapePair(sh), depth(0.0){	
	}
};
typedef PHIntermediateRepresentation PHIr;
typedef std::vector< PHIr* > PHIrs;

struct PHHapticRenderInfo{// : public PHHapticRenderDesc{
	PHHapticPointers*		pointers;
	PHSolidsForHaptic*		hsolids;
	PHSolidPairsForHaptic*	sps;
	int loopCount;
	double pdt;
	double hdt;
	bool bInterpolatePose;
	PHHapticRenderInfo();
};

class PHHapticRender : public PHHapticRenderInfo, public SceneObject{
public:
	SPR_OBJECTDEF_NOIF(PHHapticRender);
	PHHapticRender();

	// Implementation
	virtual void HapticRendering(PHHapticRenderInfo info);
	PHIrs CompIntermediateRepresentation(PHHapticPointer* pointer);
	void CompIntermediateRepresentationForDynamicProxy(PHIrs& irsNormal, PHIrs& irsFric, PHHapticPointer* pointer);
	bool CompIntermediateRepresentationShapeLevel(PHSolid* solid0, PHHapticPointer* pointer,
		PHSolidPairForHaptic* so, PHShapePairForHaptic* sh, Posed curShapePoseW[2], double t, bool bInterpolatePose, bool bPoints);
	bool CompFrictionIntermediateRepresentation(PHHapticPointer* pointer, PHSolidPairForHaptic* sp, PHShapePairForHaptic* sh);
	void PenaltyBasedRendering(PHHapticPointer* pointer);
	//void ConstraintBasedRendering(PHHapticPointer* pointer);
	void DynamicProxyRendering(PHHapticPointer* pointer);
	void VibrationRendering(PHHapticPointer* pointer);
	void SolveProxyPose(Vec3d& dr, Vec3d& dtheta, Vec3d& allDepth, PHHapticPointer* pointer, const PHIrs& irs);

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
	// ガウスザイデル法を使いAx+b>0を解く
	template <class AD, class XD, class BD>
	void GaussSeidelMinMax(VectorImp<XD>& x, const VectorImp<XD>& xmin, const VectorImp<XD>& xmax, const MatrixImp<AD>& a, const VectorImp<BD>& b) {
		int nIter = 15;					// 反復回数の上限
		double error = 0.0;
		double errorRange = 10e-8;		// 許容誤差
		int n = (int)a.height();		// 連立方程式の数(行列aの行数)
		std::vector< double > lastx;
		for (int i = 0; i < n; i++) {
			lastx.push_back(x[i]);
			x[i] = 0;
		}

		for (int k = 0; k < nIter; k++) {
			for (int i = 0; i < n; i++) {
				double term1 = 0.0;
				double term2 = 0.0;
				for (int j = 0; j < i; j++) {
					term1 += a[i][j] * x[j];
				}
				for (int j = i + 1; j < n; j++) {
					term2 += a[i][j] * lastx[j];
				}
				// xの更新(繰り返し計算の式を使用)
				x[i] = (-b[i] - term1 - term2) / a[i][i];
				if (x[i] < xmin[i]) {
					x[i] = xmin[i];
				}
				if (x[i] > xmax[i]) {
					x[i] = xmax[i];
				}
			}

			// (lastx - x)の2乗の総和と誤差範囲を比較
			error = 0.0;
			for (int i = 0; i < n; i++) {
				error += pow(x[i] - lastx[i], 2);
				//DSTR << "iterete" << i << "," << x[i] << std::endl;
			}
			if (error < errorRange) {
				//DSTR << "Finish the iteration in admissible error. " << std::endl;
				//DSTR << k << std::endl;
				return;
			}

			// 繰り返し計算のために更新後のxをlastxに保存
			for (int i = 0; i < n; i++) lastx[i] = x[i];
		}
		//nIterで計算が終わらなかったので打ち切り
		//static int iterError = 0;
		//iterError += 1;
		//DSTR << iterError << "Could not converge in iteration steps. Error = " << error << std::endl;
		//CSVOUT << error << std::endl;
		return ;
	}
};

}

#endif
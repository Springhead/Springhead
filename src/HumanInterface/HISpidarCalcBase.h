#ifndef HISPIDARCALCBASE_H
#define HISPIDARCALCBASE_H

#include <Base/BaseDebug.h>
#include <Base/Affine.h>
#include <Base/QuadProgram.h>
#include <iostream>
#include <vector>
#include <math.h>

namespace Spr{;
/**	SPIDAR(糸を使った力覚提示装置)のための姿勢計算と力覚計算を行う
基本クラス．
インタフェース(仮想関数)と，
2次計画法や繰り返し法による姿勢計算のための作業領域を用意する．
void Spr::HISpidarCalcBase::MakeWireVec(), 
void Spr::HISpidarCalcBase::UpdatePos(), 
void Spr::HISpidarCalcBase::MeasureWire(), 
の3つの関数をオーバーライドすることで個々のSPIDARのための計算を行うように
できる．	*/
class SPR_DLL HISpidarCalcBase{
protected:
	/**@name	姿勢と力覚を計算するためのパラメータ.
		姿勢ベクトル = a(ワイヤ長) = p(w(ワイヤ長))
		a: ワイヤ長 から 姿勢ベクトル への変換関数．
		w: ワイヤ長 から ワイヤ接合位置ベクトル への変換関数．
		p: ワイヤ接合位置ベクトル から 姿勢ベクトル への変換関数

		姿勢ベクトル = a(ワイヤ長) = p(w(ワイヤ長)) を偏微分すると
		Δ姿勢ベクトル = A・Δワイヤ長 = P・W・Δワイヤ長
	*/
	//@{
	PTM::VVector<float> measuredLength;				///<	計測したワイヤ長
	PTM::VVector<float> calculatedLength;			///<	前の姿勢から計算されるワイヤ長
	PTM::VMatrixRow<float> matA;						///<	行列 A
	PTM::VMatrixRow<float> matATA;						///<	A * Aの転置
	PTM::VVector<float> lengthDiff;					///<	Δワイヤ長 = 計測ワイヤ長 - 算出ワイヤ長.		lengthDiff[i] is the measured SetLength of this time minus the computed legth of last time  
	PTM::VVector<float> lengthDiffAve;				///<	Δワイヤ長の平均値
	PTM::VVector<float> postureDiff;				///<	求めた姿勢の変化量 = A * lengthDiff
	//@}
	VQuadProgram<float> quadpr;				///<	２次計画問題の計算クラス
public:
	HISpidarCalcBase(){}					///<	コンストラクタ
	virtual ~HISpidarCalcBase(){}			///<	デストラクタ
protected:
	virtual void Update()=0;				///<	姿勢情報の更新
	PTM::VVector<float>& Tension() {		///<	張力
		return quadpr.vecX;
	}
	/**	初期化
		@param minF	最小張力
		@param maxF	最大張力
		@param dof	エンドエフェクタの自由度	*/
	void Init(int dof, const VVector<float>& minF, const VVector<float>& maxF);
	virtual void MakeWireVec()=0;			///<	ワイヤ方向ベクトルの計算
	virtual void UpdatePos()=0;				///<	ワイヤ設置位置座標の更新
	virtual void MeasureWire()=0;			///<	ワイヤ長の計測
};

}	//	namespace Spr
#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/***********************************************************************/
/*                                                                     */
/*  FILE        :qp_prog02.h                                           */
/*  DATE        :2002/04/01                                            */
/*  DESCRIPTION :Quadratic Programming (Active Set Method)             */
/*                                                                     */
/*  Katsuhito AKAHANE  (kakahane@hi.pi.titech.ac.jp)                   */
/*                    modified by Shoichi HASEGAWA                     */
/*                                                                     */
/***********************************************************************/
#ifndef QUADPROGRAM_H
#define QUADPROGRAM_H
#include <Base/TMatrix.h>
using namespace PTM;
/*
二次計画法
                                                          2003/02/04　赤羽 克仁

1.目的
	ボックス制約条件下(minX<=x<=maxX)で、次元が小規模の二次計画問題を解くプログラムです。
	SPIDAR用に開発されていますが、ボックス制約条件下ならば、一般の二次計画問題を解くこともできます。
	具体的には、以下の通り
	
	f(x) = (1/2)x'Qx - c'x   -> minimize
	minX <= x <= maxX
	
	を有効制約法(Active Set Strategy Methods)を用いて解いています。
*/
/**	Quadratic Programming (C++ template)
	@param MATNN	n行n列の行列型
	@param VECN		n行のベクトル型
	@param VEC2N	2n行のベクトル型
	@param VECNI	int型のn行のベクトル型	*/
template <typename MATNN, typename VECN, typename VEC2N, typename VECNI>
class QuadProgramImp{
public:
	MATNN matQ;				///<	目的関数の2次の係数行列
	VECN vecC;				///<	目的関数の1次の係数ベクトル
	VECN vecX;				///<	解
	int Dim(){ return (int)vecX.size(); }
	typedef TYPENAME VECN::element_type T;

protected:
	MATNN matA;
	VECN minX;				
	VECN maxX;

	MATNN matR;
	VECN vecL;

	VEC2N vecXYNext;
	VECN vecD;
	VECNI isActiveSet;

public:
	QuadProgramImp(){}
	~QuadProgramImp(){}

	//
	// QP Initialize Function
	// minT <= T <= maxT
	void Init(VECN minT, VECN maxT){
		int i,j;
		minX = -minT;
		maxX = maxT;

		for(i=0; i<Dim(); i++){
			for(j=0;j<Dim();j++){
				matA[i][j] = 0;
			}
			vecX[i] = minT[i];
			isActiveSet[i] = -1;
			matA[i][i] = 1;
		}
	}

	//
	// QP Main Function
	int Solve(){
		if (matR.height() < 1) return -1;
		int i = 0;
		//while(1){
		for(;i<10;i++){
			MakeCalcMat();
			CalcMatRXL(matR,vecXYNext,vecL);
			if(isVecX_VecXNext()){
				if(CalcLambda()) return i;
			}
			else{
				CalcAlpha();
			}
			i++;
		}
		return -1;
	}

protected:
	void MakeCalcMat(){
		int i,j;
		for(i=0;i<Dim();i++){
			vecL[i] = vecC[i];
			for(j=0;j<Dim();j++){
				if(isActiveSet[j] > 0){
					vecL[i] -= matQ[i][j] * maxX[i];
					matR[i][j] = matA[i][j];
				}
				else if(isActiveSet[j] < 0){
					vecL[i] += matQ[i][j] * minX[i];
					matR[i][j] = -matA[i][j];
				}
				else{
					matR[i][j] = matQ[i][j];
				}
			}
		}

	}

	void CalcMatRXL(MATNN& a, VEC2N& x, VECN& b){
		int i,j,k;
		T p,q,s;
		// 連立１次方程式を解く
		//（ガウスの消去法 科学技術計算ハンドブックより）改善の余地あり
		for ( k=0 ; k<Dim()-1 ; ++k )
		{
			p=a[k][k];
			for ( j=k+1 ; j<Dim() ; ++j )
				a[k][j]/=p;
			b[k]/=p;
			for ( i=k+1 ; i<Dim() ; ++i )
			{
				q=a[i][k];
				for ( j=k+1 ; j<Dim() ; ++j )
					a[i][j]-=q*a[k][j];
				b[i]-=q*b[k];
			} 
		} 
		x[Dim()-1]=b[Dim()-1]/a[(Dim()-1)][(Dim()-1)];
		for ( k=Dim()-2 ; k>=0 ; --k )
		{
			s=b[k];
			for ( j=k+1 ; j<Dim() ; ++j )
				s-=a[k][j]*x[j];
			x[k]=s;
		}  

		// 結果を格納
		for(i=0;i<Dim();i++){
			if(isActiveSet[i] > 0){
				x[Dim()+i] = x[i];
				x[i] = maxX[i];
			}
			else if(isActiveSet[i] < 0){
				x[Dim()+i] = x[i];
				x[i] = -minX[i];
			}
			else{
				x[Dim()+i] = 0.0f;
				//xout[i] = x[i];
			}
		}

	}

	int isVecX_VecXNext(){
		int i;
		for(i=0;i<Dim();i++){
			if(vecX[i] != vecXYNext[i]) return 0;
		}
		return 1;
	}

	int CalcLambda(){
		int i,bval = 1;
		for(i=0;i<Dim();i++){
			if(isActiveSet[i]){
				if(vecXYNext[Dim()+i] < 0){
					isActiveSet[i] = 0;
					bval = 0;
				}
			}
		}
		return bval;
	}

	void CalcAlpha(){
		int i,minIndex = -1,bval;
		T val,alpha;
		T minAlpha = 1;

		for(i=0;i<Dim();i++){
			if(!isActiveSet[i]){
				val = vecD[i] = vecXYNext[i] - vecX[i];
				if(val < 0){
					alpha = -(minX[i] + vecX[i]) / val;
					if(alpha > 0 && minAlpha > alpha){
						minAlpha = alpha;
						minIndex = i;
						bval = -1;
					}
					else if(alpha <= 0){
						isActiveSet[i] = -1;
					}
				}
				else if(val > 0){
					alpha = (maxX[i] - vecX[i]) / val;
					if(alpha > 0 && minAlpha > alpha){
						minAlpha = alpha;
						minIndex = i;
						bval = 1;
					}
					else if(alpha <= 0){
						isActiveSet[i] = 1;
					}
				}
			}
		}
		if(minIndex >= 0){
			isActiveSet[minIndex] = bval;
			for(i=0;i<Dim();i++){
				if(!isActiveSet[i]){
					vecX[i] += minAlpha * vecD[i];
				}
			}
		}
		else{
			for(i=0;i<Dim();i++){
				vecX[i] = vecXYNext[i];
			}
		}
	}
};

/**	Quadratic Programming (C++ template)
	@param T	型名
	@param N	次元数						*/
template <typename T, int N>
class TQuadProgram:public QuadProgramImp< TMatrixRow<N, N, T>, TVector<N, T>, TVector<2*N, T>, TVector<N, int> >{
};
/**	Quadratic Programming (C++ template)
	@param T	型名	*/
template <class T>
class VQuadProgram:public QuadProgramImp< VMatrixRow<T>, VVector<T>, VVector<T>, VVector<int> >{
public:
#ifdef __GNUC__
	typedef typename VQuadProgram::T ET;
#else
	typedef T ET;
#endif
	typedef QuadProgramImp< VMatrixRow<ET>, VVector<ET>, VVector<ET>, VVector<int> > base_type;
	///	次元を設定
	void SetDim(int n){
		this->matQ.resize(n,n);
		this->vecC.resize(n);
		this->vecX.resize(n);
		this->matA.resize(n, n);
		this->minX.resize(n);	
		this->maxX.resize(n);

		this->matR.resize(n, n);
		this->vecL.resize(n);

		this->vecXYNext.resize(2*n);
		this->vecD.resize(n);
		this->isActiveSet.resize(n);
	}
	///	初期化，minTの次元で，次元を設定
	void Init(VVector<ET> minT, VVector<ET> maxT){
		SetDim((int)minT.size());
		base_type::Init(minT, maxT);
	}

	void SetOneRange(int num, ET max,ET min){
		this->minX[num] = -min;
		this->maxX[num] = max;
	}
};


#endif //QUADPROGRAM_H

#ifndef __MATRIXCALC_H_INCLUDED__
#define __MATRIXCALC_H_INCLUDED__


#include <cmath>
#include "tetgen.h"



class MatrixCalc{
private:
	void swapLines(REAL mat[],int size, int lineNum1, int lineNum2);
public:
	MatrixCalc();
	REAL	determinant(REAL mat[],int size);		//det|mat|の結果を返す
	void VecMultiplyTVec(REAL vec1[], REAL vec2[], int size ,REAL ret[]);	//size*1のベクトルvec1と1*sizeのベクトルvec2の掛け算をする
	void MatrixPlusMatrix(REAL mat1[], REAL mat2[], int size);	//mat1にmat2を足す。mat1=mat1+mat2;
	void MatrixMultiplyVec(REAL mat[], REAL vec[], int size, REAL temp[] );//マトリックスと行列の掛け算

	void CopyMatrix(REAL original[],REAL copy[], int size);//コピー。original：原本, copy:コピー先
	void CopyVector(REAL original[], REAL copy[], int size);//コピー。original：原本, copy:コピー先
	void CopyVector(int original[], int copy[], int size);//コピー。original：原本, copy:コピー先
	void SubstructVec2FromVec1(REAL vec1[], REAL vec2[],int size);//引き算。vec1=vec1-vec2
	void AddVec2ToVec1(REAL vec1[] , REAL vec2[] , int size);//足し算

	void GaussJordan(REAL mat[],REAL vec[],int size);//ピボットなしガウスジョルダン法
	void GaussJordanUsePivot(REAL mat[],REAL vec[],int size);//ピボットありガウスジョルダン法
	void LUsolve(REAL mat[], REAL vec[],REAL ret[],int size);//LU分解を用いた計算

	void ExteriorProduct(REAL v1[],REAL v2[],REAL ret[]);//外積。v1×v2
	void ExteriorProduct(REAL xa, REAL ya, REAL za, REAL xb, REAL yb,REAL zb,REAL ret[]); 
	REAL GetVectorLength(REAL v[]);//ベクトルの絶対値を取得する

	REAL CreateJudgeCoefficient(REAL mat[]);
	REAL CreateJudgeD(REAL a,REAL b,REAL c,REAL xyz[]);
	bool judgeInOout(REAL a, REAL b, REAL c, REAL d,REAL target[]);

	REAL MatrixCalc::createJudgeCoefficient(REAL mat[]);
	REAL MatrixCalc::createJudgeD(REAL a, REAL b, REAL c, REAL xyz[]);

	REAL CalcTriangleAreas(REAL p1[],REAL p2[],REAL p3[]);

};


#endif
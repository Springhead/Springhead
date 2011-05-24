#ifndef __TETRAHEDRON_H_INCLUDED__
#define __TETRAHEDRON_H_INCLUDED__

/************************************

四面体要素関係

*************************************/
#include <iostream>
#include "MatrixCalc.h"

class Tetrahedron
{
public:
	//REAL LocalRoundN[12];	//局所座標系におけるNをξ,η,ζで偏微分したものを格納している。
							//0~3:∂N/∂ξ, 4~7:∂N/∂η,8~11∂N/∂ζ,

	Tetrahedron();
	REAL LocalDetJ(REAL x1,REAL x2,REAL x3, REAL x4,
				REAL y1,REAL y2,REAL y3, REAL y4,
				REAL z1,REAL z2,REAL z3, REAL z4);					//ヤコビアンJの行列式の結果を返す

	void LocalJ(REAL x1,REAL x2,REAL x3, REAL x4,
				REAL y1,REAL y2,REAL y3, REAL y4,
				REAL z1,REAL z2,REAL z3, REAL z4,
				REAL matJ[]);												//ヤコビアンを返す

	void LocalInvJMultiplyDetJ(REAL x1,REAL x2,REAL x3, REAL x4,
				REAL y1,REAL y2,REAL y3, REAL y4,
				REAL z1,REAL z2,REAL z3, REAL z4,
				REAL invj[]);												//ヤコビアンJの逆行列にdet|J|をかけたものを返す

	void LocalInvJ(REAL x1,REAL x2,REAL x3, REAL x4,
				REAL y1,REAL y2,REAL y3, REAL y4,
				REAL z1,REAL z2,REAL z3, REAL z4,
				REAL invj[]);												//ヤコビアンの逆行列を返す

	REAL CalcVolume(REAL x1,REAL x2,REAL x3, REAL x4,
					REAL y1,REAL y2,REAL y3, REAL y4,
					REAL z1,REAL z2,REAL z3, REAL z4);			//四面体の体積を返す

	void retlocalRoundN(REAL N[]);
	
};



#endif
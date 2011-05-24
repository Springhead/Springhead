
/************************************

Žl–Ê‘Ì—v‘fŠÖŒW

*************************************/
#include <iostream>
#include "MatrixCalc.h"
#include "Tetrahedron.h"


Tetrahedron::Tetrahedron()
{
	/*for(int i=0 ; i<12 ; i++ ){
		LocalRoundN[i]=0;
	}
	LocalRoundN[0] =-1;
	LocalRoundN[1] = 1;
	LocalRoundN[4] =-1;
	LocalRoundN[6] = 1;
	LocalRoundN[8] =-1;
	LocalRoundN[11]= 1;*/
}

void Tetrahedron::LocalJ(REAL x1, REAL x2, REAL x3, REAL x4,
						 REAL y1, REAL y2, REAL y3, REAL y4, 
						 REAL z1, REAL z2, REAL z3, REAL z4, REAL matJ[])
{
	//ˆê—ñ–Ú
	matJ[0] = x2-x1;
	matJ[3] = x3-x1;
	matJ[6] = x4-x1;

	//“ñ—ñ–Ú
	matJ[1] = y2-y1;
	matJ[4] = y3-y1;
	matJ[7] = y4-y1;

	//ŽO—ñ–Ú
	matJ[2] = z2-z1;
	matJ[5] = z3-z1;
	matJ[8] = z4-z1;
}



REAL Tetrahedron::LocalDetJ(REAL x1, REAL x2, REAL x3, REAL x4,
							  REAL y1, REAL y2, REAL y3, REAL y4,
							  REAL z1, REAL z2, REAL z3, REAL z4)
{
	REAL detj;

	detj=(x2-x1)*((y3-y1)*(z4-z1)-(z3-z1)*(y4-y1))
		  +(y2-y1)*((z3-z1)*(x4-x1)-(x3-x1)*(z4-z1))
		  +(z2-z1)*((x3-x1)*(y4-y1)-(y3-y1)*(x4-x1));
	
	return detj;

}

/**
‹ts—ñ‚Ìdet{J}”{‚Ìs—ñinvj‚ð•Ô‚·
x,y,z‚ÍŽl–Ê‘Ì‚ð\¬‚·‚éÀ•W
*/
void Tetrahedron::LocalInvJMultiplyDetJ(REAL x1, REAL x2, REAL x3, REAL x4,
										REAL y1, REAL y2, REAL y3, REAL y4,
										REAL z1, REAL z2, REAL z3, REAL z4, REAL invj[])
{
	invj[0]=(y3-y1)*(z4-z1)-(z3-z1)*(y4-y1);
	invj[3]=(z3-z1)*(x4-x1)-(x3-x1)*(z4-z1);
	invj[6]=(x3-x1)*(y4-y1)-(y3-y1)*(x4-x1);

	invj[1]=(z2-z1)*(y4-y1)-(y2-y1)*(z4-z1);
	invj[4]=(x2-x1)*(z4-z1)-(z2-z1)*(x4-x1);
	invj[7]=(y2-y1)*(x4-x1)-(x2-x1)*(y4-y1);

	invj[2]=(y2-y1)*(z3-z1)-(z2-z1)*(y3-y1);
	invj[5]=(z2-z1)*(x3-x1)-(x2-x1)*(z3-z1);
	invj[8]=(x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);


}



REAL Tetrahedron::CalcVolume(REAL x1, REAL x2, REAL x3, REAL x4, 
							   REAL y1, REAL y2, REAL y3, REAL y4, 
							   REAL z1, REAL z2, REAL z3, REAL z4)
{
	MatrixCalc mc;
	REAL vol;
	//REAL *v;
	//v=new REAL[16];
	REAL v[16];

	for(int i=0 ; i<4 ; i++){
		v[i*4]=1;
	}

	v[1] =x1;
	v[5] =x2;
	v[9] =x3;
	v[13]=x4;

	v[2] =y1;
	v[6] =y2;
	v[10]=y3;
	v[14]=y4;

	v[3] =z1;
	v[7] =z2;
	v[11]=z3;
	v[15]=z4;
	
	vol=mc.determinant(v,4) / 6.0;
	//delete v;

	return vol;

}

void Tetrahedron::LocalInvJ(REAL x1, REAL x2, REAL x3, REAL x4, REAL y1, REAL y2, REAL y3, REAL y4, REAL z1, REAL z2, REAL z3, REAL z4, REAL invj[])
{
	//REAL *jinv;
	//jinv=new REAL[9];

	//
	REAL jinv[9];


	this->LocalInvJMultiplyDetJ(x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4,jinv);
	REAL dj;
	dj=this->LocalDetJ(x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4);

	for(int i=0 ; i<9 ; i++)
	{
		invj[i]=jinv[i]/dj;
	}
	//delete jinv;
}

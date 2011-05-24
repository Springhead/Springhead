#include <cmath>
#include "MatrixCalc.h"
#include <iostream>

MatrixCalc::MatrixCalc(){
}

REAL MatrixCalc::determinant(REAL mat[],int size)
{

  REAL d,m;
  REAL check=0;
  int line=0;
  REAL hugou =1.0;
  for(int k=0 ; k< size ; k++ )
  {
      if(mat[k*size+k]==0)
      {
          for(int i=k+1 ; i<size ; i++ )
          {
              if(fabs(mat[i*size+k])>check)
              {
                  check=fabs(mat[i*size+k]);
                  line=i;
              }
          }
          this->swapLines(mat,size,k,line);
          hugou*=(-1.0);
          //std::cout << "swap"<<std::endl;
      }
      d=1/mat[k*size+k];
      for(int i=k+1 ; i<size ; i++ )
      {
          m=mat[size*i+k]*d;
          for(int j=0 ; j<size ; j++ )
          {
              mat[size*i+j]-=m*mat[k*size+j];
          }
      }
  }
  //対角部分の積
  for(int i=0 ; i<size ; i++ ){
    hugou*=mat[size*i+i];
  }
  return hugou;
}


void MatrixCalc::swapLines(REAL mat[], int size,int lineNum1, int lineNum2)
{
	REAL *line = new REAL[(size*2)];
	
	for(int i=0 ; i<size ; i++ )
	{
		line[i]=mat[lineNum1*size+i];
		line[size+i]=mat[lineNum2*size+i];
	}

	for(int i=0 ; i<size ; i++)
	{
		mat[lineNum1*size+i]=line[size+i];
		mat[lineNum2*size+i]=line[i];
	}
	delete line;
}

void MatrixCalc::VecMultiplyTVec(REAL *vec1, REAL *vec2, int size, REAL *ret)
{
	
	for(int i=0 ; i<size ; i++)
	{
		for(int j=0 ; j<size ; j++)
		{
			ret[i*size+j]=vec1[i]*vec2[j];
		}
	}
	
}

void MatrixCalc::MatrixPlusMatrix(REAL mat1[], REAL mat2[], int size)
{
	for(int i=0; i<size*size ; i++)
	{
		mat1[i]+=mat2[i];
	}
}

void MatrixCalc::MatrixMultiplyVec(REAL mat[], REAL vec[], int size, REAL temp[])
{
	for(int i=0 ; i<size ; i++ )
	{
		temp[i]=0.0;
	}
	for(int i=0 ; i<size ; i++ )
	{
		for(int j=0 ; j<size ; j++ )
		{
			temp[i]+=mat[i*size+j]*vec[j];
		}
	}
}

void MatrixCalc::CopyMatrix(REAL original[], REAL copy[], int size)
{
	for(int i=0 ; i<size*size ; i++ )
	{
		copy[i]=original[i];
	}
}

void MatrixCalc::CopyVector(REAL original[], REAL copy[], int size)
{
	for(int i=0 ; i<size ; i++ )
	{
		copy[i]=original[i];
	}
}

void MatrixCalc::CopyVector(int original[], int copy[], int size)
{
	for(int i=0 ; i< size ; i++ )
	{
		copy[i] = original[i];
	}
}

void MatrixCalc::AddVec2ToVec1(REAL vec1[], REAL vec2[], int size)
{
	for(int i=0 ;i<size ; i++ )
	{
		vec1[i]+=vec2[i];
	}

}
void MatrixCalc::SubstructVec2FromVec1(REAL vec1[], REAL vec2[], int size)
{
	for(int i =0 ; i<size ; i++ )
	{
		vec1[i]-=vec2[i];
	}
}

void MatrixCalc::GaussJordan(REAL mat[], REAL vec[], int size )
{
	REAL a;

	for(int i=0;i<size;i++){
		a=mat[i*size+i];
		for(int j=0 ; j<size ; j++)
		{
			mat[i*size+j]/=a;
		}
		vec[i]/=a;
		for(int j=0 ; j<size ; j++)
		{
			if(j!=i)
			{
				a=mat[j*size+i]/mat[i*size+i];
				
				for(int k=i ; k<size; k++ )
				{
					mat[j*size+k]-=a*mat[i*size+k];
				}
				vec[j]-=a*vec[i];
			}
		}
	}

}

void MatrixCalc::GaussJordanUsePivot(REAL mat[], REAL vec[], int size)
{
	REAL a;
	REAL vtemp;
	int line;

	for(int i=0;i<size;i++){
		a=mat[i*size+i];
		line=i;
		//pivot--------------------------------------------
		for(int j=i ; j<size ; j++ )
		{
			if(a==1.0)
			{
				break;
			}
			if(fabs(mat[j*size+i])>fabs(a) ||mat[j*size+i]==1.0)
			{
				a=mat[j*size+i];
				line=j;
			}
		}
		if(line!=i){
			this->swapLines(mat,size,i,line);
			vtemp=vec[i];
			vec[i]=vec[line];
			vec[line]=vtemp;
		}
		//end pivot--------------------------------------------
		for(int j=0 ; j<size ; j++)
		{
			mat[i*size+j]/=a;
		}
		vec[i]/=a;
		for(int j=0 ; j<size ; j++)
		{
			if(j!=i)
			{
				a=mat[j*size+i]/mat[i*size+i];
				
				for(int k=i ; k<size; k++ )
				{
					mat[j*size+k]-=a*mat[i*size+k];
				}
				vec[j]-=a*vec[i];
			}
		}
	}

}

void MatrixCalc::LUsolve(REAL mat[], REAL vec[],REAL ret[], int size)
{
	REAL multi ,pivot,sum;
	int k;
	REAL *y;
	y=new REAL[size];

	for(int i=0 ; i<size-1 ; i++)
	{
		pivot = mat[size*i+i];
		for(int j=i+1; j<size ; j++)
		{
			multi = 1.0/pivot;
			sum=mat[size*i+j];
			
			for(k=0;k<i;k++)
			{
				sum-=mat[size*i+k]*mat[size*k+j];
			}
			
			mat[size*i+j]=sum*multi;
		}
		for(int j=i+1 ; j<size; j++ )
		{
			sum = mat[size*j+i+1];
			for(k=0 ; k<i+1 ; k++)
			{
				sum -= mat[size*j+k]*mat[size*k+i+1];
			}
			mat[size*j+i+1]=sum;
		}
		
	}

	//前進代入
	y[0] = vec[0]/mat[0];
	for(int i=1; i<size ; i++ )
	{
		sum=vec[i];
		for(int j=0; j<i ; j++ )
		{
			sum -= mat[size*i+j]*y[j];
		}
		y[i]=sum/mat[size*i+i];
	}

	//後退代入
	ret[size-1]=y[size-1];
	for(int i=size-2 ; i>=0; i-- )
	{
		sum=y[i];
		for(int j=size-1; j>i; j--)
		{
			sum-=mat[size*i+j]*ret[j];
			
		}
		ret[i]=sum;
	}

	delete y;
}

void MatrixCalc::ExteriorProduct(REAL v1[], REAL v2[], REAL ret[])
{
	ret[0]=v1[1]*v2[2]-v1[2]*v2[1];
	ret[1]=-v1[0]*v2[2]+v1[2]*v2[0];
	ret[2]=v1[0]*v2[1]-v1[1]*v2[0];

}

void MatrixCalc::ExteriorProduct(REAL xa, REAL ya, REAL za, REAL xb, REAL yb, REAL zb, REAL ret[])
{
	ret[0] = ya*zb-za*yb;
	ret[1] =-xa*zb+za*xb;
	ret[2] = xa*yb-ya*xb;
}

REAL MatrixCalc::GetVectorLength(REAL v[])
{
	REAL ret = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	return ret;
}
REAL MatrixCalc::CreateJudgeCoefficient(REAL mat[])
{
	REAL d;
	d =	mat[0]*(mat[4]*mat[8]-mat[5]*mat[7])+
		mat[1]*(mat[5]*mat[6]-mat[3]*mat[8])+
		mat[2]*(mat[3]*mat[7]-mat[4]*mat[6]);

	 //d=mat[0]*mat[4]*mat[8]+mat[1]*mat[5]*mat[6]+mat[2]*mat[3]*mat[7]
	 //-mat[0]*mat[5]*mat[7]-mat[1]*mat[3]*mat[8]-mat[2]*mat[4]*mat[6];
	return d;
}
REAL MatrixCalc::CreateJudgeD(REAL a, REAL b, REAL c, REAL xyz[])			//GRの節点がFEMモデルのどこにあるのかを計算する関数
{
	return -a*xyz[0]-b*xyz[1]-c*xyz[2];
}
bool MatrixCalc::judgeInOout(REAL a, REAL b, REAL c, REAL d, REAL target[])
{
	REAL dif;
	dif=a*target[0]+b*target[1]+c*target[2]+d;
	if(dif<0)
	{
		return false;
	}
	return true;
}

REAL MatrixCalc::CalcTriangleAreas(REAL p1[], REAL p2[], REAL p3[])
{
	REAL v1[3];
	REAL v2[3];
	for(int i=0 ; i<3; i++ )
	{
		v1[i]=p2[i]-p1[i];
		v2[i]=p3[i]-p1[i];
	}
	REAL e[3];

	e[0]=v1[1]*v2[2]-v1[2]*v2[1];
	e[1]=v1[2]*v2[0]-v1[0]*v2[2];
	e[2]=v1[0]*v2[1]-v1[1]*v2[0];

	return sqrt(e[0]*e[0]+e[1]*e[1]+e[2]*e[2])/2;

}
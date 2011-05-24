#include "Tetrahedron.h"
#include "MatrixCalc.h"
#include <iostream>
#include <vector>
#include "ThermalFEM.h"
#include "tetgen.h"

ThermalFEM::ThermalFEM()
{
	this->conductivity=0.0;
	this->density =0.0;
	this->c=0.0;
	this->dt=1.0;
	this->h=0.0;
	this->alpha=0.0;
	

	this->useRadiantHeat=false;
	this->usedRadiantHeat=false;
	this->useGenerationOfHeat=false;
	this->usedGenerationOfHeat=false;
	this->usedElem = false ;
	this->usedNode=false;

	this->qdirection=+1;					//directionの向きの設定 
	this->usedOptional=false;

	this->usedBorderCode=false;
	this->usedElemAreas= false;
	this->useTETGEN = false;
}


ThermalFEM::~ThermalFEM()
{
	if(this->usedNode){
		delete this->mat;
		delete this->temperature;
		delete this->K1;
		delete this->K2;
		if(this->usedRadiantHeat)
		{
			delete this->K3;
			delete this->F4;
		}
		
		delete this->CapaMat;
		delete this->CGlobal;
		delete this->FluxMat;

		delete this->F2;
		delete this->F3;
		

		if(this->usedGenerationOfHeat){
			delete this->Q;
			delete this->F1;
		}
		
		//delete this->out.pointlist;
	}
	/*if(this->usedElem)
	{
		delete this->out.tetrahedronlist;
	}*/
	if(this->usedOptional)
	{
		delete this->belong;
		delete this->optional;
	}
	if(this->usedElemAreas)
	{
		delete this->elemFaceAreas;
	}

	if(this->usedBorderCode)
	{
		delete this->borderCode;
	}
}

void ThermalFEM::SetTotalNodes(int totalNode)
{
	this->out.numberofpoints = totalNode;
	this->SetInit_relateNode(totalNode);
	
}

void ThermalFEM::SetInit_relateNode(int totalNode)
{
	//全体マトリックスの初期化
	//熱伝導マトリックスの初期化
	//熱容量マトリックスの初期化
	//熱流速マトリックスの初期化
	this->mat=new REAL[totalNode*totalNode];
	this->K1 = new REAL[totalNode*totalNode];
	this->initializeK1();
	this->K2 = new REAL[totalNode*totalNode];
	this->initializeK2();
	this->CapaMat = new REAL[totalNode*totalNode];
	this->CGlobal = new REAL[totalNode*totalNode];
	for(int i=0 ; i<totalNode*totalNode ; i++ )
	{
		mat[i] = 0 ;
		this->CapaMat[i]=0;
		this->CGlobal[i]=0;
	}

	//温度ベクトルの初期化
	//熱流束の全体ベクトルの初期化
	//節点座標を保存する行列の初期化
	//熱流束の式第二項F2の初期化
	this->temperature = new REAL[totalNode];
	this->FluxMat = new REAL[totalNode];
	if(!this->useTETGEN){
		this->out.pointlist = new REAL[totalNode*3];
	}
	this->F2 = new REAL[totalNode];
	this->F3 =new REAL[totalNode];
	for(int i=0 ; i<totalNode ; i++ )
	{
		this->temperature[i]=0.0;
		this->FluxMat[i]=0;
		this->F2[i]=0.0;
		this->F3[i]=0.0;
		if(!this->useTETGEN){
			for(int j=0 ; j<3 ; j++ )
			{
				this->out.pointlist[i*3+j]=0.0;
			}
		}
	}
	this->usedNode = true;

}

void ThermalFEM::SetTotalElems(int totalElem)
{
	this->out.numberoftetrahedra = totalElem;
	//要素の構成を保存する行列の初期化
	//this->out.tetrahedronlist = new int[totalElem*4];
	for(int i=0 ; i<totalElem ; i++)
	{	
		for(int j=0 ; j<4 ; j++)
		{
			this->out.tetrahedronlist[i*4+j];
		}
	}
	this->usedElem = true;
}

void ThermalFEM::SetElemAndNodeNum(int TotalElem ,int TotalNode)
{

	this->SetTotalNodes(TotalNode);
	this->SetTotalElems(TotalElem);
}

void ThermalFEM::SetInitialTemperature(REAL temp[])
{
	for(int i=0 ; i<this->out.numberofpoints ; i++)
	{
		this->temperature[i]=temp[i];
	}
}

void ThermalFEM::showGlobalMatrix()
{
	std::cout <<"Globalmatrix-------------------" <<std::endl;
	for(int i=0 ; i<this->out.numberofpoints*this->out.numberofpoints ; i++ )
	{
		std::cout <<mat[i]<<"\t";
		if(i%this->out.numberofpoints==(this->out.numberofpoints-1))
		{
			std::cout<<std::endl;
		}
	}
	std::cout <<"End Globalmatrix----------------" <<std::endl;
}

void ThermalFEM::showTemperature()
{
	for(int i=0 ; i<this->out.numberofpoints ; i++)
	{
		std::cout << i+1 <<"\t"<<this->temperature[i]<<std::endl;
	}
}

void ThermalFEM::SetQ(REAL *qi)
{
	if(!this->usedGenerationOfHeat)
	{
		this->useGenerationOfHeat=true;
		this->usedGenerationOfHeat=true;
		this->Q  = new REAL[this->out.numberoftetrahedra];		//呼ばれた時だけQを使えるようにする
		for(int i=0 ; i<this->out.numberoftetrahedra ; i++ )
		{
			this->Q[i]=0.0;
		}

		this->F1= new REAL[this->out.numberofpoints];
		for(int i=0 ; i<this->out.numberofpoints ; i++)
		{
			this->F1[i]=0;
		}
	}

	for(int i=0 ; i<this->out.numberoftetrahedra ; i++)
	{
		this->Q[i] = qi[i];
	}
}

void ThermalFEM::SetDT(REAL dt0)
{
	this->dt = dt0;
}

void ThermalFEM::SetDensity(REAL dens)
{
	this->density = dens;
}

void ThermalFEM::SetSpecificTemp(REAL st)
{
	this->c=st;
}
void ThermalFEM::SetHeatflux(REAL flux)
{
	this->q0=flux;
}
void ThermalFEM::SetHeatflux(REAL flux,int direction)
{
	this->q0=direction*flux;
}

void ThermalFEM::SetTransRatio(REAL ratio)
{
	this->alpha = ratio;
}

void ThermalFEM::SetRadiantHeat(REAL radiantHeat)
{
	this->h=radiantHeat;
	if(!this->usedRadiantHeat){
		this->useRadiantHeat=true;
		this->usedRadiantHeat=true;
		this->K3 = new REAL[this->out.numberofpoints*this->out.numberofpoints];
		this->initializeK3();
		this->F4 =new REAL[this->out.numberofpoints];
		this->initializeFluxVecNo4();
	}
	
}

void ThermalFEM::SetTfluid(REAL tfluid)
{
	this->Tfluid=tfluid;
}

void ThermalFEM::SetTout(REAL tout)
{
	this->Tout=tout;
}

void ThermalFEM::CreateK1Local(int elemNumber, REAL ktemp[])
{
	/*REAL *jinv;
	REAL detj;
	REAL *matk1;
	REAL *delNx;
	REAL *delNy;
	REAL *delNz;

	jinv= new REAL[9];
	matk1 = new REAL[16];
	delNx=new REAL[4];
	delNy=new REAL[4];
	delNz=new REAL[4];*/

	REAL jinv[9];//[A]にあたるところ
	REAL detj;//ヤコビアンの行列式
	REAL matk1[16];//一時保存用4*4マトリクス
	REAL delNx[4];//{Nx}
	REAL delNy[4];//{Ny}
	REAL delNz[4];//{Nz}

	/*int *n;
	n=new int[4];*/
	int n[4];

	this->GetNodeNumbers(elemNumber,n);

	//detj=6V　jはヤコビアン
	detj=tet.LocalDetJ(this->out.pointlist[n[0]*3],this->out.pointlist[n[1]*3],
					   this->out.pointlist[n[2]*3],this->out.pointlist[n[3]*3],
					   this->out.pointlist[n[0]*3+1],this->out.pointlist[n[1]*3+1],
					   this->out.pointlist[n[2]*3+1],this->out.pointlist[n[3]*3+1],
					   this->out.pointlist[n[0]*3+2],this->out.pointlist[n[1]*3+2],
					   this->out.pointlist[n[2]*3+2],this->out.pointlist[n[3]*3+2]);


	//局所座標系でのJ^(-1)にdetJをかけたものという意味で命名
	//新しくわたしたノートの｛A}にあたる部分を計算している
	tet.LocalInvJMultiplyDetJ(this->out.pointlist[n[0]*3],this->out.pointlist[n[1]*3],
					   this->out.pointlist[n[2]*3],this->out.pointlist[n[3]*3],
					   this->out.pointlist[n[0]*3+1],this->out.pointlist[n[1]*3+1],
					   this->out.pointlist[n[2]*3+1],this->out.pointlist[n[3]*3+1],
					   this->out.pointlist[n[0]*3+2],this->out.pointlist[n[1]*3+2],
					   this->out.pointlist[n[2]*3+2],this->out.pointlist[n[3]*3+2],jinv);
	

	//式⑤の{Nx}の計算
	delNx[0]=-jinv[0]-jinv[1]-jinv[2];
	delNx[1]=jinv[0];
	delNx[2]=jinv[1];
	delNx[3]=jinv[2];

	//式⑥の{Ny}の計算
	delNy[0]=-jinv[3]-jinv[4]-jinv[5];
	delNy[1]=jinv[3];
	delNy[2]=jinv[4];
	delNy[3]=jinv[5];

	//式⑦の{Nz}の計算
	delNz[0]=-jinv[6]-jinv[7]-jinv[8];
	delNz[1]=jinv[6];
	delNz[2]=jinv[7];
	delNz[3]=jinv[8];

	//{Km}の計算--------------------------------
	//{Nx}と{Nx}の転置ベクトルの掛け算をktempに保存
	mc.VecMultiplyTVec(delNx,delNx,4,ktemp);
	
	//{Ny}と{Ny}の転置ベクトルの掛け算をmatk1に保存
	mc.VecMultiplyTVec(delNy,delNy,4,matk1);
	
	//上の2つの行列の足し算
	mc.MatrixPlusMatrix(ktemp,matk1,4);
	
	//{Nz}と{Nz}の転置ベクトルの掛け算をmatk1に保存
	mc.VecMultiplyTVec(delNz,delNz,4,matk1);

	//上の2つの行列の足し算
	mc.MatrixPlusMatrix(ktemp,matk1,4);
	//{Km}の計算ここまで-----------------------

	//すべての要素にλ/36Vをかける。(detj=6V)
	for(int i=0 ; i<16 ; i++)
	{
		ktemp[i]*=this->conductivity/(6*detj);
	}
	/*delete jinv;
	delete matk1;
	delete delNx;
	delete delNy;
	delete delNz;
	delete n;*/
}

/***********************************

***********************************/
void ThermalFEM::CreateK1()
{
	
	REAL ktemp[16];	//局所座標系でのK1を一時保存
	int n[4];		//構成節点の座標を一時保存

	//要素数分の要素ごとのK1を計算し、全体座標系のK1に合成するループ
	for(int i=0 ; i<this->out.numberoftetrahedra ; i++ )
	{
		//局所座標系でのK1を算出
		this->CreateK1Local( i+1 ,ktemp);

		//要素を構成する節点を取得
		this->GetNodeNumbers((i+1),n);
		
		//算出した局所座標系でのK1を全体座標系に合成する。
		//1つの要素の剛性マトリックスを合成して、全体剛性マトリックスを作成する。
		for(int j=0; j<4 ; j++ ){
			for(int k=0 ; k<4 ; k++){
				this->K1[n[j]*this->out.numberofpoints+n[k]]+=ktemp[j*4+k];
			}
		}
	}
}

void ThermalFEM::CreateK2Local(int node1, int node2, int node3, int node4,REAL ktemp[])
{
	REAL ds = this->eta0ds(node1,node2,node3,node4);
	
	ktemp[0]=1.0/12*this->alpha*ds;
	ktemp[1]=1.0/24*this->alpha*ds;
	ktemp[2]=0.0;
	ktemp[3]=1.0/24*this->alpha*ds;
	
	ktemp[4]=1.0/24*this->alpha*ds;
	ktemp[5]=1.0/12*this->alpha*ds;
	ktemp[6]=0.0;
	ktemp[7]=1.0/24*this->alpha*ds;

	ktemp[8]=0.0;
	ktemp[9]=0.0;
	ktemp[10]=0.0;
	ktemp[11]=0.0;

	ktemp[12]=1.0/24*this->alpha*ds;
	ktemp[13]=1.0/24*this->alpha*ds;
	ktemp[14]=0.0;
	ktemp[15]=1.0/12*this->alpha*ds;
}

REAL ThermalFEM::eta0ds(int node1, int node2, int node3, int node4)
{
	REAL ds,m1,m2,m3;
	m1=(this->out.pointlist[3*(node2-1)+1]-this->out.pointlist[3*(node1-1)+1])*(this->out.pointlist[3*(node4-1)+2]-this->out.pointlist[3*(node1-1)+2])
		-(this->out.pointlist[3*(node2-1)+2]-this->out.pointlist[3*(node1-1)+2])*(this->out.pointlist[3*(node4-1)+1]-this->out.pointlist[3*(node1-1)+1]);
	
	m2=(this->out.pointlist[3*(node2-1)+2]-this->out.pointlist[3*(node1-1)+2])*(this->out.pointlist[3*(node4-1)]-this->out.pointlist[3*(node1-1)])
		-(this->out.pointlist[3*(node2-1)]-this->out.pointlist[3*(node1-1)])*(this->out.pointlist[3*(node4-1)+2]-this->out.pointlist[3*(node1-1)+2]);
	
	m3=(this->out.pointlist[3*(node2-1)]-this->out.pointlist[3*(node1-1)])*(this->out.pointlist[3*(node4-1)+1]-this->out.pointlist[3*(node1-1)+1])
		-(this->out.pointlist[3*(node2-1)+1]-this->out.pointlist[3*(node1-1)+1])*(this->out.pointlist[3*(node4-1)]-this->out.pointlist[3*(node1-1)]);
	ds=sqrt(m1*m1+m2*m2+m3*m3)/2;

	return ds;

}

REAL ThermalFEM::eta0ds(int nodes[])
{
	REAL ret;
	ret=this->eta0ds(nodes[0],nodes[1],nodes[2],nodes[3]);
	return ret;
}
void ThermalFEM::initializeK1()
{
	for( int i=0; i<this->out.numberofpoints*this->out.numberofpoints ; i++ )
	{
		this->K1[i] = 0;

	}
}

void ThermalFEM::initializeK2()
{
	for(int i=0 ; i<this->out.numberofpoints*this->out.numberofpoints ; i++ )
	{
		this->K2[i]=0;
	}
}

void ThermalFEM::CreateK2(int elemNumber, int node1, int node2, int node3)
{
	int k=0;
	/*int *n;
	n=new int[4];*/
	int n[4];

	this->checkSurface(elemNumber,node1,node2,node3,n);

	/*REAL *ktemp;
	ktemp = new REAL[16];*/
	REAL ktemp[16];
	this->CreateK2Local(n[0],n[1],n[2],n[3],ktemp);
	
	for(int i=0 ; i<4 ; i++)
	{
		for(int j=0 ; j<4 ; j++ )
		{
			this->K2[(n[i]-1)*this->out.numberofpoints+n[j]-1]+=ktemp[i*4+j];
		}
	}
	/*delete n;
	delete ktemp;*/
}

void ThermalFEM::CreateK3Local(int node1, int node2, int node3, int node4,REAL ktemp[])
{
	REAL ds = this->eta0ds(node1,node2,node3,node4);
	
	ktemp[0]=1.0/12*this->h*ds;
	ktemp[1]=1.0/24*this->h*ds;
	ktemp[2]=0.0;
	ktemp[3]=1.0/24*this->h*ds;
	
	ktemp[4]=1.0/24*this->h*ds;
	ktemp[5]=1.0/12*this->h*ds;
	ktemp[6]=0.0;
	ktemp[7]=1.0/24*this->h*ds;

	ktemp[8]=0.0;
	ktemp[9]=0.0;
	ktemp[10]=0.0;
	ktemp[11]=0.0;

	ktemp[12]=1.0/24*this->h*ds;
	ktemp[13]=1.0/24*this->h*ds;
	ktemp[14]=0.0;
	ktemp[15]=1.0/12*this->h*ds;

}

void ThermalFEM::initializeK3()
{
	for(int i=0 ; i<this->out.numberofpoints*this->out.numberofpoints ; i++ )
	{
		this->K3[i]=0;
	}
}

void ThermalFEM::CreateK3(int elemNumber, int node1, int node2, int node3)
{
	int k=0;
	//int *n;
	//n = new int[4];

	int n[4];
	//REAL *ktemp;
	//ktemp=new REAL[16];
	
	REAL ktemp[16];

	this->checkSurface(elemNumber,node1,node2,node3,n);

	this->CreateK3Local(n[0],n[1],n[2],n[3],ktemp);

	for(int i=0 ; i<4 ; i++ )
	{
		for(int j=0 ; j<4 ; j++ )
		{
			this->K3[(n[i]-1)*this->out.numberofpoints+n[j]-1]+=ktemp[i*4+j];
		}
	}
	/*delete n;
	delete ktemp;*/
}

void ThermalFEM::CreateCapacityLocal(int elemNumber, REAL ktemp[])
{
	REAL detj;
	/*int *n;
	n=new int[4];*/
	int n[4];

	this->GetNodeNumbers(elemNumber ,n);
	
	detj=tet.LocalDetJ(this->out.pointlist[n[0]*3],this->out.pointlist[n[1]*3],this->out.pointlist[n[2]*3],this->out.pointlist[n[3]*3],
					   this->out.pointlist[n[0]*3+1],this->out.pointlist[n[1]*3+1],this->out.pointlist[n[2]*3+1],this->out.pointlist[n[3]*3+1],
					   this->out.pointlist[n[0]*3+2],this->out.pointlist[n[1]*3+2],this->out.pointlist[n[2]*3+2],this->out.pointlist[n[3]*3+2]);
	
	for(int i=0; i<4 ; i++ )
	{
		for(int j=0 ; j<4 ; j++ )
		{
			if(i==j)
			{
				ktemp[i*4+j]=this->density*this->c*detj/60;
			}
			else
			{
				ktemp[i*4+j]=this->density*this->c*detj/120;
			}
		}
	}
	//delete n;
}

void ThermalFEM::CreateCapacityMat()
{
	/*REAL *ktemp;
	ktemp=new REAL[16];*/
	REAL ktemp[16];
//	int n1,n2,n3,n4;
	int *n;
	n=new int[4];
	for(int i=0 ; i<this->out.numberoftetrahedra ; i++ )
	{
		this->CreateCapacityLocal( (i+1) , ktemp);
		this->GetNodeNumbers((i+1),n);

		for(int j=0; j<4 ; j++)
		{
			for(int k=0 ; k<4 ; k++)
			{
				this->CapaMat[n[j]*this->out.numberofpoints+n[k]]+=ktemp[j*4+k];
			}
		}
	}
	//delete ktemp;
	delete n;
}
void ThermalFEM::initializeCapacityMat()
{
	for(int i=0; i<this->out.numberofpoints ; i++ )
	{
		this->CapaMat[i]=0;
	}
}

void ThermalFEM::CreateQVecLocal(int elemNumber, REAL temp[],int n[])
{
	REAL detj;
	this->GetNodeNumbers(elemNumber, n);

	detj=tet.LocalDetJ(this->out.pointlist[n[0]*3],this->out.pointlist[n[1]*3],this->out.pointlist[n[2]*3],this->out.pointlist[n[3]*3],
				  this->out.pointlist[n[0]*3+1],this->out.pointlist[n[1]*3+1],this->out.pointlist[n[2]*3+1],this->out.pointlist[n[3]*3+1],
				  this->out.pointlist[n[0]*3+2],this->out.pointlist[n[1]*3+2],this->out.pointlist[n[2]*3+2],this->out.pointlist[n[3]*3+2]);
	
	for(int i=0 ;i<4 ; i++ )
	{
		temp[i]=detj*this->Q[elemNumber-1]/24.0;
	}
}

void ThermalFEM::CreateHeatFluxVecLocalDS(int nodeNums[], REAL temp[])
{
	REAL ds;
	ds=this->eta0ds(nodeNums);
	temp[0]=ds/6;
	temp[1]=ds/6;
	temp[2]=0;
	temp[3]=ds/6;
}
void ThermalFEM::CreateHeatFluxVecNo2(int elem, int node1, int node2, int node3)
{
	/*int *n;
	n=new  int[4];
	REAL *temp;
	temp=new REAL[4];*/

	int n[4];
	REAL temp[4];

	this->checkSurface(elem,node1,node2,node3,n);
	this->CreateHeatFluxVecLocalDS(n,temp);

	for(int i=0 ; i<4 ;i++ )
	{
		this->F2[n[i]-1]+=temp[i]*this->q0;
	}
	/*delete n;
	delete temp;*/
}

void ThermalFEM::CreateHeatFluxVecNo2(int elem, int node1, int node2, int node3, REAL qelem)
{
	/*int *n;
	n=new int[4];
	REAL *temp;
	temp=new REAL[4];*/
	int n[4];
	REAL temp[4];

	this->checkSurface(elem,node1,node2,node3,n);
	this->CreateHeatFluxVecLocalDS(n,temp);

	for(int i=0 ; i<4 ;i++ )
	{
		this->F2[n[i]-1]+=temp[i]*qelem;
	}
	//delete n;
	//delete temp;
}
void ThermalFEM::checkSurface(int elemNumber, int node1, int node2, int node3, int n[])
{
	int k=0;
	for(int i=0 ; i<4 ; i++)
	{
		if(this->out.tetrahedronlist[4*(elemNumber-1)+i]==node1)
		{
			k+=i;
			break;
		}
	}

	for(int i=0 ; i<4 ; i++)
	{
		if(this->out.tetrahedronlist[4*(elemNumber-1)+i]==node2)
		{
			k+=i;
			break;
		}
	}

	for(int i=0 ; i<4 ; i++)
	{
		if(this->out.tetrahedronlist[4*(elemNumber-1)+i]==node3)
		{
			k+=i;
			break;
		}
	}
	switch(k)
	{
	case 3:
		n[0]=this->out.tetrahedronlist[4*(elemNumber-1)];
		n[1]=this->out.tetrahedronlist[4*(elemNumber-1)+2];
		n[2]=this->out.tetrahedronlist[4*(elemNumber-1)+3];
		n[3]=this->out.tetrahedronlist[4*(elemNumber-1)+1];
		break;
	case 4:
		n[0]=this->out.tetrahedronlist[4*(elemNumber-1)];
		n[1]=this->out.tetrahedronlist[4*(elemNumber-1)+1];
		n[2]=this->out.tetrahedronlist[4*(elemNumber-1)+2];
		n[3]=this->out.tetrahedronlist[4*(elemNumber-1)+3];
		break;
	case 5:
		n[0]=this->out.tetrahedronlist[4*(elemNumber-1)];
		n[1]=this->out.tetrahedronlist[4*(elemNumber-1)+3];
		n[2]=this->out.tetrahedronlist[4*(elemNumber-1)+1];
		n[3]=this->out.tetrahedronlist[4*(elemNumber-1)+2];
		break;
	case 6:
		n[0]=this->out.tetrahedronlist[4*(elemNumber-1)+1];
		n[1]=this->out.tetrahedronlist[4*(elemNumber-1)+2];
		n[2]=this->out.tetrahedronlist[4*(elemNumber-1)];
		n[3]=this->out.tetrahedronlist[4*(elemNumber-1)+3];
		break;
	default:
		break;
	}

}

void ThermalFEM::CreateHeatFluxVecNo3(int elem, int node1, int node2, int node3)
{/*
	REAL *temp;
	temp=new REAL[4];
	int *n;
	n=new int[4];*/

	REAL temp[4];
	int n[4];


	this->checkSurface(elem,node1,node2,node3,n);
	this->CreateHeatFluxVecLocalDS(n,temp);

	for(int i=0 ; i<4 ; i++ )
	{
		this->F3[n[i]]+=temp[i]*this->alpha*this->Tfluid;
	}
	//delete temp;
	//delete n;
}
void ThermalFEM::CreateHeatFluxVecNo3(int elem, int node1,int node2,int node3,REAL tfluid)
{
	//
	//REAL *temp;
	//temp=new REAL[4];
	//int *n;
	//n=new int[4];

	
	REAL temp[4];
	int n[4];
	this->checkSurface(elem,node1,node2,node3,n);
	this->CreateHeatFluxVecLocalDS(n,temp);

	for(int i=0 ; i<4 ; i++ )
	{
		this->F3[n[i]]+=temp[i]*this->alpha*tfluid;
	}
	//delete temp;
	//delete n;
}

void ThermalFEM::CreateHeatFluxVecNo4(int elem, int node1, int node2, int node3)
{
	if(this->usedRadiantHeat)
	{/*
		REAL *temp;
		temp=new REAL[4];
		int *n;
		n=new int[4];
	*/	
		
		REAL temp[4];
		int n[4];
	

		this->checkSurface(elem,node1,node2,node3,n);
		this->CreateHeatFluxVecLocalDS(n,temp);

		for(int i=0 ; i<4; i++ )
		{
			this->F4[n[i]]+=temp[i]*this->h*this->Tout;
		}
		//delete temp;
		//delete n;
	}
}

void ThermalFEM::CreateHeatFluxVecNo4(int elem, int node1, int node2, int node3, REAL tout)
{
	/*REAL *temp;
	temp=new REAL[4];
	int *n;
	n=new int[4];*/

	REAL temp[4];
	int n[4];
	
	this->checkSurface(elem,node1,node2,node3,n);
	this->CreateHeatFluxVecLocalDS(n,temp);

	for(int i=0 ; i<4; i++ )
	{
		this->F4[n[i]]+=temp[i]*this->h*tout;
	}
	//delete temp;
	//delete n;
}

void ThermalFEM::CreateHeatFluxVecNo1()
{
	if(this->useGenerationOfHeat && this->usedGenerationOfHeat){
		for(int i=0 ; i<this->out.numberoftetrahedra ; i++ )
		{
			if(this->Q[i]!=0)
			{
				this->CreateHeatFluxVecNo1(i+1);
			}
		}
	}
}

void ThermalFEM::CreateHeatFluxVecNo1(int elem)
{
	/*REAL *temp;
	temp=new REAL[4];
	int *n;
	n=new int[4];*/
	REAL temp[4];
	int n[4];
	
	this->CreateQVecLocal(elem,temp,n);

	for(int i=0; i<4 ; i++)
	{
		this->F1[n[i]]+=temp[i];
	}
	//delete temp;
	//delete n;

}
void ThermalFEM::CreateHeatFluxVecNo1(int elem, REAL genHeat)
{
	if(!this->usedGenerationOfHeat)
	{
		this->Q = new REAL[this->out.numberoftetrahedra];
		for(int i=0 ; i<this->out.numberoftetrahedra ; i++ )
		{
			this->Q[i]=0;
		}
		this->usedGenerationOfHeat=true;
		this->useGenerationOfHeat =true;

	}
	this->Q[elem-1]=genHeat;
	this->CreateHeatFluxVecNo1(elem);
}

void ThermalFEM::CreateGlobalMatrixAndVec()
{
	//左辺-----------------------------------------------------
	mc.CopyMatrix(this->K1,this->mat,this->out.numberofpoints);
	mc.MatrixPlusMatrix(this->mat,this->K2,this->out.numberofpoints);
	if(this->useRadiantHeat)
	{
		mc.MatrixPlusMatrix(this->mat,this->K3,this->out.numberofpoints);
	}

	for(int i=0; i<this->out.numberofpoints*this->out.numberofpoints ; i++)
	{
		this->CGlobal[i]=this->CapaMat[i]/this->dt;
	}
	mc.MatrixPlusMatrix(this->mat,this->CGlobal,this->out.numberofpoints);

	//左辺---------------------------------------------------------------

	//右辺-----------------------------------------------------------
	REAL *ftemp;
	ftemp = new REAL[this->out.numberofpoints];
	mc.MatrixMultiplyVec(this->CGlobal,this->temperature,this->out.numberofpoints,ftemp);

	mc.CopyVector(ftemp,this->FluxMat,this->out.numberofpoints);

	if(this->useGenerationOfHeat)
	{
		mc.AddVec2ToVec1(this->FluxMat,this->F1,this->out.numberofpoints);
	}
	if(this->qdirection<0){
		mc.AddVec2ToVec1(this->FluxMat,this->F2,this->out.numberofpoints);
	}else{
		mc.SubstructVec2FromVec1(this->FluxMat,this->F2,this->out.numberofpoints);
	}

	mc.AddVec2ToVec1(this->FluxMat,this->F3,this->out.numberofpoints);

	if(this->usedRadiantHeat){
		mc.AddVec2ToVec1(this->FluxMat,this->F4,this->out.numberofpoints);
	}
	delete ftemp;
	//右辺--------------------------------------------------------------

}

void ThermalFEM::CalcStartGaussJordan()
{
	//方程式を解く
	mc.GaussJordan(this->mat,this->FluxMat,this->out.numberofpoints);

	//温度の行列を更新する。
	mc.CopyVector(this->FluxMat,this->temperature,this->out.numberofpoints);
}
void ThermalFEM::CalcStartGaussJordanUsePivot()
{
	mc.GaussJordanUsePivot(this->mat,this->FluxMat,this->out.numberofpoints);
	mc.CopyVector(this->FluxMat,this->temperature,this->out.numberofpoints);
}
void ThermalFEM::CalcStartLUSolve()
{
	mc.LUsolve(this->mat,this->FluxMat,this->temperature,this->out.numberofpoints);
}

void ThermalFEM::GetNodeNumbers(int elemNumber, int n[])
{
	n[0]=this->out.tetrahedronlist[(elemNumber-1)*4]-1;
	n[1]=this->out.tetrahedronlist[(elemNumber-1)*4+1]-1;
	n[2]=this->out.tetrahedronlist[(elemNumber-1)*4+2]-1;
	n[3]=this->out.tetrahedronlist[(elemNumber-1)*4+3]-1;	
}

void ThermalFEM::SetTemperature(REAL temp, int nodeNumber)
{
	this->temperature[nodeNumber-1]=temp;
}

void ThermalFEM::FixTemperature(REAL temp, int nodeNumber)
{
	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{
		if(i==(nodeNumber-1))
		{
			this->mat[(nodeNumber-1)*this->out.numberofpoints+i]=1;
		}
		else
		{
			this->mat[(nodeNumber-1)*this->out.numberofpoints+i]=0;
		}
	}
	this->FluxMat[nodeNumber-1]=temp;
}

void ThermalFEM::initializeFluxVecNo1()
{
	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{
		this->F1[i]=0;
	}
}


void ThermalFEM::initializeFluxVecNo2()
{
	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{
		this->F2[i]=0;
	}
}


void ThermalFEM::initializeFluxVecNo3()
{
	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{
		this->F3[i]=0;
	}
}

void ThermalFEM::initializeFluxVecNo4()
{
	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{
		this->F4[i]=0;
	}
} 
void ThermalFEM::detectBorder()
{
	if(!this->usedBorderCode)
	{
		this->borderCode = new int[this->out.numberoftetrahedra];
		this->usedBorderCode=true;
	}
	this->detectBorder(this->borderCode);
}

void ThermalFEM::detectBorder(int s[])
{
	
	//要素を構成する面を登録する行列の初期化-------------
	//面が何回使われているかを探して、共有している面と外殻を探している
	int ***Surfaces;
	Surfaces=new int**[this->out.numberofpoints];
	for(int i=0; i<this->out.numberofpoints ; i++ )
	{
		Surfaces[i]=new int*[this->out.numberofpoints];
	}
	for(int i=0;i<this->out.numberofpoints ; i++ )
	{
		for(int j=0; j<this->out.numberofpoints; j++ )
		{
			Surfaces[i][j]= new int[this->out.numberofpoints];
		}
	}
	for( int i=0; i<this->out.numberofpoints; i++)
	{
		for( int j=0; j< this->out.numberofpoints; j++)
		{
			for( int k=0; k< this->out.numberofpoints; k++)
			{
				Surfaces[i][j][k]=0;
			}
		}
		
	}
	//end 要素を構成する面を登録する行列の初期化---------

	for(int i=0; i<this->out.numberoftetrahedra ; i++ )
	{
		s[i]=0;
	}

	//要素を構成する面を探す
	//
	int n[4];
	int surf;
	for( int i=0 ; i< this->out.numberoftetrahedra ; i++ )
	{
		this->GetNodeNumbers((i+1),n);
		this->sortInt(n,4);
		//1,2,3
		if(Surfaces[n[0]][n[1]][n[2]]==0)
		{
			Surfaces[n[0]][n[1]][n[2]]=i+1;
		}
		else
		{
			Surfaces[n[0]][n[1]][n[2]]=0;
		}
		//1,2,4
		if(Surfaces[n[0]][n[1]][n[3]]==0)
		{
			Surfaces[n[0]][n[1]][n[3]]=i+1;
		}
		else
		{
			Surfaces[n[0]][n[1]][n[3]]=0;
		}
		//1,3,4
		if(Surfaces[n[0]][n[2]][n[3]]==0)
		{
			Surfaces[n[0]][n[2]][n[3]]=i+1;
		}
		else
		{
			Surfaces[n[0]][n[2]][n[3]]=0;
		}
		//1,3,4
		if(Surfaces[n[1]][n[2]][n[3]]==0)
		{
			Surfaces[n[1]][n[2]][n[3]]=i+1;
		}
		else
		{
			Surfaces[n[1]][n[2]][n[3]]=0;
		}
	}
	

	for(int i =0; i< this->out.numberofpoints-2; i++)
	{
		for(int j =1; j< this->out.numberofpoints-1; j++)
		{
			for(int k =2; k< this->out.numberofpoints; k++)
			{
				if(Surfaces[i][j][k] !=0)
				{
					surf = this->checkSurface(Surfaces[i][j][k],i,j,k);
					switch(surf){
						case 0:
							s[Surfaces[i][j][k]-1]+=1;
							break;
						case 1:
							s[Surfaces[i][j][k]-1]+=10;
							break;
						case 2:
							s[Surfaces[i][j][k]-1]+=100;
							break;
						case 3:
							s[Surfaces[i][j][k]-1]+=1000;
							break;
						default:
							break;

					}
				}
			}
		}
	}
	

	//delete---------------------------------------------
	for(int i=0;i<this->out.numberofpoints ; i++ )
	{
		for(int j=0; j<this->out.numberofpoints; j++ )
		{
			delete Surfaces[i][j];
		}
	}
	for(int i=0; i<this->out.numberofpoints ; i++ )
	{
		delete Surfaces[i];
	}
	delete Surfaces;
}

void ThermalFEM::sortInt(int n[], int num)
{
	int temp;
	for ( int i=0; i<num-1 ; i++)
	{
		for(int j=num-1;j>i ; j--)
		{
			if(n[j]<n[j-1])
			{
				temp=n[j];
				n[j]=n[j-1];
				n[j-1]=temp;
			}
		}
	}
}
int ThermalFEM::checkSurface(int elemNumber, int node1, int node2, int node3)
{
	//kato
	//１要素内の１面を構成する節点の組み合わせは、(0,1,2),(0,1,3),(0,2,3),(1,2,3)の4組
	//kの値はそれぞれ+3,+4,+5,+6されるので、k=-3から始まるとk=0,1,2,3のどれかを返す
	int k=-3;
	for(int i=0 ; i<4 ; i++)
	{
		if(this->out.tetrahedronlist[4*(elemNumber-1)+i]==node1)
		{
			k+=i;
			break;
		}
	}

	for(int i=0 ; i<4 ; i++)
	{
		if(this->out.tetrahedronlist[4*(elemNumber-1)+i]==node2)
		{
			k+=i;
			break;
		}
	}

	for(int i=0 ; i<4 ; i++)
	{
		if(this->out.tetrahedronlist[4*(elemNumber-1)+i]==node3)
		{
			k+=i;
			break;
		}
	}
	return k;
}

void ThermalFEM::GetGlobalMatrix(REAL ret[])
{
	mc.CopyMatrix(this->mat,ret,this->out.numberofpoints);
}
void ThermalFEM::GetK1Matrix(REAL ret[])
{
	mc.CopyMatrix(this->K1,ret,this->out.numberofpoints);
}
void ThermalFEM::GetK2Matrix(REAL ret[])
{
	mc.CopyMatrix(this->K2,ret,this->out.numberofpoints);
}
void ThermalFEM::GetK3Matrix(REAL ret[])
{
	mc.CopyMatrix(this->K3,ret,this->out.numberofpoints);
}
void ThermalFEM::GetCapacityMatrix(REAL ret[])
{
	mc.CopyMatrix(this->CapaMat,ret,this->out.numberofpoints);
}
void ThermalFEM::GetGlobalFluxVector(REAL ret[])
{
	mc.CopyVector(this->FluxMat,ret,this->out.numberofpoints);
}
void ThermalFEM::GetHeatFlux1Vector(REAL ret[])
{
	if(this->usedGenerationOfHeat)
		mc.CopyVector(this->F1,ret,this->out.numberofpoints);
}
void ThermalFEM::GetHeatFlux2Vector(REAL ret[])
{
	mc.CopyVector(this->F2,ret,this->out.numberofpoints);
}
void ThermalFEM::GetHeatFlux3Vector(REAL ret[])
{
	mc.CopyVector(this->F3,ret,this->out.numberofpoints);
}
void ThermalFEM::GetHeatFlux4Vector(REAL ret[])
{
	if(this->useRadiantHeat)
	mc.CopyVector(this->F4,ret,this->out.numberofpoints);
}
void ThermalFEM::GetTemperature(REAL ret[])
{
	mc.CopyVector(this->temperature,ret,this->out.numberofpoints);
}

void ThermalFEM::SetANodeCordinates(int nodeNumber, REAL x, REAL y, REAL z)
{
	if(!this->usedNode)
	{
		std::cout << "Error! 節点数が登録されていません";
	}else{
		this->out.pointlist[(nodeNumber-1)*3]=x;
		this->out.pointlist[(nodeNumber-1)*3+1]=y;
		this->out.pointlist[(nodeNumber-1)*3+2]=z;
	}
}

void ThermalFEM::SetAnElementMember(int elementNumber,int node1, int node2, int node3, int node4)
{
	this->out.tetrahedronlist[(elementNumber-1)*4]=node1;
	this->out.tetrahedronlist[(elementNumber-1)*4+1]=node2;
	this->out.tetrahedronlist[(elementNumber-1)*4+2]=node3;
	this->out.tetrahedronlist[(elementNumber-1)*4+3]=node4;
}

void ThermalFEM::GetExteriorProduct(int elementNumber, int node1, int node2, int node3, REAL ret[])
{
	int k=0;
	int n[3];
	for(int i=0; i<4; i++)
	{
		if(this->out.tetrahedronlist[4*(elementNumber-1)+i]==node1)
		{
			k=i;
			break;
		}
	}
	for(int i=0; i<4; i++)
	{
		if(this->out.tetrahedronlist[4*(elementNumber-1)+i]==node2)
		{
			k+=i;
			break;
		}
	}
	for(int i=0; i<4; i++)
	{
		if(this->out.tetrahedronlist[4*(elementNumber-1)+i]==node3)
		{
			k+=i;
			break;
		}
	}
	switch(k)
	{
	case 3:
		n[0]=this->out.tetrahedronlist[(elementNumber-1)*4]-1;
		n[1]=this->out.tetrahedronlist[(elementNumber-1)*4+1]-1;
		n[2]=this->out.tetrahedronlist[(elementNumber-1)*4+2]-1;
		break;
	case 4:
		n[0]=this->out.tetrahedronlist[(elementNumber-1)*4]-1;
		n[1]=this->out.tetrahedronlist[(elementNumber-1)*4+3]-1;
		n[2]=this->out.tetrahedronlist[(elementNumber-1)*4+1]-1;
		break;
	case 5:
		n[0]=this->out.tetrahedronlist[(elementNumber-1)*4]-1;
		n[1]=this->out.tetrahedronlist[(elementNumber-1)*4+3]-1;
		n[2]=this->out.tetrahedronlist[(elementNumber-1)*4+2]-1;
		break;
	case 6:
		n[0]=this->out.tetrahedronlist[(elementNumber-1)*4+1]-1;
		n[1]=this->out.tetrahedronlist[(elementNumber-1)*4+3]-1;
		n[2]=this->out.tetrahedronlist[(elementNumber-1)*4+2]-1;
		break;
	default:
		break;
	}
	REAL v1[3];
	REAL v2[3];

	v1[0]=this->out.pointlist[3*n[2]]-this->out.pointlist[3*n[0]];
	v1[1]=this->out.pointlist[3*n[2]+1]-this->out.pointlist[3*n[0]+1];
	v1[2]=this->out.pointlist[3*n[2]+2]-this->out.pointlist[3*n[0]+2];

	v2[0]=this->out.pointlist[3*n[1]]-this->out.pointlist[3*n[0]];
	v2[1]=this->out.pointlist[3*n[1]+1]-this->out.pointlist[3*n[0]+1];
	v2[2]=this->out.pointlist[3*n[1]+2]-this->out.pointlist[3*n[0]+2];

	mc.ExteriorProduct(v1,v2,ret);
	REAL LineLength;
	LineLength=mc.GetVectorLength(ret);
	//for(int i=0; i<3 ; i++ )
	//{
	//	//ret[i]/=LineLength;
	//}
}

void ThermalFEM::GetAnElementMember(int elementNumber, int n[])
{
	for(int i=0 ; i<4 ; i++ )
	{
		n[i]=this->out.tetrahedronlist[(elementNumber-1)*4+i];
	}
}

void ThermalFEM::SetConductivity(REAL conduct)
{
	this->conductivity = conduct;
}

void ThermalFEM::SetPointlist(REAL pointlist[])
{
	this->mc.CopyVector(pointlist,this->out.pointlist,this->out.numberofpoints*3);
}
void ThermalFEM::SetTetrahedronlist(int tetrahedronlist[])
{
	this->mc.CopyVector(tetrahedronlist,this->out.tetrahedronlist,this->out.numberoftetrahedra*4);
}
void ThermalFEM::SetTetrahedronlistStartWithZero(int tetrahedronlist[])
{
	for(int i=0 ; i< this->out.numberoftetrahedra*4 ; i++ )
	{
		this->out.tetrahedronlist[i] = tetrahedronlist[i]+1;
	}
}

void ThermalFEM::GetElementFace(int elemNumber, int faceNumber, int n[])
{
	int num = elemNumber-1;

	switch(faceNumber)
	{
	case 1:
		n[0]=this->out.tetrahedronlist[num*4];
		n[1]=this->out.tetrahedronlist[num*4+1];
		n[2]=this->out.tetrahedronlist[num*4+2];
		break;
	case 2:
		
		n[0]=this->out.tetrahedronlist[num*4];
		n[1]=this->out.tetrahedronlist[num*4+1];
		n[2]=this->out.tetrahedronlist[num*4+3];
		break;
	case 3:
		
		n[0]=this->out.tetrahedronlist[num*4];
		n[1]=this->out.tetrahedronlist[num*4+2];
		n[2]=this->out.tetrahedronlist[num*4+3];
		break;
	case 4:
		
		n[0]=this->out.tetrahedronlist[num*4+1];
		n[1]=this->out.tetrahedronlist[num*4+2];
		n[2]=this->out.tetrahedronlist[num*4+3];
		break;
	default:
		break;
	}
	
}

void ThermalFEM::showK1()
{
	std::cout<<"\nK1 Matrix--------------------------"<<std::endl;
	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{
		for(int j=0 ; j<this->out.numberofpoints ; j++ )
		{
			std::cout<<this->K1[i*this->out.numberofpoints+j]<<"\t";
		}
		std::cout<<std::endl;
	}
	std::cout<<"\nK1 Matrix--------------------------"<<std::endl;
	std::cout<<std::endl;
}
void ThermalFEM::showK2()
{
	std::cout<<"\nK2 Matrix--------------------------"<<std::endl;
	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{
		for(int j=0 ; j<this->out.numberofpoints ; j++ )
		{
			std::cout<<this->K2[i*this->out.numberofpoints+j]<<"\t";
		}
		std::cout<<std::endl;
	}
	std::cout<<"\nK2 Matrix--------------------------"<<std::endl;
	std::cout<<std::endl;
}
void ThermalFEM::showK3()
{
	if(this->usedRadiantHeat)
	{
		std::cout<<"\nK3 Matrix--------------------------"<<std::endl;
		for(int i=0 ; i<this->out.numberofpoints ; i++ )
		{
			for(int j=0 ; j<this->out.numberofpoints ; j++ )
			{
				std::cout<<this->K3[i*this->out.numberofpoints+j]<<"\t";
			}
			std::cout<<std::endl;
		}
		std::cout<<"\nK3 Matrix--------------------------"<<std::endl;
		std::cout<<std::endl;
	}
	else
	{
		std::cout<<"K3 熱輻射項は利用されていません。"<<std::endl;
	}
}
void ThermalFEM::showCapacityMat()
{
	std::cout<<"\nCapacity Matrix--------------------------"<<std::endl;
	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{
		for(int j=0 ; j<this->out.numberofpoints ; j++ )
		{
			std::cout<< this->CapaMat[i*this->out.numberofpoints+j]<<"\t";
		}
		std::cout<<std::endl;
	}
	std::cout<<"\nCapacityMat Matrix--------------------------"<<std::endl;
	std::cout<<std::endl;

}
void ThermalFEM::showF2()
{
	std::cout << "F2 Vector------------------------------------"<<std::endl;
	for(int i=0; i<this->out.numberofpoints ; i++ )
	{
		std::cout<<i<<"\t"<<this->F2[i]<<std::endl;

	}
	std::cout << "F2 Vector end------------------------------------"<<std::endl;

}
void ThermalFEM::showF1()
{
	if(this->useGenerationOfHeat){
		std::cout << "F1 Vector------------------------------------"<<std::endl;
		for(int i=0; i<this->out.numberofpoints ; i++ )
		{
			std::cout<<i<<"\t"<<this->F1[i]<<std::endl;

		}
		std::cout << "F1 Vector end------------------------------------"<<std::endl;
	}
	else
	{
		std::cout<<"F1 内部発熱は使われていません。"<<std::endl;
	}

}
void ThermalFEM::showF3()
{
	std::cout << "F1 Vector------------------------------------"<<std::endl;
	for(int i=0; i<this->out.numberofpoints ; i++ )
	{
		std::cout<<i<<"\t"<<this->F3[i]<<std::endl;

	}
	std::cout << "F3 Vector end------------------------------------"<<std::endl;
}
void ThermalFEM::showF4()
{
	if(this->usedRadiantHeat){
		std::cout << "F4 Vector------------------------------------"<<std::endl;
		for(int i=0; i<this->out.numberofpoints ; i++ )
		{
			std::cout<<i<<"\t"<<this->F4[i]<<std::endl;

		}
		std::cout << "F4 Vector end------------------------------------"<<std::endl;
	}
	else
	{
		std::cout<<"F4 熱輻射は利用されていません。\n"<<std::endl;
	}
}

void ThermalFEM::showGlobalFluxVector()
{
	std::cout << "GlobalVector------------------------------------"<<std::endl;
	for(int i=0; i<this->out.numberofpoints ; i++ )
	{
		std::cout<<i<<"\t"<<this->FluxMat[i]<<std::endl;

	}
	std::cout << "GlobalVector end------------------------------------"<<std::endl;
}
void ThermalFEM::SetHeatfluxDirection(int direction)
{
	this->qdirection=direction;
}

//////////////////////////////////////////////////////////////////////////////////
//				以下内外任意の点の値をとるための関数							//
//				内外判定もここ													//
//////////////////////////////////////////////////////////////////////////////////
int ThermalFEM::SerchNearestPoint(REAL x, REAL y, REAL z)
{
	int ret;
	/*REAL *v;
	v=new REAL[3];*/
	REAL v[3];
	REAL l=1000.0;			//初期値　適当な値
	REAL len=0;

	for(int i=0 ; i<this->out.numberofpoints ; i++ )
	{//全てのFEM節点に対し、
		v[0]=this->out.pointlist[i*3]-x;
		v[1]=this->out.pointlist[i*3+1]-y;
		v[2]=this->out.pointlist[i*3+2]-z;
		len=mc.GetVectorLength(v);
		if(len < l )			//GRTMesh上の節点とFEM節点との距離lenがlよりも大きい＝遠かったら、lをlenに更新し、retにその時のFEMの節点番号を追加する //len>lを訂正、len>0.0を追加
		{
			l=len;
			ret=i;
		}
	}
	//delete v;
	return ret;
}

void ThermalFEM::PrepareforGetOptionalPoint(int totalofOptionalPoint)
{
	this->usedOptional=true;
	this->totalOP=totalofOptionalPoint;
	this->optional = new OptionalPoint[totalofOptionalPoint];
	this->belong   = new Belong[this->out.numberofpoints];

	for(int i=0; i<this->out.numberoftetrahedra*4 ; i++ )
	{
		this->belong[this->out.tetrahedronlist[i]-1].element.push_back((i/4+1));
	}
}
void ThermalFEM::SetOptionalPointCoordinates(int number, REAL x, REAL y, REAL z)
{
	this->optional[number].x=x;
	this->optional[number].y=y;
	this->optional[number].z=z;
}
void ThermalFEM::SetOptionalPointCoordinates(int number, REAL xyz[])
{
	this->optional[number].x=xyz[0];
	this->optional[number].y=xyz[1];
	this->optional[number].z=xyz[2];
}
void ThermalFEM::SetOptionalPointCoordinates(REAL AllCoodinates[])
{
	for(int i=0 ; i<this->totalOP ; i++ )
	{
		this->optional[i].x=AllCoodinates[0];
		this->optional[i].y=AllCoodinates[1];
		this->optional[i].z=AllCoodinates[2];
	}
}
void ThermalFEM::SerchAnOptionalPointBelong(int optionalPointNumber)
{
	bool inElem=false;
	int nearNode;
	int element=0;
	int n1,n2,n3,n4;
	/*REAL *target;
	target=new REAL[3];
	REAL *v1;
	v1=new REAL[3];
	REAL *v2;
	v2=new REAL[3];
	REAL *v3;
	v3=new REAL[3];
	REAL *v4;
	v4=new REAL[3];*/

	REAL target[3];
	REAL v1[3];
	REAL v2[3];
	REAL v3[3];
	REAL v4[3];
	
	//GRTMeshの節点の座標に近いFEM節点を探し、nearNodeに代入する
	nearNode=this->SerchNearestPoint(this->optional[optionalPointNumber].x,this->optional[optionalPointNumber].y,this->optional[optionalPointNumber].z);	

	for(int i=0 ; i<this->belong[nearNode].element.size() ; i++ )			//このnearNode節点が入っているelementsの数だけ、処理
	{
		n1=this->out.tetrahedronlist[(this->belong[nearNode].element[i]-1)*4]-1;
		n2=this->out.tetrahedronlist[(this->belong[nearNode].element[i]-1)*4+1]-1;
		n3=this->out.tetrahedronlist[(this->belong[nearNode].element[i]-1)*4+2]-1;
		n4=this->out.tetrahedronlist[(this->belong[nearNode].element[i]-1)*4+3]-1;

		target[0]=this->optional[optionalPointNumber].x;
		target[1]=this->optional[optionalPointNumber].y;
		target[2]=this->optional[optionalPointNumber].z;

		for(int j=0 ; j<3 ; j++ )
		{
			v1[j]=this->out.pointlist[n1*3+j];
			v2[j]=this->out.pointlist[n2*3+j];
			v3[j]=this->out.pointlist[n3*3+j];
			v4[j]=this->out.pointlist[n4*3+j];
		}

		inElem=this->InOrOut(target,v1,v2,v3,v4);
		if(inElem)
		{
			element=i+1;
			break;
		}
	}
	if(!inElem)
	{
		REAL len=0.0;
		REAL prelen=0.0;
		bool first=true;
		for(int i=0 ; i<this->belong[nearNode].element.size() ; i++ )
		{
			n1=this->out.tetrahedronlist[(this->belong[nearNode].element[i]-1)*4]-1;
			n2=this->out.tetrahedronlist[(this->belong[nearNode].element[i]-1)*4+1]-1;
			n3=this->out.tetrahedronlist[(this->belong[nearNode].element[i]-1)*4+2]-1;
			n4=this->out.tetrahedronlist[(this->belong[nearNode].element[i]-1)*4+3]-1;

			target[0]=this->optional[optionalPointNumber].x;
			target[1]=this->optional[optionalPointNumber].y;
			target[2]=this->optional[optionalPointNumber].z;

			for(int j=0 ; j<3 ; j++ )
			{
				v1[j]=this->out.pointlist[n1*3+j];
				v2[j]=this->out.pointlist[n2*3+j];
				v3[j]=this->out.pointlist[n3*3+j];
				v4[j]=this->out.pointlist[n4*3+j];
			}
			mc.SubstructVec2FromVec1(v1,target,3);
			mc.SubstructVec2FromVec1(v2,target,3);
			mc.SubstructVec2FromVec1(v3,target,3);
			mc.SubstructVec2FromVec1(v4,target,3);

			len=mc.GetVectorLength(v1)+mc.GetVectorLength(v2)+mc.GetVectorLength(v3)+mc.GetVectorLength(v4);
			if(prelen>len || first)
			{
				prelen=len;
				element=i+1;
				first=false;
			}
		}
	}

	this->CreateOptionalPointParameters(element,optionalPointNumber);
	/*delete target;
	delete v1;
	delete v2;
	delete v3;
	delete v4;*/
}
void ThermalFEM::SerchAllOptionalPointBelong()
{
	for(int i=0 ; i<this->totalOP ; i++ )
	{
		this->SerchAnOptionalPointBelong(i);
	}
}

bool ThermalFEM::InOrOut(REAL target[], REAL v1[], REAL v2[], REAL v3[],REAL v4[])
{
	bool ret=false;
	int n1,n2,n3;
	/*REAL *x;
	x=new REAL[4];
	REAL *y;
	y=new REAL[4];
	REAL *z;
	z=new REAL[4];
	REAL *vi;
	vi=new REAL[3];*/

	REAL x[4];
	REAL y[4];
	REAL z[4];
	REAL vi[3];

	REAL a,b,c,d;
	x[0]=v1[0];
	x[1]=v2[0];
	x[2]=v3[0];
	x[3]=v4[0];

	y[0]=v1[1];
	y[1]=v2[1];
	y[2]=v3[1];
	y[3]=v4[1];

	z[0]=v1[2];
	z[1]=v2[2];
	z[2]=v3[2];
	z[3]=v4[2];

	/*REAL *matk;
	matk=new REAL[9];*/
	REAL matk[9];
	matk[2]=1;
	matk[5]=1;
	matk[8]=1;
	
	for(int i=0; i<4 ; i++)
	{
		switch(i){
			case 0:
				n1=0;
				n2=1;
				n3=2;
				break;
			case 1:
				n1=0;
				n2=3;
				n3=1;
				break;
			case 2:
				n1=0;
				n2=2;
				n3=3;
				break;
			case 3:
				n1=1;
				n2=3;
				n3=2;
				break;
			default:
				return false;
		}
		matk[0]=y[n1];
		matk[3]=y[n2];
		matk[6]=y[n3];

		matk[1]=z[n1];
		matk[4]=z[n2];
		matk[7]=z[n3];
	
		a=mc.CreateJudgeCoefficient(matk);
		
		matk[0]=z[n1];
		matk[3]=z[n2];
		matk[6]=z[n3];
		matk[1]=x[n1];
		matk[4]=x[n2];
		matk[7]=x[n3];

		b=mc.CreateJudgeCoefficient(matk);

		matk[0]=x[n1];
		matk[3]=x[n2];
		matk[6]=x[n3];
		matk[1]=y[n1];
		matk[4]=y[n2];
		matk[7]=y[n3];

		c=mc.CreateJudgeCoefficient(matk);
		vi[0]=x[n1];
		vi[1]=y[n1];
		vi[2]=z[n1];

		d=mc.CreateJudgeD(a,b,c,vi);

		ret=mc.judgeInOout(a,b,c,d,target);
		if(!ret)
		{
			break;
		}
	}/*
	delete x;
	delete y;
	delete z;
	delete vi;
	delete matk;*/
	return ret;

}

void ThermalFEM::CreateOptionalPointParameters(int elemNumber, int optionalPointNumber)
{
	/*int *n;
	n=new int[4];*/
	int n[4];

	this->GetNodeNumbers(elemNumber,n);
	
	this->optional[optionalPointNumber].node1=n[0];
	this->optional[optionalPointNumber].node2=n[1];
	this->optional[optionalPointNumber].node3=n[2];
	this->optional[optionalPointNumber].node4=n[3];
	
	//REAL *a;
	//a=new REAL[4];
	//REAL *b;
	//b=new REAL[4];
	//REAL *c;
	//c=new REAL[4];
	//REAL *d;
	//d=new REAL[4];
	REAL a[4];
	REAL b[4];
	REAL c[4];
	REAL d[4];

	this->CreateShapeFunction(elemNumber,a,b,c,d);

	this->optional[optionalPointNumber].N1=a[0]*this->optional[optionalPointNumber].x
										  +b[0]*this->optional[optionalPointNumber].y
		                                  +c[0]*this->optional[optionalPointNumber].z
										  +d[0];

	this->optional[optionalPointNumber].N2=a[1]*this->optional[optionalPointNumber].x
										  +b[1]*this->optional[optionalPointNumber].y
		                                  +c[1]*this->optional[optionalPointNumber].z
										  +d[1];
	
	this->optional[optionalPointNumber].N3=a[2]*this->optional[optionalPointNumber].x
										  +b[2]*this->optional[optionalPointNumber].y
		                                  +c[2]*this->optional[optionalPointNumber].z
										  +d[2];
	
	this->optional[optionalPointNumber].N4=a[3]*this->optional[optionalPointNumber].x
										  +b[3]*this->optional[optionalPointNumber].y
		                                  +c[3]*this->optional[optionalPointNumber].z
										  +d[3];

	/*delete n;
	delete a;
	delete b;
	delete c;
	delete d;*/
}

void ThermalFEM::CreateShapeFunction(int elemNumber, REAL a[],REAL b[],REAL c[],REAL d[])
{/*
	int *n;
	n=new int[4];

	int *n2;
	n2=new int[3];*/

	int n[4];		//tetrahedronlistに含まれる4節点を格納する配列

	int n2[3];		//下記計算で、ai~diの行列式計算の成分の入れ物
	REAL V;

	//elemNumber番目の節点に含まれる節点番号を取ってくる関数
	this->GetAnElementMember(elemNumber,n);

	//体積Vを求める行列計算！？
	//V=1/6 * det |1 x1 y1 z1| 
	//            |1 x2 y2 z2|
	//            |1 x3 y3 z3|
	//            |1 x4 y4 z4|
	//体積*6
	//REAL *matk;
	//mat=new REAL[16];
	REAL matk[16];
	matk[0] =1;
	matk[4] =1;
	matk[8] =1;
	matk[12]=1;
	for(int i=0;i<4;i++ )
	{
		matk[4*i+1]=this->out.pointlist[(n[i]-1)*3];
		matk[4*i+2]=this->out.pointlist[(n[i]-1)*3+1];
		matk[4*i+3]=this->out.pointlist[(n[i]-1)*3+2];
	}

	V=mc.determinant(matk,4);

	//形状関数における係数ai~diの計算
	for(int i=0; i<4 ; i++ )
	{
		//n[0]~n[3]は引数にとった　elemNumber番要素に含まれる節点を格納
		//n2[0]~n2[3]にai~di計算のための行列式成分の組み合わせを格納
		switch(i)
		{
		case 0:
			n2[0]=n[1];
			n2[1]=n[2];
			n2[2]=n[3];
			break;
		case 1:
			n2[0]=n[2];
			n2[1]=n[3];
			n2[2]=n[0];
			break;
		case 2:
			n2[0]=n[3];
			n2[1]=n[0];
			n2[2]=n[1];
			break;
		case 3:
			n2[0]=n[0];
			n2[1]=n[1];
			n2[2]=n[2];
			break;
		default:
			break;
		}
		//n2[i]の値は、現在の要素に含まれる節点番号で、⇒は不明瞭　 n[i]+0 i系列　n[i]+1 j系列 n[i]+2 k系列　n[i]+3 l系列 !?
		a[i]  =  this->out.pointlist[(n2[0]-1)*3+1]*this->out.pointlist[(n2[1]-1)*3+2]
				+this->out.pointlist[(n2[1]-1)*3+1]*this->out.pointlist[(n2[2]-1)*3+2]
				+this->out.pointlist[(n2[2]-1)*3+1]*this->out.pointlist[(n2[0]-1)*3+2]
				-this->out.pointlist[(n2[0]-1)*3+1]*this->out.pointlist[(n2[2]-1)*3+2]
				-this->out.pointlist[(n2[1]-1)*3+1]*this->out.pointlist[(n2[0]-1)*3+2]
				-this->out.pointlist[(n2[2]-1)*3+1]*this->out.pointlist[(n2[1]-1)*3+2];

		b[i]  =  this->out.pointlist[(n2[0]-1)*3]*this->out.pointlist[(n2[2]-1)*3+2]
				+this->out.pointlist[(n2[1]-1)*3]*this->out.pointlist[(n2[0]-1)*3+2]
				+this->out.pointlist[(n2[2]-1)*3]*this->out.pointlist[(n2[1]-1)*3+2]
				-this->out.pointlist[(n2[0]-1)*3]*this->out.pointlist[(n2[1]-1)*3+2]
				-this->out.pointlist[(n2[1]-1)*3]*this->out.pointlist[(n2[2]-1)*3+2]
				-this->out.pointlist[(n2[2]-1)*3]*this->out.pointlist[(n2[0]-1)*3+2];

		c[i]  =  this->out.pointlist[(n2[0]-1)*3]*this->out.pointlist[(n2[1]-1)*3+1]
				+this->out.pointlist[(n2[1]-1)*3]*this->out.pointlist[(n2[2]-1)*3+1]
				+this->out.pointlist[(n2[2]-1)*3]*this->out.pointlist[(n2[0]-1)*3+1]
				-this->out.pointlist[(n2[0]-1)*3]*this->out.pointlist[(n2[2]-1)*3+1]
				-this->out.pointlist[(n2[1]-1)*3]*this->out.pointlist[(n2[0]-1)*3+1]
				-this->out.pointlist[(n2[2]-1)*3]*this->out.pointlist[(n2[1]-1)*3+1];

		d[i]  =  this->out.pointlist[(n2[0]-1)*3]*this->out.pointlist[(n2[1]-1)*3+1]*this->out.pointlist[(n2[2]-1)*3+2]
				+this->out.pointlist[(n2[1]-1)*3]*this->out.pointlist[(n2[2]-1)*3+1]*this->out.pointlist[(n2[0]-1)*3+2]
				+this->out.pointlist[(n2[2]-1)*3]*this->out.pointlist[(n2[0]-1)*3+1]*this->out.pointlist[(n2[1]-1)*3+2]
				-this->out.pointlist[(n2[0]-1)*3]*this->out.pointlist[(n2[2]-1)*3+1]*this->out.pointlist[(n2[1]-1)*3+2]
				-this->out.pointlist[(n2[1]-1)*3]*this->out.pointlist[(n2[0]-1)*3+1]*this->out.pointlist[(n2[2]-1)*3+2]
				-this->out.pointlist[(n2[2]-1)*3]*this->out.pointlist[(n2[1]-1)*3+1]*this->out.pointlist[(n2[0]-1)*3+2];

		if(i%2==0)
		{
			a[i]*=(-1.0);
			b[i]*=(-1.0);
			c[i]*=(-1.0);
		}
		else
		{
			d[i]*=(-1.0);
		}
		a[i]/=V;
		b[i]/=V;
		c[i]/=V;
		d[i]/=V;
	}/*
	delete  n;
	delete n2;
	delete matk;*/
}

REAL ThermalFEM::GetOptionalPointTemperature(int OptionalPointNumber)
{
	REAL ret;

	ret= this->optional[OptionalPointNumber].N1*this->temperature[this->optional[OptionalPointNumber].node1]
		+this->optional[OptionalPointNumber].N2*this->temperature[this->optional[OptionalPointNumber].node2]
		+this->optional[OptionalPointNumber].N3*this->temperature[this->optional[OptionalPointNumber].node3]
		+this->optional[OptionalPointNumber].N4*this->temperature[this->optional[OptionalPointNumber].node4];
	//debug
	double SUMN = this->optional[OptionalPointNumber].N1 + this->optional[OptionalPointNumber].N2 + this->optional[OptionalPointNumber].N3 + this->optional[OptionalPointNumber].N4;
	assert(std::abs(SUMN - 1.0) < 1e-6);

	return ret;
}
void ThermalFEM::GetOptionalPointTemperature(REAL optionalPointTemperatures[])
{
	for(int i=0; i<this->totalOP ; i++ )
	{
		optionalPointTemperatures[i]=this->GetOptionalPointTemperature(i);
	}
}

REAL ThermalFEM::GetElemVolume(int elemNumber)
{
  /*     int *n;
	   n=new int[4];
       REAL *x;
	   x=new REAL[4];
       REAL *y;
	   y=new REAL[4];
       REAL *z;
	   z=new REAL[4];
	    int *n;
	   n=new int[4];*/

       REAL x[4];
       REAL y[4];
       REAL z[4];
	   int n[4];

       for(int i=0 ; i<4 ; i++ )
       {
               n[i]=this->out.tetrahedronlist[(elemNumber-1)*4+i];
       }
       for(int i=0 ; i<4 ; i++ )
       {
               x[i]=this->out.pointlist[(n[i]-1)*3];
               y[i]=this->out.pointlist[(n[i]-1)*3+1];
               z[i]=this->out.pointlist[(n[i]-1)*3+2];
       }
	   /*delete n;
	   delete x;
	   delete y;
	   delete z;*/

       return this->tet.CalcVolume(x[0],x[1],x[2],x[3],y[0],y[1],y[2],y[3],z[0],z[1],z[2],z[3]);

}

//menseki keisan
void ThermalFEM::initializeElemAreas()
{
	this->elemFaceAreas = new ElemAreas[this->out.numberoftetrahedra];
	for(int i=0; i<this->out.numberoftetrahedra ; i++ )
	{
		this->elemFaceAreas[i].facet1=0;
		this->elemFaceAreas[i].facet2=0;
		this->elemFaceAreas[i].facet3=0;
		this->elemFaceAreas[i].facet4=0;
	}
}
void ThermalFEM::calcElemFaceArea(int code[])
{
	for( int i=0 ; i<this->out.numberoftetrahedra ; i++ )
	{
		this->calcElemFaceArea( (i+1), code[i]);
	}
}
void ThermalFEM::calcElemFaceArea(int elemNumber,int code)
{
	if(!this->usedElemAreas)
	{
		this->initializeElemAreas();
		this->usedElemAreas=true;
	}
/*
面1が境界のとき012
面2が境界のとき013
面3が境界のとき023
面4が境界のとき123
*/


	if(code!=0)
	{
		//int *n;
		//n=new int[4];
		//REAL *p1;
		//p1=new REAL[3];
		//REAL *p2;
		//p2=new REAL[3];
		//REAL *p3;
		//p3=new REAL[3];

		int n[4];
		REAL p1[3];
		REAL p2[3];
		REAL p3[3];

		this->GetNodeNumbers(elemNumber,n);

		if(code>=1000)
		{
			for(int i=0 ; i<3; i++ )
			{
				p1[i]=this->out.pointlist[n[1]*3+i];
				p2[i]=this->out.pointlist[n[2]*3+i];
				p3[i]=this->out.pointlist[n[3]*3+i];
			}

			this->elemFaceAreas[elemNumber-1].facet4=mc.CalcTriangleAreas(p1,p2,p3);
		}
		if((code%1000)>=100)
		{
			
			for(int i=0 ; i<3; i++ )
			{
				p1[i]=this->out.pointlist[n[0]*3+i];
				p2[i]=this->out.pointlist[n[2]*3+i];
				p3[i]=this->out.pointlist[n[3]*3+i];
			}

			this->elemFaceAreas[elemNumber-1].facet3=mc.CalcTriangleAreas(p1,p2,p3);
		}
		if((code%100)>=10)
		{
			for(int i=0 ; i<3; i++ )
			{
				p1[i]=this->out.pointlist[n[0]*3+i];
				p2[i]=this->out.pointlist[n[1]*3+i];
				p3[i]=this->out.pointlist[n[3]*3+i];
			}

			this->elemFaceAreas[elemNumber-1].facet2=mc.CalcTriangleAreas(p1,p2,p3);
		}
		if((code%10)==1)
		{
			for(int i=0 ; i<3; i++ )
			{
				p1[i]=this->out.pointlist[n[0]*3+i];
				p2[i]=this->out.pointlist[n[1]*3+i];
				p3[i]=this->out.pointlist[n[2]*3+i];
			}

			this->elemFaceAreas[elemNumber-1].facet1=mc.CalcTriangleAreas(p1,p2,p3);
		}
	/*	delete n;
		delete p1;
		delete p2;
		delete p3;*/
	}
}
void ThermalFEM::calcElemFaceArea()
{
	for(int i=0 ; i< this->out.numberofpoints ; i++ )
	{
		this->calcElemFaceArea((i+1) , 1111);
	}
}

void ThermalFEM::TFEMTetrahedralize(char *switches)
{
	tetrahedralize(switches, &in, &out);
	this->useTETGEN=true;
	this->SetInit_relateNode(this->out.numberofpoints);
	
}

void ThermalFEM::TFEMTetrahedralize(char *switches,tetgenio in)
{
	tetrahedralize(switches, &in, &out);
	this->useTETGEN=true;
	this->SetInit_relateNode(this->out.numberofpoints);
	
}

void ThermalFEM::TFEMTetrahedralize(char *switches,tetgenio in,tetgenio out)
{
	tetrahedralize(switches, &in, &out);
	this->useTETGEN=true;
	this->SetInit_relateNode(this->out.numberofpoints);
	
}


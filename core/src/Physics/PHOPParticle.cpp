#include "PHOpParticle.h"
#include "PHOpDecompositionMethods.h"
//#include "SVDDecomposition.h"
//#include "JacobiDecomposition.h"
//#include "MatrixExtension.h"
#define CHECK_INF_ERR

namespace Spr{;

void PHOpParticle::swapVectorToArr()
{
	int size = (int)pVertTempInd.size();
	pVertArr = new int[size];
	initialPvarr = true;
	for(int i=0;i<size;i++)
	{
		pVertArr[i] = pVertTempInd[i];
	}
	std::vector<int> s;
	//pVertInd.clear();
	s.swap(pVertTempInd);
}

void PHOpParticle::preCalcu(Vec3f *mPos)
{//Particle元中心計算。。。。linear blend skiningに使うなら重心のかも？
	pOrigCtr = Vec3f(0,0,0);
	for(int i=0;i<pNvertex;i++)
	{
		pOrigCtr = pOrigCtr + mPos[i];
		
	}
	pOrigCtr = pOrigCtr/(float)pNvertex;
	pCurrCtr = pOrigCtr;
	pNewCtr = pOrigCtr;

	
}


void PHOpParticle::ptclBuildStep(Vec3f *mPos)
{
	
	preCalcu(mPos);
		buildEllipRadius(mPos);
		
}


bool PHOpParticle::addNewVertex(int index)
{
	for (int i = 0; i< (int) pVertTempInd.size(); i++)
	{
		if(pVertTempInd[i] == index)
		{
			std::cout<<"VertexIndex already added in "<<i<<std::endl;
			return false;
		}
	}
	
	pTotalMass += pTempSingleVMass;
	
	pVertTempInd.push_back(index);
	pNvertex++;
	return true;
}


void PHOpParticle::buildParticleCenter(Vec3f *mPos)
{
	pCurrCtr = Vec3f(0,0,0);
	for(int i=0;i<pNvertex;i++)
	{
		pCurrCtr += mPos[i];
	}
	pCurrCtr = pCurrCtr/(float)pNvertex;

	
}

void PHOpParticle::buildEllipRadius(Vec3f *mPos)
	{
		//共分散行列の計算
		float ExxP2 = 0.0,ExyP2 = 0.0,ExzP2 = 0.0,EyyP2 = 0.0,EzzP2 = 0.0,EyzP2 = 0.0,centerX = 0.0,centerY = 0.0,centerZ = 0.0;
		
		
		//prevent 0 or negtive radius
		if(pNvertex<=0)
			return;
		if(pNvertex<3)
		{
			if(pNvertex==2)
				pMainRadius = fabs((mPos[0] - mPos[1]).norm()) / 2;
			else pMainRadius = 0.3f;
			pMainRadiusVec = Vec3f(1,0,0);
			pSecRadius = pMainRadius;
			pSecRadiusVec = Vec3f(0,1,0);
			pThrRadius = pMainRadius;
			pThrRadiusVec = Vec3f(0,0,1);

			ellAff.LookAt(pThrRadiusVec,pSecRadiusVec);
			ellipRotMatrix = ellAff.Rot();
			return ;
		}
		

		for(int i=0;i<pNvertex;i++)
		{
			float tmp = (mPos[i].X() - pCurrCtr.X());
			ExxP2 += tmp * tmp;
			tmp = (mPos[i].Y() - pCurrCtr.Y());
			EyyP2 += tmp * tmp;
			tmp = (mPos[i].Z() - pCurrCtr.Z());
			EzzP2 += tmp * tmp;
			tmp = ((mPos[i].X() - pCurrCtr.X()) * (mPos[i].Y() - pCurrCtr.Y()));
			ExyP2 += tmp;
			tmp = ((mPos[i].Z() - pCurrCtr.Z()) * (mPos[i].X() - pCurrCtr.X()));
			ExzP2 += tmp;
			tmp = ((mPos[i].Y() - pCurrCtr.Y()) * (mPos[i].Z() - pCurrCtr.Z()));
			EyzP2 += tmp;
		}
		ExxP2/=(float)pNvertex;EyyP2/=(float)pNvertex;EzzP2/=(float)pNvertex;ExyP2/=(float)pNvertex;ExzP2/=(float)pNvertex;EyzP2/=(float)pNvertex;
		Matrix3f coVMatrix,R,S;
		coVMatrix.xx = ExxP2;coVMatrix.yy = EyyP2;coVMatrix.zz = EzzP2;
		coVMatrix.xy = ExyP2;coVMatrix.yz = EyzP2;coVMatrix.xz = ExzP2;
		coVMatrix.yx = ExyP2;coVMatrix.zy = EyzP2;coVMatrix.zx = ExzP2;

		
		covMatrixList.push_back(coVMatrix);
	

		Matrix3f valMatrix,vecMatrix;
		JacobiDecomposition jd;
		jd.Jacobi(coVMatrix,vecMatrix);

		Vec3f subVec1,subVec2,subVec3,SumVec;
		subVec1.X() = vecMatrix.xx;subVec1.Y() = vecMatrix.xy;subVec1.Z() = vecMatrix.xz;

		subVec2.X() = vecMatrix.yx;subVec2.Y() = vecMatrix.yy;subVec2.Z() = vecMatrix.yz;
		
		subVec3.X() = vecMatrix.zx;subVec3.Y() = vecMatrix.zy;subVec3.Z() = vecMatrix.zz;
		float eigenV1 = coVMatrix.xx;//subVec1.norm();
		float eigenV2 = coVMatrix.yy;//subVec2.norm();
		float eigenV3 = coVMatrix.zz;//subVec3.norm();

		//prevent negtive radius
		float sum = eigenV1 + eigenV2 + eigenV3;
		float prop1 = eigenV1/sum;
		float prop2 = eigenV2/sum;
		float prop3 = eigenV3/sum;


		eigenV1 = prop1;// * pRadii;
		eigenV2 = prop2;// * pRadii;
		eigenV3 = prop3;// * pRadii;

		std::vector<float> sortE ;
		sortE.push_back(eigenV1);
		sortE.push_back(eigenV2);
		sortE.push_back(eigenV3);

		std::sort(sortE.begin(),sortE.end());
		
		

		
		if(sortE[2]==eigenV1)
		{
			pMainRadius = eigenV1;
			pMainRadiusVec = subVec1;
		}
		if(sortE[2]==eigenV2)
		{
			pMainRadius = eigenV2;
			pMainRadiusVec = subVec2;
		}
		if(sortE[2]==eigenV3)
		{
			pMainRadius = eigenV3;
			pMainRadiusVec = subVec3;
		}

		if(sortE[1]==eigenV1)
			{
				pSecRadius = eigenV1;
				pSecRadiusVec = subVec1;
		}
		if(sortE[1]==eigenV2)
			{
				pSecRadius = eigenV2;
				pSecRadiusVec = subVec2;
		}
		if(sortE[1]==eigenV3)
			{
				pSecRadius = eigenV3;
				pSecRadiusVec = subVec3;
				}

		if(sortE[0]==eigenV1)
			{
				pThrRadius = eigenV1;
				pThrRadiusVec = subVec1;
		}
		if(sortE[0]==eigenV2)
		{
			pThrRadius = eigenV2;
			pThrRadiusVec = subVec2;
		}
		if(sortE[0]==eigenV3)
		{
			pThrRadius = eigenV3;
			pThrRadiusVec = subVec3;
		}
		
		Vec3f theX = Vec3f(1,0,0);
		Vec3f theY = Vec3f(0,1,0);
		Vec3f theZ = Vec3f(0,0,1);
		dotCoeffMatrix.xx = pMainRadiusVec * theX;
		dotCoeffMatrix.xy = pMainRadiusVec * theY;
		dotCoeffMatrix.xz = pMainRadiusVec * theZ;

		dotCoeffMatrix.yx = pSecRadiusVec * theX;
		dotCoeffMatrix.yy = pSecRadiusVec * theY;
		dotCoeffMatrix.yz = pSecRadiusVec * theZ;

		dotCoeffMatrix.zx = pThrRadiusVec * theX;
		dotCoeffMatrix.zy = pThrRadiusVec * theY;
		dotCoeffMatrix.zz = pThrRadiusVec * theZ;
		dotCoeffMatrix = dotCoeffMatrix.inv();
		
		ellAff.LookAt(pThrRadiusVec,pSecRadiusVec);
		ellipRotMatrix = ellAff.Rot();
		ellipOrigOrint.FromMatrix(ellipRotMatrix);
		int y=0;

		return;
		
	}
	/*
	@brief 円また楕円のモーメントマトリクスの計算
	*/
	Matrix3f *PHOpParticle::GetAellipMtrx()
	{
#ifdef	_MSC_VER
		Spr::TQuaternion<float> &tquat = pNewOrint * pOrigOrint.Inv();
#else
		Spr::TQuaternion<float> tmp = pNewOrint * pOrigOrint.Inv();
		Spr::TQuaternion<float> &tquat = tmp;
#endif
		//calculate orthonormal matrix R

		Matrix3f Rot;
		
		tquat.ToMatrix(Rot);
		//！今a,b,cが同じに設定しています
		Matrix3f ellipM;
	
		ellipM = ellipM * (pRadii * pRadii * 0.2f * pTotalMass);



		MatrixExtension me;
	
		pMomentR = me.MatrixesMultiply3f(ellipM, Rot);
		
#ifdef CHECK_INF_ERR
		if (!FloatErrorTest::CheckBadFloatValue(ellipM.xx, ellipM.yx, ellipM.zx))
		{
			int u = 0;
			return NULL;
		}
#endif

		return &pMomentR;

	}

}//namespace
	

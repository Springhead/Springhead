#ifndef PHOpParticle_H
#define PHOpParticle_H

#include "Base/TQuaternion.h"
#include "Base/TinyVec.h"
#include "Base/TinyMat.h"
#include <vector>
#include <Foundation/Object.h>
#include <Physics/SprPHOpObj.h>

namespace Spr{;


//---------------------------------------------------------------------------
/**
//
//@brief  
//	Usage: 
//	before simulation :
//	when simulation : build currCenter ,CurrOrien
//
*/
class PHOpParticle :public SceneObject, public PHOpParticleDesc
{
	
public :	
	SPR_OBJECTDEF(PHOpParticle);
	ACCESS_DESC(PHOpParticle);
	//public TPos oritation;

	PHOpParticle(const PHOpParticleDesc& desc = PHOpParticleDesc()) :PHOpParticleDesc(desc)
	{
		//
		pType = 0;//0 は表面particleで、1は内部particle
		pTempSingleVMass = 1.0f;// 0.001f * 8.0f / 43.0f;
		//pTempSingleVMass = 0.001f * 8.0f / 43.0f;
		pParaAlpha = 1.0f;
		pNvertex = 0;
		pWvel = pVelocity = Vec3f(0.0,0.0,0.0);
		pRadii = 0.0;
		pTotalMass = 0.0;
		pOrigCtr = pOrigCtr.Zero();
		pGoalCtr.clear();
		pCurrCtr = pCurrCtr.Zero();
		isColliedbySphash = false;
		isColliedbyColliCube = false;
		isFixedbyMouse = false;
		isColliedSphashSolved = false;
		isColliedSphashSolvedReady = false;
		hitedByMouse = false;
		isFixed = false;
		pMomentInertia = pMomentInertia.Zero();
		//pColliedForceMove = pColliedForceMove.Zero();
		
		initialWArr = false;
		initialPvarr = false;
	}
	
private:
	
	
	
public :
	//初期向き計算用共分散行列
	std::vector<Matrix3f> covMatrixList;
	//初期向き計算用
	Affinef ellAff;
	//初期向き計算用
	Matrix3f dotCoeffMatrix;
	
	//一時的の頂点index
	std::vector<int> pVertTempInd;
	
	
	//projection from vertices(local index) to faces (targetMesh index)


	//頂点配列配置フラグ
	bool initialPvarr;
	//頂点配列
	int *pVertArr;
	//表面粒子と内部粒子区別用
	int pType;
	
	//粒子中心からの頂点の重み
	float *pVectDisWeightArr;
	float GetVtxDisWeight(int vi)
	{
		return pVectDisWeightArr[vi];
	}
	//本粒子に含むグループのindexリスト
	std::vector<int> pInGrpList;
	int GetinGrpListNum()
	{
		return (int)pInGrpList.size();
	}
	int GetinGrpList(int gi)
	{
		return pInGrpList[gi];
	}
	int GetOpPtclVtxId(int vi)
	{
		return pVertArr[vi];
	}
	PHOpParticleDesc* GetParticleDesc()
	{
		return this;
	}

	//重み初期化フラグ
	bool initialWArr;
	//caution pCurrOrint cannot be used directly now you need to inverse the pos before use it.
	//check commond DrawOrien in OPAppExpand.cpp
	
	
	//粒子の到達位置（未使用）
	std::vector<Vec3f> pGoalCtr;
	

	//Delayed Force Base.()
	

	//The list which include the groups that contain this particle
	
	


	int getVertexGlbIndex(int vindex)
	{
		return pVertArr[vindex];
	}
	int getVertexLclIndex(int vGindex)
	{
		for(int i =0; i< pNvertex;i++)
			{
			if(vGindex == pVertArr[i])
			{
				return i;
			}
		}
		return -1;
	}

	void preCalcu(Vec3f *mPos);
	
	void addVertMomtInertias(Vec3f &position);
	void ptclBuildStep(Vec3f *mPos);
	
	void angleVelocityBuild(float dt);
	void angleVelocityIntegration(float dt);

	bool addNewVertex(int index);
	
	void addLinkParticle(int parIndex);
	
	void buildParticleCenter(Vec3f *mPos);
	
	void buildEllipRadius(Vec3f *mPos);
	void swapVectorToArr();
	
	Matrix3f *GetAellipMtrx();

	void CovarianceMatrix(Matrix3f &cov, Vec3f pt[], int numPts)
{
    float oon = 1.0f / (float)numPts;
    Vec3f c = Vec3f(0.0f, 0.0f, 0.0f);
    float e00, e11, e22, e01, e02, e12;

    // Compute the center of mass (centroid) of the points
    for (int i = 0; i < numPts; i++)
        c += pt[i];
    c *= oon;
    
    // Compute covariance elements
    e00 = e11 = e22 = e01 = e02 = e12 = 0.0f;
    for (int i = 0; i < numPts; i++) {
        // Translate points so center of mass is at origin
        Vec3f p = pt[i] - c;
        // Compute covariance of translated points
        e00 += p.x * p.x;
        e11 += p.y * p.y;
        e22 += p.z * p.z;
        e01 += p.x * p.y;
        e02 += p.x * p.z;
        e12 += p.y * p.z;
    }
    // Fill in the covariance matrix elements
    cov[0][0] = e00 * oon;
    cov[1][1] = e11 * oon;
    cov[2][2] = e22 * oon;
    cov[0][1] = cov[1][0] = e01 * oon;
    cov[0][2] = cov[2][0] = e02 * oon;
    cov[1][2] = cov[2][1] = e12 * oon;
}
};
struct DisCmpPoint
		{//このParticleと他の間の距離とindexを求める
			
			int pIndex;
			float distance;
			bool cmpdis(const DisCmpPoint &a,const DisCmpPoint &b)
			{return a.distance<b.distance;}
			bool operator < (const DisCmpPoint &a)const
			{return (distance < a.distance);}
		};

}//namespace
#endif

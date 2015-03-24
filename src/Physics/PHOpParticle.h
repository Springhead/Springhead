#ifndef PHOpParticle_H
#define PHOpParticle_H

#include "Base/TQuaternion.h"
#include "Base/TinyVec.h"
#include "Base/TinyMat.h"
#include <vector>

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
class PHOpParticle
{
public :	
	//public TPos oritation;

	PHOpParticle()
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
	//初期向き計算用共分散行列
	std::vector<Matrix3f> covMatrixList;
	//初期向き計算用
	Affinef ellAff;
	//初期向き計算用
	Matrix3f dotCoeffMatrix;
	
	
public :
	//マウスに触られたかフラグ
	bool hitedByMouse;
	//一時的の頂点index
	std::vector<int> pVertTempInd;
	//含むメッシュのindex
	std::vector<int> pFaceInd;
	//粒子硬さ係数alpha(開発中)
	float pParaAlpha;
	//projection from vertices(local index) to faces (targetMesh index)

	//頂点配列
	int *pVertArr;
	//頂点配列配置フラグ
	bool initialPvarr;
	//頂点数
	int pNvertex;
	//自分のグループid
	int pMyGroupInd;
	//粒子のid
	int pPId;
	//所属objのid
	int pObjId;
	//表面粒子と内部粒子区別用
	int pType;
	

	//頂点慣性モーメント 
	Matrix3f pMomentInertia;
	//頂点慣性モーメントの逆 
	Matrix3f pInverseOfMomentInertia;
	//頂点持つ質量（仮）
	float pTempSingleVMass;
	//粒子の総合質量
	float pTotalMass;
	//粒子モーメント行列
	Matrix3f pMomentR;
	//粒子分解後回転行列
	Matrix3f pSmR;
	//粒子中心からの頂点の重み
	float *pVectDisWeightArr;
	//重み初期化フラグ
	bool initialWArr;
	//caution pCurrOrint cannot be used directly now you need to inverse the pos before use it.
	//check commond DrawOrien in OPAppExpand.cpp
	//現在向き
	TQuaternion<float> pCurrOrint;
	//初期向き
	TQuaternion<float> pOrigOrint;
	//予測向き
	TQuaternion<float> pNewOrint;
	//頂点の主成分分析PCAによる楕円の向き行列
	Matrix3f ellipRotMatrix;
	//粒子角速度
	Vec3f pWvel;//角速度、長さは速度の大きさ、方向は速度の方向
	//粒子球半径（球として扱う時に使う）
	float pRadii;
	//動的粒子半径
	float pDynamicRadius;
	//主半径vector
	Vec3f pMainRadiusVec;
	//副半径vector
	Vec3f pSecRadiusVec;
	//従半径vector
	Vec3f pThrRadiusVec;
	//主半径
	float pMainRadius;
	//副半径
	float pSecRadius;
	//従半径
	float pThrRadius;
	//粒子の現在位置
	Vec3f pCurrCtr;
	//粒子の初期位置
	Vec3f pOrigCtr;
	//粒子の予測位置
	Vec3f pNewCtr;
	//粒子の速度
	Vec3f pVelocity;
	//粒子の到達位置（未使用）
	std::vector<Vec3f> pGoalCtr;

	//Delayed Force Base.()
	//外力
	Vec3f pExternalForce;
	//not yet been used
	Vec3f pExternalTorque;//not yet been used
	//not yet been used
	Vec3f pExternalDisplacement;

	//The list which include the groups that contain this particle
	//本粒子に含むグループのindexリスト
	std::vector<int> pInGrpList;
	//SPhash に検出されたか
	bool isColliedbySphash;
	//ColliCubeに検出されたか
	bool isColliedbyColliCube;
	//マウスにつかまれたか
	bool isFixedbyMouse;
	//固定してるか
	bool isFixed;
	//SPhash に解決すべしか
	bool isColliedSphashSolved;
	//SPhash にまもなく解決すべしか
	bool isColliedSphashSolvedReady;


	int getVertexGlbIndex(int vindex)
	{
		return pVertArr[vindex];
	}
	int getVertexLclIndex(int vGindex)
	{
		for(int i =0; i< pNvertex;i++)
			{
			if(vGindex = pVertArr[i])
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

/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#include <Physics/PHOpObj.h>
#include "PHOpDecompositionMethods.h"

//friction 

#define CHECK_INF_ERR

using namespace PTM;
namespace Spr{;


void PHOpObj::SimpleSimulationStep()
{
		positionPredict();
		groupStep();
		integrationStep();
		
}

void PHOpObj::groupStep()
{
	if (!isRigid){
		for (int it = 0; it < objitrTime; it++)
		{
			//gAgroup計算groupCurrCenter計算
			summationTogAgroup();
			//変形後新しい中心を計算

			if (objUseReducedPP)
				ReducedPositionProject();
			else positionProject();


			if (objUseDistCstr)
				DistanceConstrain();

		}
	}
}
/*
@brief 距離拘束
*/
void PHOpObj::DistanceConstrain()
{
	
	float inextenCoe = 0.99f;//float s = inextensibility;
	
	//DistanceConstrain by group
	for (int i = 0; i < assGrpNum; i++) {
		PHOpGroup &gp = objGArr[i];
		for(int j = 1;j<gp.gNptcl;j++)
		{
			PHOpParticle &dp1 = objPArr[gp.getParticleGlbIndex(0)];
			PHOpParticle &dp2 = objPArr[gp.getParticleGlbIndex(j)];
			
			float ctrW = 1.0f / dp1.pTotalMass;
			float sideW = 1.0f / dp2.pTotalMass;
			float sumW = ctrW + sideW;
#ifdef	_MSC_VER
			Vec3f &vN = dp1.pNewCtr - dp2.pNewCtr;
#else
			Vec3f tmp1 = dp1.pNewCtr - dp2.pNewCtr;
			Vec3f &vN = tmp1;
#endif
			float currLength = vN.norm();
			float orinLength = (dp1.pOrigCtr - dp2.pOrigCtr).norm();

#ifdef	_MSC_VER
			Vec3f &dx1 = - (1.0f/sumW) * (currLength - orinLength) * (vN/currLength) * inextenCoe;
			Vec3f &dx2 = -dx1;
#else
			Vec3f tmp2 = - (1.0f/sumW) * (currLength - orinLength) * (vN/currLength) * inextenCoe;
			Vec3f tmp3 = -tmp2;
			Vec3f &dx1 = tmp2;
			Vec3f &dx2 = tmp3;
#endif
				
			dp1.pNewCtr += dx1 * ctrW;
			dp2.pNewCtr += dx2 * sideW;
		}
	}
		
}

/*
@brief 位置の予測
*/
void PHOpObj::positionPredict()
{
	int i=0;
	float restitution = 0.9f;
	//
	//Gravity Force
	bool xChanged = false,yChanged = false,zChanged = false;
	
	
	for (i = 0; i < assPsNum; i++) {
		PHOpParticle &dp = objPArr[i];
		if (dp.isFixed)
		{
			dp.pExternalForce.clear();
			continue;
		}
		//position predict
		//gravity
		if(gravityOn)
		{		
			dp.pVelocity += params.gravity * params.timeStep ;
		}
		//external force
		dp.pVelocity += dp.pExternalForce *params.timeStep / dp.pTotalMass;

#ifdef CHECK_INF_ERR
		if (!FloatErrorTest::CheckBadFloatValue(dp.pVelocity.z))
			int u = 0;
#endif

		//clear
		dp.pExternalForce.clear();
	}
	
	//速度Damp
	dampVelocities();


	for (i = 0; i < assPsNum; i++) {
		//改良！実際は重力加速度と質量の乗では？
		PHOpParticle &dp = objPArr[i];
		dp.pNewCtr = dp.pCurrCtr + dp.pVelocity * params.timeStep;
		
		//orientation predict
		Spr::TQuaternion<float> q;
		float norm = dp.pWvel.norm();
		
		//norm = fabs(norm);//debug norm abs
		if (norm <0.001f)
		{
			dp.pNewOrint = dp.pCurrOrint;
		}else
		{
			//build quat from axis and radian 
			Vec3f direction = dp.pWvel / norm;
			float arcPerSc = norm * params.timeStep;
			
			float halfAngle = arcPerSc * 0.5f;
			float scale		= sin(halfAngle);	
		
			q.x = scale * direction.x;
			q.y = scale * direction.y;
			q.z = scale * direction.z;
			q.w = cos(halfAngle);
			dp.pNewOrint = q * dp.pCurrOrint;
		}
		//dp.pNewOrint = dp.pCurrOrint;//debug stop predict
	
		//Boundary of Scene
		Vec3f &p = dp.pCurrCtr;
		Vec3f &np = dp.pNewCtr;
		Vec3f &v = dp.pVelocity;

		if (np.x < params.bounds.min.x || np.x > params.bounds.max.x) {
			np.x = p.x - v.x * params.timeStep * restitution;
			np.y = p.y;
			np.z = p.z;
			xChanged = true;
		}
		if (np.y < params.bounds.min.y || np.y > params.bounds.max.y) {
			np.y = p.y - v.y * params.timeStep * restitution;
			if(!xChanged)
				np.x = p.x;
			np.z = p.z;
			yChanged = true;
		}
		if (np.z < params.bounds.min.z || np.z > params.bounds.max.z) {
			np.z = p.z - v.z * params.timeStep * restitution;
			if(!xChanged)
				np.x = p.x;
			if(!yChanged)
				np.y = p.y;
			zChanged = true;
		}
		
        params.bounds.clamp(dp.pNewCtr);


	}
}


/*
@brief 位置の予測(摩擦あり)
*/
void PHOpObj::positionPredictFriction(PHOpHapticController *myHc)
{
	int i = 0;
	float restitution = 0.9f;
	//
	//Gravity Force
	bool xChanged = false, yChanged = false, zChanged = false;


	for (i = 0; i < assPsNum; i++) {
		PHOpParticle &dp = objPArr[i];
		if (dp.isFixed)
		{
			dp.pExternalForce.clear();
			continue;
		}
		//position predict
		//gravity
		if (gravityOn)
		{
			dp.pVelocity += params.gravity * params.timeStep;
		}
		//external force
	//	dp.pVelocity += dp.pExternalForce *params.timeStep / dp.pTempSingleVMass;

#ifdef CHECK_INF_ERR
		if (!FloatErrorTest::CheckBadFloatValue(dp.pVelocity.z))
			int u = 0;
#endif

		//clear
		dp.pExternalForce.clear();
	}

	//速度Damp
	dampVelocities();


	for (i = 0; i < assPsNum; i++) {
		//改良！実際は重力加速度と質量の乗では？
		PHOpParticle &dp = objPArr[i];
		dp.pNewCtr = dp.pCurrCtr + dp.pVelocity * params.timeStep;

		//orientation predict
		Spr::TQuaternion<float> q;
		float norm = dp.pWvel.norm();

		//norm = fabs(norm);//debug norm abs
		if (norm <0.001f)
		{
			dp.pNewOrint = dp.pCurrOrint;
		}
		else
		{
			//build quat from axis and radian 
			Vec3f direction = dp.pWvel / norm;
			float arcPerSc = norm * params.timeStep;

			float halfAngle = arcPerSc * 0.5f;
			float scale = sin(halfAngle);

			q.x = scale * direction.x;
			q.y = scale * direction.y;
			q.z = scale * direction.z;
			q.w = cos(halfAngle);
			dp.pNewOrint = q * dp.pCurrOrint;
		}
		//dp.pNewOrint = dp.pCurrOrint;//debug stop predict

		//Boundary of Scene
		Vec3f &p = dp.pCurrCtr;
		Vec3f &np = dp.pNewCtr;
		Vec3f &v = dp.pVelocity;

		if (np.x < params.bounds.min.x || np.x > params.bounds.max.x) {
			np.x = p.x - v.x * params.timeStep * restitution;
			np.y = p.y;
			np.z = p.z;
			xChanged = true;
		}
		if (np.y < params.bounds.min.y || np.y > params.bounds.max.y) {
			np.y = p.y - v.y * params.timeStep * restitution;
			if (!xChanged)
				np.x = p.x;
			np.z = p.z;
			yChanged = true;

			dp.isColliedBound = true;
			dp.pfrictionState = STATIC;
		}
		else {
			dp.isColliedBound = false;
			dp.pfrictionState = FREE;
		}
		if (np.z < params.bounds.min.z || np.z > params.bounds.max.z) {
			np.z = p.z - v.z * params.timeStep * restitution;
			if (!xChanged)
				np.x = p.x;
			if (!yChanged)
				np.y = p.y;
			zChanged = true;
		}

		params.bounds.clamp(dp.pNewCtr);

		if (dp.isColliedBound && !dp.pType) {
			//friction constraint
			bool bStatic = false;

			float mu0 = 0.35f;
			float mu = 0.3f;
			float muCur;
			float timeVaryFrictionA = 0.1f;
			float timeVaryFrictionB = 1800;
			float reflexSpring = 2000.0f;
			float hdt = 0.001f;
			double alpha = hdt * hdt * reflexSpring / (dp.pTotalMass * 0.001);
			//	DSTR << "alpha" << alpha << std::endl;
			if (dp.pfrictionState == STATIC) {
				muCur = mu; //+ mu*(timeVaryFrictionA * log(1 + timeVaryFrictionB * (dp.fricCount + 1) * hdt));
			}

			if (dp.pfrictionState == DYNAMIC) {
				muCur = mu;
			}

			Vec3d diff = (myHc->userPose * dp.pOrigCtr);
			Vec3d ortho = Vec3d(0, (diff.y - dp.pNewCtr.y), 0);
			double depth = ortho.norm();
			
			double l = muCur * depth;
			Vec3d tangent = Vec3d(diff.x - dp.pNewCtr.x, 0, 0);
			double tangentNorm = tangent.norm();
			//if (i == 0) DSTR << "alpha" << alpha << std::endl;
			//if (i == 10) DSTR << "diff" << diff << std::endl;
			//if (i == 0) DSTR << "dp.pCurrCtr.x" << dp.pCurrCtr.x << std::endl;
			//if (i == 0) DSTR << "originalpos" << dp.pOrigCtr << std::endl;
			//if (i == 0) DSTR << "tangent" << tangentNorm << std::endl;
			double proxyPos, frictionLimit;

			if (tangentNorm > 1e-5) {
				proxyPos = tangentNorm;
				double predict = proxyPos; + (v * (tangent / tangentNorm)) * hdt;

				frictionLimit = predict - alpha * (predict - l);
				
				if (proxyPos <= frictionLimit) {
					bStatic = true;
				}
				else {
					bStatic = false;
					dp.pNewCtr.x = frictionLimit;
				}
			}
			dp.fricCount++;
			if (!bStatic) {
				if (dp.pfrictionState != DYNAMIC) {
					dp.fricCount = 0;
					dp.pfrictionState = DYNAMIC;
				}
			}
			else {
				if (dp.pfrictionState != STATIC) {
					dp.fricCount = 0;
					dp.pfrictionState = STATIC;
				}
			}
			//if (i == 0 && dp.pfrictionState != STATIC) DSTR << "tangentNorm:" << tangentNorm << std::endl;
			//if (i == 0 && dp.pfrictionState != STATIC)DSTR << "frictionLimit:" << frictionLimit << std::endl;
			//if (i == 0 && dp.pfrictionState != STATIC) DSTR << "frictionState" << dp.pfrictionState << std::endl;

		}
	}
}


/*
@brief グループMatrixの計算
*/
void PHOpObj::summationTogAgroup()
{
	
	float m  = 0.0f;
	Matrix3f Asphere;
	Asphere.clear();

	for(int im=0;im<assPsNum;im++)
	{
		PHOpParticle &dp = objPArr[im];
		dp.GetAellipMtrx();
	}


	for(int im=0;im<assGrpNum;im++)
	{
		
			Matrix3f Axxb;
			Axxb.clear();//debug Axxb
			
			PHOpGroup &pg = objGArr[im];
			Vec3f gCurrentCenter = gCurrentCenter.Zero();
	
			Matrix3f Agroup;
			Agroup.clear();
			PHOpParticle &dp = objPArr[0];
#ifdef CHECK_INF_ERR
			if (!FloatErrorTest::CheckBadFloatValue(dp.pCurrCtr.x))
			{
				int u = 0;
				return;
			}
#endif
			int pcount = pg.gNptcl;
			for(int j=0;j<pcount;j++)
			{
				
				PHOpParticle &dp = objPArr[pg.getParticleGlbIndex(j)];
				
				Vec3f x = dp.pNewCtr;
				Vec3f xb = dp.pOrigCtr;
				
			
				m = dp.pTotalMass;
				x = x * m;
				Axxb.ExX() = x.x * xb.x;
				Axxb.ExY() = x.x * xb.y;
				Axxb.ExZ() = x.x * xb.z; 
				Axxb.EyX() = x.y * xb.x;
				Axxb.EyY() = x.y * xb.y;
				Axxb.EyZ() = x.y * xb.z; 
				Axxb.EzX() = x.z * xb.x; 
				Axxb.EzY() = x.z * xb.y;
				Axxb.EzZ() = x.z * xb.z;

				Agroup += dp.pMomentR;
				Agroup += Axxb;
				
				//現在グループ中心を計算

				gCurrentCenter += x ;
			}
#ifdef CHECK_INF_ERR
			if (!FloatErrorTest::CheckBadFloatValue(Agroup.xx, Agroup.yx, Agroup.zx))
			{
				int u = 0;
				return;
			}
#endif
		
			pg.gCurrCenter = gCurrentCenter * (1.0f / pg.gtotalMass);
			
			Matrix3f Mccb;
			Mccb.clear();

			Vec3f c = pg.gCurrCenter;
			Vec3f cb = pg.gOrigCenter;
			
			m = pg.gtotalMass;
			c = c * m;
			Mccb.ExX() = c.x * cb.x;
			Mccb.ExY() = c.x * cb.y;
			Mccb.ExZ() = c.x * cb.z;
			Mccb.EyX() = c.y * cb.x;
			Mccb.EyY() = c.y * cb.y;
			Mccb.EyZ() = c.y * cb.z;
			Mccb.EzX() = c.z * cb.x;
			Mccb.EzY() = c.z * cb.y;
			Mccb.EzZ() = c.z * cb.z;

			pg.gAgroup = Agroup - Mccb;
			;
	}
}

void PHOpObj::newReducedPositionProject()
{
	//粒子の慣性マトリクスを計算する（毎STEP一回）
	for (int im = 0; im<assPsNum; im++)
	{
		PHOpParticle &dp = objPArr[im];
		dp.GetAellipMtrx();
	}


	Matrix3f R, S;

	Vec3d px0_cx0;
	R = R.Zero();
	S = S.Zero();
	//polarDecomposition pd;

	float m = 0.0f;
	//計算は毎グループで行われている
	for (int i = 0; i < assPsNum; i++)
	{

		PHOpGroup &pg = objGArr[i];

		Matrix3f Axxb;
		Axxb.clear();


		Vec3f gCurrentCenter = gCurrentCenter.Zero();

		Matrix3f Agroup;
		Agroup.clear();
		//グループの足し算
		int pcount = pg.gNptcl;
		for (int j = 0; j<pcount; j++)
		{

			PHOpParticle &dp = objPArr[pg.getParticleGlbIndex(j)];
			
			Vec3f x = dp.pNewCtr;
			Vec3f xb = dp.pOrigCtr;


			m = dp.pTotalMass;
			x = x * m;
			Axxb.ExX() = x.x * xb.x;
			Axxb.ExY() = x.x * xb.y;
			Axxb.ExZ() = x.x * xb.z;
			Axxb.EyX() = x.y * xb.x;
			Axxb.EyY() = x.y * xb.y;
			Axxb.EyZ() = x.y * xb.z;
			Axxb.EzX() = x.z * xb.x;
			Axxb.EzY() = x.z * xb.y;
			Axxb.EzZ() = x.z * xb.z;

			Agroup += dp.pMomentR;
			Agroup += Axxb;

			//現在グループ中心の計算

			gCurrentCenter += x;
		}


		pg.gCurrCenter = gCurrentCenter * (1.0f / pg.gtotalMass);

		Matrix3f Mccb;
		Mccb.clear();

		Vec3f c = pg.gCurrCenter;
		Vec3f cb = pg.gOrigCenter;

		m = pg.gtotalMass;
		c = c * m;
		Mccb.ExX() = c.x * cb.x;
		Mccb.ExY() = c.x * cb.y;
		Mccb.ExZ() = c.x * cb.z;
		Mccb.EyX() = c.y * cb.x;
		Mccb.EyY() = c.y * cb.y;
		Mccb.EyZ() = c.y * cb.z;
		Mccb.EzX() = c.z * cb.x;
		Mccb.EzY() = c.z * cb.y;
		Mccb.EzZ() = c.z * cb.z;

		pg.gAgroup = Agroup - Mccb;


		//Polardecomposition、ここはJacobiで解く

		//回転行列を抽出
		//avoid bug of infinite in polardecom
		R = SolveShpMchByJacobi(pg);
		//pd.polarDecom(R, S, pg.gAgroup);
#ifdef CHECK_INF_ERR
		if (!FloatErrorTest::CheckBadFloatValue(R.xx, R.yx, R.yz))
		{
			int u = 0;
			continue;
		}
#endif
		MatrixExtension me;


		//この回転行列はグループ中心粒子で保存
		PHOpParticle &centerDp = objPArr[pg.getParticleGlbIndex(0)];
		centerDp.pSmR = R;

		//またcenter粒子の向きはshape-matchingの回転行列で決める（論文5.1　第三節）
		Spr::TQuaternion<float> orin;
		orin.FromMatrix(R);
		orin = orin * centerDp.pOrigOrint;
		centerDp.pNewOrint = orin;

		Vec3f pNbc = pNbc.Zero();
		Vec3f pGoal = pGoal.Zero();
		
		for (int im = 0; im < (int) pg.gPInd.size(); im++)
		{
			PHOpParticle &dp_g = objPArr[pg.gPInd[im]];
			
			if (dp_g.isFixed)
			{
				continue;
			}
			
			pGoal = me.MatrixMultiVec3fRight(R, (dp_g.pOrigCtr - pg.gOrigCenter));
			dp_g.pNewCtr = (1 - params.alpha) * dp_g.pNewCtr + params.alpha * (pGoal + pg.gCurrCenter);
		
		}

	}

	return;


}
/*
@brief 元のparticle到達位置計算
*/
void PHOpObj::positionProject()
{
	Matrix3f R,S;
	Vec3d px0_cx0;
	R = R.Zero();
	S = S.Zero();
	polarDecomposition pd;

	
	for(int i =0;i < assGrpNum;i++)
	{

		PHOpGroup &pg = objGArr[i];
		pd.polarDecom(R,S,pg.gAgroup);
		//beta
		R = R *(1-params.beta) + pg.gAgroup * (params.beta);//or S?

		MatrixExtension me;

		// position calculation
		for(int j=0;j<pg.gNptcl;j++)
			{
				PHOpParticle &dp = objPArr[pg.getParticleGlbIndex(j)];

				px0_cx0 = dp.pOrigCtr - pg.gOrigCenter;

				
				Vec3f tmp = me.MatrixMultiVec3fRight(R,px0_cx0) + pg.gCurrCenter;
				
				dp.pGoalCtr.push_back(tmp);
			
				
			}

		//calculate orientation
		PHOpParticle &centerDp = objPArr[pg.getParticleGlbIndex(0)];
		centerDp.pSmR = R;
		//centerDp.pMomentR
		Spr::TQuaternion<float> orin ;
		orin.FromMatrix(R);
		orin = orin * centerDp.pOrigOrint;
		centerDp.pNewOrint = orin;
	}

	Vec3f newpSum = Vec3f(0,0,0);
	Vec3f newpSum1 = Vec3f(0,0,0);
		for(int im=0;im<assPsNum;im++)
			{

				PHOpParticle &dp = objPArr[im];
				if (dp.isFixed)
				{
					continue;
				}
				int goalsize = (int)dp.pGoalCtr.size();//重なった計算数
				float scale = 1.0f / (float)goalsize;
				newpSum = dp.pNewCtr;
				newpSum1 = newpSum1.Zero();
				for(int j =0;j < goalsize;j++) //debug goalposition
				{
					newpSum += (dp.pGoalCtr[j] - dp.pNewCtr) * scale* params.alpha ;
				}

				for(int j =0;j < goalsize;j++) //debug goalposition
				{
					newpSum1 += dp.pGoalCtr[j];

				}
			
				 dp.pNewCtr = newpSum1 * scale ;
				
				
				dp.pGoalCtr.clear();
				
		}
	
}
/*
@brief 最適化したParticle到達位置計算
*/
void PHOpObj::ReducedPositionProject()
{
	Matrix3f R,S;

	Vec3d px0_cx0;
	R = R.Zero();
	S = S.Zero();
	polarDecomposition pd;

	
	for(int i =0;i < assGrpNum;i++)
	{

		PHOpGroup &pg = objGArr[i];
		//pd.NewPolarDecomposition(pg.gAgroup,R);

		//Old R+S polardecomposition
		//{
		pd.polarDecom(R,S,pg.gAgroup);
		////R = SolveShpMchByJacobi(pg);
		//
		////R = R.Unit(); //debug R

		//beta
		//R = R *(1-params.beta) + pg.gAgroup *(params.beta);
		R = R *(1-params.beta) + S *(params.beta);
		//}

		//avoid bug of infinite in polardecom
		//R = SolveShpMchByJacobi(pg);


		//calculate orientation
		PHOpParticle &centerDp = objPArr[pg.getParticleGlbIndex(0)];
		centerDp.pSmR = R;

		//centerDp.pMomentR
		Spr::TQuaternion<float> orin ;
		orin.FromMatrix(R);
		orin = orin * centerDp.pOrigOrint;
		centerDp.pNewOrint = orin;
	}

	MatrixExtension me;
	Vec3f newpSum = Vec3f(0,0,0);
	Vec3f newpSum1 = Vec3f(0,0,0);
		for(int im=0;im<assPsNum;im++)
			{

				PHOpParticle &dp = objPArr[im];
				if (dp.isFixed)
				{
					continue;
				}
				
				//Reduced Op goalPostion calculation
				Vec3f rSum = rSum.Zero();
				Vec3f pNbc = pNbc.Zero();
				int l_i = (int)dp.pInGrpList.size();
				
				for(int k=0;k < l_i;k++)
				{
					PHOpParticle &dp_k = objPArr[dp.pInGrpList[k]];
					PHOpGroup &pg_k = objGArr[dp.pInGrpList[k]];
				
					rSum += me.MatrixMultiVec3fRight(dp_k.pSmR, (dp.pOrigCtr - pg_k.gOrigCenter));//\sigma {k}{l_i}{Rgpk *(xibar - xgkbar)}
#ifdef CHECK_INF_ERR
					if(!FloatErrorTest::CheckBadFloatValue(rSum.x,rSum.y,rSum.z))
						int u=0;
#endif

					pNbc += pg_k.gCurrCenter;
				}
				pNbc = pNbc/l_i;
				rSum = rSum/l_i;
				newpSum1= (1 - params.alpha) * dp.pNewCtr + params.alpha * (rSum + pNbc);

				 dp.pNewCtr = newpSum1;
				
#ifdef CHECK_INF_ERR
				if(!FloatErrorTest::CheckBadFloatValue(newpSum1.x,newpSum1.y,newpSum1.z))
						int u=0;
#endif

				dp.pGoalCtr.clear();
				
		}
	
}
/*
@brief 時間積分
pNewCtrからpCurrCtrへ更新
*/
void PHOpObj::integrationStep()
{
	int arrind = 0;

	for (int i = 0; i<assPsNum; i++)
	{
		//update position
		PHOpParticle &dp = objPArr[i];
		double dt1 = 1.0f / (params.timeStep);
		if (dp.isFixedbyMouse || dp.isFixed)
		{
			dp.pVelocity = dp.pVelocity.Zero();
		}
		else {
				dp.pVelocity = (dp.pNewCtr - dp.pCurrCtr) * dt1;
				dp.pCurrCtr = dp.pNewCtr;
			
		}
	
		// angular velocity
		Spr::TQuaternion<float>	&newOrint = dp.pNewOrint;
#ifdef	_MSC_VER
		Spr::TQuaternion<float>	&currInv = dp.pCurrOrint.Inv();
#else
		Spr::TQuaternion<float>	tmp1 = dp.pCurrOrint.Inv();
		Spr::TQuaternion<float>	&currInv = tmp1;
#endif

		
		float dotProd = newOrint.dot(currInv);

		if (dotProd < 0.0f) {
			newOrint = -1.0f * newOrint;
		}

#ifdef	_MSC_VER
		Spr::TQuaternion<float>	&tmp = newOrint * currInv;
#else
		Spr::TQuaternion<float>	tmp2 = newOrint * currInv;
		Spr::TQuaternion<float>	&tmp = tmp2;
#endif
		Vec3f axis = tmp.Axis();
		//angle Velocity calculate
		float angle = 0.0;
		tmp.unitize();

		float w = tmp.w;
		//float w	= fabs(tmp.w);

		if ((w < 1.0f || w == 1.0f) && (w > -1.0f || w == -1.0f)){
			angle = acos(w) * 2.0f;
			//angle = fabs(angle);
		}
		else {
			int u = 0;
		}

		
		if (angle > 0.0001f || angle < -0.0001f) {
			Vec3f formerW = dp.pWvel;
			formerW.x = -formerW.x;
			formerW.y = -formerW.y;
			formerW.z = -formerW.z;
			dp.pWvel = (angle / params.timeStep) * axis;
			//prevent unstable angular Velocity
			/*	float length = (formerW - dp.pWvel).norm();
			if(length < 0.1)
			dp.pWvel = formerW.Zero();*/

			int u = 0;
		}
		else {
			dp.pWvel = axis.Zero();
		}

		//update orientation
		dp.pNewOrint.unitize();
		dp.pCurrOrint = dp.pNewOrint;
	}

}

Matrix3f PHOpObj::SolveShpMchByJacobi(PHOpGroup &pg)
{
	Matrix3f transA = pg.gAgroup.trans() * pg.gAgroup;
		Matrix3f W		= pg.gEigen.trans() * transA * pg.gEigen; // warm start [Rivers 2007]

		Vec3f eigenVs = Vec3f();

		//Jacobi j;
		j.eigenValues  = eigenVs;
		j.eigenVectors = pg.gEigen;
		j.matrix = W;
		j.Do();
		eigenVs = j.eigenValues;
		pg.gEigen = j.eigenVectors;

		for (int i = 0; i < 3; ++ i) {
					if (eigenVs[i] <= 0.0f) {
						eigenVs[i] = 0.0001f;
					}
					eigenVs[i] = 1.0f / sqrt(eigenVs[i]);
				}
				
		Matrix3f DPrime = DPrime.Unit();
				DPrime.xx = eigenVs[0];
				DPrime.yy = eigenVs[1];
				DPrime.zz = eigenVs[2];
				
				Matrix3f SInverse	= pg.gEigen * DPrime * pg.gEigen.trans();
				Matrix3f R			= pg.gAgroup * SInverse;
				
				
		return R;
}
/*
@brief 速度Damper
*/
void PHOpObj::dampVelocities()
{
	float drag = 1.0f - 0.005f;

	if (assPsNum == 1)
	{
		objPArr[0].pVelocity -= params.veloDamping *objPArr[0].pVelocity;
		return;
	}

	
	Vec3f xcm = xcm.Zero();
	Vec3f vcm = vcm.Zero();

	float m = 0.0f;

	for(int i=0;i<assPsNum;i++)
	{
		PHOpParticle &dp = objPArr[i];
		xcm += dp.pCurrCtr * dp.pTotalMass;
		vcm += dp.pVelocity * dp.pTotalMass;
		m += dp.pTotalMass;
	}

	xcm *= 1.0f / m;
	vcm *= 1.0f / m;

	Vec3f L = L.Zero();
	Matrix3f I = I.Zero();

	for(int i=0;i<assPsNum;i++)
	{
	
		PHOpParticle &dp = objPArr[i];
		Vec3f Ri = dp.pCurrCtr - xcm;
		Vec3f tmp	= PTM::cross(Ri, (dp.pVelocity * dp.pTotalMass));
				
				L += tmp;
				
				Matrix3f RTilde	= RTilde.Zero();
				RTilde.xy = - Ri.z;
				RTilde.xz = + Ri.y;
				RTilde.yx = + Ri.z;
				RTilde.yz = - Ri.x;
				RTilde.zx = - Ri.y;
				RTilde.zy = + Ri.x;
				
				Matrix3f tmpM	= RTilde * RTilde.trans();
				
				I += dp.pTotalMass * tmpM;
	}
			
			Vec3f w = I.inv() * L;
			


	for(int i=0;i<assPsNum;i++)
	{
		PHOpParticle &dp = objPArr[i];
			
				Vec3f Ri = dp.pCurrCtr - xcm;
				Vec3f DVi = vcm + PTM::cross(w, Ri) - dp.pVelocity;
				
				dp.pVelocity += params.veloDamping * DVi;
	}
	
}

}// namespace

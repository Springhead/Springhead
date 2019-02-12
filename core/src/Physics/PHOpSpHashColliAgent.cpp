/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#include "PHOpSpHashColliAgent.h"
#include "Physics/PHOpEngine.h"

namespace Spr{
	;

#define USE_AVE_RADIUS

	int TestSphereSphere(Vec3f a_c, float a_r, Vec3f b_c, float b_r)
	{
		// Calculate squared distance between centers
		Vec3f d = a_c - b_c;
		float dist2 = d.dot(d);
		// Spheres intersect if squared distance is less than squared sum of radii
		float radiusSum = a_r + b_r;
		return dist2 <= radiusSum * radiusSum;
	}
	int TestMovingSphereSphere(Vec3f s0_c, float s0_r, Vec3f d, float t0, float t1, Vec3f s1_c, float s1_r, float &t)
	{
		// Compute sphere bounding motion of s0 during time interval from t0 to t1
		//Sphere b;
		float mid = (t0 + t1) * 0.5f;
		Vec3f spb_c = s0_c + d * mid;
		float spb_r = (mid - t0) * d.norm() + s0_r;
		// If bounding sphere not overlapping s1, then no collision in this interval
		if (!TestSphereSphere(spb_c, spb_r, s1_c, s1_r)) return 0;
		// Cannot rule collision out: recurse for more accurate testing. To terminate the
		// recursion, collision is assumed when time interval becomes sufficiently small
		//	if (((t1 - t0 < 0.0001f) && (t1 - t0>0)) || (t1 - t0 > -0.0001) && (t1 - t0<0)) {
		if (t1 - t0 < 0.0001f){
		
			t = t0;
			return 1;
		}
		// Recursively test first half of interval; return collision if detected
		if (TestMovingSphereSphere(s0_c, s0_r, d, t0, mid, s1_c, s1_r, t)) return 1;
		// Recursively test second half of interval
		return TestMovingSphereSphere(s0_c, s0_r, d, mid, t1, s1_c, s1_r, t);
	}


	void PHOpSpHashColliAgent::EnableCollisionDetection(bool able)
	{
		enableCollision = able;
	}
	void PHOpSpHashColliAgent::Initial(float cellSize, CDBounds bounds)
	{
		useDirColli = true;
		CDSpatialHashTableIf* cdif = (spTable)->Cast();
		DSTR << "cdif" << cdif << std::endl;
		cdif->Init(cellSize, bounds);
		UpdateSceneInfo();
		EllipColliInit();

	}
	void PHOpSpHashColliAgent::UpdateSceneInfo()
	{
		scene = (PHSceneIf*)GetScene();
		opengIf = scene->GetOpEngine()->Cast();
		opEngine = DCAST(PHOpEngine, opengIf);
		objNum = (int)opEngine->opObjs.size();

	}
	void PHOpSpHashColliAgent::ClearDebugData()
	{
		if (useDebugInfo)
		{
			std::vector<Vec3f> swp;
			swp.swap(pPCtcPs);
			std::vector<Vec3f> swp2;
			swp2.swap(pPCtcPs2);
			std::vector<Vec3f> swp3;
			swp3.swap(pPCtcPs3);
			std::vector<Vec3f> swp4;
			swp4.swap(pPCtcPs4);
		}
	}
	bool PHOpSpHashColliAgent::IsCollisionEnabled()
	{
		return enableCollision;
	}
	
	
	void PHOpSpHashColliAgent::OpCollisionProcedure()
	{
		timeStamp++;
		if (timeStamp > (std::numeric_limits<int>::max()))
			timeStamp = 0;

		ClearDebugData();
		UpdateSceneInfo();

		//clear flags
		for (int obji = 0; obji < objNum; obji++)
		{
			std::vector<PHOpObj*>& dfmObj = opEngine->opObjs;
			for (int j = 0; j < dfmObj[obji]->assPsNum; j++)
			{
				PHOpParticle &dp = dfmObj[obji]->objPArr[j];
				dp.isColliedbyColliCube = false;
				dp.isColliedbySphash = false;
				dp.isColliedSphashSolved = false;
				dp.isColliedSphashSolvedReady = false;
				
			}
		}

		if (!useDirColli)
		OpCollisionProcedure(timeStamp);
		else{
			OpDirCollision();
		}
	}
	void PHOpSpHashColliAgent::SetIsDirCollision(bool flag)
	{
		useDirColli = flag;
	}
	bool PHOpSpHashColliAgent::GetIsDirCollision()
	{
		return useDirColli;
	}
	void PHOpSpHashColliAgent::SetCollisionCstrStiffness(float alpha)
	{
		collisionCstrStiffness = alpha;
	}
	float PHOpSpHashColliAgent::GetCollisionCstrStiffness()
	{
		return collisionCstrStiffness;
	}

	void PHOpSpHashColliAgent::OpDirCollision()
	{//directly test all particle pairs

		std::vector<PHOpObj*>& dfmObj = opEngine->opObjs;

		for (int obji = 0; obji<objNum - 1; obji++)
		{
			for (int pi = 0; pi < dfmObj[obji]->assPsNum; pi++)
			{

				PHOpParticle &dp = dfmObj[obji]->objPArr[pi];
				for (int objiw = obji + 1; objiw < objNum; objiw++)
				{
					for (int pj = 0; pj < dfmObj[objiw]->assPsNum; pj++)
					{
						PHOpParticle &dp1 = dfmObj[objiw]->objPArr[pj];
						Vec3f dis = dp.pNewCtr - dp1.pNewCtr;
						spHashColliContact(obji, pi, objiw, pj, dis);
					}
				}
			}
		}
	}
	void PHOpSpHashColliAgent::OpCollisionProcedure(int myTimeStamp)
	{
		

		std::vector<PHOpObj*>& dfmObj = opEngine->opObjs;

		int colliNum = 0;
		float enlarge = 5.0f;
		float fitmodelCoe = enlarge;

		for (int obji = 0; obji<objNum; obji++)
		{
			if (dfmObj[obji]->objNoMeshObj)
				continue;
			
			if (spTable->cellsize != -1)
			{
				spTable->ColliedHashIndexList.clear();
				std::vector<int> swap;
				swap.swap(spTable->ColliedHashIndexList);

				//余計な計算です,maxradius使うかどうか次第
#ifdef USE_MAX_RADIUS
				float maxradius = 0.0f;
				for (int obji = 0; obji<objNum; obji++)
				{
					if (dfmObj[obji]->maxRadius>maxradius)
						maxradius = dfmObj[obji]->maxRadius;
				};
#endif							
				//楕円のboxをHashCellに加入
				for (int j = 0; j<dfmObj[obji]->assPsNum; j++)
				{
					PHOpParticle &dp = dfmObj[obji]->objPArr[j];
					
					colliNum++;
					//どんな半径で衝突検出をする
					//if(spTable->selectPointsToAdd(dp.pCurrCtr,dp.pMainRadius,obji,j,myTimeStamp))
#ifdef USE_AVE_RADIUS
					//if(spTable->selectPointsToAdd(dp.pNewCtr,dfmObj[obji]->averRadius,obji,j,myTimeStamp))
					if (spTable->ReducedSelectPointsToAdd(dp.pNewCtr, dfmObj[obji]->objAverRadius * fitmodelCoe
						, obji, j, myTimeStamp))
#endif
#ifdef USE_MAX_RADIUS
						if (spTable->selectPointsToAdd(dp.pCurrCtr, maxradius, obji, j, myTimeStamp))
#endif
#ifdef USE_DYN_RADIUS
							//if (spTable->selectPointsToAdd(dp.pNewCtr, dp.pDynamicRadius, obji, j, myTimeStamp))
							if (spTable->ReducedSelectPointsToAdd(dp.pNewCtr, dp.pDynamicRadius, obji, j, myTimeStamp))
#endif
							{

						colliCheckedBySphash = true;
							}
					
				}


			}
			
		}

	
		//					
		
		//Hash中身をチャック
		ReducedSpHashColliCheck();
		
		spHashColliPlane();

	}


	/*
	@brief Hash中身をチャック
	*/
	void PHOpSpHashColliAgent::ReducedSpHashColliCheck()
	{
		PHSceneIf* scene = (PHSceneIf*)GetScene();
		PHOpEngineIf* opengIf = scene->GetOpEngine()->Cast();
		PHOpEngine* opEngine = DCAST(PHOpEngine, opengIf);
		int objNum = (int)opEngine->opObjs.size();
		std::vector<PHOpObj*>& dfmObj = opEngine->opObjs;

		solveCount = 0;

		if (colliCheckedBySphash)
		{
			//make unique
			std::sort(spTable->ColliedHashIndexList.begin(), spTable->ColliedHashIndexList.end());
			std::vector<int>::iterator iter_end;
			iter_end = std::unique(spTable->ColliedHashIndexList.begin(), spTable->ColliedHashIndexList.end());
			spTable->ColliedHashIndexList.erase(iter_end, spTable->ColliedHashIndexList.end());

			int size = (int)spTable->ColliedHashIndexList.size();
			std::vector<solvedColliPair> solvedColliPairs;
			//チェック必要な
			for (int i = 0; i < size; i++)
			{

				int differObjColli = 0;
				bool twoObjExist = false;
				int hashId = spTable->ColliedHashIndexList[i];
				int colliLength = spTable->ptclColliList[hashId].collilength;

				int limitObjInd = spTable->ptclColliList[hashId].pcInfolist[0].objIndex;
				for (int cli = 0; cli < colliLength; cli++)
				{


					int otherObj = spTable->ptclColliList[hashId].pcInfolist[cli].objIndex;
					if (otherObj != limitObjInd)
					{
						twoObjExist = true;
						//if two obj collied remember the hash index, use it later
						//colliedIndexes.push_back( spTable->colliPairList[i].hashIndex);
						differObjColli++;
					}


				}
				//二つ目のobjが発見した場合
				if (twoObjExist)
				{
					for (int cli = 0; cli < colliLength; cli++)
					{
						int pind1 = spTable->ptclColliList[hashId].pcInfolist[cli].ptclIndex;//no self colli if wanted need change this

						int oind1 = spTable->ptclColliList[hashId].pcInfolist[cli].objIndex;

						//haptic imp
						PHOpObj *dObj1; 
						
							
						dObj1 = dfmObj[oind1];

#ifdef USE_AVE_RADIUS
						float radi1 = dObj1->objAverRadius;
#endif
#ifdef USE_DYN_RADIUS
						float radi1 = dObj1->objPArr[pind1].pDynamicRadius;
#endif
						dObj1->objPArr[pind1].isColliedbySphash = true;

						Vec3f &objP1 = dObj1->objPArr[pind1].pCurrCtr;
						for (int clk = cli + 1; clk<colliLength; clk++)
						{
							int oind2 = spTable->ptclColliList[hashId].pcInfolist[clk].objIndex;
							int pind2 = spTable->ptclColliList[hashId].pcInfolist[clk].ptclIndex;
							
							PHOpObj *dObj2;
							
								dObj2 = dfmObj[oind2];
#ifdef USE_AVE_RADIUS

							float radi2 = dObj2->objAverRadius;
#endif
#ifdef USE_DYN_RADIUS
							float radi2 = dObj2->objPArr[pind2].pDynamicRadius;
#endif
							//no self colli
							if (oind2 != oind1)
							{
								Vec3f &objP2 = dObj2->objPArr[pind2].pCurrCtr;
								Vec3f linkVec = objP2 - objP1;
								float dist = fabs(linkVec.norm());
								float twinRadi = radi1 + radi2;

								if (dist < twinRadi)
								{
									//one pair check only once
									bool pairExist = false;


									for (int si = 0; si < (int) solvedColliPairs.size(); si++)

									{
										int cp1 = solvedColliPairs[si].cPInd1;
										int cp2 = solvedColliPairs[si].cPInd2;
										int co1 = solvedColliPairs[si].cOInd1;
										int co2 = solvedColliPairs[si].cOInd2;

										if ((cp1 == pind1&&cp2 == pind2&&co1 == oind1&&co2 == oind2))//(cp1==pind2&&cp2==pind1&&co1==oind2&&co2==oind1)
										{
											pairExist = true;
										}
										int u = 0;
									}

									if (!pairExist)
									{

										spHashColliContact(oind1, pind1, oind2, pind2, linkVec);
										solvedColliPair tmpP;
										tmpP.cOInd1 = oind1; tmpP.cOInd2 = oind2;
										tmpP.cPInd1 = pind1; tmpP.cPInd2 = pind2;
										solvedColliPairs.push_back(tmpP);

									}
									
								}
							}
							
						}
					}
				}
			}
		}
	}

	/*
	@brief 衝突がある場合の関数
	*/
	void PHOpSpHashColliAgent::spHashColliContact(int objIndex1, int ptclindex1, int objIndex2, int ptclindex2, Vec3f &linkVec)
	{

		std::vector<PHOpObj*>& dfmObj = opEngine->opObjs;


		PHOpObj *dObj1;
		PHOpObj *dObj2;
		PHOpParticle *dp1;
		PHOpParticle *dp2;
		float ellRadius1;
		float ellRadius2;
		int hcColliedIndex = -1;

		//if hp is only a point ctc detection can be speed up

		//use when hp is not only a point 
		//haptic imp
		/*if (objIndex1 == myHc->GetHpObjIndex())
		{
		dObj1 = myHc;
		dp1 = &myHc->objPArr[ptclindex1];
		ellRadius1 = myHc->averRadius;
		hcColliedIndex = 1;
		}
		else */
		{
			dObj1 = dfmObj[objIndex1];
			dp1 = &dfmObj[objIndex1]->objPArr[ptclindex1];
#ifdef USE_AVE_RADIUS
			ellRadius1 = dfmObj[objIndex1]->objAverRadius;
#endif
#ifdef USE_DYN_RADIUS
			ellRadius1 = dp1->pDynamicRadius;
#endif 


		}
		//haptic imp
		/*if (objIndex2 == myHc->GetHpObjIndex())
		{
		dObj2 = myHc;
		dp2 = &myHc->objPArr[ptclindex2];
		ellRadius2 = myHc->averRadius;
		hcColliedIndex = 2;
		}
		else*/
		{
			dObj2 = dfmObj[objIndex2];
			dp2 = &dfmObj[objIndex2]->objPArr[ptclindex2];
#ifdef USE_AVE_RADIUS
			ellRadius2 = dfmObj[objIndex2]->objAverRadius;
#endif
#ifdef USE_DYN_RADIUS
			ellRadius2 = dp2->pDynamicRadius;
#endif
		}
		//sphere colli solve only distance
		/*float moveDist = (linkVec.norm() - (dp1.pRadii + dp2.pRadii)) * 0.5;
		linkVec.unitize();
		dp2.pColliedForceMove = linkVec * moveDist;
		dp1.pColliedForceMove = linkVec * moveDist;*/

		//return;
		float pPdist = linkVec.norm();



		//if (pPdist > (ellRadius1 + ellRadius2))
		//	return;


		Vec3f &dpCtr1 = dp1->pNewCtr;
		Vec3f &dpCtr2 = dp2->pNewCtr;
		Spr::TQuaternion<float> &pOrint1 = dp1->pNewOrint;
		Spr::TQuaternion<float> &pOrint2 = dp2->pNewOrint;


		//sphere colli test
		if (0)
		{
			dp1->isColliedSphashSolvedReady = true;
			dp2->isColliedSphashSolvedReady = true;


			Vec3f dir = dpCtr1 - dpCtr2;
			dir.unitize();
			//add ctcps
			//pPCtcPs.push_back();


			if (!FloatErrorTest::CheckBadFloatValue(dir))
			{
				dir = dir.Zero();
				DSTR << "Collision dir has bad float value!" << std::endl;
				return;
			}

			if (dir.norm() < (ellRadius1 + ellRadius2))
			{

				//set colli flag
				dp1->isColliedSphashSolved = true;
				dp2->isColliedSphashSolved = true;

				//hapticInform
				/*if (hcColliedIndex >0)
				{
				myHc->SetHCColliedFlag(true);
				if (hcColliedIndex == 1)
				{
				myHc->AddColliedPtcl(ptclindex2, objIndex2, dir * ellRadius2 + dpCtr2);
				}
				else if (hcColliedIndex == 2)
				{
				myHc->AddColliedPtcl(ptclindex1, objIndex1, -dir * ellRadius1 + dpCtr1);
				}
				}*/
				//else
				//solve
#ifdef	_MSC_VER
				pctlColliSolve(objIndex1, ptclindex1, objIndex2, ptclindex2, -dir * ellRadius1 + dpCtr1, dir * ellRadius2 + dpCtr2);
#else
				Vec3f tmp1 = -dir * ellRadius1 + dpCtr1;
				Vec3f tmp2 = dir * ellRadius2 + dpCtr2;
				pctlColliSolve(objIndex1, ptclindex1, objIndex2, ptclindex2, tmp1, tmp2);
#endif
				Vec3f &dpCtr1 = dp1->pNewCtr;
				Vec3f &dpCtr2 = dp2->pNewCtr;
				float mindeltaT = 1.0f;
				//TestMovingSphereSphere(dpCtr1, ellRadius1, dpCtr2 - dpCtr1, 0.01f, 0.01f, dpCtr2, ellRadius2, mindeltaT);

				solveCount++;
			}

			return;
		}

		//if ((ptclindex1 == 15 && ptclindex2 == 55)|| (ptclindex1 == 55 && ptclindex2 == 15))
		{

	

		//correct ellipsoid collision solve
		MatrixExtension me;

		//Calculate EllipMatrix per step
		/*dp1->pPreEllipA.xx = 1.0f / (dp1->pMainRadius*dp1->pMainRadius);
		dp1->pPreEllipA.yy = 1.0f / (dp1->pSecRadius * dp1->pSecRadius);
		dp1->pPreEllipA.zz = 1.0f / (dp1->pThrRadius * dp1->pThrRadius);

		dp2->pPreEllipA.xx = 1.0f / (dp2->pMainRadius * dp2->pMainRadius);
		dp2->pPreEllipA.yy = 1.0f / (dp2->pSecRadius * dp2->pSecRadius);
		dp2->pPreEllipA.zz = 1.0f / (dp2->pThrRadius * dp2->pThrRadius);*/

		Matrix3f ROrit1;
		Matrix3f ROrit2;
		ROrit1 = ROrit1.Zero();
		ROrit2 = ROrit2.Zero();

		pOrint1.Inv().ToMatrix(ROrit1);
		ROrit1 = me.MatrixesMultiply3f(dp1->ellipRotMatrix, ROrit1);
		Matrix3f ellipA1tmp = me.MatrixesMultiply3f(ROrit1.trans(), dp1->pPreEllipA);
		ellipA1tmp = me.MatrixesMultiply3f(ellipA1tmp, ROrit1);


		pOrint2.Inv().ToMatrix(ROrit2);
		ROrit2 = me.MatrixesMultiply3f(dp2->ellipRotMatrix, ROrit2);
		Matrix3f ellipA2tmp = me.MatrixesMultiply3f(ROrit2.trans(), dp2->pPreEllipA);
		ellipA2tmp = me.MatrixesMultiply3f(ellipA2tmp, ROrit2);

		Vec3f vectorN1 = dpCtr2 - dpCtr1;//separate them by moving ellipsoid 2 along the direction defined by the normal vector n
		vectorN1.unitize();
		Vec3f vectorN2 = -vectorN1;



		float d = 0.0f;
		float lamdaP = 0.0f;
		Vec3f x1 = getCtcP_PvP(vectorN1, ellipA1tmp, ellipA2tmp,d,lamdaP);
		Vec3f x3 = x1;
		x1.multi(lamdaP * d);
		x3.multi(lamdaP * -d);

		Vec3f x2 = getCtcP_PvP(vectorN2, ellipA2tmp, ellipA1tmp, d, lamdaP);
		Vec3f x4 = x2;
		x2.multi(lamdaP * d);
		x4.multi(lamdaP * -d);

		

		//set ready flag
		dp1->isColliedSphashSolvedReady = true;
		dp2->isColliedSphashSolvedReady = true;

		
		float xdirs = ((x2 + dpCtr2) - (x1 + dpCtr1)).dot(vectorN1);
		//cout<<"xdirs = "<<xdirs<<endl;
		if (xdirs < 0)
		{

			//set colli flag
			dp1->isColliedSphashSolved = true;
			dp2->isColliedSphashSolved = true;


			if (useDebugInfo)
			{
				//add ctcps
				pPCtcPs.push_back(x1 + dp1->pNewCtr);
				pPCtcPs2.push_back(x2 + dp2->pNewCtr);
				pPCtcPs3.push_back(x3 + dp1->pNewCtr);
				pPCtcPs4.push_back(x4 + dp2->pNewCtr);
			}
			//hapticInform
			//haptic imp
			/*if (hcColliedIndex >0)
			{
			myHc->SetHCColliedFlag(true);
			if (hcColliedIndex == 1)
			{
			myHc->AddColliedPtcl(ptclindex2, objIndex2, x2 + dpCtr2);
			}
			else if (hcColliedIndex == 2)
			{
			myHc->AddColliedPtcl(ptclindex1, objIndex1, x1 + dpCtr1);
			}
			}
			else*/
			//solve
#ifdef	_MSC_VER
				pctlColliSolve(objIndex1, ptclindex1, objIndex2, ptclindex2, x1 + dpCtr1, x2 + dpCtr2);
#else
				Vec3f tmp1 = x1 + dpCtr1;
				Vec3f tmp2 = x2 + dpCtr2;
				pctlColliSolve(objIndex1, ptclindex1, objIndex2, ptclindex2, tmp1, tmp2);
#endif

			solveCount++;
		}
	}

	}
	void PHOpSpHashColliAgent::SetDebugMode(bool flag)
	{
		useDebugInfo = flag;
	}
	bool PHOpSpHashColliAgent::GetDebugMode()
	{
		return useDebugInfo;
	}
	/*
	@brief 衝突の解決
	*/
	void PHOpSpHashColliAgent::pctlColliSolve(int objIndex1, int ptclindex1, int objIndex2, int ptclindex2, Vec3f &ctc1, Vec3f &ctc2)
	{
		std::vector<PHOpObj*>& dfmObj = opEngine->opObjs;

		PHOpObj *dObj1;
		PHOpObj *dObj2;
		PHOpParticle *dp1;
		PHOpParticle *dp2;
		float ellRadius1;
		float ellRadius2;
		
		dObj1 = dfmObj[objIndex1];
		dp1 = &dfmObj[objIndex1]->objPArr[ptclindex1];
		ellRadius1 = dfmObj[objIndex1]->objAverRadius;

		dObj2 = dfmObj[objIndex2];
		dp2 = &dfmObj[objIndex2]->objPArr[ptclindex2];
		ellRadius2 = dfmObj[objIndex2]->objAverRadius;

		Vec3f &dpCtr1 = dp1->pNewCtr;
		Vec3f &dpCtr2 = dp2->pNewCtr;
		Vec3f &dpVel1 = dp1->pVelocity;
		Vec3f &dpVel2 = dp2->pVelocity;

		//penalty solution
		Vec3f x2tox1 = ctc1 - ctc2;
		Vec3f vectorN = x2tox1.unit();
		float moveDhalf = x2tox1.norm() * 0.5f;
		if (!FloatErrorTest::CheckBadFloatValue(vectorN))
		{
			vectorN = vectorN.Zero();

		}
		//move pos in N dir
		float totalMass = dp1->pTotalMass + dp2->pTotalMass;
		float length = fabs(x2tox1.norm()) * collisionCstrStiffness;


		dpCtr1 += -length * (dp2->pTotalMass / totalMass) * vectorN;
		dpCtr2 += length * (dp1->pTotalMass / totalMass) * vectorN;
		
	}
	
	void PHOpSpHashColliAgent::EllipColliInit()
	{//boost PvP collision
		std::vector<PHOpObj*>& dfmObj = opEngine->opObjs;

		

		for (int obji = 0; obji < objNum; obji++)
		{
			for (int pi = 0; pi < dfmObj[obji]->assPsNum; pi++)
			{
				PHOpParticle *dp = &dfmObj[obji]->objPArr[pi];
				Matrix3f &ellipA1 = dp->pPreEllipA;
				float ellRadius1 = dfmObj[obji]->objAverRadius/2;

				ellipA1 = ellipA1.Zero();

				ellipA1.xx = 1.0f / (dp->pMainRadius*dp->pMainRadius);
				ellipA1.yy = 1.0f / (dp->pSecRadius * dp->pSecRadius);
				ellipA1.zz = 1.0f / (dp->pThrRadius * dp->pThrRadius);
				
			}
		}
	}

	void PHOpSpHashColliAgent::spHashColliPlane()
	{
		std::vector<PHOpObj*>& dfmObj = opEngine->opObjs;
		float restitution = 0.9f;
		

		for (int obji = 0; obji<objNum; obji++)
		{
			for (int pci = 0; pci<dfmObj[obji]->assPsNum; pci++)
			{
				PHOpParticle& dp = dfmObj[obji]->getObjParticle(pci);
				Vec3f &dpCtr = dp.pNewCtr;
				
				for (int fi = 0; fi < (int)PlanePosList.size(); fi++)
				{
					Vec3f planeOrint = PlaneNorList[fi];

					planeOrint.unitize();
					Vec3f planePosition = PlanePosList[fi];

					Vec3f distV = dpCtr - planePosition;
					float dist = fabs(distV.dot(planeOrint));
					float distwithDir = distV.dot(planeOrint);

#ifndef COlliPlaneBallSolution
					
#ifdef USE_AVE_RADIUS
					if (dist < dfmObj[obji]->objAverRadius)
#endif
#ifdef USE_DYN_RADIUS
						if (dist <  dp.pDynamicRadius)
#endif
						{
							MatrixExtension me;
							//ellip:AInverse を計算する
							Matrix3f ellip = dp.pPreEllipA;
						
							Matrix3f ROrit, ROrit1;
							ROrit = ROrit.Zero();
							//notice here use the not inverse pCurrOrint!!
							dp.pCurrOrint.ToMatrix(ROrit);// debug currOrint inverse
							ROrit = me.MatrixesMultiply3f(ROrit, dp.ellipRotMatrix.inv());


						
							ellip = me.MatrixesMultiply3f(ROrit, ellip);
							ellip = me.MatrixesMultiply3f(ellip, ROrit.trans());
					
							Vec3f _xNumer = me.MatrixMultiVec3fRight(ellip, planeOrint);

							Vec3f _xDenomi;
							_xDenomi = me.MatrixMultiVec3fLeft(planeOrint, ellip);
							float LamdP = _xDenomi.dot(planeOrint);
							LamdP = sqrt(LamdP) ;
						
							Vec3f ctcPP = _xNumer / (LamdP);
							Vec3f ctcPN = -ctcPP;
							float OnEllipDistP1 = ctcPP.dot(planeOrint);//old distance computation
							float OnEllipDistP2 = ctcPN.dot(planeOrint);//old distance computation


					
							if ((fabs(OnEllipDistP1) < dist) || ((fabs(OnEllipDistP2) < dist)))
							{


								dfmObj[obji]->objPArr[pci].isColliedSphashSolved = true;

								float magni1 = fabs((planePosition - ctcPN).dot(planeOrint));
								float magni2 = fabs((planePosition - ctcPP).dot(planeOrint));
								float onEllipContactDistance = (OnEllipDistP1 > OnEllipDistP2) ? magni2 : magni1;


								dpCtr -= planeOrint * onEllipContactDistance* restitution * collisionCstrStiffness;


							}


						}

				}
#else
					//ball solution
					if (dist < dfmObj[obji]->averRadius)

					{
						Vec3f invertplaneOrint = -ctcPNorList[fi];
						dfmObj[obji]->objPArr[pci].isColliedbySphash = true;
						distV.unitize();
						float moveDist = dp.pRadii - dist;
						PHOpParticle& dp = dfmObj[obji]->getObjParticle(pci);

						if (distwithDir>0)
							dp.pCurrCtr = dp.pCurrCtr + (planeOrint * moveDist);
						else dp.pCurrCtr = dp.pCurrCtr + (invertplaneOrint * moveDist);

						int u = 0;
					}
			}
#endif

		}
	}

	}

	Vec3f PHOpSpHashColliAgent::getCtcP_PvP(Vec3f &vectorN, Matrix3f &ellipA1tmp, Matrix3f &ellipA2tmp, float &d, float& lamdaP)
	{
		MatrixExtension me;

		Matrix3f A1_A2inv = me.MatrixesMultiply3f(ellipA1tmp, ellipA2tmp.inv());

		Vec3f lamdV;
		lamdV = me.MatrixMultiVec3fLeft(vectorN, A1_A2inv);
	

		//we want lamda is  negtive means the collision is happen
		//solve lamda
		float lamdaDir = lamdV.dot(vectorN);
		lamdaP = sqrt(lamdaDir);

		//if(lamdaDir<0)
		//we set lamda to negtive (colli from the outside)
		lamdaP = -lamdaP;

		//solve matrix B
		Matrix3f B = (ellipA2tmp * lamdaP - ellipA1tmp).inv();
		B = me.MatrixesMultiply3f(B, ellipA2tmp);

		//solve d
		Matrix3f BtA1B = me.MatrixesMultiply3f(B.trans(), ellipA1tmp);
		BtA1B = me.MatrixesMultiply3f(BtA1B, B);

		Vec3f ntBtA1B;
		ntBtA1B = me.MatrixMultiVec3fLeft(vectorN, BtA1B);

		//d is need?
		float dwithDir = (ntBtA1B.dot(vectorN))*(lamdaP * lamdaP);
		d = sqrt(dwithDir);
		d = 1 / d;

		Vec3f x = me.MatrixMultiVec3fRight(B, vectorN);//debug change the sequance of multi
		
		return x;
	}

	
	void PHOpSpHashColliAgent:: AddContactPlane(Vec3f planeP, Vec3f planeN)
	{
		PlanePosList.push_back(planeP);
		PlaneNorList.push_back(planeN);
	}

}// namespace 

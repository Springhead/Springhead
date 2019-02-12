/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#include <Physics/PHOpHapticRenderer.h>
#define CHECK_INF_ERR

namespace Spr
{
	;
#ifdef USEGRMESH
void uniqueVector(std::vector<int> &vt)
{
	sort(vt.begin(), vt.end());
	vt.erase(unique(vt.begin(), vt.end()), vt.end());
}

void PHOpHapticRenderer::ProxySlvPlane()
{
	hitWall = false;
	PHOpParticle	*hdp = myHc->GetMyHpProxyParticle();
	Vec3f* target;

	if (myHc->CheckProxyState())
		target = &myHc->userPos;
	else target = &hdp->pNewCtr;



	for (int fi = 0; fi < (int)ctcPPosList.size(); fi++)
	{
		Vec3f& planeCenter = ctcPPosList[fi];
		Vec3f& planeOrint = ctcPNorList[fi];

		float dir = (planeCenter - *target).dot(planeOrint.unit());
		if (dir > 0)
		{
			hitWall = true;
			Vec3f add = dir * planeOrint.unit();
			add += planeOrint.unit() * myHc->proxyRadius;
			hdp->pNewCtr += add;


		}



	}


}

void PHOpHapticRenderer::ProxyTrace()
{
	std::vector<PHOpHapticController::ConstrainPlaneInfo>& cpiVec = myHc->cpiVec;

	PHOpParticle	*hdp = myHc->GetMyHpProxyParticle();
	Vec3f& currGoal = myHc->userPos;
	Vec3f& currSubStart = hdp->pCurrCtr;

	//Vec3f& currSubStart = hdp->pNewCtr;
	myHc->surrCnstrs = 0;
	int currConstrNum = myHc->constrainCount;
	//for no active proxy position fix

	Vec3f fixsubGoal;
#ifdef USE_a_little_over_InFix
	//float overDist = 1.01f;
	//float overDist2 = 0.001f;
	float overDist = 2.001f;
#else 
	float overDist = 1.0000f;
	float overDist2 = 0.0000f;
#endif
	bool obstInRoute = false;
	//make counterPlane for Proxy
	Vec3f counterPlane;

	//if (currConstrNum == 1)


#ifdef PROXYFIX_INFO
	DSTR << "Proxy fix currConstrNum" << currConstrNum << endl;
#endif



	Vec3f cnstredP;
	Vec3f cnstredN;
	for (int csti = 0; csti < myHc->constrainCount; csti++)
	{
		PHOpHapticController::ConstrainPlaneInfo &tmpcpinfo = cpiVec[csti];
		int objid = tmpcpinfo.objid;

		PHOpObj *myobj = (*opObjs)[objid];
		GRMesh *tgtMesh = myobj->targetMesh;
		if (tmpcpinfo.cstType == PHOpHapticController::cstFace)
		{
#ifdef PROXYFIX_INFO
			DSTR << "UpdateIn cstFace " << tmpcpinfo.planeid << endl;
#endif
			int ofi = tmpcpinfo.planeid;

			Vec3f &fpa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[2]];
			Vec3f &fpb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
			Vec3f &fpc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[0]];
			if (tgtMesh->faces[ofi].nVertices == 4)
				DSTR << "rectangle detected" <<std:: endl;

			Vec3f dyFaceNormal = tmpcpinfo.planeN;
			//dyFaceNormal.unitize();
			//counterPlane = dyFaceNormal;
			Vec3f pinf = tmpcpinfo.fu * fpa + tmpcpinfo.fv * fpb + tmpcpinfo.fw * fpc;
			pinf += dyFaceNormal * myHc->c_obstRadius;
			
			fixsubGoal = pinf + myHc->couterPlaneN;// *gOverDist;
			//dyFaceNormal = dyFaceNormal * myHc->c_obstRadius * overDist;

			//fixsubGoal += dyFaceNormal;
			//cnstredN = myHc->couterPlaneN;
			cnstredP = fixsubGoal;

			//we prefer to use face barycentric coordinates, rather than edge and vertex;
			break;
		}
		
		else if (tmpcpinfo.cstType == PHOpHapticController::cstEdge)
		{
#ifdef PROXYFIX_INFO
			DSTR << "UpdateIn cstEdge " << tmpcpinfo.edgeidA << " " << tmpcpinfo.edgeidB << endl;
#endif				
			Vec3f &c = tgtMesh->vertices[tmpcpinfo.edgeidA];
			Vec3f &d = tgtMesh->vertices[tmpcpinfo.edgeidB];
			Vec3f dyFaceNormal = tmpcpinfo.planeN;// .unit();
			
			Vec3f pinf = c + tmpcpinfo.segmentt *(d - c);

			//pinf += dyFaceNormal *myHc->c_obstRadius* overDist;
			pinf += dyFaceNormal * overDist;
		
			fixsubGoal = pinf;
		
			cnstredP = fixsubGoal;

		}
		else if (tmpcpinfo.cstType == PHOpHapticController::cstPoint)
		{
#ifdef PROXYFIX_INFO
			DSTR << "UpdateIn cstPoint " << tmpcpinfo.vid << endl;
#endif
			Vec3f &v = tgtMesh->vertices[tmpcpinfo.vid];
			Vec3f dyFaceNormal = tmpcpinfo.planeN;
			
			counterPlane = dyFaceNormal;

		
			fixsubGoal = v + dyFaceNormal* overDist;;
			
			cnstredP = fixsubGoal;
		}
	}

	myHc->hcFixsubGoal = fixsubGoal;
}
void PHOpHapticRenderer::ProxyMove()
{
	//	MatrixExtension ma;
#ifdef USE_a_little_over_InFix
	float overDist = 1.1f;
#else 
	float overDist = 1.0000f;
#endif

	//debug 
	std::vector<int> fieis;
	std::vector<int> fieisubs;

	std::vector<PHOpHapticController::ConstrainPlaneInfo>& cpiVec = myHc->cpiVec;
	Vec3f &fixsubGoal = myHc->hcFixsubGoal;

	PHOpHapticController::ConstrainPlaneInfo &tmpcpinfo = cpiVec[0];
	int objid = tmpcpinfo.objid;
	//dt.ClearData();
	int itrNum = 0;
	int maxitrNum;// = 100;
	if (useProxyfixSubitr)
		maxitrNum = 20;
	else maxitrNum = 2;//use odd number

	do{
		PHOpObj *myobj = (*opObjs)[objid];
		GRMesh *tgtMesh = myobj->targetMesh;
		//vector<PHOpHapticController::ConstrainPlaneInfo> inrangeEs;
		/*vector<Vec3f> &planeNs = dt.base;
		vector<Vec3f> planePs;*/
		std::vector<Vec3f> closPs;
		//vector<Vec3f> closNs;
		std::vector<int> insideRegions;
		myHc->surrCnstrList.clear();
		std::vector<int> surrSwapt;
		surrSwapt.swap(myHc->surrCnstrList);
		for (int ofi = 0; ofi < (int)tgtMesh->faces.size(); ofi++)
		{

			Vec3f &fpa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[2]];
			Vec3f &fpb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
			Vec3f &fpc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[0]];


			bool inside;
			/*	Vec3f closP = ClosestPtoTriangle(fixsubGoal,fpa,fpb,fpc,inside);*/
			int regionFlag = -1;

			Vec3f closP = CollisionFuntions::ClosestPtoTriangle(fixsubGoal, fpa, fpb, fpc, inside, regionFlag);
			if (fabs((closP - fixsubGoal).norm()) < myHc->c_obstRadius)
			{
				//edge check and vertex check?
				float t = 0;
				Vec3f closP2;
				CollisionFuntions::ClosestPtPointSegment(fixsubGoal, fpa, fpb, t, closP2);
				if (fabs((closP2 - fixsubGoal).norm()) < myHc->c_obstRadius)
				{
					fieis.push_back(ofi);
					fieisubs.push_back(1);
				}
				Vec3f closP3;
				CollisionFuntions::ClosestPtPointSegment(fixsubGoal, fpb, fpc, t, closP3);
				if (fabs((closP3 - fixsubGoal).norm()) < myHc->c_obstRadius)
				{
					fieis.push_back(ofi);
					fieisubs.push_back(2);
				}
				Vec3f closP4;
				CollisionFuntions::ClosestPtPointSegment(fixsubGoal, fpa, fpc, t, closP4);
				if (fabs((closP4 - fixsubGoal).norm()) < myHc->c_obstRadius)
				{
					fieis.push_back(ofi);
					fieisubs.push_back(3);
				}
				Vec3f dyFaceNormal = cross(fpb - fpa, fpc - fpa);
				dyFaceNormal.unitize();
			
				//test if any face is in the negtive pos of proxy
				dyFaceNormal = dyFaceNormal * myHc->c_obstRadius;
		
				float projC = ((fpa + dyFaceNormal) - fixsubGoal).dot(dyFaceNormal.unit());
				if (projC > 0)
				{
					
					closPs.push_back(closP);
					
					insideRegions.push_back(regionFlag);
					myHc->surrCnstrList.push_back(ofi);
				}
			}

		}

		myHc->surrCnstrs = (int)myHc->surrCnstrList.size();

		if (myHc->surrCnstrs == 0)
		{
#ifdef PRINT_PC_ITR
			if (itrNum > 0)
				cout << "itrNum = " << itrNum << endl;

			if (itrNum > 15)
				 mtHapticFix.printTimer();
#endif
			break;
		}


		itrNum++;
		if (useIncreaseMeshMove)
			if (itrNum > 1)overDist *= overDist;
		if (itrNum > maxitrNum)
		{
#ifdef PRINT_PC_ITR
			cout << "OverMaxitrNum " << itrNum << endl;
#endif
			break;
		}

		bool toosmallquit = false;
		for (int ci = 0; ci < (int)myHc->surrCnstrList.size(); ci++)
		{
			int ofi = myHc->surrCnstrList[ci];
			std::vector<int> ovrVs;
			Vec3f &fa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[2]];
			Vec3f &fb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
			Vec3f &fc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[0]];
			Vec3f signN = cross(fb - fa, fc - fa);
			//stoppp
			//opObjs[0].debugStepOn = true;
			//return false;

			//by region we are able to know it is a face or a edge or a vertex inside the c_obstacle
			Vec3f fpa, fpb, fpc;
			int fai, fbi, fci;
			std::vector<int> fidexlist;
			if (insideRegions[ci] == 0)
			{//a face inside
				fidexlist.push_back(2);
				fidexlist.push_back(1);
				fidexlist.push_back(0);
				fai = tgtMesh->faces[ofi].indices[fidexlist[0]];
				fbi = tgtMesh->faces[ofi].indices[fidexlist[1]];
				fci = tgtMesh->faces[ofi].indices[fidexlist[2]];
			}
			else if (insideRegions[ci] > 10)
			{//a edge inside

				if (insideRegions[ci] == 11)
				{
					fidexlist.push_back(2);
					fidexlist.push_back(1);
				}
				else if (insideRegions[ci] == 12)
				{
					fidexlist.push_back(2);
					fidexlist.push_back(0);
				}
				else if (insideRegions[ci] == 13)
				{
					fidexlist.push_back(0);
					fidexlist.push_back(1);
				}
				fai = tgtMesh->faces[ofi].indices[fidexlist[0]];
				fbi = tgtMesh->faces[ofi].indices[fidexlist[1]];
			}
			else if (insideRegions[ci] < 10 && insideRegions[ci] >0)
			{//a vertex inside

				if (insideRegions[ci] == 1)
				{
					fidexlist.push_back(2);
				}
				else if (insideRegions[ci] == 2)
				{
					fidexlist.push_back(1);
				}
				else if (insideRegions[ci] == 3)
				{
					fidexlist.push_back(0);
				}
				fai = tgtMesh->faces[ofi].indices[fidexlist[0]];
			}
					{
					
						Vec3f dyFaceNormal = (fixsubGoal - closPs[ci]).unit();// *myHc->c_obstRadius;
						//dyFaceNormal.unitize();

						dyFaceNormal = dyFaceNormal * myHc->c_obstRadius * overDist;

						//calculate mesh displacement
						//debug may inverse because the too deep inside
						Vec3f movN = (fixsubGoal - (closPs[ci] + dyFaceNormal)).unit();
						float movD = -(fixsubGoal - (closPs[ci] + dyFaceNormal)).norm();
					
						if (movN.dot(signN) > 0)
							movN = -movN;


						fixsubGoal = fixsubGoal + movD * movN;


					}


		}
		if (toosmallquit)
		{
			DSTR << "too small quit" << " itrNum =" << itrNum << std::endl;
			break;
		}
	} while (1);

	proxyFixPos = fixsubGoal;
	PHOpParticle *currp = myHc->GetMyHpProxyParticle();
#ifdef CHECK_INF_ERR
	if (!FloatErrorTest::CheckBadFloatValue(fixsubGoal))
	{
		int u = 0;
		return;
	}
#endif
	currp->pCurrCtr = fixsubGoal;// +counterPlane * 0.002f;
	currp->pNewCtr = currp->pCurrCtr;


}
bool PHOpHapticRenderer::ProxyCorrection()
{
	MatrixExtension ma;
#ifdef USE_a_little_over_InFix
	float overDist = 1.1f;
#else 
	float overDist = 1.0000f;
#endif

	//debug 
	std::vector<int> fieis;
	std::vector<int> fieisubs;

	std::vector<PHOpHapticController::ConstrainPlaneInfo>& cpiVec = myHc->cpiVec;
	Vec3f &fixsubGoal = myHc->hcFixsubGoal;

	PHOpHapticController::ConstrainPlaneInfo &tmpcpinfo = cpiVec[0];
	int objid = tmpcpinfo.objid;
	//dt.ClearData();
	int itrNum = 0;
	int maxitrNum;// = 100;
	if (useProxyfixSubitr)
		maxitrNum = 200;
	else maxitrNum = 2;//use odd number

	do{
		PHOpObj *myobj = (*opObjs)[objid];
		GRMesh *tgtMesh = myobj->targetMesh;
		//vector<PHOpHapticController::ConstrainPlaneInfo> inrangeEs;
		/*vector<Vec3f> &planeNs = dt.base;
		vector<Vec3f> planePs;*/
		std::vector<Vec3f> closPs;
		//vector<Vec3f> closNs;
		std::vector<int> insideRegions;
		myHc->surrCnstrList.clear();
		std::vector<int> surrSwapt;
		surrSwapt.swap(myHc->surrCnstrList);
		for (int ofi = 0; ofi < (int)tgtMesh->faces.size(); ofi++)
		{

			Vec3f &fpa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[2]];
			Vec3f &fpb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
			Vec3f &fpc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[0]];


			bool inside;
			/*	Vec3f closP = ClosestPtoTriangle(fixsubGoal,fpa,fpb,fpc,inside);*/
			int regionFlag = -1;

			Vec3f closP = CollisionFuntions::ClosestPtoTriangle(fixsubGoal, fpa, fpb, fpc, inside, regionFlag);
			if (fabs((closP - fixsubGoal).norm()) < myHc->c_obstRadius)
			{
				//edge check and vertex check?
				float t = 0;
				Vec3f closP2;
				CollisionFuntions::ClosestPtPointSegment(fixsubGoal, fpa, fpb, t, closP2);
				if (fabs((closP2 - fixsubGoal).norm()) < myHc->c_obstRadius)
				{
					fieis.push_back(ofi);
					fieisubs.push_back(1);
				}
				Vec3f closP3;
				CollisionFuntions::ClosestPtPointSegment(fixsubGoal, fpb, fpc, t, closP3);
				if (fabs((closP3 - fixsubGoal).norm()) < myHc->c_obstRadius)
				{
					fieis.push_back(ofi);
					fieisubs.push_back(2);
				}
				Vec3f closP4;
				CollisionFuntions::ClosestPtPointSegment(fixsubGoal, fpa, fpc, t, closP4);
				if (fabs((closP4 - fixsubGoal).norm()) < myHc->c_obstRadius)
				{
					fieis.push_back(ofi);
					fieisubs.push_back(3);
				}
				Vec3f dyFaceNormal = cross(fpb - fpa, fpc - fpa);
				dyFaceNormal.unitize();
				/*PHOpHapticController::ConstrainPlaneInfo tcpif;
				tcpif.cstType = PHOpHapticController::cstFace;
				*/
				//test if any face is in the negtive pos of proxy
				dyFaceNormal = dyFaceNormal * myHc->c_obstRadius;
				//Vec3f abFpa = fpa + dyFaceNormal;
				//Vec3f abFpb = fpb + dyFaceNormal;
				//Vec3f abFpc = fpc + dyFaceNormal;
				//	Vec3f center = (abFpa + abFpb + abFpc) / 3;
				//pro_near_fs.push_back(ofi);
				//DA.AddTempDrawBall(closP, 0.2f);
				float projC = ((fpa + dyFaceNormal) - fixsubGoal).dot(dyFaceNormal.unit());
				if (projC > 0)
				{
					/*if (tmpcpinfo.cstType == PHOpHapticController::cstFace)
					{
					bool inside = false;
					for (int csti = 0; csti < myHc->constrainCount; csti++)
					if (cpiVec[csti].planeid == ofi)
					inside = true;
					if (inside)
					continue;
					}*/

					//DSTR << "caution, fall inside" << endl;

					//be carefull that the inverse of normal is pushed in
					/*planeNs.push_back(-dyFaceNormal.unit());
					planePs.push_back(fpa + dyFaceNormal);*/
					closPs.push_back(closP);
					//closNs.push_back(dyFaceNormal);
					//unPrunedinPFId.push_back(ofi);
					insideRegions.push_back(regionFlag);
					myHc->surrCnstrList.push_back(ofi);
				}
			}

		}

		myHc->surrCnstrs = (int)myHc->surrCnstrList.size();

		if (myHc->surrCnstrs == 0)
		{
#ifdef PRINT_PC_ITR
			if (itrNum > 0)
				cout << "itrNum = " << itrNum << endl;

			if (itrNum > 15)
				 mtHapticFix.printTimer();
#endif
			break;
		}


		itrNum++;
		if (useIncreaseMeshMove)
			if (itrNum > 1)overDist *= overDist;
		if (itrNum > maxitrNum)
		{
#ifdef PRINT_PC_ITR
			cout << "OverMaxitrNum " << itrNum << endl;
#endif
			break;
		}

		bool toosmallquit = false;
		for (int ci = 0; ci < (int)myHc->surrCnstrList.size(); ci++)
		{
			int ofi = myHc->surrCnstrList[ci];
			std::vector<int> ovrVs;
			Vec3f &fa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[2]];
			Vec3f &fb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
			Vec3f &fc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[0]];
			Vec3f signN = cross(fb - fa, fc - fa);
			//stoppp
			//opObjs[0].debugStepOn = true;
			//return false;

			//by region we are able to know it is a face or a edge or a vertex inside the c_obstacle
			Vec3f fpa, fpb, fpc;
			int fai, fbi, fci;
			std::vector<int> fidexlist;
			if (insideRegions[ci] == 0)
			{//a face inside
				fidexlist.push_back(2);
				fidexlist.push_back(1);
				fidexlist.push_back(0);
				fai = tgtMesh->faces[ofi].indices[fidexlist[0]];
				fbi = tgtMesh->faces[ofi].indices[fidexlist[1]];
				fci = tgtMesh->faces[ofi].indices[fidexlist[2]];
			}
			else if (insideRegions[ci] > 10)
			{//a edge inside

				if (insideRegions[ci] == 11)
				{
					fidexlist.push_back(2);
					fidexlist.push_back(1);
				}
				else if (insideRegions[ci] == 12)
				{
					fidexlist.push_back(2);
					fidexlist.push_back(0);
				}
				else if (insideRegions[ci] == 13)
				{
					fidexlist.push_back(0);
					fidexlist.push_back(1);
				}
				fai = tgtMesh->faces[ofi].indices[fidexlist[0]];
				fbi = tgtMesh->faces[ofi].indices[fidexlist[1]];
			}
			else if (insideRegions[ci] < 10 && insideRegions[ci] >0)
			{//a vertex inside

				if (insideRegions[ci] == 1)
				{
					fidexlist.push_back(2);
				}
				else if (insideRegions[ci] == 2)
				{
					fidexlist.push_back(1);
				}
				else if (insideRegions[ci] == 3)
				{
					fidexlist.push_back(0);
				}
				fai = tgtMesh->faces[ofi].indices[fidexlist[0]];
			}
					{
						//direct move 
						/*int fai = tgtMesh->faces[ofi].indices[2];
						int fbi = tgtMesh->faces[ofi].indices[1];
						int fci = tgtMesh->faces[ofi].indices[0];

						Vec3f &fpa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[2]];
						Vec3f &fpb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
						Vec3f &fpc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[0]];
						Vec3f dyFaceNormal = cross(fpb - fpa, fpc - fpa);*/
						Vec3f dyFaceNormal = (fixsubGoal - closPs[ci]).unit();// *myHc->c_obstRadius;
						//dyFaceNormal.unitize();

						dyFaceNormal = dyFaceNormal * myHc->c_obstRadius * overDist;

						//move vertex
						/*	fpa += fixsubGoal - closPs[ci] + dyFaceNormal;
						fpb += fixsubGoal - closPs[ci] + dyFaceNormal;
						fpc += fixsubGoal - closPs[ci] + dyFaceNormal;*/

						//calculate mesh displacement
						//debug may inverse because the too deep inside
						Vec3f movN = (fixsubGoal - (closPs[ci] + dyFaceNormal)).unit();
						float movD = (fixsubGoal - (closPs[ci] + dyFaceNormal)).norm();
						////closPs  store the closest point on c_obstacle
						//Vec3f movN = (fixsubGoal - closPs[ci]).unit();
						//float movD = (fixsubGoal - closPs[ci]).norm();

						if (movN.dot(signN) > 0)
							movN = -movN;

						////debug
						//subStepProSolve = false;
						//manualModel = true;
						//return false;;

						if (movD < 0.00001f)
						{
							DSTR << "too small!" << std::endl;
							toosmallquit = true;
							break;
						}

						////move tst
						/*Vec3f mvdstfpa = fpa + movN * movD;
						Vec3f mvdstfpb = fpb + movN * movD;
						Vec3f mvdstfpc = fpc + movN * movD;*/
#ifdef DIRECTMOVE
						for (int fis = 0; fis < 3; fis++)
						{
							int fi = tgtMesh->faces[ofi].indices[fis];
							ParticleGroup *pg = &myobj->objGArr[myobj->objPArr[myobj->oVtoPmap[fi]].pMyGroupInd];

							for (int jm = 0; jm < pg->gNptcl; jm++)
							{
								PHOpParticle &dp = myobj->objPArr[pg->getParticleGlbIndex(jm)];
								if (!dp.isFixed)
									dp.pCurrCtr += myobj->objBlWeightArr[fi][jm] * movD * movN;

								//u1 += myobj->objBlWeightArr[tmpcpinfo.edgeidA][jm] * (dp.pNewOrint * (dyFaceNormal));

							}
							PHOpParticle &mdp = myobj->objPArr[myobj->oVtoPmap[fi]];

							Vec3f &vert = myobj->OrigPos[fi];
							Vec3f u; u.clear();
							for (int jm = 0; jm < pg->gNptcl; jm++)
							{
								PHOpParticle &dp = myobj->objPArr[pg->getParticleGlbIndex(jm)];
								Spr::TPose<float> &ctrpose = myobj->orinPsPoseList[pg->getParticleGlbIndex(jm)];


								Spr::TPose<float> pose = TPose<float>(dp.pCurrCtr, dp.pCurrOrint);

								pose.x = -pose.x;
								pose.y = -pose.y;
								pose.z = -pose.z;

								u += myobj->objBlWeightArr[fi][jm] * (pose * (ctrpose.Pos() + vert));// (taM *
							}
							tgtMesh->vertices[fi] = u;
						}

#else
						//follow the equation 
						Matrix3f mofIInver;
						float pRadius = myobj->objPArr[myobj->objVtoPmap[fai]].pRadii;
						bool noRotation = false;
						if (pRadius < 0.00001f)
						{
							mofIInver = mofIInver.Zero();
							noRotation = true;
						}

						else{
							mofIInver = mofIInver * (pRadius* pRadius
								* 0.4f * myobj->objPArr[myobj->objVtoPmap[fai]].pTotalMass);

							mofIInver = mofIInver.inv();
						}
						for (int fis = 0; fis < (int)fidexlist.size(); fis++)
						{
							int fi = tgtMesh->faces[ofi].indices[fis];
							Vec3f &fv = tgtMesh->vertices[fi];
							PHOpGroup *pg = &myobj->objGArr[myobj->objPArr[myobj->objVtoPmap[fi]].pMyGroupInd];
							for (int jm = 0; jm < pg->gNptcl; jm++)
							{

								PHOpParticle &dp = myobj->objPArr[pg->getParticleGlbIndex(jm)];
								if (dp.isFixed)
									continue;

								Vec3f r = dp.pCurrCtr - fv;
								if (!FloatErrorTest::CheckBadFloatValue(r))
									int u = 0;

								//delta X
								Vec3f eq;
								if (dp.pNvertex == 1 || r == Vec3f())
								{
									noRotation = true;

									dp.pCurrCtr += myobj->objBlWeightArr[fi][jm] * movD *movN;
								}
								else {
									eq = cross(cross(ma.MatrixMultiVec3fRight(mofIInver, r), movN), r) + 1 / dp.pTotalMass * movN;
									Vec3f eqx = movD / eq.dot(movN) *dp.pTotalMass *movN;

									dp.pCurrCtr += myobj->objBlWeightArr[fi][jm] * eqx;

								}
								if (!FloatErrorTest::CheckBadFloatValue(dp.pCurrCtr))
									int u = 0;
								if (!noRotation)
								{
									//delta theta
									Spr::TQuaternion<float> q;
									Vec3f dtheta = cross(ma.MatrixMultiVec3fRight(mofIInver, r), movN);
									if (dtheta == 0)
										int u = 0;
									dtheta = movD / eq.dot(movN)  * dtheta;
									int u1 = 0;
									if (dtheta.norm() < 0.000001f)
										continue;
									Vec3f direction = dtheta / dtheta.norm();
									float arcPerSc = dtheta.norm();

									float halfAngle = arcPerSc * 0.5f;
									float scale = sin(halfAngle);

									q.x = scale * direction.x;
									q.y = scale * direction.y;
									q.z = scale * direction.z;
									q.w = cos(halfAngle);
									dp.pCurrOrint = q * dp.pCurrOrint;
#ifdef CHECK_INF_ERR
									if (!FloatErrorTest::CheckBadFloatValue(dp.pCurrOrint.x))
									{
										int u = 0;
										if (!FloatErrorTest::CheckBadFloatValue(r))
											int u = 0;
									}
#endif
								}

							}
							PHOpParticle &mdp = myobj->objPArr[myobj->objVtoPmap[fi]];

							Vec3f &vert = myobj->objOrigPos[fi];
							Vec3f u; u.clear();
							for (int jm = 0; jm < pg->gNptcl; jm++)
							{
								PHOpParticle &dp = myobj->objPArr[pg->getParticleGlbIndex(jm)];
								Spr::TPose<float> &ctrpose = myobj->objOrinPsPoseList[pg->getParticleGlbIndex(jm)];


								Spr::TPose<float> pose = TPose<float>(dp.pCurrCtr, dp.pCurrOrint);

								pose.x = -pose.x;
								pose.y = -pose.y;
								pose.z = -pose.z;

								u += myobj->objBlWeightArr[fi][jm] * (pose * (ctrpose.Pos() + vert));// (taM *
							}
#ifdef CHECK_INF_ERR
							if (!FloatErrorTest::CheckBadFloatValue(u))
							{
								int u = 0;
								break;
							}
#endif
							tgtMesh->vertices[fi] = u;

						}
#endif
					

					}


		}
		if (toosmallquit)
		{
			std::cout << "too small quit" << " itrNum =" << itrNum << std::endl;
			break;
		}
	} while (1);

	proxyFixPos = fixsubGoal;
	PHOpParticle *currp = myHc->GetMyHpProxyParticle();

#ifdef CHECK_INF_ERR
	if (!FloatErrorTest::CheckBadFloatValue(fixsubGoal.x))
	{
		int u = 0;
		return false;
	}
#endif
	currp->pCurrCtr = fixsubGoal;// +counterPlane * 0.002f;
	currp->pNewCtr = currp->pCurrCtr;
	return true;

}
#endif


}//namespace 

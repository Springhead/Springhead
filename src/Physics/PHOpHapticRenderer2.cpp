#include "Physics\PHOpHapticRenderer.h"

//#define DRAW_INTSINFO
#define USE_a_little_over_InSolve
#define USE_a_little_over_InUctcFix
//#define DrawRouteInfo
#define ProxyFixInCORadius
#define USE_GLOBLE_COLLI
//#define PrintItrctingProxySolve
//#define PrintNewCtr
//#define PrintInfoHapTime
//#define PrintCalInfoColliSolve
//#define PrintNoctcProxyInfo
//#define PROXYFIX_INFO
//#define Save_GoalPos
#define Old_Functions
#define new_ProxyFunctions
#define CHECK_INF_ERR
//#define PrintInfoColliSolve
//#define PrintInfoCostMtx
//#define PrintProxyFixCstrCal

namespace Spr
{
	;


	using namespace std;

	void PHOpHapticRenderer::HpNoCtcProxyCorrection()
	{
		//noCtcItrNum++;
#ifdef PrintNoctcProxyInfo
		cout << "noCtcItrNum++" << noCtcItrNum << endl;
#endif
		float checkInsideRange = myHc->hcElmDtcRadius * 2;
		bool insideObjs = false;

#ifdef USE_a_little_over_InUctcFix
		//float overDist = 1.001f;
		float overDist = 2.001f;
#else 
		float overDist = 1.0000f;
#endif
		PHOpParticle	*hdp = myHc->GetMyHpProxyParticle();
		Vec3f& currGoal = myHc->userPos;
		Vec3f& currSubStart = hdp->pCurrCtr;
		//Vec3f& currSubStart = hdp->pNewCtr;
		myHc->surrCnstrs = 0;
		int currConstrNum = myHc->constrainCount;
		//for no active proxy position fix

		//if (!myHc->CheckProxyState())
		for (int objid = 0; objid < objNum; objid++)
		{
			currConstrNum = 1;
			//int objid = 0;//debug
			PHOpObj *myobj = (*opObjs)[objid];
			GRMesh *tgtMesh = myobj->targetMesh;
			float nearest = 100.0f;
			int nearesti = -1;
			//search nearest vertics
			for (int ovi = 0; ovi<tgtMesh->vertices.size(); ovi++)
			{
				Vec3f &v = tgtMesh->vertices[ovi];
				float dist = fabs((currSubStart - v).norm());
				if (nearest > dist)
				{
					nearest = dist;
					nearesti = ovi;
				}

			}
			if (nearest>checkInsideRange)
				continue;

			//find nst primitive and calculate average

			map<int, vector<int>>::iterator vfit = objVtoFInfos[objid].find(nearesti);
			//cout << "nearesti in " << objid << " = " << nearesti << endl;

			//use averange vertex direction
			//vector<int> fRegions;
			//vector<Vec3f> dynfs; 

			bool notbi = false;
			float nstDist = nearest, nstFDist, nstEDist;
			int nstfid, nsteidA, nsteidB, nstType = 1;
			Vec3f tmpbaseP, baseP;
			Vec3f setP = tgtMesh->vertices[nearesti];
			Vec3f efnormal, facenormal, eAvgNormal, tmpnormal;
			for (int fiv = 0; fiv < vfit->second.size(); fiv++)
			{
				/*for(int fii = 0;fii<tgtMesh->faces[facevct[fiv]].nVertices;fii++)
				{*/
				//pro_near_fs.push_back(vfit->second[fiv]);
				int fid = vfit->second[fiv];
				if (tgtMesh->faces[fid].nVertices > 3)
					DSTR << "only support triangles" << endl;
				Vec3f &a = tgtMesh->vertices[tgtMesh->faces[fid].indices[2]];
				Vec3f &b = tgtMesh->vertices[tgtMesh->faces[fid].indices[1]];
				Vec3f &c = tgtMesh->vertices[tgtMesh->faces[fid].indices[0]];
				Vec3f n = cross(b - a, c - a);
				n.unitize();
				bool inside; int region;
				tmpbaseP = CollisionFuntions::ClosestPtoTriangle(currSubStart, a, b, c, inside, region);
				//fRegions.push_back(region);
				if (region == 0)
				{
					float fdist = fabs((currSubStart - tmpbaseP).norm());
					if (fdist < nstDist)
					{
						nstfid = fid;
						nstDist = fdist;
						nstType = 3;
						facenormal = n;
#ifdef ProxyFixInCORadius
						baseP = tmpbaseP + myHc->c_obstRadius*n.unit();
#else
						baseP = tmpbaseP;
#endif
						setP = tmpbaseP;
					}
				}
				float et;
				CollisionFuntions::ClosestPtPointSegment(currSubStart, a, b, et, tmpbaseP);
				tmpnormal = currSubStart - tmpbaseP;

				float edist = fabs((tmpnormal).norm());
				if (edist < nstDist)
				{
					nsteidA = tgtMesh->faces[fid].indices[2];
					nsteidB = tgtMesh->faces[fid].indices[1];
					nstDist = edist;
					efnormal = n;
					nstType = 2;
#ifdef ProxyFixInCORadius
					baseP = tmpbaseP + myHc->c_obstRadius*n.unit();
#else
					baseP = tmpbaseP;
#endif
					setP = tmpbaseP;
				}
				CollisionFuntions::ClosestPtPointSegment(currSubStart, c, b, et, tmpbaseP);
				tmpnormal = currSubStart - tmpbaseP;
				edist = fabs((tmpnormal).norm());
				if (edist < nstDist)
				{
					nsteidA = tgtMesh->faces[fid].indices[0];
					nsteidB = tgtMesh->faces[fid].indices[1];
					nstDist = edist;
					efnormal = n;
					nstType = 2;
#ifdef ProxyFixInCORadius
					baseP = tmpbaseP + myHc->c_obstRadius*n.unit();
#else
					baseP = tmpbaseP;
#endif
					setP = tmpbaseP;
				}
				CollisionFuntions::ClosestPtPointSegment(currSubStart, a, c, et, tmpbaseP);
				tmpnormal = currSubStart - tmpbaseP;
				edist = fabs((tmpnormal).norm());
				if (edist < nstDist)
				{
					nsteidA = tgtMesh->faces[fid].indices[2];
					nsteidB = tgtMesh->faces[fid].indices[0];
					nstDist = edist;
					efnormal = n;
					nstType = 2;
#ifdef ProxyFixInCORadius
					baseP = tmpbaseP + myHc->c_obstRadius*n.unit();
#else
					baseP = tmpbaseP;
#endif
					setP = tmpbaseP;
				}
				/*
				a = a + (dyFaceNormal * myHc->c_obstRadius);
				b = b + (dyFaceNormal * myHc->c_obstRadius);
				c = c + (dyFaceNormal * myHc->c_obstRadius);
				Vec3f ctr = (a + b + c)/3;
				render->DrawLine(ctr,ctr + dyFaceNormal);
				*/

				//}

			}
#ifdef PrintNoctcProxyInfo
			DA.AddTempDrawLine(myHc->userPos, setP);
			DA.AddTempDrawFont3D(setP, "setP");
#endif

			//case of nstP, proxy along the middle line of triangle
			Vec3f counterPlane = facenormal;
			bool insideOBJ = false;
			if (nstType == 1)
			{
				/*counterPlane = (currSubStart - tgtMesh->vertices[nearesti]).unit();
				if (counterPlane.dot(avgNormal) < 0)
				counterPlane = -counterPlane;
				#ifdef ProxyFixInCORadius
				baseP = tgtMesh->vertices[nearesti] + counterPlane * myHc->c_obstRadius;
				#else
				baseP = tgtMesh->vertices[nearesti];
				#endif
				setP = tgtMesh->vertices[nearesti];
				if ((currSubStart - baseP).dot(avgNormal)<0)
				{
				insideOBJ = true;
				}*/

				//is outside c_obstacle
				bool insideC = false;
				Vec3f v = tgtMesh->vertices[nearesti];
				if (fabs((v - currSubStart).norm()) > myHc->c_obstRadius)
				{
					int size = (int)vfit->second.size();
					int dirNum = 0;
					float neareastDir = 100.0f;
					for (int fiv = 0; fiv < vfit->second.size(); fiv++)
					{
						/*for(int fii = 0;fii<tgtMesh->faces[facevct[fiv]].nVertices;fii++)
						{*/
						//pro_near_fs.push_back(vfit->second[fiv]);
						int fid = vfit->second[fiv];
						if (tgtMesh->faces[fid].nVertices > 3)
							DSTR << "only support triangles" << endl;
						Vec3f &a = tgtMesh->vertices[tgtMesh->faces[fid].indices[2]];
						Vec3f &b = tgtMesh->vertices[tgtMesh->faces[fid].indices[1]];
						Vec3f &c = tgtMesh->vertices[tgtMesh->faces[fid].indices[0]];
						Vec3f n = cross(b - a, c - a);
						n.unitize();
						float dir = (currSubStart - v).dot(n);
						if (dir < 0)
						{
							dirNum++;
						}
						//find projection nearest face
						if (fabs(dir) < fabs(neareastDir))
							neareastDir = dir;
					}
					if (dirNum == 0)
					{
						continue;
					}
					else if (dirNum <= size)
					{
						//see the nearest projection
						if (neareastDir < 0)
							continue;
					}
				}
				else insideC = true;

				//dtNctc.ClearData();
				vector<Vec3f> planeNs;// = dtNctc.base;
				vector<Vec3f> planePs;

				int size = (int)vfit->second.size();
				for (int fiv = 0; fiv < vfit->second.size(); fiv++)
				{
					/*for(int fii = 0;fii<tgtMesh->faces[facevct[fiv]].nVertices;fii++)
					{*/
					//pro_near_fs.push_back(vfit->second[fiv]);
					int fid = vfit->second[fiv];
					if (tgtMesh->faces[fid].nVertices > 3)
						DSTR << "only support triangles" << endl;
					Vec3f &a = tgtMesh->vertices[tgtMesh->faces[fid].indices[2]];
					Vec3f &b = tgtMesh->vertices[tgtMesh->faces[fid].indices[1]];
					Vec3f &c = tgtMesh->vertices[tgtMesh->faces[fid].indices[0]];
					Vec3f n = cross(b - a, c - a);
					n.unitize();
					Vec3f adash = a + n * myHc->c_obstRadius;
					float dir = (adash - currSubStart).dot(n);
					if (dir > 0)
					{
						insideOBJ = true;
						planeNs.push_back(n);
						planePs.push_back(adash);
#ifdef PrintNoctcProxyInfo
						DA.AddNewDrawLine(adash, adash + n);
						DA.AddTempDrawFont3D(adash + n, "mvP");
#endif
					}
					if (true == insideC)
					{
						float dir2 = (v - currSubStart).dot(n);
						if (dir2 < 0)
						{
							//test if it is far from vertex
							/*float dist = (tgtMesh->vertices[nearesti] - currSubStart).norm();
							if (dist > myHc->c_obstRadius)
							{
							insideOBJ = false;
							}
							else*/
							{
								//move opposite vertex
								Vec3f dir = hdp->pNewCtr - tgtMesh->vertices[nearesti];
								hdp->pNewCtr = tgtMesh->vertices[nearesti] + dir.unit() * myHc->c_obstRadius;
								hdp->pCurrCtr = hdp->pNewCtr;
								insideOBJ = false;
								insideObjs = true;
							}
						}
					}
				}


				if (true == insideOBJ)
				{
					Vec3f subGoal = currSubStart;
					for (int zi = 0; zi < planeNs.size(); zi++)
					{
						//Vec3f vt = tgtMesh->vertices[nearesti];

						//_itoa(zi, str, 2);
						Vec3f subGoalMov = (planePs[zi] - subGoal).dot(planeNs[zi]) * planeNs[zi];
						//DA.AddTempDrawLine(subGoal, subGoal + subGoalMov);
						//	DA.AddTempDrawFont3D(subGoal + subGoalMov, "subG");
						//Vec3f subsubGoal = subGoal + subGoalMov;
						Vec3f projB;
						Vec3f dirMv;
						float magniMv;
						subGoal += subGoalMov;
						for (int yi = 0; yi < zi; yi++)
						{
							if (yi == zi)
								continue;
							projB = planeNs[yi] * (planePs[yi] - subGoal).dot(planeNs[yi]);
							dirMv = projB - projB.dot(planeNs[zi]) * planeNs[zi];
							dirMv.unitize();
							magniMv = ((planePs[yi]) - subGoal).dot(planeNs[yi]) / (dirMv).dot(planeNs[yi]);
							//	DA.AddTempDrawLine(subGoal, subGoal + magniMv * dirMv);
							//	DA.AddTempDrawFont3D(subGoal + magniMv * dirMv, "ss");
							subGoal += magniMv * dirMv;

						}
					}



					insideObjs = true;
					hdp->pNewCtr = subGoal;
					hdp->pCurrCtr = hdp->pNewCtr;
				}
				continue;
			}
			else if (nstType == 2)
			{
				/*if (efnormal.dot(avgNormal) < 0)
				efnormal = -efnormal;
				if ((currSubStart - baseP).dot(efnormal)<0)
				{
				counterPlane = efnormal.unit();
				insideOBJ = true;
				}*/
				Vec3f v1, v2;
				//find another face link to this edge
				map<int, vector<int>>::iterator mvfit = objVtoFInfos[objid].find(nsteidB);
				bool findotherface = false;
				v1 = tgtMesh->vertices[nsteidB];
#ifdef PrintNoctcProxyInfo
				DA.AddTempDrawLine(v1, v1 + efnormal / 2);
				DA.AddTempDrawFont3D(v1 + efnormal / 2, "v1");
#endif
				v2 = tgtMesh->vertices[nsteidA];
				for (int fiv = 0; fiv < mvfit->second.size(); fiv++)
				{
					/*for(int fii = 0;fii<tgtMesh->faces[facevct[fiv]].nVertices;fii++)
					{*/
					//pro_near_fs.push_back(vfit->second[fiv]);
					int fid = mvfit->second[fiv];
					if (tgtMesh->faces[fid].nVertices > 3)
						DSTR << "only support triangles" << endl;
					Vec3f mn; mn.clear();
					for (int fi = 0; fi < 3; fi++)
					{
						int vid = tgtMesh->faces[fid].indices[fi];
						if (vid == nsteidA)
						{
							findotherface = true;
							Vec3f &a = tgtMesh->vertices[tgtMesh->faces[fid].indices[2]];
							Vec3f &b = tgtMesh->vertices[tgtMesh->faces[fid].indices[1]];
							Vec3f &c = tgtMesh->vertices[tgtMesh->faces[fid].indices[0]];
							mn = cross(b - a, c - a);
							mn.unitize();
							//edge avg normal
							eAvgNormal = mn;
#ifdef PrintNoctcProxyInfo
							DA.AddTempDrawLine(v2, v2 + mn / 2);
							DA.AddTempDrawFont3D(v2 + mn / 2, "v2");
#endif
						}

					}
					if (findotherface)
						break;



				}
				eAvgNormal += efnormal;
				eAvgNormal /= 2;
				eAvgNormal.unitize();
				//Vec3f &a = tgtMesh->vertices[nsteidA];
				////inside face?or inside C_obstacle only
				//float eDir = (a - currSubStart).dot(eAvgNormal);
				baseP = setP + myHc->c_obstRadius * eAvgNormal;
				//if (eDir < 0)
				if ((currSubStart - baseP).dot(eAvgNormal)<0)

				{//inside c_obstacle
					counterPlane = eAvgNormal;
					insideOBJ = true;
#ifdef PrintNoctcProxyInfo
					DA.AddTempDrawLine(hdp->pNewCtr, hdp->pNewCtr + counterPlane);
#endif
				}

			}
			else if (nstType == 3)
			{
				//should use avege or may fall in others
				if ((currSubStart - baseP).dot(facenormal)<0)

				{

					insideOBJ = true;
				}
			}

			if (insideOBJ)
			{
				cout << "no touch but inside " << "Type " << nstType << endl;
				//notbi = true;
				insideObjs = true;
				//hdp->pCurrCtr = v + counterPlane * tstDir.norm() *1.01f;//wrong because the distance should be the radius of proxy
				hdp->pNewCtr = setP + counterPlane * myHc->proxyRadius *overDist;
#ifdef CHECK_INF_ERR
				if (!FloatErrorTest::CheckBadFloatValue(hdp->pNewCtr.x))
				{
					int u = 0;
					return;
			}
#endif
				hdp->pCurrCtr = hdp->pNewCtr;
#ifdef PrintNoctcProxyInfo
				DA.AddTempDrawLine(hdp->pNewCtr, hdp->pNewCtr + counterPlane);
#endif
			}





		}
		if (insideObjs&&noCtcItrNum<3)
		{
			HpNoCtcProxyCorrection();
		}
		else
			return;

	}
	void PHOpHapticRenderer::HpConstrainSolve(Vec3f &currSubStart)
	{

		//DSTR<<"Begin HpSolveProxy"<<endl;
		//pssbPlanes.clear();

		float mPrevPrecE = 0.0001f;
		int intsCounts = 0;
		//cpiVec.clear();
		//bool nextStepOn = false;
		bool firStepOn = false;
		bool secStepOn = false;
		bool thrStepOn = false;
		PHOpParticle	*hdp = myHc->GetMyHpProxyParticle();
		Vec3f& currGoal = myHc->userPos;
		//Vec3f& currSubStart = hdp->pNewCtr;
		//Vec3f& currSubStart = hdp->pCurrCtr;

		//vector<Vec3f> cstrPlaneDualtransNormals;
		//vector<Vec3f> cstrPlaneDualtransPoss;

		//set C-obstacleRadius
		//myHc->setC_ObstacleRadius(dfmObj[0].averRadius/6);
		//myHc->hcElmDtcRadius = dfmObj[0].averRadius/3;

		Vec3f newSubGoal;
		Vec3f newSubStart;
		Vec3f nextSubGoal;
		Vec3f nextSubStart;
		int proxyLoopNum = 1;

		vector<PHOpHapticController::ConstrainPlaneInfo>& cpiVec = myHc->cpiVec;

		int cpiNum = 0;
		PHOpHapticController::ConstrainPlaneInfo &tmpcpinfo = cpiVec[0];
#ifdef USE_GLOBLE_COLLI
		firStepOn = intersectOnRoute(currSubStart, currGoal, newSubStart, newSubGoal, tmpcpinfo, false);
#else
		firStepOn = intersectOnRouteInColliPs(currSubStart, currGoal, newSubStart, newSubGoal, tmpcpinfo, false);
#endif

		//cpiArr[cpiNum] = &tmpcpinfo;
		//cpiVec.push_back(tmpcpinfo);
		//cpiNum++;
		//DSTR<<"FirstIntSolved "<<nextStepOn<<endl;
		myHc->couterPlaneN = tmpcpinfo.planeN;

		if (firStepOn)
		{
			myHc->constrainCount = 1;

			myHc->hcProxyOn = true;


#ifdef PrintInfoColliSolve
			switch (tmpcpinfo.cstType)
			{
			case PHOpHapticController::cstEdge:
				DSTR << "Fir cstEdge " << tmpcpinfo.edgeidA << " " << tmpcpinfo.edgeidB << endl;

				break;
			case PHOpHapticController::cstPoint:
				DSTR << "Fir cstPoint " << tmpcpinfo.vid << endl;
				break;
			case PHOpHapticController::cstFace:
				DSTR << "Fir cstFace " << tmpcpinfo.planeid << endl;
				break;
			default:
				break;
			}
#endif
			bool sameinside = false;
			bool parallelDetected = false;
			//myHc->GetMyHpProxyParticle()->pNewCtr = nextSubGoal;
			//for(int proLp = 0;proLp < proxyLoopNum;proLp++)
			do{


#ifdef PrintCalInfoColliSolve			
				DSTR << "NextSubStart" << newSubStart.x << " " << newSubStart.y << " " << newSubStart.z << endl;
				DSTR << "NextSubGoal" << newSubGoal.x << " " << newSubGoal.y << " " << newSubGoal.z << endl;
#endif
				PHOpHapticController::ConstrainPlaneInfo &tmpcpinfo1 = cpiVec[1];
				/*Vec3f vectorPreventPrecisionErr = ((newSubGoal - tmpcpinfo.planeP).unit())*0.0001f;
				float big = vectorPreventPrecisionErr.norm();
				newSubStart = tmpcpinfo.planeP - vectorPreventPrecisionErr;*/

#ifdef USE_GLOBLE_COLLI
				secStepOn = intersectOnRoute(newSubStart - ((newSubGoal - tmpcpinfo.planeP).unit())*mPrevPrecE, newSubGoal, nextSubStart, nextSubGoal, tmpcpinfo1, false);
				//secStepOn = intersectOnRoute(newSubStart, newSubGoal, nextSubStart, nextSubGoal, tmpcpinfo1, false);
#else
				secStepOn = intersectOnRouteInColliPs(newSubStart - ((newSubGoal - tmpcpinfo.planeP).unit())*mPrevPrecE, newSubGoal, nextSubStart, nextSubGoal, tmpcpinfo1, false);
#endif

				//DSTR<<"NextIntSolved "<<nextStepOn<<endl;
				sameinside = false;

				if (secStepOn)
				{
					myHc->constrainCount = 2;
#ifdef PrintInfoColliSolve


					switch (tmpcpinfo1.cstType)
					{
					case PHOpHapticController::cstEdge:
						DSTR << "Sec cstEdge " << tmpcpinfo1.edgeidA << " " << tmpcpinfo1.edgeidB << " ";

						break;
					case PHOpHapticController::cstPoint:
						DSTR << "Sec cstPoint " << tmpcpinfo1.vid << " ";

						break;
					case PHOpHapticController::cstFace:
						DSTR << "Sec cstFace " << tmpcpinfo1.planeid << " ";

						break;
					default:
						break;
					}



#endif
					for (int samei = 0; samei<myHc->constrainCount - 1; samei++)
					{

						switch (tmpcpinfo1.cstType)
						{
						case PHOpHapticController::cstEdge:

							if (tmpcpinfo1.edgeidA == cpiVec[samei].edgeidA&&tmpcpinfo1.edgeidB == cpiVec[samei].edgeidB)
								sameinside = true;
							break;
						case PHOpHapticController::cstPoint:

							if (tmpcpinfo1.vid == cpiVec[samei].vid)
								sameinside = true;
							break;
						case PHOpHapticController::cstFace:

							if (tmpcpinfo1.planeid == cpiVec[samei].planeid)
								sameinside = true;
							break;
						default:
							break;
						}
					}
					if (sameinside)
					{
#ifdef PrintInfoColliSolve
						DSTR << "same inside" << endl;
#endif
						myHc->constrainCount--;
						break;
					}


					/*	myHc->GetMyHpProxyParticle()->pNewCtr = nextSubStart;
					DSTR<<"pNewCtr set to nextSubStart"<<endl;*/


					int Nirs = 3 + myHc->constrainCount;
					VMatrixRow< double > clms;
					clms.resize(Nirs, Nirs);
					clms.clear(0.0);
					VVector< double > d;
					d.resize(Nirs);
					d.clear(0.0);
					int* turns = new int[Nirs];
					VVector< double > f;
					f.resize(Nirs);
					f.clear(0.0);

					//colums with number "1"
					for (int mi = 0; mi<3; mi++)
					{
						clms[mi][mi] = 1;
					}

					//make counterPlane for Proxy
					Vec3f counterPlane;
					for (int pbpi = 0; pbpi<myHc->constrainCount; pbpi++)
						//for(int pbpi = 0;pbpi<pssbPlanes.size();pbpi++)
					{
						int objid = cpiVec[pbpi].objid;

						PHOpObj *myobj = (*opObjs)[objid];
						GRMesh *tgtMesh = myobj->targetMesh;
						Vec3f planeN = cpiVec[pbpi].planeN;
						Vec3f planeP = cpiVec[pbpi].planeP;

						counterPlane += planeN;

						float fA = planeN.x, fB = planeN.y, fC = planeN.z;
						float fD = -(fA * planeP.x + fB * planeP.y + fC * planeP.z);

						//render->SetMaterial(GRRenderIf::RED);
						//render->DrawLine(planeP,planeP+(planeN * 1.5));

						clms[0][3 + pbpi] = fA;
						clms[1][3 + pbpi] = fB;
						clms[2][3 + pbpi] = fC;

						clms[3 + pbpi][0] = fA;
						clms[3 + pbpi][1] = fB;
						clms[3 + pbpi][2] = fC;

						d[3 + pbpi] = -fD;

					}
					d[0] = currGoal.x;
					d[1] = currGoal.y;
					d[2] = currGoal.z;
#ifdef PrintCalInfoColliSolve
					DSTR << "constrainPlaneMatrix proxyslove1:" << endl;
					for (int i = 0; i<Nirs; i++)
					{
						for (int j = 0; j<Nirs; j++)
							DSTR << clms[i][j] << " ";

						DSTR << "* f = " << d[i] << endl;
					}
#endif
					//GaussElimi::SolveGaussElimi(clms, d, Nirs, f);
					clms.gauss(f, d, turns);
					delete turns;
#ifdef PrintCalInfoColliSolve
					DSTR << "Result1 proxy solve f:" << endl;
					for (int i = 0; i<Nirs; i++)
					{

						DSTR << " f = " << f[i];
					}

					DSTR << endl;
#endif
					counterPlane /= 2;
					counterPlane.unitize();

					if (f[0] == 0.0f&&f[1] == 0.0f&&f[2] == 0.0f)
					{
						//parallel plane detected use only one 
#ifdef PrintInfoColliSolve
						DSTR << "parallel detected! proxy solve 1" << endl;
#endif
						myHc->constrainCount = 1;
						parallelDetected = true;

						Vec3f f = (newSubGoal - myHc->userPos) * 3;
						float magni = f.norm();
						if (magni > 10.0f)
						{
							DSTR << "tst Big Force Output!" << endl;
							f.clear();
						}
						break;

					}
					else{
						Vec3f couterPSubGoal;
						couterPSubGoal.x = f[0];
						couterPSubGoal.y = f[1];
						couterPSubGoal.z = f[2];

						newSubStart = tmpcpinfo1.planeP;
						newSubGoal = couterPSubGoal;
						myHc->couterPlaneN = counterPlane;
#ifdef STORE_LOCALCOORD
						//find local coordinates for proxyfix
						//find face or edge
						for (int ci = 0; ci < myHc->constrainCount; ci++)
						{
							if (myHc->cpiVec[ci].cstType == PHOpHapticController::cstFace)
							{
								int obji = myHc->cpiVec[ci].objid;
								GRMesh *tgtm = dfmObj[obji].targetMesh;
								float t;
								bool itflag = IntersectSegmentTriangle(newSubGoal, newSubGoal - myHc->cpiVec[ci].planeN,
									tgtm->vertices[tgtm->faces[myHc->cpiVec[ci].planeid].indices[2]] + cpiVec[ci].planeN * myHc->c_obstRadius,
									tgtm->vertices[tgtm->faces[myHc->cpiVec[ci].planeid].indices[1]] + cpiVec[ci].planeN * myHc->c_obstRadius,
									tgtm->vertices[tgtm->faces[myHc->cpiVec[ci].planeid].indices[0]] + cpiVec[ci].planeN * myHc->c_obstRadius,
									myHc->cpiVec[ci].fu, myHc->cpiVec[ci].fv, myHc->cpiVec[ci].fw, t);
								int u = 0;
								break;
							}
							/*else if (myHc->cpiVec[ci].cstType == PHOpHapticController::cstEdge)
							{
							int obji = myHc->cpiVec[ci].objid;
							GRMesh *tgtm = dfmObj[obji].targetMesh;
							float ct;
							bool itflag = IntersectSegmentCylinder(newSubGoal, newSubGoal - myHc->cpiVec[ci].planeN,
							tgtm->vertices[myHc->cpiVec[ci].edgeidA], tgtm->vertices[myHc->cpiVec[ci].edgeidB],
							myHc->c_obstRadius, ct);
							int u = 0;
							break;
							}*/
						}

						int u = 0;
#endif
					}
#ifdef Save_GoalPos
					objnewSubstarts.push_back(newSubStart);
					objnewSubgoals.push_back(newSubGoal);
#endif
					//save barycentric coordinate in first face constrain (ignore the case that no face constr inside)
					//for (int ci = 0;ci<)
				}
				else {
					break;
					/*
					myHc->GetMyHpProxyParticle()->pNewCtr = nextSubGoal;
					DSTR<<"pNewCtr set to nextSubGoal"<<endl;*/
				}

		{//Third constrain 

			PHOpHapticController::ConstrainPlaneInfo &tmpcpinfo2 = cpiVec[2];
#ifdef USE_GLOBLE_COLLI
			thrStepOn = intersectOnRoute(newSubStart - ((nextSubGoal - tmpcpinfo1.planeP).unit())*mPrevPrecE, newSubGoal, nextSubStart, nextSubGoal, tmpcpinfo2, false);
			//thrStepOn = intersectOnRoute(newSubStart , newSubGoal, nextSubStart, nextSubGoal, tmpcpinfo2, false);
#else
			thrStepOn = intersectOnRouteInColliPs(newSubStart - ((nextSubGoal - tmpcpinfo1.planeP).unit())*mPrevPrecE, newSubGoal, nextSubStart, nextSubGoal, tmpcpinfo2, false);
#endif
			if (thrStepOn)
			{
				myHc->constrainCount = 3;

#ifdef PrintInfoColliSolve

				switch (tmpcpinfo2.cstType)
				{
				case PHOpHapticController::cstEdge:
					DSTR << "Third cstEdge " << tmpcpinfo2.edgeidA << " " << tmpcpinfo2.edgeidB << " ";

					break;
				case PHOpHapticController::cstPoint:
					DSTR << "Third cstPoint " << tmpcpinfo2.vid << " ";

					break;
				case PHOpHapticController::cstFace:
					DSTR << "Third cstFace " << tmpcpinfo2.planeid << " ";

					break;
				default:
					break;
				}

				DSTR << endl;
#endif	
				for (int samei = 0; samei<myHc->constrainCount - 1; samei++)
				{

					switch (tmpcpinfo2.cstType)
					{
					case PHOpHapticController::cstEdge:

						if (tmpcpinfo2.edgeidA == cpiVec[samei].edgeidA&&tmpcpinfo2.edgeidB == cpiVec[samei].edgeidB)
							sameinside = true;
						break;
					case PHOpHapticController::cstPoint:

						if (tmpcpinfo2.vid == cpiVec[samei].vid)
							sameinside = true;
						break;
					case PHOpHapticController::cstFace:

						if (tmpcpinfo2.planeid == cpiVec[samei].planeid)
							sameinside = true;
						break;
					default:
						break;
					}
				}


				if (sameinside)
				{
#ifdef PrintInfoColliSolve
					DSTR << "same inside" << endl;
#endif
					myHc->constrainCount--;
					break;
				}
				int Nirs = 3 + myHc->constrainCount;
				int* turns = new int[Nirs];
				VMatrixRow< double > clms;
				clms.resize(Nirs, Nirs);
				clms.clear(0.0);
				VVector< double > d;
				d.resize(Nirs);
				d.clear(0.0);

				VVector< double > f;
				f.resize(Nirs);
				f.clear(0.0);

				//colums with number "1"
				for (int mi = 0; mi<3; mi++)
				{
					clms[mi][mi] = 1;
				}

				//make counterPlane for Proxy
				Vec3f counterPlane;
				for (int pbpi = 0; pbpi<myHc->constrainCount; pbpi++)
					//for(int pbpi = 0;pbpi<pssbPlanes.size();pbpi++)
				{
					int objid = cpiVec[pbpi].objid;
					//int corrTurni;
					//if (pbpi == 0)
					//{
					//	//if ((cpiVec[pbpi].planeid == 2) || (cpiVec[pbpi].planeid == 3))
					//	{
					//		corrTurni = 0;
					//	}
					//	//else corrTurni = pbpi;
					//}
					//else if (pbpi == 1)
					//{
					//	//if ((cpiVec[pbpi].planeid == 3) || (cpiVec[pbpi].planeid == 0))
					//	{
					//		corrTurni = 2;
					//	}
					//}
					//else if (pbpi == 2)
					//{
					//	//if ((cpiVec[pbpi].planeid == 2) || (cpiVec[pbpi].planeid == 0))
					//	{
					//		corrTurni = 3;
					//	}
					//}

					PHOpObj *myobj = (*opObjs)[objid];
					GRMesh *tgtMesh = myobj->targetMesh;
					Vec3f planeN = cpiVec[pbpi].planeN;
					Vec3f planeP = cpiVec[pbpi].planeP;

					counterPlane += planeN;

					float fA = planeN.x, fB = planeN.y, fC = planeN.z;
					float fD = -(fA * planeP.x + fB * planeP.y + fC * planeP.z);

					//render->SetMaterial(GRRenderIf::RED);
					//render->DrawLine(planeP,planeP+(planeN * 1.5));

					clms[0][3 + pbpi] = fA;
					clms[1][3 + pbpi] = fB;
					clms[2][3 + pbpi] = fC;

					clms[3 + pbpi][0] = fA;
					clms[3 + pbpi][1] = fB;
					clms[3 + pbpi][2] = fC;

					d[3 + pbpi] = -fD;

				}
				d[0] = currGoal.x;
				d[1] = currGoal.y;
				d[2] = currGoal.z;
#ifdef PrintCalInfoColliSolve
				DSTR << "constrainPlaneMatrix in proxy solve 2:" << endl;
				for (int i = 0; i<Nirs; i++)
				{
					for (int j = 0; j<Nirs; j++)
						DSTR << clms[i][j] << " ";

					DSTR << "* f = " << d[i] << endl;
				}
#endif
				/*GaussElimi (clms, f, -d);
				DSTR<<"Result1 f:"<<endl;
				for(int i = 0;i<Nirs;i++)
				{

				DSTR<<" springhead GaussElimi f = "<<f[i];
				}
				DSTR<<endl;*/
				//GaussElimi::SolveGaussElimi(clms, d, Nirs, f);
				clms.gauss(f, d, turns);
				delete turns;
#ifdef PrintCalInfoColliSolve
				DSTR << "Result2 f:" << endl;
				for (int i = 0; i<Nirs; i++)
				{

					DSTR << " f = " << f[i];
				}

				DSTR << endl;
#endif
				counterPlane /= myHc->constrainCount;
				counterPlane.unitize();

				if (f[0] == 0.0f&&f[1] == 0.0f&&f[2] == 0.0f)
				{
					//parallel plane detected use only one 
#ifdef PrintInfoColliSolve
					DSTR << "parallel detected!proxysolve 2" << endl;
#endif	
					myHc->constrainCount = 1;
					parallelDetected = true;
					//break;
					Vec3f f = (newSubGoal - myHc->userPos) * 3;
					float magni = f.norm();
					if (magni > 10.0f)
					{
						DSTR << "tst Big Force Output!" << endl;
						f.clear();
					}

					break;
				}
				else{
					Vec3f couterPSubGoal;
					couterPSubGoal.x = f[0];
					couterPSubGoal.y = f[1];
					couterPSubGoal.z = f[2];


					newSubGoal = couterPSubGoal;
					newSubStart = tmpcpinfo2.planeP;
					myHc->couterPlaneN = counterPlane;
#ifdef STORE_LOCALCOORD
					//find local coordinates for proxyfix
					//find face or edge
					for (int ci = 0; ci < myHc->constrainCount; ci++)
					{
						if (myHc->cpiVec[ci].cstType == PHOpHapticController::cstFace)
						{
							int obji = myHc->cpiVec[ci].objid;
							GRMesh *tgtm = dfmObj[obji].targetMesh;
							float t;
							bool itflag = IntersectSegmentTriangle(newSubStart, newSubGoal,
								tgtm->vertices[tgtm->faces[myHc->cpiVec[ci].planeid].indices[2]] + cpiVec[ci].planeN * myHc->c_obstRadius,
								tgtm->vertices[tgtm->faces[myHc->cpiVec[ci].planeid].indices[1]] + cpiVec[ci].planeN * myHc->c_obstRadius,
								tgtm->vertices[tgtm->faces[myHc->cpiVec[ci].planeid].indices[0]] + cpiVec[ci].planeN * myHc->c_obstRadius,
								myHc->cpiVec[ci].fu, myHc->cpiVec[ci].fv, myHc->cpiVec[ci].fw, t);
							int u = 0;
							//ClosestPtoTriangle(newSubStart)
							break;
						}
						/*else if (myHc->cpiVec[ci].cstType == PHOpHapticController::cstEdge)
						{
						int obji = myHc->cpiVec[ci].objid;
						GRMesh *tgtm = dfmObj[obji].targetMesh;
						float ct;
						bool itflag = IntersectSegmentCylinder(newSubStart, newSubGoal,
						tgtm->vertices[myHc->cpiVec[ci].edgeidA], tgtm->vertices[myHc->cpiVec[ci].edgeidB],
						myHc->c_obstRadius, ct);
						int u = 0;
						break;
						}*/
					}
					int u = 0;
#endif
				}
#ifdef Save_GoalPos
				objnewSubstarts.push_back(newSubStart);
				objnewSubgoals.push_back(newSubGoal);
#endif
			}
		}
				// newSubStart = newSubStart;// + myHc->couterPlaneN*0.002f;
				//myHc->GetMyHpProxyParticle()->pNewCtr = newSubStart;
#ifdef PrintCalInfoColliSolve		
				DSTR << "final" << endl;
				DSTR << "ctcNum: " << myHc->constrainCount << endl;
				DSTR << "newSubGoal: " << newSubGoal.x << " " << newSubGoal.y << " " << newSubGoal.z << endl;
				DSTR << "couterPlaneN: " << myHc->couterPlaneN.x << " " << myHc->couterPlaneN.y << " " << myHc->couterPlaneN.z << endl;
#endif		
				{
					//Over Third faces 
					//for simple if speed is not enough,only use 


				}

			} while (0);

			
			Vec3f closestP;
			if (myHc->constrainCount == 1)
			{
				hdp->pNewCtr = newSubGoal;

#ifdef PrintNewCtr
				DSTR << "pNewCtr is setting (in currConstrNum = 1)" << hdp->pNewCtr.x << " " << hdp->pNewCtr.y << " " << hdp->pNewCtr.z << endl;
#endif
				//closestP = newSubGoal;
			}
			else if (myHc->constrainCount == 2)
			{
				hdp->pNewCtr = newSubGoal;
#ifdef PrintNewCtr
				DSTR << "pNewCtr is setting (in currConstrNum = 2)" << hdp->pNewCtr.x << " " << hdp->pNewCtr.y << " " << hdp->pNewCtr.z << endl;
#endif
				//closestP = newSubGoal;
			}
			else if (myHc->constrainCount == 3)
			{
				//myHc->GetMyHpProxyParticle()->pNewCtr = newSubStart;
				hdp->pNewCtr = newSubGoal;
#ifdef PrintNewCtr
				DSTR << "pNewCtr is setting (in currConstrNum = 3)" << hdp->pNewCtr.x << " " << hdp->pNewCtr.y << " " << hdp->pNewCtr.z << endl;
#endif
				//closestP = newSubStart;
			}

			if (sameinside || parallelDetected)
			{
				hdp->pNewCtr = newSubStart;
#ifdef PrintNewCtr
				DSTR << "pNewCtr is setting parallel(in currConstrNum = 1)" << hdp->pNewCtr.x << " " << hdp->pNewCtr.y << " " << hdp->pNewCtr.z << endl;
#endif
			}

			//save local coordinates for proxyfix
			Vec3f counterP;
			for (int csti = 0; csti < myHc->constrainCount; csti++)
			{
				PHOpHapticController::ConstrainPlaneInfo &cpinfo1 = myHc->cpiVec[csti];
				GRMesh *tgtm = (*opObjs)[cpinfo1.objid]->targetMesh;
				Vec3f *a, *b, *la, *lb, *lc, *v;
				Vec3f d;
				float t;
				switch (cpinfo1.cstType)
				{
				case PHOpHapticController::cstEdge:
					a = &tgtm->vertices[cpinfo1.edgeidA];
					b = &tgtm->vertices[cpinfo1.edgeidB];
					//update local coordinates
					//SaveCylLocalCoordinates(*a, *b, myHc->GetMyHpProxyParticle()->pNewCtr, currGoal, myHc->c_obstRadius, cpinfo1.segmentt);
					CollisionFuntions::SaveCylLocalCoordinates(hdp->pNewCtr, *a, *b, cpinfo1.segmentt, d);
					//update direction
					cpinfo1.planeN = (hdp->pNewCtr - d);// .unit();
					//counterP += cpinfo1.planeN;
					break;
				case PHOpHapticController::cstPoint:
					v = &tgtm->vertices[cpinfo1.vid];
					//update direction
					cpinfo1.planeN = (hdp->pNewCtr - *v);// .unit();
					//counterP += cpinfo1.planeN;
					break;
				case PHOpHapticController::cstFace:
					la = &tgtm->vertices[tgtm->faces[cpinfo1.planeid].indices[2]];
					lb = &tgtm->vertices[tgtm->faces[cpinfo1.planeid].indices[1]];
					lc = &tgtm->vertices[tgtm->faces[cpinfo1.planeid].indices[0]];
					//update local coordinates
					CollisionFuntions::SaveTriLocalCoordinates(hdp->pNewCtr, currGoal, *la, *lb, *lc, cpinfo1.fu, cpinfo1.fv, cpinfo1.fw, t);
					//counterP += cross(*lb - *la, *lc - *la);
					break;
				default:
					break;
				}
			}
			//myHc->couterPlaneN = counterP / myHc->constrainCount;

			//outOf Constr test
			bool outTst = false;
			PHOpHapticController::ConstrainPlaneInfo tmpcpinfotst;
			outTst = intersectOnRoute(hdp->pNewCtr, currGoal, newSubStart, newSubGoal, tmpcpinfotst, false);
			if (outTst)
			{
				bool inCstr = false;

				for (int csti = 0; csti < myHc->constrainCount; csti++)
				{
					PHOpHapticController::ConstrainPlaneInfo &cpinfo1 = myHc->cpiVec[csti];
					switch (cpinfo1.cstType)
					{
					case PHOpHapticController::cstEdge:
						if ((cpinfo1.edgeidA == tmpcpinfotst.edgeidA&&cpinfo1.edgeidB == tmpcpinfotst.edgeidB)
							|| (cpinfo1.edgeidA == tmpcpinfotst.edgeidB&&cpinfo1.edgeidA == tmpcpinfotst.edgeidB))
						{
							inCstr = true;
						}
						break;
					case PHOpHapticController::cstPoint:
						if (cpinfo1.vid == tmpcpinfotst.vid)
						{
							inCstr = true;
						}
						break;
					case PHOpHapticController::cstFace:
						if (cpinfo1.planeid == tmpcpinfotst.planeid)
						{
							inCstr = true;
						}
						break;
					default:
						break;

					}
				}

				if (!inCstr)
				{
					proxyItrtNum++;
					if (proxyItrtNum < proxyItrtMaxNum)
					{
#ifdef PrintItrctingProxySolve
						DSTR << "iterating" << proxyItrtNum << endl;
#endif
						//vector<PHOpHapticController::ConstrainPlaneInfo*>::iterator unii = unique(slvItr.constrainIs.begin(), slvItr.constrainIs.end());
						//if (unii == slvItr.constrainIs.end())
						bool repeat = false;
						for (int ii = 0; ii < slvItr.constrainIs.size(); ii++)
						{
							PHOpHapticController::ConstrainPlaneInfo *cpinfo1 = slvItr.constrainIs[ii];
							switch (cpinfo1->cstType)
							{
							case PHOpHapticController::cstEdge:
								if ((cpinfo1->edgeidA == tmpcpinfotst.edgeidA&&cpinfo1->edgeidB == tmpcpinfotst.edgeidB)
									|| (cpinfo1->edgeidA == tmpcpinfotst.edgeidB&&cpinfo1->edgeidA == tmpcpinfotst.edgeidB))
								{
									repeat = true;
								}
								break;
							case PHOpHapticController::cstPoint:
								if (cpinfo1->vid == tmpcpinfotst.vid)
								{
									repeat = true;
								}
								break;
							case PHOpHapticController::cstFace:
								if (cpinfo1->planeid == tmpcpinfotst.planeid)
								{
									repeat = true;
								}
								break;
							default:
								break;

							}


						}
						//	if (!repeat)
						{
							slvItr.constrainIs.push_back(&tmpcpinfotst);

							HpConstrainSolve(hdp->pNewCtr);
						}
					}
					else {
						int u = 0;
					}
				}

			}
			else
			{
				myHc->constrainCount = 0;
#ifdef PrintInfoColliSolve
				if (myHc->hcProxyOn)
				{
					if (!firStepOn){
						DSTR << "noCTC" << endl;
					}
					else if (!secStepOn)
					{
						DSTR << "noCTC on secCTC" << endl;
					}
					else if (!thrStepOn)
					{
						DSTR << "noCTC on thrCTC" << endl;
					}
					else DSTR << "What's wrong???" << endl;
					DSTR << "hcProxyOff" << endl;
				}
#endif
				myHc->hcProxyOn = false;

				myHc->couterPlaneN.clear();
			}


		}
		else {
			//myHc->GetMyHpProxyParticle()->pNewCtr = myHc->userPos;

			myHc->constrainCount = 0;
#ifdef PrintInfoColliSolve
			if (myHc->hcProxyOn)
			{
				if (!firStepOn){
					DSTR << "noCTC" << endl;
				}
				else if (!secStepOn)
				{
					DSTR << "noCTC on secCTC" << endl;
				}
				else if (!thrStepOn)
				{
					DSTR << "noCTC on thrCTC" << endl;
				}
				else DSTR << "What's wrong???" << endl;
				DSTR << "hcProxyOff" << endl;
			}
#endif
			myHc->hcProxyOn = false;

			myHc->couterPlaneN.clear();
		}
	}


	bool PHOpHapticRenderer::intersectOnRouteInColliPs(Vec3f currSubStart, Vec3f currSubGoal, Vec3f &newSubStart, Vec3f &newSubGoal, PHOpHapticController::ConstrainPlaneInfo &cpinfo, bool inverseF)
	{
		MatrixExtension me;

		//vector<Vec3f> subRouteGoals;
		//Vec3f nearestRouteStart;
		float nearestDist = 100.0f;
		//PHOpHapticController::CtcConstrain intsctInfo;
		//vector<PHOpHapticController::CtcConstrain> intsctInfo;

		//be carefull all the projection should be found related to userpoint not newsubgoal
		Vec3f &userPoint = myHc->userPos;

		Vec3f prcRoute = currSubGoal - currSubStart;
		Vec3f &p = currSubStart;
		Vec3f &u = currSubGoal;
		//Vec3f candA,candB,candC;
		bool intsectDtcted = false;
		int IsIntsctedOverf;
		int IsIntsctedEdgecyliner;
		int IsIntsctedSphere;
		int IsIntsctedOverf2;
		//float enlargeProp = 5;
		float detectRadius = (userPoint - currSubStart).norm();// prcRoute.norm();
		if (detectRadius < myHc->hcElmDtcRadius)
			detectRadius = myHc->hcElmDtcRadius;

		for (int suspi = 0; suspi<myHc->hcColliedPs.size(); suspi++)
		{
			int obji = myHc->hcColliedPs[suspi].objIndex;

			vector<int> vsInRange;
			vector<int> fsInRange;

			PHOpParticle &dp = (*opObjs)[obji]->objPArr[myHc->hcColliedPs[suspi].ptclIndex];
			GRMesh* tgtMesh = (*opObjs)[obji]->targetMesh;
			int vertNum = tgtMesh->NVertex();
			int faceNum = tgtMesh->NFace();
			//collect face inrange
			//for(int ofi = 0;ofi < faceNum; ofi++)
			for (int finpi = 0; finpi<dp.pFaceInd.size(); finpi++)
			{
				int ofi = dp.pFaceInd[finpi];
				bool finr = false;
				int faceq1 = 2; int faceq3 = 0;

				if (inverseF)
				{
					faceq1 = 0; faceq3 = 2;
				}
				Vec3f &fpa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[faceq1]];
				Vec3f &fpb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
				Vec3f &fpc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[faceq3]];
				if (fabs((fpa - currSubStart).norm())<(detectRadius))
				{
					//vsInRange.push_back(tgtMesh->faces[ofi].indices[2]);
					finr = true;
				}
				if (fabs((fpb - currSubStart).norm())<(detectRadius))
				{
					//vsInRange.push_back(tgtMesh->faces[ofi].indices[1]);
					finr = true;
				}
				if (fabs((fpc - currSubStart).norm())<(detectRadius))
				{
					//vsInRange.push_back(tgtMesh->faces[ofi].indices[0]);
					finr = true;
				}
				if (finr)
					fsInRange.push_back(ofi);

			}
			//for(int ofi = 0;ofi < vertNum; ofi++)
			for (int vinpi = 0; vinpi <dp.pNvertex; vinpi++)
			{
				int ofi = dp.pVertArr[vinpi];
				bool finr = false;
				Vec3f &fpa = tgtMesh->vertices[ofi];

				if (fabs((fpa - currSubStart).norm())<detectRadius)
				{
					vsInRange.push_back(ofi);
				}

			}
			//new for tstoverTriface
			for (int fi = 0; fi < fsInRange.size(); fi++)
			{
				int ofi = fsInRange[fi];

				//tstOnOverTRi a triangle on the upper position of the face  
				int faceq1 = 2; int faceq3 = 0;

				if (inverseF)
				{
					faceq1 = 0; faceq3 = 2;
				}
				Vec3f &fpa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[faceq1]];
				Vec3f &fpb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
				Vec3f &fpc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[faceq3]];
				if (tgtMesh->faces[ofi].nVertices == 4)
					DSTR << "rectangle detected" << endl;

				Vec3f dyFaceNormal = cross(fpb - fpa, fpc - fpa);
				dyFaceNormal.unitize();
				dyFaceNormal = dyFaceNormal * myHc->c_obstRadius;

				Vec3f abFpa = fpa + dyFaceNormal;
				Vec3f abFpb = fpb + dyFaceNormal;
				Vec3f abFpc = fpc + dyFaceNormal;
#ifdef DrawRouteInfo
				render->SetMaterial(GRRenderIf::BLACK);
				render->DrawLine(abFpa, abFpb);
				render->DrawLine(abFpc, abFpb);
				render->DrawLine(abFpa, abFpc);
#endif	
				float fu, fv, fw, ft;

				IsIntsctedOverf = CollisionFuntions::IntersectSegmentTriangle(p, u, abFpa, abFpb, abFpc, fu, fv, fw, ft);
				if (IsIntsctedOverf == 1)
				{
					Vec3f itsctPos = abFpa*fu + abFpb*fv + abFpc*fw;
					//Vec3f itsctPos = p + ft*(u - p);
					Affinef affpos;

					float fA = ((abFpb.y - abFpa.y)*(abFpc.z - abFpa.z) - (abFpb.z - abFpa.z)*(abFpc.y - abFpa.y));
					float fB = ((abFpb.z - abFpa.z)*(abFpc.x - abFpa.x) - (abFpb.x - abFpa.x)*(abFpc.z - abFpa.z));
					float fC = ((abFpb.x - abFpa.x)*(abFpc.y - abFpa.y) - (abFpb.y - abFpa.y)*(abFpc.x - abFpa.x));
					float fD = -(fA * abFpa.x + fB * abFpa.y + fC * abFpa.z);

					Vec3f tmpv = itsctPos;// - dyFaceNormal * 0.0002f;
					float tmp = -(fA * tmpv.x + fB * tmpv.y + fC * tmpv.z);
					tmp = -(fA * fpb.x + fB * fpb.y + fC * fpb.z);
#ifdef DRAW_INTSINFO
					affpos.Pos() = itsctPos;
					render->SetMaterial(GRRenderIf::ORANGE);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();
#endif
					float dist = fabs((currSubStart - itsctPos).norm());
					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstFace;
						cpinfo.fu = fu;
						cpinfo.fw = fw;
						cpinfo.fv = fv;
						cpinfo.planeid = ofi;
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;

					}
					//subRouteGoals.push_back(itsctPos + dyFaceNormal.unit() *0.0002f);
					//subRouteGoals.push_back(itsctPos);
					/*PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstFace,ofi,obji);
					tmpcc.fu = fu;
					tmpcc.fw = fw;
					tmpcc.fv = fv;
					intsctInfo.push_back(tmpcc);*/
				}
			}

			//new for tstEdgeCylinder
			for (int fi = 0; fi < fsInRange.size(); fi++)
			{
				int i = fsInRange[fi];
				//no map is needed
				int evid1, evid2;
				//case A-B for Tri ABC
				evid1 = tgtMesh->faces[i].indices[2];
				evid2 = tgtMesh->faces[i].indices[1];

				Vec3f &a = tgtMesh->vertices[evid1];
				Vec3f &b = tgtMesh->vertices[evid2];
				float ct;
				IsIntsctedEdgecyliner = CollisionFuntions::IntersectSegmentCylinder(p, u, a, b, myHc->c_obstRadius, ct);
				if (IsIntsctedEdgecyliner == 1)
				{
					Vec3f itsctPos = p + ct *(u - p);
					Affinef affpos;
#ifdef DRAW_INTSINFO							
					affpos.Pos() = itsctPos;
					render->SetMaterial(GRRenderIf::BROWN);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();
#endif
					float ct;
					Vec3f clsP;
					CollisionFuntions::ClosestPtPointSegment(itsctPos, a, b, ct, clsP);
					Vec3f dyFaceNormal = (itsctPos - clsP).unit();

					float norm = fabs((a - b).norm());
					float segT = fabs((a - clsP).norm()) / norm;

					float dist = fabs((currSubStart - itsctPos).norm());
					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstEdge;
						cpinfo.edgeidA = evid1;
						cpinfo.edgeidB = evid2;
						cpinfo.routet = ct;
						cpinfo.segmentt = segT;
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;
					}

					//subRouteGoals.push_back(itsctPos + dyFaceNormal.unit() *0.0002f);
					////subRouteGoals.push_back(itsctPos);
					//PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstEdge,evid1,evid2,obji);
					//intsctInfo.push_back(tmpcc);
				}

				//case B-C for Tri ABC
				evid1 = tgtMesh->faces[i].indices[1];
				evid2 = tgtMesh->faces[i].indices[0];

				Vec3f &c = tgtMesh->vertices[evid1];
				Vec3f &d = tgtMesh->vertices[evid2];

				IsIntsctedEdgecyliner = CollisionFuntions::IntersectSegmentCylinder(p, u, c, d, myHc->c_obstRadius, ct);
				if (IsIntsctedEdgecyliner == 1)
				{
					Vec3f itsctPos = p + ct *(u - p);
					Affinef affpos;
#ifdef DRAW_INTSINFO	
					affpos.Pos() = itsctPos;
					render->SetMaterial(GRRenderIf::BROWN);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();
#endif


					float ct;
					Vec3f clsP;
					CollisionFuntions::ClosestPtPointSegment(itsctPos, c, d, ct, clsP);
					Vec3f dyFaceNormal = (itsctPos - clsP).unit();
					float dist = fabs((currSubStart - itsctPos).norm());
					float norm = fabs((c - d).norm());
					float segT = fabs((c - clsP).norm()) / norm;

					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstEdge;
						cpinfo.edgeidA = evid1;
						cpinfo.edgeidB = evid2;
						cpinfo.routet = ct;
						cpinfo.segmentt = segT;
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;
					}

					//subRouteGoals.push_back(itsctPos + dyFaceNormal.unit() *0.0002f);
					////subRouteGoals.push_back(itsctPos);
					//PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstEdge,evid1,evid2,obji);
					//intsctInfo.push_back(tmpcc);
				}

				//case C-A for Tri ABC
				evid1 = tgtMesh->faces[i].indices[2];
				evid2 = tgtMesh->faces[i].indices[0];

				Vec3f &e = tgtMesh->vertices[evid1];
				Vec3f &f = tgtMesh->vertices[evid2];

				IsIntsctedEdgecyliner = CollisionFuntions::IntersectSegmentCylinder(p, u, e, f, myHc->c_obstRadius, ct);
				if (IsIntsctedEdgecyliner == 1)
				{
					Vec3f itsctPos = p + ct *(u - p);
					Affinef affpos;
#ifdef DRAW_INTSINFO
					affpos.Pos() = itsctPos;
					render->SetMaterial(GRRenderIf::BROWN);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();
#endif


					float ct;
					Vec3f clsP;
					CollisionFuntions::ClosestPtPointSegment(itsctPos, e, f, ct, clsP);
					Vec3f dyFaceNormal = (itsctPos - clsP).unit();
					float dist = fabs((currSubStart - itsctPos).norm());
					float norm = fabs((e - f).norm());
					float segT = fabs((e - clsP).norm()) / norm;
					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstEdge;
						cpinfo.edgeidA = evid1;
						cpinfo.edgeidB = evid2;
						cpinfo.routet = ct;
						cpinfo.segmentt = segT;
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;
					}
					//subRouteGoals.push_back(itsctPos + dyFaceNormal.unit() *0.0002f);
					////subRouteGoals.push_back(itsctPos);
					//PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstEdge,evid1,evid2,obji);
					//intsctInfo.push_back(tmpcc);
				}
			}

			//new for tstvertexBall
			for (int vi = 0; vi<vsInRange.size(); vi++)
			{
				Vec3f &vP = tgtMesh->vertices[vsInRange[vi]];


				float vt;
				Vec3f itsctPos;
				IsIntsctedSphere = CollisionFuntions::IntersectSegmentSphere(p, (u - p).unit(), (u - p).norm(), vP, myHc->c_obstRadius, vt, itsctPos);
				if (IsIntsctedSphere == 1)
				{
#ifdef DRAW_INTSINFO	
					Affinef affpos;
					render->SetMaterial(GRRenderIf::GREEN);
					affpos.Pos() = itsctPos;
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();


					affpos.Pos() = vP;
					render->SetMaterial(GRRenderIf::BROWN);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawSphere(myHc->c_obstRadius, 6, 6, false);
					render->PopModelMatrix();
#endif
					Vec3f dyFaceNormal = (itsctPos - vP).unit();

					float dist = fabs((currSubStart - itsctPos).norm());
					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstPoint;
						cpinfo.vid = vsInRange[vi];
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;
					}
					//subRouteGoals.push_back(itsctPos + dyFaceNormal * 0.0002f);
					////subRouteGoals.push_back(itsctPos);
					//PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstPoint,vsInRange[vi],obji);
					//intsctInfo.push_back(tmpcc);

				}
			}

		}
		/*
		if(subRouteGoals.size()>0)
		{

		float minDist = 100.0f;
		int nearestP = -1;
		for(int i=0;i<subRouteGoals.size();i++)
		{
		Vec3f &goal = subRouteGoals[i];
		float dist= fabs((p - goal).norm());
		if(minDist>dist)
		{
		minDist = dist;
		nearestP = i;
		}
		}*/
		if (nearestDist<100.0f)
		{
			Vec3f nextP = userPoint - cpinfo.planeP;
			float proj = nextP.dot(cpinfo.planeN);
			float magni = fabs(proj);
			newSubGoal = cpinfo.planeP + (nextP + cpinfo.planeN * magni);
			newSubStart = cpinfo.planeP;
			return true;
		}
		//}
		return false;
	}

#ifdef Old_Functions
	bool PHOpHapticRenderer::intersectOnRoute(Vec3f currSubStart, Vec3f currSubGoal, Vec3f &newSubStart, Vec3f &newSubGoal, PHOpHapticController::ConstrainPlaneInfo &cpinfo, bool inverseF)
	{
		MatrixExtension me;
		float mPrevPrecE = 0.001f;
		//vector<Vec3f> subRouteGoals;
		//Vec3f nearestRouteStart;
		float nearestDist = 100.0f;
		//PHOpHapticController::CtcConstrain intsctInfo;
		//vector<PHOpHapticController::CtcConstrain> intsctInfo;

		//be carefull all the projection should be found related to userpoint not newsubgoal
		Vec3f &userPoint = myHc->userPos;

		Vec3f prcRoute = currSubGoal - currSubStart;
		Vec3f &p = currSubStart;
		Vec3f &u = currSubGoal;
		//Vec3f candA,candB,candC;
		bool intsectDtcted = false;
		int IsIntsctedOverf;
		int IsIntsctedEdgecyliner;
		int IsIntsctedSphere;
		int IsIntsctedOverf2;
		//float enlargeProp = 8;
		float detectRadius = (userPoint - currSubStart).norm();
		if (detectRadius < myHc->hcElmDtcRadius)
			detectRadius = myHc->hcElmDtcRadius;


		for (int obji = 0; obji<objNum; obji++)
		{
			vector<int> vsInRange;
			vector<int> fsInRange;

			GRMesh* tgtMesh = (*opObjs)[obji]->targetMesh;
			int vertNum = tgtMesh->NVertex();
			int faceNum = tgtMesh->NFace();
			//collect face inrange
			for (int ofi = 0; ofi < faceNum; ofi++)
			{
				bool finr = false;
				int faceq1 = 2; int faceq3 = 0;

				if (inverseF)
				{
					faceq1 = 0; faceq3 = 2;
				}
				Vec3f &fpa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[faceq1]];
				Vec3f &fpb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
				Vec3f &fpc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[faceq3]];
				//if(fabs((fpa - currSubStart).norm())<(myHc->hcElmDtcRadius * enlargeProp))
				if (fabs((fpa - currSubStart).norm())<(detectRadius))
				{
					//vsInRange.push_back(tgtMesh->faces[ofi].indices[2]);
					finr = true;
				}
				if (fabs((fpb - currSubStart).norm())<(detectRadius))
				{
					//vsInRange.push_back(tgtMesh->faces[ofi].indices[1]);
					finr = true;
				}
				if (fabs((fpc - currSubStart).norm())<(detectRadius))
				{
					//vsInRange.push_back(tgtMesh->faces[ofi].indices[0]);
					finr = true;
				}
				if (finr)
					fsInRange.push_back(ofi);

			}
			for (int ofi = 0; ofi < vertNum; ofi++)
			{
				bool finr = false;
				Vec3f &fpa = tgtMesh->vertices[ofi];

				if (fabs((fpa - currSubStart).norm())<detectRadius)
				{
					vsInRange.push_back(ofi);
				}

			}
			//new for tstoverTriface
			for (int fi = 0; fi < fsInRange.size(); fi++)
			{
				int ofi = fsInRange[fi];

				//tstOnOverTRi a triangle on the upper position of the face  
				int faceq1 = 2; int faceq3 = 0;

				if (inverseF)
				{
					faceq1 = 0; faceq3 = 2;
				}
				Vec3f &fpa = tgtMesh->vertices[tgtMesh->faces[ofi].indices[faceq1]];
				Vec3f &fpb = tgtMesh->vertices[tgtMesh->faces[ofi].indices[1]];
				Vec3f &fpc = tgtMesh->vertices[tgtMesh->faces[ofi].indices[faceq3]];
				if (tgtMesh->faces[ofi].nVertices == 4)
					DSTR << "rectangle detected" << endl;

				Vec3f dyFaceNormal = cross(fpb - fpa, fpc - fpa);
				dyFaceNormal.unitize();
				dyFaceNormal = dyFaceNormal * myHc->c_obstRadius;

				Vec3f abFpa = fpa + dyFaceNormal;
				Vec3f abFpb = fpb + dyFaceNormal;
				Vec3f abFpc = fpc + dyFaceNormal;
#ifdef DrawRouteInfo
				render->SetMaterial(GRRenderIf::BLACK);
				render->DrawLine(abFpa, abFpb);
				render->DrawLine(abFpc, abFpb);
				render->DrawLine(abFpa, abFpc);
#endif	
				float fu, fv, fw, ft;

				IsIntsctedOverf = CollisionFuntions::IntersectSegmentTriangle(p, u, abFpa, abFpb, abFpc, fu, fv, fw, ft);
				if (IsIntsctedOverf == 1)
				{
					Vec3f itsctPos = abFpa*fu + abFpb*fv + abFpc*fw;
					//Vec3f itsctPos = p + ft*(u - p);
					Affinef affpos;

					float fA = ((abFpb.y - abFpa.y)*(abFpc.z - abFpa.z) - (abFpb.z - abFpa.z)*(abFpc.y - abFpa.y));
					float fB = ((abFpb.z - abFpa.z)*(abFpc.x - abFpa.x) - (abFpb.x - abFpa.x)*(abFpc.z - abFpa.z));
					float fC = ((abFpb.x - abFpa.x)*(abFpc.y - abFpa.y) - (abFpb.y - abFpa.y)*(abFpc.x - abFpa.x));
					float fD = -(fA * abFpa.x + fB * abFpa.y + fC * abFpa.z);

					Vec3f tmpv = itsctPos;// - dyFaceNormal * 0.0002f;
					float tmp = -(fA * tmpv.x + fB * tmpv.y + fC * tmpv.z);
					tmp = -(fA * fpb.x + fB * fpb.y + fC * fpb.z);
#ifdef DRAW_INTSINFO
					affpos.Pos() = itsctPos;
					render->SetMaterial(GRRenderIf::ORANGE);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();
#endif
					float dist = fabs((currSubStart - itsctPos).norm());
					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstFace;
						cpinfo.fu = fu;
						cpinfo.fw = fw;
						cpinfo.fv = fv;
						cpinfo.planeid = ofi;
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;

					}
					//subRouteGoals.push_back(itsctPos + dyFaceNormal.unit() *0.0002f);
					//subRouteGoals.push_back(itsctPos);
					/*PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstFace,ofi,obji);
					tmpcc.fu = fu;
					tmpcc.fw = fw;
					tmpcc.fv = fv;
					intsctInfo.push_back(tmpcc);*/
				}
			}

			//new for tstEdgeCylinder
			for (int fi = 0; fi < fsInRange.size(); fi++)
			{
				int i = fsInRange[fi];
				//no map is needed
				int evid1, evid2;
				//case A-B for Tri ABC
				evid1 = tgtMesh->faces[i].indices[2];
				evid2 = tgtMesh->faces[i].indices[1];

				Vec3f &a = tgtMesh->vertices[evid1];
				Vec3f &b = tgtMesh->vertices[evid2];
				float ct;
				IsIntsctedEdgecyliner = CollisionFuntions::IntersectSegmentCylinder(p, u, a, b, myHc->c_obstRadius, ct);
				if (IsIntsctedEdgecyliner == 1)
				{
					Vec3f itsctPos = p + ct *(u - p);
#ifdef DRAW_INTSINFO
					Affinef affpos;

					affpos.Pos() = itsctPos;
					render->SetMaterial(GRRenderIf::BROWN);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();
#endif
					float ct;
					Vec3f clsP;
					CollisionFuntions::ClosestPtPointSegment(itsctPos, a, b, ct, clsP);
					Vec3f dyFaceNormal = (itsctPos - clsP).unit();

					float norm = fabs((a - b).norm());
					float segT = fabs((a - clsP).norm()) / norm;

					float dist = fabs((currSubStart - itsctPos).norm());
					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstEdge;
						cpinfo.edgeidA = evid1;
						cpinfo.edgeidB = evid2;
						cpinfo.routet = ct;
						cpinfo.segmentt = ct;
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;
					}

					//subRouteGoals.push_back(itsctPos + dyFaceNormal.unit() *0.0002f);
					////subRouteGoals.push_back(itsctPos);
					//PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstEdge,evid1,evid2,obji);
					//intsctInfo.push_back(tmpcc);
				}

				//case B-C for Tri ABC
				evid1 = tgtMesh->faces[i].indices[1];
				evid2 = tgtMesh->faces[i].indices[0];

				Vec3f &c = tgtMesh->vertices[evid1];
				Vec3f &d = tgtMesh->vertices[evid2];

				IsIntsctedEdgecyliner = CollisionFuntions::IntersectSegmentCylinder(p, u, c, d, myHc->c_obstRadius, ct);
				if (IsIntsctedEdgecyliner == 1)
				{
					Vec3f itsctPos = p + ct *(u - p);
#ifdef DRAW_INTSINFO
					Affinef affpos;

					affpos.Pos() = itsctPos;
					render->SetMaterial(GRRenderIf::BROWN);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();
#endif


					float ct;
					Vec3f clsP;
					CollisionFuntions::ClosestPtPointSegment(itsctPos, c, d, ct, clsP);
					Vec3f dyFaceNormal = (itsctPos - clsP).unit();
					float dist = fabs((currSubStart - itsctPos).norm());
					float norm = fabs((c - d).norm());
					float segT = fabs((c - clsP).norm()) / norm;

					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstEdge;
						cpinfo.edgeidA = evid1;
						cpinfo.edgeidB = evid2;
						cpinfo.routet = ct;
						cpinfo.segmentt = segT;
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;
					}

					//subRouteGoals.push_back(itsctPos + dyFaceNormal.unit() *0.0002f);
					////subRouteGoals.push_back(itsctPos);
					//PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstEdge,evid1,evid2,obji);
					//intsctInfo.push_back(tmpcc);
				}

				//case C-A for Tri ABC
				evid1 = tgtMesh->faces[i].indices[2];
				evid2 = tgtMesh->faces[i].indices[0];

				Vec3f &e = tgtMesh->vertices[evid1];
				Vec3f &f = tgtMesh->vertices[evid2];

				IsIntsctedEdgecyliner = CollisionFuntions::IntersectSegmentCylinder(p, u, e, f, myHc->c_obstRadius, ct);
				if (IsIntsctedEdgecyliner == 1)
				{
					Vec3f itsctPos = p + ct *(u - p);
#ifdef DRAW_INTSINFO
					Affinef affpos;

					affpos.Pos() = itsctPos;
					render->SetMaterial(GRRenderIf::BROWN);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();
#endif


					float ct;
					Vec3f clsP;
					CollisionFuntions::ClosestPtPointSegment(itsctPos, e, f, ct, clsP);
					Vec3f dyFaceNormal = (itsctPos - clsP).unit();
					float dist = fabs((currSubStart - itsctPos).norm());
					float norm = fabs((e - f).norm());
					float segT = fabs((e - clsP).norm()) / norm;
					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstEdge;
						cpinfo.edgeidA = evid1;
						cpinfo.edgeidB = evid2;
						cpinfo.routet = ct;
						cpinfo.segmentt = segT;
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;
					}
					//subRouteGoals.push_back(itsctPos + dyFaceNormal.unit() *0.0002f);
					////subRouteGoals.push_back(itsctPos);
					//PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstEdge,evid1,evid2,obji);
					//intsctInfo.push_back(tmpcc);
				}
			}

			//new for tstvertexBall
			for (int vi = 0; vi<vsInRange.size(); vi++)
			{
				Vec3f &vP = tgtMesh->vertices[vsInRange[vi]];


				float vt;
				Vec3f itsctPos;
				IsIntsctedSphere = CollisionFuntions::IntersectSegmentSphere(p, (u - p).unit(), (u - p).norm(), vP, myHc->c_obstRadius, vt, itsctPos);
				if (IsIntsctedSphere == 1)
				{
#ifdef DRAW_INTSINFO
					Affinef affpos;
					render->SetMaterial(GRRenderIf::GREEN);
					affpos.Pos() = itsctPos;
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawBox(0.02f, 0.02f, 0.02f, true);
					render->PopModelMatrix();


					affpos.Pos() = vP;
					render->SetMaterial(GRRenderIf::BROWN);
					render->PushModelMatrix();
					render->MultModelMatrix(affpos);
					render->DrawSphere(myHc->c_obstRadius, 6, 6, false);
					render->PopModelMatrix();
#endif
					Vec3f dyFaceNormal = (itsctPos - vP).unit();

					float dist = fabs((currSubStart - itsctPos).norm());
					if (dist<nearestDist)
					{
						nearestDist = dist;
						cpinfo.cstType = PHOpHapticController::cstPoint;
						cpinfo.vid = vsInRange[vi];
						cpinfo.objid = obji;
						dyFaceNormal.unitize();
						cpinfo.planeN = dyFaceNormal;
#ifdef USE_a_little_over_InSolve
						itsctPos = itsctPos + dyFaceNormal.unit() *0.0002f;
#endif
						cpinfo.planeP = itsctPos;
					}
					//subRouteGoals.push_back(itsctPos + dyFaceNormal * 0.0002f);
					////subRouteGoals.push_back(itsctPos);
					//PHOpHapticController::CtcConstrain tmpcc = PHOpHapticController::CtcConstrain(PHOpHapticController::cstPoint,vsInRange[vi],obji);
					//intsctInfo.push_back(tmpcc);

				}
			}

		}

		if (nearestDist<100.0f)
		{
			Vec3f nextP = userPoint - cpinfo.planeP;
			float proj = nextP.dot(cpinfo.planeN);
			float magni = fabs(proj);
			newSubGoal = cpinfo.planeP + (nextP + cpinfo.planeN * magni);
			newSubStart = cpinfo.planeP;

			return true;

		}
		//}
		return false;
	}
#endif


	void PHOpHapticRenderer::BuildEdgeInfo()
	{
		//find all edges
		for (int obji = 0; obji<objNum; obji++)
		{
			map<int, vector<int>> edgeInfo;
			GRMesh* tgtMesh = (*opObjs)[obji]->targetMesh;
			int faceNum = tgtMesh->NFace();
			for (int ofi = 0; ofi < faceNum; ofi++)
			{
				int &a = tgtMesh->faces[ofi].indices[2];
				int &b = tgtMesh->faces[ofi].indices[1];
				int &c = tgtMesh->faces[ofi].indices[0];
				bool fourv = false;
				if (tgtMesh->faces[ofi].nVertices>3)
					fourv = true;
				int d = -1;
				if (fourv) d = tgtMesh->faces[ofi].indices[3];

				map<int, vector<int>>::iterator l_ita, l_itb, l_itc, l_itd;
				l_ita = edgeInfo.find(a);
				l_itb = edgeInfo.find(b);
				l_itc = edgeInfo.find(c);
				l_itd = edgeInfo.find(d);

				vector<int> edgelist;

				if (l_ita == edgeInfo.end() && l_itb == edgeInfo.end() && l_itc == edgeInfo.end() && l_itd == edgeInfo.end())
				{


					edgelist.push_back(b);
					edgelist.push_back(c);
					if (fourv) edgelist.push_back(d);
					edgeInfo.insert(pair<int, vector<int>>(a, edgelist));
					continue;
				}
				else{
					bool listIna = false, listInb = false, listInc = false, listInd = false;

					bool aExistInb = false, aExistInc = false, aExistInd = false;
					bool bExistIna = false, bExistInc = false, bExistInd = false;
					bool cExistIna = false, cExistInb = false, cExistInd = false;
					bool dExistIna = false, dExistInb = false, dExistInc = false;
					if (l_ita != edgeInfo.end())
						listIna = true;
					if (l_itb != edgeInfo.end())
						listInb = true;
					if (l_itc != edgeInfo.end())
						listInc = true;
					if (fourv&&l_itd != edgeInfo.end())
						listInd = true;


					if (listIna)
					{
						vector<int> &va = l_ita->second;
						for (int vi = 0; vi<va.size(); vi++)
						{
							if (va[vi] == b) bExistIna = true;
							if (va[vi] == c) cExistIna = true;
							if (fourv&&va[vi] == d) dExistIna = true;
						}
					}
					if (listInb)
					{
						vector<int> &vb = l_itb->second;
						for (int vi = 0; vi<vb.size(); vi++)
						{
							if (vb[vi] == a) aExistInb = true;
							if (vb[vi] == c) cExistInb = true;
							if (fourv&&vb[vi] == d) dExistInb = true;
						}
					}
					if (listInc)
					{
						vector<int> &vc = l_itc->second;
						for (int vi = 0; vi<vc.size(); vi++)
						{
							if (vc[vi] == a) aExistInc = true;
							if (vc[vi] == b) bExistInc = true;
							if (fourv&&vc[vi] == d) dExistInc = true;
						}
					}
					if (fourv&&listInd)
					{
						vector<int> &vd = l_itd->second;
						for (int vi = 0; vi<vd.size(); vi++)
						{
							if (vd[vi] == a) aExistInd = true;
							if (vd[vi] == b) bExistInd = true;
							if (vd[vi] == c) cExistInd = true;
						}
					}

					if (!fourv)
					{
						if (!aExistInb&&!bExistIna)
						{
							//edgelist.push_back(b);
							if (listIna)
							{
								vector<int> &va = l_ita->second;
								va.push_back(b);
							}
							else {
								vector<int> list;
								list.push_back(b);
								edgeInfo.insert(pair<int, vector<int>>(a, list));
							}
						}
						if (!aExistInc&&!cExistIna)
						{
							//edgelist.push_back(b);
							if (listIna)
							{
								vector<int> &va = l_ita->second;
								va.push_back(c);
							}
							else {
								vector<int> list;
								list.push_back(c);
								edgeInfo.insert(pair<int, vector<int>>(a, list));
							}
						}
						if (!bExistInc&&!cExistInb)
						{
							//edgelist.push_back(b);
							if (listInb)
							{
								vector<int> &vb = l_itb->second;
								vb.push_back(c);
							}
							else {
								vector<int> list;
								list.push_back(c);
								edgeInfo.insert(pair<int, vector<int>>(b, list));
							}
						}
					}
					else {
						if (!aExistInb&&!bExistIna)
						{
							//edgelist.push_back(b);
							if (listIna)
							{
								vector<int> &va = l_ita->second;
								va.push_back(b);
							}
							else {
								vector<int> list;
								list.push_back(b);
								edgeInfo.insert(pair<int, vector<int>>(a, list));
							}
						}
						if (!bExistInc&&!cExistInb)
						{
							//edgelist.push_back(b);
							if (listInb)
							{
								vector<int> &vb = l_itb->second;
								vb.push_back(c);
							}
							else {
								vector<int> list;
								list.push_back(c);
								edgeInfo.insert(pair<int, vector<int>>(b, list));
							}
						}
						if (!cExistInd&&!dExistInc)
						{
							//edgelist.push_back(b);
							if (listInc)
							{
								vector<int> &vc = l_itc->second;
								vc.push_back(d);
							}
							else {
								vector<int> list;
								list.push_back(d);
								edgeInfo.insert(pair<int, vector<int>>(c, list));
							}
						}
						if (!dExistIna&&!aExistInd)
						{
							//edgelist.push_back(b);
							if (listInd)
							{
								vector<int> &vd = l_itd->second;
								vd.push_back(a);
							}
							else {
								vector<int> list;
								list.push_back(a);
								edgeInfo.insert(pair<int, vector<int>>(d, list));
							}
						}
					}
				}
			}
			objEdgeInfos.push_back(edgeInfo);
		}
		int u = 0;

		//refind particle from face globle
		for (int obji = 0; obji<objNum; obji++)
		{
			GRMesh* tgtMesh = (*opObjs)[obji]->targetMesh;
			(*opObjs)[obji]->objFaceBelongs.resize(tgtMesh->NFace());

			for (int pi = 0; pi<(*opObjs)[obji]->assPsNum; pi++)
			{
				PHOpParticle *dp = &(*opObjs)[obji]->objPArr[pi];
				int faceNum = (int)dp->pFaceInd.size();
				for (int fi = 0; fi<faceNum; fi++)
				{
					(*opObjs)[obji]->objFaceBelongs[dp->pFaceInd[fi]] = pi;
				}
			}
		}


	}
	void PHOpHapticRenderer::BuildVToFaceRelation()
	{
		
		DSTR << "Build V to Face Relation" << endl;
		//build v to face relates
		for (int obji = 0; obji<objNum; obji++)
		{
			map<int, vector<int>> vTofInfo;
			if ((*opObjs)[obji]->objNoMeshObj)
				return;

			GRMesh* tgtMesh = (*opObjs)[obji]->targetMesh;
			int faceNum = tgtMesh->NFace();
			for (int ofi = 0; ofi < faceNum; ofi++)
			{
				int &a = tgtMesh->faces[ofi].indices[2];
				int &b = tgtMesh->faces[ofi].indices[1];
				int &c = tgtMesh->faces[ofi].indices[0];

				bool fourv = false;
				if (tgtMesh->faces[ofi].nVertices>3)
					fourv = true;
				int d = -1;
				if (fourv) d = tgtMesh->faces[ofi].indices[3];

				map<int, vector<int>>::iterator l_ita, l_itb, l_itc, l_itd;
				l_ita = vTofInfo.find(a);
				l_itb = vTofInfo.find(b);
				l_itc = vTofInfo.find(c);
				l_itd = vTofInfo.find(d);



				if (l_ita == vTofInfo.end())
				{
					vector<int> vToflist;
					vToflist.push_back(ofi);
					//if(fourv) vToflist.push_back(d);
					vTofInfo.insert(pair<int, vector<int>>(a, vToflist));


				}
				else{

					vector<int>&la = l_ita->second;
					bool inside = false;
					for (int i = 0; i<la.size(); i++)
					{
						if (la[i] == ofi)inside = true;
					}
					if (!inside)
						la.push_back(ofi);


				}
				if (l_itb == vTofInfo.end())
				{
					vector<int> vToflist;
					vToflist.push_back(ofi);
					//if(fourv) vToflist.push_back(d);
					vTofInfo.insert(pair<int, vector<int>>(b, vToflist));


				}
				else{
					vector<int>&lb = l_itb->second;
					bool inside = false;
					for (int i = 0; i<lb.size(); i++)
					{
						if (lb[i] == ofi)inside = true;
					}
					if (!inside)
						lb.push_back(ofi);


				}
				if (l_itc == vTofInfo.end())
				{
					vector<int> vToflist;
					vToflist.push_back(ofi);
					//if(fourv) vToflist.push_back(d);
					vTofInfo.insert(pair<int, vector<int>>(c, vToflist));


				}
				else{
					vector<int>&lc = l_itc->second;
					bool inside = false;
					for (int i = 0; i<lc.size(); i++)
					{
						if (lc[i] == ofi)inside = true;
					}
					if (!inside)
						lc.push_back(ofi);
					continue;
				}
				if (fourv)
				{
					if (l_itd == vTofInfo.end())
					{
						vector<int> vToflist;
						vToflist.push_back(ofi);
						//if(fourv) vToflist.push_back(d);
						vTofInfo.insert(pair<int, vector<int>>(d, vToflist));


					}
					else
					{
						vector<int>&ld = l_itd->second;
						bool inside = false;
						for (int i = 0; i<ld.size(); i++)
						{
							if (ld[i] == ofi)inside = true;
						}
						if (!inside)
							ld.push_back(ofi);

					}
				}

			}

			objVtoFInfos.push_back(vTofInfo);
		}

	}
	
}
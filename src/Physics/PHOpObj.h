#ifndef PHOPOBJ_H
#define PHOPOBJ_H

#include <Physics/SprPHOpObj.h>
#include <Collision/SprCDShape.h>
#include <Physics/PHEngine.h>
#include <Physics/PHSpatial.h>
#include <Foundation/Object.h>
//#include <Graphics\SprGRMesh.h>
//#include <Graphics\GRMesh.h>
#include <Physics/PHOpParticle.h>
#include <Physics/PHOpGroup.h>
//#include "PHOpObjOpEngine.h"
#include <Physics/PHOpDecompositionMethods.h>

#define GROUP_DISTANCE

using namespace PTM;

namespace Spr{
	;


	//class PHOpObj: public SceneObject, public PHOpObjStatePrivate, public PHOpObjDesc {
	class PHOpObj : public SceneObject, public PHOpObjDesc {
		SPR_OBJECTDEF(PHOpObj);
		//ACCESS_DESC_STATE_PRIVATE(PHOpObj);

		//PHOpObj(const PHOpObjDesc& desc=PHOpObjDesc(), SceneIf* s=NULL);

	public:

		PHOpObj(const PHOpObjDesc& desc = PHOpObjDesc(), SceneIf* s = NULL) :PHOpObjDesc(desc)
		{
			//objId = 0;
			//DSTR<<"Don't use Default constructor, the id of Deformer is set in 0"<<std::endl;
			//objId = id;

			gravityOn = false;

			objGrouplinkCount = 5;
			objTargetVtsNum = 0;
			objUnNormalobj = false;
			objUseDistCstr = false;
			objUseReducedPP = true;

			initialVtoPmap = false;
			initialtgV = false;
			initialGArr = false;
			initialOrgP = false;
			initialPArr = false;

			objType = 0;
			objitrTime = 1;
			j.init();
		}

		~PHOpObj()
		{
			if (initialGArr)
				delete[] objGArr;
			if (initialPArr)
				delete[] objPArr;
			if (initialOrgP)
				delete[] objOrigPos;
			if (initialVtoPmap)
				delete[] objVtoPmap;
			if (initialBlWei)
			{
				for (int i = 0; i < objTargetVtsNum; i++)
				{
					delete[] objBlWeightArr[i];

				}
				delete[] objBlWeightArr;
			}
		}

		//Step計算用関数
		//void newReducedPositionProjectByOne(int i);
		//void ReducedPositionProjectByOne(int i);
		//void positionProjectByOne(int i);

		//新しいsimulation方法(慣性Matrixを更新、GoalPositionは平均ではなくそのまま）
		void newReducedPositionProject();
		//simpleの一回simulation過程のまとめる
		void SimpleSimulationStep();

		//shapematching計算をJacobi計算用の
		Jacobi j;
		//力覚Objectと区別のため使う
		int objType;
		//距離拘束のswitch
		bool objUseDistCstr;
		//力覚Objectと区別のため使う
		bool objUnNormalobj;
		//頂点の初期位置（Blendに使う）
		Vec3f* objOrigPos;
		//頂点の初期位置初期化flag
		bool initialOrgP;
		//粒子groupのlink数定数
		int objGrouplinkCount;
		//ObjectID
		int objId;
		//質量中心（開発中、未検証）
		//Vec3f objMassCenter;
		//頂点が粒子groupmemberに対する重み（Blendに使う）
		float** objBlWeightArr;
		//重み初期化されたか
		bool initialBlWei;
		//粒子の初期姿勢
		Spr::TPose<float> * objOrinPsPoseList;
		//頂点から粒子のmap
		int *objVtoPmap;
		//頂点粒子map初期化されたか
		bool initialVtoPmap;
		//半径の平均
		float objAverRadius;
		//半径の平均
		float objMaxRadius;
		//粒子BVH（開発中）
		//BVHgOp *bvhGOps;

		//古いmodel対象
		//GRMesh *targetMesh;
		//model頂点群
		Vec3f *objTargetVts;
		//model頂点群初期化されたか
		bool initialtgV;
		//model頂点群数
		int objTargetVtsNum;
		//一時粒子group保存場
		std::vector<PHOpGroup> tmpPGroups;
		//粒子groupの配列
		PHOpGroup* objGArr;
		//粒子group初期化されたか
		bool initialGArr;
		//粒子groupの数
		int assGrpNum;

		//一時粒子保存場
		std::vector<PHOpParticle> tmpPtclList;
		//粒子の配列
		PHOpParticle *objPArr;//Memoryの考えで、ここで最後に保存しました
		//粒子の配列初期化されたか
		bool initialPArr;
		//粒子の数
		int assPsNum;
		//独立alpha使用switch（開発中）
		bool objUseIndepParam;
		//重力switch
		bool gravityOn;
		//軽量化計算関数switch
		bool objUseReducedPP;
		//粒子と表面meshとの対応関係
		std::vector<int> faceBelongs;


		//int bvhNum;
		//繰り返す数え計数
		int objitrTime;


	private:
		//配置完了のlist（local用）
		std::vector<int> mPtclAssList;
	public:

		void SetGravity(bool bOn)
		{
			gravityOn = bOn;
		}

		void UpdateItrTime(int itrT, bool useIndepParamflag)
		{
			objitrTime = itrT;
			objUseIndepParam = useIndepParamflag;
		}




		void DynamicRadiusUpdate()
		{
			for (int pi = 0; pi < assPsNum; pi++)
			{
				PHOpParticle &dp = objPArr[pi];
				float maxD = 0;
				for (int vi = 0; vi < dp.pNvertex; vi++)
				{
					Vec3f &v = objTargetVts[dp.pVertArr[vi]];
					float dist = fabs((v - dp.pCurrCtr).norm());
					if (dist > maxD)
						maxD = dist;
				}
				dp.pDynamicRadius = maxD;
				if (dp.pNvertex == 1)
					dp.pDynamicRadius = objAverRadius;
			}

		}


		bool assignPtcl(int vIndex)
		{
			if (mPtclAssList.size() == 0)
			{
				mPtclAssList.push_back(vIndex);
				return true;
			}
			for (int i = 0; i< (int)mPtclAssList.size(); i++)
			{
				if (mPtclAssList[i] == vIndex)
				{
					//DSTR<<"Particle already assigned "<<i<<std::endl;
					return false;
				}

			}
			mPtclAssList.push_back(vIndex);
			return true;
		}


		void DistanceConstrain();

		//simulation
		void groupStep();


		void positionPredict();


		void positionProject();


		void ReducedPositionProject();

		//pNewCtrからpCurrCtrへ更新

		void integrationStep();


		void summationTogAgroup();


		void dampVelocities();


		void AddInternalParticle(Vec3f &Pos)//, int* linkPs, int linkNum)
		{
			int i = assPsNum;



			std::vector<DisCmpPoint> disCmpQue;
			DisCmpPoint tmpdcp;

			Vec3f p2;
			float distance;
			for (int j = 0; j < assGrpNum; j++)
			{
				if (j == i)continue;
				p2 = objPArr[j].pOrigCtr;

				p2 = Pos - p2;
				distance = fabs(p2.norm());
				tmpdcp.distance = distance;
				tmpdcp.pIndex = j;

				disCmpQue.push_back(tmpdcp);

			}
			std::sort(disCmpQue.begin(), disCmpQue.end());
			int u = 0;

			PHOpGroup* pg = new PHOpGroup();
			pg->gMyIndex = i;
			pg->gPInd.push_back(i);
			pg->gNptcl++;

			int linkCount = objGrouplinkCount;
			for (int k = 0; k<linkCount; k++)
			{
				pg->addPInds(disCmpQue[k].pIndex);
				//build groupInfo for particle ,particle belongs to...
				objPArr[disCmpQue[k].pIndex].pInGrpList.push_back(i);

			}

			assPsNum++;
			PHOpParticle* newpList = new PHOpParticle[assPsNum];
			for (int j = 0; j<assPsNum - 1; j++)
			{
				newpList[j] = objPArr[j];
			}

			objPArr = newpList;
			objPArr[assPsNum - 1].pPId = assPsNum - 1;
			objPArr[assPsNum - 1].pObjId = this->objId;
			for (int k = 0; k<linkCount; k++)
			{
				objPArr[assPsNum - 1].pInGrpList.push_back(disCmpQue[k].pIndex);
			}
			objPArr[assPsNum - 1].pRadii = 0.5f;//debug
			objPArr[assPsNum - 1].pMyGroupInd = i;
			objPArr[assPsNum - 1].pTotalMass = 1.0f;
			objPArr[assPsNum - 1].pOrigCtr = Pos;
			objPArr[assPsNum - 1].pCurrCtr = Pos;
			objPArr[assPsNum - 1].pNewCtr = Pos;
			//objPArr[assPsNum-1].pType = 1;

			assGrpNum++;
			PHOpGroup* gplist = new PHOpGroup[assGrpNum];
			for (int j = 0; j<assGrpNum - 1; j++)
			{
				gplist[j] = objGArr[j];
			}
			objGArr = gplist;
			objGArr[assGrpNum - 1] = *pg;

			buildGroupCenter();
			BuildBlendWeight();
			int u2 = 0;

			delete pg;
			delete gplist;
			delete newpList;
		}





		Matrix3f SolveShpMchByJacobi(PHOpGroup &pg);




		bool initialDeformObject(Vec3f *vts, int vtsNum, float pSize)
		{
			objOrigPos = new Vec3f[vtsNum];
			initialOrgP = true;
			objTargetVts = vts;//Tetgenため使う
			objTargetVtsNum = vtsNum;

			initalDeformVertex(vts, vtsNum);
			if (!BuildParticles(vts, vtsNum, tmpPtclList, pSize))
				return false;

			return true;
		}



		bool BuildParticles(Vec3f *vts, int vtsNum, std::vector<PHOpParticle> &dParticleArr, float pSize)
		{

			float distance;
			PHOpParticle *dp = new PHOpParticle();

#ifdef TIME_DISPLAY
			mt.startMyTimer();
#endif
			Vec3f beginP, diameterP;
			float objPtclRadius = pSize;

			//decide particle pos & add vertexes to particles
			//Particle自動生成手順
			//最初の頂点からparticle直径範囲内他の頂点を探す、
			//見つかった全ての頂点を一つのparticleにあげる
			//Simple Particle auto generation
			//From first vertex, find other vertex inside particle diameter
			//add all found vetices into one particle ->loop			
			for (int i = 0; i<vtsNum; i++)
			{

				if (!assignPtcl(i))
					continue;
				dp->addNewVertex(i);
				beginP = vts[i];

				for (int j = 0; j<vtsNum; j++)
				{
					diameterP = vts[j];
					diameterP = beginP - diameterP;
					distance = fabs(diameterP.norm());
					if (distance < objPtclRadius)
					{
						if (assignPtcl(j))
						{
							dp->addNewVertex(j);

						}

					}
				}
				dp->pObjId = objId;
				dp->pRadii = objPtclRadius;//debug origin radius

				dParticleArr.push_back(*dp);
				delete dp;
				dp = new PHOpParticle();

			}
			delete dp;

			//Particleに関する情報を生成する
			//Build Particle propoties
			objAverRadius = 0.0f;
			objMaxRadius = 0.0f;
			//objMassCenter = Vec3f(0, 0, 0);
			for (int i = 0; i < (int)dParticleArr.size(); i++)
			{
				PHOpParticle &dp = dParticleArr[i];
				int nsize = dp.pNvertex;
				Vec3f *varr; varr = new Vec3f[nsize];
				for (int j = 0; j < nsize; j++)
					varr[j] = vts[dParticleArr[i].pVertTempInd[j]];

				dp.ptclBuildStep(varr);
				dp.swapVectorToArr();

				objAverRadius += dp.pMainRadius;

				if (dp.pMainRadius>objMaxRadius)
					objMaxRadius = dp.pMainRadius;
				delete[] varr;
			}
			objAverRadius /= dParticleArr.size();

			for (int i = 0; i < (int)dParticleArr.size(); i++)
			{
				PHOpParticle &dp = dParticleArr[i];
				//initial dynamicRadius
				dp.pDynamicRadius = objAverRadius;
				//cal masscenter 
				//objMassCenter += dp.pCurrCtr;

			}
			//objMassCenter = objMassCenter / dParticleArr.size();

			DSTR << "Particle Assignment Finished " << std::endl;

#ifdef TIME_DISPLAY
			mt.endMyTimer();
			mt.printElastedTime();

			mt.startMyTimer();
#endif
			//Particle groupを作る
			//buildGroup
			Vec3f p1, p2;
			//int  nearest[4];
			//predistance = 1000;
			std::vector<DisCmpPoint> disCmpQue;
			DisCmpPoint tmpdcp;

			//particlegroup生成自動化手順
			//最初のparticleから,
			//link 数決める時 
			//ある限り隣のparticleをgroupに入れる  ->loop
			//link 数足りない時
			//あるだけ隣のparticleをgroupに入れる  ->loop
			for (int i = 0; i < (int)dParticleArr.size(); i++)
			{
				dParticleArr[i].pMyGroupInd = i;

				dParticleArr[i].pMainRadius *= objMaxRadius;
				dParticleArr[i].pSecRadius *= objMaxRadius;
				dParticleArr[i].pThrRadius *= objMaxRadius;


				p1 = dParticleArr[i].pOrigCtr;
				tmpdcp.distance = 0.0;
				tmpdcp.pIndex = i;
				disCmpQue.push_back(tmpdcp);
				for (int j = 0; j < (int)dParticleArr.size(); j++)
				{
					if (j == i)continue;
					p2 = dParticleArr[j].pOrigCtr;

					p2 = p1 - p2;
					distance = fabs(p2.norm());
					tmpdcp.distance = distance;
					tmpdcp.pIndex = j;

					disCmpQue.push_back(tmpdcp);

				}
				//隣particleへの距離のsort
				std::sort(disCmpQue.begin(), disCmpQue.end());


#ifdef GROUP_DISTANCE
				PHOpGroup  pg;

				distance = pSize;

				distance += 0.1f;

#endif 
				//build link  
				int linkCount = 0;
				//use limited linklength or constant linkCount
				for (int k = 0; k< (int)disCmpQue.size(); k++)
				{
					if (disCmpQue[k].distance <= distance)
						linkCount++;

				}
				if (linkCount < (int)disCmpQue.size())
					linkCount = objGrouplinkCount; //debug linkCount
				else linkCount = disCmpQue.size();

				if ((int)dParticleArr.size()<linkCount)
					linkCount = dParticleArr.size() - 1;

				pg.gMyIndex = i;
				pg.gPInd.push_back(i);
				pg.gNptcl++;
				for (int k = 0; k<linkCount; k++)
				{
					pg.addPInds(disCmpQue[k].pIndex);
					//build groupInfo for particle ,particle belongs 
					dParticleArr[disCmpQue[k].pIndex].pInGrpList.push_back(i);

				}


				tmpPGroups.push_back(pg);
				swapDisCmpQue(disCmpQue);
			}
			DSTR << "Particle Assignment Finished " << std::endl;
#ifdef TIME_DISPLAY
			mt.endMyTimer();
			mt.printElastedTime();

			mt.startMyTimer();
#endif
			//Arrayへmemoryを変える


			swapMemParticle(dParticleArr);

			swapMemGroup();


			//build weight relation between vertices to particle center
			//buildDisWeightForVsinP();

			//face情報の入力
			//buildFaceInfoInP();

			//VertexからOpまでの逆mapをつくる
			BuildMapFromVtoP();

			//buildGroupCenter
			buildGroupCenter();

#ifdef TIME_DISPLAY
			mt.endMyTimer();
			mt.printElastedTime();
#endif
			//particlegroupweightを作る
			//initial blend weight
			BuildBlendWeight();

			//Particle初期姿勢を記憶する(blendingに使う)
			//build Oringinal Pose of Ps
			objOrinPsPoseList = new TPose<float>[assPsNum];
			for (int j = 0; j<assPsNum; j++)
			{
				Spr::TPose<float> ctrpose = TPose<float>(objPArr[j].pOrigCtr, objPArr[j].pOrigOrint);
				ctrpose = ctrpose.Inv();
				objOrinPsPoseList[j] = ctrpose;
			}



			return true;
		}



		void buildParticleMomentInertia()
		{
			for (int j = 0; j<assPsNum; j++)
			{
				PHOpParticle &dp = objPArr[j];
				Matrix3f &pMIMatrix = dp.pMomentInertia;
				for (int i = 0; i<dp.pNvertex; i++)
				{
					Vec3f &p = objTargetVts[dp.pVertArr[i]] - dp.pCurrCtr;
					pMIMatrix.xx += p.y * p.y + p.z * p.z;
					pMIMatrix.xy += -p.x * p.y;
					pMIMatrix.xz += -p.x * p.z;
					pMIMatrix.yx += -p.y * p.x;
					pMIMatrix.yy += p.x * p.x + p.z * p.z;
					pMIMatrix.yz += -p.y * p.z;
					pMIMatrix.zx += -p.z * p.x;
					pMIMatrix.zy += -p.z * p.y;
					pMIMatrix.zz += p.x * p.x + p.y * p.y;

				}
				pMIMatrix = pMIMatrix * dp.pTempSingleVMass;
				dp.pInverseOfMomentInertia = pMIMatrix.inv();
			}

		}



		void buildDisWeightForVsinP()
		{
			DSTR << "BuildDisWeightForVsinP" << std::endl;
			if (objType != 0)
				return;
			for (int j = 0; j<assPsNum; j++)
			{
				float totalDis = 0.0f;
				objPArr[j].pVectDisWeightArr = new float[objPArr[j].pNvertex];
				objPArr[j].initialWArr = true;
				for (int k = 0; k<objPArr[j].pNvertex; k++)
				{
					Vec3f &vp = objTargetVts[objPArr[j].pVertArr[k]];
					totalDis += (vp - objPArr[j].pCurrCtr).norm();

				}

				for (int k = 0; k<objPArr[j].pNvertex; k++)
				{
					Vec3f &vp = objTargetVts[objPArr[j].pVertArr[k]];
					if (totalDis == 0.0f)
					{
						objPArr[j].pVectDisWeightArr[k] = 1;
						continue;
					}
					objPArr[j].pVectDisWeightArr[k] = (vp - objPArr[j].pCurrCtr).norm() / totalDis;

				}
				int u = 0;
			}

		}

		void buildGroupCenter()
		{
			DSTR << "BuildGroupCenter" << std::endl;
			for (int i = 0; i<assGrpNum; i++)
			{
				objGArr[i].gOrigCenter = objGArr[i].gOrigCenter.Zero();
				objGArr[i].gtotalMass = 0.0f;
				//build group totalmass
				for (int j = 0; j<objGArr[i].gNptcl; j++)
				{
					PHOpParticle &dp = objPArr[objGArr[i].getParticleGlbIndex(j)];
					objGArr[i].gOrigCenter += dp.pOrigCtr * dp.pTotalMass;
					objGArr[i].gtotalMass += dp.pTotalMass;
				}
				objGArr[i].gOrigCenter /= objGArr[i].gtotalMass;
				objGArr[i].gCurrCenter = objGArr[i].gOrigCenter;
			}

		}


		void BuildBlendWeight()
		{
			DSTR << "BuildBlendWeight" << std::endl;
			//build linear blend skin weight

			int size = 0;
			if (objUnNormalobj)
			{
				objBlWeightArr = new float*[1];
				objBlWeightArr[0] = new float[1];
				objBlWeightArr[0][0] = 1;
				initialBlWei = true;
				return;
			}

			size = objTargetVtsNum;

			objBlWeightArr = new float*[size];
			initialBlWei = true;
			for (int j = 0; j<assPsNum; j++)
			{

				Vec3f CenterPs = objPArr[j].pOrigCtr;
				PHOpGroup pg = objGArr[objPArr[j].pMyGroupInd];
				int gpSize = pg.gNptcl;//
				for (int k = 0; k<objPArr[j].pNvertex; k++)
				{//Particle内すべてvertexに対して

					std::vector<float> distancelist;
					float distanceSum = 0.0;
					int vertind = objPArr[j].pVertArr[k];
					Vec3f vert;

					vert = objTargetVts[vertind];

					float tmpdis = (CenterPs - vert).norm();
					distanceSum += tmpdis;//vertexからすべてlinkしてParticleの距離を計算する
					distancelist.push_back(tmpdis);

					float* linkWeight = new float[gpSize];
					float weightsum = 0.0;
					objBlWeightArr[vertind] = new float[gpSize];

					if (distancelist[0] == 0.0f)
					{
						objBlWeightArr[vertind][0] = 1.0;
						for (int jm = 1; jm < gpSize; jm++)
						{
							objBlWeightArr[vertind][jm] = 0.0;
						}
						continue;
					}

					for (int jm = 1; jm < gpSize; jm++)
					{//すべてlinkされたParticleに対して

						//PHOpParticle linkPs = objPArr[pg.getParticleGlbIndex(jm)];
						Vec3f linkPsCenter = objPArr[pg.getParticleGlbIndex(jm)].pOrigCtr;
						float tmpdis = (linkPsCenter - vert).norm();
						distanceSum += tmpdis;
						distancelist.push_back(tmpdis);
					}
					for (int jm = 0; jm < gpSize; jm++)
					{
						linkWeight[jm] = distanceSum / distancelist[jm];
						weightsum += linkWeight[jm];

					}
					for (int jm = 0; jm < gpSize; jm++)
					{
						objBlWeightArr[vertind][jm] = linkWeight[jm] / weightsum;

					}

				}
			}


		}

		//Build inverse relation from vertex to particle


		void BuildMapFromVtoP()
		{
			DSTR << "BuildMapFromVtoP" << std::endl;
			if (this->objType>0)return;
			objVtoPmap = new int[objTargetVtsNum];
			initialtgV = true;
			for (int pi = 0; pi < assPsNum; pi++)
			{
				for (int vi = 0; vi < objPArr[pi].pNvertex; vi++)
					objVtoPmap[objPArr[pi].pVertArr[vi]] = pi;
			}
			int u = 0;
		}


		//Build face infomation inside each Particle 
		//void buildFaceInfoInP()
		//{
		//	for (int i = 0; i<assPsNum; i++)
		//	{
		//		PHOpParticle &dp = objPArr[i];
		//		//if(targetMesh)return;
		//		if (objType>0)return;
		//		if (targetMesh->faces[0].nVertices == 4)
		//		{
		//			DSTR << "faces[0].nVertices==4 please check model or edit code" << std::endl;
		//		}
		//		int facesize = targetMesh->faces.size();
		//		for (int im = 0; im < facesize; im++)
		//		{
		//			std::vector<int> fidx;
		//			int faceVNum = targetMesh->faces[im].nVertices;
		//			for (int fic = 0; fic<faceVNum; fic++)
		//				fidx.push_back(targetMesh->faces[im].indices[fic]);
		//
		//			int count = faceVNum;
		//			bool found = false;
		//			for (int checkin = 0; checkin < faceVNum; checkin++)
		//			{
		//				for (int jm = 0; jm < dp.pNvertex; jm++)
		//				{
		//					int vidx1 = dp.pVertArr[jm];
		//					//if(count==0) 
		//					//	break;
		//					std::vector<int>::iterator iter = std::find(fidx.begin(), fidx.end(), vidx1);
		//					if (iter != fidx.end())
		//					{
		//						found = true;
		//						iter = std::find(dp.pFaceInd.begin(), dp.pFaceInd.end(), im);
		//						if (iter == dp.pFaceInd.end())
		//						{
		//							dp.pFaceInd.push_back(im);
		//							//dp.pVtoFs.insert(dp.pVtoFs.begin()+jm,im);
		//							DSTR << "Face " << im << " detected in " << i << std::endl;
		//						}
		//						break;
		//					}
		//
		//				}
		//				if (found) break;
		//			}
		//
		//			fidx.~vector();
		//		}
		//		int u = 0;
		//	}
		//}


		//clear the memory of the vector used
		void swapMemGroup()
		{
			assGrpNum = tmpPGroups.size();
			objGArr = new PHOpGroup[assGrpNum];
			initialGArr = true;
			for (int i = 0; i<assGrpNum; i++)
			{
				objGArr[i] = tmpPGroups[i];
			}
			std::vector<PHOpGroup> s;

			s.swap(tmpPGroups);
		}

		//clear memory
		void swapDisCmpQue(std::vector<DisCmpPoint> &dcp)
		{
			std::vector<DisCmpPoint> s;
			s.swap(dcp);
		}

		//clear memory
		void swapMemParticle(std::vector<PHOpParticle> &dParticleArr)
		{
			assPsNum = dParticleArr.size();
			objPArr = new PHOpParticle[assPsNum];
			initialPArr = true;
			for (int i = 0; i<assPsNum; i++)
			{
				objPArr[i] = dParticleArr[i];
				objPArr[i].pPId = i;
			}
			std::vector<PHOpParticle> s;

			s.swap(dParticleArr);
		}


		//DeformVertex初期化
		void initalDeformVertex(Vec3f *vts, int vtsNum)
		{
			for (int i = 0; i<vtsNum; i++)
			{
				objOrigPos[i] = vts[i];
			}
		}
		float GetblWeight(int vertexIndex, int linkPIndex)
		{
			return objBlWeightArr[vertexIndex][linkPIndex];
		}

		class m3Bounds
		{
		public:
			m3Bounds(){}
			inline m3Bounds(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }

			inline void set(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }

			void clamp(Vec3f &pos){//境界を超えた点をそのまま境界のどころ設定する
				if (isEmpty()) return;
				//pos.maximum(min);
				maximum(pos, min);
				minimum(pos, max);
			};
			inline bool isEmpty() const {
				if (min.x > max.x&&min.y > max.y&&min.z > max.z) return true;
				return false;
			}
			inline void minimum(Vec3f &self, Vec3f &other) {
				if (other.x < self.x) self.x = other.x;
				if (other.y < self.y) self.y = other.y;
				if (other.z < self.z) self.z = other.z;

			}
			inline void maximum(Vec3f &self, Vec3f &other) {
				if (other.x > self.x) self.x = other.x;
				if (other.y > self.y) self.y = other.y;
				if (other.z > self.z) self.z = other.z;
			}
			Vec3f min, max;
		};



		struct ObjectParams
		{
			ObjectParams()
			{
				setDefaults();
			}

			Vec3f gravity;
			float timeStep;
			m3Bounds bounds;//挙動範囲

			float alpha;
			float beta;
			float gama;

			bool quadraticMatch;
			bool volumeConservation;


			bool allowFlip;

			float veloDamping;


			void setDefaults()
			{
				timeStep = 0.01f;


				gravity = gravity.Zero();
				gravity.y = -9.81f;
				float boundcube = 15.0;
				bounds.min.x = -boundcube;
				bounds.min.y = -boundcube;
				bounds.min.z = -boundcube;
				bounds.max.x = boundcube;
				bounds.max.y = boundcube;
				bounds.max.z = boundcube;

				alpha = 1.0f;
				beta = 0.00f;
				gama = 1.0f;

				quadraticMatch = false;
				volumeConservation = false;

				allowFlip = false;

				veloDamping = 0.05f;

			}
			void SetBound(float boundLength)
			{
				float boundcube = boundLength;
				bounds.min.x = -boundcube;
				bounds.min.y = -boundcube;
				bounds.min.z = -boundcube;
				bounds.max.x = boundcube;
				bounds.max.y = boundcube;
				bounds.max.z = boundcube;
			}

		}params;

		PHOpParticle& getObjParticle(int pIndex)
		{
			return objPArr[pIndex];
		}
		PHOpGroup& getObjPGroup(int groupIndex)
		{
			return objGArr[groupIndex];
		}
		void FindObjctBound()
		{
			float minX = 0.0, maxX = 0.0;
			float minY = 0.0, maxY = 0.0;
			float minZ = 0.0, maxZ = 0.0;
			int axis = 0;
			if (assPsNum>0)
			{
				minX = objPArr[0].pCurrCtr.x;
				maxX = minX;
				minY = objPArr[0].pCurrCtr.y;
				maxY = minY;
				minZ = objPArr[0].pCurrCtr.z;
				maxZ = minZ;
			}

			for (int i = 0; i<assPsNum; i++)
			{
				if (minX>objPArr[i].pCurrCtr.x)
					minX = objPArr[i].pCurrCtr.x;
				if (maxX<objPArr[i].pCurrCtr.x)
					maxX = objPArr[i].pCurrCtr.x;

				if (minY>objPArr[i].pCurrCtr.y)
					minY = objPArr[i].pCurrCtr.y;
				if (maxY<objPArr[i].pCurrCtr.y)
					maxY = objPArr[i].pCurrCtr.y;

				if (minZ>objPArr[i].pCurrCtr.z)
					minZ = objPArr[i].pCurrCtr.z;
				if (maxZ<objPArr[i].pCurrCtr.z)
					maxZ = objPArr[i].pCurrCtr.z;

			}


		}


	};



}// namespace

#endif
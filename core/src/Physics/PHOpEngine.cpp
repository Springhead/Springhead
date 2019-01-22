/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#include <Physics/PHOpEngine.h>
#include "PHOpDecompositionMethods.h"

#define CHECK_INF_ERR
namespace Spr{
	;

	PHOpEngine::PHOpEngine(){
		fdt = 0.01;
		objidIndex = 0;
		radiusCoe = 1.0f;
		opIterationTime = 1;
		subStepProFix = true;
		noCtcItrNum = 0;
		opAnimator = DBG_NEW PHOpAnimation();

		logForce = false;
		useHaptic = false;
		useFriction = false;
		useAnime = false;
	}
	void PHOpEngine::InitialNoMeshHapticRenderer()
	{
		opHpRender = new PHOpHapticRenderer();
		myHc = new PHOpHapticController();
		myHc->InitialHapticController();
		myHc->hpObjIndex = -1;

		myHc->hcType = PHOpHapticControllerDesc::_3DOF;
#ifdef USEGRMESH
		opHpRender->initial3DOFRenderer(myHc, &opObjs);
#else
		DSTR << "Currently 3DOFRenderer is not implemented" << std::endl;
#endif
		//set defualt c_obstacle
		myHc->c_obstRadius = 0.2f;// opObjs[objId]->objAverRadius / 6;
	}
	void PHOpEngine::SetUseHaptic(bool hapticUsage)
	{
		useHaptic = hapticUsage;
	}
	bool PHOpEngine::GetUseHaptic()
	{
		return useHaptic;
	}
	void PHOpEngine::SetUseFriction(bool frictionUsage)
	{
		useFriction = frictionUsage;
	}
	bool PHOpEngine::GetUseFriction()
	{
		return useFriction;
	}

	PHOpObjDesc* PHOpEngine::GetOpObj(int i)
	{
		return opObjs[i];
	}

	/*ObjectIf* PHOpEngine:: GetHapticController()
	{
	return myHc->Cast();
	}
	void PHOpEngine::initialHapticController(ObjectIf* opObj)
	{
	myHc = new PHOpHapticController();
	myHc->IntialHapticController(opObj);
	}*/

	void PHOpEngine::InitialHapticRenderer(int objId)
	{
		opHpRender = new PHOpHapticRenderer();
		myHc = new PHOpHapticController();
		myHc->InitialHapticController(opObjs[objId]->Cast());
		opHpRender->initial6DOFRenderer(myHc, &opObjs);

		myHc->hcType = PHOpHapticControllerDesc::_6DOF;

		//set c_obstacle
		myHc->c_obstRadius = opObjs[objId]->objAverRadius / 6;
	}
	void  PHOpEngine::SetIterationCount(int count)
	{
		opIterationTime = count;
	}
	int  PHOpEngine::GetIterationCount()
	{
		return opIterationTime;
	}
	ObjectIf*  PHOpEngine::GetOpAnimator()
	{
		return opAnimator->Cast();
	}
	ObjectIf*  PHOpEngine::GetOpHapticController()
	{
		return myHc->Cast();
	}
	ObjectIf* PHOpEngine::GetOpHapticRenderer()
	{
		return opHpRender->Cast();
	}
	bool PHOpEngine::IsHapticSolve()
	{
		return subStepProSolve;
	}
	void  PHOpEngine::SetDrawPtclR(float r)
	{
		radiusCoe = r;
	}
	float  PHOpEngine::GetDrawPtclR()
	{
		return radiusCoe;
	}
	void PHOpEngine::SetHapticSolveEnable(bool enable)
	{
		subStepProSolve = enable;
	}
	bool PHOpEngine::IsProxyCorrection()
	{
		return subStepProFix;
	}
	void PHOpEngine::SetProxyCorrectionEnable(bool enable)
	{
		subStepProFix = enable;
	}

	void PHOpEngine::SetGravity(bool gflag)
	{
		for (int obji = 0; obji < (int)opObjs.size(); obji++)
		{
			opObjs[obji]->gravityOn = gflag;
		}
	}
	void PHOpEngine::StepWithBlend()
	{
		PHOpEngine::Step();
		for (int obji = 0; obji < (int)opObjs.size(); obji++)
		{
			opObjs[obji]->vertexBlending();
		}
	}
	void PHOpEngine::SetUpdateNormal(bool flag)
	{
		for (int obji = 0; obji < (int)opObjs.size(); obji++)
		{
			opObjs[obji]->updateNormals = flag;
		}
	}
	bool PHOpEngine::IsUpdateNormal(int obji)
	{
		return opObjs[obji]->updateNormals;
	}
	void PHOpEngine::SetAnimationFlag(bool flag)
	{
		useAnime = flag;
	}
	bool PHOpEngine::GetAnimationFlag()
	{
		return useAnime;
	}
	void PHOpEngine::Step(){
		
		PHSceneIf* phs = (PHSceneIf*)GetScene();
		PHOpSpHashColliAgentIf* agent = phs->GetOpColliAgent();

		for (int obji = 0; obji < (int)opObjs.size(); obji++)
		{
			if (opObjs[obji]->objNoMeshObj)
				continue;

			//predict step
			if (useFriction) opObjs[obji]->positionPredictFriction(myHc);
			else opObjs[obji]->positionPredict();
		}

		//haptic sych
		if (useHaptic)
		{

			myHc->userPose.Pos() = winPose * myHc->userPose.Pos();
			myHc->userPos = winPose *  myHc->userPos;
			//
			if (myHc->hcType == PHOpHapticControllerDesc::_6DOF)
			{
				if (!opHpRender->IsRigid())
				{
					for (int pi = 0; pi < opObjs[myHc->GetHpObjIndex()]->assPsNum; pi++)
					{
						PHOpParticle &dp = opObjs[myHc->GetHpObjIndex()]->objPArr[pi];
						Vec3f diff = (myHc->userPose *dp.pOrigCtr);//userPos

						dp.pNewCtr += (diff - dp.pNewCtr)* opHpRender->constraintSpring;

					}
				}
				else{
					//for rigid
					for (int pi = 0; pi < opObjs[myHc->GetHpObjIndex()]->assPsNum; pi++)
					{
						PHOpParticle &dp = opObjs[myHc->GetHpObjIndex()]->objPArr[pi];

						dp.pNewCtr = myHc->userPose *dp.pOrigCtr;
						dp.pNewOrint = myHc->userPose.Ori().Inv() * dp.pOrigOrint;
						//dp.pCurrOrint = dp.pNewOrint;
						dp.pCurrCtr = dp.pNewCtr;
					}
				}
			}

		}

		//animation
		if (useAnime)
			opAnimator->AnimationStep(this);


		//collision
		if (agent->IsCollisionEnabled())
		{
		agent->OpCollisionProcedure();

		}
		

		for (int itri = 0; itri < opIterationTime; itri++)//iteration default is 1
		{


			for (int obji = 0; obji < (int)opObjs.size(); obji++)
			{
				if (opObjs[obji]->objNoMeshObj)
					continue;

				//deform
				opObjs[obji]->groupStep();
			}

			//haptic
			if (useHaptic)
			{
				if (myHc->hcType == PHOpHapticControllerDesc::_3DOF)
				{//3DOF

					if (myHc->hcReady)
						HapticProcedure_3DOF();
					else DSTR << "Haptic Device is not ready" << std::endl;
				}
				else
				{//6DOF

					if (myHc->hcReady)
						HapticProcedure_6DOF();
					else DSTR << "Haptic Device is not ready" << std::endl;
				}


			}

		}

		if (useAnime)
			opAnimator->AnimationIntergration(this);

		//intergration
		for (int obji = 0; obji < (int)opObjs.size(); obji++)
		{
			if (opObjs[obji]->objNoMeshObj)
				continue;

			if (useHaptic)
			{
				if (!myHc->hcType == PHOpHapticControllerDesc::_3DOF)
				{//6DOF

					for (int pi = 0; pi < opObjs[myHc->GetHpObjIndex()]->assPsNum; pi++)
					{
						PHOpParticle &dp = opObjs[myHc->GetHpObjIndex()]->objPArr[pi];

						if (opHpRender->IsRigid())
							dp.pNewCtr = dp.pCurrCtr;
					}
				}
			}

			//if (!opObjs[obji]->isRigid)
			{
				opObjs[obji]->integrationStep();
			}

		}
	}
	TQuaternion<float> PHOpEngine::GetCurrentCameraOrientation()
	{
		return winPose;
	}
	void PHOpEngine::SetCurrentCameraOritation(TQuaternion<float> orit)
	{
		winPose = orit;
	}


	void PHOpEngine::HapticProcedure_6DOF()
	{

		Vec3f diffAcc;

		for (int pi = 0; pi < opObjs[myHc->GetHpObjIndex()]->assPsNum; pi++)
		{
			PHOpParticle &dp = opObjs[myHc->GetHpObjIndex()]->objPArr[pi];
			Vec3f diff = (myHc->userPose *dp.pOrigCtr);//userPos

			diffAcc += -(diff - dp.pNewCtr);

		}
		
		diffAcc /= opObjs[myHc->GetHpObjIndex()]->assPsNum;


		Vec3f f = winPose *diffAcc * opHpRender->outForceSpring;
	
		if (myHc->SetForce(f))
		{
			DSTR << "Set Force failed" << std::endl;
		}
	}
	void PHOpEngine::HapticProcedure_3DOF()
	{
#ifdef USEGRMESH
		;
		PHOpParticle* dp = myHc->GetMyHpProxyParticle();
		if (!myHc->CheckProxyState())
		{
			dp->pNewCtr = myHc->userPos;

			
			if (subStepProFix){
				//触っていない時のproxyfix
				noCtcItrNum = 0;
				opHpRender->HpNoCtcProxyCorrection();
				
			}

		}
		else {
			
			if (subStepProFix){
				//触っているときまずtraceする
				opHpRender->ProxyTrace();
				
				//Proxy位置の修正(関数名は修正まち)

				opHpRender->ProxyMove();
			}

		}
#ifdef CHECK_INF_ERR
		if (!FloatErrorTest::CheckBadFloatValue(dp->pNewCtr.z))
			int u = 0;
#endif
		if (subStepProSolve)
		{

			//haptic解決
			opHpRender->HpConstrainSolve(dp->pCurrCtr);

		}
#ifdef CHECK_INF_ERR
		if (!FloatErrorTest::CheckBadFloatValue(dp->pNewCtr.z))
			int u = 0;
#endif
		//haptic移動速度計算する
		PHOpObj *dpobj = opObjs[0];
		float timeStep = dpobj->params.timeStep;

		dp->pVelocity = (myHc->userPos - dp->pNewCtr) / timeStep;

		//平面とのhaptic
		opHpRender->ProxySlvPlane();


		//力の計算
		opHpRender->ForceCalculation();

		dp->pCurrCtr = dp->pNewCtr;
		myHc->hcCollied = false;


		Vec3f f;
		if (myHc->hcProxyOn || true == opHpRender->hitWall)
		{
			winPose = winPose.Inv();
			f = winPose * (dp->pCurrCtr - myHc->userPos) *opHpRender->outForceSpring;
			float magni = f.norm();
			if (magni > 10.0f)
			{
				DSTR << "Big Force Output!" << std::endl;
				;// f.clear();
			}
			DSTR << "f= " << f.x << " , " << f.y << " , " << f.z << std::endl;
		}
		else {
			f.clear();
		}

		if (opHpRender->hitWall)
			f = f * 3;
		myHc->SetForce(f);
#endif
	}

	void PHOpEngine::SetTimeStep(double dt){
		fdt = dt;
		for (int obji = 0; opObjs.size(); obji++)
		{
			opObjs[obji]->params.timeStep = fdt;
		}
	}

	double PHOpEngine::GetTimeStep(){
		return fdt;
	}
	int PHOpEngine::AddOpObj()
	{
		PHOpObj* opObj = new PHOpObj();
		opObj->objId = objidIndex;
		objidIndex++;
		opObjs.push_back(opObj);

		return  opObj->objId;
	}
	PHOpObjIf* PHOpEngine::GetOpObjIf(int obji)
	{
		return  opObjs[obji]->Cast();
	}
	bool PHOpEngine::AddChildObject(ObjectIf* o){
		PHOpObj* opObj = o->Cast();
		if (opObj){

			opObj->params.timeStep = fdt;
			opObj->objId = objidIndex;
			this->opObjs.push_back(opObj);
			objidIndex++;
			return true;
		}

		return false;
	}
	bool PHOpEngine::DelChildObject(ObjectIf* o)
	{//not test yet

		PHOpObj* opObj = o->Cast();
		if (opObj){
			//this->opObjs.push_back(opObj);
			std::vector<PHOpObj*>::iterator it;
			it = opObjs.begin();
			for (int obji = 0; obji < (int)opObjs.size(); obji++)
			{
				if (opObjs[obji]->objId == opObj->objId)
				{
					//opObjs.
					it = opObjs.erase(it);
					return true;
				}
				else{
					++it;
				}
			}


		}

		return false;
	}

}


#include <Physics\PHOpEngine.h>
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

		useHaptic = false;
	}
	void PHOpEngine::InitialNoMeshHapticRenderer()
	{
		opHpRender = new PHOpHapticRenderer();
		myHc = new PHOpHapticController();
		myHc->InitialHapticController();
		myHc->hpObjIndex = -1;
		//opObjs.push_back(myHc->hcObj);

		myHc->hcType = PHOpHapticControllerDesc::_3DOF;
		
		opHpRender->initial3DOFRenderer(myHc, &opObjs);

		//set defualt c_obstacle
		myHc->c_obstRadius = 0.2f;// opObjs[objId]->objAverRadius / 6;
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
	ObjectIf*  PHOpEngine::GetOpHapticController()
	{
		return myHc->Cast();
	}
	ObjectIf* PHOpEngine::GetOpHapticRenderer()
	{
		return opHpRender->Cast();
	}

	bool PHOpEngine::IsHapticEnabled()
	{
		return useHaptic;
	}
	bool PHOpEngine::TrySetHapticEnable(bool enable)
	{
		if (myHc->hcReady)
		{
			useHaptic = enable;
			return true;
		}
		else{
			myHc->initDevice();
			if (myHc->hcReady)
			{
				useHaptic = enable;
				return true;
			}
			else return false;
		}
	}
	bool PHOpEngine::IsHapticSolve()
	{
		return subStepProSolve;
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
		for (int obji = 0; obji < opObjs.size(); obji++)
		{
			opObjs[obji]->vertexBlending();
		}
	}
	void PHOpEngine::SetUpdateNormal(bool flag)
	{
		for (int obji = 0; obji < opObjs.size(); obji++)
		{
			opObjs[obji]->updateNormals = flag;
		}
	}
	bool PHOpEngine::IsUpdateNormal(int obji)
	{
		return opObjs[obji]->updateNormals;
	}
	void PHOpEngine::Step(){
		/*for (int obji = 0; obji < (int) opObjs.size(); obji++)
				opObjs[obji]->SimpleSimulationStep();*/

		PHSceneIf* phs = (PHSceneIf*)GetScene();
		PHOpSpHashColliAgentIf* agent = phs->GetOpColliAgent();

		for (int obji = 0; obji < (int)opObjs.size(); obji++)
		{
			if (opObjs[obji]->objNoMeshObj)
				continue;

			//predict step
			opObjs[obji]->positionPredict();

			//haptic sych
			if (useHaptic)
			{
		
				myHc->currSpg->Update(0.001f);
				Vec3f &spgpos = myHc->currSpg->GetPosition();
				myHc->userPose = myHc->currSpg->GetPose();
				myHc->userPose.Pos() = winPose * myHc->userPose.Pos()* myHc->posScale;
				
				myHc->userPos = winPose *  spgpos * myHc->posScale;
			}

			//collision
			if (agent->IsCollisionEnabled())
			{
				agent->OpCollisionProcedure();

			}

			for (int itri = 0; itri < opIterationTime; itri++)//iteration default is 1
			{
				//deform
				opObjs[obji]->groupStep();


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

			opObjs[obji]->integrationStep();
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

	//void PHOpEngine::SetCrossPlatformCoord(bool InverX, bool InverY, bool InverZ)
	//{
	//	myHc->SetCrossPlatformCoord(InverX, InverY, InverZ);
	//}

	void PHOpEngine::HapticProcedure_6DOF()
	{
		
		Vec3f diffAcc;

		for (int pi = 0; pi < opObjs[myHc->GetHpObjIndex()]->assPsNum; pi++)
		{
			PHOpParticle &dp = opObjs[myHc->GetHpObjIndex()]->objPArr[pi];
			TPose<float> nowPose = TPose<float>(dp.pCurrCtr, dp.pCurrOrint);
			Vec3f diff = (myHc->userPose *dp.pOrigCtr);//userPos
			diffAcc += -(diff - dp.pNewCtr);
			dp.pNewCtr = dp.pNewCtr + (diff - dp.pNewCtr) * opHpRender->constraintSpring;
			//dp.pColliedForce += (diff - dp.pNewCtr)* constraintSpring;
		}
		//myHc->positionPredict();
		diffAcc /= opObjs[myHc->GetHpObjIndex()]->assPsNum;


		Vec3f f = winPose *diffAcc * opHpRender->forceSpring;
		//float magni = f.norm();
		//if (magni > 10.0f)
		//{
		//	DSTR << "Big Force Output!" << std::endl;
		//	;// f.clear();
		//}
		myHc->currSpg->SetForce(f, Vec3f());
	}
	void PHOpEngine::HapticProcedure_3DOF()
	{
		
		;
		PHOpParticle* dp = myHc->GetMyHpProxyParticle();
		if (!myHc->CheckProxyState())
		{
			dp->pNewCtr = myHc->userPos;

			//dp->pNewOrint = myHc->currSpg->GetPose().Ori().Inv();
			if (subStepProFix){
				//G‚Á‚Ä‚¢‚È‚¢Žž‚Ìproxyfix
				noCtcItrNum = 0;
				opHpRender->HpNoCtcProxyCorrection();
				// cout << "noCtcItrNum = " << noCtcItrNum << endl;
			}

		}
		else {
			//dp->pNewCtr = dp->pCurrCtr;

			//dp->pVelocity = (myHc->userPos - dp->pCurrCtr) / myHc->params.timeStep;
			if (subStepProFix){
				//G‚Á‚Ä‚¢‚é‚Æ‚«‚Ü‚¸trace‚·‚é
				opHpRender->ProxyTrace();
				// HpProxyPosFix();
				//ProxyˆÊ’u‚ÌC³(ŠÖ”–¼‚ÍC³‚Ü‚¿)

				// if (!ProxyCorrection())
				;// return;
				opHpRender->ProxyMove();
			}

		}
#ifdef CHECK_INF_ERR
		if (!FloatErrorTest::CheckBadFloatValue(dp->pNewCtr.z))
			int u = 0;
#endif
		if (subStepProSolve)
		{
			
			//haptic‰ðŒˆ
			opHpRender->HpConstrainSolve(dp->pCurrCtr);
			//HpConstrainSolve();

		}
#ifdef CHECK_INF_ERR
		if (!FloatErrorTest::CheckBadFloatValue(dp->pNewCtr.z))
			int u = 0;
#endif
		//hapticˆÚ“®‘¬“xŒvŽZ‚·‚é
		PHOpObj *dpobj = opObjs[0];
		float timeStep = dpobj->params.timeStep;

		dp->pVelocity = (myHc->userPos - dp->pNewCtr) /timeStep;

		//•½–Ê‚Æ‚Ìhaptic
		opHpRender->ProxySlvPlane();


		//—Í‚ÌŒvŽZ
		opHpRender->ForceCalculation();

		dp->pCurrCtr = dp->pNewCtr;
		//TQuaternion<float> winPose = GetCurrentWin()->GetTrackball()->GetPose().Ori();
		myHc->hcCollied = false;


		Vec3f f;
		if (myHc->hcProxyOn || true == opHpRender->hitWall)
		{
			winPose = winPose.Inv();
			//if (!useConstrainForce)
			f = winPose * (dp->pCurrCtr - myHc->userPos) *opHpRender->forceSpring;
			//else 
			//	f = winPose * (dp->pCurrCtr - myHc->userPos) / myHc->posScale *constraintSpring;
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
		myHc->currSpg->SetForce(f, Vec3f());
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
			for (int obji = 0; obji < (int) opObjs.size(); obji++)
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
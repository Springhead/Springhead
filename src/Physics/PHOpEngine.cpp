
#include <Physics\PHOpEngine.h>
#include "PHOpDecompositionMethods.h"


namespace Spr{
	;

	PHOpEngine::PHOpEngine(){
		fdt = 0.01;
		objidIndex = 0;
		radiusCoe = 0.5;
		opIterationTime = 1;
	}
	void PHOpEngine:: SetGravity(bool gflag)
	{
		for (int obji = 0; obji < (int) opObjs.size(); obji++)
		{
			opObjs[obji]->gravityOn = gflag;
		}
	}
	void PHOpEngine::Step(){
		/*for (int obji = 0; obji < (int) opObjs.size(); obji++)
				opObjs[obji]->SimpleSimulationStep();*/

		PHSceneIf* phs = (PHSceneIf*)GetScene();
		PHOpSpHashColliAgentIf* agent = phs->GetOpColliAgent();

		for (int obji = 0; obji < (int)opObjs.size(); obji++)
		{
			opObjs[obji]->positionPredict();
			

			if (agent->CollisionEnabled())
			{
				agent->OpCollisionProcedure();
			
			}
			for (int itri = 0; itri < opIterationTime; itri++)//iteration default is 1
			{
				opObjs[obji]->groupStep();


			}

			opObjs[obji]->integrationStep();
		}
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
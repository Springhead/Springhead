#ifndef PHOPHAPTICRENDERER_H
#define PHOPHAPTICRENDERER_H

#include <Foundation\Object.h>
#include <Physics\PHOpHapticController.h>
#include <Collision\CDCollisionFunctions.h>

namespace Spr
{;

class PHOpHapticRenderer :public SceneObject, public PHOpHapticRendererDesc
	{
	SPR_OBJECTDEF(PHOpHapticRenderer);
	public :
		
		
		PHOpHapticController* myHc;
		std::vector<Vec3f> ctcPPosList;
		std::vector<Vec3f> ctcPNorList;

		std::vector<PHOpObj*>* opObjs;
		
		std::vector<std::map<int, std::vector<int> > > objEdgeInfos;
		std::vector<std::map<int, std::vector<int> > > objVtoFInfos;

		struct proxySolveItror
		{
			std::vector<PHOpHapticController::ConstrainPlaneInfo*> constrainIs;
		public:
			//static solution
			/*  void proxuSolveItror()
			{
			constrainIs.push_back(HapticController::ConstrainPlaneInfo());
			constrainIs.push_back(HapticController::ConstrainPlaneInfo());
			constrainIs.push_back(HapticController::ConstrainPlaneInfo());
			}*/
		}slvItr;

		PHOpHapticRenderer()
		//PHOpHapticRenderer(const PHOpHapticRendererDesc& desc = PHOpHapticRendererDesc(), SceneIf* s = NULL)
		{
			hitWall = false;
			forceSpring = 1.0f;
			constraintSpring = 1.0f;
			useIncreaseMeshMove = false;
			useConstrainForce = false;
			useProxyfixSubitr = false;
			useDualTranFix = false;
			timeStep = 0.01f;
			currDtType = -1;
			noCtcItrNum = 0;
			proxyItrtNum = 0;
			proxyItrtMaxNum = 3;
		}

		void initialRenderer(PHOpHapticController* hc, std::vector<PHOpObj*>* objs)
		{
			myHc = hc;
			opObjs = objs;
			forceOnRadius = 0.3f;
			objNum = (int)(*opObjs).size();
			BuildVToFaceRelation();
			BuildEdgeInfo();
		}

		void ProxySlvPlane();
		void ProxyMove();
		void ProxyTrace();
		bool ProxyCorrection();
		void ForceCalculation();
		void setForceOnRadius(float r)
		{
			forceOnRadius = r;
		}

		//void HpProxyPosFix();
		//void HpConstraint();
		void HpNoCtcProxyCorrection();
		void HpConstrainSolve(Vec3f &currSubStart);
		void BuildVToFaceRelation();
		void BuildEdgeInfo();
		bool intersectOnRoute(Vec3f currSubStart, Vec3f currSubGoal, Vec3f &newSubStart, Vec3f &newSubGoal, PHOpHapticController::ConstrainPlaneInfo &cpinfo, bool inverseF);
		bool intersectOnRouteInColliPs(Vec3f currSubStart, Vec3f currSubGoal, Vec3f &newSubStart, Vec3f &newSubGoal, PHOpHapticController::ConstrainPlaneInfo &cpinfo, bool inverseF);
	};
}

#endif
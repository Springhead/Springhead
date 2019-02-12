/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef PHOPHAPTICRENDERER_H
#define PHOPHAPTICRENDERER_H

#include <Foundation/Object.h>
#include <Physics/PHOpHapticController.h>
#include <Collision/CDCollisionFunctions.h>
#include <map>

namespace Spr
{;

class PHOpHapticRenderer :public SceneObject, public PHOpHapticRendererDesc
	{
	SPR_OBJECTDEF(PHOpHapticRenderer);
	public :
		ACCESS_DESC(PHOpHapticRenderer);
		
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
		
		}slvItr;

		PHOpHapticRenderer(const PHOpHapticRendererDesc& desc = PHOpHapticRendererDesc()) : PHOpHapticRendererDesc(desc)
		
		{
			hitWall = false;
			rigid = false;
			outForceSpring = 1.0f;
			extForceSpring = 1.0f;
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
		void SetRigid(bool set)
		{
			rigid = set;
			if (rigid)
			{
				if (myHc != NULL)
					myHc->hcObj->isRigid = rigid;
			}
		}
		bool IsRigid()
		{
			return rigid;
		}
#ifdef USEGRMESH
		void initial3DOFRenderer(PHOpHapticController* hc, std::vector<PHOpObj*>* objs)
		{
			myHc = hc;
			opObjs = objs;
			forceOnRadius = 0.3f;
			objNum = (int)(*opObjs).size();
			BuildVToFaceRelation();
			BuildEdgeInfo();
		}
#endif
		void initial6DOFRenderer(PHOpHapticController* hc, std::vector<PHOpObj*>* objs)
		{
			myHc = hc;
			opObjs = objs;
			forceOnRadius = 0.3f;
			objNum = (int)(*opObjs).size();
			
		}
#ifdef USEGRMESH
		void ProxySlvPlane();
		void ProxyMove();
		void ProxyTrace();
		bool ProxyCorrection();
		void HpNoCtcProxyCorrection();
		void HpConstrainSolve(Vec3f &currSubStart);
		void BuildVToFaceRelation();
		void BuildEdgeInfo();
		bool intersectOnRoute(Vec3f currSubStart, Vec3f currSubGoal, Vec3f &newSubStart, Vec3f &newSubGoal, PHOpHapticController::ConstrainPlaneInfo &cpinfo, bool inverseF);
		bool intersectOnRouteInColliPs(Vec3f currSubStart, Vec3f currSubGoal, Vec3f &newSubStart, Vec3f &newSubGoal, PHOpHapticController::ConstrainPlaneInfo &cpinfo, bool inverseF);
#endif
		void setForceOnRadius(float r)
		{
			forceOnRadius = r;
		}
		void SetForceSpring(float k)
		{
			extForceSpring = k;
		}
		float GetForceSpring()
		{
			return extForceSpring;
		}
		void SetConstraintSpring(float k)
		{
			constraintSpring = k;
		}
		float GetConstraintSrping()
		{
			return constraintSpring;
		}
		
	};
}

#endif

/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef PHOPHAPTICCONTROLLER_H
#define PHOPHAPTICCONTROLLER_H

#include <Foundation/Object.h>
#include <Physics/PHOpObj.h>
#include <sstream>

namespace Spr {
	;
	class PHOpObj;

	class PHOpHapticController: public SceneObject, public PHOpHapticControllerDesc
	{
	public:
		PHOpObj* hcObj;
		PHOpHapticController(const PHOpHapticControllerDesc& desc = PHOpHapticControllerDesc()): PHOpHapticControllerDesc(desc)
		{
			max_output_force = 6.0f;
			IsSetForceReady = false;
		}
		~PHOpHapticController()
		{
			if (logForceFile) fclose(logForceFile);
			if (logPosFile) fclose(logPosFile);
			if (logPPosFile) fclose(logPPosFile);
		}

		SPR_OBJECTDEF(PHOpHapticController);
		ACCESS_DESC(PHOpHapticController);

		
		std::vector<PvsHcCtcPInfo> hcColliedPs;
		std::vector<int> suspFaceIndex;
		SpatialVector outForce;
		
		FILE *logForceFile;//Force
		FILE *logPPosFile;//proxy pos
		FILE *logPosFile;//particle pos
		FILE *logUPosFile;//user pos
		float max_output_force;
		std::vector<int> surrCnstrList;

		struct ConstrainPlaneInfo{

			bool operator = (const ConstrainPlaneInfo &a)
			{
				this->cstType = a.cstType;
				this->edgeidA = a.edgeidA;
				this->edgeidB = a.edgeidB;
				this->objid = a.objid;
				this->planeid = a.planeid;
				this->planeN = a.planeN;
				this->planeP = a.planeP;
				this->vid = a.vid;
				this->fu = a.fu;
				this->fv = a.fv;
				this->fw = a.fw;
				this->segmentt = a.segmentt;
				this->routet = a.routet;
				return true;


			}
			int vid;
			int edgeidA;
			int edgeidB;
			int planeid;
			int objid;
			cstrainType cstType;
			Vec3f planeN;
			Vec3f planeP;
			Vec3f goalP;
			float fv, fu, fw, segmentt, routet;
		};
		
		ConstrainPlaneInfo hcBindCpi;
		
		std::vector<ConstrainPlaneInfo> cpiVec;
		std::vector<ConstrainPlaneInfo> cpiHpVec;
		std::vector<ConstrainPlaneInfo> cpiLastHpVec;
		std::vector<int> sameIdArr;
		std::vector<int> sameLIdArr;
		

		//for new HC
		
		SpatialVector GetHCOutput();


	
		
		bool InitialHapticController(PHOpObj* opObjectif);
		bool InitialHapticController();
		
		void LogForce(TQuaternion<float> winPose);
		void UpdateProxyPosition(Vec3f &pos, TQuaternion<float> winPose);
		bool CheckProxyState();
		void SetHCColliedFlag(bool flag);
		
		void AddColliedPtcl(int pIndex, int objindex, Vec3f ctcPos);
		PHOpParticle* GetMyHpProxyParticle();
		bool BeginLogForce();
		void EndLogForce();
		void ClearColliedPs();
		void setC_ObstacleRadius(float r);
		int GetHpObjIndex();
		ObjectIf* GetHpOpObj();
		bool SetForce(Vec3f f);
		
		void SetHCForceReady(bool flag);
		bool GetHCForceReady();
		void SetHCReady(bool flag);
		bool GetHCReady();
		Vec3f GetHCPosition();
		void SetHCPosition(Vec3f pos);
		void SetHCPose(Posef pose);
		Posef GetHCPose();

		
		Vec3f GetUserPos()
		{
			return userPos;
		}
		float GetC_ObstacleRadius()
		{
			return c_obstRadius;
		}
		Vec3f GetCurrentOutputForce();
		
		
	};

}//namespace
#endif

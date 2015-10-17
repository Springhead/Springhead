#ifndef PHOPHAPTICCONTROLLER_H
#define PHOPHAPTICCONTROLLER_H

#include <Foundation/Object.h>
#include <HumanInterface\SprHISpidar.h>
#include <Physics\PHOpObj.h>
#include <sstream>
#include "HumanInterface\SprHIDRUsb.h"
#include "HumanInterface\SprHIKeyMouse.h"
#include "HumanInterface\SprHISdk.h"
//#include "Physics\PHOpEngine.h"

namespace Spr {
	;


	class PHOpHapticController: public SceneObject, public PHOpHapticControllerDesc
	{
		
		PHOpObj* hcObj;
		//Matrix3f crossPlatformTransform;
	public:
		PHOpHapticController(const PHOpHapticControllerDesc& desc = PHOpHapticControllerDesc()): PHOpHapticControllerDesc(desc)
		{
			
		}
		PHOpHapticController::~PHOpHapticController()
		{
			if (logForceFile) fclose(logForceFile);
			if (logPosFile) fclose(logPosFile);
			if (logPPosFile) fclose(logPPosFile);
		}

	
		SPR_OBJECTDEF(PHOpHapticController);
		ACCESS_DESC(PHOpHapticController);

		UTRef<HISpidarGIf> currSpg;
		UTRef<HISdkIf> hiSdk;
		

		std::vector<PvsHcCtcPInfo> hcColliedPs;
		//std::vector<PvsHcCtcPInfo> segTestPs;//Ptcl of segment test in pre-ctc detection
		std::vector<int> suspFaceIndex;
		
		FILE *logForceFile;//Force
		FILE *logPPosFile;//proxy pos
		FILE *logPosFile;//particle pos
		FILE *logUPosFile;//user pos
		
		

		//proxyfix
		
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
		//CtcConstrain intsctInfo;
		ConstrainPlaneInfo hcBindCpi;
		//std::vector<ConstrainPlaneInfo> bindElements;
		std::vector<ConstrainPlaneInfo> cpiVec;
		std::vector<ConstrainPlaneInfo> cpiHpVec;
		std::vector<ConstrainPlaneInfo> cpiLastHpVec;
		std::vector<int> sameIdArr;
		std::vector<int> sameLIdArr;
		//HapticController(int id) :PHOpObj(id)

		
		/*void vertexBlending()
		{
		targetVts[0]= objPArr[0].pCurrCtr;
		}*/
		bool doCalibration();
		bool  initDevice();
		bool InitialHapticController(PHOpObj* opObjectif);
		bool InitialHapticController();
		//void UpdateHapticPosition(Vec3f &pos);

		void LogForce(TQuaternion<float> winPose);
		void UpdateProxyPosition(Vec3f &pos, TQuaternion<float> winPose);
		bool CheckProxyState();
		void SetHcColliedFlag(bool flag);
		//void BindCtcPlane(ConstrainPlaneInfo cif);
		void AddColliedPtcl(int pIndex, int objindex, Vec3f ctcPos);
		PHOpParticle* GetMyHpProxyParticle();
		bool BeginLogForce();
		void EndLogForce();
		void ClearColliedPs();
		void setC_ObstacleRadius(float r);
		int GetHpObjIndex();
		void BuildVToFaceRelation();
		ObjectIf* GetHpOpObj();

		/*void SetCrossPlatformCoord(bool InverX, bool InverY, bool InverZ)
		{
			crossPlatformTransform.xx = 1;
			crossPlatformTransform.yy = 1;
			crossPlatformTransform.zz = 1;
			if (InverX)
			{
				crossPlatformTransform.xx = -1;
			}
			if (InverY)
			{
				crossPlatformTransform.yy = -1;
			}
			if (InverZ)
			{
				crossPlatformTransform.zz = -1;
			}
		}*/
		Vec3f GetUserPos()
		{
			return userPos;
		}
		float GetC_ObstacleRadius()
		{
			return c_obstRadius;
		}
		Vec3f GetCurrentOutputForce();
		/*void AddtoOpEngine()
		{
			PHSceneIf* scene = (PHSceneIf*)GetScene();
			PHOpEngineIf* opengIf = scene->GetOpEngine()->Cast();
			PHOpEngine* opEngine = DCAST(PHOpEngine, opengIf);
			opEngine->opObjs.push_back(this);

		}*/
		
	};

}//namespace
#endif
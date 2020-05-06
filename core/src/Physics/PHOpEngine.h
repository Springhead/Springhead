#ifndef PHOPENGINE_H
#define PHOPENGINE_H


#include <SprDefs.h>
//#include <GL/glew.h>
//#include <GL/glut.h>
  #undef CreateDialog
  #undef CreateFile
  #undef GetObject
  #undef min
  #undef max
// end
#ifdef USEGRMESH
#include "Graphics/GRDrawEllipsoid.h"
#endif
#include <Physics/PHOpDecompositionMethods.h>
#include "Physics/PHOpHapticController.h"
#include "Physics/PHEngine.h"
#include "Physics/PHOpObj.h"
#include "Physics/PHOpHapticRenderer.h"
#include "Physics/PHOpAnimation.h"


namespace Spr{
	;

	class PHOpEngine : public PHEngine
	{
	public:
		SPR_OBJECTDEF(PHOpEngine);
		//時間微分
		double fdt;
		//opオブジェクト配列
		std::vector<PHOpObj*> opObjs;
		//ObjIdの計数
		int objidIndex;
		//描画する粒子の半径係数
		float radiusCoe;
#ifdef USEGRMESH
		//楕円描画する用
		DrawEllipsoid drawEll;

#endif
		UTRef<PHOpAnimation> opAnimator;	//	To avoid memory leak

		int opIterationTime;

		TQuaternion<float> winPose;

		bool subStepProFix;
		bool subStepProSolve;
		bool logForce;
		bool useAnime;

		//proxy
		int noCtcItrNum;
		PHOpHapticRenderer* opHpRender;
		PHOpHapticController *myHc;
		bool useHaptic;


		PHOpEngine();

		int  GetPriority() const {
			return SGBP_NONE;
			//return SGBP_FORCEGENERATOR;
		}//SGBP_DYNAMICALSYSTEM;}
		void SetIterationCount(int count);
		int GetIterationCount();
		void Step();

		void SetGravity(bool gflag);
		//void SetCrossPlatformCoord(bool InverX, bool InverY, bool InverZ);
		virtual bool AddChildObject(ObjectIf* o);
		virtual bool DelChildObject(ObjectIf* o);
		void SetTimeStep(double dt);
		double GetTimeStep();
		//void initialHapticController(ObjectIf*);
		//ObjectIf* GetHapticController();
		int GetOpObjNum(){ return(int)opObjs.size(); }
		PHOpObjDesc* GetOpObj(int i);
		void StepWithBlend();
		int  AddOpObj();
		PHOpObjIf* GetOpObjIf(int obji);
		void HapticProcedure_3DOF();
		void HapticProcedure_6DOF();
		void InitialHapticRenderer(int objId);
		void SetUseHaptic(bool hapticUsage);
		bool GetUseHaptic();
		
		void SetHapticSolveEnable(bool enable);
		bool IsHapticSolve();
		void SetProxyCorrectionEnable(bool enable);
		bool IsProxyCorrection();
		void InitialNoMeshHapticRenderer();

		ObjectIf* GetOpHapticController();
		ObjectIf* GetOpHapticRenderer();
		ObjectIf* GetOpAnimator();
		void SetUpdateNormal(bool flag);
		bool IsUpdateNormal(int obji);
		void SetAnimationFlag(bool flag);
		bool GetAnimationFlag();
		void SetDrawPtclR(float r);
		float GetDrawPtclR();

		void SetCurrentCameraOritation(TQuaternion<float> orit);
		TQuaternion<float> GetCurrentCameraOrientation();
	};

}
#endif

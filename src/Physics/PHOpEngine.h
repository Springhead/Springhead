#ifndef PHOPENGINE_H
#define PHOPENGINE_H


#include <SprDefs.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <Physics\PHOpDecompositionMethods.h>
#include "Physics\PHOpHapticController.h"
#include "Physics\PHEngine.h"
#include "Physics\PHOpObj.h"
#include "Physics\PHOpHapticRenderer.h"
#include "Physics\PHOpAnimation.h"


namespace Spr{
	;
	class DrawEllipsoid
	{
	public:
		float pi;


		DrawEllipsoid()
		{
			pi = 3.141592653f;
		}

		int getPoint2(float rx, float ry, float rz, float a, float b, Vec3f &p, TQuaternion<float> &tq)//TPose<float> &tp)  
		{
			p.x = (float)(rx*sin(a*pi / 180.0)*cos(b*pi / 180.0));
			p.y = (float)(ry*sin(a*pi / 180.0)*sin(b*pi / 180.0));
			p.z = (float)(rz*cos(a*pi / 180.0));
			p = tq * p;
			return 1;
		}


		Vec3f* getPointMatrix2(float rx, float ry, float rz, int slices, TQuaternion<float> &tq)//TPose<float> &tp)  
		{
			int i, j, w = 2 * slices, h = slices;
			float a = 0.0, b = 0.0;
			float hStep = (float)180.0 / (h - 1);
			float wStep = (float)360.0 / w;
			int length = w*h;
			Vec3f *matrix;
			matrix = (Vec3f *)malloc(length*sizeof(Vec3f));
			if (!matrix)return NULL;
			for (a = 0.0, i = 0; i<h; i++, a += hStep)
				for (b = 0.0, j = 0; j<w; j++, b += wStep)
					getPoint2(rx, ry, rz, a, b, matrix[i*w + j], tq);
			return matrix;
		}

		int getPoint2(float rx, float ry, float rz, float a, float b, Vec3f &p, Matrix3f dotcEM)
		{
			p.x = (float)(rx*sin(a*pi / 180.0)*cos(b*pi / 180.0));
			p.y = (float)(ry*sin(a*pi / 180.0)*sin(b*pi / 180.0));
			p.z = (float)(rz*cos(a*pi / 180.0));
			//p = dotcEM * p;
			//Vec3f tmp;
			//multi(tmp, dotcEM, p);
			MatrixExtension ma;
			p = ma.MatrixMultiVec3fRight(dotcEM, p);
			return 1;
		}
		Vec3f* getPointMatrix2(float rx, float ry, float rz, int slices, Matrix3f dotcEM)
		{
			int i, j, w = 2 * slices, h = slices;
			float a = 0.0, b = 0.0;
			float hStep = (float) 180.0 / (h - 1);
			float wStep = (float) 360.0 / w;
			int length = w*h;
			Vec3f *matrix;
			matrix = (Vec3f *)malloc(length*sizeof(Vec3f));
			if (!matrix)return NULL;
			for (a = 0.0, i = 0; i<h; i++, a += hStep)
				for (b = 0.0, j = 0; j<w; j++, b += wStep)
					getPoint2(rx, ry, rz, a, b, matrix[i*w + j], dotcEM);
			return matrix;
		}

		int drawOval(float rx, float ry, float rz, int slices, TQuaternion<float> &tq)//TPose<float> &tp)  
		{
			int i = 0, j = 0, w = 2 * slices, h = slices;
			Vec3f *mx;
			mx = getPointMatrix2(rx, ry, rz, slices, tq);
			if (!mx)return 0;
			for (; i<h - 1; i++)
			{
				for (j = 0; j<w - 1; j++)
				{
					drawSlice(mx[i*w + j], mx[i*w + j + 1], mx[(i + 1)*w + j + 1], mx[(i + 1)*w + j]);
				}
				drawSlice(mx[i*w + j], mx[i*w], mx[(i + 1)*w], mx[(i + 1)*w + j]);
			}
			free(mx);
			return 1;
		}

		int drawOval(float rx, float ry, float rz, int slices, Matrix3f dotcEM)
		{
			int i = 0, j = 0, w = 2 * slices, h = slices;
			Vec3f *mx;
			mx = getPointMatrix2(rx, ry, rz, slices, dotcEM);
			if (!mx)return 0;
			for (; i<h - 1; i++)
			{
				for (j = 0; j<w - 1; j++)
				{
					drawSlice(mx[i*w + j], mx[i*w + j + 1], mx[(i + 1)*w + j + 1], mx[(i + 1)*w + j]);
				}
				drawSlice(mx[i*w + j], mx[i*w], mx[(i + 1)*w], mx[(i + 1)*w + j]);
			}
			free(mx);
			return 1;
		}

		void drawSlice(Vec3f &p1, Vec3f &p2, Vec3f &p3, Vec3f &p4)
		{


			glBegin(GL_QUADS);

			glColor3f(1.0f, 1.0f, 0);
			glVertex3f(p4.x, p4.y, p4.z);
			glVertex3f(p3.x, p3.y, p3.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p1.x, p1.y, p1.z);



			glEnd();
		}




	};

	class PHOpEngine : public PHEngine
	{
	public:
		SPR_OBJECTDEF(PHOpEngine);
		//éûä‘î˜ï™
		double fdt;
		//opÉIÉuÉWÉFÉNÉgîzóÒ
		std::vector<PHOpObj*> opObjs;
		//ObjIdÇÃåvêî
		int objidIndex;
		//ï`âÊÇ∑ÇÈó±éqÇÃîºåaåWêî
		float radiusCoe;
		//ë»â~ï`âÊÇ∑ÇÈóp
		DrawEllipsoid drawEll;
		PHOpAnimation* opAnimator;

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
		void InitialHapticRenderer(int objId, HISdkIf* hisdk);
		bool TrySetHapticEnable(bool enable, HISdkIf* hisdk);
		bool IsHapticEnabled();
		void SetHapticSolveEnable(bool enable);
		bool IsHapticSolve();
		void SetProxyCorrectionEnable(bool enable);
		bool IsProxyCorrection();
		void InitialNoMeshHapticRenderer(HISdkIf* hisdk);

		ObjectIf* GetOpHapticController();
		ObjectIf* GetOpHapticRenderer();
		ObjectIf* GetOpAnimator();
		void SetUpdateNormal(bool flag);
		bool IsUpdateNormal(int obji);
		void SetAnimationFlag(bool flag);
		bool GetAnimationFlag();
		void SetDrawPtclR(float r);
		bool GetDrawPtclR();

		void SetCurrentCameraOritation(TQuaternion<float> orit);
		TQuaternion<float> GetCurrentCameraOrientation();
	};

}
#endif
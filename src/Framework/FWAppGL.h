#ifndef FWAPPGL_H
#define FWAPPGL_H
#include <Springhead.h>
#include <SprFramework.h>
#include <Foundation/Object.h>

namespace Spr{

	class FWAppGL : public InheritObject<FWAppGLIf, Object>, public FWAppGLDesc {
    public:
		OBJECT_DEF(FWAppGL);
		ACCESS_DESC(FWAppGL);

		static FWAppGLIf* instance;
		static void GlutDisplayFunc();
		static void GlutReshapeFunc(int w, int h);
		static void GlutIdleFunc();

		FISdkIf*          fiSdk;
		FIFileXIf*        fiFileX;

		ObjectIfs         objs;

		PHSdkIf*          phSdk;
		PHSceneIf*        phScene;

		GRSdkIf*          grSdk;
		GRSceneIf*        grScene;
		GRDebugRenderIf*  grRender;
		GRDeviceGLIf*     grDevice;

		FWSceneIf*        fwScene;

		bool              isLoadComplete;
		int               cycleCount;
		int               cycleLimit;

		FWAppGL(const FWAppGLDesc& d=FWAppGLDesc()); // コンストラクタ
		void StartApp(std::string filename, int lim=0);
		void LoadFile(std::string filename);
		void CreateScene();
		void CreateRender();
		void Step();
		void Display();
		void Reshape(int w, int h);
		void Idle();
    };

}

#endif

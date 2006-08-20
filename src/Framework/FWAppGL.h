#ifndef FWAPPGL_H
#define FWAPPGL_H
#include <Springhead.h>
#include <SprFramework.h>
#include <Foundation/Object.h>

namespace Spr{

	class FWAppGL : public InheritObject<FWAppGLIf, Object>{
    public:
		OBJECT_DEF(FWAppGL);
		ACCESS_DESC(FWAppGL);

		static UTRef<FWAppGL> instance;
		static void GlutDisplayFunc();
		static void GlutReshapeFunc(int w, int h);
		static void GlutTimerFunc(int id);
		static void GlutKeyboardFunc(unsigned char key, int x, int y);

		FISdkIf*          fiSdk;
		FIFileXIf*        fiFileX;

		ObjectIfs         objs;

		PHSceneIf*        phScene;

		GRSceneIf*        grScene;
		GRDebugRenderIf*  grRender;
		GRDeviceGLIf*     grDevice;

		FWSceneIf*        fwScene;

		bool              isLoadComplete;
		bool              isSimulating;
		int               cycleCount;
		int               cycleLimit;
		int				  timeStep;
		UTString		  filename;
		
		FWAppGL(const FWAppGLDesc& d=FWAppGLDesc());
		~FWAppGL();

		virtual void StartApp(std::string filename, int lim=0);
		virtual void LoadFile(std::string filename);
		virtual void Load();
		virtual void CreateScene();
		virtual void CreateRender();
		virtual void Step();
		virtual void Display();
		virtual void Reshape(int w, int h);
		virtual void Timer();
		virtual void Keyboard(unsigned char key, int x, int y);
		virtual FWAppGLDesc::DebugMode GetDebugMode();
		virtual void SetDebugMode(FWAppGLDesc::DebugMode m);
    };

}

#endif

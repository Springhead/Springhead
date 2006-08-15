#ifndef SPR_FWAPPGLIF_H
#define SPR_FWAPPGLIF_H

#include <Foundation/SprObject.h>

namespace Spr{

	struct FWAppGLDesc{
		enum DebugMode{
			DM_NONE,
			DM_DEBUG,
		} debugMode;
		FWAppGLDesc():debugMode(DM_NONE){}
	};

	struct FWAppGLIf : public ObjectIf, public FWAppGLDesc{
		IF_DEF(FWAppGL);
		virtual void StartApp(std::string filename, int lim=0) = 0;
		virtual void LoadFile(std::string filename) = 0;
		virtual void CreateScene() = 0;
		virtual void Display() = 0;
		virtual void Reshape(int w, int h) = 0;
		virtual void Idle() = 0;
		virtual void Keyboard(unsigned char key, int x, int y) = 0;
		virtual FWAppGLDesc::DebugMode GetDebugMode()=0;
		virtual void SetDebugMode(FWAppGLDesc::DebugMode m)=0;
	};

	FWAppGLIf* SPR_CDECL CreateFWAppGL();
}

#endif

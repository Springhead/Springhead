#ifndef SPR_FWAPPGLIF_H
#define SPR_FWAPPGLIF_H

#include <Foundation/SprObject.h>

namespace Spr{

	struct FWAppGLDesc{
	};

	struct FWAppGLIf : ObjectIf {
		IF_DEF(FWAppGL);
		virtual void StartApp(std::string filename) = 0;
		virtual void LoadFile(std::string filename) = 0;
		virtual void CreateScene() = 0;
		virtual void Display() = 0;
		virtual void Reshape(int w, int h) = 0;
		virtual void Idle() = 0;
	};

	FWAppGLIf* SPR_CDECL CreateFWAppGL();
}

#endif

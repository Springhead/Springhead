#ifndef SPR_FWAPPGLUT_H
#define SPR_FWAPPGLUT_H

#include <Framework/SprFWAppGL.h>

namespace Spr{;

class FWAppGLUT : public FWAppGL{
public:
	static FWAppGLUT* instance;
	static void SPR_CDECL GlutDisplayFunc();
	static void SPR_CDECL GlutReshapeFunc(int w, int h);
	static void SPR_CDECL GlutTimerFunc(int id);
	static void SPR_CDECL GlutKeyboardFunc(unsigned char key, int x, int y);

	void Start();
	void OnTimer();
	void ProcessArguments(int argc, char* argv[]);
	virtual int CreateWindow(int arg=0);
};


}

#endif

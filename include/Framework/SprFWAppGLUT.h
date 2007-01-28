#ifndef SPR_FWAPPGLUT_H
#define SPR_FWAPPGLUT_H

#include <Framework/SprFWAppGL.h>

namespace Spr{;

class FWWindowDesc{	//	hase	TypeDescができないようにクラスにしてある。TypeDesc側での対応が望ましい。
public:
	int width;
	int height;
	int left;
	int top;
	bool fullscreen;
	FWWindowDesc(int w=640, int h=480, int l=-1, int t=-1, bool f=false):width(w), height(h), left(l), top(t), fullscreen(f){}
};

class FWAppGLUT : public FWAppGL{
protected:
	static FWAppGLUT* instance;
	static void SPR_CDECL GlutDisplayFunc();
	static void SPR_CDECL GlutReshapeFunc(int w, int h);
	static void SPR_CDECL GlutTimerFunc(int id);
	static void SPR_CDECL GlutKeyboardFunc(unsigned char key, int x, int y);
	static void SPR_CDECL AtExit();
	int windowID;
public:
	void Start();
	void OnTimer();
	virtual void Init(int argc, char* argv[]);
	virtual void Display();
	virtual int CreateWindow(const FWWindowDesc d=FWWindowDesc());
};

}

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
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

/** @brief GLUTを用いるアプリケーションクラス
 */
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
	~FWAppGLUT();
	virtual void Init(int argc, char* argv[]);
	virtual void Display();
	virtual int CreateWindow(const FWWindowDesc d=FWWindowDesc());
	void Start();
};

}

#endif

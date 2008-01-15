/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWAPPGLUI_H
#define SPR_FWAPPGLUI_H

#include <Framework/SprFWAppGL.h>
#include <Framework/SprFWAppGLUT.h>

namespace Spr{;

class FWAppGLUI : public FWAppGL{
protected:
	static FWAppGLUI* instance;
	static void SPR_CDECL GluiDisplayFunc();
	static void SPR_CDECL GluiReshapeFunc(int w, int h);
	static void SPR_CDECL GluiTimerFunc(int id);
	static void SPR_CDECL GluiKeyboardFunc(unsigned char key, int x, int y);
	static void SPR_CDECL AtExit();
	int windowID;
public:
	~FWAppGLUI();
	virtual void Init(int argc, char* argv[]);
	virtual void Display();
	virtual int CreateWindow(const FWWindowDesc d=FWWindowDesc());
	void Start();

};

}
#endif
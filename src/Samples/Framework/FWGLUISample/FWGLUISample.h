/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWAPPSAMPLE_H
#define FWAPPSAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Framework/FWGLUI.h>

using namespace Spr;

class FWGLUISample : public FWApp{
public:
	FWGLUISample();
	
	virtual void Init(int argc, char* argv[]);
	virtual void Keyboard(int key, int x, int y);
}; 

#endif
/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HI_DRAGGER_H
#define HI_DRAGGER_H

#include <HumanInterface/HIBase.h>
#include <HumanInterface/HIDevice.h>
#include <HumanInterface/SprHIDragger.h>

namespace Spr {;

class SPR_DLL HIDragger: public HIBase, public HIDraggerDesc, public DVKeyMouseHandler{
public:
	SPR_OBJECTDEF(HIDragger);

	DVKeyMouseIf*		keyMouse;
	HIDraggerCallback*	callback;
public:
	DVKeyMouseIf*	GetKeyMouse(){ return keyMouse; }
	void			SetKeyMouse(DVKeyMouseIf* dv);

	void			SetCallback(HIDraggerCallback* func){ callback = func; }

	// HIBaseÇÃä÷êî
	virtual bool Init(const void* desc);
	virtual void Update(float dt);

	/// DVKeyMouseHandlerÇÃä÷êî
	virtual bool OnMouse(int button, int state, int x, int y);
	virtual bool OnDoubleClick(int button, int x, int y);
	virtual bool OnMouseMove(int state, int x, int y, int zdelta);
	virtual bool OnKey(int state, int key, int x, int y);

	HIDragger(const HIDraggerDesc& desc = HIDraggerDesc()){}
};

}	//	namespace Spr
#endif

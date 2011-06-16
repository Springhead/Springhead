/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "HIDragger.h"

namespace Spr {;

bool HIDragger::Init(const void* desc){
	return true;
}

void HIDragger::SetKeyMouse(DVKeyMouseIf* dv){
	keyMouse = dv;
	keyMouse->AddHandler(this);
}

void HIDragger::Update(float dt){
}

bool HIDragger::OnMouse(int button, int state, int x, int y){
	return false;
}

bool HIDragger::OnDoubleClick(int button, int x, int y){
	return false;
}

bool HIDragger::OnMouseMove(int state, int x, int y, int zdelta){
	return false;
}

bool HIDragger::OnKey(int state, int key, int x, int y){
	return false;
}

}	//	namespace Spr

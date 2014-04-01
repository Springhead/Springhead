/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DR_JOYSTICK_GLUT_H
#define DR_JOYSTICK_GLUT_H

#include <SprDefs.h>
#include <HumanInterface/HIDevice.h>
#include <GL/glut.h>
#include <map>

#ifndef GLUTCALLBACK
#define GLUTCALLBACK
#endif

namespace Spr {;

///	DVKeyMouse
class DRJoyStickGLUT: public HIRealDevice{
public:
	SPR_OBJECTDEF(DRJoyStickGLUT);
	
	class DV: public DVJoyStick{
	public:
		DV(DRJoyStickGLUT* dr, int ch):DVJoyStick(dr, ch){}
		DRJoyStickGLUT* GetRealDevice(){ return realDevice->Cast(); }

		virtual void SetPollInterval(int ms){
			pollInterval = ms;
			GetRealDevice()->SetPollInterval(this);
		}
	};

public:
	DRJoyStickGLUT();
	
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual HIVirtualDeviceIf*	Rent(const IfInfo* ii, const char* name, int portNo);

	void OnUpdate(unsigned int buttonMask, int x, int y, int z);
	void SetPollInterval(DVJoyStick* dv);
	
	static DRJoyStickGLUT*	instance;
	static void GLUTCALLBACK OnUpdateStatic(unsigned int buttonMask, int x, int y, int z);
};

}	//	namespace Spr
#endif

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
#include <Framework/SprFWJoyStick.h>
#include <map>

namespace Spr {;

///	DVKeyMouse
class FWJoyStickGLUT: public HIRealDevice{
public:
	SPR_OBJECTDEF(FWJoyStickGLUT);
	
	class DV: public DVJoyStick{
	public:
		DV(FWJoyStickGLUT* dr, int ch):DVJoyStick(dr, ch){}
		FWJoyStickGLUT* GetRealDevice(){ return realDevice->Cast(); }

		virtual void SetPollInterval(int ms){
			pollInterval = ms;
			GetRealDevice()->SetPollInterval(this);
		}
	};

public:
	FWJoyStickGLUT(const FWJoyStickGLUTDesc& desc = FWJoyStickGLUTDesc());

	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual HIVirtualDeviceIf*	Rent(const IfInfo* ii, const char* name, int portNo);

	void OnUpdate(unsigned int buttonMask, int x, int y, int z);
	void SetPollInterval(DVJoyStick* dv);
	
	static FWJoyStickGLUT*	instance;
};

}	//	namespace Spr
#endif

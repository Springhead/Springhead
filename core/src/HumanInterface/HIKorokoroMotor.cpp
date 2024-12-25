/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HIKorokoroMotor.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {


HIKorokoroMotor::HIKorokoroMotor(){
	da = NULL;
	//counter = NULL;
	//ad = NULL;
	force = 0;
	
	voltageInt = 0;
	currentInt = 0;
	//lengthLast = 0;
	//vdd = 21.11f;
	
}

}	//	namespace Spr

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**************************************************************************************
  << Explanation >>
 Takashi TOKIZAKI   2nd, July, 2008
 
 This is the Test Program for Multi-Window Applications with Springhead2 Framework.
 When you changed the Springhead Framework API or other codes, you must try following tests.
 
 1. Can you make the multi-wiundow system using this framework?
 2. Are BOTH rendering procceses ok ?
 3. Is the return value of the functiuon : "FWWin* GetCurrentWin()" ok?
 4. Are BOTH overloaded glutKeyboardFunc ok ?

 Thanks, best regards.
***************************************************************************************/

#include <windows.h>
#include <Framework/SprFWAppGLUT.h>
#include <Springhead.h>
#include <iostream>
#include "MYApp.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

int __cdecl main(int argc, char* argv[]){
	
	MYApp app;
	app.Init(argc, argv);
	
	Sleep(100);
	
	app.Start();
	
	return 0;
}

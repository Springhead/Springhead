/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/Collision/CDGJKDebug/main.cpp 

【概要】
  ContFindCommonPointSaveParam.txtを読み出して、テストをするデバッグ用プログラム
  */

#include <Springhead.h>		//	Springheadのインタフェース
#include <Collision/CDDetectorImp.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

void main() {
	UTRef<PHSdkIf> sdk = PHSdkIf::CreateSdk();
	std::ifstream file("test\\ContFindCommonPointSaveParam.txt");
	ContFindCommonPointCall(file, sdk);
}
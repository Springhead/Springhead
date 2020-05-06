/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/*
	xboxコントローラの動作確認のサンプル
	3次元位置（Posed)を出力する。
	LT:y軸下
	RT:y軸上
	thumbL:x, z
	thumbR:x, z軸回りの回転
*/

#include <conio.h>
#include <Springhead.h>
using namespace Spr;


int __cdecl main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	HIXbox360ControllerIf* con = sdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	con->Init();
	while(!_kbhit()){
		con->Update(0.001f);
		std::cout << con->GetPose() << std::endl;
	}
	_getch();
}

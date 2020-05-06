/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
	力覚インタフェースFalconの接続確認用プログラム
	位置と力覚を提示する
*/

#include <conio.h>
#include <Springhead.h>
#include <iomanip>
using namespace Spr;

int __cdecl main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	sdk->Print(DSTR);
	UTRef<HINovintFalconIf> intf = sdk->CreateHumanInterface(HINovintFalconIf::GetIfInfoStatic())->Cast();
	intf->Init(NULL);
	int t = 0;
	for(int i=0; !_kbhit() && i<100000; ++i){
		t += 1;
		intf->Update(0.001f);
#if 1
		Vec3f intfpos = intf->GetPosition();
		std::cout << std::setprecision(2) << intfpos << std::endl;
		Vec3f f(0.0, 0.0, 0.0);
		if(intfpos.y < -0.015){
			f[1] = (float) (-(intfpos.y - (-0.015)) * 1000);
		}
		intf->SetForce(f, Vec3f());
#endif

	}
	if (_kbhit()) _getch();
}

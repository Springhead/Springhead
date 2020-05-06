/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DRPORTIO_H
#define DRPORTIO_H
#include <base/BaseUtility.h>

namespace Spr {
class SPR_DLL WBLoadWindowsDriver;

/**	WindowsNT,2000 でI/Oポート直接アクセスをするために呼ぶ関数.
	これを呼んでおけば、_inp, _outp を行っても保護違反がおきなくなる。
	giveio.sys を exeと同じフォルダか、Windowsのフォルダか、パスの通った
	フォルダに入れておくことが必要。
*/
void WBGetPortIO();
}	//	namespace Spr;

#if defined _MSC_VER
 #include <conio.h>
#endif

#if defined __BORLANDC__
#define _inp bcb_inp
#define _outp bcb_outp
#define _inpw bcb_inpw
#define _outpw bcb_outpw
namespace Spr {
 unsigned char SPR_DLL bcb_inp(unsigned short adr);
 void SPR_DLL bcb_outp(unsigned short adr, unsigned char v);
 unsigned short SPR_DLL bcb_inpw(unsigned short adr);
 void SPR_DLL bcb_outpw(unsigned short adr, unsigned short v);
}	//	namespace Spr;
#endif



#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWAPPGL_H
#define SPR_FWAPPGL_H
#include <Framework/SprFWApp.h>

namespace Spr{;

/** OpenGLのコンテキスト作成機能を持つアプリケーションクラス
 */
class FWAppGL : public FWApp{
public:
	/** @brief 新しいWindowのための、GRRender, GRDeviceを作る。
	 */
	FWSdkIf::FWWin* CreateRender(int wid);
};

}

#endif

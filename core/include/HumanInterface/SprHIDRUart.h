/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef SPR_HI_DRUART_H
#define SPR_HI_DRUART_H

#include <HumanInterface/SprHIBase.h>

namespace Spr{;

/** \addtogroup gpHumanInterface */
//@{

///	
struct DRUARTMotorDriverIf : public HIRealDeviceIf{
	SPR_IFDEF(DRUARTMotorDriver);
	int NMotor();
	int NForce();
	int NBoard();
	int GetBoardType(int boardPos);
	long ReadForce(int ch);
};
struct DRUARTMotorDriverDesc{
	SPR_DESCDEF(DRUARTMotorDriver);
	int port;		///< 使用チャンネル．-1なら任意
	DRUARTMotorDriverDesc(): port(-1){}
};


//@}

}	//	namespace Spr
#endif

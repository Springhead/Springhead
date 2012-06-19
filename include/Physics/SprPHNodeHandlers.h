/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_PHNODEHANDLERS_H
#define SPR_PHNODEHANDLERS_H
#include <Foundation/SprObject.h>

namespace Spr {;
/** \addtogroup gpPhysics 	*/
//@{
///	お互いに衝突しない剛体を設定
struct PHInactiveSolids{
	std::vector<std::string> solids;	//	衝突しない剛体のリスト
};

//@}

}	//	namespace Spr

#endif	

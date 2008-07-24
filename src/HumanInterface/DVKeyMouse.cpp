/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/DVKeyMouse.h>
namespace Spr {;
bool DVKeyMouseHandler::OnMouseMove(int , int , int , int ){ return false; }
///
bool DVKeyMouseHandler::OnClick(int , int , int ){ return false; }
///
bool DVKeyMouseHandler::OnDoubleClick(int , int , int ){ return false; }
///	ÉLÅ[èàóù
bool DVKeyMouseHandler::OnKey(bool , int , int , int , int ){
	return false;
}

}	//	namespace Spr

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHConstraintBase.h
 *	@brief 拘束関係の基本クラス
*/
#ifndef SPR_PHCONSTRAINTBASEIf_H
#define SPR_PHCONSTRAINTBASEIf_H

#include <Foundation/SprObject.h>

namespace Spr{;

/** \addtogroup gpJoint */
//@{

/// 拘束、モーター、ギアの基本クラス
struct PHConstraintBaseIf : SceneObjectIf {
	SPR_IFDEF(PHConstraintBase);
};

}

#endif//SPR_PHCONSTRAINTBASEIf_H

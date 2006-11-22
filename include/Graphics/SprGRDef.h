/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprGRDef.h
 *	@brief グラフィックス定義ファイル
*/

#ifndef SPR_GRDef_H
#define SPR_GRDef_H

/**	\addtogroup	gpGraphics	*/
//@{

#define USE_GREW	
	
#if defined(USE_GREW)
#include <GL/glew.h>
#endif
#include <GL/glut.h>	
	
#if defined(USE_GREW)	
typedef	GLhandleARB	GRHandler;
#else	
typedef	GLuint		GRHandler;
#endif	
	
//@}
#endif

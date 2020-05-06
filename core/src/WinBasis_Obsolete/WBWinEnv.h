/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef WINCMP_CWINENV_H
#define WINCMP_CWINENV_H

#include "../Base/BaseDebug.h"

#if defined _MSC_VER && defined _CPPUNWIND
 #define SUPPORT_EXCEPTION
#endif

// use/don't use MFC
#if (!defined NOMFC) && (!defined USEMFC)
 #if (!defined _MSC_VER) && (!defined _MFC_VER) && (!defined _AFXDLL)
  #define NOMFC
 #else
  #define USEMFC
 #endif
#endif

#endif



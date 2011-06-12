%module Utility
//%include "workaround.i"

%{
#include "Base/TQuaternion.h"
#include "Base/Affine.h"

#ifdef SWIG
#define __EPDECL
#else
#if _DEBUG
#define __EPDECL __cdecl
#else
#define __EPDECL __fastcall
#endif
#endif

//springhead
#include "Utility/SprEPIfInfo.h"
#include "Utility/SprEPUTString.h"
//swig
#include "Utility/SprEPErrors.h"

//std
#include "Utility/SprEPvoid.h"
#include "Utility/SprEPostream.h"
#include "Utility/SprEPistream.h"
#include "Utility/SprEPstring.h"
#include "Utility/SprEPchar.h"
#include "Utility/SprEPunsignedint.h"
#include "Utility/SprEPunsignedlong.h"

void __EPDECL initUtility(void);

//CriticalSection—p
#include "Utility/UTCriticalSection.h"
%}

%include "./Utility/PyPTM-sprpy.i"
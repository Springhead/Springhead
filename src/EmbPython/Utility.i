%module Utility

%begin %{
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

//swig
#include "Utility/SprEPObject.h"
#include "Utility/SprEPvoid.h"
#include "Utility/SprEPCast.h"
#include "Utility/SprUTCriticalSection.h"//CriticalSection用
#include <string>

using namespace std;

#define EP_AS_BUILTIN 1

%}


namespace Spr{

%feature("not_newable","1");
	class IfInfo{};
	class ostream{};
	class istream{};
%feature("not_newable","");

	//クリティカルセクション
	UTCriticalSection EPCriticalSection;

}//namespace

%include "./Utility/PyPTM.i"
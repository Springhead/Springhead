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
#include "Utility/SprEPFoundation.h"
#include "Utility/SprEPObject.h"
#include "Utility/SprEPvoid.h"
#include "Utility/SprEPCast.h"
#include "Utility/UTCriticalSection.h"//CriticalSection用
#include <string>

using namespace std;
%}


namespace Spr{

	class IfInfo{};
	class ostream{};
	class istream{};
	class UTString{};

	//クリティカルセクション
	UTCriticalSection EPCriticalSection;

}//namespace

%include "./Utility/PyPTM-sprpy.i"
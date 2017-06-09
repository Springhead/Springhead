%module Base

%begin %{
#include "Base/TQuaternion.h"
#include "Base/Affine.h"
#include "Base/Spatial.h"

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
#include "SprEPUtility.h"
#include "Utility/SprEPObject.h"
#include "Utility/SprEPCast.h"
#include <string>

using namespace std;

//{*********EPvector*******
#define EPvector_Check(ob) PyList_Check(ob)
//}EPvector
%}


namespace Spr{

%feature("not_newable","1");
	class IfInfo{
	public:
	IfInfo* creator;
	};
	class ostream{};
	class istream{};
%feature("not_newable","");

}//namespace

%include "./Utility/PyPTM.i"

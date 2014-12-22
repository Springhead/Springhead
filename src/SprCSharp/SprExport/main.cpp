#include <Springhead.h>
#include <Windows.h>

#include <sstream>

using namespace Spr;
using namespace std;

extern "C" {
	__declspec(dllexport) HANDLE __cdecl Spr_new_Vec3d_0() { return new Vec3d(); }
	__declspec(dllexport) HANDLE __cdecl Spr_new_Vec3d_1(double x, double y, double z) { return new Vec3d(x, y, z); }
	__declspec(dllexport) void __cdecl Spr_delete_Vec3d(HANDLE v) { delete v; }
	__declspec(dllexport) HANDLE __cdecl Spr_Vec3d_operator_plus(HANDLE a, HANDLE b) { HANDLE rv = new Vec3d(); (*((Vec3d*)rv)) = (*((Vec3d*)a)) + (*((Vec3d*)b)); return rv; }

	__declspec(dllexport) BSTR __cdecl Spr_Vec3d_ToString(HANDLE v) {
		BSTR result = NULL;

		ostringstream* oss = new ostringstream();
		(*oss) << (*((Vec3d*)v));
		const char* str = oss->str().c_str();

		int lenA = oss->str().size();
		int lenW = ::MultiByteToWideChar(CP_ACP, 0, str, lenA, NULL, 0);
		if (lenW > 0) {
			result = ::SysAllocStringLen(0, lenW);
			::MultiByteToWideChar(CP_ACP, 0, str, lenA, result, lenW);
		}

		return result;
	}
}

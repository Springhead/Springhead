#include <Springhead.h>
#include <Windows.h>

#include <sstream>

using namespace Spr;
using namespace std;

extern "C" {
	__declspec(dllexport) HANDLE __cdecl Spr_new_Vec3d_0() { return new Vec3d(); }
	__declspec(dllexport) HANDLE __cdecl Spr_new_Vec3d_1(double x, double y, double z) { return new Vec3d(x, y, z); }
	__declspec(dllexport) void __cdecl Spr_delete_Vec3d(HANDLE v) { delete ((Vec3d*)v); }
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


	// PHSceneDesc
	__declspec(dllexport) HANDLE __cdecl Spr_PHSceneDesc_get_gravity(HANDLE _this) {
		Vec3d* rv = new Vec3d(); (*rv) = (*((PHSceneDesc*)_this)).gravity; return rv;
	}
	__declspec(dllexport) void __cdecl Spr_PHSceneDesc_set_gravity(HANDLE _this, HANDLE value) {
		(*((PHSceneDesc*)_this)).gravity = (*((Vec3d*)value));
	}

	__declspec(dllexport) double __cdecl Spr_PHSceneDesc_get_airResistanceRate(HANDLE _this) {
		return (*((PHSceneDesc*)_this)).airResistanceRate;
	}
	__declspec(dllexport) void __cdecl Spr_PHSceneDesc_set_airResistanceRate(HANDLE _this, double value) {
		(*((PHSceneDesc*)_this)).airResistanceRate = value;
	}

	__declspec(dllexport) HANDLE __cdecl Spr_new_PHSceneDesc() { return new PHSceneDesc(); }

	__declspec(dllexport) void __cdecl Spr_PHSceneDesc_Init(HANDLE _this) { (*((PHSceneDesc*)_this)).Init(); }

	__declspec(dllexport) void __cdecl Spr_delete_PHSceneDesc(HANDLE _this) { delete ((PHSceneDesc*)_this); }
}

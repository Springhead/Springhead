#include <Springhead.h>
#include <Windows.h>
#include <../src/SprCSharp/SprExport/CSUtility.h>
extern "C" {
	__declspec(dllexport) bool __cdecl Spr_UTTimerIf_SetCallback(HANDLE _this, HANDLE callback) {
		Spr::UTTimerIf::TimerFunc arg01 = (Spr::UTTimerIf::TimerFunc) callback;
		bool _val = false;
		try{
			_val = ((Spr::UTTimerIf*) _this)->SetCallback(arg01);
		}
		catch (SEH_Exception e){
			e.raise_managed_exception("SprExport.dll");
		}
		return _val;
	}
}

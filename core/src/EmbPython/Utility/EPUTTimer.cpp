#include "../../../include/EmbPython/SprEPFoundation.h"
#include "../../../include/EmbPython/SprEPUtility.h"
#include "../../../include/EmbPython/SprEPBase.h"

void SPR_CDECL PyUTTimerFunc(int id, void* arg){
	PyGILState_STATE state;
	while((state = PyGILState_Ensure()) != PyGILState_UNLOCKED);
	if (state == PyGILState_UNLOCKED){
		PyObject_CallObject((PyObject*)arg, NULL);
		PyGILState_Release(state);
	}else{
#ifdef _WIN32
		DSTR << "Fail to call python callback " << ios::hex << (unsigned)arg << " by UTTimer id=" << id << std::endl;
#else
		DSTR << "Fail to call python callback " << ios::hex << arg << " by UTTimer id=" << id << std::endl;
#endif
	}
}

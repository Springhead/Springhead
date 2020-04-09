#define _NOT_USE_PYTHON_SPR_LIB

#include <EmbPython/SprEPVersion.h>
#include PYTHON_H_REL

// #include <iostream>

#include <EmbPython/EmbPython.h>
#include <EmbPython/SprEPUtility.h>


static PyMethodDef Spr_methods[] = {
	{NULL, NULL}
};

static struct PyModuleDef Sprmodule = {
	PyModuleDef_HEAD_INIT,
	EP_MODULE_NAME,
	"Springhead on Python",
	-1,
	Spr_methods,
	NULL,
	NULL,
	NULL,
	NULL
};

extern "C" __declspec(dllexport) PyObject* _cdecl PyInit_Spr(void) {
	PyObject *module_Spr;
	module_Spr = PyModule_Create(&Sprmodule);
	PyObject *dict = PyModule_GetDict(module_Spr);

	initUtility();
	initBase(module_Spr);
	initFoundation(module_Spr);
	initFileIO(module_Spr);
	initCollision(module_Spr);
	initPhysics(module_Spr);
	initGraphics(module_Spr);
	initCreature(module_Spr);
	initHumanInterface(module_Spr);
	initFramework(module_Spr);

	return module_Spr;
}

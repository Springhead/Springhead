#include "..\..\include\EmbPython\SprEPUtility.h"

UTCriticalSection EPCriticalSection;

PyMODINIT_FUNC initUtility(void)
{
	initEPVector();
	initEPMatrix();
	initEPPose();
	initEPQuaternion();
	initEPIfInfo();
	initEPUTString();

	initEPvoid();
	initEPostream();
	initEPistream();
	initEPstring();
	initEPchar();
	initEPunsignedint();
	return NULL;
}
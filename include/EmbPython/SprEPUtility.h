#ifndef SPREPUTILITY_H
#define SPREPUTILITY_H

#ifdef SWIG
#define __EPDECL
#else
#define __EPDECL __cdecl
#endif

//springhead
#include "Utility/SprEPVector.h"
#include "Utility/SprEPMatrix.h"
#include "Utility/SprEPPose.h"
#include "Utility/SprEPQuaternion.h"
#include "Utility/SprEPAffine.h"
#include "Utility/SprEPIfInfo.h"
#include "Utility/SprEPUTString.h"
#include "Utility/SprEPAffine.h"
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

PyMODINIT_FUNC __EPDECL initUtility(void);

//CriticalSection—p
#include "UTCriticalSection.h"
extern UTCriticalSection EPCriticalSection;

#endif

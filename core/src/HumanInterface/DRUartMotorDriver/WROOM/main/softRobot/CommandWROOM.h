#pragma once
#ifdef WROOM
#include "env.h"
#endif

#ifndef _COMMANDID_CS
#include "../../../PCRobokey/CommandId.cs"
#define _COMMANDID_CS
#endif

#include "../../../PIC/fixed.h"
#include "../../../PIC/commandCommon.h"

enum BD0_PARAM {
	BD0_MODEL_NUMBER = -1,
	BD0_NTARGET = 12,
	BD0_NMOTOR = 3,
	BD0_NCURRENT = 0,
	BD0_NFORCE = 0,
	BD0_NTOUCH = 5,	//	Touch2 to Touch6
};
DEFINE_Packets(BD0, CT_0, CT_0, CT_N)

#ifndef _WIN32
CHOOSE_BoardInfo(BD0);
#define BOARD_ID 0xFF 
#endif

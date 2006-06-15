#ifndef HDSPIDARG6O_H
#define HDSPIDARG6O_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	フレームのサイズやモータの符号を含む	*/
class SPR_DLL HISpidarG6O: public HISpidarG6{
public:
	HISpidarG6O();
	bool Init(DVDeviceManager& dev);
	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarG6O);
};

}	//	namespace Spr
#endif

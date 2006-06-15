#ifndef HDSPIDARG6WB_H
#define HDSPIDARG6WB_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	フレームのサイズやモータの符号を含む	*/
class SPR_DLL HISpidarG6WB: public HISpidarG6{
public:
	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarG6WB);

	HISpidarG6WB();
	bool Init(DVDeviceManager& dev);

//	virtual ~HISpidarG6WB();
};

}	//	namespace Spr
#endif

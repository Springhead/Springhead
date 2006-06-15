#ifndef HDSPIDARG6C_H
#define HDSPIDARG6C_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	フレームのサイズやモータの符号を含む	*/
class SPR_DLL HISpidarG6C: public HISpidarG6{
public:
	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarG6C);

	HISpidarG6C();
	bool Init(DVDeviceManager& dev);

//	virtual ~HISpidarG6C();
};

}	//	namespace Spr
#endif

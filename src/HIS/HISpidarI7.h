#ifndef HDSPIDARI7_H
#define HDSPIDARI7_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-I
	フレームのサイズやモータの符号を含む	*/
class SPR_DLL HISpidarI7: public HISpidarG6{
public:
	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarI7);

	HISpidarI7();
	virtual bool Init(DVDeviceManager& dev);
};

}	//	namespace
#endif

#ifndef HDSPIDARG6X3_H
#define HDSPIDARG6X3_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G XType
(分解できて手軽に持ち運べるコンパクトなＳＰＩＤＡＲというコンセプト)
	フレームのサイズやモータの符号を含む	*/
class SPR_DLL HISpidarG6X3: public HISpidarG6{
public:
	HISpidarG6X3();
	bool Init(DVDeviceManager& dev, bool bLeft);

	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarG6X3);
};

}	//	namespace Spr
#endif

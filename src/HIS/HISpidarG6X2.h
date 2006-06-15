#ifndef HDSPIDARG6X2_H
#define HDSPIDARG6X2_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G XType
(分解できて手軽に持ち運べるコンパクトなＳＰＩＤＡＲというコンセプト)
	フレームのサイズやモータの符号を含む	*/
class SPR_DLL HISpidarG6X2: public HISpidarG6{
public:
	HISpidarG6X2();
	bool Init(DVDeviceManager& dev);

	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarG6X2);
};

}	//	namespace Spr
#endif

#ifndef HDSPIDARCANOE_H
#define HDSPIDARCANOE_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	フレームのサイズやモータの符号を含む	*/
class SPR_DLL HISpidarCanoe: public HISpidarG6{
public:
	///	デバイスのタイプ
	HIOBJECTDEF(HISpidarCanoe);

	HISpidarCanoe();
	bool Init(DVDeviceManager& dev);
	virtual void Update(float dt);
	void MakeWireVec();

//	virtual ~HISpidarCanoe();
};

}	//	namespace Spr
#endif

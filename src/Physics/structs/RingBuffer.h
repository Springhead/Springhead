#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <Springhead.h>

using namespace Spr;

class RingBuffer
{
public:
    RingBuffer();
    int Read(Vec3d &result);
    int Write(Vec3d element);
	void printBuffer();
	Vec3d getLastOne();
	int WriteNoLimit(Vec3d element);

private:
    unsigned read_index;
    unsigned write_index;
    static const unsigned buffer_size = 10;
    Vec3d buffer[buffer_size];
	
};

#endif

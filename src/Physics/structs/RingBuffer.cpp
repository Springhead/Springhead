#include "RingBuffer.h"

RingBuffer::RingBuffer() 
{
	this->read_index = 0;
	this->write_index = 0;
}

int RingBuffer::Read(Vec3d &result)
{
    volatile unsigned local_read;
	local_read= read_index;

    if (local_read == write_index)
        return 0;

    ++local_read;
    if (local_read == buffer_size)
        local_read = 0;

	result = buffer[local_read];
	read_index = local_read;

	return 1;
}

int RingBuffer::Write(Vec3d element)
{
	volatile unsigned local_write;
	local_write = write_index;

    ++local_write;
    if (local_write == buffer_size)
        local_write = 0;

    if (local_write != read_index) {
        buffer[local_write] = element;
        write_index = local_write;
        return 1;
    }

    return 0;
}

int RingBuffer::WriteNoLimit(Vec3d element)
{
	volatile unsigned local_write;
	local_write = write_index;

    ++local_write;
    if (local_write == buffer_size)
        local_write = 0;

    buffer[local_write] = element;
    write_index = local_write;
    return 1;
}


Vec3d RingBuffer::getLastOne()
{
	volatile unsigned local_write;
	local_write = write_index;
	
	return buffer[local_write];
}

void RingBuffer::printBuffer()
{
	DSTR << "******** buffer debug ********** " << std::endl;
	for (int i=0; i < buffer_size; i++)
	{
		DSTR << buffer[i] << " , ";
	}
	DSTR << "******** buffer debug ********** " << std::endl;

}
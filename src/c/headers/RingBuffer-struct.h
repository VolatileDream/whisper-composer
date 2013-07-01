#ifndef _CONCURRENT_RING_BUFFER_STRUCT_HPP_
#define _CONCURRENT_RING_BUFFER_STRUCT_HPP_

#include "RingBuffer.h"

struct WC_RingBuffer {
	// indices of the start + end of our buffer.
	unsigned int start;
	unsigned int end;

	unsigned int size;
	void* data;
	// used to hold objects, and not pointers.
	unsigned int _struct_size;
};

#endif
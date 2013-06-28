#ifndef _CONCURRENT_RING_BUFFER_HPP_
#define _CONCURRENT_RING_BUFFER_HPP_

// DO NOT, UNDER ANY CIRCUMSTANCES
// THINK THAT THIS CLASS IS A GOOD
// EXAMPLE OF THREAD SAFE CODE.

// It just so happens that this
// is highly concurrent, but only
// when you know what you're doing.


#include <array>

namespace Whisper {

template<typename T, size_t size>
class ConcurrentRingBuffer {
public:
	ConcurrentRingBuffer(): start(0), end(0) {}

	// adds an item to the buffer, returns true on success
	// Note that the buffer does not take responsibility of the pointer.
	bool add( T value ){
		// remember that start + end are stale.
		int newEnd = incr(end);
		if( start == newEnd ){
			//one space left, but we can't add an item.
			// otherwise remove/1 will think the buffer
			// is empty.
			return false;
		}

		buffer.at(end) = value;//insert the new value.
		end = newEnd;
	}

	// removes the next item from the buffer, returns NULL on failure
	// responsibility for releasing the pointer is NOT passed to the caller.
	bool remove( T& val ){
		if( start == end ){
			return false;
		}
		val = buffer.at(start);
		start = incr(start);
		return true;
	}
private:
	size_t incr( size_t i ) const {
		return (i + 1) % size;
	}
	// indices of the start + end of our buffer.
	size_t start;
	size_t end;

	std::array<T,size> buffer;
};

}

#endif
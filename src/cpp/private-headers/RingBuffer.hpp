#ifndef __WC_RING_BUFFER_HPP__
#define __WC_RING_BUFFER_HPP__

/*
DO NOT, UNDER ANY CIRCUMSTANCES THINK
THAT THIS CLASS IS A GOOD EXAMPLE OF
THREAD SAFE CODE.

It just so happens that this is highly
concurrent, but only when there is exactly
a single producer and a single consumer.
*/

#include <algorithm>

namespace Whisper {

template<typename T>
class RingBuffer {
public:

	/// Create a RingBuffer
	/// 
	///@param size The size of the ring buffer
	///@return An initialized RingBuffer<T>
	RingBuffer(unsigned int size) : size(size), buffer(new T[size]) {}

	/// Destroy the RingBuffer
	///
	/// The RingBuffer does _not_ free any objects
	/// that have been inserted into it.
	~RingBuffer(){
		// yep, that's right. we never
		// took ownership of anything
		// that was given to us.
		delete [] buffer;
	}

	/// Adds an item to the buffer, returns true on success
	///
	/// Note that the buffer does not take responsibility of the pointer.
	///
	/// @param value The object to insert into the buffer
	/// @return true if the object is inserted into the buffer
	bool add( T value ){
		// remember that start + end are stale.
		unsigned int newEnd = incr(end);
		if( start == newEnd ){
			//one space left, but we can't add an item.
			// otherwise remove/1 will think the buffer
			// is empty.
			return false;
		}

		buffer[end] = value;//insert the new value.
		end = newEnd;
		return true;
	}

	/// Checks to see if the buffer has more entries
	///
	/// Note that this should only be called from
	/// the consumer side
	/// @return true if the buffer has more entries
	bool hasMore(){
		return start != end;
	}

	/// Attempts to remove the next item from the buffer.
	///
	/// Removes the next item from the buffer, returns false if there is no such
	/// item. Responsibility for releasing the pointer is NOT passed to the caller
	/// of this function.
	///
	/// @param val A reference to place the item in.
	/// @return true if the reference was set to a element in the buffer.
	bool remove( T& val ){
		if( start == end ){
			return false;
		}
		// set the contents of val
		// to the buffer contents
		val = buffer[start];
		start = incr(start);
		return true;
	}
private:
	/// Increments a ring buffer index
	///
	/// @param i A position in the ring buffer
	/// @return The next position in the ring buffer
	unsigned int incr( unsigned int i ) const {
		return (i + 1) % size;
	}
	// indices of the start + end of our buffer.
	unsigned int start = 0;
	unsigned int end = 0;
	unsigned int size;
	T* buffer;
};

}

#endif
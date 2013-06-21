// DO NOT, UNDER ANY CIRCUMSTANCES
// THINK THAT THIS CLASS IS A GOOD
// EXAMPLE OF THREAD SAFE CODE.

// It just so happens that this
// is highly concurrent, but only
// when you know what you're doing.


#ifndef _CONCURRENT_RING_BUFFER_HPP_
#define _CONCURRENT_RING_BUFFER_HPP_

#include <array>

template<typename T, size_t size>
class ConcurrentRingBuffer {
public:
	ConcurrentRingBuffer(): start(0), end(0) {
		buffer = new std::array<T*,size>;
	}

	// adds an item to the buffer, returns true on success
	// Note that the buffer takes responsibility of the pointer.
	// And will free it when it is done with it.
	bool add( T* value ){
		// remember that start + end are stale.
		int newEnd = incr(end);
		if( start == newEnd ){
			//one space left, but we can't add an item.
			// otherwise remove/1 will think the buffer
			// is empty.
			return false;
		}

		// this is a hack to avoid deallocation in the port
		// audio callback (possibly an os interrupt)
		T* item = buffer->at(end);
		if( item != NULL ){
			delete item;
		}

		buffer->at(end) = value;//insert the new value.
		end = newEnd;
	}

	// removes the next item from the buffer, returns NULL on failure
	// responsibility for releasing the pointer is NOT passed to the caller.
	// CRB will eventually clear the pointer, either when the buffer is
	// destroyed, or `size` items have been inserted, causing an overwrite
	// to the location in memory.
	T* remove(){
		if( start == end ){
			return NULL;
		}
		T* item = buffer->at(start);
		start = incr(start);
		return item;
	}
		
	// destroys the buffer.
	~ConcurrentRingBuffer(){
		// I really, really hope that you're not still using this object...

		// clear the entire array.
		for(size_t index; index < size; index++ ){
			delete buffer->at(index);
		}
		delete buffer;
	}
private:
	size_t incr( size_t i ) const {
		return (i + 1) % size;
	}
	// indices of the start + end of our buffer.
	size_t start;
	size_t end;

	std::array<T*,size>* buffer;
};

#endif
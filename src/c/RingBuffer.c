#include "stdlib.h"
// for memmove
#include "string.h"
#include "RingBuffer-struct.h"

// DO NOT, UNDER ANY CIRCUMSTANCES
// THINK THAT THIS CLASS IS A GOOD
// EXAMPLE OF THREAD SAFE CODE.

// It just so happens that this
// is highly concurrent, but only
// when you know what you're doing.


static unsigned int incr( unsigned int number, unsigned int size ) {
	return (number + 1) % size;
}

bool WC_RB_Create(WC_RingBuffer* buffer, unsigned int sizeOfStruct, unsigned int size){
	if( buffer == NULL ){
		return false;
	}

	buffer->data = malloc( sizeOfStruct * size );

	if( buffer->data == NULL ){
		return false;
	}

	buffer->start = 0;
	buffer->end = 0;
	buffer->size = size;
	buffer->_struct_size = sizeOfStruct;

	return true;
}


// adds an item to the buffer, returns true on success
// Note that the buffer does not take responsibility of the pointer.
bool WC_RB_Add( WC_RingBuffer* buffer, void* value ){
	// remember that start + end are stale.
	int newEnd = incr(buffer->end, buffer->size);
	if( buffer->start == newEnd ){
		//one space left, but we can't add an item.
		// otherwise remove/1 will think the buffer
		// is empty.
		return false;
	}

	// this feels dirty.
	memmove( buffer->data + buffer->end*buffer->_struct_size, value, buffer->_struct_size );

	buffer->end = newEnd;
	return true;
}

// removes the next item from the buffer, returns NULL on failure
// responsibility for releasing the pointer is NOT passed to the caller.
bool WC_RB_Remove( WC_RingBuffer* buffer, void* val ){
	if( buffer->start == buffer->end ){
		return false;
	}
	// feels dirty...
	memmove( val, buffer->data + buffer->start*buffer->_struct_size, buffer->_struct_size );
	buffer->start = incr(buffer->start, buffer->size);
	return true;
}

void WC_RB_Dispose(WC_RingBuffer* buffer){
	if( buffer->data != NULL )
		free( buffer-> data );
}

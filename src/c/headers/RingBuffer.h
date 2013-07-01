#ifndef _CONCURRENT_RING_BUFFER_HPP_
#define _CONCURRENT_RING_BUFFER_HPP_

#include <stdbool.h>

struct WC_RingBuffer;
typedef struct WC_RingBuffer WC_RingBuffer;

// attempts to initialize the buffer
// on failure, returns false
// the WC_RingBuffer pointer must still be deallocated
bool WC_RB_Create(WC_RingBuffer* buffer, unsigned int sizeOfStruct, unsigned int size);

// adds an item to the buffer, returns true on success
// Note that the buffer does not take responsibility of the pointer.
bool WC_RB_Add( WC_RingBuffer* buffer, void* value );

// removes the next item from the buffer, returns NULL on failure
// responsibility for releasing the pointer is NOT passed to the caller.
bool WC_RB_Remove( WC_RingBuffer* buffer, void* val );

// disposes of the ring buffer, but
// does not free the WC_RingBuffer struct
void WC_RB_Dispose(WC_RingBuffer* buffer);

#endif
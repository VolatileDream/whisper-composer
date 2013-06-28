#ifndef _DYN_ARRAY_
#define _DYN_ARRAY_

#include <memory>

#include "IllegalArgumentsException.hpp"

namespace Whisper {

typedef unsigned int size_t;

template<typename T>
class dyn_array {
public:
	const size_t size;
	T *const data; // const pointer to variable data

	// nice create
	static dyn_array<T>* create( size_t size ){
		T* arr = new float[size];
		return new dyn_array( arr, size, true );
	}

	std::unique_ptr<dyn_array<T>> subarray(size_t offset, size_t length)
		const throw
	(IllegalArgumentsException, std::bad_alloc) {
		if(
			offset < 0
			|| length < 0
			|| offset + length > size
		){
			throw IllegalArgumentsException();
		}
		return std::unique_ptr<dyn_array<T>>( new dyn_array( data + offset, length, false ) );
	}

	~dyn_array(){
		if( canFree )
			delete [] data;
	}
private:
	dyn_array( T* arr, size_t size, bool canFree ) : size(size), data(arr), canFree(canFree) {}
	const bool canFree;
};

}
#endif
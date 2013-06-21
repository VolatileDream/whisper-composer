#ifndef __GLOBAL_WHISPER_HPP__
#define __GLOBAL_WHISPER_HPP__

typedef unsigned int size_t;

template<typename T>
class dyn_array {
public:
	const size_t size;
	T *const data; // const pointer to variable data
	static dyn_array<T>* create( size_t size ){
		T* arr = new float[size];
		return new dyn_array( arr, size );
	}
	~dyn_array(){
		delete [] data;
	}
private:
	dyn_array( T* arr, size_t size ) : size(size), data(arr) {}
};

#endif
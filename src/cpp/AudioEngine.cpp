#include <algorithm>

#include "AudioEngine.hpp"

static void copy(float* src, float* dst, size_t length){
	while( length > 0 ){
		*src = *dst;
		src++;
		dst++;
		length--;
	}
}

AudioEngine::AudioEngine( void* audioSettings ){
	// uhhh...TODO, dunno how to handle audio settings yet.
}

bool AudioEngine::passData( dyn_array<float>* content ){

	if( content == NULL || content->size == 0 ){
		return false;
	}
	
	//copy the array.
	dyn_array<float>* newContent = dyn_array<float>::create(content->size);

	float* clone = newContent->data;
	float* real = content->data;
	//do the copy.
	copy(real, clone, content->size);

	//don't be stupid now...
	clone = real = NULL;

	//attempt to store in ring buffer
	bool success = ring.add( newContent );

	//if storage fails, free the copy and return.
	if( !success ){
		delete newContent;
	}

	return success;
}

AudioEngine::~AudioEngine(){
	// TODO clean up port audio stuff
}

int AudioEngine::givePortAudioData(
		void *outputBuffer
        ,unsigned long framesPerBuffer
//        ,const PaStreamCallbackTimeInfo* timeInfo
//        ,PaStreamCallbackFlags statusFlags
    ){

	// TODO port audio stuff

}
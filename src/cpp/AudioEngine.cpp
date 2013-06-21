#include <algorithm>

#include "AudioEngine.hpp"

AudioEngine::AudioEngine( void* audioSettings ){
	// uhhh...TODO, dunno how to handle audio settings yet.
}

bool AudioEngine::passData( dyn_array<float>* content ){
	
	//copy the array.
	dyn_array<float>* newContent = dyn_array<float>::create(content->size);

	float* clone = newContent->data;
	float* real = content->data;
	//do the copy.
	//std::copy<float,float>(&clone, clone + content->size, &real);

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
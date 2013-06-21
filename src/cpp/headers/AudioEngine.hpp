#ifndef _AUDIO_ENGINE_HPP_
#define _AUDIO_ENGINE_HPP_

#include <array>

//#include "portaudio.h"

#include "global.hpp"
#include "ConcurrentRingBuffer.hpp"

class AudioEngine {
public:
	AudioEngine( void* audioSettings );
	
	// does not take responsibility of the passed pointer
	//returns true on success
	bool passData( dyn_array<float>* content );
	
	// closes the portAudio stuff down.
	~AudioEngine();
private:
	// the port audio "callback".
	int givePortAudioData(
		void *outputBuffer
        ,unsigned long framesPerBuffer
    //    ,const PaStreamCallbackTimeInfo* timeInfo
    //    ,PaStreamCallbackFlags statusFlags
    );
    ConcurrentRingBuffer<dyn_array<float>,128> ring;
};

#endif
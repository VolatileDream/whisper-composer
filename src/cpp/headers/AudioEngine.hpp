#ifndef _AUDIO_ENGINE_HPP_
#define _AUDIO_ENGINE_HPP_

#include "portaudio.h"
#include <array>

class AudioEngine {
public:
	AudioEngine( void* audioSettings );
	//returns true on success
	bool passData( array<float>* content );
	// closes the portAudio stuff down.
	~AudioEngine();
private:
	// the port audio "callback".
	int givePortAudioData(
		void *outputBuffer
        ,unsigned long framesPerBuffer
        ,const PaStreamCallbackTimeInfo* timeInfo
        ,PaStreamCallbackFlags statusFlags
    );
};

#endif
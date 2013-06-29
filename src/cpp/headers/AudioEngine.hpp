#ifndef _AUDIO_ENGINE_HPP_
#define _AUDIO_ENGINE_HPP_

#include <array>
#include <tuple>

#include "portaudio.h"

#include "dyn_array.hpp"
#include "ConcurrentRingBuffer.hpp"

#include "Sound.hpp"

#define _maxConcurrentPlayCount 127
#define _maxQueueBeforeReadCount 63

// Tune this:
// We might need to raise/lower it.
// Depends on how quickly we can check it from outside.
#define _maxFinishedSoundCount ((_maxConcurrentPlayCount+1)*2)

namespace Whisper {

class AudioEngine {
public:
	AudioEngine( void* audioSettings );
	
	// tries to add a sound, true on success
	bool addSound( Sound* sound );

	// retrieves a pointer to a sound that
	// is done playing.
	Sound* getFinishedSound();
	
	// closes the portAudio stuff down.
	~AudioEngine();
private:
	// the port audio "callback".
	int givePortAudioData(
		void *outputBuffer
        ,unsigned long framesPerBuffer
    // don't need these?
    //    ,const PaStreamCallbackTimeInfo* timeInfo
    //    ,PaStreamCallbackFlags statusFlags
    );
    // port audio stream pointer
    PaStream* stream;
    // friend the port audio callback
    friend int AudioEngineStreamCallback(const void *inputBuffer
		,void *outputBuffer
		,unsigned long framesPerBuffer
		,const PaStreamCallbackTimeInfo* timeInfo
		,PaStreamCallbackFlags statusFlags
		,void *userData
	);

	void writeOutPreExisting(float* out, unsigned long framesPerBuffer, size_t startIndex = 0);

	void copyNewSoundsToExisting();

	size_t playingSoundsLength = 0;
	std::array<std::tuple<Sound*,unsigned long>,(_maxConcurrentPlayCount+1)> playingSounds;

    ConcurrentRingBuffer<Sound*,(_maxQueueBeforeReadCount+1)> newSoundBuffer;
    ConcurrentRingBuffer<Sound*,(_maxFinishedSoundCount+1)> playedSoundBuffer;
};

}

#endif
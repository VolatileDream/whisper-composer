#ifndef _AUDIO_ENGINE_HPP_
#define _AUDIO_ENGINE_HPP_

#include <array>
#include <tuple>

#include "portaudio.h"

#include "dyn_array.hpp"
#include "ConcurrentRingBuffer.hpp"

#include "Sound.hpp"

#define _maxConcurrentPlayCount 31
#define _maxQueueBeforeReadCount 15

// Tune this:
// We might need to raise/lower it.
// Depends on how quickly we can check it from outside.
#define _maxFinishedSoundCount ((_maxConcurrentPlayCount+1)*2)

namespace Whisper {

class SoundData {
public:
	SoundData( Sound* s, bool n ) : sound(s), needsNotify(n) {}
	SoundData() : SoundData(NULL,true) {}
	Sound* sound;
	bool needsNotify;
	unsigned long offset = 0;
};

class AudioEngine {
public:
	AudioEngine( void* audioSettings );
	
	// tries to add a sound, true on success
	// flagFinish:
	//  if false, the sound will never show up
	//  in the getFinishedSound().
	bool addSound( Sound* sound, bool flagFinish = true );

	// retrieves a pointer to a sound that
	// is done playing. Keep calling until
	// this returns NULL to get all the
	// sounds that have finished playing.
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
	std::array<SoundData,(_maxConcurrentPlayCount+1)> playingSounds;

    ConcurrentRingBuffer<SoundData,(_maxQueueBeforeReadCount+1)> newSoundBuffer;
    ConcurrentRingBuffer<Sound*,(_maxFinishedSoundCount+1)> playedSoundBuffer;
};

}

#endif
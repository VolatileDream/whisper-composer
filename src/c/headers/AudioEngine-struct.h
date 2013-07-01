#ifndef _WHISPER_AUDIO_ENGINE_STRUCT_H_
#define _WHISPER_AUDIO_ENGINE_STRUCT_H_

#include "AudioEngine.h"

typedef struct {
	WC_Sound* sound;
	bool needsNotify;
	unsigned long offset;
} SoundData;

// number of songs playing at the same time
#define _maxConcurrentPlayCount 31
// number of songs added before playing
#define _maxQueuedSoundCount 15
// Tune this:
// We might need to raise/lower it.
// Depends on how quickly we can check it from outside.
#define _maxFinishedSoundCount ((_maxConcurrentPlayCount+1)*2)


struct WC_AudioEngine {
	PaStream* stream;
    unsigned int playingSoundsLength;
	
	SoundData* playingSounds ;
	WC_RingBuffer newSoundBuffer;
    WC_RingBuffer playedSoundBuffer;
};

#endif
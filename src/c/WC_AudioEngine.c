#include "WC_AudioEngine.h"

#include "AudioEngine.hpp"
#include "AudioEngineBuilder.hpp"

#include "Sound.hpp"

// attempts to initialize the engine
// returns true on success
// on failure, the engine pointer is
// freed and therefore invalid
void* WC_Create(
	unsigned int concurrentPlayCount
	, unsigned int queuedSoundCount
	, unsigned int finishedSoundCount
){

	Whisper::AudioEngineBuilder builder;
	builder.concurrentPlayCount = concurrentPlayCount;
	builder.queuedSoundCount = queuedSoundCount;
	builder.finishedSoundCount = finishedSoundCount;

	return builder.build();
}

// tries to add a sound, true on success
// flagFinish:
//  if false, the sound will never show up
//  in the getFinishedSound().
bool WC_AddSound( void* engine, Sound* sound, bool flagFinish ){
	return ((Whisper::AudioEngine*)engine)->addSound( sound, flagFinish );
}

// retrieves a pointer to a sound that
// is done playing. Keep calling until
// this returns NULL to get all the
// sounds that have finished playing.
Sound* WC_GetFinishedSound( void* engine ){
	return ((Whisper::AudioEngine*)engine)->getFinishedSound();
}

// closes the portAudio stuff down.
void WC_Dispose( void* engine ){
	delete ((Whisper::AudioEngine*)engine);
}

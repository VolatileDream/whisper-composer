#ifndef _WHISPER_AUDIOENGINE_H_
#define _WHISPER_AUDIOENGINE_H_

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "Sound.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// attempts to initialize the engine
// returns true on success
// on failure, the engine pointer is
// freed and therefore invalid
void* WC_Create(
	unsigned int concurrentPlayCount
	, unsigned int queuedSoundCount
	, unsigned int finishedSoundCount
);

// tries to add a sound, true on success
// flagFinish:
//  if false, the sound will never show up
//  in the getFinishedSound().
bool WC_AddSound( void* engine, Sound* sound, bool flagFinish );

// retrieves a pointer to a sound that
// is done playing. Keep calling until
// this returns NULL to get all the
// sounds that have finished playing.
Sound* WC_GetFinishedSound( void* engine );

// closes the portAudio stuff down.
void WC_Dispose( void* engine );

#ifdef __cplusplus
}
#endif


#endif
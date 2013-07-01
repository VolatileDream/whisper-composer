#ifndef _WHISPER_AUDIOENGINE_H_
#define _WHISPER_AUDIOENGINE_H_

#include <stdbool.h>

#include "Sound.h"

struct WC_AudioEngine;
typedef struct WC_AudioEngine WC_AudioEngine;


// Allocates/Frees an AudioEngine.
// this is meant for use by wrapper libraries.
WC_AudioEngine* WC_Allocate(void);
void WC_Free(WC_AudioEngine* engine);

// attempts to initialize the engine
// returns true on success
// on failure, the engine pointer is
// freed and therefore invalid
bool WC_Init( WC_AudioEngine* engine, void* settings );

// tries to add a sound, true on success
// flagFinish:
//  if false, the sound will never show up
//  in the getFinishedSound().
bool WC_AddSound( WC_AudioEngine* engine, WC_Sound* sound, bool flagFinish );

// retrieves a pointer to a sound that
// is done playing. Keep calling until
// this returns NULL to get all the
// sounds that have finished playing.
WC_Sound* WC_GetFinishedSound( WC_AudioEngine* engine );

// closes the portAudio stuff down.
void WC_Dispose( WC_AudioEngine* engine );

#endif
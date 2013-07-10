#ifndef _AUDIO_ENGINE_HPP_
#define _AUDIO_ENGINE_HPP_

#include <memory>

#include "AudioEngine.h"

namespace Whisper {

class AudioEngine {
public:
	AudioEngine( void* audioSettings ) : engine(WC_Allocate()) {
		if( engine == NULL ){
			throw std::bad_alloc();
		}
		WC_Init(engine, audioSettings);
	}
	
	// tries to add a sound, true on success
	// flagFinish: if false, the sound will not show up
	//		in calls to getFinishedSound()
	bool addSound( WC_Sound* sound, bool flagFinish = false ){
		return WC_AddSound( engine, sound, flagFinish );
	}

	// retrieves a pointer to a sound that
	// is done playing.
	WC_Sound* getFinishedSound(){
		return WC_GetFinishedSound(engine);
	}
	
	// closes the portAudio stuff down.
	~AudioEngine(){
		WC_Dispose(engine);
	}
private:
	WC_AudioEngine *const engine;
};

}

#endif
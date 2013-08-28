#ifndef _AUDIO_ENGINE_HPP_
#define _AUDIO_ENGINE_HPP_

#include <portaudio.h>

#include "Sound.hpp"

#include "SoundMetaData.hpp"
#include "RingBuffer.hpp"

namespace Whisper {

// forward declare for Builder
class AudioEngineBuilder;

class AudioEngine {

	friend class AudioEngineBuilder;

public:

	/// Tries to add a sound to the engines play queue.
	///
	/// @param flagFinish: if false, the sound will not show up
	///		in calls to getFinishedSound()
	/// @return true on success
	bool addSound( Sound* sound, bool flagFinish = false );

	/// Tries to retrieve a pointer to the next sound that's done playing.
	///
	/// @return nullptr if there is no such sound
	Sound* getFinishedSound();
	
	/// Shutsdown and disposes of the AudioEngine
	~AudioEngine();
	
	static int AudioEngine_PortAudioCallback(
		const void *inputBuffer
		,void *outputBuffer
		,unsigned long framesPerBuffer
		,const PaStreamCallbackTimeInfo* timeInfo
		,PaStreamCallbackFlags statusFlags
		,void *userData
	){
		AudioEngine* engine = (AudioEngine*) userData;
		return engine->fetchSoundData( outputBuffer, framesPerBuffer );
	}

private:
	/// Fetches the next set of sound data
	///
	/// Gets the next set of sound data for port audio,
	/// @param outputBuffer buffer constructed with
	/// the port audio initialization settings (interlace/float/int etc)
	/// @param framesPerBuffer the length of the buffer
	int fetchSoundData(
		void *outputBuffer
	    , unsigned long framesPerBuffer
	);

	/// Moves newly queued sounds to the internal playing sound list
	///
	/// Takes _as many sounds as possible_ from the queued sound buffer
	/// and adds them to the playing sound list
	void copyNewSoundsToExisting();

	/// writes data to the output buffer
	///
	/// writes out the next 'framesPerBuffer' out to the specified buffer
	void writeOutPreExisting(float* out, unsigned int framesPerBuffer);

	AudioEngine(){}
	
	PaStream* stream; // port audio stream
    
    SoundMetaData* playingSounds;
    unsigned int playingSoundsLength = 0;
    unsigned int playingSoundsSize = 0;
	
	RingBuffer<SoundMetaData>* newSoundBuffer;
    RingBuffer<Sound*>* playedSoundBuffer;
};

}

#endif
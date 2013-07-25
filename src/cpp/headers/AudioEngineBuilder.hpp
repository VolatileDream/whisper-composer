#ifndef __AUDIO_ENGINE_BUILDER__
#define __AUDIO_ENGINE_BUILDER__

#include <portaudio.h>

#include "AudioEngine.hpp"

#include "SoundMetaData.hpp"

// Defaults:

// number of songs playing at the same time
#define _maxConcurrentPlayCount 31

// number of songs added before playing
#define _maxQueuedSoundCount 15
// Tune this:
// We might need to raise/lower it.
// Depends on how quickly we can check it from outside.
#define _maxFinishedSoundCount ((_maxConcurrentPlayCount+1)*2)

namespace Whisper {

class AudioEngineBuilder {
public:
	/// Number of sounds that can be playing at the same time
	unsigned int concurrentPlayCount = _maxConcurrentPlayCount;
	/// Number of sounds that can be concurrently queued
	unsigned int queuedSoundCount = _maxQueuedSoundCount;
	/// Largest number of sounds that we will hold notifications for
	unsigned int finishedSoundCount = _maxFinishedSoundCount; 

	/// Doesn't setup port audio, used for testing
	bool setupPortAudio = true;

	/// Creates an AudioEngine with the given settings
	AudioEngine* build(){

		AudioEngine* engine = new AudioEngine();

		// initialize the engine data first
		engine->playingSoundsSize = concurrentPlayCount;
		engine->playingSoundsLength = 0;
		engine->stream = nullptr;

		// try to do all of our memory allocations
		engine->playingSounds = new SoundMetaData[concurrentPlayCount];
		engine->newSoundBuffer = new RingBuffer<SoundMetaData>(queuedSoundCount);
		engine->playedSoundBuffer = new RingBuffer<Sound*>(finishedSoundCount);

		if( ! setupPortAudio ){
			// don't setup port audio.
			return engine;
		}

		// Initialize port audio

		PaError err = Pa_Initialize();
		if( err != paNoError ){
			print_pa_error(err, "Error initializing port audio");
			const PaHostErrorInfo* hostError;
			
			hostError = Pa_GetLastHostErrorInfo();
			printf("%s %s\n", "Host error: ", hostError->errorText );

			goto Error;
		}
		
		err = Pa_OpenDefaultStream(
			&(engine->stream),
			
			0, // no input
			1, // mono out
			
			paFloat32, // float

			44100, //sample rate
			1024, // frames per buffer

			AudioEngine::AudioEngine_PortAudioCallback,
			engine
		);
		print_pa_error(err, "Error opening stream: ");

		err = Pa_StartStream(engine->stream);
		print_pa_error(err, "Error starting stream: ");

		return engine;

		Error:
		
		printf("Error with allocation of Audio Engine\n");
		delete engine;

		return nullptr;
	};
	
	void print_pa_error(PaError err, const char* message ){
		if( err != paNoError ){
			printf("%s%s\n", message, Pa_GetErrorText(err) );
		}
	}
};

}

#endif
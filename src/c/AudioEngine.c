#include "stdlib.h"
#include "stdio.h"
#include "portaudio.h"

#include "Sound.h"
#include "RingBuffer.h"
// include this so we can inline
// the struct in WC_AudioEngine
#include "RingBuffer-struct.h"
// the function declarations
#include "AudioEngine.h"
#include "AudioEngine-struct.h"

// Allocates/Frees an AudioEngine.
// this is meant for use by wrapper libraries.
WC_AudioEngine* WC_Allocate(void){
	return malloc(sizeof(WC_Allocate));
}
void WC_Free(WC_AudioEngine* engine){
	free(engine);
}


// actual port audio callback
static int AudioEngineStreamCallback(
	const void *inputBuffer
	,void *outputBuffer
	,unsigned long framesPerBuffer
	,const PaStreamCallbackTimeInfo* timeInfo
	,PaStreamCallbackFlags statusFlags
	,void *userData
);

static void print_pa_error(PaError err, const char* message);

// Class stuff

// attempts to initialize the engine
// returns true on success
// on failure, the engine pointer is
// freed and therefore invalid
bool WC_Init( WC_AudioEngine* engine, void* settings ){

	// initialize the engine data first
	engine->playingSoundsLength = 0;
	engine->stream = NULL;

	// try to do all of our memory allocations
	engine->playingSounds = malloc( sizeof(SoundData) * _maxConcurrentPlayCount);
	
	if( engine->playingSounds == NULL
		|| !WC_RB_Create( &(engine->newSoundBuffer), sizeof(SoundData), _maxQueuedSoundCount )
		|| !WC_RB_Create( &(engine->playedSoundBuffer), sizeof(SoundData), _maxFinishedSoundCount )
	){
		goto Error;
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

		AudioEngineStreamCallback,
		engine
	);
	print_pa_error(err, "Error opening stream: ");

	err = Pa_StartStream(engine->stream);
	print_pa_error(err, "Error starting stream: ");

	// oh no...don't do this. >.<
	return true;

	Error:
	printf("Error with allocation of Audio Engine\n");
	WC_Dispose(engine);
	return false;
}

// tries to add a sound, true on success
// flagFinish:
//  if false, the sound will never show up
//  in the getFinishedSound().
bool WC_AddSound( WC_AudioEngine* engine, WC_Sound* sound, bool flagFinish ){

	if( sound == NULL || sound->length == 0 ){
		// we're too nice to say anything, we pretend it worked.
		return true;
	}
	
	SoundData data;
	data.sound = sound;
	data.needsNotify = flagFinish;
	data.offset = 0;

	return WC_RB_Add( &engine->newSoundBuffer,  &data );
}

// retrieves a pointer to a sound that
// is done playing. Keep calling until
// this returns NULL to get all the
// sounds that have finished playing.
WC_Sound* WC_GetFinishedSound( WC_AudioEngine* engine ){
	WC_Sound* sound = NULL;
	if( WC_RB_Remove( &engine->playedSoundBuffer, &sound ) ){
		return sound;
	}
	return NULL;
}

void WC_Dispose( WC_AudioEngine* engine ){

	if( engine->playingSounds != NULL ){
		free( engine->playingSounds );
	}

	WC_RB_Dispose( &engine->newSoundBuffer );
	WC_RB_Dispose( &engine->playedSoundBuffer );
	
	if( engine->stream == NULL ){
		free( engine );
		return;
	}

	// TODO clean up port audio stuff
	// we opened a stream, close it.
	PaError err = Pa_StopStream(engine->stream);

	print_pa_error(err, "Error stopping stream: ");

	// we started port audio, stop it.
	// note that if we don't we could
	// cause all sorts of errors with
	// the audio 
	err = Pa_Terminate();

	print_pa_error(err, "Error terminating port audio: ");

	free(engine);
}

// private functions:

// port audio interaction functions:
static void print_pa_error(PaError err, const char* message ){
	if( err != paNoError ){
		printf("%s%s\n", message, Pa_GetErrorText(err) );
	}
}

static void copyNewSoundsToExisting( WC_AudioEngine* engine ){
	SoundData sound;
	bool hasSound = false;
	while(
		(hasSound = WC_RB_Remove(&engine->newSoundBuffer, &sound))
		&& engine->playingSoundsLength < _maxConcurrentPlayCount
	){
		engine->playingSounds[ engine->playingSoundsLength ] = sound;
		engine->playingSoundsLength++;
	}

	// hasSound = true; iff we couldn't add all the sounds
	if( hasSound && engine->playingSoundsLength == _maxConcurrentPlayCount ){
		//TODO Adding too many sounds
	}
}

static void writeOutPreExisting(WC_AudioEngine* engine, float* out, unsigned int framesPerBuffer, size_t startIndex){
	
	float soundAmplitude = (engine->playingSoundsLength - startIndex)*1.0/engine->playingSoundsLength;

	for(size_t i= startIndex; i < engine->playingSoundsLength; i++){

		SoundData data  = engine->playingSounds[i];

		unsigned int soundRemaining = data.sound->length - data.offset;

		unsigned int outputCount =
			framesPerBuffer < soundRemaining ?
				framesPerBuffer : soundRemaining;

		// Write the song out
		float* rawSound = data.sound->audioData + data.offset;

		for( unsigned int index = 0; index < soundRemaining; index++){
			out[index] = out[index] + soundAmplitude * rawSound[index];
		}

		// song is done playing
		if( outputCount == soundRemaining ){
			engine->playingSoundsLength--;
			engine->playingSounds[i] = engine->playingSounds[engine->playingSoundsLength];
			i--;

			if( data.needsNotify && ! WC_RB_Add( &engine->playedSoundBuffer, data.sound) ){
				// TODO what do we do?
			}
		}

	}
}

static int givePortAudioData(
	 	WC_AudioEngine* engine,
		void *outputBuffer
        ,unsigned long framesPerBuffer
//        ,const PaStreamCallbackTimeInfo* timeInfo
//        ,PaStreamCallbackFlags statusFlags
    ){

	float* out = (float*)outputBuffer;

	// do all the exsting sounds first, then new sounds.

	writeOutPreExisting(engine, out, framesPerBuffer, 0);

	// new sounds

	size_t oldSoundLength = engine->playingSoundsLength;

	copyNewSoundsToExisting(engine);

	if( oldSoundLength == engine->playingSoundsLength ){
		// no new sounds
		return 0;
	}

	// adjust the stream amplitude because we've added new sounds

	float amplitudeModifier =
		(engine->playingSoundsLength - oldSoundLength)
			*1.0/engine->playingSoundsLength;

	for(unsigned long i=0; i < framesPerBuffer; i++){
		out[i] = out[i] * amplitudeModifier;
	}

	writeOutPreExisting(engine, out, framesPerBuffer, oldSoundLength);

	return 0;
}

static int AudioEngineStreamCallback(
	const void *inputBuffer
	,void *outputBuffer
	,unsigned long framesPerBuffer
	,const PaStreamCallbackTimeInfo* timeInfo
	,PaStreamCallbackFlags statusFlags
	,void *userData
){
    /* Cast data passed through stream to our structure. */
    WC_AudioEngine* engine = (WC_AudioEngine*)userData;   
    return givePortAudioData(engine, outputBuffer, framesPerBuffer);
}
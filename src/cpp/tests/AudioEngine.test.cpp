#include "catch.hpp"

#include <iostream>

#include "AudioEngine.hpp"
#include "AudioEngineBuilder.hpp"

#include "test_time.hpp"

Whisper::Sound* createSinSound( unsigned int size = 44100 ){

  const float PI = 3.14159265358979323846;

  Whisper::Sound* sound = new Whisper::Sound();
  sound->length = size;
  sound->audioData = new float[size];

  for(unsigned int i=0; i < size; i++){
    float pos =  i/200.0 * 2.0*PI;
    sound->audioData[i] = sin(pos);
  }

  return sound;
}


Whisper::AudioEngine* getTestEngine(){
	Whisper::AudioEngineBuilder factory;
	factory.setupPortAudio = false;

	return  factory.build();
}

TEST_CASE( "AudioEngine same data", "[AudioEngine]" ){
	
	const unsigned long framesPerBuffer = 1024;
	float outputBuffer[framesPerBuffer] = {0};

	Whisper::Sound* sound = createSinSound();

	Whisper::AudioEngine* engine = getTestEngine();

	engine->addSound(sound);

	Whisper::AudioEngine::AudioEngine_PortAudioCallback(
		nullptr				//const void *inputBuffer
		, outputBuffer		//,void *outputBuffer
		, framesPerBuffer	//,unsigned long framesPerBuffer
		, nullptr			//,const PaStreamCallbackTimeInfo* timeInfo
		, 0					//,PaStreamCallbackFlags statusFlags
		, engine			//,void *userData
	);

	// first pass doesn't pickup new things.
	for( unsigned long i = 0; i < framesPerBuffer ; i++ ){
		CHECK( outputBuffer[i] == 0 );
	}

	Whisper::AudioEngine::AudioEngine_PortAudioCallback(
		nullptr				//const void *inputBuffer
		, outputBuffer		//,void *outputBuffer
		, framesPerBuffer	//,unsigned long framesPerBuffer
		, nullptr			//,const PaStreamCallbackTimeInfo* timeInfo
		, 0					//,PaStreamCallbackFlags statusFlags
		, engine			//,void *userData
	);

	// second call does
	for( unsigned long i = 0; i < framesPerBuffer ; i++ ){
		CHECK( outputBuffer[i] == sound->audioData[i] );
	}	

	delete engine;

	delete [] sound->audioData;
	delete sound;
}

TEST_CASE( "AudioEngine callback speed under 2ms", "[performance]" ){
	
	const unsigned long framesPerBuffer = 1024;
	float outputBuffer[framesPerBuffer] = {0};

	Whisper::Sound* sound = createSinSound();

	Whisper::AudioEngine* engine = getTestEngine();

	engine->addSound(sound);

	auto func = [outputBuffer, engine, framesPerBuffer] () {
		Whisper::AudioEngine::AudioEngine_PortAudioCallback(
			(const void*) nullptr	//const void *inputBuffer
			, (void*) outputBuffer	//void *outputBuffer
			, framesPerBuffer	//unsigned long framesPerBuffer
			, (PaStreamCallbackTimeInfo*) nullptr //const PaStreamCallbackTimeInfo* timeInfo
			, (PaStreamCallbackFlags) 0 //PaStreamCallbackFlags statusFlags
			, (void*) engine	//void *userData
		);
	};

	double milliseconds = 0;

	for(int i =0; i < 5 ; i++ ){
		milliseconds = test_time( func );
		CHECK( milliseconds < 2 ); // must be faster than 2 milliseconds
	}

	delete engine;

	delete [] sound->audioData;
	delete sound;
}

TEST_CASE( "AudioEngine sound finish flagged", "[AudioEngine]" ){
	
	const unsigned long framesPerBuffer = 1024;
	float outputBuffer[framesPerBuffer] = {0};

	Whisper::Sound* sound = createSinSound(128);

	Whisper::AudioEngine* engine = getTestEngine();


	engine->addSound(sound, true);

	// twice because it won't get played the first time.
	for(int i =0; i < 2 ; i++ ){
		Whisper::AudioEngine::AudioEngine_PortAudioCallback(
			(const void*) nullptr	//const void *inputBuffer
			, (void*) outputBuffer	//void *outputBuffer
			, framesPerBuffer	//unsigned long framesPerBuffer
			, (PaStreamCallbackTimeInfo*) nullptr //const PaStreamCallbackTimeInfo* timeInfo
			, (PaStreamCallbackFlags) 0 //PaStreamCallbackFlags statusFlags
			, (void*) engine	//void *userData
		);
	}
	Whisper::Sound* outSound = engine->getFinishedSound();

	CHECK( outSound == sound );

		
	engine->addSound(sound, false);

	// twice because it won't get played the first time.
	for(int i =0; i < 2 ; i++ ){
		Whisper::AudioEngine::AudioEngine_PortAudioCallback(
			(const void*) nullptr	//const void *inputBuffer
			, (void*) outputBuffer	//void *outputBuffer
			, framesPerBuffer	//unsigned long framesPerBuffer
			, (PaStreamCallbackTimeInfo*) nullptr //const PaStreamCallbackTimeInfo* timeInfo
			, (PaStreamCallbackFlags) 0 //PaStreamCallbackFlags statusFlags
			, (void*) engine	//void *userData
		);
	}
	outSound = engine->getFinishedSound();

	CHECK( ((Whisper::Sound*)nullptr) == outSound );



	delete engine;

	delete [] sound->audioData;
	delete sound;
}
